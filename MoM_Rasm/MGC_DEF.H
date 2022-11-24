#ifndef MGC_DEF_H
#define MGC_DEF_H

#include "MOM_HEAD.H"

//#include "ST_GUI.H"

//
// derived constants
//

extern	char	configname[13];



// MAINMENU CREDITS  // struc CreditsPairs ; (sizeof=0x4)
// MAINMENU CREDITS  // Role dw ?                               ; offset (00031D10)
// MAINMENU CREDITS  // Name dw ?                               ; offset (00031D10)
// MAINMENU CREDITS  // ends CreditsPairs
// MAINMENU CREDITS  struct s_CreditsPairs //; (sizeof=0x4)
// MAINMENU CREDITS  {
// MAINMENU CREDITS      char *Role;
// MAINMENU CREDITS      char *Name;
// MAINMENU CREDITS  };



/*
    Initialized Data
*/
// MAINMENU  extern struct s_GUI_WINDOW g_GUI_MainMenuWindow[]; // dseg:205E
// dseg:206A align 4
// MAINMENU  extern int CRP_Unused_Anim_Var; // dseg:206C CRP_Unused_Anim_Var
// dseg:206E __ovrbuffer dw 1000h                    ; declared in overlay.lib\OVRBUFF

// \/\/\/\/\/ in MOM_DEF, as are same in WZD
// dseg:27FF cnst_ConfigFile db 'CONFIG.MOM',0
// dseg:280A cnst_ConfigErr db 'Run INSTALL to configure MASTER OF MAGIC.',0Ah, 0Ah,0
// dseg:2836 cnst_RB db 'rb',0
// dseg:2839 cnst_MIDI_Init db 'Initializing Roland Drivers...$',0
// dseg:2859 cnst_SAVE db 'SAVE',0
// dseg:285E cnst_SAVE_ext db '.GAM',0
// dseg:2863 cnst_Set_File db 'MAGIC.SET',0
// dseg:286D cnst_WB db 'wb',0
// dseg:2870 cnst_FONT_File db 'FONTS.LBX',0
// /\/\/\/\/\ in MOM_DEF, as are same in WZD

// dseg:287A cnst_MUSIC_File db 'music',0
// MAINMENU  extern char cnst_QUIT_Message[];            // dseg:2880
// MAINMENU  extern char g_LbxNm_MAINSCRN[];             // dseg:28A7 
// MAINMENU  extern char cnst_HOTKEY_N;                  // dseg:28AE
// MAINMENU  extern char g_LbxNm_VORTEX[];               // dseg:28B0 
// ST_NEWG  extern char g_LbxNm_WIZARDS[];              // dseg:28B7
// ST_NEWG  extern char g_LbxNm_SPELLDAT[];             // dseg:28BF
// MAINMENU  extern char cnst_HOTKEY_C;                  // dseg:28C8
// MAINMENU  extern char cnst_HOTKEY_L;                  // dseg:28CA
// MAINMENU  extern char cnst_HOTKEY_H;                  // dseg:28CC
// MAINMENU  extern char cnst_HOTKEY_Q;                  // dseg:28CE
// MAINMENU  extern char cnst_HOTKEY_Esc;                // dseg:28D0
// ST_NEWG  extern char g_LbxNm_TERRSTAT[];             // dseg:28D2
// dseg:28DB cnst_HLPENTRY_File db 'hlpentry',0

// \/\/\/\/\/ in MOM_DEF, as are same in WZD
// dseg:28E4 cnst_SAVE2 db 'SAVE',0
// dseg:28E9 cnst_SAVE_ext2 db '.GAM',0
// dseg:28EE cnst_WB2 db 'wb',0
// dseg:28F1 ; char cnst_Set_File2[]
// dseg:28F1 cnst_Set_File2 db 'MAGIC.SET',0
// dseg:28FB cnst_RB2 db 'rb',0
// /\/\/\/\/\ in MOM_DEF, as are same in WZD

/*
    ? next section all for settings screen ?
*/
// dseg:28FE GUI_Settings_Wnd GUI_WINDOW <Crsr_Normal, 0, 0, 0, 319, 199>
// MAINMENU  extern int GUI_LoadSave_State; // dseg:290A  ; 3 indicates returning from the settings screen
// dseg:290C GUI_Settings_Tops dw    53,   64,   75,  102,  113,  124,  135,  153,  146,  157,  167,  163,  174
// dseg:2926 COL_Settings db 4 dup(62h)
// dseg:292A cnst_LOAD_File db 'LOAD.LBX'
// dseg:2932 cnst_ZeroString_3 db 0
// dseg:2933 cnst_Set_File3 db 'MAGIC.SET',0
// dseg:293D cnst_RB3 db 'rb',0
// dseg:2940 cnst_HOTKEY_Esc2 db 1Bh,0
// dseg:2942 cnst_HOTKEY_O db 'O',0
// dseg:2944 cnst_WB3 db 'wb',0
// dseg:2947 cnst_Settings_0 db 'Sound Effects',0
// dseg:2955 cnst_Settings_1 db 'Background Music',0
// dseg:2966 cnst_Settings_2 db 'Event Music',0
// dseg:2972 cnst_Settings_3 db 'City Spell Events',0
// dseg:2984 cnst_Settings_4 db 'Overland Spell Events',0
// dseg:299A cnst_Settings_5 db 'Summoning Events',0
// dseg:29AB cnst_Settings_6 db 'End of Turn Summary',0
// dseg:29BF cnst_Settings_7 db 'Raze City',0
// dseg:29C9 cnst_Settings_8 db 'Random Events',0
// dseg:29D7 cnst_Settings_9 db 'End of Turn Wait',0
// dseg:29E8 cnst_Settings_A db 'Strategic Combat Only',0
// dseg:29FE cnst_Settings_B db 'Auto Unit Information',0
// dseg:2A14 cnst_Settings_C db 'Movement Animations',0
// dseg:2A28 cnst_Settings_D db 'Enemy Moves',0
// dseg:2A34 cnst_Settings_E db 'Enemy Spells',0
// dseg:2A41 cnst_Settings_F db 'Spell Book Ordering',0
// dseg:2A55 cnst_Settings_10 db 'Spell Animations',0
// dseg:2A66 cnst_Settings_11 db 'Show Node Owners',0
// dseg:2A77 cnst_Settings_12 db 'Expanding Help',0
// dseg:2A86 cnst_SettingsGrp_1 db 'Sound and Music',0
// dseg:2A96 cnst_SettingsGrp_2 db 'Messages',0
// dseg:2A9F cnst_SettingsGrp_3 db 'Combat',0
// dseg:2AA6 cnst_SettingsGrp_4 db 'Display',0
// dseg:2AAE cnst_HLPENTRY_File2 db 'hlpentry.lbx',0
// dseg:2ABB db 0


// dseg:385E g_TBL_IntroFrameCounts dw    15,   12,   30,   18,   25,   32,   24,   32,   30,   37,   30,   11,   32
// dseg:3878 cnst_MUSIC_File2 db 'music.lbx'         ; should use dseg:287a
// dseg:3881 cnst_ZeroString_5 db 0
// dseg:3882 cnst_SOUNDFX_File db 'soundfx.lbx',0
// dseg:388E cnst_HOTKEY_Esc5 db 1Bh,0               ; should use dseg:28d0
// dseg:3890 cnst_INTRO_File db 'INTRO.LBX',0
// dseg:389A cnst_Simtex_CR2 db 'Copyright  Simtex Software, 1995   V1.31',0 ; should use dseg:3067
// dseg:38C3 align 2
// dseg:38C4 COL_HLP_Titles db 0B9h,4 dup( 2Fh)      ; this should ideally have been 16 bytes long
// dseg:38C9 COL_HLP_Text db 0B8h,4 dup( 37h)        ; this should ideally have been 16 bytes long
// dseg:38CE cnst_HELP_File db 'HELP',0              ; could use dseg:2a81
// dseg:38D3 cnst_HELP_File2 db 'help',0             ; should use dseg:38ce
// MAINMENU CREDITS  extern int GAME_FirstShownCred;             // dseg:38D8
// MAINMENU CREDITS  extern int GAME_CreditTextTop;              // dseg:38DA
// MAINMENU CREDITS  extern int GAME_CredsRollTimer;             // dseg:38DC


// MAINMENU CREDITS  extern struct s_CreditsPairs TBL_Credits_Texts[];
// dseg:38DE TBL_Credits_Texts CreditsPairs <offset cnst_Credits_00, offset cnst_Credits_01>; 0 ; could use dseg:2947 or the other way around
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 1
// dseg:38DE CreditsPairs <offset cnst_Credits_20, offset cnst_Credits_21>; 2
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_31>; 3
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_01>; 4
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_51>; 5
// dseg:38DE CreditsPairs 2 dup(<offset cnst_ZeroString_6, offset cnst_ZeroString_6>); 6
// dseg:38DE CreditsPairs <offset cnst_Credits_80, offset cnst_Credits_81>; 8
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 9
// dseg:38DE CreditsPairs <offset cnst_Credits_100, offset cnst_Credits_101>; 10
// dseg:38DE CreditsPairs <offset cnst_Credits_110, offset cnst_Credits_111>; 11
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_121>; 12
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_131>; 13
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_141>; 14
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_151>; 15
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_51>; 16
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 17
// dseg:38DE CreditsPairs <offset cnst_Credits_180, offset cnst_Credits_181>; 18
// dseg:38DE CreditsPairs <offset cnst_Credits_190, offset cnst_Credits_191>; 19
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 20
// dseg:38DE CreditsPairs <offset cnst_Credits_210, offset cnst_Credits_211>; 21
// dseg:38DE CreditsPairs <offset cnst_Credits_220, offset cnst_Credits_221>; 22
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_231>; 23
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_241>; 24
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_251>; 25
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_261>; 26
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_271>; 27
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_281>; 28
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_291>; 29
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_301>; 30
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_311>; 31
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_321>; 32
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_331>; 33
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_341>; 34
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_351>; 35
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_361>; 36
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 37
// dseg:38DE CreditsPairs <offset cnst_Credits_380, offset cnst_Credits_381>; 38
// dseg:38DE CreditsPairs <offset cnst_Credits_390, offset cnst_Credits_391>; 39
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_401>; 40
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_411>; 41
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 42
// dseg:38DE CreditsPairs <offset cnst_Credits_430, offset cnst_Credits_431>; 43
// dseg:38DE CreditsPairs 2 dup(<offset cnst_ZeroString_6, offset cnst_ZeroString_6>); 44
// dseg:38DE CreditsPairs <offset cnst_Credits_460, offset cnst_Credits_461>; 46
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 47

// dseg:399E cnst_Credits_00 db 'Game Designer'
// dseg:39AB cnst_ZeroString_6 db 0
// dseg:39AC cnst_Credits_01 db 'Steve Barcia',0
// dseg:39B9 cnst_Credits_20 db 'Programmers',0
// dseg:39C5 cnst_Credits_21 db 'Jim Cowlishaw',0
// dseg:39D3 cnst_Credits_31 db 'Ken Burd',0
// dseg:39DC cnst_Credits_51 db 'Grissel Barcia',0
// dseg:39EB cnst_Credits_80 db 'Producer',0
// dseg:39F4 cnst_Credits_81 db 'Doug Caspian-Kaufman',0
// dseg:3A09 cnst_Credits_100 db 'Art Director',0
// dseg:3A16 cnst_Credits_101 db 'Jeff Dee',0
// dseg:3A1F cnst_Credits_110 db 'Artists',0
// dseg:3A27 cnst_Credits_111 db 'Shelly Hollen',0
// dseg:3A35 cnst_Credits_121 db 'Amanda Dee',0
// dseg:3A40 cnst_Credits_131 db 'Steve Austin',0
// dseg:3A4D cnst_Credits_141 db 'George Purdy',0
// dseg:3A5A cnst_Credits_151 db 'Patrick Owens',0
// dseg:3A68 cnst_Credits_180 db 'Music Producer',0
// dseg:3A77 cnst_Credits_181 db 'The Fat Man',0
// dseg:3A83 cnst_Credits_190 db 'Composer',0
// dseg:3A8C cnst_Credits_191 db 'Dave Govett',0
// dseg:3A98 cnst_Credits_210 db 'QA Lead',0
// dseg:3AA0 cnst_Credits_211 db 'Destin Strader',0
// dseg:3AAF cnst_Credits_220 db 'Play Test',0
// dseg:3AB9 cnst_Credits_221 db 'Mike Balogh',0
// dseg:3AC5 cnst_Credits_231 db 'Damon Harris',0
// dseg:3AD2 cnst_Credits_241 db 'Geoff Gessner',0
// dseg:3AE0 cnst_Credits_251 db 'Tammy Talbott',0
// dseg:3AEE cnst_Credits_261 db 'Mick Uhl',0
// dseg:3AF7 cnst_Credits_271 db 'Jim Hendry',0
// dseg:3B02 cnst_Credits_281 db 'Frank Brown',0
// dseg:3B0E cnst_Credits_291 db 'Jim Tricario',0
// dseg:3B1B cnst_Credits_301 db 'Jen MacLean',0
// dseg:3B27 cnst_Credits_311 db 'Brian Wilson',0
// dseg:3B34 cnst_Credits_321 db 'Brian Helleson',0
// dseg:3B43 cnst_Credits_331 db 'Jeff Dinger',0
// dseg:3B4F cnst_Credits_341 db 'Chris Bowling',0
// dseg:3B5D cnst_Credits_351 db 'Charles Brubaker',0
// dseg:3B6E cnst_Credits_361 db 'Tom Hughes',0
// dseg:3B79 cnst_Credits_380 db 'Sound Effects',0   ; could use dseg:2947 or the other way around
// dseg:3B87 cnst_Credits_381 db 'Midian',0
// dseg:3B8E cnst_Credits_390 db 'Speech',0
// dseg:3B95 cnst_Credits_391 db 'Mark Reis',0
// dseg:3B9F cnst_Credits_401 db 'Peter Woods',0
// dseg:3BAB cnst_Credits_411 db 'David Ellis',0
// dseg:3BB7 cnst_Credits_430 db 'Manual',0
// dseg:3BBE cnst_Credits_431 db 'Petra Schlunk',0
// dseg:3BCC cnst_Credits_460 db 'Special Thanks',0
// dseg:3BDB cnst_Credits_461 db 'Jenny Cowlishaw',0

// MAINMENU CREDITS  extern char cnst_Credits_DOT; // dseg:3BEB  db '.',0               ; should use dseg:30c9
// dseg:3BED align 2



/*
    Uninitialized Data
*/
// dseg:52B2 g_GUI_String_2 dw 0
// dseg:52B4 g_GUI_String_1 dw 0
// MAINMENU  extern int GUI_Load_Lbl_Index;                      // dseg:52B6
// dseg:52B8 IMG_FullScreen_BG@ dw 0

// MAINMENU  extern unsigned int gsa_VORTEX_3_MenuQuitToDOS;     // dseg:52BA
// MAINMENU  extern SAMB_addr sa_VORTEX_003;
// MAINMENU  extern SAMB_ptr fp_VORTEX_003;
// MAINMENU  extern SAMB_ptr p_VORTEX_003;
// extern SAMB_ptr _menu_quittodos_seg;

// MAINMENU  extern int GUI_HoF_Lbl_Index;                       // dseg:52BC

// MAINMENU  extern unsigned int gsa_VORTEX_2_MenuHallOfFame;    // dseg:52BE
// MAINMENU  extern SAMB_addr sa_VORTEX_002;
// MAINMENU  extern SAMB_ptr fp_VORTEX_002;
// MAINMENU  extern SAMB_ptr p_VORTEX_002;

// MAINMENU  extern int GUI_Quit_Lbl_Index;                      // dseg:52C0

// MAINMENU  extern unsigned int gsa_VORTEX_5_MenuLoadGame;      // dseg:52C2
// MAINMENU  extern SAMB_addr sa_VORTEX_005;
// MAINMENU  extern SAMB_ptr fp_VORTEX_005;
// MAINMENU  extern SAMB_ptr p_VORTEX_005;

// MAINMENU  extern int g_GUI_MainMenuSelected;                  // dseg:52C4
// ST_NEWG  extern unsigned int gsa_WIZARDS_0to13[];            // dseg:52C6
// dseg:52E2 IMG_NewG_BtnBorder@ dw 0
// MAINMENU  extern int g_GAME_HaveSaves;                        // dseg:52E4
// dseg:52E6 NEWG_HaveLastProfile dw 0

// MAINMENU  extern unsigned int gsa_MAINSCRN_0_AnimatedLogo;    // dseg:52E8
// MAINMENU  extern SAMB_addr sa_MAINSCRN_000;
// MAINMENU  extern SAMB_ptr fp_MAINSCRN_000;
// MAINMENU  extern SAMB_ptr p_MAINSCRN_000;

// MAINMENU  extern int GUI_NewGame_Label;                       // dseg:52EA

// MAINMENU  extern unsigned int gsa_VORTEX_4_MenuNewGame;       // dseg:52EC
// MAINMENU  extern SAMB_addr sa_VORTEX_004;
// MAINMENU  extern SAMB_ptr fp_VORTEX_004;
// MAINMENU  extern SAMB_ptr p_VORTEX_004;

// MAINMENU  extern int GUI_Continue_Label;                      // dseg:52EE

// MAINMENU  extern unsigned int gsa_VORTEX_1_MenuContinue;      // dseg:52F0
// MAINMENU  extern SAMB_addr sa_VORTEX_001;
// MAINMENU  extern SAMB_ptr fp_VORTEX_001;
// MAINMENU  extern SAMB_ptr p_VORTEX_001;

// MAINMENU  extern int g_GAME_HaveContSave;                     // dseg:52F2

// MAINMENU  extern unsigned int gsa_MAINSCRN_5_ScreenBottom;    // dseg:52F4
// MAINMENU  extern SAMB_addr sa_MAINSCRN_005;
// MAINMENU  extern SAMB_ptr fp_MAINSCRN_005;
// MAINMENU  extern SAMB_ptr p_MAINSCRN_005;
// extern SAMB_ptr _menu_bottom_castle_seg;

// dseg:52F6 NEWG_Screen0_Frame dw 0
// dseg:52F8 IMG_No_Load_Btn@ dw 0
// dseg:52FA NEWG_LastRetorts@ dw 0
// MAINMENU  extern int g_GUI_MAINMENUJump;                    // dseg:52FC  XREF: GAME_MainMenu(), SCREEN_Menu(), SCREEN_LoadSave()
// MAINMENU  extern int g_GAME_ValidSaveCount;                   // dseg:52FE
// MAINMENU  extern int g_GAME_ValidSaves_Main[];                // dseg:5300

// TODO(JimBalcomb): figure out what this is for and where it belongs in the code-base
// MAINMENU  extern int g_ScreenChangeFade; // dseg:64AE

// MAINMENU CREDITS  extern unsigned int gsa_GUI_Credits_Text_IMG; // dseg:948C


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    MAGIC.EXE seg001
*/

// MGC s01p01
// int MGC_Main(void);

// MGC s01p03
// void GAME_MainMenu(void);

// MGC s01p04
// void GAME_LoadMainImages(void);

// MGC s01p05
// int SCREEN_Menu(void);

// MGC s01p06
// void SCREEN_Menu_Draw(void);

// MGC s01p07
// void GAME_Load_TERRSTAT_0(void);

// MGC s01p08
// void GAME_Load_SPELLDAT_0(void);

/*
    MAGIC.EXE ovl060
*/
// MGC o60p01
int GAME_DrawCredits(void);
// MGC o60p02c.c
void GAME_PrepareCredits(void);


#ifdef __cplusplus
}
#endif


#endif /* MGC_DEF_H */
