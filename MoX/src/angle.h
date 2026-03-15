/*
    Angle

    WIZARDS.EXE
        seg034

1oom
C:\STU\devel\ReMoO1\src\util_math.c
int Get_Angle(int dx, int dy)
|-> static int calc_angle_do(unsigned int dx, unsigned int dy)
static const uint8_t tbl_math_tan_0[]
static const uint8_t tbl_math_tan_22[]
static const uint16_t tbl_math_tan_45[]
static const uint16_t tbl_math_tan_67[]
static const uint16_t tbl_math_sin[]
static const uint16_t tbl_math_cos[]

*/
#ifndef ANGLE_H
#define ANGLE_H

#include "MOX_TYPE.h"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  seg034
*/

// WZD s34p01
// int16_t Get_Angle(int16_t dx, int16_t dy);
int16_t Get_Angle(int x_dist, int y_dist);

// WZD s34p02
// int16_t calc_angle_do(int16_t dx, int16_t dy);
int16_t Get_Base_Angle(unsigned int y, unsigned int x);

// WZD s34p03
int16_t Cos(int16_t angle, int16_t radius);

// WZD s34p04
int16_t Sin(int16_t angle, int16_t radius);

// WZD s34p05
// drake178: VGA_DrawTexture()

// WZD s34p06
// drake178: VGA_DrawTexture_R()



#ifdef __cplusplus
}
#endif

#endif  /* ANGLE_H */
