#ifndef SEG028_H
#define SEG028_H

#include "ST_SA.H"  /* SAMB_addr */





#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    seg028
*/

// s28p02
void FLIC_Prepare(int FlicWidth, int FlicHeight, SAMB_addr sa_FLIC_Header);

// s28p11
void DELETE__FLIC_Draw_XY_Redux(int x, int y, SAMB_addr sa_FLIC_Header);
void DELETE__FLIC_Draw_XY(int Left, int Top, SAMB_addr sa_FLIC_Header);


// s28p14
void FLIC_Set_CurrentFrame(SAMB_addr sa_FLIC_Header, int Frame_Index);

// s28p15
void FLIC_Reset_CurrentFrame(SAMB_addr sa_FLIC_Header);

// s28p16
word FLIC_Get_CurrentFrame(SAMB_addr sa_FLIC_Header);


#ifdef __cplusplus
}
#endif


#endif /* SEG028_H */
