.set LOCALID_NURSE, 1

pokecenter_route32_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, pokecenter_route32_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

pokecenter_route32_OnTransition:
	setrespawn HEAL_LOCATION_ROUTE32
	end

pokecenter_route32_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

pokecenter_route32_EventScript_Hiker::
	msgbox pokecenter_route32_Text_Hiker, MSGBOX_NPC
	end

pokecenter_route32_EventScript_Picnicker::
	msgbox pokecenter_route32_Text_Picnicker, MSGBOX_NPC
	end

pokecenter_route32_Text_Hiker::
	.string "Shortcuts are lies.\p"
	.string "Good lies.$"

pokecenter_route32_Text_Picnicker::
	.string "I packed lunch.\p"
	.string "My POKéMON packed\n"
	.string "appetite.$"
