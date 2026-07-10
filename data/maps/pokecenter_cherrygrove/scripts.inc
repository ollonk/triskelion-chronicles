.set LOCALID_NURSE, 1

pokecenter_cherrygrove_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, pokecenter_cherrygrove_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

pokecenter_cherrygrove_OnTransition:
	setrespawn HEAL_LOCATION_CHERRYGROVE_CITY
	end

pokecenter_cherrygrove_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

pokecenter_cherrygrove_EventScript_TiredYoungster::
	msgbox pokecenter_cherrygrove_Text_TiredYoungster, MSGBOX_NPC
	end

pokecenter_cherrygrove_EventScript_PokeLinkKid::
	goto_if_set FLAG_POKELINK_APP_MAP, pokecenter_cherrygrove_EventScript_PokeLinkKidAfterMap
	msgbox pokecenter_cherrygrove_Text_PokeLinkKidBeforeMap, MSGBOX_NPC
	end

pokecenter_cherrygrove_EventScript_PokeLinkKidAfterMap::
	msgbox pokecenter_cherrygrove_Text_PokeLinkKidAfterMap, MSGBOX_NPC
	end

pokecenter_cherrygrove_EventScript_AntidoteTip::
	msgbox pokecenter_cherrygrove_Text_AntidoteTip, MSGBOX_NPC
	end

pokecenter_cherrygrove_EventScript_KantoTraveler::
	msgbox pokecenter_cherrygrove_Text_KantoTraveler, MSGBOX_NPC
	end

pokecenter_cherrygrove_Text_TiredYoungster::
	.string "I made it from NEW BARK.\p"
	.string "Then I healed.\p"
	.string "Then I felt brave again.$"

pokecenter_cherrygrove_Text_PokeLinkKidBeforeMap::
	.string "My POKéLINK says I'm\n"
	.string "in CHERRYGROVE.\p"
	.string "Thanks, I guess.$"

pokecenter_cherrygrove_Text_PokeLinkKidAfterMap::
	.string "Map app?\p"
	.string "Good.\p"
	.string "Now get lost\n"
	.string "professionally.$"

pokecenter_cherrygrove_Text_AntidoteTip::
	.string "Buy ANTIDOTES.\p"
	.string "You'll say you don't\n"
	.string "need them.\p"
	.string "Then you will.$"

pokecenter_cherrygrove_Text_KantoTraveler::
	.string "KANTO is across the\n"
	.string "water.\p"
	.string "Feels close.\p"
	.string "Still far.$"
