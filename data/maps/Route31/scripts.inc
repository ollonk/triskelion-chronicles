.set LOCALID_COLLECTION_LOG_COLLECTOR, 2

Route31_MapScripts::
	.byte 0

Route31_EventScript_RouteSign::
	msgbox Route31_Text_RouteSign, MSGBOX_SIGN
	end

Route31_EventScript_CollectionLogCollectorTrigger::
	goto_if_set FLAG_POKELINK_COLLECTION_LOG_ROUTE31_EXPLAINED, Route31_EventScript_CollectionLogCollectorTriggerDone
	lockall
	call Route31_EventScript_CollectionLogCollectorAlert
	applymovement LOCALID_COLLECTION_LOG_COLLECTOR, Common_Movement_FaceDown
	waitmovement 0
	goto Route31_EventScript_CollectionLogCollectorTriggerFinish

Route31_EventScript_CollectionLogCollectorTriggerEast::
	goto_if_set FLAG_POKELINK_COLLECTION_LOG_ROUTE31_EXPLAINED, Route31_EventScript_CollectionLogCollectorTriggerDone
	lockall
	call Route31_EventScript_CollectionLogCollectorAlert
	applymovement LOCALID_COLLECTION_LOG_COLLECTOR, Route31_Movement_CollectionLogCollectorStepRight
	waitmovement 0
	goto Route31_EventScript_CollectionLogCollectorTriggerFinish

Route31_EventScript_CollectionLogCollectorTriggerAbove::
	goto_if_set FLAG_POKELINK_COLLECTION_LOG_ROUTE31_EXPLAINED, Route31_EventScript_CollectionLogCollectorTriggerDone
	lockall
	call Route31_EventScript_CollectionLogCollectorAlert
	applymovement LOCALID_COLLECTION_LOG_COLLECTOR, Common_Movement_FaceUp
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	call Route31_EventScript_InstallCollectionLogApp
	releaseall
	end

Route31_EventScript_CollectionLogCollectorTriggerWest::
	goto_if_set FLAG_POKELINK_COLLECTION_LOG_ROUTE31_EXPLAINED, Route31_EventScript_CollectionLogCollectorTriggerDone
	lockall
	call Route31_EventScript_CollectionLogCollectorAlert
	applymovement LOCALID_COLLECTION_LOG_COLLECTOR, Common_Movement_FaceLeft
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	call Route31_EventScript_InstallCollectionLogApp
	releaseall
	end

Route31_EventScript_CollectionLogCollectorTriggerEastSide::
	goto_if_set FLAG_POKELINK_COLLECTION_LOG_ROUTE31_EXPLAINED, Route31_EventScript_CollectionLogCollectorTriggerDone
	lockall
	call Route31_EventScript_CollectionLogCollectorAlert
	applymovement LOCALID_COLLECTION_LOG_COLLECTOR, Common_Movement_FaceRight
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterLeft
	waitmovement 0
	call Route31_EventScript_InstallCollectionLogApp
	releaseall
	end

Route31_EventScript_CollectionLogCollectorAlert::
	setvar VAR_POKELINK_COLLECTION_LOG_ROUTE31_STATE, 1
	applymovement LOCALID_COLLECTION_LOG_COLLECTOR, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_COLLECTION_LOG_COLLECTOR, Common_Movement_Delay48
	waitmovement 0
	return

Route31_EventScript_CollectionLogCollectorTriggerFinish::
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	call Route31_EventScript_InstallCollectionLogApp
	releaseall
	end

Route31_EventScript_CollectionLogCollectorTriggerDone::
	setvar VAR_POKELINK_COLLECTION_LOG_ROUTE31_STATE, 1
	end

Route31_EventScript_CollectionLogCollector::
	lock
	faceplayer
	goto_if_set FLAG_POKELINK_COLLECTION_LOG_ROUTE31_EXPLAINED, Route31_EventScript_CollectionLogCollectorAfter
	setvar VAR_POKELINK_COLLECTION_LOG_ROUTE31_STATE, 1
	call Route31_EventScript_InstallCollectionLogApp
	release
	end

Route31_EventScript_CollectionLogCollectorAfter::
	msgbox Route31_Text_CollectionLogAfter, MSGBOX_DEFAULT
	release
	end

Route31_EventScript_CaveWatcher::
	msgbox Route31_Text_CaveWatcher, MSGBOX_NPC
	end

Route31_EventScript_InstallCollectionLogApp::
	msgbox Route31_Text_CollectionLogGush, MSGBOX_DEFAULT
	msgbox Route31_Text_CollectionLogExplain, MSGBOX_DEFAULT
	setflag FLAG_POKELINK_APP_COLLECTION_LOG
	setflag FLAG_POKELINK_COLLECTION_LOG_ROUTE31_EXPLAINED
	special PokeLink_RefreshWorldState
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox Route31_Text_CollectionLogUnlocked, MSGBOX_DEFAULT
	waitfanfare
	msgbox Route31_Text_CollectionLogDemo, MSGBOX_DEFAULT
	special PokeLink_ShowCollectionLogDemoPopup
	return

Route31_Text_RouteSign::
	.string "ROUTE 31\n"
	.string "VIOLET CITY -\n"
	.string "CHERRYGROVE CITY$"

Route31_Text_CaveWatcher::
	.string "DARK CAVE is colder than\n"
	.string "it looks from out here.\p"
	.string "A spare BALL never feels\n"
	.string "heavy until you need it.$"

Route31_Text_CollectionLogGush::
	.string "Oh! Yes! My log finally\n"
	.string "caught it!\p"
	.string "A GREAT BALL from ROUTE 31.\n"
	.string "Look at that tidy entry!$"

Route31_Text_CollectionLogExplain::
	.string "The COLLECTION LOG is a\n"
	.string "POKELINK app.\p"
	.string "It tracks rare finds by kind:\n"
	.string "TMs, held items, Evolution\l"
	.string "items, Mega Stones, and\n"
	.string "other odd treasures.\p"
	.string "When you find something new,\n"
	.string "it records what it was and\l"
	.string "where you got it.$"

Route31_Text_CollectionLogUnlocked::
	.string "COLLECTION LOG app\n"
	.string "unlocked!$"

Route31_Text_CollectionLogDemo::
	.string "Here, watch the alert.\p"
	.string "It stays out of your way,\n"
	.string "then disappears on its own.$"

Route31_Text_CollectionLogAfter::
	.string "Duplicates are still useful,\n"
	.string "but the first find is the\l"
	.string "exciting one.$"

Route31_Movement_CollectionLogCollectorStepRight::
	walk_right
	step_end

Route31_EventScript_Wade::
	trainerbattle_single TRAINER_JOHTO_BUG_CATCHER_WADE, Route31_Text_WadeIntro, Route31_Text_WadeDefeat
	msgbox Route31_Text_TrainerPostBattle, MSGBOX_AUTOCLOSE
	end

Route31_Text_WadeIntro::
	.string "I caught a whole bunch\n"
	.string "of POKEMON.\p"
	.string "Let me battle with\n"
	.string "them!$"

Route31_Text_WadeDefeat::
	.string "Aww, I lost.$"

Route31_Text_TrainerPostBattle::
	.string "BUG POKEMON grow fast,\n"
	.string "so I keep catching more.$"
