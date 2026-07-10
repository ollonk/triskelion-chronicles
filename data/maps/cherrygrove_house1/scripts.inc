cherrygrove_house1_MapScripts::
	.byte 0

cherrygrove_house1_EventScript_RestockGrandmother::
	msgbox cherrygrove_house1_Text_RestockGrandmother, MSGBOX_NPC
	end

cherrygrove_house1_EventScript_RoutePacer::
	msgbox cherrygrove_house1_Text_RoutePacer, MSGBOX_NPC
	end

cherrygrove_house1_Text_RestockGrandmother::
	.string "CHERRYGROVE is a fine\n"
	.string "place to learn your habits.\p"
	.string "If a route feels rough,\n"
	.string "step back and restock.$"

cherrygrove_house1_Text_RoutePacer::
	.string "I count my steps between\n"
	.string "town and the MART.\p"
	.string "A short route feels longer\n"
	.string "when your team is tired.$"
