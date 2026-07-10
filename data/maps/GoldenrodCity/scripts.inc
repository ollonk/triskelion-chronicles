.set LOCALID_MARA_GOLDENROD, 6

GoldenrodCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, GoldenrodCity_OnTransition
	.byte 0

GoldenrodCity_OnTransition:
	hideobjectat LOCALID_MARA_GOLDENROD, MAP_GOLDENROD_CITY
	setvar VAR_GOLDENROD_MARA_STATE, GOLDENROD_MARA_STATE_HIDDEN
	goto_if_set FLAG_DEFEATED_MARA_GOLDENROD, GoldenrodCity_OnTransitionShowMaraDone
	goto_if_unset FLAG_POKELINK_APP_RADIO, GoldenrodCity_OnTransitionRefresh
	goto_if_set FLAG_MET_MARA_GOLDENROD, GoldenrodCity_OnTransitionShowMaraWaiting
	setvar VAR_GOLDENROD_MARA_STATE, GOLDENROD_MARA_STATE_READY
	showobjectat LOCALID_MARA_GOLDENROD, MAP_GOLDENROD_CITY
	goto GoldenrodCity_OnTransitionRefresh

GoldenrodCity_OnTransitionShowMaraWaiting:
	setvar VAR_GOLDENROD_MARA_STATE, GOLDENROD_MARA_STATE_WAITING
	showobjectat LOCALID_MARA_GOLDENROD, MAP_GOLDENROD_CITY
	goto GoldenrodCity_OnTransitionRefresh

GoldenrodCity_OnTransitionShowMaraDone:
	setvar VAR_GOLDENROD_MARA_STATE, GOLDENROD_MARA_STATE_DONE
	showobjectat LOCALID_MARA_GOLDENROD, MAP_GOLDENROD_CITY

GoldenrodCity_OnTransitionRefresh:
	special PokeLink_RefreshWorldState
	end

GoldenrodCity_EventScript_MaraTrigger::
	setvar VAR_0x8009, 1
	lockall
	playbgm MUS_ENCOUNTER_MAY, TRUE
	applymovement LOCALID_MARA_GOLDENROD, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_MARA_GOLDENROD, Common_Movement_Delay48
	waitmovement 0
	applymovement LOCALID_MARA_GOLDENROD, Common_Movement_FaceLeft
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	goto GoldenrodCity_EventScript_MaraFirstMeet

GoldenrodCity_EventScript_Mara::
	setvar VAR_0x8009, 0
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_MARA_GOLDENROD, GoldenrodCity_EventScript_MaraAfterBattle
	goto_if_unset FLAG_POKELINK_APP_RADIO, GoldenrodCity_EventScript_MaraBeforeRadio
	playbgm MUS_ENCOUNTER_MAY, TRUE
	goto_if_set FLAG_MET_MARA_GOLDENROD, GoldenrodCity_EventScript_MaraAskBattle
	goto GoldenrodCity_EventScript_MaraFirstMeet

GoldenrodCity_EventScript_MaraFirstMeet::
	setflag FLAG_MET_MARA_GOLDENROD
	setvar VAR_GOLDENROD_MARA_STATE, GOLDENROD_MARA_STATE_WAITING
	goto_if_set FLAG_BADGE03_GET, GoldenrodCity_EventScript_MaraIntroAfterWhitney
	msgbox GoldenrodCity_Text_MaraIntro, MSGBOX_DEFAULT
	goto GoldenrodCity_EventScript_MaraAskBattle

GoldenrodCity_EventScript_MaraIntroAfterWhitney::
	msgbox GoldenrodCity_Text_MaraIntroAfterWhitney, MSGBOX_DEFAULT
	goto GoldenrodCity_EventScript_MaraAskBattle

GoldenrodCity_EventScript_MaraAskBattle::
	msgbox GoldenrodCity_Text_MaraAskBattle, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, GoldenrodCity_EventScript_MaraDeclined
	msgbox GoldenrodCity_Text_MaraYes, MSGBOX_DEFAULT
	switch VAR_TRISKELION_STARTER_SPECIES
	case SPECIES_BULBASAUR, GoldenrodCity_EventScript_BattleMaraWartortle
	case SPECIES_CHARMANDER, GoldenrodCity_EventScript_BattleMaraIvysaur
	case SPECIES_SQUIRTLE, GoldenrodCity_EventScript_BattleMaraCharmeleon
	case SPECIES_CHIKORITA, GoldenrodCity_EventScript_BattleMaraCroconaw
	case SPECIES_CYNDAQUIL, GoldenrodCity_EventScript_BattleMaraBayleef
	case SPECIES_TOTODILE, GoldenrodCity_EventScript_BattleMaraQuilava
	case SPECIES_TREECKO, GoldenrodCity_EventScript_BattleMaraMarshtomp
	case SPECIES_TORCHIC, GoldenrodCity_EventScript_BattleMaraGrovyle
	case SPECIES_MUDKIP, GoldenrodCity_EventScript_BattleMaraCombusken
	goto GoldenrodCity_EventScript_BattleMaraBayleef

@ Mara keeps the starter line weak to the player's original starter.
GoldenrodCity_EventScript_BattleMaraIvysaur::
	trainerbattle_no_intro TRAINER_MARA_GOLDENROD_IVYSAUR, GoldenrodCity_Text_MaraDefeat
	goto GoldenrodCity_EventScript_MaraDefeated

GoldenrodCity_EventScript_BattleMaraCharmeleon::
	trainerbattle_no_intro TRAINER_MARA_GOLDENROD_CHARMELEON, GoldenrodCity_Text_MaraDefeat
	goto GoldenrodCity_EventScript_MaraDefeated

GoldenrodCity_EventScript_BattleMaraWartortle::
	trainerbattle_no_intro TRAINER_MARA_GOLDENROD_WARTORTLE, GoldenrodCity_Text_MaraDefeat
	goto GoldenrodCity_EventScript_MaraDefeated

GoldenrodCity_EventScript_BattleMaraBayleef::
	trainerbattle_no_intro TRAINER_MARA_GOLDENROD_BAYLEEF, GoldenrodCity_Text_MaraDefeat
	goto GoldenrodCity_EventScript_MaraDefeated

GoldenrodCity_EventScript_BattleMaraQuilava::
	trainerbattle_no_intro TRAINER_MARA_GOLDENROD_QUILAVA, GoldenrodCity_Text_MaraDefeat
	goto GoldenrodCity_EventScript_MaraDefeated

GoldenrodCity_EventScript_BattleMaraCroconaw::
	trainerbattle_no_intro TRAINER_MARA_GOLDENROD_CROCONAW, GoldenrodCity_Text_MaraDefeat
	goto GoldenrodCity_EventScript_MaraDefeated

GoldenrodCity_EventScript_BattleMaraGrovyle::
	trainerbattle_no_intro TRAINER_MARA_GOLDENROD_GROVYLE, GoldenrodCity_Text_MaraDefeat
	goto GoldenrodCity_EventScript_MaraDefeated

GoldenrodCity_EventScript_BattleMaraCombusken::
	trainerbattle_no_intro TRAINER_MARA_GOLDENROD_COMBUSKEN, GoldenrodCity_Text_MaraDefeat
	goto GoldenrodCity_EventScript_MaraDefeated

GoldenrodCity_EventScript_BattleMaraMarshtomp::
	trainerbattle_no_intro TRAINER_MARA_GOLDENROD_MARSHTOMP, GoldenrodCity_Text_MaraDefeat
	goto GoldenrodCity_EventScript_MaraDefeated

GoldenrodCity_EventScript_MaraDefeated::
	setflag FLAG_DEFEATED_MARA_GOLDENROD
	setvar VAR_GOLDENROD_MARA_STATE, GOLDENROD_MARA_STATE_DONE
	special PokeLink_RefreshWorldState
	msgbox GoldenrodCity_Text_MaraAfterWin, MSGBOX_DEFAULT
	fadedefaultbgm
	goto_if_eq VAR_0x8009, 1, GoldenrodCity_EventScript_MaraReleaseAll
	release
	end

GoldenrodCity_EventScript_MaraDeclined::
	msgbox GoldenrodCity_Text_MaraDeclined, MSGBOX_DEFAULT
	fadedefaultbgm
	goto_if_eq VAR_0x8009, 1, GoldenrodCity_EventScript_MaraReleaseAll
	release
	end

GoldenrodCity_EventScript_MaraReleaseAll::
	releaseall
	end

GoldenrodCity_EventScript_MaraAfterBattle::
	msgbox GoldenrodCity_Text_MaraAfterBattle, MSGBOX_DEFAULT
	release
	end

GoldenrodCity_EventScript_MaraBeforeRadio::
	msgbox GoldenrodCity_Text_MaraBeforeRadio, MSGBOX_DEFAULT
	release
	end

GoldenrodCity_EventScript_DeptStoreShopper::
	msgbox GoldenrodCity_Text_DeptStoreShopper, MSGBOX_NPC
	end

GoldenrodCity_EventScript_GameCornerRegular::
	msgbox GoldenrodCity_Text_GameCornerRegular, MSGBOX_NPC
	end

GoldenrodCity_EventScript_RadioFan::
	msgbox GoldenrodCity_Text_RadioFan, MSGBOX_NPC
	end

GoldenrodCity_EventScript_BikeShopKid::
	msgbox GoldenrodCity_Text_BikeShopKid, MSGBOX_NPC
	end

GoldenrodCity_EventScript_StationCommuter::
	msgbox GoldenrodCity_Text_StationCommuter, MSGBOX_NPC
	end

GoldenrodCity_EventScript_PokeCenterSign::
	msgbox GoldenrodCity_Text_PokeCenterSign, MSGBOX_SIGN
	end

GoldenrodCity_Text_PokeCenterSign::
	.string "POKEMON CENTER$"

GoldenrodCity_EventScript_DeptStoreSign::
	msgbox GoldenrodCity_Text_DeptStoreSign, MSGBOX_SIGN
	end

GoldenrodCity_Text_DeptStoreSign::
	.string "Full Selection of\n"
	.string "POKEMON Goods!\p"
	.string "GOLDENROD CITY\n"
	.string "DEPT. STORE$"

GoldenrodCity_EventScript_RadioTowerSign::
	msgbox GoldenrodCity_Text_RadioTowerSign, MSGBOX_SIGN
	end

GoldenrodCity_Text_RadioTowerSign::
	.string "GOLDENROD RADIO TOWER$"

GoldenrodCity_EventScript_GameCornerSign::
	msgbox GoldenrodCity_Text_GameCornerSign, MSGBOX_SIGN
	end

GoldenrodCity_Text_GameCornerSign::
	.string "Your Playground!\p"
	.string "GOLDENROD CITY\n"
	.string "GAME CORNER$"

GoldenrodCity_EventScript_BikeShopSign::
	msgbox GoldenrodCity_Text_BikeShopSign, MSGBOX_SIGN
	end

GoldenrodCity_Text_BikeShopSign::
	.string "The World is a\n"
	.string "Cycle Path!\n"
	.string "BIKE SHOP$"

GoldenrodCity_EventScript_TrainStationSign::
	msgbox GoldenrodCity_Text_TrainStationSign, MSGBOX_SIGN
	end

GoldenrodCity_Text_TrainStationSign::
	.string "GOLDENROD STATION$"

GoldenrodCity_EventScript_FlowerShopSign::
	msgbox GoldenrodCity_Text_FlowerShopSign, MSGBOX_SIGN
	end

GoldenrodCity_Text_FlowerShopSign::
	.string "Blooming Beautiful\n"
	.string "FLOWER SHOP$"

GoldenrodCity_EventScript_LeagueOfficeSign::
	msgbox GoldenrodCity_Text_LeagueOfficeSign, MSGBOX_SIGN
	end

@ @dialogue id=GOLDENROD_LEAGUE_OFFICE_SIGN map=GoldenrodCity speaker=Sign context=LeagueOffice tags=Goldenrod,League
GoldenrodCity_Text_LeagueOfficeSign::
	.string "GOLDENROD LEAGUE OFFICE\p"
	.string "Badges checked here!$"

GoldenrodCity_Text_DeptStoreShopper::
	.string "The DEPT. STORE has\n"
	.string "everything except restraint.\p"
	.string "That is apparently my\n"
	.string "problem to bring.$"

@ @dialogue id=GOLDENROD_GAME_CORNER_REGULAR map=GoldenrodCity speaker=Regular context=GameCorner tags=Goldenrod,GameCorner
GoldenrodCity_Text_GameCornerRegular::
	.string "The GAME CORNER is loud,\n"
	.string "but nobody misses a payout.$"

GoldenrodCity_Text_RadioFan::
	.string "RADIO TOWER jingles get\n"
	.string "stuck in your head.\p"
	.string "That is how you know\n"
	.string "they worked.$"

GoldenrodCity_Text_BikeShopKid::
	.string "A fast BIKE makes the\n"
	.string "whole city feel smaller.$"

GoldenrodCity_Text_StationCommuter::
	.string "The train schedule is\n"
	.string "perfect until you are late.$"

GoldenrodCity_Text_MaraIntro::
	.string "There you are!\p"
	.string "GOLDENROD is huge.\n"
	.string "I got lost twice.\p"
	.string "Then I found snacks.\n"
	.string "So it worked out.$"

GoldenrodCity_Text_MaraIntroAfterWhitney::
	.string "PLAIN BADGE?\p"
	.string "I knew you'd get it.\p"
	.string "I still wanted to\n"
	.string "say it first.$"

GoldenrodCity_Text_MaraAskBattle::
	.string "Want to compare notes?\p"
	.string "By battling, obviously.$"

GoldenrodCity_Text_MaraYes::
	.string "Good!\p"
	.string "I wanted to see how\n"
	.string "much better you got.$"

GoldenrodCity_Text_MaraDefeat::
	.string "Yep.\n"
	.string "That was better.$"

GoldenrodCity_Text_MaraAfterWin::
	.string "I'm learning.\p"
	.string "You're learning faster.\p"
	.string "Annoying, but good.$"

GoldenrodCity_Text_MaraDeclined::
	.string "Okay!\p"
	.string "I'll be near here.\n"
	.string "Still lost.$"

GoldenrodCity_Text_MaraAfterBattle::
	.string "I found the POKEMON\n"
	.string "CENTER.\p"
	.string "That counts as progress.$"

GoldenrodCity_Text_MaraBeforeRadio::
	.string "The RADIO TOWER is loud.\p"
	.string "Useful, probably.\n"
	.string "Still loud.$"
