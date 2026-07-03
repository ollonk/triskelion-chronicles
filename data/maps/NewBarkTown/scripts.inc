.set LOCALID_SURPRISE_BLOCKER, 1

NewBarkTown_MapScripts::
	.byte 0

NewBarkTown_EventScript_SurpriseBlocker::
	lock
	faceplayer
	msgbox NewBarkTown_Text_SurpriseBlocker, MSGBOX_DEFAULT
	release
	end

NewBarkTown_EventScript_SurpriseBlockerCoord::
	goto_if_set FLAG_GOT_ELMS_LAB_STARTER, NewBarkTown_EventScript_SurpriseBlockerCoordDone
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, NewBarkTown_Movement_PlayerStepBackFromRoute
	waitmovement 0
	msgbox NewBarkTown_Text_SurpriseBlocker, MSGBOX_DEFAULT
	releaseall
NewBarkTown_EventScript_SurpriseBlockerCoordDone::
	end

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
