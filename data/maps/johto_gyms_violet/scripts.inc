.set LOCALID_FALKNER, 1
.set LOCALID_BIRD_KEEPER_ABE, 2
.set LOCALID_BIRD_KEEPER_ROD, 3

johto_gyms_violet_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, johto_gyms_violet_OnTransition
	.byte 0

johto_gyms_violet_OnTransition:
	special PokeLink_RefreshWorldState
	end

johto_gyms_violet_EventScript_Falkner::
	lock
	faceplayer
	goto_if_set FLAG_DEFEATED_FALKNER, johto_gyms_violet_EventScript_FalknerAfter
	trainerbattle_single TRAINER_FALKNER, johto_gyms_violet_Text_FalknerIntro, johto_gyms_violet_Text_FalknerDefeat, johto_gyms_violet_EventScript_FalknerDefeated, NO_MUSIC
	msgbox johto_gyms_violet_Text_FalknerPostBattle, MSGBOX_DEFAULT
	release
	end

johto_gyms_violet_EventScript_FalknerDefeated::
	message johto_gyms_violet_Text_ReceivedZephyrBadge
	waitmessage
	call Common_EventScript_PlayGymBadgeFanfare
	msgbox johto_gyms_violet_Text_ZephyrBadgeInfo, MSGBOX_DEFAULT
	setflag FLAG_DEFEATED_FALKNER
	setflag FLAG_BADGE01_GET
	setvar VAR_ACT1_STORY_STAGE, ACT1_STAGE_ZEPHYR_BADGE
	special PokeLink_RefreshWorldState
	return

johto_gyms_violet_EventScript_FalknerAfter::
	msgbox johto_gyms_violet_Text_FalknerPostBattle, MSGBOX_DEFAULT
	release
	end

johto_gyms_violet_EventScript_Abe::
	trainerbattle_single TRAINER_VIOLET_GYM_BIRD_KEEPER_ABE, johto_gyms_violet_Text_AbeIntro, johto_gyms_violet_Text_AbeDefeat
	msgbox johto_gyms_violet_Text_BirdKeeperPostBattle, MSGBOX_AUTOCLOSE
	end

johto_gyms_violet_EventScript_Rod::
	trainerbattle_single TRAINER_VIOLET_GYM_BIRD_KEEPER_ROD, johto_gyms_violet_Text_RodIntro, johto_gyms_violet_Text_RodDefeat
	msgbox johto_gyms_violet_Text_BirdKeeperPostBattle, MSGBOX_AUTOCLOSE
	end

johto_gyms_violet_Text_FalknerIntro::
	.string "I am FALKNER, VIOLET's\n"
	.string "GYM LEADER.\p"
	.string "My FLYING-type POKEMON\n"
	.string "are trained to punish\l"
	.string "careless moves.\p"
	.string "Show me what your team\n"
	.string "can do.$"

johto_gyms_violet_Text_FalknerDefeat::
	.string "You handled that well.$"

johto_gyms_violet_Text_ReceivedZephyrBadge::
	.string "{PLAYER} received the\n"
	.string "ZEPHYR BADGE!$"

johto_gyms_violet_Text_ZephyrBadgeInfo::
	.string "That BADGE proves you can\n"
	.string "handle JOHTO's first GYM.\p"
	.string "Keep training before you\n"
	.string "head for AZALEA.$"

johto_gyms_violet_Text_FalknerPostBattle::
	.string "A strange wind has been\n"
	.string "moving through ILEX FOREST.\p"
	.string "If you hear trouble there,\n"
	.string "face it before BUGSY.$"

johto_gyms_violet_Text_AbeIntro::
	.string "The elegant battle style\n"
	.string "of bird POKEMON is mine!$"

johto_gyms_violet_Text_AbeDefeat::
	.string "I lost altitude!$"

johto_gyms_violet_Text_RodIntro::
	.string "FALKNER trains us to read\n"
	.string "every gust.\p"
	.string "What does your team read?$"

johto_gyms_violet_Text_RodDefeat::
	.string "Your timing was better.$"

johto_gyms_violet_Text_BirdKeeperPostBattle::
	.string "FALKNER checks every battle.\n"
	.string "We all train seriously here.$"
