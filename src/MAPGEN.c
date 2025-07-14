/*
    MAGIC.EXE
    ovr051
*/

#include "MAPGEN.h"

#include "MOX/FLIC_Draw.h"
#include "MOX/Fields.h"
#include "MOX/Fonts.h"
#include "MOX/Graphics.h"
#include "MOX/LBX_Load.h"
#include "MOX/Mouse.h"
#include "MOX/MOM_DATA.h"
#include "MOX/MOX_TYPE.h"
#include "MOX/Video.h"




// MGC dseg:8DD6
// drake78: IMG_NEWG_MapBuildBG
/*
    loaded in GAME_New_Screen_7()
    used in NEWG_CreateWorld__WIP()
*/
extern SAMB_ptr IMG_NEWG_MapBuildBG;



// MGC dseg:3334                                                 BEGIN:  ovr051 - Initialized Data

// MGC dseg:3334 00 00 FF FF 00 00 01 00 00 00                   TILE_Cardinal_XMod dw 0, 0FFFFh, 0, 1, 0
// MGC dseg:3334                                                                                         ; DATA XREF: NEWG_CreateLands+240r ...
// MGC dseg:333E 01 00 00 00 FF FF 00 00 00 00                   TILE_Cardinal_YMod dw 1, 0, 0FFFFh, 0, 0
// MGC dseg:333E                                                                                         ; DATA XREF: NEWG_CreateLands+24Fr ...
// MGC dseg:3348 02 00 03 00 00 00 01 00                         TILE_OppositeDirs dw 2, 3, 0, 1         ; DATA XREF: NEWG_CreateRiver+1F8r
// MGC dseg:3350 00 00 FF FF 00 00 01 00 00 00                   TILE_Cardinal_XMod2 dw 0, 0FFFFh, 0, 1, 0
// MGC dseg:3350                                                                                         ; DATA XREF: NEWG_SetSpecLands+1A3r ...
// MGC dseg:3350                                                                                         ; should use dseg:3334
// MGC dseg:335A 01 00 00 00 FF FF 00 00 00 00                   TILE_Cardinal_YMod2 dw 1, 0, 0FFFFh, 0, 0
// MGC dseg:335A                                                                                         ; DATA XREF: NEWG_SetSpecLands+1B2r ...
// MGC dseg:335A                                                                                         ; should use dseg:333e

// MGC dseg:3364
struct s_mouse_list mouselist_mapgen[1] =
{
    { crsr_Hourglass, 0, SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX }
};

// MGC dseg:3370 26 34 2C 34 37 34 43 34 50 34 60 34 68 34 77 34+UU_EZ_Names@_Array dw offset UU_cnst_Tower, offset UU_cnst_ChaosNode, offset UU_cnst_NatureNode, offset UU_cnst_SorceryNode, offset UU_cnst_Cave, offset UU_cnst_Dungeon, offset UU_cnst_AncientTmpl, offset UU_cnst_Keep, offset UU_cnst_Lair, offset UU_cnst_Ruins, offset UU_cnst_FallenTmpl ; "tower"
// MGC dseg:3386 A2 00 A2 00 A2 00 A2 00 BB 00 BB 00 BB 00 BB 00+TILE_River_Types dw _Grasslands1, _Grasslands1, _Grasslands1, _Grasslands1
// MGC dseg:3386 BC 00 BC 00 BC 00 BC 00 BD 00 C1 00 E9 00 BD 00+                                        ; DATA XREF: NEWG_SetRiverTiles+127t
// MGC dseg:3386 B9 00 B9 00 B9 00 B9 00 ED 00 EE 00 EF 00 ED 00+dw _River1000, _River1000, _River1000, _River1000
// MGC dseg:3386 BF 00 C3 00 EB 00 BF 00 FB 00 FC 00 FD 00 FE 00+dw _River0100, _River0100, _River0100, _River0100
// MGC dseg:3386 BA 00 BA 00 BA 00 BA 00 C0 00 C4 00 EC 00 C0 00+dw _River1100_1, _River1100_2, _River1100_3, _River1100_1
// MGC dseg:3386 F0 00 F1 00 F2 00 F0 00 F3 00 F4 00 F5 00 F6 00+dw _River0010, _River0010, _River0010, _River0010
// MGC dseg:3386 BE 00 C2 00 EA 00 BE 00 FF 00 00 01 01 01 02 01+dw _River1010_1, _River1010_2, _River1010_3, _River1010_1
// MGC dseg:3386 F7 00 F8 00 F9 00 FA 00 D4 01 D5 01 D6 01 D7 01 dw _River0110_1, _River0110_2, _River0110_3, _River0110_1
// MGC dseg:3386                                                 dw _River1110_1, _River1110_2, _River1110_3, _River1110_4
// MGC dseg:3386                                                 dw _River0001, _River0001, _River0001, _River0001
// MGC dseg:3386                                                 dw _River1001_1, _River1001_2, _River1001_3, _River1001_1
// MGC dseg:3386                                                 dw _River0101_1, _River0101_2, _River0101_3, _River0101_1
// MGC dseg:3386                                                 dw _River1101_1, _River1101_2, _River1101_3, _River1101_4
// MGC dseg:3386                                                 dw _River0011_1, _River0011_2, _River0011_3, _River0011_1
// MGC dseg:3386                                                 dw _River1011_1, _River1011_2, _River1011_3, _River1011_4
// MGC dseg:3386                                                 dw _River0111_1, _River0111_2, _River0111_3, _River0111_4
// MGC dseg:3386                                                 dw _River1111_1, _River1111_2, _River1111_3, _River1111_4

// MGC dseg:3406
char builddat_lbx_file__MGC_ovr051[] = "BUILDDAT";

// MGC dseg:340F
char str_BuildingTheWorlds[] = "Building The Worlds...";

// MGC dseg:3426 74 6F 77 65 72 00                               UU_cnst_Tower db 'tower',0              ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:342C 63 68 61 6F 73 20 6E 6F 64 65 00                UU_cnst_ChaosNode db 'chaos node',0     ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:3437 6E 61 74 75 72 65 20 6E 6F 64 65 00             UU_cnst_NatureNode db 'nature node',0   ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:3443 73 6F 72 63 65 72 79 20 6E 6F 64 65 00          UU_cnst_SorceryNode db 'sorcery node',0 ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:3450 6D 79 73 74 65 72 69 6F 75 73 20 63 61 76 65 00 UU_cnst_Cave db 'mysterious cave',0     ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:3460 64 75 6E 67 65 6F 6E 00                         UU_cnst_Dungeon db 'dungeon',0          ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:3468 61 6E 63 69 65 6E 74 20 74 65 6D 70 6C 65 00    UU_cnst_AncientTmpl db 'ancient temple',0
// MGC dseg:3477 61 62 61 6E 64 6F 6E 65 64 20 6B 65 65 70 00    UU_cnst_Keep db 'abandoned keep',0      ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:3486 6D 6F 6E 73 74 65 72 20 6C 61 69 72 00          UU_cnst_Lair db 'monster lair',0        ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:3493 61 6E 63 69 65 6E 74 20 72 75 69 6E 73 00       UU_cnst_Ruins db 'ancient ruins',0      ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:34A1 66 61 6C 6C 65 6E 20 74 65 6D 70 6C 65 00       UU_cnst_FallenTmpl db 'fallen temple',0 ; DATA XREF: dseg:UU_EZ_Names@_Arrayo
// MGC dseg:34AF 54 45 52 52 54 59 50 45 00                      cnst_TERRTYPE_File db 'TERRTYPE',0      ; DATA XREF: NEWG_SetDeserts+26o ...
// MGC dseg:34B8 43 49 54 59 4E 41 4D 45 00                      cnst_CITYNAME_File db 'CITYNAME',0      ; DATA XREF: CTY_SetDefaultName+23o

// MGC dseg:34C1 00 00 00                                        align 4

// MGC dseg:34C1                                                 END:  ovr051 - Initialized Data



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
    
    FLIC_Draw(75, 67, IMG_NEWG_MapBuildBG);

    Set_Font_Style(3, 3, ST_NULL, ST_NULL);

    Print(88, 76, str_BuildingTheWorlds);  // "Building The Worlds..."
    
    Clear_Fields();

    Deactivate_Auto_Function();
    
    Toggle_Pages();
    
    Copy_On_To_Off_Page();
    
    // TODO  WIZ_SetProfiles();

    Draw_Building_The_Worlds(5);

    // TODO  NEWG_ClearLandmasses(0);

    Draw_Building_The_Worlds(10);

    // TODO  NEWG_ClearLandmasses(1);

    Draw_Building_The_Worlds(15);

    // TODO  NEWG_CreateLands(0);

    Draw_Building_The_Worlds(20);

    // TODO  NEWG_CreateLands(1);

    Draw_Building_The_Worlds(25);

    // TODO  NEWG_SetBaseLands(0);

    Draw_Building_The_Worlds(30);

    // TODO  NEWG_SetBaseLands(1);

    Draw_Building_The_Worlds(35);

    // TODO  NEWG_SetSpecLands(0);

    Draw_Building_The_Worlds(40);

    // TODO  NEWG_SetSpecLands(1);

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

// o51p02
// NEWG_EZ_MarkHadnLeft()

// o51p03
// NEWG_TileIsleExtend()

// o51p04
// NEWG_CreateTowers()

// o51p05
// NEWG_CreateCapitals()

// o51p06
// NEWG_ClearLandmasses()

// o51p07
// NEWG_EqualizeNodes()

// o51p08
// NEWG_SetSpecLands()

// o51p09
// NEWG_SetBaseLands()

// o51p10
// NEWG_CreateLands()

// o51p11
// NEWG_CreateNodes()

// o51p12
// NEWG_CreateNodeAura()

// o51p13
// NODE_IsAuraUnique()

// o51p14
// NEWG_SetNodeType()

// o51p15
// TILE_SetLandMass()

// o51p16
// NEWG_CreateEZs()

// o51p17
// NEWG_CreateEncounter()

// o51p18
// UU_UNIT_RandomRacial()

// o51p19
// NEWG_SetDeserts()

// o51p20
// NEWG_CreateShores()

// o51p21
// NEWG_RandomizeTiles()

// o51p22
// NEWG_CreateRiver()

// o51p23
// NEWG_SetRiverTiles()

// o51p24
// NEWG_CreateNeutrals()

// o51p25
// CTY_SetDefaultName()

// o51p26
// NEWG_CreateRoads()

// o51p27
// CRP_NEWG_CreatePathGrids()

// o51p28
// NEWG_CreateSpecials()

// o51p29
// NEWG_DesertSpecials()

// o51p30
// NEWG_HillSpecials()

// o51p31
// NEWG_MntnSpecials()

// o51p32
// NEWG_SetScoutingMaps()

// o51p33
// TILE_SetScouting()

// o51p34
// NEWG_SetMoveMaps()

// o51p35
// TILE_InvalidOutflow()

// o51p36
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

// o51p37
// UNIT_Create()

// o51p38
// NEWG_AnimateOceans()

// o51p39
// TILE_IsOcean()

// o51p40
// TILE_HasNode()

// o51p41
// TILE_HasTower()

// o51p42
// TILE_IsForest()

// o51p43
// TILE_IsRiver()

// o51p44
// UU_TILE_IsRiverOutlet()

// o51p45
// UU_TILE_IsShore()

// o51p46
// TILE_IsMountains()

// o51p47
// TILE_IsHills()

// o51p48
// TILE_IsSwamp()

// o51p49
// TILE_IsDesert()

// o51p50
// TILE_IsGrasslands()

// o51p51
// TILE_GetFood()

// o51p52
// TILE_GetMaxPop()

// o51p53
// TILE_GetCatchment()

// o51p54
// UU_Empty_Tile_Fn()
