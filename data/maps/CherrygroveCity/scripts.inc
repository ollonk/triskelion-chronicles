CherrygroveCity_MapScripts::
	.byte 0

@ TODO: Wire this to Cherrygrove's guide old man after the tour NPC exists.
CherrygroveCity_EventScript_InstallPokeLinkMapApp::
	lock
	faceplayer
	goto_if_set FLAG_POKELINK_APP_MAP, CherrygroveCity_EventScript_AlreadyInstalledMapApp
	msgbox CherrygroveCity_Text_InstallMapApp, MSGBOX_DEFAULT
	setflag FLAG_POKELINK_APP_MAP
	playfanfare MUS_OBTAIN_ITEM
	msgbox CherrygroveCity_Text_InstalledMapApp, MSGBOX_DEFAULT
	waitfanfare
	release
	end

CherrygroveCity_EventScript_AlreadyInstalledMapApp::
	msgbox CherrygroveCity_Text_AlreadyInstalledMapApp, MSGBOX_DEFAULT
	release
	end

CherrygroveCity_EventScript_PokeCenterSign::
	msgbox CherrygroveCity_Text_PokeCenterSign, MSGBOX_SIGN
	end

CherrygroveCity_Text_PokeCenterSign::
	.string "POKEMON CENTER$"

CherrygroveCity_EventScript_MartSign::
	msgbox CherrygroveCity_Text_MartSign, MSGBOX_SIGN
	end

CherrygroveCity_Text_MartSign::
	.string "POKEMON MART$"

CherrygroveCity_EventScript_GuideGentSign::
	msgbox CherrygroveCity_Text_GuideGentSign, MSGBOX_SIGN
	end

CherrygroveCity_Text_GuideGentSign::
	.string "GUIDE GENT'S HOUSE$"

CherrygroveCity_EventScript_TownSign::
	msgbox CherrygroveCity_Text_TownSign, MSGBOX_SIGN
	end

CherrygroveCity_Text_TownSign::
	.string "CHERRYGROVE CITY\n"
	.string "The City of Cute,\n"
	.string "Fragrant Flowers$"

CherrygroveCity_Text_InstallMapApp::
	.string "I know every road in JOHTO.\p"
	.string "Let me install a MAP app on\n"
	.string "your POKéLINK.$"

CherrygroveCity_Text_InstalledMapApp::
	.string "The MAP app was installed\n"
	.string "on your POKéLINK!$"

CherrygroveCity_Text_AlreadyInstalledMapApp::
	.string "That MAP app will keep you\n"
	.string "from losing your way.$"
