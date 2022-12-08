
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MainMenu.H"

#include "MoM_main.H"

#include "LBX_Load.H"
#include "FLIC_Draw.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


char mainscrn_lbx_file[] = "MAINSCRN";          // MGC  dseg:28A7
char vortex_lbx_file[] = "VORTEX";              // MGC  dseg:28B0

SAMB_ptr mainmenu_top;
SAMB_ptr mainmenu_bot;
SAMB_ptr mainmenu_c;
SAMB_ptr mainmenu_l;
SAMB_ptr mainmenu_n;
SAMB_ptr mainmenu_h;
SAMB_ptr mainmenu_q;

int16_t field_idx_hotkey_C;
int16_t field_idx_hotkey_L;
int16_t field_idx_hotkey_N;
int16_t field_idx_hotkey_H;
int16_t field_idx_hotkey_Q;
int16_t field_idx_hotkey_ESC;
int16_t field_idx_click_C;
int16_t field_idx_click_L;
int16_t field_idx_click_N;
int16_t field_idx_click_H;
int16_t field_idx_click_Q;


// MGC s01p04
void Main_Menu_Load_Pictures(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Menu_Load_Pictures()\n", __FILE__, __LINE__);
#endif

    mainmenu_top = LBX_Load(mainscrn_lbx_file, 0);
    mainmenu_bot = LBX_Load(mainscrn_lbx_file, 5);
    mainmenu_c   = LBX_Load(vortex_lbx_file, 1);
    mainmenu_h   = LBX_Load(vortex_lbx_file, 2);
    mainmenu_q   = LBX_Load(vortex_lbx_file, 3);
    mainmenu_n   = LBX_Load(vortex_lbx_file, 4);
    mainmenu_l   = LBX_Load(vortex_lbx_file, 5);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] mainmenu_top: %p\n", __FILE__, __LINE__, mainmenu_top);
    dbg_prn("DEBUG: [%s, %d] mainmenu_bot: %p\n", __FILE__, __LINE__, mainmenu_bot);
    dbg_prn("DEBUG: [%s, %d] mainmenu_c: %p\n", __FILE__, __LINE__, mainmenu_c);
    dbg_prn("DEBUG: [%s, %d] mainmenu_h: %p\n", __FILE__, __LINE__, mainmenu_h);
    dbg_prn("DEBUG: [%s, %d] mainmenu_q: %p\n", __FILE__, __LINE__, mainmenu_q);
    dbg_prn("DEBUG: [%s, %d] mainmenu_n: %p\n", __FILE__, __LINE__, mainmenu_n);
    dbg_prn("DEBUG: [%s, %d] mainmenu_l: %p\n", __FILE__, __LINE__, mainmenu_l);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Menu_Load_Pictures()\n", __FILE__, __LINE__);
#endif

}

// MGC s01p05
// int Main_Menu_Screen(void)
void Main_Menu_Screen(void)
{
    uint8_t flag_done;
    // int16_t main_menu_selection;
    int16_t input_field_idx;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Menu_Screen()\n", __FILE__, __LINE__);
#endif

    input_field_idx = ST_FALSE;
    // main_menu_selection = ST_UNDEFINED;
    flag_done = ST_FALSE;


    Main_Menu_Load_Pictures();

    Main_Menu_Add_Fields();

    field_idx_hotkey_Q = 1;

    while(flag_done == ST_FALSE)
    {
        // // input_field_idx = Get_Input();
        // input_field_idx = field_idx_hotkey_Q;
        // // if(input_field_idx != ST_FALSE) { flag_done = ST_TRUE; }
        // if((input_field_idx == field_idx_hotkey_Q) || (input_field_idx == field_idx_click_Q) || (input_field_idx == field_idx_hotkey_ESC))
        // {
        //     flag_done = ST_TRUE;
        //     // main_menu_selection = 3;
        //     g_Current_Screen = scr_Quit_To_DOS;
        // }
        // // if(main_menu_selection == ST_UNDEFINED) { flag_done = ST_FALSE; }

        if(flag_done == ST_FALSE)
        {
            Main_Menu_Screen_Draw();
            // Toggle_Pages();
            // STU_Export_VBB_To_BMP32();
        }
    }  /* while(flag_done == ST_FALSE) */

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Menu_Screen()\n", __FILE__, __LINE__);
#endif

}

void Main_Menu_Add_Fields(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Menu_Add_Fields()\n", __FILE__, __LINE__);
#endif



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Menu_Add_Fields()\n", __FILE__, __LINE__);
#endif
}

// MGC s01p06
void Main_Menu_Screen_Draw(void)
{
    uint16_t menu_x_start;
    uint16_t menu_y_start;
    uint8_t menu_shift;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Menu_Screen_Draw()\n", __FILE__, __LINE__);
#endif

    menu_x_start = 123;
    menu_y_start = 141;
    menu_shift = 0;

    FLIC_Draw(0, 0, mainmenu_top);
    FLIC_Draw(0, 41, mainmenu_bot);

    FLIC_Draw(menu_x_start, (menu_y_start + 12), mainmenu_l);
    FLIC_Draw(menu_x_start, (menu_y_start + (12 * menu_shift)), mainmenu_c);
    FLIC_Draw(menu_x_start, (menu_y_start + 24), mainmenu_n);
    FLIC_Draw(menu_x_start, (menu_y_start + 36), mainmenu_h);
    FLIC_Draw(menu_x_start, (menu_y_start + 48), mainmenu_q);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Menu_Screen_Draw()\n", __FILE__, __LINE__);
#endif

}
