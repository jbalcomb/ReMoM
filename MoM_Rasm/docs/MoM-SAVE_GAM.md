

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






