Route30_MapScripts::
	.byte 0

Route30_EventScript_RouteSign::
	msgbox Route30_Text_RouteSign, MSGBOX_SIGN
	end

Route30_Text_RouteSign::
	.string "ROUTE 30\n"
	.string "VIOLET CITY -\n"
	.string "CHERRYGROVE CITY$"

Route30_EventScript_Joey::
	trainerbattle_single TRAINER_JOHTO_YOUNGSTER_JOEY, Route30_Text_JoeyIntro, Route30_Text_JoeyDefeat
	msgbox Route30_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route30_EventScript_Mikey::
	trainerbattle_single TRAINER_JOHTO_YOUNGSTER_MIKEY, Route30_Text_MikeyIntro, Route30_Text_MikeyDefeat
	msgbox Route30_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route30_EventScript_Don::
	trainerbattle_single TRAINER_JOHTO_BUG_CATCHER_DON, Route30_Text_DonIntro, Route30_Text_DonDefeat
	msgbox Route30_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route30_Text_JoeyIntro::
	.string "I just lost, so I'm\n"
	.string "trying to find more\l"
	.string "POKEMON.\p"
	.string "Wait! You look weak!\n"
	.string "Come on, let's battle!$"

Route30_Text_JoeyDefeat::
	.string "Aww, I lost again!$"

Route30_Text_MikeyIntro::
	.string "You're a POKEMON\n"
	.string "trainer, right?\p"
	.string "Then you have to\n"
	.string "battle!$"

Route30_Text_MikeyDefeat::
	.string "That's strange.\n"
	.string "I won before.$"

Route30_Text_DonIntro::
	.string "I found a bunch of\n"
	.string "BUG POKEMON.\p"
	.string "Let me show you!$"

Route30_Text_DonDefeat::
	.string "My bugs were squished!$"

Route30_Text_TrainerPostBattle::
	.string "Route battles are the\n"
	.string "best way to toughen up.$"
