
#include "ST_TYPE.H"
#include "ST_DEF.H"     /* FP_SEG(), FP_OFF(); */
#include "MGC_DEF.H"

#include "MGC_main.H"
#include "WZD_main.H"

/*
    EMM_Pages_Reserved = EMM_PAGES_REQUIRED;
    EMM_SetMinKB(EMM_MIN_KB);
    RAM_SetMinKB(RAM_MIN_KB);
*/
#include "ST_EMM.H"     /* EMM_Pages_Reserved */
#include "ST_DBG.H"     /* DBG_IsDisabled(), DBG_ScreenDump() */
#include "ST_FLIC.H"
#include "ST_HLP.H"     /* HLP_IDX_... */
#include "ST_LBX.H"     /* LBX_Error(), LBXE_LoadSingle() */
#include "ST_SA.H"      /* SA_Allocate_MemBlk(), SA_Allocate_Space(); */
#include "ST_VGA.H"     /* font_name */

#include "seg001.H"     /* GAME_LoadMainImages(); */
#include "seg014.H"     /* Hardware_Init(), Load_Font_File(); */
#include "seg019.H"     /* VGA_TextDraw_Init() */
#include "seg020.H"     /* palette_block, palette_data; PAL_Load_Palette() */
#include "seg021.H"     /* FLIC_Load_Palette(); */
#include "seg022.H"     /* ST_MoveData() */
#include "seg028.H"     /* FLIC_Draw_XY(); */
/* VGA_TextDraw_Init() */
/* IN_Init() */
/* MD_Init() */
/* MoM_Tables_Init() */
/* VGA_Set_DSP_Addr() */
/* VGA_DAC_Write() */

#include "STU_BMP.H"
#include "STU_DBG.H"    /* DLOG(); Debug_Log_Startup(), Debug_Log_Shutdown(); */
#include "STU_TST.H"    /* TLOG(); Test_Log_Startup(), Test_Log_Shutdown();*/
#include "STU_VGA.H"    /* STU_VGA_RAM_Dump(); */

#include <ASSERT.H>     /* NDEBUG; assert(); */
#include <CONIO.H>      /* getch() */
#include <STDLIB.H>     /* abort(), getenv(); */
#include <STRING.H>     /* strcmp() */
#include <stdio.h>
#include <alloc.h>


/*
    BCpp31PG, pg 175/190
        #pragma warn -yyy
    BCpp31UG, pg 162/171
    BCpp30UG, pg 158/167
        -waus*  Identifier is assigned a value that is never used.
        -wccc*  Condition is always true/false.
        -weff*  Code has no effect.
        -wpar*  Parameter parameter is never used.
        -wuse   Identifier is declared but never used.
        -wrpt*  Nonportable pointer conversion.
*/
#pragma warn -aus
#pragma warn -eff
#pragma warn -par


/*
    EMM - Uninitialized
        ? ALL Xref seg012 and seg013 ?
*/
//dseg:A5E0 UU_EMMData_PrevLvl dw 0
extern unsigned int g_EMM_Overlay_Handle;           // dseg:A5E2
extern unsigned int EMMDATAH_Level;                 // dseg:A5E4
extern unsigned int EmmHndlNbr_EMMDATAH;            // dseg:A5E6
// ? dseg:A5E8 UU_EMM_Handle dw 0
extern unsigned int g_EmmHndlNbr_VGAFILEH;          // dseg:A5EA
extern unsigned int g_LBX_EmmRsvd;                  // dseg:A5EC
extern unsigned int EmmHndlNbr_YOMOMA;              // dseg:A5EE
extern unsigned int EMM_OK;                         // dseg:A5F0
extern EMM_Record EMM_Table[];                      // dseg:A5F2 g_EMS_Table EMS_Record 28h dup(<0>)
extern unsigned int g_EMM_MinKB;                    // dseg:A7D2


unsigned char g_EMM_tested = 0;
unsigned char g_EMM_validated = 0;
unsigned char g_Load_Font_File_tested = 0;
unsigned char g_Load_Font_File_validated = 0;
unsigned char g_MAINSCRN_LBX_EMM_tested = 0;
unsigned char g_MAINSCRN_LBX_EMM_validated = 0;
unsigned char g_FLIC_Load_Palette_tested = 0;
unsigned char g_FLIC_Load_Palette_validated = 0;


void MoM_Startup(void);
void MoM_Shutdown(void);
void Init_STGE(void);

void test_VBB_Pattern(void);

void test_wait_for_esc(void);
void test_TST_Test_Screen(void);

void TST_Test_Screen(void);
void TST_Add_Test_Screen_Fields(void);
int TST_Do_Test_Screen(void);
void TST_Draw_Test_Screen(void);


void test_MGC_Main(void);
void test_VGA_VRAM(void);
void test_Load_MAINSCRN_LBX_EMM(void);
void test_Load_MAINSCRN_000(void);
void test_Load_MAINSCRN_005(void);
void test_VGA_SetDirectDraw(void);
void test_VGA_Set_DSP_Addr(void);
void test_PAL_Load_Palette(void);
void test_Load_Font_File(void);
void test_EMM_Init(void);
void test_EMM_Startup(void);
void test_EMM_Load_LBX_File(void);
void test_GAME_LoadMainImages(void);

void test_FLIC_Load_Palette(void);
void test_FLIC_Draw_EMM(void);
void test_FLIC_Draw_XY(void);

void test_FLIC_Draw_Back(void);
void test_FLIC_Draw_Frame_Back(void);
void test_FLIC_Draw_Frame_2BMP(void);

void test_STU_Export_Palette_XBGR(void);
void test_STU_Export_VBB_To_BMP(void);

void test_SA_Error(void);
void test_LBX_Error(void);


int main(int argc, char *argv[])
{
    char * cstr_ComSpec;
    Debug_Log_Startup();
    Test_Log_Startup();
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: TST_MGC main()\n", __FILE__, __LINE__);
#endif

    /* get comspec environment parameter */
    cstr_ComSpec = getenv("COMSPEC");
    /* display comspec parameter */
    dbg_prn("Command processor: %s\n", cstr_ComSpec);
    // flags = fnsplit(s,drive,dir,file,ext);
    if( strcmp(cstr_ComSpec,"Z:\\COMMAND.COM") == 0)
    {
        dbg_prn("Running in DOXBox...\n");
    }

#if defined(__VSCODE__)
    abort();
#endif

    // test_VGA_SetDirectDraw();
    // test_VGA_Set_DSP_Addr();

    // test_PAL_Load_Palette();  // TEST_SUCCESS, for FONTS.LBX,2

    // test_Load_Font_File();

    // test_EMM_Init();  // TEST_SUCCESS
    // test_EMM_Startup();  // TEST_SUCCESS
    // test_EMM_Load_LBX_File();

    //test_GAME_LoadMainImages();

    // test_Load_MAINSCRN_LBX_EMM();  // TEST_SUCCESS

    // test_Load_MAINSCRN_000();  // TEST_SUCCESS
    // test_Load_MAINSCRN_005();
    // 
    // // test_VGA_VRAM();
    // 
    // // test_FLIC_Load_Palette();
    // test_FLIC_Draw_EMM();
    // // test_FLIC_Draw_XY();

    // test_MGC_Main();
    MGC_main(argc, argv);
    // test_WZD_Main();
    // WZD_main(argc, argv);

    // test_wait_for_esc();
    // test_TST_Test_Screen();

    // // // Init_STGE();
    // // test_VBB_Pattern();
    // test_FLIC_Draw_Back();
    // // test_FLIC_Draw_Frame_Back();

    // test_STU_Export_Palette_XBGR();
    // test_STU_Export_VBB_To_BMP();

    // test_SA_Error();
    // test_LBX_Error();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: TST_MGC main()\n", __FILE__, __LINE__);
#endif
    Test_Log_Shutdown();
    Debug_Log_Shutdown();
    return 0;
}

void test_VBB_Pattern(void)
{
    VGA_SetModeY();

    test_VBB_Pattern_Write();
    // test_VBB_Pattern_Read();
    test_VBB_Pattern_Draw();
    // test_VBB_Pattern_Print();
    // test_VBB_Pattern_Dump();

    getch();

    VGA_SetTextMode();

}

void test_wait_for_esc(void)
{
    wait_for_esc();
}

void test_TST_Test_Screen(void)
{
    Init_STGE();
    TST_Test_Screen();
}

void MoM_Startup(void)
{

}
void MoM_Shutdown(void)
{

}
void Init_STGE(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Init_STGE()\n", __FILE__, __LINE__);
#endif

    EMM_Pages_Reserved = EMM_PAGES_REQUIRED;
    EMM_SetMinKB(EMM_MIN_KB);
    RAM_SetMinKB(RAM_MIN_KB);
    /* BEGIN: MGC main() |-> Hardware_Init() */
    EMM_Startup();
    VGA_SetModeY();
    Load_Font_File(GAME_FONT_FILE);
    IN_Init(INPUT_TYPE_KEYBOARD_AND_MOUSE);
    VGA_Set_DSP_Addr();
    /* END: MGC main() |-> Hardware_Init() */

    MoM_Tables_Init(6100);
    GAME_LoadMainImages();
    GAME_Load_TERRSTAT_0();
    GAME_Load_SPELLDAT_0();

    VGA_Set_DSP_Addr();
    PAL_Load_Palette(0, -1, 0);
    VGA_DAC_Write();
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Draw Screen-Page
    VGA_SetDirectDraw();
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Render Screen-Page
    VGA_Set_DSP_Addr();
    PAL_Load_Palette(0, -1, 0);  // EMPERATO
    VGA_DAC_Write();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Init_STGE()\n", __FILE__, __LINE__);
#endif
    // Quit();
    // VGA_SetTextMode();
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
        FLIC_Draw(0, 0, fp_MAINSCRN_000);
    // }
    FLIC_Draw(0, 41, fp_MAINSCRN_005);
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
        FLIC_Draw(MenuArea_X_Left, (MenuArea_Y_Top + 12), fp_VORTEX_005);
    }
    if ( g_GAME_HaveContSave != 0 )
    {
        if ( MouseOver_ControlIndex == GUI_Continue_Label )
            FLIC_Reset_CurrentFrame(gsa_VORTEX_1_MenuContinue);
        else
            FLIC_Set_CurrentFrame(gsa_VORTEX_1_MenuContinue, 1);
        FLIC_Draw(MenuArea_X_Left, (MenuArea_Y_Top + (12 * Continue_Move_Down)), fp_VORTEX_001);
    }
    if ( MouseOver_ControlIndex == GUI_NewGame_Label )
        FLIC_Reset_CurrentFrame(gsa_VORTEX_4_MenuNewGame);
    else
        FLIC_Set_CurrentFrame(gsa_VORTEX_4_MenuNewGame, 1);
    FLIC_Draw(MenuArea_X_Left, (MenuArea_Y_Top + 24), fp_VORTEX_004);
    if ( MouseOver_ControlIndex == GUI_HoF_Lbl_Index )
        FLIC_Reset_CurrentFrame(gsa_VORTEX_2_MenuHallOfFame);
    else
        FLIC_Set_CurrentFrame(gsa_VORTEX_2_MenuHallOfFame, 1);
    FLIC_Draw(MenuArea_X_Left, (MenuArea_Y_Top + 36), fp_VORTEX_002);
    if ( MouseOver_ControlIndex == GUI_Quit_Lbl_Index )
        FLIC_Reset_CurrentFrame(gsa_VORTEX_3_MenuQuitToDOS);
    else
        FLIC_Set_CurrentFrame(gsa_VORTEX_3_MenuQuitToDOS, 1);
    FLIC_Draw(MenuArea_X_Left, (MenuArea_Y_Top + 48), fp_VORTEX_003);
    // FLIC_Draw_Back(MenuArea_X_Left, (MenuArea_Y_Top + 48), -1, fp_VORTEX_003, video_back_buffer);
    VGA_DAC_Write();
}

void test_MGC_Main(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_MGC_Main()\n", __FILE__, __LINE__);
#endif

    // MGC main()
    DLOG("CALL: EMM_Pages_Reserved = EMM_PAGES_REQUIRED;");
    EMM_Pages_Reserved = EMM_PAGES_REQUIRED;
    // MGC main()
    DLOG("CALL: EMM_SetMinKB(EMM_MIN_KB);");
    EMM_SetMinKB(EMM_MIN_KB);
    // MGC main()
    DLOG("CALL: RAM_SetMinKB(RAM_MIN_KB);");
    RAM_SetMinKB(RAM_MIN_KB);

    // MGC main() |-> Hardware_Init()
    DLOG("CALL: EMM_Startup();");
    EMM_Startup();
    // MGC main() |-> Hardware_Init()
    DLOG("CALL: VGA_SetModeY();");
    VGA_SetModeY();
    // MGC main() |-> Hardware_Init()
    // s14p03
    DLOG("CALL: Load_Font_File(GAME_FONT_FILE);");
    Load_Font_File(GAME_FONT_FILE);  // "FONTS.LBX"
        // |-> ... LBXE_LoadSingle(FONTS.LBX,0), SA_Allocate_MemBlk()
        // |-> ... LBXE_LoadSingle(FONTS.LBX,1), SA_Allocate_MemBlk()
        // |-> VGA_TextDraw_Init()

    // MGC main() |-> Hardware_Init()
    DLOG("CALL: IN_Init(1);");
    IN_Init(1);  // INPUT_TYPE_KEYBOARD_AND_MOUSE
        // |-> MD_Init()
    // MGC main() |-> Hardware_Init()
    DLOG("CALL: VGA_Set_DSP_Addr();");
    VGA_Set_DSP_Addr();

    // MGC main()
    DLOG("CALL: MoM_Tables_Init(6100);");
    MoM_Tables_Init(6100);
    // MGC main()
    // s20p01
    DLOG("CALL: PAL_Load_Palette(0, -1, 0);");
    PAL_Load_Palette(0, -1, 0);
    // MGC main()
    DLOG("CALL: VGA_DAC_Write();");
    VGA_DAC_Write();
    // MGC main()
    DLOG("CALL: GAME_LoadMainImages();");
    GAME_LoadMainImages();
    // MGC main()
    DLOG("CALL: GAME_Load_TERRSTAT_0();");
    GAME_Load_TERRSTAT_0();
    // MGC main()
    DLOG("CALL: GAME_Load_SPELLDAT_0();");
    GAME_Load_SPELLDAT_0();
    // MGC main()
    DLOG("CALL: VGA_DrawFilledRect(0, 0, 319, 199, 0);");
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Draw Screen-Page
    // MGC main()
    DLOG("CALL: VGA_SetDirectDraw();");
    VGA_SetDirectDraw();
    // MGC main()
    DLOG("CALL: VGA_DrawFilledRect(0, 0, 319, 199, 0);");
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Render Screen-Page
    // MGC main()
    DLOG("CALL: VGA_Set_DSP_Addr();");
    VGA_Set_DSP_Addr();
    // MGC main()
    DLOG("CALL: PAL_Load_Palette(0, -1, 0);");
    PAL_Load_Palette(0, -1, 0);  // EMPERATO
    // MGC main()
    DLOG("CALL: VGA_DAC_Write();");
    VGA_DAC_Write();

    // MGC main()
    DLOG("CALL: GAME_MainMenu();");
    GAME_MainMenu();  // MGC_DEF.H  _s01p03c.c
    // ...
    //          Screen_Action = SCREEN_Menu();  // MGC_DEF.H  _s01p05c.c
    // MGC main() |-> GAME_MainMenu() |-> SCREEN_Menu()
    //             SCREEN_Menu_Draw();  // MGC_DEF.H  _s01p06c.c
    //             SCRN_SimplePageFlip();
    // MGC main() |-> GAME_MainMenu() |-> SCREEN_Menu() |-> SCREEN_Menu_Draw()
    //     FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);  // NOTE(JimBalcomb): This is the first call to FLIC_Draw_XY()
    //     FLIC_Draw_XY(0, 41, gsa_MAINSCRN_5_ScreenBottom);
    //     FLIC_Draw_XY(MenuArea_X_Left, (MenuArea_Y_Top + 12), gsa_VORTEX_5_MenuLoadGame);
    //     FLIC_Draw_XY(MenuArea_X_Left, (MenuArea_Y_Top + (12 * Continue_Move_Down)), gsa_VORTEX_1_MenuContinue);
    //     FLIC_Draw_XY(MenuArea_X_Left, (MenuArea_Y_Top + 24), gsa_VORTEX_4_MenuNewGame);
    //     FLIC_Draw_XY(MenuArea_X_Left, (MenuArea_Y_Top + 36), gsa_VORTEX_2_MenuHallOfFame);
    //     FLIC_Draw_XY(MenuArea_X_Left, (MenuArea_Y_Top + 48), gsa_VORTEX_3_MenuQuitToDOS);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_MGC_Main()\n", __FILE__, __LINE__);
#endif
    // Quit();
    // VGA_SetTextMode();
}

/*
    Test
        Write & Read VGA Graphics Mode Video Memory (VRAM)

    Sequencer Address Register and the Sequencer Data Register
    Sequencer Registers
    Map Mask Register (Index 02h)
        Memory Plane Write Enable

    Screen-Page Index
    Screen-Page Segment Address

*/
void test_VGA_VRAM(void)
{
//     // #define CLRSCR0() { VGA_DrawFilledRect(0,0,319,199,0); VGA_SetDrawAddress(); }
//     // #define CLRSCR1() { VGA_DrawFilledRect(0,0,319,199,0); VGA_SetDirectDraw();  }
//     // VGA_DrawFilledRect(0,0,319,199,0);
//     // VGA_SetDrawAddress();
//     // VGA_SetDirectDraw();
// 
//     Load_Font_File(GAME_FONT_FILE);
//     // unsigned int gsa_DSP_Addr = 0xA000;                 // dseg:41C4
//     // int g_RSP_Idx = 0;                                  // dseg:41C6
//     VGA_Set_DSP_Addr();  // Sets Screen-Page Segment Addres, based on Screen-Page Index    gsa_DSP_Addr = VRAM_BASE + ( (1 - g_RSP_Idx) << 10 );
//     // FLIC_Draw_EMM() uses gsa_DSP_Addr to set the Dst_Sgmt ... Also, Offset to Scan-Line
//     // NOTE: VGA_PageFlip() is the place that g_RSP_Idx is changed - it sets it to (1 - g_RSP_Idx)
//     PAL_Load_Palette(0, -1, 0);  // requires Load_Font_File(), LBXE_LoadReplace(), LBX_Load_Entry(), SA_Allocate_MemBlk(), SA_Allocate_Space(), 
//     VGA_DAC_Write();
//         // outportb( 0x3C8, itrVgaDacColors );
//         // outportb( 0x3C9, ptr_Palette[ofstPalette++] );
//         // outportb( 0x3C9, ptr_Palette[ofstPalette++] );
//         // outportb( 0x3C9, ptr_Palette[ofstPalette++] );
// 
// // asm     mov dx, 0x03C4 //; SC_INDEX
// // asm     mov al, 0x02  //; SC_MAP_MASK
// // asm     out dx, al
// 
// // asm mov dx, 0x03C5 //; SC_DATA
// // asm mov al, bl
// // asm out dx, al
// 
//     VGA_SetModeY();
//     getch();
//     // VGA_DrawFilledRect(0,0,319,199,0);
//     VGA_Set_DSP_Addr();  // gsa_DSP_Addr = VRAM_BASE + ( (1 - g_RSP_Idx) << 10 );
//     // VGA_DrawFilledRect(0,0,319,199,0);
//     VGA_SetDirectDraw();  // gsa_DSP_Addr = VRAM_BASE + ( (g_RSP_Idx) << 10 );
//     // ? Set the *Draw* to the Back-Buffer
//     // ? Set the *Draw* to the Front-Buffer
// 
//     // ; NOTE(JimBalcomb,20220721): I can not recall what the issue was such that I chose/needed to comment out this chunk of code
//     // ; mov	ax, VIDEO_RAM
//     // ; mov	es, ax
//     // ; sub	di, di
//     // ; mov	ax, di
//     // ; mov	cx, 8000h                           ; 32,768 times ... 65,536 zeros
//     // ; rep stosw  ; WORD [ES:DI] = AX;
// 
// 
//     VGA_SetTextMode();


    VGA_SetModeY();
    // g_RSP_Idx = 0;
    // gsa_DSP_Addr = 0xA000;
    VGA_DrawFilledRect(0,0,319,199,5);
    getch();
    // BAD!! STU_VGA_RAM_Dump_1();
    STU_VGA_RAM_Dump();
    
    VGA_SetTextMode();
}

void test_VGA_SetDirectDraw(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_VGA_SetDirectDraw()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dbg_prn("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dbg_prn("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    g_RSP_Idx = 0;
    VGA_SetDirectDraw();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dbg_prn("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dbg_prn("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    g_RSP_Idx = 1;
    VGA_SetDirectDraw();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dbg_prn("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dbg_prn("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    if ( g_RSP_Idx == 0 )
    {
        if ( gsa_DSP_Addr != 0xA000 )
        {
            dbg_prn("TEST: [%s, %d] FAILURE: ( (g_RSP_Idx == 0 ) && (gsa_DSP_Addr != 0xA000) )\n", __FILE__, __LINE__);
            dbg_prn("TEST: [%s, %d] FAILURE: g_RSP_Idx: %d; gsa_DSP_Addr: 0x%04X;\n", __FILE__, __LINE__, g_RSP_Idx, gsa_DSP_Addr);
            exit(1);
        }
        else
        {
            dbg_prn("TEST: [%s, %d] SUCCESS: ( (g_RSP_Idx == 0 ) && (gsa_DSP_Addr == 0xA000) )\n", __FILE__, __LINE__);
        }
    }
    else
    {
        dbg_prn("TEST: [%s, %d] SKIP: g_RSP_Idx != 0\n", __FILE__, __LINE__);
    }

    if ( g_RSP_Idx == 1 )
    {
        if ( gsa_DSP_Addr != 0xA400 )
        {
            dbg_prn("DEBUG: [%s, %d] FAILURE: ( (g_RSP_Idx == 1 ) && (gsa_DSP_Addr != 0xA400) )\n", __FILE__, __LINE__);
            dbg_prn("DEBUG: [%s, %d] FAILURE: g_RSP_Idx: %d; gsa_DSP_Addr: 0x%04X;\n", __FILE__, __LINE__, g_RSP_Idx, gsa_DSP_Addr);
            exit(1);
        }
        else
        {
            dbg_prn("DEBUG: [%s, %d] SUCCESS: ( (g_RSP_Idx == 1 ) && (gsa_DSP_Addr == 0xA400) )\n", __FILE__, __LINE__);
        }
    }
    else
    {
        dbg_prn("TEST: [%s, %d] SKIP: g_RSP_Idx != 1\n", __FILE__, __LINE__);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_VGA_SetDirectDraw()\n", __FILE__, __LINE__);
#endif
}

void test_Load_MAINSCRN_LBX_EMM(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_Load_MAINSCRN_LBX_EMM()\n", __FILE__, __LINE__);
#endif

    if (!g_EMM_tested) { test_EMM_Startup(); }
    if (!g_EMM_validated) {  abort(); }

    // MGC main() |-> GAME_LoadMainImages()
    EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);
    g_MAINSCRN_LBX_EMM_tested = 1;
    if( validate_MAINSCRN_LBX_EMM() ) { g_MAINSCRN_LBX_EMM_validated = 1; }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_Load_MAINSCRN_LBX_EMM()\n", __FILE__, __LINE__);
#endif
}

/*
unsigned int gsa_MAINSCRN_0_AnimatedLogo;   // dseg:52E8
gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
*/
void test_Load_MAINSCRN_000(void)
{
    SAMB_data sa_MAINSCRN_000;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_Load_MAINSCRN_000()\n", __FILE__, __LINE__);
#endif

    if(!g_EMM_tested) { test_EMM_Startup(); }
    if(!g_EMM_validated) {  abort(); }

    // GAME_LoadMainImages()
    // EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);
    if(!g_MAINSCRN_LBX_EMM_tested) { test_Load_MAINSCRN_LBX_EMM(); }
    if(!g_MAINSCRN_LBX_EMM_validated) { abort(); }

    sa_MAINSCRN_000 = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
    TST_LBX_MAINSCRN_000.Segment_Address = sa_MAINSCRN_000;

    validate_FLIC_Header(sa_MAINSCRN_000);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_Load_MAINSCRN_000()\n", __FILE__, __LINE__);
#endif
}

/*
unsigned int gsa_MAINSCRN_0_AnimatedLogo;   // dseg:52E8
gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
*/
void test_Load_MAINSCRN_005(void)
{
    SAMB_data sa_MAINSCRN_005;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_Load_MAINSCRN_005()\n", __FILE__, __LINE__);
#endif

    if (!g_EMM_tested) { test_EMM_Startup(); }
    if (!g_EMM_validated) { abort(); }

    // GAME_LoadMainImages()
    // EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);
    if(!g_MAINSCRN_LBX_EMM_tested) { test_Load_MAINSCRN_LBX_EMM(); }
    if(!g_MAINSCRN_LBX_EMM_validated) { abort(); }

    sa_MAINSCRN_005 = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 5);
    TST_LBX_MAINSCRN_005.Segment_Address = sa_MAINSCRN_005;

    validate_FLIC_Header(sa_MAINSCRN_005);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_Load_MAINSCRN_005()\n", __FILE__, __LINE__);
#endif
}

/*
    _s26p02c.c  void VGA_Set_DSP_Addr(void)
    
    ST_VGA.H
        extern unsigned int gsa_DSP_Addr;                   // dseg:41C4
        extern int g_RSP_Idx;                               // dseg:41C6
    ST_VGA.C
        unsigned int gsa_DSP_Addr = 0xA000;                 // dseg:41C4
        int g_RSP_Idx = 0;                                  // dseg:41C6
    
    Pre-Initialized
        screen-page index 0; screen-page adress 0xA000;
    In-Game
        g_RSP_Idx changed in VGA_PageFlip()
        gsa_DSP_Addr changed in VGA_Set_DSP_Addr() and VGA_SetDirectDraw()
        VGA_PageFlip() sets the (render) screen-page index as (1 - screen-page index)
        VGA_Set_DSP_Addr() sets the screen-page address based on (1 - screen-page index)
        VGA_SetDirectDraw() sets the screen-page address based on the screen-page index, rather than (1 - screen-page index)

*/
void test_VGA_Set_DSP_Addr(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_VGA_Set_DSP_Addr()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dbg_prn("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dbg_prn("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    g_RSP_Idx = 0;
    VGA_Set_DSP_Addr();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dbg_prn("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dbg_prn("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    g_RSP_Idx = 1;
    VGA_Set_DSP_Addr();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dbg_prn("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dbg_prn("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    if ( g_RSP_Idx == 0 )
    {
        if ( gsa_DSP_Addr != 0xA400 )
        {
            dbg_prn("TEST: [%s, %d] FAILURE: ( (g_RSP_Idx == 0 ) && (gsa_DSP_Addr != 0xA400) )\n", __FILE__, __LINE__);
            dbg_prn("TEST: [%s, %d] FAILURE: g_RSP_Idx: %d; gsa_DSP_Addr: 0x%04X;\n", __FILE__, __LINE__, g_RSP_Idx, gsa_DSP_Addr);
            exit(1);
        }
        else
        {
            dbg_prn("TEST: [%s, %d] SUCCESS: ( (g_RSP_Idx == 0 ) && (gsa_DSP_Addr == 0xA400) )\n", __FILE__, __LINE__);
        }
    }
    else
    {
        dbg_prn("TEST: [%s, %d] SKIP: g_RSP_Idx != 0\n", __FILE__, __LINE__);
    }

    if ( g_RSP_Idx == 1 )
    {
        if ( gsa_DSP_Addr != 0xA000 )
        {
            dbg_prn("DEBUG: [%s, %d] FAILURE: ( (g_RSP_Idx == 1 ) && (gsa_DSP_Addr != 0xA000) )\n", __FILE__, __LINE__);
            dbg_prn("DEBUG: [%s, %d] FAILURE: g_RSP_Idx: %d; gsa_DSP_Addr: 0x%04X;\n", __FILE__, __LINE__, g_RSP_Idx, gsa_DSP_Addr);
            exit(1);
        }
        else
        {
            dbg_prn("DEBUG: [%s, %d] SUCCESS: ( (g_RSP_Idx == 1 ) && (gsa_DSP_Addr == 0xA000) )\n", __FILE__, __LINE__);
        }
    }
    else
    {
        dbg_prn("TEST: [%s, %d] SKIP: g_RSP_Idx != 1\n", __FILE__, __LINE__);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_VGA_Set_DSP_Addr()\n", __FILE__, __LINE__);
#endif

}

void test_PAL_Load_Palette(void)
{
    char font_file[11] = "FONTS.LBX";
    char font_name[16];
    SAMB_ptr sad1_PaletteLbxEntry;
    SAMB_ptr sah1_PaletteLbxEntry;
    int entry;
    int start_color;
    int end_color;
    // int i;

    // ~== Load_Font_File()
    strcpy(font_name, font_file);
    sah1_PaletteLbxEntry    = SA_Allocate_Space(348);           // 348 paragraphs = 386 * 16 bytes = 5568 bytes
    p_Palette               = SA_Allocate_Space(64);            //  64 paragraphs =  64 * 16 bytes = 1024 bytes
    p_PaletteFlags          = p_Palette + (48 * 16);            // ~== p_PaletteFlags = &p_Palette[768];
    // for ( i = 0; i < 768; i++)
    // {
    //     *(p_Palette + i) = 0;
    // }
    // for ( i = 0; i < 256; i++)
    // {
    //     *(p_PaletteFlags + i) = 0;
    // }

    dbg_prn("DEBUG: [%s, %d] sah1_PaletteLbxEntry: %p\n", __FILE__, __LINE__, sah1_PaletteLbxEntry);
    dbg_prn("DEBUG: [%s, %d] p_Palette: %p\n", __FILE__, __LINE__, p_Palette);
    dbg_prn("DEBUG: [%s, %d] p_PaletteFlags: %p\n", __FILE__, __LINE__, p_PaletteFlags);

    // ~== s20p01 PAL_Load_Palette()
    // void PAL_Load_Palette(int Palette_Index, int First_Color, int Last_Color)
    // MGC main() |-> PAL_Load_Palette(0, -1, 0);  // EMPERATO
    // MoO2  Module: fonts  Load_Palette(signed integer (2 bytes) entry, signed integer (2 bytes) start_color, signed integer (2 bytes) end_color)
    entry = 0;
    start_color = -1;
    end_color = 0;
    sad1_PaletteLbxEntry  = (SAMB_ptr)MK_FP(LBXE_LoadReplace(font_name, entry+2, (SAMB_addr)FP_SEG(sah1_PaletteLbxEntry)),0);

    validate_PaletteLbxEntry_2(sad1_PaletteLbxEntry);

    // s20p05 VGA_SetDACChanged()
    // if ( start_color == -1 ) { VGA_SetDACChanged(0, 255); } else { VGA_SetDACChanged(start_color, end_color); }

    dbg_prn("DEBUG: [%s, %d] sad1_PaletteLbxEntry: 0x%04X\n", __FILE__, __LINE__, sad1_PaletteLbxEntry);

    // ~== VGA_DAC_Write()

}


/*
    MGC main()
        |-> Hardware_Init(1, 2, GAME_FONT_FILE, 0, 0, 0, 0, 0, 0, 0, 0);  // Defaults for 'No Sound'
            |-> Load_Font_File()
    
    Load_Font_File()
        |-> ... LBXE_LoadSingle(), SA_Allocate_Space(), VGA_TextDraw_Init()
    
*/
void test_Load_Font_File(void)
{
    int test_status;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_Load_Font_File()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED

    dbg_prn("DEBUG: [%s, %d] font_name: %s\n", __FILE__, __LINE__, GAME_FONT_FILE);

    dbg_prn("DEBUG: [%s, %d] font_name: %s\n", __FILE__, __LINE__, font_name);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_FontStyleData: 0x%04X\n", __FILE__, __LINE__, gsa_FontStyleData);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_BorderStyleData: 0x%04X\n", __FILE__, __LINE__, gsa_BorderStyleData);
    // dbg_prn("DEBUG: [%s, %d] sah1_PaletteLbxEntry: %p\n", __FILE__, __LINE__, sah1_PaletteLbxEntry);
    dbg_prn("DEBUG: [%s, %d] palette_block: %p\n", __FILE__, __LINE__, palette_block);
    dbg_prn("DEBUG: [%s, %d] p_Palette: %p\n", __FILE__, __LINE__, p_Palette);
    dbg_prn("DEBUG: [%s, %d] p_PaletteFlags: %p\n", __FILE__, __LINE__, p_PaletteFlags);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_PaletteSaved: 0x%04X\n", __FILE__, __LINE__, gsa_PaletteSaved);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_ReplacementColors: 0x%04X\n", __FILE__, __LINE__, gsa_ReplacementColors);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_VGAFILEH_Header: 0x%04X\n", __FILE__, __LINE__, gsa_VGAFILEH_Header);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_IntensityScaleTable: 0x%04X\n", __FILE__, __LINE__, gsa_IntensityScaleTable);

    // SM2LM dbg_prn("DEBUG: [%s, %d] UU_g_VGA_TextDraw_Initd: %u\n", __FILE__, __LINE__, UU_g_VGA_TextDraw_Initd);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gfp_VGA_TextLine_Lefts: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Lefts);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gfp_VGA_TextLine_Rights: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Rights);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gfp_VGA_TextLine_Tops: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Tops);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gfp_VGA_TextLine_Starts: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Starts);


    Load_Font_File(GAME_FONT_FILE);


    dbg_prn("DEBUG: [%s, %d] font_name: %s\n", __FILE__, __LINE__, font_name);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_FontStyleData: 0x%04X\n", __FILE__, __LINE__, gsa_FontStyleData);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_BorderStyleData: 0x%04X\n", __FILE__, __LINE__, gsa_BorderStyleData);
    // dbg_prn("DEBUG: [%s, %d] sah1_PaletteLbxEntry: %p\n", __FILE__, __LINE__, sah1_PaletteLbxEntry);
    dbg_prn("DEBUG: [%s, %d] palette_block: %p\n", __FILE__, __LINE__, palette_block);
    dbg_prn("DEBUG: [%s, %d] p_Palette: %p\n", __FILE__, __LINE__, p_Palette);
    dbg_prn("DEBUG: [%s, %d] p_PaletteFlags: %p\n", __FILE__, __LINE__, p_PaletteFlags);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_PaletteSaved: 0x%04X\n", __FILE__, __LINE__, gsa_PaletteSaved);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_ReplacementColors: 0x%04X\n", __FILE__, __LINE__, gsa_ReplacementColors);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_VGAFILEH_Header: 0x%04X\n", __FILE__, __LINE__, gsa_VGAFILEH_Header);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_IntensityScaleTable: 0x%04X\n", __FILE__, __LINE__, gsa_IntensityScaleTable);

    // SM2LM dbg_prn("DEBUG: [%s, %d] UU_g_VGA_TextDraw_Initd: %u\n", __FILE__, __LINE__, UU_g_VGA_TextDraw_Initd);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gfp_VGA_TextLine_Lefts: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Lefts);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gfp_VGA_TextLine_Rights: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Rights);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gfp_VGA_TextLine_Tops: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Tops);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gfp_VGA_TextLine_Starts: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Starts);

    // ASSERT()
    if ( strcmp(font_name, "FONTS.LBX") != 0 )
    {
        dbg_prn("DEBUG: [%s, %d] FAILURE: ( strcmp(font_name, \"FONTS.LBX\") != 0 )\n", __FILE__, __LINE__);
        dbg_prn("DEBUG: [%s, %d] FAILURE: font_name: %s;\n", __FILE__, __LINE__, font_name);
        exit(1);
    }
    else
    {
        dbg_prn("DEBUG: [%s, %d] SUCCESS: ( strcmp(font_name, \"FONTS.LBX\") == 0 )\n", __FILE__, __LINE__);
    }

    // validate_Palette_0();
    test_status = validate_PaletteFlags_1();
    tst_prn("TEST: [%s, %d] test_status: %d\n", __FILE__, __LINE__, test_status);
    // TSTPRN(("TEST: [%s, %d] test_status: %d\n", __FILE__, __LINE__, test_status));  // WTF!?! "Warning: Condition is always true in function..."
    if ( test_status == -1 )  // TEST_FAILURE
    {
        dbg_prn("DEBUG: [%s, %d] FAILURE: validate_PaletteFlags_1()\n", __FILE__, __LINE__);
        exit(1);
    }

    g_Load_Font_File_tested = 1;
    if ( validate_Load_Font_File() ) { g_Load_Font_File_validated = 1; }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_Load_Font_File()\n", __FILE__, __LINE__);
#endif
}


void test_EMM_Init(void)
{
    int result;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_EMM_Init()\n", __FILE__, __LINE__);
#endif

    result = EMM_Init();

    if ( result == ST_SUCCESS )
    {
        dbg_prn("DEBUG: [%s, %d] SUCCESS: \n", __FILE__, __LINE__);
    }
    if ( result == ST_FAILURE )
    {
        dbg_prn("DEBUG: [%s, %d] FAILURE: \n", __FILE__, __LINE__);
    }
    if ( ( result != ST_SUCCESS ) && ( result != ST_FAILURE ) )
    {
        dbg_prn("DEBUG: [%s, %d] INVALID: \n", __FILE__, __LINE__);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_EMM_Init()\n", __FILE__, __LINE__);
#endif
}

void test_EMM_Startup(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_EMM_Startup()\n", __FILE__, __LINE__);
#endif
    // MGC main()
    EMM_Pages_Reserved = EMM_PAGES_REQUIRED;
    // EMM_SetMinKB(EMM_MIN_KB);
    // RAM_SetMinKB(RAM_MIN_KB);
    // MGC main() |-> Hardware_Init() |-> EMM_Startup()
    EMM_Startup();
    g_EMM_tested = 1;
    if ( validate_EMM_Startup() ) { g_EMM_validated = 1; }
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_EMM_Startup()\n", __FILE__, __LINE__);
#endif
}

/*
    Test:
        s13p03          void EMM_Load_LBX_File_1(char * LbxFileName);
        s13p04          int EMM_Load_LBX_File(char * LbxFileName, int Reserved);
    Prerequisites:
        EMM_Startup() |-> EMM_Init() {EMM_PageFrameBaseAddress}

*/
void test_EMM_Load_LBX_File(void)
{

    int Title_Frame_Index;  // _s01p06c.c  SCREEN_Menu_Draw()
    struct s_FLIC_HDR * pFlicHeader; // _s21p07c.c  FLIC_Load_Palette()
    SAMB_addr SAMB_data;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_EMM_Load_LBX_File()\n", __FILE__, __LINE__);
#endif

    // main() |-> Hardware_Init() |->
    EMM_Startup();

#ifdef STU_TEST
    if ( EMM_PageFrameBaseAddress != 0xE000 )
    {
        dbg_prn("DEBUG: [%s, %d] FAILURE: EMM_Init()\n", __FILE__, __LINE__);
        dbg_prn("DEBUG: [%s, %d] EMM_PageFrameBaseAddress: 0x%04X }\n", __FILE__, __LINE__, EMM_PageFrameBaseAddress);
        exit(1);
    }
    if (
        ( EMM_OK != ST_TRUE) ||
        ( EmmHndlNbr_YOMOMA == 0 ) ||
        ( g_EmmHndlNbr_VGAFILEH == 0 ) ||
        ( EmmHndlNbr_EMMDATAH == 0 ) ||
        ( g_EMM_Open_Handles != 3 ) ||
        ( EMM_Pages_Reserved != 31 )
    )
    {
        dbg_prn("DEBUG: [%s, %d] FAILURE: EMM_Startup()\n", __FILE__, __LINE__);
        dbg_prn("DEBUG: [%s, %d] EMM_OK: %d\n", __FILE__, __LINE__, EMM_OK);
        dbg_prn("DEBUG: [%s, %d] EmmHndlNbr_YOMOMA: %d\n", __FILE__, __LINE__, EmmHndlNbr_YOMOMA);
        dbg_prn("DEBUG: [%s, %d] g_EmmHndlNbr_VGAFILEH: %d\n", __FILE__, __LINE__, g_EmmHndlNbr_VGAFILEH);
        dbg_prn("DEBUG: [%s, %d] EmmHndlNbr_EMMDATAH: %d\n", __FILE__, __LINE__, EmmHndlNbr_EMMDATAH);
        dbg_prn("DEBUG: [%s, %d] g_EMM_Open_Handles: %d\n", __FILE__, __LINE__, g_EMM_Open_Handles);
        dbg_prn("DEBUG: [%s, %d] EMM_Pages_Reserved: %d\n", __FILE__, __LINE__, EMM_Pages_Reserved);
        exit(1);
    }
#endif

    // MGC main() |->
    // void GAME_LoadMainImages(void)
    EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);
        // |-> EMM_Load_LBX_File(LbxFileName=MAINSCRN, EmmRsvd=1)

    gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
    sa_MAINSCRN_000 = gsa_MAINSCRN_0_AnimatedLogo;
    fp_MAINSCRN_000 = (SAMB_ptr)MK_FP(gsa_MAINSCRN_0_AnimatedLogo, 0);
        // |-> LBX_Load_Entry(LbxName=MAINSCRN, LbxEntry=0, SAMB_head=0x0000, LoadType=0, LbxHdrFmt=0)
        // |-> EMM_LBX_Load_Entry(EmmHndlNm=MAINSCRN, LbxEntry=0, SAMB_head=0x0000, LoadType=0, FormatType=0)
        // |-> EMM_LBX_FLIC_Header(EmmHndl=7, EmmHndlNm=MAINSCRN, LbxEntry=0, SAMB_head=0x0000, LoadType=0)
        // |-> SA_Allocate_MemBlk(nparas = 1)

    // ...
    // ...
    // ...

    // DBG_MAINSCRN_000 = gsa_MAINSCRN_0_AnimatedLogo;
    TST_LBX_MAINSCRN_000.Segment_Address = gsa_MAINSCRN_0_AnimatedLogo;
    // dbg_prn("DEBUG: [%s, %d] DBG_MAINSCRN_000: 0x%04X\n", __FILE__, __LINE__, DBG_MAINSCRN_000);
    dbg_prn("DEBUG: [%s, %d] MAINSCRN_000.Segment_Address: 0x%04X\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.Segment_Address);


    pFlicHeader = (struct s_FLIC_HDR *)MK_FP(gsa_MAINSCRN_0_AnimatedLogo, 0);
    //pFlicHeader = (void _FAR *) ( ((unsigned long) (gsa_MAINSCRN_0_AnimatedLogo) << 16) | (0) ) )
    dbg_prn("DEBUG: [%s, %d] pFlicHeader: %p\n", __FILE__, __LINE__, pFlicHeader);
    
    dbg_prn("DEBUG: [%s, %d] Width = %d\n", __FILE__, __LINE__, pFlicHeader->Width);

    SAMB_data = gsa_MAINSCRN_0_AnimatedLogo;
    dbg_prn("DEBUG: [%s, %d] SAMB_data = gsa_MAINSCRN_0_AnimatedLogo: (0x%04X = 0x%04X) == %s\n", __FILE__, __LINE__, SAMB_data, gsa_MAINSCRN_0_AnimatedLogo, ((SAMB_data == gsa_MAINSCRN_0_AnimatedLogo) ? "TRUE" : "FALSE"));
    dbg_prn("DEBUG: [%s, %d] Width = %d\n", __FILE__, __LINE__,                    farpeekw(SAMB_data, FlicHdr_Width));
    dbg_prn("DEBUG: [%s, %d] Height = %d\n", __FILE__, __LINE__,                   farpeekw(SAMB_data, FlicHdr_Height));
    dbg_prn("DEBUG: [%s, %d] CurrentFrame = %d\n", __FILE__, __LINE__,             farpeekw(SAMB_data, FlicHdr_CurrentFrame));
    dbg_prn("DEBUG: [%s, %d] FrameCount = %d\n", __FILE__, __LINE__,               farpeekw(SAMB_data, FlicHdr_FrameCount));
    dbg_prn("DEBUG: [%s, %d] LoopFrame = %d\n", __FILE__, __LINE__,                farpeekw(SAMB_data, FlicHdr_LoopFrame));
    dbg_prn("DEBUG: [%s, %d] EmmHandleNumber = %d\n", __FILE__, __LINE__,          farpeekb(SAMB_data, FlicHdr_EmmHandleNumber));
    dbg_prn("DEBUG: [%s, %d] EmmLogicalPageIndex = %d\n", __FILE__, __LINE__,      farpeekb(SAMB_data, FlicHdr_EmmLogicalPageIndex));
    dbg_prn("DEBUG: [%s, %d] EmmLogicalPageOffset = 0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_EmmLogicalPageOffset));
    dbg_prn("DEBUG: [%s, %d] PaletteDataOffset = 0x%04X\n", __FILE__, __LINE__,    farpeekw(SAMB_data, FlicHdr_PaletteDataOffset));

    // int SCREEN_Menu(void)
    FLIC_Reset_CurrentFrame(gsa_MAINSCRN_0_AnimatedLogo);

    // SCREEN_Menu() |-> SCREEN_Menu_Draw() |->
    Title_Frame_Index = FLIC_Get_CurrentFrame(gsa_MAINSCRN_0_AnimatedLogo);
    FLIC_Set_CurrentFrame(gsa_MAINSCRN_0_AnimatedLogo, 0);
    DLOG("CALL: VGA_SetModeY();");
    VGA_SetModeY();
    // FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);
    FLIC_Draw(0, 0, fp_MAINSCRN_000);
        // ST_MoveData(destoff=0xB47C, destseg=0x0000, srcoff=0x0000, srcseg=0x20D1, nbytes=16)
        // FLIC_Load_Palette(FlicHdr_SgmtAddr=0x20D1, Frame_Index=0)
        // FLIC_Draw_EMM_C(ScreenPage_X=0, ScreenPage_Y=0, SAMB_data_FLIC_HDR=0x20D1, Frame_Index=0)
    // void FLIC_Load_Palette(SAMB_addr FlicHdr_SgmtAddr, int Frame_Index)

    DLOG("CALL: VGA_SetTextMode();");
    VGA_SetTextMode();
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_EMM_Load_LBX_File()\n", __FILE__, __LINE__);
#endif
}

void test_GAME_LoadMainImages(void)
{
    int itr_EMM_Table_Index;
    WORD EMM_Handle_Number;
    BYTE EMM_Physical_Page_Number;
    WORD EMM_Logical_Page_Number;
    WORD LBX_EntryCount;
    WORD LBX_MagSig_Hi;
    WORD LBX_MagSig_Lo;
    WORD LBX_Type;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_GAME_LoadMainImages()\n", __FILE__, __LINE__);
#endif

    EMM_Startup();
    GAME_LoadMainImages();
    validate_GAME_LoadMainImages();

    /*
        Test what, where, how?
            ?
            EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);
            ?
            gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
            ?
            Did MAINSCRN.LBX load into EMM properly?
            ? 

    */
    // TST_LBX_MAINSCRN_000.LBX_File_Name: MAINSCRN.LBX
    // TST_LBX_MAINSCRN_000.LBX_Name: MAINSCRN
    // TST_LBX_MAINSCRN_000.EMM_Table_Index: 0
    // TST_LBX_MAINSCRN_000.LBX_File_Size_B: 196511
    // TST_LBX_MAINSCRN_000.LBX_File_Size_PR: 0
    // TST_LBX_MAINSCRN_000.LBX_File_Size_PG: 12
    // TST_LBX_MAINSCRN_000.EMM_Logical_Page_Count: 11
    // TST_LBX_MAINSCRN_000.EMM_Handle_Number: 4
    dbg_prn("DEBUG: [%s, %d] g_EMM_Open_Handles: %u \n", __FILE__, __LINE__, g_EMM_Open_Handles);
    dbg_prn("DEBUG: [%s, %d] g_EMM_Pages_Reserved: %d \n", __FILE__, __LINE__, EMM_Pages_Reserved);
    // same logic as in EMM_CheckHandleOpen()
    for ( itr_EMM_Table_Index = 0; itr_EMM_Table_Index < g_EMM_Open_Handles; itr_EMM_Table_Index++ )
    {
        if ( stricmp(EMM_Table[itr_EMM_Table_Index].eEmmHndlNm, TST_LBX_MAINSCRN_000.LBX_Name) == 0 )
        {
            dbg_prn("DEBUG: [%s, %d] EMM_Table[%d].eEmmHndlNm: %s \n", __FILE__, __LINE__, itr_EMM_Table_Index, EMM_Table[itr_EMM_Table_Index].eEmmHndlNm);
            dbg_prn("DEBUG: [%s, %d] EMM_Table[%d].eEmmHndlNbr: %u \n", __FILE__, __LINE__, itr_EMM_Table_Index, EMM_Table[itr_EMM_Table_Index].eEmmHndlNbr);
            dbg_prn("DEBUG: [%s, %d] EMM_Table[%d].eEmmRsrvd: %d \n", __FILE__, __LINE__, itr_EMM_Table_Index, EMM_Table[itr_EMM_Table_Index].eEmmRsrvd);
        }
    }
    //TST_LBX_MAINSCRN_000.EMM_Table_Index
    //EMM_Table[itr_EMM_Table_Index].eEmmHndlNbr

    // s12p10  EMM_GetPageFrame()
    // s12p08  EMM_MapnRead()  s12p11  EMM_Map4()  s12p12  EMM_MapMulti4()
    // EMM_MAP_PAGE(_epp_,_ehn_,_elp_)
    EMM_Handle_Number = EMM_Table[TST_LBX_MAINSCRN_000.EMM_Table_Index].eEmmHndlNbr;
    EMM_Physical_Page_Number = 0;
    EMM_Logical_Page_Number = 0;
    // EMM_MAP_PAGE(0,4,0);
    // EMM_MAP_PAGE(EMM_Physical_Page_Number, EMM_Handle_Number, EMM_Logical_Page_Number);
    EMM_Map4(EMM_Handle_Number, EMM_Logical_Page_Number);
    dbg_prn("DEBUG: [%s, %d] EMM_PageFrameBaseAddress: 0x%04X\n", __FILE__, __LINE__, EMM_PageFrameBaseAddress);
    // dbg_prn("DEBUG: [%s, %d] LBX_EntryCount: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 0));
    // dbg_prn("DEBUG: [%s, %d] LBX_MagSig_Hi: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 2));
    // dbg_prn("DEBUG: [%s, %d] LBX_MagSig_Lo: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 4));
    // dbg_prn("DEBUG: [%s, %d] LBX_Type: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 6));
    LBX_EntryCount = (WORD)(EMM_PageFrameBaseAddress + 0);
    LBX_MagSig_Hi  = (WORD)(EMM_PageFrameBaseAddress + 2);
    LBX_MagSig_Lo  = (WORD)(EMM_PageFrameBaseAddress + 4);
    LBX_Type       = (WORD)(EMM_PageFrameBaseAddress + 6);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_GAME_LoadMainImages()\n", __FILE__, __LINE__);
#endif
}


/*  s21p07  void FLIC_Load_Palette(SAMB_addr sa_FLIC_Header, int Frame_Index);  */
void test_FLIC_Load_Palette(void)
{
    int test_status;
    //  void FLIC_Draw_XY(int Left, int Top, SAMB_addr sa_FLIC_Header)
    //      static struct s_FLIC_HDR FLIC_Header;
    //      int Frame_Index;
    SAMB_addr sa_FLIC_Header;
    static struct s_FLIC_HDR PS_FLIC_Header;
    int Frame_Index;
    struct s_FLIC_HDR _FAR * pPS_FLIC_Header;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_FLIC_Load_Palette()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED

    // TST_LBX_MAINSCRN_000.Segment_Address = gsa_MAINSCRN_0_AnimatedLogo;
    tst_prn("TEST: [%s, %d] TST_LBX_MAINSCRN_000.Segment_Address: 0x%04X\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.Segment_Address);
    sa_FLIC_Header = TST_LBX_MAINSCRN_000.Segment_Address;
    tst_prn("TEST: [%s, %d] sa_FLIC_Header: 0x%04X\n", __FILE__, __LINE__, sa_FLIC_Header);

    if(!validate_FLIC_Header(sa_FLIC_Header)) { test_status = -1; }  // TEST_FAILURE

    if(!g_Load_Font_File_tested) { test_Load_Font_File(); }
    if(!g_Load_Font_File_validated) { test_status = -1; }  // TEST_FAILURE

    // FLIC_Draw_XY()
    // int ST_MoveData(unsigned int destoff, unsigned int destseg, unsigned int srcoff, unsigned int srcseg, unsigned int nbytes);
    ST_MoveData((unsigned int)&PS_FLIC_Header, 0, 0, sa_FLIC_Header, sizeof(PS_FLIC_Header));
    //  ?  memcpy(); memmove() |-> movmem();  movedata();  ?

    // FLIC_Draw_XY()
    Frame_Index = PS_FLIC_Header.Current_Frame;

    // FLIC_Draw_XY()
    if ( PS_FLIC_Header.Palette_Header_Offset != 0 )
    {
        TLOG("CALL: FLIC_Load_Palette(sa_FLIC_Header, Frame_Index);");
        // FLIC_Load_Palette(sa_FLIC_Header, Frame_Index);  // s21p07
        // FLIC_Load_Palette_FP_EMM(sa_FLIC_Header, Frame_Index);
        FLIC_Load_Palette(MK_FP(sa_FLIC_Header,0), Frame_Index);  // s21p07
        if(!validate_PaletteFlags_M00()) { test_status = -1; }  // TEST_FAILURE
        if(!validate_Palette_M00()) { test_status = -1; }  // TEST_FAILURE
    }

    g_FLIC_Load_Palette_tested = 1;
    // if ( validate_FLIC_Load_Palette() ) { g_FLIC_Load_Palette_validated = 1; }
    if ( test_status != -1 ) { g_FLIC_Load_Palette_validated = 1; }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_FLIC_Load_Palette()\n", __FILE__, __LINE__);
#endif
}

/*  s27p03  void FLIC_Draw_EMM_C(int ScreenPage_X, int ScreenPage_Y, SAMB_addr SAMB_data_FLIC_HDR, int Frame_Index)  */
void test_FLIC_Draw_EMM(void)
{
    int test_status;
    //  void FLIC_Draw_XY(int Left, int Top, SAMB_addr sa_FLIC_Header)
    //      static struct s_FLIC_HDR FLIC_Header;
    //      int Frame_Index;
    int Left;
    int Top;
    SAMB_addr sa_FLIC_Header;
    static struct s_FLIC_HDR PS_FLIC_Header;
    int Frame_Index;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_FLIC_Draw_EMM()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED

    // TST_LBX_MAINSCRN_000.Segment_Address = gsa_MAINSCRN_0_AnimatedLogo;
    tst_prn("TEST: [%s, %d] TST_LBX_MAINSCRN_000.Segment_Address: 0x%04X\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.Segment_Address);
    sa_FLIC_Header = TST_LBX_MAINSCRN_000.Segment_Address;
    tst_prn("TEST: [%s, %d] sa_FLIC_Header: 0x%04X\n", __FILE__, __LINE__, sa_FLIC_Header);

    if(!validate_FLIC_Header(sa_FLIC_Header)) { test_status = -1; }  // TEST_FAILURE

    if(!g_Load_Font_File_tested) { test_Load_Font_File(); }
    if(!g_Load_Font_File_validated) { test_status = -1; }  // TEST_FAILURE

    // FLIC_Draw_XY()
    // int ST_MoveData(unsigned int destoff, unsigned int destseg, unsigned int srcoff, unsigned int srcseg, unsigned int nbytes);
    ST_MoveData((unsigned int)&PS_FLIC_Header, 0, 0, sa_FLIC_Header, sizeof(PS_FLIC_Header));
    //  ?  memcpy(); memmove() |-> movmem();  movedata();  ?

    // FLIC_Draw_XY()
    // Frame_Index = PS_FLIC_Header.Current_Frame;
    Frame_Index = 0;

    // main() |-> GAME_MainMenu() |-> SCREEN_Menu() |-> SCREEN_Menu_Draw() |-> FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);
    // FLIC_Draw_EMM_C(Left, Top, sa_FLIC_Header, Frame_Index);
    Left = 0;
    Top = 0;
    // FLIC_Draw_EMM_C(Left, Top, sa_FLIC_Header, Frame_Index);
    // 
    // // no-workie DBG_ScreenDump();
    // 
    // // // STU_VGA_DAC_Dump("MENUDAC.BIN");  // STU_VGA.C/.H
    // // // // VGA_Set_DSP_Addr();
    // // // VGA_PageFlip();
    // // VGA_DAC_Write();

    VGA_SetModeY();
    // g_RSP_Idx = 0;
    // gsa_DSP_Addr = 0xA000;
    VGA_DrawFilledRect(0,0,319,199,5);
    getch();
    // FLIC_Draw_EMM_C(Left, Top, sa_FLIC_Header, Frame_Index);
    FLIC_Draw_Frame_EMM(Left, Top, sa_FLIC_Header, Frame_Index);
    // VGA_PageFlip();
    getch();
    VGA_DAC_Write();
    getch();
    FLIC_Load_Palette(MK_FP(sa_FLIC_Header,0), Frame_Index);  // s21p07
    getch();
    VGA_DAC_Write();
    getch();
    // VGA_PageFlip();
    // getch();
    // BAD!! STU_VGA_RAM_Dump_1();
    STU_VGA_RAM_Dump();

    VGA_SetTextMode();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_FLIC_Draw_EMM()\n", __FILE__, __LINE__);
#endif
}

/*
    Test
        FLIC_Draw_XY()
*/
/*  s29p11  void FLIC_Draw_XY(int Left, int Top, SAMB_addr sa_FLIC_Header)  */
void test_FLIC_Draw_XY(void)
{
    //  void FLIC_Draw_XY(int Left, int Top, SAMB_addr sa_FLIC_Header)
    //      static struct s_FLIC_HDR FLIC_Header;
    //      int Frame_Index;
    int Left;
    int Top;
    SAMB_addr sa_FLIC_Header;
    static struct s_FLIC_HDR PS_FLIC_Header;
    int Frame_Index;
    struct s_FLIC_HDR _FAR * pPS_FLIC_Header;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_FLIC_Draw_XY()\n", __FILE__, __LINE__);
#endif

    // GAME_LoadMainImages()
    // gsa_MAINSCRN_0_AnimatedLogo  = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
    // gsa_MAINSCRN_5_ScreenBottom  = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 5);
    // gsa_VORTEX_1_MenuContinue    = LBXE_LoadSingle(g_LbxNm_VORTEX, 1);
    // gsa_VORTEX_2_MenuHallOfFame  = LBXE_LoadSingle(g_LbxNm_VORTEX, 2);
    // gsa_VORTEX_3_MenuQuitToDOS   = LBXE_LoadSingle(g_LbxNm_VORTEX, 3);
    // gsa_VORTEX_4_MenuNewGame     = LBXE_LoadSingle(g_LbxNm_VORTEX, 4);
    // gsa_VORTEX_5_MenuLoadGame    = LBXE_LoadSingle(g_LbxNm_VORTEX, 5);

    // TST_LBX_MAINSCRN_000.Segment_Address = gsa_MAINSCRN_0_AnimatedLogo;
    tst_prn("TEST: [%s, %d] TST_LBX_MAINSCRN_000.Segment_Address: 0x%04X\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.Segment_Address);
    sa_FLIC_Header = TST_LBX_MAINSCRN_000.Segment_Address;
    tst_prn("TEST: [%s, %d] sa_FLIC_Header: 0x%04X\n", __FILE__, __LINE__, sa_FLIC_Header);
    validate_FLIC_Header(sa_FLIC_Header);

    test_Load_Font_File();

    // FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);
    // e0s0 FLIC_Draw_A(Left, Top, PS_FLIC_Header.Width, FLIC_Frame_Ofst, FLIC_Frame_Sgmt);

    // FLIC_Draw_XY(32, 20, gsa_VORTEX_3_MenuQuitToDOS);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_FLIC_Draw_XY()\n", __FILE__, __LINE__);
#endif
}

void test_FLIC_Draw_Back(void)
{
    int MenuArea_X_Left;
    int MenuArea_Y_Top;
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_FLIC_Draw_Back()\n", __FILE__, __LINE__);
#endif

    MenuArea_X_Left = 123;
    MenuArea_Y_Top = 141;

    video_back_buffer = SA_Allocate_MemBlk(4000);
    video_back_buffer_seg = FP_SEG(video_back_buffer);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] video_back_buffer: %p\n", __FILE__, __LINE__, video_back_buffer);
    dbg_prn("DEBUG: [%s, %d] video_back_buffer_seg: 0x%04X\n", __FILE__, __LINE__, video_back_buffer_seg);
#endif
    memset(video_back_buffer, CYAN, 320*200);

    Load_Font_File(GAME_FONT_FILE);

    PAL_Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View

    p_MAINSCRN_000 = LBXE_LoadSingle_LM(g_LbxNm_MAINSCRN, 0);   // TESTSCRN.LBX  320 x 40
    p_MAINSCRN_005 = LBXE_LoadSingle_LM(g_LbxNm_MAINSCRN, 5);   // TESTSCRN.LBX  320 x 160
    p_VORTEX_001 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 1);       // 
    p_VORTEX_002 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 2);       // 
    p_VORTEX_003 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 3);       // Quit To DOS
    p_VORTEX_004 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 4);       // 
    p_VORTEX_005 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 5);       // 

    // // FLIC_Load_Palette(fp_FLIC_File, current_frame_index);
    // FLIC_Load_Palette(p_MAINSCRN_000, 0);
    
    // FLIC_Draw_Back(0,0,0,p_VORTEX_005,video_back_buffer);



    VGA_SetModeY();
    // g_RSP_Idx = 0;
    // gsa_DSP_Addr = 0xA000;

    VGA_Set_DSP_Addr();
    // g_RSP_Idx = 0;
    // gsa_DSP_Addr = 0xA400;

//     Fill_Video_Back_Buffer(CYAN, video_back_buffer);
//     // Draw_Video_Back_Buffer(0, 0, 320, 200, video_back_buffer);
//     test_VBB_Pattern_Draw();
//     // VGA_PageFlip();
//     getch();
// 
//     Fill_VRAM(GREEN);
//     VGA_PageFlip();
//     getch();

    VGA_DrawFilledRect(0,0,319,199,MAGENTA);
    // getch();
    VGA_PageFlip();
    getch();

    FLIC_Draw(MenuArea_X_Left, MenuArea_Y_Top + 48, p_VORTEX_003);
    // getch();
    VGA_PageFlip();
    // getch();
    VGA_DAC_Write();
    getch();

    // FLIC_Draw_Back(32, 20, 0, p_VORTEX_003, video_back_buffer);
    // getch();
    // Draw_Video_Back_Buffer(0, 0, 320, 200, video_back_buffer);
    // getch();

    // Fill_Video_Back_Buffer(CYAN, video_back_buffer);
    // getch();
    // Draw_Video_Back_Buffer(0, 0, 320, 200, video_back_buffer);
    // getch();
    // VGA_PageFlip();
    // getch();

    // FLIC_Draw_Back(MenuArea_X_Left, MenuArea_Y_Top + 48, 0, p_VORTEX_003, video_back_buffer);
    // getch();

    FLIC_Draw_Back(32, 20, 0, p_VORTEX_003, video_back_buffer);
    // Draw_Video_Back_Buffer(0, 0, 320, 200, video_back_buffer);
    test_VBB_Pattern_Draw();
    // VGA_PageFlip();
    getch();

    FLIC_Draw_Back(0, 0, 0, p_MAINSCRN_005, video_back_buffer);
    test_VBB_Pattern_Draw();
    getch();

    FLIC_Draw_Back(0, 0, 0, p_MAINSCRN_005, video_back_buffer);
    FLIC_Draw_Back(0, 161, 0, p_MAINSCRN_000, video_back_buffer);
    FLIC_Draw_Back(12, 12, 0, p_VORTEX_003, video_back_buffer);  // q
    FLIC_Draw_Back(12, 24, 0, p_VORTEX_001, video_back_buffer);  // c
    FLIC_Draw_Back(12, 36, 0, p_VORTEX_002, video_back_buffer);  // h
    FLIC_Draw_Back(12, 48, 0, p_VORTEX_004, video_back_buffer);  // n
    FLIC_Draw_Back(12, 60, 0, p_VORTEX_005, video_back_buffer);  // l
    // Draw_Video_Back_Buffer(0, 0, 320, 200, video_back_buffer);
    test_VBB_Pattern_Draw();
    // VGA_PageFlip();
    getch();

    // Fill_VRAM(GREEN);  // How slow is it?!? It's Sofa-King Slow!!
    // VGA_PageFlip();
    // getch();


    VGA_SetTextMode();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_FLIC_Draw_Back()\n", __FILE__, __LINE__);
#endif

}

void test_FLIC_Draw_Frame_Back(void)
{
    int test_status;
    static struct s_FLIC_HDR PS_FLIC_Header;  // persistent, local
    int x;
    int y;
    int w;
    int current_frame_index;
    SAMB_ptr video_back_buffer;
    unsigned int dos_largest_memory_block;
    unsigned long int current_coreleft;
    unsigned long int current_farcoreleft;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_FLIC_Draw_Frame_Back()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    _BX = 0xFFFF;
    _AH = 0x48;             // ALLOCMEM
    geninterrupt(0x21);     // DOS_INT
    dos_largest_memory_block = _BX;  // BX = size in paras of the largest block of memory available
    dbg_prn("DEBUG: [%s, %d] dos_largest_memory_block PR: %u\n", __FILE__, __LINE__, dos_largest_memory_block);
    dbg_prn("DEBUG: [%s, %d] dos_largest_memory_block B: %u\n", __FILE__, __LINE__, (dos_largest_memory_block * 16));
    dbg_prn("DEBUG: [%s, %d] dos_largest_memory_block KB: %u\n", __FILE__, __LINE__, (dos_largest_memory_block * 16 / 64));
#endif
#ifdef STU_DEBUG
    current_coreleft = (unsigned long) coreleft();
    current_farcoreleft = (unsigned long) farcoreleft();
    dbg_prn("current_coreleft: %lu\n", current_coreleft);
    dbg_prn("current_farcoreleft: %lu\n", current_farcoreleft);
#endif
    // printf("The difference between the highest allocated block and\n");
    // printf("the top of the heap is: %lu bytes\n", (unsigned long) coreleft());
    // printf("The difference between the highest allocated block in the far\n");
    // printf("heap and the top of the far heap is: %lu bytes\n", farcoreleft());

    test_status = 0;  // TEST_UNDEFINED

    if(!g_Load_Font_File_tested) { test_Load_Font_File(); }
    if(!g_Load_Font_File_validated) { test_status = -1; }  // TEST_FAILURE

    PAL_Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View


    p_MAINSCRN_000 = LBXE_LoadSingle_LM(g_LbxNm_MAINSCRN, 0);
    // p_MAINSCRN_005 = LBXE_LoadSingle_LM(g_LbxNm_MAINSCRN, 5);
    p_VORTEX_001 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 1); // 70 paragraphs
    p_VORTEX_002 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 2);
    p_VORTEX_003 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 3);
    p_VORTEX_004 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 4);
    p_VORTEX_005 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 5);


    memcpy((void *)&PS_FLIC_Header, (const void *)p_VORTEX_005, sizeof(PS_FLIC_Header));

    // video_back_buffer = SA_Allocate_Space(4000);  // 4000 paragraphs = 64000 bytes / 16 bytes per paragraph

    current_frame_index = 0;
    x = 0;
    y = 0;


    // VGA_SetModeY();  // HERE: g_RSP_Idx = 0; gsa_DSP_Addr = 0xA000;
    // VGA_DrawFilledRect(0,0,319,199,5);
    // getch();

    // FLIC_Draw_Frame_Back(x, y, w, p_VORTEX_005, video_back_buffer);

    // SCREEN_Menu()  seg001.C, Line 540
    // seg020.C  void PAL_Load_Palette(int entry, int start_color, int end_color)
    PAL_Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View

    FLIC_Load_Palette(p_VORTEX_005, current_frame_index);



    // VGA_PageFlip();
    // getch();

    // VGA_DAC_Write();
    // getch();

    // FLIC_Load_Palette(MK_FP(sa_FLIC_Header,0), Frame_Index);  // s21p07
    // getch();

    // VGA_DAC_Write();
    // getch();
    
    // VGA_PageFlip();
    // getch();

    // // BAD!! STU_VGA_RAM_Dump_1();
    // STU_VGA_RAM_Dump();

    // VGA_SetTextMode();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_FLIC_Draw_EMM()\n", __FILE__, __LINE__);
#endif
}

void test_FLIC_Draw_Frame_2BMP(void)
{

}

void test_STU_Export_Palette_XBGR(void)
{
    Load_Font_File(GAME_FONT_FILE);
    PAL_Load_Palette(2,-1,0);
    STU_Export_Palette_XBGR();
}

void test_STU_Export_VBB_To_BMP(void)
{
//     int MenuArea_X_Left;
//     int MenuArea_Y_Top;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_STU_Export_VBB_To_BMP()\n", __FILE__, __LINE__);
#endif

    // Load_Font_File(GAME_FONT_FILE);
    // PAL_Load_Palette(2,-1,0);
    // VGA_DAC_Write();
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_FLIC_Draw_Back()\n", __FILE__, __LINE__);
#endif

//     MenuArea_X_Left = 123;
//     MenuArea_Y_Top = 141;

    video_back_buffer = SA_Allocate_MemBlk(4000);
    video_back_buffer_seg = FP_SEG(video_back_buffer);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] video_back_buffer: %p\n", __FILE__, __LINE__, video_back_buffer);
    dbg_prn("DEBUG: [%s, %d] video_back_buffer_seg: 0x%04X\n", __FILE__, __LINE__, video_back_buffer_seg);
#endif
    // memset(video_back_buffer, CYAN, 320*200);
    // STU_Export_VBB_To_BIN();
    // Load_Font_File(GAME_FONT_FILE);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] p_Palette_XBGR: %p\n", __FILE__, __LINE__, p_Palette_XBGR);
#endif
    // STU_Export_Palette_XBGR();
    // STU_Export_VBB_To_BMP();
    // STU_Export_VBB_To_BMP32();

    // Fill_Video_Back_Buffer(MAGENTA, video_back_buffer);
    // STU_Export_VBB_To_BMP();

    // Load_Font_File(GAME_FONT_FILE);
    // PAL_Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View
    // STU_Export_Palette_XBGR();


    // Load_Font_File(GAME_FONT_FILE);
    // PAL_Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View
    // p_MAINSCRN_000 = LBXE_LoadSingle_LM("TESTSCRN.LBX", 0);   // TESTSCRN.LBX  320 x 40
    // FLIC_Draw_Back(0, 0, 0, p_MAINSCRN_005, video_back_buffer);
    // STU_Export_Palette_XBGR();

    // Load_Font_File(GAME_FONT_FILE);
    // PAL_Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View
    // p_MAINSCRN_000 = LBXE_LoadSingle_LM("TESTSCRN.LBX", 0);   // TESTSCRN.LBX  320 x 40
    // p_MAINSCRN_005 = LBXE_LoadSingle_LM(g_LbxNm_MAINSCRN, 5);   // TESTSCRN.LBX  320 x 160
    // p_VORTEX_001 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 1);       // 
    // p_VORTEX_002 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 2);       // 
    // p_VORTEX_003 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 3);       // Quit To DOS
    // p_VORTEX_004 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 4);       // 
    // p_VORTEX_005 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 5);       // 
    // FLIC_Draw_Back(0, 0, 0, p_MAINSCRN_005, video_back_buffer);
    // FLIC_Draw_Back(0, 161, 0, p_MAINSCRN_000, video_back_buffer);
    // FLIC_Draw_Back(12, 12, 0, p_VORTEX_003, video_back_buffer);  // q
    // FLIC_Draw_Back(12, 24, 0, p_VORTEX_001, video_back_buffer);  // c
    // FLIC_Draw_Back(12, 36, 0, p_VORTEX_002, video_back_buffer);  // h
    // FLIC_Draw_Back(12, 48, 0, p_VORTEX_004, video_back_buffer);  // n
    // FLIC_Draw_Back(12, 60, 0, p_VORTEX_005, video_back_buffer);  // l
    // STU_Export_Palette_XBGR();
    // STU_Export_VBB_To_BIN();
    // STU_Export_VBB_To_BMP();
    // STU_Export_VBB_To_BMP32();




    VGA_SetModeY();      // g_RSP_Idx = 0;  gsa_DSP_Addr = 0xA000;
    VGA_Set_DSP_Addr();  // g_RSP_Idx = 0;  gsa_DSP_Addr = 0xA400;

    Load_Font_File(GAME_FONT_FILE);
    PAL_Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View
    p_MAINSCRN_000 = LBXE_LoadSingle_LM("TESTSCRN.LBX", 0);   // TESTSCRN.LBX  320 x 40
    p_MAINSCRN_005 = LBXE_LoadSingle_LM(g_LbxNm_MAINSCRN, 5);   // TESTSCRN.LBX  320 x 160
    p_VORTEX_001 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 1);       // 
    p_VORTEX_002 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 2);       // 
    p_VORTEX_003 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 3);       // Quit To DOS
    p_VORTEX_004 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 4);       // 
    p_VORTEX_005 = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 5);       // 
    FLIC_Draw_Back(0, 0, 0, p_MAINSCRN_005, video_back_buffer);
    FLIC_Draw_Back(0, 161, 0, p_MAINSCRN_000, video_back_buffer);
    FLIC_Draw_Back(12, 12, 0, p_VORTEX_003, video_back_buffer);  // q
    FLIC_Draw_Back(12, 24, 0, p_VORTEX_001, video_back_buffer);  // c
    FLIC_Draw_Back(12, 36, 0, p_VORTEX_002, video_back_buffer);  // h
    FLIC_Draw_Back(12, 48, 0, p_VORTEX_004, video_back_buffer);  // n
    FLIC_Draw_Back(12, 60, 0, p_VORTEX_005, video_back_buffer);  // l
    test_VBB_Pattern_Draw();
    getch();
    VGA_DAC_Write();
    getch();
    VGA_PageFlip();
    getch();
    STU_Export_Palette_XBGR();
    STU_Export_VBB_To_BIN();
    STU_Export_VBB_To_BMP();
    STU_Export_VBB_To_BMP32();


    VGA_SetTextMode();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_STU_Export_VBB_To_BMP()\n", __FILE__, __LINE__);
#endif
}

// s08p19  void SA_Alloc_Error(int SA_Error_Number, int blocks);
void test_SA_Error(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_SA_Error()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] Debug_Disabled: %d\n", __FILE__, __LINE__, Debug_Disabled);
#endif
    Debug_Disabled = 0;
    SA_Alloc_Error(1,111);
    Debug_Disabled = 1;
    SA_Alloc_Error(1,111);
    SA_Alloc_Error(2,111);
    SA_Alloc_Error(3,111);
    SA_Alloc_Error(4,111);
    // DNE SA_Alloc_Error(5,0);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_SA_Error()\n", __FILE__, __LINE__);
#endif
}
// s10p15  void LBX_Error(char * name, int LBX_Error_Number, int entry, int pages)
void test_LBX_Error(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_LBX_Error()\n", __FILE__, __LINE__);
#endif
    LBX_Error("MAINSCRN",  1, 0, 111);
    LBX_Error("MAINSCRN",  2, 0, 111);
    LBX_Error("MAINSCRN",  3, 0, 111);
    LBX_Error("MAINSCRN",  4, 0, 111);
    LBX_Error("MAINSCRN",  5, 0, 111);
    LBX_Error("MAINSCRN",  6, 0, 111);
    LBX_Error("MAINSCRN",  7, 0, 111);
    LBX_Error("MAINSCRN",  8, 0, 111);
    LBX_Error("MAINSCRN",  9, 0, 111);
    LBX_Error("MAINSCRN", 10, 0, 111);
    LBX_Error("MAINSCRN", 11, 0, 111);
    LBX_Error("MAINSCRN", 12, 0, 111);
    LBX_Error("MAINSCRN", 13, 0, 111);
    LBX_Error("MAINSCRN", 14, 0, 111);
    LBX_Error("MAINSCRN", 15, 0, 111);
    LBX_Error("MAINSCRN", 16, 0, 111);
    // DNE LBX_Error("MAINSCRN", 17, 0, 111);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_LBX_Error()\n", __FILE__, __LINE__);
#endif
}
