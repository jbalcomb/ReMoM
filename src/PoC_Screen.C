
#include "MoX.H"
#include "MoM_main.H"  /* current_screen; enum e_SCREENS */
#include "PoC_Screen.h"
#include "PNG_Draw.H"
#include "PNG_Load.H"
#include "FLIC_Draw2.H"
#ifdef STU_DEBUG
#include "STU_DBG.H"
#include "STU_TST.H"
#endif
// #include <assert.h>


struct s_mouse_list mouse_list_poc[1] = { {1, 0, 0, 0, 319, 199} };

char PoC_main_lbx_file[] = "MAIN";
uint8_t * PoC_main_background;
uint8_t * test_screen_background_seg;

char png_file_magic_spirit[] = "MagicSpirit.png";
char png_file_grassland_a[] = "00000000a.png";
struct s_PNG_PICT* magic_spirit_png_pict;
struct s_PNG_PICT* grassland_a_png_pict;



void Load_PoC_Resources(void)
{
    PoC_main_background = (uint8_t * )LBX_Load(PoC_main_lbx_file, 0);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: STU_TST: Validate_MAIN_LBX_000(): %d\n", __FILE__, __LINE__, Validate_MAIN_LBX_000(PoC_main_background));
#endif

    magic_spirit_png_pict = PNG_Load(png_file_magic_spirit);
    grassland_a_png_pict = PNG_Load(png_file_grassland_a);
}

void PoC_Screen_Add_Fields(void)
{

}

void PoC_Screen(void)
{
    int leave_screen_flag;
    int quit_hotkey;
    int input_field_idx;

    // Set_Mouse_List(1, mouse_list_poc);

    PoC_Screen_Add_Fields();
    // quit_hotkey = Add_Hot_Key("Q", ST_UNDEFINED);
    quit_hotkey = Add_Hot_Key('Q');

    // MoO2  Assign_Auto_Function()
    // Set_Redraw_Function()

    leave_screen_flag = ST_FALSE;
    while (leave_screen_flag == ST_FALSE)
    {
        Mark_Time();
        
        input_field_idx = Get_Input();
        // NOTE: we may have just recieved a Close, Destroy, and/or Quit message ... quit_game_flag = ST_TRUE;  current_screen = scr_Quit_To_DOS;
        // TODO  figure if/how to treat a Close Event as any other user input
        // if((input_field_idx == _quit_hotkey) || (input_field_idx == _quit_button) || (input_field_idx == _esc_hotkey))
        // {
        //     leave_screen_flag = ST_TRUE;
        //     current_screen = scr_Quit_To_DOS;
        // }
        // else if
        // {
        //     
        // }
        if(quit_game_flag == ST_TRUE)
        {
            current_screen = scr_Quit_To_DOS;
            leave_screen_flag = ST_TRUE;
        }
        else if(input_field_idx == quit_hotkey)
        {
            leave_screen_flag = ST_TRUE;
        }

        if(leave_screen_flag == ST_FALSE)
        {
            PoC_Screen_Draw();
            Toggle_Pages();
        }

        Release_Time(2);  // if my math is right, should should burn-out for 36 ms
        // NOTE: on Main_Menu_Screen, this is inside the block for do-draw-&-flip
        // NOTE: on Main_Screen, the ticks is 1
    }

}

void PoC_Screen_Draw(void)
{
    // test_screen_background_seg = LBX_Reload(halofam_lbx_file, 0, _screen_seg);
    // FLIC_Draw(0, 0, test_screen_background_seg);
    
    // switch (video_mode)
    // {
    //     case vm_Mode_Y:
    //     {
    //         FLIC_Draw(0, 0, PoC_main_background);
    //     } break;
    //     case vm_Mode_Y_2x:
    //     case vm_Mode_Y_2x_XBGR:
    //     {
    //         Draw(0, 0, PoC_main_background, 640, 2);
    //     } break;
    // }
    Draw(0, 0, PoC_main_background, 640, 2);

    int itr_x;
    int itr_y;
    for(itr_y = 0; itr_y < 10; itr_y++)
    {
        for(itr_x = 0; itr_x < 12; itr_x++)
        {
            PNG_Draw((0+(40*itr_x)), (40+(36*itr_y)), grassland_a_png_pict);
        }
    }

    PNG_Draw(21, 61, magic_spirit_png_pict);
    
}
