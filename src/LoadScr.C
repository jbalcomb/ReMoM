/*
    WIZARDS.EXE
        ovr101
        ovr160
    MAGIC.EXE
        ovr050

WZD
    Load_Screen
    Load_Screen_Draw
    Load_Screen_Help
    Loaded_Game_Update
MGC
    Load_Screen
    Load_Screen_Draw

MoO2  Module: LOADSAVE

*/

#include "MoX.H"
#include "LoadScr.H"
#include "MainScr.H"
#include "MainScr_Maps.H"





// WZD dseg:60D1 1B 00                                           cnst_HOTKEY_Esc1B db 1Bh,0              ; DATA XREF: OVL_RazeCityDialog+155o
// WZD dseg:60D1                                                                                         ; should use dseg:2c56
// WZD dseg:60D3 00                                              align 2
// WZD dseg:60D4 01 00 00 00 00 00 00 00 3F 01 C7 00             mouse_list_loadsave s_MOUSE_LIST <crsr_Finger, 0, 0, 0, 319, 199>
struct s_mouse_list mouse_list_loadsave[1] = {
    {1, 0, 0, 0, 319, 199}
};

// WZD dseg:60D4                                                                                         ; DATA XREF: Settings_Screen+112o ...
// WZD dseg:60D4                                                                                         ; should use Normal_Fullscreen
// WZD dseg:60E0 FF FF                                           loadsave_settings_flag dw -1            ; DATA XREF: Settings_Screen+39Fr ...
// WZD dseg:60E0                                                                                         ; 3 indicates returning from the settings screen


// WZD dseg:60E0
int16_t loadsave_settings_flag = ST_UNDEFINED;





/*
                                            ¿ BEGIN: ovr160 - Initialized Data  LOADSAVE ?
*/
// WZD dseg:712C
char load_lbx_file[] = "LOAD.LBX";

// WZD dseg:7134
// char cnst_ZeroString_22[] = "";
int16_t cnst_ZeroString_22 = 0;

// WZD dseg:7135
char cnst_SAVE3[] = "SAVE";

// WZD dseg:713A
char cnst_SAVE_ext3[] = ".GAM";

// WZD dseg:713F
char cnst_HOTKEY_Esc22 = '\x1B';

// WZD dseg:7141 4D 41 47 49 43 2E 45 58 45 00                   cnst_MAGIC_EXE_File3 db 'MAGIC.EXE',0   ; DATA XREF: Load_Screen+3D1o
// WZD dseg:7141                                                                                         ; should use dseg:51f4
// WZD dseg:714B 4A 45 4E 4E 59 00                               cnst_MAGICEXE_arg0_3 db 'JENNY',0       ; DATA XREF: Load_Screen+3CDo
// WZD dseg:714B                                                                                         ; should use dseg:51fe

// WZD dseg:7151
char load_screen_copyright[] = "Copyright  Simtex Software, 1995   V1.31";

// WZD dseg:717A
char cnst_HLPENTRY_File8[] = "hlpentry.lbx";

// WZD dseg:7187 00                                              db    0

/*
                                            ¿ END: ovr160 - Initialized Data  LOADSAVE ?
*/





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
// WZD dseg:C998 00 00                                           GUI_Settings_Offset dw 0                ; DATA XREF: Settings_Screen:loc_A90CCw ...

// WZD dseg:C99A
SAMB_ptr save_inactive;

// WZD dseg:C99C
// AKA IDK_SaveSlots_Array
int16_t save_game_slots[8];

// WZD dseg:C9AC
int16_t loadsave_save_button;

// WZD dseg:C9AE
int16_t loadsave_quit_button;

// WZD dseg:C9B0
int16_t loadsave_ok_button;

// WZD dseg:C9B2
int16_t selected_load_game_slot_idx;

// WZD dseg:C9B4
int16_t selected_save_game_slot_idx;

// WZD dseg:C9B6
int16_t loadsave_load_button;

// WZD dseg:C9B8
int16_t load_screen_fade_in_flag;  // ; set to 0 on entering the settings screen

// WZD dseg:C9BA
SAMB_ptr load_inactive;

// WZD dseg:C9BC
SAMB_ptr selection_marker;

// WZD dseg:C9BE
int16_t save_game_count;

// WZD dseg:C9C0
SAMB_ptr settings_button;

// WZD dseg:C9C2
SAMB_ptr text_fill;

// WZD dseg:C9C4
SAMB_ptr ok_active;

// WZD dseg:C9C6
SAMB_ptr load_active;

// WZD dseg:C9C8
SAMB_ptr quit_active;

// WZD dseg:C9CA
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
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: GAME_NextHumanStack()\n", __FILE__, __LINE__);
#endif


    _unit_window_start_x = 247;
    _unit_window_start_y = 79;


    OVL_Action_XPos = ST_UNDEFINED;
    OVL_Action_YPos = ST_UNDEFINED;


    // ; recalculates the visibility arrays for both planes
    // ; after clearing them entirely, and marks contacted
    // ; players accordingly if they haven't been already
    Update_Scouted_And_Contacted();


    // ; does nothing and returns zero; at some point must have been some wizard data refresh function
    // DONT  o62p01_Empty_pFxn(_human_player_idx);


    WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);


    // ; recalculates the visibility arrays for both planes
    // ; after clearing them entirely, and marks contacted
    // ; players accordingly if they haven't been already
    Update_Scouted_And_Contacted();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: GAME_NextHumanStack()\n", __FILE__, __LINE__);
#endif

}



/*
    WIZARDS.EXE ovr160
*/

// WZD o160p01
void Load_Screen(void)
{
    char found_file[20];
    char buffer2[16];
    char match_string[16];
    int16_t save_slot_fields[8];
    int16_t loaded_game_flag;
    int16_t first_draw_done_flag;
// Left= word ptr -0Ah
    int16_t x_start;
    int16_t y_start;
    int16_t input_field_idx;
    int16_t hotkey_ESC;
// var_4= word ptr -4
    int16_t leave_screen_flag;
    int16_t itr;
    int16_t itr_save_slot_input_field_array;

    int16_t itr_save_slot_fields;
    int16_t itr_save_game_count;



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_Screen()\n", __FILE__, __LINE__);
#endif

    loaded_game_flag = ST_FALSE;

    load_screen_fade_in_flag = ST_TRUE;  // set to 0 on entering the settings screen

    // TODO: maybe, someday, come to terms with the way this logic looks in the Dasm. Pretty sure it's testing for a specific/known valid value and resetting on anything else
    if(loadsave_settings_flag != 3)  /* 3 indicates returning from the settings screen */
    {
        DLOG("(loadsave_settings_flag != 3)");
        Fade_Out();
        Fill(0, 0, 319, 199, 0);
        Set_Page_On();
        Fill(0, 0, 319, 199, 0);
        Set_Page_Off();
        Load_Palette(1, -1, 0);  // LOADSAVE - load save palette
    }
    else
    {
        // if(loadsave_settings_flag != 3)
        // {
        //     loadsave_settings_flag = ST_UNDEFINED;
        // }
        loadsave_settings_flag = ST_UNDEFINED;
    }

    loadsave_background = LBX_Reload(load_lbx_file,  0, _screen_seg);
    quit_active         = LBX_Reload_Next(load_lbx_file,  2, _screen_seg);
    load_active         = LBX_Reload_Next(load_lbx_file,  1, _screen_seg);
    save_active         = LBX_Reload_Next(load_lbx_file,  3, _screen_seg);
    ok_active           = LBX_Reload_Next(load_lbx_file,  4, _screen_seg);

    // DNE  Quit inactive
    load_inactive       = LBX_Reload_Next(load_lbx_file,  6, _screen_seg);
    save_inactive       = LBX_Reload_Next(load_lbx_file,  7, _screen_seg);
    // DNE  Ok   inactive

    text_fill           = LBX_Reload_Next(load_lbx_file,  9, _screen_seg);
    selection_marker    = LBX_Reload_Next(load_lbx_file, 10, _screen_seg);
    settings_button     = LBX_Reload_Next(load_lbx_file, 12, _screen_seg);


    save_game_count = 0;
    
    for(itr = 1; itr < 9; itr++)
    {
        strcpy(match_string, cnst_SAVE3);
// ¿ #pragma warning(suppress : 4996) ?
        itoa(itr, buffer2, 10);
        strcat(match_string, buffer2);
        strcat(match_string, cnst_SAVE_ext3);
        if(DIR(match_string, found_file) == 0)  /* File Not Found */
        {
            save_game_slots[save_game_count] = ST_UNDEFINED;
        }
        else
        {
            save_game_slots[save_game_count] = itr;
            save_game_count++;
        }

    }


        leave_screen_flag = ST_FALSE;
        selected_load_game_slot_idx = ST_UNDEFINED;  // controls drawing of load_active : load_inactive
        selected_save_game_slot_idx = ST_UNDEFINED;  // controls drawing of save_active : save_inactive
        first_draw_done_flag = ST_FALSE;

        x_start = 43;
        y_start = 171;

        Set_Mouse_List(1, mouse_list_loadsave);

        Clear_Fields();

        hotkey_ESC = Add_Hot_Key(cnst_HOTKEY_Esc22);

        loadsave_quit_button      = Add_Hidden_Field((x_start +   0), y_start, (x_start +  39), (y_start + 13), cnst_ZeroString_22, ST_UNDEFINED);
        loadsave_load_button      = Add_Hidden_Field((x_start +  40), y_start, (x_start +  78), (y_start + 13), cnst_ZeroString_22, ST_UNDEFINED);
        loadsave_save_button      = Add_Hidden_Field((x_start + 122), y_start, (x_start + 159), (y_start + 13), cnst_ZeroString_22, ST_UNDEFINED);
        loadsave_ok_button        = Add_Hidden_Field((x_start + 231), y_start, (x_start + 270), (y_start + 13), cnst_ZeroString_22, ST_UNDEFINED);
        loadsave_settings_button  = Add_Hidden_Field((x_start + 172), y_start, (x_start + 229), (y_start + 13), cnst_ZeroString_22, ST_UNDEFINED);

        Set_Font(3, 1, 3, ST_NULL);

//         // ~== MGC
//         // if previous_screen == scr_Menu_Screen
//         for(itr_save_slot_fields = 0; itr_save_slot_fields < 8; itr_save_slot_fields++)
//         {
//             // save_slot_fields[itr_save_slot_fields] = -1000
//         }
//         for(itr_save_slot_fields = 0; save_game_count < 8; itr_save_slot_fields++)
//         {
//             // save_slot_fields[save_game_slots__[itr_save_slot_fields]] = Add_Hidden_Field(x1_QuitLoadSave, ((save_game_slots__[itr_save_slot_fields] * 15) + 47), (x1_QuitLoadSave + 220), ((save_game_slots__[itr_save_slot_fields] * 15) + 56), cnst_ZeroString_7, -1)
//         }
//         
//         // ~== WZD
//         for(itr_save_slot_fields = 0; itr_save_slot_fields < 8; itr_save_slot_fields++)
//         {
//             // drake178: GUI_CreateEditSelect()
//             // TODO  save_slot_fields[itr_save_slot_fields] = Add_Continuous_String_Input_Field(start_x, (start_y + (itr_save_slot_fields * 15) + 47), 260, magic_set.Save_Names[itr_save_slot_fields * 20], 19, 0, selection_marker, ST_UNDEFINED);
//         }
        for(itr_save_slot_fields = 0; itr_save_slot_fields < 8; itr_save_slot_fields++)
        {
            // TODO  save_slot_fields[itr_save_slot_fields] = Add_Continuous_String_Input_Field(x_start, (y_start + (itr_save_slot_fields * 15) + 47), 260, magic_set.Save_Names[itr_save_slot_fields], 19, 0, selection_marker, ST_UNDEFINED);
            save_slot_fields[itr_save_slot_fields] = INVALID_FIELD;
        }

    // MGC
    // _settings_button = Add_Hidden_Field(172, y1_QuitLoadSave, 229, (y1_QuitLoadSave + 13), cnst_ZeroString_7, -1);



    selected_load_game_slot_idx = ST_UNDEFINED;
    leave_screen_flag = ST_FALSE;
    // TODO  Assign_Auto_Function(Load_Screen_Draw, 1);
    Load_Screen_Help();

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
        for(itr_save_slot_fields = 0; itr_save_slot_fields < 8; itr_save_slot_fields++)
        {
            if(save_slot_fields[itr_save_slot_fields] == input_field_idx)
            {
                selected_save_game_slot_idx = itr_save_slot_fields;
                selected_load_game_slot_idx = ST_UNDEFINED;
                for(itr_save_game_count = 0; itr_save_game_count < save_game_count; itr_save_game_count++)
                {
                    if(save_game_slots[itr_save_game_count] == itr_save_slot_fields)
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
            // DONT  GAME_EXE_Swap(cnst_MAGIC_EXE_File3, cnst_MAGICEXE_arg0_3, cnst_ZeroString_22, cnst_ZeroString_22);

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
// TODO         if(input_field_idx == loadsave_settings_button)
// TODO         {
// TODO             loadsave_settings_flag = 2;
// TODO             Settings_Screen();
// TODO             leave_screen_flag = ST_TRUE;
// TODO         }


        if(leave_screen_flag == ST_FALSE)
        {
            DLOG("(leave_screen_flag == ST_FALSE)");
            Load_Screen_Draw();
            Toggle_Pages();
            if( (load_screen_fade_in_flag != ST_FALSE) && (first_draw_done_flag == ST_FALSE) )
            {
                DLOG("( (load_screen_fade_in_flag != ST_FALSE) && (first_draw_done_flag == ST_FALSE) )");
                Copy_On_To_Off_Page();
                if(loadsave_settings_flag != 3)
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
    // TODO  Deactivate_Auto_Function();
    Deactivate_Help_List();

    if(loadsave_settings_flag == ST_UNDEFINED)  /* ; 3 indicates returning from the settings screen */
    {
        // current_screen = scr_Main_Screen;
        // HACK: merge MGC Load_Screen() and WZD Load_Screen()
        current_screen = previous_screen;
    }

    if(current_screen == scr_Main_Screen)
    {
        PageFlipEffect = 2;
        Fade_Out();
        Fill(0, 0, 319, 199, 0);
        Set_Page_On();
        Fill(0, 0, 319, 199, 0);
        Set_Page_Off();
        // HERE: set the palette back to the /normal/ palette
        Load_Palette(0, -1, ST_NULL);  // EMPERATO - main game palette
        Set_Palette_Changes(0, 255);
        Calculate_Remap_Colors();
        // TODO  Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1);
    }

    if(loaded_game_flag == ST_TRUE)
    {
        Loaded_Game_Update();
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Load_Screen()\n", __FILE__, __LINE__);
#endif
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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_Screen_Draw()\n", __FILE__, __LINE__);
#endif

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

    /*
        MGC
            current_menu_screen = Get_Mouse_Field();
    */
    /*
        WZD
            moused_field = Get_Mouse_Field();
    */
    moused_field = Get_Mouse_Field();


    x_start = 43;

    color_array[2] = 171;


    Set_Page_Off();
    Fill(0, 0, 319, 199, 0);


    /*
        MGC
            FLIC_Draw(0, 0, _background_seg);
    */
    /*
        WZD
            FLIC_Draw(0, 0, loadsave_background);
    */
    FLIC_Draw(0, 0, loadsave_background);


    /*
        MGC
            Fill( 42, 170, 123, 184, 0);
            Fill(171, 170, 271, 184, 0);
    */

    /*
        WZD
            Fill( 42, 170, 160, 184, 0);
            Fill(171, 170, 271, 184, 0);
    */
    Fill( 42, 170, 160, 184, 0);
    Fill(171, 170, 271, 184, 0);


    for(itr_save_gam = 0; itr_save_gam < save_game_count; itr_save_gam++)
    {
        FLIC_Draw(x_start, (47 + (15 * save_game_slots[itr_save_gam])), text_fill);
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
    Print_Centered(160, 193, load_screen_copyright);
    

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Load_Screen_Draw()\n", __FILE__, __LINE__);
#endif
}

// WZD o160p03
void Load_Screen_Help(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_Screen_Help()\n", __FILE__, __LINE__);
#endif

    // TODO  add manifest-constant for help entry record size
    // TODO  add manifest-constant for help list count
    LBX_Load_Data_Static(cnst_HLPENTRY_File8, 27, (SAMB_ptr)_help_entries, 0, 6, 10);
    Set_Help_List(_help_entries, 27);  // TODO  ¿ bug - someone put entry_num instead of help_count ?

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Load_Screen_Help()\n", __FILE__, __LINE__);
#endif
}

// WZD o160p04
void Loaded_Game_Update(void)
{
    int16_t itr;
    int16_t itr_players;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Loaded_Game_Update()\n", __FILE__, __LINE__);
#endif

// DIFF DNE   WZD  s01p06  Loaded_Game_Update_WZD()
    // mov     [GAME_RazeCity], 0

    _human_player_idx = 0;

    PageFlipEffect = 0;
    
    CTY_CatchmentRefresh__NOOP();

    GAME_Overland_Init();

    Patch_Units_Upkeep_And_Sound();

    // j_LD_CTY_ResRefresh();

    // j_LD_MAP_TFUnk40_Eval();           ; not sure what this resource is or would have been, this function enumerates the first five tiles that have it, and records their coordinates

    // j_CTY_CheckMinFarmers();

    _unit_stack_count = 0;

    // j_SND_PlayBkgrndTrack();

    GFX_Swap_Cities();

    // j_CTY_ResetRoadConns();

    // j_GAME_DeleteDeadUnits();

    // j_AI_ResetUnitMoves();

// DIFF DNE   WZD  s01p06  Loaded_Game_Update_WZD()
    // for(itr = 1; itr < NUM_PLAYERS; itr++)
    // {
    //     TBL_Wizards[itr].Avg_Unit_Value = 0;
    // }


    GAME_NextHumanStack();

    // if(_difficulty = 0 /* "Intro" */) { magic_set.Random_Events = ST_FALSE; }

    // for(itr = 0; itr < 100; itr++)
    // {
    //     TBL_OvlMovePathsEMS[itr] = ST_UNDEFINED;
    // }
    // CRP_UNIT_OverlandPath = ST_UNDEFINED;
    // j_CONTX_CreateChains();
    // j_CONTX_CreateLChains();
    // for(itr = 0; itr < _num_players; itr++)
    // {
    //     AI_CONTX_Reevals[itr] = ST_FALSE;
    // }

    g_TimeStop_PlayerNum = ST_NONE;
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        // if(_players[itr].Globals.Time_Stop > 0)
        if(_players[itr_players].Globals[TIME_STOP] > 0)
        {
            g_TimeStop_PlayerNum = itr_players + 1;
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Loaded_Game_Update()\n", __FILE__, __LINE__);
#endif
}


// WZD o51p01
// _main() |-> Loaded_Game_Update_WZD() |-> GAME_Overland_Init()
// Load_Screen |-> [WZD ovr160] Loaded_Game_Update() |-> GAME_Overland_Init()
/*
    Does this not feel like ~ Init Main Screen?
    There are a few oddities...
        city buildings
        staff lock flags
        nominal skill calc
        ! City Recalculate All - Main Screen needs that for the Resources Window
        GFX_Swap_Cities(), which has a terribly misleading name, loads other screens stuff
        ? G_WLD_StaticAssetRfrsh() Meh. More oddities...

*/
void GAME_Overland_Init(void)
{
    int16_t itr_cities;
    int16_t itr_units;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: GAME_Overland_Init()\n", __FILE__, __LINE__);
#endif


    G_WLD_StaticAssetRfrsh();


    itr_cities = 0;
    while(itr_cities++ < _cities)
    {
        _CITIES[itr_cities].bldg_status[NONE] = bs_Built; // ? ~ enum City Building Status B_Replaced;
    }


    itr_units = 0;
    while(itr_units++ < _units)
    {
        if(_UNITS[itr_units].wp == 2) {_UNITS[itr_units].wp = 0; }

        // NOTE: the DASM thinks world_plane is passed here as well, but IsPassableTower() makes no xref to it
        if(IsPassableTower(_UNITS[itr_units].wx, _UNITS[itr_units].wy) == ST_TRUE)
        {
            _UNITS[itr_units].In_Tower = ST_TRUE;
        }
        else
        {
            _UNITS[itr_units].In_Tower = ST_FALSE;
        }

    }


    skill_staff_locked = 0;
    mana_staff_locked = 0;
    research_staff_locked = 0;

    // TODO  _players[0].Nominal_Skill = Player_Base_Casting_Skill(0);
    

    // NIU?  CRP_OVL_MapWindowX = 0;
    // NIU?  CRP_OVL_MapWindowY = 0;
    _prev_world_x = 0;
    _prev_world_y = 0;
    // _curr_world_x = 0;
    // _curr_world_y = 0;
    _map_x = 0;
    _map_y = 0;

    // j_o62p01_Empty_pFxn(_human_player_idx)

    _unit = 0;  // 0: None / No Unit

    // // TODO(JimBalcomb,20230629): validate the SAVE_GAM data for _FORTRESSES
    // // // _active_world_x = _FORTRESSES[HUMAN_PLAYER_IDX].wx;
    // // // _active_world_y = _FORTRESSES[HUMAN_PLAYER_IDX].wy;
    // // OVL_Map_CenterX = _FORTRESSES[HUMAN_PLAYER_IDX].wx;
    // // OVL_Map_CenterY = _FORTRESSES[HUMAN_PLAYER_IDX].wy;
    // OVL_Map_CenterX = 24;
    // OVL_Map_CenterY = 16;
    _active_world_x = _FORTRESSES[HUMAN_PLAYER_IDX].wx;
    _active_world_y = _FORTRESSES[HUMAN_PLAYER_IDX].wy;

    _unit_window_start_x = 247;
    _unit_window_start_y = 79;

    // TODO(JimBalcomb,20230629): validate the SAVE_GAM data for _FORTRESSES
    // _world_plane = _FORTRESSES[HUMAN_PLAYER_IDX].wp;
    // _map_plane = _FORTRESSES[HUMAN_PLAYER_IDX].wp;  // TODO(JimBalcomb,20230614): Why is this getting set to 100?
    // _map_plane = 0;
    _map_plane = _FORTRESSES[HUMAN_PLAYER_IDX].wp;


    TILE_VisibilityUpdt();


    Allocate_Reduced_Map();

    // Center_Map(&_curr_world_x, &_curr_world_y, _FORTRESSES[HUMAN_PLAYER_IDX].wx, _FORTRESSES[HUMAN_PLAYER_IDX].wy, _world_plane);
    // TODO(JimBalcomb,20230629): validate the SAVE_GAM data for _FORTRESSES
    // Center_Map(&_map_x, &_map_y, _FORTRESSES[HUMAN_PLAYER_IDX].wx, _FORTRESSES[HUMAN_PLAYER_IDX].wy, _map_plane);
    // Center_Map(&_map_x, &_map_y, 24, 16, 0);
    Center_Map(&_map_x, &_map_y, _FORTRESSES[HUMAN_PLAYER_IDX].wx, _FORTRESSES[HUMAN_PLAYER_IDX].wy, _map_plane);

    Set_Unit_Draw_Priority();
    Reset_Stack_Draw_Priority();
    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);


    GFX_Swap_Cities();


    WIZ_NextIdleStack(_human_player_idx, &_map_x, &_map_y, &_map_plane);


    // j_o108p02_Empty_pFxn()

    // TODO  All_City_Calculations()


    // TODO  if(Check_Release_Version() != ST_FALSE)
    // TODO  {
    // TODO      // WZD s22p05
    // TODO      RNG_SetSeed(0x2737, 0);  // LFSR_LO, LFSR_HI
    // TODO      // ; sets the linear feedback shift register
    // TODO  }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: GAME_Overland_Init()\n", __FILE__, __LINE__);
#endif
}

// WZD o51p02
// Loaded_Game_Update_WZD()  |-> GAME_Overland_Init()
// Loaded_Game_Update()    |-> GAME_Overland_Init()
// GAME_Overland_Init()  |-> G_WLD_StaticAssetRfrsh()
// NOTE: no XREFs to j_G_WLD_StaticAssetRfrsh()
void G_WLD_StaticAssetRfrsh(void)
{
// LFSR_LO= word ptr -4
// LFSR_HI= word ptr -2

    int16_t itr_cities;
    int16_t itr_players;

    Randomize();

//    LFSR_HI = 0;
//    LFSR_LO = 0x03E8;
//    if(!Check_Release_Version())
//    {
//        RNG_SetSeed(LFSR_LO, LFSR_HI)
//    }

    all_units_moved = ST_FALSE;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: all_units_moved: %d\n", __FILE__, __LINE__, all_units_moved);
#endif

    reset_active_stack = ST_TRUE;
    Reset_Draw_Active_Stack();
    _map_plane = 0;  // Arcanus

    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        // TODO  CTY_Recalculate(itr_cities);
    }

//    _WIZ_SetPowerBases();

//    SBK_SomePageSaveVar = 0;
//    CMB_SpellBookPage = 0;
//    SBK_Candidate_Page = 0;

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        // TODO  WIZ_RefreshResearch(itr_players);
    }

}
