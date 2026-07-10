#include "global.h"
#include "event_data.h"
#include "item.h"
#include "money.h"
#include "pokemon.h"
#include "script.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/species.h"

struct DebugRoomItem
{
    u16 itemId;
    u16 quantity;
};

static const struct DebugRoomItem sDebugRoomSupplies[] =
{
    {ITEM_FULL_RESTORE, 99},
    {ITEM_MAX_POTION, 99},
    {ITEM_MAX_REVIVE, 99},
    {ITEM_FULL_HEAL, 99},
    {ITEM_ETHER, 99},
    {ITEM_ELIXIR, 99},
    {ITEM_POKE_BALL, 99},
    {ITEM_GREAT_BALL, 99},
    {ITEM_ULTRA_BALL, 99},
    {ITEM_PREMIER_BALL, 99},
};

static const u16 sDebugRoomFlyFlags[] =
{
    FLAG_VISITED_LITTLEROOT_TOWN,
    FLAG_VISITED_OLDALE_TOWN,
    FLAG_VISITED_DEWFORD_TOWN,
    FLAG_VISITED_LAVARIDGE_TOWN,
    FLAG_VISITED_FALLARBOR_TOWN,
    FLAG_VISITED_VERDANTURF_TOWN,
    FLAG_VISITED_PACIFIDLOG_TOWN,
    FLAG_VISITED_PETALBURG_CITY,
    FLAG_VISITED_SLATEPORT_CITY,
    FLAG_VISITED_MAUVILLE_CITY,
    FLAG_VISITED_RUSTBORO_CITY,
    FLAG_VISITED_FORTREE_CITY,
    FLAG_VISITED_LILYCOVE_CITY,
    FLAG_VISITED_MOSSDEEP_CITY,
    FLAG_VISITED_SOOTOPOLIS_CITY,
    FLAG_VISITED_EVER_GRANDE_CITY,
    FLAG_LANDMARK_BATTLE_FRONTIER,
    FLAG_LANDMARK_POKEMON_LEAGUE,
    FLAG_VISITED_NEW_BARK_TOWN,
    FLAG_VISITED_CHERRYGROVE_CITY,
    FLAG_VISITED_VIOLET_CITY,
    FLAG_VISITED_AZALEA_TOWN,
    FLAG_VISITED_GOLDENROD_CITY,
    FLAG_VISITED_ECRUTEAK_CITY,
    FLAG_VISITED_OLIVINE_CITY,
    FLAG_VISITED_PALLET_TOWN,
    FLAG_VISITED_VIRIDIAN_CITY,
    FLAG_VISITED_PEWTER_CITY,
    FLAG_VISITED_CERULEAN_CITY,
    FLAG_VISITED_LAVENDER_TOWN,
    FLAG_VISITED_VERMILION_CITY,
    FLAG_VISITED_CELADON_CITY,
    FLAG_VISITED_FUCHSIA_CITY,
    FLAG_VISITED_CINNABAR_ISLAND,
    FLAG_VISITED_INDIGO_PLATEAU,
    FLAG_VISITED_SAFFRON_CITY,
};

bool8 DebugRoom_GiveMewtwo(void)
{
    struct Pokemon mon;
    u32 iv = MAX_PER_STAT_IVS;
    u8 metLevel = 1;
    u32 i;

    if (CalculatePlayerPartyCount() >= PARTY_SIZE)
        return FALSE;

    CreateMonWithNature(&mon, SPECIES_MEWTWO, 85, USE_RANDOM_IVS, NATURE_MODEST);
    SetMonData(&mon, MON_DATA_MET_LEVEL, &metLevel);
    for (i = 0; i < NUM_STATS; i++)
        SetMonData(&mon, MON_DATA_HP_IV + i, &iv);

    SetMonMoveSlot(&mon, MOVE_PSYSTRIKE, 0);
    SetMonMoveSlot(&mon, MOVE_AURA_SPHERE, 1);
    SetMonMoveSlot(&mon, MOVE_ICE_BEAM, 2);
    SetMonMoveSlot(&mon, MOVE_RECOVER, 3);
    CalculateMonStats(&mon);

    return GiveMonToPlayer(&mon) == MON_GIVEN_TO_PARTY;
}

bool8 DebugRoom_GiveSupplies(void)
{
    u32 i;
    bool8 addedAllItems = TRUE;

    for (i = 0; i < ARRAY_COUNT(sDebugRoomSupplies); i++)
    {
        if (!AddBagItem(sDebugRoomSupplies[i].itemId, sDebugRoomSupplies[i].quantity))
            addedAllItems = FALSE;
    }

    SetMoney(&gSaveBlock1Ptr->money, MAX_MONEY);
    return addedAllItems;
}

void DebugRoom_UnlockFlyDestinations(void)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sDebugRoomFlyFlags); i++)
        FlagSet(sDebugRoomFlyFlags[i]);
}
