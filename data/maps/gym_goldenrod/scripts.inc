.set LOCALID_WHITNEY, 1
.set LOCALID_LASS_CARRIE, 2
.set LOCALID_BEAUTY_VICTORIA, 3

gym_goldenrod_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, gym_goldenrod_OnTransition
	.byte 0

gym_goldenrod_OnTransition:
	special PokeLink_RefreshWorldState
	end

gym_goldenrod_EventScript_Whitney::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_WHITNEY, gym_goldenrod_EventScript_WhitneyAfter
	trainerbattle_single TRAINER_WHITNEY, gym_goldenrod_Text_WhitneyIntro, gym_goldenrod_Text_WhitneyDefeat, gym_goldenrod_EventScript_WhitneyDefeated, NO_MUSIC
	msgbox gym_goldenrod_Text_WhitneyPostBattle, MSGBOX_DEFAULT
	release
	end

gym_goldenrod_EventScript_WhitneyDefeated::
	message gym_goldenrod_Text_ReceivedPlainBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox gym_goldenrod_Text_PlainBadgeInfo, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_WHITNEY
	setflag FLAG_BADGE03_GET
	setflag FLAG_LEAGUE_PROSPECT_TRIAL_AVAILABLE
	setvar VAR_ACT1_STORY_STAGE, ACT1_STAGE_PLAIN_BADGE
	special PokeLink_RefreshWorldState
	return

gym_goldenrod_EventScript_WhitneyAfter::
	msgbox gym_goldenrod_Text_WhitneyPostBattle, MSGBOX_DEFAULT
	release
	end

gym_goldenrod_EventScript_Carrie::
	trainerbattle_single TRAINER_GOLDENROD_GYM_LASS_CARRIE, gym_goldenrod_Text_CarrieIntro, gym_goldenrod_Text_CarrieDefeat
	msgbox gym_goldenrod_Text_GymTrainerPostBattle, MSGBOX_AUTOCLOSE
	end

gym_goldenrod_EventScript_Victoria::
	trainerbattle_single TRAINER_GOLDENROD_GYM_BEAUTY_VICTORIA, gym_goldenrod_Text_VictoriaIntro, gym_goldenrod_Text_VictoriaDefeat
	msgbox gym_goldenrod_Text_GymTrainerPostBattle, MSGBOX_AUTOCLOSE
	end

gym_goldenrod_Text_WhitneyIntro::
	.string "Hi! I'm WHITNEY!\p"
	.string "Cute POKEMON, tough POKEMON,\n"
	.string "surprising POKEMON...\p"
	.string "GOLDENROD loves all of them,\n"
	.string "and so do I!$"

gym_goldenrod_Text_WhitneyDefeat::
	.string "No way! You were amazing!$"

gym_goldenrod_Text_ReceivedPlainBadge::
	.string "{PLAYER} received the\n"
	.string "PLAIN BADGE!$"

gym_goldenrod_Text_PlainBadgeInfo::
	.string "The PLAIN BADGE means you\n"
	.string "kept your balance in chaos.\p"
	.string "That's harder than it sounds\n"
	.string "in GOLDENROD.$"

gym_goldenrod_Text_WhitneyPostBattle::
	.string "You should visit the RADIO\n"
	.string "TOWER before leaving town.\p"
	.string "Trainers who listen hear\n"
	.string "trouble before it arrives.$"

gym_goldenrod_Text_CarrieIntro::
	.string "Normal type doesn't mean\n"
	.string "ordinary!$"

gym_goldenrod_Text_CarrieDefeat::
	.string "That was not ordinary.$"

gym_goldenrod_Text_VictoriaIntro::
	.string "GOLDENROD battles are loud,\n"
	.string "bright, and very real!$"

gym_goldenrod_Text_VictoriaDefeat::
	.string "You handled the spotlight.$"

gym_goldenrod_Text_GymTrainerPostBattle::
	.string "WHITNEY's team turns simple\n"
	.string "moves into big moments.$"
