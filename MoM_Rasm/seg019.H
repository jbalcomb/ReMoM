#ifndef SEG019_H
#define SEG019_H

#include "MoX_TYPE.H"

extern uint16_t UU_TextLine_Initd;              // dseg:4318
// ...
extern SAMB_ptr p_TextLine_Starts;                  // dseg:A820
extern SAMB_ptr p_TextLine_Tops;                    // dseg:A824
extern SAMB_ptr p_TextLine_Rights;                  // dseg:A828
extern SAMB_ptr p_TextLine_Lefts;                   // dseg:A82C



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg019
*/

//[?]_s19p05         VGA_GetTextHeight
//[?]_s19p06         VGA_GetTextWidth

// s19p11
int16_t VGA_GetFontHeight(void);

// s19p14
void VGA_TextDraw_Init(void);


#ifdef __cplusplus
}
#endif


#endif /* SEG019_H */
