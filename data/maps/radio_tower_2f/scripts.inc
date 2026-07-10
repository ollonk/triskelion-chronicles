radio_tower_2f_MapScripts::
	.byte 0

radio_tower_2f_EventScript_NightProducer::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_RADIO_TOWER_AWAKENING, radio_tower_2f_EventScript_NightProducerAfter
	msgbox radio_tower_2f_Text_NightProducerIntro, MSGBOX_DEFAULT
	giveitem ITEM_AWAKENING
	goto_if_eq VAR_RESULT, FALSE, radio_tower_2f_EventScript_NightProducerBagFull
	setflag FLAG_RECEIVED_RADIO_TOWER_AWAKENING
	msgbox radio_tower_2f_Text_NightProducerAfter, MSGBOX_DEFAULT
	release
	end

radio_tower_2f_EventScript_NightProducerAfter::
	msgbox radio_tower_2f_Text_NightProducerAfter, MSGBOX_DEFAULT
	release
	end

radio_tower_2f_EventScript_NightProducerBagFull::
	msgbox radio_tower_2f_Text_NightProducerBagFull, MSGBOX_DEFAULT
	release
	end

radio_tower_2f_EventScript_LetterReader::
	msgbox radio_tower_2f_Text_LetterReader, MSGBOX_NPC
	end

radio_tower_2f_Text_NightProducerIntro::
	.string "Night requests are rough.\p"
	.string "A spare AWAKENING keeps\n"
	.string "me awake.\p"
	.string "You might need it more.$"

radio_tower_2f_Text_NightProducerAfter::
	.string "Sleep moves are cute\n"
	.string "until you miss a whole\l"
	.string "turn.$"

radio_tower_2f_Text_NightProducerBagFull::
	.string "Your BAG is full?\p"
	.string "I will keep it on the\n"
	.string "desk for now.$"

radio_tower_2f_Text_LetterReader::
	.string "We read every letter on\n"
	.string "a quiet day.\p"
	.string "Some trainers write better\n"
	.string "than DJs talk.$"
