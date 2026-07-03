OlivineCity_MapScripts::
	.byte 0

OlivineCity_EventScript_TownSign::
	msgbox OlivineCity_Text_TownSign, MSGBOX_SIGN
	end

OlivineCity_Text_TownSign::
	.string "OLIVINE CITY\n"
	.string "The Port Closest to\n"
	.string "Foreign Lands$"

OlivineCity_EventScript_PokeCenterSign::
	msgbox OlivineCity_Text_PokeCenterSign, MSGBOX_SIGN
	end

OlivineCity_Text_PokeCenterSign::
	.string "POKEMON CENTER$"

OlivineCity_EventScript_MartSign::
	msgbox OlivineCity_Text_MartSign, MSGBOX_SIGN
	end

OlivineCity_Text_MartSign::
	.string "POKEMON MART$"

OlivineCity_EventScript_LighthouseSign::
	msgbox OlivineCity_Text_LighthouseSign, MSGBOX_SIGN
	end

OlivineCity_Text_LighthouseSign::
	.string "OLIVINE LIGHTHOUSE\p"
	.string "Also known as the\n"
	.string "GLITTER LIGHTHOUSE$"
