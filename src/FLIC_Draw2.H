#ifndef FLIC_DRAW2_H
#define FLIC_DRAW2_H

#include "MoX.H"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WZD  seg029
*/

// WZD s29p01
void FLIC_Draw_Frame_2x(int16_t x_start, int16_t y_start, int16_t width, byte_ptr frame_data, int16_t pitch, int16_t scale);



/*
    WZD  seg030
*/

// WZD s30p11
void Draw(int16_t x_start, int16_t y_start, SAMB_ptr p_FLIC_File, int16_t pitch, int16_t scale);



#ifdef __cplusplus
}
#endif

#endif /* FLIC_DRAW2_H */
