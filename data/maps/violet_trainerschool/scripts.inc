violet_trainerschool_MapScripts::
	.byte 0

violet_trainerschool_EventScript_StatusTeacher::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_VIOLET_SCHOOL_PARLYZ_HEAL, violet_trainerschool_EventScript_StatusTeacherAfter
	msgbox violet_trainerschool_Text_StatusTeacherIntro, MSGBOX_DEFAULT
	giveitem ITEM_PARALYZE_HEAL
	goto_if_eq VAR_RESULT, FALSE, violet_trainerschool_EventScript_StatusTeacherBagFull
	setflag FLAG_RECEIVED_VIOLET_SCHOOL_PARLYZ_HEAL
	msgbox violet_trainerschool_Text_StatusTeacherAfter, MSGBOX_DEFAULT
	release
	end

violet_trainerschool_EventScript_StatusTeacherAfter::
	msgbox violet_trainerschool_Text_StatusTeacherAfter, MSGBOX_DEFAULT
	release
	end

violet_trainerschool_EventScript_StatusTeacherBagFull::
	msgbox violet_trainerschool_Text_StatusTeacherBagFull, MSGBOX_DEFAULT
	release
	end

violet_trainerschool_EventScript_TypeStudent::
	msgbox violet_trainerschool_Text_TypeStudent, MSGBOX_NPC
	end

violet_trainerschool_EventScript_FalknerStudent::
	goto_if_set FLAG_BADGE01_GET, violet_trainerschool_EventScript_FalknerStudentAfterBadge
	msgbox violet_trainerschool_Text_FalknerStudentBeforeBadge, MSGBOX_NPC
	end

violet_trainerschool_EventScript_FalknerStudentAfterBadge::
	msgbox violet_trainerschool_Text_FalknerStudentAfterBadge, MSGBOX_NPC
	end

violet_trainerschool_EventScript_ItemStudent::
	msgbox violet_trainerschool_Text_ItemStudent, MSGBOX_NPC
	end

violet_trainerschool_EventScript_Chalkboard::
	msgbox violet_trainerschool_Text_Chalkboard, MSGBOX_SIGN
	end

violet_trainerschool_Text_StatusTeacherIntro::
	.string "VIOLET's routes love status\n"
	.string "lessons.\p"
	.string "A PARALYZE HEAL is boring\n"
	.string "until it saves a trip.$"

violet_trainerschool_Text_StatusTeacherAfter::
	.string "Speed matters.\p"
	.string "PARALYSIS cuts into more\n"
	.string "than pride.$"

violet_trainerschool_Text_StatusTeacherBagFull::
	.string "Make room in your BAG\n"
	.string "and I will keep the lesson\l"
	.string "aside.$"

violet_trainerschool_Text_TypeStudent::
	.string "I wrote TYPE MATCHUPS on\n"
	.string "my notebook.\p"
	.string "Then a trainer won because\n"
	.string "his POKEMON held on.$"

violet_trainerschool_Text_FalknerStudentBeforeBadge::
	.string "FALKNER's pupils watch\n"
	.string "how you handle height.\p"
	.string "A grounded plan still\n"
	.string "needs good timing.$"

violet_trainerschool_Text_FalknerStudentAfterBadge::
	.string "You beat FALKNER?\p"
	.string "Then you know even birds\n"
	.string "hate bad timing.$"

violet_trainerschool_Text_ItemStudent::
	.string "My teacher says items are\n"
	.string "part of a plan.\p"
	.string "Using them late is panic.\n"
	.string "Packing early is prep.$"

violet_trainerschool_Text_Chalkboard::
	.string "TRISKELION CHRONICLES\n"
	.string "FIELD NOTES\p"
	.string "POKEMON from many regions\n"
	.string "can appear across JOHTO.\p"
	.string "Moves, abilities, items,\n"
	.string "and evolutions follow newer\l"
	.string "battle discoveries.\p"
	.string "Your POKéLINK opens from\n"
	.string "the START menu after you\l"
	.string "pick it up at home.\p"
	.string "Apps include MESSAGES,\n"
	.string "PHONE, GLOOMSCROLL,\l"
	.string "TETRIS, RADIO, and more.\p"
	.string "Check POKéLINK often.\n"
	.string "Some leads update as your\l"
	.string "badges and route progress\l"
	.string "change.$"
