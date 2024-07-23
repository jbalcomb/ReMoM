/*
    WIZARDS.EXE
        ovr160
    MAGIC.EXE
        ovr050

WZD
    Load_Screen
    Load_Screen_Draw
    Set_Load_Screen_Help_List_MGC
    Loaded_Game_Update
MGC
    Load_Screen
    Load_Screen_Draw

MoO2  Module: LOADSAVE

*/

#include "MoM.H"





// MGC dseg:290A
int16_t IDK_previous_screen = ST_UNDEFINED;

// MGC dseg:2ABC
struct s_mouse_list mouse_list_newgame0[1] = { {crsr_Finger, 0, SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX} };



// MGC dseg:303E                                                 ¿ BEGIN:  ovr050 ?

// MGC dseg:303E 57 49 5A 41 52 44 53 2E 45 58 45                cnst_EXESwap_File db 'WIZARDS.EXE'      ; DATA XREF: GAME_WizardsLaunch+38o

// MGC dseg:3049
char cnst_ZeroString_7[] = "";

// MGC dseg:304A 4A 45 4E 4E 59 00                               cnst_EXESwap_Arg db 'JENNY',0           ; DATA XREF: GAME_WizardsLaunch+30o ...

// MGC dseg:3050
char load_lbx_file[] = "LOAD.LBX";

// MGC dseg:3059
char cnst_HOTKEY_Esc3[] = "\x1B";

// MGC dseg:305B
char cnst_HOTKEY_L_2[] = "L";

// MGC dseg:305D
char cnst_SAVE3[] = "SAVE";

// MGC dseg:3062
char cnst_SAVE_ext3[] = ".GAM";



// MGC dseg:52B6
int16_t _load_button;

// MGC dseg:52B8
SAMB_ptr _background_seg;

// MGC dseg:52C0
int16_t _quit_button;

// MGC dseg:52F8
SAMB_ptr _load_inactive_seg;

// MGC dseg:52FC
int16_t g_GUI_MainScreenJump;

// MGC dseg:52FE
int16_t save_game_count;

// MGC dseg:64AE
int16_t screen_changed;

// MGC dseg:8A12
int16_t save_game_slots__[8];

// MGC dseg:8E8E
SAMB_ptr _quit_active_seg;

// MGC dseg:8E90
SAMB_ptr _load_active_seg;

// MGC dseg:8E92
int16_t _ok_button;

// MGC dseg:8E96
SAMB_ptr _selection_marker_seg;

// MGC dseg:8E98
int16_t selected_load_game_slot_idx;

// MGC dseg:8E9E
int16_t _settings_button;

// MGC dseg:8EA0
SAMB_ptr _settings_button_seg;

// MGC dseg:8EA2
SAMB_ptr _text_fill_seg;

// MGC dseg:8EAC
SAMB_ptr _ok_inactive_seg;

// MGC dseg:8EAE
SAMB_ptr _save_inactive_seg;





/*
    MAGIC.EXE  ovr050
*/

// WZD o50p01

// WZD o50p02

// WZD o50p03
/*
displays the load/save screen, from where the
settings screen can be reached or saved games can be
loaded (no saving here for obvious reasons)
*/
void Load_Screen_MGC__WIP(void)
{
    char found_file[20];
    char buffer2[16];
    char match_string[16];
    int16_t save_slot_fields[NUM_SAVE_SLOTS];
    // int16_t loaded_game_flag;  // DNE in MGC
    int16_t first_draw_done_flag;
    // int16_t x_start;
    int16_t input_field_idx;
    int16_t hotkey_ESC;
    int16_t y1_QuitLoadSave;
    int16_t x1_QuitLoadSave;
    int16_t Loop_Var;
    int16_t leave_screen_flag;  // _DI_
    int16_t itr;  // _SI_


    _background_seg        = LBX_Reload(     load_lbx_file,  0, _screen_seg);
    _quit_active_seg       = LBX_Reload_Next(load_lbx_file,  2, _screen_seg);
    _load_active_seg       = LBX_Reload_Next(load_lbx_file,  1, _screen_seg);
    _load_inactive_seg     = LBX_Reload_Next(load_lbx_file,  5, _screen_seg);  // WZD uses entry 6 for load_inactive
    _ok_inactive_seg       = LBX_Reload_Next(load_lbx_file,  8, _screen_seg);
    _text_fill_seg         = LBX_Reload_Next(load_lbx_file,  9, _screen_seg);
    _settings_button_seg   = LBX_Reload_Next(load_lbx_file, 12, _screen_seg);
    _selection_marker_seg  = LBX_Reload_Next(load_lbx_file, 10, _screen_seg);


    screen_changed == ST_FALSE;
    leave_screen_flag = ST_FALSE;
    first_draw_done_flag = ST_FALSE;

    if(IDK_previous_screen == scr_Settings_Screen)  /* ; 3 indicates returning from the settings screen */
    {
        screen_changed = ST_TRUE;
        Fade_Out();
        Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_BLACK);
        Set_Page_On();
        Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_BLACK);
        Set_Page_Off();
        Load_Palette(1, -1, ST_NULL);
    }

    x1_QuitLoadSave = 43;
    y1_QuitLoadSave = 171;  // ; y1 - Quit,Load,Save

    Set_Mouse_List(1, mouse_list_newgame0);

    Clear_Fields();

    hotkey_ESC = Add_Hot_Key(cnst_HOTKEY_Esc3[0]);

    _quit_button = Add_Hidden_Field(x1_QuitLoadSave, y1_QuitLoadSave, (x1_QuitLoadSave + 39), (y1_QuitLoadSave + 13), cnst_HOTKEY_Esc3[0], ST_UNDEFINED);

    _load_button = Add_Hidden_Field((x1_QuitLoadSave + 40), y1_QuitLoadSave, (x1_QuitLoadSave + 78), (y1_QuitLoadSave + 13), cnst_HOTKEY_Esc3[0], ST_UNDEFINED);

    _ok_button = INVALID_FIELD;

    save_game_count = 0;

    for(itr = 1; itr < NUM_SAVE_GAME_FILES; itr++)
    {
        strcpy(match_string, cnst_SAVE3);
        itoa(itr, buffer2, 10);
        strcat(match_string, buffer2);
        strcat(match_string, cnst_SAVE_ext3);
        if(DIR(match_string, found_file) == 0)  /* File Not Found */
        {
            save_game_slots__[save_game_count] = ST_UNDEFINED;
        }
        else
        {
            save_game_slots__[save_game_count] = itr;
            save_game_count++;
        }
    }

    for(itr = 0; itr < NUM_SAVE_SLOTS; itr)
    {
        save_slot_fields[itr] = INVALID_FIELD;
    }

    for(itr = 0; itr < save_game_count; itr)
    {
        // ; should either use GAME_ValidSaves_Main, or leave its counter alone too
        save_slot_fields[save_game_slots__[itr]] = Add_Hidden_Field(x1_QuitLoadSave, (47 + (save_game_slots__[itr] * 15)), (x1_QuitLoadSave + 220), (56 + (save_game_slots__[itr] * 15)), cnst_ZeroString_7[0], ST_UNDEFINED);
    }

    _settings_button = Add_Hidden_Field(172, y1_QuitLoadSave, 229, (y1_QuitLoadSave + 13), cnst_ZeroString_7[0], ST_UNDEFINED);

    selected_load_game_slot_idx = ST_UNDEFINED;

    leave_screen_flag = ST_FALSE;

    Assign_Auto_Function(Load_Screen_Draw_MGC__WIP, 1);

    Set_Load_Screen_Help_List_MGC();

    while(leave_screen_flag == ST_FALSE)
    {
        Mark_Time();

        input_field_idx = Get_Input();


        for(itr = 0; itr < NUM_SAVE_SLOTS; itr)
        {
            if(input_field_idx == save_slot_fields[itr])
            {
                for(Loop_Var = 0; Loop_Var < save_game_count; Loop_Var++)
                {
                    if(save_game_slots__[Loop_Var] == itr)
                    {
                        selected_load_game_slot_idx = itr;
                    }
                }
            }
        }


        if(
            (input_field_idx == _quit_button)
            &&
            (input_field_idx == hotkey_ESC)
        )
        {
            // in WZD,
            // no ESC hotkey
            // UNDEF's the loadsave_settings_flag
            // also calls an empty function pointer
            // Save_SAVE_GAM(8)
            // execl(MAGIC.EXE)
            leave_screen_flag = ST_TRUE;
            // TODO  SND_Empty_Fn();  // ; used in place of WIZARDS.EXE's SND_PlayClickSound
        }


        if(input_field_idx == _load_button)
        {
            Deactivate_Help_List();
            // ; fades out the screen, if the passed save index is not
            // ; that of the continue save (index 8), loads that game
            // ; and saves it as the current continue, then launches
            // ; WIZARDS.EXE (which always loads that save on startup)
            // TODO  GAME_WizardsLaunch(selected_load_game_slot_idx);
            leave_screen_flag = ST_TRUE;
        }


        if(input_field_idx == _settings_button)
        {
            Deactivate_Help_List();
            // TODO  Settings_Screen();
            g_GUI_MainScreenJump = 1;
            leave_screen_flag = ST_TRUE;
        }


        if(leave_screen_flag == ST_FALSE)
        {
            Load_Screen_Draw_MGC__WIP();

            Toggle_Pages();
            
            if(screen_changed == ST_FALSE)  /* ; in WZD, uses IDK_Load_Screen_Fade_In */
            {
                if(first_draw_done_flag == ST_FALSE)  /* ; in WZD, uses var_Skip_Fade_In */
                {
                    Copy_On_To_Off_Page();
                    // TODO  VGA_Fade_In();
                    first_draw_done_flag = ST_TRUE;
                    
                }
            }
            
            Release_Time(2);

        }
    }

    Deactivate_Auto_Function();
    Deactivate_Help_List();

}


// WZD o50p04
void Load_Screen_Draw_MGC__WIP(void)
{

}


// NewGame_Screen_1()
// NewGame_Screen_Draw_1()
// GAME_RandBookBinders()
// GAME_New_Screen_1()
// Draw_NewGame_Screen1()
// GAME_New_Screen_2()
// GAME_New_Screen_3()
// GAME_Draw_NewScr3()
// GAME_New_Screen_7()
// GAME_Draw_NewScr7()
// GAME_New_Screen_6()
// GAME_Draw_NewScr6()
// GAME_DrawRetortsStr()
// NEWG_DrawDefShelf()
// GAME_New_Screen_4()
// GAME_Draw_NewScr4()
// GAME_New_Screen_5()
// GAME_SpellSel_GUI()
// SCRN_Draw_NewScr5()
// SCRN_Draw_NewScr5_2()
// WIZ_CopyDefault()

// WZD o50p26
// Fade_Out()

// WZD o50p27
// VGA_Fade_In()

// WZD o50p28
// HLPENTRY.LBX  "wizview scrn Help"
void Set_Load_Screen_Help_List_MGC(void)
{
    LBX_Load_Data_Static(hlpentry_lbx_file, 6, (SAMB_ptr)_help_entries, 26, 4, 10);
    Set_Help_List((char *)&_help_entries[0], 4);
}

// HLP_Load_NewGOptions()
// HLP_Load_BannerSel()
// HLP_Load_PortraitSel()
// HLP_Load_WizardSel()
// HLP_Load_WizCreate()
// HLP_Load_RaceSel()
// STR_ListSeparator()
// CRP_Empty_Dialog_Fn2()
// CRP_Empty_Dialog_Fn1()
// Do_Toggle_Pages()
