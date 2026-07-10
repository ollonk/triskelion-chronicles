.set LOCALID_ABRACAB_DISPATCHER, 1

goldenrod_underground_corridor_MapScripts::
	.byte 0

goldenrod_underground_corridor_EventScript_AbraCabDispatcher::
	lock
	faceplayer
	goto_if_set FLAG_POKELINK_APP_ABRACAB, goldenrod_underground_corridor_EventScript_AbraCabAfter
	msgbox goldenrod_underground_corridor_Text_AbraCabIntro, MSGBOX_DEFAULT
	setflag FLAG_POKELINK_APP_ABRACAB
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox goldenrod_underground_corridor_Text_AbraCabUnlocked, MSGBOX_DEFAULT
	waitfanfare
	release
	end

goldenrod_underground_corridor_EventScript_AbraCabAfter::
	msgbox goldenrod_underground_corridor_Text_AbraCabAfter, MSGBOX_DEFAULT
	release
	end

goldenrod_underground_corridor_Text_AbraCabIntro::
	.string "Need a ride without touching\n"
	.string "traffic?\p"
	.string "ABRACAB uses registered\n"
	.string "teleport lanes under town.\p"
	.string "Your POKéLINK can handle\n"
	.string "the dispatch app.$"

goldenrod_underground_corridor_Text_AbraCabUnlocked::
	.string "POKéLINK AbraCab app\n"
	.string "unlocked!$"

goldenrod_underground_corridor_Text_AbraCabAfter::
	.string "ABRACAB fares are cheaper\n"
	.string "than sore feet.\p"
	.string "Mostly.$"
