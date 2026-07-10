.set LOCALID_RADIO_PRODUCER, 1

radio_tower_1f_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, radio_tower_1f_OnTransition
	.byte 0

radio_tower_1f_OnTransition:
	special PokeLink_RefreshWorldState
	end

radio_tower_1f_EventScript_RadioProducer::
	lock
	faceplayer
	goto_if_set FLAG_POKELINK_APP_RADIO, radio_tower_1f_EventScript_RadioProducerAfter
	msgbox radio_tower_1f_Text_RadioProducerIntro, MSGBOX_DEFAULT
	setflag FLAG_POKELINK_APP_RADIO
	setvar VAR_ACT1_STORY_STAGE, ACT1_STAGE_RADIO_UNLOCKED
	special PokeLink_RefreshWorldState
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox radio_tower_1f_Text_RadioUnlocked, MSGBOX_DEFAULT
	waitfanfare
	release
	end

radio_tower_1f_EventScript_RadioProducerAfter::
	msgbox radio_tower_1f_Text_RadioProducerAfter, MSGBOX_DEFAULT
	release
	end

radio_tower_1f_Text_RadioProducerIntro::
	.string "Old POKéGEAR, new POKéLINK...\n"
	.string "same problem.\p"
	.string "A TRAINER misses half of JOHTO\n"
	.string "if they never tune in.\p"
	.string "Here, I'll install the RADIO\n"
	.string "app on your POKéLINK.$"

radio_tower_1f_Text_RadioUnlocked::
	.string "POKéLINK Radio app\n"
	.string "unlocked!$"

radio_tower_1f_Text_RadioProducerAfter::
	.string "The RADIO app can carry\n"
	.string "local shows and odd alerts.\p"
	.string "Keep it handy after GYMS.\n"
	.string "Stories travel fast here.$"
