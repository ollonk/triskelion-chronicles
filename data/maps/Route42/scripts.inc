Route42_MapScripts::
	.byte 0

Route42_EventScript_RouteSignWest::
	msgbox Route42_Text_RouteSignWest, MSGBOX_SIGN
	end

Route42_Text_RouteSignWest::
	.string "ROUTE 42\n"
	.string "ECRUTEAK CITY - MAHOGANY TOWN$"

Route42_EventScript_MtMortarSign::
	msgbox Route42_Text_MtMortarSign, MSGBOX_SIGN
	end

Route42_Text_MtMortarSign::
	.string "MT. MORTAR\p"
	.string "WATERFALL CAVE\n"
	.string "INSIDE$"

Route42_EventScript_RouteSignEast::
	msgbox Route42_Text_RouteSignEast, MSGBOX_SIGN
	end

Route42_Text_RouteSignEast::
	.string "ROUTE 42\n"
	.string "ECRUTEAK CITY -\n"
	.string "MAHOGANY TOWN$"
