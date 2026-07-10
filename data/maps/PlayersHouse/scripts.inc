.set LOCALID_POKELINK_PICKUP, 1

PlayersHouse_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PlayersHouse_OnTransition
	.byte 0

PlayersHouse_OnTransition:
	goto_if_set FLAG_GOT_POKELINK, PlayersHouse_EventScript_HidePokeLinkPickup
	goto_if_unset FLAG_GOT_ELMS_LAB_STARTER, PlayersHouse_EventScript_HidePokeLinkPickup
	goto_if_unset FLAG_HIDE_POKELINK_PICKUP, PlayersHouse_EventScript_PokeLinkPickupReady
	clearflag FLAG_HIDE_POKELINK_PICKUP
	addobject LOCALID_POKELINK_PICKUP
	end

PlayersHouse_EventScript_PokeLinkPickupReady::
	end

PlayersHouse_EventScript_HidePokeLinkPickup::
	setflag FLAG_HIDE_POKELINK_PICKUP
	removeobject LOCALID_POKELINK_PICKUP
	end

PlayersHouse_EventScript_PokeLink::
	lockall
	goto_if_set FLAG_GOT_POKELINK, PlayersHouse_EventScript_PokeLinkAfter
	goto_if_unset FLAG_GOT_ELMS_LAB_STARTER, PlayersHouse_EventScript_PokeLinkBeforeStarter
	msgbox PlayersHouse_Text_PokeLinkOnTable, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message PlayersHouse_Text_ReceivedPokeLink
	waitmessage
	waitfanfare
	setflag FLAG_GOT_POKELINK
	setflag FLAG_HIDE_POKELINK_PICKUP
	setflag FLAG_POKELINK_APP_PHONE
	setflag FLAG_POKELINK_APP_MESSAGES
	setflag FLAG_POKELINK_APP_TETRIS
	setflag FLAG_POKELINK_APP_GLOOMSCROLL
	removeobject LOCALID_POKELINK_PICKUP
	special PokeLink_RefreshWorldState
	msgbox PlayersHouse_Text_PokeLinkAppsActive, MSGBOX_DEFAULT
	releaseall
	end

PlayersHouse_EventScript_PokeLinkBeforeStarter::
	msgbox PlayersHouse_Text_PokeLinkBeforeStarter, MSGBOX_DEFAULT
	releaseall
	end

PlayersHouse_EventScript_PokeLinkAfter::
	msgbox PlayersHouse_Text_PokeLinkAfter, MSGBOX_DEFAULT
	releaseall
	end

PlayersHouse_Text_PokeLinkBeforeStarter:
	.string "There's nothing on the table\n"
	.string "right now.\p"
	.string "Better check in at the lab\n"
	.string "first.$"

PlayersHouse_Text_PokeLinkOnTable:
	.string "Your POKéLINK is on the table.\p"
	.string "It's fully charged and ready\n"
	.string "for the road.$"

PlayersHouse_Text_ReceivedPokeLink:
	.string "{PLAYER} picked up the POKéLINK!$"

PlayersHouse_Text_PokeLinkAppsActive:
	.string "The PHONE, MESSAGES,\n"
	.string "TETRIS, and GLOOMSCROLL apps\l"
	.string "are active.\p"
	.string "POKéLINK was added to the\n"
	.string "START menu.$"

PlayersHouse_Text_PokeLinkAfter:
	.string "The table is clear.\p"
	.string "Your POKéLINK is ready from\n"
	.string "the START menu.$"
