#ifndef SEG027_H
#define SEG027_H

#include "ST_SA.H"  /* SAMB_addr */


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg027
*/

// s27p03
void FLIC_Draw_EMM_C(int ScreenPage_X, int ScreenPage_Y, SAMB_addr sa_FLIC_Header, int Frame_Index);


#ifdef __cplusplus
}
#endif


#endif /* SEG027_H */
