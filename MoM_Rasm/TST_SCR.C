
#include "ST_DEF.H"     /* ST_FALSE, STRUE; */

// #include "MOM_DEF.H"
/*
    cnst_ConfigFile, cnst_ConfigErr

Config_Data, 
g_ScreenChangeFade,
g_GUI_MainScreenJump,
GUI_LoadSave_State,
cnst_QUIT_Message
    
*/

#include "MAINMENU.H"   /* */
/*
mainscrn_lbx_file, vortex_lbx_file
mainmenu_top, mainmenu_bot, mainmenu_c, mainmenu_h, mainmenu_q, mainmenu_n, mainmenu_l
*/

#include <STDIO.H>      /* FILE; fclose(), fopen(), fread(), fwrite(); */


// void test_TST_Test_Screen(void)
// // ~== MGC s01p03
// void TST_Test_Screen(void)
// void TST_Add_Test_Screen_Fields(void)
// // ~== MGC s01p05
// int TST_Do_Test_Screen(void)
// // ~== MGC s01p06
// void TST_Draw_Test_Screen(void)

void test_TST_Test_Screen(void);

void TST_Test_Screen(void);
void TST_Add_Test_Screen_Fields(void);
int TST_Do_Test_Screen(void);
void TST_Draw_Test_Screen(void);



void test_TST_Test_Screen(void)
{
    Init_STGE();
    TST_Test_Screen();
}

// s01p03
void TST_Test_Screen(void)
{
    char Read_Buffer[30];
    int Screen_Action;
    FILE *fp;
    int flag_quit;
    if ( DISK_FileFind(cnst_ConfigFile, Read_Buffer) == 0 )
    {
        Exit(cnst_ConfigErr);
    }
    fp = fopen(cnst_ConfigFile, cnst_RB);
    fread(Config_Data, 18, 1, fp);
    fclose(fp);
    g_ScreenChangeFade = 1;  // ST_TRUE
    g_GUI_MainScreenJump = -1;  // ST_UNDEFINED
    IN_SetEscOverride();
    flag_quit = ST_FALSE;
    while ( flag_quit == ST_FALSE )
    {
        VGA_SetOutlineColor(0);
        HLP_ClearHelp();
        Screen_Action = TST_Do_Test_Screen();  // MGC_DEF.H  _s01p05c.c
        HLP_ClearHelp();
        g_ScreenChangeFade = ST_FALSE;
        switch (Screen_Action)
        {
            case 3:  // "Quit To DOS"
                flag_quit = ST_TRUE;
                GUI_LoadSave_State = ST_UNDEFINED;
                break;
        }
    }
    Exit(cnst_QUIT_Message);
}

void TST_Add_Test_Screen_Fields(void)
{

}

// s01p05
int TST_Do_Test_Screen(void)
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
    int input_control_index;
    if ( g_GUI_MainScreenJump != -1 )
    {
        return -1;
    }
    GUI_LoadSave_State = ST_UNDEFINED;
    First_Draw_Done = ST_FALSE;
    g_GAME_HaveContSave = ST_FALSE;
    g_GUI_MainMenuSelected = ST_UNDEFINED;
    g_GAME_HaveSaves = ST_FALSE;
    flag_done = ST_FALSE;
    g_ScreenChangeFade = ST_TRUE;
    if ( g_ScreenChangeFade != ST_FALSE )
    {
        //j_VGA_Fade_Out
    }
    VGA_DrawFilledRect(0, 0, 319, 199, 0);
    VGA_SetDirectDraw();
    VGA_DrawFilledRect(0, 0, 319, 199, 0);
    VGA_Set_DSP_Addr();
    PAL_Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View
    // j_GAME_LoadSettings
    // HLP_Entry_Table.Entry_Index = -1;
    HLP_IDX_0 = -1;
    HLP_IDX_1 = -1;
    HLP_IDX_2 = -1;
    HLP_IDX_3 = -1;
    HLP_IDX_4 = -1;
    g_GAME_ValidSaveCount = 0;
    for ( itr_SaveGam = 1; itr_SaveGam < 10; itr_SaveGam++ )
    {
        strcpy(File_Name, cnst_SAVE);
        itoa(itr_SaveGam, Conversion_String, 10);
        strcat(File_Name, Conversion_String);
        strcat(File_Name, cnst_SAVE_ext);
        if(DISK_FileFind(File_Name, Found_File_Name) == 0)
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
    CTRL_Clear();
    Continue_Move_Down = ST_FALSE;
    if ( g_GAME_HaveSaves == 0 )
    {
        GUI_Load_Lbl_Index = -1000;
        Continue_Move_Down = 1;
    }
    else
    {
        GUI_Load_Lbl_Index = CTRL_CreateClickLabel(108, 150, 211, 161, 0, -1);
        HLP_IDX_0 = 629; // HLP_LOAD
        HLP_IDX_0_X1 = 108;
        HLP_IDX_0_Y1 = 148;
        HLP_IDX_0_X2 = 211;
        HLP_IDX_0_Y2 = 162;
    }
    if ( g_GAME_HaveContSave == 0 )
    {
        GUI_Continue_Label = -1000;
    }
    else
    {
        GUI_Continue_Label = CTRL_CreateClickLabel(108, (138 + (12 * Continue_Move_Down)), 211, (149 + (12 * Continue_Move_Down)), 0, -1);
        HLP_IDX_1 = 628; // HLP_CONTINUE
        HLP_IDX_1_X1 = 108;
        HLP_IDX_1_Y1 = (138 + (12 * Continue_Move_Down));
        HLP_IDX_1_X2 = 211;
        HLP_IDX_1_Y2 = (149 + (12 * Continue_Move_Down));
    }
    GUI_NewGame_Label = CTRL_CreateClickLabel(108, 162, 211, 173, 0, -1);
    HLP_IDX_2 = 630; // HLP_NEWGAME
    HLP_IDX_2_X1 = 108;
    HLP_IDX_2_Y1 = 162;
    HLP_IDX_2_X2 = 211;
    HLP_IDX_2_Y2 = 173;
    GUI_HoF_Lbl_Index = CTRL_CreateClickLabel(108, 174, 211, 185, 0, -1);
    HLP_IDX_3 = 798; // HLP_HALLOFFAME
    HLP_IDX_3_X1 = 108;
    HLP_IDX_3_Y1 = 174;
    HLP_IDX_3_X2 = 211;
    HLP_IDX_3_Y2 = 185;
    GUI_Quit_Lbl_Index = CTRL_CreateClickLabel(108, 186, 211, 199, 0, -1);
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
        Continue_Hotkey_Index = CTRL_CreateHotkey(cnst_HOTKEY_C, -1);
    }
    if ( g_GAME_HaveSaves == 0 )
    {
        Load_Hotkey_Index = -1000;
    }
    else
    {
        Load_Hotkey_Index = CTRL_CreateHotkey(cnst_HOTKEY_L, -1);
    }
    NewGame_Hotkey_Index = CTRL_CreateHotkey(cnst_HOTKEY_N, -1);
    HallofFame_Hotkey_Index = CTRL_CreateHotkey(cnst_HOTKEY_H, -1);
    Quit_Hotkey_Index = CTRL_CreateHotkey(cnst_HOTKEY_Q, -1);
    Escape_Hotkey_Index = CTRL_CreateHotkey(cnst_HOTKEY_Esc, -1);
    GUI_SetWindows(1, g_GUI_MainMenuWindow);
    CRL_Save_RSP(MD_GetX(), MD_GetY());
    // GUI_MouseEMUMoveTo(GUI_NewGame_Label);
    FLIC_Reset_CurrentFrame(gsa_MAINSCRN_0_AnimatedLogo);
    g_GUI_MainMenuSelected = ST_UNDEFINED;
    SCRN_Set_Redraw_Function(TST_Draw_Test_Screen, 2);
    // GUI_SetHelp(HLP_IDX_0, 5);
    IN_Set_Skip(4);
    while ( flag_done == ST_FALSE )
    {
        CLK_SaveCounter();
        input_control_index = IN_GetInput();
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
            TST_Draw_Test_Screen();  // MGC_DEF.H  _s01p06c.c
            SCRN_SimplePageFlip();
            if ( !((g_ScreenChangeFade == ST_FALSE) & (First_Draw_Done != ST_FALSE)) )
            {
                VGA_Copy_RSP_DSP();
                First_Draw_Done = ST_TRUE;
                g_ScreenChangeFade = ST_FALSE;
            }
            CLK_Wait(2);
        }
    }  /* while ( flag_done == ST_FALSE ) */
    SCRN_DisableRedraw();
    HLP_ClearHelp();
    return g_GUI_MainMenuSelected;
}

// s01p06
void TST_Draw_Test_Screen(void)
{
    // int Title_Frame_Index;
    int Loop_Var;
    int Continue_Move_Down;
    int MouseOver_ControlIndex;
    int MenuArea_X_Left;
    int MenuArea_Y_Top;
    MenuArea_X_Left = 123;
    MenuArea_Y_Top = 141;
    MouseOver_ControlIndex = GUI_MousedControl();
    VGA_Set_DSP_Addr();
    VGA_DrawFilledRect(0, 0, 319, 199, 0);
    // Title_Frame_Index = FLIC_Get_CurrentFrame(gsa_MAINSCRN_0_AnimatedLogo);
    FLIC_Set_CurrentFrame(gsa_MAINSCRN_0_AnimatedLogo, 0);
    // for ( Loop_Var = 0; Loop_Var <= Title_Frame_Index; Loop_Var++ )
    // {
        FLIC_Draw(0, 0, fmainmenu_top);
    // }
    FLIC_Draw(0, 41, fmainmenu_bot);
    if ( g_ScreenChangeFade == 0 )
    {
        // j_GAME_DrawCredits()
    }
    Continue_Move_Down = 0;
    if ( g_GAME_HaveSaves == 0 )
    {
        Continue_Move_Down = 1;
    }
    else
    {
        if ( MouseOver_ControlIndex == GUI_Load_Lbl_Index )
            FLIC_Reset_CurrentFrame(gsa_VORTEX_5_MenuLoadGame);
        else
            FLIC_Set_CurrentFrame(gsa_VORTEX_5_MenuLoadGame, 1);
        FLIC_Draw(MenuArea_X_Left, (MenuArea_Y_Top + 12), fmainmenu_l);
    }
    if ( g_GAME_HaveContSave != 0 )
    {
        if ( MouseOver_ControlIndex == GUI_Continue_Label )
            FLIC_Reset_CurrentFrame(gsa_VORTEX_1_MenuContinue);
        else
            FLIC_Set_CurrentFrame(gsa_VORTEX_1_MenuContinue, 1);
        FLIC_Draw(MenuArea_X_Left, (MenuArea_Y_Top + (12 * Continue_Move_Down)), fmainmenu_c);
    }
    if ( MouseOver_ControlIndex == GUI_NewGame_Label )
        FLIC_Reset_CurrentFrame(gsa_VORTEX_4_MenuNewGame);
    else
        FLIC_Set_CurrentFrame(gsa_VORTEX_4_MenuNewGame, 1);
    FLIC_Draw(MenuArea_X_Left, (MenuArea_Y_Top + 24), fmainmenu_n);
    if ( MouseOver_ControlIndex == GUI_HoF_Lbl_Index )
        FLIC_Reset_CurrentFrame(gsa_VORTEX_2_MenuHallOfFame);
    else
        FLIC_Set_CurrentFrame(gsa_VORTEX_2_MenuHallOfFame, 1);
    FLIC_Draw(MenuArea_X_Left, (MenuArea_Y_Top + 36), fmainmenu_h);
    if ( MouseOver_ControlIndex == GUI_Quit_Lbl_Index )
        FLIC_Reset_CurrentFrame(gsa_VORTEX_3_MenuQuitToDOS);
    else
        FLIC_Set_CurrentFrame(gsa_VORTEX_3_MenuQuitToDOS, 1);
    FLIC_Draw(MenuArea_X_Left, (MenuArea_Y_Top + 48), fmainmenu_q);
    // FLIC_Draw_Back(MenuArea_X_Left, (MenuArea_Y_Top + 48), -1, fmainmenu_q, video_back_buffer);
    VGA_DAC_Write();
}
