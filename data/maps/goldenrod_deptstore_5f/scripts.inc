goldenrod_deptstore_5f_MapScripts::
	.byte 0

goldenrod_deptstore_5f_EventScript_TmClerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemart goldenrod_deptstore_5f_Pokemart_TMs
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

goldenrod_deptstore_5f_Pokemart_TMs:
	.2byte ITEM_TM_HIDDEN_POWER
	.2byte ITEM_TM_SAFEGUARD
	.2byte ITEM_TM_DOUBLE_TEAM
	.2byte ITEM_TM_REFLECT
	.2byte ITEM_TM_SECRET_POWER
	.2byte ITEM_TM_REST
	.2byte ITEM_TM_ATTRACT
	.2byte ITEM_TM_THIEF
	pokemartlistend
