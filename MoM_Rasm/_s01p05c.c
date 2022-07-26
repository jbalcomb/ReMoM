// _s01p05c.c SCREEN_Menu()
// MGC_DEF.H

#include <DOS.H>

#include "MOM_DEF.H"
#include "MGC_DEF.H"

#include "ST_GUI.H"
#include "ST_HLP.H"

#include "STU_DBG.H"


int SCREEN_Menu(void)
{
    char Found_File_Name[30];
    char Conversion_String[30];
    char File_Name[30];
    int Load_Hotkey_Index;
    int HallofFame_Hotkey_Index;
    int Escape_Hotkey_Index;
    int First_Draw_Done;
    int Quit_Hotkey_Index;
    int NewGame_Hotkey_Index;
    int Continue_Hotkey_Index;
    int Continue_Move_Down;
    int itr_SaveGam;

    int flag_done;
    // unsigned int c_sgmt;
    // unsigned int c_ofst;
    // //void _FAR *fxnptr_ScreenDraw;
    // void (_FAR *fxnptr_ScreenDraw) (void);
    int input_control_index;

#ifdef DEBUG
    unsigned int dbg_Loop_Once;
#endif

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: SCREEN_Menu()\n", __FILE__, __LINE__);
#endif

    // 1oom :: DNE - not sure if DNE in ORION
    if ( g_GUI_MainScreenJump != -1 )
    {
        // tmp_AX = g_GUI_MainScreenJump;
        // itr_SaveGam = tmp_AX;
        // g_GUI_MainScreenJump = -1;
        // tmp_AX = itr_SaveGam;
        // goto Done;
        // 202201170857: no idea what code would cause this noise in the dasm, but I'm done with it (FWIW, it is flagged /bad-pink/ in IDA)
        return -1;
    }

    // Research(JimBalcomb): mayhap, this was a macro to initialize the screen values? on account of g_GUI_MainMenuSelected is set again later?
    GUI_LoadSave_State = ST_UNDEFINED;
    First_Draw_Done = ST_FALSE;
    g_GAME_HaveContSave = ST_FALSE;
    g_GUI_MainMenuSelected = ST_UNDEFINED;  // {0:continue,1:load,2:new,3:quit,4:HoF} ; holds the mouse focus control instead during the redraw functions of other screens
    g_GAME_HaveSaves = ST_FALSE;
    // CRP_Unused_Anim_Var = 0; // NOTE(JimBalcomb): 2022017: I have not attempted to research this, neither in MGC nor WZD
    flag_done = ST_FALSE;  // 1oom :: bool flag_done = false;

    // NOTE(JimBalcomb): this setting and testing here really is like this in the dasm
    g_ScreenChangeFade = ST_TRUE;  // 1oom :: bool flag_fadein = false;
    // 1oom :: if (ui_draw_finish_mode != 0) { ui_palette_fadeout_19_19_1(); }
    if ( g_ScreenChangeFade != ST_FALSE )
    {
        //j_VGA_Fade_Out
    }

    VGA_DrawFilledRect(0, 0, 319, 199, 0);
    VGA_SetDirectDraw();
    VGA_DrawFilledRect(0, 0, 319, 199, 0);
    VGA_SetDrawFrame();

    // _s20p01a.c  ST_VGA.H
    // //VGA_LoadPalette(2, -1);  // argument missing in dasm
    VGA_LoadPalette(2, -1, 0); // ARCANUS - Magic Castle View

    // j_GAME_LoadSettings

    // HLP_Entry_Table.Entry_Index = -1;
    HLP_IDX_0 = -1;
    HLP_IDX_1 = -1;
    HLP_IDX_2 = -1;
    HLP_IDX_3 = -1;
    HLP_IDX_4 = -1;

    // TODO(JimBalcomb,20220723): diff this SAVE.GAM code with that in main() and elsewhere ... if ~==, MACRO
    g_GAME_ValidSaveCount = 0;

    for ( itr_SaveGam = 1; itr_SaveGam < 10; itr_SaveGam++ )
    {
        strcpy(File_Name, cnst_SAVE);
        itoa(itr_SaveGam, Conversion_String, 10);
        strcat(File_Name, Conversion_String);
        strcat(File_Name, cnst_SAVE_ext);

        if ( DISK_FileFind(File_Name, Found_File_Name) == 0 )
        {
            g_GAME_ValidSaves_Main[g_GAME_ValidSaveCount] = ST_UNDEFINED;
        }
        else
        {
            if (itr_SaveGam < 9)
            {
                g_GAME_ValidSaves_Main[g_GAME_ValidSaveCount] = itr_SaveGam - 1;
                g_GAME_ValidSaveCount++;
            }
            else
            {
                g_GAME_HaveContSave = ST_TRUE;
            }
        }
    }

    if ( g_GAME_ValidSaveCount > 0 )
    {
        g_GAME_HaveSaves = ST_TRUE;
    }

    GUI_Clear();  // ? 1oom :: uiobj_table_clear(); ?

    Continue_Move_Down = ST_FALSE;

    if ( g_GAME_HaveSaves == 0 )
    {
        GUI_Load_Lbl_Index = -1000;  // 1oom :: oi_loadgame = UIOBJI_INVALID;
        Continue_Move_Down = 1;
    }
    else
    {
        GUI_Load_Lbl_Index = GUI_CreateClickLabel(108, 150, 211, 161, 0, -1);
        HLP_IDX_0 = 629; // HLP_LOAD
        HLP_IDX_0_X1 = 108;
        HLP_IDX_0_Y1 = 148;
        HLP_IDX_0_X2 = 211;
        HLP_IDX_0_Y2 = 162;
    }

    // 1oom :: if (d->have_continue) { oi_continue = uiobj_add_mousearea(0x3c, 0x7f, 0x104, 0x8e, MOO_KEY_UNKNOWN); } else { oi_continue = UIOBJI_INVALID; }
    if ( g_GAME_HaveContSave == 0 )
    {
        GUI_Continue_Label = -1000;
    }
    else
    {
        GUI_Continue_Label = GUI_CreateClickLabel(108, (138 + (12 * Continue_Move_Down)), 211, (149 + (12 * Continue_Move_Down)), 0, -1);
        HLP_IDX_1 = 628; // HLP_CONTINUE
        HLP_IDX_1_X1 = 108;
        HLP_IDX_1_Y1 = (138 + (12 * Continue_Move_Down));
        HLP_IDX_1_X2 = 211;
        HLP_IDX_1_Y2 = (149 + (12 * Continue_Move_Down));
    }

    GUI_NewGame_Label = GUI_CreateClickLabel(108, 162, 211, 173, 0, -1);
    HLP_IDX_2 = 630; // HLP_NEWGAME
    HLP_IDX_2_X1 = 108;
    HLP_IDX_2_Y1 = 162;
    HLP_IDX_2_X2 = 211;
    HLP_IDX_2_Y2 = 173;

    GUI_HoF_Lbl_Index = GUI_CreateClickLabel(108, 174, 211, 185, 0, -1);
    HLP_IDX_3 = 798; // HLP_HALLOFFAME
    HLP_IDX_3_X1 = 108;
    HLP_IDX_3_Y1 = 174;
    HLP_IDX_3_X2 = 211;
    HLP_IDX_3_Y2 = 185;

    GUI_Quit_Lbl_Index = GUI_CreateClickLabel(108, 186, 211, 199);
    HLP_IDX_4 = 631; // HLP_QUITTODOS
    HLP_IDX_4_X1 = 108;
    HLP_IDX_4_Y1 = 186;
    HLP_IDX_4_X2 = 211;
    HLP_IDX_4_Y2 = 199;

    if ( g_GAME_HaveContSave == 0 )
    {
        Continue_Hotkey_Index = -1000;
    }
    else
    {
        Continue_Hotkey_Index = GUI_CreateHotkey(cnst_HOTKEY_C, -1);
    }

    if ( g_GAME_HaveSaves == 0 )
    {
        Load_Hotkey_Index = -1000;
    }
    else
    {
        Load_Hotkey_Index = GUI_CreateHotkey(cnst_HOTKEY_L, -1);
    }

    NewGame_Hotkey_Index = GUI_CreateHotkey(cnst_HOTKEY_N, -1);
    HallofFame_Hotkey_Index = GUI_CreateHotkey(cnst_HOTKEY_H, -1);
    Quit_Hotkey_Index = GUI_CreateHotkey(cnst_HOTKEY_Q, -1);
    Escape_Hotkey_Index = GUI_CreateHotkey(cnst_HOTKEY_Esc, -1);

/*
MGC_DEF.C
char cnst_HOTKEY_N = 'N';                   // dseg:28AE
char g_LbxNm_VORTEX[] = "VORTEX";           // dseg:28B0
char g_LbxNm_WIZARDS[] = "WIZARDS";         // dseg:28B7
char g_LbxNm_SPELLDAT[] = "SPELLDAT";       // dseg:28BF
char cnst_HOTKEY_C = 'C';                   // dseg:28C8
char cnst_HOTKEY_L = 'L';                   // dseg:28CA
char cnst_HOTKEY_H = 'H';                   // dseg:28CC
char cnst_HOTKEY_Q = 'Q';                   // dseg:28CE
char cnst_HOTKEY_Esc = '\x1B';              // dseg:28D0
*/
/*
DEBUG: [_s01p05c.c, 219] GUI_Load_Lbl_Index: 1
DEBUG: [_s01p05c.c, 220] GUI_Continue_Label: 2
DEBUG: [_s01p05c.c, 221] GUI_NewGame_Label: 3
DEBUG: [_s01p05c.c, 222] GUI_HoF_Lbl_Index: 4
DEBUG: [_s01p05c.c, 223] GUI_Quit_Lbl_Index: 5
DEBUG: [_s01p05c.c, 224] Continue_Hotkey_Index: 6
DEBUG: [_s01p05c.c, 225] Load_Hotkey_Index: 7
DEBUG: [_s01p05c.c, 226] NewGame_Hotkey_Index: 8
DEBUG: [_s01p05c.c, 227] HallofFame_Hotkey_Index: 9
DEBUG: [_s01p05c.c, 228] Quit_Hotkey_Index: 10
DEBUG: [_s01p05c.c, 229] Escape_Hotkey_Index: 11
*/
/*
DEBUG: [_s34p22c.c, 49] CtrlTbl[1].Hotkey=0, InputCode=27
DEBUG: [_s34p22c.c, 49] CtrlTbl[2].Hotkey=0, InputCode=27
DEBUG: [_s34p22c.c, 49] CtrlTbl[3].Hotkey=0, InputCode=27
DEBUG: [_s34p22c.c, 49] CtrlTbl[4].Hotkey=0, InputCode=27
DEBUG: [_s34p22c.c, 49] CtrlTbl[5].Hotkey=0, InputCode=27
DEBUG: [_s34p22c.c, 49] CtrlTbl[6].Hotkey=65, InputCode=27
DEBUG: [_s34p22c.c, 49] CtrlTbl[7].Hotkey=77, InputCode=27
DEBUG: [_s34p22c.c, 49] CtrlTbl[8].Hotkey=84, InputCode=27
DEBUG: [_s34p22c.c, 49] CtrlTbl[9].Hotkey=79, InputCode=27
DEBUG: [_s34p22c.c, 49] CtrlTbl[10].Hotkey=77, InputCode=27
DEBUG: [_s34p22c.c, 49] CtrlTbl[11].Hotkey=32, InputCode=27
*/
#ifdef DEBUG
/*    
    GUI_Load_Lbl_Index
    GUI_Continue_Label
    GUI_NewGame_Label
    GUI_HoF_Lbl_Index
    GUI_Quit_Lbl_Index

    Continue_Hotkey_Index
    Load_Hotkey_Index
    NewGame_Hotkey_Index
    HallofFame_Hotkey_Index
    Quit_Hotkey_Index
    Escape_Hotkey_Index
*/
    dlvfprintf("DEBUG: [%s, %d] GUI_Load_Lbl_Index: %u\n", __FILE__, __LINE__, GUI_Load_Lbl_Index);
    dlvfprintf("DEBUG: [%s, %d] GUI_Continue_Label: %u\n", __FILE__, __LINE__, GUI_Continue_Label);
    dlvfprintf("DEBUG: [%s, %d] GUI_NewGame_Label: %u\n", __FILE__, __LINE__, GUI_NewGame_Label);
    dlvfprintf("DEBUG: [%s, %d] GUI_HoF_Lbl_Index: %u\n", __FILE__, __LINE__, GUI_HoF_Lbl_Index);
    dlvfprintf("DEBUG: [%s, %d] GUI_Quit_Lbl_Index: %u\n", __FILE__, __LINE__, GUI_Quit_Lbl_Index);
    dlvfprintf("DEBUG: [%s, %d] Continue_Hotkey_Index: %u\n", __FILE__, __LINE__, Continue_Hotkey_Index);
    dlvfprintf("DEBUG: [%s, %d] Load_Hotkey_Index: %u\n", __FILE__, __LINE__, Load_Hotkey_Index);
    dlvfprintf("DEBUG: [%s, %d] NewGame_Hotkey_Index: %u\n", __FILE__, __LINE__, NewGame_Hotkey_Index);
    dlvfprintf("DEBUG: [%s, %d] HallofFame_Hotkey_Index: %u\n", __FILE__, __LINE__, HallofFame_Hotkey_Index);
    dlvfprintf("DEBUG: [%s, %d] Quit_Hotkey_Index: %u\n", __FILE__, __LINE__, Quit_Hotkey_Index);
    dlvfprintf("DEBUG: [%s, %d] Escape_Hotkey_Index: %u\n", __FILE__, __LINE__, Escape_Hotkey_Index);
#endif

    GUI_SetWindows(1, g_GUI_MainMenuWindow);

    // TODO(JimBalcomb,20220723): figure out if this SaveCursorArea is just because of the subsequent MouseEmuMoveTo
    GUI_SaveCursorArea_RSP(MOUSE_GetX(), MOUSE_GetY());

    // GUI_MouseEMUMoveTo(GUI_NewGame_Label);

    FLIC_ResetFrame(gsa_MAINSCRN_0_AnimatedLogo);

    // TODO(JimBalcomb): 2022017: double check it's accurate that this is being set a second time here, and add a note to avoid questioning this again in the future
    g_GUI_MainMenuSelected = ST_UNDEFINED;

    // // mov  ax, 2
    // // push ax
    // // mov  ax, seg seg001
    // // push ax
    // // mov  ax, offset SCREEN_MainMenu_Draw
    // // push ax
    // // call GUI_Set_Redraw_Function
    // c_sgmt = FP_SEG(SCREEN_Menu_Draw);
    // c_ofst = FP_OFF(SCREEN_Menu_Draw);
    // fxnptr_ScreenDraw = (void _FAR *) MK_FP(c_sgmt,c_ofst);
    // GUI_Set_Redraw_Function(fxnptr_ScreenDraw, 2);
    GUI_Set_Redraw_Function(SCREEN_Menu_Draw, 2);

    // GUI_SetHelp(HLP_IDX_0, 5);

    GUI_SetDelay(4);


    while ( flag_done == ST_FALSE )
    {
        CLK_SaveCounter();  // 1oom :: ui_delay_prepare()


        // ST_GUI.H  _s34p66c.c  int GUI_GetInput(void)
        input_control_index = GUI_GetInput();  // _s34p66c.c  1oom :: oi1 = uiobj_handle_input_cond();
#ifdef DEBUG
        dlvfprintf("DEBUG: [%s, %d] input_control_index: %d\n", __FILE__, __LINE__, input_control_index);
#endif

        // 1oom :: if (oi1 != 0) { flag_done = true; }
        if ( input_control_index != ST_FALSE )
        {
            flag_done = ST_TRUE;
        }

        if ( (input_control_index == Continue_Hotkey_Index) || (input_control_index == GUI_Continue_Label) )
        {
            flag_done = ST_TRUE;
            g_GUI_MainMenuSelected = 0;
        }
        if ( (input_control_index == Load_Hotkey_Index) || (input_control_index == GUI_Load_Lbl_Index) )
        {
            flag_done = ST_TRUE;
            g_GUI_MainMenuSelected = 1;
        }
        if ( (input_control_index == NewGame_Hotkey_Index) || (input_control_index == GUI_NewGame_Label) )
        {
            flag_done = ST_TRUE;
            g_GUI_MainMenuSelected = 2;
        }
        if ( (input_control_index == Quit_Hotkey_Index) || (input_control_index == GUI_Quit_Lbl_Index) || (input_control_index == Escape_Hotkey_Index) )
        {
            flag_done = ST_TRUE;
            g_GUI_MainMenuSelected = 3;
        }
        if ( (input_control_index == HallofFame_Hotkey_Index) || (input_control_index == GUI_HoF_Lbl_Index) )
        {
            flag_done = ST_TRUE;
            g_GUI_MainMenuSelected = 4;
        }

        if ( g_GUI_MainMenuSelected == ST_UNDEFINED )
        {
            flag_done = ST_FALSE;
        }

        if ( flag_done == ST_FALSE )
        {
            SCREEN_Menu_Draw();  // MGC_DEF.H  _s01p06c.c
            GUI_SimplePageFlip();  // 1oom :: uiobj_finish_frame();
            // Research(JimBalcomb): 2022017: Should not the screen now be drawn and rendered?
            // ...the Screen-Page Index and Address are indeed be updated properly

            // 1oom :: if ((ui_draw_finish_mode != 0) && !flag_fadein) { ui_palette_fadein_4b_19_1(); flag_fadein = true; }
            if ( !((g_ScreenChangeFade == ST_FALSE) & (First_Draw_Done != ST_FALSE)) )
            {
                HERE("( !((g_ScreenChangeFade == ST_FALSE) && (First_Draw_Done != ST_FALSE)) )");
                // j_VGA_Fade_In()
                VGA_Copy_RSP_DSP();  // Research(JimBalcomb)): 20220117: what does this accomplish?
                First_Draw_Done = ST_TRUE;
                g_ScreenChangeFade = ST_FALSE;
            }
            else
            {
                HERE("( !((g_ScreenChangeFade != ST_FALSE) || (First_Draw_Done == ST_FALSE)) )");
            }

            CLK_Wait(2);  // 1oom :: ui_delay_ticks_or_click(2);
        }

    }  /* while ( flag_done == ST_FALSE ) */

    GUI_DisableRedraw();  // 1oom :: uiobj_unset_callback();

    GUI_ClearHelp();

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: SCREEN_Menu()\n", __FILE__, __LINE__);
#endif
    return g_GUI_MainMenuSelected;
// #ifdef DEBUG
//     return 3;  // DEBUG: g_GUI_MainMenuSelected = ( (Quit_Hotkey_Index) || (GUI_Quit_Lbl_Index) || (Escape_Hotkey_Index) )
// #endif
}
