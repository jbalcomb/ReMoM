#ifndef MOM_DEF_H
#define MOM_DEF_H




/*
    Experience Level
*/
/*
    Normal Unit
        Experience Level    Ring Symbol
        Recruit             1 Silver
        Regular             2 Silver
        Veteran             3 Silver
        Elite               1 Gold
        Ulta-Elite          2 Gold
        Champion
    Hero Unit
        Hero
        Myrmidon            1 Silver
        Captain             2 Silver
        Commander           3 Silver
        Champion            1 Gold
        Lord                2 Gold
        Grand Lord          3 Gold
        Super Hero          1 Red
        Demi-God            2 Red
*/



/*

    Manifest Constants for Master of Magic (~v1.31)

        Specific to MoM, not MoX (SimTex *Game-Engine*)

*/

#define NUM_MAGIC_TYPES        5

// MoO2: _NUM_SHIPS, _NUM_COLONIES, _NUM_PLAYERS, _NUM_STARS, _NUM_NEBULAS
#define PLANE_COUNT_MAX             2
#define PLAYER_COUNT_MAX            6
#define NUM_DIFFICULTY_LEVEL        5   /* Intro, Easy, Normal, Hard, Impossible */
#define NUM_UNIT_ENCHANTMENTS      32
#define NUM_CITY_ENCHANTMENTS      26   // ¿ wha'hapen? CTY_EnchantsUpkeep() has 26, but s_CITY has 25 ? Includes Nightshade!
#define NUM_OVERLAND_ENCHANTMENTS  24
#define NUM_BUILDINGS              36
#define NUM_RESOURCE_TYPES          5
#define CITY_COUNT_MAX            100
#define UNIT_COUNT_MAX           1009
#define TOWER_COUNT_MAX             6
#define FORTRESS_COUNT_MAX          6
// #define NODE_COUNT_MAX          30
// #define LAIR_COUNT_MAX         102
#define NUM_PLANES                  2
#define NUM_PLAYERS                 6
#define NUM_CITIES                100
#define NUM_UNITS                1009
#define NUM_HEROES                  6   // "Hero Slots"
#define NUM_TOWERS                  6
#define NUM_FORTRESSES              6
#define NUM_NODES                  30
#define NUM_LAIRS                 102
#define MAX_STACK                   9

#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   200

#define SCREEN_XMIN       0
#define SCREEN_YMIN       0
#define SCREEN_XMAX     319
#define SCREEN_YMAX     199

#define CURSOR_WIDTH    16
#define CURSOR_HEIGHT   16



// Banner Color Remapping
// 5 Banners (banner_idx)
// Units - 5 color indices
// 214 + {0,1,2,3,4} = {214,215,216,218,219}
// Different than array remap colors for other things - e.g., Towers(Owned,Unowned)
//     Draw_Unit_StatFig()
//         FLIC_Remap_Color(UnitDraw_WorkArea, itr_color_remap + 214, COL_Banners[ ((banner_idx * 5) + itr_color_remap) ]);

//     Draw_Unit_StatFig()
//         if(UNIT_HasInvisibility(unit_idx) == ST_TRUE)
//             Replace_Color_All(UnitDraw_WorkArea, 233);
//             Outline_Picture_Pixels(UnitDraw_WorkArea, 1);

// MainScr_Maps.C
// reduced_map_box_color = 13;
// MoX_DEF.H
// #define MINIMAP_MARK_COLOR 15

// LILWIZ.LBX
// WIZARDS.LBX
// ¿ order of wizards ?
// ¿ order of colors ?
/*
LILWIZ.LBX
    blue
    green
    purple
    red
    yellow
*/
/*
LILWIZ.LBX
    MERLE2
    SHAMAN2
    VOODOO2
    EASTERN2
    ARAB2
    WHITEH2
    REDEYES2
    LIZARD2
    BARON2
    BLOND2
    GALEN2
    HEDBAND2
    AZTEC2
    WITCH2
*/
/*
WIZARDS.LBX
    MERLIN
    SHAMAN
    PRIESTES
    WU
    ARAB
    OBERIC
    WRAITH
    DRACONIA
    EVILWIZ
    FREYA
    GALE
    ARIEL
    AZTEC
    KARLA
*/


/*
    World Map Dimensions
*/
// #define WMAP_WIDTH      60  // in Squares
// #define WMAP_HEIGHT     40  // in Squares
/*
    Main Screen Movement Map Dimensions
*/
#define MMAP_WIDTH      12  // in Squares
#define MMAP_HEIGHT     10  // in Squares
/*
    Main Screen Reduced Map Dimensions
*/
#define RMAP_WIDTH      58  // in Squares
#define RMAP_HEIGHT     30  // in Squares
/*
    World / Movement-Map Square Dimensions
*/
#define SQUARE_WIDTH    20  // in Pixels
#define SQUARE_HEIGHT   18  // in Pixels
#define TILE_WIDTH      20
#define TILE_HEIGHT     18

#define WORLD_WIDTH         60  // in Squares
#define WORLD_HEIGHT        40  // in Squares

#define _MAP_MAX_X      60  // MoO2
#define _MAP_MAX_Y      40  // MoO2

#define WORLD_SIZE          2400
#define WORLD_SIZE_DB       2400  // 1-byte values
#define WORLD_SIZE_DW       4800  // 2-byte values

#define MAP_WIDTH           12  // in Squares
#define MAP_HEIGHT          10  // in Squares
#define MAP_SIZE           120  // in Squares

#define MAP_WIDTH_HALF       6  // in Squares
#define MAP_HEIGHT_HALF      5  // in Squares

#define MAP_SCREEN_X                  0  // in Pixels
#define MAP_SCREEN_Y                 20  // in Pixels

#define REDUCED_MAP_SCREEN_X        251  // in Pixels
#define REDUCED_MAP_SCREEN_Y         21  // in Pixels
#define REDUCED_MAP_SQUARE_WIDTH      1  // in Pixels
#define REDUCED_MAP_SQUARE_HEIGHT     1  // in Pixels

#define WORLD_X_MIN          0  // in Squares
// #define WORLD_X_MAX         59  // in Squares
#define WORLD_X_MAX         (WORLD_WIDTH - 1)  // in Squares
#define WORLD_Y_MIN          0  // in Squares
// #define WORLD_Y_MAX         39  // in Squares
#define WORLD_Y_MAX         (WORLD_HEIGHT - 1)  // in Squares

#define MAP_X_MIN          0  // in Squares
#define MAP_X_MAX         (MAP_WIDTH - 1)  // in Squares
#define MAP_Y_MIN          0  // in Squares
#define MAP_Y_MAX         (MAP_HEIGHT - 1)  // in Squares

// {}
#define MAP_SQUARE_SCREEN_X_MIN          0  // in Pixels
// #define MAP_SQUARE_SCREEN_X_MAX        300  // in Pixels
#define MAP_SQUARE_SCREEN_X_MAX        (MAP_SCREEN_X + ((MAP_WIDTH - 1) * SQUARE_WIDTH))  // in Pixels
#define MAP_SQUARE_SCREEN_Y_MIN          0  // in Pixels
// #define MAP_SQUARE_SCREEN_Y_MAX        180  // in Pixels
#define MAP_SQUARE_SCREEN_Y_MAX        (MAP_SCREEN_Y + ((MAP_HEIGHT - 1) * SQUARE_HEIGHT))   // in Pixels



#define MOVEMENT_MAP_SCREEN_X         0
#define MOVEMENT_MAP_SCREEN_Y        20
#define REDUCED_MAP_SCREEN_X        251
#define REDUCED_MAP_SCREEN_Y         21

#define REDUCED_MAP_X   251
#define REDUCED_MAP_Y    21
#define REDUCED_MAP_W    58
#define REDUCED_MAP_H    30

#define ARMYLIST_REDUCED_MAP_X    86
#define ARMYLIST_REDUCED_MAP_Y   164
#define ARMYLIST_REDUCED_MAP_W    49
#define ARMYLIST_REDUCED_MAP_H    33

#define CITYLIST_REDUCED_MAP_X    42
#define CITYLIST_REDUCED_MAP_Y   162
#define CITYLIST_REDUCED_MAP_W    49
#define CITYLIST_REDUCED_MAP_H    33


#define NUM_TERRAIN_TYPES   762

#define STATFIG_WIDTH       18  // in Pixels
#define STATFIG_HEIGHT      16  // in Pixels

/*
hanging out in MoX_DEF.H


#define HUMAN_PLAYER_IDX 0
#define NEUTRAL_PLAYER_IDX 5

// World (Grid) Dimensions, in Map Sqaures
#define WORLD_WIDTH 60
#define WORLD_HEIGHT 40
#define MOVEMENT_MAP_WIDTH 12
#define MOVEMENT_MAP_HEIGHT 10
#define REDUCED_MAP_WIDTH 58
#define REDUCED_MAP_HEIGTH 30

#define PLAYER_COUNT_MAX 6
#define CITY_COUNT_MAX 100
#define UNIT_COUNT_MAX 1009
#define TOWER_COUNT_MAX 6
#define FORTRESS_COUNT_MAX 6

#define MINIMAP_MARK_COLOR 15

*/

/*
    Worlds / Planes
*/
#define ARCANUS_PLANE  0
#define MYRROR_PLANE   1


/*
1-byte
0b00000000  0x0     0
0b00000001  0x1     1
0b00000010  0x2     2
0b00000100  0x4     4
0b00001000  0x8     8
0b00010000  0x10   16
0b00100000  0x20   32
0b01000000  0x40   64
0b10000000  0x80  128
2-byte
0b0000000000000000  0x0000      0
0b0000000000000001  0x0001      1
0b0000000000000010  0x0002      2
0b0000000000000100  0x0004      4
0b0000000000001000  0x0008      8
0b0000000000010000  0x0010     16
0b0000000000100000  0x0020     32
0b0000000001000000  0x0040     64
0b0000000010000000  0x0080    128
0b0000000100000000  0x0100    256
0b0000001000000000  0x0200    512
0b0000010000000000  0x0400   1024
0b0000100000000000  0x0800   2048
0b0001000000000000  0x1000   4096
0b0010000000000000  0x2000   8192
0b0100000000000000  0x4000  16384
0b1000000000000000  0x8000  32768
*/



/*
    Unit Action / Status / State
    - per 'MoM Demo'

    ¿ vs. enum Unit_Status {} in MoX_Data.H ?

    ¿ "UA" for "Unit Action" ?
    ¿ "US" for "Unit Status" ?

PS.  'MoM Demo' looks to have an array of strings for the defines / enum
*/
#define NO_ORDERS       0x00
#define PATROL          0x01
#define BUILD           0x02    // "BUILD", as in "Build Road"
#define GOTO            0x03
#define DONE            0x04
#define WAIT            0x05
#define CASTING         0x06
#define PURIFY          0x07
#define MELD            0x08
#define BUILD_OUTPOST   0x09    // ¿ "Build Outpost", like MoO1, rather than "Settle", for "Settlers" ?
#define WEIGHT_ANCHOR   0x0A
#define BOAT_RIDER      0x0B

/*
    Unit Type - Movement Type
    s_UNIT_TYPE.Move_Flags
    enum MOVEFLAGS
    data and code looks like a bitfield

    UnitView & UnitList
        Movement Type |-> Movement Type Icon  UNITVIEW.LBX, {11,12,13}
        can't handle multiples - only tests 1 bit, in-order

PS. 'MoM Demo' has "Forest Walk", "Mountain Walk", "Pathfinding"
*/
// 0b00000000  0x00      0
#define MV_WALKING           0x00      // 0b00000000  0x00      0
#define MV_CAVALRY           0x01      // 0b00000001  0x01      1
#define MV_SAILING           0x02      // 0b00000010  0x02      2
#define MV_SWIMMING          0x04      // 0b00000100  0x04      4
#define MV_FLYING            0x08      // 0b00001000  0x08      8
#define MV_TELEPORT          0x10      // 0b00010000  0x10     16
#define MV_FORESTER          0x20      // 0b00100000  0x20     32
#define MV_MOUNTAINEER       0x40      // 0b01000000  0x40     64
#define MV_MERGING           0x80      // 0b10000000  0x80    128

/*
    Movement Modes
*/
// 0b00000000  0x00      0
#define CAVALRY             0x0001      // 0b00000001  0x01      1
#define SAILING             0x0002      // 0b00000010  0x02      2
#define SWIMMING            0x0004      // 0b00000100  0x04      4
#define FLYING              0x0008      // 0b00001000  0x08      8
#define TELEPORTING         0x0010      // 0b00010000  0x10     16
#define FORESTER            0x0020      // 0b00100000  0x20     32
#define MOUNTAINEER         0x0040      // 0b01000000  0x40     64
// 0b10000000  0x80    128

// "Planar Travel" -000A Move_PlaneShift dw ?
// "Wind Walking"  -0008 Move_WindWalk dw ?
// "walking"       -0006 Move_Ground dw ?
/* ¿ array / enum offsets ?*/
// #define OFS_CAVALRY          0x00
// #define OFS_FORESTER         0x02
// #define OFS_MOUNTAINEER      0x04
// #define OFS_SWIMMING         0x06
// #define OFS_SAILING          0x08
// #define OFS_FLYING           0x0A
// #define OFS_PLANARTRAVEL     0x0C
// #define OFS_WALKING          0x0E
#define OFS_CAVALRY          1
#define OFS_FORESTER         2
#define OFS_MOUNTAINEER      3
#define OFS_SWIMMING         4
#define OFS_SAILING          5
#define OFS_FLYING           6
#define OFS_PLANARTRAVEL     7
#define OFS_WALKING          8

// MM: Movement Mode
#define MM_CAVALRY          0
#define MM_FORESTER         1
#define MM_MOUNTAINEER      2
#define MM_SWIMMING         3
#define MM_SAILING          4
#define MM_FLYING           5
#define MM_PLANARTRAVEL     6
#define MM_WALKING          7





/*
    Buildings
*/
// drake178: struc BLD_LIST
#define NONE                0x00
#define HOUSING             0x01
#define TRADE_GOODS         0x02
#define BARRACKS            0x03
#define ARMORY              0x04
#define FIGHTERS_GUILD      0x05
#define ARMORERS_GUILD      0x06
#define WAR_COLLEGE         0x07
#define SMITHY              0x08
#define STABLE              0x09
#define ANIMISTS_GUILD      0x0A
#define FANTASTIC_STABLE    0x0B
#define SHIPWRIGHT_GUILD    0x0C
#define SHIP_YARD           0x0D
#define MARITIME_GUILD      0x0E
#define SAWMILL             0x0F
#define LIBRARY             0x10
#define SAGES_GUILD         0x11
#define ORACLE              0x12
#define ALCHEMISTS_GUILD    0x13
#define UNIVERSITY          0x14
#define WIZARDS_GUILD       0x15
#define SHRINE              0x16
#define TEMPLE              0x17
#define PARTHENON           0x18
#define CATHEDRAL           0x19
#define MARKETPLACE         0x1A
#define BANK                0x1B
#define MERCHANTS_GUILD     0x1C
#define GRANARY             0x1D
#define FARMERS_MARKET      0x1E
#define FORESTERS_GUILD     0x1F
#define BUILDERS_HALL       0x20
#define MECHANICIANS_GUILD  0x21
#define MINERS_GUILD        0x22
#define CITY_WALLS          0x23






/*
    Unit Abilities
*/
// 0b0000000000000000  0x0000      0
// 0b0000000000000001  0x0001      1
// 0b0000000000000010  0x0002      2
#define UA_PLANARTRAVEL     0x04        // 0b0000000000000100  0x0004      4
// 0b0000000000001000  0x0008      8
// 0b0000000000010000  0x0010     16
// 0b0000000000100000  0x0020     32
#define UA_INVISIBILITY     0x40        // 0b0000000001000000  0x0040     64
// 0b0000000010000000  0x0080    128
// 0b0000000100000000  0x0100    256
// 0b0000001000000000  0x0200    512
// 0b0000010000000000  0x0400   1024
// 0b0000100000000000  0x0800   2048
#define UA_WINDWALKING      0x1000      // 0b0001000000000000  0x1000   4096
// 0b0010000000000000  0x2000   8192
// 0b0100000000000000  0x4000  16384
// 0b1000000000000000  0x8000  32768


/*
    Unit Enchantments
*/
// 0b0000000000000000  0x0000      0
#define UE_WINDWALKING      0x0001      // 0b0000000000000001  0x0001      1
#define UE_FLIGHT           0x0002      // 0b0000000000000010  0x0002      2
// 0b0000000000000100  0x0004      4
// 0b0000000000001000  0x0008      8
// 0b0000000000010000  0x0010     16
// 0b0000000000100000  0x0020     32
// 0b0000000001000000  0x0040     64
// 0b0000000010000000  0x0080    128
#define UE_WATERWALKING     0x0100      // 0b0000000100000000  0x0100    256
// 0b0000001000000000  0x0200    512
// 0b0000010000000000  0x0400   1024
// 0b0000100000000000  0x0800   2048
#define UE_PLANARTRAVEL     0x1000      // 0b0001000000000000  0x1000   4096
// 0b0010000000000000  0x2000   8192
// 0b0100000000000000  0x4000  16384
#define UE_INVISIBILITY     0x8000      // 0b1000000000000000  0x8000  32768


/*
    Unit Mutations
*/
// 00000003 ; enum MUT_FLAGS (bitfield)
// 00000003 WQ_MASK        = 3
// 00000003 Wp_Magic  = 1
// 00000003 Wp_Mithril  = 2
// 00000003 Wp_Adamantium  = 3
// 00000004 CC_Defense  = 4                         ; Chaos Channels - demon skin armor
// 00000008 CC_Flight  = 8                          ; Chaos Channels - demon wings
// 00000010 CC_Breath  = 10h                        ; Chaos Channels - fire breathing
// 00000020 R_Undead  = 20h
// 00000040 C_StasisInit  = 40h
// 00000080 C_StasisLinger  = 80h
// 0b00000000  0x00      0
// 0b00000001  0x01      1
// 0b00000010  0x02      2
// 0b00000100  0x04      4
#define CC_Flight           0x08    // 0b00001000  0x08      8
// 0b00010000  0x10     16
// 0b00100000  0x20     32
// 0b01000000  0x40     64
// 0b10000000  0x80    128


/*
    Item Enchantments / Attributes / Effects
*/



/*
    Spells
*/

/*
// ¿ enum ? ¿ seo_ for Spell - Enchantment - Overland; oe_ for Overland Enchantment vs. ue_ and ce_ for Unit and City ?
¿ spaces ?
¿ articles ?
¿ as seen in the LBX's ?

SPL_
SPL_UE_
SPL_CE_
SPL_OE_

*/

/*

struc s_SPELL_DATA
00000020 Param0 db ?                             ; unit type, base damage, UE flag, or CE index
*/

#define SPL_HYDRA 111
/*
; enum Spells
No_spell  = 0
Earth_to_Mud  = 1
Resist_Elements  = 2
Wall_of_Stone  = 3
Giant_Strength  = 4
Web  = 5
War_Bears  = 6
Stone_Skin  = 7
Water_Walking  = 8
Sprites  = 9
Earth_Lore  = 10
Cracks_Call  = 11
Natures_eye  = 12
Ice_Bolt  = 13
Giant_Spiders  = 14
Change_Terrain  = 15
Path_Finding  = 16
Cockatrices  = 17
Transmute  = 18
Natures_Cures  = 19
Basilisk  = 20
Elemental_Armor  = 21
Petrify  = 22
Stone_Giant  = 23
Iron_Skin  = 24
Ice_Storm  = 25
Earthquake  = 26
Gorgons  = 27
Move_Fortress  = 28
Gaias_Blessing  = 29
Earth_Elemental  = 30
Regeneration  = 31
Behemoth  = 32
Entangle  = 33
Nature_Awareness  = 34
Call_Lightning  = 35
Colossus  = 36
Earth_Gate  = 37
Herb_Mastery  = 38
Great_Wyrm  = 39
Natures_Wrath  = 40
Resist_Magic  = 41
Dispel_Magic_True  = 42
Floating_Island  = 43
Guardian_Wind  = 44
Phantom_Warriors  = 45
Confusion  = 46
Word_of_Recall  = 47
Counter_Magic  = 48
Nagas  = 49
Psionic_Blast  = 50
Blur  = 51
Disenchant_True  = 52
Vertigo  = 53
Spell_Lock  = 54
Enchant_Road  = 55
Flight  = 56
Wind_Mastery  = 57
Spell_Blast  = 58
Aura_of_Majesty  = 59
Phantom_Beast  = 60
Disjunction_True  = 61
Invisibility  = 62
Wind_Walking  = 63
Banish  = 64
Storm_Giant  = 65
Air_Elemental  = 66
Mind_Storm  = 67
Stasis  = 68
Magic_Immunity  = 69
Haste  = 70
Djinn  = 71
Spell_Ward  = 72
Creature_Binding  = 73
Mass_Invisibility  = 74
Great_Unsummoning  = 75
Spell_Binding  = 76
Flying_Fortress  = 77
Sky_Drake  = 78
Suppress_Magic  = 79
Time_Stop  = 80
Warp_Wood  = 81
Disrupt  = 82
Fire_Bolt  = 83
Hell_Hounds  = 84
Corruption  = 85
Eldritch_Weapon  = 86
Wall_of_Fire  = 87
Shatter  = 88
Warp_Creature  = 89
Fire_Elemental  = 90
Lightning_Bolt  = 91
Fire_Giant  = 92
Chaos_Channels  = 93
Flame_Blade  = 94
Gargoyles  = 95
Fireball  = 96
Doombat  = 97
Raise_Volcano  = 98
Immolation  = 99
Chimeras  = 100
Warp_Lightning  = 101
Metal_Fires  = 102
Chaos_Spawn  = 103
Doom_Bolt  = 104
Magic_Vortex  = 105
Efreet  = 106
Fire_Storm  = 107
Warp_Reality  = 108
Flame_Strike  = 109
Chaos_Rift  = 110
Hydra  = 111
Disintegrate  = 112
Meteor_Storm  = 113
Great_Wasting  = 114
Call_Chaos  = 115
Chaos_Surge  = 116
Doom_Mastery  = 117
Great_Drake  = 118
Call_The_Void  = 119
Armageddon  = 120
Bless  = 121
Star_Fires  = 122
Endurance  = 123
Holy_Weapon  = 124
Healing  = 125
Holy_Armor  = 126
Just_Cause  = 127
True_Light  = 128
Guardian_Spirit  = 129
Heroism  = 130
True_Sight  = 131
Plane_Shift  = 132
Resurrection  = 133
Dispel_Evil  = 134
Planar_Seal  = 135
Unicorns  = 136
Raise_Dead  = 137
Planar_Travel  = 138
Heavenly_Light  = 139
Prayer  = 140
Lionheart  = 141
Incarnation  = 142
Invulnerability  = 143
Righteousness  = 144
Prosperity  = 145
Altar_of_Battle  = 146
Angel  = 147
Stream_of_Life  = 148
Mass_Healing  = 149
Holy_Word  = 150
High_Prayer  = 151
Inspirations  = 152
Astral_Gate  = 153
Holy_Arms  = 154
Consecration  = 155
Life_Force  = 156
Tranquility  = 157
Crusade  = 158
Arch_Angel  = 159
Charm_of_Life  = 160
Skeletons  = 161
Weakness  = 162
Dark_Rituals  = 163
Cloak_of_Fear  = 164
Black_Sleep  = 165
Ghouls  = 166
Life_Drain  = 167
Terror  = 168
Darkness  = 169
Mana_Leak  = 170
Drain_Power  = 171
Possession  = 172
Lycantrophy  = 173
Black_Prayer  = 174
Black_Channels  = 175
Night_Stalker  = 176
Subversion  = 177
Wall_of_Darkness  = 178
Berserk  = 179
Shadow_Demons  = 180
Wraith_Form  = 181
Wrack  = 182
Evil_Presence  = 183
Wraiths  = 184
Cloud_of_Shadow  = 185
Warp_Node  = 186
Black_Wind  = 187
Zombie_Mastery  = 188
Famine  = 189
Cursed_Lands  = 190
Cruel_Unminding  = 191
Word_of_Death  = 192
Death_Knights  = 193
Death_Spell  = 194
Animate_Dead  = 195
Pestilence  = 196
Eternal_Night  = 197
Evil_Omens  = 198
Death_Wish  = 199
Demon_Lord  = 200
Magic_Spirit  = 201
Dispel_Magic  = 202
Summoning_Circle  = 203
Disenchant_Area  = 204
Recall_Hero  = 205
Detect_Magic  = 206
Enchant_Item  = 207
Summon_Hero  = 208
Awareness  = 209
Disjunction  = 210
Create_Artifact  = 211
Summon_Champion  = 212
Spell_Of_Mastery  = 213
Spell_Of_Return  = 214
*/


/*
    Overland Enchantments
*/

#define ETERNAL_NIGHT       0x00
#define EVIL_OMENS          0x01
#define ZOMBIE_MASTERY      0x02
#define AURA_OF_MAJESTY     0x03
#define WIND_MASTERY        0x04
#define SUPPRESS_MAGIC      0x05
#define TIME_STOP           0x06
#define NATURE_AWARENESS    0x07
#define NATURES_WRATH       0x08
#define HERB_MASTERY        0x09
#define CHAOS_SURGE         0x0A
#define DOOM_MASTERY        0x0B
#define GREAT_WASTING       0x0C
#define METEOR_STORM        0x0D
#define ARMAGEDDON          0x0E
#define TRANQUILITY         0x0F
#define LIFE_FORCE          0x10
#define CRUSADE             0x11
#define JUST_CAUSE          0x12
#define HOLY_ARMS           0x13
#define PLANAR_SEAL         0x14
#define CHARM_OF_LIFE       0x15
#define DETECT_MAGIC        0x16
#define AWARENESS           0x17

/*
    City Enchantments
*/
#define WALL_OF_FIRE        0x00
#define CHAOS_RIFT          0x01
#define DARK_RITUALS        0x02
#define EVIL_PRESENCE       0x03
#define CURSED_LANDS        0x04
#define PESTILENCE          0x05
#define CLOUD_OF_SHADOW     0x06
#define FAMINE              0x07
#define FLYING_FORTRESS     0x08
#define NATURE_WARD         0x09
#define SORCERY_WARD        0x0A
#define CHAOS_WARD          0x0B
#define LIFE_WARD           0x0C
#define DEATH_WARD          0x0D
#define NATURES_EYE         0x0E
#define EARTH_GATE          0x0F
#define STREAM_OF_LIFE      0x10
#define GAIAS_BLESSING      0x11
#define INSPIRATIONS        0x12
#define PROSPERITY          0x13
#define ASTRAL_GATE         0x14
#define HEAVENLY_LIGHT      0x15
#define CONSECRATION        0x16
#define WALL_OF_DARKNESS    0x17
#define ALTAR_OF_BATTLE     0x18



/*
    Screen Layout Variables
*/

/*
    Army List Screen
*/
#define NUM_ARMY_LIST   6
#define NUM_HERO_PORTRAIT   6
/*
    City List Screen
*/
#define NUM_CITY_LIST   9

/*
    Main Screen
        Game Buttons
        Movement Map
        Reduced Map
        Status Window
        Summary Window


Status Window
    Print_Integer_Right(265, 68, _players[_human_player_idx].gold_reserve);
    Print_Integer_Right(303, 68, _players[_human_player_idx].mana_reserve);
    Print(268, 68, cstr_GP);
    Print(306, 68, cstr_MP);

Summary Window
    FLIC_Draw(240, 76, deselect_background);
    Print_Integer_Right(273, 101, gold);
    Print(278, 101, cstr_Gold);
    Print_Integer_Right(273, 133, food);
    Print(278, 133, cstr_Food);
    Print_Integer_Right(273, 165, mana);
    Print(278, 165, cstr_Mana);
    Print_Centered(280, 157, cstr_Conjunction);
    Print_Centered(280, 158, cstr_Conjunction);
    Print_Centered(280, 158, cstr_Conjunction);
    Print_Centered(280, 158, cstr_Bad_Moon);
    Print_Centered(280, 158, cstr_Good_Moon);
    Print_Centered(280, 158, cstr_Mana_Short);

Next Turn Button
    FLIC_Draw(240, 173, next_turn_background_seg);
    FLIC_Draw(246, 178, next_turn_button_seg);  // ? offset by 6,5 to support the 3-D illusion of being depressed

Main_Screen_Draw_Unit_Action_Buttons()
    push    [main_done_button]      
    mov     ax, 176
    mov     ax, 246
    push    [main_patrol_button]    
    mov     ax, 176
    mov     ax, 280
    push    [main_wait_button]      
    mov     ax, 186
    mov     ax, 246

    switch(special_action_flag)
    push    [main_purify_button]    
    mov     ax, 186
    mov     ax, 280
    push    [main_meld_button]      
    mov     ax, 186
    mov     ax, 280
    push    [main_build_button]     
    mov     ax, 186
    mov     ax, 280
    push    [main_lock_build_button]
    mov     ax, 186
    mov     ax, 280

    push    [main_lock_done_button]
    mov     ax, 176
    mov     ax, 246
    push    [main_lock_patrol_button]
    mov     ax, 176
    mov     ax, 280
    push    [main_lock_build_button]
    mov     ax, 186
    mov     ax, 280
    push    [main_wait_button]      
    mov     ax, 186
    mov     ax, 246
*/



#endif /* MOM_DEF_H */
