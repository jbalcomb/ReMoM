#ifndef MOX_H
#define MOX_H

/*
    SimTex Game-Engine (STGE)
    MoX
    Mox - Common

*/

/* ~ STGE */
#include "MoX_TYPE.H"
#include "MoX_DEF.H"
#include "MoX_BITS.H"

#include "MoX_Data.H"   // WTFMATE

/* ~ MoX ~ MoM v1.31*/
#include "ALLOC.H"
#include "ArmyList.H"
#include "CITYCALC.H"
#include "CityList.H"
#include "Explore.H"
#include "Fields.H"
#include "Fonts.H"
#include "GENDRAW.H"
#include "Graphics.H"
#include "Help.H"
#include "Init.H"
#include "Input.H"
#include "Items.H"
#include "Keyboard.H"
#include "Lair.H"
#include "LOADER.H"
#include "Mouse.H"
#include "MovePath.H"
#include "MoX_CFG.H"
#include "MoX_CMB.H"
#include "MoX_DIR.H"
#include "MoX_GAM.H"
#include "MoX_RNG.H"
#include "MoX_SET.H"
#include "MoX_UnOrg.H"  // WTFMATE
#include "MoX_Util.H"   // WTFMATE
#include "NEXTTURN.H"
#include "paragrph.H"
#include "RACETYPE.H"
#include "SCORE.H"      // Hall Of Fame (HoF)
#include "SETTLE.H"
#include "special.H"
#include "Spellbook.H"
#include "Terrain.H"
#include "TerrType.H"
#include "Timer.H"
#include "UnitMove.H"   // WTFMATE
#include "UnitStat.H"
#include "UNITSTK.H"
#include "UNITTYPE.H"   // WTFMATE
#include "Video.H"
#include "Video2.H"
#include "WZD_o059.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

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
    Exit
*/
// WZD s05p02
// TODO  extern void (*Exit_With_Message)(char* string);

/*
    Allocate.C
*/
// WZD dseg:D49A
extern char near_buffer[];
// WZD s08p04
byte_ptr Near_Allocate_First(int16_t size);
// WZD s08p05
byte_ptr Near_Allocate_Next(int16_t size);
// WZD s08p07
SAMB_ptr Allocate_Space(uint16_t size);
// WZD s08p10
void Mark_Block(SAMB_ptr block);
// WZD s08p11
void Release_Block(SAMB_ptr block);
// WZD s08p12
SAMB_ptr Allocate_First_Block(SAMB_ptr SAMB_head, uint16_t size);
// WZD s08p14
SAMB_ptr Allocate_Next_Block(SAMB_ptr SAMB_head, uint16_t size);

/*
    FLIC_Draw.C
*/



/*
    LBX_Load.C
*/
// WZD s10p01
SAMB_ptr LBX_Load(char* lbx_name, int16_t entry_num);
// WZD s10p02
SAMB_ptr LBX_Reload(char* lbx_name, int16_t entry_num, SAMB_ptr SAMB_head);
// WZD s10p03
SAMB_ptr LBX_Reload_Next(char* lbx_name, int16_t entry_num, SAMB_ptr SAMB_head);
// WZD s10p04
SAMB_ptr LBX_Load_Data(char* lbx_name, int16_t entry_num, int16_t start_rec, int16_t num_recs, int16_t record_size);
// WZD s10p12
void LBX_Load_Data_Static(char* lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, uint16_t start_rec, uint16_t num_recs, uint16_t record_size);



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
    scr_Main_Menu_Screen = 0,

    scr_Continue = 10,
    scr_Load_Screen = 11,
    scr_New_Game_Screen = 12,
    scr_Quit_To_DOS = 13,
    scr_Hall_Of_Fame_Screen = 14,
    scr_Settings_Screen = 15,

    /* WZD */
    scr_City_Screen = 100,
    /* scr_Load_Screen = 101, */
    scr_Armies_Screen = 102,
    scr_Cities_Screen = 103,
    scr_Quit = 104,
    scr_Main_Screen = 105,
    scr_Magic_Screen = 106,
    scr_RoadBuilding = 107,  /* ?!? */
    scr_Production = 108,  /* Production Pop-Up - ¿ from the City List Screen and City Screen ? */
    scr_Item_Screen = 109,
    scr_NextTurn = 110,  /* ¿ drake187: NextTurnProc ? ¿ MoO2: Do_Next_Turn_() ? */
    /* ¿default? */
    scr_Spellbook_Screen = 112,
    /* ¿default? */
    scr_Advisor_Screen = 114,   /* WZD 0x0E */
    // scr_Diplomac = 115,  /* ?!? */
    scr_Diplomacy_Screen = 115,
    // ¿!? Surveyor, Cartographer, etc.

    scr_Test_Screen = 200,

    scr_PoC_Screen = 240
};



extern int16_t current_screen;
extern int16_t previous_screen;



#ifdef __cplusplus
}
#endif

#endif  /* MOX_H */
