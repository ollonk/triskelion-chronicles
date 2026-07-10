.set LOCALID_NURSE, 1

pokecenter_violet_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, pokecenter_violet_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

pokecenter_violet_OnTransition:
	setrespawn HEAL_LOCATION_VIOLET_CITY
	end

pokecenter_violet_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

pokecenter_violet_EventScript_SageResting::
	msgbox pokecenter_violet_Text_SageResting, MSGBOX_NPC
	end

pokecenter_violet_EventScript_BirdKeeper::
	goto_if_set FLAG_BADGE01_GET, pokecenter_violet_EventScript_BirdKeeperAfterBadge
	msgbox pokecenter_violet_Text_BirdKeeperBeforeBadge, MSGBOX_NPC
	end

pokecenter_violet_EventScript_BirdKeeperAfterBadge::
	msgbox pokecenter_violet_Text_BirdKeeperAfterBadge, MSGBOX_NPC
	end

pokecenter_violet_EventScript_PokeLinkUser::
	goto_if_set FLAG_POKELINK_APP_FLASHLIGHT, pokecenter_violet_EventScript_PokeLinkUserAfterFlashlight
	msgbox pokecenter_violet_Text_PokeLinkUserBeforeFlashlight, MSGBOX_NPC
	end

pokecenter_violet_EventScript_PokeLinkUserAfterFlashlight::
	msgbox pokecenter_violet_Text_PokeLinkUserAfterFlashlight, MSGBOX_NPC
	end

pokecenter_violet_EventScript_HoennTraveler::
	msgbox pokecenter_violet_Text_HoennTraveler, MSGBOX_NPC
	end

pokecenter_violet_Text_SageResting::
	.string "The tower sways.\p"
	.string "I swayed back.\p"
	.string "That was not wisdom.$"

pokecenter_violet_Text_BirdKeeperBeforeBadge::
	.string "FALKNER looks calm.\p"
	.string "His birds do not.$"

pokecenter_violet_Text_BirdKeeperAfterBadge::
	.string "ZEPHYR BADGE?\p"
	.string "Then you already\n"
	.string "looked up.$"

pokecenter_violet_Text_PokeLinkUserBeforeFlashlight::
	.string "My screen is bright.\p"
	.string "The sages say no.$"

pokecenter_violet_Text_PokeLinkUserAfterFlashlight::
	.string "Flashlight app?\p"
	.string "Good.\p"
	.string "Dark caves hate that.$"

pokecenter_violet_Text_HoennTraveler::
	.string "I came from HOENN.\p"
	.string "Your tower is polite.\p"
	.string "It only almost falls.$"
