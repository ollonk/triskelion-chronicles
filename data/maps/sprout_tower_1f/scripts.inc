sprout_tower_1f_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, sprout_tower_1f_OnTransition
	.byte 0

sprout_tower_1f_OnTransition:
	special PokeLink_RefreshWorldState
	end

sprout_tower_1f_EventScript_SageNico::
	trainerbattle_single TRAINER_SPROUT_TOWER_SAGE_NICO, sprout_tower_1f_Text_SageNicoIntro, sprout_tower_1f_Text_SageNicoDefeat
	msgbox sprout_tower_1f_Text_SagePostBattle, MSGBOX_AUTOCLOSE
	end

sprout_tower_1f_EventScript_Pilgrim::
	msgbox sprout_tower_1f_Text_Pilgrim, MSGBOX_NPC
	end

sprout_tower_1f_Text_SageNicoIntro::
	.string "SPROUT TOWER is where new\n"
	.string "TRAINERS learn patience.\p"
	.string "Show me how you battle.$"

sprout_tower_1f_Text_SageNicoDefeat::
	.string "You kept your focus.$"

sprout_tower_1f_Text_SagePostBattle::
	.string "Take your time here.\n"
	.string "The tower rewards patience.$"

sprout_tower_1f_Text_Pilgrim::
	.string "The tower sways in strong\n"
	.string "wind.\p"
	.string "It has stood that way\n"
	.string "for years.$"
