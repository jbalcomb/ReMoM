#ifndef UNITTYPE_H
#define UNITTYPE_H

#include "MoM.H"



enum e_UNIT_TYPE
{
    ut_Dwarf  = 0,
    ut_Barbarian  = 1,
    ut_Sage  = 2,
    ut_Dervish  = 3,
    ut_Beastmaster  = 4,
    ut_Bard  = 5,
    ut_OrcWarrior  = 6,
    ut_Healer  = 7,
    ut_Huntress  = 8,
    ut_Thief  = 9,
    ut_Druid  = 10,
    ut_War_Monk  = 11,
    ut_WarriorMage  = 12,
    ut_Magician  = 13,
    ut_Assassin  = 14,
    ut_WindMage  = 15,
    ut_Ranger  = 16,
    ut_Draconian  = 17,
    ut_Witch  = 18,
    ut_GoldenOne  = 19,
    ut_Ninja  = 20,
    ut_Rogue  = 21,
    ut_Amazon  = 22,
    ut_Warlock  = 23,
    ut_Unknown  = 24,
    ut_Illusionist  = 25,
    ut_Swordsman  = 26,
    ut_Priestess  = 27,
    ut_Paladin  = 28,
    ut_BlackKnight  = 29,
    ut_ElvenArcher  = 30,
    ut_Knight  = 31,
    ut_Necromancer  = 32,
    ut_ChaosWarrior  = 33,
    ut_Chosen  = 34,
    ut_Trireme  = 35,
    ut_Galley  = 36,
    ut_Catapult  = 37,
    ut_Warship  = 38,
    ut_BarbSpearmen  = 39,
    ut_BarbSwordsmen  = 40,
    ut_BarbBowmen  = 41,
    ut_BarbCavalry  = 42,
    ut_BarbShaman  = 43,
    ut_BarbSettlers  = 44,
    ut_Berserkers  = 45,
    ut_BeastSpearmen  = 46,
    ut_BeastSwordsmen  = 47,
    ut_BeastHalberdiers  = 48,
    ut_BeastBowmen  = 49,
    ut_BeastPriests  = 50,
    ut_BeastMagicians  = 51,
    ut_BeastEngineers  = 52,
    ut_BeastSettlers  = 53,
    ut_Centaurs  = 54,
    ut_Manticores  = 55,
    ut_Minotaurs  = 56,
    ut_DrowSpearmen  = 57,
    ut_DrowSwordsmen  = 58,
    ut_DrowHalberdiers  = 59,
    ut_DrowCavalry  = 60,
    ut_DrowPriests  = 61,
    ut_DrowSettlers  = 62,
    ut_Nightblades  = 63,
    ut_Warlocks  = 64,
    ut_Nightmares  = 65,
    ut_DracSpearmen  = 66,
    ut_DracSwordsmen  = 67,
    ut_DracHalberdiers  = 68,
    ut_DracBowmen  = 69,
    ut_DracShaman  = 70,
    ut_DracMagicians  = 71,
    ut_DracEngineers  = 72,
    ut_DracSettlers  = 73,
    ut_DoomDrakes  = 74,
    ut_AirShip  = 75,
    ut_DwarfSwordsmen  = 76,
    ut_DwarfHalberdiers  = 77,
    ut_DwarfEngineers  = 78,
    ut_Hammerhands  = 79,
    ut_SteamCannon  = 80,
    ut_Golem  = 81,
    ut_DwarfSettlers  = 82,
    ut_GnollSpearmen  = 83,
    ut_GnollSwordsmen  = 84,
    ut_GnollHalberdiers  = 85,
    ut_GnollBowmen  = 86,
    ut_GnollSettlers  = 87,
    ut_WolfRiders  = 88,
    ut_HflngSpearmen  = 89,
    ut_HflngSwordsmen  = 90,
    ut_HflngBowmen  = 91,
    ut_HflngShaman  = 92,
    ut_HflngSettlers  = 93,
    ut_Slingers  = 94,
    ut_HElfSpearmen  = 95,
    ut_HElfSwordsmen  = 96,
    ut_HElfHalberdiers  = 97,
    ut_HElfCavalry  = 98,
    ut_HElfMagicians  = 99,
    ut_HElfSettlers  = 100,
    ut_Longbowmen  = 101,
    ut_ElvenLords  = 102,
    ut_Pegasai  = 103,
    ut_HMenSpearmen  = 104,
    ut_HMenSwordsmen  = 105,
    ut_HMenBowmen  = 106,
    ut_HMenCavalry  = 107,
    ut_HMenPriests  = 108,
    ut_HMenMagicians  = 109,
    ut_HMenEngineers  = 110,
    ut_HMenSettlers  = 111,
    ut_HMenPikemen  = 112,
    ut_Paladins  = 113,
    ut_KlckSpearmen  = 114,
    ut_KlckSwordsmen  = 115,
    ut_KlckHalberdiers  = 116,
    ut_Klck_Engineers  = 117,
    ut_KlckSettlers  = 118,
    ut_StagBeetle  = 119,
    ut_LizSpearmen  = 120,
    ut_LizSwordsmen  = 121,
    ut_LizHalberdiers  = 122,
    ut_Javelineers  = 123,
    ut_LizShaman  = 124,
    ut_LizSettlers  = 125,
    ut_DragonTurtle  = 126,
    ut_NmdSpearmen  = 127,
    ut_NmdSwordsmen  = 128,
    ut_NmdBowmen  = 129,
    ut_NmdPriests  = 130,
    ut_NmdMagicians  = 131,
    ut_NmdSettlers  = 132,
    ut_Horsebowmen  = 133,
    ut_NmdPikemen  = 134,
    ut_Rangers  = 135,
    ut_Griffins  = 136,
    ut_OrcSpearmen  = 137,
    ut_OrcSwordsmen  = 138,
    ut_OrcHalberdiers  = 139,
    ut_OrcBowmen  = 140,
    ut_OrcCavalry  = 141,
    ut_OrcShaman  = 142,
    ut_OrcMagicians  = 143,
    ut_OrcEngineers  = 144,
    ut_OrcSettlers  = 145,
    ut_WyvernRiders  = 146,
    ut_TrlSpearmen  = 147,
    ut_TrlSwordsmen  = 148,
    ut_TrlHalberdiers  = 149,
    ut_TrlShaman  = 150,
    ut_TrlSettlers  = 151,
    ut_WarTrolls  = 152,
    ut_WarMammoths  = 153,
    ut_Magic_Spirit  = 154,
    ut_Hell_Hounds  = 155,
    ut_Gargoyles  = 156,
    ut_Fire_Giant  = 157,
    ut_Fire_Elemental  = 158,
    ut_Chaos_Spawn  = 159,
    ut_Chimera  = 160,
    ut_Doom_Bat  = 161,
    ut_Efreet  = 162,
    ut_Hydra  = 163,
    ut_Great_Drake  = 164,
    ut_Skeletons  = 165,
    ut_Ghouls  = 166,
    ut_Night_Stalker  = 167,
    ut_Werewolves  = 168,
    ut_Demon  = 169,
    ut_Wraiths  = 170,
    ut_Shadow_Demons  = 171,
    ut_Death_Knights  = 172,
    ut_Demon_Lord  = 173,
    ut_Zombies  = 174,
    ut_Unicorns  = 175,
    ut_Guardian_Spirit  = 176,
    ut_Angel  = 177,
    ut_Arch_Angel  = 178,
    ut_War_Bears  = 179,
    ut_Sprites  = 180,
    ut_Cockatrices  = 181,
    ut_Basilisk  = 182,
    ut_Giant_Spiders  = 183,
    ut_Stone_Giant  = 184,
    ut_Colossus  = 185,
    ut_Gorgons  = 186,
    ut_Earth_Elemental  = 187,
    ut_Behemoth  = 188,
    ut_Great_Wyrm  = 189,
    ut_Floating_Island  = 190,
    ut_Phantom_Beast  = 191,
    ut_Phantom_Warriors  = 192,
    ut_Storm_Giant  = 193,
    ut_Air_Elemental  = 194,
    ut_Djinn  = 195,
    ut_Sky_Drake  = 196,
    ut_Nagas  = 197,
    UNIT_TYPE_MAX  = 198
};


/*
    (Special) Ranged Attack Types

2-Byte, Signed
"Special"   {100, 101, 102, 103, 104, 105}

*/
enum e_RANGED_ATTACK_TYPE
{
    rat_None         =  -1,
    /* rat_None         =   0, ... BU_IsRangedUnit() checks > 0 */

    rat_Rock         =  10,
    rat_Cannon       =  11,

    rat_Bow          =  20,
    rat_Sling        =  21,
    rat_Unknown      =  22,

    rat_Lightning    =  30,
    rat_Fireball     =  31,
    rat_Sorcery      =  32,
    rat_Deathbolt    =  33,
    rat_Icebolt      =  34,
    rat_Pr_Shaman    =  35,
    rat_Drow         =  36,
    rat_Sprite       =  37,
    rat_Nat_Bolt     =  38,

    srat_Thrown      = 100,
    srat_FireBreath  = 101,
    srat_Lightning   = 102,
    srat_StoneGaze   = 103,
    srat_MultiGaze   = 104,
    srat_DeathGaze   = 105
};

enum e_RANGED_ATTACK_GROUP
{
    rag_Boulder = 1,  /* { 10, ...,  19} / 10 */
    rag_Missile = 2,  /* { 20, ...,  29} / 10 */
    rag_Magic   = 3,  /* { 30, ...,  39} / 10 */
    rag_Short   = 10  /* {100, ..., 109} / 10 */
};

// sizeof:  24h  36d
#pragma pack(push)
#pragma pack(2)
struct s_UNIT_TYPE
{
    /* 00 */  char **  name;                    /* offset (00036AA0) */
    /* 02 */  int8_t   Melee;
    /* 03 */  int8_t   Ranged;
    /* 04 */  int8_t   Ranged_Type;             // enum e_RANGED_ATTACK_TYPE
    /* 05 */  int8_t   Ammo;
    /* 06 */  int8_t   To_Hit;
    /* 07 */  int8_t   Defense;
    /* 08 */  int8_t   Resist;
    /* 09 */  int8_t   Move_Halves;
    /* 0A */  int16_t  Cost;
    /* 0C */  int8_t   Upkeep;
    /* 0D */  int8_t   Race;
    /* 0E */  // int8_t   Bldng1_or_Portrait;      /* enum BLD_TYPE */
    union
    {
        int8_t   reqd_bldg_1;
        int8_t   hero_portrait;  /* entry_num PORTAITS.LBX */
    };
    /* 0F */  // int8_t   Bldng2_or_HeroType;      /* enum BLD_TYPE */
    union
    {
        int8_t   reqd_bldg_2;
        int8_t   hero_type;  /* used by Hero_Slot_Types() to determine the 3 item slots slot type */
    };
    /* 10 */  SAMB_ptr pict_seg;                /* segment pointers to LBX_Alloc_Space headers for reserved EMM file links */
    /* 12 */  int8_t   Hits;
    /* 13 */  int8_t   Sight;
    /* 14 */  int8_t   Transport;
    /* 15 */  int8_t   Figures;
    /* 16 */  int8_t   Construction;
    /* 17 */  int8_t   Spec_Att_Attr;
    /* 18 */  uint16_t Move_Flags;              /* enum MOVEFLAGS */  /* TODO: check the data-type - UnitHasFlying() uses `byte ptr` and `CBW` */
    /* 1A */  int16_t Attribs_1;               /* enum ATTRIB_1 */
    /* 1C */  int8_t Attribs_2;               /* enum ATTRIB_2 */
    /* 1B */  // 2-byte alignment padding
    /* 1E */  uint16_t Abilities;               /* enum ABL_FLAGS */  // macros - UA_...
    /* 20 */  uint16_t Attack_Flags;            /* enum ATK_FLAGS */
    /* 22 */  int16_t  Sound;
};
#pragma pack(pop)



/*

    BEGIN: STU-MoXer :: Generate_Unit_Type_Data()

*/
extern char * _unit_type_names[];
extern struct s_UNIT_TYPE _unit_type_table[];



#endif  /* UNITTYPE_H */
/*
WIZARDS.EXE
Data Segment (DSEG)
    019C
    1D74
    (1D74 - 019C) = 1BD8
    (7540 - 412)  = 7128  (7128 / 36) = 198

24h 36d per element

3E8 - 19C = 24C
1000 - 412 = 588
...16 * 36 = 576 + 12 = 588 ... 

[0] D3 21 05 00 FF 00 00 04 0A 04 64 00 02 04 0A 00 00 00 0A 01 00 01 00 00 41 00 00 00 00 00 00 00 00 00 00 00
[1] D9 21 05 03 64 00 00 03 06 04 64 00 02 00 11 00 01 00 09 01 00 01 00 00 01 00 00 00 00 00 00 00 00 00 00 00
[2] E3 21 01 06 20 00 00 04 06 04 64 00 02 08 1C 03 02 00 05 01 00 01 00 00 01 00 00 00 00 00 00 00 00 00 00 00
[3] E8 21 04 04 14 08 00 04 06 04 64 00 02 08 02 01 03 00 06 01 00 01 00 00 01 00 00 00 00 00 00 00 00 00 00 00
[4] F0 21 05 00 FF 00 00 04 06 04 64 00 02 01 12 00 04 00 07 03 00 01 00 00 21 00 00 00 00 00 00 00 00 00 00 00
[5] FC 21 04 00 FF 00 00 05 06 04 64 00 02 08 16 00 05 00 06 01 00 01 00 00 01 00 00 00 00 00 00 00 00 00 00 00
[6] 01 22 06 03 64 00 00 04 06 04 64 00 02 0C 06 00 06 00 08 01 00 01 00 00 41 00 00 00 00 00 00 00 00 00 00 00
[7] 0D 22 03 06 23 00 00 05 07 04 64 00 02 08 1E 00 07 00 05 01 00 01 00 00 01 00 00 00 00 00 10 00 00 00 00 00
[8] 14 22 05 04 14 08 00 03 06 04 64 00 02 08 1F 01 08 00 07 01 00 01 00 00 60 00 00 00 00 00 00 00 00 00 00 00
[9] 1D 22 05 00 FF 00 00 05 06 04 64 00 02 08 20 00 09 00 07 01 00 01 00 00 01 00 00 00 00 00 00 00 00 00 00 00
[10] 23 22 01 08 26 00 00 05 06 04 96 00 03 08 0C 03 0A 00 05 03 00 01 00 00 01 00 00 00 00 00 00 40 00 00 00 00
[11] 29 22 06 00 FF 00 00 05 06 04 96 00 03 08 14 00 0B 00 06 01 00 01 00 00 01 00 00 00 00 00 00 00 00 00 00 00
[12] 32 22 04 04 1F 00 00 04 06 04 96 00 03 08 1A 02 0C 00 07 01 00 01 00 00 01 00 00 00 00 00 00 00 00 00 00 00
[13] 3F 22 01 08 1F 00 00 05 0A 04 96 00 03 08 0F 03 0D 00 05 01 00 01 00 00 01 00 04 00 00 00 00 00 00 00 00 00
[14] 48 22 03 00 FF 00 00 05 06 04 96 00 03 08 21 00 0E 00 06 01 00 01 00 05 01 00 00 00 00 00 00 00 04 00 00 00
240h 576d [15] 51 22 01 06 22 00 00 05 06 04 C8 00 04 08 13 03 0F 00 05 01 00 01 00 00 01 00 04 00 00 00 00 10 00 00 00 00
[16] 5B 22 06 05 14 08 00 05 06 04 C8 00 04 08 19 01 10 00 08 02 00 01 00 00 61 00 00 00 00 00 00 00 00 00 00 00
[17] 62 22 07 05 65 00 00 05 06 06 C8 00 04 03 01 00 11 00 08 01 00 01 00 00 08 00 00 02 00 00 00 00 00 00 00 00
[18] 6C 22 01 08 1F 00 00 05 06 04 C8 00 04 08 03 03 12 00 05 01 00 01 00 00 01 00 04 00 00 00 00 00 00 00 00 00
[19] 72 22 06 06 1F 00 00 06 06 04 C8 00 04 08 22 00 13 00 06 01 00 01 00 00 01 00 00 00 00 00 00 00 00 00 00 00
[20] 7D 22 06 00 FF 00 00 05 06 04 2C 01 06 08 1D 00
14 00 07 01 00 01 00 00 01 00 00 00 00 00 40 00
00 00 00 00 83 22 07 00 FF 00 00 05 06 04 2C 01
06 08 08 00 15 00 08 01 00 01 00 00 01 00 00 00
00 00 00 00 00 00 00 00 89 22 07 04 64 00 00 04
06 04 2C 01 06 08 0D 00 16 00 08 01 00 01 00 00
01 00 00 00 00 00 00 00 00 00 00 00 90 22 01 08
1E 00 00 05 0A 04 2C 01 06 08 15 03 17 00 05 01
00 01 00 00 01 00 04 00 00 00 00 00 00 00 00 00
98 22 05 05 1F 00 00 04 0A 04 2C 01 06 08 05 02
18 00 08 01 00 01 00 00 01 00 00 00 00 00 00 00
00 00 00 00 A0 22 01 05 20 00 00 04 06 04 F4 01
0A 08 18 03 19 00 05 01 00 01 00 00 01 00 04 00
00 00 00 00 40 00 00 00 AC 22 06 00 FF 00 00 05
06 04 F4 01 0A 08 07 00 1A 00 0A 01 00 01 00 00
01 00 00 00 00 00 00 00 00 00 00 00 B6 22 02 08
23 00 00 05 06 04 F4 01 0A 08 17 03 1B 00 05 01
00 01 00 00 01 00 00 00 00 00 10 40 00 00 00 00
C0 22 09 00 FF 00 00 05 06 04 F4 01 0A 08 0E 00
1C 00 08 01 00 01 00 00 01 00 20 00 00 00 10 00
03 00 00 00 C8 22 09 00 FF 00 00 05 06 04 F4 01
0A 08 10 00 1D 00 0A 01 00 01 00 00 01 00 20 00
00 00 00 00 03 00 00 00 D5 22 05 08 14 08 00 06
06 04 F4 01 0A 07 1B 01 1E 00 06 01 00 01 00 00
21 00 00 00 00 00 00 00 00 00 00 00 CE 22 08 00
FF 00 00 05 06 04 F4 01 0A 08 09 00 1F 00 09 01
00 01 00 00 01 00 00 00 00 00 00 00 00 00 00 00
E2 22 01 07 21 00 00 05 06 04 F4 01 0A 08 0B 03
20 00 05 01 00 01 00 00 01 00 04 00 00 00 00 00
08 00 00 00 EE 22 08 08 1E 00 00 05 09 04 F4 01
0A 08 04 02 21 00 08 01 00 01 00 00 01 00 00 00
00 00 00 00 01 00 00 00 FC 22 0C 00 FF 00 00 08
0C 04 58 02 00 13 00 00 27 00 0C 01 00 01 00 00
01 00 24 00 00 00 00 00 00 00 00 00 03 23 06 00
FF 00 00 04 04 04 3C 00 02 0E 0C 00 00 00 0A 01
02 01 00 00 02 00 00 80 00 00 00 00 00 00 00 00
0B 23 08 02 14 08 00 04 06 06 64 00 02 0E 0D 00
00 00 14 01 05 01 00 00 02 00 00 80 00 00 00 00
00 00 00 00 12 23 00 0A 0A 0A 00 02 04 02 64 00
02 0E 21 00 00 00 0A 01 00 01 00 00 00 00 00 80
00 00 08 01 00 00 00 00 1B 23 0A 0A 0A 63 00 05
07 08 A0 00 02 0E 0E 03 00 00 1E 01 03 01 00 00
02 00 00 80 00 00 00 01 00 00 00 00 23 23 01 01
64 00 00 02 05 02 0F 00 01 00 00 00 00 00 01 01
00 08 00 00 00 00 00 80 00 00 00 00 00 00 00 00
2C 23 03 01 64 00 00 02 05 02 1E 00 01 00 08 03
00 00 01 01 00 06 00 00 00 00 00 80 00 00 02 00
00 00 00 00 36 23 01 01 14 08 00 01 05 02 1E 00
01 00 0F 03 00 00 01 01 00 06 00 00 00 00 00 80
00 00 00 00 00 00 00 00 3D 23 04 01 64 00 00 02
05 04 3C 00 01 00 09 03 00 00 03 01 00 04 00 00
01 00 00 80 00 00 00 00 02 00 00 00 45 23 02 02
23 04 00 03 07 02 32 00 01 00 16 00 00 00 01 01
00 04 00 00 00 00 00 80 00 00 10 40 00 00 00 00
4C 23 00 00 FF 00 00 01 05 02 3C 00 02 00 00 00
00 00 0A 01 00 01 00 00 00 00 00 80 00 00 20 00
00 00 00 00 55 23 07 03 64 00 00 03 07 02 78 00
02 00 06 00 00 00 03 01 00 06 00 00 00 00 00 00
00 00 00 00 00 00 00 00 23 23 02 00 FF 00 00 02
05 02 14 00 01 01 00 00 00 00 02 01 00 08 00 00
00 00 00 80 00 00 00 00 00 00 00 00 2C 23 04 00
FF 00 00 02 05 02 28 00 01 01 03 08 00 00 02 01
00 06 00 00 00 00 00 80 00 00 02 00 00 00 00 00
60 23 05 00 FF 00 00 03 05 02 50 00 02 01 04 00
00 00 02 01 00 06 00 00 00 00 00 80 00 00 00 00
00 00 00 00 36 23 02 01 14 08 00 01 05 02 3C 00
02 01 0F 03 00 00 02 01 00 06 00 00 00 00 00 80
00 00 00 00 00 00 00 00 6C 23 04 04 23 04 00 04
08 02 96 00 03 01 18 00 00 00 02 01 00 04 00 00
00 00 00 80 01 00 10 40 00 00 00 00 74 23 02 05
1F 04 00 03 09 02 B4 00 04 01 15 00 00 00 02 01
00 04 00 00 00 00 04 80 02 00 00 00 00 00 00 00
7E 23 02 00 FF 00 00 01 05 02 3C 00 02 01 20 00
00 00 02 01 00 06 01 00 00 00 00 80 00 00 08 00
00 00 00 00 4C 23 01 00 FF 00 00 01 05 02 78 00
04 01 00 00 00 00 14 01 00 01 00 00 00 00 00 80
00 00 20 00 00 00 00 00 88 23 03 02 14 06 00 03
05 04 64 00 02 01 09 03 00 00 03 01 00 04 00 00
01 00 00 00 00 00 00 00 00 00 00 00 91 23 05 00
FF 00 00 03 06 04 A0 00 03 01 0A 03 00 00 07 02
00 02 00 06 08 00 00 02 00 00 00 00 04 00 00 00
9C 23 0C 00 FF 00 02 04 07 02 C8 00 04 01 06 00
00 00 0C 01 00 02 00 00 00 00 00 00 00 00 02 00
00 00 00 00 23 23 01 01 24 04 00 02 07 02 19 00
01 02 00 00 00 00 01 01 00 08 00 00 00 00 00 80
00 00 00 00 00 00 00 00 2C 23 03 01 24 04 00 02
07 02 32 00 01 02 03 08 00 00 01 01 00 06 00 00
00 00 00 80 00 00 02 00 00 00 00 00 60 23 04 01
24 04 00 03 07 02 64 00 02 02 04 00 00 00 01 01
00 06 00 00 00 00 00 80 00 00 00 00 00 00 00 00
3D 23 04 01 24 04 00 02 07 04 64 00 02 02 03 09
00 00 03 01 00 04 00 00 01 00 00 80 00 00 00 00
02 00 00 00 6C 23 03 06 23 04 00 04 0A 02 C8 00
02 02 18 00 00 00 01 01 00 04 00 00 00 00 00 80
01 00 10 40 00 00 00 00 4C 23 00 00 FF 00 00 01
07 02 96 00 03 02 00 00 00 00 0A 01 00 01 00 00
00 00 00 80 00 00 20 00 00 00 00 00 A6 23 04 00
FF 00 00 03 07 02 78 00 03 02 05 00 00 00 01 01
00 06 00 01 00 00 00 00 00 00 40 00 04 00 00 00
B2 23 01 07 1F 04 00 04 09 02 F0 00 04 02 15 00
00 00 01 01 00 04 00 00 00 00 04 00 04 00 00 00
00 00 00 00 BB 23 08 05 24 04 00 04 08 06 A0 00
04 02 0B 03 00 00 0A 02 00 02 00 00 08 00 00 02
00 00 00 00 00 00 00 00 23 23 01 01 65 00 00 03
06 04 19 00 01 03 00 00 00 00 01 01 00 08 00 00
08 00 00 82 00 00 00 00 00 00 00 00 2C 23 03 01
65 00 00 03 06 04 32 00 01 03 03 08 00 00 01 01
00 06 00 00 08 00 00 82 00 00 02 00 00 00 00 00
60 23 04 01 65 00 00 04 06 04 64 00 02 03 04 00
00 00 01 01 00 06 00 00 08 00 00 82 00 00 00 00
00 00 00 00 36 23 01 01 14 08 00 02 06 04 2D 00
02 03 0F 03 00 00 01 01 00 06 00 00 08 00 00 82
00 00 00 00 00 00 00 00 45 23 02 02 23 04 00 04
08 04 4B 00 02 03 16 00 00 00 01 01 00 04 00 00
08 00 00 82 00 00 10 40 00 00 00 00 74 23 01 05
1F 04 00 04 0A 04 B4 00 03 03 15 00 00 00 01 01
00 04 00 00 08 00 04 82 02 00 00 00 00 00 00 00
7E 23 01 01 65 00 00 02 06 04 64 00 02 03 0A 21
00 00 01 01 00 06 01 00 08 00 00 82 00 00 08 00
00 00 00 00 4C 23 00 00 FF 00 00 02 06 02 96 00
03 03 00 00 00 00 0A 01 00 01 00 00 08 00 00 82
00 00 20 00 00 00 00 00 C6 23 08 06 65 00 00 03
09 06 A0 00 03 03 09 03 00 00 0A 02 00 02 00 00
08 00 00 02 00 00 00 00 00 00 00 00 D2 23 05 0A
0A 0A 00 05 08 08 C8 00 04 03 0D 00 00 00 14 02
00 01 00 00 08 00 00 02 00 00 08 00 00 00 00 00
2C 23 03 00 FF 00 00 02 08 02 32 00 02 04 03 08
00 00 03 01 00 06 00 00 40 00 00 80 00 00 02 00
00 00 00 00 60 23 04 00 FF 00 00 03 08 02 64 00
02 04 04 00 00 00 03 01 00 06 00 00 40 00 00 80
00 00 00 00 00 00 00 00 7E 23 01 00 FF 00 00 01
08 02 28 00 02 04 20 00 00 00 03 01 00 06 02 00
40 00 00 80 00 00 08 00 00 00 00 00 DB 23 08 00
FF 00 00 04 09 02 A0 00 04 04 05 00 00 00 04 01
00 06 00 00 40 00 00 00 00 00 00 00 00 00 00 00
E7 23 00 0C 0A 0A 00 02 09 02 B4 00 04 04 22 03
00 00 0C 01 00 01 00 00 40 00 00 00 00 00 00 00
00 00 00 00 F4 23 0C 00 FF 00 00 08 0F 02 C8 00
04 04 06 00 00 00 14 01 00 01 00 00 00 00 C0 00
00 00 00 00 00 00 00 00 4C 23 00 00 FF 00 00 01
08 02 96 00 02 04 00 00 00 00 1E 01 00 01 00 00
00 00 00 80 00 00 20 00 00 00 00 00 23 23 03 00
FF 00 00 02 04 02 0A 00 01 05 00 00 00 00 01 01
00 08 00 00 00 00 00 80 00 00 00 00 00 00 00 00
2C 23 05 00 FF 00 00 02 04 02 14 00 01 05 03 08
00 00 01 01 00 06 00 00 00 00 00 80 00 00 02 00
00 00 00 00 60 23 06 00 FF 00 00 03 04 02 28 00
01 05 04 00 00 00 01 01 00 06 00 00 00 00 00 80
00 00 00 80 00 00 00 00 36 23 03 01 14 08 00 01
04 02 1E 00 01 05 0F 03 00 00 01 01 00 06 00 00
00 00 00 80 00 00 00 00 00 00 00 00 4C 23 02 00
FF 00 00 01 04 02 3C 00 02 05 00 00 00 00 0A 01
00 01 00 00 00 00 00 80 00 00 20 00 00 00 00 00
FA 23 07 00 FF 00 00 03 04 06 64 00 02 05 09 03
00 00 05 01 00 04 00 00 00 00 00 00 00 00 00 00
00 00 00 00 23 23 01 00 FF 00 00 02 06 02 0F 00
01 06 00 00 00 00 01 01 00 08 00 00 00 00 00 84
00 00 00 00 00 00 00 00 2C 23 02 00 FF 00 00 02
06 02 1E 00 01 06 03 08 00 00 01 01 00 08 00 00

dseg:019C D3 21 05 00 FF 00 00 04 0A 04 64 00 02 04 0A 00+_unit_type_table s_UNIT_TYPE <offset cnst_Dwarf,   5, 0, 0FFh, 0, 0,   4,  10,   4,   100,   2, R_Dwarf, _AnimistsGuild, 0, 0,  10,   1, 0,   1, 0, 0, M_Cavalry or M_Mntnr, 0, 0, 0, 0, 0>; 0
dseg:019C 00 00 0A 01 00 01 00 00 41 00 00 00 00 00 00 00+                                        ; DATA XREF: sub_4A90D+2DCr ...
dseg:019C 00 00 00 00 D9 21 05 03 64 00 00 03 06 04 64 00+s_UNIT_TYPE <offset cnst_Barbarian,   5,   3,  64h, 0, 0,   3,   6,   4,   100,   2, 0, _SagesGuild, 0,      1,   9,   1, 0,   1, 0, 0, M_Cavalry, 0, 0, 0, 0, 0>; 1 ; "Dwarf"
dseg:019C 02 00 11 00 01 00 09 01 00 01 00 00 01 00 00 00+s_UNIT_TYPE <offset cnst_Sage,   1,   6,  20h, 0, 0,   4,   6,   4,   100,   2, R_High_Man, _MerchantsGuild, _Barracks,      2,   5,   1, 0,   1, 0, 0, M_Cavalry, 0, 0, 0, 0, 0>; 2
dseg:019C 00 00 00 00 00 00 00 00 E3 21 01 06 20 00 00 04+s_UNIT_TYPE <offset cnst_Dervish,   4,   4,  14h,   8, 0,   4,   6,   4,   100,   2, R_High_Man, _Housing, _Trade_Goods,      3,   6,   1, 0,   1, 0, 0, M_Cavalry, 0, 0, 0, 0, 0>; 3
dseg:019C 06 04 64 00 02 08 1C 03 02 00 05 01 00 01 00 00+s_UNIT_TYPE <offset cnst_Beastmaster,   5, 0, 0FFh, 0, 0,   4,   6,   4,   100,   2, R_Beastman, _Oracle, 0,      4,   7,   3, 0,   1, 0, 0, M_Cavalry or M_Forester, 0, 0, 0, 0, 0>; 4
dseg:019C 01 00 00 00 00 00 00 00 00 00 00 00 E8 21 04 04+s_UNIT_TYPE <offset cnst_Bard,   4, 0, 0FFh, 0, 0,   5,   6,   4,   100,   2, R_High_Man, _Shrine, 0,      5,   6,   1, 0,   1, 0, 0, M_Cavalry, 0, 0, 0, 0, 0>; 5
dseg:019C 14 08 00 04 06 04 64 00 02 08 02 01 03 00 06 01+s_UNIT_TYPE <offset cnst_OrcWarrior,   6,   3,  64h, 0, 0,   4,   6,   4,   100,   2, R_Orc, _ArmorersGuild, 0,      6,   8,   1, 0,   1, 0, 0, M_Cavalry or M_Mntnr, 0, 0, 0, 0, 0>; 6
dseg:019C 00 01 00 00 01 00 00 00 00 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Healer,   3,   6,  23h, 0, 0,   5,   7,   4,   100,   2, R_High_Man, _FarmersMarket, 0,      7,   5,   1, 0,   1, 0, 0, M_Cavalry, 0, 0, Ab_Healer, 0, 0>; 7
dseg:019C F0 21 05 00 FF 00 00 04 06 04 64 00 02 01 12 00+s_UNIT_TYPE <offset cnst_Huntress,   5,   4,  14h,   8, 0,   3,   6,   4,   100,   2, R_High_Man, _ForestersGuild, _Trade_Goods,      8,   7,   1, 0,   1, 0, 0, M_Forester or M_Mntnr, 0, 0, 0, 0, 0>; 8
dseg:019C 04 00 07 03 00 01 00 00 21 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Thief,   5, 0, 0FFh, 0, 0,   5,   6,   4,   100,   2, R_High_Man, _BuildersHall, 0,      9,   7,   1, 0,   1, 0, 0, M_Cavalry, 0, 0, 0, 0, 0>; 9
dseg:019C 00 00 00 00 FC 21 04 00 FF 00 00 05 06 04 64 00+s_UNIT_TYPE <offset cnst_Druid,   1,   8,  26h, 0, 0,   5,   6,   4,   150,   3, R_High_Man, _Shipwright, _Barracks,    0Ah,   5,   3, 0,   1, 0, 0, M_Cavalry, 0, 0, Ab_Purify, 0, 0>; 10
dseg:019C 02 08 16 00 05 00 06 01 00 01 00 00 01 00 00 00+s_UNIT_TYPE <offset cnst_WarMonk,   6, 0, 0FFh, 0, 0,   5,   6,   4,   150,   3, R_High_Man, _University, 0,    0Bh,   6,   1, 0,   1, 0, 0, M_Cavalry, 0, 0, 0, 0, 0>; 11
dseg:019C 00 00 00 00 00 00 00 00 01 22 06 03 64 00 00 04+s_UNIT_TYPE <offset cnst_WarriorMage,   4,   4,  1Fh, 0, 0,   4,   6,   4,   150,   3, R_High_Man, _Marketplace, _Housing,    0Ch,   7,   1, 0,   1, 0, 0, M_Cavalry, 0, 0, 0, 0, 0>; 12
dseg:019C 06 04 64 00 02 0C 06 00 06 00 08 01 00 01 00 00+s_UNIT_TYPE <offset cnst_Magician,   1,   8,  1Fh, 0, 0,   5,  10,   4,   150,   3, R_High_Man, _Sawmill, _Barracks,    0Dh,   5,   1, 0,   1, 0, 0, M_Cavalry, Imm_Missiles, 0, 0, 0, 0>; 13
dseg:019C 41 00 00 00 00 00 00 00 00 00 00 00 0D 22 03 06+s_UNIT_TYPE <offset cnst_Assassin,   3, 0, 0FFh, 0, 0,   5,   6,   4,   150,   3, R_High_Man, _MechaniciansGuild, 0,    0Eh,   6,   1, 0,   1, 0,   5, M_Cavalry, 0, 0, 0, Att_Poison, 0>; 14
dseg:019C 23 00 00 05 07 04 64 00 02 08 1E 00 07 00 05 01+s_UNIT_TYPE <offset cnst_WindMage,   1,   6,  22h, 0, 0,   5,   6,   4,   200,   4, R_High_Man, _AlchemistsGuild, _Barracks,    0Fh,   5,   1, 0,   1, 0, 0, M_Cavalry, Imm_Missiles, 0, Ab_WindWalking, 0, 0>; 15
dseg:019C 00 01 00 00 01 00 00 00 00 00 10 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Ranger,   6,   5,  14h,   8, 0,   5,   6,   4,   200,   4, R_High_Man, _Cathedral, _Trade_Goods,    10h,   8,   2, 0,   1, 0, 0, M_Cavalry or M_Forester or M_Mntnr, 0, 0, 0, 0, 0>; 16
dseg:019C 14 22 05 04 14 08 00 03 06 04 64 00 02 08 1F 01+s_UNIT_TYPE <offset cnst_Draconian,   7,   5,  65h, 0, 0,   5,   6,   6,   200,   4, R_Draconian, _Trade_Goods, 0,    11h,   8,   1, 0,   1, 0, 0, M_Flying, Ab_Flying, 0, 0, 0, 0>; 17
dseg:019C 08 00 07 01 00 01 00 00 60 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Witch,   1,   8,  1Fh, 0, 0,   5,   6,   4,   200,   4, R_High_Man, _Barracks, _Barracks,    12h,   5,   1, 0,   1, 0, 0, M_Cavalry, Imm_Missiles, 0, 0, 0, 0>; 18
dseg:019C 00 00 00 00 1D 22 05 00 FF 00 00 05 06 04 64 00+s_UNIT_TYPE <offset cnst_GoldenOne,   6,   6,  1Fh, 0, 0,   6,   6,   4,   200,   4, R_High_Man, _MinersGuild, 0,    13h,   6,   1, 0,   1, 0, 0, M_Cavalry, 0, 0, 0, 0, 0>; 19
dseg:019C 02 08 20 00 09 00 07 01 00 01 00 00 01 00 00 00+s_UNIT_TYPE <offset cnst_Ninja,   6, 0, 0FFh, 0, 0,   5,   6,   4,   300,   6, R_High_Man, _Granary, 0,    14h,   7,   1, 0,   1, 0, 0, M_Cavalry, 0, 0, Ab_Invisibility, 0, 0>; 20
dseg:019C 00 00 00 00 00 00 00 00 23 22 01 08 26 00 00 05+s_UNIT_TYPE <offset cnst_Rogue,   7, 0, 0FFh, 0, 0,   5,   6,   4,   300,   6, R_High_Man, _Smithy, 0,    15h,   8,   1, 0,   1, 0, 0, M_Cavalry, 0, 0, 0, 0, 0>; 21
dseg:019C 06 04 96 00 03 08 0C 03 0A 00 05 03 00 01 00 00+s_UNIT_TYPE <offset cnst_Amazon,   7,   4,  64h, 0, 0,   4,   6,   4,   300,   6, R_High_Man, _ShipYard, 0,    16h,   8,   1, 0,   1, 0, 0, M_Cavalry, 0, 0, 0, 0, 0>; 22
dseg:019C 01 00 00 00 00 00 00 40 00 00 00 00 29 22 06 00+s_UNIT_TYPE <offset cnst_Warlock,   1,   8,  1Eh, 0, 0,   5,  10,   4,   300,   6, R_High_Man, _WizardsGuild, _Barracks,    17h,   5,   1, 0,   1, 0, 0, M_Cavalry, Imm_Missiles, 0, 0, 0, 0>; 23
dseg:019C FF 00 00 05 06 04 96 00 03 08 14 00 0B 00 06 01+s_UNIT_TYPE <offset cnst_Unknown,   5,   5,  1Fh, 0, 0,   4,  10,   4,   300,   6, R_High_Man, _FightersGuild, _Housing,    18h,   8,   1, 0,   1, 0, 0, M_Cavalry, 0, 0, 0, 0, 0>; 24
dseg:019C 00 01 00 00 01 00 00 00 00 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Illusionist,   1,   5,  20h, 0, 0,   4,   6,   4,   500,  10, R_High_Man, _Parthenon, _Barracks,    19h,   5,   1, 0,   1, 0, 0, M_Cavalry, Imm_Missiles, 0, 0, Att_Illusion, 0>; 25
dseg:019C 32 22 04 04 1F 00 00 04 06 04 96 00 03 08 1A 02+s_UNIT_TYPE <offset cnst_Swordsman,   6, 0, 0FFh, 0, 0,   5,   6,   4,   500,  10, R_High_Man, _WarCollege, 0,    1Ah,  10,   1, 0,   1, 0, 0, M_Cavalry, 0, 0, 0, 0, 0>; 26
dseg:019C 0C 00 07 01 00 01 00 00 01 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Priestess,   2,   8,  23h, 0, 0,   5,   6,   4,   500,  10, R_High_Man, _Temple, _Barracks,    1Bh,   5,   1, 0,   1, 0, 0, M_Cavalry, 0, 0, Ab_Healer or Ab_Purify, 0, 0>; 27
dseg:019C 00 00 00 00 3F 22 01 08 1F 00 00 05 0A 04 96 00+s_UNIT_TYPE <offset cnst_Paladin,   9, 0, 0FFh, 0, 0,   5,   6,   4,   500,  10, R_High_Man, _MaritimeGuild, 0,    1Ch,   8,   1, 0,   1, 0, 0, M_Cavalry, Imm_Magic, 0, Ab_Healer, Att_ArmorPrc or Att_1stStrike, 0>; 28
dseg:019C 03 08 0F 03 0D 00 05 01 00 01 00 00 01 00 04 00+s_UNIT_TYPE <offset cnst_BlackKnight,   9, 0, 0FFh, 0, 0,   5,   6,   4,   500,  10, R_High_Man, _Library, 0,    1Dh,  10,   1, 0,   1, 0, 0, M_Cavalry, Imm_Magic, 0, 0, Att_ArmorPrc or Att_1stStrike, 0>; 29
dseg:019C 00 00 00 00 00 00 00 00 48 22 03 00 FF 00 00 05+s_UNIT_TYPE <offset cnst_ElvenArcher,   5,   8,  14h,   8, 0,   6,   6,   4,   500,  10, R_High_Elf, _Bank, _Trade_Goods,    1Eh,   6,   1, 0,   1, 0, 0, M_Cavalry or M_Forester, 0, 0, 0, 0, 0>; 30
dseg:019C 06 04 96 00 03 08 21 00 0E 00 06 01 00 01 00 05+s_UNIT_TYPE <offset cnst_Knight,   8, 0, 0FFh, 0, 0,   5,   6,   4,   500,  10, R_High_Man, _Stable, 0,    1Fh,   9,   1, 0,   1, 0, 0, M_Cavalry, 0, 0, 0, 0, 0>; 31
dseg:019C 01 00 00 00 00 00 00 00 04 00 00 00 51 22 01 06+s_UNIT_TYPE <offset cnst_Necromancer,   1,   7,  21h, 0, 0,   5,   6,   4,   500,  10, R_High_Man, _FantasticStable, _Barracks,    20h,   5,   1, 0,   1, 0, 0, M_Cavalry, Imm_Missiles, 0, 0, Att_LifeSteal, 0>; 32
dseg:019C 22 00 00 05 06 04 C8 00 04 08 13 03 0F 00 05 01+s_UNIT_TYPE <offset cnst_ChaosWarrior,   8,   8,  1Eh, 0, 0,   5,   9,   4,   500,  10, R_High_Man, _Armory, _Housing,    21h,   8,   1, 0,   1, 0, 0, M_Cavalry, 0, 0, 0, Att_ArmorPrc, 0>; 33
dseg:019C 00 01 00 00 01 00 04 00 00 00 00 10 00 00 00 00+s_UNIT_TYPE <offset cnst_Chosen,  12, 0, 0FFh, 0, 0,   8,  12,   4,   600, 0, Race_Life, 0, 0,    27h,  12,   1, 0,   1, 0, 0, M_Cavalry, Imm_Missiles or Imm_Magic, 0, 0, 0, 0>; 34
dseg:019C 5B 22 06 05 14 08 00 05 06 04 C8 00 04 08 19 01+s_UNIT_TYPE <offset cnst_Trireme,   6, 0, 0FFh, 0, 0,   4,   4,   4,    60,   2, Race_Generic, _Shipwright, 0, 0,  10,   1,   2,   1, 0, 0, M_Sailing, Std_Unit, 0, 0, 0, 0>; 35
dseg:019C 10 00 08 02 00 01 00 00 61 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Galley,   8,   2,  14h,   8, 0,   4,   6,   6,   100,   2, Race_Generic, _ShipYard, 0, 0,  20,   1,   5,   1, 0, 0, M_Sailing, Std_Unit, 0, 0, 0, 0>; 36
dseg:019C 00 00 00 00 62 22 07 05 65 00 00 05 06 06 C8 00+s_UNIT_TYPE <offset cnst_Catapult, 0,  10,  0Ah,  10, 0,   2,   4,   2,   100,   2, Race_Generic, _MechaniciansGuild, 0, 0,  10,   1, 0,   1, 0, 0, 0, Std_Unit, 0, Ab_WallCrshr or Ab_LongRange, 0, 0>; 37
dseg:019C 04 03 01 00 11 00 08 01 00 01 00 00 08 00 00 02+s_UNIT_TYPE <offset cnst_Warship,  10,  10,  0Ah,  99, 0,   5,   7,   8,   160,   2, Race_Generic, _MaritimeGuild, _Barracks, 0,  30,   1,   3,   1, 0, 0, M_Sailing, Std_Unit, 0, Ab_LongRange, 0, 0>; 38
dseg:019C 00 00 00 00 00 00 00 00 6C 22 01 08 1F 00 00 05+s_UNIT_TYPE <offset cnst_Spearmen,   1,   1,  64h, 0, 0,   2,   5,   2,    15,   1, 0, 0, 0, 0,   1,   1, 0,   8, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 39
dseg:019C 06 04 C8 00 04 08 03 03 12 00 05 01 00 01 00 00+s_UNIT_TYPE <offset cnst_Swordsmen,   3,   1,  64h, 0, 0,   2,   5,   2,    30,   1, 0, _Smithy, _Barracks, 0,   1,   1, 0,   6, 0, 0, 0, Std_Unit, 0, Ab_LargeShield, 0, 0>; 40
dseg:019C 01 00 04 00 00 00 00 00 00 00 00 00 72 22 06 06+s_UNIT_TYPE <offset cnst_Bowmen,   1,   1,  14h,   8, 0,   1,   5,   2,    30,   1, 0, _Sawmill, _Barracks, 0,   1,   1, 0,   6, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 41
dseg:019C 1F 00 00 06 06 04 C8 00 04 08 22 00 13 00 06 01+s_UNIT_TYPE <offset cnst_Cavalry,   4,   1,  64h, 0, 0,   2,   5,   4,    60,   1, 0, _Stable, _Barracks, 0,   3,   1, 0,   4, 0, 0, M_Cavalry, Std_Unit, 0, 0, Att_1stStrike, 0>; 42
dseg:019C 00 01 00 00 01 00 00 00 00 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Shaman,   2,   2,  23h,   4, 0,   3,   7,   2,    50,   1, 0, _Shrine, 0, 0,   1,   1, 0,   4, 0, 0, 0, Std_Unit, 0, Ab_Healer or Ab_Purify, 0, 0>; 43
dseg:019C 7D 22 06 00 FF 00 00 05 06 04 2C 01 06 08 1D 00+s_UNIT_TYPE <offset cnst_Settlers, 0, 0, 0FFh, 0, 0,   1,   5,   2,    60,   2, 0, 0, 0, 0,  10,   1, 0,   1, 0, 0, 0, Std_Unit, 0, Ab_CrOutpost, 0, 0>; 44
dseg:019C 14 00 07 01 00 01 00 00 01 00 00 00 00 00 40 00+s_UNIT_TYPE <offset cnst_Berserkers,   7,   3,  64h, 0, 0,   3,   7,   2,   120,   2, 0, _ArmorersGuild, 0, 0,   3,   1, 0,   6, 0, 0, 0, 0, 0, 0, 0, 0>; 45
dseg:019C 00 00 00 00 83 22 07 00 FF 00 00 05 06 04 2C 01+s_UNIT_TYPE <offset cnst_Spearmen,   2, 0, 0FFh, 0, 0,   2,   5,   2,    20,   1, R_Beastman, 0, 0, 0,   2,   1, 0,   8, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 46
dseg:019C 06 08 08 00 15 00 08 01 00 01 00 00 01 00 00 00+s_UNIT_TYPE <offset cnst_Swordsmen,   4, 0, 0FFh, 0, 0,   2,   5,   2,    40,   1, R_Beastman, _Barracks, _Smithy, 0,   2,   1, 0,   6, 0, 0, 0, Std_Unit, 0, Ab_LargeShield, 0, 0>; 47
dseg:019C 00 00 00 00 00 00 00 00 89 22 07 04 64 00 00 04+s_UNIT_TYPE <offset cnst_Halberdiers,   5, 0, 0FFh, 0, 0,   3,   5,   2,    80,   2, R_Beastman, _Armory, 0, 0,   2,   1, 0,   6, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 48
dseg:019C 06 04 2C 01 06 08 0D 00 16 00 08 01 00 01 00 00+s_UNIT_TYPE <offset cnst_Bowmen,   2,   1,  14h,   8, 0,   1,   5,   2,    60,   2, R_Beastman, _Sawmill, _Barracks, 0,   2,   1, 0,   6, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 49
dseg:019C 01 00 00 00 00 00 00 00 00 00 00 00 90 22 01 08+s_UNIT_TYPE <offset cnst_Priests,   4,   4,  23h,   4, 0,   4,   8,   2,   150,   3, R_Beastman, _Parthenon, 0, 0,   2,   1, 0,   4, 0, 0, 0, Std_Unit, Spl_Healing, Ab_Healer or Ab_Purify, 0, 0>; 50
dseg:019C 1E 00 00 05 0A 04 2C 01 06 08 15 03 17 00 05 01+s_UNIT_TYPE <offset cnst_Magicians,   2,   5,  1Fh,   4, 0,   3,   9,   2,   180,   4, R_Beastman, _WizardsGuild, 0, 0,   2,   1, 0,   4, 0, 0, 0, Imm_Missiles or Std_Unit, Spl_Fireball, 0, 0, 0>; 51
dseg:019C 00 01 00 00 01 00 04 00 00 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Engineers,   2, 0, 0FFh, 0, 0,   1,   5,   2,    60,   2, R_Beastman, _BuildersHall, 0, 0,   2,   1, 0,   6,   1, 0, 0, Std_Unit, 0, Ab_WallCrshr, 0, 0>; 52
dseg:019C 98 22 05 05 1F 00 00 04 0A 04 2C 01 06 08 05 02+s_UNIT_TYPE <offset cnst_Settlers,   1, 0, 0FFh, 0, 0,   1,   5,   2,   120,   4, R_Beastman, 0, 0, 0,  20,   1, 0,   1, 0, 0, 0, Std_Unit, 0, Ab_CrOutpost, 0, 0>; 53
dseg:019C 18 00 08 01 00 01 00 00 01 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Centaurs,   3,   2,  14h,   6, 0,   3,   5,   4,   100,   2, R_Beastman, _Stable, _Barracks, 0,   3,   1, 0,   4, 0, 0, M_Cavalry, 0, 0, 0, 0, 0>; 54
dseg:019C 00 00 00 00 A0 22 01 05 20 00 00 04 06 04 F4 01+s_UNIT_TYPE <offset cnst_Manticores,   5, 0, 0FFh, 0, 0,   3,   6,   4,   160,   3, R_Beastman, _AnimistsGuild, _Barracks, 0,   7,   2, 0,   2, 0,   6, M_Flying, Ab_Flying, 0, 0, Att_Poison, 0>; 55
dseg:019C 0A 08 18 03 19 00 05 01 00 01 00 00 01 00 04 00+s_UNIT_TYPE <offset cnst_Minotaurs,  12, 0, 0FFh, 0,   2,   4,   7,   2,   200,   4, R_Beastman, _ArmorersGuild, 0, 0,  12,   1, 0,   2, 0, 0, 0, 0, 0, Ab_LargeShield, 0, 0>; 56
dseg:019C 00 00 00 00 40 00 00 00 AC 22 06 00 FF 00 00 05+s_UNIT_TYPE <offset cnst_Spearmen,   1,   1,  24h,   4, 0,   2,   7,   2,    25,   1, R_Dark_Elf, 0, 0, 0,   1,   1, 0,   8, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 57
dseg:019C 06 04 F4 01 0A 08 07 00 1A 00 0A 01 00 01 00 00+s_UNIT_TYPE <offset cnst_Swordsmen,   3,   1,  24h,   4, 0,   2,   7,   2,    50,   1, R_Dark_Elf, _Barracks, _Smithy, 0,   1,   1, 0,   6, 0, 0, 0, Std_Unit, 0, Ab_LargeShield, 0, 0>; 58
dseg:019C 01 00 00 00 00 00 00 00 00 00 00 00 B6 22 02 08+s_UNIT_TYPE <offset cnst_Halberdiers,   4,   1,  24h,   4, 0,   3,   7,   2,   100,   2, R_Dark_Elf, _Armory, 0, 0,   1,   1, 0,   6, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 59
dseg:019C 23 00 00 05 06 04 F4 01 0A 08 17 03 1B 00 05 01+s_UNIT_TYPE <offset cnst_Cavalry,   4,   1,  24h,   4, 0,   2,   7,   4,   100,   2, R_Dark_Elf, _Barracks, _Stable, 0,   3,   1, 0,   4, 0, 0, M_Cavalry, Std_Unit, 0, 0, Att_1stStrike, 0>; 60
dseg:019C 00 01 00 00 01 00 00 00 00 00 10 40 00 00 00 00+s_UNIT_TYPE <offset cnst_Priests,   3,   6,  23h,   4, 0,   4,  10,   2,   200,   2, R_Dark_Elf, _Parthenon, 0, 0,   1,   1, 0,   4, 0, 0, 0, Std_Unit, Spl_Healing, Ab_Healer or Ab_Purify, 0, 0>; 61
dseg:019C C0 22 09 00 FF 00 00 05 06 04 F4 01 0A 08 0E 00+s_UNIT_TYPE <offset cnst_Settlers, 0, 0, 0FFh, 0, 0,   1,   7,   2,   150,   3, R_Dark_Elf, 0, 0, 0,  10,   1, 0,   1, 0, 0, 0, Std_Unit, 0, Ab_CrOutpost, 0, 0>; 62
dseg:019C 1C 00 08 01 00 01 00 00 01 00 20 00 00 00 10 00+s_UNIT_TYPE <offset cnst_Nightblades,   4, 0, 0FFh, 0, 0,   3,   7,   2,   120,   3, R_Dark_Elf, _FightersGuild, 0, 0,   1,   1, 0,   6, 0,   1, 0, 0, 0, Ab_Invisibility, Att_Poison, 0>; 63
dseg:019C 03 00 00 00 C8 22 09 00 FF 00 00 05 06 04 F4 01+s_UNIT_TYPE <offset cnst_Warlocks,   1,   7,  1Fh,   4, 0,   4,   9,   2,   240,   4, R_Dark_Elf, _WizardsGuild, 0, 0,   1,   1, 0,   4, 0, 0, 0, Imm_Missiles, Spl_DoomBolt, 0, 0, 0>; 64
dseg:019C 0A 08 10 00 1D 00 0A 01 00 01 00 00 01 00 20 00+s_UNIT_TYPE <offset cnst_Nightmares,   8,   5,  24h,   4, 0,   4,   8,   6,   160,   4, R_Dark_Elf, _FantasticStable, _Barracks, 0,  10,   2, 0,   2, 0, 0, M_Flying, Ab_Flying, 0, 0, 0, 0>; 65
dseg:019C 00 00 00 00 03 00 00 00 D5 22 05 08 14 08 00 06+s_UNIT_TYPE <offset cnst_Spearmen,   1,   1,  65h, 0, 0,   3,   6,   4,    25,   1, R_Draconian, 0, 0, 0,   1,   1, 0,   8, 0, 0, M_Flying, Ab_Flying or Std_Unit, 0, 0, 0, 0>; 66
dseg:019C 06 04 F4 01 0A 07 1B 01 1E 00 06 01 00 01 00 00+s_UNIT_TYPE <offset cnst_Swordsmen,   3,   1,  65h, 0, 0,   3,   6,   4,    50,   1, R_Draconian, _Barracks, _Smithy, 0,   1,   1, 0,   6, 0, 0, M_Flying, Ab_Flying or Std_Unit, 0, Ab_LargeShield, 0, 0>; 67
dseg:019C 21 00 00 00 00 00 00 00 00 00 00 00 CE 22 08 00+s_UNIT_TYPE <offset cnst_Halberdiers,   4,   1,  65h, 0, 0,   4,   6,   4,   100,   2, R_Draconian, _Armory, 0, 0,   1,   1, 0,   6, 0, 0, M_Flying, Ab_Flying or Std_Unit, 0, 0, 0, 0>; 68
dseg:019C FF 00 00 05 06 04 F4 01 0A 08 09 00 1F 00 09 01+s_UNIT_TYPE <offset cnst_Bowmen,   1,   1,  14h,   8, 0,   2,   6,   4,    45,   2, R_Draconian, _Sawmill, _Barracks, 0,   1,   1, 0,   6, 0, 0, M_Flying, Ab_Flying or Std_Unit, 0, 0, 0, 0>; 69
dseg:019C 00 01 00 00 01 00 00 00 00 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Shaman,   2,   2,  23h,   4, 0,   4,   8,   4,    75,   2, R_Draconian, _Shrine, 0, 0,   1,   1, 0,   4, 0, 0, M_Flying, Ab_Flying or Std_Unit, 0, Ab_Healer or Ab_Purify, 0, 0>; 70
dseg:019C E2 22 01 07 21 00 00 05 06 04 F4 01 0A 08 0B 03+s_UNIT_TYPE <offset cnst_Magicians,   1,   5,  1Fh,   4, 0,   4,  10,   4,   180,   3, R_Draconian, _WizardsGuild, 0, 0,   1,   1, 0,   4, 0, 0, M_Flying, Imm_Missiles or Ab_Flying or Std_Unit, Spl_Fireball, 0, 0, 0>; 71
dseg:019C 20 00 05 01 00 01 00 00 01 00 04 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Engineers,   1,   1,  65h, 0, 0,   2,   6,   4,   100,   2, R_Draconian, _AnimistsGuild, _MechaniciansGuild, 0,   1,   1, 0,   6,   1, 0, M_Flying, Ab_Flying or Std_Unit, 0, Ab_WallCrshr, 0, 0>; 72
dseg:019C 08 00 00 00 EE 22 08 08 1E 00 00 05 09 04 F4 01+s_UNIT_TYPE <offset cnst_Settlers, 0, 0, 0FFh, 0, 0,   2,   6,   2,   150,   3, R_Draconian, 0, 0, 0,  10,   1, 0,   1, 0, 0, M_Flying, Ab_Flying or Std_Unit, 0, Ab_CrOutpost, 0, 0>; 73
dseg:019C 0A 08 04 02 21 00 08 01 00 01 00 00 01 00 00 00+s_UNIT_TYPE <offset cnst_DoomDrakes,   8,   6,  65h, 0, 0,   3,   9,   6,   160,   3, R_Draconian, _Stable, _Barracks, 0,  10,   2, 0,   2, 0, 0, M_Flying, Ab_Flying, 0, 0, 0, 0>; 74
dseg:019C 00 00 00 00 01 00 00 00 FC 22 0C 00 FF 00 00 08+s_UNIT_TYPE <offset cnst_AirShip,   5,  10,  0Ah,  10, 0,   5,   8,   8,   200,   4, R_Draconian, _ShipYard, 0, 0,  20,   2, 0,   1, 0, 0, M_Flying, Ab_Flying, 0, Ab_WallCrshr, 0, 0>; 75
dseg:019C 0C 04 58 02 00 13 00 00 27 00 0C 01 00 01 00 00+s_UNIT_TYPE <offset cnst_Swordsmen,   3, 0, 0FFh, 0, 0,   2,   8,   2,    50,   2, R_Dwarf, _Barracks, _Smithy, 0,   3,   1, 0,   6, 0, 0, M_Mntnr, Std_Unit, 0, Ab_LargeShield, 0, 0>; 76
dseg:019C 01 00 24 00 00 00 00 00 00 00 00 00 03 23 06 00+s_UNIT_TYPE <offset cnst_Halberdiers,   4, 0, 0FFh, 0, 0,   3,   8,   2,   100,   2, R_Dwarf, _Armory, 0, 0,   3,   1, 0,   6, 0, 0, M_Mntnr, Std_Unit, 0, 0, 0, 0>; 77
dseg:019C FF 00 00 04 04 04 3C 00 02 0E 0C 00 00 00 0A 01+s_UNIT_TYPE <offset cnst_Engineers,   1, 0, 0FFh, 0, 0,   1,   8,   2,    40,   2, R_Dwarf, _BuildersHall, 0, 0,   3,   1, 0,   6,   2, 0, M_Mntnr, Std_Unit, 0, Ab_WallCrshr, 0, 0>; 78
dseg:019C 02 01 00 00 02 00 00 80 00 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Hammerhands,   8, 0, 0FFh, 0, 0,   4,   9,   2,   160,   4, R_Dwarf, _FightersGuild, 0, 0,   4,   1, 0,   6, 0, 0, M_Mntnr, 0, 0, 0, 0, 0>; 79
dseg:019C 0B 23 08 02 14 08 00 04 06 06 64 00 02 0E 0D 00+s_UNIT_TYPE <offset cnst_SteamCannon, 0,  12,  0Ah,  10, 0,   2,   9,   2,   180,   4, R_Dwarf, _MinersGuild, _Barracks, 0,  12,   1, 0,   1, 0, 0, M_Mntnr, 0, 0, 0, 0, 0>; 80
dseg:019C 00 00 14 01 05 01 00 00 02 00 00 80 00 00 00 00+s_UNIT_TYPE <offset cnst_Golem,  12, 0, 0FFh, 0, 0,   8,  15,   2,   200,   4, R_Dwarf, _ArmorersGuild, 0, 0,  20,   1, 0,   1, 0, 0, 0, Imm_Death or Imm_Poison, 0, 0, 0, 0>; 81
dseg:019C 00 00 00 00 12 23 00 0A 0A 0A 00 02 04 02 64 00+s_UNIT_TYPE <offset cnst_Settlers, 0, 0, 0FFh, 0, 0,   1,   8,   2,   150,   2, R_Dwarf, 0, 0, 0,  30,   1, 0,   1, 0, 0, 0, Std_Unit, 0, Ab_CrOutpost, 0, 0>; 82
dseg:019C 02 0E 21 00 00 00 0A 01 00 01 00 00 00 00 00 80+s_UNIT_TYPE <offset cnst_Spearmen,   3, 0, 0FFh, 0, 0,   2,   4,   2,    10,   1, R_Gnoll, 0, 0, 0,   1,   1, 0,   8, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 83
dseg:019C 00 00 08 01 00 00 00 00 1B 23 0A 0A 0A 63 00 05+s_UNIT_TYPE <offset cnst_Swordsmen,   5, 0, 0FFh, 0, 0,   2,   4,   2,    20,   1, R_Gnoll, _Barracks, _Smithy, 0,   1,   1, 0,   6, 0, 0, 0, Std_Unit, 0, Ab_LargeShield, 0, 0>; 84
dseg:019C 07 08 A0 00 02 0E 0E 03 00 00 1E 01 03 01 00 00+s_UNIT_TYPE <offset cnst_Halberdiers,   6, 0, 0FFh, 0, 0,   3,   4,   2,    40,   1, R_Gnoll, _Armory, 0, 0,   1,   1, 0,   6, 0, 0, 0, Std_Unit, 0, Ab_Negate1stSt, 0, 0>; 85
dseg:019C 02 00 00 80 00 00 00 01 00 00 00 00 23 23 01 01+s_UNIT_TYPE <offset cnst_Bowmen,   3,   1,  14h,   8, 0,   1,   4,   2,    30,   1, R_Gnoll, _Sawmill, _Barracks, 0,   1,   1, 0,   6, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 86
dseg:019C 64 00 00 02 05 02 0F 00 01 00 00 00 00 00 01 01+s_UNIT_TYPE <offset cnst_Settlers,   2, 0, 0FFh, 0, 0,   1,   4,   2,    60,   2, R_Gnoll, 0, 0, 0,  10,   1, 0,   1, 0, 0, 0, Std_Unit, 0, Ab_CrOutpost, 0, 0>; 87
dseg:019C 00 08 00 00 00 00 00 80 00 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_WolfRiders,   7, 0, 0FFh, 0, 0,   3,   4,   6,   100,   2, R_Gnoll, _Stable, _Barracks, 0,   5,   1, 0,   4, 0, 0, 0, 0, 0, 0, 0, 0>; 88
dseg:019C 2C 23 03 01 64 00 00 02 05 02 1E 00 01 00 08 03+s_UNIT_TYPE <offset cnst_Spearmen,   1, 0, 0FFh, 0, 0,   2,   6,   2,    15,   1, R_Halfling, 0, 0, 0,   1,   1, 0,   8, 0, 0, 0, Abl_Lucky or Std_Unit, 0, 0, 0, 0>; 89
dseg:019C 00 00 01 01 00 06 00 00 00 00 00 80 00 00 02 00+s_UNIT_TYPE <offset cnst_Swordsmen,   2, 0, 0FFh, 0, 0,   2,   6,   2,    30,   1, R_Halfling, _Barracks, _Smithy, 0,   1,   1, 0,   8, 0, 0, 0, Abl_Lucky or Std_Unit, 0, Ab_LargeShield, 0, 0>; 90
dseg:019C 00 00 00 00 36 23 01 01 14 08 00 01 05 02 1E 00+s_UNIT_TYPE <offset cnst_Bowmen,   1,   1,  14h,   8, 0,   1,   6,   2,    45,   1, R_Halfling, _Sawmill, _Barracks, 0,   1,   1, 0,   6, 0, 0, 0, Abl_Lucky or Std_Unit, 0, 0, 0, 0>; 91
dseg:019C 01 00 0F 03 00 00 01 01 00 06 00 00 00 00 00 80+s_UNIT_TYPE <offset cnst_Shaman,   1,   2,  23h,   4, 0,   3,   8,   2,    75,   1, R_Halfling, _Shrine, 0, 0,   1,   1, 0,   4, 0, 0, 0, Abl_Lucky or Std_Unit, 0, Ab_Healer or Ab_Purify, 0, 0>; 92
dseg:019C 00 00 00 00 00 00 00 00 3D 23 04 01 64 00 00 02+s_UNIT_TYPE <offset cnst_Settlers, 0, 0, 0FFh, 0, 0,   1,   6,   2,    90,   1, R_Halfling, 0, 0, 0,  10,   1, 0,   1, 0, 0, 0, Abl_Lucky or Std_Unit, 0, Ab_CrOutpost, 0, 0>; 93
dseg:019C 05 04 3C 00 01 00 09 03 00 00 03 01 00 04 00 00+s_UNIT_TYPE <offset cnst_Slingers,   1,   2,  15h,   6, 0,   2,   6,   2,   100,   1, R_Halfling, _Armory, 0, 0,   1,   1, 0,   8, 0, 0, 0, Abl_Lucky, 0, 0, 0, 0>; 94
dseg:019C 01 00 00 80 00 00 00 00 02 00 00 00 45 23 02 02+s_UNIT_TYPE <offset cnst_Spearmen,   1, 0, 0FFh, 0,   1,   2,   6,   2,    15,   1, R_High_Elf, 0, 0, 0,   1,   1, 0,   8, 0, 0, M_Forester, Std_Unit, 0, 0, 0, 0>; 95
dseg:019C 23 04 00 03 07 02 32 00 01 00 16 00 00 00 01 01+s_UNIT_TYPE <offset cnst_Swordsmen,   3, 0, 0FFh, 0,   1,   2,   6,   2,    30,   1, R_High_Elf, _Barracks, _Smithy, 0,   1,   1, 0,   6, 0, 0, M_Forester, Std_Unit, 0, Ab_LargeShield, 0, 0>; 96
dseg:019C 00 04 00 00 00 00 00 80 00 00 10 40 00 00 00 00+s_UNIT_TYPE <offset cnst_Halberdiers,   4, 0, 0FFh, 0,   1,   3,   6,   2,    60,   1, R_High_Elf, _Armory, 0, 0,   1,   1, 0,   6, 0, 0, M_Forester, Std_Unit, 0, 0, 0, 0>; 97
dseg:019C 4C 23 00 00 FF 00 00 01 05 02 3C 00 02 00 00 00+s_UNIT_TYPE <offset cnst_Cavalry,   4, 0, 0FFh, 0,   1,   2,   6,   4,    60,   1, R_High_Elf, _Barracks, _Stable,      1,   3,   1, 0,   4, 0, 0, M_Cavalry or M_Forester, Std_Unit, 0, 0, Att_1stStrike, 0>; 98
dseg:019C 00 00 0A 01 00 01 00 00 00 00 00 80 00 00 20 00+s_UNIT_TYPE <offset cnst_Magicians,   1,   5,  1Fh,   4,   1,   3,  10,   4,   180,   2, R_High_Elf, _WizardsGuild, 0, 0,   1,   1, 0,   4, 0, 0, M_Forester, Imm_Missiles or Std_Unit, Spl_Fireball, 0, 0, 0>; 99
dseg:019C 00 00 00 00 55 23 07 03 64 00 00 03 07 02 78 00+s_UNIT_TYPE <offset cnst_Settlers, 0, 0, 0FFh, 0,   1,   1,   6,   2,    90,   2, R_High_Elf, 0, 0, 0,  10,   1, 0,   1, 0, 0, M_Forester, Std_Unit, 0, Ab_CrOutpost, 0, 0>; 100
dseg:019C 02 00 06 00 00 00 03 01 00 06 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Longbowmen,   1,   3,  14h,   8,   1,   2,   6,   2,    80,   2, R_High_Elf, _Sawmill, _Barracks, 0,   1,   1, 0,   6, 0, 0, M_Forester, 0, 0, 0, 0, 0>; 101
dseg:019C 00 00 00 00 00 00 00 00 23 23 02 00 FF 00 00 02+s_UNIT_TYPE <offset cnst_ElvenLords,   5, 0, 0FFh, 0,   2,   4,   9,   4,   160,   3, R_High_Elf, _ArmorersGuild, 0, 0,   3,   1, 0,   4, 0, 0, M_Cavalry or M_Forester, 0, 0, 0, Att_ArmorPrc or Att_1stStrike, 0>; 102
dseg:019C 05 02 14 00 01 01 00 00 00 00 02 01 00 08 00 00+s_UNIT_TYPE <offset cnst_Pegasai,   5,   3,  14h,   8,   1,   4,   8,   6,   160,   3, R_High_Elf, _FantasticStable, _Barracks, 0,   5,   2, 0,   2, 0, 0, M_Flying, Ab_Flying, 0, 0, 0, 0>; 103
dseg:019C 00 00 00 80 00 00 00 00 00 00 00 00 2C 23 04 00+s_UNIT_TYPE <offset cnst_Spearmen,   1, 0, 0FFh, 0, 0,   2,   4,   2,    10,   1, R_High_Man, 0, 0, 0,   1,   1, 0,   8, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 104
dseg:019C FF 00 00 02 05 02 28 00 01 01 03 08 00 00 02 01+s_UNIT_TYPE <offset cnst_Swordsmen,   3, 0, 0FFh, 0, 0,   2,   4,   2,    20,   1, R_High_Man, _Barracks, _Smithy, 0,   1,   1, 0,   6, 0, 0, 0, Std_Unit, 0, Ab_LargeShield, 0, 0>; 105
dseg:019C 00 06 00 00 00 00 00 80 00 00 02 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Bowmen,   1,   1,  14h,   8, 0,   1,   4,   2,    30,   1, R_High_Man, _Sawmill, _Barracks, 0,   1,   1, 0,   6, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 106
dseg:019C 60 23 05 00 FF 00 00 03 05 02 50 00 02 01 04 00+s_UNIT_TYPE <offset cnst_Cavalry,   4, 0, 0FFh, 0, 0,   2,   4,   4,    40,   1, R_High_Man, _Barracks, _Stable, 0,   3,   1, 0,   4, 0, 0, M_Cavalry, Std_Unit, 0, 0, Att_1stStrike, 0>; 107
dseg:019C 00 00 02 01 00 06 00 00 00 00 00 80 00 00 00 00+s_UNIT_TYPE <offset cnst_Priests,   3,   4,  23h,   4, 0,   4,   7,   2,   100,   1, R_High_Man, _Parthenon, 0, 0,   1,   1, 0,   4, 0, 0, 0, Std_Unit, Spl_Healing, Ab_Healer or Ab_Purify, 0, 0>; 108
dseg:019C 00 00 00 00 36 23 02 01 14 08 00 01 05 02 3C 00+s_UNIT_TYPE <offset cnst_Magicians,   1,   5,  1Fh,   4, 0,   3,   8,   2,   120,   2, R_High_Man, _WizardsGuild, 0, 0,   1,   1, 0,   6, 0, 0, 0, Imm_Missiles or Std_Unit, Spl_Fireball, 0, 0, 0>; 109
dseg:019C 02 01 0F 03 00 00 02 01 00 06 00 00 00 00 00 80+s_UNIT_TYPE <offset cnst_Engineers,   1, 0, 0FFh, 0, 0,   1,   4,   2,    40,   1, R_High_Man, _BuildersHall, 0, 0,   1,   1, 0,   6,   1, 0, 0, Std_Unit, 0, Ab_WallCrshr, 0, 0>; 110
dseg:019C 00 00 00 00 00 00 00 00 6C 23 04 04 23 04 00 04+s_UNIT_TYPE <offset cnst_Settlers, 0, 0, 0FFh, 0, 0,   1,   4,   2,    60,   2, R_High_Man, 0, 0, 0,  10,   1, 0,   1, 0, 0, 0, Std_Unit, 0, Ab_CrOutpost, 0, 0>; 111
dseg:019C 08 02 96 00 03 01 18 00 00 00 02 01 00 04 00 00+s_UNIT_TYPE <offset cnst_Pikemen,   5, 0, 0FFh, 0, 0,   3,   5,   2,    80,   1, R_High_Man, _FightersGuild, 0, 0,   1,   1, 0,   8, 0, 0, 0, 0, 0, Ab_Negate1stSt, Att_ArmorPrc, 0>; 112
dseg:019C 00 00 00 80 01 00 10 40 00 00 00 00 74 23 02 05+s_UNIT_TYPE <offset cnst_Paladins,   6, 0, 0FFh, 0, 0,   5,   8,   4,   200,   3, R_High_Man, _Cathedral, _ArmorersGuild, 0,   4,   1, 0,   4, 0,   1, M_Cavalry, Imm_Magic, Ab_HolyBonus, 0, Att_ArmorPrc or Att_1stStrike, 0>; 113
dseg:019C 1F 04 00 03 09 02 B4 00 04 01 15 00 00 00 02 01+s_UNIT_TYPE <offset cnst_Spearmen,   1, 0, 0FFh, 0, 0,   4,   5,   2,    20,   1, R_Klackon, 0, 0, 0,   1,   1, 0,   8, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 114
dseg:019C 00 04 00 00 00 00 04 80 02 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Swordsmen,   3, 0, 0FFh, 0, 0,   4,   5,   2,    40,   1, R_Klackon, _Barracks, _Smithy, 0,   1,   1, 0,   6, 0, 0, 0, Std_Unit, 0, Ab_LargeShield, 0, 0>; 115
dseg:019C 7E 23 02 00 FF 00 00 01 05 02 3C 00 02 01 20 00+s_UNIT_TYPE <offset cnst_Halberdiers,   4, 0, 0FFh, 0, 0,   5,   5,   2,    80,   1, R_Klackon, _Armory, 0, 0,   1,   1, 0,   6, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 116
dseg:019C 00 00 02 01 00 06 01 00 00 00 00 80 00 00 08 00+s_UNIT_TYPE <offset cnst_Engineers,   1, 0, 0FFh, 0, 0,   1,   5,   2,    80,   1, R_Klackon, _BuildersHall, 0, 0,   1,   1, 0,   6,   1, 0, 0, Std_Unit, 0, Ab_WallCrshr, 0, 0>; 117
dseg:019C 00 00 00 00 4C 23 01 00 FF 00 00 01 05 02 78 00+s_UNIT_TYPE <offset cnst_Settlers, 0, 0, 0FFh, 0, 0,   1,   5,   2,   120,   2, R_Klackon, 0, 0, 0,  10,   1, 0,   1, 0, 0, 0, Std_Unit, 0, Ab_CrOutpost, 0, 0>; 118
dseg:019C 04 01 00 00 00 00 14 01 00 01 00 00 00 00 00 80+s_UNIT_TYPE <offset cnst_StagBeetle,  15,   5,  65h, 0, 0,   7,   6,   4,   160,   2, R_Klackon, _Stable, _ArmorersGuild, 0,  20,   1, 0,   1, 0, 0, 0, 0, 0, 0, 0, 0>; 119
dseg:019C 00 00 20 00 00 00 00 00 88 23 03 02 14 06 00 03+s_UNIT_TYPE <offset cnst_Spearmen,   1, 0, 0FFh, 0, 0,   3,   4,   2,    10,   1, R_Lizardman, 0, 0, 0,   2,   1, 0,   8, 0, 0, M_Swimming, Std_Unit, 0, 0, 0, 0>; 120
dseg:019C 05 04 64 00 02 01 09 03 00 00 03 01 00 04 00 00+s_UNIT_TYPE <offset cnst_Swordsmen,   3, 0, 0FFh, 0, 0,   3,   4,   2,    20,   1, R_Lizardman, _Barracks, _Smithy, 0,   2,   1, 0,   6, 0, 0, M_Swimming, Std_Unit, 0, Ab_LargeShield, 0, 0>; 121
dseg:019C 01 00 00 00 00 00 00 00 00 00 00 00 91 23 05 00+s_UNIT_TYPE <offset cnst_Halberdiers,   4, 0, 0FFh, 0, 0,   4,   4,   2,    40,   1, R_Lizardman, _Armory, 0, 0,   2,   1, 0,   6, 0, 0, M_Swimming, Std_Unit, 0, Ab_Negate1stSt, 0, 0>; 122
dseg:019C FF 00 00 03 06 04 A0 00 03 01 0A 03 00 00 07 02+s_UNIT_TYPE <offset cnst_Javelineers,   4,   3,  14h,   6, 0,   4,   5,   2,    80,   2, R_Lizardman, _FightersGuild, 0, 0,   2,   1, 0,   6, 0, 0, M_Swimming, 0, 0, 0, 0, 0>; 123
dseg:019C 00 02 00 06 08 00 00 02 00 00 00 00 04 00 00 00+s_UNIT_TYPE <offset cnst_Shaman,   2,   2,  23h,   4, 0,   3,   6,   2,    60,   2, R_Lizardman, _Shrine, 0, 0,   2,   1, 0,   4, 0, 0, M_Swimming, Std_Unit, 0, Ab_Healer or Ab_Purify, 0, 0>; 124
dseg:019C 9C 23 0C 00 FF 00 02 04 07 02 C8 00 04 01 06 00+s_UNIT_TYPE <offset cnst_Settlers, 0, 0, 0FFh, 0, 0,   2,   4,   2,   120,   2, R_Lizardman, 0, 0, 0,  20,   1, 0,   1, 0, 0, M_Swimming, Std_Unit, 0, Ab_CrOutpost, 0, 0>; 125
dseg:019C 00 00 0C 01 00 02 00 00 00 00 00 00 00 00 02 00+s_UNIT_TYPE <offset cnst_DragonTurtle,  10,   5,  65h, 0, 0,   8,   8,   4,   100,   1, R_Lizardman, _Stable, _ArmorersGuild, 0,  15,   1, 0,   1, 0, 0, M_Swimming, 0, 0, 0, 0, 0>; 126
dseg:019C 00 00 00 00 23 23 01 01 24 04 00 02 07 02 19 00+s_UNIT_TYPE <offset cnst_Spearmen,   1, 0, 0FFh, 0, 0,   2,   4,   2,    10,   1, R_Nomad, 0, 0, 0,   1,   1, 0,   8, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 127
dseg:019C 01 02 00 00 00 00 01 01 00 08 00 00 00 00 00 80+s_UNIT_TYPE <offset cnst_Swordsmen,   3, 0, 0FFh, 0, 0,   2,   4,   2,    20,   1, R_Nomad, _Barracks, _Smithy, 0,   1,   1, 0,   6, 0, 0, 0, Std_Unit, 0, Ab_LargeShield, 0, 0>; 128
dseg:019C 00 00 00 00 00 00 00 00 2C 23 03 01 24 04 00 02+s_UNIT_TYPE <offset cnst_Bowmen,   1,   1,  14h,   8, 0,   1,   4,   2,    30,   1, R_Nomad, _Sawmill, _Barracks, 0,   1,   1, 0,   6, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 129
dseg:019C 07 02 32 00 01 02 03 08 00 00 01 01 00 06 00 00+s_UNIT_TYPE <offset cnst_Priests,   3,   4,  23h,   4, 0,   4,   7,   2,   100,   1, R_Nomad, _Parthenon, 0, 0,   1,   1, 0,   4, 0, 0, 0, Std_Unit, Spl_Healing, Ab_Healer or Ab_Purify, 0, 0>; 130
dseg:019C 00 00 00 80 00 00 02 00 00 00 00 00 60 23 04 01+s_UNIT_TYPE <offset cnst_Magicians,   1,   5,  1Fh,   4, 0,   3,   8,   2,   120,   2, R_Nomad, _WizardsGuild, 0, 0,   1,   1, 0,   4, 0, 0, 0, Imm_Missiles or Std_Unit, Spl_Fireball, 0, 0, 0>; 131
dseg:019C 24 04 00 03 07 02 64 00 02 02 04 00 00 00 01 01+s_UNIT_TYPE <offset cnst_Settlers, 0, 0, 0FFh, 0, 0,   1,   4,   2,    60,   2, R_Nomad, 0, 0, 0,  10,   1, 0,   1, 0, 0, 0, Std_Unit, 0, Ab_CrOutpost, 0, 0>; 132
dseg:019C 00 06 00 00 00 00 00 80 00 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Horsebowmen,   4,   2,  14h,   8, 0,   2,   4,   4,    60,   1, R_Nomad, _Stable, _Barracks, 0,   3,   1, 0,   4, 0, 0, M_Cavalry, 0, 0, 0, 0, 0>; 133
dseg:019C 3D 23 04 01 24 04 00 02 07 04 64 00 02 02 03 09+s_UNIT_TYPE <offset cnst_Pikemen,   5, 0, 0FFh, 0, 0,   3,   5,   2,    80,   3, R_Nomad, _FightersGuild, 0, 0,   1,   1, 0,   8, 0, 0, 0, 0, 0, Ab_Negate1stSt, Att_ArmorPrc, 0>; 134
dseg:019C 00 00 03 01 00 04 00 00 01 00 00 80 00 00 00 00+s_UNIT_TYPE <offset cnst_Rangers,   4,   3,  14h,   8, 0,   4,   6,   4,   120,   2, R_Nomad, _AnimistsGuild, _Barracks, 0,   2,   1, 0,   4, 0, 0, M_Forester or M_Mntnr, 0, 0, 0, 0, 0>; 135
dseg:019C 02 00 00 00 6C 23 03 06 23 04 00 04 0A 02 C8 00+s_UNIT_TYPE <offset cnst_Griffins,   9, 0, 0FFh, 0, 0,   5,   7,   4,   200,   2, R_Nomad, _FantasticStable, _Barracks, 0,  10,   1, 0,   2, 0, 0, M_Flying, Ab_Flying, 0, 0, Att_ArmorPrc or Att_1stStrike, 0>; 136
dseg:019C 02 02 18 00 00 00 01 01 00 04 00 00 00 00 00 80+s_UNIT_TYPE <offset cnst_Spearmen,   1, 0, 0FFh, 0, 0,   2,   4,   2,    10,   1, R_Orc, 0, 0, 0,   1,   1, 0,   8, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 137
dseg:019C 01 00 10 40 00 00 00 00 4C 23 00 00 FF 00 00 01+s_UNIT_TYPE <offset cnst_Swordsmen,   3, 0, 0FFh, 0, 0,   2,   4,   2,    20,   1, R_Orc, _Barracks, _Smithy, 0,   1,   1, 0,   6, 0, 0, 0, Std_Unit, 0, Ab_LargeShield, 0, 0>; 138
dseg:019C 07 02 96 00 03 02 00 00 00 00 0A 01 00 01 00 00+s_UNIT_TYPE <offset cnst_Halberdiers,   4, 0, 0FFh, 0, 0,   3,   4,   2,    40,   1, R_Orc, _Armory, 0, 0,   1,   1, 0,   6, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 139
dseg:019C 00 00 00 80 00 00 20 00 00 00 00 00 A6 23 04 00+s_UNIT_TYPE <offset cnst_Bowmen,   1,   1,  14h,   8, 0,   1,   4,   2,    30,   1, R_Orc, _Sawmill, _Barracks, 0,   1,   1, 0,   6, 0, 0, 0, Std_Unit, 0, 0, 0, 0>; 140
dseg:019C FF 00 00 03 07 02 78 00 03 02 05 00 00 00 01 01+s_UNIT_TYPE <offset cnst_Cavalry,   4, 0, 0FFh, 0, 0,   2,   4,   4,    40,   1, R_Orc, _Barracks, _Stable,      1,   3,   1, 0,   4, 0, 0, M_Cavalry, Std_Unit, 0, 0, 0, 0>; 141
dseg:019C 00 06 00 01 00 00 00 00 00 00 40 00 04 00 00 00+s_UNIT_TYPE <offset cnst_Shaman,   2,   2,  23h,   4, 0,   3,   6,   2,    60,   1, R_Orc, _Shrine, 0, 0,   1,   1, 0,   4, 0, 0, 0, Std_Unit, 0, Ab_Healer or Ab_Purify, 0, 0>; 142
dseg:019C B2 23 01 07 1F 04 00 04 09 02 F0 00 04 02 15 00+s_UNIT_TYPE <offset cnst_Magicians,   1,   5,  1Fh,   4, 0,   3,   8,   2,   120,   2, R_Orc, _WizardsGuild, 0, 0,   1,   1, 0,   4, 0, 0, 0, Imm_Missiles or Std_Unit, Spl_Fireball, 0, 0, 0>; 143
dseg:019C 00 00 01 01 00 04 00 00 00 00 04 00 04 00 00 00+s_UNIT_TYPE <offset cnst_Engineers,   1, 0, 0FFh, 0, 0,   1,   4,   2,    40,   1, R_Orc, _BuildersHall, 0, 0,   1,   1, 0,   6,   1, 0, 0, Std_Unit, 0, Ab_WallCrshr, 0, 0>; 144
dseg:019C 00 00 00 00 BB 23 08 05 24 04 00 04 08 06 A0 00+s_UNIT_TYPE <offset cnst_Settlers, 0, 0, 0FFh, 0, 0,   1,   4,   2,    60,   2, R_Orc, 0, 0, 0,  10,   1, 0,   1, 0, 0, 0, Std_Unit, 0, Ab_CrOutpost, 0, 0>; 145
dseg:019C 04 02 0B 03 00 00 0A 02 00 02 00 00 08 00 00 02+s_UNIT_TYPE <offset cnst_WyvernRiders,   5, 0, 0FFh, 0, 0,   5,   7,   6,   200,   2, R_Orc, _FantasticStable, _Barracks, 0,  10,   1, 0,   2, 0,   6, M_Flying, Ab_Flying, 0, 0, Att_Poison, 0>; 146
dseg:019C 00 00 00 00 00 00 00 00 23 23 01 01 65 00 00 03+s_UNIT_TYPE <offset cnst_Spearmen,   3, 0, 0FFh, 0, 0,   2,   7,   2,    30,   2, R_Troll, 0, 0, 0,   4,   1, 0,   4, 0, 0, 0, Std_Unit, 0, Ab_Regeneration, 0, 0>; 147
dseg:019C 06 04 19 00 01 03 00 00 00 00 01 01 00 08 00 00+s_UNIT_TYPE <offset cnst_Swordsmen,   5, 0, 0FFh, 0, 0,   2,   7,   2,    60,   3, R_Troll, _Barracks, _Smithy, 0,   4,   1, 0,   4, 0, 0, 0, Std_Unit, 0, Ab_LargeShield or Ab_Regeneration, 0, 0>; 148
dseg:019C 08 00 00 82 00 00 00 00 00 00 00 00 2C 23 03 01+s_UNIT_TYPE <offset cnst_Halberdiers,   6, 0, 0FFh, 0, 0,   3,   7,   2,   120,   3, R_Troll, _Armory, 0, 0,   4,   1, 0,   4, 0, 0, 0, Std_Unit, 0, Ab_Regeneration or Ab_Negate1stSt, 0, 0>; 149
dseg:019C 65 00 00 03 06 04 32 00 01 03 03 08 00 00 01 01+s_UNIT_TYPE <offset cnst_Shaman,   4,   2,  23h,   4, 0,   3,   8,   2,   180,   2, R_Troll, _Shrine, 0, 0,   4,   1, 0,   4, 0, 0, 0, Std_Unit, 0, Ab_Healer or Ab_Regeneration or Ab_Purify, 0, 0>; 150
dseg:019C 00 06 00 00 08 00 00 82 00 00 02 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Settlers, 0, 0, 0FFh, 0, 0,   1,   7,   2,   180,   2, R_Troll, 0, 0, 0,  40,   1, 0,   1, 0, 0, 0, Std_Unit, 0, Ab_CrOutpost or Ab_Regeneration, 0, 0>; 151
dseg:019C 60 23 04 01 65 00 00 04 06 04 64 00 02 03 04 00+s_UNIT_TYPE <offset cnst_WarTrolls,   8, 0, 0FFh, 0, 0,   4,   8,   4,   160,   3, R_Troll, _FightersGuild, 0, 0,   5,   1, 0,   4, 0, 0, 0, 0, 0, Ab_Regeneration, 0, 0>; 152
dseg:019C 00 00 01 01 00 06 00 00 08 00 00 82 00 00 00 00+s_UNIT_TYPE <offset cnst_WarMammoths,  10, 0, 0FFh, 0, 0,   6,   9,   4,   240,   4, R_Troll, _Stable, _ArmorersGuild, 0,  12,   1, 0,   2, 0, 0, 0, 0, 0, Ab_WallCrshr, Att_1stStrike, 0>; 153
dseg:019C 00 00 00 00 36 23 01 01 14 08 00 02 06 04 2D 00+s_UNIT_TYPE <offset cnst_MagicSpirit,   5, 0, 0FFh, 0, 0,   4,   8,   2,    30,   1, Race_Arcane, _ArmorersGuild, 0, 0,  10,   1, 0,   1, 0, 0, 0, 0, 0, Ab_Summoned or Ab_Meld or Ab_NonCorporeal, 0, 0>; 154
dseg:019C 02 03 0F 03 00 00 01 01 00 06 00 00 08 00 00 82+s_UNIT_TYPE <offset cnst_HellHounds,   3,   3,  65h, 0,   1,   2,   6,   4,    40,   1, Race_Chaos, _ArmorersGuild, 0, 0,   4,   1, 0,   4, 0, 0, 0, 0, 0, Ab_Summoned, 0, 0>; 155
dseg:019C 00 00 00 00 00 00 00 00 45 23 02 02 23 04 00 04+s_UNIT_TYPE <offset cnst_Gargoyles,   4, 0, 0FFh, 0,   1,   8,   7,   4,   200,   5, Race_Chaos, _ArmorersGuild, 0, 0,   4,   1, 0,   4, 0, 0, M_Flying, Imm_Stoning or Imm_Poison, 0, Ab_Summoned, 0, 0>; 156
dseg:019C 08 04 4B 00 02 03 16 00 00 00 01 01 00 04 00 00+s_UNIT_TYPE <offset cnst_FireGiant,  10,  10,  0Ah,   2,   1,   5,   7,   4,   150,   3, Race_Chaos, _ArmorersGuild, 0, 0,  15,   1, 0,   1, 0, 0, M_Mntnr, Imm_Fire, 0, Ab_Summoned or Ab_WallCrshr, 0, 0>; 157
dseg:019C 08 00 00 82 00 00 10 40 00 00 00 00 74 23 01 05+s_UNIT_TYPE <offset cnst_FireElemental,  12, 0, 0FFh, 0, 0,   4,   6,   2,   100,   4, Race_Chaos, _ArmorersGuild, 0, 0,  10,   1, 0,   1, 0, 0, 0, Imm_Fire or Imm_Stoning or Imm_Poison, 0, Ab_Summoned, 0, 0>; 158
dseg:019C 1F 04 00 04 0A 04 B4 00 03 03 15 00 00 00 01 01+s_UNIT_TYPE <offset cnst_ChaosSpawn,   1,   4,  68h, 0, 0,   6,  10,   2,   400,  12, Race_Chaos, _ArmorersGuild, 0, 0,  15,   1, 0,   1, 0,   4, M_Flying, Ab_Flying, Ab_CauseFear, Ab_Summoned, Att_Poison, 0>; 159
dseg:019C 00 04 00 00 08 00 04 82 02 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Chimera,   7,   4,  65h, 0,   1,   5,   8,   4,   350,  10, Race_Chaos, _ArmorersGuild, 0, 0,   8,   1, 0,   4, 0, 0, M_Flying, Ab_Flying, 0, Ab_Summoned, 0, 0>; 160
dseg:019C 7E 23 01 01 65 00 00 02 06 04 64 00 02 03 0A 21+s_UNIT_TYPE <offset cnst_DoomBat,  10, 0, 0FFh, 0,   1,   5,   9,   8,   300,   8, Race_Chaos, _ArmorersGuild, 0, 0,  20,   1, 0,   1, 0, 0, M_Flying, Ab_Flying, Ab_Immolation, Ab_Summoned, 0, 0>; 161
dseg:019C 00 00 01 01 00 06 01 00 08 00 00 82 00 00 08 00+s_UNIT_TYPE <offset cnst_Efreet,   9,   9,  1Fh, 0,   2,   7,  10,   6,   550,  15, Race_Chaos, _ArmorersGuild, 0, 0,  12,   1, 0,   1, 0, 0, M_Flying, Imm_Fire or Ab_Flying or Ab_Caster_20, 0, Ab_Summoned, 0, 0>; 162
dseg:019C 00 00 00 00 4C 23 00 00 FF 00 00 02 06 02 96 00+s_UNIT_TYPE <offset cnst_Hydra,   6,   5,  65h, 0,   1,   4,  11,   2,   650,  14, Race_Chaos, _ArmorersGuild, 0, 0,  10,   1, 0,   9, 0, 0, 0, 0, 0, Ab_Summoned or Ab_Regeneration, 0, 0>; 163
dseg:019C 03 03 00 00 00 00 0A 01 00 01 00 00 08 00 00 82+s_UNIT_TYPE <offset cnst_GreatDrake,  30,  30,  65h, 0,   3,  10,  12,   4,   900,  30, Race_Chaos, _ArmorersGuild, 0, 0,  30,   1, 0,   1, 0, 0, M_Flying, Ab_Flying, 0, Ab_Summoned, 0, 0>; 164
dseg:019C 00 00 20 00 00 00 00 00 C6 23 08 06 65 00 00 03+s_UNIT_TYPE <offset cnst_Skeletons,   3, 0, 0FFh, 0,   1,   4,   5,   2,    25,   1, Race_Death, _ArmorersGuild, 0, 0,   1,   1, 0,   6, 0, 0, 0, Imm_Missiles or Imm_Illusion or Imm_Cold or Imm_Death or Imm_Poison, 0, Ab_Summoned, 0, 0>; 165
dseg:019C 09 06 A0 00 03 03 09 03 00 00 0A 02 00 02 00 00+s_UNIT_TYPE <offset cnst_Ghouls,   4, 0, 0FFh, 0,   1,   3,   6,   2,    80,   1, Race_Death, _ArmorersGuild, 0, 0,   3,   1, 0,   4, 0,   1, 0, Imm_Illusion or Imm_Cold or Imm_Death or Imm_Poison, 0, Ab_Summoned or Ab_CrUndead, Att_Poison, 0>; 166
dseg:019C 08 00 00 02 00 00 00 00 00 00 00 00 D2 23 05 0A+s_UNIT_TYPE <offset cnst_NightStalker,   7,   1,  69h, 0,   1,   3,   8,   4,   250,   1, Race_Death, _ArmorersGuild, 0, 0,  10,   1, 0,   1, 0, 254, 0, Imm_Illusion or Imm_Cold or Imm_Death or Imm_Poison, 0, Ab_Summoned or Ab_Invisibility, 0, 0>; 167
dseg:019C 0A 0A 00 05 08 08 C8 00 04 03 0D 00 00 00 14 02+s_UNIT_TYPE <offset cnst_Werewolves,   5, 0, 0FFh, 0,   1,   1,   6,   4,   250,   5, Race_Death, _ArmorersGuild, 0, 0,   5,   1, 0,   6, 0, 0, 0, Imm_Illusion or Imm_Cold or Imm_Death or Imm_Poison or Imm_Weapon, 0, Ab_Summoned or Ab_Regeneration, 0, 0>; 168
dseg:019C 00 01 00 00 08 00 00 02 00 00 08 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Demon,  14, 0, 0FFh, 0,   1,   5,   7,   4,    80,   3, Race_Death, _ArmorersGuild, 0, 0,  12,   1, 0,   1, 0, 0, M_Flying, Imm_Missiles or Imm_Illusion or Imm_Cold or Imm_Death or Imm_Poison or Imm_Weapon or Ab_Flying, 0, \
dseg:019C 2C 23 03 00 FF 00 00 02 08 02 32 00 02 04 03 08+             Ab_Summoned, 0, 0>         ; 169
dseg:019C 00 00 03 01 00 06 00 00 40 00 00 80 00 00 02 00+s_UNIT_TYPE <offset cnst_Wraiths,   7, 0, 0FFh, 0,   2,   6,   8,   4,   500,   5, Race_Death, _ArmorersGuild, 0, 0,   8,   1, 0,   4, 0, 253, M_Flying, Imm_Illusion or Imm_Cold or Imm_Death or Imm_Poison or Imm_Weapon or Ab_Flying, 0, \
dseg:019C 00 00 00 00 60 23 04 00 FF 00 00 03 08 02 64 00+             Ab_Summoned or Ab_NonCorporeal, Att_LifeSteal, 0>; 170
dseg:019C 02 04 04 00 00 00 03 01 00 06 00 00 40 00 00 80+s_UNIT_TYPE <offset cnst_ShadowDemons,   5,   4,  21h,   8,   2,   4,   8,   2,   325,   7, Race_Death, _ArmorersGuild, 0, 0,   5,   1, 0,   4, 0, 0, M_Flying, Imm_Illusion or Imm_Cold or Imm_Death or Imm_Poison or Imm_Weapon or Ab_Flying, 0, \
dseg:019C 00 00 00 00 00 00 00 00 7E 23 01 00 FF 00 00 01+             Ab_Summoned or Ab_PlaneShift or Ab_NonCorporeal or Ab_Regeneration, 0, 0>; 171
dseg:019C 08 02 28 00 02 04 20 00 00 00 03 01 00 06 02 00+s_UNIT_TYPE <offset cnst_DeathKnights,   9, 0, 0FFh, 0,   3,   8,  10,   6,   600,   8, Race_Death, _ArmorersGuild, 0, 0,   8,   1, 0,   4, 0, 252, M_Flying, Imm_Illusion or Imm_Cold or Imm_Death or Imm_Poison or Imm_Weapon or Ab_Flying, 0, Ab_Summoned, \
dseg:019C 40 00 00 80 00 00 08 00 00 00 00 00 DB 23 08 00+             Att_ArmorPrc or Att_1stStrike or Att_LifeSteal, 0>; 172
dseg:019C FF 00 00 04 09 02 A0 00 04 04 05 00 00 00 04 01+s_UNIT_TYPE <offset cnst_DemonLord,  20,  10,  21h,   8,   3,  10,  12,   4,   900,  15, Race_Death, _ArmorersGuild, 0, 0,  20,   1, 0,   1, 0, 251, M_Flying, \
dseg:019C 00 06 00 00 40 00 00 00 00 00 00 00 00 00 00 00+             Imm_Illusion or Imm_Cold or Imm_Death or Imm_Poison or Imm_Weapon or Ab_Flying or Sum_Demon_1 or Sum_Demon_2, Ab_CauseFear, Ab_Summoned, Att_LifeSteal, 0>; 173
dseg:019C E7 23 00 0C 0A 0A 00 02 09 02 B4 00 04 04 22 03+s_UNIT_TYPE <offset cnst_Zombies,   4, 0, 0FFh, 0,   1,   3,   3,   2,    30, 0, Race_Death, _ArmorersGuild, 0, 0,   3,   1, 0,   6, 0, 0, 0, Imm_Illusion or Imm_Cold or Imm_Death or Imm_Poison, 0, Ab_Summoned, 0, 0>; 174
dseg:019C 00 00 0C 01 00 01 00 00 40 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Unicorns,   5, 0, 0FFh, 0,   2,   3,   7,   4,   250,   5, Race_Life, _ArmorersGuild, 0, 0,   6,   1, 0,   4, 0,   2, M_Teleport, Imm_Poison, Ab_ResistAll, Ab_Summoned, 0, 0>; 175
dseg:019C 00 00 00 00 F4 23 0C 00 FF 00 00 08 0F 02 C8 00+s_UNIT_TYPE <offset cnst_GuardianSpirit,  10, 0, 0FFh, 0, 0,   4,  10,   2,    50,   1, Race_Life, _ArmorersGuild, 0, 0,  10,   1, 0,   1, 0,   1, 0, 0, Ab_ResistAll, Ab_Summoned or Ab_Meld or Ab_NonCorporeal, 0, 0>; 176
dseg:019C 04 04 06 00 00 00 14 01 00 01 00 00 00 00 C0 00+s_UNIT_TYPE <offset cnst_Angel,  13, 0, 0FFh, 0,   2,   7,   8,   6,   550,  15, Race_Life, _ArmorersGuild, 0, 0,  15,   1, 0,   1, 0,   1, M_Flying, Imm_Illusion or Ab_Flying, Ab_HolyBonus, Ab_Summoned, Att_DsplEvil, 0>; 177
dseg:019C 00 00 00 00 00 00 00 00 4C 23 00 00 FF 00 00 01+s_UNIT_TYPE <offset cnst_ArchAngel,  15, 0, 0FFh, 0,   3,  10,  12,   8,   950,  20, Race_Life, _ArmorersGuild, 0, 0,  18,   1, 0,   1, 0,   2, M_Flying, Imm_Illusion or Ab_Flying or Ab_Caster_40, Ab_HolyBonus, Ab_Summoned, 0, 0>; 178
dseg:019C 08 02 96 00 02 04 00 00 00 00 1E 01 00 01 00 00+s_UNIT_TYPE <offset cnst_WarBears,   7, 0, 0FFh, 0, 0,   3,   6,   4,    70,   2, Race_Nature, _ArmorersGuild, 0, 0,   8,   1, 0,   2, 0, 0, M_Forester, 0, 0, Ab_Summoned, 0, 0>; 179
dseg:019C 00 00 00 80 00 00 20 00 00 00 00 00 23 23 03 00+s_UNIT_TYPE <offset cnst_Sprites,   2,   3,  25h,   4,   1,   2,   8,   4,   100,   3, Race_Nature, _ArmorersGuild, 0, 0,   1,   1, 0,   4, 0, 0, M_Flying or M_Forester, Ab_Flying, 0, Ab_Summoned, 0, 0>; 180
dseg:019C FF 00 00 02 04 02 0A 00 01 05 00 00 00 00 01 01+s_UNIT_TYPE <offset cnst_Cockatrices,   4, 0, 0FFh, 0,   1,   3,   7,   4,   275,   8, Race_Nature, _ArmorersGuild, 0, 0,   3,   1, 0,   4, 0, 253, M_Flying, Ab_Flying, 0, Ab_Summoned, Att_StnTouch, 0>; 181
dseg:019C 00 08 00 00 00 00 00 80 00 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_Basilisk,  15,   1,  67h, 0,   1,   4,   7,   4,   325,   7, Race_Nature, _ArmorersGuild, 0, 0,  30,   1, 0,   1, 0, 255, 0, 0, 0, Ab_Summoned, 0, 0>; 182
dseg:019C 2C 23 05 00 FF 00 00 02 04 02 14 00 01 05 03 08+s_UNIT_TYPE <offset cnst_GiantSpiders,   4, 0, 0FFh, 0,   1,   3,   7,   4,   200,   4, Race_Nature, _ArmorersGuild, 0, 0,  10,   1, 0,   2, 0,   4, 0, 0, Spl_Web, Ab_Summoned, Att_Poison, 0>; 183
dseg:019C 00 00 01 01 00 06 00 00 00 00 00 80 00 00 02 00+s_UNIT_TYPE <offset cnst_StoneGiant,  15,  15,  0Ah,   2,   2,   8,   9,   4,   450,   9, Race_Nature, _ArmorersGuild, 0, 0,  20,   1, 0,   1, 0, 0, M_Mntnr, Imm_Stoning or Imm_Poison, 0, Ab_Summoned or Ab_WallCrshr, 0, 0>; 184
dseg:019C 00 00 00 00 60 23 06 00 FF 00 00 03 04 02 28 00+s_UNIT_TYPE <offset cnst_Colossus,  20,  20,  0Ah,   2,   3,  10,  15,   4,   800,  17, Race_Nature, _ArmorersGuild, 0, 0,  30,   1, 0,   1, 0, 0, 0, Imm_Stoning or Imm_Poison, 0, Ab_Summoned or Ab_WallCrshr, Att_1stStrike, 0>; 185
dseg:019C 01 05 04 00 00 00 01 01 00 06 00 00 00 00 00 80+s_UNIT_TYPE <offset cnst_Gorgons,   8,   1,  67h, 0,   2,   7,   9,   4,   599,  15, Race_Nature, _ArmorersGuild, 0, 0,   9,   1, 0,   4, 0, 254, M_Flying, Ab_Flying, 0, Ab_Summoned, 0, 0>; 186
dseg:019C 00 00 00 80 00 00 00 00 36 23 03 01 14 08 00 01+s_UNIT_TYPE <offset cnst_EarthElemental,  25, 0, 0FFh, 0,   1,   4,   8,   2,   160,   7, Race_Nature, _ArmorersGuild, 0, 0,  30,   1, 0,   1, 0, 0, 0, Imm_Stoning or Imm_Poison, 0, Ab_Summoned or Ab_WallCrshr, 0, 0>; 187
dseg:019C 04 02 1E 00 01 05 0F 03 00 00 01 01 00 06 00 00+s_UNIT_TYPE <offset cnst_Behemoth,  25, 0, 0FFh, 0,   2,   9,  10,   4,   700,  15, Race_Nature, _ArmorersGuild, 0, 0,  45,   1, 0,   1, 0, 0, 0, 0, 0, Ab_Summoned, 0, 0>; 188
dseg:019C 00 00 00 80 00 00 00 00 00 00 00 00 4C 23 02 00+s_UNIT_TYPE <offset cnst_GreatWyrm,  25, 0, 0FFh, 0,   3,  12,  12,   6,  1000,  20, Race_Nature, _ArmorersGuild, 0, 0,  45,   1, 0,   1, 0,  15, M_Merging, 0, 0, Ab_Summoned, Att_Poison, 0>; 189
dseg:019C FF 00 00 01 04 02 3C 00 02 05 00 00 00 00 0A 01+s_UNIT_TYPE <offset cnst_FloatingIsland, 0, 0, 0FFh, 0, 0, 0,  10,   4,    50,   5, Race_Sorcery, _ArmorersGuild, 0, 0,  45,   1,   8,   1, 0, 0, M_Sailing, Imm_Stoning or Imm_Death or Imm_Poison, 0, Ab_Summoned, 0, 0>; 190
dseg:019C 00 01 00 00 00 00 00 80 00 00 20 00 00 00 00 00+s_UNIT_TYPE <offset cnst_PhantomBeast,  18, 0, 0FFh, 0,   1, 0,   8,   4,   225,   4, Race_Sorcery, _ArmorersGuild, 0, 0,  20,   1, 0,   1, 0, 0, 0, Imm_Stoning or Imm_Death or Imm_Poison, 0, Ab_Summoned or Ab_NonCorporeal, Att_Illusion, 0>; 191
dseg:019C FA 23 07 00 FF 00 00 03 04 06 64 00 02 05 09 03+s_UNIT_TYPE <offset cnst_PhantomWarriors,   3, 0, 0FFh, 0, 0, 0,   6,   2,    20,   2, Race_Sorcery, _ArmorersGuild, 0, 0,   1,   1, 0,   6, 0, 0, 0, Imm_Stoning or Imm_Death or Imm_Poison, 0, Ab_Summoned or Ab_NonCorporeal, Att_Illusion, 0>; 192
dseg:019C 00 00 05 01 00 04 00 00 00 00 00 00 00 00 00 00+s_UNIT_TYPE <offset cnst_StormGiant,  12,  10,  1Eh,   4,   2,   7,   9,   4,   500,  10, Race_Sorcery, _ArmorersGuild, 0, 0,  20,   1, 0,   1, 0, 0, 0, 0, 0, Ab_Summoned or Ab_WallCrshr, Att_ArmorPrc, 0>; 193
dseg:019C 00 00 00 00 23 23 01 00 FF 00 00 02 06 02 0F 00+s_UNIT_TYPE <offset cnst_AirElemental,  15, 0, 0FFh, 0,   1,   8,   9,  10,   170,   8, Race_Sorcery, _ArmorersGuild, 0, 0,  10,   1, 0,   1, 0, 0, M_Flying, Imm_Stoning or Imm_Poison or Imm_Weapon or Ab_Flying, 0, Ab_Summoned or Ab_Invisibility, 0, 0>; 194
dseg:019C 01 06 00 00 00 00 01 01 00 08 00 00 00 00 00 84+s_UNIT_TYPE <offset cnst_Djinn,  15,   8,  22h, 0,   3,   8,  10,   6,   650,  17, Race_Sorcery, _ArmorersGuild, 0, 0,  20,   1, 0,   1, 0, 0, M_Flying or M_Teleport, Ab_Flying or Ab_Caster_20, 0, Ab_Summoned or Ab_WindWalking, 0, 0>; 195
dseg:019C 00 00 00 00 00 00 00 00 2C 23 02 00 FF 00 00 02+s_UNIT_TYPE <offset cnst_SkyDrake,  20,  20,  66h, 0,   3,  10,  14,   8,  1000,  25, Race_Sorcery, _ArmorersGuild, 0, 0,  25,   1, 0,   1, 0, 0, M_Flying, Imm_Illusion or Imm_Magic or Ab_Flying, 0, Ab_Summoned, 0, 0>; 196
dseg:019C 06 02 1E 00 01 06 03 08 00 00 01 01 00 08 00 00+s_UNIT_TYPE <offset cnst_Nagas,   4, 0, 0FFh, 0,   1,   3,   7,   2,   120,   2, Race_Sorcery, _ArmorersGuild, 0, 0,   6,   1, 0,   2, 0,   4, M_Swimming, 0, 0, Ab_Summoned, Att_1stStrike or Att_Poison, 0>; 197

*/