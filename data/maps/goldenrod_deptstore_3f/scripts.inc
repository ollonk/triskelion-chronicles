goldenrod_deptstore_3f_MapScripts::
	.byte 0

goldenrod_deptstore_3f_EventScript_HeldItemsClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart goldenrod_deptstore_3f_Pokemart_HeldItems
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

goldenrod_deptstore_3f_Pokemart_HeldItems:
	.2byte ITEM_SILK_SCARF
	.2byte ITEM_SHARP_BEAK
	.2byte ITEM_SILVER_POWDER
	.2byte ITEM_MIRACLE_SEED
	.2byte ITEM_CHARCOAL
	.2byte ITEM_MYSTIC_WATER
	.2byte ITEM_SOFT_SAND
	.2byte ITEM_HARD_STONE
	.2byte ITEM_MAGNET
	.2byte ITEM_POISON_BARB
	.2byte ITEM_BLACK_GLASSES
	.2byte ITEM_SPELL_TAG
	pokemartlistend
