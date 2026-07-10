Route33_MapScripts::
	.byte 0

Route33_EventScript_RouteSign::
	msgbox Route33_Text_RouteSign, MSGBOX_SIGN
	end

Route33_EventScript_RainWatcher::
	msgbox Route33_Text_RainWatcher, MSGBOX_NPC
	end

Route33_Text_RouteSign::
	.string "ROUTE 33\n"
	.string "UNION CAVE - AZALEA TOWN$"

Route33_Text_RainWatcher::
	.string "The rain hides footsteps\n"
	.string "coming out of UNION CAVE.\p"
	.string "It also hides how tired\n"
	.string "everyone looks.$"

Route33_EventScript_Anthony::
	trainerbattle_single TRAINER_JOHTO_HIKER_ANTHONY, Route33_Text_AnthonyIntro, Route33_Text_AnthonyDefeat
	msgbox Route33_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route33_Text_AnthonyIntro::
	.string "Whoa! You're soaked\n"
	.string "from the cave too?\p"
	.string "Let's have a battle!$"

Route33_Text_AnthonyDefeat::
	.string "Whoa, beaten!$"

Route33_Text_TrainerPostBattle::
	.string "UNION CAVE is tough,\n"
	.string "but this rain is worse.$"
