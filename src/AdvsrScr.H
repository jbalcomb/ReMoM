/*
    Advisor Screen

    WIZARDS.EXE
        ovr076
    
*/

#ifndef ADVSRSCR_H
#define ADVSRSCR_H

#include "MoM.H"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr076
*/

// WZD o76p01
void Advisor_Screen(int16_t advisor_idx);

// WZD o76p02
void TaxCollector_Window(void);

// WZD o76p03
void IDK_AdvsrScr_Historian(void);

// WZD o76p04
// sub_655C4()

// WZD o76p05
// sub_65B47()

// WZD o76p06
void Status_Screen(void);

// WZD o76p07
void Status_Screen_Draw(void);

// WZD o76p08
// IDK_Cartographer_Screen()

// WZD o76p09
// OVL_CartographRedraw()

// WZD o76p10
// Cartographer_Screen_Draw()

// WZD o76p11
void GrandVizier_Window(void);



#ifdef __cplusplus
}
#endif


#endif  /* ADVSRSCR_H */
