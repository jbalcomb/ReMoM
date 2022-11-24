#ifndef MAINMENU_H
#define MAINMENU_H

/* SEE: MGC-MainMenu.md */



extern char mainscrn_lbx_file[];          // MGC  dseg:28A7
extern char vortex_lbx_file[];            // MGC  dseg:28B0

// SAMB_ptr mainmenu_top;
// SAMB_ptr mainmenu_bot;
// SAMB_ptr mainmenu_c;
// SAMB_ptr mainmenu_l;
// SAMB_ptr mainmenu_n;
// SAMB_ptr mainmenu_h;
// SAMB_ptr mainmenu_q;

unsigned char * mainmenu_top;
unsigned char * mainmenu_bot;
unsigned char * mainmenu_c;
unsigned char * mainmenu_l;
unsigned char * mainmenu_n;
unsigned char * mainmenu_h;
unsigned char * mainmenu_q;


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
