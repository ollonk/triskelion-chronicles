.set LOCALID_DEBUG_MEWTWO, 1
.set LOCALID_DEBUG_SUPPLIES, 2
.set LOCALID_DEBUG_POKELINK, 3
.set LOCALID_DEBUG_FLY, 4
.set LOCALID_DEBUG_EXIT_NEW_BARK, 5
.set LOCALID_DEBUG_EXIT_PALLET, 6
.set LOCALID_DEBUG_EXIT_LITTLEROOT, 7

DebugRoom_MapScripts::
	.byte 0

DebugRoom_EventScript_Mewtwo::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_DEBUG_MEWTWO, DebugRoom_EventScript_MewtwoReceived
	msgbox DebugRoom_Text_MewtwoOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, DebugRoom_EventScript_Done
	specialvar VAR_RESULT, DebugRoom_GiveMewtwo
	goto_if_eq VAR_RESULT, TRUE, DebugRoom_EventScript_MewtwoGiven
	msgbox DebugRoom_Text_PartyFull, MSGBOX_DEFAULT
	release
	end

DebugRoom_EventScript_MewtwoGiven::
	setflag FLAG_RECEIVED_DEBUG_MEWTWO
	msgbox DebugRoom_Text_MewtwoGiven, MSGBOX_DEFAULT
	release
	end

DebugRoom_EventScript_MewtwoReceived::
	msgbox DebugRoom_Text_MewtwoReceived, MSGBOX_DEFAULT
	release
	end

DebugRoom_EventScript_Supplies::
	lock
	faceplayer
	msgbox DebugRoom_Text_SuppliesOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, DebugRoom_EventScript_Done
	specialvar VAR_RESULT, DebugRoom_GiveSupplies
	goto_if_eq VAR_RESULT, TRUE, DebugRoom_EventScript_SuppliesDone
	msgbox DebugRoom_Text_SuppliesPartial, MSGBOX_DEFAULT
	release
	end

DebugRoom_EventScript_SuppliesDone::
	msgbox DebugRoom_Text_SuppliesDone, MSGBOX_DEFAULT
	release
	end

DebugRoom_EventScript_PokeLink::
	lock
	faceplayer
	msgbox DebugRoom_Text_PokeLinkOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, DebugRoom_EventScript_Done
	special PokeLink_UnlockAllApps
	special PokeLink_RefreshWorldState
	msgbox DebugRoom_Text_PokeLinkDone, MSGBOX_DEFAULT
	release
	end

DebugRoom_EventScript_Fly::
	lock
	faceplayer
	msgbox DebugRoom_Text_FlyOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, DebugRoom_EventScript_Done
	special DebugRoom_UnlockFlyDestinations
	msgbox DebugRoom_Text_FlyDone, MSGBOX_DEFAULT
	release
	end

DebugRoom_EventScript_ExitNewBark::
	lock
	faceplayer
	msgbox DebugRoom_Text_ExitNewBark, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, DebugRoom_EventScript_Done
	release
	warp MAP_NEW_BARK_TOWN, 17, 15
	waitstate
	end

DebugRoom_EventScript_ExitPallet::
	lock
	faceplayer
	msgbox DebugRoom_Text_ExitPallet, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, DebugRoom_EventScript_Done
	release
	warp MAP_RG_PALLET_TOWN, 16, 14
	waitstate
	end

DebugRoom_EventScript_ExitLittleroot::
	lock
	faceplayer
	msgbox DebugRoom_Text_ExitLittleroot, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, DebugRoom_EventScript_Done
	release
	warp MAP_LITTLEROOT_TOWN, 7, 17
	waitstate
	end

DebugRoom_EventScript_Done::
	release
	end

DebugRoom_EventScript_Sign::
	msgbox DebugRoom_Text_Sign, MSGBOX_SIGN
	end

DebugRoom_Text_MewtwoOffer::
	.string "Create a level 85 MEWTWO\n"
	.string "for this save file?$"

DebugRoom_Text_PartyFull::
	.string "Your party is full.\n"
	.string "Make room before receiving\l"
	.string "the debug MEWTWO.$"

DebugRoom_Text_MewtwoGiven::
	.string "MEWTWO joined your party.$"

DebugRoom_Text_MewtwoReceived::
	.string "The debug MEWTWO has already\n"
	.string "been received on this save.$"

DebugRoom_Text_SuppliesOffer::
	.string "Restock debug supplies and\n"
	.string "set money to maximum?$"

DebugRoom_Text_SuppliesDone::
	.string "Supplies and money restored.$"

DebugRoom_Text_SuppliesPartial::
	.string "Money was restored.\p"
	.string "Some supplies could not be\n"
	.string "added because a pocket was full.$"

DebugRoom_Text_PokeLinkOffer::
	.string "Unlock every POKéLINK app?$"

DebugRoom_Text_PokeLinkDone::
	.string "Every registered POKéLINK app\n"
	.string "is now unlocked.$"

DebugRoom_Text_FlyOffer::
	.string "Mark every available Fly\n"
	.string "destination as visited?$"

DebugRoom_Text_FlyDone::
	.string "Available Fly destinations\n"
	.string "were marked as visited.$"

DebugRoom_Text_ExitNewBark::
	.string "Warp to NEW BARK TOWN?$"

DebugRoom_Text_ExitPallet::
	.string "Warp to PALLET TOWN?$"

DebugRoom_Text_ExitLittleroot::
	.string "Warp to LITTLEROOT TOWN?$"

DebugRoom_Text_Sign::
	.string "TRISKELION DEBUG ROOM\p"
	.string "The door returns to NEW BARK.\p"
	.string "Talk to the exit guides for\n"
	.string "NEW BARK, PALLET, or LITTLEROOT.$"
