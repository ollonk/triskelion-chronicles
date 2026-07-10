.set LOCALID_RANGER_ELIAN, 1
.set LOCALID_ROCKET_GRUNT_1, 2
.set LOCALID_ROCKET_GRUNT_2, 3
.set LOCALID_ROCKET_COMMANDER, 4

IlexForest_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, IlexForest_OnTransition
	.byte 0

IlexForest_OnTransition:
	hideobjectat LOCALID_ROCKET_GRUNT_1, MAP_ILEX_FOREST
	hideobjectat LOCALID_ROCKET_GRUNT_2, MAP_ILEX_FOREST
	hideobjectat LOCALID_ROCKET_COMMANDER, MAP_ILEX_FOREST
	goto_if_unset FLAG_BADGE01_GET, IlexForest_OnTransitionRefresh
	goto_if_set FLAG_ROCKET_ILEX_EVENT_CLEARED, IlexForest_OnTransitionRefresh
	setflag FLAG_ROCKET_ILEX_EVENT_STARTED
	setvar VAR_ACT1_STORY_STAGE, ACT1_STAGE_ILEX_STARTED
	goto_if_set FLAG_ROCKET_ILEX_GRUNT_1_DEFEATED, IlexForest_OnTransitionSkipGrunt1
	showobjectat LOCALID_ROCKET_GRUNT_1, MAP_ILEX_FOREST
IlexForest_OnTransitionSkipGrunt1:
	goto_if_set FLAG_ROCKET_ILEX_GRUNT_2_DEFEATED, IlexForest_OnTransitionSkipGrunt2
	showobjectat LOCALID_ROCKET_GRUNT_2, MAP_ILEX_FOREST
IlexForest_OnTransitionSkipGrunt2:
	showobjectat LOCALID_ROCKET_COMMANDER, MAP_ILEX_FOREST
IlexForest_OnTransitionRefresh:
	special PokeLink_RefreshWorldState
	end

IlexForest_EventScript_RangerElian::
	lock
	faceplayer
	goto_if_unset FLAG_BADGE01_GET, IlexForest_EventScript_RangerBeforeBadge
	goto_if_unset FLAG_ROCKET_ILEX_EVENT_CLEARED, IlexForest_EventScript_RangerDuringRocket
	goto_if_set FLAG_POKELINK_APP_ENCOUNTERS, IlexForest_EventScript_RangerAfterApp
	msgbox IlexForest_Text_RangerEncountersIntro, MSGBOX_DEFAULT
	setflag FLAG_POKELINK_APP_ENCOUNTERS
	setflag FLAG_POKELINK_AREA_ILEX_FOREST
	setvar VAR_ACT1_STORY_STAGE, ACT1_STAGE_ILEX_CLEARED
	special PokeLink_RefreshWorldState
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox IlexForest_Text_EncountersUnlocked, MSGBOX_DEFAULT
	waitfanfare
	release
	end

IlexForest_EventScript_RangerBeforeBadge::
	msgbox IlexForest_Text_RangerBeforeBadge, MSGBOX_DEFAULT
	release
	end

IlexForest_EventScript_RangerDuringRocket::
	msgbox IlexForest_Text_RangerDuringRocket, MSGBOX_DEFAULT
	release
	end

IlexForest_EventScript_RangerAfterApp::
	msgbox IlexForest_Text_RangerAfterApp, MSGBOX_DEFAULT
	release
	end

IlexForest_EventScript_RocketGrunt1::
	trainerbattle_single TRAINER_ROCKET_ILEX_GRUNT_1, IlexForest_Text_Grunt1Intro, IlexForest_Text_Grunt1Defeat
	setflag FLAG_ROCKET_ILEX_GRUNT_1_DEFEATED
	hideobjectat LOCALID_ROCKET_GRUNT_1, MAP_ILEX_FOREST
	msgbox IlexForest_Text_GruntRetreat, MSGBOX_AUTOCLOSE
	end

IlexForest_EventScript_RocketGrunt2::
	trainerbattle_single TRAINER_ROCKET_ILEX_GRUNT_2, IlexForest_Text_Grunt2Intro, IlexForest_Text_Grunt2Defeat
	setflag FLAG_ROCKET_ILEX_GRUNT_2_DEFEATED
	hideobjectat LOCALID_ROCKET_GRUNT_2, MAP_ILEX_FOREST
	msgbox IlexForest_Text_GruntRetreat, MSGBOX_AUTOCLOSE
	end

IlexForest_EventScript_RocketCommander::
	lock
	faceplayer
	goto_if_set FLAG_ROCKET_ILEX_EVENT_CLEARED, IlexForest_EventScript_RocketCommanderAfter
	goto_if_unset FLAG_ROCKET_ILEX_GRUNT_1_DEFEATED, IlexForest_EventScript_RocketCommanderWaiting
	goto_if_unset FLAG_ROCKET_ILEX_GRUNT_2_DEFEATED, IlexForest_EventScript_RocketCommanderWaiting
	msgbox IlexForest_Text_CommanderIntro, MSGBOX_DEFAULT
	trainerbattle_no_intro TRAINER_ROCKET_ILEX_COMMANDER, IlexForest_Text_CommanderDefeat
	setflag FLAG_ROCKET_ILEX_COMMANDER_DEFEATED
	setflag FLAG_ROCKET_ILEX_EVENT_CLEARED
	setvar VAR_ACT1_STORY_STAGE, ACT1_STAGE_ILEX_CLEARED
	special PokeLink_RefreshWorldState
	msgbox IlexForest_Text_CommanderRetreat, MSGBOX_DEFAULT
	msgbox IlexForest_Text_CelebiPulse, MSGBOX_DEFAULT
	hideobjectat LOCALID_ROCKET_COMMANDER, MAP_ILEX_FOREST
	release
	end

IlexForest_EventScript_RocketCommanderWaiting::
	msgbox IlexForest_Text_CommanderWaiting, MSGBOX_DEFAULT
	release
	end

IlexForest_EventScript_RocketCommanderAfter::
	msgbox IlexForest_Text_CommanderAfter, MSGBOX_DEFAULT
	release
	end

IlexForest_EventScript_Shrine::
	lockall
	goto_if_set FLAG_ROCKET_ILEX_EVENT_CLEARED, IlexForest_EventScript_ShrineAfter
	msgbox IlexForest_Text_ShrineQuiet, MSGBOX_SIGN
	releaseall
	end

IlexForest_EventScript_ShrineAfter::
	msgbox IlexForest_Text_ShrineAfter, MSGBOX_SIGN
	releaseall
	end

IlexForest_Text_RangerBeforeBadge::
	.string "ILEX FOREST is old enough\n"
	.string "to notice hasty footsteps.\p"
	.string "Earn VIOLET's BADGE before\n"
	.string "you test the deeper trail.$"

IlexForest_Text_RangerDuringRocket::
	.string "I'm RANGER ELIAN.\p"
	.string "Those red-uniformed thugs\n"
	.string "are circling the shrine.\p"
	.string "If you can battle, clear\n"
	.string "their lookouts first.$"

IlexForest_Text_RangerEncountersIntro::
	.string "The forest settled after\n"
	.string "that pulse from the shrine.\p"
	.string "Your POKéLINK caught enough\n"
	.string "data to track local species.\p"
	.string "I'll finish the calibration.$"

IlexForest_Text_EncountersUnlocked::
	.string "POKéLINK Encounters app\n"
	.string "unlocked!$"

IlexForest_Text_RangerAfterApp::
	.string "The Encounters app updates\n"
	.string "when you enter key areas.\p"
	.string "It's a field log, not a\n"
	.string "replacement for attention.$"

IlexForest_Text_Grunt1Intro::
	.string "TEAM ROCKET owns whatever\n"
	.string "power is hiding here!$"

IlexForest_Text_Grunt1Defeat::
	.string "This forest fights back?$"

IlexForest_Text_Grunt2Intro::
	.string "Quiet! The shrine reacts\n"
	.string "when amateurs wander in.$"

IlexForest_Text_Grunt2Defeat::
	.string "That's not part of the plan!$"

IlexForest_Text_GruntRetreat::
	.string "The ROCKET GRUNT slips\n"
	.string "back through the trees.$"

IlexForest_Text_CommanderIntro::
	.string "Enough.\p"
	.string "A shrine that bends time\n"
	.string "belongs to TEAM ROCKET.\p"
	.string "If CELEBI answers, JOHTO's\n"
	.string "future answers to us.$"

IlexForest_Text_CommanderDefeat::
	.string "The signal broke?$"

IlexForest_Text_CommanderRetreat::
	.string "The COMMANDER checks a\n"
	.string "cracked receiver.\p"
	.string "Retreat. This forest is\n"
	.string "listening too closely.$"

IlexForest_Text_CelebiPulse::
	.string "A green light ripples from\n"
	.string "the shrine.\p"
	.string "For one second, the leaves\n"
	.string "fall upward.$"

IlexForest_Text_CommanderWaiting::
	.string "My lookouts are still in\n"
	.string "place.\p"
	.string "You haven't earned my time.$"

IlexForest_Text_CommanderAfter::
	.string "Only bootprints remain\n"
	.string "where TEAM ROCKET stood.$"

IlexForest_Text_ShrineQuiet::
	.string "The small shrine is quiet.$"

IlexForest_Text_ShrineAfter::
	.string "The shrine is quiet, but\n"
	.string "the air around it feels awake.$"
