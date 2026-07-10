goldenrod_deptstore_6f_MapScripts::
	.byte 0

goldenrod_deptstore_6f_EventScript_RefreshmentsClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart goldenrod_deptstore_6f_Pokemart_Refreshments
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

goldenrod_deptstore_6f_EventScript_RestShopper::
	msgbox goldenrod_deptstore_6f_Text_RestShopper, MSGBOX_NPC
	end

goldenrod_deptstore_6f_Pokemart_Refreshments:
	.2byte ITEM_FRESH_WATER
	.2byte ITEM_SODA_POP
	.2byte ITEM_LEMONADE
	pokemartlistend

goldenrod_deptstore_6f_Text_RestShopper::
	.string "A long route feels shorter\n"
	.string "with cold drinks packed.$"
