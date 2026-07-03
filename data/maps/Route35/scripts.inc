Route35_MapScripts::
	.byte 0

Route35_EventScript_NationalParkSign::
	msgbox Route35_Text_NationalParkSign, MSGBOX_SIGN
	end

Route35_Text_NationalParkSign::
	.string "NATIONAL PARK\n"
	.string "North entrance$"

Route35_EventScript_RouteSign::
	msgbox Route35_Text_RouteSign, MSGBOX_SIGN
	end

Route35_Text_RouteSign::
	.string "ROUTE 35\n"
	.string "GOLDENROD CITY - NATIONAL PARK$"

Route35_EventScript_Ivan::
	trainerbattle_single TRAINER_JOHTO_CAMPER_IVAN, Route35_Text_IvanIntro, Route35_Text_IvanDefeat
	msgbox Route35_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route35_EventScript_Elliot::
	trainerbattle_single TRAINER_JOHTO_CAMPER_ELLIOT, Route35_Text_ElliotIntro, Route35_Text_ElliotDefeat
	msgbox Route35_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route35_EventScript_Brooke::
	trainerbattle_single TRAINER_JOHTO_PICNICKER_BROOKE, Route35_Text_BrookeIntro, Route35_Text_BrookeDefeat
	msgbox Route35_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route35_EventScript_Kim::
	trainerbattle_single TRAINER_JOHTO_PICNICKER_KIM, Route35_Text_KimIntro, Route35_Text_KimDefeat
	msgbox Route35_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route35_EventScript_Bryan::
	trainerbattle_single TRAINER_JOHTO_BIRD_KEEPER_BRYAN, Route35_Text_BryanIntro, Route35_Text_BryanDefeat
	msgbox Route35_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route35_EventScript_Walt::
	trainerbattle_single TRAINER_JOHTO_KINDLER_WALT, Route35_Text_WaltIntro, Route35_Text_WaltDefeat
	msgbox Route35_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route35_EventScript_Arnie::
	trainerbattle_single TRAINER_JOHTO_BUG_CATCHER_ARNIE, Route35_Text_ArnieIntro, Route35_Text_ArnieDefeat
	msgbox Route35_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route35_EventScript_Irwin::
	trainerbattle_single TRAINER_JOHTO_PSYCHIC_IRWIN, Route35_Text_IrwinIntro, Route35_Text_IrwinDefeat
	msgbox Route35_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route35_EventScript_Dirk::
	trainerbattle_single TRAINER_JOHTO_GENTLEMAN_DIRK, Route35_Text_DirkIntro, Route35_Text_DirkDefeat
	msgbox Route35_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route35_Text_IvanIntro::
	.string "I came from GOLDENROD\n"
	.string "to train here!$"

Route35_Text_IvanDefeat::
	.string "Training is hard!$"

Route35_Text_ElliotIntro::
	.string "The park is north,\n"
	.string "but battle me first!$"

Route35_Text_ElliotDefeat::
	.string "I should visit the park.$"

Route35_Text_BrookeIntro::
	.string "My PIKACHU is quick!\n"
	.string "Can you keep up?$"

Route35_Text_BrookeDefeat::
	.string "You kept up.$"

Route35_Text_KimIntro::
	.string "A cute POKEMON can\n"
	.string "still be fierce!$"

Route35_Text_KimDefeat::
	.string "You were fiercer.$"

Route35_Text_BryanIntro::
	.string "My bird POKEMON can\n"
	.string "see the whole route!$"

Route35_Text_BryanDefeat::
	.string "They missed this one.$"

Route35_Text_WaltIntro::
	.string "My hot-blooded team\n"
	.string "is ready!$"

Route35_Text_WaltDefeat::
	.string "Burned out!$"

Route35_Text_ArnieIntro::
	.string "BUG POKEMON are\n"
	.string "stronger than you think!$"

Route35_Text_ArnieDefeat::
	.string "Think again, I guess.$"

Route35_Text_IrwinIntro::
	.string "Watch this trick!\n"
	.string "Four VOLTORB!$"

Route35_Text_IrwinDefeat::
	.string "That trick fizzled.$"

Route35_Text_DirkIntro::
	.string "The park is safe on\n"
	.string "my watch!$"

Route35_Text_DirkDefeat::
	.string "You're cleared to pass.$"

Route35_Text_TrainerPostBattle::
	.string "Lots of trainers gather\n"
	.string "near NATIONAL PARK.$"
