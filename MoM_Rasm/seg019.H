

#ifndef SEG019_H
#define SEG019_H


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
int VGA_GetFontHeight(void);

// s19p14
void VGA_TextDraw_Init(void);


#ifdef __cplusplus
}
#endif


#endif /* SEG019_H */
