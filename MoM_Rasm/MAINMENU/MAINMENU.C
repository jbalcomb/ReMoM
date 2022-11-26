
#include "MAINMENU.H"

#include "MOM_DEF.H"
#include "MGC_DEF.H"    /* g_GUI_MainMenuWindow */

#include "MoX_DIR.H"    /* DIR() */

#include "ST_CRSR.H"
#include "ST_CTRL.H"
#include "ST_FLIC.H"    /* FLIC_Draw()) */
#include "seg028.H"     /* FLIC_Get_CurrentFrame(), FLIC_Reset_CurrentFrame(), FLIC_Set_CurrentFrame() */
#include "ST_GUI.H"     /* MD_CDraw_Disable(), MD_CDraw_Restore() */
#include "ST_LBX.H"
#include "ST_SCRN.H"    /* SCRN_Set_Redraw_Function() */
#include "ST_TXT.H"     /* Set_Outline_Color() */
#include "ST_VGA.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


#pragma warn -aus   // Identifier is assigned a value that is never used.
/*
    Main_Menu_Add_fields()
        field_idex_hotkey_*
*/
#pragma warn -rpt  // Nonportable pointer conversion.
/*
    Main_Menu_Load_pictures()
        mainmenu_top = LBX_Load(mainscrn_lbx_file, 0);
        mainmenu_bot = LBX_Load(mainscrn_lbx_file, 5);
        mainmenu_c   = LBX_Load(vortex_lbx_file, 1);
        mainmenu_h   = LBX_Load(vortex_lbx_file, 2);
        mainmenu_q   = LBX_Load(vortex_lbx_file, 3);
        mainmenu_n   = LBX_Load(vortex_lbx_file, 4);
        mainmenu_l   = LBX_Load(vortex_lbx_file, 5);
*/



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

// MGC  dseg:2077  cnst_ZeroString_4 db 0
// used as key code for all of the calls to GUI_CreateClickLabel()

/*
    Initialized Data
*/

struct s_GUI_WINDOW g_GUI_MainMenuWindow[1] = {1, 0, 0, 0, 319, 199}; // dseg:205E

char mainscrn_lbx_file[] = "MAINSCRN";          // MGC  dseg:28A7
char vortex_lbx_file[] = "VORTEX";              // MGC  dseg:28B0

unsigned char * mainmenu_top;  // MAINSCRN.LBX, 0
unsigned char * mainmenu_bot;  // MAINSCRN.LBX, 5
unsigned char * mainmenu_c;  // VORTEX.LBX, 1  "Continue"
unsigned char * mainmenu_h;  // VORTEX.LBX, 2  "Hall Of Fame"
unsigned char * mainmenu_q;  // VORTEX.LBX, 3  "Quit To DOS"
unsigned char * mainmenu_n;  // VORTEX.LBX, 4  "New Game"
unsigned char * mainmenu_l;  // VORTEX.LBX, 5  "Load Game"


// MGC  dseg:206C  CRP_Unused_Anim_Var dw 0

// MGC  dseg:27FF  cnst_ConfigFile db 'CONFIG.MOM',0

// MGC  dseg:280A  cnst_ConfigErr db 'Run INSTALL to configure MASTER OF MAGIC.',0Ah

// MGC  dseg:2859  cnst_SAVE db 'SAVE',0
char save_file_name[] = "SAVE";

// MGC  dseg:285E  cnst_SAVE_ext db '.GAM',0
char save_file_extension[] = ".GAM";

// MGC  dseg:2880
char quit_message[] = "Thank you for playing Master of Magic!";

// dseg:28AE 4E 00                   cnst_HOTKEY_N db 'N',0
// dseg:28C8 43 00                   cnst_HOTKEY_C db 'C',0
// dseg:28CA 4C 00                   cnst_HOTKEY_L db 'L',0
// dseg:28CC 48 00                   cnst_HOTKEY_H db 'H',0
// dseg:28CE 51 00                   cnst_HOTKEY_Q db 'Q',0
// dseg:28D0 1B 00                   cnst_HOTKEY_Esc db 1Bh,0
char hotkey_N = 'N';
char hotkey_C = 'C';
char hotkey_L = 'L';
char hotkey_H = 'H';
char hotkey_Q = 'Q';
char hotkey_ESC = 27;

// MGC  dseg:290A  GUI_LoadSave_State dw -1                ; 3 indicates returning from the settings screen
int GUI_LoadSave_State = -1;

// dseg:52B6 00 00                   GUI_Load_Lbl_Index dw 0
// dseg:52BC 00 00                   GUI_HoF_Lbl_Index dw 0
// dseg:52C0 00 00                   GUI_Quit_Lbl_Index dw 0
// dseg:52EA 00 00                   g_GUI_NewGame_Label dw 0
// dseg:52EE 00 00                   GUI_Continue_Label dw 0
signed int field_idx_click_L;
signed int field_idx_click_H;
signed int field_idx_click_Q;
signed int field_idx_click_N;
signed int field_idx_click_C;


// MGC  dseg:52C4  main_menu_selection dw 0
// {0: continue, 1: load, 2: new, 3: quit, 4: HoF}
// holds the mouse focus control instead during the redraw functions of other screens
int main_menu_selection = 0;

// MGC  dseg:52E4  save_exists dw 0
// OXRB:Menu,MenuDraw.:.static?module-scoped?private?
int save_exists = 0;

// MGC  dseg:52F2  save9_exists dw 0
// OXRB:Menu,MenuDraw.:.static?module-scoped?private?
int save9_exists = 0;

// MGC  dseg:52FC  main_menu_jump dw 0
int main_menu_jump = 0;

// MGC  dseg:52FE  save_gam_count dw 0
int save_gam_count = 0;

// MGC  dseg:5300  save_gams dw 8 dup(0)
unsigned char save_gams[8];

// MGC  dseg:64AE  screen_fade dw 0
int screen_fade = 0;

// MGC  dseg:87A4  g_CFG_HW CONFIG_DATA <0>
// CONFIG_DATA g_CFG_HW;

// dseg:205E                         ; GUI_WINDOW g_GUI_MainMenuWindow
// MGC  dseg:205E   01 00 00 00 00 00 00 00 g_GUI_MainMenuWindow GUI_WINDOW <Crsr_Normal, 0, 0, 0, 319, 199>
// // MGC_DEF.H
// extern struct s_GUI_WINDOW g_GUI_MainMenuWindow[]; // dseg:205E
// // MGC_DEF.C
// // dseg:205E GUI_MainMenuWindow GUI_WINDOW <Crsr_Normal, 0, 0, 0, 319, 199>
// //struct s_GUI_WINDOW g_GUI_MainMenuWindow[1] = {1, 0, 0, 319, 199, 0};    // dseg:205E
// // dseg:206A align 4
// ...
// struct s_GUI_WINDOW g_GUI_MainMenuWindow[1] = {1, 0, 0, 0, 319, 199}; // dseg:205E
// struct s_GUI_WINDOW main_menu_window[1] = {1, 0, 0, 0, 319, 199};

// ST_GUI.H  struct s_GUI_WINDOW
// sizeof 0Ch 12d (6 values * 2 bytes)
// ST_GUI  struct default_mouse_list {
// ST_GUI      signed short image_num;                               // ; enum Cursor_Types
// ST_GUI      signed short center_offset;  // ? drake178:"Offset", but maybe "HotSpot" ?
// ST_GUI      signed short x1;
// ST_GUI      signed short y1;
// ST_GUI      signed short x2;
// ST_GUI      signed short y2;
// ST_GUI  };
// TODO  struct default_mouse_list main_menu_window = {1, 0, 0, 0, 319, 199};
// MoO2
// struct (12 bytes) default_mouse_list
// Address: 02:0017C522
// signed integer (2 bytes) image_num
// signed integer (2 bytes) center_offset
// signed integer (2 bytes) x1
// signed integer (2 bytes) y1
// signed integer (2 bytes) x2
// signed integer (2 bytes) y2
// ...
// current_mouse_list
//     pointer (4 bytes)
// Address: 02:001B1204
// ...
// current_mouse_list_count
//     signed integer (2 bytes)
// Address: 02:001B1224
// ...
// Set_Mouse_List
//     Address: 01:00123E6C
//     Num params: 2
//     Return type: void (1 bytes) 
//         pointer (4 bytes) list
//         signed integer (2 bytes) count






// MAGIC.EXE  s01p04
/*
    Loads MoM/Main Screen background and Buttons
    
    Load LBX File, into EMM, as 'Reserved'
        MAINSCRN.LBX
    Load LBX Entry
        MAINSCRN.LBX, 0  "MOMBACK",  "MOM animated logo"
        MAINSCRN.LBX, 5  "CASTLE",   "MOM screen buttom"
        VORTEX.LBX,   1  "MAINTEXT,  "c"
        VORTEX.LBX,   2  "MAINTEXT,  "hof"
        VORTEX.LBX,   3  "MAINTEXT,  "q"
        VORTEX.LBX,   4  "MAINTEXT,  "n"
        VORTEX.LBX,   5  "MAINTEXT,  "l"
*/
// void GAME_LoadMainImages(void)
void Main_Menu_Load_Pictures(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Menu_Load_Pictures()\n", __FILE__, __LINE__);
#endif

// #if defined(__DOS16__)
    EMM_Load_LBX_File_1(mainscrn_lbx_file);
// #endif

    // Nonportable pointer conversion ...
    // TODO(JimBalcomb,20221124): move ST LBXE_LoadSingle_LM() to MoX LBX_Load()
    mainmenu_top = LBXE_LoadSingle_LM(mainscrn_lbx_file, 0);
    mainmenu_bot = LBXE_LoadSingle_LM(mainscrn_lbx_file, 5);
    mainmenu_c   = LBXE_LoadSingle_LM(vortex_lbx_file, 1);
    mainmenu_h   = LBXE_LoadSingle_LM(vortex_lbx_file, 2);
    mainmenu_q   = LBXE_LoadSingle_LM(vortex_lbx_file, 3);
    mainmenu_n   = LBXE_LoadSingle_LM(vortex_lbx_file, 4);
    mainmenu_l   = LBXE_LoadSingle_LM(vortex_lbx_file, 5);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Main_Menu_Load_Pictures()\n", __FILE__, __LINE__);
#endif

}


// s01p03
// void GAME_MainMenu(void)
void Main_Menu_Screen_Control(void)
{
    // char Read_Buffer[30];
    int Screen_Action;
    // FILE *fp;
    int flag_quit;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Main_Menu_Screen_Control()\n", __FILE__, __LINE__);
#endif

    // Load_CONFIG_MOM();

    screen_fade = ST_TRUE;
    main_menu_jump = ST_UNDEFINED;
    IN_SetEscOverride();  // Set_Global_Esc();

    flag_quit = ST_FALSE;
    while(flag_quit == ST_FALSE)
    {
        VGA_SetOutlineColor(0);  // Set_Outline_Color(0);
        HLP_ClearHelp();  // Deactivate_Help_List();
        Screen_Action = Main_Menu_Screen();  // MGC s01p05
        HLP_ClearHelp();  // Deactivate_Help_List();
        screen_fade = ST_FALSE;
        switch(Screen_Action)
        {
            case 0:  // "Continue"
                // SND_Stop_Music()
                // j_GAME_WizardsLaunch(8)  // e_SAVE9GAM
                GUI_LoadSave_State = ST_UNDEFINED;
                break;
            case 1:  // "Load Game"
                // j_GAME_LoadSaveScreen
                break;
            case 2:  // "New Game"
                // j_GAME_New_Create
                GUI_LoadSave_State = ST_UNDEFINED;
                break;
            case 3:  // "Quit To DOS"
                flag_quit = ST_TRUE;
                GUI_LoadSave_State = ST_UNDEFINED;
                break;
            case 4:  // "Hall Of Fame"
                // j_GAME_Hall_of_Fame
                // j_GAME_PrepareCredits
                // jmp  short $+2 ... somehow this jumps back to @@LoopWhileZero
                break;
        }
        
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: GAME_MainMenu()\n", __FILE__, __LINE__);
#endif

    Exit(quit_message);  // Exit_With_Message(quit_message);
}

// MGC s01p05
void Main_Menu_Add_Fields(void)
{
    int field_idx_hotkey_C;
    int field_idx_hotkey_L;
    int field_idx_hotkey_N;
    int field_idx_hotkey_H;
    int field_idx_hotkey_Q;
    int field_idx_hotkey_ESC;
    char Found_File_Name[30];
    char Conversion_String[30];
    char File_Name[30];
    int menu_shift;
    int itr_save_gams;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Main_Menu_Add_Fields()\n", __FILE__, __LINE__);
#endif

    // ? macro ? ~ Clear_Help()
    // // HLP_Entry_Table.Entry_Index = -1;
    // HLP_IDX_0 = -1;
    // HLP_IDX_1 = -1;
    // HLP_IDX_2 = -1;
    // HLP_IDX_3 = -1;
    // HLP_IDX_4 = -1;

    // ~ MoO2 Check_For_Saved_Games()
    save_gam_count = 0;
    for(itr_save_gams = 1; itr_save_gams < 10; itr_save_gams++)
    {
        strcpy(File_Name, save_file_name);
        itoa(itr_save_gams, Conversion_String, 10);
        strcat(File_Name, Conversion_String);
        strcat(File_Name, save_file_extension);
        // if(DIR(match_string, found_file) == 0)
        if(DIR(File_Name, Found_File_Name) == 0)
        {
            save_gams[save_gam_count] = ST_UNDEFINED;
        }
        else
        {
            if(itr_save_gams < 9)
            {
                save_gams[save_gam_count] = itr_save_gams - 1;
                save_gam_count++;
            }
            else
            {
                save9_exists = ST_TRUE;
            }
        }
    }
    if(save_gam_count > 0)
    {
        save_exists = ST_TRUE;
    }

    CTRL_Clear();  // Clear_Fields();

    menu_shift = 0;

    if(save_exists == ST_FALSE)
    {
        field_idx_click_L = -1000;  // INVALID
        menu_shift = 1;
    }
    else
    {
        field_idx_click_L = CTRL_CreateClickLabel(108, 150, 211, 161, 0, -1);
        // HLP_IDX_0 = 629; // HLP_LOAD
        // HLP_IDX_0_X1 = 108;
        // HLP_IDX_0_Y1 = 148;
        // HLP_IDX_0_X2 = 211;
        // HLP_IDX_0_Y2 = 162;
    }

    if(save9_exists == ST_FALSE)
    {
        field_idx_click_C = -1000;  // INVALID
    }
    else
    {
        field_idx_click_C = CTRL_CreateClickLabel(108, (138 + (12 * menu_shift)), 211, (149 + (12 * menu_shift)), 0, -1);
        // HLP_IDX_1 = 628; // HLP_CONTINUE
        // HLP_IDX_1_X1 = 108;
        // HLP_IDX_1_Y1 = (138 + (12 * menu_shift));
        // HLP_IDX_1_X2 = 211;
        // HLP_IDX_1_Y2 = (149 + (12 * menu_shift));
    }

    field_idx_click_N = CTRL_CreateClickLabel(108, 162, 211, 173, 0, -1);
    // HLP_IDX_2 = 630; // HLP_NEWGAME
    // HLP_IDX_2_X1 = 108;
    // HLP_IDX_2_Y1 = 162;
    // HLP_IDX_2_X2 = 211;
    // HLP_IDX_2_Y2 = 173;

    field_idx_click_H = CTRL_CreateClickLabel(108, 174, 211, 185, 0, -1);
    // HLP_IDX_3 = 798; // HLP_HALLOFFAME
    // HLP_IDX_3_X1 = 108;
    // HLP_IDX_3_Y1 = 174;
    // HLP_IDX_3_X2 = 211;
    // HLP_IDX_3_Y2 = 185;

    field_idx_click_Q = CTRL_CreateClickLabel(108, 186, 211, 199, 0, -1);
    // HLP_IDX_4 = 631; // HLP_QUITTODOS
    // HLP_IDX_4_X1 = 108;
    // HLP_IDX_4_Y1 = 186;
    // HLP_IDX_4_X2 = 211;
    // HLP_IDX_4_Y2 = 199;

    if(save9_exists == 0)
        field_idx_hotkey_C = -1000;  // INVALID
    else
        field_idx_hotkey_C = CTRL_CreateHotkey(hotkey_C, -1);
    if(save_exists == 0)
        field_idx_hotkey_L = -1000;
    else
        field_idx_hotkey_L = CTRL_CreateHotkey(hotkey_L, -1);
    field_idx_hotkey_N = CTRL_CreateHotkey(hotkey_N, -1);
    field_idx_hotkey_H = CTRL_CreateHotkey(hotkey_H, -1);
    field_idx_hotkey_Q = CTRL_CreateHotkey(hotkey_Q, -1);
    field_idx_hotkey_ESC = CTRL_CreateHotkey(hotkey_ESC, -1);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Main_Menu_Add_Fields()\n", __FILE__, __LINE__);
#endif
}

// MGC s01p05
// int SCREEN_Menu(void)
int Main_Menu_Screen(void)
{
    char Found_File_Name[30];
    char Conversion_String[30];
    char File_Name[30];
    int field_idx_hotkey_C;
    int field_idx_hotkey_L;
    int field_idx_hotkey_N;
    int field_idx_hotkey_H;
    int field_idx_hotkey_Q;
    int field_idx_hotkey_ESC;
    int draw_done;
    int menu_shift;
    int itr_save_gams;
    int flag_done;
    int input_field_idx;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Main_Menu_Screen()\n", __FILE__, __LINE__);
#endif

    if(main_menu_jump != ST_UNDEFINED)
    {
        return main_menu_jump;
    }

    GUI_LoadSave_State = ST_UNDEFINED;
    draw_done = ST_FALSE;
    save9_exists = ST_FALSE;
    main_menu_selection = ST_UNDEFINED;
    save_exists = ST_FALSE;
    flag_done = ST_FALSE;

    screen_fade = ST_TRUE;
    // if(screen_fade != ST_FALSE) { /* VGA_Fade_Out */ }

    // ? macro ? ~== Clear Both Screen Pages
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // Fill()
    VGA_SetDirectDraw();  // Set_Page_On()
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // Fill()
    VGA_Set_DSP_Addr();  // Set_Page_Off();

    PAL_Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View

    /* GAME_LoadSettings */

    // ? macro ? ~ Clear_Help()
    // // HLP_Entry_Table.Entry_Index = -1;
    // HLP_IDX_0 = -1;
    // HLP_IDX_1 = -1;
    // HLP_IDX_2 = -1;
    // HLP_IDX_3 = -1;
    // HLP_IDX_4 = -1;

    // ~ MoO2 Check_For_Saved_Games()
    save_gam_count = 0;
    for(itr_save_gams = 1; itr_save_gams < 10; itr_save_gams++)
    {
        strcpy(File_Name, save_file_name);
        itoa(itr_save_gams, Conversion_String, 10);
        strcat(File_Name, Conversion_String);
        strcat(File_Name, save_file_extension);
        if(DIR(File_Name, Found_File_Name) == 0)
        {
            save_gams[save_gam_count] = ST_UNDEFINED;
        }
        else
        {
            if(itr_save_gams < 9)
            {
                save_gams[save_gam_count] = itr_save_gams - 1;
                save_gam_count++;
            }
            else
            {
                save9_exists = ST_TRUE;
            }
        }
    }
    if(save_gam_count > 0)
    {
        save_exists = ST_TRUE;
    }

    CTRL_Clear();  // Clear_Fields();

    menu_shift = 0;

    if(save_exists == ST_FALSE)
    {
        field_idx_click_L = -1000;  // INVALID
        menu_shift = 1;
    }
    else
    {
        field_idx_click_L = CTRL_CreateClickLabel(108, 150, 211, 161, 0, -1);
        // HLP_IDX_0 = 629; // HLP_LOAD
        // HLP_IDX_0_X1 = 108;
        // HLP_IDX_0_Y1 = 148;
        // HLP_IDX_0_X2 = 211;
        // HLP_IDX_0_Y2 = 162;
    }

    if(save9_exists == ST_FALSE)
    {
        field_idx_click_C = -1000;  // INVALID
    }
    else
    {
        field_idx_click_C = CTRL_CreateClickLabel(108, (138 + (12 * menu_shift)), 211, (149 + (12 * menu_shift)), 0, -1);
        // HLP_IDX_1 = 628; // HLP_CONTINUE
        // HLP_IDX_1_X1 = 108;
        // HLP_IDX_1_Y1 = (138 + (12 * menu_shift));
        // HLP_IDX_1_X2 = 211;
        // HLP_IDX_1_Y2 = (149 + (12 * menu_shift));
    }

    field_idx_click_N = CTRL_CreateClickLabel(108, 162, 211, 173, 0, -1);
    // HLP_IDX_2 = 630; // HLP_NEWGAME
    // HLP_IDX_2_X1 = 108;
    // HLP_IDX_2_Y1 = 162;
    // HLP_IDX_2_X2 = 211;
    // HLP_IDX_2_Y2 = 173;

    field_idx_click_H = CTRL_CreateClickLabel(108, 174, 211, 185, 0, -1);
    // HLP_IDX_3 = 798; // HLP_HALLOFFAME
    // HLP_IDX_3_X1 = 108;
    // HLP_IDX_3_Y1 = 174;
    // HLP_IDX_3_X2 = 211;
    // HLP_IDX_3_Y2 = 185;

    field_idx_click_Q = CTRL_CreateClickLabel(108, 186, 211, 199, 0, -1);
    // HLP_IDX_4 = 631; // HLP_QUITTODOS
    // HLP_IDX_4_X1 = 108;
    // HLP_IDX_4_Y1 = 186;
    // HLP_IDX_4_X2 = 211;
    // HLP_IDX_4_Y2 = 199;

    if(save9_exists == 0)
        field_idx_hotkey_C = -1000;  // INVALID
    else
        field_idx_hotkey_C = CTRL_CreateHotkey(hotkey_C, -1);
    if(save_exists == 0)
        field_idx_hotkey_L = -1000;  // INVALID
    else
        field_idx_hotkey_L = CTRL_CreateHotkey(hotkey_L, -1);
    field_idx_hotkey_N = CTRL_CreateHotkey(hotkey_N, -1);
    field_idx_hotkey_H = CTRL_CreateHotkey(hotkey_H, -1);
    field_idx_hotkey_Q = CTRL_CreateHotkey(hotkey_Q, -1);
    field_idx_hotkey_ESC = CTRL_CreateHotkey(hotkey_ESC, -1);

    // GUI_SetWindows(1, &main_menu_window);  // Set_Mouse_List(1, &main_menu_window);
    GUI_SetWindows(1, g_GUI_MainMenuWindow);

    // TODO(JimBalcomb,20220723): figure out if this SaveCursorArea is just because of the subsequent MouseEmuMoveTo
    CRL_Save_RSP(MD_GetX(), MD_GetY());  // CRL_Save_RSP(MD_Get_X(), MD_Get_Y());
    // GUI_MouseEMUMoveTo(GUI_NewGame_Label);

    // MGC s28p15  FLIC_Reset_CurrentFrame  FLIC_ResetFrame  LBX_IMG_ResetFrame
    // FLIC_Reset_CurrentFrame(mainmenu_top);
    FLIC_Reset_CurrentFrame(FP_SEG(mainmenu_top));

    // TODO(JimBalcomb): 2022017: double check it's accurate that this is being set a second time here, and add a note to avoid questioning this again in the future
    main_menu_selection = ST_UNDEFINED;

    SCRN_Set_Redraw_Function(Main_Menu_Screen_Draw, 2);

    // GUI_SetHelp(HLP_IDX_0, 5);

    IN_Set_Skip(4);  // Set_Input_Delay(4);


    while(flag_done == ST_FALSE)
    {
        CLK_SaveCounter();  // CLK_Save();

        // ST_GUI.H  _s34p66c.c  int IN_GetInput(void)
        input_field_idx = IN_GetInput();  // _s34p66c.c  1oom :: oi1 = uiobj_handle_input_cond();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] input_field_idx: %d\n", __FILE__, __LINE__, input_field_idx);
#endif

        if(input_field_idx != ST_FALSE) { flag_done = ST_TRUE; }

        if((input_field_idx == field_idx_hotkey_C) || (input_field_idx == field_idx_click_C))
        {
            flag_done = ST_TRUE;
            main_menu_selection = 0;
        }
        if((input_field_idx == field_idx_hotkey_L) || (input_field_idx == field_idx_click_L))
        {
            flag_done = ST_TRUE;
            main_menu_selection = 1;
        }
        if((input_field_idx == field_idx_hotkey_N) || (input_field_idx == field_idx_click_N))
        {
            flag_done = ST_TRUE;
            main_menu_selection = 2;
        }
        if((input_field_idx == field_idx_hotkey_Q) || (input_field_idx == field_idx_click_Q) || (input_field_idx == field_idx_hotkey_ESC))
        {
            flag_done = ST_TRUE;
            main_menu_selection = 3;
        }
        if((input_field_idx == field_idx_hotkey_H) || (input_field_idx == field_idx_click_H))
        {
            flag_done = ST_TRUE;
            main_menu_selection = 4;
        }

        if(main_menu_selection == ST_UNDEFINED) { flag_done = ST_FALSE; }

        if(flag_done == ST_FALSE)
        {
            Main_Menu_Screen_Draw();
            SCRN_SimplePageFlip();  // Toggle_Pages();
            if(!((screen_fade == ST_FALSE) & (draw_done != ST_FALSE)))
            {
                // Fade_In()
                VGA_Copy_RSP_DSP();  // Copy_Off_To_On_Page();  // TODO(JimBalcomb,20220117): figure out what Copy_Off_To_On_Page() actually does
                draw_done = ST_TRUE;
                screen_fade = ST_FALSE;
            }
            CLK_Wait(2);
        }

    }  /* while ( flag_done == ST_FALSE ) */

    SCRN_DisableRedraw();

    HLP_ClearHelp();  // Deactivate_Help_List();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Main_Menu_Screen()\n", __FILE__, __LINE__);
#endif

    return main_menu_selection;
}

// s01p06
// void SCREEN_Menu_Draw(void)
void Main_Menu_Screen_Draw(void)
{
    int Title_Frame_Index;
    int Loop_Var;
    int menu_shift;
    int mouse_field;
    int menu_x_start;
    int menu_y_start;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Main_Menu_Screen_Draw()\n", __FILE__, __LINE__);
#endif

    menu_x_start = 123;
    menu_y_start = 141;

    mouse_field = GUI_MousedControl();  // MGC s34p25  Get_Mouse_Field()
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] mouse_field: %d\n", __FILE__, __LINE__, mouse_field);
#endif

    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // Fill()
    VGA_Set_DSP_Addr();  // Set_Page_Off();

    Title_Frame_Index = FLIC_Get_CurrentFrame(FP_SEG(mainmenu_top));
    FLIC_Set_CurrentFrame(FP_SEG(mainmenu_top), 0);
    for(Loop_Var = 0; Loop_Var <= Title_Frame_Index; Loop_Var++)
    {
        FLIC_Draw(0, 0, mainmenu_top);
    }
    FLIC_Draw(0, 41, mainmenu_bot);
    
    if(screen_fade == 0) { /* GAME_DrawCredits() */ }

    /*
        Draw Buttons
            NOTE: uses VORTEX, not MAINSCRN
    */
    menu_shift = 0;
    if(save_exists == 0)
    {
        menu_shift = 1;
    }
    else
    {
        if(mouse_field == field_idx_click_L)
            FLIC_Reset_CurrentFrame(FP_SEG(mainmenu_l));
        else
            FLIC_Set_CurrentFrame(FP_SEG(mainmenu_l), 1);
        FLIC_Draw(menu_x_start, (menu_y_start + 12), mainmenu_l);
    }

    if(save9_exists != 0)
    {
        if(mouse_field == field_idx_click_C )
            FLIC_Reset_CurrentFrame(FP_SEG(mainmenu_c));
        else
            FLIC_Set_CurrentFrame(FP_SEG(mainmenu_c), 1);
        FLIC_Draw(menu_x_start, (menu_y_start + (12 * menu_shift)), mainmenu_c);
    }

    if(mouse_field == field_idx_click_N)
        FLIC_Reset_CurrentFrame(FP_SEG(mainmenu_n));
    else
        FLIC_Set_CurrentFrame(FP_SEG(mainmenu_n), 1);
    FLIC_Draw(menu_x_start, (menu_y_start + 24), mainmenu_n);

    if(mouse_field == field_idx_click_H)
        FLIC_Reset_CurrentFrame(FP_SEG(mainmenu_h));
    else
        FLIC_Set_CurrentFrame(FP_SEG(mainmenu_h), 1);
    FLIC_Draw(menu_x_start, (menu_y_start + 36), mainmenu_h);

    if(mouse_field == field_idx_click_Q)
        FLIC_Reset_CurrentFrame(FP_SEG(mainmenu_q));
    else
        FLIC_Set_CurrentFrame(FP_SEG(mainmenu_q), 1);
    FLIC_Draw(menu_x_start, (menu_y_start + 48), mainmenu_q);

    // 
    VGA_DAC_Write();

    // ? CRP_Unused_Anim_Var = ((CRP_Unused_Anim_Var + 1) / 20); ?

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Main_Menu_Screen_Draw()\n", __FILE__, __LINE__);
#endif
}
