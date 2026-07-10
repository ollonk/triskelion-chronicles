johto_daycare_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, johto_daycare_OnTransition
	.byte 0

johto_daycare_OnTransition:
	setflag FLAG_LANDMARK_POKEMON_DAYCARE
	end
