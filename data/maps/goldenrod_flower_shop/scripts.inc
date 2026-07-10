goldenrod_flower_shop_MapScripts::
	.byte 0

goldenrod_flower_shop_EventScript_BerryFlorist::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_GOLDENROD_FLOWER_SHOP_PERSIM_BERRY, goldenrod_flower_shop_EventScript_BerryFloristAfter
	msgbox goldenrod_flower_shop_Text_BerryFloristIntro, MSGBOX_DEFAULT
	giveitem ITEM_PERSIM_BERRY
	goto_if_eq VAR_RESULT, FALSE, goldenrod_flower_shop_EventScript_BerryFloristBagFull
	setflag FLAG_RECEIVED_GOLDENROD_FLOWER_SHOP_PERSIM_BERRY
	msgbox goldenrod_flower_shop_Text_BerryFloristAfter, MSGBOX_DEFAULT
	release
	end

goldenrod_flower_shop_EventScript_BerryFloristAfter::
	msgbox goldenrod_flower_shop_Text_BerryFloristAfter, MSGBOX_DEFAULT
	release
	end

goldenrod_flower_shop_EventScript_BerryFloristBagFull::
	msgbox goldenrod_flower_shop_Text_BerryFloristBagFull, MSGBOX_DEFAULT
	release
	end

goldenrod_flower_shop_EventScript_NoiseGardener::
	msgbox goldenrod_flower_shop_Text_NoiseGardener, MSGBOX_NPC
	end

goldenrod_flower_shop_Text_BerryFloristIntro::
	.string "CONFUSION makes battles\n"
	.string "look silly until it costs\l"
	.string "you.\p"
	.string "A PERSIM BERRY helps.$"

goldenrod_flower_shop_Text_BerryFloristAfter::
	.string "Plants need steady hands.\p"
	.string "So do confused POKEMON.$"

goldenrod_flower_shop_Text_BerryFloristBagFull::
	.string "Your BAG has no room?\p"
	.string "Set something aside and\n"
	.string "come back for the berry.$"

goldenrod_flower_shop_Text_NoiseGardener::
	.string "Flowers near GOLDENROD\n"
	.string "learn to grow around\l"
	.string "noise.$"
