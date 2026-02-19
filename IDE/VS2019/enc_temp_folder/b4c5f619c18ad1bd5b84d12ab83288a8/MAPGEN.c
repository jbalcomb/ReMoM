/*
    MAGIC.EXE
    ovr051
    ...
    ovr054
    ovr055

MoO2
    Module: HOMEGEN
    Module: INITGAME
    Module: MAPGEN
*/

#ifdef STU_DEBUG
#include "../../STU/src/STU_DBG.h"
#endif

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/Mouse.h"
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DAT.h"  /* _players[]; enum e_SCOUT_BITS{} */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/Video.h"

#include "INITGAME.h"
#include "NEXTTURN.h"
#include "RACETYPE.h"
#include "special.h"
#include "Terrain.h"
#include "TerrType.h"
#include "UNITTYPE.h"
#include "MGC_DATA.h"  /* terrain_stats_table[] */

#include <assert.h>     /* assert() */
#include <string.h>     /* memcpy() memset(), strcat(), strcpy(), stricmp() */

#include "MAPGEN.h"

#include "../../MoM/src/MovePath.h"  /* struct s_MOVE_MODE_COST_MAPS */
// MOM_Data.c./h but also MovePath.h
// WZD dseg:9CAC
// drake178: TBL_MoveMaps_EMS
extern struct s_MOVE_MODE_COST_MAPS * movement_mode_cost_maps;



// forward declate to shut up VS can be made static, because NIU
static void NEWG_SetDeserts__STUB(void);
static void CRP_NEWG_CreatePathGrid__STUB(void * moves2, void * move_path_chunks);
// forward declate to shut up VS can be made static, because NIU
// fix 'static' on NEWG_SetDeserts__STUB() and CRP_NEWG_CreatePathGrid__STUB()





// MGC  dseg:8EA4
extern int16_t NEWG_Clicked_Race;

// MGC  dseg:8DD6
// drake78: IMG_NEWG_MapBuildBG
/*
    loaded in GAME_New_Screen_7()
    used in Init_New_Game()
*/
extern SAMB_ptr newgame_BUILDWOR_map_build_bar_seg;



// MGC  dseg:3334                                                 BEGIN:  ovr051 - Initialized Data

// MGC  dseg:3334
/*
    down, left, up, right, center
*/
int16_t TILE_Cardinal_XMod[5] = { 0, -1, 0, 1, 0 };
// MGC  dseg:333E
int16_t TILE_Cardinal_YMod[5] = { 1, 0, -1, 0, 0 };

// MGC  dseg:3348
int16_t TILE_OppositeDirs[4] = { 2, 3, 0, 1 };

// MGC  dseg:3350
int16_t TILE_Cardinal_XMod2[5] = { 0, -1, 0, 1, 0 };
// MGC  dseg:335A
int16_t TILE_Cardinal_YMod2[5] = { 1, 0, -1, 0, 0 };

// MGC  dseg:3364
struct s_mouse_list mouselist_mapgen[1] =
{
    { crsr_Hourglass, 0, SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX }
};

// MGC  dseg:3370 26 34 2C 34 37 34 43 34 50 34 60 34 68 34 77 34+UU_EZ_Names@_Array dw offset UU_cnst_Tower, offset UU_cnst_ChaosNode, offset UU_cnst_NatureNode, offset UU_cnst_SorceryNode, offset UU_cnst_Cave, offset UU_cnst_Dungeon, offset UU_cnst_AncientTmpl, offset UU_cnst_Keep, offset UU_cnst_Lair, offset UU_cnst_Ruins, offset UU_cnst_FallenTmpl ; "tower"
// MGC  dseg:3386 A2 00 A2 00 A2 00 A2 00 BB 00 BB 00 BB 00 BB 00+TILE_River_Types dw _Grasslands1, _Grasslands1, _Grasslands1, _Grasslands1
// MGC  dseg:3386 BC 00 BC 00 BC 00 BC 00 BD 00 C1 00 E9 00 BD 00+                                        ; DATA XREF: NEWG_SetRiverTiles+127t
// MGC  dseg:3386 B9 00 B9 00 B9 00 B9 00 ED 00 EE 00 EF 00 ED 00+dw _River1000, _River1000, _River1000, _River1000
// MGC  dseg:3386 BF 00 C3 00 EB 00 BF 00 FB 00 FC 00 FD 00 FE 00+dw _River0100, _River0100, _River0100, _River0100
// MGC  dseg:3386 BA 00 BA 00 BA 00 BA 00 C0 00 C4 00 EC 00 C0 00+dw _River1100_1, _River1100_2, _River1100_3, _River1100_1
// MGC  dseg:3386 F0 00 F1 00 F2 00 F0 00 F3 00 F4 00 F5 00 F6 00+dw _River0010, _River0010, _River0010, _River0010
// MGC  dseg:3386 BE 00 C2 00 EA 00 BE 00 FF 00 00 01 01 01 02 01+dw _River1010_1, _River1010_2, _River1010_3, _River1010_1
// MGC  dseg:3386 F7 00 F8 00 F9 00 FA 00 D4 01 D5 01 D6 01 D7 01 dw _River0110_1, _River0110_2, _River0110_3, _River0110_1
// MGC  dseg:3386                                                 dw _River1110_1, _River1110_2, _River1110_3, _River1110_4
// MGC  dseg:3386                                                 dw _River0001, _River0001, _River0001, _River0001
// MGC  dseg:3386                                                 dw _River1001_1, _River1001_2, _River1001_3, _River1001_1
// MGC  dseg:3386                                                 dw _River0101_1, _River0101_2, _River0101_3, _River0101_1
// MGC  dseg:3386                                                 dw _River1101_1, _River1101_2, _River1101_3, _River1101_4
// MGC  dseg:3386                                                 dw _River0011_1, _River0011_2, _River0011_3, _River0011_1
// MGC  dseg:3386                                                 dw _River1011_1, _River1011_2, _River1011_3, _River1011_4
// MGC  dseg:3386                                                 dw _River0111_1, _River0111_2, _River0111_3, _River0111_4
// MGC  dseg:3386                                                 dw _River1111_1, _River1111_2, _River1111_3, _River1111_4

// MGC  dseg:3406
char builddat_lbx_file__MGC_ovr051[] = "BUILDDAT";

// MGC  dseg:340F
char str_BuildingTheWorlds[] = "Building The Worlds...";

// MGC  dseg:3426 74 6F 77 65 72 00                               UU_cnst_Tower db 'tower',0              ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:342C 63 68 61 6F 73 20 6E 6F 64 65 00                UU_cnst_ChaosNode db 'chaos node',0     ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:3437 6E 61 74 75 72 65 20 6E 6F 64 65 00             UU_cnst_NatureNode db 'nature node',0   ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:3443 73 6F 72 63 65 72 79 20 6E 6F 64 65 00          UU_cnst_SorceryNode db 'sorcery node',0 ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:3450 6D 79 73 74 65 72 69 6F 75 73 20 63 61 76 65 00 UU_cnst_Cave db 'mysterious cave',0     ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:3460 64 75 6E 67 65 6F 6E 00                         UU_cnst_Dungeon db 'dungeon',0          ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:3468 61 6E 63 69 65 6E 74 20 74 65 6D 70 6C 65 00    UU_cnst_AncientTmpl db 'ancient temple',0
// MGC  dseg:3477 61 62 61 6E 64 6F 6E 65 64 20 6B 65 65 70 00    UU_cnst_Keep db 'abandoned keep',0      ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:3486 6D 6F 6E 73 74 65 72 20 6C 61 69 72 00          UU_cnst_Lair db 'monster lair',0        ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:3493 61 6E 63 69 65 6E 74 20 72 75 69 6E 73 00       UU_cnst_Ruins db 'ancient ruins',0      ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:34A1 66 61 6C 6C 65 6E 20 74 65 6D 70 6C 65 00       UU_cnst_FallenTmpl db 'fallen temple',0 ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC  dseg:34AF 54 45 52 52 54 59 50 45 00                      cnst_TERRTYPE_File db 'TERRTYPE',0      ; DATA XREF: NEWG_SetDeserts+26o ...

// MGC  dseg:34B8
char cityname_lbx_file__MGC_ovr051[] = "CITYNAME";
// // WZD dseg:6EC7
// char cityname_lbx_file__ovr143[] = "CITYNAME";

// MGC  dseg:34C1 00 00 00                                        align 4

// MGC  dseg:34C1                                                 END:  ovr051 - Initialized Data





// MGC  dseg:8EB0                                                 BEGIN:  ovr051 - Uninitialized Data  (MAPGEN)

// MGC  dseg:8EB0
int16_t NEWG_Landmass_Races[200];

// MGC  dseg:9040
/*
set 1 in NEWG_ClearLandmasses__WIP(), both times
incd in TILE_SetLandMass()
otherwise NIU
¿ MoO2  Module: MAPGEN  _num_maps_to_generate ?
*/
int16_t NEWG_LandmassCount;

// MGC  dseg:9042 00 00                                           dw 0
// MGC  dseg:9044 00 00                                           dw 0
// MGC  dseg:9046 00 00                                           dw 0
// MGC  dseg:9048 00 00                                           dw 0
// MGC  dseg:904A 00 00                                           dw 0
// MGC  dseg:904A
// MGC  dseg:904A                                                 END:  ovr051 - Uninitialized Data  (MAPGEN)





// MGC o51p01
// drake178: NEWG_CreateWorld()
/*
NEWG_CreateWorld()
¿ ~ MoO2 Module MAPGEN ?
¿ MoO2  Module: INITGAME  Init_New_Game_() ?
¿ MoO2  Module: HOMEGEN  Generate_Home_Worlds_() ?
creates the basic profiles of the AI wizards, then
generates the game map for both planes, drawing and
updating a progress bar in the process
BUG: may freeze if tower or fortress generation fails
due to there not being enough valid locations - also
inherits numerous smaller issues from the subroutines
and their order of execution
*/
/*

*/
void Init_New_Game(void)
{
    int16_t IDK1 = 0;  // _DI_
    int16_t IDK2 = 0;  // _SI_

    // bldg_data_table = SA_MK_FP0(LBX_Load_Data(cnst_BUILDDAT_File, 0, 0, 36, 52));
    bldg_data_table = (struct s_BLDG_DATA *)LBX_Load_Data(builddat_lbx_file__MGC_ovr051, 0, 0, 36, 52);
    
    Set_Mouse_List(1, mouselist_mapgen);

    Copy_On_To_Off_Page();
    
    Set_Page_Off();
    
    FLIC_Draw(75, 67, newgame_BUILDWOR_map_build_bar_seg);

    Set_Font_Style(3, 3, ST_NULL, ST_NULL);

    Print(88, 76, str_BuildingTheWorlds);  // "Building The Worlds..."
    
    Clear_Fields();

    Deactivate_Auto_Function();
    
    Toggle_Pages();
    
    Copy_On_To_Off_Page();
    
    Init_Computer_Players();

    Draw_Building_The_Worlds(5);

    NEWG_ClearLandmasses__WIP(ARCANUS_PLANE);

    Draw_Building_The_Worlds(10);

    NEWG_ClearLandmasses__WIP(MYRROR_PLANE);

    Draw_Building_The_Worlds(15);

    NEWG_CreateLands__WIP(ARCANUS_PLANE);

    Draw_Building_The_Worlds(20);

    NEWG_CreateLands__WIP(MYRROR_PLANE);

    Draw_Building_The_Worlds(25);

    NEWG_SetBaseLands__WIP(ARCANUS_PLANE);

    Draw_Building_The_Worlds(30);

    NEWG_SetBaseLands__WIP(MYRROR_PLANE);

    Draw_Building_The_Worlds(35);

    NEWG_SetSpecLands__WIP(ARCANUS_PLANE);

    Draw_Building_The_Worlds(40);

    NEWG_SetSpecLands__WIP(MYRROR_PLANE);

    Draw_Building_The_Worlds(45);

    Generate_Nodes();

    Draw_Building_The_Worlds(50);

    NEWG_EqualizeNodes__WIP(ARCANUS_PLANE);

    NEWG_EqualizeNodes__WIP(MYRROR_PLANE);

    Draw_Building_The_Worlds(55);

    Generate_Towers();

    Draw_Building_The_Worlds(60);

    NEWG_TileIsleExtend__WIP(ARCANUS_PLANE);

    NEWG_TileIsleExtend__WIP(MYRROR_PLANE);

    Generate_Lairs();

    Draw_Building_The_Worlds(65);

    _units = 0;



// All Grasslands: each city area square yields food2 = 3
// At (30,20) on Arcanus, the full 21-square diamond is in bounds
// max pop = (21 * 3) / 2 = 63 / 2 = 31
/*
All Grasslands
wx              wy
   17,18,19     33
16,17,18,19,20  34
16,17,18,19,20  36
16,17,18,19,20  37
   17,18,19     38
3 + 5 + 5 + 5 + 3 = 21 squares
21 map square at 3 food per tt_Grasslands1 = 63 food total
63 / 2 = 31 max population
*/
    // TEST_F(City_Maximum_Size_NewGame_test, AllGrasslands_Returns31)
    Set_Terrain_All(ARCANUS_PLANE, tt_Grasslands1);
    int16_t result = City_Maximum_Size_NewGame(30, 20, ARCANUS_PLANE);
    // EXPECT_EQ(result, 31);



    // must be the _world_map is done here
    Generate_Home_City__WIP();

    Draw_Building_The_Worlds(70);

    Generate_Neutral_Cities__WIP(ARCANUS_PLANE);

    Generate_Neutral_Cities__WIP(MYRROR_PLANE);

    Draw_Building_The_Worlds(75);

    Generate_Terrain_Specials(ARCANUS_PLANE);

    Generate_Terrain_Specials(MYRROR_PLANE);

    Generate_Roads(ARCANUS_PLANE);

    Generate_Roads(MYRROR_PLANE);

    Draw_Building_The_Worlds(80);

    NEWG_CreateShores__STUB();

    Draw_Building_The_Worlds(85);
                                          
    for(IDK1 = 0; IDK1 < 10; IDK1++)
    {
        for(IDK2 = 0; ((IDK2 < 2000) && (NEWG_CreateRiver__STUB(0) != 0)); IDK2++) { }
        for(IDK2 = 0; ((IDK2 < 2000) && (NEWG_CreateRiver__STUB(1) != 0)); IDK2++) { }
    }

    NEWG_SetRiverTiles__STUB(ARCANUS_PLANE);

    NEWG_SetRiverTiles__STUB(MYRROR_PLANE);

    NEWG_SetDeserts__STUB();

    Draw_Building_The_Worlds(85);

    NEWG_RandomizeTiles__STUB();

    Movement_Mode_Cost_Maps(ARCANUS_PLANE);

    Movement_Mode_Cost_Maps(MYRROR_PLANE);

    Draw_Building_The_Worlds(90);

    CRP_NEWG_CreatePathGrids__WIP(ARCANUS_PLANE);

    CRP_NEWG_CreatePathGrids__WIP(MYRROR_PLANE);

    Draw_Building_The_Worlds(95);

    Init_Square_Explored();

    NEWG_AnimateOceans__STUB();

    Set_Upper_Lair_Guardian_Count();

}

// MGC o51p02
// drake178: NEWG_EZ_MarkHadnLeft()
/*
; copies the encounter guardian amounts into the upper
; nibble of the same byte to create the had'n'left
; structure marking both the initial and remaining
; monsters in the records (not that it is actually used
; for anything other than to complicate things)
*/
/*

*/
void Set_Upper_Lair_Guardian_Count(void)
{
    int16_t itr_lairs = 0;  // _SI_

    for(itr_lairs = 0; itr_lairs < NUM_LAIRS; itr_lairs++)
    {

        _LAIRS[itr_lairs].guard1_count = (_LAIRS[itr_lairs].guard1_count | (_LAIRS[itr_lairs].guard1_count << 4));

        _LAIRS[itr_lairs].guard2_count = (_LAIRS[itr_lairs].guard2_count | (_LAIRS[itr_lairs].guard2_count << 4));

    }

}


// MGC o51p03
// drake178: NEWG_TileIsleExtend()
/*
; PATCHED / rewritten in the worldgen customizer
;
; attempts to extend some of the single tile islands
; created by the node and tower generator functions
;
; BUG: the switch values are botched, resulting in
; significantly less added tiles
*/
/*

*/
void NEWG_TileIsleExtend__WIP(int16_t wp)
{
    int16_t Grid_Index = 0;
    int16_t Can_Convert = 0;
    int16_t Random_Y_Modifier = 0;
    int16_t Random_X_Modifier = 0;
    int16_t Convert_Attempts = 0;
    int16_t square_has_tower = 0;
    int16_t itr_towers = 0;
    int16_t terrain_type = 0;
    int16_t itr_wy = 0;  // _DI_
    int16_t itr_wx = 0;  // _SI_

    for(itr_wy = 1; itr_wy < 39; itr_wy++)
    {

        for(itr_wx = 1; itr_wx < 59; itr_wx++)
        {

            // ; check if there is a tower of wizardry on the tile

            terrain_type = _world_maps[((wp * WORLD_SIZE) + (itr_wy * WORLD_WIDTH) + itr_wx )];

            square_has_tower = ST_FALSE;

            for(itr_towers = 0; itr_towers < NUM_TOWERS; itr_towers++)
            {

                if(
                    (_TOWERS[itr_towers].wy == itr_wy)
                    &&
                    (_TOWERS[itr_towers].wx == itr_wx)
                )
                {

                    square_has_tower = ST_TRUE;

                }

            }

            // ; skip if the tile does not have Ocean in all cardinal
            // ; directions (diagonals are not checked)

            if(
                (terrain_type == tt_SorceryNode)
                ||
                (terrain_type == tt_NatureNode)
                ||
                (terrain_type == tt_ChaosNode)
                ||
                (square_has_tower == ST_TRUE)
            )
            {

                if(
                    (TILE_IsOcean(itr_wx, (itr_wy - 1), wp) == ST_TRUE)  // ; tile above
                    &&
                    (TILE_IsOcean((itr_wx - 1), itr_wy, wp) == ST_TRUE)  // ; tile to the left
                    &&
                    (TILE_IsOcean((itr_wx + 1), itr_wy, wp) == ST_TRUE)  // ; tile to the right
                    &&
                    (TILE_IsOcean(itr_wx, (itr_wy + 1), wp) == ST_TRUE)  // ; tile below
                )
                {

                    if(Random(3) > 1)
                    {

                        Convert_Attempts = (1 + Random(8));

                        for(itr_towers = 0; itr_towers < Convert_Attempts; itr_towers++)
                        {

                            Random_X_Modifier = (Random(3) - 2);
                            
                            Random_Y_Modifier = (Random(3) - 2);

                            if(
                                (Random_X_Modifier != 0)
                                ||
                                (Random_Y_Modifier != 0)
                            )
                            {

                                if(
                                    (TILE_HasTower((itr_wx + Random_X_Modifier), (itr_wy + Random_Y_Modifier)) == ST_FALSE)
                                    &&
                                    (TILE_HasNode((itr_wx + Random_X_Modifier), (itr_wy + Random_Y_Modifier), wp) == ST_FALSE)
                                )
                                {

                                    // ; PATCHED here previously to fix the BUG below

                                    Grid_Index = ((Random_Y_Modifier * 3) + Random_X_Modifier + 1);

                                    Can_Convert = ST_FALSE;

                                    // ; BUG: the actual range is -3 to +5, not 1 to 9

                                    switch(Grid_Index)
                                    {
                                        case 0:
                                        {

                                            // DNE

                                        } break;
                                        case 1:
                                        {

                                            if(
                                                (_world_maps[((wp * WORLD_SIZE) + ((itr_wy + Random_Y_Modifier + 1) * WORLD_WIDTH) + (itr_wx + Random_X_Modifier))] == tt_Ocean1)
                                                &&
                                                (_world_maps[((wp * WORLD_SIZE) + ((itr_wy + Random_Y_Modifier) * WORLD_WIDTH) + (itr_wx + Random_X_Modifier + 1))] == tt_Ocean1)
                                            )
                                            {

                                                Can_Convert = ST_FALSE;

                                            }
                                            else
                                            {

                                                Can_Convert = ST_TRUE;

                                            }

                                        } break;
                                        case 2:
                                        {

                                            Can_Convert = ST_TRUE;

                                        }
                                        case 3:
                                        {

                                            if(
                                                (_world_maps[((wp * WORLD_SIZE) + ((itr_wy + Random_Y_Modifier + 1) * WORLD_WIDTH) + (itr_wx + Random_X_Modifier))] == tt_Ocean1)
                                                &&
                                                (_world_maps[((wp * WORLD_SIZE) + ((itr_wy + Random_Y_Modifier) * WORLD_WIDTH) + (itr_wx + Random_X_Modifier - 1))] == tt_Ocean1)
                                            )
                                            {

                                                Can_Convert = ST_FALSE;

                                            }
                                            else
                                            {

                                                Can_Convert = ST_TRUE;

                                            }

                                        } break;
                                        case 4:
                                        {

                                            Can_Convert = ST_TRUE;

                                        }
                                        case 5:
                                        {

                                            Can_Convert = ST_FALSE;

                                        }
                                        case 6:
                                        {

                                            Can_Convert = ST_TRUE;

                                        }
                                        case 7:
                                        {

                                            if(
                                                (_world_maps[((wp * WORLD_SIZE) + ((itr_wy + Random_Y_Modifier - 1) * WORLD_WIDTH) + (itr_wx + Random_X_Modifier))] == tt_Ocean1)
                                                &&
                                                (_world_maps[((wp * WORLD_SIZE) + ((itr_wy + Random_Y_Modifier) * WORLD_WIDTH) + (itr_wx + Random_X_Modifier + 1))] == tt_Ocean1)
                                            )
                                            {

                                                Can_Convert = ST_FALSE;

                                            }
                                            else
                                            {

                                                Can_Convert = ST_TRUE;

                                            }

                                        } break;
                                        case 8:
                                        {

                                            Can_Convert = ST_TRUE;

                                        }
                                        case 9:
                                        {

                                            if(
                                                (_world_maps[((wp * WORLD_SIZE) + ((itr_wy + Random_Y_Modifier - 1) * WORLD_WIDTH) + (itr_wx + Random_X_Modifier))] == tt_Ocean1)
                                                &&
                                                (_world_maps[((wp * WORLD_SIZE) + ((itr_wy + Random_Y_Modifier) * WORLD_WIDTH) + (itr_wx + Random_X_Modifier - 1))] == tt_Ocean1)
                                            )
                                            {

                                                Can_Convert = ST_FALSE;

                                            }
                                            else
                                            {

                                                Can_Convert = ST_TRUE;

                                            }

                                        } break;

                                    }

                                    if(Can_Convert == ST_TRUE)
                                    {

                                        TILE_SetLandMass__WIP(wp, (itr_wx + Random_X_Modifier), (itr_wy + Random_Y_Modifier));

                                        _world_maps[((wp * WORLD_SIZE) + ((itr_wy + Random_Y_Modifier) * WORLD_WIDTH) + (itr_wx + Random_X_Modifier))] = tt_Grasslands1;

                                    }

                                }

                            }

                        }

                    }
                    
                }

            }

        }

    }

}


// MGC o51p04
// drake178: NEWG_CreateTowers()
/*
; PATCHED / rewritten in the worldgen customizer
;
; creates the six towers of wizardry connecting the
; planes, which need to be at least 4 tiles away from
; any nodes, and at least 10 tiles away from each other
; if possible
;
; BUG: fails to initialize the attempt counter,
;  effectively resulting in the first 500 tries being
;  either discarded or becoming more than 500
*/
/*

*/
void Generate_Towers(void)
{
    int16_t Tries = 0;
    int16_t Min_Distance = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t itr1 = 0;  // _DI_
    int16_t itr2 = 0;  // _SI_

    Min_Distance = 10;

    for(itr1 = 0; itr1 < NUM_TOWERS; itr1++)
    {

        while(1)
        {

            Tries++;  // BUGBUG uninitialized!

            if(Tries > 500)
            {

                Tries = 450;

                Min_Distance--;

            }

            wx = (2 + Random(54));

            wy = (2 + Random(34));

            if(
                (_world_maps[((ARCANUS_PLANE * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] != tt_Ocean1)
                ||
                (_world_maps[((MYRROR_PLANE * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] != tt_Ocean1)
                ||
                (Random(40) == 1)
            )
            {

                for(itr2 = 0; itr2 < itr1; itr2++)
                {

                    if(Delta_XY_With_Wrap(wx, wy, _TOWERS[itr2].wx, _TOWERS[itr2].wy, WORLD_WIDTH) < Min_Distance)
                    {

                        break;

                    }

                }

                for(itr2 = 0; itr2 < NUM_NODES; itr2++)
                {

                    if(Delta_XY_With_Wrap(wx, wy, _NODES[itr2].wx, _NODES[itr2].wy, WORLD_WIDTH) < 4)
                    {

                        break;

                    }

                }

                _TOWERS[itr1].wx = (int8_t)wx;

                _TOWERS[itr1].wy = (int8_t)wy;

                _TOWERS[itr1].owner_idx = ST_UNDEFINED;

                TILE_SetLandMass__WIP(ARCANUS_PLANE, wx, wy);

                TILE_SetLandMass__WIP(MYRROR_PLANE, wx, wy);

                _world_maps[((ARCANUS_PLANE * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] = _Grasslands1;

                _world_maps[((MYRROR_PLANE * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] = _Grasslands1;

                break;

            }

        }

    }

}


// MGC o51p05
// drake178: NEWG_CreateCapitals()
/*
; PATCHED / rewritten in the worldgen customizer
;
; creates the fortress and corresponding capital city
; records for each player, along with their basic
; starting units
;
; BUG: the condition to allow AI high elves only on
;  Forest tiles can restart the whole fortress
;  generation process, yielding capitals closer to
;  each other or other objects than intended
*/
/*
SEEALSO: MoX-NewGame-Capitols.md

MoO2
MoDule: HOMEGEN
Generate_Home_Worlds_()
Generate_Home_Worlds_1_()

Randomize_Home_Worlds_()
Modify_Home_Worlds_()

Assign_Home_Star_Names_()

Init_Homeworld_Colony_()
    |-> Init_Homeworld_Colony2_()
        |-> Create_Ship_()

Module: ERICNET
    code (0 bytes) Init_Homeworld_Colony2_
        Address: 01:00013A3D
    code (0 bytes) Init_Homeworld_Colony_
        Address: 01:00013FA7


Init_New_Game_()

movsx   eax, si
push    eax
push    offset aMapD                    ; "Map:  %d"
push    17h
push    0
call    __wcpp_1_unwind_leave_
add     esp, 10h


push    dword ptr _settings.IDK_random_seed
push    offset aSeedLd                  ; "Seed: %ld"
push    24
push    0
mov     [ebp+IDK_Debug_Exit_Done], 1
call    __wcpp_1_unwind_leave_
add     esp, 10h


xor     edi, edi
inc     esi
call    Screen_Flic_Capture

Change_Home_City_Name_Popup(HUMAN_PLAYER_IDX);

The largest section of the main movement screen is occupied by the
main movement view. Here you can see a small town with a flag on top
that represents your starting city (aka “enchanted fortress”).

*/
void Generate_Home_City__WIP(void)
{
    int16_t max_pop_failures = 0;
    int16_t minimum_site_distance = 0;  // used for _NODES[], _TOWERS[], _LAIRS[]
    int16_t minimum_fortress_distance = 0;
    int16_t itr = 0;
    int16_t Invalid = 0;
    int16_t Tries_Per_Distance = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t wp = 0;
    int16_t UU_Fortresses[NUM_PLANES] = { 0, 0 };  // tracks fortress record populations? 1:1 with player_idx in the players loop?
    int16_t player_idx = 0;
    int16_t unit_type = 0;  // _DI_
    int16_t bldg_idx = 0;  // _SI_

    minimum_fortress_distance = 16;

    minimum_site_distance = 8;

// ; generate capitals for each player, including choosing
// ; a starting race for the AI wizards
// ; 
// ; contains multiple BUGs in addition to the methodology
// ;  being utterly horrible

    while(1)
    {
Loop_Distances:

        minimum_fortress_distance--;

        minimum_site_distance--;

        SETMIN(minimum_site_distance, 1);

        SETMIN(minimum_fortress_distance, 10);

        Tries_Per_Distance = 0;

        UU_Fortresses[ARCANUS_PLANE] = 0;

        UU_Fortresses[MYRROR_PLANE] = 0;

        max_pop_failures = 0;

        while(1)
        {
Loop_MaxPopTries:

            for(player_idx = 0; player_idx < NUM_PLAYERS; player_idx++)  // ; BUG: there are only at most 5 players
            {

                if(Tries_Per_Distance < 1000)
                {
// jump back here if the location is invalid
// jump back here if the max pop is too low
Loop_Location_1:
                    Tries_Per_Distance++;
                    wp = ARCANUS_PLANE;
                    if(_players[player_idx].myrran != ST_FALSE)
                    {
                        wp = MYRROR_PLANE;
                    }
                    wx = (2 + Random((WORLD_WIDTH - 6)));  // { 3, ..., 56}
                    wy = (2 + Random((WORLD_HEIGHT - 6)));  // { 3}

                    Invalid = ST_FALSE;

                    if(_world_maps[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] == tt_Ocean1)
                    {

                        Invalid = ST_TRUE;
                        
                    }

                    // ; invalidate the attempt if any other capital is
                    // ; closer than the minimum distance
                    for(bldg_idx = 0; bldg_idx < player_idx; bldg_idx++)
                    {

                        if(_FORTRESSES[bldg_idx].wp == wp)
                        {

                            if(Delta_XY_With_Wrap(wx, wy, _FORTRESSES[bldg_idx].wx, _FORTRESSES[bldg_idx].wy, WORLD_WIDTH) < minimum_fortress_distance)
                            {

                                Invalid = ST_TRUE;

                            }

                        }

                    }

                    // ; invalidate the attempt if any node is closer than the
                    // ; minimum distance
                    // ; 
                    // ; BUG: the encounter zone check below will also catch
                    // ;  these
                    // ; BUG: because the distance is halved, same tile is not
                    // ; excluded

                    for(bldg_idx = 0; bldg_idx < NUM_NODES; bldg_idx++)
                    {

                        if(_NODES[bldg_idx].wp == wp)
                        {

                            if(Delta_XY_With_Wrap(wx, wy, _NODES[bldg_idx].wx, _NODES[bldg_idx].wy, WORLD_WIDTH) < minimum_site_distance)
                            {

                                Invalid = ST_TRUE;

                            }

                        }

                    }

                    // ; invalidate the attempt if any tower is closer than
                    // ; the minimum distance
                    // ; 
                    // ; BUG: the encounter zone check below will also catch
                    // ;  these
                    // ; BUG: because the distance is halved, same tile is not
                    // ; excluded

                    for(bldg_idx = 0; bldg_idx < NUM_TOWERS; bldg_idx++)
                    {

                        if(Range(wx, wy, _TOWERS[bldg_idx].wx, _TOWERS[bldg_idx].wy) < minimum_site_distance)  // BUGBUG  should be Delta_XY_With_Wrap()
                        {

                            Invalid = ST_TRUE;

                        }

                    }

                    // ; invalidate the attempt if any encounter zone is
                    // ; closer than the minimum distance
                    // ; BUG: because the distance is halved, same tile is not
                    // ; excluded

                    for(bldg_idx = 0; bldg_idx < NUM_LAIRS; bldg_idx++)
                    {

                        if(_LAIRS[bldg_idx].wp == wp)
                        {

                            if(Delta_XY_With_Wrap(wx, wy, _LAIRS[bldg_idx].wx, _LAIRS[bldg_idx].wy, WORLD_WIDTH) < minimum_site_distance)
                            {

                                Invalid = ST_TRUE;

                            }

                        }

                    }

                    if(
                        (Tries_Per_Distance < 1000)
                        &&
                        (Invalid == ST_TRUE)
                    )
                    {
                        goto Loop_Location_1;
                    }

                    if((8 - (player_idx / 3)) < City_Maximum_Size_NewGame(wx, wy, wp))
                    {
                        max_pop_failures++;
                        if(max_pop_failures > 500)
                        {
                            goto Loop_MaxPopTries;
                        }
                        else
                        {
                            goto Loop_Location_1;
                        }
                    }
                    else
                    {
                        // ; create a fortress record for the player, then
                        // ; increase the loop variable
                        _FORTRESSES[player_idx].wx = (int8_t)wx;
                        _FORTRESSES[player_idx].wy = (int8_t)wy;
                        _FORTRESSES[player_idx].wp = (int8_t)wp;
                        _FORTRESSES[player_idx].wx = (int8_t)wx;
                        _FORTRESSES[player_idx].active = ST_TRUE;
                        UU_Fortresses[wp]++;

#ifdef STU_DEBUG
                        printf("player_idx: %d; terrain type index: %d;\n", player_idx, _world_maps[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)]);
                        dbg_prn("player_idx: %d; terrain type index: %d;\n", player_idx, _world_maps[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)]);
#endif

                    }

                }

            }

            if(Tries_Per_Distance >= 1000)
            {

                goto Loop_Distances;

            }



// @@Do_The_Cities:


    
            // ; select a random race for the AI player, and set it
            // ; into the next city record - myrran wizards get a
            // ; myrran race, while arcanians above normal difficulty
            // ; can only pick halflings, high elves, high men,
            // ; klackons, or nomads

            for(_cities = 0, itr = 0; itr < _num_players; _cities++, itr++)
            {

                if(itr == 0)
                {

                    _CITIES[_cities].race = (int8_t)NEWG_Clicked_Race;

                }
                else
                {
                        
                    if(_players[itr].myrran == ST_FALSE)
                    {

                        switch((Random(5) - 1))
                        {
                            case 0: { _CITIES[_cities].race = rt_Beastmen;  } break;
                            case 1: { _CITIES[_cities].race = rt_Dark_Elf;  } break;
                            case 2: { _CITIES[_cities].race = rt_Draconian; } break;
                            case 3: { _CITIES[_cities].race = rt_Dwarf;     } break;
                            case 4: { _CITIES[_cities].race = rt_Troll;     } break;
                        }

                    }
                    else
                    {

                        if(_difficulty <= god_Normal)
                        {

                            switch((Random(9) - 1))
                            {
                                case 0: { _CITIES[_cities].race = rt_Barbarian; } break;
                                case 1: { _CITIES[_cities].race = rt_Gnoll;     } break;
                                case 2: { _CITIES[_cities].race = rt_Halfling;  } break;
                                case 3: { _CITIES[_cities].race = rt_High_Elf;  } break;
                                case 4: { _CITIES[_cities].race = rt_High_Men;  } break;
                                case 5: { _CITIES[_cities].race = rt_Klackon;   } break;
                                case 6: { _CITIES[_cities].race = rt_Lizardman; } break;
                                case 7: { _CITIES[_cities].race = rt_Nomad;     } break;
                                case 8: { _CITIES[_cities].race = rt_Orc;       } break;
                            }

                        }
                        else
                        {

                            switch((Random(5) - 1))
                            {
                                case 0: { _CITIES[_cities].race = rt_Klackon;  } break;
                                case 1: { _CITIES[_cities].race = rt_High_Elf; } break;
                                case 2: { _CITIES[_cities].race = rt_Halfling; } break;
                                case 3: { _CITIES[_cities].race = rt_High_Men; } break;
                                case 4: { _CITIES[_cities].race = rt_Nomad;    } break;
                            }

                        }

                    }

                    /*
                    HERE:
                    fucks off because it requires a Forest square for High Elves
                    definitely is not just falling through the loop
                    otherwise, it does through the balance of the loop in a perfectly normal looking manner
                    */
                    if(
                        (_CITIES[_cities].race == rt_High_Elf)
                        &&
                        (Square_Is_Forest_NewGame(wx, wy, wp) == ST_FALSE)
                    )
                    {

                        goto Loop_Location_1;

                    }

                }

                // ; create a capital city for the player based on their
                // ; fortress record

                _CITIES[_cities].wx = _FORTRESSES[itr].wx;
                _CITIES[_cities].wy = _FORTRESSES[itr].wy;
                _CITIES[_cities].wp = _FORTRESSES[itr].wp;
                _CITIES[_cities].owner_idx = (int8_t)itr;
                _CITIES[_cities].size = 1;  // CTY_Hamlet ... ~== (4 / 4)
                _CITIES[_cities].population = 4;
                _CITIES[_cities].bldg_cnt = 0;
                _CITIES[_cities].Prod_Accu = 0;
                _CITIES[_cities].Pop_10s = 0;
                if(itr == HUMAN_PLAYER_IDX)
                {
                    _CITIES[_cities].construction = bt_Housing;
                }
                else
                {
                    _CITIES[_cities].construction = bt_AUTOBUILD;
                }
                _CITIES[_cities].enchantments[WALL_OF_FIRE] = ST_FALSE;
                _CITIES[_cities].enchantments[WALL_OF_DARKNESS] = ST_FALSE;  // out of order
                _CITIES[_cities].enchantments[CHAOS_RIFT] = ST_FALSE;
                _CITIES[_cities].enchantments[DARK_RITUALS] = ST_FALSE;
                _CITIES[_cities].enchantments[EVIL_PRESENCE] = ST_FALSE;
                _CITIES[_cities].enchantments[CURSED_LANDS] = ST_FALSE;
                _CITIES[_cities].enchantments[PESTILENCE] = ST_FALSE;
                _CITIES[_cities].enchantments[CLOUD_OF_SHADOW] = ST_FALSE;
                _CITIES[_cities].enchantments[ALTAR_OF_BATTLE] = ST_FALSE;  // out of order
                _CITIES[_cities].enchantments[FAMINE] = ST_FALSE;
                _CITIES[_cities].enchantments[FLYING_FORTRESS] = ST_FALSE;
                _CITIES[_cities].enchantments[DEATH_WARD] = ST_FALSE;  // out of order
                _CITIES[_cities].enchantments[CHAOS_WARD] = ST_FALSE;  // out of order
                _CITIES[_cities].enchantments[LIFE_WARD] = ST_FALSE;  // out of order
                _CITIES[_cities].enchantments[SORCERY_WARD] = ST_FALSE;  // out of order
                _CITIES[_cities].enchantments[NATURE_WARD] = ST_FALSE;  // out of order
                _CITIES[_cities].enchantments[NATURES_EYE] = ST_FALSE;
                _CITIES[_cities].enchantments[EARTH_GATE] = ST_FALSE;
                _CITIES[_cities].enchantments[STREAM_OF_LIFE] = ST_FALSE;
                _CITIES[_cities].enchantments[GAIAS_BLESSING] = ST_FALSE;
                _CITIES[_cities].enchantments[INSPIRATIONS] = ST_FALSE;
                _CITIES[_cities].enchantments[PROSPERITY] = ST_FALSE;
                _CITIES[_cities].enchantments[ASTRAL_GATE] = ST_FALSE;
                _CITIES[_cities].enchantments[HEAVENLY_LIGHT] = ST_FALSE;
                _CITIES[_cities].enchantments[CONSECRATION] = ST_FALSE;
                _CITIES[_cities].enchantments[NIGHTSHADE] = ST_FALSE;
                _CITIES[_cities].farmer_count = 3;
                for(bldg_idx = 0; bldg_idx < NUM_BUILDINGS; bldg_idx++)
                {
                    _CITIES[_cities].bldg_status[bldg_idx] = bs_NotBuilt;
                }
                _CITIES[_cities].bldg_status[bt_NONE] = bs_Replaced;
                _CITIES[_cities].bldg_status[bt_Smithy] = bs_Built;
                _CITIES[_cities].bldg_status[bt_Barracks] = bs_Built;
                _CITIES[_cities].bldg_status[bt_BuildersHall] = bs_Built;

                // Random_City_Name_By_Race(_CITIES[city_idx].race, _CITIES[city_idx].name);
                Random_City_Name_By_Race_NewGame(_CITIES[_cities].race, _CITIES[_cities].name);

            }

            // if we get here, we're Done-Done?!?
            goto Done_Done;

        }

    }



Done_Done:

    // ; create a corresponding spearmen unit in the capital
    // ; of each wizard whose starting race is not dwarf

    for(itr = 0; itr < _num_players; itr++)
    {
    
        switch(_CITIES[itr].race)
        {
            case rt_Barbarian: { unit_type = ut_BarbSpearmen;   } break;
            case rt_Beastmen:  { unit_type = ut_BeastSpearmen;  } break;
            case rt_Dark_Elf:  { unit_type = ut_DrowSpearmen;   } break;
            case rt_Draconian: { unit_type = ut_DracSpearmen;   } break;
            case rt_Dwarf:     { unit_type = ut_DwarfSwordsmen; } break;
            case rt_Gnoll:     { unit_type = ut_GnollSpearmen;  } break;
            case rt_Halfling:  { unit_type = ut_HflngSpearmen;  } break;
            case rt_High_Elf:  { unit_type = ut_HElfSpearmen;   } break;
            case rt_High_Men:  { unit_type = ut_HMenSpearmen;   } break;
            case rt_Klackon:   { unit_type = ut_KlckSpearmen;   } break;
            case rt_Lizardman: { unit_type = ut_LizSpearmen;    } break;
            case rt_Nomad:     { unit_type = ut_NmdSpearmen;    } break;
            case rt_Orc:       { unit_type = ut_OrcSpearmen;    } break;
            case rt_Troll:     { unit_type = ut_TrlSpearmen;    } break;
        }
    
        // Dwarves get two Swordsmen
        if(_CITIES[itr].race != rt_Dwarf)
        {

            Create_Unit_NewGame(unit_type, itr, _CITIES[itr].wx, _CITIES[itr].wy, _CITIES[itr].wp, itr);

        }

    }

    // ; create a corresponding swordsmen unit in the capital
    // ; of each wizard, with an additional one if their
    // ; starting race is dwarf

    for(itr = 0; itr < _num_players; itr++)
    {
    
        switch(_CITIES[itr].race)
        {
            case rt_Barbarian: { unit_type = ut_BarbSwordsmen;  } break;
            case rt_Beastmen:  { unit_type = ut_BeastSwordsmen; } break;
            case rt_Dark_Elf:  { unit_type = ut_DrowSwordsmen;  } break;
            case rt_Draconian: { unit_type = ut_DracSwordsmen;  } break;
            case rt_Dwarf:     { unit_type = ut_DwarfSwordsmen; } break;
            case rt_Gnoll:     { unit_type = ut_GnollSwordsmen; } break;
            case rt_Halfling:  { unit_type = ut_HflngSwordsmen; } break;
            case rt_High_Elf:  { unit_type = ut_HElfSwordsmen;  } break;
            case rt_High_Men:  { unit_type = ut_HMenSwordsmen;  } break;
            case rt_Klackon:   { unit_type = ut_KlckSwordsmen;  } break;
            case rt_Lizardman: { unit_type = ut_LizSwordsmen;   } break;
            case rt_Nomad:     { unit_type = ut_NmdSwordsmen;   } break;
            case rt_Orc:       { unit_type = ut_OrcSwordsmen;   } break;
            case rt_Troll:     { unit_type = ut_TrlSwordsmen;   } break;
        }
    
        if(_CITIES[itr].race == rt_Dwarf)
        {

            Create_Unit_NewGame(unit_type, itr, _CITIES[itr].wx, _CITIES[itr].wy, _CITIES[itr].wp, itr);

        }

        Create_Unit_NewGame(unit_type, itr, _CITIES[itr].wx, _CITIES[itr].wy, _CITIES[itr].wp, itr);

    }

    // @@Done

}


// MGC o51p06
// drake178: NEWG_ClearLandmasses()
/*

; PATCHED / rewritten in the worldgen customizer to
; block clear both planes at once
;
; clears the landmass array for the selected plane
*/
/*
_landmasses[((wp * WORLD_SIZE) + (itr_wy * WORLD_WIDTH) + itr_wx)] = 0; 
*/
void NEWG_ClearLandmasses__WIP(int16_t wp)
{
    int16_t wy = 0;  // _SI_
    int16_t wx = 0;  // _CX_

    NEWG_LandmassCount = 1;

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        
        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {

            // _landmasses[((wp * WORLD_SIZE) + (itr_wy * WORLD_WIDTH) + itr_wx)] = 0;
            SET_LANDMASS(wx,wy,wp,0);

        }

    }
}


// MGC o51p07
// drake178: NEWG_EqualizeNodes()
/*
; reduces the amount of sorcery nodes on the plane if
; there are too many, converting random ones to chaos
; and nature until there's at least a set amount of
; each of those
;
; INCONSISTENT: will almost always yield a fixed
;  configuration (4-6-6 or 8-3-3) if there are excess
;  sorcery nodes, due to ignoring their actual amount
*/
/*

*/
void NEWG_EqualizeNodes__WIP(int16_t wp)
{
    int16_t Excess_Sorcery = 0;
    int16_t Nature_Convert = 0;
    int16_t Chaos_Convert = 0;
    int16_t node_idx = 0;
    int16_t Nature_Count = 0;
    int16_t Sorcery_Count = 0;
    int16_t Chaos_Count = 0;
    int16_t random_node_idx = 0;  // _DI_

    Chaos_Count = 0;
    Sorcery_Count = 0;
    Nature_Count = 0;

    // ; count the node types on the selected plane

    for(node_idx = 0; node_idx < NUM_NODES; node_idx++)
    {

        if(_NODES[node_idx].wp == wp)
        {

            switch(_NODES[node_idx].type)
            {

                case nt_Sorcery: { Sorcery_Count++; } break;

                case nt_Nature:  { Nature_Count++;  } break;

                case nt_Chaos:   { Chaos_Count++;   } break;

            }

        }

    }

    Chaos_Convert = 0;
    Nature_Convert = 0;

    if(wp == ARCANUS_PLANE)
    {
        // ; if there are more than 9 sorcery nodes, set the
        // ; convert amounts such that there will be at least 6
        // ; each of chaos and nature ones
        // ; 
        // ; INCONSISTENT: does not decrement the excess sorcery
        // ;  count despite setting up the value, so 1 has the
        // ;  same result as 7

        if(Sorcery_Count > 9)
        {

            Excess_Sorcery = (Sorcery_Count - 9);

        }
        else
        {

            Excess_Sorcery = 0;

        }

        while((Chaos_Count < 6) && (Excess_Sorcery > 0))
        {

            Chaos_Count++;

            Chaos_Convert++;

        }

        while((Nature_Count < 6) && (Excess_Sorcery > 0))
        {

            Nature_Count++;

            Nature_Convert++;

        }

    }
    else  /* MYRROR_PLANE */
    {
        // ; if there are more than 4 sorcery nodes, set the
        // ; convert amounts such that there will be at least 3
        // ; each of chaos and nature ones
        // ; 
        // ; INCONSISTENT: does not decrement the excess sorcery
        // ;  count despite setting up the value, so 1 has the
        // ;  same result as 10

        if(Sorcery_Count > 4)
        {

            Excess_Sorcery = (Sorcery_Count - 4);

        }
        else
        {

            Excess_Sorcery = 0;

        }

        while((Chaos_Count < 3) && (Excess_Sorcery > 0))
        {

            Chaos_Count++;

            Chaos_Convert++;

        }

        while((Nature_Count < 3) && (Excess_Sorcery > 0))
        {

            Nature_Count++;

            Nature_Convert++;

        }

    }

    // ; convert the specified amount of random sorcery nodes
    // ; on the plane to chaos

    while(Chaos_Convert > 0)
    {
        
        random_node_idx = (Random(30) - 1);

        if(_NODES[random_node_idx].wp == wp)
        {

            if(_NODES[random_node_idx].type == nt_Sorcery)
            {

                _NODES[random_node_idx].type = nt_Chaos;

                _world_maps[((wp * WORLD_SIZE) + (_NODES[random_node_idx].wy * WORLD_WIDTH) + _NODES[random_node_idx].wx)] = tt_ChaosNode;

                // ; entirely unnecessary, already done before

                TILE_SetLandMass__WIP(wp, _NODES[random_node_idx].wx, _NODES[random_node_idx].wy);

                Chaos_Convert--;

            }

        }

    }

    // ; convert the specified amount of random sorcery nodes
    // ; on the plane to nature

    while(Nature_Convert > 0)
    {
        
        random_node_idx = (Random(30) - 1);

        if(_NODES[random_node_idx].wp == wp)
        {

            if(_NODES[random_node_idx].type == nt_Sorcery)
            {

                _NODES[random_node_idx].type = nt_Nature;

                _world_maps[((wp * WORLD_SIZE) + (_NODES[random_node_idx].wy * WORLD_WIDTH) + _NODES[random_node_idx].wx)] = tt_NatureNode;

                // ; entirely unnecessary, already done before

                TILE_SetLandMass__WIP(wp, _NODES[random_node_idx].wx, _NODES[random_node_idx].wy);

                Nature_Convert--;

            }

        }

    }

}


// MGC o51p08
// drake178: NEWG_SetSpecLands()
/*
; PATCHED / rewritten in the worldgen customizer to
; include the inland lake fill inherited from v1.50
; (except with non-grassland-only terrain), and allow
; customizing both max tundra reach and desert and
; swamp creation attempts
;
; creates random patches of Tundra, Desert, and Swamp,
; converting existing land based on various conditions
*/
/*

*/
void NEWG_SetSpecLands__WIP(int16_t wp)
{
    int16_t UU_Prev_Dir = 0;
    int16_t Max_Tries = 0;
    int16_t Direction = 0;
    int16_t Origin_Direction = 0;
    int16_t Processing_Y = 0;
    int16_t Processing_X = 0;
    int16_t itr = 0;
    int16_t Origin_Y = 0;
    int16_t Origin_X = 0;
    int16_t Tries = 0;
    int16_t itr_wx = 0;  // _DI_
    int16_t itr_wy = 0;  // _SI_

    // ; randomly convert Grassland and Forest tiles in the
    // ; top and bottom 10 rows of the map into Tundra with
    // ; the chance decreasing the further away the tile is
    // ; from the corresponding edge

    for(itr_wy = 2; itr_wy < 8; itr_wy++)
    {

        for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
        {

            // top 2 thru 7
            if(
                (_world_maps[(wp * WORLD_SIZE + (itr_wy * WORLD_WIDTH) + itr_wx)] == tt_Grasslands1)
                ||
                (_world_maps[(wp * WORLD_SIZE + (itr_wy * WORLD_WIDTH) + itr_wx)] == tt_Forest1)
            )
            {

                if((2 + Random(8)) >= itr_wy)
                {

                    _world_maps[(wp * WORLD_SIZE + (itr_wy * WORLD_WIDTH) + itr_wx)] = tt_Tundra1;
                    
                }

            }

            // bottom 32 thru 37
            if(
                (_world_maps[(wp * WORLD_SIZE + ((39 - itr_wy) * WORLD_WIDTH) + itr_wx)] == tt_Grasslands1)
                ||
                (_world_maps[(wp * WORLD_SIZE + ((39 - itr_wy) * WORLD_WIDTH) + itr_wx)] == tt_Forest1)
            )
            {

                if((2 + Random(8)) >= itr_wy)
                {

                    // { (39-2), (39-3), (39-4), (39-5), (39-6), (39-7) } = { 37, 36, 35, 34, 33, 32 } 
                    _world_maps[(wp * WORLD_SIZE + ((39 - itr_wy) * WORLD_WIDTH) + itr_wx)] = tt_Tundra1;
                    
                }

            }

        }

    }

    // ; randomly create up to 8 patches of Desert
    for(itr = 0; itr < 8; itr++)
    {

        Origin_X = (2 + Random(54));

        Origin_Y = (8 + Random(24));

        if(_world_maps[(wp * WORLD_SIZE + (Origin_Y * WORLD_WIDTH) + Origin_X)] == tt_Forest1)
        {

            _world_maps[(wp * WORLD_SIZE + (Origin_Y * WORLD_WIDTH) + Origin_X)] = tt_Desert1;

            for(Origin_Direction = 0; Origin_Direction < 5; Origin_Direction++)
            {

                Processing_X = (Origin_X + TILE_Cardinal_XMod2[Origin_Direction]);

                Processing_Y = (Origin_Y + TILE_Cardinal_YMod2[Origin_Direction]);

                UU_Prev_Dir = ST_UNDEFINED;

                Max_Tries = (4 + Random(6));

                for(Tries = 0; Tries < Max_Tries; Tries++)
                {

                    Direction = (Random(4) - 1);

                    itr_wx = (Processing_X + TILE_Cardinal_XMod2[Direction]);

                    itr_wy = (Processing_Y + TILE_Cardinal_YMod2[Direction]);

                    if(itr_wx >= WORLD_WIDTH)
                    {

                        itr_wx -= WORLD_WIDTH;

                    }

                    if(itr_wy >= WORLD_HEIGHT)
                    {

                        itr_wy -= WORLD_HEIGHT;

                    }

                    if(itr_wx < 0)
                    {

                        itr_wx += WORLD_WIDTH;

                    }

                    if(itr_wy < 0)
                    {

                        itr_wy += WORLD_HEIGHT;

                    }

                    Processing_X = itr_wx;

                    Processing_Y = itr_wy;

                    if(_world_maps[(wp * WORLD_SIZE + (Processing_Y * WORLD_WIDTH) + Processing_X)] != tt_Ocean1)
                    {

                        _world_maps[(wp * WORLD_SIZE + (Processing_Y * WORLD_WIDTH) + Processing_X)] = tt_Desert1;

                    }

                }

            }

        }

    }

    // ; randomly create up to 8 patches of Swamps, converting
    // ; only Forest tiles in the patch
    for(itr = 0; itr < 8; itr++)
    {

        Origin_X = (1 + Random(56));

        Origin_Y = (10 + Random(20));

        if(
            (Origin_Y < 35)
            ||
            (Origin_Y > 45)
        )
        {

            if(_world_maps[(wp * WORLD_SIZE + (Origin_Y * WORLD_WIDTH) + Origin_X)] == tt_Forest1)
            {

                _world_maps[(wp * WORLD_SIZE + (Origin_Y * WORLD_WIDTH) + Origin_X)] = tt_Swamp1;

                for(Origin_Direction = 0; Origin_Direction < 5; Origin_Direction++)
                {

                    Processing_X = (Origin_X + TILE_Cardinal_XMod2[Origin_Direction]);

                    Processing_Y = (Origin_Y + TILE_Cardinal_YMod2[Origin_Direction]);

                    UU_Prev_Dir = ST_UNDEFINED;

                    Max_Tries = (2 + Random(3));

                    for(Tries = 0; Tries < Max_Tries; Tries++)
                    {

                        Direction = (Random(4) - 1);

                        itr_wx = (Processing_X + TILE_Cardinal_XMod2[Direction]);

                        itr_wy = (Processing_Y + TILE_Cardinal_YMod2[Direction]);

                        if(itr_wx >= WORLD_WIDTH)
                        {

                            itr_wx -= WORLD_WIDTH;

                        }

                        if(itr_wy >= WORLD_HEIGHT)
                        {

                            itr_wy -= WORLD_HEIGHT;

                        }

                        if(itr_wx < 0)
                        {

                            itr_wx += WORLD_WIDTH;

                        }

                        if(itr_wy < 0)
                        {

                            itr_wy += WORLD_HEIGHT;

                        }

                        Processing_X = itr_wx;

                        Processing_Y = itr_wy;

                        if(_world_maps[(wp * WORLD_SIZE + (Processing_Y * WORLD_WIDTH) + Processing_X)] == tt_Forest1)
                        {

                            _world_maps[(wp * WORLD_SIZE + (Processing_Y * WORLD_WIDTH) + Processing_X)] = tt_Swamp1;

                        }

                    }

                }

            }

        }

    }

}


// MGC o51p09
// drake178: NEWG_SetBaseLands()
/*

*/
/*

*/
void NEWG_SetBaseLands__WIP(int16_t wp)
{
    int16_t Tiles_To_Add = 0;
    int16_t Tiles_Added = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;  // _SI_

    // ; convert each non-ocean tile into a basic land based
    // ; on the amount of "hits" it got during the continent
    // ; generation
    // ;    1: Grassland ($A2) with 3/4 chance, or Forest
    // ;  2-3: Forest ($A3)
    // ;  4-5: Hills ($AB)
    // ;   6+: Mountains ($A4)

    for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy++)
    {

        for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
        {

            if(_world_maps[(wp * WORLD_SIZE + (itr_wy * WORLD_WIDTH) + itr_wx)] != 0)
            {

                // ; convert the tile into a grassland, forest, hill, or
                // ; mountain tile based on the amount of "hits" it got
                // ; during the continent generation
                // ;    1: Grassland ($A2) with 3/4 chance, or Forest
                // ;  2-3: Forest ($A3)
                // ;  4-5: Hills ($AB)
                // ;   6+: Mountains ($A4)

// tt_Ocean1      = 0x0,
// tt_BugGrass    = 0x1,
// tt_Shore1_Fst  = 0x2,
// DEDU what is terrain type 3? ...TT_Shore1_Lst>

                if(_world_maps[(wp * WORLD_SIZE + (itr_wy * WORLD_WIDTH) + itr_wx)] <= 3)
                {

                    if(_world_maps[(wp * WORLD_SIZE + (itr_wy * WORLD_WIDTH) + itr_wx)] > 1)
                    {

                        _world_maps[(wp * WORLD_SIZE + (itr_wy * WORLD_WIDTH) + itr_wx)] = tt_Forest1;

                    }
                    else
                    {

                        if(Random(4) == 1)
                        {

                            _world_maps[(wp * WORLD_SIZE + (itr_wy * WORLD_WIDTH) + itr_wx)] = tt_Grasslands1;

                        }

                    }

                }
                else if(
                    (_world_maps[(wp * WORLD_SIZE + (itr_wy * WORLD_WIDTH) + itr_wx)] == 4)
                    ||
                    (_world_maps[(wp * WORLD_SIZE + (itr_wy * WORLD_WIDTH) + itr_wx)] == 5)
                )
                {

                    _world_maps[(wp * WORLD_SIZE + (itr_wy * WORLD_WIDTH) + itr_wx)] = tt_Hills1;

                }
                else
                {
                    
                    _world_maps[(wp * WORLD_SIZE + (itr_wy * WORLD_WIDTH) + itr_wx)] = tt_Mountain1;

                }

            }

        }

    }

    for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
    {

        _world_maps[(wp * WORLD_SIZE + (WORLD_YMIN * WORLD_WIDTH) + itr_wx)] = tt_Tundra1;

        _world_maps[(wp * WORLD_SIZE + (WORLD_YMAX * WORLD_WIDTH) + itr_wx)] = tt_Tundra1;

        // ; convert a horizontal strip of 1-4 (random) tiles in
        // ; the row below the top one into tundra, starting at
        // ; the current X coordinate (in si), and aborting if
        // ; the right edge of the map is reached

        if(Random(4) == 1)
        {

            Tiles_To_Add = Random(4);

            for(Tiles_Added = 0; ((Tiles_Added < Tiles_To_Add) & ((itr_wx + Tiles_Added) < WORLD_WIDTH)); Tiles_Added++)
            {

                _world_maps[(wp * WORLD_SIZE + ((WORLD_YMIN + 1) * WORLD_WIDTH) + itr_wx + Tiles_Added)] = tt_Tundra1;                

            }

        }

        // ; convert a horizontal strip of 1-4 (random) tiles in
        // ; the row above the bottom one into tundra, starting
        // ; at the current X coordinate (in si), and aborting if
        // ; the right edge of the map is reached

        if(Random(4) == 1)
        {

            Tiles_To_Add = Random(4);

            for(Tiles_Added = 0; ((Tiles_Added < Tiles_To_Add) & ((itr_wx + Tiles_Added) < WORLD_WIDTH)); Tiles_Added++)
            {

                _world_maps[(wp * WORLD_SIZE + ((WORLD_YMAX - 1) * WORLD_WIDTH) + itr_wx + Tiles_Added)] = tt_Tundra1;                

            }

        }

    }

}


// MGC o51p10
// drake178: NEWG_CreateLands()
/*
; PATCHED / rewritten in the worldgen customizer to
; allow fine tuning the desired land tiles and set
; continent size limits, as well as to override the
; origin coordinate constraints to support more
; combinations of those two settings
;
; clears the tile map of the selected plane, and then
; generates continents according to the land size
; settings
*/
/*
¿ populates _landmass[] ?
per plane
~ MoO2 "map"
*/
void NEWG_CreateLands__WIP(int16_t wp)
{
    int16_t Used_Map_Sections[5][5] = { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } };
    int16_t Origin_Y = 0;
    int16_t Origin_X = 0;
    int16_t Origin_Direction = 0;
    int16_t Max_Direction = 0;
    int16_t Section_Height = 0;
    int16_t Section_Width = 0;
    int16_t Origins_Remaining = 0;
    int16_t Same_Dir_Steps = 0;
    int16_t Last_Direction = 0;
    int16_t Direction = 0;
    int16_t Steps_Taken = 0;
    int16_t Steps_To_Take = 0;
    int16_t Next_Tile_Y = 0;
    int16_t Next_Tile_X = 0;
    int16_t Processed_Tile_Y = 0;
    int16_t Processed_Tile_X = 0;
    int16_t Desired_Land_Tiles = 0;
    int16_t Generated_Land_Tiles = 0;
    int16_t itr_wy = 0;  // _DI_
    int16_t itr_wx = 0;  // _SI_

    // ; clear the map tile array of the plane
    for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy++)
    {

        for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
        {

            _world_maps[((wp * WORLD_SIZE) + (itr_wy * WORLD_WIDTH) + itr_wx)] = 0;

        }

    }

    // ; clear the map section array
    // ; 
    // ; INCONSISTENT: the array is 5 by 5 in size, yet the
    // ; divisions result in only 4 by 3 being used
    for(itr_wy = 0; itr_wy < 5; itr_wy++)
    {

        for(itr_wx = 0; itr_wx < 5; itr_wx++)
        {

            Used_Map_Sections[itr_wy][itr_wx] = 0;

        }

    }

    Origins_Remaining = 8;

    Section_Width = 16;

    Section_Height = 11;

    Generated_Land_Tiles = 0;

    // ~ galaxy size, _NUM_STARS
    switch(_landsize)
    {
        case gol_Small:  { Desired_Land_Tiles = 360; } break;
        case gol_Medium: { Desired_Land_Tiles = 480; } break;
        case gol_Large:  { Desired_Land_Tiles = 720; } break;
    }

    // ~ _num_maps_to_generate, _n_maps_generated
    while(Generated_Land_Tiles <= Desired_Land_Tiles)
    {

        Last_Direction = ST_UNDEFINED;

        Same_Dir_Steps = 1;

        while(Origins_Remaining > 0)
        {

            // Origin_X = (6 + Random(46));
            // Origin_Y = (6 + Random(26));
            Origin_X = 6;
            Origin_X += Random(46);
            Origin_Y = 6;
            Origin_Y += Random(26);
            assert(Origin_X >= 7);
            assert(Origin_Y >= 7);

            if(Used_Map_Sections[(Origin_Y / Section_Height)][(Origin_X / Section_Width)] != 1)
            {

                Used_Map_Sections[(Origin_Y / Section_Height)][(Origin_X / Section_Width)] = 1;

                Origins_Remaining--;
                
            }

        }

        Max_Direction = (2 + Random(3));

        Origin_Direction = 0;

        while(Origin_Direction < Max_Direction)
        {

            switch(_landsize)
            {

                case gol_Small:  { Steps_To_Take = ( 5 + Random(10)); } break;
                case gol_Medium: { Steps_To_Take = (10 + Random(10)); } break;
                case gol_Large:  { Steps_To_Take = (20 + Random(10)); } break;

            }

            assert(Origin_X >= 7);
            assert(Origin_Y >= 7);
            Processed_Tile_X = (Origin_X + TILE_Cardinal_XMod2[Origin_Direction]);
            Processed_Tile_Y = (Origin_Y + TILE_Cardinal_YMod2[Origin_Direction]);
            assert(Processed_Tile_X >= 6);
            assert(Processed_Tile_Y >= 6);

            Steps_Taken = 0;

            while((Steps_Taken < Steps_To_Take) && (Generated_Land_Tiles <= Desired_Land_Tiles))
            {

                if(_world_maps[((wp * WORLD_SIZE) + (Processed_Tile_Y * WORLD_WIDTH) + Processed_Tile_X)] == 0)
                {

                    Generated_Land_Tiles++;

                }

                _world_maps[((wp * WORLD_SIZE) + (Processed_Tile_Y * WORLD_WIDTH) + Processed_Tile_X)] += 1;

                TILE_SetLandMass__WIP(wp, Processed_Tile_X, Processed_Tile_Y);

                /*
                
                    WTF  DEDU  What is the loop here?
                    ...looks like `Last_Direction = ST_UNDEFINED;` should be a breaking condition

                */
                while(1)
                {

                    Direction = (Random(4) - 1);

                    if(Direction == Last_Direction)
                    {

                        if(Random((Same_Dir_Steps * 2)) != 1)
                        {

                            continue;

                        }
                        else
                        {

                            Same_Dir_Steps++;

                        }

                    }
                    else
                    {

                        Same_Dir_Steps = 1;
                        
                    }

                    Last_Direction = Direction;

                    // assert(Processed_Tile_X >= 6);
                    // assert(Processed_Tile_Y >= 6);
                    Next_Tile_X = (Processed_Tile_X + TILE_Cardinal_XMod[Direction]);
                    Next_Tile_Y = (Processed_Tile_Y + TILE_Cardinal_YMod[Direction]);
                    // assert(Next_Tile_X >= 5);
                    // assert(Next_Tile_Y >= 5);

                    if(
                        (Next_Tile_X < 2)
                        ||
                        (Next_Tile_Y < 4)
                        ||
                        (Next_Tile_X >= 58)
                        ||
                        (Next_Tile_Y >= 36)
                    )
                    {
                        
                        Last_Direction = ST_UNDEFINED;

                    }
                    else
                    {

                        break;
                        
                    }

                }

                Processed_Tile_X = Next_Tile_X;

                Processed_Tile_Y = Next_Tile_Y;

                Steps_Taken++;

            }

            Origin_Direction++;

        }

        // printf("Generated_Land_Tiles: %d\n", Generated_Land_Tiles);

    }

}


// MGC o51p11
// drake178: NEWG_CreateNodes()
/*
; PATCHED / rewritten in the worldgen customizer to
; create nodes after capitals and towers
;
; generates 30 magical nodes with non-overlapping power
; auras across the two planes, and sets their tiles to
; the unique node terrain types
;
; BUG: when checking for land, the Myrran nodes look at
;  the Arcanus tile instead
*/
/*

*/
void Generate_Nodes(void)
{
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t Y_Section_Base = 0;
    int16_t X_Section_Base = 0;
    int16_t itr = 0;  // _SI_
    int16_t itr2 = 0;  // _DI_

    // ; generate 16 arcanian nodes, complete with auras and
    // ; terrain type setting

    for(itr = 0; itr < 16; itr++)
    {

        while(1)
        {
somehow1:
            // X_Section_Base = ((itr / 5) * 12);
            X_Section_Base = ((itr % 5) * 12);

            Y_Section_Base = ((itr / 5) * 10);

            wx = (X_Section_Base + (Random(24) - 1));

            wy = (Y_Section_Base + (Random(20) - 1));

            if(wx >= WORLD_WIDTH)
            {

                wx -= WORLD_WIDTH;

            }

            if(wy >= WORLD_HEIGHT)
            {

                wy -= WORLD_HEIGHT;

            }

            if(wx < 0)
            {

                wx += WORLD_WIDTH;

            }

            if(wy < 0)
            {

                wy += WORLD_HEIGHT;

            }

            if(
                (wx >= 3)
                &&
                (wy >= 2)
                &&
                (wx < 57)
                &&
                (wy < 37)
                &&
                (
                    (_world_maps[((0 * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] != tt_Ocean1)
                    ||
                    (Random(40) == 1)
                )
            )
            {
                
                for(itr2 = 0; itr2 < itr; itr2++)
                {
                    
                    if(Delta_XY_With_Wrap(wx, wy, _NODES[itr2].wx, _NODES[itr2].wy, WORLD_WIDTH) < 3)
                    {

                        goto somehow1;

                    }

                }

                _NODES[itr].wx = (int8_t)wx;
                _NODES[itr].wy = (int8_t)wy;
                _NODES[itr].wp = ARCANUS_PLANE;
                _NODES[itr].flags = 0;
                _NODES[itr].owner_idx = ST_UNDEFINED;
                _NODES[itr].power = (4 + Random(6));

                NEWG_CreateNodeAura__WIP(_NODES[itr].power, &_NODES[itr].Aura_Xs[0], &_NODES[itr].Aura_Ys[0], wx, wy);

                if(NODE_IsAuraUnique__WIP(itr) != ST_TRUE)
                {

                    NEWG_SetNodeType__WIP(_NODES[itr].power, &_NODES[itr].Aura_Xs[0], &_NODES[itr].Aura_Ys[0], _NODES[itr].wp, &_NODES[itr].type);

                    break;

                }

            }

        }

    }

    // ; generate 14 myrran nodes, complete with auras and
    // ; terrain type setting
    // ; 
    // ; BUG: checks the Arcanus tile for land instead

    for(itr = 16; itr < NUM_NODES; itr++)
    {

        while(1)
        {
somehow2:
            // X_Section_Base = ((itr / 5) * 12);
            X_Section_Base = (((itr - 20) % 5) * 12);

            Y_Section_Base = (((itr - 20) / 5) * 10);

            wx = (X_Section_Base + (Random(24) - 1));

            wy = (Y_Section_Base + (Random(40) - 1));

            if(wx >= WORLD_WIDTH)
            {

                wx -= WORLD_WIDTH;

            }

            if(wy >= WORLD_HEIGHT)
            {

                wy -= WORLD_HEIGHT;

            }

            if(wx < 0)
            {

                wx += WORLD_WIDTH;

            }

            if(wy < 0)
            {

                wy += WORLD_HEIGHT;

            }

            // ; ensure a minimum distance of 2 from top and bottom,
            // ; and 3 from the left and right map edges

            if(
                (wx >= 3)
                &&
                (wy >= 2)
                &&
                (wx < 57)
                &&
                (wy < 37)
                &&
                (
                    (_world_maps[((0 * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] != tt_Ocean1)  // ; BUG: this is the Arcanus tile
                    ||
                    (Random(25) == 1)
                )
            )
            {
                
                for(itr2 = 0; itr2 < itr; itr2++)
                {
                    
                    if(Delta_XY_With_Wrap(wx, wy, _NODES[itr2].wx, _NODES[itr2].wy, WORLD_WIDTH) < 3)
                    {

                        goto somehow2;

                    }

                }

                _NODES[itr].wx = (int8_t)wx;
                _NODES[itr].wy = (int8_t)wy;
                _NODES[itr].wp = MYRROR_PLANE;
                _NODES[itr].flags = 0;
                _NODES[itr].owner_idx = ST_UNDEFINED;
                _NODES[itr].power = (9 + Random(11));

                NEWG_CreateNodeAura__WIP(_NODES[itr].power, &_NODES[itr].Aura_Xs[0], &_NODES[itr].Aura_Ys[0], wx, wy);

                if(NODE_IsAuraUnique__WIP(itr) != ST_TRUE)
                {

                    NEWG_SetNodeType__WIP(_NODES[itr].power, &_NODES[itr].Aura_Xs[0], &_NODES[itr].Aura_Ys[0], _NODES[itr].wp, &_NODES[itr].type);

                    break;

                }

            }

        }

    }

}


// MGC o51p12
// drake178: NEWG_CreateNodeAura()
/*
; PATCHED / functionality moved into the node generator
; in the worldgen customizer
;
; generates and fills the passed byte arrays with the
; X and Y coordinates of the node's power aura
*/
/*

*/
void NEWG_CreateNodeAura__WIP(int16_t power, int8_t * Aura_Xs, int8_t * Aura_Ys, int16_t wx, int16_t wy)
{
    int16_t Invalid_Tile = 0;
    int16_t Random_Y = 0;
    int16_t Random_X = 0;
    int16_t Aura_Tile_Y = 0;
    int16_t Aura_Tile_X = 0;
    int16_t itr = 0;  // _SI_
    int16_t itr2 = 0;  // _DI_

    Aura_Xs[0] = (int8_t)wx;
    Aura_Ys[0] = (int8_t)wy;

    // ; pick tiles 2 to 8 from those adjacent to the node
    for(itr = 1; ((itr < power) && (itr < 9)); itr++)
    {

        while(1)
        {

            Random_X = (Random(4) - 1);

            Random_Y = (Random(4) - 1);

            Aura_Tile_X = (wx + TILE_Cardinal_XMod[Random_X]);

            Aura_Tile_Y = (wy + TILE_Cardinal_YMod[Random_Y]);

            Invalid_Tile = ST_FALSE;

            // ; invalidate the tile if it's already in the array

            for(itr2 = 0; itr2 < itr; itr2++)
            {

                if(
                    (Aura_Xs[itr2] == Aura_Tile_X)
                    &&
                    (Aura_Ys[itr2] == Aura_Tile_Y)
                )
                {

                    Invalid_Tile = ST_TRUE;

                }

            }

            // ; invalidate the tile if it's out of bounds

            if(
                (Aura_Tile_X <= 0)
                ||
                (Aura_Tile_Y <= 0)
                ||
                (Aura_Tile_X >= 59)
                ||
                (Aura_Tile_Y >= 39)
            )
            {

                Invalid_Tile = ST_TRUE;

            }

            if(Invalid_Tile != ST_TRUE)
            {

                break;

            }

        }

        Aura_Xs[itr] = (int8_t)Aura_Tile_X;

        Aura_Ys[itr] = (int8_t)Aura_Tile_Y;

    }

    for(itr = 9; itr < power; itr++)
    {

        while(1)
        {

            Random_X = (Random(4) - 1);

            Random_Y = (Random(4) - 1);

            Aura_Tile_X = (wx + (TILE_Cardinal_XMod[Random_X] * Random(2)));

            Aura_Tile_Y = (wy + (TILE_Cardinal_YMod[Random_Y] * Random(2)));

            Invalid_Tile = ST_FALSE;

            // ; invalidate the tile if it's already in the array

            for(itr2 = 0; itr2 < itr; itr2++)
            {

                if(
                    (Aura_Xs[itr2] == Aura_Tile_X)
                    &&
                    (Aura_Ys[itr2] == Aura_Tile_Y)
                )
                {

                    Invalid_Tile = ST_TRUE;

                }

            }

            // ; invalidate the tile if it's out of bounds

            if(
                (Aura_Tile_X <= 0)
                ||
                (Aura_Tile_Y <= 0)
                ||
                (Aura_Tile_X >= 59)
                ||
                (Aura_Tile_Y >= 39)
            )
            {

                Invalid_Tile = ST_TRUE;

            }

            if(Invalid_Tile != ST_TRUE)
            {

                break;

            }

        }

        Aura_Xs[itr] = (int8_t)Aura_Tile_X;

        Aura_Ys[itr] = (int8_t)Aura_Tile_Y;

    }

}


// MGC o51p13
// drake178: NODE_IsAuraUnique()
int16_t NODE_IsAuraUnique__WIP(int16_t node_idx)
{
    int16_t Aura_Ys[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Aura_Xs[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t power = 0;
    int16_t itr2 = 0;  // _SI_
    int16_t itr3 = 0;  // _DI_
    int16_t itr1 = 0;  // _CX_

    power = _NODES[node_idx].power;

    // ; copy the aura coordinates into the local array

    for(itr1 = 0; itr1 < power; itr1++)
    {

        Aura_Xs[itr1] = _NODES[node_idx].Aura_Xs[itr1];
        Aura_Ys[itr1] = _NODES[node_idx].Aura_Ys[itr1];

    }

    for(itr1 = 0; itr1 < node_idx; itr1++)
    {

        for(itr2 = 0; _NODES[itr1].power > itr2; itr2++)
        {
            
            for(itr3 = 0; itr3 < power; itr3++)
            {

                if(
                    (Aura_Xs[itr3] == _NODES[itr1].Aura_Xs[itr2])
                    &&
                    (Aura_Ys[itr3] == _NODES[itr1].Aura_Ys[itr2])
                )
                {

                    return ST_TRUE;

                }

            }

        }

    }

    return ST_FALSE;

}


// MGC o51p14
// drake178: NEWG_SetNodeType()
/*
; determines the type of the node using a series of
; random rolls, then sets it into the passed pointer,
; and changes the tile type of the first aura tile to
; the terrain associated with the node type
;
; before v1.1, this function used to consider the types
; of terrain in the node's aura, but now the calculated
; bias values are simply re-randomized instead
*/
/*

*/
void NEWG_SetNodeType__WIP(int16_t power, int8_t * Aura_Xs, int8_t * Aura_Ys, int16_t wp, int8_t * type)
{
    int16_t Nature_Bias = 0;
    int16_t Chaos_Bias = 0;
    int16_t Sorcery_Bias = 0;
    int16_t itr = 0;  // _DI_

    Sorcery_Bias = 0;
    Chaos_Bias = 0;
    Nature_Bias = 0;

    for(itr = 0; itr < power; itr++)
    {

        if(_world_maps[((wp * WORLD_SIZE) + (Aura_Ys[itr] * WORLD_WIDTH) + Aura_Xs[itr])] == tt_Ocean1)
        {

            Sorcery_Bias++;

        }

        if(
            (_world_maps[((wp * WORLD_SIZE) + (Aura_Ys[itr] * WORLD_WIDTH) + Aura_Xs[itr])] == tt_Mountain1)
            ||
            (_world_maps[((wp * WORLD_SIZE) + (Aura_Ys[itr] * WORLD_WIDTH) + Aura_Xs[itr])] == tt_Desert1)
        )
        {

            Chaos_Bias++;

        }


        if(
            (_world_maps[((wp * WORLD_SIZE) + (Aura_Ys[itr] * WORLD_WIDTH) + Aura_Xs[itr])] == tt_Grasslands1)
            ||
            (_world_maps[((wp * WORLD_SIZE) + (Aura_Ys[itr] * WORLD_WIDTH) + Aura_Xs[itr])] == tt_Forest1)
        )
        {

            Chaos_Bias++;

        }

    }

    Sorcery_Bias = Random(25);

    Chaos_Bias = Random(15);

    Nature_Bias = Random(15);
        
    if(
        (Chaos_Bias > Sorcery_Bias)
        &&
        (Chaos_Bias > Nature_Bias)
    )
    {

        _world_maps[((wp * WORLD_SIZE) + (Aura_Ys[itr] * WORLD_WIDTH) + Aura_Xs[itr])] = tt_ChaosNode;

        *type = nt_Chaos;

        TILE_SetLandMass__WIP(wp, Aura_Xs[0], Aura_Ys[0]);

    }
    else
    {

        if(Sorcery_Bias > Nature_Bias)
        {

            _world_maps[((wp * WORLD_SIZE) + (Aura_Ys[itr] * WORLD_WIDTH) + Aura_Xs[itr])] = tt_SorceryNode;

            *type = nt_Sorcery;

        }
        else
        {

            _world_maps[((wp * WORLD_SIZE) + (Aura_Ys[itr] * WORLD_WIDTH) + Aura_Xs[itr])] = tt_NatureNode;

            *type = nt_Nature;

        }

        TILE_SetLandMass__WIP(wp, Aura_Xs[0], Aura_Ys[0]);

    }

}


// MGC o51p15
// drake178: TILE_SetLandMass()
/*
; sets the landmass for the tile - either a new one if
; all surrounding tiles are ocean, or that of the first
; neighbouring one, consolidating continents if the
; tile is adjacent to more than one
*/
/*
sets landmass_idx in _landmass[]

*/
void TILE_SetLandMass__WIP(int16_t wp, int16_t wx, int16_t wy)
{
    int16_t Landmass_ID_Array[5] = { 0, 0, 0, 0, 0 };
    int16_t Conflicting_ID = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;
    int16_t Next_Adjacent_Index = 0;
    int16_t Array_Index = 0;
    int16_t First_Found_Landmass = 0;
    int16_t IDK = 0;  // _CX_

    First_Found_Landmass = 0;

    IDK = 0;

    // ; center tile
    if(GET_LANDMASS(wx,wy,wp) != 0)
    {
        Landmass_ID_Array[IDK] = GET_LANDMASS(wx,wy,wp);
        IDK++;
    }

    // ; tile up to the left
    if(GET_LANDMASS((wx - 1), (wy - 1), wp) != 0)
    {
        Landmass_ID_Array[IDK] = GET_LANDMASS((wx - 1), (wy - 1), wp);
        IDK++;
    }

    // ; tile up
    if(GET_LANDMASS(wx,(wy - 1),wp) != 0)
    {
        Landmass_ID_Array[IDK] = GET_LANDMASS(wx, (wy - 1), wp);
        IDK++;
    }

    // ; tile up to the right
    if(GET_LANDMASS((wx + 1), (wy - 1), wp) != 0)
    {
        Landmass_ID_Array[IDK] = GET_LANDMASS((wx + 1), (wy - 1), wp);
        IDK++;
    }

    // ; tile to the left
    if(GET_LANDMASS((wx - 1), wy, wp) != 0)
    {
        Landmass_ID_Array[IDK] = GET_LANDMASS((wx - 1), wy, wp);
        IDK++;
    }

    // ; tile to the right
    if(GET_LANDMASS((wx + 1), wy, wp) != 0)
    {
        Landmass_ID_Array[IDK] = GET_LANDMASS((wx + 1), wy, wp);
        IDK++;
    }

    // ; tile down to the left
    if(GET_LANDMASS((wx - 1), (wy + 1), wp) != 0)
    {
        Landmass_ID_Array[IDK] = GET_LANDMASS((wx - 1), (wy + 1), wp);
        IDK++;
    }

    // ; tile down
    if(GET_LANDMASS(wx, (wy + 1), wp) != 0)
    {
        Landmass_ID_Array[IDK] = GET_LANDMASS(wx, (wy + 1), wp);
        IDK++;
    }

    // ; tile down to the right
    if(GET_LANDMASS((wx + 1), (wy + 1), wp) != 0)
    {
        Landmass_ID_Array[IDK] = GET_LANDMASS((wx + 1), (wy + 1), wp);
        IDK++;
    }

    if(IDK != 0)
    {

        // ; if the tile is bordered by multiple different land
        // ; masses, consolidate them all into one

        SET_LANDMASS((wx - 1), wy, wp, (uint8_t)NEWG_LandmassCount);

        NEWG_LandmassCount++;

    }
    else
    {

        First_Found_Landmass = Landmass_ID_Array[0];

        for(Array_Index = 1; Array_Index < IDK; Array_Index++)
        {

            if(Landmass_ID_Array[Array_Index] != First_Found_Landmass)
            {

                // ; convert all instances of the conflicting ID in the
                // ; local array into the first found one

                Conflicting_ID = Landmass_ID_Array[Array_Index];

                for(Next_Adjacent_Index = Array_Index; Next_Adjacent_Index < IDK; Next_Adjacent_Index++)
                {

                    if(Landmass_ID_Array[Next_Adjacent_Index] == Conflicting_ID)
                    {

                        Landmass_ID_Array[Next_Adjacent_Index] = First_Found_Landmass;

                    }

                }

                // ; convert all instances of the conflicting ID on the
                // ; world map into the first found one

                for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy++)
                {

                    for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
                    {

                        if(GET_LANDMASS((itr_wx + 1), (itr_wy + 1), wp) == Conflicting_ID)
                        {

                            SET_LANDMASS((itr_wx + 1), (itr_wy + 1), wp, (uint8_t)First_Found_Landmass);

                        }

                    }

                }

            }

        }

        SET_LANDMASS((wx - 1), wy, wp, (uint8_t)First_Found_Landmass);

    }

}


// MGC o51p16
// drake178: NEWG_CreateEZs()
/*
; populates the encounter zone table
*/
/*

*/
void Generate_Lairs(void)
{
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t itr2 = 0;
    int16_t wp = 0;
    int16_t Strong_Lair_Amt = 0;
    int16_t itr = 0;  // _SI_
    int16_t IDK = 0;  // _DI_

    Strong_Lair_Amt = 25;

    // create the 6 Tower of Wizardry encounters on Arcanus

    for(itr = 0; itr < NUM_TOWERS; itr++)
    {

        NEWG_CreateEncounter__WIP(itr, ARCANUS_PLANE, _TOWERS[itr].wx, _TOWERS[itr].wy, 3, 0, (650 + (50 * Random(11))));

    }

    // ; copy the Arcanus tower encounters into a separate set
    // ; on Myrror (same records otherwise)

    for(itr = NUM_TOWERS; itr < (2 * NUM_TOWERS); itr++)
    {

        memcpy(&_LAIRS[itr], &_LAIRS[(itr - NUM_TOWERS)], sizeof(struct s_LAIR));

        _LAIRS[itr].wp = MYRROR_PLANE;

    }

    // ; create the 30 node encounters

    for(itr = 0; itr < NUM_NODES; itr++)
    {

        IDK = (((((_NODES[itr].power - 1) * _magic) / 2) / 6) + 1);

        SETMAX(IDK, 4);

        SETMIN(IDK, 1);

        NEWG_CreateEncounter__WIP(((2 * NUM_TOWERS) + itr), _NODES[itr].wp, _NODES[itr].wx, _NODES[itr].wy, IDK, (_NODES[itr].type + 1), ((((_NODES[itr].power * _NODES[itr].power) * (4 + Random(11))) * (_magic + 1)) / 2));

    }

    // ; create 25 random strong encounters on non-ocean tiles

    for(itr = 0; itr < Strong_Lair_Amt; itr++)
    {

        wp = (Random(2) - 1);

        wx = (2 + Random(54));

        wy = (2 + Random(34));

        if(TILE_IsOcean(wx, wy, wp) != ST_TRUE)
        {

            for(itr2 = 0; ((itr + 42) > itr2); itr2++)
            {

                if(Delta_XY_With_Wrap(wx, wy, _LAIRS[itr2].wx, _LAIRS[itr2].wy, WORLD_WIDTH) < 2)
                {
                    break;
                }

            }

            if(wp == ARCANUS_PLANE)
            {

                IDK = (50 + (Random(29) * 50));

            }
            else
            {

                IDK = (100 + (Random(24) * 100));

            }

            NEWG_CreateEncounter__WIP((((2 * NUM_TOWERS) + NUM_NODES) + itr), wp, wx, wy, IDK, 4, IDK);

        }

    }

    // ; create 32 weak random encounters on non-ocean tiles

    for(itr = 0; itr < 32; itr++)
    {

        wp = (Random(2) - 1);

        wx = (2 + Random(54));

        wy = (2 + Random(34));

        if(TILE_IsOcean(wx, wy, wp) != ST_TRUE)
        {

            for(itr2 = 0; ((itr + (((2 * NUM_TOWERS) + NUM_NODES) + Strong_Lair_Amt)) > itr2); itr2++)
            {

                if(Delta_XY_With_Wrap(wx, wy, _LAIRS[itr2].wx, _LAIRS[itr2].wy, WORLD_WIDTH) < 2)
                {
                    break;
                }

            }

            if(wp == ARCANUS_PLANE)
            {

                IDK = (Random(10) * 10);

            }
            else
            {

                IDK = (Random(20) * 10);

            }

            NEWG_CreateEncounter__WIP((((2 * NUM_TOWERS) + NUM_NODES + Strong_Lair_Amt) + itr), wp, wx, wy, IDK, 4, IDK);

        }

    }

    // ; mark the last 3 records in the table as empty

    for(itr = ((2 * NUM_TOWERS) + NUM_NODES + Strong_Lair_Amt + 32); itr < ((2 * NUM_TOWERS) + NUM_NODES + Strong_Lair_Amt + 32 + 3); itr++)
    {

        _LAIRS[itr].intact = ST_FALSE;

    }

}


// MGC o51p17
// drake178: NEWG_CreateEncounter()
/*
; clears and then populates the specified encounter
; record with guardians and treasure based on zone
; type, altering the passed budget based on the plane
; and the difficulty setting
;
; BUG: has multiple smaller bugs and inconsistencies
*/
/*

*/
void NEWG_CreateEncounter__WIP(int16_t lair_idx, int16_t wp, int16_t wx, int16_t wy, int16_t UU, int16_t lair_type, int16_t budget)
{
    int16_t Tries = 0;
    int16_t Divided_Budget = 0;
    int16_t Highest_Cost = 0;
    int16_t Picked_Unit_Type = 0;
    int16_t Price = 0;
    int16_t race_type = 0;
    int16_t unit_type = 0;
    int16_t IDK;  // _DI_

    budget = ((budget * (_difficulty + 1)) / 4);  // 

    _LAIRS[lair_idx].wx = (int8_t)wx;

    _LAIRS[lair_idx].wy = (int8_t)wy;

    _LAIRS[lair_idx].wp = (int8_t)wp;

    _LAIRS[lair_idx].intact = ST_TRUE;

// FFFFFFFF ; enum EZ_Types
// FFFFFFFF EZ_Tower  = 0
// FFFFFFFF EZ_Chaos_Node  = 1
// FFFFFFFF EZ_Nature_Node  = 2
// FFFFFFFF EZ_Sorcery_Node  = 3
// FFFFFFFF EZ_Cave  = 4
// FFFFFFFF EZ_Dungeon  = 5
// FFFFFFFF EZ_Ancient_Temple  = 6
// FFFFFFFF EZ_Keep  = 7
// FFFFFFFF EZ_Monster_Lair  = 8
// FFFFFFFF EZ_Ruins  = 9
// FFFFFFFF EZ_Fallen_Temple  = 10
// enum e_LAIR_TYPE
// {
//     lt_Tower            =  1,
//     lt_Chaos_Node       =  2,
//     lt_Nature_Node      =  3,
//     lt_Sorcery_Node     =  4,
//     lt_Cave             =  5,
//     lt_Dungeon          =  6,
//     lt_Ancient_Temple   =  7,
//     lt_Keep             =  8,
//     lt_Monster_Lair     =  9,
//     lt_Ruins            = 10,
//     lt_Fallen_Temple    = 11
// };

    switch(lair_type)
    {
        case 0:
        {

            _LAIRS[lair_idx].type = lt_Tower;

        } break;
        case 1:
        {

            _LAIRS[lair_idx].type = lt_Sorcery_Node;

        } break;
        case 2:
        {

            _LAIRS[lair_idx].type = lt_Nature_Node;

        } break;
        case 3:
        {

            _LAIRS[lair_idx].type = lt_Chaos_Node;

        } break;
        default:
        {

            _LAIRS[lair_idx].type = (lt_Cave + (Random(7) - 1));

        } break;

    }

    _LAIRS[lair_idx].Loot_Gold = 0;

    _LAIRS[lair_idx].Loot_Mana = 0;

    _LAIRS[lair_idx].Spell_n_Special = 0;

    _LAIRS[lair_idx].Item_Count = 0;

    _LAIRS[lair_idx].Misc_Flags = 0;

    _LAIRS[lair_idx].guard1_unit_type = 0;

    _LAIRS[lair_idx].guard2_unit_type = 0;

    _LAIRS[lair_idx].guard1_count = 0;

    _LAIRS[lair_idx].guard2_count = 0;

    if(lair_type == 1)
    {

            race_type = rt_Sorcery;

    }
    else if(lair_type == 3)
    {

            race_type = rt_Chaos;

    }
    else if(lair_type == 2)
    {

            race_type = rt_Nature;
        
    }
    else if(lair_type == 0)  // New Game Lair Type - Tower
    {

        // ; assign a race based on a random roll of 6, with 1
        // ; and 2 corresponding to Death, and the rest of the
        // ; numbers to one other realm each

        switch((Random(6) - 1))
        {
            case 0:
            case 1:
            {

                race_type = rt_Death;

            } break;
            case 2:
            {

                race_type = rt_Chaos;

            } break;
            case 3:
            {
                
                race_type = rt_Life;

            } break;
            case 4:
            {

                race_type = rt_Nature;

            }
            case 5:
            {
                
                race_type = rt_Sorcery;

            } break;
        }

    }
    else if(
        (_LAIRS[lair_idx].type = lt_Ancient_Temple)
        ||
        (_LAIRS[lair_idx].type = lt_Ruins)
        ||
        (_LAIRS[lair_idx].type = lt_Fallen_Temple)
    )
    {
        
        // ; assign a race based on a random roll of 4, with 1
        // ; to 3 corresponding to Death, and 4 to Life

        switch((Random(4) - 1))
        {
            case 0:
            case 1:
            case 2:
            {
                race_type = rt_Death;
            } break;
            case 3:
            {
                race_type = rt_Life;
            } break;

        }

    }
    else
    {

        // ; assign a race based on a random roll of 5, with 1
        // ; and 2 corresponding to Death, 3 and 4 to Chaos, and
        // ; 5 to Nature
        // ; 
        // ; BUG: the first four cases all fall through to the
        // ; final one, resulting in 100% Nature creatures

        switch((Random(5) - 1))
        {
            case 0: { race_type = rt_Death;  }  // ; BUG: falls through all the way to the last case
            case 1: { race_type = rt_Death;  }  // ; BUG: falls through all the way to the last case
            case 2: { race_type = rt_Chaos;  }  // ; BUG: falls through all the way to the last case
            case 3: { race_type = rt_Chaos;  }  // ; BUG: falls through all the way to the last case
            case 4: { race_type = rt_Nature; }  // ; BUG: falls through all the way to the last case
        }

    }

    Tries = 0;
    while(Tries < 200)
    {

        Divided_Budget = (budget / Random(4));

        Highest_Cost = 0;

        Picked_Unit_Type = ST_UNDEFINED;

        // ; loop through the units to try and find the one with
        // ; the highest cost that is under the divided budget,
        // ; ignoring those that do not match the realm or have
        // ; transport capability

        for(unit_type = 150; unit_type < NUM_UNIT_TYPES; unit_type++)
        {

            if(
                (_unit_type_table[unit_type].race_type == race_type)
                &&
                (_unit_type_table[unit_type].Transport == 0)
                &&
                (_unit_type_table[unit_type].cost > Highest_Cost)
                &&
                (_unit_type_table[unit_type].cost < Divided_Budget)
            )
            {

                Highest_Cost = _unit_type_table[unit_type].cost;

                Picked_Unit_Type = unit_type;

            }

        }

        Tries++;

        if(
            (Picked_Unit_Type != ST_UNDEFINED)
            ||
            (Tries >= 200)
        )
        {

            if(Tries < 200)
            {

                _LAIRS[lair_idx].guard1_unit_type = (uint8_t)Picked_Unit_Type;

                _LAIRS[lair_idx].guard1_count = (budget / _unit_type_table[_LAIRS[lair_idx].guard1_unit_type].cost);

                if(
                    (_LAIRS[lair_idx].guard1_count > 1)
                    &&
                    (Random(2) == 1)
                )
                {

                    _LAIRS[lair_idx].guard1_count -= 1;

                }

                SETMAX(_LAIRS[lair_idx].guard1_count, 8);

                budget -= (_unit_type_table[_LAIRS[lair_idx].guard1_unit_type].cost * _LAIRS[lair_idx].guard1_count);

            }
        }

    }  /* while(Tries < 200) */



    Tries = 0;

    if(
        (_LAIRS[lair_idx].guard1_count != 9)  // ; conflicting condition - will always jump
        &&
        (_LAIRS[lair_idx].guard1_count != 0)
    )
    {

        while(Tries < 200)
        {

            Divided_Budget = (budget / Random((10 - _LAIRS[lair_idx].guard1_count)));

            Highest_Cost = 0;

            Picked_Unit_Type = ST_UNDEFINED;

            // ; loop through the units to try and find the one with
            // ; the highest cost that is under the divided budget,
            // ; ignoring those that do not match the realm, have
            // ; transport capability, or are already featured as the
            // ; primary guardians

            for(unit_type = 150; unit_type < NUM_UNIT_TYPES; unit_type++)
            {

                if(
                    (_unit_type_table[unit_type].race_type == race_type)
                    &&
                    (_unit_type_table[unit_type].Transport == 0)
                    &&
                    (_unit_type_table[unit_type].cost > Highest_Cost)
                    &&
                    (_unit_type_table[unit_type].cost < Divided_Budget)
                )
                {

                    if(_LAIRS[lair_idx].guard1_unit_type != unit_type)
                    {

                        Highest_Cost = _unit_type_table[unit_type].cost;

                        Picked_Unit_Type = unit_type;

                    }

                }

            }

            Tries++;

            if(
                (Picked_Unit_Type != ST_UNDEFINED)
                ||
                (Tries >= 200)
            )
            {

                if(Tries < 200)
                {

                    _LAIRS[lair_idx].guard2_unit_type = (uint8_t)Picked_Unit_Type;

                    _LAIRS[lair_idx].guard2_count = (budget / _unit_type_table[_LAIRS[lair_idx].guard2_unit_type].cost);

                    if((_LAIRS[lair_idx].guard1_count + _LAIRS[lair_idx].guard2_count) > 9)
                    {

                        _LAIRS[lair_idx].guard2_count = (9 - _LAIRS[lair_idx].guard1_count);

                    }

                }

            }

        }

    }  /* while(Tries < 200) */



    if(_LAIRS[lair_idx].guard1_unit_type == 0)
    {

        _LAIRS[lair_idx].guard1_count = 0;

    }

    if(_LAIRS[lair_idx].guard2_unit_type == 0)
    {

        _LAIRS[lair_idx].guard2_count = 0;

    }

    // ; PATCHED here to make the more expensive creature
    // ; always be the primary guardian

    IDK = (_unit_type_table[_LAIRS[lair_idx].guard1_unit_type].cost * _LAIRS[lair_idx].guard1_count);

    // ; BUG: this is not always the smaller part of the
    // ; treasure budget

    IDK += ((_unit_type_table[_LAIRS[lair_idx].guard2_unit_type].cost * _LAIRS[lair_idx].guard2_count) / 2);

    if(_difficulty != god_Impossible)
    {

        IDK = ((IDK * (_difficulty + 1)) / 4);

    }

    if(wp == ARCANUS_PLANE)
    {

        IDK = ((IDK * (49 + Random(76))) / 100);

    }
    else
    {

        IDK = ((IDK * (75 + Random(100))) / 100);

    }

    SETMIN(IDK, 50);

    // ; PATCHED here to prevent high-budget sites from
    // ; awarding only a single special pick (NOT included in
    // ; the Community Patch)

    if(lair_type == 0)  // New Game Lair Type - Temple
    {

        Price = Random(4);

        IDK -= (((Price * Price) * 50) - 100);

        _LAIRS[lair_idx].Spell_n_Special = (int8_t)Price;

    }

    // ; PATCHED here to prevent high-budget sites from
    // ; awarding only a single special pick (NOT included in
    // ; the Community Patch)

    if(IDK >= 50)
    {

        switch(Random(15))
        {
            case 0:
            {
                // DNE
            } break;
            case 1:
            {
                Price = (Random(20) * 10);
                SETMAX(Price, IDK);
                Price = ((Price / 10) * 10);
                IDK -= 200;
                _LAIRS[lair_idx].Loot_Gold = Price;
            }
            case 2:
            {

            } break;
            case 3:
            {
                Price = (Random(20) * 10);
                SETMAX(Price, IDK);
                Price = ((Price / 10) * 10);
                IDK -= 200;
                _LAIRS[lair_idx].Loot_Mana = Price;
            } break;
            case 4:
            {

            } break;
            case 5:
            {

                // ; magical item
                // ; 
                // ; qualify:  300
                // ; value:    400 - 3000
                // ; max:      3
                // ; rounded down to the nearest 10 that fits the budget

                if(
                    (_LAIRS[lair_idx].Item_Count < 3)
                    &&
                    (IDK >= 300)
                )
                {

                    Price = (300 + (Random(27) * 100));

                    SETMAX(Price, IDK);

                    Price = ((Price / 10) * 10);

                    IDK -= Price;

                    _LAIRS[lair_idx].Item_Values[_LAIRS[lair_idx].Item_Count] = Price;

                    _LAIRS[lair_idx].Item_Count += 1;

                }

            } break;
            case 6:
            {

            } break;
            case 7:
            {

            } break;
            case 8:
            {

            } break;
            case 9:
            {

            } break;
            case 10:
            {

                // ; prisoner hero
                // ; 
                // ; qualify:  400
                // ; value:    1000
                // ; max:      1

                if(
                    (IDK >= 400)
                    &&
                    (_LAIRS[lair_idx].Misc_Flags == 0)
                )
                {

                    _LAIRS[lair_idx].Misc_Flags = 1;

                    IDK -= 1000;

                }

            } break;
            case 11:
            {

                // ; spell
                // ; 
                // ; qualify:  rarity * rarity * 50
                // ; value:    rarity * rarity * 50
                // ; max:      1
                // ; if there is already a spell in the hoard, the new
                // ; rarity is added to the old one, allowing higher
                // ; rarities to be awarded cheaper than they should be

                // ; PATCHED here to upgrade spell treasure properly and
                // ; award the highest possible rarity instead of
                // ; discarding if the rolled price is too high

                Price = Random(4);

                if(
                    (((Price * Price) * 50) < IDK)
                    &&
                    (_LAIRS[lair_idx].Spell_n_Special < 4)
                )
                {

                    IDK -= ((Price * Price) * 50);

                    _LAIRS[lair_idx].Spell_n_Special += Price;

                    SETMAX(_LAIRS[lair_idx].Spell_n_Special, 4);
                    
                }

            } break;
            case 12:
            {

            } break;
            case 13:
            {

            } break;
            case 14:
            {

                // ; spellbook / retort
                // ; 
                // ; qualify:  1000
                // ; value:    3000
                // ; max:      2
                // ; always 2 if there's at least 2k budget remaining
                // ; (discards all other treasure in the end)

                Price = Random(4);

                if(
                    (Price == 1)
                    &&
                    (IDK >= 3000)
                )
                {

                    _LAIRS[lair_idx].Spell_n_Special = 6;

                    IDK -= 3000;

                }
                else if(IDK >= 2000)
                {

                    _LAIRS[lair_idx].Spell_n_Special = 6;

                    IDK -= 3000;

                }
                else if(IDK >= 1000)
                {

                    _LAIRS[lair_idx].Spell_n_Special = 5;

                    IDK -= 3000;

                }

            } break;
            case 15:
            {

            } break;


        }

    }

    // ; discard all other treasure if there is a spellbook /
    // ; retort
    // ; 
    // ; INCONSISTENT: can result in a single special instead
    // ; of 2 if enough of the total has been wasted on these

    if(_LAIRS[lair_idx].Spell_n_Special > 4)
    {

        _LAIRS[lair_idx].Loot_Gold = 0;
        
        _LAIRS[lair_idx].Loot_Mana = 0;
        
        _LAIRS[lair_idx].Item_Count = 0;
        
        _LAIRS[lair_idx].Misc_Flags = 0;  // clear Prisoner

    }

}


// MGC o51p18
// drake178: UU_UNIT_RandomRacial()
// UU_UNIT_RandomRacial()

// MGC o51p19
// drake178: NEWG_SetDeserts()
/*

*/
/*

*/
static void NEWG_SetDeserts__STUB(void)
{



}


// MGC o51p20
// drake178: NEWG_CreateShores()
/*
; creates shores based on the surrounding land type
; tiles, then uses the same concept to consolidate
; mountain and hill ranges, as well as replace tundra
; tiles
;
; WARNING: the hills and mountains would generate a
;  break at the X wrap boundary
; BUG: turns certain single tile hills into grasslands
*/
/*

*/
void NEWG_CreateShores__STUB(void)
{
// TERRTYPE_Pointer= word ptr -0Ah
// Adjacent_X= word ptr -8
// Tile_Border_Flags= word ptr -6
// wp= word ptr -4
// X_Coord= word ptr -2
// 
// TERRTYPE_Pointer = Near_Allocate_First(2560);
// 
// LBX_Load_Data_Static(cnst_TERRTYPE_File, 0, TERRTYPE_Pointer, 0, 5, 512);
// 
// wp = 0;

// ; create shore tiles based on the ocean - non-ocean
// ; terrain borders using the first 512 byte record of
// ; TERRTYPE.LBX

// ; create tundra tiles based on the tundra - non-tundra
// ; borders using the first record of TERRTYPE.LBX, and
// ; adding 600 to the ocean tile type

// ; consolidate neighbouring hill tiles into hill ranges
// ; using the second record of TERRTYPE.LBX (+10h)
// ; 
// ; BUG: turns single tile hills into grasslands, as
// ; _1Mountain1 + 10h = _Grasslands4 rather than _1Hills1
// ; WARNING: would create breaks at the X wrap boundary
// ; if land tiles would exist there

}

// MGC o51p21
// drake178: NEWG_RandomizeTiles()
/*
*/
/*
*/
void NEWG_RandomizeTiles__STUB(void)
{



}


// MGC o51p22
// drake178: NEWG_CreateRiver()
/*

*/
/*

*/
int16_t NEWG_CreateRiver__STUB(int16_t wp)
{

    return ST_FALSE;

}


// MGC o51p23
// drake178: NEWG_SetRiverTiles()
void NEWG_SetRiverTiles__STUB(int16_t wp)
/*

*/
/*

*/
{



}


// MGC o51p24
// drake178: NEWG_CreateNeutrals()
/*
; creates up to 15 neutral cities on the selected
; plane
;
; BUG: contains a number of minor errors and
;  inconsistencies, most of which have no impact at all
*/
/*
similar mess as Generate_Home_City()

*/
void Generate_Neutral_Cities__WIP(int16_t wp)
{
    int16_t location_is_forest_square = 0;
    int16_t Tries = 0;
    int16_t Best_Ranged_Unit = 0;
    int16_t Best_Melee_Unit = 0;
    int16_t itr3 = 0;
    int16_t Invalid = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t itr2 = 0;  // _SI_
    int16_t itr1 = 0;  // _DI_

    Tries = 0;

    // ; fill the default continental race table with random
    // ; values corresponding to the selected plane,
    // ; excluding high elves from Arcanus and giving high
    // ; men a double chance instead

    for(itr1 = 1; itr1 < 200; itr1++)
    {

        if(wp == ARCANUS_PLANE)
        {

            // ; select a random Arcanian race except high elves, with
            // ; high men having double chance, and save its index
            // ; into the element of the array corresponding to di

            // BUGBUG  cmp     bx, 12                            ; switch 13 cases
            switch((Random(9) - 1))
            {
                case 0: { NEWG_Landmass_Races[itr1] = rt_Barbarian; } break;
                case 1: { NEWG_Landmass_Races[itr1] = rt_Gnoll;     } break;
                case 2: { NEWG_Landmass_Races[itr1] = rt_Halfling;  } break;
                case 3: { NEWG_Landmass_Races[itr1] = rt_High_Men;  } break;  // no forest check, no High Elves
                case 4: { NEWG_Landmass_Races[itr1] = rt_High_Men;  } break;
                case 5: { NEWG_Landmass_Races[itr1] = rt_Klackon;   } break;
                case 6: { NEWG_Landmass_Races[itr1] = rt_Lizardman; } break;
                case 7: { NEWG_Landmass_Races[itr1] = rt_Nomad;     } break;
                case 8: { NEWG_Landmass_Races[itr1] = rt_Orc;       } break;
                // WTF?
                case  9: { NEWG_Landmass_Races[itr1] = rt_Barbarian; } break;
                case 10: { NEWG_Landmass_Races[itr1] = rt_High_Men;  } break;
                case 11: { NEWG_Landmass_Races[itr1] = rt_Nomad;     } break;
                case 12: { NEWG_Landmass_Races[itr1] = rt_High_Men;  } break;
            }

        }
        else  /* MYRROR_PLANE */
        {

            // ; select a random Myrran race and save its index into
            // ; the element of the array corresponding to di

            switch((Random(5) - 1))
            {
                case 0: { NEWG_Landmass_Races[itr1] = rt_Beastmen;  } break;
                case 1: { NEWG_Landmass_Races[itr1] = rt_Dark_Elf;  } break;
                case 2: { NEWG_Landmass_Races[itr1] = rt_Draconian; } break;
                case 3: { NEWG_Landmass_Races[itr1] = rt_Dwarf;     } break;
                case 4: { NEWG_Landmass_Races[itr1] = rt_Troll;     } break;
            }

        }

    }

    for(itr1 = 0; itr1 < 15; itr1++)
    {

        if(Tries < 1000)
        {

// jump back here if the location is invalid
Loop_Location_2:
// ...same as in Generate_Home_City()

            Tries++;

            wx = (3 + Random(54));

            wy = (3 + Random(34));

            Invalid = ST_FALSE;

            if(TERRAIN_TYPE(wx, wy, wp) == tt_Ocean1)
            {

                Invalid = ST_TRUE;

            }

            if(
                (TERRAIN_TYPE(wx, wy, wp) >= tt_Shore1_Fst)  // ; there are no such tiles yet at this stage
                ||
                (TERRAIN_TYPE(wx, wy, wp) <= tt_Shore1_Lst)  // ; there are no such tiles yet at this stage
            )
            {

                Invalid = ST_TRUE;

            }

            // ; invalidate if less than 5 tiles away from a capital

            // ; invalidate if less than 4 tiles away from an already
            // ; created city

            // ; invalidate of less than 4 tiles away from a node
            // ; 
            // ; the encounter exclusion below would also catch these

            // ; invalidate if less than 4 tiles away from a tower
            // ; 
            // ; the encounter exclusion below would also catch these

            // ; invalidate if less than 4 tiles away from an
            // ; encounter zone - including nodes and towers

            if(
                (Tries < 1000)
                &&
                (Invalid == ST_TRUE)
            )
            {

                goto Loop_Location_2;

            }

            location_is_forest_square = Square_Is_Forest_NewGame(wx, wy, wp);

            if(wp == ARCANUS_PLANE)
            {

                // ; select a random Arcanian race, and assign it to the
                // ; next record in the city table, replacing high elves
                // ; with high men if the tile is not a forest

                // BUGBUG  cmp     bx, 12                            ; switch 13 cases
                switch((Random(9) - 1))
                {
                    case 0: { _CITIES[_cities].race = rt_Barbarian; } break;
                    case 1: { _CITIES[_cities].race = rt_Gnoll;     } break;
                    case 2: { _CITIES[_cities].race = rt_Halfling;  } break;
                    // case 3: { _CITIES[_cities].race = rt_High_Elf;  } break;
                    case 3: {
                        if(location_is_forest_square == ST_TRUE)
                        {
                            _CITIES[_cities].race = rt_High_Elf;
                        }
                        else
                        {
                            _CITIES[_cities].race = rt_High_Men;
                        }
                    } break;
                    case 4: { _CITIES[_cities].race = rt_High_Men;  } break;
                    case 5: { _CITIES[_cities].race = rt_Klackon;   } break;
                    case 6: { _CITIES[_cities].race = rt_Lizardman; } break;
                    case 7: { _CITIES[_cities].race = rt_Nomad;     } break;
                    case 8: { _CITIES[_cities].race = rt_Orc;       } break;
                    // WTF?
                    case  9: { _CITIES[_cities].race = rt_Barbarian; } break;
                    case 10: { _CITIES[_cities].race = rt_High_Men;  } break;
                    case 11: { _CITIES[_cities].race = rt_Nomad;     } break;
                    case 12: { _CITIES[_cities].race = rt_High_Men;  } break;
                }

            }
            else
            {

                // ; select a random Myrran race, and assign it to the
                // ; next record in the city table

                switch((Random(5) - 1))
                {
                    case 0: { _CITIES[_cities].race = rt_Beastmen;  } break;
                    case 1: { _CITIES[_cities].race = rt_Dark_Elf;  } break;
                    case 2: { _CITIES[_cities].race = rt_Draconian; } break;
                    case 3: { _CITIES[_cities].race = rt_Dwarf;     } break;
                    case 4: { _CITIES[_cities].race = rt_Troll;     } break;
                }

            }

            if(Random(4) > 1)
            {

                _CITIES[_cities].race = (int8_t)NEWG_Landmass_Races[(_landmasses[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) +wx)])];

            }

            _CITIES[_cities].wx = (int8_t)wx;

            _CITIES[_cities].wy = (int8_t)wy;
            
            _CITIES[_cities].wp = (int8_t)wp;
            
            _CITIES[_cities].owner_idx = NEUTRAL_PLAYER_IDX;

            // ; BUG: ignores terrain
            _CITIES[_cities].population = (1 +  ((_difficulty + 1) / 3) + Random(4));

            if(
                (_difficulty > god_Normal)
                &&
                (Random(5) == 1)
            )
            {

                // ; BUG: ignores terrain
                _CITIES[_cities].population = (((_difficulty + 1) / 3) + Random(10));

            }

            _CITIES[_cities].size = (_CITIES[_cities].population / 4);

            _CITIES[_cities].bldg_cnt = 0;

            _CITIES[_cities].enchantments[WALL_OF_DARKNESS] = ST_FALSE;
            _CITIES[_cities].enchantments[WALL_OF_FIRE] = ST_FALSE;
            _CITIES[_cities].enchantments[CHAOS_RIFT] = ST_FALSE;
            _CITIES[_cities].enchantments[DARK_RITUALS] = ST_FALSE;
            _CITIES[_cities].enchantments[EVIL_PRESENCE] = ST_FALSE;
            _CITIES[_cities].enchantments[CURSED_LANDS] = ST_FALSE;
            _CITIES[_cities].enchantments[PESTILENCE] = ST_FALSE;
            _CITIES[_cities].enchantments[CLOUD_OF_SHADOW] = ST_FALSE;
            _CITIES[_cities].enchantments[ALTAR_OF_BATTLE] = ST_FALSE;
            _CITIES[_cities].enchantments[FAMINE] = ST_FALSE;
            _CITIES[_cities].enchantments[FLYING_FORTRESS] = ST_FALSE;
            _CITIES[_cities].enchantments[DEATH_WARD] = ST_FALSE;
            _CITIES[_cities].enchantments[CHAOS_WARD] = ST_FALSE;
            _CITIES[_cities].enchantments[LIFE_WARD] = ST_FALSE;
            _CITIES[_cities].enchantments[SORCERY_WARD] = ST_FALSE;
            _CITIES[_cities].enchantments[NATURE_WARD] = ST_FALSE;
            _CITIES[_cities].enchantments[NATURES_EYE] = ST_FALSE;
            _CITIES[_cities].enchantments[EARTH_GATE] = ST_FALSE;
            _CITIES[_cities].enchantments[STREAM_OF_LIFE] = ST_FALSE;
            _CITIES[_cities].enchantments[GAIAS_BLESSING] = ST_FALSE;
            _CITIES[_cities].enchantments[INSPIRATIONS] = ST_FALSE;
            _CITIES[_cities].enchantments[PROSPERITY] = ST_FALSE;
            _CITIES[_cities].enchantments[ASTRAL_GATE] = ST_FALSE;
            _CITIES[_cities].enchantments[HEAVENLY_LIGHT] = ST_FALSE;
            _CITIES[_cities].enchantments[CONSECRATION] = ST_FALSE;
            _CITIES[_cities].enchantments[NIGHTSHADE] = ST_FALSE;

            _CITIES[_cities].construction = bt_AUTOBUILD;

            // mark all buildings as not built

            for(itr2 = 0; itr2 < NUM_BUILDINGS; itr2++)
            {
                _CITIES[_cities].bldg_status[itr2] = bs_NotBuilt;
            }


            // ; add buildings based on population
            _CITIES[_cities].bldg_status[bt_NONE] = bs_Replaced;

            switch(_CITIES[_cities].population)  // - 2;  switch 24 cases
            {
                case 24:
                case 23:
                case 22:
                case 21:
                case 20:
                case 19:
                case 18:
                case 17:
                case 16:
                case 15:
                case 14:
                case 13:
                case 12: { _CITIES[_cities].bldg_status[bt_Shrine] = bs_Built; }
                case 11: { _CITIES[_cities].bldg_status[bt_ArmorersGuild] = bs_Built; }
                case 10: { _CITIES[_cities].bldg_status[bt_FightersGuild] = bs_Built; }
                case  9:
                case  8: { _CITIES[_cities].bldg_status[bt_CityWalls] = bs_Built; }
                case  7:
                case  6: { _CITIES[_cities].bldg_status[bt_Stable] = bs_Built; }
                case  5: { _CITIES[_cities].bldg_status[bt_Granary] = bs_Built; }
                case  4:
                case  3: { _CITIES[_cities].bldg_status[bt_Armory] = bs_Built; }
                case  2: { _CITIES[_cities].bldg_status[bt_BuildersHall] = bs_Built; }
                case  1: { _CITIES[_cities].bldg_status[bt_Smithy] = bs_Built; }
                case  0: { _CITIES[_cities].bldg_status[bt_Barracks] = bs_Built; }
            }

            // ; remove any buildings prohibited by race

            for(itr3 = 0; itr3 <= _race_type_table[_CITIES[_cities].race].cant_build_count; itr3++)
            {

                _CITIES[_cities].bldg_status[_race_type_table[_CITIES[_cities].race].cant_build[itr3]] = bs_NotBuilt;

            }

            // ; remove any buildings with unbuilt prerequisites
            // ; 
            // ; BUG: buildings with a terrain requirement will be
            // ; randomly removed through a read out of the structure
            // ; bounds, whereas if they also have a building req,
            // ; that will be ignored completely

            for(itr2 = 1; itr2 < (NUM_BUILDINGS - 1); itr2++)
            {

                if(_CITIES[_cities].bldg_status[bt_NONE] == bs_Built)
                {

                    // ¿ BUGBUG  code should not be same on both branches ?  maybe, was two different fields, then got merged into a union?
                    // ...or, should just be checking reqd_bldg_2, because the first one is reqd_terrain and not reqd_bldg_1
                    if(bldg_data_table[itr2].reqd_bldg_1 > 100)  // bldg_idx  >= 100 is Terrain Type
                    {

                        if(_CITIES[_cities].bldg_status[bldg_data_table[itr2].reqd_terrain] == bs_NotBuilt)  // ; BUG: out of bounds
                        {

                            _CITIES[_cities].bldg_status[itr2] = bs_NotBuilt;

                        }

                    }
                    else
                    {

                        if(
                            (_CITIES[_cities].bldg_status[bldg_data_table[itr2].reqd_bldg_1] == bs_NotBuilt)
                            ||
                            (_CITIES[_cities].bldg_status[bldg_data_table[itr2].reqd_bldg_2] == bs_NotBuilt)
                        )
                        {

                            _CITIES[_cities].bldg_status[itr2] = bs_NotBuilt;

                        }

                    }

                }

            }

            // ; mark any buildings that have been replaced as such
            // ; 
            // ; WARNING: only considers buildings whose replacer is
            // ; built, but not replaced itself, which means that the
            // ; bottom of a chain may get ignored

            for(itr2 = 1; itr2 < (NUM_BUILDINGS - 1); itr2++)
            {

                if(
                    (bldg_data_table[itr2].replace_bldg > bt_NONE)
                    &&
                    (_CITIES[_cities].bldg_status[itr2] == bs_Built)  // ; ignores replaced buildings
                    &&
                    (_CITIES[_cities].bldg_status[bldg_data_table[itr2].replace_bldg] == bs_Built)
                )
                {

                    _CITIES[_cities].bldg_status[bldg_data_table[itr2].replace_bldg] = bs_Replaced;

                }

            }

            // ; find the highest level (by unit type index) close
            // ; combat unit that can be recruited in the city

            Best_Melee_Unit = 0;

            for(itr2 = ut_BarbSwordsmen; itr2 < ut_Magic_Spirit; itr2++)
            {

                if(
                    (_unit_type_table[itr2].race_type == _CITIES[_cities].race)
                    &&
                    (_unit_type_table[itr2].reqd_bldg_1 != 0)
                    &&
                    (
                        (_CITIES[_cities].bldg_status[_unit_type_table[itr2].reqd_bldg_1] == bs_Built)
                        ||
                        (_CITIES[_cities].bldg_status[_unit_type_table[itr2].reqd_bldg_1] == bs_Replaced)
                    )
                    &&
                    (_unit_type_table[itr2].reqd_bldg_2 != 0)
                    &&
                    (
                        (_CITIES[_cities].bldg_status[_unit_type_table[itr2].reqd_bldg_2] == bs_Built)
                        ||
                        (_CITIES[_cities].bldg_status[_unit_type_table[itr2].reqd_bldg_2] == bs_Replaced)
                    )
                    &&
                    ((_unit_type_table[itr2].Abilities & UA_CREATEOUTPOST) == 0)
                    &&
                    (_unit_type_table[itr2].Construction == 0)
                    &&
                    (_unit_type_table[itr2].Transport == 0)
                    &&
                    (
                        (_unit_type_table[itr2].Ranged_Type == rat_NONE)
                        ||
                        (_unit_type_table[itr2].Ranged_Type >= srat_Thrown)
                    )
                )
                {

                    Best_Melee_Unit = itr2;

                }

            }

            // ; find the highest level (by unit type index) ranged
            // ; attack unit that can be recruited in the city

            Best_Ranged_Unit = 0;

            for(itr2 = ut_BarbSwordsmen; itr2 < ut_Magic_Spirit; itr2++)
            {

                if(
                    (_unit_type_table[itr2].race_type == _CITIES[_cities].race)
                    &&
                    (_unit_type_table[itr2].reqd_bldg_1 != 0)
                    &&
                    (
                        (_CITIES[_cities].bldg_status[_unit_type_table[itr2].reqd_bldg_1] == bs_Built)
                        ||
                        (_CITIES[_cities].bldg_status[_unit_type_table[itr2].reqd_bldg_1] == bs_Replaced)
                    )
                    &&
                    (_unit_type_table[itr2].reqd_bldg_2 != 0)
                    &&
                    (
                        (_CITIES[_cities].bldg_status[_unit_type_table[itr2].reqd_bldg_2] == bs_Built)
                        ||
                        (_CITIES[_cities].bldg_status[_unit_type_table[itr2].reqd_bldg_2] == bs_Replaced)
                    )
                    &&
                    ((_unit_type_table[itr2].Abilities & UA_CREATEOUTPOST) == 0)
                    &&
                    (_unit_type_table[itr2].Construction == 0)
                    &&
                    (_unit_type_table[itr2].Transport == 0)
                    &&
                    (
                        (_unit_type_table[itr2].Ranged_Type > rat_NONE)
                        ||
                        (_unit_type_table[itr2].Ranged_Type < srat_Thrown)
                    )
                )
                {

                    Best_Ranged_Unit = itr2;

                }

            }

            // ; create up to population / 4 (rounded down) close
            // ; combat units to garrison the city
            // ; 
            // ; WARNING: dark elves will have no such units,
            // ;  resulting in half the intended starting garrison

            for(itr2 = 0; (((_CITIES[_cities].population / 4) > itr2) && (itr2 < MAX_STACK)); itr2++)
            {

                Create_Unit_NewGame(Best_Melee_Unit, NEUTRAL_PLAYER_IDX, _CITIES[_cities].wx, _CITIES[_cities].wy, _CITIES[_cities].wp, _cities);

            }

            if(Best_Ranged_Unit == 0)
            {

                // ; create another up to population / 4 (rounded down)
                // ; close combat units for the garrison, up to at most
                // ; 9 units total
                // ; 
                // ; WARNING: dark elves will have no such units

                for(itr2 = 0; (((_CITIES[_cities].population / 4) > itr2) && (itr2 < MAX_STACK)); itr2++)
                {

                    Create_Unit_NewGame(Best_Melee_Unit, NEUTRAL_PLAYER_IDX, _CITIES[_cities].wx, _CITIES[_cities].wy, _CITIES[_cities].wp, _cities);

                }

            }
            else
            {

                // ; create up to population / 4 (rounded down) ranged
                // ; attack units for the garrison, up to at most 9 units
                // ; total
                // ; 
                // ; WARNING: the sawmill is not included in the default
                // ;  building list, and the shrine can't be reached with
                // ;  the generated populations, so there won't ever be
                // ;  a ranged attack unit for any race other than dark
                // ;  elves, who only have these and no close combat ones

                for(itr2 = 0; (((_CITIES[_cities].population / 4) > itr2) && (itr2 < MAX_STACK)); itr2++)
                {

                    Create_Unit_NewGame(Best_Ranged_Unit, NEUTRAL_PLAYER_IDX, _CITIES[_cities].wx, _CITIES[_cities].wy, _CITIES[_cities].wp, _cities);

                }

            }

            Random_City_Name_By_Race_NewGame(_CITIES[_cities].race, _CITIES[_cities].name);

        }

    }

}


// MGC o51p25
// drake178: CTY_SetDefaultName()
/*
; chooses one of the default names for the city based
; on its race, ensures that it's unique, and then
; copies it into the passed city record
*/
/*
WZD_o143.c
    for each name record
    loops through every city
    if it finds a match, it goes to the next name record
void Random_City_Name_By_Race(int16_t race_idx, char * name)

*/
// void Random_City_Name_By_Race_NewGame(int16_t race_type, char * name)
// {
//     char Local_Name_String[LEN_TEMP_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//     int16_t city_name_idx = 0;
//     int16_t Tries = 0;
//     char * city_names_buffer = 0;  // _DI_
//     int16_t itr_cities = 0;  // _SI_
//     /* HACK */ char * city_names_buffer_ptr = 0;
// 
//     city_names_buffer = (char *)Near_Allocate_First(280);
// 
//     LBX_Load_Data_Static(cnst_CITYNAME_File, 0, (SAMB_ptr)city_names_buffer, race_type, 1, 280);
// 
//     Tries = 0;
// 
// Loop_Tries:
// 
//     Tries++;
// 
//     city_name_idx = (Random(20) - 1);
// 
//     for(itr_cities = 0; itr_cities < _cities; itr_cities++)
//     {
// 
//         // TODO  MGC farstrcpy()
//         strcpy(Local_Name_String, _CITIES[itr_cities].name);
// 
//         city_names_buffer_ptr = &city_names_buffer[(city_name_idx * LEN_CITY_NAME)];
// 
//         // if(_stricmp(Local_Name_String, cityname_buffer[(city_name_idx * LEN_CITY_NAME)]) == 0)
//         if(strcmp(Local_Name_String, city_names_buffer_ptr) == 0)
//         {
// 
//             if(Tries < 200)
//             {
// 
//                 goto Loop_Tries;
// 
//             }
//             else  /* give up, borrow one */
//             {
// 
//                 LBX_Load_Data_Static(cnst_CITYNAME_File, 0, (SAMB_ptr)city_names_buffer, (Random(NUM_RACES) - 1), 1, 280);
// 
//             }
// 
//         }
// 
//     }
// 
//     // TODO  MGC farstrcpy()
//     // strcpy(name, city_names_buffer[(city_name_idx * LEN_CITY_NAME)]);
//     city_names_buffer_ptr = &city_names_buffer[(city_name_idx * LEN_CITY_NAME)];
//     strcpy(name, city_names_buffer_ptr);
// 
// }
// void Random_City_Name_By_Race(int16_t race_idx, char * name)
void Random_City_Name_By_Race_NewGame(int16_t race_idx, char * name)
{
    char Local_Name_String[LEN_TEMP_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t city_name_idx = 0;
    int16_t attempts = 0;
    char * city_names_buffer = 0;  // _DI_
    int16_t itr_cities = 0;  // _SI_
    /* HACK */ char hack_new_name[LEN_TEMP_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    /* HACK */ char * city_names_buffer_ptr = 0;

    city_names_buffer = (char *)Near_Allocate_First(280);

    LBX_Load_Data_Static(cityname_lbx_file__MGC_ovr051, 0, (SAMB_ptr)city_names_buffer, race_idx, 1, 280);

    for(attempts = 0; attempts < 200; attempts++)
    {

        city_name_idx = (Random(20) - 1);

        for(itr_cities = 0; itr_cities < _cities; itr_cities++)
        {

            strcpy(Local_Name_String, _CITIES[itr_cities].name);

            strcpy(hack_new_name, _CITIES[itr_cities].name);

            // if(stricmp(Local_Name_String, city_names_buffer[(city_name_idx * 14)]) != 0)
            city_names_buffer_ptr = &city_names_buffer[(city_name_idx * LEN_CITY_NAME)];

            // TODO  stricmp() vs. strcmp()  if(stricmp(Local_Name_String, city_names_buffer_ptr) != 0)
            if(strcmp(Local_Name_String, city_names_buffer_ptr) != 0)
            {

                continue;

            }

            if(attempts < 200)
            {

                break;

            }

            LBX_Load_Data_Static(cityname_lbx_file__MGC_ovr051, 0, (SAMB_ptr)city_names_buffer, (Random(NUM_RACES) - 1), 1, 280);

        }

    }
    
    // strcpy(name, city_names_buffer[(city_name_idx * 14)]);
    city_names_buffer_ptr = &city_names_buffer[(city_name_idx * LEN_CITY_NAME)];
    strcpy(name, city_names_buffer_ptr);

}

// MGC o51p26
// drake178: NEWG_CreateRoads()
/*
; creates roads on the tiles of every city, as well as
; between neutral ones on the same continents that are
; closer to each other than 11 tiles with no water
; tiles on the most direct path
;
; BUG: creates normal roads instead of enchanted ones
;  on the tiles of Myrran cities
*/
/*

*/
void Generate_Roads(int16_t wp)
{
    int8_t Road_Ys[70] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int8_t Road_Xs[70] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t dst_wy = 0;
    int16_t dst_wx = 0;
    int16_t src_wy = 0;
    int16_t src_wx = 0;
    int16_t city_idx = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t Line_Index = 0;
    int16_t Road_Length = 0;
    int16_t Invalid_Road = 0;
    int16_t dst_city_idx = 0;
    int16_t src_city_idx = 0;

    // ; clear out the terrain flags tables
    // ; 
    // ; already done by the terrain special function

    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {

        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {

            SET_MAP_SQUARE_FLAG(wx, wy, wp, 0);

        }

    }

    for(src_city_idx = 0; src_city_idx < _cities; src_city_idx++)
    {

        src_wx = _CITIES[src_city_idx].wx;

        src_wy = _CITIES[src_city_idx].wy;

        for(dst_city_idx = (src_city_idx + 1); dst_city_idx < _cities; dst_city_idx++)
        {

            dst_wx = _CITIES[dst_city_idx].wx;

            dst_wy = _CITIES[dst_city_idx].wy;

            if(
                (_CITIES[src_city_idx].wp != wp)
                ||
                (_CITIES[dst_city_idx].wp != wp)
                ||
                (_CITIES[src_city_idx].owner_idx != NEUTRAL_PLAYER_IDX)
                ||
                (_CITIES[dst_city_idx].owner_idx != NEUTRAL_PLAYER_IDX)
                ||
                (_landmasses[((wp * WORLD_SIZE) + (src_wy * WORLD_WIDTH) + src_wx)] != _landmasses[((wp * WORLD_SIZE) + (dst_wy * WORLD_WIDTH) + dst_wx)])
                ||
                (Range(src_wx, src_wy, dst_wx, dst_wy) >= 11)
            )
            {

                continue;

            }

            Road_Length = Path_Wrap(src_wx, src_wy, dst_wx, dst_wy, &Road_Xs[0], &Road_Ys[0], WORLD_WIDTH);

            Invalid_Road = 0;

            Line_Index = 0;

            // ; invalidate the road if it would pass through an ocean
            // ; or shore tile
            // ; 
            // ; contains exclusions for tile types that will never be
            // ; present on the map at this stage of the process

            while(((Road_Length - 1) > Line_Index) && (Invalid_Road == ST_FALSE))
            {

                wx = Road_Xs[Line_Index];

                wy = Road_Ys[Line_Index];

                if(
                    (_world_maps[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx )] >= tt_Shore1_Fst)
                    &&
                    (_world_maps[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx )] <= tt_Shore1_Lst)
                )
                {

                    Invalid_Road = ST_TRUE;

                }

                if(_world_maps[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx )] == tt_Ocean1)
                {

                    Invalid_Road = ST_TRUE;

                }

                // ; BUG: this batch should start at $C5 (not that any of
                // ; these can be present on the map at this stage)
                if(
                    (_world_maps[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx )] >= _Shore00001R10)
                    &&
                    (_world_maps[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx )] <= TT_Shore2F_end)
                )
                {

                    Invalid_Road = ST_TRUE;

                }

                if(
                    (_world_maps[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx )] >= tt_Shore2_1st)
                    &&
                    (_world_maps[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx )] <= tt_Ocean2)
                )
                {

                    Invalid_Road = ST_TRUE;

                }

                Line_Index++;

            }

            if(Invalid_Road == ST_FALSE)
            {

                // ; create a road or enchanted road along the specified
                // ; line depending on the plane

                for(Line_Index = 0; ((Road_Length - 1) > Line_Index); Line_Index++)
                {

                    wx = Road_Xs[Line_Index];

                    wy = Road_Ys[Line_Index];

                    SET_MAP_SQUARE_FLAG(wx, wy, wp, (GET_MAP_SQUARE_FLAG(wx, wy, wp) | MSF_ROAD));

                    if(wp == MYRROR_PLANE)
                    {

                        SET_MAP_SQUARE_FLAG(wx, wy, wp, (GET_MAP_SQUARE_FLAG(wx, wy, wp) | MSF_EROAD));

                    }

                }

            }

        }

    }  /* for(src_city_idx = 0; src_city_idx < _cities; src_city_idx++) */



    // ; create roads under each city
    // ; 
    // ; BUG: ignores the passed plane value, and instead
    // ;  processes every city
    // ; BUG: Myrran roads will have normal roads as the tile
    // ;  coordinates upgraded are not set up before doing so

    for(city_idx = 0; city_idx < _cities; city_idx++)
    {

        // ; PATCHED here to fix the normal roads under Myrran
        // ; Cities BUG

        SET_MAP_SQUARE_FLAG(
            _CITIES[city_idx].wx,
            _CITIES[city_idx].wy,
            _CITIES[city_idx].wp,
            (
                GET_MAP_SQUARE_FLAG(
                    _CITIES[city_idx].wx,
                    _CITIES[city_idx].wy,
                    _CITIES[city_idx].wp
                )
                |
                MSF_ROAD
            )
        );

        if(wp == MYRROR_PLANE)
        {

            // ; BUG: these are not the city coordinates
            SET_MAP_SQUARE_FLAG(
                wx,
                wy,
                wp,
                (
                    GET_MAP_SQUARE_FLAG(
                        wx,
                        wy,
                        wp
                    )
                    |
                    MSF_EROAD
                )
            );

        }

    }

}


// MGC o51p27
// drake178: CRP_NEWG_CreatePathGrids()
/*
; the arrays created by this function are never used
; by the game, and can be repurposed even though they
; are part of the save game file
;
; creates the segmented pathing grids of the selected
; plane for both land and sea exclusive movement types
*/
/*

MGC  ovr055 OON function  CRP_NEWG_CreatePathGrid__STUB()

*/
void CRP_NEWG_CreatePathGrids__WIP(int16_t wp)
{

    // TODO  EMM_Map4_EMMDATAH()

    CRP_NEWG_CreatePathGrid__STUB(&movement_mode_cost_maps[wp].walking.moves2[0], &UU_TBL_1[wp]);

    CRP_NEWG_CreatePathGrid__STUB(&movement_mode_cost_maps[wp].sailing.moves2[0], &UU_TBL_2[wp]);

}


// MGC o51p28
// drake178: NEWG_CreateSpecials()
/*
; generates terrain specials for the selected plane,
; and also clears out its terrain flags table
;
; WARNING: alters tile types, which may have an adverse
;  effect on previously used maximum populations
*/
/*
*/
void Generate_Terrain_Specials(int16_t wp)
{
    int16_t Create_Radius = 0;
    int16_t City_Present = 0;
    int16_t EZ_Present = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;
    int16_t itr = 0;
    int16_t wy = 0;  // _DI_
    int16_t wx = 0;  // _SI_
    
    // ; clear the terrain flag and terrain special map of the
    // ; plane
    for(wy = 0; wy < WORLD_HEIGHT; wy++)
    {

        for(wx = 0; wx < WORLD_WIDTH; wx++)
        {

            SET_TERRAIN_SPECIAL(wx, wy, wp, 0);

            SET_MAP_SQUARE_FLAG(wx, wy, wp, 0);

        }

    }

    if(wp == ARCANUS_PLANE)
    {

        Create_Radius = 4;

    }
    else
    {

        Create_Radius = 3;

    }

    for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy += Create_Radius)
    {

        for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx += Create_Radius)
        {

            wy = (itr_wy + Random((Create_Radius * 2)));

            wx = (itr_wx + Random((Create_Radius * 2)));

            City_Present = ST_FALSE;

            EZ_Present = ST_FALSE;

            // ; check if there's an encounter on the tile

            for(itr = 0; itr < NUM_LAIRS; itr++)
            {

                if(
                    (_LAIRS[itr].wp == wp)
                    &&
                    (_LAIRS[itr].wx == wx)
                    &&
                    (_LAIRS[itr].wy == wy)
                )
                {

                    EZ_Present = ST_TRUE;

                }

            }

            // ; check if there's a tower on the tile
            // ; 
            // ; the above loop will already catch these

            for(itr = 0; itr < NUM_TOWERS; itr++)
            {

                if(
                    (_TOWERS[itr].wx == wx)
                    &&
                    (_TOWERS[itr].wy == wy)
                )
                {

                    EZ_Present = ST_TRUE;

                }

            }

            // ; check if there is a city on the tile

            for(itr = 0; itr < _cities; itr++)
            {

                if(
                    (_CITIES[itr].wp == wp)
                    &&
                    (_CITIES[itr].wx == wx)
                    &&
                    (_CITIES[itr].wy == wy)
                )
                {

                    City_Present = ST_TRUE;

                }

            }

            if(
                (GET_TERRAIN_SPECIAL(wx, wy, wp) != 0)
                ||
                (City_Present != ST_FALSE)
                ||
                (EZ_Present != ST_FALSE)
                ||
                (
                    (wy <= 2)  // BUGBUG  should be || not &&
                    &&
                    (wy >= 38)
                    &&
                    (wx <= 2)
                    &&
                    (wx >= 58)
                )
            )
            {

                continue;

            }

            // ; this may reduce maximum population

            if(
                (Square_Is_Grassland_NewGame(wx, wy, wp) == ST_TRUE)
                ||
                (
                    (Square_Is_Forest_NewGame(wx, wy, wp) == ST_TRUE)
                    &&
                    (Random(2) == 1)
                )
            )
            {

                switch((Random(7) - 1))
                {
                    case 0:
                    case 1:
                    case 2:
                    {
                        SET_TERRAIN_TYPE(wx, wy, wp, tt_Mountain1);
                    } break;
                    case 3:
                    case 4:
                    case 5:
                    {
                        SET_TERRAIN_TYPE(wx, wy, wp, tt_Hills1);
                    } break;
                    case 6:
                    {
                        SET_TERRAIN_TYPE(wx, wy, wp, tt_Swamp1);
                    } break;
                }

            }

            if(Square_Is_Forest_NewGame(wx, wy, wp) == ST_TRUE)
            {

                SET_TERRAIN_SPECIAL(wx, wy, wp, TS_WILDGAME);

            }

            if(Square_Is_Mountain_NewGame(wx, wy, wp) == ST_TRUE)
            {

                SET_TERRAIN_SPECIAL(wx, wy, wp, Mountain_Terrain_Special(wp));

            }

            if(Square_Is_Hills_NewGame(wx, wy, wp) == ST_TRUE)
            {

                SET_TERRAIN_SPECIAL(wx, wy, wp, Hills_Terrain_Special(wp));

            }

            if(Square_Is_Swamp_NewGame(wx, wy, wp) == ST_TRUE)
            {

                SET_TERRAIN_SPECIAL(wx, wy, wp, ts_Nightshade);

            }

            if(Square_Is_Desert_NewGame(wx, wy, wp) == ST_TRUE)
            {

                SET_TERRAIN_SPECIAL(wx, wy, wp, Desert_Terrain_Special(wp));

            }

        }

    }

}


// MGC o51p29
// drake178: NEWG_DesertSpecials()
/*
; returns a random desert tile terrain special based on
; the plane
; Arcanus         Myrror
;   2:3    gems    1:5
;   1:3    quork   3:5
;   -      crysx   1:5
*/
/*

*/
int16_t Desert_Terrain_Special(int16_t wp)
{
    int16_t _DI_ = 0;  // _DI_
    int16_t terrain_special = 0;  // _SI_

    if(wp == ARCANUS_PLANE)
    {
        _DI_ = Random(9);
    }
    else
    {
        _DI_ = (5 + Random(5));
    }

    switch((_DI_ - 1))  // - 1;  switch 10 cases
    {
        case  0:
        case  1:
        case  2:
        case  3:
        case  4:
        case  5: { terrain_special = ts_Gems; } break;   // min Myrror roll
        case  6: 
        case  7: 
        case  8: { terrain_special = ts_QuorkCrystals; } break;  // max Arcanus roll 
        case  9: { terrain_special = ts_CrysxCrystals; } break;

    }

    return terrain_special;

}


// MGC o51p30
// drake178: NEWG_HillSpecials()
/*
; returns a random hill tile terrain special based on
; the plane
; Arcanus          Myrror
;   6:18    iron    1:10
;   4:18   silver   1:10
;   4:18    gold    4:10
;   3:18    coal    1:10
;   1:18   mithril  2:10
;   -    adamantium 1:10
*/
/*
pg 182  (PDF Page 191)
Hills
-----------------------
Iron        33.3    10
Coal        16.7    10
Silver      22.2    10
Gold        22.2    40
Mithril      5.6    20
Adamantium   0      10

Arcanus:
{0, ..., 17}
Iron    = (5 + 1):18 = 6:18 6/18 = 33.3
Silver  = (3 + 1):18 = 4:18 4/18 = 22.2
Coal    = (2 + 1):18 = 3:18 3/18 = 16.7
Gold    =              4:18 4/18 = 22.2
Mithril =              1:18 1/18 =  5.6
Adamantium  N/A

Myrror:
{10, ..., 19}
Iron       = 1:10  10
Silver     = 1:10  10
Coal       = 1:10  10
Gold       = 4:10  40
Mithril    = 2:10  20
Adamantium = 1:10  10

*/
int16_t Hills_Terrain_Special(int16_t wp)
{
    int16_t _DI_ = 0;  // _DI_
    int16_t terrain_special = 0;  // _SI_

    if(wp == ARCANUS_PLANE)
    {
        _DI_ = Random(18);
    }
    else
    {
        _DI_ = (10 + Random(10));
    }

    switch((_DI_ - 1))  // - 1;  switch 20 cases
    {
        case  0:
        case  1:
        case  2:
        case  3:
        case  4: { terrain_special = ts_Iron; } break;
        case  5:
        case  6:
        case  7: { terrain_special = ts_Silver; } break;
        case  8:
        case  9: { terrain_special = ts_Coal; } break;
        case 10: { terrain_special = ts_Iron; } break;  // min Myrror roll
        case 11: { terrain_special = ts_Coal; } break;
        case 12: { terrain_special = ts_Silver; } break;
        case 13: 
        case 14: 
        case 15: 
        case 16: { terrain_special = ts_Gold; } break;
        case 17:
        case 18: { terrain_special = ts_Mithril; } break;  // max Arcanus roll 
        case 19: { terrain_special = ts_Adamantium; } break;
    }

    return terrain_special;

}


// MGC o51p31
// drake178: NEWG_MntnSpecials()
/*
; returns a random mountain tile terrain special based
; on the plane
; Arcanus          Myrror
;   5:18    coal    1:10
;   4:18    iron    1:10
;   3:18   silver   1:10
;   3:18    gold    2:10
;   3:18   mithril  3:10
;   -    adamantium 2:10
*/
/*

*/
int16_t Mountain_Terrain_Special(int16_t wp)
{
    int16_t _DI_ = 0;  // _DI_
    int16_t terrain_special = 0;  // _SI_

    if(wp == ARCANUS_PLANE)
    {
        _DI_ = Random(18);
    }
    else
    {
        _DI_ = (10 + Random(10));
    }

    switch((_DI_ - 1))  // - 1;  switch 20 cases
    {
        case  0:
        case  1:
        case  2: { terrain_special = ts_Iron; } break;
        case  3:
        case  4: { terrain_special = ts_Silver; } break;
        case  5: 
        case  6: 
        case  7: 
        case  8: { terrain_special = ts_Coal; } break;
        case  9: { terrain_special = ts_Gold; } break;
        case 10: { terrain_special = ts_Iron; } break;  // min Myrror roll
        case 11: { terrain_special = ts_Silver; } break;
        case 12: { terrain_special = ts_Coal; } break;
        case 13: 
        case 14: { terrain_special = ts_Gold; } break;
        case 15: 
        case 16: 
        case 17: { terrain_special = ts_Mithril; } break;  // max Arcanus roll
        case 18: 
        case 19: { terrain_special = ts_Adamantium; } break;
    }

    return terrain_special;

}


// MGC o51p32
// drake178: NEWG_SetScoutingMaps()
/*
; clears the scouting maps, and marks the human
; player's initial exploration around their fortress
; city
*/
/*

Init_New_Game() |-> 

*/
void Init_Square_Explored(void)
{
    int16_t X_Modifier = 0;
    int16_t wp = 0;
    int16_t wy = 0;  // _DI_
    int16_t wx = 0;  // _SI_

    for(wp = 0; wp < 2; wp++)
    {

        for(wy = 0; wy < WORLD_HEIGHT; wy++)
        {

            for(wx = 0; wx < WORLD_WIDTH; wx++)
            {

                SET_SQUARE_EXPLORED(wx,wy,wp,ST_FALSE);

            }

        }

    }

    /*
        Wow...
        Just, make explored around the human players starting city...
    */
    wp = _FORTRESSES[HUMAN_PLAYER_IDX].wp;
    wx = _FORTRESSES[HUMAN_PLAYER_IDX].wx;
    wy = _FORTRESSES[HUMAN_PLAYER_IDX].wy;

    // ; mark the 3 by 3 area centered around the capital as
    // ; fully explored
    // ; 
    // ; WARNING: the corners will be overwritten below too
    for(X_Modifier = -1; X_Modifier <= 1; X_Modifier++)
    {

        Set_Square_Explored_Bits(wp, (wx + X_Modifier), wy, (SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight));
        
    }
    for(X_Modifier = -1; X_Modifier <= 1; X_Modifier++)
    {

        Set_Square_Explored_Bits(wp, (wx + X_Modifier), (wy - 1), (SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight));
        
    }
    for(X_Modifier = -1; X_Modifier <= 1; X_Modifier++)
    {

        Set_Square_Explored_Bits(wp, (wx + X_Modifier), (wy + 1), (SCT_BottomLeft | SCT_TopLeft | SCT_TopRight | SCT_BottomRight));
        
    }

    Set_Square_Explored_Bits(wp, (wx - 1), (wy - 2), SCT_BottomRight);

    Set_Square_Explored_Bits(wp, wx, (wy - 2), (SCT_BottomLeft | SCT_BottomRight));

    Set_Square_Explored_Bits(wp, (wx + 1), (wy - 2), SCT_BottomLeft);

    Set_Square_Explored_Bits(wp, (wx - 1), (wy + 2), SCT_TopRight);

    Set_Square_Explored_Bits(wp, wx, (wy + 2), (SCT_TopLeft | SCT_TopRight));

    Set_Square_Explored_Bits(wp, (wx - 2), (wy - 1), SCT_BottomRight);

    Set_Square_Explored_Bits(wp, (wx - 2), wy, (SCT_TopRight | SCT_BottomRight));

    Set_Square_Explored_Bits(wp, (wx - 2), (wy + 1), SCT_TopRight);

    Set_Square_Explored_Bits(wp, (wx + 2), (wy - 1), SCT_BottomLeft);

    Set_Square_Explored_Bits(wp, (wx + 2), (wy + 1), SCT_TopLeft);

    Set_Square_Explored_Bits(wp, (wx + 2), wy, (SCT_BottomLeft | SCT_TopLeft));

    Set_Square_Explored_Bits(wp, (wx - 1), (wy - 1), (SCT_BottomLeft | SCT_TopRight | SCT_BottomRight));

    Set_Square_Explored_Bits(wp, (wx - 1), (wy + 1), (SCT_TopLeft | SCT_TopRight | SCT_BottomRight));

    Set_Square_Explored_Bits(wp, (wx - 1), (wy + 1), (SCT_BottomLeft | SCT_TopLeft | SCT_TopRight));

    Set_Square_Explored_Bits(wp, (wx + 1), (wy - 1), (SCT_BottomLeft | SCT_TopLeft | SCT_BottomRight));

}


// MGC o51p33
// drake178: TILE_SetScouting()
/*
; sets the pased scouting flags for the specified tile,
; or does nothing if the coordinates are out of range
; (the plane is not boundary checked though)
*/
/*

NOT Set_Map_Square_Explored_Flags_XYP_Range()
NOT j_Set_Map_Square_Explored_Flags_XYP()

*/
void Set_Square_Explored_Bits(int16_t wp, int16_t wx, int16_t wy, int16_t bits)
{

    if(
        (wx >= 0)
        &&
        (wx < WORLD_WIDTH)
        &&
        (wy >= 0)
        &&
        (wy < WORLD_HEIGHT)
    )
    {

        SET_SQUARE_EXPLORED(wx,wy,wp,bits);

    }

}


// MGC o51p34
// drake178: NEWG_SetMoveMaps()
/*
; sets the movement map for the selected plane based
; on tile types
*/
/*
    populates movement_mode_cost_maps[]
    from terrain_stats_table[], then roads and eroads

SEEALSO:  CITYCALC.c  Make_Road()

*/
void Movement_Mode_Cost_Maps(int16_t wp)
{
    int16_t terrain_type_idx = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;  //  _DI_

    // TODO  EMM_Map4_EMMDATAH();

    for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy++)
    {

        for(itr_wy = 0; itr_wy < WORLD_WIDTH; itr_wy++)
        {

            terrain_type_idx = TERRAIN_TYPE_INDEX(itr_wx, itr_wy, wp);

// void Make_Road(int16_t wx, int16_t wy, int16_t wp)
//     movement_mode_cost_maps[wp].UU_MvMd.moves2[((wy * WORLD_WIDTH) + wx)] = 1;
//     movement_mode_cost_maps[wp].walking.moves2[((wy * WORLD_WIDTH) + wx)] = 1;
//     movement_mode_cost_maps[wp].forester.moves2[((wy * WORLD_WIDTH) + wx)] = 1;
//     movement_mode_cost_maps[wp].mountaineer.moves2[((wy * WORLD_WIDTH) + wx)] = 1;
//     movement_mode_cost_maps[wp].swimming.moves2[((wy * WORLD_WIDTH) + wx)] = 1;
//     _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] |= MSF_ROAD;
//     _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] &= ~(MSF_EROAD);
//     if(wp == MYRROR_PLANE)
//         Make_Road_Enchanted(wx, wy, wp);

// struct s_MOVE_COST_MAP
// {
//     /* 0x0000 */ int8_t moves2[WORLD_SIZE];
//     /* 0x0960 */
// };
// #pragma pack(pop)
// 
// 
// // drake178: HMP_MAPS
// // sizeof:  3840h  14,400
// /*
//     movement cost per movement mode, in moves2 units
//     one *map* per movement mode, as world map - 60x40 - map squares
// */
// #pragma pack(push)
// #pragma pack(2)
// struct s_MOVE_MODE_COST_MAPS
// {
//     /* 0x0000 */ struct s_MOVE_COST_MAP UU_MvMd;
//     /* 0x0960 */ struct s_MOVE_COST_MAP walking;

            // ; AL = (byte *)(terrain_stats_table + (terrain_type * 6))
            // [es:bx+HMP_MAPS.Walking.HalfMPCost], al
            // movement_mode_cost_maps[((wp * 14400) + (itr_wy * WORLD_WIDTH) + itr_wx)] = terrain_stats_table[terrain_type_idx];
            // terrain_stats_table[terrain_type_idx][]
            // {1,0,2,3,4,5}
            // {Walking,Unused_MoveType,Forester,Mountaineer,Swimming,Sailing}
            movement_mode_cost_maps[wp].walking.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = terrain_stats_table[((terrain_type_idx * 6) + 1)];
            movement_mode_cost_maps[wp].UU_MvMd.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = terrain_stats_table[((terrain_type_idx * 6) + 0)];
            movement_mode_cost_maps[wp].forester.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = terrain_stats_table[((terrain_type_idx * 6) + 2)];
            movement_mode_cost_maps[wp].mountaineer.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = terrain_stats_table[((terrain_type_idx * 6) + 3)];
            movement_mode_cost_maps[wp].swimming.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = terrain_stats_table[((terrain_type_idx * 6) + 4)];
            movement_mode_cost_maps[wp].sailing.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = terrain_stats_table[((terrain_type_idx * 6) + 5)];

            if((MAP_SQUARE_FLAG(itr_wx,itr_wy,wp) & MSF_ROAD) != 0)
            {

                if(wp != ARCANUS_PLANE)
                {

                    movement_mode_cost_maps[wp].walking.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = 0;
                    movement_mode_cost_maps[wp].UU_MvMd.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = 0;
                    movement_mode_cost_maps[wp].forester.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = 0;
                    movement_mode_cost_maps[wp].mountaineer.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = 0;
                    movement_mode_cost_maps[wp].swimming.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = 0;

                }
                else
                {

                    movement_mode_cost_maps[wp].walking.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = 1;
                    movement_mode_cost_maps[wp].UU_MvMd.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = 1;
                    movement_mode_cost_maps[wp].forester.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = 1;
                    movement_mode_cost_maps[wp].mountaineer.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = 1;
                    movement_mode_cost_maps[wp].swimming.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = 1;

                }

                movement_mode_cost_maps[wp].sailing.moves2[((itr_wy * WORLD_WIDTH) + itr_wx)] = ST_UNDEFINED;

            }

        }

    }

}


// MGC o51p35
// drake178: TILE_InvalidOutflow()
// TILE_InvalidOutflow()

// MGC o51p36
// drake178: NEWG_UpdateProgress()
// ~ MoO2  Module: MISC  Draw_Advancing_Fill_Message_()
/*
PATCHED in the worldgen customizer to use a created
background image and always redraw, allowing the
progress bar to be reset if necessary
updates the progress bar to the specified percentage
before doing a simple page flip
WARNING: can't go backwards with this setup
*/
/*

*/
void Draw_Building_The_Worlds(int16_t percent)
{
    int16_t width = 0;
    Set_Page_Off();
    if(percent == 0)
    {
        return;
    }
    if(percent >= 100)
    {
        Fill(90, 89, 225, 94, 172);
    }
    else
    {
        width = ((percent * 136) / 100);
        Fill(90, 89, (90 + width), 94, 172);
    }
    Toggle_Pages();
}

// MGC o51p37
// drake178: UNIT_Create()
/*
*/
/*

*/
void Create_Unit_NewGame(int16_t unit_type, int16_t player_idx, int16_t wx, int16_t wy, int16_t wp, int16_t city_idx)
{
    int16_t WTF = 0;

    if(unit_type == 0)
    {

        return;

    }

    _UNITS[_units].wx = (int8_t)wx;
    _UNITS[_units].wy = (int8_t)wy;
    _UNITS[_units].wp = (int8_t)wp;
    _UNITS[_units].owner_idx = (int8_t)player_idx;
    _UNITS[_units].moves2_max = _unit_type_table[unit_type].Move_Halves;
    _UNITS[_units].type = (uint8_t)unit_type;
    _UNITS[_units].Hero_Slot = ST_UNDEFINED;
    // DNE  _UNITS[_units].in_tower = ST_FALSE;
    // _UNITS[_units].Finished = ST_TRUE;
    _UNITS[_units].Finished = ST_FALSE;
    // _UNITS[_units].moves2 = 0;
    _UNITS[_units].moves2 = _unit_type_table[unit_type].Move_Halves;
    _UNITS[_units].Sight_Range = _unit_type_table[unit_type].Sight;
    _UNITS[_units].dst_wx = 0;
    _UNITS[_units].dst_wy = 0;
    _UNITS[_units].Status = us_Ready;
    _UNITS[_units].Level = 0;
    _UNITS[_units].XP = 0;
    _UNITS[_units].Damage = 0;
    _UNITS[_units].Draw_Priority = 0;
    _UNITS[_units].enchantments = 0;
    // DNE  _UNITS[_units].mutations = 0;
    _UNITS[_units].Move_Failed = ST_FALSE;
    _UNITS[_units].Rd_Constr_Left = ST_UNDEFINED;
    _UNITS[_units].mutations = 0;

    if(_players[player_idx].alchemy != ST_FALSE)
    {
        _UNITS[_units].mutations = wq_Magic;                        
    }

    // WTF?
    if(city_idx >= 0)
    {

        // ; ; PATCHED here to prevent units starting with XP

        if(_CITIES[WTF].bldg_status[bt_FightersGuild] > bs_Replaced)
        {

            _UNITS[_units].XP = TBL_Experience[UL_REGULAR];

            _UNITS[_units].Level = 1;

        }

        if(_CITIES[WTF].bldg_status[bt_WarCollege] > bs_Replaced)
        {

            _UNITS[_units].XP = TBL_Experience[UL_VETERAN];

            _UNITS[_units].Level = 2;

        }

        if(_UNITS[_units].Level > 3)
        {

            _UNITS[_units].XP = TBL_Experience[UL_ELITE];

            _UNITS[_units].Level = 3;

        }

        if(_players[player_idx].warlord > ST_FALSE)
        {

            _UNITS[_units].Level += 1;

        }

    }

    _units++;

}


// MGC o51p38
// drake178: NEWG_AnimateOceans()
/*

*/
/*

*/
void NEWG_AnimateOceans__STUB(void)
{



}


// MGC o51p39
// drake178: TILE_IsOcean()
int16_t TILE_IsOcean(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _CX_

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(
        (terrain_type != tt_BugGrass)
        &&
        (
            (terrain_type < tt_Grasslands1)
            ||
            (
                (terrain_type > TT_RiverM_end)
                &&
                (terrain_type < TT_Rivers_1st)
            )
            ||
            (
                (terrain_type > tt_Desert_Lst)
                &&
                (terrain_type < TT_4WRiver1)
            )
            ||
            (
                (terrain_type > TT_4WRiver5)
                &&
                (terrain_type < tt_Tundra_1st)
            )
        )
    )
    {

        return ST_TRUE;

    }
    else
    {

        return ST_FALSE;

    }

}


// MGC o51p40
// drake178: TILE_HasNode()
int16_t TILE_HasNode(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t itr_nodes = 0;  // _CX_
    
    for(itr_nodes = 0; itr_nodes < NUM_NODES; itr_nodes++)
    {

        if(
            (_NODES[itr_nodes].wx == wx)
            &&
            (_NODES[itr_nodes].wy == wy)
            &&
            (_NODES[itr_nodes].wp == wp)
        )
        {

            return ST_TRUE;

        }

    }

    return ST_FALSE;

}


// MGC o51p41
// drake178: TILE_HasTower()
int16_t TILE_HasTower(int16_t wx, int16_t wy)
{
    int16_t itr_towers = 0;  // _DX_
    
    for(itr_towers = 0; itr_towers < NUM_TOWERS; itr_towers++)
    {

        if(
            (_TOWERS[itr_towers].wx == wx)
            &&
            (_TOWERS[itr_towers].wy == wy)
        )
        {

            return ST_TRUE;

        }

    }

    return ST_FALSE;

}


// MGC o51p42
// drake178: Square_Is_Forest_NewGame()
/*

*/
/*

~==
Terrain.c
// WZD s161p23
// drake178: TILE_IsVisibleForest()
// AKA Square_Is_Explored_Forest()
int16_t Square_Is_Forest(int16_t wx, int16_t wy, int16_t wp)

*/
int16_t Square_Is_Forest_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _CX_

    terrain_type = (TERRAIN_TYPE(wx, wy, wp) % NUM_TERRAIN_TYPES);

    if(
        (terrain_type == tt_Forest1)
        ||
        (terrain_type == tt_Forest2)
        ||
        (terrain_type == tt_Forest3)
    )
    {

        return ST_TRUE;

    }
    else
    {

        return ST_FALSE;

    }

}


// MGC o51p43
// drake178: TILE_IsRiver()
// TILE_IsRiver()

// MGC o51p44
// drake178: UU_TILE_IsRiverOutlet()
// UU_TILE_IsRiverOutlet()

// MGC o51p45
// drake178: UU_TILE_IsShore()
// UU_TILE_IsShore()

// MGC o51p46
// drake178: TILE_IsMountains()
int16_t Square_Is_Mountain_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _CX_

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(
        (terrain_type == tt_Mountain1)
        ||
        (
            (terrain_type >= tt_Mountains_Fst)
            &&
            (terrain_type <= tt_Mountains_Lst)
        )
    )
    {

        return ST_TRUE;

    }
    else
    {

        return ST_FALSE;

    }

}

// MGC o51p47
// drake178: TILE_IsHills()
int16_t Square_Is_Hills_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _CX_

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(
        (terrain_type == tt_Hills1)
        ||
        (
            (terrain_type >= tt_Mountains_Fst)
            &&
            (terrain_type <= tt_Mountains_Lst)
        )
    )
    {

        return ST_TRUE;

    }
    else
    {

        return ST_FALSE;

    }

}


// MGC o51p48
// drake178: TILE_IsSwamp()
int16_t Square_Is_Swamp_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _CX_

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(
        (terrain_type == tt_Swamp2)
        ||
        (terrain_type == tt_Swamp3)
        ||
        (terrain_type == tt_Swamp1)
    )
    {

        return ST_TRUE;

    }
    else
    {

        return ST_FALSE;

    }

}

// MGC o51p49
// drake178: TILE_IsDesert()
int16_t Square_Is_Desert_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _CX_

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(
        (terrain_type == tt_Desert1)
        ||
        (terrain_type == tt_Desert2)
        ||
        (terrain_type == tt_Desert3)
        ||
        (terrain_type == tt_Desert4)
        ||
        (
            (terrain_type >= tt_Desert_Fst)
            &&
            (terrain_type <= tt_Desert_Lst)
        )
    )
    {

        return ST_TRUE;

    }
    else
    {

        return ST_FALSE;

    }

}


// MGC o51p50
// drake178: TILE_IsGrasslands()
/*
*/
/*
*/
int16_t Square_Is_Grassland_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _CX_

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(
        (terrain_type == tt_Grasslands1)
        ||
        (terrain_type == tt_Grasslands2)
        ||
        (terrain_type == tt_Grasslands3)
        ||
        (terrain_type == tt_Grasslands4)
        ||
        (terrain_type == tt_BugGrass)
    )
    {

        return ST_TRUE;

    }
    else
    {

        return ST_FALSE;

    }

}


// MGC o51p51
// drake178: TILE_GetFood()
/*
; returns the amount of food provided by the tile based
; on its type, in units of 1/2
;
; BUG: swamps do not yield the described amount of 1/2,
;  while nature nodes yield less than in WIZARDS.EXE
*/
/*
returns food, not food2

Terrain.c
// WZD s161p03
int16_t Square_Food2(int16_t wx, int16_t wy, int16_t wp)

traced with terrain_type == tt_Grasslands1

*/
int16_t Square_Food2_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _SI_
    int16_t food_units = 0;  // _CX_

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

// #ifdef STU_DEBUG
//     if(terrain_type == tt_Grasslands1)
//     {
//         STU_DEBUG_BREAK();
//     }
// #endif

    if(
        (terrain_type == tt_Ocean1)
        ||
        (terrain_type >= tt_Ocean2)
    )
    {
        return 0;
    }
    else if(terrain_type == tt_BugGrass)  // NOTE: sames {tt_Grasslands1, ..., tt_Grasslands3}
    {
        return 3;
    }
    else if(terrain_type < tt_Grasslands1)  // >= tt_Shore1_Fst  <= tt_Shore1_Lst
    {
        return 1;
    }
    else if(terrain_type > TT_4WRiver5)  // >= TT_Shore3_1st
    {
        return 1;
    }
    else if(terrain_type > TT_Shore2_end)  // >= TT_4WRiver1
    {
        return 4;
    }
    else if(terrain_type > tt_Desert_Lst)  // >= tt_Shore2_1st
    {
        return 1;
    }
    else if(terrain_type > tt_Hills_Lst)  // >= tt_Desert_Fst
    {
        return 0;
    }
    else if(terrain_type > tt_Mountains_Lst)  // >= tt_Hills_Fst
    {
        return 1;
    }
    else if(terrain_type > tt_Rivers_end)  // >= tt_Mountains_Fst
    {
        return 0;
    }
    else if(terrain_type > tt_Forest3)  // >= TT_RiverM_1st
    {
        return 4;
    }
    else
    {

// switch(terrain_type - tt_Grasslands1)
// ; switch 23 cases
// jt_sq_ng_tt_fd_00_10_11_18
// jt_sq_ng_tt_fd_01_21_22
// jt_sq_ng_tt_fd_02
// jt_sq_ng_tt_fd_03_12_13_14
// jt_sq_ng_tt_fd_04_15_16
// jt_sq_ng_tt_fd_05_19_20
// jt_sq_ng_tt_fd_06
// jt_sq_ng_tt_fd_07
// jt_sq_ng_tt_fd_08
// jt_sq_ng_tt_fd_09
// jt_sq_ng_tt_fd_17
        switch(terrain_type)
        {

            case tt_Grasslands1:
            case tt_Grasslands2:
            case tt_Grasslands3:
            case tt_Grasslands4:
            {
                food_units = 3;
            } break;

            case tt_Forest1:
            case tt_Forest2:
            case tt_Forest3:
            {
                food_units = 1;
            } break;

            case tt_Mountain1:
            {
                food_units = 0;
            } break;

            case tt_Desert1:
            case tt_Desert2:
            case tt_Desert3:
            case tt_Desert4:
            {
                food_units = 0;
            } break;

            case tt_Swamp1:  // BUGBUG: manual says 1/2 food
            case tt_Swamp2:  // BUGBUG: manual says 1/2 food
            case tt_Swamp3:  // BUGBUG: manual says 1/2 food
            {
                food_units = 0;
            } break;

            case tt_Tundra1:
            case tt_Tundra2:
            case tt_Tundra3:
            {
                food_units = 0;
            } break;

            case tt_SorceryNode:
            {
                food_units = 4;
            } break;

            case tt_NatureNode:
            {
                food_units = 4;  // BUGBUG  is 5 in Terrain.c // WZD s161p03  int16_t Square_Food2(int16_t wx, int16_t wy, int16_t wp)
            } break;

            case tt_ChaosNode:
            {
                food_units = 0;
            } break;

            case tt_Hills1:
            {
                food_units = 1;
            } break;

            case tt_Volcano:
            {
                food_units = 0;
            } break;

        }

    }

    return food_units;

}


// MGC o51p52
// drake178: TILE_GetMaxPop()
/*
; returns the amount of food found on the surrounding
; tiles in a radius equal to a city's catchment area
; (the base maximum population of a city built there)
*/
/*
Surveyor.c
    Surveyor_Window_Display()
        Compute_Base_Values_For_Map_Square(l_mx, l_my, _map_plane, &val, &production_bonus, &gold_bonus, &unit_cost_reduction, &gold_units, &magic_units, &have_nightshade, &have_mithril, &have_adamantium, &have_shore, &is_unexplored);
        Print(245, 151, str_MaximumPop);
        Print_Integer_Right(306, 151, val);
CITYCALC.c
    void Compute_Base_Values_For_Map_Square(int16_t wx, int16_t wy, int16_t wp, int16_t *MaxPop, int16_t *production_bonus, int16_t *gold_bonus, int16_t *unit_cost_reduction, int16_t *gold_units, int16_t *magic_units, int16_t *have_nightshade, int16_t *have_mithril, int16_t *have_adamantium, int16_t *have_shore, int16_t *is_unexplored)
        // HERE: *MaxPop is ...
        // *MaxPop = Square_Food2(curr_wx, itr_wy, wp);
        // *MaxPop += ((Tile_Food + food2_remainder) / 2);
        *MaxPop = (*MaxPop / 4);  // ¿ 2 food2 per population unit ?
        if(map_square_has_city == ST_TRUE)
            *MaxPop = City_Maximum_Size(city_idx);
CITYCALC.c
    int16_t City_Maximum_Size(int16_t city_idx)

¿ TILE_GetFood() ~== Square_Food2() ?

could be considered the equivalent of City_Maximum_Size
given that it's only missing City_Food_WildGame(), but that doesn't matter because we haven't done terrain specials yet

*/
int16_t City_Maximum_Size_NewGame(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t wy_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t wx_array[CITY_AREA_SIZE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t city_area_squares = 0;  // AKA useable_map_squares
    int16_t city_area_food_units = 0;  // _DI_  AKA food2_units
    int16_t itr = 0;  // _SI_

#ifdef STU_DEBUG
    if(
        (wx == 30)
        &&
        (wx == 20)
        &&
        (wp ==  0)
    )
    {
        STU_DEBUG_BREAK();
    }
#endif

    city_area_squares = Get_Useable_City_Area_NewGame(wx, wy, wp, &wx_array[0], &wy_array[0]);

    city_area_food_units = 0;

    for(itr = 0; itr < city_area_squares; itr++)
    {

        city_area_food_units += Square_Food2_NewGame(wx_array[itr], wy_array[itr], wp);

    }

    return (city_area_food_units / 2);

}


// MGC o51p53
// drake178: TILE_GetCatchment()
/*
*/
/*
CITYCALC.c
// WZD o142p08
int16_t Get_Useable_City_Area(int16_t city_wx, int16_t city_wy, int16_t city_wp, int16_t *wx_array, int16_t *wy_array)

*/
int16_t Get_Useable_City_Area_NewGame(int16_t city_wx, int16_t city_wy, int16_t city_wp, int16_t * wx_array, int16_t * wy_array)
{
    // DNE  uint8_t * terrain_flags_table_row = 0;
    int16_t square_y = 0;
    int16_t square_x_max = 0;
    int16_t square_x_min = 0;
    int16_t map_square_count = 0;  // _SI_
    int16_t itr_city_area_squares = 0;  // _CX_
    int16_t itr_world_x = 0;  // _DI_
    int16_t square_x = 0;  // _DX_

    map_square_count = 0;

    for(itr_city_area_squares = -2; itr_city_area_squares <= 2; itr_city_area_squares++)
    {

        square_y = (city_wy + itr_city_area_squares);

        if(
            (square_y >= 0)
            &&
            (square_y < WORLD_HEIGHT) )
        {

            square_x_min = -2;
            square_x_max =  2;

            if(
                (itr_city_area_squares == -2)
                ||
                (itr_city_area_squares ==  2)
            )
            {

                square_x_min = -1;
                square_x_max =  1;

            }

            // DNE  terrain_flags_table_row = (uint8_t *)&_map_square_flags[(city_wp * WORLD_SIZE) + (square_y * WORLD_WIDTH)];

            itr_world_x = square_x_min;

            while(itr_world_x <= square_x_max)
            {

                square_x = city_wx + itr_world_x;

                if(square_x < 0)
                {
                    square_x += WORLD_WIDTH;
                }

                if(square_x > WORLD_WIDTH)
                {
                    square_x -= WORLD_WIDTH;
                }

                // DNE  if( (*(terrain_flags_table_row + square_x) & MSF_CORRUPTION) == 0)
                // DNE  {
                    wx_array[map_square_count] = square_x;
                    wy_array[map_square_count] = square_y;
                    map_square_count++;
                // DNE  }

                itr_world_x++;

            }

        }

    }

    return map_square_count;

}


// MGC o51p54
// drake178: UU_Empty_Tile_Fn()
void o51p54_empty_function(void)
{
// push    bp
// mov     bp, sp
// pop     bp
// retf
}



/*

    MAGIC.EXE  ovr054

*/

// MGC o54p01
// drake178: CRP_SPATH_Arbitrary()
// CRP_SPATH_Arbitrary()
// MGC o54p02
// drake178: UU_SPATH_Segmented()
// UU_SPATH_Segmented()
// MGC o54p03
// drake178: UU_SPATH_15by15()
// UU_SPATH_15by15()
// MGC o54p04
// drake178: UU_SPATH_Segments()
// UU_SPATH_Segments()
// MGC o54p05
// drake178: UU_SPATH_GetBorderTiles()
// UU_SPATH_GetBorderTiles()



/*

    MAGIC.EXE  ovr055

*/

// drake178: CRP_NEWG_CreatePathGrid()
/*
*/
/*
*/
static void CRP_NEWG_CreatePathGrid__STUB(void * moves2, void * move_path_chunks)
{



}
