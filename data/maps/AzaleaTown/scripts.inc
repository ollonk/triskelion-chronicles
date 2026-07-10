.set LOCALID_DORIAN, 5

AzaleaTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, AzaleaTown_OnTransition
	.byte 0

AzaleaTown_OnTransition:
	hideobjectat LOCALID_DORIAN, MAP_AZALEA_TOWN
	setvar VAR_AZALEA_DORIAN_STATE, DORIAN_AZALEA_STATE_HIDDEN
	goto_if_set FLAG_DEFEATED_DORIAN_AZALEA, AzaleaTown_OnTransitionShowDorianDone
	goto_if_unset FLAG_ROCKET_ILEX_EVENT_CLEARED, AzaleaTown_OnTransitionRefresh
	goto_if_set FLAG_BADGE02_GET, AzaleaTown_OnTransitionRefresh
	setvar VAR_AZALEA_DORIAN_STATE, DORIAN_AZALEA_STATE_READY
	showobjectat LOCALID_DORIAN, MAP_AZALEA_TOWN
	goto AzaleaTown_OnTransitionRefresh

AzaleaTown_OnTransitionShowDorianDone:
	setvar VAR_AZALEA_DORIAN_STATE, DORIAN_AZALEA_STATE_DONE
	showobjectat LOCALID_DORIAN, MAP_AZALEA_TOWN

AzaleaTown_OnTransitionRefresh:
	special PokeLink_RefreshWorldState
	end

AzaleaTown_EventScript_DorianTrigger::
	setvar VAR_0x8009, 1
	lockall
	playbgm MUS_RG_ENCOUNTER_RIVAL, TRUE
	applymovement LOCALID_DORIAN, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_DORIAN, Common_Movement_Delay48
	waitmovement 0
	applymovement LOCALID_DORIAN, Common_Movement_FaceRight
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	goto AzaleaTown_EventScript_DorianFirstBattle

AzaleaTown_EventScript_Dorian::
	setvar VAR_0x8009, 0
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_DORIAN_AZALEA, AzaleaTown_EventScript_DorianAfterBattle
	goto_if_unset FLAG_ROCKET_ILEX_EVENT_CLEARED, AzaleaTown_EventScript_DorianTooEarly
	goto_if_set FLAG_BADGE02_GET, AzaleaTown_EventScript_DorianTooLate
	playbgm MUS_RG_ENCOUNTER_RIVAL, TRUE
	goto AzaleaTown_EventScript_DorianFirstBattle

AzaleaTown_EventScript_DorianFirstBattle::
	setflag FLAG_MET_DORIAN_AZALEA
	msgbox AzaleaTown_Text_DorianIntro, MSGBOX_DEFAULT
	msgbox AzaleaTown_Text_DorianBeforeBattle, MSGBOX_DEFAULT
	switch VAR_TRISKELION_STARTER_SPECIES
	case SPECIES_BULBASAUR, AzaleaTown_EventScript_BattleDorianCharmander
	case SPECIES_CHARMANDER, AzaleaTown_EventScript_BattleDorianSquirtle
	case SPECIES_SQUIRTLE, AzaleaTown_EventScript_BattleDorianBulbasaur
	case SPECIES_CHIKORITA, AzaleaTown_EventScript_BattleDorianCyndaquil
	case SPECIES_CYNDAQUIL, AzaleaTown_EventScript_BattleDorianTotodile
	case SPECIES_TOTODILE, AzaleaTown_EventScript_BattleDorianChikorita
	case SPECIES_TREECKO, AzaleaTown_EventScript_BattleDorianTorchic
	case SPECIES_TORCHIC, AzaleaTown_EventScript_BattleDorianMudkip
	case SPECIES_MUDKIP, AzaleaTown_EventScript_BattleDorianTreecko
	goto AzaleaTown_EventScript_BattleDorianCyndaquil

@ Dorian takes the same-region starter with type advantage over the player.
AzaleaTown_EventScript_BattleDorianBulbasaur::
	trainerbattle_no_intro TRAINER_DORIAN_AZALEA_BULBASAUR, AzaleaTown_Text_DorianDefeat
	goto AzaleaTown_EventScript_DorianDefeated

AzaleaTown_EventScript_BattleDorianCharmander::
	trainerbattle_no_intro TRAINER_DORIAN_AZALEA_CHARMANDER, AzaleaTown_Text_DorianDefeat
	goto AzaleaTown_EventScript_DorianDefeated

AzaleaTown_EventScript_BattleDorianSquirtle::
	trainerbattle_no_intro TRAINER_DORIAN_AZALEA_SQUIRTLE, AzaleaTown_Text_DorianDefeat
	goto AzaleaTown_EventScript_DorianDefeated

AzaleaTown_EventScript_BattleDorianChikorita::
	trainerbattle_no_intro TRAINER_DORIAN_AZALEA_CHIKORITA, AzaleaTown_Text_DorianDefeat
	goto AzaleaTown_EventScript_DorianDefeated

AzaleaTown_EventScript_BattleDorianCyndaquil::
	trainerbattle_no_intro TRAINER_DORIAN_AZALEA_CYNDAQUIL, AzaleaTown_Text_DorianDefeat
	goto AzaleaTown_EventScript_DorianDefeated

AzaleaTown_EventScript_BattleDorianTotodile::
	trainerbattle_no_intro TRAINER_DORIAN_AZALEA_TOTODILE, AzaleaTown_Text_DorianDefeat
	goto AzaleaTown_EventScript_DorianDefeated

AzaleaTown_EventScript_BattleDorianTreecko::
	trainerbattle_no_intro TRAINER_DORIAN_AZALEA_TREECKO, AzaleaTown_Text_DorianDefeat
	goto AzaleaTown_EventScript_DorianDefeated

AzaleaTown_EventScript_BattleDorianTorchic::
	trainerbattle_no_intro TRAINER_DORIAN_AZALEA_TORCHIC, AzaleaTown_Text_DorianDefeat
	goto AzaleaTown_EventScript_DorianDefeated

AzaleaTown_EventScript_BattleDorianMudkip::
	trainerbattle_no_intro TRAINER_DORIAN_AZALEA_MUDKIP, AzaleaTown_Text_DorianDefeat
	goto AzaleaTown_EventScript_DorianDefeated

AzaleaTown_EventScript_DorianDefeated::
	setflag FLAG_DEFEATED_DORIAN_AZALEA
	setvar VAR_AZALEA_DORIAN_STATE, DORIAN_AZALEA_STATE_DONE
	special PokeLink_RefreshWorldState
	msgbox AzaleaTown_Text_DorianAfterWin, MSGBOX_DEFAULT
	fadedefaultbgm
	goto_if_eq VAR_0x8009, 1, AzaleaTown_EventScript_DorianReleaseAll
	release
	end

AzaleaTown_EventScript_DorianReleaseAll::
	releaseall
	end

AzaleaTown_EventScript_DorianAfterBattle::
	msgbox AzaleaTown_Text_DorianAfterBattle, MSGBOX_DEFAULT
	release
	end

AzaleaTown_EventScript_DorianTooEarly::
	msgbox AzaleaTown_Text_DorianTooEarly, MSGBOX_DEFAULT
	release
	end

AzaleaTown_EventScript_DorianTooLate::
	msgbox AzaleaTown_Text_DorianTooLate, MSGBOX_DEFAULT
	release
	end

AzaleaTown_EventScript_ApricornBoy::
	msgbox AzaleaTown_Text_ApricornBoy, MSGBOX_NPC
	end

AzaleaTown_EventScript_WellNeighbor::
	msgbox AzaleaTown_Text_WellNeighbor, MSGBOX_NPC
	end

AzaleaTown_EventScript_Slowpoke::
	msgbox AzaleaTown_Text_Slowpoke, MSGBOX_NPC
	end

AzaleaTown_EventScript_GymFan::
	msgbox AzaleaTown_Text_GymFan, MSGBOX_NPC
	end

AzaleaTown_EventScript_TownSign::
	msgbox AzaleaTown_Text_TownSign, MSGBOX_SIGN
	end

AzaleaTown_Text_TownSign::
	.string "AZALEA TOWN\n"
	.string "Where People and POKEMON\n"
	.string "Live in Happy Harmony$"

AzaleaTown_EventScript_PokeCenterSign::
	msgbox AzaleaTown_Text_PokeCenterSign, MSGBOX_SIGN
	end

AzaleaTown_Text_PokeCenterSign::
	.string "POKEMON CENTER$"

AzaleaTown_EventScript_MartSign::
	msgbox AzaleaTown_Text_MartSign, MSGBOX_SIGN
	end

AzaleaTown_Text_MartSign::
	.string "POKEMON MART$"

AzaleaTown_EventScript_SlowpokeWellSign::
	msgbox AzaleaTown_Text_SlowpokeWellSign, MSGBOX_SIGN
	end

AzaleaTown_Text_SlowpokeWellSign::
	.string "SLOWPOKE WELL\p"
	.string "Also known as the\n"
	.string "RAINMAKER WELL.\p"
	.string "Locals believe that a\n"
	.string "SLOWPOKE's yawn summons rain.\p"
	.string "Records show that a\n"
	.string "SLOWPOKE's yawn ended a\n"
	.string "drought 400 years ago.$"

AzaleaTown_Text_ApricornBoy::
	.string "KURT says a good BALL\n"
	.string "starts with patience.\p"
	.string "I am better at the\n"
	.string "waiting than the carving.$"

AzaleaTown_Text_WellNeighbor::
	.string "When SLOWPOKE nap near\n"
	.string "the well, everyone lowers\l"
	.string "their voice.$"

AzaleaTown_Text_Slowpoke::
	.string "SLOWPOKE: ... ...Yawn?$"

AzaleaTown_Text_GymFan::
	.string "BUGSY studies tiny wings\n"
	.string "and tiny legs.\p"
	.string "Then somehow those tiny\n"
	.string "things beat you.$"

AzaleaTown_Text_DorianIntro::
	.string "You're the NEW BARK kid.\p"
	.string "I expected taller.\p"
	.string "I'm DORIAN.\n"
	.string "I check the people getting\l"
	.string "ahead of me.\p"
	.string "So far?\n"
	.string "Bad list.$"

AzaleaTown_Text_DorianBeforeBattle::
	.string "Try to make this useful.$"

AzaleaTown_Text_DorianDefeat::
	.string "Hmph.\n"
	.string "Bad read.$"

AzaleaTown_Text_DorianAfterWin::
	.string "You're not polished.\p"
	.string "That's the annoying part.\p"
	.string "BUGSY keeps notes.\n"
	.string "Try not to be an easy page.$"

AzaleaTown_Text_DorianAfterBattle::
	.string "Still here?\n"
	.string "Good.\p"
	.string "I dislike being wrong twice.$"

AzaleaTown_Text_DorianTooEarly::
	.string "Team Rocket?\p"
	.string "Clear the forest first.\n"
	.string "Then we can talk rankings.$"

AzaleaTown_Text_DorianTooLate::
	.string "HIVE BADGE already?\p"
	.string "Fine.\n"
	.string "We compare later.$"
