

;struc struc_atexit_t ; (sizeof=0x8)
;atexit_t dw 4 dup(?)
;ends struc_atexit_t
;
;
;struc FILE ; (sizeof=0x14, standard type)
;level dw ?
;flags dw ?
;fd  db ?
;hold db	?
;bsize dw ?
;buffer dd ?
;curp dd	?
;istemp dw ?
;token dw ?
;ends FILE
;
;
;struc BCpp30_LdDesc ; (sizeof=0xFF)
;LdErrorMsg db 15 dup(?)
;LdStack	db 128 dup(?)
;LdPSP dw ?
;LdPathName db 80 dup(?)
;LdAX dw	?
;LdExeSignature dw ?
;LdLength dw ?
;LdNbPages dw ?
;LdNbItems dw ?
;LdHdrSize dw ?
;LdMin dw ?
;LdMax dw ?
;LdSS dw	?
;LdSP dw	?
;LdCheckSum dw ?
;LdIP dw	?
;LdCS dw	?
;LdLoadAddr dw ?
;LdRelocFactor dw ?
;ends BCpp30_LdDesc
;
;
;struc BCpp30_StartExit ; (sizeof=0x5)
;calltype db ?
;priority db ?
;addrlow	dw ?
;addrhigh db ?
;ends BCpp30_StartExit
;
;
;struc BCpp30_DOSH_dosSearchInfo	; (sizeof=0x2B)
;ds_drive db ?
;ds_pattern db 13 dup(?)
;ds_reserved db 7 dup(?)
;ds_attrib db ?
;ds_time	dw ?
;ds_date	dw ?
;ds_size	dd ?
;ds_nameZ db 13 dup(?)
;ends BCpp30_DOSH_dosSearchInfo
;
;
;struc _stub_descr ; (sizeof=0x20)
;int_code db 2 dup(?)
;memswap	dw ?
;fileoff	dd ?
;codesize dw ?
;relsize	dw ?
;nentries dw ?
;prevstub dw ?
;workarea db 16 dup(?)
;ends _stub_descr
;
;
;struc struct_CitySizes ; (sizeof=0x29)
;Outpost	db 8 dup(?)
;Hamlet db 7 dup(?)
;Village	db 8 dup(?)
;Town db	5 dup(?)
;City db	5 dup(?)
;Capital	db 8 dup(?)
;ends struct_CitySizes
;
;
;struc struct_CityEnchantments ;	(sizeof=0x16E)
;SummoningCircl db 17 dup(?)
;EarthGate db 11	dup(?)
;StreamOfLife db	15 dup(?)
;AstralGate db 12 dup(?)
;Fortress db 9 dup(?)
;DarkRituals db 13 dup(?)
;AltarOfBattle db 16 dup(?)
;WallOfFire db 13 dup(?)
;ChaosRift db 11	dup(?)
;EvilPresence db	14 dup(?)
;CursedLands db 13 dup(?)
;Pestilence db 11 dup(?)
;CloudOfShadow db 16 dup(?)
;Famine db 7 dup(?)
;Flying db 7 dup(?)
;NatureWard db 12 dup(?)
;SorceryWard db 13 dup(?)
;ChaosWard db 11	dup(?)
;LifeWard db 10 dup(?)
;DeathWard db 11	dup(?)
;NatureSEye db 13 dup(?)
;StreamOfLife_0 db 15 dup(?)
;GaiasBlessing db 16 dup(?)
;Inspirations db	13 dup(?)
;Prosperity db 11 dup(?)
;HeavenlyLight db 15 dup(?)
;Consecration db	13 dup(?)
;WallOfDarkness db 17 dup(?)
;Nightshade db 11 dup(?)
;ends struct_CityEnchantments
;
;
;struc struct_HeroTitles	; (sizeof=0x130)
;aDwarf db 6 dup(?)
;aBarbarian db 10 dup(?)
;aSage db 5 dup(?)
;aDervish db 8 dup(?)
;aBeastmaster db	12 dup(?)
;aBard db 5 dup(?)
;aOrcWarrior db 12 dup(?)
;aHealer	db 7 dup(?)
;aHuntress db 9 dup(?)
;aThief db 6 dup(?)
;aDruid db 6 dup(?)
;aWarMonk db 9 dup(?)
;aWarriorMage db	13 dup(?)
;aMagician db 9 dup(?)
;aAssassin db 9 dup(?)
;aWindMage db 10	dup(?)
;aRanger	db 7 dup(?)
;aDraconian db 10 dup(?)
;aWitch db 6 dup(?)
;aGoldenOne db 11 dup(?)
;aNinja db 6 dup(?)
;aRogue db 6 dup(?)
;aAmazon	db 7 dup(?)
;aWarlock db 8 dup(?)
;aUnknown db 8 dup(?)
;aIllusionist db	12 dup(?)
;aSwordsman db 10 dup(?)
;aPriestess db 10 dup(?)
;aPaladin db 8 dup(?)
;aBlackKnight db	13 dup(?)
;aElvenArcher db	13 dup(?)
;aNecromancer db	12 dup(?)
;aChaosWarrior db 14 dup(?)
;aChosen	db 7 dup(?)
;ends struct_HeroTitles
;
;
;struc struct_UnitTypes ; (sizeof=0x37E)
;aTrireme db 8 dup(?)
;aGalley	db 7 dup(?)
;aCatapult db 9 dup(?)
;aWarship db 8 dup(?)
;aSpearmen db 9 dup(?)
;aSwordsmen db 10 dup(?)
;aBowmen	db 7 dup(?)
;aCavalry db 8 dup(?)
;aShaman	db 7 dup(?)
;aSettlers db 9 dup(?)
;aBerserkers db 11 dup(?)
;aHalberdiers db	12 dup(?)
;aPriests db 8 dup(?)
;aMagicians db 10 dup(?)
;aEngineers db 10 dup(?)
;aCentaurs db 9 dup(?)
;aManticores db 11 dup(?)
;aMinotaurs db 10 dup(?)
;aNightblades db	12 dup(?)
;aWarlocks db 9 dup(?)
;aNightmares db 11 dup(?)
;aDoomDrakes db 12 dup(?)
;aAirShip db 9 dup(?)
;aHammerhands db	12 dup(?)
;aSteamCannon db	13 dup(?)
;aGolem db 6 dup(?)
;aWolfRiders db 12 dup(?)
;aSlingers db 9 dup(?)
;aLongbowmen db 11 dup(?)
;aElvenLords db 12 dup(?)
;aPegasai db 8 dup(?)
;aPikemen db 8 dup(?)
;aPaladins db 9 dup(?)
;aStagBeetle db 12 dup(?)
;aJavelineers db	12 dup(?)
;aDragonTurtle db 14 dup(?)
;aHorsebowmen db	12 dup(?)
;aRangers db 8 dup(?)
;aGriffins db 9 dup(?)
;aWyvernRiders db 14 dup(?)
;aWarTrolls db 11 dup(?)
;aWarMammoths db	13 dup(?)
;aMagicSpirit db	13 dup(?)
;aHellHounds db 12 dup(?)
;aGargoyles db 10 dup(?)
;aFireGiant db 11 dup(?)
;aFireElemental db 15 dup(?)
;aChaosSpawn db 12 dup(?)
;aChimera db 8 dup(?)
;aDoomBat db 9 dup(?)
;aEfreet	db 7 dup(?)
;aHydra db 6 dup(?)
;aGreatDrake db 12 dup(?)
;aSkeletons db 10 dup(?)
;aGhouls	db 7 dup(?)
;aNightStalker db 14 dup(?)
;aWerewolves db 11 dup(?)
;aDemon db 6 dup(?)
;aWraiths db 8 dup(?)
;aShadowDemons db 14 dup(?)
;aDeathKnights db 14 dup(?)
;aDemonLord db 11 dup(?)
;aZombies db 8 dup(?)
;aUnicorns db 9 dup(?)
;aGuardianSpirit	db 16 dup(?)
;aAngel db 6 dup(?)
;aArchAngel db 11 dup(?)
;aWarBears db 10	dup(?)
;aSprites db 8 dup(?)
;aCockatrices db	12 dup(?)
;aBasilisk db 9 dup(?)
;aGiantSpiders db 14 dup(?)
;aStoneGiant db 12 dup(?)
;aColossus db 9 dup(?)
;aGorgons db 8 dup(?)
;aEarthElemental	db 16 dup(?)
;aBehemoth db 9 dup(?)
;aGreatWyrm db 11 dup(?)
;aFloatingIsland	db 16 dup(?)
;aPhantomBeast db 14 dup(?)
;aPhantomWarrior	db 17 dup(?)
;aStormGiant db 12 dup(?)
;aAirElemental db 14 dup(?)
;aDjinn db 6 dup(?)
;aSkyDrake db 10	dup(?)
;aNagas db 6 dup(?)
;ends struct_UnitTypes
;
;
;struc struct_Units ; (sizeof=0x37E)
;Trireme	db 8 dup(?)
;Galley db 7 dup(?)
;Catapult db 9 dup(?)
;Warship	db 8 dup(?)
;Spearmen db 9 dup(?)
;Swordsmen db 10	dup(?)
;Bowmen db 7 dup(?)
;Cavalry	db 8 dup(?)
;Shaman db 7 dup(?)
;Settlers db 9 dup(?)
;Berserkers db 11 dup(?)
;Halberdiers db 12 dup(?)
;Priests	db 8 dup(?)
;Magicians db 10	dup(?)
;Engineers db 10	dup(?)
;Centaurs db 9 dup(?)
;Manticores db 11 dup(?)
;Minotaurs db 10	dup(?)
;Nightblades db 12 dup(?)
;Warlocks db 9 dup(?)
;Nightmares db 11 dup(?)
;DoomDrakes db 12 dup(?)
;AirShip	db 9 dup(?)
;Hammerhands db 12 dup(?)
;SteamCannon db 13 dup(?)
;Golem db 6 dup(?)
;WolfRiders db 12 dup(?)
;Slingers db 9 dup(?)
;Longbowmen db 11 dup(?)
;ElvenLords db 12 dup(?)
;Pegasai	db 8 dup(?)
;Pikemen	db 8 dup(?)
;Paladins db 9 dup(?)
;StagBeetle db 12 dup(?)
;Javelineers db 12 dup(?)
;DragonTurtle db	14 dup(?)
;Horsebowmen db 12 dup(?)
;Rangers	db 8 dup(?)
;Griffins db 9 dup(?)
;WyvernRiders db	14 dup(?)
;WarTrolls db 11	dup(?)
;WarMammoths db 13 dup(?)
;MagicSpirit db 13 dup(?)
;HellHounds db 12 dup(?)
;Gargoyles db 10	dup(?)
;FireGiant db 11	dup(?)
;FireElemental db 15 dup(?)
;ChaosSpawn db 12 dup(?)
;Chimera	db 8 dup(?)
;DoomBat	db 9 dup(?)
;Efreet db 7 dup(?)
;Hydra db 6 dup(?)
;GreatDrake db 12 dup(?)
;Skeletons db 10	dup(?)
;Ghouls db 7 dup(?)
;NightStalker db	14 dup(?)
;Werewolves db 11 dup(?)
;Demon db 6 dup(?)
;Wraiths	db 8 dup(?)
;ShadowDemons db	14 dup(?)
;DeathKnights db	14 dup(?)
;DemonLord db 11	dup(?)
;Zombies	db 8 dup(?)
;Unicorns db 9 dup(?)
;GuardianSpirit db 16 dup(?)
;Angel db 6 dup(?)
;ArchAngel db 11	dup(?)
;WarBears db 10 dup(?)
;Sprites	db 8 dup(?)
;Cockatrices db 12 dup(?)
;Basilisk db 9 dup(?)
;GiantSpiders db	14 dup(?)
;StoneGiant db 12 dup(?)
;Colossus db 9 dup(?)
;Gorgons	db 8 dup(?)
;EarthElemental db 16 dup(?)
;Behemoth db 9 dup(?)
;GreatWyrm db 11	dup(?)
;FloatingIsland db 16 dup(?)
;PhantomBeast db	14 dup(?)
;PhantomWarriors	db 17 dup(?)
;StormGiant db 12 dup(?)
;AirElemental db	14 dup(?)
;Djinn db 6 dup(?)
;SkyDrake db 10 dup(?)
;Nagas db 6 dup(?)
;ends struct_Units
;
;
;struc struct_Races ; (sizeof=0x5D)
;Beastmen db 9 dup(?)
;DarkElf	db 9 dup(?)
;Dwarven	db 8 dup(?)
;Gnoll db 6 dup(?)
;Halfling db 9 dup(?)
;HighElf	db 9 dup(?)
;HighMen	db 9 dup(?)
;Klackon	db 8 dup(?)
;Lizardman db 10	dup(?)
;Nomad db 6 dup(?)
;Orc db 4 dup(?)
;Troll db 6 dup(?)
;ends struct_Races
;
;
;struc struct_MagicRealms ; (sizeof=0x22)
;aArcane	db 7 dup(?)
;aNature	db 7 dup(?)
;aSorcery db 8 dup(?)
;aChaos db 6 dup(?)
;aDeath db 6 dup(?)
;ends struct_MagicRealms
;
;
;struc struct_SpecialAbilities ;	(sizeof=0xCB)
;Alchemy	db 8 dup(?)
;Warlord	db 8 dup(?)
;ChaosMastery db	14 dup(?)
;NatureMastery db 15 dup(?)
;SorceryMastery db 16 dup(?)
;InfernalPower db 15 dup(?)
;DivinePower db 13 dup(?)
;SageMaster db 12 dup(?)
;Channeler db 10	dup(?)
;Myrran db 7 dup(?)
;Archmage db 9 dup(?)
;ManaFocusing db	14 dup(?)
;NodeMastery db 13 dup(?)
;Famous db 7 dup(?)
;Runemaster db 11 dup(?)
;Conjurer db 9 dup(?)
;Charismatic db 12 dup(?)
;Artificer db 10	dup(?)
;ends struct_SpecialAbilities
;
;
;; enum magic
;e_FLI_MagSig  =	0AF11h
;e_FLI_Frame_MagSig  = 0F1FAh
;e_LBX_MagSig  =	0FEADh
;
;
;; enum VGA
;MAP_MASK  = 2
;MEMORY_MODE  = 4
;VGA_256_COLOR_MODE  = 13h
;CRTC_MODE_CONTROL  = 17h
;SC_INDEX  = 3C4h
;SC_DATA	 = 3C5h
;PALETTE_INDEX  = 3C8h
;PALETTE_DATA  =	3C9h
;GC_INDEX  = 3CEh
;GC_DATA	 = 3CFh
;CRTC_INDEX  = 3D4h
INPUT_STATUS  =	3DAh
;VIDEO_RAM  = 0A000h
;TEXT_MODE_VIDEO_RAM  = 0B800h


; enum BIOS_Data_Area
DAILY_TIMER_COUNTER_lw	= 46Ch
DAILY_TIMER_COUNTER_hw	= 46Eh


;; enum length_size_offset
;LBX_HEADER_LENGTH  = 200h
;
;
;; enum enum_MS_DOS_EXE
;e_MsDosExe_MagSig  = 5A4Dh
;
;
;; enum LBX_ErrorType
;e_LBX_ErrorType_0  = 0
;e_NOT_FOUND  = 1
;e_CORRUPTED  = 2
;e_INSUFFICIENT_MEMORY  = 3
;e_MISALLOCATED	= 4
;e_ALLOCATION_TOO_SMALL	= 5
;e_Whoops  = 6
;e_NOT_AN_LBX  =	7
;e_EXCEEDS_LBX_ENTRIES  = 8
;e_INCORRECT_RECORD_SIZE	 = 9
;e_EXCEEDS_DEFINED_RECORDS  = 0Ah
;e_EMS_RELOAD_WO_RELEASE	 = 0Bh
;e_INSUFFICIENT_EMM  = 0Ch
;e_RESERVED_EMM_PICTURES_ONLY  =	0Dh
;e_EMM_LOAD_ERROR  = 0Eh
;e_LBX_ErrorType_15  = 0Fh
;e_VGA_FRAME_EXCEEDS_65535  = 10h
;
;
;; enum enum_MEM_Error_Type
;enum_MEM_Error_Type_0  = 0
;enum_MEM_Error_Type_1  = 1
;enum_MEM_Error_Type_2  = 2
;enum_MEM_Error_Type_3  = 3
;enum_MEM_Error_Type_4  = 4
;
;
;; enum e_SoundDrivers
;SNDRV_9_ROLAND	= 9
;
;
;; enum enum_BCpp30_StartUp
;dPtrSize_div2  = 1
;PSPHigh	 = 2
;dPtrSize_mul4  = 8
;lgth_ZeroDivMSG	 = 0Eh
;lgth_NullCheck	= 19h
;lgth_abortMSG  = 1Eh
;PSPEnv	= 2Ch
;lgth_CopyRight	= 2Fh
;PSPCmd	= 80h
;MINSTACK_mul2  = 200h
;CheckSum  = 0D5Ch
;edata@	= 0AA4Ah
;
;
;; enum enum_BCpp30_StartExit
;e_PNEAR	 = 0
;e_PFAR	= 1
;e_NOTUSED  = 0FFh
;
;
;; enum enum_BCpp_RTL
;sys_nerr  = 30h
;e_parameter  = 57h
;e_dosFinalError	 = 58h

