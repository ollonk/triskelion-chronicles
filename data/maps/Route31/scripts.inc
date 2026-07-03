Route31_MapScripts::
	.byte 0

Route31_EventScript_RouteSign::
	msgbox Route31_Text_RouteSign, MSGBOX_SIGN
	end

Route31_Text_RouteSign::
	.string "ROUTE 31\n"
	.string "VIOLET CITY -\n"
	.string "CHERRYGROVE CITY$"

Route31_EventScript_Wade::
	trainerbattle_single TRAINER_JOHTO_BUG_CATCHER_WADE, Route31_Text_WadeIntro, Route31_Text_WadeDefeat
	msgbox Route31_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route31_Text_WadeIntro::
	.string "I caught a whole bunch\n"
	.string "of POKEMON.\p"
	.string "Let me battle with\n"
	.string "them!$"

Route31_Text_WadeDefeat::
	.string "Aww, I lost.$"

Route31_Text_TrainerPostBattle::
	.string "BUG POKEMON grow fast,\n"
	.string "so I keep catching more.$"
