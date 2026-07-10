Route34_MapScripts::
	.byte 0

Route34_EventScript_RouteSign::
	msgbox Route34_Text_RouteSign, MSGBOX_SIGN
	end

Route34_EventScript_DayCareWatcher::
	msgbox Route34_Text_DayCareWatcher, MSGBOX_NPC
	end

Route34_Text_RouteSign::
	.string "ROUTE 34\n"
	.string "GOLDENROD CITY -\n"
	.string "AZALEA TOWN\p"
	.string "ILEX FOREST\n"
	.string "SOMEWHERE BETWEEN$"

Route34_Text_DayCareWatcher::
	.string "POKEMON come out of DAY\n"
	.string "CARE with stronger legs.\p"
	.string "TRAINERS come out with\n"
	.string "lighter wallets.$"

Route34_EventScript_Todd::
	trainerbattle_single TRAINER_JOHTO_CAMPER_TODD, Route34_Text_ToddIntro, Route34_Text_ToddDefeat
	msgbox Route34_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route34_EventScript_Samuel::
	trainerbattle_single TRAINER_JOHTO_YOUNGSTER_SAMUEL, Route34_Text_SamuelIntro, Route34_Text_SamuelDefeat
	msgbox Route34_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route34_EventScript_Ian::
	trainerbattle_single TRAINER_JOHTO_YOUNGSTER_IAN, Route34_Text_IanIntro, Route34_Text_IanDefeat
	msgbox Route34_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route34_EventScript_Gina::
	trainerbattle_single TRAINER_JOHTO_PICNICKER_GINA, Route34_Text_GinaIntro, Route34_Text_GinaDefeat
	msgbox Route34_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route34_EventScript_Brandon::
	trainerbattle_single TRAINER_JOHTO_POKEFAN_BRANDON, Route34_Text_BrandonIntro, Route34_Text_BrandonDefeat
	msgbox Route34_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route34_EventScript_Irene::
	trainerbattle_single TRAINER_JOHTO_COOLTRAINER_IRENE, Route34_Text_IreneIntro, Route34_Text_IreneDefeat
	msgbox Route34_Text_CooltrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route34_EventScript_Jenn::
	trainerbattle_single TRAINER_JOHTO_COOLTRAINER_JENN, Route34_Text_JennIntro, Route34_Text_JennDefeat
	msgbox Route34_Text_CooltrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route34_EventScript_Kate::
	trainerbattle_single TRAINER_JOHTO_COOLTRAINER_KATE, Route34_Text_KateIntro, Route34_Text_KateDefeat
	msgbox Route34_Text_CooltrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route34_EventScript_Keith::
	trainerbattle_single TRAINER_JOHTO_GENTLEMAN_KEITH, Route34_Text_KeithIntro, Route34_Text_KeithDefeat
	msgbox Route34_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route34_Text_ToddIntro::
	.string "My POKEMON just came\n"
	.string "back from DAY CARE.\p"
	.string "Let's see how it does!$"

Route34_Text_ToddDefeat::
	.string "It needs more time.$"

Route34_Text_SamuelIntro::
	.string "I'm raising a balanced\n"
	.string "team for GOLDENROD!$"

Route34_Text_SamuelDefeat::
	.string "I lost my balance.$"

Route34_Text_IanIntro::
	.string "I caught some strong\n"
	.string "POKEMON in ILEX.$"

Route34_Text_IanDefeat::
	.string "Not strong enough!$"

Route34_Text_GinaIntro::
	.string "I'll show you how cute\n"
	.string "grass POKEMON can be!$"

Route34_Text_GinaDefeat::
	.string "They're cute even when\n"
	.string "they lose.$"

Route34_Text_BrandonIntro::
	.string "My SNUBBULL is scary\n"
	.string "cute!$"

Route34_Text_BrandonDefeat::
	.string "Scary strong too?$"

Route34_Text_IreneIntro::
	.string "Are you ready for a\n"
	.string "serious battle?$"

Route34_Text_IreneDefeat::
	.string "That was serious.$"

Route34_Text_JennIntro::
	.string "We're training beyond\n"
	.string "the DAY CARE.$"

Route34_Text_JennDefeat::
	.string "I see your training\n"
	.string "paid off.$"

Route34_Text_KateIntro::
	.string "This is our private\n"
	.string "training spot!$"

Route34_Text_KateDefeat::
	.string "You found our weakness.$"

Route34_Text_KeithIntro::
	.string "Hold it! I patrol this\n"
	.string "road at night.$"

Route34_Text_KeithDefeat::
	.string "All clear, then.$"

Route34_Text_TrainerPostBattle::
	.string "GOLDENROD is close,\n"
	.string "but the road is busy.$"

Route34_Text_CooltrainerPostBattle::
	.string "The shore path is good\n"
	.string "for tougher training.$"
