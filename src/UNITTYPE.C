
#include "MoX_TYPE.H"

#include "UNITTYPE.H"



/*
struct s_TST_UNIT_TYPE
{
    char * name;
};

struct s_TST_UNIT_TYPE _tst_unit_type_table[] =
{
    {&_unit_type_names[0]},
    {&_unit_type_names[1]}
};
*/


/*

    BEGIN: STU-MoXer :: Generate_Unit_Type_Data()

*/



char unit_type_name_000[] = "Dwarf";
char unit_type_name_001[] = "Barbarian";
char unit_type_name_002[] = "Sage";
char unit_type_name_003[] = "Dervish";
char unit_type_name_004[] = "Beastmaster";
char unit_type_name_005[] = "Bard";
char unit_type_name_006[] = "Orc Warrior";
char unit_type_name_007[] = "Healer";
char unit_type_name_008[] = "Huntress";
char unit_type_name_009[] = "Thief";
char unit_type_name_010[] = "Druid";
char unit_type_name_011[] = "War Monk";
char unit_type_name_012[] = "Warrior Mage";
char unit_type_name_013[] = "Magician";
char unit_type_name_014[] = "Assassin";
char unit_type_name_015[] = "Wind Mage";
char unit_type_name_016[] = "Ranger";
char unit_type_name_017[] = "Draconian";
char unit_type_name_018[] = "Witch";
char unit_type_name_019[] = "Golden One";
char unit_type_name_020[] = "Ninja";
char unit_type_name_021[] = "Rogue";
char unit_type_name_022[] = "Amazon";
char unit_type_name_023[] = "Warlock";
char unit_type_name_024[] = "Unknown";
char unit_type_name_025[] = "Illusionist";
char unit_type_name_026[] = "Swordsman";
char unit_type_name_027[] = "Priestess";
char unit_type_name_028[] = "Paladin";
char unit_type_name_029[] = "Black Knight";
char unit_type_name_030[] = "Elven Archer";
char unit_type_name_031[] = "Necromancer";
char unit_type_name_032[] = "Chaos Warrior";
char unit_type_name_033[] = "Chosen";

char unit_type_name_034[] = "Trireme";
char unit_type_name_035[] = "Galley";
char unit_type_name_036[] = "Catapult";
char unit_type_name_037[] = "Warship";
char unit_type_name_038[] = "Spearmen";
char unit_type_name_039[] = "Swordsmen";
char unit_type_name_040[] = "Bowmen";
char unit_type_name_041[] = "Cavalry";
char unit_type_name_042[] = "Shaman";
char unit_type_name_043[] = "Settlers";
char unit_type_name_044[] = "Berserkers";
char unit_type_name_045[] = "Halberdiers";
char unit_type_name_046[] = "Priests";
char unit_type_name_047[] = "Magicians";
char unit_type_name_048[] = "Engineers";
char unit_type_name_049[] = "Centaurs";
char unit_type_name_050[] = "Manticores";
char unit_type_name_051[] = "Minotaurs";
char unit_type_name_052[] = "Nightblades";
char unit_type_name_053[] = "Warlocks";
char unit_type_name_054[] = "Nightmares";
char unit_type_name_055[] = "Doom Drakes";
char unit_type_name_056[] = "Air Ship";
char unit_type_name_057[] = "Hammerhands";
char unit_type_name_058[] = "Steam Cannon";
char unit_type_name_059[] = "Golem";
char unit_type_name_060[] = "Wolf Riders";
char unit_type_name_061[] = "Slingers";
char unit_type_name_062[] = "Longbowmen";
char unit_type_name_063[] = "Elven Lords";
char unit_type_name_064[] = "Pegasai";
char unit_type_name_065[] = "Pikemen";
char unit_type_name_066[] = "Paladins";
char unit_type_name_067[] = "Stag Beetle";
char unit_type_name_068[] = "Javelineers";
char unit_type_name_069[] = "Dragon Turtle";
char unit_type_name_070[] = "Horsebowmen";
char unit_type_name_071[] = "Rangers";
char unit_type_name_072[] = "Griffins";
char unit_type_name_073[] = "Wyvern Riders";
char unit_type_name_074[] = "War Trolls";
char unit_type_name_075[] = "War Mammoths";
char unit_type_name_076[] = "Magic Spirit";
char unit_type_name_077[] = "Hell Hounds";
char unit_type_name_078[] = "Gargoyles";
char unit_type_name_079[] = "Fire Giant";
char unit_type_name_080[] = "Fire Elemental";
char unit_type_name_081[] = "Chaos Spawn";
char unit_type_name_082[] = "Chimera";
char unit_type_name_083[] = "Doom Bat";
char unit_type_name_084[] = "Efreet";
char unit_type_name_085[] = "Hydra";
char unit_type_name_086[] = "Great Drake";
char unit_type_name_087[] = "Skeletons";
char unit_type_name_088[] = "Ghouls";
char unit_type_name_089[] = "Night Stalker";
char unit_type_name_090[] = "Werewolves";
char unit_type_name_091[] = "Demon";
char unit_type_name_092[] = "Wraiths";
char unit_type_name_093[] = "Shadow Demons";
char unit_type_name_094[] = "Death Knights";
char unit_type_name_095[] = "Demon Lord";
char unit_type_name_096[] = "Zombies";
char unit_type_name_097[] = "Unicorns";
char unit_type_name_098[] = "Guardian Spirit";
char unit_type_name_099[] = "Angel";
char unit_type_name_100[] = "Arch Angel";
char unit_type_name_101[] = "War Bears";
char unit_type_name_102[] = "Sprites";
char unit_type_name_103[] = "Cockatrices";
char unit_type_name_104[] = "Basilisk";
char unit_type_name_105[] = "Giant Spiders";
char unit_type_name_106[] = "Stone Giant";
char unit_type_name_107[] = "Colossus";
char unit_type_name_108[] = "Gorgons";
char unit_type_name_109[] = "Earth Elemental";
char unit_type_name_110[] = "Behemoth";
char unit_type_name_111[] = "Great Wyrm";
char unit_type_name_112[] = "Floating Island";
char unit_type_name_113[] = "Phantom Beast";
char unit_type_name_114[] = "Phantom Warriors";
char unit_type_name_115[] = "Storm Giant";
char unit_type_name_116[] = "Air Elemental";
char unit_type_name_117[] = "Djinn";
char unit_type_name_118[] = "Sky Drake";
char unit_type_name_119[] = "Nagas";















































char * _unit_type_names[] =
{
    unit_type_name_000,
    unit_type_name_001,
    unit_type_name_002,
    unit_type_name_003,
    unit_type_name_004,
    unit_type_name_005,
    unit_type_name_006,
    unit_type_name_007,
    unit_type_name_008,
    unit_type_name_009,
    unit_type_name_010,
    unit_type_name_011,
    unit_type_name_012,
    unit_type_name_013,
    unit_type_name_014,
    unit_type_name_015,
    unit_type_name_016,
    unit_type_name_017,
    unit_type_name_018,
    unit_type_name_019,
    unit_type_name_020,
    unit_type_name_021,
    unit_type_name_022,
    unit_type_name_023,
    unit_type_name_024,
    unit_type_name_025,
    unit_type_name_026,
    unit_type_name_027,
    unit_type_name_028,
    unit_type_name_029,
    unit_type_name_030,
    unit_type_name_031,
    unit_type_name_032,
    unit_type_name_033,
    unit_type_name_034,
    unit_type_name_035,
    unit_type_name_036,
    unit_type_name_037,
    unit_type_name_038,
    unit_type_name_039,
    unit_type_name_040,
    unit_type_name_041,
    unit_type_name_042,
    unit_type_name_043,
    unit_type_name_044,
    unit_type_name_045,
    unit_type_name_046,
    unit_type_name_047,
    unit_type_name_048,
    unit_type_name_049,
    unit_type_name_050,
    unit_type_name_051,
    unit_type_name_052,
    unit_type_name_053,
    unit_type_name_054,
    unit_type_name_055,
    unit_type_name_056,
    unit_type_name_057,
    unit_type_name_058,
    unit_type_name_059,
    unit_type_name_060,
    unit_type_name_061,
    unit_type_name_062,
    unit_type_name_063,
    unit_type_name_064,
    unit_type_name_065,  /* [65] */
    unit_type_name_066,
    unit_type_name_067,
    unit_type_name_068,
    unit_type_name_069,
    unit_type_name_070,
    unit_type_name_071,
    unit_type_name_072,
    unit_type_name_073,
    unit_type_name_074,
    unit_type_name_075,
    unit_type_name_076,
    unit_type_name_077,
    unit_type_name_078,
    unit_type_name_079,
    unit_type_name_080,
    unit_type_name_081,
    unit_type_name_082,
    unit_type_name_083,
    unit_type_name_084,
    unit_type_name_085,
    unit_type_name_086,
    unit_type_name_087,
    unit_type_name_088,
    unit_type_name_089,
    unit_type_name_090,
    unit_type_name_091,
    unit_type_name_092,
    unit_type_name_093,
    unit_type_name_094,
    unit_type_name_095,
    unit_type_name_096,
    unit_type_name_097,
    unit_type_name_098,
    unit_type_name_099,
    unit_type_name_100,
    unit_type_name_101,
    unit_type_name_102,
    unit_type_name_103,
    unit_type_name_104,
    unit_type_name_105,
    unit_type_name_106,
    unit_type_name_107,
    unit_type_name_108,
    unit_type_name_109,
    unit_type_name_110,
    unit_type_name_111,
    unit_type_name_112,
    unit_type_name_113,
    unit_type_name_114,
    unit_type_name_115,
    unit_type_name_116,
    unit_type_name_117,
    unit_type_name_118,
    unit_type_name_119
};


/*
; enum Race_Code
R_Barbarian  = 0
R_Beastman  = 1
R_Dark_Elf  = 2
R_Draconian  = 3
R_Dwarf  = 4
R_Gnoll  = 5
R_Halfling  = 6
R_High_Elf  = 7
R_High_Man  = 8
R_Klackon  = 9
R_Lizardman  = 0Ah
R_Nomad  = 0Bh
R_Orc  = 0Ch
R_Troll  = 0Dh
Race_Generic  = 0Eh
Race_Arcane  = 0Fh
Race_Nature  = 10h
Race_Sorcery  = 11h
Race_Chaos  = 12h
Race_Life  = 13h
Race_Death  = 14h
*/
/* Name, Melee, Ranged, Ranged_Type, Ammo, To_Hit, Defense, Resist, Move_Halves, Cost, Upkeep, Race, Bldng1_or_Portrait, Bldng2_or_HeroType, pict_seg, Hits, Sight, Transport, Figures, Construction, Spec_Att_Attr, Move_Flags, Attribs_1, Attribs_2, Abilities, Attack_Flags, Sound */
struct s_UNIT_TYPE _unit_type_table[] =
{
    /* BEGIN: Hero Unit Type */
    /* Name,                 mel, rng,  rt, amm, hit, def, res, mov, cos, upk, rac, bl1, bl2,   pict, hp, sco, tra, fig, con, spec,   mvfl,   att1,   att2,    abl,   atfl, snd */
    {&_unit_type_names[0],     5,   0,  -1,   0,   0,   4,  10,   4, 100,   2,   4,  10,   0, 0x0000, 10,   1,   0,   1,   0,    0, 0x0041, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[1],     5,   3, 100,   0,   0,   3,   6,   4, 100,   2,   0,  17,   0, 0x0000,  9,   1,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[2],     1,   6,  32,   0,   0,   4,   6,   4, 100,   2,   8,  28,   3, 0x0000,  5,   1,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[3],     4,   4,  20,   8,   0,   4,   6,   4, 100,   2,   8,   2,   1, 0x0000,  6,   1,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[4],     5,   0,  -1,   0,   0,   4,   6,   4, 100,   2,   1,  18,   0, 0x0000,  7,   3,   0,   1,   0,    0, 0x0021, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[5],     4,   0,  -1,   0,   0,   5,   6,   4, 100,   2,   8,  22,   0, 0x0000,  6,   1,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[6],     6,   3, 100,   0,   0,   4,   6,   4, 100,   2,  12,   6,   0, 0x0000,  8,   1,   0,   1,   0,    0, 0x0041, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[7],     3,   6,  35,   0,   0,   5,   7,   4, 100,   2,   8,  30,   0, 0x0000,  5,   1,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x0010, 0x0000, 0}, 
    {&_unit_type_names[8],     5,   4,  20,   8,   0,   3,   6,   4, 100,   2,   8,  31,   1, 0x0000,  7,   1,   0,   1,   0,    0, 0x0060, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[9],     5,   0,  -1,   0,   0,   5,   6,   4, 100,   2,   8,  32,   0, 0x0000,  7,   1,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[10],    1,   8,  38,   0,   0,   5,   6,   4, 150,   3,   8,  12,   3, 0x0000,  5,   3,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x4000, 0x0000, 0}, 
    {&_unit_type_names[11],    6,   0,  -1,   0,   0,   5,   6,   4, 150,   3,   8,  20,   0, 0x0000,  6,   1,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[12],    4,   4,  31,   0,   0,   4,   6,   4, 150,   3,   8,  26,   2, 0x0000,  7,   1,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[13],    1,   8,  31,   0,   0,   5,  10,   4, 150,   3,   8,  15,   3, 0x0000,  5,   1,   0,   1,   0,    0, 0x0001, 0x0004, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[14],    3,   0,  -1,   0,   0,   5,   6,   4, 150,   3,   8,  33,   0, 0x0000,  6,   1,   0,   1,   0,    5, 0x0001, 0x0000, 0x0000, 0x0000, 0x0004, 0}, 
    {&_unit_type_names[15],    1,   6,  34,   0,   0,   5,   6,   4, 200,   4,   8,  19,   3, 0x0000,  5,   1,   0,   1,   0,    0, 0x0001, 0x0004, 0x0000, 0x1000, 0x0000, 0}, 
    {&_unit_type_names[16],    6,   5,  20,   8,   0,   5,   6,   4, 200,   4,   8,  25,   1, 0x0000,  8,   2,   0,   1,   0,    0, 0x0061, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[17],    7,   5, 101,   0,   0,   5,   6,   6, 200,   4,   3,   1,   0, 0x0000,  8,   1,   0,   1,   0,    0, 0x0008, 0x0200, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[18],    1,   8,  31,   0,   0,   5,   6,   4, 200,   4,   8,   3,   3, 0x0000,  5,   1,   0,   1,   0,    0, 0x0001, 0x0004, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[19],    6,   6,  31,   0,   0,   6,   6,   4, 200,   4,   8,  34,   0, 0x0000,  6,   1,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[20],    6,   0,  -1,   0,   0,   5,   6,   4, 300,   6,   8,  29,   0, 0x0000,  7,   1,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x0040, 0x0000, 0}, 
    {&_unit_type_names[21],    7,   0,  -1,   0,   0,   5,   6,   4, 300,   6,   8,   8,   0, 0x0000,  8,   1,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[22],    7,   4, 100,   0,   0,   4,   6,   4, 300,   6,   8,  13,   0, 0x0000,  8,   1,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[23],    1,   8,  30,   0,   0,   5,  10,   4, 300,   6,   8,  21,   3, 0x0000,  5,   1,   0,   1,   0,    0, 0x0001, 0x0004, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[24],    5,   5,  31,   0,   0,   4,  10,   4, 300,   6,   8,   5,   2, 0x0000,  8,   1,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[25],    1,   5,  32,   0,   0,   4,   6,   4, 500,  10,   8,  24,   3, 0x0000,  5,   1,   0,   1,   0,    0, 0x0001, 0x0004, 0x0000, 0x0000, 0x0040, 0}, 
    {&_unit_type_names[26],    6,   0,  -1,   0,   0,   5,   6,   4, 500,  10,   8,   7,   0, 0x0000, 10,   1,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[27],    2,   8,  35,   0,   0,   5,   6,   4, 500,  10,   8,  23,   3, 0x0000,  5,   1,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x4010, 0x0000, 0}, 
    {&_unit_type_names[28],    9,   0,  -1,   0,   0,   5,   6,   4, 500,  10,   8,  14,   0, 0x0000,  8,   1,   0,   1,   0,    0, 0x0001, 0x0020, 0x0000, 0x0010, 0x0003, 0}, 
    {&_unit_type_names[29],    9,   0,  -1,   0,   0,   5,   6,   4, 500,  10,   8,  16,   0, 0x0000, 10,   1,   0,   1,   0,    0, 0x0001, 0x0020, 0x0000, 0x0000, 0x0003, 0}, 
    {&_unit_type_names[31],    5,   8,  20,   8,   0,   6,   6,   4, 500,  10,   7,  27,   1, 0x0000,  6,   1,   0,   1,   0,    0, 0x0021, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[30],    8,   0,  -1,   0,   0,   5,   6,   4, 500,  10,   8,   9,   0, 0x0000,  9,   1,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
    {&_unit_type_names[32],    1,   7,  33,   0,   0,   5,   6,   4, 500,  10,   8,  11,   3, 0x0000,  5,   1,   0,   1,   0,    0, 0x0001, 0x0004, 0x0000, 0x0000, 0x0008, 0}, 
    {&_unit_type_names[33],    8,   8,  30,   0,   0,   5,   9,   4, 500,  10,   8,   4,   2, 0x0000,  8,   1,   0,   1,   0,    0, 0x0001, 0x0000, 0x0000, 0x0000, 0x0001, 0}, 
    {&_unit_type_names[34],   12,   0,  -1,   0,   0,   8,  12,   4, 600,   0,  19,   0,   0, 0x0000, 12,   1,   0,   1,   0,    0, 0x0001, 0x0024, 0x0000, 0x0000, 0x0000, 0}, 
    /* END: Hero Unit Type */

/* Name,                 mel, rng, rt, amm, hit, def, res, mov, cos, upk, rac, bl1, bl2,   pict, hp, sco, tra, fig, con, spec,   mvfl,   att1,   att2,    abl,   atfl, snd */
{&_unit_type_names[35],    6,   0, -1,   0,   0,   4,   4,   4,  60,   2,  14,  12,   0, 0x0000, 10,   1,   2,   1,   0,    0, 0x0002, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[36],    8,   2, 20,   8,   0,   4,   6,   6, 100,   2,  14,  13,   0, 0x0000, 20,   1,   5,   1,   0,    0, 0x0002, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[37],    0,  10, 10,  10,   0,   2,   4,   2, 100,   2,  14,  33,   0, 0x0000, 10,   1,   0,   1,   0,    0, 0x0000, 0x8000, 0x0000, 0x0108, 0x0000, 0}, 
{&_unit_type_names[38],   10,  10, 10,  99,   0,   5,   7,   8, 160,   2,  14,  14,   3, 0x0000, 30,   1,   3,   1,   0,    0, 0x0002, 0x8000, 0x0000, 0x0100, 0x0000, 0}, 
{&_unit_type_names[39],    1,  1, 100,   0,   0,   2,   5,   2,  15,   1,   0,   0,   0, 0x0000,  1,   1,   0,   8,   0,    0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[40],    3,  1, 100,   0,   0,   2,   5,   2,  30,   1,   0,   8,   3, 0x0000,  1,   1,   0,   6,   0,    0, 0x0000, 0x8000, 0x0000, 0x0002, 0x0000, 0}, 
{&_unit_type_names[41],    1,  1,  20,   8,   0,   1,   5,   2,  30,   1,   0,  15,   3, 0x0000,  1,   1,   0,   6,   0,    0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[42],    4,  1, 100,   0,   0,   2,   5,   4,  60,   1,   0,   9,   3, 0x0000,  3,   1,   0,   4,   0,    0, 0x0001, 0x8000, 0x0000, 0x0000, 0x0002, 0}, 
{&_unit_type_names[43],    2,  2,  35,   4,   0,   3,   7,   2,  50,   1,   0,  22,   0, 0x0000,  1,   1,   0,   4,   0,    0, 0x0000, 0x8000, 0x0000, 0x4010, 0x0000, 0}, 
{&_unit_type_names[44],    0,  0,  -1,   0,   0, 1, 5, 2, 60, 2, 0, 0, 0, 0x0000, 10, 1, 0, 1, 0, 0, 0x0000, 0x8000, 0x0000, 0x0020, 0x0000, 0}, 
{&_unit_type_names[45],    7,  3, 100,   0,   0, 3, 7, 2, 120, 2, 0, 6, 0, 0x0000, 3, 1, 0, 6, 0, 0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[39],    2,  0,  -1,   0,   0, 2, 5, 2, 20, 1, 1, 0, 0, 0x0000, 2, 1, 0, 8, 0, 0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[40],    4,  0,  -1,   0,   0, 2, 5, 2, 40, 1, 1, 3, 8, 0x0000, 2, 1, 0, 6, 0, 0, 0x0000, 0x8000, 0x0000, 0x0002, 0x0000, 0}, 
{&_unit_type_names[46],    5,  0,  -1,   0,   0, 3, 5, 2, 80, 2, 1, 4, 0, 0x0000, 2, 1, 0, 6, 0, 0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[41],    2,  1,  20,   8,   0, 1, 5, 2, 60, 2, 1, 15, 3, 0x0000, 2, 1, 0, 6, 0, 0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[47],    4,  4,  35,   4,   0, 4, 8, 2, 150, 3, 1, 24, 0, 0x0000, 2, 1, 0, 4, 0, 0, 0x0000, 0x8000, 0x0001, 0x4010, 0x0000, 0}, 
{&_unit_type_names[48],    2,  5,  31,   4,   0, 3, 9, 2, 180, 4, 1, 21, 0, 0x0000, 2, 1, 0, 4, 0, 0, 0x0000, 0x8004, 0x0002, 0x0000, 0x0000, 0}, 
{&_unit_type_names[49],    2,  0,  -1,   0,   0, 1, 5, 2, 60, 2, 1, 32, 0, 0x0000, 2, 1, 0, 6, 1, 0, 0x0000, 0x8000, 0x0000, 0x0008, 0x0000, 0}, 
{&_unit_type_names[44],    1,  0,  -1,   0,   0, 1, 5, 2, 120, 4, 1, 0, 0, 0x0000, 20, 1, 0, 1, 0, 0, 0x0000, 0x8000, 0x0000, 0x0020, 0x0000, 0}, 
{&_unit_type_names[50],    3,  2,  20,   6,   0, 3, 5, 4, 100, 2, 1, 9, 3, 0x0000, 3, 1, 0, 4, 0, 0, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
/* 55 */ {&_unit_type_names[51],    5,  0,  -1,   0,   0, 3, 6, 4, 160, 3, 1, 10, 3, 0x0000, 7, 2, 0, 2, 0, 6, 0x0008, 0x0200, 0x0000, 0x0000, 0x0004, 0}, 
{&_unit_type_names[52],   12,  0,  -1,   0,   2, 4, 7, 2, 200, 4, 1, 6, 0, 0x0000, 12, 1, 0, 2, 0, 0, 0x0000, 0x0000, 0x0000, 0x0002, 0x0000, 0}, 
{&_unit_type_names[39],    1,  1,  36,   4,   0, 2, 7, 2, 25, 1, 2, 0, 0, 0x0000, 1, 1, 0, 8, 0, 0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[40],    3,  1,  36,   4,   0, 2, 7, 2, 50, 1, 2, 3, 8, 0x0000, 1, 1, 0, 6, 0, 0, 0x0000, 0x8000, 0x0000, 0x0002, 0x0000, 0}, 
{&_unit_type_names[46],    4,  1,  36,   4,   0, 3, 7, 2, 100, 2, 2, 4, 0, 0x0000, 1, 1, 0, 6, 0, 0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[42],    4,  1,  36,   4,   0, 2, 7, 4, 100, 2, 2, 3, 9, 0x0000, 3, 1, 0, 4, 0, 0, 0x0001, 0x8000, 0x0000, 0x0000, 0x0002, 0}, 
{&_unit_type_names[47],    3,  6,  35,   4,   0, 4, 10, 2, 200, 2, 2, 24, 0, 0x0000, 1, 1, 0, 4, 0, 0, 0x0000, 0x8000, 0x0001, 0x4010, 0x0000, 0}, 
{&_unit_type_names[44],    0,  0,  -1,   0,   0, 1, 7, 2, 150, 3, 2, 0, 0, 0x0000, 10, 1, 0, 1, 0, 0, 0x0000, 0x8000, 0x0000, 0x0020, 0x0000, 0}, 
{&_unit_type_names[53],    4,  0,  -1,   0,   0, 3, 7, 2, 120, 3, 2, 5, 0, 0x0000, 1, 1, 0, 6, 0, 1, 0x0000, 0x0000, 0x0000, 0x0040, 0x0004, 0}, 
{&_unit_type_names[54],    1,  7,  31,   4,   0, 4, 9, 2, 240, 4, 2, 21, 0, 0x0000, 1, 1, 0, 4, 0, 0, 0x0000, 0x0004, 0x0004, 0x0000, 0x0000, 0}, 
{&_unit_type_names[55],    8,  5,  36,   4,   0, 4, 8, 6, 160, 4, 2, 11, 3, 0x0000, 10, 2, 0, 2, 0, 0, 0x0008, 0x0200, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[39],    1,  1, 101,   0,   0, 3, 6, 4, 25, 1, 3, 0, 0, 0x0000, 1, 1, 0, 8, 0, 0, 0x0008, 0x8200, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[40],    3,  1, 101,   0,   0, 3, 6, 4, 50, 1, 3, 3, 8, 0x0000, 1, 1, 0, 6, 0, 0, 0x0008, 0x8200, 0x0000, 0x0002, 0x0000, 0}, 
{&_unit_type_names[46],    4,  1, 101,   0,   0, 4, 6, 4, 100, 2, 3, 4, 0, 0x0000, 1, 1, 0, 6, 0, 0, 0x0008, 0x8200, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[41],    1,  1,  20,   8,   0, 2, 6, 4, 45, 2, 3, 15, 3, 0x0000, 1, 1, 0, 6, 0, 0, 0x0008, 0x8200, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[43],    2,  2,  35,   4,   0, 4, 8, 4, 75, 2, 3, 22, 0, 0x0000, 1, 1, 0, 4, 0, 0, 0x0008, 0x8200, 0x0000, 0x4010, 0x0000, 0}, 
{&_unit_type_names[48],    1,  5,  31,   4,   0, 4, 10, 4, 180, 3, 3, 21, 0, 0x0000, 1, 1, 0, 4, 0, 0, 0x0008, 0x8204, 0x0002, 0x0000, 0x0000, 0}, 
{&_unit_type_names[49],    1,  1, 101,   0,   0, 2, 6, 4, 100, 2, 3, 10, 33, 0x0000, 1, 1, 0, 6, 1, 0, 0x0008, 0x8200, 0x0000, 0x0008, 0x0000, 0}, 
{&_unit_type_names[44],    0,  0,  -1,   0,   0, 2, 6, 2, 150, 3, 3, 0, 0, 0x0000, 10, 1, 0, 1, 0, 0, 0x0008, 0x8200, 0x0000, 0x0020, 0x0000, 0}, 
{&_unit_type_names[56],    8,  6, 101,   0,   0, 3, 9, 6, 160, 3, 3, 9, 3, 0x0000, 10, 2, 0, 2, 0, 0, 0x0008, 0x0200, 0x0000, 0x0000, 0x0000, 0}, 
/* 75 */ {&_unit_type_names[57],    5, 10,  10,  10,   0, 5, 8, 8, 200, 4, 3, 13, 0, 0x0000, 20, 2, 0, 1, 0, 0, 0x0008, 0x0200, 0x0000, 0x0008, 0x0000, 0}, 
{&_unit_type_names[40],    3,  0,  -1,   0,   0, 2, 8, 2, 50, 2, 4, 3, 8, 0x0000, 3, 1, 0, 6, 0, 0, 0x0040, 0x8000, 0x0000, 0x0002, 0x0000, 0}, 
{&_unit_type_names[46],    4,  0,  -1,   0,   0, 3, 8, 2, 100, 2, 4, 4, 0, 0x0000, 3, 1, 0, 6, 0, 0, 0x0040, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[49],    1,  0,  -1,   0,   0, 1, 8, 2, 40, 2, 4, 32, 0, 0x0000, 3, 1, 0, 6, 2, 0, 0x0040, 0x8000, 0x0000, 0x0008, 0x0000, 0}, 
{&_unit_type_names[58],    8,  0,  -1,   0,   0, 4, 9, 2, 160, 4, 4, 5, 0, 0x0000, 4, 1, 0, 6, 0, 0, 0x0040, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[59],    0, 12,  10,  10,   0, 2, 9, 2, 180, 4, 4, 34, 3, 0x0000, 12, 1, 0, 1, 0, 0, 0x0040, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[60],   12,  0,  -1,   0,   0, 8, 15, 2, 200, 4, 4, 6, 0, 0x0000, 20, 1, 0, 1, 0, 0, 0x0000, 0x00C0, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[44],    0,  0,  -1,   0,   0, 1, 8, 2, 150, 2, 4, 0, 0, 0x0000, 30, 1, 0, 1, 0, 0, 0x0000, 0x8000, 0x0000, 0x0020, 0x0000, 0}, 
{&_unit_type_names[39],    3,  0,  -1,   0,   0, 2, 4, 2, 10, 1, 5, 0, 0, 0x0000, 1, 1, 0, 8, 0, 0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[40],    5,  0,  -1,   0,   0, 2, 4, 2, 20, 1, 5, 3, 8, 0x0000, 1, 1, 0, 6, 0, 0, 0x0000, 0x8000, 0x0000, 0x0002, 0x0000, 0}, 
{&_unit_type_names[46],    6,  0,  -1,   0,   0, 3, 4, 2, 40, 1, 5, 4, 0, 0x0000, 1, 1, 0, 6, 0, 0, 0x0000, 0x8000, 0x0000, 0x8000, 0x0000, 0}, 
{&_unit_type_names[41],    3,  1,  20,   8,   0, 1, 4, 2, 30, 1, 5, 15, 3, 0x0000, 1, 1, 0, 6, 0, 0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[44],    2,  0,  -1,   0,   0, 1, 4, 2, 60, 2, 5, 0, 0, 0x0000, 10, 1, 0, 1, 0, 0, 0x0000, 0x8000, 0x0000, 0x0020, 0x0000, 0}, 
{&_unit_type_names[61],    7,  0,  -1,   0,   0, 3, 4, 6, 100, 2, 5, 9, 3, 0x0000, 5, 1, 0, 4, 0, 0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[39],    1,  0,  -1,   0,   0, 2, 6, 2, 15, 1, 6, 0, 0, 0x0000, 1, 1, 0, 8, 0, 0, 0x0000, 0x8400, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[40],    2,  0,  -1,   0,   0, 2, 6, 2, 30, 1, 6, 3, 8, 0x0000, 1, 1, 0, 8, 0, 0, 0x0000, 0x8400, 0x0000, 0x0002, 0x0000, 0}, 
{&_unit_type_names[41],    1,  1,  20,   8,   0, 1, 6, 2, 45, 1, 6, 15, 3, 0x0000, 1, 1, 0, 6, 0, 0, 0x0000, 0x8400, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[43],    1,  2,  35,   4,   0, 3, 8, 2, 75, 1, 6, 22, 0, 0x0000, 1, 1, 0, 4, 0, 0, 0x0000, 0x8400, 0x0000, 0x4010, 0x0000, 0}, 
{&_unit_type_names[44],    0,  0,  -1,   0,   0, 1, 6, 2, 90, 1, 6, 0, 0, 0x0000, 10, 1, 0, 1, 0, 0, 0x0000, 0x8400, 0x0000, 0x0020, 0x0000, 0}, 
{&_unit_type_names[62],    1,  2,  21,   6,   0, 2, 6, 2, 100, 1, 6, 4, 0, 0x0000, 1, 1, 0, 8, 0, 0, 0x0000, 0x0400, 0x0000, 0x0000, 0x0000, 0}, 
/* 95 */ {&_unit_type_names[39],    1,  0,  -1,   0,   1, 2, 6, 2, 15, 1, 7, 0, 0, 0x0000, 1, 1, 0, 8, 0, 0, 0x0020, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[40],    3,  0,  -1,   0,   1, 2, 6, 2, 30, 1, 7, 3, 8, 0x0000, 1, 1, 0, 6, 0, 0, 0x0020, 0x8000, 0x0000, 0x0002, 0x0000, 0}, 
{&_unit_type_names[46],    4,  0,  -1,   0,   1, 3, 6, 2, 60, 1, 7, 4, 0, 0x0000, 1, 1, 0, 6, 0, 0, 0x0020, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[42],    4,  0,  -1,   0,   1, 2, 6, 4, 60, 1, 7, 3, 9, 0x0000, 3, 1, 0, 4, 0, 0, 0x0021, 0x8000, 0x0000, 0x0000, 0x0002, 0}, 
{&_unit_type_names[48],    1,  5,  31,   4,   1, 3, 10, 4, 180, 2, 7, 21, 0, 0x0000, 1, 1, 0, 4, 0, 0, 0x0020, 0x8004, 0x0002, 0x0000, 0x0000, 0}, 
{&_unit_type_names[44],    0,  0,  -1,   0,   1, 1, 6, 2, 90, 2, 7, 0, 0, 0x0000, 10, 1, 0, 1, 0, 0, 0x0020, 0x8000, 0x0000, 0x0020, 0x0000, 0}, 
{&_unit_type_names[63],    1,  3,  20,   8,   1, 2, 6, 2, 80, 2, 7, 15, 3, 0x0000, 1, 1, 0, 6, 0, 0, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[64],    5,  0,  -1,   0,   2, 4, 9, 4, 160, 3, 7, 6, 0, 0x0000, 3, 1, 0, 4, 0, 0, 0x0021, 0x0000, 0x0000, 0x0000, 0x0003, 0}, 
{&_unit_type_names[65],    5,  3,  20,   8,   1, 4, 8, 6, 160, 3, 7, 11, 3, 0x0000, 5, 2, 0, 2, 0, 0, 0x0008, 0x0200, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[39],    1,  0,  -1,   0,   0, 2, 4, 2, 10, 1, 8, 0, 0, 0x0000, 1, 1, 0, 8, 0, 0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
/* 105 */ {&_unit_type_names[40],    3,  0,  -1,   0,   0, 2, 4, 2, 20, 1, 8, 3, 8, 0x0000, 1, 1, 0, 6, 0, 0, 0x0000, 0x8000, 0x0000, 0x0002, 0x0000, 0}, 
{&_unit_type_names[41],    1,  1,  20,   8,   0, 1, 4, 2, 30, 1, 8, 15, 3, 0x0000, 1, 1, 0, 6, 0, 0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[42],    4,  0,  -1,   0,   0, 2, 4, 4, 40, 1, 8,  3, 9, 0x0000, 3, 1, 0, 4, 0, 0, 0x0001, 0x8000, 0x0000, 0x0000, 0x0002, 0}, 
/*                          Name,  mel, rng, rt, amm, hit, def, res, mov, cos, upk, rac, bl1, bl2,   pict, hp, sco, tra, fig, con, spec,   mvfl,   att1,   att2,    abl,   atfl, snd */
/* 108 */ {&_unit_type_names[47],    3,  4,  35,   4,   0,   4,   7,   2, 100,   2,   8,  24,   0, 0x0000,  1,   1,   0,   4,   0,    0, 0x0000, 0x8000, 0x0001, 0x4010, 0x0000,   0},  /* Priests, High Men */
/* 109 */ {&_unit_type_names[48],    1,  5,  31,   4,   0,   3,   8,   2, 120,   3,   8,  21,   0, 0x0000,  1,   1,   0,   6,   0,    0, 0x0000, 0x8004, 0x0002, 0x0000, 0x0000,   0},  /* Magicians, High-Men */ 
/* 110 */ {&_unit_type_names[49],    1,  0,  -1,   0,   0,   1,   4,   2,  40,   1,   8,  32,   0, 0x0000,  1,   1,   0,   6,   1,    0, 0x0000, 0x8000, 0x0000, 0x0008, 0x0000,   0}, 
/* 111 */ {&_unit_type_names[44],    0,  0,  -1,   0,   0,   1,   4,   2,  60,   2,   8,   0,   0, 0x0000, 10,   1,   0,   1,   0,    0, 0x0000, 0x8000, 0x0000, 0x0020, 0x0000,   0}, 
/* Name,                 mel, rng, rt, amm, hit, def, res, mov, cos, upk, rac, bl1, bl2,   pict, hp, sco, tra, fig, con, spec,   mvfl,   att1,   att2,    abl,   atfl, snd */
/* 112 */ {&_unit_type_names[65],    5,   0, -1,   0,   0,   3,   5,   2,  80,   2,   8,   5,   0, 0x0000,  1,   1,   0,   8,   0,    0, 0x0000, 0x0000, 0x0000, 0x8000, 0x0001,   0},  /* Pikemen, High-Men */
/* 113 */ {&_unit_type_names[66],    6,  0,  -1,   0,   0,   5,   8,   4, 200,   3,   8,  25,   6, 0x0000,  4,   1,   0,   4,   0,    1, 0x0001, 0x0020, 0x0080, 0x0000, 0x0003,   0},   /* Paladin, High-Men */
{&_unit_type_names[39],    1,  0,  -1,   0,   0,   4,   5,   2,  20,   1,   9,   0,   0, 0x0000,  1,   1,   0,   8,   0,    0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000,   0}, 
{&_unit_type_names[40],    3,  0,  -1,   0,   0,   4,   5,   2,  40,   1,   9,   3,   8, 0x0000,  1,   1,   0,   6,   0,    0, 0x0000, 0x8000, 0x0000, 0x0002, 0x0000,   0}, 
/* Name,                 mel, rng, rt, amm, hit, def, res, mov, cos, upk, rac, bl1, bl2,   pict, hp, sco, tra, fig, con, spec,   mvfl,   att1,   att2,    abl,   atfl, snd */
{&_unit_type_names[46],    4,  0,  -1,   0,   0,   5,   5,   2,  80,   1,   9,   4,   0, 0x0000,  1,   1,   0,   6,   0,    0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000,   0},
{&_unit_type_names[49],    1,  0,  -1,   0,   0,   1,   5,   2,  80,   1,   9,  32,   0, 0x0000,  1,   1,   0,   6,   1,    0, 0x0000, 0x8000, 0x0000, 0x0008, 0x0000,   0}, 
{&_unit_type_names[44],    0,  0,  -1,   0,   0,   1,   5,   2, 120, 2, 9, 0, 0, 0x0000, 10, 1, 0, 1, 0, 0, 0x0000, 0x8000, 0x0000, 0x0020, 0x0000, 0}, 
{&_unit_type_names[68],   15,  5, 101,   0,   0,   7,   6,   4, 160, 2, 9, 9, 6, 0x0000, 20, 1, 0, 1, 0, 0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[39],    1,  0,  -1,   0,   0,   3,   4,   2, 10, 1, 10, 0, 0, 0x0000, 2, 1, 0, 8, 0, 0, 0x0004, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[40],    3,  0,  -1,   0,   0,   3,   4,   2, 20, 1, 10, 3, 8, 0x0000, 2, 1, 0, 6, 0, 0, 0x0004, 0x8000, 0x0000, 0x0002, 0x0000, 0}, 
{&_unit_type_names[46],    4,  0,  -1,   0,   0,   4,   4,   2, 40, 1, 10, 4, 0, 0x0000, 2, 1, 0, 6, 0, 0, 0x0004, 0x8000, 0x0000, 0x8000, 0x0000, 0}, 
{&_unit_type_names[69],    4,  3,  20,   6,   0,   4,   5,   2, 80, 2, 10, 5, 0, 0x0000, 2, 1, 0, 6, 0, 0, 0x0004, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[43],    2,  2,  35,   4,   0,   3,   6,   2, 60, 2, 10, 22, 0, 0x0000, 2, 1, 0, 4, 0, 0, 0x0004, 0x8000, 0x0000, 0x4010, 0x0000, 0}, 
{&_unit_type_names[44],    0,  0,  -1,   0,   0,   2,   4,   2, 120, 2, 10, 0, 0, 0x0000, 20, 1, 0, 1, 0, 0, 0x0004, 0x8000, 0x0000, 0x0020, 0x0000, 0}, 
{&_unit_type_names[70],   10,  5, 101,   0,   0,   8,   8,   4, 100, 1, 10, 9, 6, 0x0000, 15, 1, 0, 1, 0, 0, 0x0004, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[39],    1,  0,  -1,   0,   0,   2,   4,   2, 10, 1, 11, 0, 0, 0x0000, 1, 1, 0, 8, 0, 0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[40],    3,  0,  -1,   0,   0,   2,   4,   2, 20, 1, 11, 3, 8, 0x0000, 1, 1, 0, 6, 0, 0, 0x0000, 0x8000, 0x0000, 0x0002, 0x0000, 0}, 
{&_unit_type_names[41],    1,  1,  20,   8,   0,   1,   4,   2, 30, 1, 11, 15, 3, 0x0000, 1, 1, 0, 6, 0, 0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[47],    3,  4,  35,   4,   0,   4,   7,   2, 100, 1, 11, 24, 0, 0x0000, 1, 1, 0, 4, 0, 0, 0x0000, 0x8000, 0x0001, 0x4010, 0x0000, 0}, 
{&_unit_type_names[48],    1,  5,  31,   4,   0,   3,   8,   2, 120, 2, 11, 21, 0, 0x0000, 1, 1, 0, 4, 0, 0, 0x0000, 0x8004, 0x0002, 0x0000, 0x0000, 0}, 
{&_unit_type_names[44],    0,  0,  -1,   0,   0,   1,   4,   2, 60, 2, 11, 0, 0, 0x0000, 10, 1, 0, 1, 0, 0, 0x0000, 0x8000, 0x0000, 0x0020, 0x0000, 0}, 
{&_unit_type_names[71],    4,  2,  20,   8,   0,   2,   4,   4, 60, 1, 11, 9, 3, 0x0000, 3, 1, 0, 4, 0, 0, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[66],    5,  0,  -1,   0,   0,   3,   5,   2, 80, 3, 11, 5, 0, 0x0000, 1, 1, 0, 8, 0, 0, 0x0000, 0x0000, 0x0000, 0x8000, 0x0001, 0}, 
{&_unit_type_names[72],    4,  3,  20,   8,   0,   4,   6,   4, 120, 2, 11, 10, 3, 0x0000, 2, 1, 0, 4, 0, 0, 0x0060, 0x0000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[73],    9,  0,  -1,   0,   0,   5,   7,   4, 200, 2, 11, 11, 3, 0x0000, 10, 1, 0, 2, 0, 0, 0x0008, 0x0200, 0x0000, 0x0000, 0x0003, 0}, 
{&_unit_type_names[39],    1,  0,  -1,   0,   0,   2,   4,   2, 10, 1, 12, 0, 0, 0x0000, 1, 1, 0, 8, 0, 0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[40],    3,  0,  -1,   0,   0,   2,   4,   2, 20, 1, 12, 3, 8, 0x0000, 1, 1, 0, 6, 0, 0, 0x0000, 0x8000, 0x0000, 0x0002, 0x0000, 0}, 
{&_unit_type_names[46],    4,  0,  -1,   0,   0,   3,   4,   2, 40, 1, 12, 4, 0, 0x0000, 1, 1, 0, 6, 0, 0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[41],    1,  1,  20,   8,   0,   1,   4,   2, 30, 1, 12, 15, 3, 0x0000, 1, 1, 0, 6, 0, 0, 0x0000, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[42],    4,  0,  -1,   0,   0,   2,   4,   4, 40, 1, 12, 3, 9, 0x0000, 3, 1, 0, 4, 0, 0, 0x0001, 0x8000, 0x0000, 0x0000, 0x0000, 0}, 
{&_unit_type_names[43],    2,  2,  35,   4,   0,   3,   6,   2, 60, 1, 12, 22, 0, 0x0000, 1, 1, 0, 4, 0, 0, 0x0000, 0x8000, 0x0000, 0x4010, 0x0000, 0}, 
{&_unit_type_names[48],    1,  5,  31,   4,   0,   3,   8,   2, 120, 2, 12, 21, 0, 0x0000, 1, 1, 0, 4, 0, 0, 0x0000, 0x8004, 0x0002, 0x0000, 0x0000, 0}, 
{&_unit_type_names[49],    1,  0,  -1,   0,   0,   1,   4,   2, 40, 1, 12, 32, 0, 0x0000, 1, 1, 0, 6, 1, 0, 0x0000, 0x8000, 0x0000, 0x0008, 0x0000, 0}, 
{&_unit_type_names[44],    0,  0,  -1,   0,   0,   1,   4,   2, 60, 2, 12, 0, 0, 0x0000, 10, 1, 0, 1, 0, 0, 0x0000, 0x8000, 0x0000, 0x0020, 0x0000, 0}, 
{&_unit_type_names[74],    5,  0,  -1,   0,   0,   5,   7,   6, 200, 2, 12, 11, 3, 0x0000, 10, 1, 0, 2, 0, 6, 0x0008, 0x0200, 0x0000, 0x0000, 0x0004, 0}, 
{&_unit_type_names[39],    3,  0,  -1,   0,   0,   2,   7,   2, 30, 2, 13, 0, 0, 0x0000, 4, 1, 0, 4, 0, 0, 0x0000, 0x8000, 0x0000, 0x2000, 0x0000, 0}, 
{&_unit_type_names[40],    5,  0,  -1,   0,   0,   2,   7,   2, 60, 3, 13, 3, 8, 0x0000, 4, 1, 0, 4, 0, 0, 0x0000, 0x8000, 0x0000, 0x2002, 0x0000, 0}, 
{&_unit_type_names[46],    6,  0,  -1,   0,   0, 3, 7, 2, 120, 3, 13, 4, 0, 0x0000, 4, 1, 0, 4, 0, 0, 0x0000, 0x8000, 0x0000, 0xA000, 0x0000, 0}, 
{&_unit_type_names[43],    4,  2,  35,   4,   0, 3, 8, 2, 180, 2, 13, 22, 0, 0x0000, 4, 1, 0, 4, 0, 0, 0x0000, 0x8000, 0x0000, 0x6010, 0x0000, 0}, 
{&_unit_type_names[44],    0,  0,  -1,   0,   0, 1, 7, 2, 180, 2, 13, 0, 0, 0x0000, 40, 1, 0, 1, 0, 0, 0x0000, 0x8000, 0x0000, 0x2020, 0x0000, 0}, 
{&_unit_type_names[75],    8,  0,  -1,   0,   0, 4, 8, 4, 160, 3, 13, 5, 0, 0x0000, 5, 1, 0, 4, 0, 0, 0x0000, 0x0000, 0x0000, 0x2000, 0x0000, 0}, 
{&_unit_type_names[76],   10,  0,  -1,   0,   0, 6, 9, 4, 240, 4, 13, 9, 6, 0x0000, 12, 1, 0, 2, 0, 0, 0x0000, 0x0000, 0x0000, 0x0008, 0x0002, 0}, 
{&_unit_type_names[77],    5,  0,  -1,   0,   0, 4, 8, 2, 30, 1, 15, 6, 0, 0x0000, 10, 1, 0, 1, 0, 0, 0x0000, 0x0000, 0x0000, 0x0C01, 0x0000, 0}, 
{&_unit_type_names[78],    3,  3, 101,   0,   1, 2, 6, 4, 40, 1, 18, 6, 0, 0x0000, 4, 1, 0, 4, 0, 0, 0x0000, 0x0000, 0x0000, 0x0001, 0x0000, 0}, 
{&_unit_type_names[79],    4,  0,  -1,   0,   1, 8, 7, 4, 200, 5, 18, 6, 0, 0x0000, 4, 1, 0, 4, 0, 0, 0x0008, 0x0082, 0x0000, 0x0001, 0x0000, 0}, 
{&_unit_type_names[80],   10, 10,  10,   2,   1, 5, 7, 4, 150, 3, 18, 6, 0, 0x0000, 15, 1, 0, 1, 0, 0, 0x0040, 0x0001, 0x0000, 0x0009, 0x0000, 0}, 
{&_unit_type_names[81],   12,  0,  -1,   0,   0, 4, 6, 2, 100, 4, 18, 6, 0, 0x0000, 10, 1, 0, 1, 0, 0, 0x0000, 0x0083, 0x0000, 0x0001, 0x0000, 0}, 
{&_unit_type_names[82],    1,  4, 104,   0,   0, 6, 10, 2, 400, 12, 18, 6, 0, 0x0000, 15, 1, 0, 1, 0, 4, 0x0008, 0x0200, 0x0020, 0x0001, 0x0004, 0}, 
{&_unit_type_names[83],    7,  4, 101,   0,   1, 5, 8, 4, 350, 10, 18, 6, 0, 0x0000, 8, 1, 0, 4, 0, 0, 0x0008, 0x0200, 0x0000, 0x0001, 0x0000, 0}, 
{&_unit_type_names[84],   10,  0,  -1,   0,   1, 5, 9, 8, 300, 8, 18, 6, 0, 0x0000, 20, 1, 0, 1, 0, 0, 0x0008, 0x0200, 0x0008, 0x0001, 0x0000, 0}, 
{&_unit_type_names[85],    9,  9,  31,   0,   2, 7, 10, 6, 550, 15, 18, 6, 0, 0x0000, 12, 1, 0, 1, 0, 0, 0x0008, 0x2201, 0x0000, 0x0001, 0x0000, 0}, 
{&_unit_type_names[86],    6,  5, 101,   0,   1, 4, 11, 2, 650, 14, 18, 6, 0, 0x0000, 10, 1, 0, 9, 0, 0, 0x0000, 0x0000, 0x0000, 0x2001, 0x0000, 0}, 
{&_unit_type_names[87],   30, 30, 101,   0,   3, 10, 12, 4, 900, 30, 18, 6, 0, 0x0000, 30, 1, 0, 1, 0, 0, 0x0008, 0x0200, 0x0000, 0x0001, 0x0000, 0}, 
{&_unit_type_names[88],    3,  0,  -1,   0,   1, 4, 5, 2, 25, 1, 20, 6, 0, 0x0000, 1, 1, 0, 6, 0, 0, 0x0000, 0x00DC, 0x0000, 0x0001, 0x0000, 0}, 
{&_unit_type_names[89],    4,  0,  -1,   0,   1, 3, 6, 2, 80, 1, 20, 6, 0, 0x0000, 3, 1, 0, 4, 0, 1, 0x0000, 0x00D8, 0x0000, 0x0081, 0x0004, 0}, 
{&_unit_type_names[90],    7,  1, 105,   0,   1, 3, 8, 4, 250, 1, 20, 6, 0, 0x0000, 10, 1, 0, 1, 0, -2, 0x0000, 0x00D8, 0x0000, 0x0041, 0x0000, 0}, 
{&_unit_type_names[91],    5,  0,  -1,   0,   1, 1, 6, 4, 250, 5, 20, 6, 0, 0x0000, 5, 1, 0, 6, 0, 0, 0x0000, 0x01D8, 0x0000, 0x2001, 0x0000, 0}, 
{&_unit_type_names[92],   14,  0,  -1,   0,   1, 5, 7, 4, 80, 3, 20, 6, 0, 0x0000, 12, 1, 0, 1, 0, 0, 0x0008, 0x03DC, 0x0000, 0x0001, 0x0000, 0}, 
{&_unit_type_names[93],    7,  0,  -1,   0,   2, 6, 8, 4, 500, 5, 20, 6, 0, 0x0000, 8, 1, 0, 4, 0, -3, 0x0008, 0x03D8, 0x0000, 0x0801, 0x0008, 0}, 
{&_unit_type_names[94],    5,  4,  33,   8,   2, 4, 8, 2, 325, 7, 20, 6, 0, 0x0000, 5, 1, 0, 4, 0, 0, 0x0008, 0x03D8, 0x0000, 0x2805, 0x0000, 0}, 
{&_unit_type_names[95],    9,  0,  -1,   0,   3, 8, 10, 6, 600, 8, 20, 6, 0, 0x0000, 8, 1, 0, 4, 0, -4, 0x0008, 0x03D8, 0x0000, 0x0001, 0x000B, 0}, 
{&_unit_type_names[96],   20, 10,  33,   8,   3, 10, 12, 4, 900, 15, 20, 6, 0, 0x0000, 20, 1, 0, 1, 0, -5, 0x0008, 0x1BD8, 0x0020, 0x0001, 0x0008, 0}, 
{&_unit_type_names[97],    4,  0,  -1,   0,   1, 3, 3, 2, 30, 0, 20, 6, 0, 0x0000, 3, 1, 0, 6, 0, 0, 0x0000, 0x00D8, 0x0000, 0x0001, 0x0000, 0}, 
{&_unit_type_names[98],    5,  0,  -1,   0,   2, 3, 7, 4, 250, 5, 19, 6, 0, 0x0000, 6, 1, 0, 4, 0, 2, 0x0010, 0x0080, 0x0040, 0x0001, 0x0000, 0}, 
{&_unit_type_names[99],   10,  0,  -1,   0,   0, 4, 10, 2, 50, 1, 19, 6, 0, 0x0000, 10, 1, 0, 1, 0, 1, 0x0000, 0x0000, 0x0040, 0x0C01, 0x0000, 0}, 
{&_unit_type_names[100],  13,  0,  -1,   0,   2, 7, 8, 6, 550, 15, 19, 6, 0, 0x0000, 15, 1, 0, 1, 0, 1, 0x0008, 0x0208, 0x0080, 0x0001, 0x0800, 0}, 
{&_unit_type_names[101],  15,  0,  -1,   0,   3, 10, 12, 8, 950, 20, 19, 6, 0, 0x0000, 18, 1, 0, 1, 0, 2, 0x0008, 0x4208, 0x0080, 0x0001, 0x0000, 0}, 
{&_unit_type_names[102],   7,  0,  -1,   0,   0, 3, 6, 4, 70, 2, 16, 6, 0, 0x0000, 8, 1, 0, 2, 0, 0, 0x0020, 0x0000, 0x0000, 0x0001, 0x0000, 0}, 
{&_unit_type_names[103],   2,  3,  37,   4,   1, 2, 8, 4, 100, 3, 16, 6, 0, 0x0000, 1, 1, 0, 4, 0, 0, 0x0028, 0x0200, 0x0000, 0x0001, 0x0000, 0}, 
{&_unit_type_names[104],   4,  0,  -1,   0,   1, 3, 7, 4, 275, 8, 16, 6, 0, 0x0000, 3, 1, 0, 4, 0, -3, 0x0008, 0x0200, 0x0000, 0x0001, 0x0080, 0}, 
{&_unit_type_names[105],  15,  1, 103,   0,   1, 4, 7, 4, 325, 7, 16, 6, 0, 0x0000, 30, 1, 0, 1, 0, -1, 0x0000, 0x0000, 0x0000, 0x0001, 0x0000, 0}, 
{&_unit_type_names[106],   4,  0,  -1,   0,   1, 3, 7, 4, 200, 4, 16, 6, 0, 0x0000, 10, 1, 0, 2, 0, 4, 0x0000, 0x0000, 0x0010, 0x0001, 0x0004, 0}, 
{&_unit_type_names[107],  15, 15,  10,   2,   2, 8, 9, 4, 450, 9, 16, 6, 0, 0x0000, 20, 1, 0, 1, 0, 0, 0x0040, 0x0082, 0x0000, 0x0009, 0x0000, 0}, 
{&_unit_type_names[108],  20, 20,  10,   2,   3, 10, 15, 4, 800, 17, 16, 6, 0, 0x0000, 30, 1, 0, 1, 0, 0, 0x0000, 0x0082, 0x0000, 0x0009, 0x0002, 0}, 
{&_unit_type_names[109],   8,  1, 103,   0,   2, 7, 9, 4, 599, 15, 16, 6, 0, 0x0000, 9, 1, 0, 4, 0, -2, 0x0008, 0x0200, 0x0000, 0x0001, 0x0000, 0}, 
{&_unit_type_names[110],  25,  0,  -1,   0,   1, 4, 8, 2, 160, 7, 16, 6, 0, 0x0000, 30, 1, 0, 1, 0, 0, 0x0000, 0x0082, 0x0000, 0x0009, 0x0000, 0}, 
{&_unit_type_names[111],  25,  0,  -1,   0,   2, 9, 10, 4, 700, 15, 16, 6, 0, 0x0000, 45, 1, 0, 1, 0, 0, 0x0000, 0x0000, 0x0000, 0x0001, 0x0000, 0}, 
{&_unit_type_names[112],  25,  0,  -1,   0,   3, 12, 12, 6, 1000, 20, 16, 6, 0, 0x0000, 45, 1, 0, 1, 0, 15, 0x0080, 0x0000, 0x0000, 0x0001, 0x0004, 0}, 
{&_unit_type_names[113],   0,  0,  -1,   0,   0, 0, 10, 4, 50, 5, 17, 6, 0, 0x0000, 45, 1, 8, 1, 0, 0, 0x0002, 0x00C2, 0x0000, 0x0001, 0x0000, 0}, 
{&_unit_type_names[114],  18,  0,  -1,   0,   1, 0, 8, 4, 225, 4, 17, 6, 0, 0x0000, 20, 1, 0, 1, 0, 0, 0x0000, 0x00C2, 0x0000, 0x0801, 0x0040, 0}, 
{&_unit_type_names[115],   3,  0,  -1,   0,   0, 0, 6, 2, 20, 2, 17, 6, 0, 0x0000, 1, 1, 0, 6, 0, 0, 0x0000, 0x00C2, 0x0000, 0x0801, 0x0040, 0}, 
{&_unit_type_names[116],  12, 10,  30,   4,   2, 7, 9, 4, 500, 10, 17, 6, 0, 0x0000, 20, 1, 0, 1, 0, 0, 0x0000, 0x0000, 0x0000, 0x0009, 0x0001, 0}, 
{&_unit_type_names[117],  15,  0,  -1,   0,   1, 8, 9, 10, 170, 8, 17, 6, 0, 0x0000, 10, 1, 0, 1, 0, 0, 0x0008, 0x0382, 0x0000, 0x0041, 0x0000, 0}, 
{&_unit_type_names[118],  15,  8,  34,   0,   3, 8, 10, 6, 650, 17, 17, 6, 0, 0x0000, 20, 1, 0, 1, 0, 0, 0x0018, 0x2200, 0x0000, 0x1001, 0x0000, 0}, 
{&_unit_type_names[119],  20, 20, 102,   0,   3, 10, 14, 8, 1000, 25, 17, 6, 0, 0x0000, 25, 1, 0, 1, 0, 0, 0x0008, 0x0228, 0x0000, 0x0001, 0x0000, 0}, 
{&_unit_type_names[119],   4,  0,  -1,   0,   1, 3, 7, 2, 120, 2, 17, 6, 0, 0x0000, 6, 1, 0, 2, 0, 4, 0x0004, 0x0000, 0x0000, 0x0001, 0x0006, 0}, 
};
