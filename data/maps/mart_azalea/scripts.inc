mart_azalea_MapScripts::
	.byte 0

mart_azalea_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart mart_azalea_Pokemart
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

mart_azalea_EventScript_ForestShopper::
	msgbox mart_azalea_Text_ForestShopper, MSGBOX_NPC
	end

	.align 2
mart_azalea_Pokemart:
	.2byte ITEM_POKE_BALL
	.2byte ITEM_GREAT_BALL
	.2byte ITEM_POTION
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_REPEL
	.2byte ITEM_HARBOR_MAIL
	pokemartlistend

mart_azalea_Text_ForestShopper::
	.string "I buy REPEL before I buy\n"
	.string "a snack here.\p"
	.string "The forest has opinions\n"
	.string "about shortcuts.$"
