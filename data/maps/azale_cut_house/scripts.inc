azale_cut_house_MapScripts::
	.byte 0

azale_cut_house_EventScript_ForestPrepper::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_AZALEA_CUT_HOUSE_PECHA_BERRY, azale_cut_house_EventScript_ForestPrepperAfter
	msgbox azale_cut_house_Text_ForestPrepperIntro, MSGBOX_DEFAULT
	giveitem ITEM_PECHA_BERRY
	goto_if_eq VAR_RESULT, FALSE, azale_cut_house_EventScript_ForestPrepperBagFull
	setflag FLAG_RECEIVED_AZALEA_CUT_HOUSE_PECHA_BERRY
	msgbox azale_cut_house_Text_ForestPrepperAfter, MSGBOX_DEFAULT
	release
	end

azale_cut_house_EventScript_ForestPrepperAfter::
	msgbox azale_cut_house_Text_ForestPrepperAfter, MSGBOX_DEFAULT
	release
	end

azale_cut_house_EventScript_ForestPrepperBagFull::
	msgbox azale_cut_house_Text_ForestPrepperBagFull, MSGBOX_DEFAULT
	release
	end

azale_cut_house_EventScript_CharcoalKid::
	msgbox azale_cut_house_Text_CharcoalKid, MSGBOX_NPC
	end

azale_cut_house_Text_ForestPrepperIntro::
	.string "ILEX FOREST is beautiful,\n"
	.string "but poison can make it\l"
	.string "feel very long.\p"
	.string "Take this PECHA BERRY.$"

azale_cut_house_Text_ForestPrepperAfter::
	.string "The best CUT path is the\n"
	.string "one you planned before\l"
	.string "stepping into grass.$"

azale_cut_house_Text_ForestPrepperBagFull::
	.string "Your BAG is packed tight.\p"
	.string "Make room and I will hand\n"
	.string "over the berry.$"

azale_cut_house_Text_CharcoalKid::
	.string "Charcoal makers learn to\n"
	.string "read the forest by smell.\p"
	.string "Fresh leaves mean the path\n"
	.string "was used recently.$"
