.set LOCALID_NURSE, 1

pokecenter_goldenrod_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, pokecenter_goldenrod_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

pokecenter_goldenrod_OnTransition:
	setrespawn HEAL_LOCATION_GOLDENROD_CITY
	end

pokecenter_goldenrod_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

pokecenter_goldenrod_EventScript_VsSeekerCoach::
	lock
	faceplayer
	goto_if_set FLAG_POKELINK_APP_VS_SEEKER, pokecenter_goldenrod_EventScript_VsSeekerCoachAfter
	msgbox pokecenter_goldenrod_Text_VsSeekerCoachIntro, MSGBOX_DEFAULT
	setflag FLAG_POKELINK_APP_VS_SEEKER
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox pokecenter_goldenrod_Text_VsSeekerUnlocked, MSGBOX_DEFAULT
	waitfanfare
	release
	end

pokecenter_goldenrod_EventScript_VsSeekerCoachAfter::
	msgbox pokecenter_goldenrod_Text_VsSeekerCoachAfter, MSGBOX_DEFAULT
	release
	end

pokecenter_goldenrod_Text_VsSeekerCoachIntro::
	.string "Some TRAINERS want a rematch\n"
	.string "after they have time to train.\p"
	.string "Your POKéLINK can track those\n"
	.string "signals with VS SEEKER.\p"
	.string "I'll activate it for you.$"

pokecenter_goldenrod_Text_VsSeekerUnlocked::
	.string "POKéLINK VS Seeker app\n"
	.string "unlocked!$"

pokecenter_goldenrod_Text_VsSeekerCoachAfter::
	.string "Check VS SEEKER when old rivals\n"
	.string "start itching for another battle.$"

pokecenter_goldenrod_EventScript_WhitneyFan::
	goto_if_set FLAG_BADGE03_GET, pokecenter_goldenrod_EventScript_WhitneyFanAfterBadge
	msgbox pokecenter_goldenrod_Text_WhitneyFanBeforeBadge, MSGBOX_NPC
	end

pokecenter_goldenrod_EventScript_WhitneyFanAfterBadge::
	msgbox pokecenter_goldenrod_Text_WhitneyFanAfterBadge, MSGBOX_NPC
	end

pokecenter_goldenrod_EventScript_RadioListener::
	goto_if_set FLAG_POKELINK_APP_RADIO, pokecenter_goldenrod_EventScript_RadioListenerAfterApp
	msgbox pokecenter_goldenrod_Text_RadioListenerBeforeApp, MSGBOX_NPC
	end

pokecenter_goldenrod_EventScript_RadioListenerAfterApp::
	msgbox pokecenter_goldenrod_Text_RadioListenerAfterApp, MSGBOX_NPC
	end

pokecenter_goldenrod_EventScript_BikeFan::
	goto_if_set FLAG_CHOSE_MACH_BIKE, pokecenter_goldenrod_EventScript_BikeFanMach
	goto_if_set FLAG_CHOSE_ACRO_BIKE, pokecenter_goldenrod_EventScript_BikeFanAcro
	msgbox pokecenter_goldenrod_Text_BikeFanBeforeBike, MSGBOX_NPC
	end

pokecenter_goldenrod_EventScript_BikeFanMach::
	msgbox pokecenter_goldenrod_Text_BikeFanMach, MSGBOX_NPC
	end

pokecenter_goldenrod_EventScript_BikeFanAcro::
	msgbox pokecenter_goldenrod_Text_BikeFanAcro, MSGBOX_NPC
	end

pokecenter_goldenrod_EventScript_DayCareHint::
	msgbox pokecenter_goldenrod_Text_DayCareHint, MSGBOX_NPC
	end

pokecenter_goldenrod_EventScript_DepartmentShopper::
	msgbox pokecenter_goldenrod_Text_DepartmentShopper, MSGBOX_NPC
	end

pokecenter_goldenrod_Text_WhitneyFanBeforeBadge::
	.string "WHITNEY is cute.\p"
	.string "That is not a plan.$"

pokecenter_goldenrod_Text_WhitneyFanAfterBadge::
	.string "PLAIN BADGE?\p"
	.string "She cried, didn't she?$"

pokecenter_goldenrod_Text_RadioListenerBeforeApp::
	.string "RADIO TOWER is north.\p"
	.string "They talk for free.$"

pokecenter_goldenrod_Text_RadioListenerAfterApp::
	.string "You got Radio?\p"
	.string "Try not to hum indoors.$"

pokecenter_goldenrod_Text_BikeFanBeforeBike::
	.string "BIKE SHOP has HOENN\n"
	.string "BIKES.\p"
	.string "Fast one.\p"
	.string "Weird one.$"

pokecenter_goldenrod_Text_BikeFanMach::
	.string "MACH BIKE?\p"
	.string "Blink less.$"

pokecenter_goldenrod_Text_BikeFanAcro::
	.string "ACRO BIKE?\p"
	.string "Hop outside.$"

pokecenter_goldenrod_Text_DayCareHint::
	.string "Eggs like steps.\p"
	.string "Nobody knows why.\p"
	.string "They just do.$"

pokecenter_goldenrod_Text_DepartmentShopper::
	.string "I went for POTIONS.\p"
	.string "I bought a scarf.\p"
	.string "For my POKéMON.$"
