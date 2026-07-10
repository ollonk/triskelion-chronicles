route30_house1_MapScripts::
	.byte 0

route30_house1_EventScript_BerryKeeper::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_ROUTE30_HOUSE_ORAN_BERRY, route30_house1_EventScript_BerryKeeperAfter
	msgbox route30_house1_Text_BerryKeeperIntro, MSGBOX_DEFAULT
	giveitem ITEM_ORAN_BERRY
	goto_if_eq VAR_RESULT, FALSE, route30_house1_EventScript_BerryKeeperBagFull
	setflag FLAG_RECEIVED_ROUTE30_HOUSE_ORAN_BERRY
	msgbox route30_house1_Text_BerryKeeperAfter, MSGBOX_DEFAULT
	release
	end

route30_house1_EventScript_BerryKeeperAfter::
	msgbox route30_house1_Text_BerryKeeperAfter, MSGBOX_DEFAULT
	release
	end

route30_house1_EventScript_BerryKeeperBagFull::
	msgbox route30_house1_Text_BerryKeeperBagFull, MSGBOX_DEFAULT
	release
	end

route30_house1_EventScript_PathWatcher::
	msgbox route30_house1_Text_PathWatcher, MSGBOX_NPC
	end

route30_house1_Text_BerryKeeperIntro::
	.string "ROUTE 30 is all berries\n"
	.string "and surprise battles.\p"
	.string "Here, an ORAN BERRY.\n"
	.string "It can turn a close fight.$"

route30_house1_Text_BerryKeeperAfter::
	.string "Give that berry to a\n"
	.string "POKEMON before trouble,\l"
	.string "not after.$"

route30_house1_Text_BerryKeeperBagFull::
	.string "No room? Berries bruise\n"
	.string "if you cram them in.\p"
	.string "Come back after making\n"
	.string "space.$"

route30_house1_Text_PathWatcher::
	.string "The north path changes\n"
	.string "once you know where\l"
	.string "TRAINERS are standing.$"
