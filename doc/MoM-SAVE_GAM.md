

6 words (12 bytes)
Far Pointer
fread() as 35 records of size 12
...420 bytes total  (420 / 16 = 26.25 PR)
...for 6 wizards = 2520 bytes

00000000 struc HERO_REC ; (sizeof=0xC, standard type)
00000000 Level dw ?
00000002 Abilities_LO dw ?                       ; enum Hero_Ability_LO
00000004 Abilities_HO dw ?                       ; enum Hero_Ability_HO
00000006 Casting_Skill db ?
00000007 Spells db 4 dup(?)
0000000B Unused_0Bh db ?

experience_level
{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }
["Hero", "Myrmidon", "Captain", "Commander", "Champion", "Lord", "Grand Lord", "Super Hero", "Demi-God"]


LOAD__p0_heroes()
{
    FILE * file_pointer;
    file_name[] = "SAVETEST.GAM";
    file_pointer = fopen(file_name, "rb");
    fseek(file_pointer, 0, SEEK_SET);
    fread(p0_heroes, 12, 35, file_pointer);
    fclose(file_pointer);
}




p0_heroes
p1_heroes
p2_heroes
p3_heroes
p4_heroes
p5_heroes
_num_players
_landsize
_magic
_difficulty
_cities
_units
_turn
_unit
_players
_world_maps
UU_TBL_1
UU_TBL_2
TBL_Landmasses
TBL_Nodes
_FORTRESSES
_TOWERS
TBL_Lairs
TBL_Items
_CITIES
_UNITS
TBL_Terr_Specials
TBL_Scouting
TBL_MoveMaps_EMS
_events_table
_map_square_flags
grand_vizier
TBL_Premade_Items
TBL_Hero_Names




Game Options
Game Settings
Wizards
Retorts
Spells
Items
Heroes
Cities
Nodes
Fortresses




"Overland"
"Terrain"
"Arcanus Plane"
"The plane button toggles between the Prime and Myrror planes."
"unit"
"switch planes"
"current plane", "next [plane]"
"Node Owners"
"squares"
"CITY VIEW" "Ifa nearby city and this city control the same square, each city receives only half of the square's value."


map is 40 x 60, 2400 tiles
plane: {arcanus,myrror}




MGC
dseg:65AC  p0_heroes
dseg:65B0  p1_heroes
dseg:65B4  p2_heroes
dseg:65B8  p3_heroes
dseg:65BC  p4_heroes
dseg:65C0  p5_heroes


?
MoO2 _global_data_seg
...
    main()
        Allocate_Data_Space_()
            _global_screen_seg = Allocate_Space(1790000)
            ...
            _global_data_seg = Allocate_Dos_Space(10240)
?


MGC
dseg:6756
World_Data
MGC
dseg:89D2
TBL_Cities




MGC o57
    MoM_Init_Tables()
        ...
        tmp_World_Data_Paras = 3519;  // 3519 paragraphs = 56304 bytes
        World_Data = Allocate_Space(tmp_World_Data_Paras);
        ...
        TBL_Cities = Allocate_First_Block(World_Data, 714);



WZD o152
    MoM_Init_Tables()
        ...
        tmp_World_Data_Paras = 3519;  // 3519 paragraphs = 56304 bytes
        World_Data = Allocate_Space(tmp_World_Data_Paras);
        ...
        TBL_Cities = Allocate_First_Block(World_Data, 714);





Save_SAVE_GAM()
Load_SAVE_GAM()

420 + 420 + 420 + 420 + 420 + 420 + 420 + 2 + 2 + 2 + 2 + 2 + 2 + 2 + 2 + 7344 + 9600 + 192 + 192 + 4800 + 1440 + 24 + 24 + 2448 + 6900 + 4800 + 4800 + 28800 + 100 + 4800 + 2 + 250 + 560
80032

*p0_heroes = read (35 *   12) =  420    420
*p1_heroes = read (35 *   12) =  420    840
*p2_heroes = read (35 *   12) =  420   1260
*p3_heroes = read (35 *   12) =  420   1680
*p4_heroes = read (35 *   12) =  420   2100
*p5_heroes = read (35 *   12) =  420   2520
wizards    = read ( 1 *    2) =    2   2522
land_size  = read ( 1 *    2) =    2   2524
magic      = read ( 1 *    2) =    2   2526
difficulty = read ( 1 *    2) =    2   2528
cities     = read ( 1 *    2) =    2   2530
units      = read ( 1 *    2) =    2   2532
turn       = read ( 1 *    2) =    2   2534
unit       = read ( 1 *    2) =    2   2536
*p_wizards = read ( 6 * 1224) = 7344   9880
word_maps  = read ( 2 * 4800) = 9600  19480
 2 *  96 = 192
 2 *  96 = 192
2 * 2400 = 4800
30 * 48 = 1440
6 * 4 = 24
6 * 4 = 24
2448
6900
4800
4800
28800
100
4800
2
250
560

2 * 4800
2 * 96
2 * 96
2 * 2400
30 * 48
6 * 4
6 * 4
102 * 24
138 * 50
100 * 114
1009 * 32
2 * 2400
2 * 2400
2 * 14400
1 * 100
2 * 2400
grand_vizier  = read  1 * 2
premade_items = read 250 * 1
?
if (file_size != 57764) { flag = ST_TRUE }
    memcpy(, 545, 0)
?
read 35 * 16




Offset
0x00000000  Heroes  2520 bytes
0x000009D8  Game      16 bytes
0x000009E8  
0x00002698
0x00004c18
0x00004d98
0x00006058
0x000065f8
0x00006610
0x00006628
0x00006fb8
0x00008980
0x00008a48
0x00008a7a
0x00008aac
0x0000b734
0x00013554
0x00014814
0x00015ad4
0x0001cb54
0x0001cbb8
0x0001de78
0x0001de7a
0x0001df74
0x0001e1a4


Top level data blocks[]
Offset	Length	Quantity	Contents / Comments
0x000000	0x000c	35 × (5 + 1)	Hero stats (12 bytes × 35 heroes × 6 players)
0x0009d8	0x0010	1	Overall game data
0x0009e8	0x04c8	5 + 1	Wizards + "Neutral wizard" data
0x002698	0x12c0	2	Terrain maps
0x004c18	0x00c0	2	Unused maps
0x004d98	0x0960	2	Land mass IDs
0x006058	0x0030	30	Node attributes
0x0065f8	0x0004	6	Fortresses data
0x006610	0x0004	6	Towers data
0x006628	0x0018	99 + 3	Encounter zones data
0x006fb8	0x0032	132	Items (does not include items during their creation)
0x008980	0x0032	4	Items being created by the AI in reverse order (0x88-x)
0x008a48	0x0032	1	Item found or offered by a merchant
0x008a7a	0x0032	1	Item being created by Enchant Item or Create Artifact
0x008aac	0x0072	100	Cities
0x00b734	0x0020	1000 + 9	Units
0x013554	0x0960	2	Mineral maps
0x014814	0x0960	2	Player exploration maps
0x015ad4	0x0960	6 × 2	Precalculated half movements points cost maps
0x01cb54	0x0002	50	Events data
0x01cbb8	0x0960	2	Terrain flags maps
0x01de78	0x0002	1	Grand Vizier flag
0x01de7a	0x0001	250	Items in Game flags
0x01df74	0x0010	35	Chosen hero names
0x01e1a4			End of file (123,300 bytes)


0C
12 * 35 = 420
1A4
420 * 6 = 2520
9D8


Hero Stats
Each player has 35 heroes, heroes are stored wizard per wizard, so the offset of each hero in file is:

hero_offset = (hero_number + (wizard_number × 35)) × 0x000c
The heroes are in the same order as in this list. The data for each hero is:

Offset in Hero	Item length	Quantity	Contents / Comments
0x00	0x02	1	Level and Status (signed integer)
from	to	
0x0000	0x0008	hero level - 1
available hero

0xffec	-	recruited or killed by irreversible damage hero
0xfff7	0xffff	0 - hero level
dead hero

0x02	0x01	1	Hero ability bitmask
0x80	Blademaster*
0x40	Blademaster
0x10	Legendary*
0x08	Legendary
0x02	Leadership*
0x01	Leadership
0x03	0x01	1	Hero ability bitmask
0x80	Might
0x20	Constitution*
0x10	Constitution
0x04	Armsmaster*
0x02	Armsmaster
0x04	0x01	1	Hero ability bitmask
0x40	Sage*
0x20	Sage
0x08	Arcane Power*
0x04	Arcane Power
0x01	Might*
0x05	0x01	1	Hero ability or gender bitmask
0x80	Agility
0x40	Female
0x20	Noble
0x10	Charmed
0x08	Lucky
0x04	Agility*
0x02	Prayermaster*
0x01	Prayermaster
0x06	0x01	1	Casting skill
mp = (casting_skill + 1) × 2.5 × hero_level
(rounded down)
0x07	0x01	4	Spell number (0 is "None")
Due to a bug, probably because those bytes are read as signed bytes, hero won't receive his spell if its value exceeds 0x7f
0x0b	0x01	1	Unused



Overall game data
Offset	Offset	Length	Contents / Comments
in file
0x0009d8	0x00	0x02	Number of wizards including the player, from 2 to 5
0x0009da	0x02	0x02	Land Size setting
0	Small
1	Medium
2	Large
0x0009dc	0x04	0x02	Magic Intensity setting
0	Weak
1	Normal
2	Powerful
0x0009de	0x06	0x02	Difficulty Level setting
0	Intro
1	Easy
2	Normal
3	Hard
4	Impossible
0x0009e0	0x08	0x02	Number of cities, from 0 to 100
0x0009e2	0x0a	0x02	Number of units, from 0 to 1000 (signed integer)
0x0009e4	0x0c	0x02	Current turn in number of months (signed integer)
Turn 0 is January 1400
0x0009e6	0x0e	0x02	Top unit of selected group (signed integer)
Unused because reset at game loading



Wizard Data
The offset of each wizard in file is (given a wizard number between 0 and 5, the number 5 being the "neutral"):

wizard_offset = (wizard_number × 0x04c8) + 0x0009e8
Offset in Wizard Data	Item length	Quantity	Content/Comments
0x0000	0x01	1	Portrait
0x00	Merlin
0x01	Raven
0x02	Sharee
0x03	Lo Pan
0x04	Jafar
0x05	Oberic
0x06	Rjak
0x07	Sss'ra
0x08	Tauron
0x09	Freya
0x0a	Horus
0x0b	Ariel
0x0c	Tlaloc
0x0d	Kali







MGC
ovr48
o48p01 SAVEGAM_Save()
o48p02 SAVEGAM_Load()

WZD
ovr50
o50p01  SAVEGAM_Save()
o50p02  SAVEGAM_Load()


SAVE1.GAM
SAVE2.GAM
SAVE3.GAM
SAVE4.GAM
SAVE5.GAM
SAVE6.GAM
SAVE7.GAM
SAVE8.GAM
SAVE9.GAM

SAVE9.GAM is the auto-save
it is checked for to see if "Continue" should be on the Main Menu









WZD
Load Saved Game
XREF:
_main+2DB     call j_GAME_Load
sub_F2940+41D call j_GAME_Load



MGC
Save_SAVE_GAM

gfp_TBL_HeroStats_P0      35 *    12 =   420    420
gfp_TBL_HeroStats_P1      35 *    12 =   420    840
gfp_TBL_HeroStats_P2      35 *    12 =   420    1260
gfp_TBL_HeroStats_P3      35 *    12 =   420    1680
gfp_TBL_HeroStats_P4      35 *    12 =   420    2100
gfp_TBL_HeroStats_P5      35 *    12 =   420    2520
game_data.Total_Wizards    1 *     2 =     2    2522
game_data.Land_Size        1 *     2 =     2    2524
game_data.Magic            1 *     2 =     2    2526
game_data.Difficulty       1 *     2 =     2    2528
game_data.Total_Cities     1 *     2 =     2    2530
game_data.Total_Units      1 *     2 =     2    2532
game_data.Turn             1 *     2 =     2    2534
game_data.Current_Unit     1 *     2 =     2    2536
wizard_data                6 *  1224 =  7344    9880
gfp_TBL_Maps               2 *  4800 =  9600    19480
gfp_UU_TBL_2               2 *    96 =   192    19672
gfp_UU_TBL_1               2 *    96 =   192    19864
gfp_TBL_Landmasses         2 *  2400 =  4800    24664
gfp_TBL_Nodes             30 *    48 =  1440    26104
gfp_TBL_Fortresses         6 *     4 =    24    26128
gfp_TBL_Towers             6 *     4 =    24    26152
gfp_TBL_Encounters       102 *    24 =  2448    28600
gfp_TBL_Items            138 *    50 =  6900    35500
gfp_TBL_Cities           100 *   114 = 11400    46900
p_Units_Table           1009 *    32 = 32288    79188
gfp_TBL_Terr_Specials      2 *  2400 =  4800    83988
gfp_TBL_Scouting           2 *  2400 =  4800    88788
gfp_TBL_MoveMaps_EMM       2 * 14400 = 28800    117588
p_Events                   1 *   100 =   100    117688
p_Terrain_Flags            2 *  2400 =  4800    122488
grand_vizier_flag          1 *     2 =     2    122490
p_Premade_Items          250 *     1 =   250    122740
p_Hero_Names              35 *    16 =   560    123300


Arcanus 60×40 map (2 bytes per tile, 000 through 2F9) ... Value	Terrain

Arcanus 60×40 bonus deposits
Myrror 60×40 bonus deposits

Arcanus 60×40 exploration by YOU (bit flags)
Myrror 60×40 exploration by YOU (00 = unexplored, 0F = fully explored)

Arcanus 60×40 precalculated ½MP cost for Unused mode of transportation
Arcanus 60×40 precalculated ½MP cost for Walking mode of transportation
Arcanus 60×40 precalculated ½MP cost for Forester mode of transportation
Arcanus 60×40 precalculated ½MP cost for Mountaineer mode of transportation
Arcanus 60×40 precalculated ½MP cost for Swimming mode of transportation
Arcanus 60×40 precalculated ½MP cost for Sailing mode of transportation
Myrror 60×40 precalculated ½MP cost for Unused mode of transportation
Myrror 60×40 precalculated ½MP cost for Walking mode of transportation
Myrror 60×40 precalculated ½MP cost for Forester mode of transportation
Myrror 60×40 precalculated ½MP cost for Mountaineer mode of transportation
Myrror 60×40 precalculated ½MP cost for Swimming mode of transportation
Myrror 60×40 precalculated ½MP cost for Sailing mode of transportation

Events Data (2 × 50 bytes)

Arcanus 60×40 terrain flags
Myrror 60×40 terrain flags

Grand Vizier (0 = Off, 1 = On)

Items in Game (0 = not in game, 1 = item in game)

Chosen Hero Names
When heroes are dismissed or die, the game keeps track of the hero names here.
Offset	Length	Contents
00-0D	0E	Hero name (13 characters + '\0')
0E-0F	02s (int)	Experience
