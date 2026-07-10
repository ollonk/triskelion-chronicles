goldenrod_deptstore_2f_MapScripts::
	.byte 0

goldenrod_deptstore_2f_EventScript_SuppliesClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart goldenrod_deptstore_2f_Pokemart_Supplies
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

goldenrod_deptstore_2f_EventScript_BattleItemsClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart goldenrod_deptstore_2f_Pokemart_BattleItems
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

goldenrod_deptstore_2f_Pokemart_Supplies:
	.2byte ITEM_POKE_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_POTION
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_AWAKENING
	.2byte ITEM_BURN_HEAL
	.2byte ITEM_ICE_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_REPEL
	pokemartlistend

goldenrod_deptstore_2f_Pokemart_BattleItems:
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_DEFENSE
	.2byte ITEM_X_SPEED
	.2byte ITEM_X_ACCURACY
	.2byte ITEM_DIRE_HIT
	.2byte ITEM_GUARD_SPEC
	pokemartlistend
