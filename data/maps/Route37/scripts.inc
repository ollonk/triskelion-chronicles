Route37_MapScripts::
	.byte 0

Route37_EventScript_RouteSign::
	msgbox Route37_Text_RouteSign, MSGBOX_SIGN
	end

Route37_Text_RouteSign::
	.string "ROUTE 37\n"
	.string "ECRUTEAK CITY - ROUTE 36$"

Route37_EventScript_Ann::
	trainerbattle_single TRAINER_JOHTO_TWINS_ANN, Route37_Text_AnnIntro, Route37_Text_AnnDefeat
	msgbox Route37_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route37_EventScript_Anne::
	trainerbattle_single TRAINER_JOHTO_TWINS_ANNE, Route37_Text_AnneIntro, Route37_Text_AnneDefeat
	msgbox Route37_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route37_EventScript_Greg::
	trainerbattle_single TRAINER_JOHTO_PSYCHIC_GREG, Route37_Text_GregIntro, Route37_Text_GregDefeat
	msgbox Route37_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route37_Text_AnnIntro::
	.string "ANN: We'll battle\n"
	.string "together!$"

Route37_Text_AnnDefeat::
	.string "ANN: Oh no!$"

Route37_Text_AnneIntro::
	.string "ANNE: We won't lose\n"
	.string "together!$"

Route37_Text_AnneDefeat::
	.string "ANNE: We did lose!$"

Route37_Text_GregIntro::
	.string "I sense a strong\n"
	.string "trainer nearby.\p"
	.string "It's you!$"

Route37_Text_GregDefeat::
	.string "My senses were right.$"

Route37_Text_TrainerPostBattle::
	.string "ECRUTEAK is just north.\n"
	.string "Keep your focus.$"
