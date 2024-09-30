#ifndef MOM_H
#define MOM_H

/* ~ STGE */
#include "MoX\MoX_Lib.H"

#include "MoM_DEF.H"

#include "MoM_Data.H"

#include "MoM_DBG.H"

/* ~ MoX ~ MoM v1.31*/
#include "AdvsrScr.H"
#include "AIBUILD.H"
#include "AIDATA.H"
#include "AIDUDES.H"
#include "ALLOC.H"
#include "ArmyList.H"
#include "City_ovr55.H"
#include "CityList.H"
#include "CITYCALC.H"
#include "CITYSCAP.H"
#include "CityScr.H"
#include "Combat.H"
#include "DIPLOMAC.H"
#include "EVENTS.H"
#include "Explore.H"
#include "Help.H"
#include "HIRE.H"
#include "Init.H"
#include "Input.H"
#include "ItemMake.H"
#include "Items.H"
#include "ItemScrn.H"
#include "ItemView.H"
#include "Keyboard.H"
#include "Lair.H"
#include "LOADER.H"
#include "LoadScr.H"
#include "LVLMAKE.H"
#include "MainMenu.H"
#include "MagicScr.H"
// TODO  #include "MainScr_Maps.H"
#include "MainScr.H"
#include "MovePath.H"
#include "MoX_CFG.H"
#include "MoX_CMB.H"
#include "MoX_DIR.H"
#include "MoX_DOS.H"
#include "MoX_GAM.H"
#include "MoX_RNG.H"
#include "MoX_UnOrg.H"  // WTFMATE
#include "MoX_Util.H"   // WTFMATE
#include "NEXTTURN.H"
#include "Outpost.H"
#include "paragrph.H"
#include "ProdScr.H"
#include "RACETYPE.H"
#include "REPORT.H"
#include "Roads.H"
#include "SBookScr.H"
#include "SCORE.H"      // Hall Of Fame (HoF)
#include "Settings.H"
#include "SETTLE.H"
#include "special.H"
#include "Spellbook.H"
#include "SOUND.H"
#include "SPLMASTR.H"
#include "Terrain.H"
#include "TerrType.H"
#include "Timer.H"
#include "UnitList.H"
#include "UnitMove.H"   // WTFMATE
#include "UnitStat.H"
#include "UNITSTK.H"
#include "UNITTYPE.H"   // WTFMATE
#include "UnitView.H"
#include "Video.H"
#include "Video2.H"
#include "WIZVIEW.H"
#include "WZD_o059.H"
#include "WZD_o129.H"
#include "WZD_o143.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif
#include "STU\STU_DBG.H"

#include "MoX_PFL.H"

#include <assert.h>
#include <math.h>       /* sqrt() */
#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), fseek(); */
#include <stdlib.h>     /* itoa(); */
#include <string.h>     /* memset(), strcat(), strcpy(); */

#ifdef __cplusplus
extern "C" {
#endif




/*
    WIZARDS.EXE  ovr067, ovr068, ovr150
    MainScr_Maps.C
*/
extern SAMB_ptr m_terrain_lbx_002;
extern uint16_t * m_terrain_lbx_001;
extern SAMB_ptr m_terrain_lbx_000;
// WZD o67p04
void Set_Draw_Active_Stack_Always(void);
// WZD o67p06
void Reset_Draw_Active_Stack(void);
// WZD o67p10
void Set_Entities_On_Map_Window(int16_t world_x, int16_t world_y, int16_t world_plane);
// WZD o67p13
void Set_Unit_Draw_Priority(void);
// WZD o67p14
void Reset_Stack_Draw_Priority(void);
// WZD o68p01
void Center_Map(int16_t * map_x, int16_t * map_y, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane);
// WZD o150p01
void Reset_Map_Draw(void);
// WZD o150p17
void Cartograph_Screen_Draw_Map_Terrain__NOWORKIE(int16_t cartograph_plane, SAMB_ptr cartograph_seg);



/*
    DNE
        Main Menu, Settings
    MGC
        {0,1,2,3,4} == {Continue,Load_Game,New_Game,Quit_To_DOS,Hall_Of_Fame}
    WZD
        {0,1,2,3,4,5,6,7,8,9,10,11/13,12,11/13,14,15} == {City,Load,Armies,Cities,Quit,MainGame,Magic,RoadBuilding,Production,Items,NextTurn,¿default?,Spellbook,¿default?,Advisor,Diplomacy}
*/
enum e_SCREENS
{
    scr_Main_Menu_Screen    = 0,

    scr_Continue            = 10,
    scr_Load_Screen         = 11,
    scr_New_Game_Screen     = 12,
    scr_Quit_To_DOS         = 13,
    scr_Hall_Of_Fame_Screen = 14,
    scr_Settings_Screen     = 15,

    /* WZD */
    scr_City_Screen         = 100,
    /* scr_Load_Screen = 101, */
    scr_Armies_Screen       = 102,
    scr_Cities_Screen       = 103,
    scr_Quit                = 104,
    scr_Main_Screen         = 105,
    scr_Magic_Screen        = 106,
    scr_Road_Build          = 107,
    scr_Production_Screen   = 108,  /* Production Pop-Up - ¿ from the City List Screen and City Screen ? */
    scr_Item_Screen         = 109,
    scr_NextTurn            = 110,  /* ¿ drake187: NextTurnProc ? ¿ MoO2: Do_Next_Turn_() ? */
    /* ¿default? */
    scr_Spellbook_Screen    = 112,
    /* ¿default? */
    scr_Advisor_Screen      = 114,   /* WZD 0x0E */
    scr_Diplomacy_Screen    = 115,
    // ¿!? Surveyor, Cartographer, etc.

    scr_Test_Screen         = 200,

    scr_PoC_Screen          = 240
};



/*
    WIZARDS.EXE seg001
*/

// WZD s01p02
// OON XREF:  main()  WZD s01p02
void Screen_Control(void);



#ifdef __cplusplus
}
#endif

#endif  /* MOM_H */
