Route36_MapScripts::
	.byte 0

Route36_EventScript_RouteSign::
	msgbox Route36_Text_RouteSign, MSGBOX_SIGN
	end

Route36_Text_RouteSign::
	.string "ROUTE 36$"

Route36_EventScript_RuinsSign::
	msgbox Route36_Text_RuinsSign, MSGBOX_SIGN
	end

Route36_Text_RuinsSign::
	.string "RUINS OF ALPH\n"
	.string "NORTH ENTRANCE$"

Route36_EventScript_TipsSign::
	msgbox Route36_Text_TipsSign, MSGBOX_SIGN
	end

Route36_Text_TipsSign::
	.string "TRAINER TIPS\p"
	.string "POKEMON stats vary--even\n"
	.string "within the same species.\p"
	.string "Their stats may be similar\n"
	.string "at first.\p"
	.string "However, differences will\n"
	.string "be obvious as they grow.$"

Route36_EventScript_Mark::
	trainerbattle_single TRAINER_JOHTO_PSYCHIC_MARK, Route36_Text_MarkIntro, Route36_Text_MarkDefeat
	msgbox Route36_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route36_EventScript_Alan::
	trainerbattle_single TRAINER_JOHTO_SCHOOL_KID_ALAN, Route36_Text_AlanIntro, Route36_Text_AlanDefeat
	msgbox Route36_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route36_Text_MarkIntro::
	.string "I can read your mind.\n"
	.string "You want to battle!$"

Route36_Text_MarkDefeat::
	.string "I didn't foresee that.$"

Route36_Text_AlanIntro::
	.string "School taught me type\n"
	.string "matchups.\p"
	.string "Let me test them!$"

Route36_Text_AlanDefeat::
	.string "I failed the test.$"

Route36_Text_TrainerPostBattle::
	.string "The road splits here.\n"
	.string "So do training styles.$"
