Route30_MapScripts::
	.byte 0

Route30_EventScript_RouteSign::
	msgbox Route30_Text_RouteSign, MSGBOX_SIGN
	end

Route30_EventScript_MrPokemonsHouseSign::
	msgbox Route30_Text_MrPokemonsHouseSign, MSGBOX_SIGN
	end

Route30_EventScript_BerryKeeper::
	msgbox Route30_Text_BerryKeeper, MSGBOX_NPC
	end

Route30_Text_RouteSign::
	.string "ROUTE 30\n"
	.string "ROUTE 31 -\n"
	.string "CHERRYGROVE CITY$"

Route30_Text_MrPokemonsHouseSign::
	.string "MR. POKEMON'S HOUSE$"

Route30_Text_BerryKeeper::
	.string "Those BERRY trees are for\n"
	.string "passing TRAINERS.\p"
	.string "Take what you need, and\n"
	.string "check back another day.$"

Route30_EventScript_Calvin::
	trainerbattle_single TRAINER_ACT1_YOUNGSTER_CALVIN, Route30_Text_CalvinIntro, Route30_Text_CalvinDefeat
	msgbox Route30_Text_CalvinPostBattle, MSGBOX_AUTOCLOSE
	end

Route30_EventScript_Mikey::
	trainerbattle_single TRAINER_JOHTO_YOUNGSTER_MIKEY, Route30_Text_MikeyIntro, Route30_Text_MikeyDefeat
	msgbox Route30_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route30_EventScript_Don::
	trainerbattle_single TRAINER_JOHTO_BUG_CATCHER_DON, Route30_Text_DonIntro, Route30_Text_DonDefeat
	msgbox Route30_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route30_EventScript_Mira::
	trainerbattle_single TRAINER_ACT1_LASS_MIRA, Route30_Text_MiraIntro, Route30_Text_MiraDefeat
	msgbox Route30_Text_MiraPostBattle, MSGBOX_AUTOCLOSE
	end

Route30_Text_CalvinIntro::
	.string "This spot's open today,\n"
	.string "so I'm training here.\p"
	.string "Want a battle?$"

Route30_Text_CalvinDefeat::
	.string "Aww, back to training!$"

Route30_Text_CalvinPostBattle::
	.string "My friend JOEY hasn't been\n"
	.string "around lately.\p"
	.string "He used to call everyone\n"
	.string "about his RATTATA.\p"
	.string "Maybe he finally found\n"
	.string "something better to do.$"

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

Route30_Text_MiraIntro::
	.string "My POKEMON and I are\n"
	.string "checking the north path.\p"
	.string "Battle me if you're\n"
	.string "heading that way!$"

Route30_Text_MiraDefeat::
	.string "Your path looks clear.$"

Route30_Text_MiraPostBattle::
	.string "A good route team needs\n"
	.string "answers for small surprises.$"

Route30_Text_TrainerPostBattle::
	.string "Route battles are the\n"
	.string "best way to toughen up.$"
