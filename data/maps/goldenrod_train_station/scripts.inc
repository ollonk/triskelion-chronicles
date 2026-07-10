goldenrod_train_station_MapScripts::
	.byte 0

goldenrod_train_station_EventScript_PatientCommuter::
	msgbox goldenrod_train_station_Text_PatientCommuter, MSGBOX_NPC
	end

goldenrod_train_station_EventScript_MagnetFan::
	msgbox goldenrod_train_station_Text_MagnetFan, MSGBOX_NPC
	end

goldenrod_train_station_Text_PatientCommuter::
	.string "I bought a ticket before\n"
	.string "checking the schedule.\p"
	.string "Now I am learning\n"
	.string "patience.$"

goldenrod_train_station_Text_MagnetFan::
	.string "The MAGNET TRAIN feels\n"
	.string "faster in your chest than\l"
	.string "on the clock.$"
