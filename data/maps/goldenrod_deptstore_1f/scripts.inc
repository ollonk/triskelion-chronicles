.set LOCALID_DELIVERY_CLERK, 1

goldenrod_deptstore_1f_MapScripts::
	.byte 0

goldenrod_deptstore_1f_EventScript_DeliveryClerk::
	lock
	faceplayer
	goto_if_set FLAG_POKELINK_APP_DELIVERY, goldenrod_deptstore_1f_EventScript_DeliveryClerkAfter
	msgbox goldenrod_deptstore_1f_Text_DeliveryClerkIntro, MSGBOX_DEFAULT
	setflag FLAG_POKELINK_APP_DELIVERY
	playfanfare MUS_REGISTER_MATCH_CALL
	msgbox goldenrod_deptstore_1f_Text_DeliveryUnlocked, MSGBOX_DEFAULT
	waitfanfare
	release
	end

goldenrod_deptstore_1f_EventScript_DeliveryClerkAfter::
	msgbox goldenrod_deptstore_1f_Text_DeliveryClerkAfter, MSGBOX_DEFAULT
	release
	end

@ @dialogue id=POKELINK_DELIBIRD_DELIVERY_UNLOCK_INTRO map=goldenrod_deptstore_1f speaker=DeliveryClerk context=Intro tags=Goldenrod,PokeLink,DeptStore
goldenrod_deptstore_1f_Text_DeliveryClerkIntro::
	.string "Welcome to GOLDENROD\n"
	.string "DEPT. STORE!\p"
	.string "We also support DELIBIRD\n"
	.string "DELIVERY for TRAINERS.\p"
	.string "I'll add the order app to\n"
	.string "your POKéLINK.$"

@ @dialogue id=POKELINK_DELIBIRD_DELIVERY_UNLOCKED map=goldenrod_deptstore_1f speaker=System context=Unlocked tags=Goldenrod,PokeLink,DeptStore
goldenrod_deptstore_1f_Text_DeliveryUnlocked::
	.string "POKéLINK Delibird Delivery\n"
	.string "app unlocked!$"

goldenrod_deptstore_1f_Text_DeliveryClerkAfter::
	.string "Place an order from the\n"
	.string "app and a courier will\l"
	.string "try to reach you.$"
