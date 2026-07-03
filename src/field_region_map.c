#include "global.h"
#include "bg.h"
#include "event_data.h"
#include "field_effect.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "item_menu.h"
#include "main.h"
#include "malloc.h"
#include "menu.h"
#include "money.h"
#include "overworld.h"
#include "palette.h"
#include "region_map.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "text.h"
#include "text_window.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/maps.h"
#include "constants/songs.h"

/*
 *  This is the type of map shown when interacting with the metatiles for
 *  a wall-mounted Region Map (on the wall of the Pokemon Centers near the PC)
 *  It does not zoom, and pressing A or B closes the map
 *
 *  For the region map in the pokenav, see pokenav_region_map.c
 *  For the region map in the pokedex, see pokdex_area_screen.c/pokedex_area_region_map.c
 *  For the fly map, and utility functions all of the maps use, see region_map.c
 */

enum {
    WIN_MAPSEC_NAME,
    WIN_TITLE,
};

enum {
    TAG_PLAYER_ICON,
    TAG_CURSOR,
};

static EWRAM_DATA struct {
    MainCallback callback;
    u32 unused;
    struct RegionMap regionMap;
    u16 state;
    bool8 abraCabMode;
} *sFieldRegionMapHandler = NULL;

static void MCB2_InitRegionMapRegisters(void);
static void VBCB_FieldUpdateRegionMap(void);
static void MCB2_FieldUpdateRegionMap(void);
static void FieldUpdateRegionMap(void);
static void PrintRegionMapSecName();
static void PrintTitleWindowText();
static u32 GetAbraCabFare(void);
static bool8 CanUseAbraCabOnCurrentSelection(void);
static void ReturnToFieldFromAbraCabMapSelect(void);
static void FieldCallback_UseAbraCab(void);
static void Task_UseAbraCab(u8 taskId);

static const u8 sText_AbraCabArrival[] = _("An AbraCab psychic appears!\pGo, ABRA! Use TELEPORT!");

static const struct BgTemplate sFieldRegionMapBgTemplates[] = {
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    }, {
        .bg = 2,
        .charBaseIndex = 2,
        .mapBaseIndex = 28,
        .screenSize = 2,
        .paletteMode = 1,
        .priority = 2,
        .baseTile = 0
    }
};

static const struct WindowTemplate sFieldRegionMapWindowTemplates[] =
{
    [WIN_MAPSEC_NAME] = {
        .bg = 0,
        .tilemapLeft = 17,
        .tilemapTop = 17,
        .width = 12,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 1
    },
    [WIN_TITLE] = {
        .bg = 0,
        .tilemapLeft = 19,
        .tilemapTop = 1,
        .width = 10,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 25
    },
    DUMMY_WIN_TEMPLATE
};

void FieldInitRegionMap(MainCallback callback)
{
    SetVBlankCallback(NULL);
    sFieldRegionMapHandler = Alloc(sizeof(*sFieldRegionMapHandler));
    sFieldRegionMapHandler->state = 0;
    sFieldRegionMapHandler->abraCabMode = FALSE;
    sFieldRegionMapHandler->callback = callback;
    SetMainCallback2(MCB2_InitRegionMapRegisters);
}

void FieldInitAbraCabMap(MainCallback callback)
{
    SetVBlankCallback(NULL);
    sFieldRegionMapHandler = Alloc(sizeof(*sFieldRegionMapHandler));
    sFieldRegionMapHandler->state = 0;
    sFieldRegionMapHandler->abraCabMode = TRUE;
    sFieldRegionMapHandler->callback = callback;
    SetMainCallback2(MCB2_InitRegionMapRegisters);
}

static void MCB2_InitRegionMapRegisters(void)
{
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    SetGpuReg(REG_OFFSET_BG2HOFS, 0);
    SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    SetGpuReg(REG_OFFSET_BG3HOFS, 0);
    SetGpuReg(REG_OFFSET_BG3VOFS, 0);
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(1, sFieldRegionMapBgTemplates, ARRAY_COUNT(sFieldRegionMapBgTemplates));
    InitWindows(sFieldRegionMapWindowTemplates);
    DeactivateAllTextPrinters();
    LoadUserWindowBorderGfx(0, 0x27, BG_PLTT_ID(13));
    ClearScheduledBgCopiesToVram();
    SetMainCallback2(MCB2_FieldUpdateRegionMap);
    SetVBlankCallback(VBCB_FieldUpdateRegionMap);
}

static void VBCB_FieldUpdateRegionMap(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void MCB2_FieldUpdateRegionMap(void)
{
    FieldUpdateRegionMap();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
    DoScheduledBgTilemapCopiesToVram();
}

static void FieldUpdateRegionMap(void)
{
    switch (sFieldRegionMapHandler->state)
    {
        case 0:
            InitRegionMap(&sFieldRegionMapHandler->regionMap, FALSE);
            CreateRegionMapPlayerIcon(TAG_PLAYER_ICON, TAG_PLAYER_ICON);
            CreateRegionMapCursor(TAG_CURSOR, TAG_CURSOR);
            sFieldRegionMapHandler->state++;
            break;
        case 1:
            DrawStdFrameWithCustomTileAndPalette(WIN_TITLE, FALSE, 0x27, 0xd);
            FillWindowPixelBuffer(WIN_TITLE, PIXEL_FILL(1));
            PrintTitleWindowText();
            ScheduleBgCopyTilemapToVram(0);
            DrawStdFrameWithCustomTileAndPalette(WIN_MAPSEC_NAME, FALSE, 0x27, 0xd);
            PrintRegionMapSecName();
            BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
            sFieldRegionMapHandler->state++;
            break;
        case 2:
            SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
            ShowBg(0);
            ShowBg(2);
            sFieldRegionMapHandler->state++;
            break;
        case 3:
            if (!gPaletteFade.active)
            {
                sFieldRegionMapHandler->state++;
            }
            break;
        case 4:
            switch (DoRegionMapInputCallback())
            {
                case MAP_INPUT_MOVE_END:
                case MAP_INPUT_L_BUTTON:
                    PrintRegionMapSecName();
                    PrintTitleWindowText();
                    break;
                case MAP_INPUT_A_BUTTON:
                case MAP_INPUT_B_BUTTON:
                    sFieldRegionMapHandler->state++;
                    break;
                case MAP_INPUT_R_BUTTON:
                    if (sFieldRegionMapHandler->abraCabMode && CanUseAbraCabOnCurrentSelection())
                    {
                        u32 fare = GetAbraCabFare();

                        if (!IsEnoughMoney(&gSaveBlock1Ptr->money, fare))
                        {
                            PlaySE(SE_FAILURE);
                        }
                        else
                        {
                            PlaySE(SE_SELECT);
                            RemoveMoney(&gSaveBlock1Ptr->money, fare);
                            SetFlyDestination(&sFieldRegionMapHandler->regionMap);
                            gSkipShowMonAnim = TRUE;
                            ReturnToFieldFromAbraCabMapSelect();
                        }
                    }
                    else if (!sFieldRegionMapHandler->abraCabMode
                        && sFieldRegionMapHandler->regionMap.mapSecType == MAPSECTYPE_CITY_CANFLY
                        && FlagGet(OW_FLAG_POKE_RIDER) && Overworld_MapTypeAllowsTeleportAndFly(gMapHeader.mapType) == TRUE)
                    {
                        PlaySE(SE_SELECT);
                        SetFlyDestination(&sFieldRegionMapHandler->regionMap);
                        gSkipShowMonAnim = TRUE;
                        ReturnToFieldFromFlyMapSelect();
                    }
            }
            break;
        case 5:
            BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
            sFieldRegionMapHandler->state++;
            break;
        case 6:
            if (!gPaletteFade.active)
            {
                FreeRegionMapIconResources();
                SetMainCallback2(sFieldRegionMapHandler->callback);
                TRY_FREE_AND_SET_NULL(sFieldRegionMapHandler);
                FreeAllWindowBuffers();
            }
            break;
    }
}

static void ReturnToFieldFromAbraCabMapSelect(void)
{
    SetMainCallback2(CB2_ReturnToField);
    gFieldCallback = FieldCallback_UseAbraCab;
}

static void FieldCallback_UseAbraCab(void)
{
    FadeInFromBlack();
    CreateTask(Task_UseAbraCab, 0);
    gFieldCallback = NULL;
}

static void Task_UseAbraCab(u8 taskId)
{
    switch (gTasks[taskId].data[0])
    {
    case 0:
        if (!IsWeatherNotFadingIn())
            return;
        DisplayItemMessageOnField(taskId, sText_AbraCabArrival, Task_UseAbraCab);
        gTasks[taskId].data[0]++;
        break;
    case 1:
        ClearDialogWindowAndFrame(0, TRUE);
        DestroyTask(taskId);
        FieldCallback_UseFly();
        break;
    }
}

static void PrintRegionMapSecName(void)
{
    if (sFieldRegionMapHandler->regionMap.mapSecType != MAPSECTYPE_NONE)
    {
        FillWindowPixelBuffer(WIN_MAPSEC_NAME, PIXEL_FILL(1));
        AddTextPrinterParameterized(WIN_MAPSEC_NAME, FONT_NORMAL, sFieldRegionMapHandler->regionMap.mapSecName, 0, 1, 0, NULL);
        ScheduleBgCopyTilemapToVram(0);
    }
    else
    {
        FillWindowPixelBuffer(WIN_MAPSEC_NAME, PIXEL_FILL(1));
        CopyWindowToVram(WIN_MAPSEC_NAME, COPYWIN_FULL);
    }
}

static void PrintTitleWindowText(void)
{
    static const u8 FlyPromptText[] = _("{R_BUTTON} FLY");
    static const u8 AbraCabPromptText[] = _("{R_BUTTON} CAB");
    const u8 *mapPageName = GetRegionMapPageName(&sFieldRegionMapHandler->regionMap);
    u32 mapPageOffset = GetStringCenterAlignXOffset(FONT_NORMAL, mapPageName, 0x38);
    u32 flyOffset = GetStringCenterAlignXOffset(FONT_NORMAL, FlyPromptText, 0x38);

    FillWindowPixelBuffer(WIN_TITLE, PIXEL_FILL(1));

    if (sFieldRegionMapHandler->abraCabMode && CanUseAbraCabOnCurrentSelection())
    {
        ConvertIntToDecimalStringN(gStringVar1, GetAbraCabFare(), STR_CONV_MODE_LEFT_ALIGN, 6);
        StringExpandPlaceholders(gStringVar4, gText_PokedollarVar1);
        AddTextPrinterParameterized(WIN_TITLE, FONT_NORMAL, AbraCabPromptText, 2, 1, 0, NULL);
        AddTextPrinterParameterized(WIN_TITLE, FONT_NARROW, gStringVar4, 36, 2, 0, NULL);
        ScheduleBgCopyTilemapToVram(WIN_TITLE);
    }
    else if (!sFieldRegionMapHandler->abraCabMode
        && sFieldRegionMapHandler->regionMap.mapSecType == MAPSECTYPE_CITY_CANFLY
        && FlagGet(OW_FLAG_POKE_RIDER) && Overworld_MapTypeAllowsTeleportAndFly(gMapHeader.mapType) == TRUE)
    {
        AddTextPrinterParameterized(WIN_TITLE, FONT_NORMAL, FlyPromptText, flyOffset, 1, 0, NULL);
        ScheduleBgCopyTilemapToVram(WIN_TITLE);
    }
    else
    {
        AddTextPrinterParameterized(WIN_TITLE, FONT_NORMAL, mapPageName, mapPageOffset, 1, 0, NULL);
        CopyWindowToVram(WIN_TITLE, COPYWIN_FULL);
    }
}

static bool8 CanUseAbraCabOnCurrentSelection(void)
{
    return sFieldRegionMapHandler->regionMap.mapSecType == MAPSECTYPE_CITY_CANFLY
        && Overworld_MapTypeAllowsTeleportAndFly(gMapHeader.mapType) == TRUE
        && FilterFlyDestination(&sFieldRegionMapHandler->regionMap) != WARP_ID_NONE;
}

static u32 GetAbraCabFare(void)
{
    s16 xDistance = sFieldRegionMapHandler->regionMap.cursorPosX - sFieldRegionMapHandler->regionMap.playerIconSpritePosX;
    s16 yDistance = sFieldRegionMapHandler->regionMap.cursorPosY - sFieldRegionMapHandler->regionMap.playerIconSpritePosY;
    u32 fare;

    if (xDistance < 0)
        xDistance = -xDistance;
    if (yDistance < 0)
        yDistance = -yDistance;

    fare = 250 + (xDistance + yDistance) * 25;
    if (fare > 99999)
        fare = 99999;

    return fare;
}
