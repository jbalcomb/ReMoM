/*
    MAGIC.EXE
    ovr051

MoO2
    Module: HOMEGEN
    Module: INITGAME
    Module: MAPGEN
*/

#include "MAPGEN.h"

#include "../../MoM/src/INITGAME.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/Mouse.h"
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/Video.h"

#include "RACETYPE.h"
#include "special.h"
#include "TerrType.h"
#include "UNITTYPE.h"

#include <assert.h>     /* assert() */
#include <string.h>     /* memcpy() memset(), strcat(), strcpy(), stricmp() */



// MGC  dseg:8DD6
// drake78: IMG_NEWG_MapBuildBG
/*
    loaded in GAME_New_Screen_7()
    used in NEWG_CreateWorld__WIP()
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
// MGC  dseg:34B8 43 49 54 59 4E 41 4D 45 00                      cnst_CITYNAME_File db 'CITYNAME',0      ; DATA XREF: CTY_SetDefaultName+23o

// MGC  dseg:34C1 00 00 00                                        align 4

// MGC  dseg:34C1                                                 END:  ovr051 - Initialized Data





// MGC  dseg:8EB0                                                 BEGIN:  ovr051 - Uninitialized Data  (MAPGEN)
// MGC  dseg:8EB0
// MGC  dseg:8EB0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+NEWG_Landmass_Races dw 200 dup(0)       ; DATA XREF: NEWG_CreateNeutrals+39w ...

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
void NEWG_CreateWorld__WIP(void)
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

    NEWG_ClearLandmasses__WIP(0);

    Draw_Building_The_Worlds(10);

    NEWG_ClearLandmasses__WIP(1);

    Draw_Building_The_Worlds(15);

    NEWG_CreateLands__WIP(0);

    Draw_Building_The_Worlds(20);

    NEWG_CreateLands__WIP(1);

    Draw_Building_The_Worlds(25);

    NEWG_SetBaseLands__WIP(0);

    Draw_Building_The_Worlds(30);

    NEWG_SetBaseLands__WIP(1);

    Draw_Building_The_Worlds(35);

    NEWG_SetSpecLands__WIP(0);

    Draw_Building_The_Worlds(40);

    NEWG_SetSpecLands__WIP(1);

    Draw_Building_The_Worlds(45);

    NEWG_CreateNodes__WIP();

    Draw_Building_The_Worlds(50);

    NEWG_EqualizeNodes__WIP(0);

    NEWG_EqualizeNodes__WIP(1);

    Draw_Building_The_Worlds(55);

    NEWG_CreateTowers__WIP();

    Draw_Building_The_Worlds(60);

    NEWG_TileIsleExtend__WIP(0);

    NEWG_TileIsleExtend__WIP(1);

    NEWG_CreateEZs__WIP();

    Draw_Building_The_Worlds(65);

    _units = 0;

    // TODO  NEWG_CreateCapitals();

    Draw_Building_The_Worlds(70);

    // TODO  NEWG_CreateNeutrals(0);

    // TODO  NEWG_CreateNeutrals(1);

    Draw_Building_The_Worlds(75);

    // TODO  NEWG_CreateSpecials(0);

    // TODO  NEWG_CreateSpecials(1);

    // TODO  NEWG_CreateRoads(0);

    // TODO  NEWG_CreateRoads(1);

    Draw_Building_The_Worlds(80);

    // TODO  NEWG_CreateShores();

    Draw_Building_The_Worlds(85);
                                          
    for(IDK1 = 0; IDK1 < 10; IDK1++)
    {
        for(IDK2 = 0; IDK2 < 2000; IDK2++)
        {
            // TODO  if(NEWG_CreateRiver(0) != 0)
            // TODO  {
            // TODO      break;
            // TODO  }
        }
        for(IDK2 = 0; IDK2 < 2000; IDK2++)
        {
            // if(NEWG_CreateRiver(1) != 0)
            // {
            //     break;
            // }
        }

    }

    // TODO  NEWG_SetRiverTiles(0);

    // TODO  NEWG_SetRiverTiles(1);

    // TODO  NEWG_SetDeserts();

    Draw_Building_The_Worlds(85);

    // TODO  NEWG_RandomizeTiles();

    // TODO  NEWG_SetMoveMaps(0);

    // TODO  NEWG_SetMoveMaps(1);

    Draw_Building_The_Worlds(90);

    // TODO  CRP_NEWG_CreatePathGrids(0);

    // TODO  CRP_NEWG_CreatePathGrids(1);

    Draw_Building_The_Worlds(95);

    // TODO  NEWG_SetScoutingMaps();

    // TODO  NEWG_AnimateOceans();

    // TODO  NEWG_EZ_MarkHadnLeft();

}

// MGC o51p02
// drake178: NEWG_EZ_MarkHadnLeft()

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
void NEWG_CreateTowers__WIP(void)
{
    int16_t Tries = 0;
    int16_t Min_Distance = 0;
    int16_t YPos = 0;
    int16_t XPos = 0;
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

            XPos = (2 + Random(54));

            YPos = (2 + Random(34));

            if(
                (_world_maps[((ARCANUS_PLANE * WORLD_SIZE) + (YPos * WORLD_WIDTH) + XPos)] != tt_Ocean1)
                ||
                (_world_maps[((MYRROR_PLANE * WORLD_SIZE) + (YPos * WORLD_WIDTH) + XPos)] != tt_Ocean1)
                ||
                (Random(40) == 1)
            )
            {

                for(itr2 = 0; itr2 < itr1; itr2++)
                {

                    if(Delta_XY_With_Wrap(XPos, YPos, _TOWERS[itr2].wx, _TOWERS[itr2].wy, WORLD_WIDTH) < Min_Distance)
                    {

                        break;

                    }

                }

                for(itr2 = 0; itr2 < NUM_NODES; itr2++)
                {

                    if(Delta_XY_With_Wrap(XPos, YPos, _NODES[itr2].wx, _NODES[itr2].wy, WORLD_WIDTH) < 4)
                    {

                        break;

                    }

                }

                _TOWERS[itr1].wx = (int8_t)XPos;

                _TOWERS[itr1].wy = (int8_t)YPos;

                _TOWERS[itr1].owner_idx = ST_UNDEFINED;

                TILE_SetLandMass__WIP(ARCANUS_PLANE, XPos, YPos);

                TILE_SetLandMass__WIP(MYRROR_PLANE, XPos, YPos);

                _world_maps[((ARCANUS_PLANE * WORLD_SIZE) + (YPos * WORLD_WIDTH) + XPos)] = _Grasslands1;

                _world_maps[((MYRROR_PLANE * WORLD_SIZE) + (YPos * WORLD_WIDTH) + XPos)] = _Grasslands1;

                break;

            }

        }

    }

}


// MGC o51p05
// drake178: NEWG_CreateCapitals()

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
// TT_BugGrass    = 0x1,
// TT_Shore1_1st  = 0x2,
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
void NEWG_CreateNodes__WIP(void)
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
void NEWG_CreateEZs__WIP(void)
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

    _LAIRS[lair_idx].wx = wx;

    _LAIRS[lair_idx].wy = wy;

    _LAIRS[lair_idx].wp = wp;

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

                _LAIRS[lair_idx].guard1_unit_type = Picked_Unit_Type;

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

                    _LAIRS[lair_idx].guard2_unit_type = Picked_Unit_Type;

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

        _LAIRS[lair_idx].Spell_n_Special = Price;

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
// NEWG_SetDeserts()

// MGC o51p20
// drake178: NEWG_CreateShores()
// NEWG_CreateShores()

// MGC o51p21
// drake178: NEWG_RandomizeTiles()
// NEWG_RandomizeTiles()

// MGC o51p22
// drake178: NEWG_CreateRiver()
// NEWG_CreateRiver()

// MGC o51p23
// drake178: NEWG_SetRiverTiles()
// NEWG_SetRiverTiles()

// MGC o51p24
// drake178: NEWG_CreateNeutrals()
// NEWG_CreateNeutrals()

// MGC o51p25
// drake178: CTY_SetDefaultName()
// CTY_SetDefaultName()

// MGC o51p26
// drake178: NEWG_CreateRoads()
// NEWG_CreateRoads()

// MGC o51p27
// drake178: CRP_NEWG_CreatePathGrids()
// CRP_NEWG_CreatePathGrids()

// MGC o51p28
// drake178: NEWG_CreateSpecials()
// NEWG_CreateSpecials()

// MGC o51p29
// drake178: NEWG_DesertSpecials()
// NEWG_DesertSpecials()

// MGC o51p30
// drake178: NEWG_HillSpecials()
// NEWG_HillSpecials()

// MGC o51p31
// drake178: NEWG_MntnSpecials()
// NEWG_MntnSpecials()

// MGC o51p32
// drake178: NEWG_SetScoutingMaps()
// NEWG_SetScoutingMaps()

// MGC o51p33
// drake178: TILE_SetScouting()
// TILE_SetScouting()

// MGC o51p34
// drake178: NEWG_SetMoveMaps()
// NEWG_SetMoveMaps()

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
// UNIT_Create()

// MGC o51p38
// drake178: NEWG_AnimateOceans()
// NEWG_AnimateOceans()

// MGC o51p39
// drake178: TILE_IsOcean()
int16_t TILE_IsOcean(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;  // _CX_

    terrain_type = TERRAIN_TYPE(wx, wy, wp);

    if(
        (terrain_type != TT_BugGrass)
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
                (terrain_type > TT_Desert_end)
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
// drake178: TILE_IsForest()
// TILE_IsForest()

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
// TILE_IsMountains()

// MGC o51p47
// drake178: TILE_IsHills()
// TILE_IsHills()

// MGC o51p48
// drake178: TILE_IsSwamp()
// TILE_IsSwamp()

// MGC o51p49
// drake178: TILE_IsDesert()
// TILE_IsDesert()

// MGC o51p50
// drake178: TILE_IsGrasslands()
// TILE_IsGrasslands()

// MGC o51p51
// drake178: TILE_GetFood()
// TILE_GetFood()

// MGC o51p52
// drake178: TILE_GetMaxPop()
// TILE_GetMaxPop()

// MGC o51p53
// drake178: TILE_GetCatchment()
// TILE_GetCatchment()

// MGC o51p54
// drake178: UU_Empty_Tile_Fn()
// UU_Empty_Tile_Fn()
