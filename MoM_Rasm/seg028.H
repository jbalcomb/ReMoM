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
// void FLIC_Prepare(int FlicWidth, int FlicHeight, SAMB_head FlicHdr_SgmtAddr);

// s28p11
void FLIC_Draw_XY(int Left, int Top, SAMB_addr sa_FLIC_Header);
void FLIC_Draw_XY_Shim(int Left, int Top, SAMB_addr sa_FLIC_Header);


#ifdef __cplusplus
}
#endif


#endif /* SEG028_H */
