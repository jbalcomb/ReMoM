// seg001
// MGC_DEF.H


#include "ST_TYPE.H"
#include "ST_DEF.H"
#include "seg001.H"

#include "MOM_DEF.H"
#include "MGC_DEF.H"

#include "MoX_MoM.H"  /*  EMM_PAGES_REQUIRED, EMM_MIN_KB, RAM_MIN_KB, GAME_FONT_FILE; RAM_SetMinKB() */

#include "seg020.H"
#include "ST_CRSR.H"
#include "ST_CTRL.H"
#include "ST_EMM.H"
#include "MoX_EMM.H"  /* EMM_Pages_Reserved */
#include "MoX_EXIT.H"  /* Exit() */
#include "ST_FLIC.H"
#include "ST_GUI.H"
#include "ST_HLP.H"
#include "ST_LBX.H"
#include "ST_SA.H"
#include "MoX_SA.H" /* SA_MK_FP0() */
#include "ST_SCRN.H"
#include "ST_TXT.H"
#include "ST_VGA.H"

#include <STDIO.H>

// #ifdef STU_DEBUG
// #include "STU_DBG.H"
// #endif
// #ifdef TEST
// #include "STU_TST.H"
// #endif


#pragma warn -par  // Parameter parameter is never used.
// Warning: Parameter 'argc' is never ued in function MGC_Main
// Warning: Parameter 'argv' is never ued in function MGC_Main


// s01p01
/*
Unable to resolve configuration with compilerPath "J:/STU/DBWD/BORLANDC/BIN/BCC.EXE"
*/

/*
    ? Tell the Compiler that the Linker will find these later ?
*/
extern unsigned int EMM_PageFrame_Base_Address;   // dseg:40E4

/*
Hardware Settings
Game Settings
Sound, Music, and Speech Drivers and Settings
Saved Games
Initialize Hardware
Disable Debug
Initialize Tables
Set ESC Override
Load Palette, Set Palette
Play Intro (if !"JENNY")
Load Main Images
Load TerrStat
Prepare Credits
Clear Screen
Main Menu Music
Load Palette, Set Palette
Main Menu
*/

// int MGC_Main(void);
// 
// int main(void)
// {
//     Debug_Log_Startup();
// 
//     MGC_Main();
// 
//     Debug_Log_Shutdown();
//     return 0;
// }

// s01p01
//int main(int argc, char *argv[])
//int main(void)
int MGC_Main(int argc, char *argv[])
{
    char Temp_String[30];
    char File_Name[40];
    FILE * fileptr;
    //DBGLOG("DEBUG: [%s, %d] BEGIN: MGC_Main()\n", __FILE__, __LINE__);
    //TRACE(("message %d\n", 1));
    // dlvfprintf("DEBUG: [%s, %d] BEGIN: MGC_Main()\n", __FILE__, __LINE__);
    
    EMM_Pages_Reserved = EMM_PAGES_REQUIRED;

    /*
        CONFIG.MOM
    */
//    if(DOS_FileFind(cnst_ConfigFile, Temp_String) != 0)
//    {
//        Exit(cnst_ConfigErr);  // "Run INSTALL to configure MASTER OF MAGIC.",0Ah,0Ah,0
//    }
//    // LoadConfigMom()
//    fileptr = fopen(cnst_ConfigFile, "rb");  // CONFIG.MOM
//    fread(g_CFG_HW, 18, 1, fileptr);
//    fclose(fileptr);

    /*
        MAGIC.SET
    */
    //LoadMagicSet()
    // call j_GAME_LoadSettings             // either loads the settings from MAGIC.SET or, if the file is not found, resets them and creates it

    /*
        MIDI Driver
        DSP Driver
    */
    /*
        SAVE.GAM
        for ( itr_SaveGamNbr = 1; itr_SaveGamNbr < 9; itr_SaveGamNbr++ )
    */

    EMM_SetMinKB(EMM_MIN_KB);
    RAM_SetMinKB(RAM_MIN_KB);

    //Settings.InputType = 1;
    // _s14p01c.c
    //int __cdecl __far Hardware_Init(int Input_Type, int Snd_Chnls, char *FontFile, int M_Drv, int M_IO, int M_IRQ, int M_DMA, int D_Drv, int D_IO, int D_IRQ, int D_DMA)
    // Hardware_Init(1, 2, GAME_FONT_FILE, 0, 0, 0, 0, 0, 0, 0, 0);  // Defaults for 'No Sound'
    Hardware_Init(1, GAME_FONT_FILE);  // argInputType, argFontFileName
    /*
        |-> EMM_Startup()
        |-> VGA_SetModeY()
        |-> VGA_DAC_Init() |-> ... LBXE_LoadSingle(), SA_Allocate_MemBlk(); ... VGA_TextDraw_Init()
        |-> SND_Init()
        |-> IN_Init() |-> MD_Init()
        |-> RNG_TimerSeed()
        |-> VGA_Set_DSP_Addr()
    */

    DBG_Disable();

    /*
        j_LBX_Tables_Init();  // stub057
            |-> LBX_Tables_Init(6100);  // ovr057
    */
    // TODO(JimBalcomb,20220818): move whatever this 6100 is to a manifest constant
    MoM_Tables_Init(6100);
    
    IN_SetEscOverride();

    //VGA_LoadPalette(0, -1);  // argument missing in dasm
    VGA_LoadPalette(0, -1, 0);
    VGA_DAC_Write();

    /*
        if not "JENNY", then Play_Intro()
    */

    GAME_LoadMainImages();
    GAME_Load_TERRSTAT_0();
    GAME_Load_SPELLDAT_0();

    /*
        Main Menu
    */

    /*
        Main Menu - Credits
    */
    // //j_GAME_PrepareCredits()
    // GAME_PrepareCredits();


    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Draw Screen-Page
    VGA_SetDirectDraw();
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Render Screen-Page
    VGA_Set_DSP_Addr();

    /*
        SND_Stop_Music()
        MainMenu_Music_Seg = LBXE_LoadSingle("music", 104) // MUSIC_Main_Menu 68h
        if (Settings.BG_Music == 1)
        {
            SND_PlayFile(MainMenu_Music_Seg)
        }
    */

    //VGA_LoadPalette(0, -1);  // argument missing in dasm
    VGA_LoadPalette(0, -1, 0);  // EMPERATO
    VGA_DAC_Write();



    GAME_MainMenu();  // MGC_DEF.H  _s01p03c.c




    /*
        SAVEGAM_Save(8)
    */

    //GAME_QuitWithReport();
    Exit("Thank you for playing Master of Magic!$");


    //DBGLOG("DEBUG: [%s, %d] END: MGC_Main()\n", __FILE__, __LINE__);
    // dlvfprintf("DEBUG: [%s, %d] END: MGC_Main()\n", __FILE__, __LINE__);

    return 0;
}

// s01p03
void GAME_MainMenu(void)
{
    char Read_Buffer[30];
    int Screen_Action;
    FILE *fp;
    int flag_quit;

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: BEGIN: GAME_MainMenu()\n", __FILE__, __LINE__);
// #endif

    if ( DISK_FileFind(cnst_ConfigFile, Read_Buffer) == 0 )
    {
        Exit(cnst_ConfigErr);
    }
// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] Read_Buffer: %s\n", __FILE__, __LINE__, Read_Buffer);
// #endif

   fp = fopen(cnst_ConfigFile, cnst_RB);
   fread(Config_Data, 18, 1, fp);
   fclose(fp);


   g_ScreenChangeFade = 1;  // ST_TRUE
   g_GUI_MainScreenJump = -1;  // ST_UNDEFINED
   IN_SetEscOverride();


    // Loop While ? == 0
    // first run through always calls SCREEN_Menu()
    flag_quit = ST_FALSE;

    while ( flag_quit == ST_FALSE )
    {

        VGA_SetOutlineColor(0);
        HLP_ClearHelp();

        // HERE("Screen_Action = SCREEN_Menu();");
        Screen_Action = SCREEN_Menu();  // MGC_DEF.H  _s01p05c.c
        // DEBUG(JimBalcomb): here, the screen is rendered correctly - ? happens after input ?

// #ifdef STU_DEBUG
//         //Screen_Action = 3;  // DEBUG: default to `Quit To DOS`
//         dlvfprintf("DEBUG: [%s, %d] Screen_Action: %d\n", __FILE__, __LINE__, Screen_Action);
// #endif

        HLP_ClearHelp();
        g_ScreenChangeFade = ST_FALSE;

        switch (Screen_Action)
        {
            case 0:  // "Continue"

// #ifdef STU_DEBUG
//                 dlvfprintf("DEBUG: [%s, %d] Menu Event 0\n", __FILE__, __LINE__);
// #endif

                // SND_Stop_Music()
                // j_GAME_WizardsLaunch(8)  // e_SAVE9GAM
                GUI_LoadSave_State = ST_UNDEFINED;
                break;
            case 1:  // "Load Game"

// #ifdef STU_DEBUG
//                 dlvfprintf("DEBUG: [%s, %d] Menu Event 1\n", __FILE__, __LINE__);
// #endif

                // j_GAME_LoadSaveScreen
                break;
            case 2:  // "New Game"

// #ifdef STU_DEBUG
//                 dlvfprintf("DEBUG: [%s, %d] Menu Event 2\n", __FILE__, __LINE__);
// #endif

                // j_GAME_New_Create
                GUI_LoadSave_State = ST_UNDEFINED;
                break;
            case 3:  // "Quit To DOS"

// #ifdef STU_DEBUG
//                 dlvfprintf("DEBUG: [%s, %d] Menu Event 3\n", __FILE__, __LINE__);
// #endif

                flag_quit = ST_TRUE;
                GUI_LoadSave_State = ST_UNDEFINED;
                break;
            case 4:  // "Hall Of Fame"

// #ifdef STU_DEBUG
//                 dlvfprintf("DEBUG: [%s, %d] Menu Event 4\n", __FILE__, __LINE__);
// #endif

                // j_GAME_Hall_of_Fame
                // j_GAME_PrepareCredits
                // jmp  short $+2 ... somehow this jumps back to @@LoopWhileZero
                break;
        }
        
    }

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: END: GAME_MainMenu()\n", __FILE__, __LINE__);
// #endif

    Exit(cnst_QUIT_Message);
}

// s01p04
/*
    Only MAGIC.EXE?
    Loads MoM/Main Screen background and Buttons as well as New Game - Wizard's Portraits and Spell Data
    
    Load LBX File, into EMM, as 'Reserved'
        MAINSCRN.LBX
        WIZARDS.LBX
        SPELLDAT.LBX
    Load LBX Entry
        MAINSCRN.LBX, 0  "MOMBACK",  "MOM animated logo"
        MAINSCRN.LBX, 5  "CASTLE",   "MOM screen buttom"
        VORTEX.LBX,   1  "MAINTEXT,  "c"
        VORTEX.LBX,   2  "MAINTEXT,  "hof"
        VORTEX.LBX,   3  "MAINTEXT,  "q"
        VORTEX.LBX,   4  "MAINTEXT,  "n"
        VORTEX.LBX,   5  "MAINTEXT,  "l"
        WIZARDS.LBX,  0  "MERLIN",   ""
        WIZARDS.LBX,  1  "SHAMAN",   ""
        WIZARDS.LBX,  2  "PRIESTES", ""
        WIZARDS.LBX,  3  "WU",       ""
        WIZARDS.LBX,  4  "ARAB",     ""
        WIZARDS.LBX,  5  "OBERIC",   ""
        WIZARDS.LBX,  6  "WRAITH",   ""
        WIZARDS.LBX,  7  "DRACONIA", ""
        WIZARDS.LBX,  8  "EVILWIZ",  ""
        WIZARDS.LBX,  9  "FREYA",    ""
        WIZARDS.LBX, 10  "GALE",     ""
        WIZARDS.LBX, 11  "ARIEL",    ""
        WIZARDS.LBX, 12  "AZTEC",    ""
        WIZARDS.LBX, 13  "KARLA",    ""
*/
void GAME_LoadMainImages(void)
{

    int itr_wizards;

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: BEGIN: GAME_LoadMainImages()\n", __FILE__, __LINE__);
// #endif

    // HERE("BEFORE: EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);");
    EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);
    // HERE("AFTER: EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);");

//#ifdef TEST
    // DELETE HERE("CALL: dl_TST_LBX_MAINSCRN_000();");
    // DELETE dl_TST_LBX_MAINSCRN_000();

    //g_EMM_Table[TST_LBX_MAINSCRN_000.EMM_Table_Index].eEmmHndlNbr == TST_LBX_MAINSCRN_000.EMM_Handle_Number;

//#endif

    // TODO(JimBalcomb,20220724): add DEBUG - if MAINSCRN LbxFileSize == 196511
//#ifdef STU_DEBUG
//    dlvfprintf("DEBUG: [%s, %d] EmmLogicalPage: %d\n", __FILE__, __LINE__, EmmLogicalPage);
//    dlvfprintf("DEBUG: [%s, %d] LbxFileSize: %ld\n", __FILE__, __LINE__, LbxFileSize);
//    dlvfprintf("DEBUG: [%s, %d] UU_varNbytesRead: %ld\n", __FILE__, __LINE__, UU_varNbytesRead);
//    dlvfprintf("DEBUG: [%s, %d] tmp_LbxFileSize: %ld\n", __FILE__, __LINE__, tmp_LbxFileSize);
//    dlvfprintf("DEBUG: [%s, %d] UU_varNbytesRead + tmp_LbxFileSize: %ld\n", __FILE__, __LINE__, (UU_varNbytesRead + tmp_LbxFileSize));
//#endif

//#ifdef STU_DEBUG
//    EMM_Map4(EmmHndlNbr, 0);
//    dlvfprintf("DEBUG: [%s, %d] EMM_PageFrameBaseAddress: 0x%04X\n", __FILE__, __LINE__, EMM_PageFrameBaseAddress);
//    dlvfprintf("DEBUG: [%s, %d] LBX_EntryCount: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 0));
//    dlvfprintf("DEBUG: [%s, %d] LBX_MagSig_Hi: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 2));
//    dlvfprintf("DEBUG: [%s, %d] LBX_MagSig_Lo: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 4));
//    dlvfprintf("DEBUG: [%s, %d] LBX_Type: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 6));
//#endif

    /*
        In-Process
            Load LBX Entry
                LBX (File) Name
                    MAINSCRN.LBX
                LBX Entry Index
                    0
            SAMB Memory Allocation Type / Strategy
                Type 0: standalone block, no header
            Label / Identifier / Variable for Address/Pointer:
                sa_MAINSCRN_000


    */
   /*
        as coded, returns a SAMB_data FLIC_Header
        LBXE_LoadSingle() |-> LBX_Load_Entry() |-> EMM_Load_LBX_Entry() |-> EMM_LBX_FLIC_header()
   */
    gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);

// #ifdef STU_DEBUG
//     // DBG_MAINSCRN_000 = gsa_MAINSCRN_0_AnimatedLogo;
//     TST_LBX_MAINSCRN_000.Segment_Address = gsa_MAINSCRN_0_AnimatedLogo;
//     // dlvfprintf("DEBUG: [%s, %d] DBG_MAINSCRN_000: 0x%04X\n", __FILE__, __LINE__, DBG_MAINSCRN_000);
//     dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.Segment_Address: 0x%04X\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.Segment_Address);
// #endif

    gsa_MAINSCRN_5_ScreenBottom = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 5);

// #ifdef STU_DEBUG
//     // DBG_MAINSCRN_005 = gsa_MAINSCRN_5_ScreenBottom;
//     // dlvfprintf("DEBUG: [%s, %d] DBG_MAINSCRN_005: 0x%04X\n", __FILE__, __LINE__, DBG_MAINSCRN_005);
// #endif

    gsa_VORTEX_1_MenuContinue    = LBXE_LoadSingle(g_LbxNm_VORTEX, 1);
    gsa_VORTEX_2_MenuHallOfFame  = LBXE_LoadSingle(g_LbxNm_VORTEX, 2);
    gsa_VORTEX_3_MenuQuitToDOS   = LBXE_LoadSingle(g_LbxNm_VORTEX, 3);
    gsa_VORTEX_4_MenuNewGame     = LBXE_LoadSingle(g_LbxNm_VORTEX, 4);
    gsa_VORTEX_5_MenuLoadGame    = LBXE_LoadSingle(g_LbxNm_VORTEX, 5);

    // TODO(JimBalcomb,20220915): figure out where these end up getting used (WZD?) and if that makes it make sense that these are loaded here
    // NOTE(JimBalcomb,20220919): the WIZARDS and SPELLDAT here are used in the 'New Game' code  (SEE: SCRN_Draw_NewScr5_2)
    // gsa_WIZARDS_0to13 is used in SCRN_New_ 1,2,3,4,5_2,6,7
    EMM_Load_LBX_File_1(g_LbxNm_WIZARDS);
    
    for ( itr_wizards = 0; itr_wizards < 14; itr_wizards++ )
    {
        gsa_WIZARDS_0to13[itr_wizards] = LBXE_LoadSingle(g_LbxNm_WIZARDS, itr_wizards);
    }

    EMM_Load_LBX_File_1(g_LbxNm_SPELLDAT);

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: END: GAME_LoadMainImages()\n", __FILE__, __LINE__);
// #endif

}

// s01p05
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

// #ifdef STU_DEBUG
//     unsigned int dbg_Loop_Once;
// #endif

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] BEGIN: SCREEN_Menu()\n", __FILE__, __LINE__);
// #endif

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
    VGA_Set_DSP_Addr();

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

    CTRL_Clear();  // ? 1oom :: uiobj_table_clear(); ?

    Continue_Move_Down = ST_FALSE;

    if ( g_GAME_HaveSaves == 0 )
    {
        GUI_Load_Lbl_Index = -1000;  // 1oom :: oi_loadgame = UIOBJI_INVALID;
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

    // 1oom :: if (d->have_continue) { oi_continue = uiobj_add_mousearea(0x3c, 0x7f, 0x104, 0x8e, MOO_KEY_UNKNOWN); } else { oi_continue = UIOBJI_INVALID; }
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

// #ifdef STU_DEBUG
// /*    
//     GUI_Load_Lbl_Index
//     GUI_Continue_Label
//     GUI_NewGame_Label
//     GUI_HoF_Lbl_Index
//     GUI_Quit_Lbl_Index
// 
//     Continue_Hotkey_Index
//     Load_Hotkey_Index
//     NewGame_Hotkey_Index
//     HallofFame_Hotkey_Index
//     Quit_Hotkey_Index
//     Escape_Hotkey_Index
// */
//     dlvfprintf("DEBUG: [%s, %d] GUI_Load_Lbl_Index: %u\n", __FILE__, __LINE__, GUI_Load_Lbl_Index);
//     dlvfprintf("DEBUG: [%s, %d] GUI_Continue_Label: %u\n", __FILE__, __LINE__, GUI_Continue_Label);
//     dlvfprintf("DEBUG: [%s, %d] GUI_NewGame_Label: %u\n", __FILE__, __LINE__, GUI_NewGame_Label);
//     dlvfprintf("DEBUG: [%s, %d] GUI_HoF_Lbl_Index: %u\n", __FILE__, __LINE__, GUI_HoF_Lbl_Index);
//     dlvfprintf("DEBUG: [%s, %d] GUI_Quit_Lbl_Index: %u\n", __FILE__, __LINE__, GUI_Quit_Lbl_Index);
//     dlvfprintf("DEBUG: [%s, %d] Continue_Hotkey_Index: %u\n", __FILE__, __LINE__, Continue_Hotkey_Index);
//     dlvfprintf("DEBUG: [%s, %d] Load_Hotkey_Index: %u\n", __FILE__, __LINE__, Load_Hotkey_Index);
//     dlvfprintf("DEBUG: [%s, %d] NewGame_Hotkey_Index: %u\n", __FILE__, __LINE__, NewGame_Hotkey_Index);
//     dlvfprintf("DEBUG: [%s, %d] HallofFame_Hotkey_Index: %u\n", __FILE__, __LINE__, HallofFame_Hotkey_Index);
//     dlvfprintf("DEBUG: [%s, %d] Quit_Hotkey_Index: %u\n", __FILE__, __LINE__, Quit_Hotkey_Index);
//     dlvfprintf("DEBUG: [%s, %d] Escape_Hotkey_Index: %u\n", __FILE__, __LINE__, Escape_Hotkey_Index);
// #endif

    GUI_SetWindows(1, g_GUI_MainMenuWindow);

    // TODO(JimBalcomb,20220723): figure out if this SaveCursorArea is just because of the subsequent MouseEmuMoveTo
    CRL_Save_RSP(MD_GetX(), MD_GetY());

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
    // // call SCRN_Set_Redraw_Function
    // c_sgmt = FP_SEG(SCREEN_Menu_Draw);
    // c_ofst = FP_OFF(SCREEN_Menu_Draw);
    // fxnptr_ScreenDraw = (void _FAR *) MK_FP(c_sgmt,c_ofst);
    // SCRN_Set_Redraw_Function(fxnptr_ScreenDraw, 2);
    SCRN_Set_Redraw_Function(SCREEN_Menu_Draw, 2);

    // GUI_SetHelp(HLP_IDX_0, 5);

    IN_Set_Skip(4);


    while ( flag_done == ST_FALSE )
    {
        CLK_SaveCounter();  // 1oom :: ui_delay_prepare()


        // ST_GUI.H  _s34p66c.c  int IN_GetInput(void)
        input_control_index = IN_GetInput();  // _s34p66c.c  1oom :: oi1 = uiobj_handle_input_cond();

// #ifdef STU_DEBUG
//         dlvfprintf("DEBUG: [%s, %d] input_control_index: %d\n", __FILE__, __LINE__, input_control_index);
// #endif

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
            SCRN_SimplePageFlip();  // 1oom :: uiobj_finish_frame();
            // Research(JimBalcomb): 2022017: Should not the screen now be drawn and rendered?
            // ...the Screen-Page Index and Address are indeed be updated properly

            // 1oom :: if ((ui_draw_finish_mode != 0) && !flag_fadein) { ui_palette_fadein_4b_19_1(); flag_fadein = true; }
            if ( !((g_ScreenChangeFade == ST_FALSE) & (First_Draw_Done != ST_FALSE)) )
            {
                // HERE("( !((g_ScreenChangeFade == ST_FALSE) && (First_Draw_Done != ST_FALSE)) )");
                // j_VGA_Fade_In()
                VGA_Copy_RSP_DSP();  // Research(JimBalcomb)): 20220117: what does this accomplish?
                First_Draw_Done = ST_TRUE;
                g_ScreenChangeFade = ST_FALSE;
            }
            else
            {
                // HERE("( !((g_ScreenChangeFade != ST_FALSE) || (First_Draw_Done == ST_FALSE)) )");
            }

            CLK_Wait(2);  // 1oom :: ui_delay_ticks_or_click(2);
        }

    }  /* while ( flag_done == ST_FALSE ) */

    SCRN_DisableRedraw();  // 1oom :: uiobj_unset_callback();

    HLP_ClearHelp();

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] END: SCREEN_Menu()\n", __FILE__, __LINE__);
// #endif

    return g_GUI_MainMenuSelected;

// #ifdef STU_DEBUG
//     return 3;  // DEBUG: g_GUI_MainMenuSelected = ( (Quit_Hotkey_Index) || (GUI_Quit_Lbl_Index) || (Escape_Hotkey_Index) )
// #endif
}

// s01p06
void SCREEN_Menu_Draw(void)
{
    int Title_Frame_Index;
    int Loop_Var;
    int Continue_Move_Down;
    int MouseOver_ControlIndex;
    int MenuArea_X_Left;
    int MenuArea_Y_Top;

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] BEGIN: SCREEN_Menu_Draw()\n", __FILE__, __LINE__);
// #endif

    MenuArea_X_Left = 123;
    MenuArea_Y_Top = 141;

    // HERE("GUI_MousedControl()");
    MouseOver_ControlIndex = GUI_MousedControl();

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] MouseOver_ControlIndex: %d\n", __FILE__, __LINE__, MouseOver_ControlIndex);
// #endif

    // HERE("VGA_Set_DSP_Addr();");
    VGA_Set_DSP_Addr();
    // HERE("VGA_DrawFilledRect(0, 0, 319, 199, 0);");
    VGA_DrawFilledRect(0, 0, 319, 199, 0);

    // HERE("Title_Frame_Index = FLIC_GetCurFrame(gsa_MAINSCRN_0_AnimatedLogo);");
    Title_Frame_Index = FLIC_GetCurFrame(gsa_MAINSCRN_0_AnimatedLogo);

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] Title_Frame_Index: %d\n", __FILE__, __LINE__, Title_Frame_Index);
// #endif

    // HERE("FLIC_SetFrame(gsa_MAINSCRN_0_AnimatedLogo, 0);");
    FLIC_SetFrame(gsa_MAINSCRN_0_AnimatedLogo, 0);

    // TODO(JimBalcomb): figure out and fix the logic for looping the 20 frames of the "MOM animated logo"
    for ( Loop_Var = 0; Loop_Var <= Title_Frame_Index; Loop_Var++ )
    {

//         #ifdef STU_DEBUG
//             dlvfprintf("DEBUG: [%s, %d] Loop_Var: %d, Title_Frame_Index: %d, (Loop_Var <= Title_Frame_Index): %d\n", __FILE__, __LINE__, Loop_Var, Title_Frame_Index, (Loop_Var <= Title_Frame_Index));
//         #endif

        // HERE("FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);");
        FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);  // NOTE(JimBalcomb): This is the first call to FLIC_Draw_XY()
    }
    //HERE("FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);");
    //FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);  // NOTE(JimBalcomb): This is the first call to FLIC_Draw_XY()
    // NOTE(JimBalcomb): this LBX Entry/FLIC includes a 256-color palette chunk

    // HERE("FLIC_Draw_XY(0, 41, gsa_MAINSCRN_5_ScreenBottom);");
    FLIC_Draw_XY(0, 41, gsa_MAINSCRN_5_ScreenBottom);

    if ( g_ScreenChangeFade == 0 )
    {
        // j_GAME_DrawCredits()
    }

    /*
        Draw Buttons
            NOTE: uses VORTEX, not MAINSCRN
    */
    Continue_Move_Down = 0;
    if ( g_GAME_HaveSaves == 0 )
    {
        Continue_Move_Down = 1;
    }
    else
    {
        if ( MouseOver_ControlIndex == GUI_Load_Lbl_Index )
            FLIC_ResetFrame(gsa_VORTEX_5_MenuLoadGame);
        else
            FLIC_SetFrame(gsa_VORTEX_5_MenuLoadGame, 1);
        FLIC_Draw_XY(MenuArea_X_Left, (MenuArea_Y_Top + 12), gsa_VORTEX_5_MenuLoadGame);
    }

    if ( g_GAME_HaveContSave != 0 )
    {
        if ( MouseOver_ControlIndex == GUI_Continue_Label )
            FLIC_ResetFrame(gsa_VORTEX_1_MenuContinue);
        else
            FLIC_SetFrame(gsa_VORTEX_1_MenuContinue, 1);
        FLIC_Draw_XY(MenuArea_X_Left, (MenuArea_Y_Top + (12 * Continue_Move_Down)), gsa_VORTEX_1_MenuContinue);
    }

    if ( MouseOver_ControlIndex == GUI_NewGame_Label )
        FLIC_ResetFrame(gsa_VORTEX_4_MenuNewGame);
    else
        FLIC_SetFrame(gsa_VORTEX_4_MenuNewGame, 1);
    FLIC_Draw_XY(MenuArea_X_Left, (MenuArea_Y_Top + 24), gsa_VORTEX_4_MenuNewGame);

    if ( MouseOver_ControlIndex == GUI_HoF_Lbl_Index )
        FLIC_ResetFrame(gsa_VORTEX_2_MenuHallOfFame);
    else
        FLIC_SetFrame(gsa_VORTEX_2_MenuHallOfFame, 1);
    FLIC_Draw_XY(MenuArea_X_Left, (MenuArea_Y_Top + 36), gsa_VORTEX_2_MenuHallOfFame);

    if ( MouseOver_ControlIndex == GUI_Quit_Lbl_Index )
        FLIC_ResetFrame(gsa_VORTEX_3_MenuQuitToDOS);
    else
        FLIC_SetFrame(gsa_VORTEX_3_MenuQuitToDOS, 1);
    FLIC_Draw_XY(MenuArea_X_Left, (MenuArea_Y_Top + 48), gsa_VORTEX_3_MenuQuitToDOS);


    // // VGA_LoadPalette(2, -1, 0);
    // // VGA_DAC_Write();
    // // STU_VGA_DAC_Dump("MENUDAC.BIN");
    // // //VGA_Set_DSP_Addr();
    // // VGA_PageFlip();
    // // DEBUG(JimBalcomb): doing all of this here does make the screen render correctly
    // VGA_LoadPalette(2, -1, 0);
    // VGA_DAC_Write();
    // DEBUG(JimBalcomb): doing just these two here does make the screen render correctly
    VGA_DAC_Write();
    // DEBUG(JimBalcomb): just VGA_DAC_Write() here does make the screen render correctly
    // so, ...  FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo) has 256-clor custom palette...
    ///... and, calls FLIC_LoadPalette() to load that
    // ... and, FLIC_LoadPalette() sets the palette update flags
    // so, VGA_DAC_Write() needs to be called to /apply/ the palette
    // the calls to VGA_DAC_Write() are in GUI_Redraw_WaitOne() and GUI_Redraw_WaitTimer()
    // the calls to GUI_Redraw_WaitOne() and GUI_Redraw_WaitTimer() are in GUI_ProcessInput()


    // mov  ax, [CRP_Unused_Anim_Var]
    // inc  ax
    // mov  bx, 20
    // cwd
    // idiv bx
    // mov  [CRP_Unused_Anim_Var], dx

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] END: SCREEN_Menu_Draw()\n", __FILE__, __LINE__);
// #endif
}

// s01p07
void GAME_Load_TERRSTAT_0(void)
{
    int LbxEntryIndex = 0;
    int RecFirst = 0;
    int RecCount = 770;
    int RecSize = 6;

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: BEGIN: GAME_Load_TERRSTAT_0()\n", __FILE__, __LINE__);
// #endif

    gfp_TBL_Moves_Per_Tile = (SAMB_ptr) MK_FP(LBXR_LoadSingle(g_LbxNm_TERRSTAT, LbxEntryIndex, RecFirst, RecCount, RecSize),0);

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: END: GAME_Load_TERRSTAT_0()\n", __FILE__, __LINE__);
// #endif
}

// s01p08
void GAME_Load_SPELLDAT_0(void)
{
    int LbxEntryIndex = 0;
    int RecFirst = 0;
    int RecCount = 215;
    int RecSize = 36;

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: BEGIN: GAME_Load_SPELLDAT_0()\n", __FILE__, __LINE__);
// #endif

    gfp_TBL_Spell_Data = (SAMB_ptr) MK_FP(LBXR_LoadSingle(g_LbxNm_SPELLDAT, LbxEntryIndex, RecFirst, RecCount, RecSize),0);

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: BEGIN: GAME_Load_SPELLDAT_0()\n", __FILE__, __LINE__);
// #endif
}

