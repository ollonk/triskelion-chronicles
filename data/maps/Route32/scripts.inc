Route32_MapScripts::
	.byte 0

Route32_EventScript_RouteSignNorth::
	msgbox Route32_Text_RouteSignNorth, MSGBOX_SIGN
	end

Route32_Text_RouteSignNorth::
	.string "ROUTE 32\n"
	.string "VIOLET CITY -\n"
	.string "AZALEA TOWN$"

Route32_EventScript_RouteSignSouth::
	msgbox Route32_Text_RouteSignSouth, MSGBOX_SIGN
	end

Route32_EventScript_BirdWatcher::
	msgbox Route32_Text_BirdWatcher, MSGBOX_NPC
	end

Route32_Text_RouteSignSouth::
	.string "UNION CAVE\n"
	.string "AHEAD$"

Route32_Text_BirdWatcher::
	.string "HOOTHOOT sleep through\n"
	.string "daytime battles.\p"
	.string "At night, they watch\n"
	.string "every step.$"

Route32_EventScript_Justin::
	trainerbattle_single TRAINER_JOHTO_FISHERMAN_JUSTIN, Route32_Text_JustinIntro, Route32_Text_JustinDefeat
	msgbox Route32_Text_FisherPostBattle, MSGBOX_AUTOCLOSE
	end

Route32_EventScript_Ralph::
	trainerbattle_single TRAINER_JOHTO_FISHERMAN_RALPH, Route32_Text_RalphIntro, Route32_Text_RalphDefeat
	msgbox Route32_Text_FisherPostBattle, MSGBOX_AUTOCLOSE
	end

Route32_EventScript_Henry::
	trainerbattle_single TRAINER_JOHTO_FISHERMAN_HENRY, Route32_Text_HenryIntro, Route32_Text_HenryDefeat
	msgbox Route32_Text_FisherPostBattle, MSGBOX_AUTOCLOSE
	end

Route32_EventScript_Albert::
	trainerbattle_single TRAINER_JOHTO_YOUNGSTER_ALBERT, Route32_Text_AlbertIntro, Route32_Text_AlbertDefeat
	msgbox Route32_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route32_EventScript_Gordon::
	trainerbattle_single TRAINER_JOHTO_YOUNGSTER_GORDON, Route32_Text_GordonIntro, Route32_Text_GordonDefeat
	msgbox Route32_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route32_EventScript_Roland::
	trainerbattle_single TRAINER_JOHTO_CAMPER_ROLAND, Route32_Text_RolandIntro, Route32_Text_RolandDefeat
	msgbox Route32_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route32_EventScript_Liz::
	trainerbattle_single TRAINER_JOHTO_PICNICKER_LIZ, Route32_Text_LizIntro, Route32_Text_LizDefeat
	msgbox Route32_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route32_EventScript_Peter::
	trainerbattle_single TRAINER_JOHTO_BIRD_KEEPER_PETER, Route32_Text_PeterIntro, Route32_Text_PeterDefeat
	msgbox Route32_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route32_Text_JustinIntro::
	.string "Whoa! You made me\n"
	.string "lose that fish!$"

Route32_Text_JustinDefeat::
	.string "Splash?$"

Route32_Text_RalphIntro::
	.string "The water here is\n"
	.string "calm and deep.\p"
	.string "My POKEMON are too!$"

Route32_Text_RalphDefeat::
	.string "I was reeled in!$"

Route32_Text_HenryIntro::
	.string "I love fishing, but\n"
	.string "battle is fun too!$"

Route32_Text_HenryDefeat::
	.string "That one got away!$"

Route32_Text_AlbertIntro::
	.string "I've been getting\n"
	.string "stronger in the grass.$"

Route32_Text_AlbertDefeat::
	.string "I'm not strong enough!$"

Route32_Text_GordonIntro::
	.string "My POKEMON is freshly\n"
	.string "caught!$"

Route32_Text_GordonDefeat::
	.string "It still needs training.$"

Route32_Text_RolandIntro::
	.string "I haven't seen you\n"
	.string "around before.\p"
	.string "Let's battle!$"

Route32_Text_RolandDefeat::
	.string "Not bad at all.$"

Route32_Text_LizIntro::
	.string "You startled me!\n"
	.string "I wasn't ready!$"

Route32_Text_LizDefeat::
	.string "I should have been\n"
	.string "ready.$"

Route32_Text_PeterIntro::
	.string "I train bird POKEMON\n"
	.string "on this route.$"

Route32_Text_PeterDefeat::
	.string "My birds fell behind.$"

Route32_Text_FisherPostBattle::
	.string "Fishing takes patience.\n"
	.string "So does training.$"

Route32_Text_TrainerPostBattle::
	.string "This road to UNION CAVE\n"
	.string "has plenty of trainers.$"
