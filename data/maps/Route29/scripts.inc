.set LOCALID_MARA, 1

Route29_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route29_OnTransition
	.byte 0

Route29_OnTransition:
	special PokeLink_RefreshWorldState
	end

Route29_EventScript_Mara::
	setvar VAR_0x8009, 0
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_MARA_ROUTE29, Route29_EventScript_MaraAfterBattle
	goto Route29_EventScript_MaraFirstBattle

Route29_EventScript_MaraTriggerNorth::
	setvar VAR_0x8008, 0
	goto Route29_EventScript_MaraScriptedEncounter

Route29_EventScript_MaraTriggerSouth::
	setvar VAR_0x8008, 1
	goto Route29_EventScript_MaraScriptedEncounter

Route29_EventScript_MaraTriggerFarSouth::
	setvar VAR_0x8008, 2
	goto Route29_EventScript_MaraScriptedEncounter

Route29_EventScript_MaraScriptedEncounter::
	setvar VAR_0x8009, 1
	lockall
	playbgm MUS_ENCOUNTER_MAY, TRUE
	applymovement LOCALID_MARA, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_MARA, Common_Movement_Delay48
	waitmovement 0
	call_if_eq VAR_0x8008, 0, Route29_EventScript_MaraApproachNorth
	call_if_eq VAR_0x8008, 1, Route29_EventScript_MaraApproachSouth
	call_if_eq VAR_0x8008, 2, Route29_EventScript_MaraApproachFarSouth
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	goto Route29_EventScript_MaraFirstBattle

Route29_EventScript_MaraFirstBattle::
	msgbox Route29_Text_MaraIntro, MSGBOX_DEFAULT
	msgbox Route29_Text_MaraBeforeBattle, MSGBOX_DEFAULT
	switch VAR_TRISKELION_STARTER_SPECIES
	case SPECIES_BULBASAUR, Route29_EventScript_BattleMaraSquirtle
	case SPECIES_CHARMANDER, Route29_EventScript_BattleMaraBulbasaur
	case SPECIES_SQUIRTLE, Route29_EventScript_BattleMaraCharmander
	case SPECIES_CHIKORITA, Route29_EventScript_BattleMaraTotodile
	case SPECIES_CYNDAQUIL, Route29_EventScript_BattleMaraChikorita
	case SPECIES_TOTODILE, Route29_EventScript_BattleMaraCyndaquil
	case SPECIES_TREECKO, Route29_EventScript_BattleMaraMudkip
	case SPECIES_TORCHIC, Route29_EventScript_BattleMaraTreecko
	case SPECIES_MUDKIP, Route29_EventScript_BattleMaraTorchic
	goto Route29_EventScript_BattleMaraChikorita

@ Later Mara fights should branch from VAR_TRISKELION_STARTER_* the same way.
Route29_EventScript_BattleMaraBulbasaur::
	trainerbattle_no_intro TRAINER_MARA_ROUTE29_BULBASAUR, Route29_Text_MaraDefeat
	goto Route29_EventScript_MaraDefeated

Route29_EventScript_BattleMaraCharmander::
	trainerbattle_no_intro TRAINER_MARA_ROUTE29_CHARMANDER, Route29_Text_MaraDefeat
	goto Route29_EventScript_MaraDefeated

Route29_EventScript_BattleMaraSquirtle::
	trainerbattle_no_intro TRAINER_MARA_ROUTE29_SQUIRTLE, Route29_Text_MaraDefeat
	goto Route29_EventScript_MaraDefeated

Route29_EventScript_BattleMaraChikorita::
	trainerbattle_no_intro TRAINER_MARA_ROUTE29_CHIKORITA, Route29_Text_MaraDefeat
	goto Route29_EventScript_MaraDefeated

Route29_EventScript_BattleMaraCyndaquil::
	trainerbattle_no_intro TRAINER_MARA_ROUTE29_CYNDAQUIL, Route29_Text_MaraDefeat
	goto Route29_EventScript_MaraDefeated

Route29_EventScript_BattleMaraTotodile::
	trainerbattle_no_intro TRAINER_MARA_ROUTE29_TOTODILE, Route29_Text_MaraDefeat
	goto Route29_EventScript_MaraDefeated

Route29_EventScript_BattleMaraTreecko::
	trainerbattle_no_intro TRAINER_MARA_ROUTE29_TREECKO, Route29_Text_MaraDefeat
	goto Route29_EventScript_MaraDefeated

Route29_EventScript_BattleMaraTorchic::
	trainerbattle_no_intro TRAINER_MARA_ROUTE29_TORCHIC, Route29_Text_MaraDefeat
	goto Route29_EventScript_MaraDefeated

Route29_EventScript_BattleMaraMudkip::
	trainerbattle_no_intro TRAINER_MARA_ROUTE29_MUDKIP, Route29_Text_MaraDefeat
	goto Route29_EventScript_MaraDefeated

Route29_EventScript_MaraDefeated::
	setflag FLAG_DEFEATED_MARA_ROUTE29
	setvar VAR_ACT1_STORY_STAGE, ACT1_STAGE_MARA_DEFEATED
	special HealPlayerParty
	special PokeLink_RefreshWorldState
	msgbox Route29_Text_MaraAfterWin, MSGBOX_DEFAULT
	fadedefaultbgm
	goto_if_eq VAR_0x8009, 1, Route29_EventScript_MaraScriptedRelease
	release
	end

Route29_EventScript_MaraScriptedRelease::
	releaseall
	end

Route29_EventScript_MaraAfterBattle::
	msgbox Route29_Text_MaraRematch, MSGBOX_DEFAULT
	release
	end

Route29_EventScript_MaraApproachNorth::
	applymovement LOCALID_MARA, Route29_Movement_MaraApproachNorth
	waitmovement 0
	return

Route29_EventScript_MaraApproachSouth::
	applymovement LOCALID_MARA, Route29_Movement_MaraApproachSouth
	waitmovement 0
	return

Route29_EventScript_MaraApproachFarSouth::
	applymovement LOCALID_MARA, Route29_Movement_MaraApproachFarSouth
	waitmovement 0
	return

Route29_Movement_MaraApproachNorth:
	walk_left
	walk_left
	walk_in_place_faster_left
	step_end

Route29_Movement_MaraApproachSouth:
	walk_down
	walk_left
	walk_left
	walk_in_place_faster_left
	step_end

Route29_Movement_MaraApproachFarSouth:
	walk_down
	walk_down
	walk_left
	walk_left
	walk_in_place_faster_left
	step_end

Route29_EventScript_RouteSign::
	msgbox Route29_Text_RouteSign, MSGBOX_SIGN
	end

Route29_EventScript_FieldGuide::
	msgbox Route29_Text_FieldGuide, MSGBOX_NPC
	end

Route29_Text_RouteSign::
	.string "ROUTE 29\n"
	.string "NEW BARK TOWN - CHERRYGROVE CITY$"

Route29_Text_FieldGuide::
	.string "Wild POKEMON live in the\n"
	.string "tall grass.\p"
	.string "If your team gets worn down,\n"
	.string "head back and heal.$"

Route29_Text_MaraIntro::
	.string "Hey! You're from NEW BARK too,\n"
	.string "right?\p"
	.string "I'm MARA.\p"
	.string "I just started out too, so\n"
	.string "let's see how we're doing.\p"
	.string "Friendly battle?$"

Route29_Text_MaraBeforeBattle::
	.string "Let's see what our starters\n"
	.string "can do!$"

Route29_Text_MaraDefeat::
	.string "Okay, I have some training\n"
	.string "to do.$"

Route29_Text_MaraAfterWin::
	.string "You're already getting the\n"
	.string "hang of this.\p"
	.string "I healed your team.\p"
	.string "I'm heading toward CHERRYGROVE\n"
	.string "too. Maybe we'll keep running\l"
	.string "into each other.$"

Route29_Text_MaraRematch::
	.string "I'm going to train a little\n"
	.string "more before I move on.$"
