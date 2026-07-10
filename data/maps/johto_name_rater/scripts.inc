johto_name_rater_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, johto_name_rater_OnTransition
	.byte 0

johto_name_rater_OnTransition:
	special PokeLink_RefreshWorldState
	end

johto_name_rater_EventScript_LeagueAide::
	lock
	faceplayer
	goto_if_set FLAG_GOT_PROSPECT_TRIAL_PSEUDO, johto_name_rater_EventScript_AideAfterTrial
	goto_if_set FLAG_LEAGUE_PROSPECT_TRIAL_CLEARED, johto_name_rater_EventScript_AideRewardWaiting
	goto_if_unset FLAG_BADGE03_GET, johto_name_rater_EventScript_AideNeedsBadge
	setflag FLAG_LEAGUE_PROSPECT_TRIAL_AVAILABLE
	goto_if_set FLAG_LEAGUE_PROSPECT_TRIAL_STARTED, johto_name_rater_EventScript_AideTrialStarted
	msgbox johto_name_rater_Text_AideOfferTrial, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, johto_name_rater_EventScript_AideDeclineTrial
	setflag FLAG_LEAGUE_PROSPECT_TRIAL_STARTED
	special PokeLink_RefreshWorldState
	msgbox johto_name_rater_Text_AideTrialStarted, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_AideNeedsBadge::
	msgbox johto_name_rater_Text_AideNeedsBadge, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_AideDeclineTrial::
	msgbox johto_name_rater_Text_AideDeclineTrial, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_AideTrialStarted::
	msgbox johto_name_rater_Text_AideTrialStarted, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_AideRewardWaiting::
	msgbox johto_name_rater_Text_AideRewardWaiting, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_AideAfterTrial::
	msgbox johto_name_rater_Text_AideAfterTrial, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_Trainee1::
	lock
	faceplayer
	goto_if_unset FLAG_BADGE03_GET, johto_name_rater_EventScript_TraineeNeedsBadge
	goto_if_unset FLAG_LEAGUE_PROSPECT_TRIAL_STARTED, johto_name_rater_EventScript_TraineeNeedsDesk
	setvar VAR_0x8004, TRAINER_LEAGUE_TRAINEE_GOLDENROD_1
	checktrainerflag VAR_0x8004
	goto_if TRUE, johto_name_rater_EventScript_Trainee1After
	trainerbattle_single TRAINER_LEAGUE_TRAINEE_GOLDENROD_1, johto_name_rater_Text_Trainee1Intro, johto_name_rater_Text_Trainee1Defeat
	msgbox johto_name_rater_Text_Trainee1After, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_Trainee1After::
	msgbox johto_name_rater_Text_Trainee1After, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_Trainee2::
	lock
	faceplayer
	goto_if_unset FLAG_BADGE03_GET, johto_name_rater_EventScript_TraineeNeedsBadge
	goto_if_unset FLAG_LEAGUE_PROSPECT_TRIAL_STARTED, johto_name_rater_EventScript_TraineeNeedsDesk
	setvar VAR_0x8004, TRAINER_LEAGUE_TRAINEE_GOLDENROD_1
	checktrainerflag VAR_0x8004
	goto_if FALSE, johto_name_rater_EventScript_Trainee2NeedsFirst
	setvar VAR_0x8004, TRAINER_LEAGUE_TRAINEE_GOLDENROD_2
	checktrainerflag VAR_0x8004
	goto_if TRUE, johto_name_rater_EventScript_Trainee2After
	trainerbattle_single TRAINER_LEAGUE_TRAINEE_GOLDENROD_2, johto_name_rater_Text_Trainee2Intro, johto_name_rater_Text_Trainee2Defeat
	msgbox johto_name_rater_Text_Trainee2After, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_TraineeNeedsBadge::
	msgbox johto_name_rater_Text_TraineeNeedsBadge, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_TraineeNeedsDesk::
	msgbox johto_name_rater_Text_TraineeNeedsDesk, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_Trainee2NeedsFirst::
	msgbox johto_name_rater_Text_Trainee2NeedsFirst, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_Trainee2After::
	msgbox johto_name_rater_Text_Trainee2After, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_Evaluator::
	lock
	faceplayer
	goto_if_set FLAG_GOT_PROSPECT_TRIAL_PSEUDO, johto_name_rater_EventScript_PseudoAlreadyReceived
	goto_if_set FLAG_LEAGUE_PROSPECT_TRIAL_CLEARED, johto_name_rater_EventScript_OfferPseudoReward
	goto_if_unset FLAG_BADGE03_GET, johto_name_rater_EventScript_EvaluatorNeedsBadge
	goto_if_unset FLAG_LEAGUE_PROSPECT_TRIAL_STARTED, johto_name_rater_EventScript_EvaluatorNeedsDesk
	setvar VAR_0x8004, TRAINER_LEAGUE_TRAINEE_GOLDENROD_1
	checktrainerflag VAR_0x8004
	goto_if FALSE, johto_name_rater_EventScript_EvaluatorNeedsStations
	setvar VAR_0x8004, TRAINER_LEAGUE_TRAINEE_GOLDENROD_2
	checktrainerflag VAR_0x8004
	goto_if FALSE, johto_name_rater_EventScript_EvaluatorNeedsStations
	trainerbattle_single TRAINER_LEAGUE_EVALUATOR_GOLDENROD, johto_name_rater_Text_EvaluatorIntro, johto_name_rater_Text_EvaluatorDefeat, johto_name_rater_EventScript_EvaluatorDefeated
	msgbox johto_name_rater_Text_EvaluatorPassed, MSGBOX_DEFAULT
	goto johto_name_rater_EventScript_OfferPseudoReward

johto_name_rater_EventScript_EvaluatorDefeated::
	setflag FLAG_LEAGUE_PROSPECT_TRIAL_CLEARED
	special PokeLink_RefreshWorldState
	return

johto_name_rater_EventScript_EvaluatorNeedsBadge::
	msgbox johto_name_rater_Text_EvaluatorNeedsBadge, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_EvaluatorNeedsDesk::
	msgbox johto_name_rater_Text_EvaluatorNeedsDesk, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_EvaluatorNeedsStations::
	msgbox johto_name_rater_Text_EvaluatorNeedsStations, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_OfferPseudoReward::
	msgbox johto_name_rater_Text_PseudoRewardIntro, MSGBOX_DEFAULT
	multichoice 0, 0, MULTI_LEAGUE_PROSPECT_PSEUDO, FALSE
	switch VAR_RESULT
	case 0, johto_name_rater_EventScript_ChooseDratini
	case 1, johto_name_rater_EventScript_ChooseLarvitar
	case 2, johto_name_rater_EventScript_ChooseBeldum
	case 3, johto_name_rater_EventScript_ChooseBagon
	case 4, johto_name_rater_EventScript_CancelPseudoChoice
	case MULTI_B_PRESSED, johto_name_rater_EventScript_CancelPseudoChoice
	goto johto_name_rater_EventScript_CancelPseudoChoice

johto_name_rater_EventScript_ChooseDratini::
	bufferspeciesname STR_VAR_1, SPECIES_DRATINI
	msgbox johto_name_rater_Text_ConfirmPseudoChoice, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, johto_name_rater_EventScript_OfferPseudoReward
	msgbox johto_name_rater_Text_DratiniChoice, MSGBOX_DEFAULT
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_DRATINI
	givemon SPECIES_DRATINI, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, johto_name_rater_EventScript_ReceiveDratiniParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, johto_name_rater_EventScript_ReceiveDratiniPC
	goto johto_name_rater_EventScript_NoRoomForPseudo

johto_name_rater_EventScript_ChooseLarvitar::
	bufferspeciesname STR_VAR_1, SPECIES_LARVITAR
	msgbox johto_name_rater_Text_ConfirmPseudoChoice, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, johto_name_rater_EventScript_OfferPseudoReward
	msgbox johto_name_rater_Text_LarvitarChoice, MSGBOX_DEFAULT
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_LARVITAR
	givemon SPECIES_LARVITAR, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, johto_name_rater_EventScript_ReceiveLarvitarParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, johto_name_rater_EventScript_ReceiveLarvitarPC
	goto johto_name_rater_EventScript_NoRoomForPseudo

johto_name_rater_EventScript_ChooseBeldum::
	bufferspeciesname STR_VAR_1, SPECIES_BELDUM
	msgbox johto_name_rater_Text_ConfirmPseudoChoice, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, johto_name_rater_EventScript_OfferPseudoReward
	msgbox johto_name_rater_Text_BeldumChoice, MSGBOX_DEFAULT
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_BELDUM
	givemon SPECIES_BELDUM, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, johto_name_rater_EventScript_ReceiveBeldumParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, johto_name_rater_EventScript_ReceiveBeldumPC
	goto johto_name_rater_EventScript_NoRoomForPseudo

johto_name_rater_EventScript_ChooseBagon::
	bufferspeciesname STR_VAR_1, SPECIES_BAGON
	msgbox johto_name_rater_Text_ConfirmPseudoChoice, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, johto_name_rater_EventScript_OfferPseudoReward
	msgbox johto_name_rater_Text_BagonChoice, MSGBOX_DEFAULT
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_BAGON
	givemon SPECIES_BAGON, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, johto_name_rater_EventScript_ReceiveBagonParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, johto_name_rater_EventScript_ReceiveBagonPC
	goto johto_name_rater_EventScript_NoRoomForPseudo

johto_name_rater_EventScript_ReceiveDratiniParty::
	setvar VAR_PROSPECT_TRIAL_PSEUDO_CHOICE, PSEUDO_CHOICE_DRATINI
	goto johto_name_rater_EventScript_ReceivePseudoParty

johto_name_rater_EventScript_ReceiveLarvitarParty::
	setvar VAR_PROSPECT_TRIAL_PSEUDO_CHOICE, PSEUDO_CHOICE_LARVITAR
	goto johto_name_rater_EventScript_ReceivePseudoParty

johto_name_rater_EventScript_ReceiveBeldumParty::
	setvar VAR_PROSPECT_TRIAL_PSEUDO_CHOICE, PSEUDO_CHOICE_BELDUM
	goto johto_name_rater_EventScript_ReceivePseudoParty

johto_name_rater_EventScript_ReceiveBagonParty::
	setvar VAR_PROSPECT_TRIAL_PSEUDO_CHOICE, PSEUDO_CHOICE_BAGON
	goto johto_name_rater_EventScript_ReceivePseudoParty

johto_name_rater_EventScript_ReceiveDratiniPC::
	setvar VAR_PROSPECT_TRIAL_PSEUDO_CHOICE, PSEUDO_CHOICE_DRATINI
	goto johto_name_rater_EventScript_ReceivePseudoPC

johto_name_rater_EventScript_ReceiveLarvitarPC::
	setvar VAR_PROSPECT_TRIAL_PSEUDO_CHOICE, PSEUDO_CHOICE_LARVITAR
	goto johto_name_rater_EventScript_ReceivePseudoPC

johto_name_rater_EventScript_ReceiveBeldumPC::
	setvar VAR_PROSPECT_TRIAL_PSEUDO_CHOICE, PSEUDO_CHOICE_BELDUM
	goto johto_name_rater_EventScript_ReceivePseudoPC

johto_name_rater_EventScript_ReceiveBagonPC::
	setvar VAR_PROSPECT_TRIAL_PSEUDO_CHOICE, PSEUDO_CHOICE_BAGON
	goto johto_name_rater_EventScript_ReceivePseudoPC

johto_name_rater_EventScript_ReceivePseudoParty::
	call johto_name_rater_EventScript_RecordPseudoGift
	playfanfare MUS_OBTAIN_ITEM
	message johto_name_rater_Text_ReceivedPseudo
	waitfanfare
	waitmessage
	msgbox johto_name_rater_Text_AfterPseudoGift, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_ReceivePseudoPC::
	call johto_name_rater_EventScript_RecordPseudoGift
	playfanfare MUS_OBTAIN_ITEM
	message johto_name_rater_Text_ReceivedPseudo
	waitfanfare
	waitmessage
	call Common_EventScript_TransferredToPC
	msgbox johto_name_rater_Text_AfterPseudoGift, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_RecordPseudoGift::
	setflag FLAG_LEAGUE_PROSPECT_TRIAL_CLEARED
	setflag FLAG_GOT_PROSPECT_TRIAL_PSEUDO
	special PokeLink_RefreshWorldState
	@ TODO: Collection Log currently tracks items, not gift Pokemon sources.
	return

johto_name_rater_EventScript_NoRoomForPseudo::
	msgbox gText_NoMoreRoomForPokemon, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_CancelPseudoChoice::
	msgbox johto_name_rater_Text_CancelPseudoChoice, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_PseudoAlreadyReceived::
	msgbox johto_name_rater_Text_PseudoAlreadyReceived, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_EeveeGift::
	lock
	faceplayer
	goto_if_set FLAG_GOT_GOLDENROD_EEVEE, johto_name_rater_EventScript_EeveeAlreadyReceived
	msgbox johto_name_rater_Text_EeveeOffer, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, johto_name_rater_EventScript_EeveeDeclined
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_EEVEE
	givemon SPECIES_EEVEE, 5
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, johto_name_rater_EventScript_ReceiveEeveeParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, johto_name_rater_EventScript_ReceiveEeveePC
	goto johto_name_rater_EventScript_NoRoomForEevee

johto_name_rater_EventScript_ReceiveEeveeParty::
	call johto_name_rater_EventScript_RecordEeveeGift
	playfanfare MUS_OBTAIN_ITEM
	message johto_name_rater_Text_ReceivedEevee
	waitfanfare
	waitmessage
	msgbox johto_name_rater_Text_AfterEeveeGift, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_ReceiveEeveePC::
	call johto_name_rater_EventScript_RecordEeveeGift
	playfanfare MUS_OBTAIN_ITEM
	message johto_name_rater_Text_ReceivedEevee
	waitfanfare
	waitmessage
	call Common_EventScript_TransferredToPC
	msgbox johto_name_rater_Text_AfterEeveeGift, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_RecordEeveeGift::
	setflag FLAG_GOT_GOLDENROD_EEVEE
	special PokeLink_RefreshWorldState
	@ TODO: Collection Log currently tracks items, not gift Pokemon sources.
	return

johto_name_rater_EventScript_EeveeDeclined::
	msgbox johto_name_rater_Text_EeveeDeclined, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_NoRoomForEevee::
	msgbox gText_NoMoreRoomForPokemon, MSGBOX_DEFAULT
	release
	end

johto_name_rater_EventScript_EeveeAlreadyReceived::
	msgbox johto_name_rater_Text_EeveeAlreadyReceived, MSGBOX_DEFAULT
	release
	end

johto_name_rater_Text_AideNeedsBadge::
	.string "League test?\n"
	.string "Not yet.\p"
	.string "Win the PLAIN BADGE first.$"

@ @dialogue id=LEAGUE_PROSPECT_TRIAL_AIDE_OFFER map=johto_name_rater speaker=LeagueAide context=OfferTrial tags=Goldenrod,League,Trial
johto_name_rater_Text_AideOfferTrial::
	.string "PLAIN BADGE?\n"
	.string "Good.\p"
	.string "The League has a little test.\p"
	.string "Not a hard one.\n"
	.string "Just enough to see if you quit.$"

johto_name_rater_Text_AideDeclineTrial::
	.string "No rush.\p"
	.string "Paperwork waits better\n"
	.string "than people do.$"

johto_name_rater_Text_AideTrialStarted::
	.string "Three stations.\p"
	.string "First trainee.\n"
	.string "Second trainee.\p"
	.string "Then the evaluator.$"

johto_name_rater_Text_AideRewardWaiting::
	.string "You passed.\p"
	.string "The evaluator has your\n"
	.string "reward papers.$"

johto_name_rater_Text_AideAfterTrial::
	.string "Badges, badges...\p"
	.string "Why are they always shiny?$"

johto_name_rater_Text_TraineeNeedsBadge::
	.string "League test?\n"
	.string "Not yet.$"

johto_name_rater_Text_TraineeNeedsDesk::
	.string "Check in at the desk.\p"
	.string "Rules first.\n"
	.string "Then battling.$"

johto_name_rater_Text_Trainee1Intro::
	.string "First test.\n"
	.string "Don't overthink it.$"

johto_name_rater_Text_Trainee1Defeat::
	.string "You didn't.$"

johto_name_rater_Text_Trainee1After::
	.string "You didn't overthink it.\p"
	.string "That helps here.$"

johto_name_rater_Text_Trainee2NeedsFirst::
	.string "First test first.\p"
	.string "The forms say so.$"

johto_name_rater_Text_Trainee2Intro::
	.string "Second test.\n"
	.string "Try not to look tired.$"

johto_name_rater_Text_Trainee2Defeat::
	.string "You looked a little tired.$"

johto_name_rater_Text_Trainee2After::
	.string "You looked a little tired.\p"
	.string "Still counts.$"

johto_name_rater_Text_EvaluatorNeedsBadge::
	.string "PLAIN BADGE first.\p"
	.string "Then my clipboard\n"
	.string "gets interested.$"

johto_name_rater_Text_EvaluatorNeedsDesk::
	.string "Desk first.\p"
	.string "This office likes order.$"

johto_name_rater_Text_EvaluatorNeedsStations::
	.string "Finish the stations.\p"
	.string "Then come back.$"

@ @dialogue id=LEAGUE_PROSPECT_TRIAL_EVALUATOR_INTRO map=johto_name_rater speaker=Evaluator context=Intro tags=Goldenrod,League,Trial
johto_name_rater_Text_EvaluatorIntro::
	.string "Last test.\n"
	.string "No speech.\p"
	.string "Battle.$"

johto_name_rater_Text_EvaluatorDefeat::
	.string "Good.$"

johto_name_rater_Text_EvaluatorPassed::
	.string "Good.\n"
	.string "You listen.\p"
	.string "You adjust.\n"
	.string "That matters.$"

johto_name_rater_Text_PseudoRewardIntro::
	.string "You passed.\p"
	.string "These POKEMON are rare.\n"
	.string "They are also a lot of work.\p"
	.string "Pick one.$"

johto_name_rater_Text_ConfirmPseudoChoice::
	.string "Choose {STR_VAR_1}?\p"
	.string "This choice sticks.$"

johto_name_rater_Text_DratiniChoice::
	.string "DRATINI grows slowly.\p"
	.string "Then suddenly it doesn't.$"

johto_name_rater_Text_LarvitarChoice::
	.string "LARVITAR eats a lot.\p"
	.string "Mountains worry about that.$"

johto_name_rater_Text_BeldumChoice::
	.string "BELDUM is stubborn.\p"
	.string "It mostly solves problems\n"
	.string "headfirst.$"

johto_name_rater_Text_BagonChoice::
	.string "BAGON wants the sky.\p"
	.string "It starts on the ground.$"

johto_name_rater_Text_ReceivedPseudo::
	.string "{PLAYER} received {STR_VAR_1}!$"

johto_name_rater_Text_AfterPseudoGift::
	.string "Raise it slowly.\p"
	.string "That is the point.$"

johto_name_rater_Text_CancelPseudoChoice::
	.string "Take your time.\p"
	.string "This choice sticks.$"

johto_name_rater_Text_PseudoAlreadyReceived::
	.string "Still raising it?\n"
	.string "Good.\p"
	.string "Rare does not mean easy.$"

@ @dialogue id=GIFT_EEVEE_OFFER map=johto_name_rater speaker=GiftNPC context=Offer tags=Goldenrod,Gift,Eevee
johto_name_rater_Text_EeveeOffer::
	.string "That EEVEE likes you.\p"
	.string "It changes easy.\n"
	.string "You look like you travel.\p"
	.string "Want to raise it?$"

johto_name_rater_Text_ReceivedEevee::
	.string "{PLAYER} received EEVEE!$"

johto_name_rater_Text_AfterEeveeGift::
	.string "Take good care of it.\p"
	.string "It has a lot of ways\n"
	.string "to grow.$"

johto_name_rater_Text_EeveeDeclined::
	.string "That's fine.\p"
	.string "EEVEE waits better\n"
	.string "than I do.$"

johto_name_rater_Text_EeveeAlreadyReceived::
	.string "How's EEVEE?\p"
	.string "Still deciding what to be?$"
