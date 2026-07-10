cherrygrove_house0_MapScripts::
	.byte 0

cherrygrove_house0_EventScript_PokeBallNeighbor::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_CHERRYGROVE_HOUSE_POKE_BALL, cherrygrove_house0_EventScript_PokeBallNeighborAfter
	msgbox cherrygrove_house0_Text_PokeBallNeighborIntro, MSGBOX_DEFAULT
	giveitem ITEM_POKE_BALL
	goto_if_eq VAR_RESULT, FALSE, cherrygrove_house0_EventScript_PokeBallNeighborBagFull
	setflag FLAG_RECEIVED_CHERRYGROVE_HOUSE_POKE_BALL
	msgbox cherrygrove_house0_Text_PokeBallNeighborAfter, MSGBOX_DEFAULT
	release
	end

cherrygrove_house0_EventScript_PokeBallNeighborAfter::
	msgbox cherrygrove_house0_Text_PokeBallNeighborAfter, MSGBOX_DEFAULT
	release
	end

cherrygrove_house0_EventScript_PokeBallNeighborBagFull::
	msgbox cherrygrove_house0_Text_PokeBallNeighborBagFull, MSGBOX_DEFAULT
	release
	end

cherrygrove_house0_EventScript_WindowWatcher::
	msgbox cherrygrove_house0_Text_WindowWatcher, MSGBOX_NPC
	end

cherrygrove_house0_Text_PokeBallNeighborIntro::
	.string "I had one spare POKE BALL.\p"
	.string "Take it before ROUTE 30\n"
	.string "makes you wish you had.$"

cherrygrove_house0_Text_PokeBallNeighborAfter::
	.string "A spare POKE BALL is a\n"
	.string "little courage with a\l"
	.string "button on it.$"

cherrygrove_house0_Text_PokeBallNeighborBagFull::
	.string "Your BAG is too full?\p"
	.string "Make room and I will keep\n"
	.string "it by the door.$"

cherrygrove_house0_Text_WindowWatcher::
	.string "The sea wind changes before\n"
	.string "rain does.\p"
	.string "Old sailors swear TRAINERS\n"
	.string "should learn that too.$"
