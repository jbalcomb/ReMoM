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

#include "TerrType.h"

#include <assert.h>



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

    // TODO  NEWG_CreateNodes();

    Draw_Building_The_Worlds(50);

    // TODO  NEWG_EqualizeNodes(0);

    // TODO  NEWG_EqualizeNodes(1);

    Draw_Building_The_Worlds(55);

    // TODO  NEWG_CreateTowers();

    Draw_Building_The_Worlds(60);

    // TODO  NEWG_TileIsleExtend(0);

    // TODO  NEWG_TileIsleExtend(1);

    // TODO  NEWG_CreateEZs();

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

// MGC o51p04
// drake178: NEWG_CreateTowers()

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
// NEWG_EqualizeNodes()

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

                    assert(Processed_Tile_X >= 6);
                    assert(Processed_Tile_Y >= 6);
                    Next_Tile_X = (Processed_Tile_X + TILE_Cardinal_XMod[Direction]);
                    Next_Tile_Y = (Processed_Tile_Y + TILE_Cardinal_YMod[Direction]);
                    assert(Next_Tile_X >= 5);
                    assert(Next_Tile_Y >= 5);

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
// NEWG_CreateNodes()

// MGC o51p12
// drake178: NEWG_CreateNodeAura()
// NEWG_CreateNodeAura()

// MGC o51p13
// drake178: NODE_IsAuraUnique()
// NODE_IsAuraUnique()

// MGC o51p14
// drake178: NEWG_SetNodeType()
// NEWG_SetNodeType()

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
// NEWG_CreateEZs()

// MGC o51p17
// drake178: NEWG_CreateEncounter()
// NEWG_CreateEncounter()

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
// TILE_IsOcean()

// MGC o51p40
// drake178: TILE_HasNode()
// TILE_HasNode()

// MGC o51p41
// drake178: TILE_HasTower()
// TILE_HasTower()

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
