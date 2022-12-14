
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoM_main.H"

#include "Allocate.H"
#include "Fonts.H"
#include "LBX_Load.H"

#include "MainMenu.H"

#define STU_DEBUG 1
#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


#define MOM_FONT_FILE "FONTS.LBX"
#define GAME_FONT_FILE MOM_FONT_FILE


uint8_t g_State_Run;

int16_t g_Current_Screen;


uint8_t g_Video_Back_Buffer[64000];

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

    // Init_MGC()
    // Init_WZD()
    // Init_MoM()
    Load_Palette(0, -1, 0);
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
