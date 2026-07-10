cherrygrove_house2_MapScripts::
	.byte 0

cherrygrove_house2_EventScript_NotebookGirl::
	msgbox cherrygrove_house2_Text_NotebookGirl, MSGBOX_NPC
	end

cherrygrove_house2_EventScript_QuietTrainer::
	msgbox cherrygrove_house2_Text_QuietTrainer, MSGBOX_NPC
	end

cherrygrove_house2_Text_NotebookGirl::
	.string "Every TRAINER tells you\n"
	.string "something about a route.\p"
	.string "The loud ones are not\n"
	.string "always the dangerous ones.$"

cherrygrove_house2_Text_QuietTrainer::
	.string "A quiet team can still be\n"
	.string "ready.\p"
	.string "I check moves before I\n"
	.string "check the weather.$"
