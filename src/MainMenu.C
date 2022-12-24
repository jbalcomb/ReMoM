
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MainMenu.H"

#include "MoM_main.H"

#include "Fields.H"
#include "FLIC_Draw.H"
#include "Input.H"
#include "LBX_Load.H"
#include "Mouse.H"

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

int16_t main_menu_loaded = ST_FALSE;


int16_t mouse_list_menu_count = 1;
struct s_mouse_list mouse_list_menu[1] = {
    {1, 0, 0, 0, 319, 199}
};



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


// MGC ⊆ s01p05 Main_Menu_Screen()
void Main_Menu_Add_Fields(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Menu_Add_Fields()\n", __FILE__, __LINE__);
#endif

    Clear_Fields();

    // field_idx_click_C = Add_Hidden_Field(108, (138 + (12 * 0)), 211, (149 + (12 * 0)), 0, -1);
    // field_idx_click_L = Add_Hidden_Field(108, 150, 211, 161, 0, -1);
    // field_idx_click_N = Add_Hidden_Field(108, 162, 211, 173, 0, -1);
    // field_idx_click_H = Add_Hidden_Field(108, 174, 211, 185, 0, -1);
    // field_idx_click_Q = Add_Hidden_Field(108, 186, 211, 199, 0, -1);

    field_idx_hotkey_C = Add_Hot_Key('C');
    field_idx_hotkey_L = Add_Hot_Key('L');
    field_idx_hotkey_N = Add_Hot_Key('N');
    field_idx_hotkey_H = Add_Hot_Key('H');
    field_idx_hotkey_Q = Add_Hot_Key('Q');
    field_idx_hotkey_ESC = Add_Hot_Key('\x1B');

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Menu_Add_Fields()\n", __FILE__, __LINE__);
#endif
}


// MGC s01p05
void Main_Menu_Screen(void)
{
    int16_t input_field_idx;
    int16_t mouse_x;
    int16_t mouse_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Menu_Screen()\n", __FILE__, __LINE__);
#endif

    // TODO  ~== Clear Both Screen Pages
    // TODO  Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View

    if(!main_menu_loaded)
    {
        Main_Menu_Load_Pictures();
        main_menu_loaded = ST_TRUE;
    }

    input_field_idx = ST_FALSE;



    // TODO  if(screen_change) { Fade_Out(); }

    // TODO  Fill(0, 0, 319,199);
    // TODO  Set_Page_On();
    // TODO  Fill(0, 0, 319,199);
    // TODO  Set_Page_Off();

    Load_Palette(2, -1, 0);

    // TODO  Load_Settings()

    // TODO  Clear_Help()

    // ¿ TODO  Clear_Fields() ?
    Main_Menu_Add_Fields();

    Set_Mouse_List(1, mouse_list_menu);


    // CRL_Save_RSP(Pointer_X, Pointer_Y)
    // GUI_MouseEmuMove(g_GUI_NewGame_Label)


    // ? being here breaks the animation frame cycling ? FLIC_Reset_CurrentFrame(mainmenu_top);

    // TODO  Set_Redraw_Function(Main_Menu_Screen_Draw, 2);

    // TODO  Set_Help(HLP_IDX_0, 5);

    // TODO  Set_Input_Delay(4);



//  while(flag_done == ST_FALSE) {

        // TODO  CLK_Save();

        input_field_idx = Get_Input();

        if(input_field_idx == field_idx_hotkey_Q)
        {
            g_Current_Screen = scr_Quit_To_DOS;
        }

//      if(flag_done == ST_FALSE) {
    
            Main_Menu_Screen_Draw();
            Toggle_Pages();
            mouse_x = Pointer_X();
            mouse_y = Pointer_Y();
            Check_Mouse_Shape(mouse_x, mouse_y);  // AKA GUI_FindWindow();  // MGC s33p02  ST_GUI.H
            Draw_Mouse(mouse_x, mouse_y);
            // STU_Export_VBB_To_BMP32();

            // // Save_Mouse(mouse_x, mouse_y);
            // // Restore_Mouse();

//          if(!((screen_fade == ST_FALSE) & (draw_done != ST_FALSE))) {

                // TODO  Fade_In()
                // TODO  Copy_Off_To_On_Page();
                // TODO  draw_done = ST_TRUE;
                // TODO  screen_fade = ST_FALSE;

//          }  /* if(!((screen_fade == ST_FALSE) & (draw_done != ST_FALSE))) */

            // TODO  CLK_Wait(2);

//      }  /* if(flag_done == ST_FALSE) */

//  }  /* while ( flag_done == ST_FALSE ) */

    // TODO  Disable_Redraw();

    // TODO  Deactivate_Help_List();



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Menu_Screen()\n", __FILE__, __LINE__);
#endif

}

// MGC s01p06
void Main_Menu_Screen_Draw(void)
{
    int16_t logo_frame_index;
    int16_t itr_logo;
    uint16_t menu_x_start;
    uint16_t menu_y_start;
    uint8_t menu_shift;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Menu_Screen_Draw()\n", __FILE__, __LINE__);
#endif

    menu_x_start = 123;
    menu_y_start = 141;

    // TODO  mouse_field = Get_Mouse_Field();  // MGC s34p25 GUI_MousedControl()

    logo_frame_index = FLIC_Get_CurrentFrame(mainmenu_top);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] logo_frame_index: %d\n", __FILE__, __LINE__, logo_frame_index);
#endif
    FLIC_Set_CurrentFrame(mainmenu_top, 0);
    for(itr_logo = 0; itr_logo <= logo_frame_index; itr_logo++)
    {
        FLIC_Draw(0, 0, mainmenu_top);
    }
    FLIC_Draw(0, 41, mainmenu_bot);

    menu_shift = 0;

    FLIC_Draw(menu_x_start, (menu_y_start + 12), mainmenu_l);
    FLIC_Draw(menu_x_start, (menu_y_start + (12 * menu_shift)), mainmenu_c);
    FLIC_Draw(menu_x_start, (menu_y_start + 24), mainmenu_n);
    FLIC_Draw(menu_x_start, (menu_y_start + 36), mainmenu_h);
    FLIC_Draw(menu_x_start, (menu_y_start + 48), mainmenu_q);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Menu_Screen_Draw()\n", __FILE__, __LINE__);
#endif

}
