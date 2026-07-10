.set LOCALID_SURPRISE_BLOCKER, 1
.set LOCALID_POKELINK_REMINDER, 2

NewBarkTown_MapScripts::
	.byte 0

NewBarkTown_EventScript_SurpriseBlocker::
	lock
	faceplayer
	call NewBarkTown_EventScript_RouteBlockerMessage
	release
	end

NewBarkTown_EventScript_PokeLinkReminder::
	lock
	faceplayer
	goto_if_set FLAG_GOT_POKELINK, NewBarkTown_EventScript_PokeLinkReminderReady
	goto_if_set FLAG_GOT_ELMS_LAB_STARTER, NewBarkTown_EventScript_PokeLinkReminderGoHome
	msgbox NewBarkTown_Text_PokeLinkReminderBeforeStarter, MSGBOX_DEFAULT
	release
	end

NewBarkTown_EventScript_PokeLinkReminderGoHome::
	msgbox NewBarkTown_Text_PokeLinkReminderGoHome, MSGBOX_DEFAULT
	release
	end

NewBarkTown_EventScript_PokeLinkReminderReady::
	msgbox NewBarkTown_Text_PokeLinkReminder, MSGBOX_DEFAULT
	release
	end

NewBarkTown_EventScript_SurpriseBlockerCoord::
	goto_if_set FLAG_GOT_POKELINK, NewBarkTown_EventScript_SurpriseBlockerCoordDone
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, NewBarkTown_Movement_PlayerStepBackFromRoute
	waitmovement 0
	call NewBarkTown_EventScript_RouteBlockerMessage
	releaseall
NewBarkTown_EventScript_SurpriseBlockerCoordDone::
	end

NewBarkTown_EventScript_RouteBlockerMessage::
	goto_if_set FLAG_GOT_ELMS_LAB_STARTER, NewBarkTown_EventScript_RouteBlockerNeedsPokeLink
	msgbox NewBarkTown_Text_SurpriseBlocker, MSGBOX_DEFAULT
	return

NewBarkTown_EventScript_RouteBlockerNeedsPokeLink::
	msgbox NewBarkTown_Text_SurpriseBlockerNeedsPokeLink, MSGBOX_DEFAULT
	return

NewBarkTown_EventScript_TownSign::
	msgbox NewBarkTown_Text_TownSign, MSGBOX_SIGN
	end

NewBarkTown_EventScript_ElmsLabSign::
	msgbox NewBarkTown_Text_ElmsLabSign, MSGBOX_SIGN
	end

NewBarkTown_EventScript_PlayersHouseSign::
	msgbox NewBarkTown_Text_PlayersHouseSign, MSGBOX_SIGN
	end

NewBarkTown_EventScript_GoldsHouseSign::
	msgbox NewBarkTown_Text_GoldsHouseSign, MSGBOX_SIGN
	end

NewBarkTown_EventScript_ElmsHouseSign::
	msgbox NewBarkTown_Text_ElmsHouseSign, MSGBOX_SIGN
	end

NewBarkTown_Movement_PlayerStepBackFromRoute:
	walk_right
	step_end

NewBarkTown_Text_TownSign:
	.string "NEW BARK TOWN\p"
	.string "The Town Where the Winds of\n"
	.string "a New Beginning Blow$"

NewBarkTown_Text_ElmsLabSign:
	.string "ELM POKEMON LAB$"

NewBarkTown_Text_PlayersHouseSign:
	.string "{PLAYER}'s House$"

NewBarkTown_Text_GoldsHouseSign:
	.string "GOLD'S HOUSE$"

NewBarkTown_Text_ElmsHouseSign:
	.string "ELM'S HOUSE$"

NewBarkTown_Text_SurpriseBlocker:
	.string "Hey, {PLAYER}!\p"
	.string "I heard there's some big surprise\n"
	.string "waiting at PROF. ELM'S LAB.\p"
	.string "You should go see that before\n"
	.string "heading out to ROUTE 29.$"

NewBarkTown_Text_SurpriseBlockerNeedsPokeLink:
	.string "Hey, {PLAYER}!\p"
	.string "Did you remember to bring\n"
	.string "your POKéLINK with you?\p"
	.string "I know that you're always\n"
	.string "forgetting it at home, and\p"
	.string "you wouldn't want to go on a\n"
	.string "POKEMON journey without it!$"

NewBarkTown_Text_PokeLinkReminderBeforeStarter:
	.string "Going farther than CHERRYGROVE?\p"
	.string "PROF. ELM wanted to see you\n"
	.string "before you left town.$"

NewBarkTown_Text_PokeLinkReminderGoHome:
	.string "Going farther than CHERRYGROVE?\p"
	.string "Did you remember to bring\n"
	.string "your POKéLINK with you?\p"
	.string "I know that you're always\n"
	.string "forgetting it at home.$"

NewBarkTown_Text_PokeLinkReminder:
	.string "Going farther than CHERRYGROVE?\p"
	.string "Make sure your POKéLINK is ready.\p"
	.string "Maps, logs, alerts...\n"
	.string "big journeys have a way of\l"
	.string "getting bigger than planned.$"
