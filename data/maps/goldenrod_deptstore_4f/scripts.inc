goldenrod_deptstore_4f_MapScripts::
	.byte 0

goldenrod_deptstore_4f_EventScript_MedicineClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart goldenrod_deptstore_4f_Pokemart_Medicine
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

goldenrod_deptstore_4f_EventScript_FieldClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart goldenrod_deptstore_4f_Pokemart_Field
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

goldenrod_deptstore_4f_Pokemart_Medicine:
	.2byte ITEM_POTION
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_REVIVE
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_BURN_HEAL
	.2byte ITEM_ICE_HEAL
	.2byte ITEM_FULL_HEAL
	pokemartlistend

goldenrod_deptstore_4f_Pokemart_Field:
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_REPEL
	.2byte ITEM_SUPER_REPEL
	.2byte ITEM_FRESH_WATER
	.2byte ITEM_SODA_POP
	.2byte ITEM_LEMONADE
	pokemartlistend
