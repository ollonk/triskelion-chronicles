VioletCity_MapScripts::
	.byte 0

VioletCity_EventScript_GymWatcher::
	msgbox VioletCity_Text_GymWatcher, MSGBOX_NPC
	end

VioletCity_EventScript_TowerWatcher::
	msgbox VioletCity_Text_TowerWatcher, MSGBOX_NPC
	end

VioletCity_EventScript_PokemonCenterGirl::
	msgbox VioletCity_Text_PokemonCenterGirl, MSGBOX_NPC
	end

VioletCity_EventScript_SchoolKid::
	msgbox VioletCity_Text_SchoolKid, MSGBOX_NPC
	end

VioletCity_EventScript_TownSign::
	msgbox VioletCity_Text_TownSign, MSGBOX_SIGN
	end

VioletCity_Text_TownSign::
	.string "VIOLET CITY\n"
	.string "The City of\n"
	.string "Nostalgic Scents$"

VioletCity_EventScript_PokeCenterSign::
	msgbox VioletCity_Text_PokeCenterSign, MSGBOX_SIGN
	end

VioletCity_Text_PokeCenterSign::
	.string "POKEMON CENTER$"

VioletCity_EventScript_MartSign::
	msgbox VioletCity_Text_MartSign, MSGBOX_SIGN
	end

VioletCity_Text_MartSign::
	.string "POKEMON MART$"

VioletCity_EventScript_SproutTowerSign::
	msgbox VioletCity_Text_SproutTowerSign, MSGBOX_SIGN
	end

VioletCity_Text_SproutTowerSign::
	.string "SPROUT TOWER\n"
	.string "Experience the Way\n"
	.string "of POKEMON$"

VioletCity_Text_GymWatcher::
	.string "FALKNER uses FLYING-type\n"
	.string "POKEMON.\p"
	.string "Bring a steady team before\n"
	.string "you challenge him.$"

VioletCity_Text_TowerWatcher::
	.string "The SAGES train inside\n"
	.string "SPROUT TOWER.\p"
	.string "It's a good first test\n"
	.string "before the GYM.$"

VioletCity_Text_PokemonCenterGirl::
	.string "TRAINERS come down from\n"
	.string "the tower looking worn out.\p"
	.string "Heal before you go back in.$"

VioletCity_Text_SchoolKid::
	.string "STATUS moves are on the\n"
	.string "test.\p"
	.string "I know the answers, but\n"
	.string "using them in battle is\l"
	.string "harder.$"
