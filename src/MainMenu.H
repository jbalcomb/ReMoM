#ifndef MAINMENU_H
#define MAINMENU_H

#include "MoX_TYPE.H"

extern char mainscrn_lbx_file[];
extern char vortex_lbx_file[];

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

// MGC s01p04
void Main_Menu_Load_Pictures(void);

// MGC s01p05
// int Main_Menu_Screen(void);
void Main_Menu_Screen(void);

void Main_Menu_Add_Fields(void);

// MGC s01p06
void Main_Menu_Screen_Draw(void);


#ifdef __cplusplus
}
#endif


#endif /* MAINMENU_H */
