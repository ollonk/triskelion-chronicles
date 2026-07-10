mr_pokemons_house_MapScripts::
	.byte 0

mr_pokemons_house_EventScript_MrPokemon::
	msgbox mr_pokemons_house_Text_MrPokemon, MSGBOX_NPC
	end

mr_pokemons_house_EventScript_FieldAssistant::
	msgbox mr_pokemons_house_Text_FieldAssistant, MSGBOX_NPC
	end

mr_pokemons_house_Text_MrPokemon::
	.string "I'm MR. POKEMON.\p"
	.string "People bring me strange\n"
	.string "finds from all over JOHTO.\p"
	.string "Every so often, one of them\n"
	.string "turns into a real mystery.$"

mr_pokemons_house_Text_FieldAssistant::
	.string "MR. POKEMON keeps careful\n"
	.string "notes on every oddity.\p"
	.string "I am just here to compare\n"
	.string "a shell to a stone.$"
