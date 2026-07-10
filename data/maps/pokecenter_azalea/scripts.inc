.set LOCALID_NURSE, 1

pokecenter_azalea_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, pokecenter_azalea_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

pokecenter_azalea_OnTransition:
	setrespawn HEAL_LOCATION_AZALEA_TOWN
	end

pokecenter_azalea_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

pokecenter_azalea_EventScript_BugCatcher::
	goto_if_set FLAG_BADGE02_GET, pokecenter_azalea_EventScript_BugCatcherAfterBadge
	msgbox pokecenter_azalea_Text_BugCatcherBeforeBadge, MSGBOX_NPC
	end

pokecenter_azalea_EventScript_BugCatcherAfterBadge::
	msgbox pokecenter_azalea_Text_BugCatcherAfterBadge, MSGBOX_NPC
	end

pokecenter_azalea_EventScript_ForestLocal::
	goto_if_set FLAG_ROCKET_ILEX_EVENT_CLEARED, pokecenter_azalea_EventScript_ForestLocalAfterRocket
	msgbox pokecenter_azalea_Text_ForestLocalBeforeRocket, MSGBOX_NPC
	end

pokecenter_azalea_EventScript_ForestLocalAfterRocket::
	msgbox pokecenter_azalea_Text_ForestLocalAfterRocket, MSGBOX_NPC
	end

pokecenter_azalea_EventScript_RocketWaryTrainer::
	msgbox pokecenter_azalea_Text_RocketWaryTrainer, MSGBOX_NPC
	end

pokecenter_azalea_EventScript_EncountersFan::
	goto_if_set FLAG_POKELINK_APP_ENCOUNTERS, pokecenter_azalea_EventScript_EncountersFanAfterApp
	msgbox pokecenter_azalea_Text_EncountersFanBeforeApp, MSGBOX_NPC
	end

pokecenter_azalea_EventScript_EncountersFanAfterApp::
	msgbox pokecenter_azalea_Text_EncountersFanAfterApp, MSGBOX_NPC
	end

pokecenter_azalea_Text_BugCatcherBeforeBadge::
	.string "BUGSY takes notes.\p"
	.string "I lost.\p"
	.string "Now I am notes.$"

pokecenter_azalea_Text_BugCatcherAfterBadge::
	.string "HIVE BADGE?\p"
	.string "Then he took notes\n"
	.string "on you.$"

pokecenter_azalea_Text_ForestLocalBeforeRocket::
	.string "The forest feels wrong.\p"
	.string "Trees should not feel\n"
	.string "busy.$"

pokecenter_azalea_Text_ForestLocalAfterRocket::
	.string "The forest is quiet\n"
	.string "again.\p"
	.string "Better quiet.$"

pokecenter_azalea_Text_RocketWaryTrainer::
	.string "TEAM ROCKET stepped\n"
	.string "on my lunch.\p"
	.string "I stepped on their foot.\p"
	.string "Fair.$"

pokecenter_azalea_Text_EncountersFanBeforeApp::
	.string "I write down wild\n"
	.string "POKéMON.\p"
	.string "Then I lose the paper.$"

pokecenter_azalea_Text_EncountersFanAfterApp::
	.string "Encounters app?\p"
	.string "Show-off.$"
