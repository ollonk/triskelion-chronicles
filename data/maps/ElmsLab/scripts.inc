.set LOCALID_ELM, 1
.set LOCALID_OAK, 2
.set LOCALID_BIRCH, 3
.set LOCALID_AIDE, 4
.set LOCALID_BALL_GRASS, 5
.set LOCALID_BALL_FIRE, 6
.set LOCALID_BALL_WATER, 7

ElmsLab_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ElmsLab_OnTransition
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, ElmsLab_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, ElmsLab_OnFrame
	.byte 0

ElmsLab_OnTransition:
	goto_if_set FLAG_GOT_ELMS_LAB_STARTER, ElmsLab_EventScript_SetPostStarterLab
	goto_if_eq VAR_ELMS_LAB_STARTER_STATE, 0, ElmsLab_EventScript_EndMapScript
	clearflag FLAG_HIDE_ELMS_LAB_ELM
	call ElmsLab_EventScript_SetPostIntroLab
	end

ElmsLab_EventScript_SetPostStarterLab::
	goto_if_set FLAG_SYS_POKEDEX_GET, ElmsLab_EventScript_SetPostPokedexLab
	setvar VAR_ELMS_LAB_STARTER_STATE, 4
	clearflag FLAG_HIDE_ELMS_LAB_ELM
	call ElmsLab_EventScript_SetPostIntroLab
	end

ElmsLab_EventScript_SetPostPokedexLab::
	setvar VAR_ELMS_LAB_STARTER_STATE, 5
	clearflag FLAG_HIDE_ELMS_LAB_ELM
	call ElmsLab_EventScript_SetStarterChoiceLab
	end

ElmsLab_EventScript_EndMapScript::
	setflag FLAG_HIDE_ELMS_LAB_ELM
	end

ElmsLab_EventScript_SetPostIntroLab::
	call ElmsLab_EventScript_SetStarterChoiceLab
	return

ElmsLab_EventScript_SetStarterChoiceLab::
	setobjectxyperm LOCALID_OAK, 12, 11
	setobjectmovementtype LOCALID_OAK, MOVEMENT_TYPE_FACE_DOWN
	setobjectxyperm LOCALID_ELM, 14, 11
	setobjectmovementtype LOCALID_ELM, MOVEMENT_TYPE_FACE_DOWN
	setobjectxyperm LOCALID_BIRCH, 16, 11
	setobjectmovementtype LOCALID_BIRCH, MOVEMENT_TYPE_FACE_DOWN
	return

ElmsLab_OnWarp:
	map_script_2 VAR_ELMS_LAB_STARTER_STATE, 0, ElmsLab_EventScript_SetIntroFacing
	.2byte 0

ElmsLab_EventScript_SetIntroFacing::
	turnobject OBJ_EVENT_ID_PLAYER, DIR_NORTH
	turnobject LOCALID_OAK, DIR_SOUTH
	turnobject LOCALID_BIRCH, DIR_SOUTH
	end

ElmsLab_OnFrame:
	map_script_2 VAR_ELMS_LAB_STARTER_STATE, 0, ElmsLab_EventScript_BeginSurpriseScene
	.2byte 0

ElmsLab_EventScript_BeginSurpriseScene::
	lockall
	setvar VAR_ELMS_LAB_STARTER_STATE, 1
	delay 20
	playse SE_PIN
	applymovement LOCALID_OAK, Common_Movement_ExclamationMark
	applymovement LOCALID_BIRCH, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox ElmsLab_Text_OakBirchSurprise, MSGBOX_DEFAULT
	msgbox ElmsLab_Text_WrongPerson, MSGBOX_DEFAULT
	closemessage
	delay 40
	applymovement OBJ_EVENT_ID_PLAYER, ElmsLab_Movement_PlayerStepInside
	waitmovement 0
	clearflag FLAG_HIDE_ELMS_LAB_ELM
	addobject LOCALID_ELM
	setobjectxyperm LOCALID_ELM, 13, 21
	setobjectmovementtype LOCALID_ELM, MOVEMENT_TYPE_FACE_UP
	applymovement LOCALID_ELM, ElmsLab_Movement_ElmEnterBehindPlayer
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	applymovement LOCALID_ELM, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	msgbox ElmsLab_Text_ElmSeesOak, MSGBOX_DEFAULT
	applymovement LOCALID_ELM, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox ElmsLab_Text_ElmSeesBirch, MSGBOX_DEFAULT
	applymovement LOCALID_ELM, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox ElmsLab_Text_ElmBirthdayRuined, MSGBOX_DEFAULT
	msgbox ElmsLab_Text_ElmIntroducesPlayer, MSGBOX_DEFAULT
	msgbox ElmsLab_Text_BirchHasHoennStarters, MSGBOX_DEFAULT
	msgbox ElmsLab_Text_OakNotOutdone, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_OAK, ElmsLab_Movement_OakGetKantoStarters
	waitmovement 0
	playse SE_PC_ON
	waitse
	delay 20
	applymovement LOCALID_OAK, ElmsLab_Movement_OakReturnFromPC
	waitmovement 0
	msgbox ElmsLab_Text_ElmComeBack, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_OAK, ElmsLab_Movement_OakGatherForStarterChoice
	waitmovement 0
	applymovement LOCALID_BIRCH, ElmsLab_Movement_BirchGatherForStarterChoice
	waitmovement 0
	applymovement LOCALID_ELM, ElmsLab_Movement_ElmGatherForStarterChoice
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, ElmsLab_Movement_PlayerFollowToStarterChoice
	waitmovement 0
	call ElmsLab_EventScript_SetStarterChoiceLab
	msgbox ElmsLab_Text_ElmNineChoices, MSGBOX_DEFAULT
	applymovement LOCALID_ELM, ElmsLab_Movement_ElmSetVisible
	waitmovement 0
	releaseall
	end

ElmsLab_Movement_PlayerStepInside:
	walk_up
	walk_up
	walk_left
	step_end

ElmsLab_Movement_ElmEnterBehindPlayer:
	set_visible
	walk_up
	walk_up
	walk_in_place_faster_up
	step_end

ElmsLab_Movement_ElmSetVisible:
	set_visible
	step_end

ElmsLab_Movement_OakGetKantoStarters:
	walk_down
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_up
	step_end

ElmsLab_Movement_OakReturnFromPC:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_up
	walk_in_place_faster_down
	step_end

ElmsLab_Movement_OakGatherForStarterChoice:
	walk_down
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	walk_in_place_faster_down
	step_end

ElmsLab_Movement_ElmGatherForStarterChoice:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_in_place_faster_down
	step_end

ElmsLab_Movement_BirchGatherForStarterChoice:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_right
	walk_right
	walk_in_place_faster_down
	step_end

ElmsLab_Movement_PlayerFollowToStarterChoice:
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	walk_in_place_faster_up
	step_end

ElmsLab_Movement_ElmWalkToStarterTable:
	walk_in_place_faster_down
	step_end

ElmsLab_Movement_OakWalkToStarterTable:
	walk_in_place_faster_down
	step_end

ElmsLab_Movement_BirchWalkToStarterTable:
	walk_in_place_faster_down
	step_end

ElmsLab_Movement_OakStopPlayerForPokedex:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_in_place_faster_down
	step_end

ElmsLab_Movement_OakReturnAfterPokedex:
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_in_place_faster_down
	step_end

ElmsLab_Movement_AideWalkToPokeBallGift:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_down
	walk_in_place_faster_right
	step_end

ElmsLab_Movement_AideReturnAfterPokeBallGift:
	walk_in_place_faster_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_in_place_faster_down
	step_end

ElmsLab_EventScript_HideStarterBalls::
	removeobject LOCALID_BALL_GRASS
	removeobject LOCALID_BALL_FIRE
	removeobject LOCALID_BALL_WATER
	delay 12
	return

ElmsLab_EventScript_ShowStarterBalls::
	playse SE_SELECT
	addobject LOCALID_BALL_GRASS
	addobject LOCALID_BALL_FIRE
	addobject LOCALID_BALL_WATER
	delay 16
	return

ElmsLab_EventScript_PrepareJohtoStarters::
	applymovement LOCALID_ELM, ElmsLab_Movement_ElmWalkToStarterTable
	waitmovement 0
	call ElmsLab_EventScript_HideStarterBalls
	setvar VAR_ELMS_LAB_STARTER_STATE, 1
	call ElmsLab_EventScript_ShowStarterBalls
	return

ElmsLab_EventScript_PrepareKantoStarters::
	applymovement LOCALID_OAK, ElmsLab_Movement_OakWalkToStarterTable
	waitmovement 0
	call ElmsLab_EventScript_HideStarterBalls
	setvar VAR_ELMS_LAB_STARTER_STATE, 2
	call ElmsLab_EventScript_ShowStarterBalls
	return

ElmsLab_EventScript_PrepareHoennStarters::
	applymovement LOCALID_BIRCH, ElmsLab_Movement_BirchWalkToStarterTable
	waitmovement 0
	call ElmsLab_EventScript_HideStarterBalls
	setvar VAR_ELMS_LAB_STARTER_STATE, 3
	call ElmsLab_EventScript_ShowStarterBalls
	return

ElmsLab_EventScript_ShowElmOffer::
	msgbox ElmsLab_Text_ElmOffersPokemon, MSGBOX_DEFAULT
	release
	end

ElmsLab_EventScript_ShowOakOffer::
	msgbox ElmsLab_Text_OakOffersPokemon, MSGBOX_DEFAULT
	release
	end

ElmsLab_EventScript_ShowBirchOffer::
	msgbox ElmsLab_Text_BirchOffersPokemon, MSGBOX_DEFAULT
	release
	end

ElmsLab_EventScript_Elm::
	lock
	faceplayer
	goto_if_set FLAG_GOT_ELMS_LAB_STARTER, ElmsLab_EventScript_ElmAfterStarter
	goto_if_eq VAR_ELMS_LAB_STARTER_STATE, 1, ElmsLab_EventScript_ShowElmOffer
	call ElmsLab_EventScript_PrepareJohtoStarters
	goto ElmsLab_EventScript_ShowElmOffer

ElmsLab_EventScript_ElmAfterStarter::
	msgbox ElmsLab_Text_ElmAfterStarter, MSGBOX_DEFAULT
	release
	end

ElmsLab_EventScript_Oak::
	lock
	faceplayer
	goto_if_set FLAG_GOT_ELMS_LAB_STARTER, ElmsLab_EventScript_OakAfterStarter
	goto_if_eq VAR_ELMS_LAB_STARTER_STATE, 2, ElmsLab_EventScript_ShowOakOffer
	call ElmsLab_EventScript_PrepareKantoStarters
	goto ElmsLab_EventScript_ShowOakOffer

ElmsLab_EventScript_OakAfterStarter::
	goto_if_unset FLAG_GOT_POKELINK, ElmsLab_EventScript_OakAfterStarterNoPokeLink
	goto_if_set FLAG_POKELINK_APP_DEXNAV, ElmsLab_EventScript_OakAfterStarterDexNavReady
	msgbox ElmsLab_Text_OakAfterStarter, MSGBOX_DEFAULT
	msgbox ElmsLab_Text_OakDexNavSync, MSGBOX_DEFAULT
	setflag FLAG_POKELINK_APP_DEXNAV
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox ElmsLab_Text_DexNavUnlocked, MSGBOX_DEFAULT
	waitfanfare
	release
	end

ElmsLab_EventScript_OakAfterStarterNoPokeLink::
	msgbox ElmsLab_Text_OakAfterStarter, MSGBOX_DEFAULT
	release
	end

ElmsLab_EventScript_OakAfterStarterDexNavReady::
	msgbox ElmsLab_Text_OakAfterStarter, MSGBOX_DEFAULT
	release
	end

ElmsLab_EventScript_Birch::
	lock
	faceplayer
	goto_if_set FLAG_GOT_ELMS_LAB_STARTER, ElmsLab_EventScript_BirchAfterStarter
	goto_if_eq VAR_ELMS_LAB_STARTER_STATE, 3, ElmsLab_EventScript_ShowBirchOffer
	call ElmsLab_EventScript_PrepareHoennStarters
	goto ElmsLab_EventScript_ShowBirchOffer

ElmsLab_EventScript_BirchAfterStarter::
	msgbox ElmsLab_Text_BirchAfterStarter, MSGBOX_DEFAULT
	release
	end

ElmsLab_EventScript_Aide::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_ELMS_LAB_POKE_BALLS, ElmsLab_EventScript_AideAfterStarter
	goto_if_set FLAG_SYS_POKEDEX_GET, ElmsLab_EventScript_AideGivePokeBallsAndRelease
	goto_if_set FLAG_GOT_ELMS_LAB_STARTER, ElmsLab_EventScript_AideAfterStarter
	msgbox ElmsLab_Text_AideGreeting, MSGBOX_DEFAULT
	release
	end

ElmsLab_EventScript_AideAfterStarter::
	msgbox ElmsLab_Text_AideAfterStarter, MSGBOX_DEFAULT
	release
	end

ElmsLab_EventScript_OakStopsPlayerForPokedex::
	lockall
	goto_if_set FLAG_SYS_POKEDEX_GET, ElmsLab_EventScript_OakPokedexDone
	playse SE_PIN
	applymovement LOCALID_OAK, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_OAK, ElmsLab_Movement_OakStopPlayerForPokedex
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox ElmsLab_Text_OakPokedexApology, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message ElmsLab_Text_ReceivedPokedex
	waitmessage
	waitfanfare
	setflag FLAG_SYS_POKEDEX_GET
	special SetUnlockedPokedexFlags
	setflag FLAG_SYS_NATIONAL_DEX
	special EnableNationalPokedex
	setflag FLAG_RECEIVED_POKEDEX_FROM_BIRCH
	setvar VAR_CABLE_CLUB_TUTORIAL_STATE, 1
	msgbox ElmsLab_Text_OakExplainPokedex, MSGBOX_DEFAULT
	applymovement LOCALID_AIDE, ElmsLab_Movement_AideWalkToPokeBallGift
	waitmovement 0
	call ElmsLab_EventScript_AideGivePokeBalls
	applymovement LOCALID_AIDE, ElmsLab_Movement_AideReturnAfterPokeBallGift
	waitmovement 0
	applymovement LOCALID_OAK, ElmsLab_Movement_OakReturnAfterPokedex
	waitmovement 0
	setvar VAR_ELMS_LAB_STARTER_STATE, 5
	releaseall
	end

ElmsLab_EventScript_OakPokedexDone::
	setvar VAR_ELMS_LAB_STARTER_STATE, 5
	releaseall
	end

ElmsLab_EventScript_AideGivePokeBalls::
	msgbox ElmsLab_Text_AideGivesPokeBalls, MSGBOX_DEFAULT
	giveitem ITEM_POKE_BALL, 10
	goto_if_eq VAR_RESULT, FALSE, ElmsLab_EventScript_AideNoRoomForPokeBalls
	setflag FLAG_RECEIVED_ELMS_LAB_POKE_BALLS
	return

ElmsLab_EventScript_AideGivePokeBallsAndRelease::
	call ElmsLab_EventScript_AideGivePokeBalls
	release
	end

ElmsLab_EventScript_AideNoRoomForPokeBalls::
	msgbox ElmsLab_Text_AideNoRoomForPokeBalls, MSGBOX_DEFAULT
	return

ElmsLab_EventScript_GrassBall::
	lock
	goto_if_set FLAG_GOT_ELMS_LAB_STARTER, ElmsLab_EventScript_AlreadyChoseStarter
	switch VAR_ELMS_LAB_STARTER_STATE
	case 2, ElmsLab_EventScript_ConfirmBulbasaur
	case 3, ElmsLab_EventScript_ConfirmTreecko
	goto ElmsLab_EventScript_ConfirmChikorita

ElmsLab_EventScript_FireBall::
	lock
	goto_if_set FLAG_GOT_ELMS_LAB_STARTER, ElmsLab_EventScript_AlreadyChoseStarter
	switch VAR_ELMS_LAB_STARTER_STATE
	case 2, ElmsLab_EventScript_ConfirmCharmander
	case 3, ElmsLab_EventScript_ConfirmTorchic
	goto ElmsLab_EventScript_ConfirmCyndaquil

ElmsLab_EventScript_WaterBall::
	lock
	goto_if_set FLAG_GOT_ELMS_LAB_STARTER, ElmsLab_EventScript_AlreadyChoseStarter
	switch VAR_ELMS_LAB_STARTER_STATE
	case 2, ElmsLab_EventScript_ConfirmSquirtle
	case 3, ElmsLab_EventScript_ConfirmMudkip
	goto ElmsLab_EventScript_ConfirmTotodile

ElmsLab_EventScript_ConfirmBulbasaur::
	showmonpic SPECIES_BULBASAUR, 10, 3
	msgbox ElmsLab_Text_TakeBulbasaur, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ElmsLab_EventScript_CancelSpecies
	setvar VAR_STARTER_MON, 0
	goto ElmsLab_EventScript_GiveBulbasaur

ElmsLab_EventScript_ConfirmCharmander::
	showmonpic SPECIES_CHARMANDER, 10, 3
	msgbox ElmsLab_Text_TakeCharmander, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ElmsLab_EventScript_CancelSpecies
	setvar VAR_STARTER_MON, 1
	goto ElmsLab_EventScript_GiveCharmander

ElmsLab_EventScript_ConfirmSquirtle::
	showmonpic SPECIES_SQUIRTLE, 10, 3
	msgbox ElmsLab_Text_TakeSquirtle, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ElmsLab_EventScript_CancelSpecies
	setvar VAR_STARTER_MON, 2
	goto ElmsLab_EventScript_GiveSquirtle

ElmsLab_EventScript_ConfirmChikorita::
	showmonpic SPECIES_CHIKORITA, 10, 3
	msgbox ElmsLab_Text_TakeChikorita, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ElmsLab_EventScript_CancelSpecies
	setvar VAR_STARTER_MON, 0
	goto ElmsLab_EventScript_GiveChikorita

ElmsLab_EventScript_ConfirmCyndaquil::
	showmonpic SPECIES_CYNDAQUIL, 10, 3
	msgbox ElmsLab_Text_TakeCyndaquil, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ElmsLab_EventScript_CancelSpecies
	setvar VAR_STARTER_MON, 1
	goto ElmsLab_EventScript_GiveCyndaquil

ElmsLab_EventScript_ConfirmTotodile::
	showmonpic SPECIES_TOTODILE, 10, 3
	msgbox ElmsLab_Text_TakeTotodile, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ElmsLab_EventScript_CancelSpecies
	setvar VAR_STARTER_MON, 2
	goto ElmsLab_EventScript_GiveTotodile

ElmsLab_EventScript_ConfirmTreecko::
	showmonpic SPECIES_TREECKO, 10, 3
	msgbox ElmsLab_Text_TakeTreecko, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ElmsLab_EventScript_CancelSpecies
	setvar VAR_STARTER_MON, 0
	goto ElmsLab_EventScript_GiveTreecko

ElmsLab_EventScript_ConfirmTorchic::
	showmonpic SPECIES_TORCHIC, 10, 3
	msgbox ElmsLab_Text_TakeTorchic, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ElmsLab_EventScript_CancelSpecies
	setvar VAR_STARTER_MON, 1
	goto ElmsLab_EventScript_GiveTorchic

ElmsLab_EventScript_ConfirmMudkip::
	showmonpic SPECIES_MUDKIP, 10, 3
	msgbox ElmsLab_Text_TakeMudkip, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ElmsLab_EventScript_CancelSpecies
	setvar VAR_STARTER_MON, 2
	goto ElmsLab_EventScript_GiveMudkip

ElmsLab_EventScript_CancelSpecies::
	hidemonpic
	msgbox ElmsLab_Text_ChooseAnotherProfessor, MSGBOX_DEFAULT
	release
	end

ElmsLab_EventScript_GiveBulbasaur::
	bufferspeciesname STR_VAR_1, SPECIES_BULBASAUR
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_BULBASAUR
	call ElmsLab_EventScript_StoreKantoGrassStarter
	givemon SPECIES_BULBASAUR, 5
	goto ElmsLab_EventScript_FinishGiveStarter

ElmsLab_EventScript_GiveCharmander::
	bufferspeciesname STR_VAR_1, SPECIES_CHARMANDER
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_CHARMANDER
	call ElmsLab_EventScript_StoreKantoFireStarter
	givemon SPECIES_CHARMANDER, 5
	goto ElmsLab_EventScript_FinishGiveStarter

ElmsLab_EventScript_GiveSquirtle::
	bufferspeciesname STR_VAR_1, SPECIES_SQUIRTLE
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_SQUIRTLE
	call ElmsLab_EventScript_StoreKantoWaterStarter
	givemon SPECIES_SQUIRTLE, 5
	goto ElmsLab_EventScript_FinishGiveStarter

ElmsLab_EventScript_GiveChikorita::
	bufferspeciesname STR_VAR_1, SPECIES_CHIKORITA
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_CHIKORITA
	call ElmsLab_EventScript_StoreJohtoGrassStarter
	givemon SPECIES_CHIKORITA, 5
	goto ElmsLab_EventScript_FinishGiveStarter

ElmsLab_EventScript_GiveCyndaquil::
	bufferspeciesname STR_VAR_1, SPECIES_CYNDAQUIL
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_CYNDAQUIL
	call ElmsLab_EventScript_StoreJohtoFireStarter
	givemon SPECIES_CYNDAQUIL, 5
	goto ElmsLab_EventScript_FinishGiveStarter

ElmsLab_EventScript_GiveTotodile::
	bufferspeciesname STR_VAR_1, SPECIES_TOTODILE
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_TOTODILE
	call ElmsLab_EventScript_StoreJohtoWaterStarter
	givemon SPECIES_TOTODILE, 5
	goto ElmsLab_EventScript_FinishGiveStarter

ElmsLab_EventScript_GiveTreecko::
	bufferspeciesname STR_VAR_1, SPECIES_TREECKO
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_TREECKO
	call ElmsLab_EventScript_StoreHoennGrassStarter
	givemon SPECIES_TREECKO, 5
	goto ElmsLab_EventScript_FinishGiveStarter

ElmsLab_EventScript_GiveTorchic::
	bufferspeciesname STR_VAR_1, SPECIES_TORCHIC
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_TORCHIC
	call ElmsLab_EventScript_StoreHoennFireStarter
	givemon SPECIES_TORCHIC, 5
	goto ElmsLab_EventScript_FinishGiveStarter

ElmsLab_EventScript_GiveMudkip::
	bufferspeciesname STR_VAR_1, SPECIES_MUDKIP
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_MUDKIP
	call ElmsLab_EventScript_StoreHoennWaterStarter
	givemon SPECIES_MUDKIP, 5
	goto ElmsLab_EventScript_FinishGiveStarter

ElmsLab_EventScript_StoreKantoGrassStarter::
	setvar VAR_TRISKELION_STARTER_SPECIES, SPECIES_BULBASAUR
	setvar VAR_TRISKELION_STARTER_REGION, STARTER_REGION_KANTO
	setvar VAR_TRISKELION_STARTER_ELEMENT, STARTER_ELEMENT_GRASS
	setflag FLAG_TRISKELION_CHOSE_KANTO_STARTER
	return

ElmsLab_EventScript_StoreKantoFireStarter::
	setvar VAR_TRISKELION_STARTER_SPECIES, SPECIES_CHARMANDER
	setvar VAR_TRISKELION_STARTER_REGION, STARTER_REGION_KANTO
	setvar VAR_TRISKELION_STARTER_ELEMENT, STARTER_ELEMENT_FIRE
	setflag FLAG_TRISKELION_CHOSE_KANTO_STARTER
	return

ElmsLab_EventScript_StoreKantoWaterStarter::
	setvar VAR_TRISKELION_STARTER_SPECIES, SPECIES_SQUIRTLE
	setvar VAR_TRISKELION_STARTER_REGION, STARTER_REGION_KANTO
	setvar VAR_TRISKELION_STARTER_ELEMENT, STARTER_ELEMENT_WATER
	setflag FLAG_TRISKELION_CHOSE_KANTO_STARTER
	return

ElmsLab_EventScript_StoreJohtoGrassStarter::
	setvar VAR_TRISKELION_STARTER_SPECIES, SPECIES_CHIKORITA
	setvar VAR_TRISKELION_STARTER_REGION, STARTER_REGION_JOHTO
	setvar VAR_TRISKELION_STARTER_ELEMENT, STARTER_ELEMENT_GRASS
	setflag FLAG_TRISKELION_CHOSE_JOHTO_STARTER
	return

ElmsLab_EventScript_StoreJohtoFireStarter::
	setvar VAR_TRISKELION_STARTER_SPECIES, SPECIES_CYNDAQUIL
	setvar VAR_TRISKELION_STARTER_REGION, STARTER_REGION_JOHTO
	setvar VAR_TRISKELION_STARTER_ELEMENT, STARTER_ELEMENT_FIRE
	setflag FLAG_TRISKELION_CHOSE_JOHTO_STARTER
	return

ElmsLab_EventScript_StoreJohtoWaterStarter::
	setvar VAR_TRISKELION_STARTER_SPECIES, SPECIES_TOTODILE
	setvar VAR_TRISKELION_STARTER_REGION, STARTER_REGION_JOHTO
	setvar VAR_TRISKELION_STARTER_ELEMENT, STARTER_ELEMENT_WATER
	setflag FLAG_TRISKELION_CHOSE_JOHTO_STARTER
	return

ElmsLab_EventScript_StoreHoennGrassStarter::
	setvar VAR_TRISKELION_STARTER_SPECIES, SPECIES_TREECKO
	setvar VAR_TRISKELION_STARTER_REGION, STARTER_REGION_HOENN
	setvar VAR_TRISKELION_STARTER_ELEMENT, STARTER_ELEMENT_GRASS
	setflag FLAG_TRISKELION_CHOSE_HOENN_STARTER
	return

ElmsLab_EventScript_StoreHoennFireStarter::
	setvar VAR_TRISKELION_STARTER_SPECIES, SPECIES_TORCHIC
	setvar VAR_TRISKELION_STARTER_REGION, STARTER_REGION_HOENN
	setvar VAR_TRISKELION_STARTER_ELEMENT, STARTER_ELEMENT_FIRE
	setflag FLAG_TRISKELION_CHOSE_HOENN_STARTER
	return

ElmsLab_EventScript_StoreHoennWaterStarter::
	setvar VAR_TRISKELION_STARTER_SPECIES, SPECIES_MUDKIP
	setvar VAR_TRISKELION_STARTER_REGION, STARTER_REGION_HOENN
	setvar VAR_TRISKELION_STARTER_ELEMENT, STARTER_ELEMENT_WATER
	setflag FLAG_TRISKELION_CHOSE_HOENN_STARTER
	return

ElmsLab_EventScript_FinishGiveStarter::
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, ElmsLab_EventScript_StarterToParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, ElmsLab_EventScript_StarterToPC
	hidemonpic
	goto Common_EventScript_NoMoreRoomForPokemon

ElmsLab_EventScript_StarterToParty::
	call ElmsLab_EventScript_ReceivedStarter
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ElmsLab_EventScript_StarterReceived
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto ElmsLab_EventScript_StarterReceived

ElmsLab_EventScript_StarterToPC::
	call ElmsLab_EventScript_ReceivedStarter
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ElmsLab_EventScript_StarterTransferredToPC
	call Common_EventScript_NameReceivedBoxMon
	goto ElmsLab_EventScript_StarterTransferredToPC

ElmsLab_EventScript_StarterTransferredToPC::
	call Common_EventScript_TransferredToPC
	goto ElmsLab_EventScript_StarterReceived

ElmsLab_EventScript_ReceivedStarter::
	hidemonpic
	playfanfare MUS_OBTAIN_ITEM
	message ElmsLab_Text_ReceivedStarter
	waitmessage
	waitfanfare
	return

ElmsLab_EventScript_StarterReceived::
	setflag FLAG_SYS_POKEMON_GET
	setflag FLAG_RESCUED_BIRCH
	setflag FLAG_GOT_ELMS_LAB_STARTER
	clearflag FLAG_HIDE_POKELINK_PICKUP
	setvar VAR_ELMS_LAB_STARTER_STATE, 4
	setvar VAR_ACT1_STORY_STAGE, ACT1_STAGE_STARTER_RECEIVED
	special PokeLink_RefreshWorldState
	msgbox ElmsLab_Text_StarterReceived, MSGBOX_DEFAULT
	release
	end

ElmsLab_EventScript_AlreadyChoseStarter::
	msgbox ElmsLab_Text_AlreadyChoseStarter, MSGBOX_DEFAULT
	release
	end

ElmsLab_EventScript_PC::
	msgbox ElmsLab_Text_PC, MSGBOX_SIGN
	end

ElmsLab_EventScript_Bookshelf::
	msgbox ElmsLab_Text_Bookshelf, MSGBOX_SIGN
	end

ElmsLab_EventScript_HealingMachine::
	lockall
	msgbox ElmsLab_Text_HealingMachine, MSGBOX_DEFAULT
	closemessage
	call Common_EventScript_OutOfCenterPartyHeal
	releaseall
	end

ElmsLab_Text_OakBirchSurprise:
	.string "OAK: Surprise!\p"
	.string "BIRCH: Happy birthday!$"

ElmsLab_Text_WrongPerson:
	.string "OAK: Oh! Pardon me.\p"
	.string "BIRCH: You're not ELM, are you?$"

ElmsLab_Text_ElmSeesOak:
	.string "ELM: PROF. OAK?$"

ElmsLab_Text_ElmSeesBirch:
	.string "ELM: PROF. BIRCH?$"

ElmsLab_Text_ElmBirthdayRuined:
	.string "ELM: What are you two doing here?\p"
	.string "Balloons? Streamers?\n"
	.string "Oh! That's right!\p"
	.string "My birthday came around again.\n"
	.string "I completely forgot!\p"
	.string "Thank you both for coming all\n"
	.string "this way.$"

ElmsLab_Text_ElmIntroducesPlayer:
	.string "ELM: This is {PLAYER}.\p"
	.string "{PLAYER} is just about to set out\n"
	.string "with a first POKEMON.$"

ElmsLab_Text_BirchHasHoennStarters:
	.string "BIRCH: Then this is perfect!\p"
	.string "I brought three POKEMON from\n"
	.string "HOENN. They're lively ones!$"

ElmsLab_Text_OakNotOutdone:
	.string "OAK: And the PC network should\n"
	.string "let me retrieve three from KANTO.\p"
	.string "ELM, I hope you don't mind.\n"
	.string "I'll use your PC for a moment.$"

ElmsLab_Text_ElmComeBack:
	.string "ELM: Excellent!\p"
	.string "{PLAYER}, come back here with us.\n"
	.string "We'll set everything out together.$"

ElmsLab_Text_ElmNineChoices:
	.string "ELM: Three professors in one lab\n"
	.string "is a little crowded, yes.\p"
	.string "OAK: But the LEAGUE is changing.\n"
	.string "JOHTO, KANTO, and HOENN are\l"
	.string "not as separate as they were.\p"
	.string "BIRCH: More fieldwork! More\n"
	.string "habitats! More chances to get\l"
	.string "chased by wild POKEMON!\p"
	.string "ELM: And more responsibility\n"
	.string "for new TRAINERS.\p"
	.string "OAK: Eight sanctioned BADGES\n"
	.string "qualify a TRAINER for the first\l"
	.string "Champion Assessment.\p"
	.string "BIRCH: After that, tournament\n"
	.string "people start saying things like\l"
	.string "interregional liability.\p"
	.string "ELM: For now, choose carefully.\n"
	.string "Your first partner will matter.\p"
	.string "Talk to one of us, then check the\n"
	.string "POKE BALLS on the table.$"

ElmsLab_Text_ElmAfterStarter:
	.string "ELM: Take good care of that\n"
	.string "POKEMON.\p"
	.string "I'm sure it will grow wonderfully\n"
	.string "with you.$"

ElmsLab_Text_OakOffersPokemon:
	.string "OAK: Very well.\p"
	.string "I've retrieved BULBASAUR,\n"
	.string "CHARMANDER, and SQUIRTLE.$"

ElmsLab_Text_OakAfterStarter:
	.string "OAK: Your journey is beginning.\p"
	.string "Meet many POKEMON, and learn\n"
	.string "all you can.$"

ElmsLab_Text_OakDexNavSync:
	.string "OAK: Your POKEDEX can use\n"
	.string "a field-search module now.\p"
	.string "I'll activate DEXNAV.$"

ElmsLab_Text_DexNavUnlocked:
	.string "DEXNAV app unlocked!$"

ElmsLab_Text_BirchOffersPokemon:
	.string "BIRCH: How about HOENN's three?\p"
	.string "TREECKO, TORCHIC, and MUDKIP\n"
	.string "are all fine partners.$"

ElmsLab_Text_BirchAfterStarter:
	.string "BIRCH: Take that POKEMON out into\n"
	.string "the grass now and then.\p"
	.string "That's where you'll really get to\n"
	.string "know each other.$"

ElmsLab_Text_ElmOffersPokemon:
	.string "ELM: Then please look at JOHTO's\n"
	.string "three POKEMON.\p"
	.string "CHIKORITA, CYNDAQUIL, and\n"
	.string "TOTODILE are ready.$"

ElmsLab_Text_AideGreeting:
	.string "There are three POKE BALLS on\n"
	.string "the table.\p"
	.string "Speak with a professor, and the\n"
	.string "POKE BALLS will be prepared.$"

ElmsLab_Text_AideAfterStarter:
	.string "The PROF. looks happy.\p"
	.string "He says he forgot his own birthday\n"
	.string "again this year.$"

ElmsLab_Text_ChooseAnotherProfessor:
	.string "You can check the other POKE BALLS,\n"
	.string "or speak with another professor.$"

ElmsLab_Text_TakeBulbasaur:
	.string "Would you like BULBASAUR,\n"
	.string "the SEED POKEMON?$"

ElmsLab_Text_TakeCharmander:
	.string "Would you like CHARMANDER,\n"
	.string "the LIZARD POKEMON?$"

ElmsLab_Text_TakeSquirtle:
	.string "Would you like SQUIRTLE,\n"
	.string "the TINY TURTLE POKEMON?$"

ElmsLab_Text_TakeChikorita:
	.string "Would you like CHIKORITA,\n"
	.string "the LEAF POKEMON?$"

ElmsLab_Text_TakeCyndaquil:
	.string "Would you like CYNDAQUIL,\n"
	.string "the FIRE MOUSE POKEMON?$"

ElmsLab_Text_TakeTotodile:
	.string "Would you like TOTODILE,\n"
	.string "the BIG JAW POKEMON?$"

ElmsLab_Text_TakeTreecko:
	.string "Would you like TREECKO,\n"
	.string "the WOOD GECKO POKEMON?$"

ElmsLab_Text_TakeTorchic:
	.string "Would you like TORCHIC,\n"
	.string "the CHICK POKEMON?$"

ElmsLab_Text_TakeMudkip:
	.string "Would you like MUDKIP,\n"
	.string "the MUD FISH POKEMON?$"

ElmsLab_Text_ReceivedStarter:
	.string "{PLAYER} received {STR_VAR_1}!$"

ElmsLab_Text_StarterReceived:
	.string "ELM: A wonderful choice!\p"
	.string "Take good care of that\n"
	.string "POKEMON.\p"
	.string "Your journey should begin\n"
	.string "while the day is young.$"

ElmsLab_Text_AlreadyChoseStarter:
	.string "The remaining POKEMON are resting\n"
	.string "inside their POKE BALLS.$"

ElmsLab_Text_OakPokedexApology:
	.string "OAK: Ah, {PLAYER}, one moment!\p"
	.string "I came all this way and still had\n"
	.string "to use ELM's PC network.\p"
	.string "As an apology for being so\n"
	.string "unprepared, please take this.$"

ElmsLab_Text_ReceivedPokedex:
	.string "{PLAYER} received the POKEDEX!$"

ElmsLab_Text_OakExplainPokedex:
	.string "OAK: That POKEDEX records data on\n"
	.string "POKEMON you meet or catch.\p"
	.string "I've set it for wider listings,\n"
	.string "so your travels should be covered.$"

ElmsLab_Text_AideGivesPokeBalls:
	.string "AIDE: If you're filling a POKEDEX,\n"
	.string "you'll need these, too.$"

ElmsLab_Text_AideNoRoomForPokeBalls:
	.string "AIDE: Oh! Your BAG is full.\p"
	.string "Please make room, then come back\n"
	.string "for these POKE BALLS.$"

ElmsLab_Text_PC:
	.string "The PC is full of research notes\n"
	.string "about starter POKEMON.$"

ElmsLab_Text_Bookshelf:
	.string "Books about POKEMON habitats are\n"
	.string "packed onto the shelf.$"

ElmsLab_Text_HealingMachine:
	.string "You placed your POKEMON in the\n"
	.string "healing machine.$"
