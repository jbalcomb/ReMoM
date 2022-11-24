#ifndef MAINMENU_H
#define MAINMENU_H

#include "ST_GUI.H"     /* s_GUI_WINDOW */

/*
    SEE: MGC-MainMenu.md
*/


/*
    Initialized Data
*/
extern struct s_GUI_WINDOW g_GUI_MainMenuWindow[]; // dseg:205E

extern char mainscrn_lbx_file[];          // MGC  dseg:28A7
extern char vortex_lbx_file[];            // MGC  dseg:28B0

extern unsigned char * mainmenu_top;  // MAINSCRN.LBX, 0
extern unsigned char * mainmenu_bot;  // MAINSCRN.LBX, 5
extern unsigned char * mainmenu_c;  // VORTEX.LBX, 1  "Continue"
extern unsigned char * mainmenu_h;  // VORTEX.LBX, 2  "Hall Of Fame"
extern unsigned char * mainmenu_q;  // VORTEX.LBX, 3  "Quit To DOS"
extern unsigned char * mainmenu_n;  // VORTEX.LBX, 4  "New Game"
extern unsigned char * mainmenu_l;  // VORTEX.LBX, 5  "Load Game"


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg001
*/


// MGC s01p04
// void GAME_LoadMainImages(void);
void Main_Menu_Load_Pictures(void);

// MGC s01p03
// void GAME_MainMenu(void);
void Main_Menu_Screen_Control(void);

// MGC s01p05
// int SCREEN_Menu(void);
int Main_Menu_Screen(void);

void Main_Menu_Add_Fields(void);

// MGC s01p06
// void SCREEN_Menu_Draw(void);
void Main_Menu_Screen_Draw(void);


#ifdef __cplusplus
}
#endif


#endif /* MAINMENU_H */
