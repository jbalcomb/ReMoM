#ifndef MOX_H
#define MOX_H

/*
    SimTex Game-Engine (STGE)
    MoX
    Mox - Common

*/

/* ~ STGE */
#include "MoX\MoX_Lib.H"



/*
    ¿ MoX Migration Plan ?
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif





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
// // WZD s08p01
// void Near_Allocate_Mark(void);
// // WZD s08p02
// void Near_Allocate_Undo(void);
// WZD s08p03
void Near_Allocate_Reset(void);  // used by Hire_Hero_Load()
// WZD s08p04
byte_ptr Near_Allocate_First(int16_t size);
// WZD s08p05
byte_ptr Near_Allocate_Next(int16_t size);
// WZD s08p06
void Near_Allocation_Error(int16_t size);
// WZD s08p07
SAMB_ptr Allocate_Space(uint16_t size);
// // WZD s08p08 / s08p09
// SAMB_ptr Allocate_Space_No_Header(uint16_t size);
// WZD s08p10
void Mark_Block(SAMB_ptr block);
// WZD s08p11
void Release_Block(SAMB_ptr block);
// WZD s08p12
SAMB_ptr Allocate_First_Block(SAMB_ptr SAMB_head, uint16_t size);
// WZD s08p13
void Reset_First_Block(SAMB_ptr block);  // used by Hier_Hero_Load()
// WZD s08p14
SAMB_ptr Allocate_Next_Block(SAMB_ptr SAMB_head, uint16_t size);
// WZD s08p15
uint16_t Get_Free_Blocks(SAMB_ptr SAMB_head);
/*
    MoO2
    Module: allocate
*/
SAMB_ptr Allocate_Dos_Space(int32_t size);
SAMB_ptr Allocate_Dos_Data_Space(int32_t size);



/*
    LBX_Load.C
*/
enum enum_LBX_ERROR
{
    le_not_found          =  1,
    le_corrupted          =  2,
    le_low_RAM            =  3,
    le_alloc_fail         =  4,
    le_reload_fail        =  5,
    le_bad_header         =  7,
    le_entries_exceeded   =  8,
    le_recsize_mismatch   =  9,
    le_records_exceeded   = 10,
    le_load_reg_first     = 11,
    le_low_EMS            = 12,
    le_cantload_reserved  = 13,
    le_low_ResEM          = 14,
    le_EM_swap_fail       = 15,
    le_anim_over_64k      = 16
};
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
// WZD s10p15
void Error_Handler(char * file_name, int16_t error_num, int16_t entry_num, int16_t pages);

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



#ifdef __cplusplus
}
#endif

#endif  /* MOX_H */
