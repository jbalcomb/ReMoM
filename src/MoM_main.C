
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoM_main.H"

#include "MoX_CFG.H"
#include "MoX_Data.H"
#include "MoX_SET.H"
#include "MoX_GAM.H"

#include "Allocate.H"
#include "Fonts.H"
#include "Input.H"
#include "LBX_Load.H"
#include "Video.H"

#include "MainMenu.H"

#define STU_DEBUG 1
#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


#define MOM_FONT_FILE "FONTS.LBX"
#define GAME_FONT_FILE MOM_FONT_FILE


uint8_t g_State_Run;

int16_t g_Current_Screen;

// uint8_t g_Video_Back_Buffer[64000];

uint8_t g_Palette_XBGR[1024];





void Screen_Control(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Screen_Control()\n", __FILE__, __LINE__);
#endif

    switch(g_Current_Screen)
    {
        case scr_Main_Menu:
        {
            DLOG("case scr_Main_Menu:");
            Main_Menu_Screen();
        } break;
        case scr_Quit_To_DOS:
        {
            DLOG("case scr_Quit_To_DOS:");
            g_State_Run = ST_FALSE;
        } break;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Screen_Control()\n", __FILE__, __LINE__);
#endif

}



void MoM_main(void)
{
    char font_file[] = GAME_FONT_FILE;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: MoM_main()\n", __FILE__, __LINE__);
#endif

    // Init_STGE()
    // |-> Init_Drivers(GAME_FONT_FILE) |-> Load_Font_File(font_file)
    Load_Font_File(font_file);
    Init_Mouse_Keyboard(1);
    // TODO  Release_Version()
    // TODO  MoM_Tables_Init()
    // TODO  Set_Global_Escape()
    Load_Palette(0, -1, 0);
    // TODO  Apply_Palette()

    // ?
    // Init_MGC()
    // Init_WZD()
    // Init_MoM()
    // ?

    MoM_Tables_Init();

    Read_CONFIG_MOM();
    Write_CONFIG_MOM();
    Read_MAGIC_SET();
    Write_MAGIC_SET();
    Read_SAVE_GAM();
    Write_SAVE_GAM();

    Load_CONFIG_MOM();
    Load_MAGIC_SET();
    Load_SAVE_GAM(-1);
    Save_SAVE_GAM(0);


    // HERE:  draw_page_num = 0; current_video_page = 0xA000;
    // TODO  Fill(0,0,319,199);  // Clear Off-Screen
    // TODO  Set_Page_On()
    // TODO  Fill(0,0,319,199);  // Clear On-Screen
    // TODO  Set_Page_Off()
    draw_page_num = 0;
    current_video_page = video_page_buffer[0];    
    Set_Page_Off();


    Load_Palette(0, -1, 0);
    // TODO  Apply_Palette()

    g_Current_Screen = scr_Main_Menu;


    g_State_Run = 1;  // ST_TRUE


    // DBG  Main_Menu_Load_Pictures();
    // DBG  // FLIC_Draw(int x_start, int y_start, SAMB_ptr p_FLIC_File)
    // DBG  FLIC_Draw(0, 0, mainmenu_top);
    // DBG  FLIC_Draw(0, 41, mainmenu_bot);
    // DBG  int menu_x_start = 123;
    // DBG  int menu_y_start = 141;
    // DBG  int menu_shift = 0;
    // DBG  FLIC_Draw(menu_x_start, (menu_y_start + 12), mainmenu_l);
    // DBG  FLIC_Draw(menu_x_start, (menu_y_start + (12 * menu_shift)), mainmenu_c);
    // DBG  FLIC_Draw(menu_x_start, (menu_y_start + 24), mainmenu_n);
    // DBG  FLIC_Draw(menu_x_start, (menu_y_start + 36), mainmenu_h);
    // DBG  FLIC_Draw(menu_x_start, (menu_y_start + 48), mainmenu_q);
    // DBG  STU_Export_VBB_To_BMP32();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: MoM_main()\n", __FILE__, __LINE__);
#endif

}


void MoM_Tables_Init(void)
{
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: MoM_Tables_Init()\n", __FILE__, __LINE__);
#endif

    // 635 paragraphs = 16 * 635 = 10,160 bytes
    // Gfx_Swap = Allocate_Space(635);


    tmp_World_Data_Paras = 3519;
    // 3519 paragraphs = 16 * 3519 = 56,304 bytes
    World_Data = Allocate_Space(tmp_World_Data_Paras);
    // 714 paragraphs = 16 * 714 = 11,424 bytes
    TBL_Cities = Allocate_First_Block(World_Data, 714);
    // 602 paragraphs = 16 * 602 = 9632 bytes
    TBL_Maps = Allocate_Next_Block(World_Data, 602);

    UU_TBL_1 = Allocate_Next_Block(World_Data, 14);
    UU_TBL_2 = Allocate_Next_Block(World_Data, 14);

    TBL_Landmasses = Allocate_Next_Block(World_Data, 302);
    TBL_Terr_Specials = Allocate_Next_Block(World_Data, 302);
    TBL_Terrain_Flags = Allocate_Next_Block(World_Data, 302);
    TBL_Scouting = Allocate_Next_Block(World_Data, 302);

    Visibility_Arcanus = Allocate_Next_Block(World_Data, 19);
    Visibility_Myrror = Allocate_Next_Block(World_Data, 19);

    World_Data_Extra = Allocate_Next_Block(World_Data, Get_Free_Blocks(World_Data) - 1);

/*
929 = (3519 - 714 - 602 - 14 - 14 - 302 - 302 - 302 - 302 - 19 - 19)
2590 = 714 + 602 + 14+14 + 302+302+302+302 + 19+19
929 = (3519 - 2590)

1174 - 929 = 245
     1  Allocate_First_Block()
    10  Allocate_Next_Block()

SA_GET_SIZE(SAMB_head): 3519
SA_GET_USED(SAMB_head): 2345
3519 - 2345 = 1174

BEGIN: Allocate_First_Block( SAMB_head = 000001B4252A35EC, size = 714 )
SA_GET_SIZE(SAMB_head): 3519
SA_GET_USED(SAMB_head): 716

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 602 )
SA_GET_USED(SAMB_head): 716
SA_GET_USED(SAMB_head): 1063

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 14 )
SA_GET_USED(SAMB_head): 1063
SA_GET_USED(SAMB_head): 1078

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 14 )
SA_GET_USED(SAMB_head): 1078
SA_GET_USED(SAMB_head): 1093

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 302 )
SA_GET_USED(SAMB_head): 1093
SA_GET_USED(SAMB_head): 1396

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 302 )
SA_GET_USED(SAMB_head): 1396
SA_GET_USED(SAMB_head): 1699

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 302 )
SA_GET_USED(SAMB_head): 1699
SA_GET_USED(SAMB_head): 2258

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 302 )
SA_GET_USED(SAMB_head): 2258
SA_GET_USED(SAMB_head): 2305

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 19 )
SA_GET_USED(SAMB_head): 2305
SA_GET_USED(SAMB_head): 2325

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 19 )
SA_GET_USED(SAMB_head): 2325
SA_GET_USED(SAMB_head): 2345

BEGIN: Allocate_Next_Block( SAMB_head = 000001B4252A35EC, size = 1174 )
SA_GET_USED(SAMB_head): 2345

*/


    /*
    
    */
    // mov     ax, 632                         ; rewritten in the overland Djikstra patch
    // call    EMM_EMMDATAH_AllocFirst         ; clears the EMM Data block, then creates an LBX
    // mov     [word ptr TBL_TempMoveMap_EMS], ax ; 278h EMMData paragraphs
    // 
    // mov     ax, 1802
    // call    EMM_EMMDATAH_AllocNext          ; creates an LBX allocation into the EMM Data block
    // mov     [word ptr TBL_MoveMaps_EMS], ax ; 70Ah EMMData paragraphs
    // 
    // mov     ax, 301
    // call    EMM_EMMDATAH_AllocNext          ; creates an LBX allocation into the EMM Data block
    // mov     [word ptr TBL_SharedTiles_EMS@], ax ; bitflag table of the map tiles that are in the
    // 
    // mov     ax, 301
    // call    EMM_EMMDATAH_AllocNext          ; creates an LBX allocation into the EMM Data block
    // mov     [word ptr TBL_Catchments_EMS@], ax ; bitflag table of the map tiles that are in the
    // 
    // mov     ax, 1033
    // call    EMM_EMMDATAH_AllocNext          ; creates an LBX allocation into the EMM Data block
    // mov     [word ptr TBL_OvlMovePathsEMS@], ax ; 409h EMMData paragraphs

    TBL_TempMoveMap_EMS = Allocate_Space(632);
    TBL_MoveMaps_EMS = Allocate_Space(1802);
    TBL_SharedTiles_EMS = Allocate_Space(301);
    TBL_Catchments_EMS = Allocate_Space(301);
    TBL_OvlMovePaths_EMS = Allocate_Space(1033);

    /*
    
    */



    p0_heroes = Allocate_Space(28);  // 28 paragraphs = 448 bytes
    p1_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
    p2_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
    p3_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
    p4_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes
    p5_heroes = Allocate_Space(27);  // 27 paragraphs = 432 bytes


    TBL_Units = Allocate_Space(2028);  // 2028 paragraphs = 32448 bytes
    // Active_Unit = Allocate_Space(8);  // 8 paragraphs = 128 bytes
    TBL_Nodes = Allocate_Space(92);
    TBL_Fortresses = Allocate_Space(3);
    TBL_Towers = Allocate_Space(3);
    TBL_Encounters = Allocate_Space(351);
    TBL_Events = Allocate_Space(7);
    TBL_Hero_Names = Allocate_Space(37);
    TBL_Items = Allocate_Space(433);
    TBL_Premade_Items = Allocate_Space(17);
    // TBL_Spell_Data = Allocate_Space(485);
    // UnitDraw_WorkArea = Allocate_Space(60);
    // SND_Music_Segment = Allocate_Space(350);

// ?
// mov     [AI_Arc_MainWarConts@], (offset TBL_Wizards.Spells_Known+17E8h)
// mov     [AI_Myr_MainWarConts@], (offset TBL_Wizards.Spells_Known+17F4h)
// mov     [AI_CONTX_Reevals@], (offset TBL_Wizards.Spells_Known+1800h)
// mov     [Wiz5_Spell_28h@], (offset TBL_Wizards.Spells_Known+1810h)
// mov     [Wiz5_Spell_3Ch@], (offset TBL_Wizards.Spells_Known+1824h)
// mov     [Wiz5_Spell_50h@], (offset TBL_Wizards.Spells_Known+1838h)
// mov     [Wiz5_Spell_64h@], (offset TBL_Wizards.Spells_Known+184Ch)
// mov     [AI_Arc_NewColConts@], (offset TBL_Wizards.Spells_Known+1860h)
// mov     [AI_Myr_NewColConts@], (offset TBL_Wizards.Spells_Known+1874h)
// mov     [AI_Arc_NewColTgtXs@], (offset TBL_Wizards.Spells_Known+1888h)
// mov     [AI_Myr_NewColTgtXs@], (offset TBL_Wizards.Spells_Known+189Ch)
// mov     [AI_Arc_NewColTgtYs@], (offset TBL_Wizards.Spells_Known+18B0h)
// mov     [AI_Myr_NewColTgtYs@], (offset TBL_Wizards.Spells_Known+18BCh)
// mov     [AI_SCircle_Reevals@], (offset TBL_Wizards.Spells_Known+18C8h)
// ?

    // EMM_ContXXX_H = EMM_GetHandle(4, cnst_EMM_ContH_Name, 1)


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: MoM_Tables_Init()\n", __FILE__, __LINE__);
#endif

}
