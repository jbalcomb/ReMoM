
#include "MoX_TYPE.H"       /* SAMB_ptr */
#include "MoX_DEF.H"

#include "MoX_MoM.H"

#include "MoX_EXIT.H"
#include "MoX_FLIC.H"
#include "MoX_LBX.H"

#include "STU_DBG.H"


// s01p03
void Main_Menu_Screen_(void);
// s01p04
void GAME_LoadMainImages(void);
// s01p05
int SCREEN_Menu(void);
// s01p06
void SCREEN_Menu_Draw(void);



char mainscrn_lbx_file[] = "MAINSCRN";          // MGC  dseg:28A7

char vortex_lbx_file[] = "VORTEX";              // MGC  dseg:28B0

// char g_LbxNm_WIZARDS[] = "WIZARDS";         // dseg:28B7
// char g_LbxNm_SPELLDAT[] = "SPELLDAT";       // dseg:28BF

// SAMB_ptr gsa_VORTEX_3_MenuQuitToDOS;    // MGC  dseg:52BA
// SAMB_ptr gsa_VORTEX_2_MenuHallOfFame;   // MGC  dseg:52BE
// SAMB_ptr gsa_VORTEX_5_MenuLoadGame;     // MGC  dseg:52C2
// SAMB_ptr gsa_MAINSCRN_0_AnimatedLogo;   // MGC  dseg:52E8
// SAMB_ptr gsa_VORTEX_4_MenuNewGame;      // MGC  dseg:52EC
// SAMB_ptr gsa_VORTEX_1_MenuContinue;     // MGC  dseg:52F0
// SAMB_ptr gsa_MAINSCRN_5_ScreenBottom;   // MGC  dseg:52F4

SAMB_ptr mainmenu_top;
SAMB_ptr mainmenu_bot;
SAMB_ptr mainmenu_c;
SAMB_ptr mainmenu_l;
SAMB_ptr mainmenu_n;
SAMB_ptr mainmenu_h;
SAMB_ptr mainmenu_q;


// s01p03
void Main_Menu_Screen_(void)
{
    int flag_quit;
    int Screen_Action;

    flag_quit = ST_FALSE;

    while ( flag_quit == ST_FALSE )
    {
        Screen_Action = SCREEN_Menu();  // MGC_DEF.H  _s01p05c.c
        switch (Screen_Action)
        {
            case 0:  // "Continue"
                break;
            case 1:  // "Load Game"
                break;
            case 2:  // "New Game"
                break;
            case 3:  // "Quit To DOS"
                flag_quit = ST_TRUE;
                break;
            case 4:  // "Hall Of Fame"
                break;
        }
        
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: GAME_MainMenu()\n", __FILE__, __LINE__);
#endif

    Exit_With_Message(_leave_game_message);
}

// s01p04
void GAME_LoadMainImages(void)
{
    int itr_wizards;

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: BEGIN: GAME_LoadMainImages()\n", __FILE__, __LINE__);
// #endif

    // EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);
    // EMM_Load_LBX_File_1(g_LbxNm_WIZARDS);
    // EMM_Load_LBX_File_1(g_LbxNm_SPELLDAT);

    mainmenu_top = Farload(mainscrn_lbx_file, 0);
    mainmenu_bot = Farload(mainscrn_lbx_file, 5);
    mainmenu_c   = Farload(vortex_lbx_file, 1);
    mainmenu_h   = Farload(vortex_lbx_file, 2);
    mainmenu_q   = Farload(vortex_lbx_file, 3);
    mainmenu_n   = Farload(vortex_lbx_file, 4);
    mainmenu_l   = Farload(vortex_lbx_file, 5);

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: END: GAME_LoadMainImages()\n", __FILE__, __LINE__);
// #endif

}

// s01p05
int SCREEN_Menu(void)
{
    return 3;
}
// s01p06
void SCREEN_Menu_Draw(void)
{

}
