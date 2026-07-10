radio_tower_3f_MapScripts::
	.byte 0

radio_tower_3f_EventScript_RecordingTech::
	msgbox radio_tower_3f_Text_RecordingTech, MSGBOX_NPC
	end

radio_tower_3f_EventScript_SoundRunner::
	msgbox radio_tower_3f_Text_SoundRunner, MSGBOX_NPC
	end

radio_tower_3f_Text_RecordingTech::
	.string "Recording rooms look calm\n"
	.string "because panic has to be\l"
	.string "quiet.$"

radio_tower_3f_Text_SoundRunner::
	.string "If a DJ says one more\n"
	.string "take, bring water.\p"
	.string "One more take means six.$"
