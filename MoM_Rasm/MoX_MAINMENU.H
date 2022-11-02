#ifndef MOX_MAINMENU_H
#define MOX_MAINMENU_H

#include "MoX_TYPE.H"       /* SAMB_ptr */


extern SAMB_ptr mainmenu_top;
extern SAMB_ptr mainmenu_bot;
extern SAMB_ptr mainmenu_c;
extern SAMB_ptr mainmenu_l;
extern SAMB_ptr mainmenu_n;
extern SAMB_ptr mainmenu_h;
extern SAMB_ptr mainmenu_q;


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    MGC  seg001
*/

// s01p03
void GAME_MainMenu(void);



#ifdef __cplusplus
}
#endif



#endif MOX_MAINMENU_H
