goldenrod_bike_shop_MapScripts::
	.byte 0

goldenrod_bike_shop_EventScript_Owner::
	lock
	faceplayer
	goto_if_set FLAG_GOT_GOLDENROD_BIKE, goldenrod_bike_shop_EventScript_AlreadyGotBike
	goto_if_unset FLAG_BADGE03_GET, goldenrod_bike_shop_EventScript_NeedsPlainBadge
	msgbox goldenrod_bike_shop_Text_OfferBike, MSGBOX_DEFAULT
	multichoice 0, 0, MULTI_GOLDENROD_BIKE_CHOICE, TRUE
	switch VAR_RESULT
	case 0, goldenrod_bike_shop_EventScript_GiveMachBike
	case 1, goldenrod_bike_shop_EventScript_GiveAcroBike
	case 2, goldenrod_bike_shop_EventScript_CancelChoice
	case MULTI_B_PRESSED, goldenrod_bike_shop_EventScript_CancelChoice
	end

goldenrod_bike_shop_EventScript_GiveMachBike::
	msgbox goldenrod_bike_shop_Text_ChoseMachBike, MSGBOX_DEFAULT
	giveitem ITEM_MACH_BIKE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_GOT_GOLDENROD_BIKE
	setflag FLAG_CHOSE_MACH_BIKE
	setvar VAR_GOLDENROD_BIKE_CHOICE, BIKE_CHOICE_MACH
	special PokeLink_RefreshWorldState
	msgbox goldenrod_bike_shop_Text_TakeCare, MSGBOX_DEFAULT
	release
	end

goldenrod_bike_shop_EventScript_GiveAcroBike::
	msgbox goldenrod_bike_shop_Text_ChoseAcroBike, MSGBOX_DEFAULT
	giveitem ITEM_ACRO_BIKE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_GOT_GOLDENROD_BIKE
	setflag FLAG_CHOSE_ACRO_BIKE
	setvar VAR_GOLDENROD_BIKE_CHOICE, BIKE_CHOICE_ACRO
	special PokeLink_RefreshWorldState
	msgbox goldenrod_bike_shop_Text_TakeCare, MSGBOX_DEFAULT
	release
	end

goldenrod_bike_shop_EventScript_CancelChoice::
	msgbox goldenrod_bike_shop_Text_CancelChoice, MSGBOX_DEFAULT
	release
	end

goldenrod_bike_shop_EventScript_NeedsPlainBadge::
	msgbox goldenrod_bike_shop_Text_NeedsPlainBadge, MSGBOX_DEFAULT
	release
	end

goldenrod_bike_shop_EventScript_AlreadyGotBike::
	goto_if_set FLAG_CHOSE_MACH_BIKE, goldenrod_bike_shop_EventScript_AlreadyGotMachBike
	goto_if_set FLAG_CHOSE_ACRO_BIKE, goldenrod_bike_shop_EventScript_AlreadyGotAcroBike
	msgbox goldenrod_bike_shop_Text_TakeCare, MSGBOX_DEFAULT
	release
	end

goldenrod_bike_shop_EventScript_AlreadyGotMachBike::
	msgbox goldenrod_bike_shop_Text_AlreadyGotMachBike, MSGBOX_DEFAULT
	release
	end

goldenrod_bike_shop_EventScript_AlreadyGotAcroBike::
	msgbox goldenrod_bike_shop_Text_AlreadyGotAcroBike, MSGBOX_DEFAULT
	release
	end

goldenrod_bike_shop_EventScript_TireCustomer::
	msgbox goldenrod_bike_shop_Text_TireCustomer, MSGBOX_NPC
	end

	@ TODO: Let the player obtain the unchosen Hoenn bike in a later act.

goldenrod_bike_shop_Text_NeedsPlainBadge::
	.string "These are imported HOENN\n"
	.string "BIKES, tuned for serious\l"
	.string "roadwork.\p"
	.string "Come back after earning\n"
	.string "GOLDENROD's PLAIN BADGE.$"

goldenrod_bike_shop_Text_OfferBike::
	.string "I used to hear shops gave\n"
	.string "BIKES away for advertising.\p"
	.string "That is not much of a\n"
	.string "business plan, is it?\p"
	.string "Still, you beat WHITNEY,\n"
	.string "so I can spare one import.\p"
	.string "Choose carefully. For now,\n"
	.string "you get one HOENN BIKE.$"

goldenrod_bike_shop_Text_ChoseMachBike::
	.string "The MACH BIKE is built\n"
	.string "for speed.\p"
	.string "Once it gets going, it\n"
	.string "really moves.$"

goldenrod_bike_shop_Text_ChoseAcroBike::
	.string "The ACRO BIKE is built\n"
	.string "for tricks and control.\p"
	.string "It rewards careful hands.$"

goldenrod_bike_shop_Text_CancelChoice::
	.string "No rush.\p"
	.string "A BIKE choice is worth\n"
	.string "thinking through.$"

goldenrod_bike_shop_Text_TakeCare::
	.string "Take good care of it.\p"
	.string "Those HOENN parts are\n"
	.string "not easy to replace.$"

goldenrod_bike_shop_Text_AlreadyGotMachBike::
	.string "How is the MACH BIKE?\p"
	.string "Fast riders should mind\n"
	.string "tight corners.$"

goldenrod_bike_shop_Text_AlreadyGotAcroBike::
	.string "How is the ACRO BIKE?\p"
	.string "Trick riding is all about\n"
	.string "timing.$"

goldenrod_bike_shop_Text_TireCustomer::
	.string "I came to browse and got\n"
	.string "a lecture on tires.\p"
	.string "Honestly? Good lecture.$"
