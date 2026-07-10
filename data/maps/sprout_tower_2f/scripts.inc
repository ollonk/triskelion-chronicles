sprout_tower_2f_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, sprout_tower_2f_OnTransition
	.byte 0

sprout_tower_2f_OnTransition:
	special PokeLink_RefreshWorldState
	end

sprout_tower_2f_EventScript_SageJin::
	trainerbattle_single TRAINER_SPROUT_TOWER_SAGE_JIN, sprout_tower_2f_Text_SageJinIntro, sprout_tower_2f_Text_SageJinDefeat
	msgbox sprout_tower_2f_Text_SagePostBattle, MSGBOX_AUTOCLOSE
	end

sprout_tower_2f_EventScript_Elder::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_FLASH, sprout_tower_2f_EventScript_ElderAfterGift
	msgbox sprout_tower_2f_Text_ElderBeforeGift, MSGBOX_DEFAULT
	giveitem ITEM_HM_FLASH
	goto_if_eq VAR_RESULT, FALSE, sprout_tower_2f_EventScript_ElderBagFull
	setflag FLAG_RECEIVED_HM_FLASH
	setflag FLAG_POKELINK_APP_FLASHLIGHT
	setflag FLAG_CLEARED_SPROUT_TOWER
	special PokeLink_RefreshWorldState
	playfanfare MUS_OBTAIN_ITEM
	msgbox sprout_tower_2f_Text_FlashlightReady, MSGBOX_DEFAULT
	waitfanfare
	release
	end

sprout_tower_2f_EventScript_ElderBagFull::
	msgbox sprout_tower_2f_Text_ElderBagFull, MSGBOX_DEFAULT
	release
	end

sprout_tower_2f_EventScript_ElderAfterGift::
	msgbox sprout_tower_2f_Text_ElderAfterGift, MSGBOX_DEFAULT
	release
	end

sprout_tower_2f_Text_SageJinIntro::
	.string "A good TRAINER adjusts\n"
	.string "when a battle changes.\p"
	.string "Let me see if you can.$"

sprout_tower_2f_Text_SageJinDefeat::
	.string "Good adjustment.$"

sprout_tower_2f_Text_SagePostBattle::
	.string "I train with different\n"
	.string "POKEMON to stay sharp.$"

sprout_tower_2f_Text_ElderBeforeGift::
	.string "You made it to the top\n"
	.string "and battled well.\p"
	.string "Take HM05. It will help\n"
	.string "with dark places.$"

sprout_tower_2f_Text_FlashlightReady::
	.string "POKéLINK flashlight calibrated!$"

sprout_tower_2f_Text_ElderBagFull::
	.string "Your BAG is full.\p"
	.string "Return when you can carry\n"
	.string "HM05.$"

sprout_tower_2f_Text_ElderAfterGift::
	.string "Use FLASH when a cave\n"
	.string "gets too dark to read.$"
