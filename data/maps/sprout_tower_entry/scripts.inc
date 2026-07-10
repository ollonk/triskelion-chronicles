sprout_tower_entry_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, sprout_tower_entry_OnTransition
	.byte 0

sprout_tower_entry_OnTransition:
	special PokeLink_RefreshWorldState
	end

sprout_tower_entry_EventScript_CautiousPilgrim::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_SPROUT_TOWER_ENTRY_REPEL, sprout_tower_entry_EventScript_CautiousPilgrimAfter
	msgbox sprout_tower_entry_Text_CautiousPilgrimIntro, MSGBOX_DEFAULT
	giveitem ITEM_REPEL
	goto_if_eq VAR_RESULT, FALSE, sprout_tower_entry_EventScript_CautiousPilgrimBagFull
	setflag FLAG_RECEIVED_SPROUT_TOWER_ENTRY_REPEL
	msgbox sprout_tower_entry_Text_CautiousPilgrimAfter, MSGBOX_DEFAULT
	release
	end

sprout_tower_entry_EventScript_CautiousPilgrimAfter::
	msgbox sprout_tower_entry_Text_CautiousPilgrimAfter, MSGBOX_DEFAULT
	release
	end

sprout_tower_entry_EventScript_CautiousPilgrimBagFull::
	msgbox sprout_tower_entry_Text_CautiousPilgrimBagFull, MSGBOX_DEFAULT
	release
	end

sprout_tower_entry_EventScript_FloorWatcher::
	msgbox sprout_tower_entry_Text_FloorWatcher, MSGBOX_NPC
	end

sprout_tower_entry_Text_CautiousPilgrimIntro::
	.string "SPROUT TOWER creaks like\n"
	.string "it is breathing.\p"
	.string "I brought extra REPEL.\n"
	.string "Take one if you want quiet.$"

sprout_tower_entry_Text_CautiousPilgrimAfter::
	.string "REPEL helps when you are\n"
	.string "trying to reach the next\l"
	.string "set of stairs.$"

sprout_tower_entry_Text_CautiousPilgrimBagFull::
	.string "Too much in your BAG?\p"
	.string "Travel light before\n"
	.string "climbing.$"

sprout_tower_entry_Text_FloorWatcher::
	.string "The tower sways, but the\n"
	.string "floor is honest.\p"
	.string "Watch the paths more than\n"
	.string "the walls.$"
