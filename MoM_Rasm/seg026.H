#ifndef SEG026_H
#define SEG026_H


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    seg026
*/

// s26p01
void VGA_SetDirectDraw(void);

// s26p02
void VGA_Set_DSP_Addr(void);

// s26p03
int VGA_IsDefaultDrawScreenPage(void);

// s26p04
void VGA_PageFlip(void);

// s26p05
void VGA_Copy_RSP_DSP(void);
// // s26p06
// void VGA_Copy_RSP_DSP_YM(void);
// // s26p07
// void VGA_Copy_RSP_DSP_NM(void);


#ifdef __cplusplus
}
#endif


#endif /* SEG026_H */
