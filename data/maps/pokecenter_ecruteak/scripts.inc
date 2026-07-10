.set LOCALID_NURSE, 1

pokecenter_ecruteak_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, pokecenter_ecruteak_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

pokecenter_ecruteak_OnTransition:
	setrespawn HEAL_LOCATION_ECRUTEAK_CITY
	end

pokecenter_ecruteak_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

pokecenter_ecruteak_EventScript_OldWoman::
	msgbox pokecenter_ecruteak_Text_OldWoman, MSGBOX_NPC
	end

pokecenter_ecruteak_EventScript_TowerTraveler::
	msgbox pokecenter_ecruteak_Text_TowerTraveler, MSGBOX_NPC
	end

pokecenter_ecruteak_Text_OldWoman::
	.string "ECRUTEAK is old.\p"
	.string "The floorboards know\n"
	.string "more than I do.$"

pokecenter_ecruteak_Text_TowerTraveler::
	.string "I came for towers.\p"
	.string "I found stairs.\p"
	.string "Many stairs.$"
