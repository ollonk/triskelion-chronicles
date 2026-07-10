.set LOCALID_POKELINK_ARCADE_ATTENDANT, 1

goldenrod_game_corner_MapScripts::
	.byte 0

	@ Game Corner prices
	.set ABRA_COINS, 200
	.set SANDSHREW_COINS, 700
	.set DRATINI_COINS, 2100
	.set SILK_SCARF_COINS, 1000
	.set TM_THUNDERBOLT_COINS, 4000
	.set TM_ICE_BEAM_COINS, 4000
	.set TM_FLAMETHROWER_COINS, 4000
	.set COINS_PRICE_50, 1000
	.set COINS_PRICE_500, 10000

goldenrod_game_corner_EventScript_CoinCaseGiver::
	lock
	faceplayer
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, TRUE, goldenrod_game_corner_EventScript_CoinCaseGiverAfter
	msgbox goldenrod_game_corner_Text_CoinCaseGiverIntro, MSGBOX_DEFAULT
	giveitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_COIN_CASE
	special PokeLink_RefreshWorldState
	msgbox goldenrod_game_corner_Text_CoinCaseGiverAfter, MSGBOX_DEFAULT
	release
	end

goldenrod_game_corner_EventScript_CoinCaseGiverAfter::
	msgbox goldenrod_game_corner_Text_CoinCaseGiverAfter, MSGBOX_DEFAULT
	release
	end

goldenrod_game_corner_EventScript_CoinsClerk::
	lock
	faceplayer
	msgbox goldenrod_game_corner_Text_CoinsClerkIntro, MSGBOX_DEFAULT
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_NeedCoinCase
	message goldenrod_game_corner_Text_BuyCoinsPrompt
	waitmessage
	showmoneybox 0, 0
	showcoinsbox 1, 6
	multichoicedefault 15, 0, MULTI_GAME_CORNER_COINS, 0, FALSE
	switch VAR_RESULT
	case 0, goldenrod_game_corner_EventScript_Buy50Coins
	case 1, goldenrod_game_corner_EventScript_Buy500Coins
	goto goldenrod_game_corner_EventScript_CancelBuyCoins
	end

goldenrod_game_corner_EventScript_Buy50Coins::
	checkcoins VAR_TEMP_1
	goto_if_ge VAR_TEMP_1, (MAX_COINS + 1 - 50), goldenrod_game_corner_EventScript_NoRoomForCoins
	checkmoney COINS_PRICE_50
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_NotEnoughMoney
	addcoins 50
	removemoney COINS_PRICE_50
	updatemoneybox
	updatecoinsbox 1, 6
	playse SE_SHOP
	msgbox goldenrod_game_corner_Text_ThankYouHereAreYourCoins, MSGBOX_DEFAULT
	hidemoneybox
	hidecoinsbox 0, 5
	release
	end

goldenrod_game_corner_EventScript_Buy500Coins::
	checkcoins VAR_TEMP_1
	goto_if_ge VAR_TEMP_1, (MAX_COINS + 1 - 500), goldenrod_game_corner_EventScript_NoRoomForCoins
	checkmoney COINS_PRICE_500
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_NotEnoughMoney
	addcoins 500
	removemoney COINS_PRICE_500
	updatemoneybox
	updatecoinsbox 1, 6
	playse SE_SHOP
	msgbox goldenrod_game_corner_Text_ThankYouHereAreYourCoins, MSGBOX_DEFAULT
	hidemoneybox
	hidecoinsbox 0, 5
	release
	end

goldenrod_game_corner_EventScript_NotEnoughMoney::
	msgbox goldenrod_game_corner_Text_NotEnoughMoney, MSGBOX_DEFAULT
	hidemoneybox
	hidecoinsbox 0, 5
	release
	end

goldenrod_game_corner_EventScript_CancelBuyCoins::
	msgbox goldenrod_game_corner_Text_CancelBuyCoins, MSGBOX_DEFAULT
	hidemoneybox
	hidecoinsbox 0, 5
	release
	end

goldenrod_game_corner_EventScript_NoRoomForCoins::
	msgbox goldenrod_game_corner_Text_CoinCaseFull, MSGBOX_DEFAULT
	hidemoneybox
	hidecoinsbox 0, 5
	release
	end

goldenrod_game_corner_EventScript_PrizeClerk::
	lock
	faceplayer
	msgbox goldenrod_game_corner_Text_PrizeClerkIntro, MSGBOX_DEFAULT
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_NeedCoinCase
	showcoinsbox 1, 1
	goto goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory

goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory::
	message goldenrod_game_corner_Text_WhichPrize
	waitmessage

goldenrod_game_corner_EventScript_ChoosePrizeCategory::
	multichoice 13, 0, MULTI_GOLDENROD_GAME_CORNER_PRIZE_CATEGORIES, FALSE
	switch VAR_RESULT
	case 0, goldenrod_game_corner_EventScript_ChoosePokemonPrize
	case 1, goldenrod_game_corner_EventScript_ChooseItemPrize
	goto goldenrod_game_corner_EventScript_CancelPrize
	end

goldenrod_game_corner_EventScript_ChoosePokemonPrize::
	message goldenrod_game_corner_Text_WhichPokemonPrize
	waitmessage
	multichoice 11, 0, MULTI_GOLDENROD_GAME_CORNER_POKEMON, FALSE
	switch VAR_RESULT
	case 0, goldenrod_game_corner_EventScript_ConfirmAbra
	case 1, goldenrod_game_corner_EventScript_ConfirmSandshrew
	case 2, goldenrod_game_corner_EventScript_ConfirmDratini
	goto goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory
	end

goldenrod_game_corner_EventScript_ConfirmAbra::
	bufferspeciesname STR_VAR_1, SPECIES_ABRA
	msgbox goldenrod_game_corner_Text_ConfirmPrize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory
	goto goldenrod_game_corner_EventScript_BuyAbra

goldenrod_game_corner_EventScript_ConfirmSandshrew::
	bufferspeciesname STR_VAR_1, SPECIES_SANDSHREW
	msgbox goldenrod_game_corner_Text_ConfirmPrize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory
	goto goldenrod_game_corner_EventScript_BuySandshrew

goldenrod_game_corner_EventScript_ConfirmDratini::
	bufferspeciesname STR_VAR_1, SPECIES_DRATINI
	msgbox goldenrod_game_corner_Text_ConfirmPrize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory
	goto goldenrod_game_corner_EventScript_BuyDratini

goldenrod_game_corner_EventScript_BuyAbra::
	checkcoins VAR_TEMP_2
	goto_if_lt VAR_TEMP_2, ABRA_COINS, goldenrod_game_corner_EventScript_NotEnoughCoins
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_ABRA
	givemon SPECIES_ABRA, 10
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, goldenrod_game_corner_EventScript_ReceiveAbraParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, goldenrod_game_corner_EventScript_ReceiveAbraPC
	goto goldenrod_game_corner_EventScript_NoMoreRoomForPokemonPrize

goldenrod_game_corner_EventScript_ReceiveAbraParty::
	removecoins ABRA_COINS
	updatecoinsbox 1, 1
	playfanfare MUS_OBTAIN_ITEM
	message goldenrod_game_corner_Text_ReceivedAbra
	waitfanfare
	waitmessage
	goto goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory

goldenrod_game_corner_EventScript_ReceiveAbraPC::
	removecoins ABRA_COINS
	updatecoinsbox 1, 1
	playfanfare MUS_OBTAIN_ITEM
	message goldenrod_game_corner_Text_ReceivedAbra
	waitfanfare
	waitmessage
	call Common_EventScript_TransferredToPC
	goto goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory

goldenrod_game_corner_EventScript_BuySandshrew::
	checkcoins VAR_TEMP_2
	goto_if_lt VAR_TEMP_2, SANDSHREW_COINS, goldenrod_game_corner_EventScript_NotEnoughCoins
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_SANDSHREW
	givemon SPECIES_SANDSHREW, 12
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, goldenrod_game_corner_EventScript_ReceiveSandshrewParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, goldenrod_game_corner_EventScript_ReceiveSandshrewPC
	goto goldenrod_game_corner_EventScript_NoMoreRoomForPokemonPrize

goldenrod_game_corner_EventScript_ReceiveSandshrewParty::
	removecoins SANDSHREW_COINS
	updatecoinsbox 1, 1
	playfanfare MUS_OBTAIN_ITEM
	message goldenrod_game_corner_Text_ReceivedSandshrew
	waitfanfare
	waitmessage
	goto goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory

goldenrod_game_corner_EventScript_ReceiveSandshrewPC::
	removecoins SANDSHREW_COINS
	updatecoinsbox 1, 1
	playfanfare MUS_OBTAIN_ITEM
	message goldenrod_game_corner_Text_ReceivedSandshrew
	waitfanfare
	waitmessage
	call Common_EventScript_TransferredToPC
	goto goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory

goldenrod_game_corner_EventScript_BuyDratini::
	checkcoins VAR_TEMP_2
	goto_if_lt VAR_TEMP_2, DRATINI_COINS, goldenrod_game_corner_EventScript_NotEnoughCoins
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_DRATINI
	givemon SPECIES_DRATINI, 15
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, goldenrod_game_corner_EventScript_ReceiveDratiniParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, goldenrod_game_corner_EventScript_ReceiveDratiniPC
	goto goldenrod_game_corner_EventScript_NoMoreRoomForPokemonPrize

goldenrod_game_corner_EventScript_ReceiveDratiniParty::
	removecoins DRATINI_COINS
	updatecoinsbox 1, 1
	playfanfare MUS_OBTAIN_ITEM
	message goldenrod_game_corner_Text_ReceivedDratini
	waitfanfare
	waitmessage
	goto goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory

goldenrod_game_corner_EventScript_ReceiveDratiniPC::
	removecoins DRATINI_COINS
	updatecoinsbox 1, 1
	playfanfare MUS_OBTAIN_ITEM
	message goldenrod_game_corner_Text_ReceivedDratini
	waitfanfare
	waitmessage
	call Common_EventScript_TransferredToPC
	goto goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory

goldenrod_game_corner_EventScript_NoMoreRoomForPokemonPrize::
	msgbox gText_NoMoreRoomForPokemon, MSGBOX_DEFAULT
	goto goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory

goldenrod_game_corner_EventScript_ChooseItemPrize::
	message goldenrod_game_corner_Text_WhichItemPrize
	waitmessage
	multichoice 10, 0, MULTI_GOLDENROD_GAME_CORNER_ITEMS, FALSE
	switch VAR_RESULT
	case 0, goldenrod_game_corner_EventScript_ConfirmSilkScarf
	case 1, goldenrod_game_corner_EventScript_ConfirmThunderbolt
	case 2, goldenrod_game_corner_EventScript_ConfirmIceBeam
	case 3, goldenrod_game_corner_EventScript_ConfirmFlamethrower
	goto goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory
	end

goldenrod_game_corner_EventScript_ConfirmSilkScarf::
	bufferitemname STR_VAR_1, ITEM_SILK_SCARF
	msgbox goldenrod_game_corner_Text_ConfirmPrize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory
	goto goldenrod_game_corner_EventScript_BuySilkScarf

goldenrod_game_corner_EventScript_ConfirmThunderbolt::
	bufferitemname STR_VAR_1, ITEM_TM_THUNDERBOLT
	msgbox goldenrod_game_corner_Text_ConfirmPrize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory
	goto goldenrod_game_corner_EventScript_BuyThunderbolt

goldenrod_game_corner_EventScript_ConfirmIceBeam::
	bufferitemname STR_VAR_1, ITEM_TM_ICE_BEAM
	msgbox goldenrod_game_corner_Text_ConfirmPrize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory
	goto goldenrod_game_corner_EventScript_BuyIceBeam

goldenrod_game_corner_EventScript_ConfirmFlamethrower::
	bufferitemname STR_VAR_1, ITEM_TM_FLAMETHROWER
	msgbox goldenrod_game_corner_Text_ConfirmPrize, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory
	goto goldenrod_game_corner_EventScript_BuyFlamethrower

goldenrod_game_corner_EventScript_BuySilkScarf::
	checkcoins VAR_TEMP_2
	goto_if_lt VAR_TEMP_2, SILK_SCARF_COINS, goldenrod_game_corner_EventScript_NotEnoughCoins
	checkitemspace ITEM_SILK_SCARF
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_BagIsFull
	removecoins SILK_SCARF_COINS
	additem ITEM_SILK_SCARF
	setflag FLAG_RECEIVED_GOLDENROD_GAME_CORNER_SILK_SCARF
	special PokeLink_RefreshWorldState
	updatecoinsbox 1, 1
	playse SE_SHOP
	msgbox goldenrod_game_corner_Text_HereYouGo, MSGBOX_DEFAULT
	goto goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory

goldenrod_game_corner_EventScript_BuyThunderbolt::
	checkcoins VAR_TEMP_2
	goto_if_lt VAR_TEMP_2, TM_THUNDERBOLT_COINS, goldenrod_game_corner_EventScript_NotEnoughCoins
	checkitemspace ITEM_TM_THUNDERBOLT
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_BagIsFull
	removecoins TM_THUNDERBOLT_COINS
	additem ITEM_TM_THUNDERBOLT
	setflag FLAG_RECEIVED_GOLDENROD_GAME_CORNER_TM_THUNDERBOLT
	special PokeLink_RefreshWorldState
	updatecoinsbox 1, 1
	playse SE_SHOP
	msgbox goldenrod_game_corner_Text_HereYouGo, MSGBOX_DEFAULT
	goto goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory

goldenrod_game_corner_EventScript_BuyIceBeam::
	checkcoins VAR_TEMP_2
	goto_if_lt VAR_TEMP_2, TM_ICE_BEAM_COINS, goldenrod_game_corner_EventScript_NotEnoughCoins
	checkitemspace ITEM_TM_ICE_BEAM
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_BagIsFull
	removecoins TM_ICE_BEAM_COINS
	additem ITEM_TM_ICE_BEAM
	setflag FLAG_RECEIVED_GOLDENROD_GAME_CORNER_TM_ICE_BEAM
	special PokeLink_RefreshWorldState
	updatecoinsbox 1, 1
	playse SE_SHOP
	msgbox goldenrod_game_corner_Text_HereYouGo, MSGBOX_DEFAULT
	goto goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory

goldenrod_game_corner_EventScript_BuyFlamethrower::
	checkcoins VAR_TEMP_2
	goto_if_lt VAR_TEMP_2, TM_FLAMETHROWER_COINS, goldenrod_game_corner_EventScript_NotEnoughCoins
	checkitemspace ITEM_TM_FLAMETHROWER
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_BagIsFull
	removecoins TM_FLAMETHROWER_COINS
	additem ITEM_TM_FLAMETHROWER
	setflag FLAG_RECEIVED_GOLDENROD_GAME_CORNER_TM_FLAMETHROWER
	special PokeLink_RefreshWorldState
	updatecoinsbox 1, 1
	playse SE_SHOP
	msgbox goldenrod_game_corner_Text_HereYouGo, MSGBOX_DEFAULT
	goto goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory

goldenrod_game_corner_EventScript_NotEnoughCoins::
	msgbox goldenrod_game_corner_Text_NotEnoughCoins, MSGBOX_DEFAULT
	goto goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory

goldenrod_game_corner_EventScript_BagIsFull::
	call Common_EventScript_BagIsFull
	goto goldenrod_game_corner_EventScript_ReturnToChoosePrizeCategory

goldenrod_game_corner_EventScript_CancelPrize::
	msgbox goldenrod_game_corner_Text_CancelPrize, MSGBOX_DEFAULT
	hidecoinsbox 0, 0
	release
	end

goldenrod_game_corner_EventScript_NeedCoinCase::
	msgbox goldenrod_game_corner_Text_NeedCoinCase, MSGBOX_DEFAULT
	release
	end

goldenrod_game_corner_EventScript_SlotMachine0::
	lockall
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_NoCoinCaseReleaseAll
	setvar VAR_0x8004, 0
	specialvar VAR_RESULT, GetSlotMachineId
	playslotmachine VAR_RESULT
	releaseall
	end

goldenrod_game_corner_EventScript_SlotMachine1::
	lockall
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_NoCoinCaseReleaseAll
	setvar VAR_0x8004, 1
	specialvar VAR_RESULT, GetSlotMachineId
	playslotmachine VAR_RESULT
	releaseall
	end

goldenrod_game_corner_EventScript_SlotMachine2::
	lockall
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_NoCoinCaseReleaseAll
	setvar VAR_0x8004, 2
	specialvar VAR_RESULT, GetSlotMachineId
	playslotmachine VAR_RESULT
	releaseall
	end

goldenrod_game_corner_EventScript_SlotMachine3::
	lockall
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_NoCoinCaseReleaseAll
	setvar VAR_0x8004, 3
	specialvar VAR_RESULT, GetSlotMachineId
	playslotmachine VAR_RESULT
	releaseall
	end

goldenrod_game_corner_EventScript_SlotMachine4::
	lockall
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_NoCoinCaseReleaseAll
	setvar VAR_0x8004, 4
	specialvar VAR_RESULT, GetSlotMachineId
	playslotmachine VAR_RESULT
	releaseall
	end

goldenrod_game_corner_EventScript_SlotMachine5::
	lockall
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_NoCoinCaseReleaseAll
	setvar VAR_0x8004, 5
	specialvar VAR_RESULT, GetSlotMachineId
	playslotmachine VAR_RESULT
	releaseall
	end

goldenrod_game_corner_EventScript_SlotMachine6::
	lockall
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_NoCoinCaseReleaseAll
	setvar VAR_0x8004, 6
	specialvar VAR_RESULT, GetSlotMachineId
	playslotmachine VAR_RESULT
	releaseall
	end

goldenrod_game_corner_EventScript_SlotMachine7::
	lockall
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_NoCoinCaseReleaseAll
	setvar VAR_0x8004, 7
	specialvar VAR_RESULT, GetSlotMachineId
	playslotmachine VAR_RESULT
	releaseall
	end

goldenrod_game_corner_EventScript_SlotMachine8::
	lockall
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_NoCoinCaseReleaseAll
	setvar VAR_0x8004, 8
	specialvar VAR_RESULT, GetSlotMachineId
	playslotmachine VAR_RESULT
	releaseall
	end

goldenrod_game_corner_EventScript_VoltorbFlip::
	lockall
	checkitem ITEM_COIN_CASE
	goto_if_eq VAR_RESULT, FALSE, goldenrod_game_corner_EventScript_NoCoinCaseReleaseAll
	checkcoins VAR_TEMP_2
	goto_if_ge VAR_TEMP_2, (MAX_COINS + 1 - 25), goldenrod_game_corner_EventScript_CoinCaseFullReleaseAll
	msgbox goldenrod_game_corner_Text_VoltorbFlipIntro, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, goldenrod_game_corner_EventScript_VoltorbFlipQuitNoBox
	setvar VAR_TEMP_1, 0
	setvar VAR_TEMP_3, 0
	showcoinsbox 1, 1
	goto goldenrod_game_corner_EventScript_VoltorbFlipRound

goldenrod_game_corner_EventScript_VoltorbFlipRound::
	addvar VAR_TEMP_1, 1
	buffernumberstring STR_VAR_1, VAR_TEMP_1
	buffernumberstring STR_VAR_2, VAR_TEMP_3
	message goldenrod_game_corner_Text_VoltorbFlipRound
	waitmessage
	multichoice 13, 0, MULTI_GOLDENROD_VOLTORB_FLIP, FALSE
	switch VAR_RESULT
	case 0, goldenrod_game_corner_EventScript_VoltorbFlipLeft
	case 1, goldenrod_game_corner_EventScript_VoltorbFlipMiddle
	case 2, goldenrod_game_corner_EventScript_VoltorbFlipRight
	goto goldenrod_game_corner_EventScript_VoltorbFlipCashOut
	end

goldenrod_game_corner_EventScript_VoltorbFlipLeft::
	random 4
	copyvar VAR_TEMP_2, VAR_RESULT
	goto goldenrod_game_corner_EventScript_VoltorbFlipResolve

goldenrod_game_corner_EventScript_VoltorbFlipMiddle::
	random 4
	copyvar VAR_TEMP_2, VAR_RESULT
	goto goldenrod_game_corner_EventScript_VoltorbFlipResolve

goldenrod_game_corner_EventScript_VoltorbFlipRight::
	random 4
	copyvar VAR_TEMP_2, VAR_RESULT
	goto goldenrod_game_corner_EventScript_VoltorbFlipResolve

goldenrod_game_corner_EventScript_VoltorbFlipResolve::
	switch VAR_TEMP_2
	case 0, goldenrod_game_corner_EventScript_VoltorbFlipVoltorb
	case 1, goldenrod_game_corner_EventScript_VoltorbFlipSafeOne
	case 2, goldenrod_game_corner_EventScript_VoltorbFlipSafeTwo
	goto goldenrod_game_corner_EventScript_VoltorbFlipSafeThree
	end

goldenrod_game_corner_EventScript_VoltorbFlipSafeOne::
	addvar VAR_TEMP_3, 1
	msgbox goldenrod_game_corner_Text_VoltorbFlipSafeOne, MSGBOX_DEFAULT
	goto goldenrod_game_corner_EventScript_VoltorbFlipContinue

goldenrod_game_corner_EventScript_VoltorbFlipSafeTwo::
	addvar VAR_TEMP_3, 2
	msgbox goldenrod_game_corner_Text_VoltorbFlipSafeTwo, MSGBOX_DEFAULT
	goto goldenrod_game_corner_EventScript_VoltorbFlipContinue

goldenrod_game_corner_EventScript_VoltorbFlipSafeThree::
	addvar VAR_TEMP_3, 5
	msgbox goldenrod_game_corner_Text_VoltorbFlipSafeThree, MSGBOX_DEFAULT
	goto goldenrod_game_corner_EventScript_VoltorbFlipContinue

goldenrod_game_corner_EventScript_VoltorbFlipContinue::
	goto_if_ge VAR_TEMP_1, 5, goldenrod_game_corner_EventScript_VoltorbFlipCashOut
	buffernumberstring STR_VAR_2, VAR_TEMP_3
	msgbox goldenrod_game_corner_Text_VoltorbFlipKeepGoing, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, goldenrod_game_corner_EventScript_VoltorbFlipCashOut
	goto goldenrod_game_corner_EventScript_VoltorbFlipRound

goldenrod_game_corner_EventScript_VoltorbFlipCashOut::
	goto_if_eq VAR_TEMP_3, 0, goldenrod_game_corner_EventScript_VoltorbFlipNoCoins
	addcoins VAR_TEMP_3
	goto_if_eq VAR_RESULT, TRUE, goldenrod_game_corner_EventScript_VoltorbFlipCoinCaseFull
	updatecoinsbox 1, 1
	buffernumberstring STR_VAR_1, VAR_TEMP_3
	playse SE_SHOP
	msgbox goldenrod_game_corner_Text_VoltorbFlipCashOut, MSGBOX_DEFAULT
	hidecoinsbox 0, 0
	releaseall
	end

goldenrod_game_corner_EventScript_VoltorbFlipVoltorb::
	msgbox goldenrod_game_corner_Text_VoltorbFlipVoltorb, MSGBOX_DEFAULT
	hidecoinsbox 0, 0
	releaseall
	end

goldenrod_game_corner_EventScript_VoltorbFlipNoCoins::
	msgbox goldenrod_game_corner_Text_VoltorbFlipNoCoins, MSGBOX_DEFAULT
	hidecoinsbox 0, 0
	releaseall
	end

goldenrod_game_corner_EventScript_VoltorbFlipCoinCaseFull::
	msgbox goldenrod_game_corner_Text_CoinCaseFull, MSGBOX_DEFAULT
	hidecoinsbox 0, 0
	releaseall
	end

goldenrod_game_corner_EventScript_VoltorbFlipQuitNoBox::
	releaseall
	end

goldenrod_game_corner_EventScript_NoCoinCaseReleaseAll::
	msgbox goldenrod_game_corner_Text_NeedCoinCase, MSGBOX_DEFAULT
	releaseall
	end

goldenrod_game_corner_EventScript_CoinCaseFullReleaseAll::
	msgbox goldenrod_game_corner_Text_CoinCaseFull, MSGBOX_DEFAULT
	releaseall
	end

goldenrod_game_corner_EventScript_ArcadeAttendant::
	msgbox goldenrod_game_corner_Text_ArcadeAttendantAfter, MSGBOX_NPC
	end

goldenrod_game_corner_EventScript_SlotsFan::
	msgbox goldenrod_game_corner_Text_SlotsFan, MSGBOX_NPC
	end

goldenrod_game_corner_EventScript_VoltorbFan::
	msgbox goldenrod_game_corner_Text_VoltorbFan, MSGBOX_NPC
	end

goldenrod_game_corner_EventScript_PrizeFan::
	msgbox goldenrod_game_corner_Text_PrizeFan, MSGBOX_NPC
	end

goldenrod_game_corner_EventScript_TiredGambler::
	msgbox goldenrod_game_corner_Text_TiredGambler, MSGBOX_NPC
	end

goldenrod_game_corner_Text_CoinCaseGiverIntro::
	.string "You came to the GAME CORNER\n"
	.string "without a COIN CASE?\p"
	.string "I keep a spare for people\n"
	.string "who learn that lesson once.$"

goldenrod_game_corner_Text_CoinCaseGiverAfter::
	.string "The COIN CASE keeps your\n"
	.string "coins out of your BAG.\p"
	.string "That makes losing track of\n"
	.string "them feel official.$"

goldenrod_game_corner_Text_CoinsClerkIntro::
	.string "Welcome to the GOLDENROD\n"
	.string "GAME CORNER.$"

goldenrod_game_corner_Text_BuyCoinsPrompt::
	.string "Would you like to buy\n"
	.string "some coins?$"

goldenrod_game_corner_Text_ThankYouHereAreYourCoins::
	.string "Thank you!\n"
	.string "Here are your coins.$"

goldenrod_game_corner_Text_NotEnoughMoney::
	.string "You do not have enough\n"
	.string "money.$"

goldenrod_game_corner_Text_CancelBuyCoins::
	.string "Take your time.\n"
	.string "The machines will wait.$"

goldenrod_game_corner_Text_CoinCaseFull::
	.string "Your COIN CASE is full.$"

goldenrod_game_corner_Text_NeedCoinCase::
	.string "You need a COIN CASE\n"
	.string "to use coins here.$"

goldenrod_game_corner_Text_PrizeClerkIntro::
	.string "Coins may be exchanged\n"
	.string "for prizes here.$"

goldenrod_game_corner_Text_WhichPrize::
	.string "Which prize counter\n"
	.string "would you like?$"

goldenrod_game_corner_Text_WhichPokemonPrize::
	.string "Which POKEMON prize\n"
	.string "would you like?$"

goldenrod_game_corner_Text_WhichItemPrize::
	.string "Which item prize\n"
	.string "would you like?$"

goldenrod_game_corner_Text_ConfirmPrize::
	.string "So your choice is\n"
	.string "{STR_VAR_1}?$"

goldenrod_game_corner_Text_NotEnoughCoins::
	.string "You do not have enough\n"
	.string "coins.$"

goldenrod_game_corner_Text_CancelPrize::
	.string "Please come back after\n"
	.string "winning more coins.$"

goldenrod_game_corner_Text_HereYouGo::
	.string "Here you go!\n"
	.string "Use it well.$"

goldenrod_game_corner_Text_ReceivedAbra::
	.string "{PLAYER} received ABRA!$"

goldenrod_game_corner_Text_ReceivedSandshrew::
	.string "{PLAYER} received SANDSHREW!$"

goldenrod_game_corner_Text_ReceivedDratini::
	.string "{PLAYER} received DRATINI!$"

goldenrod_game_corner_Text_VoltorbFlipIntro::
	.string "VOLTORB FLIP is a small\n"
	.string "risk table.\p"
	.string "Pick cards to bank coins.\n"
	.string "Pick VOLTORB and lose them.\p"
	.string "Play a round?$"

goldenrod_game_corner_Text_VoltorbFlipRound::
	.string "Round {STR_VAR_1}.\n"
	.string "Banked coins: {STR_VAR_2}\p"
	.string "Pick a card.$"

goldenrod_game_corner_Text_VoltorbFlipSafeOne::
	.string "Safe card!\n"
	.string "You banked 1 coin.$"

goldenrod_game_corner_Text_VoltorbFlipSafeTwo::
	.string "A 2-card!\n"
	.string "You banked 2 coins.$"

goldenrod_game_corner_Text_VoltorbFlipSafeThree::
	.string "A 5-card!\n"
	.string "You banked 5 coins.$"

goldenrod_game_corner_Text_VoltorbFlipKeepGoing::
	.string "You have {STR_VAR_2} coins\n"
	.string "banked.\p"
	.string "Flip again?$"

goldenrod_game_corner_Text_VoltorbFlipCashOut::
	.string "You cashed out\n"
	.string "{STR_VAR_1} coins!$"

goldenrod_game_corner_Text_VoltorbFlipVoltorb::
	.string "VOLTORB!\p"
	.string "The table clears your\n"
	.string "banked coins.$"

goldenrod_game_corner_Text_VoltorbFlipNoCoins::
	.string "No coins banked.\n"
	.string "You stepped away.$"

goldenrod_game_corner_Text_ArcadeAttendantAfter::
	.string "POKELINK games are good\n"
	.string "between battles.\p"
	.string "Real coins are for the\n"
	.string "machines, though.$"

goldenrod_game_corner_Text_SlotsFan::
	.string "I count three spins,\n"
	.string "then breathe.\p"
	.string "It does not help, but\n"
	.string "it feels professional.$"

goldenrod_game_corner_Text_VoltorbFan::
	.string "VOLTORB FLIP rewards\n"
	.string "knowing when to leave.$"

goldenrod_game_corner_Text_PrizeFan::
	.string "The prize DRATINI is\n"
	.string "expensive for a reason.$"

goldenrod_game_corner_Text_TiredGambler::
	.string "I had a hot streak.\n"
	.string "Then I kept playing.$"
