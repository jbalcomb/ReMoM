#ifndef MAINMENU_H
#define MAINMENU_H

#include "MOM.H"

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
    WZD  DNE
    MGC  seg001
*/

// MGC s01p01
// _main

// MGC s01p02
// SND_Empty_FN

// MGC s01p03
void Main_Menu_Screen_Control(void);

// MGC s01p04
// MoO2: Reload_Main_Menu_Screen()
void Main_Menu_Load_Pictures(void);

// MGC s01p05
int16_t Main_Menu_Screen(void);

void Main_Menu_Add_Fields(void);

// MGC s01p06
void Main_Menu_Screen_Draw(void);

// MGC s01p07
// Load_TERRSTAT

// MGC s01p08
// Load_SPELLDAT


#ifdef __cplusplus
}
#endif


#endif  /* MAINMENU_H */
