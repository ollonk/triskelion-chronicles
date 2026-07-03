#include "global.h"
#include "bg.h"
#include "day_night.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "fieldmap.h"
#include "frontier_pass.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "item.h"
#include "item_menu.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "metatile_behavior.h"
#include "money.h"
#include "overworld.h"
#include "palette.h"
#include "pokedex.h"
#include "pokelink.h"
#include "random.h"
#include "rtc.h"
#include "scanline_effect.h"
#include "script.h"
#include "script_movement.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "strings.h"
#include "shop.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "trainer_card.h"
#include "window.h"
#include "constants/event_object_movement.h"
#include "constants/event_objects.h"
#include "constants/rgb.h"
#include "constants/items.h"
#include "constants/songs.h"
#include "constants/species.h"

enum
{
    WIN_HEADER,
    WIN_APPS,
    WIN_DETAIL,
    WIN_HELP,
};

enum
{
    POKELINK_STATE_FADE_IN,
    POKELINK_STATE_MAIN,
    POKELINK_STATE_MESSAGE,
    POKELINK_STATE_GLOOM_CONFIRM,
    POKELINK_STATE_GLOOM_FEED,
    POKELINK_STATE_SIGHTINGS_LIST,
    POKELINK_STATE_SIGHTINGS_DETAIL,
    POKELINK_STATE_COLLECTION_LIST,
    POKELINK_STATE_COLLECTION_DETAIL,
    POKELINK_STATE_LAUNCH,
    POKELINK_STATE_EXIT,
};

#define POKELINK_TEST_UNLOCK_ALL TRUE
#define POKELINK_GRID_COLS 4
#define POKELINK_GRID_ROWS 3
#define POKELINK_PAGE_SIZE (POKELINK_GRID_COLS * POKELINK_GRID_ROWS)
#define POKELINK_TILE_WIDTH 48
#define POKELINK_TILE_HEIGHT 30
#define POKELINK_ICON_SPRITE_SIZE 32
#define POKELINK_ICON_SPRITE_TILE_COUNT 16
#define POKELINK_ICON_NONE 0xFF
#define TAG_POKELINK_ICONS 0x5010
#define TAG_POKELINK_CURSOR 0x5011
#define TAG_POKELINK_ASSETS 0x5012
#define TAG_POKELINK_SIGHTINGS_ICON 0x5013
#define TAG_POKELINK_SIGHTINGS_ICON_PAL 0x5014
#define TAG_POKELINK_COLLECTION_ICON 0x5015
#define TAG_POKELINK_COLLECTION_ICON_PAL 0x5016
#define POKELINK_DELIVERY_LOCAL_ID 0x7F
#define POKELINK_DELIVERY_PATH_RADIUS 6
#define POKELINK_DELIVERY_PATH_DIAMETER (POKELINK_DELIVERY_PATH_RADIUS * 2 + 1)
#define POKELINK_DELIVERY_MAX_PATH_STEPS 16
#define POKELINK_SIGHTINGS_VISIBLE_ROWS 7
#define POKELINK_SIGHTINGS_FLAG_NONE 0xFFFF
#define POKELINK_COLLECTION_VISIBLE_ROWS 6
#define POKELINK_COLLECTION_FLAG_NONE 0xFFFF

struct PokeLinkApp
{
    const u8 *name;
    const u8 *shortName;
    const u8 *description;
    const u8 *message;
    u16 unlockFlag;
    bool8 availableByDefault;
    bool8 canFavorite;
};

struct PokeLinkSightingsEntry
{
    u16 species;
    const u8 *region;
    const u8 *classification;
    const u8 *rumorText;
    const u8 *whereaboutsText;
    const u8 *conditionsText;
    const u8 *fieldNotesText;
    u16 rumorFlag;
    u16 locatedFlag;
    u16 encounteredFlag;
};

struct PokeLinkCollectionArea
{
    const u8 *name;
    u16 visitedFlag;
};

struct PokeLinkCollectionCategory
{
    const u8 *name;
};

struct PokeLinkCollectionEntry
{
    u16 itemId;
    u8 category;
    u8 area;
    u8 quantity;
    u16 visibilityFlag;
    u16 seenFlag;
    u16 obtainedFlag;
    u16 lockedFlag;
    const u8 *note;
};

struct PokeLinkState
{
    u8 cursor;
    u8 top;
    u8 visibleApps[POKELINK_APP_COUNT];
    u8 visibleCount;
    u8 launchAppId;
    u8 iconSpriteIds[POKELINK_PAGE_SIZE];
    u8 cursorSpriteId;
};

static void CB2_PokeLink(void);
static void VBlankCB_PokeLink(void);
static void Task_PokeLink(u8 taskId);
static void BuildVisibleAppList(void);
static void DrawPokeLink(void);
static void DrawPokeLinkHeader(void);
static void DrawPokeLinkApps(void);
static void DrawPokeLinkDetail(void);
static void DrawPokeLinkHelp(void);
static void DrawPokeLinkMessage(u8 appId);
static void DrawPokeLinkMessageText(const u8 *text);
static void DrawGloomscrollConfirm(u8 cursor);
static void DrawGloomscrollFeed(u8 step);
static void DrawGloomscrollResultMessage(void);
static const u8 *GetGloomscrollConfirmText(void);
static void StartGloomscrollFeed(u8 taskId);
static void DrawSightingsList(u8 cursor, u8 top);
static void DrawSightingsDetail(u8 cursor);
static void MoveSightingsCursor(s16 *cursor, s16 *top, s8 delta);
static u8 GetSightingsStatus(const struct PokeLinkSightingsEntry *entry);
static const u8 *GetSightingsStatusText(u8 status);
static const u8 *GetSightingsDisplayName(const struct PokeLinkSightingsEntry *entry, u8 status);
static void DrawCollectionList(u8 mode, u8 cursor, u8 top);
static void DrawCollectionDetail(u8 mode, u8 cursor);
static void MoveCollectionCursor(s16 *cursor, s16 *top, s8 delta, u8 mode);
static u8 ToggleCollectionMode(s16 *cursor, s16 *top, u8 mode);
static void GetCollectionAreaProgress(u8 area, u8 *found, u8 *total);
static void GetCollectionCategoryProgress(u8 category, u8 *found, u8 *total);
static u8 GetCollectionEntryStatus(const struct PokeLinkCollectionEntry *entry);
static const u8 *GetCollectionStatusText(u8 status);
static const u8 *GetCollectionEntryDisplayName(const struct PokeLinkCollectionEntry *entry, u8 status);
static u8 *BufferCollectionProgress(u8 found, u8 total);
static void FillPokeLinkAppsRectClipped(s16 x, s16 y, s16 width, s16 height, u8 color);
static void DrawGloomscrollPost(s16 y, u8 accent, const u8 *handle, const u8 *body);
static void DrawGloomscrollShort(s16 x, u8 accent, const u8 *caption);
static void MovePokeLinkCursor(s8 xDelta, s8 yDelta);
static void LoadPokeLinkBackground(void);
static void LoadPokeLinkGraphics(void);
static void CreatePokeLinkSprites(void);
static void UpdatePokeLinkSprites(void);
static void SetPokeLinkSpritesVisible(bool8 visible);
static void DestroyPokeLinkSprites(void);
static void GetPokeLinkAppIconCoords(u8 pagePos, s16 *x, s16 *y);
static void ToggleFavorite(u8 appId);
static bool8 IsAppAvailable(u8 appId);
static bool8 IsFavoriteSlotValid(u16 value);
static u16 *GetFavoriteVarPtr(u8 slot);
static u8 GetDefaultShortcutAppId(u8 slot);
static void CleanupPokeLink(void);
static void OpenSelectedApp(u8 taskId, u8 appId);
static u8 CountBadges(void);
static void FieldCallback_OpenDelibirdDelivery(void);
static void Task_OpenDelibirdDeliveryAfterReturn(u8 taskId);
static const u16 *GetDelibirdDeliveryInventory(void);
static void DelibirdDeliveryReturnCallback(void);
static void Task_DelibirdDeliveryArrives(u8 taskId);
static void Task_DelibirdDeliveryStartCourier(u8 taskId);
static void Task_DelibirdDeliveryMoveCourier(u8 taskId);
static void Task_DelibirdDeliveryMoveCourierAway(u8 taskId);
static void Task_DelibirdDeliveryAskTip(u8 taskId);
static void Task_DelibirdDeliveryPromptTip(u8 taskId);
static void Task_DelibirdDeliveryShowTipMenu(u8 taskId);
static void Task_DelibirdDeliveryProcessTipMenu(u8 taskId);
static void Task_DelibirdDeliveryFinish(u8 taskId);
static void AddDelibirdDeliveryItems(void);
static bool8 SpawnDelibirdDeliveryCourier(void);
static void RemoveDelibirdDeliveryCourier(void);
static bool8 BuildDelibirdDeliveryPath(s16 *spawnX, s16 *spawnY, u8 elevation);
static bool8 IsDelibirdDeliveryTileUsable(s16 x, s16 y, u8 elevation);
static bool8 IsDelibirdDeliveryTileInSearchArea(s16 x, s16 y, s16 centerX, s16 centerY);
static u8 GetDelibirdDeliveryMoveAction(u8 direction);
static void FinishDelibirdDeliveryTask(u8 taskId);
static bool8 PlayerIsOnSurfableWater(void);
void CB2_InitPokeLinkTetris(void);
void CB2_InitPokeLinkRadio(void);

static const u8 sText_PokeLinkTitle[] = _("POKéLINK");
static const u8 sText_PokeLinkSubtitle[] = _("ONLINE");
static const u8 sText_PokeLinkHelp[] = _("{DPAD_NONE} Apps {A_BUTTON} Open {B_BUTTON} Back SELECT Fav.");
static const u8 sText_PokeLinkMessageHelp[] = _("{A_BUTTON}/{B_BUTTON} Back");
static const u8 sText_PokeLinkConfirmHelp[] = _("{DPAD_UPDOWN} Pick {A_BUTTON} OK {B_BUTTON} Back");
static const u8 sText_PokeLinkFavoriteSet[] = _("Added to quick shortcuts.");
static const u8 sText_PokeLinkFavoriteCleared[] = _("Removed from quick shortcuts.");
static const u8 sText_PokeLinkCannotFavorite[] = _("This app can't be a shortcut.");
static const u8 sText_PokeLinkNone[] = _("None");
static const u8 sText_PokeLinkName[] = _("Name");
static const u8 sText_PokeLinkMoney[] = _("Money");
static const u8 sText_PokeLinkBadges[] = _("Badges");
static const u8 sText_PokeLinkShortcutSet[] = _("Shortcut set");
static const u8 sText_PokeLinkReady[] = _("Ready");
static const u8 sText_AppMap[] = _("Map");
static const u8 sText_AppSightings[] = _("Sightings");
static const u8 sText_AppCollectionLog[] = _("Collection Log");
static const u8 sText_AppProfile[] = _("Tetris");
static const u8 sText_AppGloomscroll[] = _("Gloomscroll");
static const u8 sText_AppDexNav[] = _("DexNav");
static const u8 sText_AppRadio[] = _("Radio");
static const u8 sText_AppVsSeeker[] = _("VS Seeker");
static const u8 sText_AppFlashlight[] = _("Flashlight");
static const u8 sText_AppDelivery[] = _("Delibird Delivery");
static const u8 sText_AppAbraCab[] = _("AbraCab");
static const u8 sText_AppNotes[] = _("Notes");
static const u8 sShort_Map[] = _("MAP");
static const u8 sShort_Sightings[] = _("SEEN");
static const u8 sShort_CollectionLog[] = _("LOG");
static const u8 sShort_Profile[] = _("TETR");
static const u8 sShort_Gloomscroll[] = _("GLOM");
static const u8 sShort_DexNav[] = _("DEX");
static const u8 sShort_Radio[] = _("RDO");
static const u8 sShort_VsSeeker[] = _("VS");
static const u8 sShort_Flashlight[] = _("LITE");
static const u8 sShort_Delivery[] = _("DLVY");
static const u8 sShort_AbraCab[] = _("CAB");
static const u8 sShort_Notes[] = _("NOTE");
static const u8 sPokeLinkTextColors[] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY};
static const u8 sPokeLinkDarkTextColors[] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY};

static const u8 sDesc_Map[] = _("Browse the region map.");
static const u8 sDesc_Sightings[] = _("Tracks strange reports and unresolved sightings.");
static const u8 sDesc_CollectionLog[] = _("Tracks items, gifts, hidden pickups, and treasures.");
static const u8 sDesc_Profile[] = _("Stack blocks and chase a high score.");
static const u8 sDesc_Gloomscroll[] = _("Lose track of time online.");
static const u8 sDesc_DexNav[] = _("Field encounter search upgrade.");
static const u8 sDesc_Radio[] = _("Pokégear-style radio channels.");
static const u8 sDesc_VsSeeker[] = _("Trainer rematch tools.");
static const u8 sDesc_Flashlight[] = _("Field lighting utility.");
static const u8 sDesc_Delivery[] = _("Order mart basics to the field.");
static const u8 sDesc_AbraCab[] = _("Paid teleport travel service.");
static const u8 sDesc_Notes[] = _("Story notes and quest reminders.");

static const u8 sMsg_DexNav[] = _("DexNav app installed.\pDEXNAV_ENABLED is currently FALSE, so the app is a stub for now.");
static const u8 sMsg_Radio[] = _("Opening Radio...");
static const u8 sMsg_VsSeeker[] = _("VS Seeker app installed.\pTODO: wire rematch tracking here and retire Match Call.");
static const u8 sMsg_Flashlight[] = _("Flashlight app installed.\pTODO: call field lighting logic without requiring HM05.");
static const u8 sMsg_Delivery[] = _("Opening Delibird Delivery...");
static const u8 sMsg_AbraCab[] = _("Opening AbraCab...");
static const u8 sMsg_Notes[] = _("Notes app prototype.\pTODO: populate this with event-flag-based story objectives.");
static const u8 sMsg_Gloomscroll[] = _("GLOOMSCROLL closed.\pOkay! It's time to stop scrolling\nand go outside!");
static const u8 sText_GloomscrollConfirmNight[] = _("Scroll until night?");
static const u8 sText_GloomscrollConfirmMorning[] = _("Scroll until morning?");
static const u8 sText_GloomscrollWarning[] = _("Time may slip away.");
static const u8 sText_GloomscrollYes[] = _("Yes");
static const u8 sText_GloomscrollNo[] = _("No");
static const u8 sText_GloomscrollHandle0[] = _("LassMira");
static const u8 sText_GloomscrollHandle1[] = _("BugCatcherKen");
static const u8 sText_GloomscrollHandle2[] = _("Route34Live");
static const u8 sText_GloomscrollPost0[] = _("saw tree move");
static const u8 sText_GloomscrollPost1[] = _("ledge ranking");
static const u8 sText_GloomscrollPost2[] = _("night team?");
static const u8 sText_GloomscrollShort0[] = _("Rival");
static const u8 sText_GloomscrollShort1[] = _("ZUBAT");
static const u8 sText_GloomscrollShort2[] = _("Grass");
static const u8 sText_GloomscrollLoading[] = _("Still scrolling...");
static const u8 sText_GloomscrollLikes[] = _("LIKES");
static const u8 sText_GloomscrollClosed[] = _("GLOOMSCROLL closed.");
static const u8 sText_GloomscrollResult0[] = _("Okay! It's time to stop");
static const u8 sText_GloomscrollResult1[] = _("scrolling and go outside!");
static const u8 sText_DeliveryWalker[] = _("Sky is on his way.");
static const u8 sText_DeliverySurfer[] = _("Sky is surfing out.");
static const u8 sText_DeliveryAskPlayer[] = _("Sky: {PLAYER}?\nDelibird Delivery order?");
static const u8 sText_DeliveryAskTip[] = _("Sky: Great.\nPick a tip.");
static const u8 sText_DeliveryThanks[] = _("Sky: Thanks!\nHere's your order.");
static const u8 sText_DeliveryNoTipThanks[] = _("Sky: Hmph.\nHere's your order.");
static const u8 sText_DeliverySquelched[] = _("Sky: Cheapskate!\nYour order got squelched.");
static const u8 sText_DeliveryNoOrder[] = _("No delivery order was placed.");
static const u8 sText_TipNone[] = _("No tip");
static const u8 sText_TipSmall[] = _("100");
static const u8 sText_TipBig[] = _("500");
static const u8 sText_SightingsHelpList[] = _("{DPAD_UPDOWN} Pick {A_BUTTON} Case {B_BUTTON} Back");
static const u8 sText_SightingsHelpDetail[] = _("{B_BUTTON} List");
static const u8 sText_SightingsUnknownName[] = _("???");
static const u8 sText_SightingsUnknown[] = _("Unknown");
static const u8 sText_SightingsRumored[] = _("Rumored");
static const u8 sText_SightingsSighted[] = _("Sighted");
static const u8 sText_SightingsLocated[] = _("Located");
static const u8 sText_SightingsEncountered[] = _("Encountered");
static const u8 sText_SightingsCaught[] = _("Caught");
static const u8 sText_SightingsLegendary[] = _("Legendary");
static const u8 sText_SightingsMythical[] = _("Mythical");
static const u8 sText_RegionKanto[] = _("Kanto");
static const u8 sText_RegionJohto[] = _("Johto");
static const u8 sText_RegionHoenn[] = _("Hoenn");
static const u8 sText_RegionSinnoh[] = _("Sinnoh");
static const u8 sText_SightingsListHeader[] = _("CASE       ORIGIN  STATUS");
static const u8 sText_SightingsStatusLabel[] = _("Status:");
static const u8 sText_SightingsRegionLabel[] = _("Region:");
static const u8 sText_SightingsClassLabel[] = _("Class:");
static const u8 sText_SightingsRumorLabel[] = _("Rumor:");
static const u8 sText_SightingsWhereLabel[] = _("Where:");
static const u8 sText_SightingsConditionsLabel[] = _("Cond:");
static const u8 sText_SightingsNotesLabel[] = _("Notes:");
static const u8 sText_SightingsLockedRumor[] = _("Reports are too vague to verify.");
static const u8 sText_SightingsLockedWhere[] = _("No reliable whereabouts yet.");
static const u8 sText_SightingsUnknownConditions[] = _("Unknown.");
static const u8 sText_SightingsNoNotes[] = _("No field notes recorded.");
static const u8 sText_SightingsSeenNotes[] = _("A confirmed sighting is logged.");
static const u8 sText_SightingsCaughtNotes[] = _("Case closed in the field log.");
static const u8 sText_SightingsRumorGeneric[] = _("Old reports describe a rare presence.");
static const u8 sText_SightingsWhereGeneric[] = _("Follow rumors before exact places.");
static const u8 sText_SightingsConditionsGeneric[] = _("Check unusual local conditions.");
static const u8 sText_CollectionHelpList[] = _("{DPAD_UPDOWN} Pick {A_BUTTON} Open {B_BUTTON} Back L/R Tab");
static const u8 sText_CollectionHelpDetail[] = _("{B_BUTTON} List L/R Tab");
static const u8 sText_CollectionAreaTab[] = _("AREA");
static const u8 sText_CollectionCategoryTab[] = _("CATEGORY");
static const u8 sText_CollectionListHeaderArea[] = _("AREA          FOUND  STATUS");
static const u8 sText_CollectionListHeaderCategory[] = _("CATEGORY      FOUND  STATUS");
static const u8 sText_CollectionFoundLabel[] = _("Found:");
static const u8 sText_CollectionStatusLabel[] = _("Status:");
static const u8 sText_CollectionUnknown[] = _("Unknown");
static const u8 sText_CollectionSurveyed[] = _("Surveyed");
static const u8 sText_CollectionSeen[] = _("Seen");
static const u8 sText_CollectionLocked[] = _("Locked");
static const u8 sText_CollectionClaimed[] = _("Claimed");
static const u8 sText_CollectionCleared[] = _("Cleared");
static const u8 sText_CollectionUnknownItem[] = _("???");
static const u8 sText_CollectionNoNote[] = _("Future log hooks go here.");
static const u8 sText_AreaRoute29[] = _("Route 29");
static const u8 sText_AreaVioletCity[] = _("Violet City");
static const u8 sText_AreaAzaleaTown[] = _("Azalea Town");
static const u8 sText_AreaGoldenrodCity[] = _("Goldenrod");
static const u8 sText_AreaNationalPark[] = _("Natl. Park");
static const u8 sText_AreaRuinsOfAlph[] = _("Ruins Alph");
static const u8 sText_CategoryTmsHms[] = _("TMs/HMs");
static const u8 sText_CategoryBattleItems[] = _("Battle Items");
static const u8 sText_CategoryMegaStones[] = _("Mega Stones");
static const u8 sText_CategoryHeldItems[] = _("Held Items");
static const u8 sText_CategoryEvolutionItems[] = _("Evo Items");
static const u8 sText_CategoryMedicine[] = _("Medicine");
static const u8 sText_CategoryPokeBalls[] = _("Poke Balls");
static const u8 sText_CategoryBerries[] = _("Berries");
static const u8 sText_CategoryValuables[] = _("Valuables");
static const u8 sText_CategoryKeyItems[] = _("Key Items");
static const u8 sText_CategoryGiftsTrades[] = _("Gifts/Trades");
static const u8 sText_CategoryHiddenItems[] = _("Hidden Items");
static const u8 sText_CategoryOther[] = _("Other");
static const u8 sText_CollectionNoteVisible[] = _("Visible pickup.");
static const u8 sText_CollectionNoteHidden[] = _("Hidden pickup.");
static const u8 sText_CollectionNoteGift[] = _("Gift or reward.");
static const u8 sText_CollectionNoteProgress[] = _("Requires later progress.");

enum
{
    SIGHTINGS_STATUS_UNKNOWN,
    SIGHTINGS_STATUS_RUMORED,
    SIGHTINGS_STATUS_SIGHTED,
    SIGHTINGS_STATUS_LOCATED,
    SIGHTINGS_STATUS_ENCOUNTERED,
    SIGHTINGS_STATUS_CAUGHT
};

enum
{
    COLLECTION_MODE_AREA,
    COLLECTION_MODE_CATEGORY
};

enum
{
    COLLECTION_STATUS_UNKNOWN,
    COLLECTION_STATUS_SURVEYED,
    COLLECTION_STATUS_SEEN,
    COLLECTION_STATUS_LOCKED,
    COLLECTION_STATUS_CLAIMED,
    COLLECTION_STATUS_CLEARED
};

enum
{
    COLLECTION_CAT_TMS_HMS,
    COLLECTION_CAT_BATTLE_ITEMS,
    COLLECTION_CAT_MEGA_STONES,
    COLLECTION_CAT_HELD_ITEMS,
    COLLECTION_CAT_EVOLUTION_ITEMS,
    COLLECTION_CAT_MEDICINE,
    COLLECTION_CAT_POKE_BALLS,
    COLLECTION_CAT_BERRIES,
    COLLECTION_CAT_VALUABLES,
    COLLECTION_CAT_KEY_ITEMS,
    COLLECTION_CAT_GIFTS_TRADES,
    COLLECTION_CAT_HIDDEN_ITEMS,
    COLLECTION_CAT_OTHER,
    COLLECTION_CAT_COUNT
};

enum
{
    COLLECTION_AREA_ROUTE_29,
    COLLECTION_AREA_VIOLET_CITY,
    COLLECTION_AREA_AZALEA_TOWN,
    COLLECTION_AREA_GOLDENROD_CITY,
    COLLECTION_AREA_NATIONAL_PARK,
    COLLECTION_AREA_RUINS_OF_ALPH,
    COLLECTION_AREA_COUNT
};

#define SIGHTING_ENTRY(_species, _region, _class)                               \
    {                                                                           \
        .species = _species,                                                    \
        .region = _region,                                                      \
        .classification = _class,                                               \
        .rumorText = sText_SightingsRumorGeneric,                               \
        .whereaboutsText = sText_SightingsWhereGeneric,                         \
        .conditionsText = sText_SightingsConditionsGeneric,                     \
        .fieldNotesText = sText_SightingsNoNotes,                               \
        .rumorFlag = POKELINK_SIGHTINGS_FLAG_NONE,                              \
        .locatedFlag = POKELINK_SIGHTINGS_FLAG_NONE,                            \
        .encounteredFlag = POKELINK_SIGHTINGS_FLAG_NONE,                        \
    }

static const struct PokeLinkSightingsEntry sSightingsEntries[] =
{
    SIGHTING_ENTRY(SPECIES_ARTICUNO,  sText_RegionKanto,  sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_ZAPDOS,    sText_RegionKanto,  sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_MOLTRES,   sText_RegionKanto,  sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_MEWTWO,    sText_RegionKanto,  sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_MEW,       sText_RegionKanto,  sText_SightingsMythical),
    SIGHTING_ENTRY(SPECIES_RAIKOU,    sText_RegionJohto,  sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_ENTEI,     sText_RegionJohto,  sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_SUICUNE,   sText_RegionJohto,  sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_LUGIA,     sText_RegionJohto,  sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_HO_OH,     sText_RegionJohto,  sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_CELEBI,    sText_RegionJohto,  sText_SightingsMythical),
    SIGHTING_ENTRY(SPECIES_REGIROCK,  sText_RegionHoenn,  sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_REGICE,    sText_RegionHoenn,  sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_REGISTEEL, sText_RegionHoenn,  sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_LATIAS,    sText_RegionHoenn,  sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_LATIOS,    sText_RegionHoenn,  sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_KYOGRE,    sText_RegionHoenn,  sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_GROUDON,   sText_RegionHoenn,  sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_RAYQUAZA,  sText_RegionHoenn,  sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_JIRACHI,   sText_RegionHoenn,  sText_SightingsMythical),
    SIGHTING_ENTRY(SPECIES_DEOXYS,    sText_RegionHoenn,  sText_SightingsMythical),
    SIGHTING_ENTRY(SPECIES_UXIE,      sText_RegionSinnoh, sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_MESPRIT,   sText_RegionSinnoh, sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_AZELF,     sText_RegionSinnoh, sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_DIALGA,    sText_RegionSinnoh, sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_PALKIA,    sText_RegionSinnoh, sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_HEATRAN,   sText_RegionSinnoh, sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_REGIGIGAS, sText_RegionSinnoh, sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_GIRATINA,  sText_RegionSinnoh, sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_CRESSELIA, sText_RegionSinnoh, sText_SightingsLegendary),
    SIGHTING_ENTRY(SPECIES_PHIONE,    sText_RegionSinnoh, sText_SightingsMythical),
    SIGHTING_ENTRY(SPECIES_MANAPHY,   sText_RegionSinnoh, sText_SightingsMythical),
    SIGHTING_ENTRY(SPECIES_DARKRAI,   sText_RegionSinnoh, sText_SightingsMythical),
    SIGHTING_ENTRY(SPECIES_SHAYMIN,   sText_RegionSinnoh, sText_SightingsMythical),
    SIGHTING_ENTRY(SPECIES_ARCEUS,    sText_RegionSinnoh, sText_SightingsMythical),
};

#undef SIGHTING_ENTRY

#define COLLECTION_ENTRY(_item, _category, _area, _qty, _note)                  \
    {                                                                           \
        .itemId = _item,                                                        \
        .category = _category,                                                  \
        .area = _area,                                                          \
        .quantity = _qty,                                                       \
        .visibilityFlag = POKELINK_COLLECTION_FLAG_NONE,                        \
        .seenFlag = POKELINK_COLLECTION_FLAG_NONE,                              \
        .obtainedFlag = POKELINK_COLLECTION_FLAG_NONE,                          \
        .lockedFlag = POKELINK_COLLECTION_FLAG_NONE,                            \
        .note = _note,                                                          \
    }

static const struct PokeLinkCollectionArea sCollectionAreas[] =
{
    [COLLECTION_AREA_ROUTE_29] = {sText_AreaRoute29, POKELINK_COLLECTION_FLAG_NONE},
    [COLLECTION_AREA_VIOLET_CITY] = {sText_AreaVioletCity, POKELINK_COLLECTION_FLAG_NONE},
    [COLLECTION_AREA_AZALEA_TOWN] = {sText_AreaAzaleaTown, POKELINK_COLLECTION_FLAG_NONE},
    [COLLECTION_AREA_GOLDENROD_CITY] = {sText_AreaGoldenrodCity, POKELINK_COLLECTION_FLAG_NONE},
    [COLLECTION_AREA_NATIONAL_PARK] = {sText_AreaNationalPark, POKELINK_COLLECTION_FLAG_NONE},
    [COLLECTION_AREA_RUINS_OF_ALPH] = {sText_AreaRuinsOfAlph, POKELINK_COLLECTION_FLAG_NONE},
};

static const struct PokeLinkCollectionCategory sCollectionCategories[] =
{
    [COLLECTION_CAT_TMS_HMS] = {sText_CategoryTmsHms},
    [COLLECTION_CAT_BATTLE_ITEMS] = {sText_CategoryBattleItems},
    [COLLECTION_CAT_MEGA_STONES] = {sText_CategoryMegaStones},
    [COLLECTION_CAT_HELD_ITEMS] = {sText_CategoryHeldItems},
    [COLLECTION_CAT_EVOLUTION_ITEMS] = {sText_CategoryEvolutionItems},
    [COLLECTION_CAT_MEDICINE] = {sText_CategoryMedicine},
    [COLLECTION_CAT_POKE_BALLS] = {sText_CategoryPokeBalls},
    [COLLECTION_CAT_BERRIES] = {sText_CategoryBerries},
    [COLLECTION_CAT_VALUABLES] = {sText_CategoryValuables},
    [COLLECTION_CAT_KEY_ITEMS] = {sText_CategoryKeyItems},
    [COLLECTION_CAT_GIFTS_TRADES] = {sText_CategoryGiftsTrades},
    [COLLECTION_CAT_HIDDEN_ITEMS] = {sText_CategoryHiddenItems},
    [COLLECTION_CAT_OTHER] = {sText_CategoryOther},
};

static const struct PokeLinkCollectionEntry sCollectionEntries[] =
{
    COLLECTION_ENTRY(ITEM_POTION,      COLLECTION_CAT_MEDICINE,        COLLECTION_AREA_ROUTE_29,      1, sText_CollectionNoteVisible),
    COLLECTION_ENTRY(ITEM_POKE_BALL,   COLLECTION_CAT_POKE_BALLS,      COLLECTION_AREA_ROUTE_29,      3, sText_CollectionNoteGift),
    COLLECTION_ENTRY(ITEM_ORAN_BERRY,  COLLECTION_CAT_BERRIES,         COLLECTION_AREA_ROUTE_29,      1, sText_CollectionNoteHidden),
    COLLECTION_ENTRY(ITEM_TM01,        COLLECTION_CAT_TMS_HMS,         COLLECTION_AREA_VIOLET_CITY,   1, sText_CollectionNoteGift),
    COLLECTION_ENTRY(ITEM_X_ATTACK,    COLLECTION_CAT_BATTLE_ITEMS,    COLLECTION_AREA_VIOLET_CITY,   1, sText_CollectionNoteVisible),
    COLLECTION_ENTRY(ITEM_FIRE_STONE,  COLLECTION_CAT_EVOLUTION_ITEMS, COLLECTION_AREA_AZALEA_TOWN,   1, sText_CollectionNoteProgress),
    COLLECTION_ENTRY(ITEM_LEFTOVERS,   COLLECTION_CAT_HELD_ITEMS,      COLLECTION_AREA_GOLDENROD_CITY,1, sText_CollectionNoteHidden),
    COLLECTION_ENTRY(ITEM_BICYCLE,     COLLECTION_CAT_KEY_ITEMS,       COLLECTION_AREA_GOLDENROD_CITY,1, sText_CollectionNoteGift),
    COLLECTION_ENTRY(ITEM_NUGGET,      COLLECTION_CAT_VALUABLES,       COLLECTION_AREA_NATIONAL_PARK, 1, sText_CollectionNoteHidden),
    COLLECTION_ENTRY(ITEM_VENUSAURITE, COLLECTION_CAT_MEGA_STONES,     COLLECTION_AREA_NATIONAL_PARK, 1, sText_CollectionNoteProgress),
    COLLECTION_ENTRY(ITEM_HM01,        COLLECTION_CAT_TMS_HMS,         COLLECTION_AREA_RUINS_OF_ALPH, 1, sText_CollectionNoteGift),
    COLLECTION_ENTRY(ITEM_NUGGET,      COLLECTION_CAT_HIDDEN_ITEMS,    COLLECTION_AREA_RUINS_OF_ALPH, 1, sText_CollectionNoteHidden),
};

#undef COLLECTION_ENTRY

static const u16 sDelibirdDeliveryBasicItems[] =
{
    ITEM_POKE_BALL,
    ITEM_POTION,
    ITEM_ANTIDOTE,
    ITEM_PARALYZE_HEAL,
    ITEM_AWAKENING,
    ITEM_ESCAPE_ROPE,
    ITEM_REPEL,
    ITEM_NONE
};

static const u16 sDelibirdDeliveryMidItems[] =
{
    ITEM_POKE_BALL,
    ITEM_GREAT_BALL,
    ITEM_POTION,
    ITEM_SUPER_POTION,
    ITEM_ANTIDOTE,
    ITEM_PARALYZE_HEAL,
    ITEM_BURN_HEAL,
    ITEM_ICE_HEAL,
    ITEM_AWAKENING,
    ITEM_ESCAPE_ROPE,
    ITEM_REPEL,
    ITEM_SUPER_REPEL,
    ITEM_NONE
};

static const u16 sDelibirdDeliveryLateItems[] =
{
    ITEM_POKE_BALL,
    ITEM_GREAT_BALL,
    ITEM_ULTRA_BALL,
    ITEM_SUPER_POTION,
    ITEM_HYPER_POTION,
    ITEM_MAX_POTION,
    ITEM_ANTIDOTE,
    ITEM_PARALYZE_HEAL,
    ITEM_BURN_HEAL,
    ITEM_ICE_HEAL,
    ITEM_AWAKENING,
    ITEM_ESCAPE_ROPE,
    ITEM_SUPER_REPEL,
    ITEM_MAX_REPEL,
    ITEM_NONE
};

static const struct PokeLinkApp sPokeLinkApps[POKELINK_APP_COUNT] =
{
    [POKELINK_APP_MAP]        = {sText_AppMap,        sShort_Map,        sDesc_Map,        NULL,             FLAG_POKELINK_APP_MAP,        FALSE, TRUE},
    [POKELINK_APP_SIGHTINGS]  = {sText_AppSightings,  sShort_Sightings,  sDesc_Sightings,  NULL,             0,                            TRUE,  TRUE},
    [POKELINK_APP_COLLECTION_LOG] = {sText_AppCollectionLog, sShort_CollectionLog, sDesc_CollectionLog, NULL, 0,                            TRUE,  TRUE},
    [POKELINK_APP_PROFILE]    = {sText_AppProfile,    sShort_Profile,    sDesc_Profile,    NULL,             0,                            TRUE,  TRUE},
    [POKELINK_APP_GLOOMSCROLL] = {sText_AppGloomscroll, sShort_Gloomscroll, sDesc_Gloomscroll, sMsg_Gloomscroll, 0,                            TRUE,  TRUE},
    [POKELINK_APP_DEXNAV]     = {sText_AppDexNav,     sShort_DexNav,     sDesc_DexNav,     sMsg_DexNav,      FLAG_POKELINK_APP_DEXNAV,     FALSE, TRUE},
    [POKELINK_APP_RADIO]      = {sText_AppRadio,      sShort_Radio,      sDesc_Radio,      sMsg_Radio,       FLAG_POKELINK_APP_RADIO,      FALSE, TRUE},
    [POKELINK_APP_VS_SEEKER]  = {sText_AppVsSeeker,   sShort_VsSeeker,   sDesc_VsSeeker,   sMsg_VsSeeker,    FLAG_POKELINK_APP_VS_SEEKER,  FALSE, TRUE},
    [POKELINK_APP_FLASHLIGHT] = {sText_AppFlashlight, sShort_Flashlight, sDesc_Flashlight, sMsg_Flashlight,  FLAG_POKELINK_APP_FLASHLIGHT, FALSE, TRUE},
    [POKELINK_APP_DELIVERY]   = {sText_AppDelivery,   sShort_Delivery,   sDesc_Delivery,   sMsg_Delivery,    FLAG_POKELINK_APP_DELIVERY,   FALSE, TRUE},
    [POKELINK_APP_ABRACAB]    = {sText_AppAbraCab,    sShort_AbraCab,    sDesc_AbraCab,    sMsg_AbraCab,     FLAG_POKELINK_APP_ABRACAB,    FALSE, TRUE},
    [POKELINK_APP_NOTES]      = {sText_AppNotes,      sShort_Notes,      sDesc_Notes,      sMsg_Notes,       FLAG_POKELINK_APP_NOTES,      FALSE, TRUE},
};

static const u8 sDefaultShortcutApps[] =
{
    POKELINK_APP_PROFILE,
    POKELINK_APP_SIGHTINGS,
    POKELINK_APP_MAP,
    POKELINK_APP_COLLECTION_LOG,
};

enum
{
    POKELINK_ICON_MAP,
    POKELINK_ICON_DEXNAV,
    POKELINK_ICON_RADIO,
    POKELINK_ICON_VS_SEEKER,
    POKELINK_ICON_FLASHLIGHT,
    POKELINK_ICON_DELIVERY,
    POKELINK_ICON_ABRACAB,
    POKELINK_ICON_NOTES,
    POKELINK_ICON_PROFILE,
    POKELINK_ICON_SETTINGS,
    POKELINK_ICON_GLOOMSCROLL,
    POKELINK_ICON_SIGHTINGS,
    POKELINK_ICON_COLLECTION_LOG,
};

static const u8 sPokeLinkAppIconIndexes[POKELINK_APP_COUNT] =
{
    [POKELINK_APP_MAP]        = POKELINK_ICON_MAP,
    [POKELINK_APP_SIGHTINGS]  = POKELINK_ICON_SIGHTINGS,
    [POKELINK_APP_COLLECTION_LOG] = POKELINK_ICON_COLLECTION_LOG,
    [POKELINK_APP_PROFILE]    = POKELINK_ICON_PROFILE,
    [POKELINK_APP_GLOOMSCROLL] = POKELINK_ICON_GLOOMSCROLL,
    [POKELINK_APP_DEXNAV]     = POKELINK_ICON_DEXNAV,
    [POKELINK_APP_RADIO]      = POKELINK_ICON_RADIO,
    [POKELINK_APP_VS_SEEKER]  = POKELINK_ICON_VS_SEEKER,
    [POKELINK_APP_FLASHLIGHT] = POKELINK_ICON_FLASHLIGHT,
    [POKELINK_APP_DELIVERY]   = POKELINK_ICON_DELIVERY,
    [POKELINK_APP_ABRACAB]    = POKELINK_ICON_ABRACAB,
    [POKELINK_APP_NOTES]      = POKELINK_ICON_NOTES,
};

static const struct BgTemplate sPokeLinkBgTemplates[] =
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 2,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0
    },
};

static const struct WindowTemplate sPokeLinkWindowTemplates[] =
{
    [WIN_HEADER] =
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 1,
        .height = 1,
        .paletteNum = 15,
        .baseBlock = 1
    },
    [WIN_APPS] =
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 2,
        .width = 28,
        .height = 14,
        .paletteNum = 15,
        .baseBlock = 2
    },
    [WIN_DETAIL] =
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 16,
        .width = 26,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 394
    },
    [WIN_HELP] =
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 18,
        .width = 28,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 446
    },
    DUMMY_WIN_TEMPLATE
};

static const u8 sPokeLinkIconsGfx[] = INCBIN_U8("graphics/pokelink/pokelink_icons_32x32.4bpp");
static const u8 sPokeLinkCursorGfx[] = INCBIN_U8("graphics/pokelink/pokelink_cursor.4bpp");
static const u16 sPokeLinkAssetsPal[] = INCBIN_U16("graphics/pokelink/pokelink_icons_32x32.gbapal");
static const u8 sPokeLinkSightingsIconGfx[] = INCBIN_U8("graphics/pokelink/sightings_icon_32x32.4bpp");
static const u16 sPokeLinkSightingsIconPal[] = INCBIN_U16("graphics/pokelink/sightings_icon_sherlock.gbapal");
static const u8 sPokeLinkCollectionIconGfx[] = INCBIN_U8("graphics/pokelink/collection_log_icon_32x32.4bpp");
static const u16 sPokeLinkCollectionIconPal[] = INCBIN_U16("graphics/pokelink/collection_log_icon.gbapal");
static const u16 sPokeLinkBgPal[] = INCBIN_U16("graphics/pokelink/pokelink_bg.gbapal");
static const u32 sPokeLinkBgGfxLZ[] = INCBIN_U32("graphics/pokelink/pokelink_bg.4bpp.lz");
static const u32 sPokeLinkBgTilemapLZ[] = INCBIN_U32("graphics/pokelink/pokelink_bg.bin.lz");

static const struct OamData sPokeLinkIconOam =
{
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 1
};

static const struct OamData sPokeLinkCursorOam =
{
    .shape = SPRITE_SHAPE(32x32),
    .size = SPRITE_SIZE(32x32),
    .priority = 0
};

static const union AnimCmd sPokeLinkIconAnim_Map[] =
{
    ANIMCMD_FRAME(POKELINK_ICON_MAP * POKELINK_ICON_SPRITE_TILE_COUNT, 0),
    ANIMCMD_END
};
static const union AnimCmd sPokeLinkIconAnim_DexNav[] =
{
    ANIMCMD_FRAME(POKELINK_ICON_DEXNAV * POKELINK_ICON_SPRITE_TILE_COUNT, 0),
    ANIMCMD_END
};
static const union AnimCmd sPokeLinkIconAnim_Radio[] =
{
    ANIMCMD_FRAME(POKELINK_ICON_RADIO * POKELINK_ICON_SPRITE_TILE_COUNT, 0),
    ANIMCMD_END
};
static const union AnimCmd sPokeLinkIconAnim_VsSeeker[] =
{
    ANIMCMD_FRAME(POKELINK_ICON_VS_SEEKER * POKELINK_ICON_SPRITE_TILE_COUNT, 0),
    ANIMCMD_END
};
static const union AnimCmd sPokeLinkIconAnim_Flashlight[] =
{
    ANIMCMD_FRAME(POKELINK_ICON_FLASHLIGHT * POKELINK_ICON_SPRITE_TILE_COUNT, 0),
    ANIMCMD_END
};
static const union AnimCmd sPokeLinkIconAnim_Delivery[] =
{
    ANIMCMD_FRAME(POKELINK_ICON_DELIVERY * POKELINK_ICON_SPRITE_TILE_COUNT, 0),
    ANIMCMD_END
};
static const union AnimCmd sPokeLinkIconAnim_AbraCab[] =
{
    ANIMCMD_FRAME(POKELINK_ICON_ABRACAB * POKELINK_ICON_SPRITE_TILE_COUNT, 0),
    ANIMCMD_END
};
static const union AnimCmd sPokeLinkIconAnim_Notes[] =
{
    ANIMCMD_FRAME(POKELINK_ICON_NOTES * POKELINK_ICON_SPRITE_TILE_COUNT, 0),
    ANIMCMD_END
};
static const union AnimCmd sPokeLinkIconAnim_Profile[] =
{
    ANIMCMD_FRAME(POKELINK_ICON_PROFILE * POKELINK_ICON_SPRITE_TILE_COUNT, 0),
    ANIMCMD_END
};
static const union AnimCmd sPokeLinkIconAnim_Settings[] =
{
    ANIMCMD_FRAME(POKELINK_ICON_SETTINGS * POKELINK_ICON_SPRITE_TILE_COUNT, 0),
    ANIMCMD_END
};
static const union AnimCmd sPokeLinkIconAnim_Gloomscroll[] =
{
    ANIMCMD_FRAME(POKELINK_ICON_GLOOMSCROLL * POKELINK_ICON_SPRITE_TILE_COUNT, 0),
    ANIMCMD_END
};
static const union AnimCmd sPokeLinkIconAnim_Sightings[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};
static const union AnimCmd sPokeLinkIconAnim_CollectionLog[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sPokeLinkIconAnimTable[] =
{
    sPokeLinkIconAnim_Map,
    sPokeLinkIconAnim_DexNav,
    sPokeLinkIconAnim_Radio,
    sPokeLinkIconAnim_VsSeeker,
    sPokeLinkIconAnim_Flashlight,
    sPokeLinkIconAnim_Delivery,
    sPokeLinkIconAnim_AbraCab,
    sPokeLinkIconAnim_Notes,
    sPokeLinkIconAnim_Profile,
    sPokeLinkIconAnim_Settings,
    sPokeLinkIconAnim_Gloomscroll,
    sPokeLinkIconAnim_Sightings,
    sPokeLinkIconAnim_CollectionLog
};

static const union AnimCmd sPokeLinkCursorAnim[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sPokeLinkCursorAnimTable[] =
{
    sPokeLinkCursorAnim
};

static const struct SpriteTemplate sPokeLinkIconSpriteTemplate =
{
    .tileTag = TAG_POKELINK_ICONS,
    .paletteTag = TAG_POKELINK_ASSETS,
    .oam = &sPokeLinkIconOam,
    .anims = sPokeLinkIconAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const struct SpriteTemplate sPokeLinkCursorSpriteTemplate =
{
    .tileTag = TAG_POKELINK_CURSOR,
    .paletteTag = TAG_POKELINK_ASSETS,
    .oam = &sPokeLinkCursorOam,
    .anims = sPokeLinkCursorAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static EWRAM_DATA struct PokeLinkState *sPokeLink = NULL;
static EWRAM_DATA bool8 sInitialMessagePending = FALSE;
static EWRAM_DATA bool8 sInitialGloomscrollPending = FALSE;
static EWRAM_DATA bool8 sInitialSightingsPending = FALSE;
static EWRAM_DATA bool8 sInitialCollectionPending = FALSE;
static EWRAM_DATA u8 sInitialMessageAppId = 0;
static EWRAM_DATA u16 sPokeLinkIconTileStart = 0;
static EWRAM_DATA u16 sPokeLinkSightingsIconTileStart = 0;
static EWRAM_DATA u16 sPokeLinkCollectionIconTileStart = 0;
static EWRAM_DATA u8 sPokeLinkAssetsPalSlot = 0;
static EWRAM_DATA u8 sPokeLinkSightingsIconPalSlot = 0;
static EWRAM_DATA u8 sPokeLinkCollectionIconPalSlot = 0;
static EWRAM_DATA u8 sDelibirdTipWindowId = 0;
static EWRAM_DATA u8 sDelibirdTipCursor = 0;
static EWRAM_DATA u8 sDelibirdDeliveryObjectEventId = 0;
static EWRAM_DATA u8 sDelibirdDeliveryMoveStep = 0;
static EWRAM_DATA u8 sDelibirdDeliveryPathLength = 0;
static EWRAM_DATA u8 sDelibirdDeliveryPath[POKELINK_DELIVERY_MAX_PATH_STEPS] = {0};
static EWRAM_DATA u8 sDelibirdDeliveryFacePlayerAction = 0;
static EWRAM_DATA u8 sDelibirdDeliveryFaceCourierDirection = 0;
static EWRAM_DATA bool8 sDelibirdDeliverySurfing = FALSE;
static EWRAM_DATA bool8 sDelibirdDeliveryObjectActive = FALSE;

void CB2_InitPokeLink(void)
{
    switch (gMain.state)
    {
    case 0:
    default:
        SetVBlankCallback(NULL);
        SetGpuReg(REG_OFFSET_DISPCNT, 0);
        DmaClearLarge16(3, (void *)VRAM, VRAM_SIZE, 0x1000);
        DmaClear32(3, OAM, OAM_SIZE);
        DmaClear16(3, PLTT, PLTT_SIZE);
        ResetBgsAndClearDma3BusyFlags(0);
        InitBgsFromTemplates(0, sPokeLinkBgTemplates, ARRAY_COUNT(sPokeLinkBgTemplates));
        ChangeBgX(0, 0, BG_COORD_SET);
        ChangeBgY(0, 0, BG_COORD_SET);
        ChangeBgX(1, 0, BG_COORD_SET);
        ChangeBgY(1, 0, BG_COORD_SET);
        InitWindows(sPokeLinkWindowTemplates);
        FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, 32, 32);
        CopyBgTilemapBufferToVram(0);
        DeactivateAllTextPrinters();
        ResetPaletteFade();
        ResetTasks();
        ResetSpriteData();
        FreeAllSpritePalettes();
        ScanlineEffect_Stop();
        LoadMessageBoxAndBorderGfx();
        Menu_LoadStdPalAt(BG_PLTT_ID(15));
        LoadPokeLinkBackground();
        gMain.state++;
        break;
    case 1:
        sPokeLink = AllocZeroed(sizeof(*sPokeLink));
        if (sPokeLink == NULL)
        {
            SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
            return;
        }
        BuildVisibleAppList();
        if (sPokeLink->visibleCount == 0)
            sPokeLink->visibleApps[sPokeLink->visibleCount++] = POKELINK_APP_PROFILE;
        LoadPokeLinkGraphics();
        CreatePokeLinkSprites();
        DrawPokeLink();
        SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
        ShowBg(1);
        ShowBg(0);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        SetVBlankCallback(VBlankCB_PokeLink);
        CreateTask(Task_PokeLink, 0);
        SetMainCallback2(CB2_PokeLink);
        gMain.state = 0;
        break;
    }
}

void PokeLink_OpenAppFromField(u8 appId)
{
    if (appId >= POKELINK_APP_COUNT)
        appId = POKELINK_APP_PROFILE;

    if (appId == POKELINK_APP_MAP && IsAppAvailable(appId))
    {
        FieldInitRegionMap(CB2_ReturnToFieldWithOpenMenu);
    }
    else if (appId == POKELINK_APP_DELIVERY)
    {
        LockPlayerFieldControls();
        gFieldCallback = FieldCallback_OpenDelibirdDelivery;
        SetMainCallback2(CB2_ReturnToField);
    }
    else if (appId == POKELINK_APP_ABRACAB)
    {
        FieldInitAbraCabMap(CB2_ReturnToFieldWithOpenMenu);
    }
    else if (appId == POKELINK_APP_PROFILE)
    {
        SetMainCallback2(CB2_InitPokeLinkTetris);
    }
    else if (appId == POKELINK_APP_RADIO)
    {
        SetMainCallback2(CB2_InitPokeLinkRadio);
    }
    else if (appId == POKELINK_APP_GLOOMSCROLL)
    {
        sInitialGloomscrollPending = TRUE;
        SetMainCallback2(CB2_InitPokeLink);
    }
    else if (appId == POKELINK_APP_SIGHTINGS)
    {
        sInitialSightingsPending = TRUE;
        SetMainCallback2(CB2_InitPokeLink);
    }
    else if (appId == POKELINK_APP_COLLECTION_LOG)
    {
        sInitialCollectionPending = TRUE;
        SetMainCallback2(CB2_InitPokeLink);
    }
    else
    {
        sInitialMessageAppId = appId;
        sInitialMessagePending = TRUE;
        SetMainCallback2(CB2_InitPokeLink);
    }
}

const u8 *PokeLink_GetShortcutName(u8 slot)
{
    u8 appId = PokeLink_GetShortcutAppId(slot);

    if (appId == POKELINK_SHORTCUT_NONE)
        return sText_PokeLinkNone;

    return sPokeLinkApps[appId].name;
}

u8 PokeLink_GetShortcutAppId(u8 slot)
{
    u16 *varPtr;
    u8 appId;

    if (slot >= POKELINK_FAVORITE_COUNT)
        return POKELINK_SHORTCUT_NONE;

    varPtr = GetFavoriteVarPtr(slot);
    if (varPtr != NULL && IsFavoriteSlotValid(*varPtr))
        return *varPtr - 1;

    appId = GetDefaultShortcutAppId(slot);
    if (appId == POKELINK_SHORTCUT_NONE)
        return POKELINK_SHORTCUT_NONE;

    return appId;
}

static void CB2_PokeLink(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB_PokeLink(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

#define tState data[0]
#define tTimer data[1]
#define tMenuCursor data[2]
#define tSightingsCursor data[3]
#define tSightingsTop data[4]
#define tCollectionMode data[5]
#define tCollectionCursor data[6]
#define tCollectionTop data[7]

static void Task_PokeLink(u8 taskId)
{
    u8 appId;
    s16 *data = gTasks[taskId].data;

    switch (tState)
    {
    case POKELINK_STATE_FADE_IN:
        if (!gPaletteFade.active)
        {
            if (sInitialGloomscrollPending)
            {
                sInitialGloomscrollPending = FALSE;
                tMenuCursor = 0;
                DrawGloomscrollConfirm(tMenuCursor);
                tState = POKELINK_STATE_GLOOM_CONFIRM;
            }
            else if (sInitialSightingsPending)
            {
                sInitialSightingsPending = FALSE;
                tSightingsCursor = 0;
                tSightingsTop = 0;
                DrawSightingsList(tSightingsCursor, tSightingsTop);
                tState = POKELINK_STATE_SIGHTINGS_LIST;
            }
            else if (sInitialCollectionPending)
            {
                sInitialCollectionPending = FALSE;
                tCollectionMode = COLLECTION_MODE_AREA;
                tCollectionCursor = 0;
                tCollectionTop = 0;
                DrawCollectionList(tCollectionMode, tCollectionCursor, tCollectionTop);
                tState = POKELINK_STATE_COLLECTION_LIST;
            }
            else if (sInitialMessagePending)
            {
                DrawPokeLinkMessage(sInitialMessageAppId);
                sInitialMessagePending = FALSE;
                sInitialMessageAppId = 0;
                tState = POKELINK_STATE_MESSAGE;
            }
            else
            {
                tState = POKELINK_STATE_MAIN;
            }
        }
        break;
    case POKELINK_STATE_MAIN:
        if (JOY_NEW(DPAD_UP))
            MovePokeLinkCursor(0, -1);
        else if (JOY_NEW(DPAD_DOWN))
            MovePokeLinkCursor(0, 1);
        else if (JOY_NEW(DPAD_LEFT))
            MovePokeLinkCursor(-1, 0);
        else if (JOY_NEW(DPAD_RIGHT))
            MovePokeLinkCursor(1, 0);
        else if (JOY_NEW(SELECT_BUTTON))
        {
            appId = sPokeLink->visibleApps[sPokeLink->cursor];
            ToggleFavorite(appId);
            DrawPokeLinkDetail();
            DrawPokeLinkHelp();
        }
        else if (JOY_NEW(A_BUTTON))
        {
            appId = sPokeLink->visibleApps[sPokeLink->cursor];
            OpenSelectedApp(taskId, appId);
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            tState = POKELINK_STATE_EXIT;
        }
        break;
    case POKELINK_STATE_MESSAGE:
        if (JOY_NEW(A_BUTTON | B_BUTTON))
        {
            PlaySE(SE_SELECT);
            DrawPokeLink();
            tState = POKELINK_STATE_MAIN;
        }
        break;
    case POKELINK_STATE_GLOOM_CONFIRM:
        if (JOY_NEW(DPAD_UP | DPAD_DOWN))
        {
            tMenuCursor ^= 1;
            PlaySE(SE_SELECT);
            DrawGloomscrollConfirm(tMenuCursor);
        }
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            if (tMenuCursor == 0)
                StartGloomscrollFeed(taskId);
            else
            {
                DrawPokeLink();
                tState = POKELINK_STATE_MAIN;
            }
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            DrawPokeLink();
            tState = POKELINK_STATE_MAIN;
        }
        break;
    case POKELINK_STATE_GLOOM_FEED:
        tTimer++;
        if (tTimer % 24 == 0)
            DrawGloomscrollFeed((tTimer / 24) % 3);
        if (tTimer >= 156)
        {
            DayNight_AdvanceToNextDayNightPeriod();
            DrawGloomscrollResultMessage();
            tState = POKELINK_STATE_MESSAGE;
        }
        break;
    case POKELINK_STATE_SIGHTINGS_LIST:
        if (JOY_NEW(DPAD_UP))
            MoveSightingsCursor(&tSightingsCursor, &tSightingsTop, -1);
        else if (JOY_NEW(DPAD_DOWN))
            MoveSightingsCursor(&tSightingsCursor, &tSightingsTop, 1);
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            DrawSightingsDetail(tSightingsCursor);
            tState = POKELINK_STATE_SIGHTINGS_DETAIL;
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            DrawPokeLink();
            tState = POKELINK_STATE_MAIN;
        }
        break;
    case POKELINK_STATE_SIGHTINGS_DETAIL:
        if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            DrawSightingsList(tSightingsCursor, tSightingsTop);
            tState = POKELINK_STATE_SIGHTINGS_LIST;
        }
        break;
    case POKELINK_STATE_COLLECTION_LIST:
        if (JOY_NEW(DPAD_UP))
            MoveCollectionCursor(&tCollectionCursor, &tCollectionTop, -1, tCollectionMode);
        else if (JOY_NEW(DPAD_DOWN))
            MoveCollectionCursor(&tCollectionCursor, &tCollectionTop, 1, tCollectionMode);
        else if (JOY_NEW(L_BUTTON | R_BUTTON | DPAD_LEFT | DPAD_RIGHT))
        {
            PlaySE(SE_SELECT);
            tCollectionMode = ToggleCollectionMode(&tCollectionCursor, &tCollectionTop, tCollectionMode);
            DrawCollectionList(tCollectionMode, tCollectionCursor, tCollectionTop);
        }
        else if (JOY_NEW(A_BUTTON))
        {
            PlaySE(SE_SELECT);
            DrawCollectionDetail(tCollectionMode, tCollectionCursor);
            tState = POKELINK_STATE_COLLECTION_DETAIL;
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            DrawPokeLink();
            tState = POKELINK_STATE_MAIN;
        }
        break;
    case POKELINK_STATE_COLLECTION_DETAIL:
        if (JOY_NEW(L_BUTTON | R_BUTTON | DPAD_LEFT | DPAD_RIGHT))
        {
            PlaySE(SE_SELECT);
            tCollectionMode = ToggleCollectionMode(&tCollectionCursor, &tCollectionTop, tCollectionMode);
            DrawCollectionList(tCollectionMode, tCollectionCursor, tCollectionTop);
            tState = POKELINK_STATE_COLLECTION_LIST;
        }
        else if (JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_SELECT);
            DrawCollectionList(tCollectionMode, tCollectionCursor, tCollectionTop);
            tState = POKELINK_STATE_COLLECTION_LIST;
        }
        break;
    case POKELINK_STATE_LAUNCH:
        if (!gPaletteFade.active)
        {
            appId = sPokeLink->launchAppId;
            CleanupPokeLink();
            DestroyTask(taskId);
            PokeLink_OpenAppFromField(appId);
        }
        break;
    case POKELINK_STATE_EXIT:
        if (!gPaletteFade.active)
        {
            CleanupPokeLink();
            DestroyTask(taskId);
            SetMainCallback2(CB2_ReturnToFieldWithOpenMenu);
        }
        break;
    }
}

#undef tState
#undef tTimer
#undef tMenuCursor
#undef tSightingsCursor
#undef tSightingsTop
#undef tCollectionMode
#undef tCollectionCursor
#undef tCollectionTop

static void BuildVisibleAppList(void)
{
    u8 i;

    sPokeLink->visibleCount = 0;
    sPokeLink->cursor = 0;
    sPokeLink->top = 0;

    for (i = 0; i < POKELINK_APP_COUNT; i++)
    {
        if (IsAppAvailable(i))
            sPokeLink->visibleApps[sPokeLink->visibleCount++] = i;
    }
}

static void DrawPokeLink(void)
{
    DrawPokeLinkHeader();
    DrawPokeLinkApps();
    DrawPokeLinkDetail();
    DrawPokeLinkHelp();
}

static void DrawPokeLinkHeader(void)
{
    FillWindowPixelBuffer(WIN_HEADER, PIXEL_FILL(0));
    PutWindowTilemap(WIN_HEADER);
    CopyWindowToVram(WIN_HEADER, COPYWIN_FULL);
}

static void DrawPokeLinkApps(void)
{
    u8 i;
    u8 pagePos;
    s16 iconX;
    s16 iconY;

    FillWindowPixelBuffer(WIN_APPS, PIXEL_FILL(0));

    for (i = 0; i < POKELINK_PAGE_SIZE && sPokeLink->top + i < sPokeLink->visibleCount; i++)
    {
        pagePos = i;
        GetPokeLinkAppIconCoords(pagePos, &iconX, &iconY);
    }

    PutWindowTilemap(WIN_APPS);
    CopyWindowToVram(WIN_APPS, COPYWIN_FULL);
    UpdatePokeLinkSprites();
}

static void DrawPokeLinkDetail(void)
{
    u8 appId = sPokeLink->visibleApps[sPokeLink->cursor];
    u8 textX;

    FillWindowPixelBuffer(WIN_DETAIL, PIXEL_FILL(0));
    textX = GetStringCenterAlignXOffset(FONT_NORMAL, sPokeLinkApps[appId].name, sPokeLinkWindowTemplates[WIN_DETAIL].width * 8);
    AddTextPrinterParameterized3(WIN_DETAIL, FONT_NORMAL, textX, 1, sPokeLinkTextColors, TEXT_SKIP_DRAW, sPokeLinkApps[appId].name);

    PutWindowTilemap(WIN_DETAIL);
    CopyWindowToVram(WIN_DETAIL, COPYWIN_FULL);
}

static void DrawPokeLinkHelp(void)
{
    FillWindowPixelBuffer(WIN_HELP, PIXEL_FILL(0));
    AddTextPrinterParameterized(WIN_HELP, FONT_NARROW, sText_PokeLinkHelp, 0, 1, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_HELP);
    CopyWindowToVram(WIN_HELP, COPYWIN_FULL);
}

static void DrawPokeLinkMessage(u8 appId)
{
    if (appId >= POKELINK_APP_COUNT)
        appId = POKELINK_APP_PROFILE;

    SetPokeLinkSpritesVisible(FALSE);
    FillWindowPixelBuffer(WIN_DETAIL, PIXEL_FILL(1));
    AddTextPrinterParameterized(WIN_DETAIL, FONT_NORMAL, sPokeLinkApps[appId].name, 0, 0, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(WIN_DETAIL, FONT_NARROW, sPokeLinkApps[appId].message, 0, 12, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_DETAIL);
    CopyWindowToVram(WIN_DETAIL, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_HELP, PIXEL_FILL(0));
    AddTextPrinterParameterized(WIN_HELP, FONT_NARROW, sText_PokeLinkMessageHelp, 0, 1, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_HELP);
    CopyWindowToVram(WIN_HELP, COPYWIN_FULL);
}

static void DrawGloomscrollConfirm(u8 cursor)
{
    SetPokeLinkSpritesVisible(FALSE);

    FillWindowPixelBuffer(WIN_APPS, PIXEL_FILL(0));
    FillWindowPixelRect(WIN_APPS, PIXEL_FILL(2), 34, 10, 156, 88);
    FillWindowPixelRect(WIN_APPS, PIXEL_FILL(1), 38, 14, 148, 80);
    FillWindowPixelRect(WIN_APPS, PIXEL_FILL(2), 42, 18, 140, 20);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NORMAL, 57, 21, sPokeLinkTextColors, TEXT_SKIP_DRAW, sText_AppGloomscroll);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NORMAL, 43, 47, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, GetGloomscrollConfirmText());
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 64, 62, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sText_GloomscrollWarning);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NORMAL, 83, 78, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sText_GloomscrollYes);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NORMAL, 126, 78, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sText_GloomscrollNo);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NORMAL, cursor == 0 ? 72 : 115, 78, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, gText_SelectorArrow2);
    PutWindowTilemap(WIN_APPS);
    CopyWindowToVram(WIN_APPS, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_DETAIL, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WIN_DETAIL, FONT_NORMAL, 0, 1, sPokeLinkTextColors, TEXT_SKIP_DRAW, sText_AppGloomscroll);
    PutWindowTilemap(WIN_DETAIL);
    CopyWindowToVram(WIN_DETAIL, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_HELP, PIXEL_FILL(0));
    AddTextPrinterParameterized(WIN_HELP, FONT_NARROW, sText_PokeLinkConfirmHelp, 0, 1, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_HELP);
    CopyWindowToVram(WIN_HELP, COPYWIN_FULL);
}

static void DrawGloomscrollFeed(u8 step)
{
    static const u8 *const sHandles[] =
    {
        sText_GloomscrollHandle0,
        sText_GloomscrollHandle1,
        sText_GloomscrollHandle2,
    };
    static const u8 *const sPosts[] =
    {
        sText_GloomscrollPost0,
        sText_GloomscrollPost1,
        sText_GloomscrollPost2,
    };
    static const u8 *const sShorts[] =
    {
        sText_GloomscrollShort0,
        sText_GloomscrollShort1,
        sText_GloomscrollShort2,
    };
    u8 next = (step + 1) % ARRAY_COUNT(sPosts);
    u8 last = (step + 2) % ARRAY_COUNT(sPosts);

    SetPokeLinkSpritesVisible(FALSE);

    FillWindowPixelBuffer(WIN_APPS, PIXEL_FILL(0));
    FillWindowPixelRect(WIN_APPS, PIXEL_FILL(2), 6, 4, 212, 14);
    FillWindowPixelRect(WIN_APPS, PIXEL_FILL(3), 12, 8, 20, 3);
    FillWindowPixelRect(WIN_APPS, PIXEL_FILL(3), 38, 8, 46, 3);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 74, 3, sPokeLinkTextColors, TEXT_SKIP_DRAW, sText_AppGloomscroll);

    DrawGloomscrollPost(25, 4 + step, sHandles[step], sPosts[step]);
    DrawGloomscrollPost(55, 5, sHandles[next], sPosts[next]);
    DrawGloomscrollPost(85, 6, sHandles[last], sPosts[last]);
    DrawGloomscrollShort(165, 4 + step, sShorts[step]);

    PutWindowTilemap(WIN_APPS);
    CopyWindowToVram(WIN_APPS, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_DETAIL, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WIN_DETAIL, FONT_NARROW, 67, 4, sPokeLinkTextColors, TEXT_SKIP_DRAW, sText_GloomscrollLoading);
    PutWindowTilemap(WIN_DETAIL);
    CopyWindowToVram(WIN_DETAIL, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_HELP, PIXEL_FILL(0));
    AddTextPrinterParameterized(WIN_HELP, FONT_NARROW, sText_GloomscrollWarning, 0, 1, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_HELP);
    CopyWindowToVram(WIN_HELP, COPYWIN_FULL);
}

static void DrawGloomscrollResultMessage(void)
{
    SetPokeLinkSpritesVisible(FALSE);

    FillWindowPixelBuffer(WIN_APPS, PIXEL_FILL(0));
    FillWindowPixelRect(WIN_APPS, PIXEL_FILL(2), 24, 14, 176, 84);
    FillWindowPixelRect(WIN_APPS, PIXEL_FILL(1), 28, 18, 168, 76);
    FillWindowPixelRect(WIN_APPS, PIXEL_FILL(2), 34, 24, 156, 18);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NORMAL, 48, 26, sPokeLinkTextColors, TEXT_SKIP_DRAW, sText_GloomscrollClosed);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NORMAL, 39, 54, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sText_GloomscrollResult0);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NORMAL, 38, 70, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sText_GloomscrollResult1);
    PutWindowTilemap(WIN_APPS);
    CopyWindowToVram(WIN_APPS, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_DETAIL, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WIN_DETAIL, FONT_NORMAL, 0, 1, sPokeLinkTextColors, TEXT_SKIP_DRAW, sText_AppGloomscroll);
    PutWindowTilemap(WIN_DETAIL);
    CopyWindowToVram(WIN_DETAIL, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_HELP, PIXEL_FILL(0));
    AddTextPrinterParameterized(WIN_HELP, FONT_NARROW, sText_PokeLinkMessageHelp, 0, 1, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_HELP);
    CopyWindowToVram(WIN_HELP, COPYWIN_FULL);
}

static const u8 *GetGloomscrollConfirmText(void)
{
    return GetTimeOfDay() == TIME_NIGHT ? sText_GloomscrollConfirmMorning : sText_GloomscrollConfirmNight;
}

static void StartGloomscrollFeed(u8 taskId)
{
    gTasks[taskId].data[1] = 0;
    DrawGloomscrollFeed(0);
    gTasks[taskId].data[0] = POKELINK_STATE_GLOOM_FEED;
}

static void DrawSightingsList(u8 cursor, u8 top)
{
    u8 i;
    u8 entryId;

    SetPokeLinkSpritesVisible(FALSE);
    FillWindowPixelBuffer(WIN_APPS, PIXEL_FILL(0));
    FillWindowPixelRect(WIN_APPS, PIXEL_FILL(2), 0, 0, 224, 14);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 4, 1, sPokeLinkTextColors, TEXT_SKIP_DRAW, sText_SightingsListHeader);

    for (i = 0; i < POKELINK_SIGHTINGS_VISIBLE_ROWS && top + i < ARRAY_COUNT(sSightingsEntries); i++)
    {
        u8 y = 17 + i * 13;
        u8 status;
        const struct PokeLinkSightingsEntry *entry;

        entryId = top + i;
        entry = &sSightingsEntries[entryId];
        status = GetSightingsStatus(entry);

        if (entryId == cursor)
            AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 2, y, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, gText_SelectorArrow2);

        AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 14, y, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, GetSightingsDisplayName(entry, status));
        AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 91, y, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, entry->region);
        AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 136, y, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, GetSightingsStatusText(status));
    }

    PutWindowTilemap(WIN_APPS);
    CopyWindowToVram(WIN_APPS, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_DETAIL, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WIN_DETAIL, FONT_NORMAL, 0, 1, sPokeLinkTextColors, TEXT_SKIP_DRAW, sText_AppSightings);
    PutWindowTilemap(WIN_DETAIL);
    CopyWindowToVram(WIN_DETAIL, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_HELP, PIXEL_FILL(0));
    AddTextPrinterParameterized(WIN_HELP, FONT_NARROW, sText_SightingsHelpList, 0, 1, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_HELP);
    CopyWindowToVram(WIN_HELP, COPYWIN_FULL);
}

static void DrawSightingsDetail(u8 cursor)
{
    const struct PokeLinkSightingsEntry *entry = &sSightingsEntries[cursor];
    u8 status = GetSightingsStatus(entry);
    const u8 *rumor = status == SIGHTINGS_STATUS_UNKNOWN ? sText_SightingsLockedRumor : entry->rumorText;
    const u8 *whereabouts = status == SIGHTINGS_STATUS_UNKNOWN ? sText_SightingsLockedWhere : entry->whereaboutsText;
    const u8 *conditions = status == SIGHTINGS_STATUS_UNKNOWN ? sText_SightingsUnknownConditions : entry->conditionsText;
    const u8 *notes = entry->fieldNotesText;

    if (status == SIGHTINGS_STATUS_SIGHTED || status == SIGHTINGS_STATUS_LOCATED || status == SIGHTINGS_STATUS_ENCOUNTERED)
        notes = sText_SightingsSeenNotes;
    else if (status == SIGHTINGS_STATUS_CAUGHT)
        notes = sText_SightingsCaughtNotes;

    SetPokeLinkSpritesVisible(FALSE);
    FillWindowPixelBuffer(WIN_APPS, PIXEL_FILL(0));
    FillWindowPixelRect(WIN_APPS, PIXEL_FILL(2), 0, 0, 224, 14);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NORMAL, 4, 0, sPokeLinkTextColors, TEXT_SKIP_DRAW, GetSightingsDisplayName(entry, status));

    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 4, 18, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sText_SightingsStatusLabel);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 53, 18, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, GetSightingsStatusText(status));
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 4, 30, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sText_SightingsRegionLabel);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 53, 30, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, entry->region);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 113, 30, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sText_SightingsClassLabel);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 154, 30, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, entry->classification);

    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 4, 45, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sText_SightingsRumorLabel);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 43, 45, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, rumor);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 4, 60, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sText_SightingsWhereLabel);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 43, 60, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, whereabouts);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 4, 75, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sText_SightingsConditionsLabel);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 43, 75, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, conditions);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 4, 90, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sText_SightingsNotesLabel);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 43, 90, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, notes);

    PutWindowTilemap(WIN_APPS);
    CopyWindowToVram(WIN_APPS, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_DETAIL, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WIN_DETAIL, FONT_NORMAL, 0, 1, sPokeLinkTextColors, TEXT_SKIP_DRAW, sText_AppSightings);
    PutWindowTilemap(WIN_DETAIL);
    CopyWindowToVram(WIN_DETAIL, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_HELP, PIXEL_FILL(0));
    AddTextPrinterParameterized(WIN_HELP, FONT_NARROW, sText_SightingsHelpDetail, 0, 1, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_HELP);
    CopyWindowToVram(WIN_HELP, COPYWIN_FULL);
}

static void MoveSightingsCursor(s16 *cursor, s16 *top, s8 delta)
{
    s16 newCursor = *cursor + delta;

    if (newCursor < 0 || newCursor >= ARRAY_COUNT(sSightingsEntries))
        return;

    *cursor = newCursor;
    if (*cursor < *top)
        *top = *cursor;
    else if (*cursor >= *top + POKELINK_SIGHTINGS_VISIBLE_ROWS)
        *top = *cursor - POKELINK_SIGHTINGS_VISIBLE_ROWS + 1;

    PlaySE(SE_SELECT);
    DrawSightingsList(*cursor, *top);
}

static u8 GetSightingsStatus(const struct PokeLinkSightingsEntry *entry)
{
    u16 nationalDexNum = SpeciesToNationalPokedexNum(entry->species);

    if (GetSetPokedexFlag(nationalDexNum, FLAG_GET_CAUGHT))
        return SIGHTINGS_STATUS_CAUGHT;
    if (entry->encounteredFlag != POKELINK_SIGHTINGS_FLAG_NONE && FlagGet(entry->encounteredFlag))
        return SIGHTINGS_STATUS_ENCOUNTERED;
    if (entry->locatedFlag != POKELINK_SIGHTINGS_FLAG_NONE && FlagGet(entry->locatedFlag))
        return SIGHTINGS_STATUS_LOCATED;
    if (GetSetPokedexFlag(nationalDexNum, FLAG_GET_SEEN))
        return SIGHTINGS_STATUS_SIGHTED;
    if (entry->rumorFlag != POKELINK_SIGHTINGS_FLAG_NONE && FlagGet(entry->rumorFlag))
        return SIGHTINGS_STATUS_RUMORED;
    return SIGHTINGS_STATUS_UNKNOWN;
}

static const u8 *GetSightingsStatusText(u8 status)
{
    static const u8 *const sStatusTexts[] =
    {
        [SIGHTINGS_STATUS_UNKNOWN] = sText_SightingsUnknown,
        [SIGHTINGS_STATUS_RUMORED] = sText_SightingsRumored,
        [SIGHTINGS_STATUS_SIGHTED] = sText_SightingsSighted,
        [SIGHTINGS_STATUS_LOCATED] = sText_SightingsLocated,
        [SIGHTINGS_STATUS_ENCOUNTERED] = sText_SightingsEncountered,
        [SIGHTINGS_STATUS_CAUGHT] = sText_SightingsCaught,
    };

    if (status >= ARRAY_COUNT(sStatusTexts))
        status = SIGHTINGS_STATUS_UNKNOWN;

    return sStatusTexts[status];
}

static const u8 *GetSightingsDisplayName(const struct PokeLinkSightingsEntry *entry, u8 status)
{
    if (status == SIGHTINGS_STATUS_UNKNOWN)
        return sText_SightingsUnknownName;

    return GetSpeciesName(entry->species);
}

static void DrawCollectionList(u8 mode, u8 cursor, u8 top)
{
    u8 i;
    u8 count = mode == COLLECTION_MODE_AREA ? ARRAY_COUNT(sCollectionAreas) : ARRAY_COUNT(sCollectionCategories);

    SetPokeLinkSpritesVisible(FALSE);
    FillWindowPixelBuffer(WIN_APPS, PIXEL_FILL(0));
    FillWindowPixelRect(WIN_APPS, PIXEL_FILL(2), 0, 0, 224, 14);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 4, 1, sPokeLinkTextColors, TEXT_SKIP_DRAW,
        mode == COLLECTION_MODE_AREA ? sText_CollectionListHeaderArea : sText_CollectionListHeaderCategory);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 181, 1, sPokeLinkTextColors, TEXT_SKIP_DRAW,
        mode == COLLECTION_MODE_AREA ? sText_CollectionAreaTab : sText_CollectionCategoryTab);

    for (i = 0; i < POKELINK_COLLECTION_VISIBLE_ROWS && top + i < count; i++)
    {
        u8 id = top + i;
        u8 y = 18 + i * 14;
        u8 found = 0;
        u8 total = 0;
        u8 status;
        const u8 *name;

        if (mode == COLLECTION_MODE_AREA)
        {
            name = sCollectionAreas[id].name;
            GetCollectionAreaProgress(id, &found, &total);
            status = (total != 0 && found == total) ? COLLECTION_STATUS_CLEARED
                   : (found != 0 || (sCollectionAreas[id].visitedFlag != POKELINK_COLLECTION_FLAG_NONE && FlagGet(sCollectionAreas[id].visitedFlag))) ? COLLECTION_STATUS_SURVEYED
                   : COLLECTION_STATUS_UNKNOWN;
        }
        else
        {
            name = sCollectionCategories[id].name;
            GetCollectionCategoryProgress(id, &found, &total);
            status = (total != 0 && found == total) ? COLLECTION_STATUS_CLEARED
                   : found != 0 ? COLLECTION_STATUS_SURVEYED
                   : COLLECTION_STATUS_UNKNOWN;
        }

        if (id == cursor)
            AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 2, y, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, gText_SelectorArrow2);

        AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 14, y, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, name);
        AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 104, y, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, BufferCollectionProgress(found, total));
        AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 146, y, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, GetCollectionStatusText(status));
    }

    PutWindowTilemap(WIN_APPS);
    CopyWindowToVram(WIN_APPS, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_DETAIL, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WIN_DETAIL, FONT_NORMAL, 0, 1, sPokeLinkTextColors, TEXT_SKIP_DRAW, sText_AppCollectionLog);
    PutWindowTilemap(WIN_DETAIL);
    CopyWindowToVram(WIN_DETAIL, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_HELP, PIXEL_FILL(0));
    AddTextPrinterParameterized(WIN_HELP, FONT_NARROW, sText_CollectionHelpList, 0, 1, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_HELP);
    CopyWindowToVram(WIN_HELP, COPYWIN_FULL);
}

static void DrawCollectionDetail(u8 mode, u8 cursor)
{
    u8 found = 0;
    u8 total = 0;
    u8 printed = 0;
    u8 i;
    u8 j;
    u8 y;
    u8 status;
    const u8 *title;

    SetPokeLinkSpritesVisible(FALSE);
    FillWindowPixelBuffer(WIN_APPS, PIXEL_FILL(0));
    FillWindowPixelRect(WIN_APPS, PIXEL_FILL(2), 0, 0, 224, 14);

    if (mode == COLLECTION_MODE_AREA)
    {
        title = sCollectionAreas[cursor].name;
        GetCollectionAreaProgress(cursor, &found, &total);
    }
    else
    {
        title = sCollectionCategories[cursor].name;
        GetCollectionCategoryProgress(cursor, &found, &total);
    }

    status = (total != 0 && found == total) ? COLLECTION_STATUS_CLEARED
           : found != 0 ? COLLECTION_STATUS_SURVEYED
           : COLLECTION_STATUS_UNKNOWN;

    AddTextPrinterParameterized3(WIN_APPS, FONT_NORMAL, 4, 0, sPokeLinkTextColors, TEXT_SKIP_DRAW, title);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 4, 18, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sText_CollectionFoundLabel);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 50, 18, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, BufferCollectionProgress(found, total));
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 103, 18, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sText_CollectionStatusLabel);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 153, 18, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, GetCollectionStatusText(status));

    if (mode == COLLECTION_MODE_AREA)
    {
        for (i = 0; i < ARRAY_COUNT(sCollectionCategories) && printed < 5; i++)
        {
            found = 0;
            total = 0;
            for (j = 0; j < ARRAY_COUNT(sCollectionEntries); j++)
            {
                if (sCollectionEntries[j].area == cursor && sCollectionEntries[j].category == i)
                {
                    total++;
                    if (GetCollectionEntryStatus(&sCollectionEntries[j]) == COLLECTION_STATUS_CLAIMED)
                        found++;
                }
            }

            if (total == 0)
                continue;

            y = 35 + printed * 13;
            AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 8, y, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sCollectionCategories[i].name);
            AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 106, y, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, BufferCollectionProgress(found, total));
            printed++;
        }
    }
    else
    {
        for (i = 0; i < ARRAY_COUNT(sCollectionEntries) && printed < 5; i++)
        {
            const struct PokeLinkCollectionEntry *entry = &sCollectionEntries[i];
            if (entry->category != cursor)
                continue;

            y = 35 + printed * 13;
            status = GetCollectionEntryStatus(entry);
            AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 8, y, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, GetCollectionEntryDisplayName(entry, status));
            AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 92, y, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sCollectionAreas[entry->area].name);
            AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 164, y, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, GetCollectionStatusText(status));
            printed++;
        }
    }

    if (printed == 0)
        AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 8, 40, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sText_CollectionNoNote);

    PutWindowTilemap(WIN_APPS);
    CopyWindowToVram(WIN_APPS, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_DETAIL, PIXEL_FILL(0));
    AddTextPrinterParameterized3(WIN_DETAIL, FONT_NORMAL, 0, 1, sPokeLinkTextColors, TEXT_SKIP_DRAW, sText_AppCollectionLog);
    PutWindowTilemap(WIN_DETAIL);
    CopyWindowToVram(WIN_DETAIL, COPYWIN_FULL);

    FillWindowPixelBuffer(WIN_HELP, PIXEL_FILL(0));
    AddTextPrinterParameterized(WIN_HELP, FONT_NARROW, sText_CollectionHelpDetail, 0, 1, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_HELP);
    CopyWindowToVram(WIN_HELP, COPYWIN_FULL);
}

static void MoveCollectionCursor(s16 *cursor, s16 *top, s8 delta, u8 mode)
{
    s16 count = mode == COLLECTION_MODE_AREA ? ARRAY_COUNT(sCollectionAreas) : ARRAY_COUNT(sCollectionCategories);
    s16 newCursor = *cursor + delta;

    if (newCursor < 0 || newCursor >= count)
        return;

    *cursor = newCursor;
    if (*cursor < *top)
        *top = *cursor;
    else if (*cursor >= *top + POKELINK_COLLECTION_VISIBLE_ROWS)
        *top = *cursor - POKELINK_COLLECTION_VISIBLE_ROWS + 1;

    PlaySE(SE_SELECT);
    DrawCollectionList(mode, *cursor, *top);
}

static u8 ToggleCollectionMode(s16 *cursor, s16 *top, u8 mode)
{
    *cursor = 0;
    *top = 0;
    return mode == COLLECTION_MODE_AREA ? COLLECTION_MODE_CATEGORY : COLLECTION_MODE_AREA;
}

static void GetCollectionAreaProgress(u8 area, u8 *found, u8 *total)
{
    u8 i;

    *found = 0;
    *total = 0;
    for (i = 0; i < ARRAY_COUNT(sCollectionEntries); i++)
    {
        if (sCollectionEntries[i].area != area)
            continue;
        (*total)++;
        if (GetCollectionEntryStatus(&sCollectionEntries[i]) == COLLECTION_STATUS_CLAIMED)
            (*found)++;
    }
}

static void GetCollectionCategoryProgress(u8 category, u8 *found, u8 *total)
{
    u8 i;

    *found = 0;
    *total = 0;
    for (i = 0; i < ARRAY_COUNT(sCollectionEntries); i++)
    {
        if (sCollectionEntries[i].category != category)
            continue;
        (*total)++;
        if (GetCollectionEntryStatus(&sCollectionEntries[i]) == COLLECTION_STATUS_CLAIMED)
            (*found)++;
    }
}

static u8 GetCollectionEntryStatus(const struct PokeLinkCollectionEntry *entry)
{
    if ((entry->obtainedFlag != POKELINK_COLLECTION_FLAG_NONE && FlagGet(entry->obtainedFlag))
     || CheckBagHasItem(entry->itemId, entry->quantity))
        return COLLECTION_STATUS_CLAIMED;
    if (entry->lockedFlag != POKELINK_COLLECTION_FLAG_NONE && FlagGet(entry->lockedFlag))
        return COLLECTION_STATUS_LOCKED;
    if (entry->seenFlag != POKELINK_COLLECTION_FLAG_NONE && FlagGet(entry->seenFlag))
        return COLLECTION_STATUS_SEEN;
    if (entry->visibilityFlag != POKELINK_COLLECTION_FLAG_NONE && FlagGet(entry->visibilityFlag))
        return COLLECTION_STATUS_SURVEYED;
    return COLLECTION_STATUS_UNKNOWN;
}

static const u8 *GetCollectionStatusText(u8 status)
{
    static const u8 *const sStatusTexts[] =
    {
        [COLLECTION_STATUS_UNKNOWN] = sText_CollectionUnknown,
        [COLLECTION_STATUS_SURVEYED] = sText_CollectionSurveyed,
        [COLLECTION_STATUS_SEEN] = sText_CollectionSeen,
        [COLLECTION_STATUS_LOCKED] = sText_CollectionLocked,
        [COLLECTION_STATUS_CLAIMED] = sText_CollectionClaimed,
        [COLLECTION_STATUS_CLEARED] = sText_CollectionCleared,
    };

    if (status >= ARRAY_COUNT(sStatusTexts))
        status = COLLECTION_STATUS_UNKNOWN;

    return sStatusTexts[status];
}

static const u8 *GetCollectionEntryDisplayName(const struct PokeLinkCollectionEntry *entry, u8 status)
{
    if (status == COLLECTION_STATUS_CLAIMED || status == COLLECTION_STATUS_SEEN || status == COLLECTION_STATUS_LOCKED)
        return ItemId_GetName(entry->itemId);

    return sText_CollectionUnknownItem;
}

static u8 *BufferCollectionProgress(u8 found, u8 total)
{
    u8 *str = ConvertIntToDecimalStringN(gStringVar1, found, STR_CONV_MODE_LEFT_ALIGN, 2);
    *str++ = CHAR_SLASH;
    str = ConvertIntToDecimalStringN(str, total, STR_CONV_MODE_LEFT_ALIGN, 2);
    *str = EOS;
    return gStringVar1;
}

static void FillPokeLinkAppsRectClipped(s16 x, s16 y, s16 width, s16 height, u8 color)
{
    s16 right = x + width;
    s16 bottom = y + height;

    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (right > 224)
        right = 224;
    if (bottom > 112)
        bottom = 112;
    if (right <= x || bottom <= y)
        return;

    FillWindowPixelRect(WIN_APPS, PIXEL_FILL(color), x, y, right - x, bottom - y);
}

static void DrawGloomscrollPost(s16 y, u8 accent, const u8 *handle, const u8 *body)
{
    FillPokeLinkAppsRectClipped(10, y, 142, 23, 1);
    FillPokeLinkAppsRectClipped(12, y + 2, 138, 1, 3);
    FillPokeLinkAppsRectClipped(16, y + 5, 12, 12, accent);
    FillPokeLinkAppsRectClipped(20, y + 9, 4, 4, 1);
    if (y >= 0 && y < 92)
    {
        AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 34, y + 2, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, handle);
        AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 34, y + 12, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, body);
        AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, 113, y + 2, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, sText_GloomscrollLikes);
    }
}

static void DrawGloomscrollShort(s16 x, u8 accent, const u8 *caption)
{
    FillPokeLinkAppsRectClipped(x, 25, 46, 68, 2);
    FillPokeLinkAppsRectClipped(x + 3, 28, 40, 62, 1);
    FillPokeLinkAppsRectClipped(x + 7, 32, 32, 34, accent);
    FillPokeLinkAppsRectClipped(x + 20, 43, 7, 10, 1);
    FillPokeLinkAppsRectClipped(x + 17, 46, 13, 5, 1);
    AddTextPrinterParameterized3(WIN_APPS, FONT_NARROW, x + 10, 74, sPokeLinkDarkTextColors, TEXT_SKIP_DRAW, caption);
}

static void MovePokeLinkCursor(s8 xDelta, s8 yDelta)
{
    s16 cursor = sPokeLink->cursor;
    s16 pageStart = (cursor / POKELINK_PAGE_SIZE) * POKELINK_PAGE_SIZE;
    s16 pagePos = cursor - pageStart;
    s16 col = pagePos % POKELINK_GRID_COLS;
    s16 row = pagePos / POKELINK_GRID_COLS;
    s16 newCursor;

    if (xDelta != 0)
    {
        col += xDelta;
        if (col < 0 || col >= POKELINK_GRID_COLS)
            return;
    }

    if (yDelta != 0)
    {
        row += yDelta;
        if (row < 0)
        {
            if (pageStart == 0)
                return;

            newCursor = pageStart - POKELINK_PAGE_SIZE + col;
            if (newCursor >= pageStart)
                newCursor = pageStart - 1;

            sPokeLink->cursor = newCursor;
            sPokeLink->top = (sPokeLink->cursor / POKELINK_PAGE_SIZE) * POKELINK_PAGE_SIZE;
            PlaySE(SE_SELECT);
            DrawPokeLinkApps();
            DrawPokeLinkDetail();
            return;
        }
        else if (row >= POKELINK_GRID_ROWS)
        {
            if (pageStart + POKELINK_PAGE_SIZE >= sPokeLink->visibleCount)
                return;

            newCursor = pageStart + POKELINK_PAGE_SIZE + col;
            if (newCursor >= sPokeLink->visibleCount)
                newCursor = pageStart + POKELINK_PAGE_SIZE;

            sPokeLink->cursor = newCursor;
            sPokeLink->top = (sPokeLink->cursor / POKELINK_PAGE_SIZE) * POKELINK_PAGE_SIZE;
            PlaySE(SE_SELECT);
            DrawPokeLinkApps();
            DrawPokeLinkDetail();
            return;
        }
    }

    newCursor = pageStart + row * POKELINK_GRID_COLS + col;
    if (newCursor >= sPokeLink->visibleCount)
        return;

    sPokeLink->cursor = newCursor;
    sPokeLink->top = (sPokeLink->cursor / POKELINK_PAGE_SIZE) * POKELINK_PAGE_SIZE;
    PlaySE(SE_SELECT);
    DrawPokeLinkApps();
    DrawPokeLinkDetail();
}

static void LoadPokeLinkBackground(void)
{
    LoadPalette(sPokeLinkBgPal, BG_PLTT_ID(0), PLTT_SIZE_4BPP);
    LZ77UnCompVram(sPokeLinkBgGfxLZ, (void *)BG_CHAR_ADDR(2));
    LZ77UnCompVram(sPokeLinkBgTilemapLZ, (void *)BG_SCREEN_ADDR(30));
}

static void LoadPokeLinkGraphics(void)
{
    static const struct SpriteSheet sIconSheet =
    {
        .data = sPokeLinkIconsGfx,
        .size = 11 * POKELINK_ICON_SPRITE_TILE_COUNT * TILE_SIZE_4BPP,
        .tag = TAG_POKELINK_ICONS
    };
    static const struct SpriteSheet sCursorSheet =
    {
        .data = sPokeLinkCursorGfx,
        .size = POKELINK_ICON_SPRITE_TILE_COUNT * TILE_SIZE_4BPP,
        .tag = TAG_POKELINK_CURSOR
    };
    static const struct SpriteSheet sSightingsIconSheet =
    {
        .data = sPokeLinkSightingsIconGfx,
        .size = POKELINK_ICON_SPRITE_TILE_COUNT * TILE_SIZE_4BPP,
        .tag = TAG_POKELINK_SIGHTINGS_ICON
    };
    static const struct SpriteSheet sCollectionIconSheet =
    {
        .data = sPokeLinkCollectionIconGfx,
        .size = POKELINK_ICON_SPRITE_TILE_COUNT * TILE_SIZE_4BPP,
        .tag = TAG_POKELINK_COLLECTION_ICON
    };
    static const struct SpritePalette sAssetsPalette =
    {
        .data = sPokeLinkAssetsPal,
        .tag = TAG_POKELINK_ASSETS
    };
    static const struct SpritePalette sSightingsIconPalette =
    {
        .data = sPokeLinkSightingsIconPal,
        .tag = TAG_POKELINK_SIGHTINGS_ICON_PAL
    };
    static const struct SpritePalette sCollectionIconPalette =
    {
        .data = sPokeLinkCollectionIconPal,
        .tag = TAG_POKELINK_COLLECTION_ICON_PAL
    };

    sPokeLinkIconTileStart = LoadSpriteSheet(&sIconSheet);
    LoadSpriteSheet(&sCursorSheet);
    sPokeLinkSightingsIconTileStart = LoadSpriteSheet(&sSightingsIconSheet);
    sPokeLinkCollectionIconTileStart = LoadSpriteSheet(&sCollectionIconSheet);
    sPokeLinkAssetsPalSlot = LoadSpritePalette(&sAssetsPalette);
    sPokeLinkSightingsIconPalSlot = LoadSpritePalette(&sSightingsIconPalette);
    sPokeLinkCollectionIconPalSlot = LoadSpritePalette(&sCollectionIconPalette);
}

static void CreatePokeLinkSprites(void)
{
    u8 i;

    for (i = 0; i < POKELINK_PAGE_SIZE; i++)
    {
        sPokeLink->iconSpriteIds[i] = CreateSprite(&sPokeLinkIconSpriteTemplate, 0, 0, 1);
        if (sPokeLink->iconSpriteIds[i] != MAX_SPRITES)
            gSprites[sPokeLink->iconSpriteIds[i]].invisible = TRUE;
    }

    sPokeLink->cursorSpriteId = CreateSprite(&sPokeLinkCursorSpriteTemplate, 0, 0, 0);
    if (sPokeLink->cursorSpriteId != MAX_SPRITES)
        gSprites[sPokeLink->cursorSpriteId].invisible = TRUE;
}

static void UpdatePokeLinkSprites(void)
{
    u8 i;
    u8 appId;
    u8 iconIndex;
    s16 x;
    s16 y;

    for (i = 0; i < POKELINK_PAGE_SIZE; i++)
    {
        if (sPokeLink->iconSpriteIds[i] == MAX_SPRITES)
            continue;

        if (sPokeLink->top + i >= sPokeLink->visibleCount)
        {
            gSprites[sPokeLink->iconSpriteIds[i]].invisible = TRUE;
            continue;
        }

        appId = sPokeLink->visibleApps[sPokeLink->top + i];
        iconIndex = sPokeLinkAppIconIndexes[appId];
        if (iconIndex == POKELINK_ICON_NONE)
        {
            gSprites[sPokeLink->iconSpriteIds[i]].invisible = TRUE;
            continue;
        }

        GetPokeLinkAppIconCoords(i, &x, &y);
        gSprites[sPokeLink->iconSpriteIds[i]].x = x;
        gSprites[sPokeLink->iconSpriteIds[i]].y = y;
        if (iconIndex == POKELINK_ICON_SIGHTINGS)
        {
            gSprites[sPokeLink->iconSpriteIds[i]].sheetTileStart = sPokeLinkSightingsIconTileStart;
            gSprites[sPokeLink->iconSpriteIds[i]].oam.paletteNum = sPokeLinkSightingsIconPalSlot;
        }
        else if (iconIndex == POKELINK_ICON_COLLECTION_LOG)
        {
            gSprites[sPokeLink->iconSpriteIds[i]].sheetTileStart = sPokeLinkCollectionIconTileStart;
            gSprites[sPokeLink->iconSpriteIds[i]].oam.paletteNum = sPokeLinkCollectionIconPalSlot;
        }
        else
        {
            gSprites[sPokeLink->iconSpriteIds[i]].sheetTileStart = sPokeLinkIconTileStart;
            gSprites[sPokeLink->iconSpriteIds[i]].oam.paletteNum = sPokeLinkAssetsPalSlot;
        }
        if (gSprites[sPokeLink->iconSpriteIds[i]].animNum != iconIndex)
            StartSpriteAnim(&gSprites[sPokeLink->iconSpriteIds[i]], iconIndex);
        gSprites[sPokeLink->iconSpriteIds[i]].invisible = FALSE;
    }

    if (sPokeLink->cursorSpriteId != MAX_SPRITES)
    {
        GetPokeLinkAppIconCoords(sPokeLink->cursor - sPokeLink->top, &x, &y);
        gSprites[sPokeLink->cursorSpriteId].x = x;
        gSprites[sPokeLink->cursorSpriteId].y = y;
        gSprites[sPokeLink->cursorSpriteId].invisible = FALSE;
    }
}

static void SetPokeLinkSpritesVisible(bool8 visible)
{
    u8 i;

    if (sPokeLink == NULL)
        return;

    for (i = 0; i < POKELINK_PAGE_SIZE; i++)
    {
        if (sPokeLink->iconSpriteIds[i] != MAX_SPRITES)
            gSprites[sPokeLink->iconSpriteIds[i]].invisible = !visible;
    }

    if (sPokeLink->cursorSpriteId != MAX_SPRITES)
        gSprites[sPokeLink->cursorSpriteId].invisible = !visible;
}

static void DestroyPokeLinkSprites(void)
{
    u8 i;

    if (sPokeLink == NULL)
        return;

    for (i = 0; i < POKELINK_PAGE_SIZE; i++)
    {
        if (sPokeLink->iconSpriteIds[i] != MAX_SPRITES)
            DestroySprite(&gSprites[sPokeLink->iconSpriteIds[i]]);
    }

    if (sPokeLink->cursorSpriteId != MAX_SPRITES)
        DestroySprite(&gSprites[sPokeLink->cursorSpriteId]);

    FreeSpriteTilesByTag(TAG_POKELINK_ICONS);
    FreeSpriteTilesByTag(TAG_POKELINK_CURSOR);
    FreeSpriteTilesByTag(TAG_POKELINK_SIGHTINGS_ICON);
    FreeSpriteTilesByTag(TAG_POKELINK_COLLECTION_ICON);
    FreeSpritePaletteByTag(TAG_POKELINK_ASSETS);
    FreeSpritePaletteByTag(TAG_POKELINK_SIGHTINGS_ICON_PAL);
    FreeSpritePaletteByTag(TAG_POKELINK_COLLECTION_ICON_PAL);
}

static void GetPokeLinkAppIconCoords(u8 pagePos, s16 *x, s16 *y)
{
    *x = 43 + (pagePos % POKELINK_GRID_COLS) * POKELINK_TILE_WIDTH;
    *y = 50 + (pagePos / POKELINK_GRID_COLS) * POKELINK_TILE_HEIGHT;
}

static void ToggleFavorite(u8 appId)
{
    u8 i;
    u16 *varPtr;

    if (!sPokeLinkApps[appId].canFavorite)
    {
        PlaySE(SE_FAILURE);
        DrawPokeLinkMessageText(sText_PokeLinkCannotFavorite);
        return;
    }

    for (i = 0; i < POKELINK_FAVORITE_COUNT; i++)
    {
        varPtr = GetFavoriteVarPtr(i);
        if (varPtr != NULL && *varPtr == appId + 1)
        {
            *varPtr = 0;
            PlaySE(SE_SELECT);
            DrawPokeLinkMessageText(sText_PokeLinkFavoriteCleared);
            return;
        }
    }

    for (i = 0; i < POKELINK_FAVORITE_COUNT; i++)
    {
        varPtr = GetFavoriteVarPtr(i);
        if (varPtr != NULL && !IsFavoriteSlotValid(*varPtr))
        {
            *varPtr = appId + 1;
            PlaySE(SE_SELECT);
            DrawPokeLinkMessageText(sText_PokeLinkFavoriteSet);
            return;
        }
    }

    varPtr = GetFavoriteVarPtr(0);
    if (varPtr != NULL)
        *varPtr = appId + 1;
    PlaySE(SE_SELECT);
    DrawPokeLinkMessageText(sText_PokeLinkFavoriteSet);
}

static void DrawPokeLinkMessageText(const u8 *text)
{
    FillWindowPixelBuffer(WIN_HELP, PIXEL_FILL(0));
    AddTextPrinterParameterized(WIN_HELP, FONT_NARROW, text, 0, 1, TEXT_SKIP_DRAW, NULL);
    PutWindowTilemap(WIN_HELP);
    CopyWindowToVram(WIN_HELP, COPYWIN_FULL);
}

static bool8 IsAppAvailable(u8 appId)
{
    if (appId >= POKELINK_APP_COUNT)
        return FALSE;

    if (POKELINK_TEST_UNLOCK_ALL == TRUE)
        return TRUE;

    if (sPokeLinkApps[appId].availableByDefault)
        return TRUE;

    return FlagGet(sPokeLinkApps[appId].unlockFlag);
}

static bool8 IsFavoriteSlotValid(u16 value)
{
    u8 appId;

    if (value == 0)
        return FALSE;

    appId = value - 1;
    return appId < POKELINK_APP_COUNT && sPokeLinkApps[appId].canFavorite && IsAppAvailable(appId);
}

static u16 *GetFavoriteVarPtr(u8 slot)
{
    switch (slot)
    {
    case 0:
        return GetVarPointer(VAR_POKELINK_FAVORITE_1);
    case 1:
        return GetVarPointer(VAR_POKELINK_FAVORITE_2);
    case 2:
        return GetVarPointer(VAR_POKELINK_FAVORITE_3);
    default:
        return NULL;
    }
}

static u8 GetDefaultShortcutAppId(u8 slot)
{
    u8 i;
    u8 availableIndex = 0;

    for (i = 0; i < ARRAY_COUNT(sDefaultShortcutApps); i++)
    {
        u8 appId = sDefaultShortcutApps[i];
        if (IsAppAvailable(appId) && sPokeLinkApps[appId].canFavorite)
        {
            if (availableIndex == slot)
                return appId;
            availableIndex++;
        }
    }

    return POKELINK_SHORTCUT_NONE;
}

static void CleanupPokeLink(void)
{
    SetVBlankCallback(NULL);
    DestroyPokeLinkSprites();
    FreeAllWindowBuffers();
    FREE_AND_SET_NULL(sPokeLink);
}

static void OpenSelectedApp(u8 taskId, u8 appId)
{
    if (appId == POKELINK_APP_MAP
     || appId == POKELINK_APP_PROFILE
     || appId == POKELINK_APP_RADIO
     || appId == POKELINK_APP_DELIVERY
     || appId == POKELINK_APP_ABRACAB)
    {
        PlaySE(SE_SELECT);
        sPokeLink->launchAppId = appId;
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].data[0] = POKELINK_STATE_LAUNCH;
    }
    else if (appId == POKELINK_APP_GLOOMSCROLL)
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].data[2] = 0;
        DrawGloomscrollConfirm(0);
        gTasks[taskId].data[0] = POKELINK_STATE_GLOOM_CONFIRM;
    }
    else if (appId == POKELINK_APP_SIGHTINGS)
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].data[3] = 0;
        gTasks[taskId].data[4] = 0;
        DrawSightingsList(0, 0);
        gTasks[taskId].data[0] = POKELINK_STATE_SIGHTINGS_LIST;
    }
    else if (appId == POKELINK_APP_COLLECTION_LOG)
    {
        PlaySE(SE_SELECT);
        gTasks[taskId].data[5] = COLLECTION_MODE_AREA;
        gTasks[taskId].data[6] = 0;
        gTasks[taskId].data[7] = 0;
        DrawCollectionList(COLLECTION_MODE_AREA, 0, 0);
        gTasks[taskId].data[0] = POKELINK_STATE_COLLECTION_LIST;
    }
    else
    {
        PlaySE(SE_SELECT);
        DrawPokeLinkMessage(appId);
        gTasks[taskId].data[0] = POKELINK_STATE_MESSAGE;
    }
}

static u8 CountBadges(void)
{
    u8 i;
    u8 count = 0;

    for (i = 0; i < NUM_BADGES; i++)
    {
        if (FlagGet(gBadgeFlags[i]))
            count++;
    }

    return count;
}

static void FieldCallback_OpenDelibirdDelivery(void)
{
    LockPlayerFieldControls();
    FadeInFromBlack();
    CreateTask(Task_OpenDelibirdDeliveryAfterReturn, 0);
    gFieldCallback = NULL;
}

static void Task_OpenDelibirdDeliveryAfterReturn(u8 taskId)
{
    if (IsWeatherNotFadingIn() == TRUE)
    {
        CreateDelibirdDeliveryMenu(GetDelibirdDeliveryInventory(), DelibirdDeliveryReturnCallback);
        DestroyTask(taskId);
    }
}

static const u16 *GetDelibirdDeliveryInventory(void)
{
    u8 badges = CountBadges();

    if (badges >= 6)
        return sDelibirdDeliveryLateItems;
    if (badges >= 3)
        return sDelibirdDeliveryMidItems;
    return sDelibirdDeliveryBasicItems;
}

static void DelibirdDeliveryReturnCallback(void)
{
    LockPlayerFieldControls();
    FreezeObjectEvents();
    CreateTask(Task_DelibirdDeliveryArrives, 0);
}

static void Task_DelibirdDeliveryArrives(u8 taskId)
{
    if (gMartPurchaseHistory[0].itemId == ITEM_NONE)
    {
        DisplayItemMessageOnField(taskId, sText_DeliveryNoOrder, Task_DelibirdDeliveryFinish);
    }
    else
    {
        sDelibirdDeliveryMoveStep = 0;
        sDelibirdDeliverySurfing = PlayerIsOnSurfableWater();
        DisplayItemMessageOnField(taskId, sDelibirdDeliverySurfing ? sText_DeliverySurfer : sText_DeliveryWalker, Task_DelibirdDeliveryStartCourier);
    }
}

static void Task_DelibirdDeliveryStartCourier(u8 taskId)
{
    ClearDialogWindowAndFrame(0, TRUE);

    if (SpawnDelibirdDeliveryCourier() == TRUE)
        gTasks[taskId].func = Task_DelibirdDeliveryMoveCourier;
    else
        Task_DelibirdDeliveryAskTip(taskId);
}

static void Task_DelibirdDeliveryMoveCourier(u8 taskId)
{
    struct ObjectEvent *courier;

    if (!sDelibirdDeliveryObjectActive || !gObjectEvents[sDelibirdDeliveryObjectEventId].active)
    {
        Task_DelibirdDeliveryAskTip(taskId);
        return;
    }

    courier = &gObjectEvents[sDelibirdDeliveryObjectEventId];

    if (!ObjectEventIsMovementOverridden(courier))
    {
        if (sDelibirdDeliveryMoveStep < sDelibirdDeliveryPathLength)
        {
            if (!ObjectEventSetHeldMovement(courier, GetDelibirdDeliveryMoveAction(sDelibirdDeliveryPath[sDelibirdDeliveryMoveStep])))
                sDelibirdDeliveryMoveStep++;
        }
        else
        {
            ObjectEventSetHeldMovement(courier, sDelibirdDeliveryFacePlayerAction);
            PlayerFaceDirection(sDelibirdDeliveryFaceCourierDirection);
            Task_DelibirdDeliveryAskTip(taskId);
        }
    }
    else
    {
        ObjectEventClearHeldMovementIfFinished(courier);
    }
}

static void Task_DelibirdDeliveryAskTip(u8 taskId)
{
    StringExpandPlaceholders(gStringVar4, sText_DeliveryAskPlayer);
    DisplayItemMessageOnField(taskId, gStringVar4, Task_DelibirdDeliveryPromptTip);
}

static void Task_DelibirdDeliveryPromptTip(u8 taskId)
{
    DisplayItemMessageOnField(taskId, sText_DeliveryAskTip, Task_DelibirdDeliveryShowTipMenu);
}

static void Task_DelibirdDeliveryShowTipMenu(u8 taskId)
{
    static const struct WindowTemplate sTipWindowTemplate =
    {
        .bg = 0,
        .tilemapLeft = 20,
        .tilemapTop = 7,
        .width = 8,
        .height = 6,
        .paletteNum = 15,
        .baseBlock = 0x125
    };

    ClearDialogWindowAndFrame(0, TRUE);
    sDelibirdTipCursor = 0;
    sDelibirdTipWindowId = AddWindow(&sTipWindowTemplate);
    DrawStdWindowFrame(sDelibirdTipWindowId, FALSE);
    AddTextPrinterParameterized(sDelibirdTipWindowId, FONT_NORMAL, sText_TipNone, 8, 1, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(sDelibirdTipWindowId, FONT_NORMAL, sText_TipSmall, 8, 17, TEXT_SKIP_DRAW, NULL);
    AddTextPrinterParameterized(sDelibirdTipWindowId, FONT_NORMAL, sText_TipBig, 8, 33, TEXT_SKIP_DRAW, NULL);
    InitMenuNormal(sDelibirdTipWindowId, FONT_NORMAL, 0, 1, 16, 3, sDelibirdTipCursor);
    PutWindowTilemap(sDelibirdTipWindowId);
    CopyWindowToVram(sDelibirdTipWindowId, COPYWIN_FULL);
    gTasks[taskId].func = Task_DelibirdDeliveryProcessTipMenu;
}

static void Task_DelibirdDeliveryProcessTipMenu(u8 taskId)
{
    if (JOY_NEW(DPAD_UP))
    {
        PlaySE(SE_SELECT);
        sDelibirdTipCursor = Menu_MoveCursor(-1);
    }
    else if (JOY_NEW(DPAD_DOWN))
    {
        PlaySE(SE_SELECT);
        sDelibirdTipCursor = Menu_MoveCursor(1);
    }
    else if (JOY_NEW(A_BUTTON))
    {
        static const u16 sTipAmounts[] = {0, 100, 500};
        u16 tip = sTipAmounts[sDelibirdTipCursor];

        PlaySE(SE_SELECT);
        ClearStdWindowAndFrame(sDelibirdTipWindowId, TRUE);
        RemoveWindow(sDelibirdTipWindowId);

        if (tip != 0)
        {
            if (IsEnoughMoney(&gSaveBlock1Ptr->money, tip))
                RemoveMoney(&gSaveBlock1Ptr->money, tip);
            else
                tip = 0;
        }

        if (tip == 0 && (Random() & 3) == 0)
        {
            DisplayItemMessageOnField(taskId, sText_DeliverySquelched, Task_DelibirdDeliveryFinish);
        }
        else
        {
            AddDelibirdDeliveryItems();
            DisplayItemMessageOnField(taskId, tip == 0 ? sText_DeliveryNoTipThanks : sText_DeliveryThanks, Task_DelibirdDeliveryFinish);
        }
    }
}

static void Task_DelibirdDeliveryFinish(u8 taskId)
{
    if (JOY_NEW(A_BUTTON | B_BUTTON))
    {
        ClearDialogWindowAndFrame(0, TRUE);

        if (sDelibirdDeliveryObjectActive && sDelibirdDeliveryPathLength != 0)
        {
            sDelibirdDeliveryMoveStep = 0;
            gTasks[taskId].func = Task_DelibirdDeliveryMoveCourierAway;
        }
        else
        {
            FinishDelibirdDeliveryTask(taskId);
        }
    }
}

static void Task_DelibirdDeliveryMoveCourierAway(u8 taskId)
{
    struct ObjectEvent *courier;
    u8 pathIndex;

    if (!sDelibirdDeliveryObjectActive || !gObjectEvents[sDelibirdDeliveryObjectEventId].active)
    {
        FinishDelibirdDeliveryTask(taskId);
        return;
    }

    courier = &gObjectEvents[sDelibirdDeliveryObjectEventId];

    if (!ObjectEventIsMovementOverridden(courier))
    {
        if (sDelibirdDeliveryMoveStep < sDelibirdDeliveryPathLength)
        {
            pathIndex = sDelibirdDeliveryPathLength - 1 - sDelibirdDeliveryMoveStep;
            if (!ObjectEventSetHeldMovement(courier, GetDelibirdDeliveryMoveAction(GetOppositeDirection(sDelibirdDeliveryPath[pathIndex]))))
                sDelibirdDeliveryMoveStep++;
        }
        else
        {
            FinishDelibirdDeliveryTask(taskId);
        }
    }
    else
    {
        ObjectEventClearHeldMovementIfFinished(courier);
    }
}

static void AddDelibirdDeliveryItems(void)
{
    u8 i;

    for (i = 0; i < ARRAY_COUNT(gMartPurchaseHistory); i++)
    {
        if (gMartPurchaseHistory[i].itemId != ITEM_NONE && gMartPurchaseHistory[i].quantity != 0)
            AddBagItem(gMartPurchaseHistory[i].itemId, gMartPurchaseHistory[i].quantity);
    }
}

static bool8 SpawnDelibirdDeliveryCourier(void)
{
    s16 x;
    s16 y;
    u16 graphicsId;
    u8 elevation;

    elevation = PlayerGetElevation();
    if (elevation == 0)
        elevation = 3;

    RemoveDelibirdDeliveryCourier();

    if (BuildDelibirdDeliveryPath(&x, &y, elevation) == FALSE)
        return FALSE;

    graphicsId = sDelibirdDeliverySurfing ? OBJ_EVENT_GFX_FRLG_SWIMMER_M_WATER : OBJ_EVENT_GFX_MAN_2;

    sDelibirdDeliveryObjectEventId = SpawnSpecialObjectEventParameterized(
        graphicsId,
        MOVEMENT_TYPE_FACE_DOWN,
        POKELINK_DELIVERY_LOCAL_ID,
        x,
        y,
        elevation);

    sDelibirdDeliveryObjectActive = sDelibirdDeliveryObjectEventId < OBJECT_EVENTS_COUNT;
    if (sDelibirdDeliveryObjectActive)
    {
        gObjectEvents[sDelibirdDeliveryObjectEventId].invisible = FALSE;
        gSprites[gObjectEvents[sDelibirdDeliveryObjectEventId].spriteId].invisible = FALSE;
    }

    return sDelibirdDeliveryObjectActive;
}

static void RemoveDelibirdDeliveryCourier(void)
{
    RemoveObjectEventByLocalIdAndMap(POKELINK_DELIVERY_LOCAL_ID, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    sDelibirdDeliveryObjectActive = FALSE;
    sDelibirdDeliveryObjectEventId = 0;
}

static bool8 BuildDelibirdDeliveryPath(s16 *spawnX, s16 *spawnY, u8 elevation)
{
    static const u8 sDestinationDirections[] = {DIR_NORTH, DIR_SOUTH, DIR_WEST, DIR_EAST};
    static const u8 sSearchDirections[] = {DIR_NORTH, DIR_SOUTH, DIR_WEST, DIR_EAST};
    u8 visited[POKELINK_DELIVERY_PATH_DIAMETER][POKELINK_DELIVERY_PATH_DIAMETER] = {0};
    u8 pathDirection[POKELINK_DELIVERY_PATH_DIAMETER][POKELINK_DELIVERY_PATH_DIAMETER] = {0};
    u8 distance[POKELINK_DELIVERY_PATH_DIAMETER][POKELINK_DELIVERY_PATH_DIAMETER] = {0};
    s16 queueX[POKELINK_DELIVERY_PATH_DIAMETER * POKELINK_DELIVERY_PATH_DIAMETER];
    s16 queueY[POKELINK_DELIVERY_PATH_DIAMETER * POKELINK_DELIVERY_PATH_DIAMETER];
    s16 playerX;
    s16 playerY;
    s16 destX;
    s16 destY;
    s16 currentX;
    s16 currentY;
    s16 nextX;
    s16 nextY;
    s16 bestX;
    s16 bestY;
    s16 centerX;
    s16 centerY;
    u8 bestDistance;
    u8 direction;
    u8 i;
    u8 localX;
    u8 localY;
    u8 nextLocalX;
    u8 nextLocalY;
    u16 head;
    u16 tail;

    PlayerGetDestCoords(&playerX, &playerY);
    centerX = playerX - POKELINK_DELIVERY_PATH_RADIUS;
    centerY = playerY - POKELINK_DELIVERY_PATH_RADIUS;

    for (i = 0; i < ARRAY_COUNT(sDestinationDirections); i++)
    {
        destX = playerX;
        destY = playerY;
        MoveCoords(sDestinationDirections[i], &destX, &destY);

        if (!IsDelibirdDeliveryTileUsable(destX, destY, elevation))
            continue;

        memset(visited, 0, sizeof(visited));
        memset(pathDirection, 0, sizeof(pathDirection));
        memset(distance, 0, sizeof(distance));

        head = 0;
        tail = 0;
        localX = destX - centerX;
        localY = destY - centerY;
        visited[localY][localX] = TRUE;
        queueX[tail] = destX;
        queueY[tail] = destY;
        tail++;
        bestX = destX;
        bestY = destY;
        bestDistance = 0;

        while (head < tail)
        {
            currentX = queueX[head];
            currentY = queueY[head];
            head++;

            localX = currentX - centerX;
            localY = currentY - centerY;
            if (distance[localY][localX] > bestDistance)
            {
                bestX = currentX;
                bestY = currentY;
                bestDistance = distance[localY][localX];
            }

            for (direction = 0; direction < ARRAY_COUNT(sSearchDirections); direction++)
            {
                nextX = currentX;
                nextY = currentY;
                MoveCoords(sSearchDirections[direction], &nextX, &nextY);

                if (!IsDelibirdDeliveryTileInSearchArea(nextX, nextY, playerX, playerY)
                 || !IsDelibirdDeliveryTileUsable(nextX, nextY, elevation))
                    continue;

                nextLocalX = nextX - centerX;
                nextLocalY = nextY - centerY;
                if (visited[nextLocalY][nextLocalX])
                    continue;

                visited[nextLocalY][nextLocalX] = TRUE;
                pathDirection[nextLocalY][nextLocalX] = GetOppositeDirection(sSearchDirections[direction]);
                distance[nextLocalY][nextLocalX] = distance[localY][localX] + 1;
                queueX[tail] = nextX;
                queueY[tail] = nextY;
                tail++;
            }
        }

        if (bestDistance == 0 || bestDistance > POKELINK_DELIVERY_MAX_PATH_STEPS)
            continue;

        sDelibirdDeliveryPathLength = 0;
        currentX = bestX;
        currentY = bestY;

        while ((currentX != destX || currentY != destY) && sDelibirdDeliveryPathLength < POKELINK_DELIVERY_MAX_PATH_STEPS)
        {
            localX = currentX - centerX;
            localY = currentY - centerY;
            direction = pathDirection[localY][localX];
            if (direction == DIR_NONE)
                break;

            sDelibirdDeliveryPath[sDelibirdDeliveryPathLength] = direction;
            sDelibirdDeliveryPathLength++;
            MoveCoords(direction, &currentX, &currentY);
        }

        if (currentX == destX && currentY == destY && sDelibirdDeliveryPathLength != 0)
        {
            *spawnX = bestX;
            *spawnY = bestY;
            sDelibirdDeliveryFaceCourierDirection = sDestinationDirections[i];
            sDelibirdDeliveryFacePlayerAction = GetFaceDirectionMovementAction(GetOppositeDirection(sDestinationDirections[i]));
            return TRUE;
        }
    }

    sDelibirdDeliveryPathLength = 0;
    return FALSE;
}

static bool8 IsDelibirdDeliveryTileUsable(s16 x, s16 y, u8 elevation)
{
    bool8 isWater = MetatileBehavior_IsSurfableWaterOrUnderwater(MapGridGetMetatileBehaviorAt(x, y));

    if (GetMapBorderIdAt(x, y) == CONNECTION_INVALID
     || MapGridGetCollisionAt(x, y) != 0
     || IsElevationMismatchAt(elevation, x, y)
     || GetObjectEventIdByPosition(x, y, elevation) != OBJECT_EVENTS_COUNT)
        return FALSE;

    if (sDelibirdDeliverySurfing)
        return isWater;

    return !isWater;
}

static bool8 IsDelibirdDeliveryTileInSearchArea(s16 x, s16 y, s16 centerX, s16 centerY)
{
    return x >= centerX - POKELINK_DELIVERY_PATH_RADIUS
        && x <= centerX + POKELINK_DELIVERY_PATH_RADIUS
        && y >= centerY - POKELINK_DELIVERY_PATH_RADIUS
        && y <= centerY + POKELINK_DELIVERY_PATH_RADIUS;
}

static u8 GetDelibirdDeliveryMoveAction(u8 direction)
{
    if (sDelibirdDeliverySurfing)
        return GetWalkFastMovementAction(direction);

    return GetWalkNormalMovementAction(direction);
}

static void FinishDelibirdDeliveryTask(u8 taskId)
{
    RemoveDelibirdDeliveryCourier();
    ScriptMovement_UnfreezeObjectEvents();
    UnfreezeObjectEvents();
    UnlockPlayerFieldControls();
    DestroyTask(taskId);
}

static bool8 PlayerIsOnSurfableWater(void)
{
    s16 x;
    s16 y;

    PlayerGetDestCoords(&x, &y);
    return MetatileBehavior_IsSurfableWaterOrUnderwater(MapGridGetMetatileBehaviorAt(x, y));
}
