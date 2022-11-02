#ifndef SEG028_H
#define SEG028_H

#include "ST_SA.H"  /* SAMB_addr */





#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

// ~== MoO2
void Draw(signed short int x, signed short int y, unsigned char * picture);

/*
    seg028
*/

// s28p02
void FLIC_Prepare(int FlicWidth, int FlicHeight, SAMB_addr sa_FLIC_Header);

// s28p11
void FLIC_Draw_XY(int Left, int Top, SAMB_addr sa_FLIC_Header);


// s28p14
void FLIC_SetFrame(unsigned int sa_FLIC_Header, int Frame_Index);

// s28p15
void FLIC_ResetFrame(SAMB_addr sa_FLIC_Header);

// s28p16
word FLIC_GetCurFrame(SAMB_addr sa_FLIC_Header);


#ifdef __cplusplus
}
#endif


#endif /* SEG028_H */
