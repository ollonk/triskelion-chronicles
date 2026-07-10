.set LOCALID_BUGSY, 1
.set LOCALID_BUG_CATCHER_AL, 2
.set LOCALID_TWINS_LIA_MIA, 3

gym_azalea_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, gym_azalea_OnTransition
	.byte 0

gym_azalea_OnTransition:
	special PokeLink_RefreshWorldState
	end

gym_azalea_EventScript_Bugsy::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_BUGSY, gym_azalea_EventScript_BugsyAfter
	goto_if_unset FLAG_ROCKET_ILEX_EVENT_CLEARED, gym_azalea_EventScript_BugsyNeedIlex
	trainerbattle_single TRAINER_BUGSY, gym_azalea_Text_BugsyIntro, gym_azalea_Text_BugsyDefeat, gym_azalea_EventScript_BugsyDefeated, NO_MUSIC
	msgbox gym_azalea_Text_BugsyPostBattle, MSGBOX_DEFAULT
	release
	end

gym_azalea_EventScript_BugsyDefeated::
	message gym_azalea_Text_ReceivedHiveBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox gym_azalea_Text_HiveBadgeInfo, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_BUGSY
	setflag FLAG_BADGE02_GET
	setvar VAR_ACT1_STORY_STAGE, ACT1_STAGE_HIVE_BADGE
	special PokeLink_RefreshWorldState
	return

gym_azalea_EventScript_BugsyNeedIlex::
	msgbox gym_azalea_Text_BugsyNeedIlex, MSGBOX_DEFAULT
	release
	end

gym_azalea_EventScript_BugsyAfter::
	msgbox gym_azalea_Text_BugsyPostBattle, MSGBOX_DEFAULT
	release
	end

gym_azalea_EventScript_Al::
	trainerbattle_single TRAINER_AZALEA_GYM_BUG_CATCHER_AL, gym_azalea_Text_AlIntro, gym_azalea_Text_AlDefeat
	msgbox gym_azalea_Text_GymTrainerPostBattle, MSGBOX_AUTOCLOSE
	end

gym_azalea_EventScript_LiaMia::
	trainerbattle_single TRAINER_AZALEA_GYM_TWINS_LIA_MIA, gym_azalea_Text_LiaMiaIntro, gym_azalea_Text_LiaMiaDefeat
	msgbox gym_azalea_Text_GymTrainerPostBattle, MSGBOX_AUTOCLOSE
	end

gym_azalea_Text_BugsyIntro::
	.string "I'm BUGSY.\p"
	.string "Bugs look fragile until\n"
	.string "you study how they adapt.\p"
	.string "Let's see how your team\n"
	.string "handles a swarm!$"

gym_azalea_Text_BugsyDefeat::
	.string "Excellent observation!$"

gym_azalea_Text_ReceivedHiveBadge::
	.string "{PLAYER} received the\n"
	.string "HIVE BADGE!$"

gym_azalea_Text_HiveBadgeInfo::
	.string "The HIVE BADGE means you\n"
	.string "understand momentum.\p"
	.string "Keep watching the small\n"
	.string "details. They win battles.$"

gym_azalea_Text_BugsyNeedIlex::
	.string "I can't take a challenge\n"
	.string "while ILEX FOREST is unsafe.\p"
	.string "TEAM ROCKET was seen near\n"
	.string "the old shrine.\p"
	.string "Please clear that first.$"

gym_azalea_Text_BugsyPostBattle::
	.string "ILEX FOREST's recovery is\n"
	.string "already changing local data.\p"
	.string "Your POKéLINK should be\n"
	.string "worth checking often.$"

gym_azalea_Text_AlIntro::
	.string "BUG POKEMON evolve fast.\n"
	.string "So do good TRAINERS!$"

gym_azalea_Text_AlDefeat::
	.string "You evolved faster!$"

gym_azalea_Text_LiaMiaIntro::
	.string "Two eyes miss details.\n"
	.string "Four eyes catch them!$"

gym_azalea_Text_LiaMiaDefeat::
	.string "We blinked first!$"

gym_azalea_Text_GymTrainerPostBattle::
	.string "BUGSY says every battle is\n"
	.string "field research.$"
