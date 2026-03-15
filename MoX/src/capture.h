/*
    WIZARDS.EXE
        seg024

MoO2
    Module: capture
    Module: fields

*/
#ifndef CAPTURE_H
#define CAPTURE_H

#include "MOX_TYPE.h"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WZD  seg024
*/

// WZD s24p01
void F11_Key__WIP(void);

// WZD s24p02
short int Screen_Flic_Capture(void);

// WZD s24p03
void VGA_GetDACBlock(short int dac_block, uint8_t *buffer);

// WZD s24p04
void VGA_ReadScreenLine(short int line, char *scanline_buffer);

// WZD s24p05
void Release_Version(void);

// WZD s24p06
int Check_Release_Version(void);

// WZD s24p07
void Screen_Picture_Capture(int16_t x1, int16_t y1, int16_t x2, int16_t y2, byte_ptr pict_seg);

// WZD s24p08
void Capture_Screen_Block(byte_ptr frame_data, int16_t x1, int16_t y1, int16_t x2, int16_t y2);



#ifdef __cplusplus
}
#endif

#endif /* CAPTURE_H */
