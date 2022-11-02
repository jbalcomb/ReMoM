#ifndef SEG021_H
#define SEG021_H

#include "ST_TYPE.H"

#include "ST_SA.H"


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg021
*/

// s21p01
void VGA_DAC_Write(void);

// s21p07
void FLIC_Load_Palette(SAMB_ptr p_FLIC_Header, int frame_index);
void FLIC_LoadPalette_Redux(SAMB_addr sa_FLIC_Header, int Frame_Index);
void FLIC_LoadPalette(SAMB_addr sa_FLIC_Header, int Frame_Index);


#ifdef __cplusplus
}
#endif


#endif /* SEG021_H */
