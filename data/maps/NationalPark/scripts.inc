NationalPark_MapScripts::
	.byte 0

NationalPark_EventScript_ParkSign::
	msgbox NationalPark_Text_ParkSign, MSGBOX_SIGN
	end

NationalPark_Text_ParkSign::
	.string "RELAXATION SQUARE\n"
	.string "NATIONAL PARK$"

NationalPark_EventScript_TipsSign::
	msgbox NationalPark_Text_TipsSign, MSGBOX_SIGN
	end

NationalPark_Text_TipsSign::
	.string "TRAINER TIPS\p"
	.string "Print out MAIL by opening\n"
	.string "it then pressing START.$"

NationalPark_EventScript_Jack::
	trainerbattle_single TRAINER_JOHTO_SCHOOL_KID_JACK, NationalPark_Text_JackIntro, NationalPark_Text_JackDefeat
	msgbox NationalPark_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

NationalPark_EventScript_Beverly::
	trainerbattle_single TRAINER_JOHTO_POKEFAN_BEVERLY, NationalPark_Text_BeverlyIntro, NationalPark_Text_BeverlyDefeat
	msgbox NationalPark_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

NationalPark_EventScript_William::
	trainerbattle_single TRAINER_JOHTO_POKEFAN_WILLIAM, NationalPark_Text_WilliamIntro, NationalPark_Text_WilliamDefeat
	msgbox NationalPark_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

NationalPark_EventScript_Krise::
	trainerbattle_single TRAINER_JOHTO_LASS_KRISE, NationalPark_Text_KriseIntro, NationalPark_Text_KriseDefeat
	msgbox NationalPark_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

NationalPark_Text_JackIntro::
	.string "I'm studying POKEMON\n"
	.string "while I battle!$"

NationalPark_Text_JackDefeat::
	.string "I need to study more.$"

NationalPark_Text_BeverlyIntro::
	.string "Isn't my SNUBBULL\n"
	.string "adorable?$"

NationalPark_Text_BeverlyDefeat::
	.string "Still adorable!$"

NationalPark_Text_WilliamIntro::
	.string "My RAICHU and I love\n"
	.string "this park!$"

NationalPark_Text_WilliamDefeat::
	.string "Oh, my RAICHU!$"

NationalPark_Text_KriseIntro::
	.string "The park is peaceful,\n"
	.string "but I still battle!$"

NationalPark_Text_KriseDefeat::
	.string "That was peaceful.$"

NationalPark_Text_TrainerPostBattle::
	.string "NATIONAL PARK draws\n"
	.string "all kinds of trainers.$"
