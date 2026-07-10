#ifndef GUARD_POKELINK_H
#define GUARD_POKELINK_H

enum PokeLinkAppId
{
    POKELINK_APP_MAP,
    POKELINK_APP_SIGHTINGS,
    POKELINK_APP_COLLECTION_LOG,
    POKELINK_APP_PROFILE,
    POKELINK_APP_GLOOMSCROLL,
    POKELINK_APP_DEXNAV,
    POKELINK_APP_RADIO,
    POKELINK_APP_VS_SEEKER,
    POKELINK_APP_PHONE,
    POKELINK_APP_DELIVERY,
    POKELINK_APP_ABRACAB,
    POKELINK_APP_MESSAGES,
    POKELINK_APP_COUNT
};

#define POKELINK_APP_ENCOUNTERS POKELINK_APP_SIGHTINGS
#define POKELINK_APP_FLASHLIGHT POKELINK_APP_PHONE
#define POKELINK_APP_NOTES      POKELINK_APP_MESSAGES

#define POKELINK_FAVORITE_COUNT 3
#define POKELINK_SHORTCUT_NONE  0xFF

void CB2_InitPokeLink(void);
void PokeLink_OpenAppFromField(u8 appId);
const u8 *PokeLink_GetShortcutName(u8 slot);
u8 PokeLink_GetShortcutAppId(u8 slot);
void PokeLink_UnlockAllApps(void);
void PokeLink_RefreshWorldState(void);
void PokeLink_TryLogCollectedItem(u16 itemId, u16 quantity);
void PokeLink_ShowCollectionLogDemoPopup(void);

#endif // GUARD_POKELINK_H
