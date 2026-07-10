.set LOCALID_GUIDE_GENT, 1

CherrygroveCity_MapScripts::
	.byte 0

CherrygroveCity_EventScript_InstallPokeLinkMapApp::
	lock
	faceplayer
	goto_if_set FLAG_POKELINK_APP_MAP, CherrygroveCity_EventScript_AlreadyInstalledMapApp
	msgbox CherrygroveCity_Text_TourIntro, MSGBOX_DEFAULT
	closemessage
	setobjectxy LOCALID_GUIDE_GENT, 38, 12
	setobjectxy OBJ_EVENT_ID_PLAYER, 38, 13
	applymovement LOCALID_GUIDE_GENT, Common_Movement_FaceDown
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_FaceUp
	waitmovement 0
	msgbox CherrygroveCity_Text_TourStart, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_GUIDE_GENT, CherrygroveCity_Movement_GuideTourWest
	applymovement OBJ_EVENT_ID_PLAYER, CherrygroveCity_Movement_PlayerTourWest
	waitmovement 0
	msgbox CherrygroveCity_Text_TourMart, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_GUIDE_GENT, CherrygroveCity_Movement_GuideTourEast
	applymovement OBJ_EVENT_ID_PLAYER, CherrygroveCity_Movement_PlayerTourEast
	waitmovement 0
	msgbox CherrygroveCity_Text_TourCenter, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_GUIDE_GENT, CherrygroveCity_Movement_GuideTourSouth
	applymovement OBJ_EVENT_ID_PLAYER, CherrygroveCity_Movement_PlayerTourSouth
	waitmovement 0
	msgbox CherrygroveCity_Text_TourRoads, MSGBOX_DEFAULT
	setflag FLAG_POKELINK_APP_MAP
	setvar VAR_ACT1_STORY_STAGE, ACT1_STAGE_MAP_UNLOCKED
	special PokeLink_RefreshWorldState
	playfanfare MUS_OBTAIN_ITEM
	msgbox CherrygroveCity_Text_InstalledMapApp, MSGBOX_DEFAULT
	waitfanfare
	release
	end

CherrygroveCity_Movement_GuideTourWest:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_down
	step_end

CherrygroveCity_Movement_PlayerTourWest:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

CherrygroveCity_Movement_GuideTourEast:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_down
	step_end

CherrygroveCity_Movement_PlayerTourEast:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_up
	step_end

CherrygroveCity_Movement_GuideTourSouth:
	walk_down
	walk_down
	walk_in_place_faster_down
	step_end

CherrygroveCity_Movement_PlayerTourSouth:
	walk_down
	walk_down
	walk_in_place_faster_up
	step_end

CherrygroveCity_EventScript_AlreadyInstalledMapApp::
	msgbox CherrygroveCity_Text_AlreadyInstalledMapApp, MSGBOX_DEFAULT
	release
	end

CherrygroveCity_EventScript_FlowerWoman::
	msgbox CherrygroveCity_Text_FlowerWoman, MSGBOX_NPC
	end

CherrygroveCity_EventScript_RunningGirl::
	msgbox CherrygroveCity_Text_RunningGirl, MSGBOX_NPC
	end

CherrygroveCity_EventScript_ShoreFisher::
	msgbox CherrygroveCity_Text_ShoreFisher, MSGBOX_NPC
	end

CherrygroveCity_EventScript_PokeCenterSign::
	msgbox CherrygroveCity_Text_PokeCenterSign, MSGBOX_SIGN
	end

CherrygroveCity_Text_PokeCenterSign::
	.string "POKEMON CENTER$"

CherrygroveCity_EventScript_MartSign::
	msgbox CherrygroveCity_Text_MartSign, MSGBOX_SIGN
	end

CherrygroveCity_Text_MartSign::
	.string "POKEMON MART$"

CherrygroveCity_EventScript_GuideGentSign::
	msgbox CherrygroveCity_Text_GuideGentSign, MSGBOX_SIGN
	end

CherrygroveCity_Text_GuideGentSign::
	.string "GUIDE GENT'S HOUSE\n"
	.string "New TRAINERS welcome$"

CherrygroveCity_EventScript_TownSign::
	msgbox CherrygroveCity_Text_TownSign, MSGBOX_SIGN
	end

CherrygroveCity_Text_TownSign::
	.string "CHERRYGROVE CITY\n"
	.string "Fresh sea air and\n"
	.string "friendly roads$"

CherrygroveCity_Text_TourIntro::
	.string "New in CHERRYGROVE?\n"
	.string "I'll show you the basics.$"

CherrygroveCity_Text_TourStart::
	.string "Keep this road in mind.\p"
	.string "West goes back to NEW BARK,\n"
	.string "and north leads deeper into\l"
	.string "JOHTO.$"

CherrygroveCity_Text_TourMart::
	.string "The MART is where you stock\n"
	.string "up before the next route.\p"
	.string "POKE BALLS, POTIONS, and\n"
	.string "status cures save trips back.$"

CherrygroveCity_Text_TourCenter::
	.string "The POKEMON CENTER is open\n"
	.string "day and night.\p"
	.string "Heal up whenever your team\n"
	.string "starts looking tired.$"

CherrygroveCity_Text_TourRoads::
	.string "That's the town.\p"
	.string "Before you go, I'll add a\n"
	.string "MAP app to your POKéLINK.$"

CherrygroveCity_Text_InstalledMapApp::
	.string "MAP app unlocked!\p"
	.string "It shows towns, routes, and\n"
	.string "how the roads connect.$"

CherrygroveCity_Text_AlreadyInstalledMapApp::
	.string "That MAP app will keep you\n"
	.string "from losing your way.$"

CherrygroveCity_Text_FlowerWoman::
	.string "BERRIES grow back after\n"
	.string "some time.\p"
	.string "Pick them when you can,\n"
	.string "but come back later.$"

CherrygroveCity_Text_RunningGirl::
	.string "I know every shortcut\n"
	.string "between the MART and the\l"
	.string "POKEMON CENTER.$"

CherrygroveCity_Text_ShoreFisher::
	.string "The tide leaves tiny\n"
	.string "treasures in the sand.\p"
	.string "Check the shore closely\n"
	.string "when you pass by.$"
