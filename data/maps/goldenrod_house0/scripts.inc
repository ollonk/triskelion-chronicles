goldenrod_house0_MapScripts::
	.byte 0

goldenrod_house0_EventScript_GreatBallNeighbor::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_GOLDENROD_HOUSE_GREAT_BALL, goldenrod_house0_EventScript_GreatBallNeighborAfter
	msgbox goldenrod_house0_Text_GreatBallNeighborIntro, MSGBOX_DEFAULT
	giveitem ITEM_GREAT_BALL
	goto_if_eq VAR_RESULT, FALSE, goldenrod_house0_EventScript_GreatBallNeighborBagFull
	setflag FLAG_RECEIVED_GOLDENROD_HOUSE_GREAT_BALL
	msgbox goldenrod_house0_Text_GreatBallNeighborAfter, MSGBOX_DEFAULT
	release
	end

goldenrod_house0_EventScript_GreatBallNeighborAfter::
	msgbox goldenrod_house0_Text_GreatBallNeighborAfter, MSGBOX_DEFAULT
	release
	end

goldenrod_house0_EventScript_GreatBallNeighborBagFull::
	msgbox goldenrod_house0_Text_GreatBallNeighborBagFull, MSGBOX_DEFAULT
	release
	end

goldenrod_house0_EventScript_ListMaker::
	msgbox goldenrod_house0_Text_ListMaker, MSGBOX_NPC
	end

goldenrod_house0_Text_GreatBallNeighborIntro::
	.string "Big city, bigger bags,\n"
	.string "right?\p"
	.string "Take this GREAT BALL.\n"
	.string "You will see something\l"
	.string "worth trying it on.$"

goldenrod_house0_Text_GreatBallNeighborAfter::
	.string "A good BALL is not a\n"
	.string "promise.\p"
	.string "It is just a better\n"
	.string "chance.$"

goldenrod_house0_Text_GreatBallNeighborBagFull::
	.string "Your BAG is full already?\p"
	.string "Come back after the city\n"
	.string "teaches you to pack light.$"

goldenrod_house0_Text_ListMaker::
	.string "GOLDENROD can make every\n"
	.string "errand feel urgent.\p"
	.string "I make a list before I\n"
	.string "step outside.$"
