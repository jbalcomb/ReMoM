
#include "MainMenu.H"
#include "MOM_SCR.H"
#include "Settings.H"



char mainscrn_lbx_file[] = "MAINSCRN";          // MGC  dseg:28A7
char vortex_lbx_file[] = "VORTEX";              // MGC  dseg:28B0

SAMB_ptr mainmenu_top;
SAMB_ptr mainmenu_bot;
SAMB_ptr mainmenu_c;
SAMB_ptr mainmenu_l;
SAMB_ptr mainmenu_n;
SAMB_ptr mainmenu_h;
SAMB_ptr mainmenu_q;

int16_t _continue_hotkey;
int16_t _load_hotkey;
int16_t _new_hotkey;
int16_t _hof_hotkey;
int16_t _quit_hotkey;
int16_t _esc_hotkey;


int16_t main_menu_screen_loaded = ST_FALSE;



// DNE in WZD
// MGC dseg:64AE
int16_t screen_change;


/*
    BEGIN: Main Menu Screen - Initialized Data
*/

// MGC dseg:205E 01 00 00 00 00 00 00 00 3F 01 C7 00 00 00       main_menu_window dw 1,0,0,0,319,199,0
struct s_mouse_list mouse_list_mainmenu[1] = {
    {1, 0, 0, 0, 319, 199}
};
// MGC dseg:206A 00 00                   dw 0
// MGC dseg:206C 00 00                   IDK_frame_count dw 0

/*
    END: Main Menu Screen - Initialized Data
*/

/*
    BEGIN: Main Menu Screen - Uninitialized Data
*/
// MGC dseg:52B2 _bdata_start

// MGC dseg:52B2 g_GUI_String_2 dw 0
// MGC dseg:52B4 g_GUI_String_1 dw 0
// MGC dseg:52B6
int16_t _load_button;
// MGC dseg:52B8 _background_seg dw 0
// MGC dseg:52BA gsa_VORTEX_3_MenuQuitToDOS dw 0
// MGC dseg:52BC
int16_t _hof_button;
// MGC dseg:52BE gsa_VORTEX_2_MenuHallOfFame dw 0
// MGC dseg:52C0
int16_t _quit_button;
// MGC dseg:52C2 gsa_VORTEX_5_MenuLoadGame dw 0
// MGC dseg:52C4
int16_t current_menu_screen;
// MGC dseg:52C6
// MGC dseg:52C6 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+wizard_portrait_segs dw 14 dup(    0)   ; array of 14 single-loaded images
// MGC dseg:52E2 IMG_NewG_BtnBorder@ dw 0
// MGC dseg:52E4
int16_t load_flag;
// MGC dseg:52E6 NEWG_HaveLastProfile dw 0
// MGC dseg:52E8 sa_MAINSCRN_000 dw 0
// MGC dseg:52EA
int16_t _new_button;
// MGC dseg:52EC gsa_VORTEX_4_MenuNewGame dw 0
// MGC dseg:52EE
int16_t _continue_button;
// MGC dseg:52F0 gsa_VORTEX_1_MenuContinue dw 0
// MGC dseg:52F2
int16_t cont_flag;
// MGC dseg:52F4 sa_MAINSCRN_005 dw 0
// MGC dseg:52F6 NEWG_Screen0_Frame dw 0
// MGC dseg:52F8 _load_inactive_seg dw 0
// MGC dseg:52FA NEWG_LastRetorts@ dw 0
// MGC dseg:52FC g_GUI_MainScreenJump dw 0
// MGC dseg:52FE
int16_t save_game_count;  // BUGBUG  why is this clashing with the one over in LoadScr?
// MGC dseg:5300
int16_t save_game_slots[8];  // BUGBUG  why is this clashing with the one over in LoadScr?

/*
    END: Main Menu Screen - Uninitialized Data
*/


// MGC s01p01

// MGC s01p02

// MGC s01p03
void Main_Menu_Screen_Control(void)
{
    char Read_Buffer[30];
    // file
    int16_t quit_flag;
    int16_t main_menu_selection;
    
    // if(DIR("CONFIG.MOM", Read_Buffer) == ST_FAILURE)  // offset CONFIG_MOM  MGC dseg:27FF CONFIG_MOM db 'CONFIG.MOM',0
    // {
    //     // dseg:280A cnst_ConfigErr db 'Run INSTALL to configure MASTER OF MAGIC.',0Ah,0Ah,0
    //     Exit_With_Message(cnst_ConfigErr);
    // }               

    // file = fopen(CONFIG_MOM, "RB");
    // fread(&config_mom, 18, 1, file);
    // fclose(file);

    quit_flag = ST_FALSE;

    // screen_change = ST_TRUE;
    // g_GUI_MainScreenJump = ST_UNDEFINED;
    // Set_Global_Escape();

    while(quit_flag == ST_FALSE)
    {
        // VGA_SetOutlineColor()
        // Deactivate_Help_List();
        main_menu_selection = Main_Menu_Screen();
        // Deactivate_Help_List();
        // screen_change = ST_FALSE;

/*
switch(current_screen)
0 case scr_Continue
1 case scr_LoadGame
2 case scr_NewGame
3 case scr_QuitToDos
4 case scr_HallOfFame
*/
        switch(main_menu_selection)
        {
            case 0:
            {
                // Stop_Music__STUB();
                // j_GAME_WizardsLaunch(8);
            } break;
            case 1:
            {
                // j_GAME_LoadSaveScreen();
            } break; 
            case 2:
            {
                // j_GAME_New_Create();
            } break;
            case 3:
            {
                quit_flag = ST_TRUE;
            } break;
            case 4:
            {
                // j_GAME_Hall_of_Fame();
                // j_Init_Credits();
            } break;
        }
        // IDK_previous_screen = e_ST_UNDEFINED;
    }

    // MGC dseg:2880 cnst_QUIT_Message db 'Thank you for playing Master of Magic!',0
    // Exit_With_Message(cnst_QUIT_Message);

}

// MGC s01p04
void Main_Menu_Load_Pictures(void)
{

    mainmenu_top = LBX_Load(mainscrn_lbx_file, 0);
    mainmenu_bot = LBX_Load(mainscrn_lbx_file, 5);
    mainmenu_c   = LBX_Load(vortex_lbx_file, 1);
    mainmenu_h   = LBX_Load(vortex_lbx_file, 2);
    mainmenu_q   = LBX_Load(vortex_lbx_file, 3);
    mainmenu_n   = LBX_Load(vortex_lbx_file, 4);
    mainmenu_l   = LBX_Load(vortex_lbx_file, 5);

}


// MGC ⊆ s01p05 Main_Menu_Screen()
void Main_Menu_Add_Fields(void)
{

    Clear_Fields();

    // _continue_button = Add_Hidden_Field(108, (138 + (12 * 0)), 211, (149 + (12 * 0)), 0, -1);
    // _load_button = Add_Hidden_Field(108, 150, 211, 161, 0, -1);
    // _new_button = Add_Hidden_Field(108, 162, 211, 173, 0, -1);
    // _hof_button = Add_Hidden_Field(108, 174, 211, 185, 0, -1);
    // _quit_button = Add_Hidden_Field(108, 186, 211, 199, 0, -1);

    _continue_hotkey = Add_Hot_Key('C');
    _load_hotkey = Add_Hot_Key('L');
    _new_hotkey = Add_Hot_Key('N');
    _hof_hotkey = Add_Hot_Key('H');
    _quit_hotkey = Add_Hot_Key('Q');
    _esc_hotkey = Add_Hot_Key('\x1B');

}


// MGC  s01p05  Main_Menu_Screen()
/*

    ¿ Oddities ?
        no call to Clear_Help()
        does not set input_field_idx to false/none
        DI is leave_screen_flag
        SI is input_field_idx
    
    Alterations:
        move adding button and hotkey fields to Main_Menu_Add_Fields()
            includes move the hotkey index variables out of Main_Menu_Screen()
            includes moving the call to Clear_Fields()
        move save game check loop to Check_For_Saved_Games()
        ? need to set current_screen here instead of passing a menu selection back to Screen_Control() ?
            ? add that code here or wrap this in another function ~ Main_Menu_Screen_Control() ?
            HACK:'d in current_screen

*/
int16_t Main_Menu_Screen(void)
{
    char Found_File_Name[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char Conversion_String[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char File_Name[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
// int16_t _load_hotkey = 0;
// int16_t _hof_hotkey = 0;
// int16_t _esc_hotkey = 0;
    int16_t first_draw_done_flag = 0;
    // int16_t _quit_hotkey = 0;
    // int16_t _new_hotkey = 0;
    // int16_t _continue_hotkey = 0;
    int16_t menu_shift = 0;
    int16_t itr_saves__retval = 0;
    int16_t leave_screen_flag = 0;  // _DI_
    int16_t input_field_idx = 0;  // _SI_

    int16_t mouse_x = 0;
    int16_t mouse_y = 0;
    
    int16_t hotkey_idx_Z = 0;  // debug_hotkey
    int16_t hotkey_idx_T = 0;  // test_hotkey

    if(!main_menu_screen_loaded)
    {
        Main_Menu_Load_Pictures();
        main_menu_screen_loaded = ST_TRUE;
    }

    // if(g_GUI_MainScreenJump != e_ST_UNDEFINED)
    // {
    //     itr_saves__retval = g_GUI_MainScreenJump;
    //     g_GUI_MainScreenJump = e_ST_UNDEFINED;
    //     return itr_saves__retval;
    // }

    // IDK_previous_screen = ST_UNDEFINED;
    first_draw_done_flag = ST_FALSE;
    cont_flag = ST_FALSE;
    // current_menu_screen = ST_UNDEFINED;
    load_flag = ST_FALSE;
    // IDK_frame_count = 0;

    leave_screen_flag = ST_FALSE;

    screen_change = ST_TRUE;

    if(screen_change == ST_FALSE)
    {

        Fade_Out();

    }

    Fill(0, 0, 319, 199, 7);
    Set_Page_On();
    Fill(0, 0, 319, 199, 3);
    Set_Page_Off();
    // CLS();

    Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View

    Load_MAGIC_SET();

    _help_entries[0].help_idx = ST_UNDEFINED;
    _help_entries[1].help_idx = ST_UNDEFINED;
    _help_entries[2].help_idx = ST_UNDEFINED;
    _help_entries[3].help_idx = ST_UNDEFINED;
    _help_entries[4].help_idx = ST_UNDEFINED;
    
    // ~ MoO2 Check_For_Saved_Games_()
    save_game_count = 0;
    for(itr_saves__retval = 1; itr_saves__retval < 10; itr_saves__retval++)
    {
        strcpy(File_Name, "SAVE");
#pragma warning(suppress : 4996)
        itoa(itr_saves__retval, Conversion_String, 10);
        strcat(File_Name, Conversion_String);
        strcat(File_Name, ".GAM");
        if(DIR(File_Name, Found_File_Name) != ST_FAILURE)
        {
            if(itr_saves__retval < 9)
            {
                // ; save_gam[array idx] = file slot idx
                save_game_slots[save_game_count] = itr_saves__retval;
                save_game_count++;
            }
            else
            {
                cont_flag = ST_TRUE;
            }
        }
        else
        {
            save_game_slots[save_game_count] = ST_UNDEFINED;
        }

    }
    if(save_game_count > 0)
    {
        load_flag = ST_TRUE;
    }

    
    // TODO Main_Menu_Add_Fields();
    Clear_Fields();
    menu_shift = 0;
    if(load_flag == ST_FALSE)
    {
        _load_button = INVALID_FIELD;
        menu_shift = 1;
    }
    else
    {
        _load_button = Add_Hidden_Field(108, 150, 211, 161, 0, ST_UNDEFINED);
        _help_entries[0].help_idx = HLP_LOAD;
        _help_entries[0].x1 = 108;
        _help_entries[0].y1 = 148;
        _help_entries[0].x2 = 211;
        _help_entries[0].y2 = 162;
    }

    if(cont_flag == ST_FALSE)
    {
        _continue_button = INVALID_FIELD;
    }
    else
    {
        _continue_button = Add_Hidden_Field(108, (138 + (12 * menu_shift)), 211, (149 + (12 * menu_shift)), 0, ST_UNDEFINED);
        _help_entries[1].help_idx = HLP_CONTINUE;
        _help_entries[1].x1 = 108;
        _help_entries[1].y1 = (138 + (12 * menu_shift));
        _help_entries[1].x2 = 211;
        _help_entries[1].y2 = (149 + (12 * menu_shift));
    }

    _new_button = Add_Hidden_Field(108, 162, 211, 173, 0, ST_UNDEFINED);
    _help_entries[1].help_idx = HLP_NEW_GAME;
    _help_entries[1].x1 = 108;
    _help_entries[1].y1 = 162;
    _help_entries[1].x2 = 211;
    _help_entries[1].y2 = 173;

    _hof_button = Add_Hidden_Field(108, 174, 211, 185, 0, ST_UNDEFINED);
    _help_entries[2].help_idx = HLP_HALL_OF_FAME;
    _help_entries[2].x1 = 108;
    _help_entries[2].y1 = 174;
    _help_entries[2].x2 = 211;
    _help_entries[2].y2 = 185;

    _quit_button = Add_Hidden_Field(108, 186, 211, 199, 0, ST_UNDEFINED);
    _help_entries[2].help_idx = HLP_QUIT_TO_DOS;
    _help_entries[2].x1 = 108;
    _help_entries[2].y1 = 186;
    _help_entries[2].x2 = 211;
    _help_entries[2].y2 = 199;

    if(cont_flag == ST_FALSE)
        _continue_hotkey = INVALID_FIELD;
    else
        _continue_hotkey = Add_Hot_Key('C');
    if(load_flag == ST_FALSE)
        _load_hotkey = INVALID_FIELD;
    else
        _load_hotkey = Add_Hot_Key('L');
    _new_hotkey = Add_Hot_Key('N');
    _hof_hotkey = Add_Hot_Key('H');
    _quit_hotkey = Add_Hot_Key('Q');
    _esc_hotkey = Add_Hot_Key(27); // ? '\1B' = 322?

    hotkey_idx_Z = Add_Hot_Key('Z');  // debug_hotkey  ...  Derp. 'D' is already used for the "Done" button.
    hotkey_idx_T = Add_Hot_Key('T');  // test_hotkey



    Set_Mouse_List(1, mouse_list_mainmenu);

    Save_Mouse_On_Page(Pointer_X(), Pointer_Y());



    // TODO  GUI_MouseEMUMoveTo(_newgame_button);



    // ¿ being here breaks the animation frame cycling ?
    // FLIC_ResetFrame(mainmenu_top);


    
    // AKA main_menu_selection
    current_menu_screen = ST_UNDEFINED;

    Assign_Auto_Function(Main_Menu_Screen_Draw, 2);

    Set_Help_List(_help_entries, 5);
    
    Set_Input_Delay(4);

    // DNE input_field_idx = ST_FALSE;
    while(leave_screen_flag == ST_FALSE)
    {

        Mark_Time();

        input_field_idx = Get_Input();

        if(quit_game_flag == ST_TRUE)
        {
            current_screen = scr_Quit_To_DOS;
            leave_screen_flag = ST_TRUE;
            current_menu_screen = 3;  // Quit To DOS
        }
        else
        {
            if(input_field_idx != ST_FALSE)
            {
                leave_screen_flag = ST_TRUE;
            }

            if((input_field_idx == _continue_hotkey) || (input_field_idx == _continue_button))
            {
                leave_screen_flag = ST_TRUE;
                current_menu_screen = 0;
                /* HACK */  current_screen = scr_Continue;
            }
            if((input_field_idx == _load_hotkey) || (input_field_idx == _load_button))
            {
                leave_screen_flag = ST_TRUE;
                current_menu_screen = 1;
                /* HACK */  current_screen = scr_Load_Screen;
            }
            if((input_field_idx == _new_hotkey) || (input_field_idx == _new_button))
            {
                leave_screen_flag = ST_TRUE;
                current_menu_screen = 2;
                /* HACK */  current_screen = scr_New_Game_Screen;
            }
            if((input_field_idx == _quit_hotkey) || (input_field_idx == _quit_button) || (input_field_idx == _esc_hotkey))
            {
                leave_screen_flag = ST_TRUE;
                current_menu_screen = 3;
                /* HACK */  current_screen = scr_Quit_To_DOS;
            }
            if((input_field_idx == _hof_hotkey) || (input_field_idx == _hof_button))
            {
                leave_screen_flag = ST_TRUE;
                current_menu_screen = 4;
                /* HACK */  current_screen = scr_Hall_Of_Fame_Screen;
            }
        }

        // ST_DEBUG Hot-Keys
        if(input_field_idx == hotkey_idx_Z)  /* Debug Hot-Key */
        {
            // // TODO  DLOG("STU_DEBUG: debug_hotkey");
            DBG_debug_flag = !DBG_debug_flag;  // ~== `^= 1`
            if(DBG_debug_flag)
            {
        
            }
        
        }
        if(input_field_idx == hotkey_idx_T)  /* Test Hot-Key */
        {

        }
        

        if(current_menu_screen == ST_UNDEFINED)
        {
            leave_screen_flag = ST_FALSE;
        }

        if(leave_screen_flag == ST_FALSE)
        {
            if(DBG_debug_flag)
            {
                Main_Menu_Screen_Draw_Debug();
            }
            else
            {
                Main_Menu_Screen_Draw();
            }
            Toggle_Pages();

            // do a full draw - initial or on-change
            if((screen_change != ST_FALSE) || (first_draw_done_flag == ST_FALSE))
            {
                Fade_In();
                Copy_On_To_Off_Page();
                first_draw_done_flag = ST_TRUE;
                screen_change = ST_FALSE;
            }
            Release_Time(2);
        }

    }  /* while(leave_screen_flag == ST_FALSE) */

    Deactivate_Auto_Function();
    Deactivate_Help_List();

    return current_menu_screen;

}

// MGC s01p06
void Main_Menu_Screen_Draw(void)
{
    int16_t logo_frame_idx;
    int16_t itr_logo;
    uint16_t menu_x_start;
    uint16_t menu_y_start;
    uint8_t menu_shift;
    int16_t scanned_field;

    menu_x_start = 123;
    menu_y_start = 141;

    scanned_field = Scan_Input();

    Set_Page_Off();
    Fill(0, 0, 319, 199, ST_BLACK);
    // CLROFF();

    /*
        draw the main menu logo, with animation, and the rest of the screen background
    */
    logo_frame_idx = FLIC_Get_CurrentFrame(mainmenu_top);
    FLIC_Set_CurrentFrame(mainmenu_top, 0);
    for(itr_logo = 0; itr_logo <= logo_frame_idx; itr_logo++)
    {
        FLIC_Draw(0, 0, mainmenu_top);
    }
    FLIC_Draw(0, 41, mainmenu_bot);


    if(screen_change == ST_FALSE)
    {
        Draw_Credits();
    }


    /*
        Draw Buttons
            NOTE: uses VORTEX, not MAINSCRN
    */
    menu_shift = 0;

    
    
    if(load_flag != ST_FALSE)
    {
        if(scanned_field == _load_button)
            FLIC_Reset_CurrentFrame(mainmenu_l);
        else
            FLIC_Set_CurrentFrame(mainmenu_l, 1);
        FLIC_Draw(menu_x_start, (menu_y_start + 12), mainmenu_l);
    }
    else
    {
        menu_shift = 1;
    }

    if(cont_flag != ST_FALSE)
    {
        if(scanned_field == _continue_button )
            FLIC_Reset_CurrentFrame(mainmenu_c);
        else
            FLIC_Set_CurrentFrame(mainmenu_c, 1);
        FLIC_Draw(menu_x_start, (menu_y_start + (12 * menu_shift)), mainmenu_c);
    }

    if(scanned_field == _new_button)
        FLIC_Reset_CurrentFrame(mainmenu_n);
    else
        FLIC_Set_CurrentFrame(mainmenu_n, 1);
    FLIC_Draw(menu_x_start, (menu_y_start + 24), mainmenu_n);

    if(scanned_field == _hof_button)
        FLIC_Reset_CurrentFrame(mainmenu_h);
    else
        FLIC_Set_CurrentFrame(mainmenu_h, 1);
    FLIC_Draw(menu_x_start, (menu_y_start + 36), mainmenu_h);

    if(scanned_field == _quit_button)
        FLIC_Reset_CurrentFrame(mainmenu_q);
    else
        FLIC_Set_CurrentFrame(mainmenu_q, 1);
    FLIC_Draw(menu_x_start, (menu_y_start + 48), mainmenu_q);

    // TODO  IDK_frame_count = ((IDK_frame_count + 1) % 20);

}


/*
NOTE(JimBalcomb,20250302): added per a request from Blake, but can't won't work ... EMM_Open_Handles, str_Initializing_Roland_drivers ... already deing in win_Mom, MoM_Data
*/
void Main_Menu_Screen_Draw_Debug()
{
    int16_t logo_frame_idx;
    int16_t itr_logo;
    uint16_t menu_x_start;
    uint16_t menu_y_start;
    uint8_t menu_shift;
    int16_t scanned_field;

    menu_x_start = 123;
    menu_y_start = 141;

    scanned_field = Scan_Input();

    Set_Page_Off();
    Fill(0, 0, 319, 199, ST_BLACK);

    /*
        draw the main menu logo, with animation, and the rest of the screen background
    */
    logo_frame_idx = FLIC_Get_CurrentFrame(mainmenu_top);
    FLIC_Set_CurrentFrame(mainmenu_top, 0);
    for(itr_logo = 0; itr_logo <= logo_frame_idx; itr_logo++)
    {
        FLIC_Draw(0, 0, mainmenu_top);
    }
    FLIC_Draw(0, 41, mainmenu_bot);

    if(screen_change == ST_FALSE) { /* Draw_Credits() */ }

    /*
        Draw Buttons
            NOTE: uses VORTEX, not MAINSCRN
    */
//     menu_shift = 0;
// 
//     
//     
//     if(load_flag != ST_FALSE)
//     {
//         if(scanned_field == _load_button)
//             FLIC_Reset_CurrentFrame(mainmenu_l);
//         else
//             FLIC_Set_CurrentFrame(mainmenu_l, 1);
//         FLIC_Draw(menu_x_start, (menu_y_start + 12), mainmenu_l);
//     }
//     else
//     {
//         menu_shift = 1;
//     }
// 
//     if(cont_flag != ST_FALSE)
//     {
//         if(scanned_field == _continue_button )
//             FLIC_Reset_CurrentFrame(mainmenu_c);
//         else
//             FLIC_Set_CurrentFrame(mainmenu_c, 1);
//         FLIC_Draw(menu_x_start, (menu_y_start + (12 * menu_shift)), mainmenu_c);
//     }
// 
//     if(scanned_field == _new_button)
//         FLIC_Reset_CurrentFrame(mainmenu_n);
//     else
//         FLIC_Set_CurrentFrame(mainmenu_n, 1);
//     FLIC_Draw(menu_x_start, (menu_y_start + 24), mainmenu_n);
// 
//     if(scanned_field == _hof_button)
//         FLIC_Reset_CurrentFrame(mainmenu_h);
//     else
//         FLIC_Set_CurrentFrame(mainmenu_h, 1);
//     FLIC_Draw(menu_x_start, (menu_y_start + 36), mainmenu_h);
// 
//     if(scanned_field == _quit_button)
//         FLIC_Reset_CurrentFrame(mainmenu_q);
//     else
//         FLIC_Set_CurrentFrame(mainmenu_q, 1);
//     FLIC_Draw(menu_x_start, (menu_y_start + 48), mainmenu_q);
// 
//     // TODO  IDK_frame_count = ((IDK_frame_count + 1) % 20);
// 
}
