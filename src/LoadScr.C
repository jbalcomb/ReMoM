/*
    WIZARDS.EXE
        ovr101
        ovr160
    MAGIC.EXE
        ovr050

WZD
    Load_Screen
    Load_Screen_Draw
    Set_Load_Screen_Help_List
    Loaded_Game_Update
MGC
    Load_Screen
    Load_Screen_Draw

MoO2  Module: LOADSAVE

*/

#include "LoadScr.H"

#include "MOX/LOADSAVE.H"
#include "MOX/MOX_DAT.H"  /* _screen_seg */
#include "MOX/MOX_SET.H"  /* magic_set */

#include "AIDUDES.H"
#include "CITYCALC.H"
#include "Combat.H"
#include "Explore.H"
#include "LOADER.H"
#include "MainScr.H"
#include "MainScr_Maps.H"
#include "MOM_SCR.H"
#include "NEXTTURN.H"
#include "Settings.H"
#include "Spellbook.H"
#include "UNITSTK.H"
#include "WZD_o143.H"



/*
    Settings.C
*/
// WZD dseg:60D4
extern struct s_mouse_list mouse_list_loadsave[1];
// WZD dseg:60E0
extern int16_t loadsave_settings_flag;
// WZD dseg:C9B0
extern int16_t loadsave_ok_button;
// WZD dseg:C9B8
extern int16_t load_screen_fade_in_flag;



// WZD dseg:712C                                                 BEGIN:  ovr160 - Strings  (LOADSAVE)

// WZD dseg:712C
char load_lbx_file__ovr160[] = "LOAD.LBX";

// WZD dseg:7134
char str_empty_string__ovr160[] = "";
// WZD dseg:7135
char cnst_SAVE3[] = "SAVE";
// WZD dseg:713A
char cnst_SAVE_ext3[] = ".GAM";
// WZD dseg:713F
char cnst_HOTKEY_Esc22 = '\x1B';

// WZD dseg:7141 4D 41 47 49 43 2E 45 58 45 00                   cnst_MAGIC_EXE_File3 db 'MAGIC.EXE',0   ; DATA XREF: Load_Screen+3D1o
// WZD dseg:714B 4A 45 4E 4E 59 00                               cnst_MAGICEXE_arg0_3 db 'JENNY',0       ; DATA XREF: Load_Screen+3CDo

// WZD dseg:7151
char load_screen_copyright[] = "Copyright  Simtex Software, 1995   V1.31";

// WZD dseg:717A
char hlpentry_lbx_file__ovr160[] = "hlpentry.lbx";

// WZD dseg:7187 00                                              align 2

// WZD dseg:7187                                                 END:  ovr160 - Strings  (LOADSAVE)



/*
                                            ¿ BEGIN: Settings / Load Screen - Uninitialized Data ?
*/

// WZD dseg:C980
int16_t loadsave_settings_button;

// WZD dseg:C982 00                                              db    0
// WZD dseg:C983 00                                              db    0
// WZD dseg:C984 00                                              db    0
// WZD dseg:C985 00                                              db    0
// WZD dseg:C986 00 00 00 00                                     RP_IMG_GUI_ChkBoxDn@ dw 2 dup(     0)   ; DATA XREF: Settings_Screen+B6w
// WZD dseg:C986                                                                                         ; array of 2 unused images appended into the sandbox
// WZD dseg:C98A 00                                              db    0
// WZD dseg:C98B 00                                              db    0
// WZD dseg:C98C 00 00 00 00 00 00                               IMG_GUI_ChkBoxNo@ dw 3 dup(     0)      ; DATA XREF: Settings_Screen+8Ew ...
// WZD dseg:C98C                                                                                         ; array of 3 images appended into the sandbox
// WZD dseg:C992                                                 ; unsigned int IMG_GUI_ChkBoxYes
// WZD dseg:C992 00 00 00 00 00 00                               IMG_GUI_ChkBoxYes@ dw 3 dup(     0)     ; DATA XREF: Settings_Screen+66w ...
// WZD dseg:C992                                                                                         ; array of 3 images appended into the sandbox
// WZD dseg:C998 00 00                                           magic_set_ptr dw 0                ; DATA XREF: Settings_Screen:loc_A90CCw ...

// WZD dseg:C99A
SAMB_ptr save_inactive;

// WZD dseg:C99C
// AKA IDK_SaveSlots_Array
int16_t save_game_slots__ovr160[NUM_SAVE_SLOTS];  // BUGBUG  why is this clashing with the one over in MainMenu?

// WZD dseg:C9AC
int16_t loadsave_save_button;

// WZD dseg:C9AE
int16_t loadsave_quit_button;

// WZD dseg:C9B0
// Settings.C  int16_t loadsave_ok_button;

// WZD dseg:C9B2
int16_t selected_load_game_slot_idx;

// WZD dseg:C9B4
int16_t selected_save_game_slot_idx;

// WZD dseg:C9B6
int16_t loadsave_load_button;

// WZD dseg:C9B8
// Settings.C  int16_t load_screen_fade_in_flag;  // ; set to ST_FALSE on entering the settings screen

// WZD dseg:C9BA
SAMB_ptr load_inactive;

// WZD dseg:C9BC
SAMB_ptr selection_marker;

// WZD dseg:C9BE
int16_t save_game_count__ovr160;  // BUGBUG  why is this clashing with the one over in MainMenu?

// WZD dseg:C9C0
SAMB_ptr settings_button;

// WZD dseg:C9C2
SAMB_ptr loadsave_text_fill_seg;

// WZD dseg:C9C4
SAMB_ptr ok_active;

// WZD dseg:C9C6
SAMB_ptr load_active;

// WZD dseg:C9C8
SAMB_ptr quit_active;

// WZD dseg:C9CA
// MGC dseg:52B8
SAMB_ptr loadsave_background;

/*
                                            ¿ END: ovr160 - Uninitialized Data  LOADSAVE ?
*/
// WZD dseg:C9CC 00 00                                           IMG_GUI_ScrollDnArr@ dw 0               ; DATA XREF: sub_A9A30+31r ...



// dseg:D494 00 00                                           GUI_Help_NearAlloc dw 0                 ; DATA XREF: Draw_Help_Entry_+BBw ...
// dseg:D494                                                                                         ; 1048 bytes of LBX_NearAlloc_First space

// dseg:D496
SAMB_ptr save_active;

// dseg:D498 00 00                                           LBX_NearBuf_Mark dw 0                   ; DATA XREF: LBX_NearAlloc_Mark:loc_150B1w ...








/*
    WIZARDS.EXE ovr101
*/

// WZD o101p01  [1 of 1]
void GAME_NextHumanStack(void)
{
    
    _unit_window_start_x = 247;
    _unit_window_start_y = 79;

    _combat_wx = ST_UNDEFINED;
    _combat_wy = ST_UNDEFINED;

Check_Cities_Data();
    Update_Scouted_And_Contacted__WIP();
Check_Cities_Data();

    // ; does nothing and returns zero; at some point must have been some wizard data refresh function
    // DONT  o62p01_Empty_pFxn(_human_player_idx);

Check_Cities_Data();
    WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
Check_Cities_Data();

Check_Cities_Data();
    Update_Scouted_And_Contacted__WIP();
Check_Cities_Data();

}



/*
    WIZARDS.EXE ovr160
*/

// WZD o160p01
void Load_Screen(void)
{
    char found_file[LEN_FILE_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char buffer2[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char match_string[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t save_slot_fields[NUM_SAVE_SLOTS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t loaded_game_flag = 0;  // DNE in MGC
    int16_t first_draw_done_flag = 0;
    int16_t x_start = 0;
    int16_t input_field_idx = 0;
    int16_t hotkey_ESC = 0;
    int16_t itr_save_game_count = 0;
    int16_t leave_screen_flag = 0;
    int16_t itr_save_slot_fields = 0;  // _SI_
    int16_t y_start = 0;  // _DI_
    int16_t itr = 0;  // _SI_
    int16_t itr_save_slot_input_field_array = 0;  // _SI_

    loaded_game_flag = ST_FALSE;

    load_screen_fade_in_flag = ST_TRUE;  // set to 0 on entering the settings screen

    // TODO: maybe, someday, come to terms with the way this logic looks in the Dasm. Pretty sure it's testing for a specific/known valid value and resetting on anything else
    if(loadsave_settings_flag != 3)  /* 3 indicates returning from the settings screen */
    {
        Fade_Out();
        Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, 0);
        Set_Page_On();
        Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, 0);
        Set_Page_Off();
        Load_Palette(1, -1, 0);  // LOADSAVE - load save palette
    }
    else
    {
        loadsave_settings_flag = ST_UNDEFINED;
    }

    // LOAD.LBX, 000  LOADSAVE    Load & Save back
    // LOAD.LBX, 001  LOADSAVE    Load active
    // LOAD.LBX, 002  LOADSAVE    Quit active
    // LOAD.LBX, 003  LOADSAVE    Save active
    // LOAD.LBX, 004  LOADSAVE    Ok   active
    // LOAD.LBX, 005  LOAD_BUT    Load inactive
    // LOAD.LBX, 006  LOAD_BUT    Quit inactive
    // LOAD.LBX, 007  LOAD_BUT    Save inactive
    // LOAD.LBX, 008  LOAD_BUT    Ok   inactive
    // LOAD.LBX, 009  LOADSAVE    Text Fill
    // LOAD.LBX, 010  LOADBACK    selection marker
    // LOAD.LBX, 011  SETTING2    Settings Backgrnd
    // LOAD.LBX, 012  LOADSAVE    Settings button

    loadsave_background     = LBX_Reload(     load_lbx_file__ovr160,  0, _screen_seg);
    quit_active             = LBX_Reload_Next(load_lbx_file__ovr160,  2, _screen_seg);
    load_active             = LBX_Reload_Next(load_lbx_file__ovr160,  1, _screen_seg);
    save_active             = LBX_Reload_Next(load_lbx_file__ovr160,  3, _screen_seg);
    ok_active               = LBX_Reload_Next(load_lbx_file__ovr160,  4, _screen_seg);
    // DNE  Quit inactive
    load_inactive           = LBX_Reload_Next(load_lbx_file__ovr160,  6, _screen_seg);  // MGC uses entry 5 for load_inactive
    save_inactive           = LBX_Reload_Next(load_lbx_file__ovr160,  7, _screen_seg);
    // DNE  Ok   inactive
    loadsave_text_fill_seg  = LBX_Reload_Next(load_lbx_file__ovr160,  9, _screen_seg);
    selection_marker        = LBX_Reload_Next(load_lbx_file__ovr160, 10, _screen_seg);
    settings_button         = LBX_Reload_Next(load_lbx_file__ovr160, 12, _screen_seg);


    save_game_count__ovr160 = 0;
    
    for(itr = 1; itr < NUM_SAVE_GAME_FILES; itr++)
    {
        strcpy(match_string, cnst_SAVE3);
        itoa(itr, buffer2, 10);
        strcat(match_string, buffer2);
        strcat(match_string, cnst_SAVE_ext3);
        if(DIR(match_string, found_file) == 0)  /* File Not Found */
        {
            save_game_slots__ovr160[save_game_count__ovr160] = ST_UNDEFINED;
        }
        else
        {
            save_game_slots__ovr160[save_game_count__ovr160] = itr;
            save_game_count__ovr160++;
        }
    }


    leave_screen_flag = ST_FALSE;    // BUG: set below
    selected_load_game_slot_idx = ST_UNDEFINED;    // BUG: set below  // controls drawing of load_active : load_inactive
    selected_save_game_slot_idx = ST_UNDEFINED;  // controls drawing of save_active : save_inactive
    first_draw_done_flag = ST_FALSE;

    x_start = 43;   // ; start X for Quit, Load, Save buttons
    y_start = 171;  // ; start Y for Quit, Load, Save buttons

    Set_Mouse_List(1, mouse_list_loadsave);

    Clear_Fields();

    hotkey_ESC = Add_Hot_Key(cnst_HOTKEY_Esc22);

    loadsave_quit_button      = Add_Hidden_Field( x_start       , y_start, (x_start +  39), (y_start + 13), str_empty_string__ovr160[0], ST_UNDEFINED);
    loadsave_load_button      = Add_Hidden_Field((x_start +  40), y_start, (x_start +  78), (y_start + 13), str_empty_string__ovr160[0], ST_UNDEFINED);
    loadsave_save_button      = Add_Hidden_Field(     122       , y_start,      159       , (y_start + 13), str_empty_string__ovr160[0], ST_UNDEFINED);
    loadsave_ok_button        = Add_Hidden_Field(     231       , y_start,      270       , (y_start + 13), str_empty_string__ovr160[0], ST_UNDEFINED);
    loadsave_settings_button  = Add_Hidden_Field(     172       , y_start,      229       , (y_start + 13), str_empty_string__ovr160[0], ST_UNDEFINED);

    Set_Font_Style(3, 1, 3, ST_NULL);

    for(itr_save_slot_fields = 0; itr_save_slot_fields < NUM_SAVE_SLOTS; itr_save_slot_fields++)
    {
        save_slot_fields[itr_save_slot_fields] = Add_Continuous_String_Input_Field(x_start, (47 + (itr_save_slot_fields * 15)), 260, magic_set.Save_Names[itr_save_slot_fields], (LEN_SAVE_DESCRIPTION - 1), ST_TRANSPARENT, selection_marker, ST_UNDEFINED, ST_NULL);
    }

    selected_load_game_slot_idx = ST_UNDEFINED;  // BUG: set above

    leave_screen_flag = ST_FALSE;  // BUG: set above

    Assign_Auto_Function(Load_Screen_Draw, 1);

    Set_Load_Screen_Help_List();

    while(leave_screen_flag == ST_FALSE)
    {

        input_field_idx = Get_Input();

        Mark_Time();


        /*
            check each of all 8 game slot fields
                if matched, set flag to draw save button
                check that slot index against the indices of existing save games
                    if matched, set flag to draw load button
        */
        for(itr_save_slot_fields = 0; itr_save_slot_fields < NUM_SAVE_SLOTS; itr_save_slot_fields++)
        {
            if(save_slot_fields[itr_save_slot_fields] == input_field_idx)
            {
                selected_save_game_slot_idx = itr_save_slot_fields;
                selected_load_game_slot_idx = ST_UNDEFINED;
                for(itr_save_game_count = 0; itr_save_game_count < save_game_count__ovr160; itr_save_game_count++)
                {
                    if(save_game_slots__ovr160[itr_save_game_count] == itr_save_slot_fields)
                    {
                        selected_load_game_slot_idx = itr_save_slot_fields;
                    }
                }
            }
        }


        if(input_field_idx == loadsave_quit_button)
        {
            // ~== current_screen = scr_Main_Menu_Screen
            // DONT  loadsave_settings_flag = ST_UNDEFINED;
            // DONT  s01p15_Empty_pFxn();
            // DONT  Save_SAVE_GAM(8);
            // DONT  GAME_EXE_Swap("MAGIC.EXE", "JENNY", str_empty_string__ovr160, str_empty_string__ovr160);

            loadsave_settings_flag = ST_UNDEFINED;
            Save_SAVE_GAM(8);
            current_screen = scr_Main_Menu_Screen;
            leave_screen_flag = ST_TRUE;
        }


        if( (input_field_idx == loadsave_ok_button) || (input_field_idx == hotkey_ESC) )
        {
            loadsave_settings_flag = ST_UNDEFINED;
            leave_screen_flag = ST_TRUE;
        }


        if( (input_field_idx == loadsave_load_button) && (selected_load_game_slot_idx >= 0) )
        {
            loadsave_settings_flag = ST_UNDEFINED;
            Deactivate_Help_List();
            Load_SAVE_GAM(selected_load_game_slot_idx);
            loaded_game_flag = ST_TRUE;
            leave_screen_flag = ST_TRUE;
            /* HACK */ current_screen = scr_Main_Screen;
        }


        if( (input_field_idx == loadsave_save_button) && (selected_save_game_slot_idx >= 0) )
        {
            loadsave_settings_flag = ST_UNDEFINED;
            Save_SAVE_GAM(selected_save_game_slot_idx);
            leave_screen_flag = ST_TRUE;
        }


        /*
            Settings Screen
        */
        if(input_field_idx == loadsave_settings_button)
        {
            loadsave_settings_flag = 2;
            Settings_Screen();
            leave_screen_flag = ST_TRUE;
        }


        if(leave_screen_flag == ST_FALSE)
        {
            Load_Screen_Draw();
            Toggle_Pages();
            if( (load_screen_fade_in_flag != ST_FALSE) && (first_draw_done_flag == ST_FALSE) )
            {
                Copy_On_To_Off_Page();
                if(loadsave_settings_flag != 3)  /* ; 3 indicates returning from the settings screen */
                {
                    Fade_In();
                }
                first_draw_done_flag = ST_TRUE;
            }
            Release_Time(1);
        }
    }


// @@LeaveScreen

    Clear_Fields();
    Deactivate_Auto_Function();
    Deactivate_Help_List();

    // How did I get here?  ...Menu or Main?
    // But, what if I selected 'Quit'?
    if(current_screen != scr_Main_Menu_Screen)
    {
        if(loadsave_settings_flag == ST_UNDEFINED)
        {
            // TODO  WZD vs. MGC
            // current_screen = scr_Main_Screen;
            // HACK: merge MGC Load_Screen() and WZD Load_Screen()
            current_screen = previous_screen;
        }
        if (loaded_game_flag == ST_TRUE)
        {
            current_screen = scr_Main_Screen;
        }
    }

    if(current_screen == scr_Main_Screen)
    {
        _page_flip_effect = pfe_TogglePagesFadeIn;
        Fade_Out();
        Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);
        Set_Page_On();
        Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);
        Set_Page_Off();
        // HERE: set the palette back to the /normal/ palette
        Load_Palette(0, ST_UNDEFINED, ST_NULL);  // EMPERATO - main game palette
        Set_Palette_Changes(0, 255);
        Calculate_Remap_Colors();
        Cycle_Palette_Color__STUB(198, 40, 0, 0, 63, 0, 0, 1);
    }

    if(loaded_game_flag == ST_TRUE)
    {
        Loaded_Game_Update();
    }

}

// WZD o160p02
void Load_Screen_Draw(void)
{
    /*
        MGC
            uint8_t color_array[12];
    */
    /*
        WZD
            uint8_t color_array[4];
            int16_t x_start;
    */
    uint8_t color_array[4];
    int16_t x_start;

    int16_t moused_field;
    int16_t itr_save_gam;

    /*
        MGC
            mov     ax, [word ptr COL_SaveLoad1+2]
            mov     dx, [word ptr COL_SaveLoad1]
            mov     [word ptr bp+var_4+2], ax
            mov     [word ptr bp+var_4], dx
            mov     ax, [word ptr COL_SaveLoad2+2]
            mov     dx, [word ptr COL_SaveLoad2]
            mov     [word ptr bp+var_8+2], ax
            mov     [word ptr bp+var_8], dx
    */
    /*
        WZD
            color_array[0] = 34;
            color_array[1] = 43;
    */
    color_array[0] = 34;
    color_array[1] = 43;

    /* MGC  current_menu_screen = Auto_Input(); */
    moused_field = Auto_Input();

    x_start = 43;

    color_array[2] = 171;

    Set_Page_Off();

    Fill(0, 0, SCREEN_XMAX, SCREEN_YMAX, 0);

    /* MGC  FLIC_Draw(0, 0, _background_seg); */
    FLIC_Draw(0, 0, loadsave_background);

    /* MGC  Fill( 42, 170, 123, 184, 0); */
    Fill( 42, 170, 160, 184, 0);

    Fill(171, 170, 271, 184, 0);


    for(itr_save_gam = 0; itr_save_gam < save_game_count__ovr160; itr_save_gam++)
    {
        FLIC_Draw(x_start, (47 + (save_game_slots__ovr160[itr_save_gam] * 15)), loadsave_text_fill_seg);
    }


    if(moused_field == loadsave_quit_button)
    {
        // manually, draw in the down-state
        FLIC_Draw(44, 172, quit_active);
    }
    else
    {
        FLIC_Draw(43, 171, quit_active);
    }


    if(moused_field == loadsave_settings_button)
    {
        // manually, draw in the down-state
        FLIC_Draw(173, 172, settings_button);
    }
    else
    {
        FLIC_Draw(172, 171, settings_button);
    }


    if(selected_load_game_slot_idx >= 0)
    {
        if(moused_field == loadsave_load_button)
        {
            // manually, draw in the down-state
            FLIC_Draw(84, 172, load_active);
        }
        else
        {
            FLIC_Draw(83, 171, load_active);
        }
    }
    else
    {
        FLIC_Draw(83, 171, load_inactive);
    }


    if(selected_save_game_slot_idx >= 0)
    {
        if(moused_field == loadsave_save_button)
        {
            // manually, draw in the down-state
            FLIC_Draw(123, 172, save_active);
        }
        else
        {
            FLIC_Draw(122, 171, save_active);
        }
    }
    else
    {
        FLIC_Draw(122, 171, save_inactive);
    }


    if(moused_field == loadsave_ok_button)
    {
        // manually, draw in the down-state
        FLIC_Draw(232, 172, ok_active);
    }
    else
    {
        FLIC_Draw(231, 171, ok_active);
    }

    Set_Font_Colors_15(0, &color_array[0]);
    Print_Centered(160, 193, load_screen_copyright);  // "Copyright  Simtex Software, 1995   V1.31"

}

// WZD o160p03
// HLPENTRY.LBX,  26  "Load Screen Help"
// HLPENTRY.LBX,  27  "Save Screen Help"
void Set_Load_Screen_Help_List(void)
{
    LBX_Load_Data_Static(hlpentry_lbx_file__ovr160, 27, (SAMB_ptr)_help_entries, 0, 6, 10);
    Set_Help_List((char *)&_help_entries[0], 27);  // ¿ BUGBUG - someone put entry_num instead of help_count ?
}

// WZD o160p04
/*
main_()
    |-> Loaded_Game_Update__seg001()
Load_Screen()
    |-> Loaded_Game_Update__ovr160()
*/
void Loaded_Game_Update(void)
{
    int16_t itr;
    int16_t itr_players;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Loaded_Game_Update()\n", __FILE__, __LINE__);
#endif


    /* seg001 */ GAME_RazeCity = ST_FALSE;


    _human_player_idx = HUMAN_PLAYER_IDX;


    _page_flip_effect = pfe_None;
    

    Reset_City_Area_Bitfields();


    Init_Overland();


    Patch_Units_Upkeep_And_Sound();


    // DONT  o142p04  NOOP_Current_Player_All_City_Areas() |-> empty_fxn_o142p03()


    // DONT  o142p19  LD_MAP_TFUnk40_Eval()


Check_Cities_Data();
    All_Colony_Calculations();
Capture_Cities_Data();


    _unit_stack_count = 0;


    // DOMSDOS  Play_Background_Music__STUB();
    sdl2_Play_Background_Music__WIP();


    Cache_Graphics_Overland();


    Reset_City_Road_Connection_Bitfields();


    Delete_Dead_Units();


    All_AI_Refresh_Units_Movement();


    /* seg001 */  for(itr = 1; itr < NUM_PLAYERS; itr++)
    /* seg001 */  {
    /* seg001 */      _players[itr].average_unit_cost = 0;
    /* seg001 */  }


    GAME_NextHumanStack();


    if(_difficulty == god_Intro)
    {
        magic_set.random_events = ST_FALSE;
    }


    /*
        BEGIN:  AI CONT / MOVE
    */
    for(itr = 0; itr < 100; itr++)
    {
        TBL_OvlMovePaths_EMS[itr] = ST_UNDEFINED;
    }
    CRP_UNIT_OverlandPath = ST_UNDEFINED;
    CONTX_CreateChains__WIP();
    CONTX_CreateLChains__WIP();
    for(itr = 0; itr < _num_players; itr++)
    {
        _ai_reevaluate_continents_countdown[itr] = ST_FALSE;
    }
    /*
        END:  AI CONT / MOVE
    */


    g_TimeStop_PlayerNum = ST_NONE;
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        if(_players[itr_players].Globals[TIME_STOP] > 0)
        {
            g_TimeStop_PlayerNum = (itr_players + 1);
        }
    }



    /*
        BEGIN:  STU Debug
    */

        // TST_Validate_GameData();

        TST_Patch_Game_Data();

    /*
        END:  STU Debug
    */

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Loaded_Game_Update()\n", __FILE__, __LINE__);
#endif

}


// WZD o51p01
// drake178: GAME_Overland_Init()
/*
OON XREF: Loaded_Game_Update()

_main()     |-> Loaded_Game_Update_WZD()          |-> Init_Overland() |-> PreInit_Overland()
Load_Screen |-> [WZD ovr160] Loaded_Game_Update() |-> Init_Overland() |-> PreInit_Overland()

SEEALSO:  MoM-Init.md
*/
void Init_Overland(void)
{
    int16_t itr_cities = 0;  // _SI_
    int16_t itr_units = 0;  // _SI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Init_Overland()\n", __FILE__, __LINE__);
#endif


    PreInit_Overland();


    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        _CITIES[itr_cities].bldg_status[NONE] = bs_Built;
    }

    
    for(itr_units = 0; itr_units < _units; itr_units++)
    {

        if(_UNITS[itr_units].wp == 2)
        {
            _UNITS[itr_units].wp = 0;
        }

        // NOTE: the DASM thinks world_plane is passed here as well, but IsPassableTower() makes no xref to it
        // TODO  if(IsPassableTower(_UNITS[itr_units].wx, _UNITS[itr_units].wy, _UNITS[itr_units].wp) == ST_TRUE)
        if(IsPassableTower(_UNITS[itr_units].wx, _UNITS[itr_units].wy) == ST_TRUE)
        {
            _UNITS[itr_units].in_tower = ST_TRUE;
        }
        else
        {
            _UNITS[itr_units].in_tower = ST_FALSE;
        }

    }


    skill_staff_locked = 0;
    mana_staff_locked = 0;
    research_staff_locked = 0;
    _players[HUMAN_PLAYER_IDX].Nominal_Skill = Player_Base_Casting_Skill(HUMAN_PLAYER_IDX);
    

    // NIU?  CRP_OVL_MapWindowX = 0;
    // NIU?  CRP_OVL_MapWindowY = 0;
    _prev_world_x = 0;
    _prev_world_y = 0;
    _map_x = 0;
    _map_y = 0;

    // DONT  j_o62p01_empty_function(_human_player_idx)

    _unit = 0;  // 0: None / No Unit

    _active_world_x = _FORTRESSES[HUMAN_PLAYER_IDX].wx;
    _active_world_y = _FORTRESSES[HUMAN_PLAYER_IDX].wy;

    _unit_window_start_x = 247;
    _unit_window_start_y = 79;

    _map_plane = _FORTRESSES[HUMAN_PLAYER_IDX].wp;

    All_AI_Players_Contacted();

    Allocate_Reduced_Map();

    Center_Map(&_map_x, &_map_y, _FORTRESSES[HUMAN_PLAYER_IDX].wx, _FORTRESSES[HUMAN_PLAYER_IDX].wy, _map_plane);

    Set_Unit_Draw_Priority();

    Reset_Stack_Draw_Priority();

    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

    Cache_Graphics_Overland();

Capture_Cities_Data();
    WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);
Check_Cities_Data();

    // DONT  j_o108p02_empty_function()

    All_City_Calculations();
Capture_Cities_Data();

    if(Check_Release_Version() == ST_FALSE)
    {
        __debugbreak();
        Set_Random_Seed(10039);  // 0x2737
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Init_Overland()\n", __FILE__, __LINE__);
#endif
}

// WZD o51p02
// drake178: G_WLD_StaticAssetRfrsh()
/*
OON XREF: Loaded_Game_Update() |-> Init_Overland()

Loaded_Game_Update()
    |-> Init_Overland()
        |-> PreInit_Overland()

SEEALSO:  MoM-Init.md
*/
void PreInit_Overland(void)
{
    uint32_t debug_random_seed = 0;
    int16_t itr_cities = 0;
    int16_t itr_players = 0;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: PreInit_Overland()\n", __FILE__, __LINE__);
#endif


    Randomize();
    debug_random_seed = 1000;
    if(!Check_Release_Version())
    {
        Set_Random_Seed(debug_random_seed);
    }


    all_units_moved = ST_FALSE;
    reset_active_stack = ST_TRUE;
    Reset_Draw_Active_Stack();


    _map_plane = ARCANUS_PLANE;
    // === All_City_Calculations()
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        Do_City_Calculations(itr_cities);
    }


    Players_Update_Magic_Power();
    SBK_SomePageSaveVar = 0;
    CMB_SpellBookPage = 0;
    SBK_Candidate_Page = 0;
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        Player_Research_Spells(itr_players);
    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: PreInit_Overland()\n", __FILE__, __LINE__);
#endif
}
