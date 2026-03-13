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

#include "MOX_TYPE.h"
#include "angle.h"



// WZD seg034:0006
// GEMINI
/* Tangent values for angles 0 to 44 degrees.
 * Represents tan(angle) * 256. 
 * Because tan(45) = 1, all values below 45 degrees fit in a single byte (< 256).
 */
const unsigned char Tangents_0_44[45] = {
      0,   4,   8,  13,  17,  22,  26,  31,  35,
     40,  45,  49,  54,  59,  63,  68,  73,  78,
     83,  88,  93,  98, 103, 108, 113, 119, 124,
    130, 136, 141, 147, 153, 159, 166, 172, 179,
    185, 192, 200, 207, 214, 222, 230, 238, 247
};

// WZD seg034:0033
const unsigned short angle_to_sin[] = { 0, 1143, 2287, 3429, 4571, 5711, 6850, 7986, 9120, 10252, 11380, 12504, 13625, 14742, 15854, 16962, 18064, 19160, 20251, 21336, 22414, 23486, 24550, 25607, 26655, 27696, 28729, 29752, 30767, 31772, 32768, 33753, 34728, 35693, 36647, 37589, 38521, 39440, 40348, 41243, 42125, 42995, 43852, 44695, 45525, 46341, 47142, 47930, 48702, 49460, 50203, 50931, 51643, 52339, 53019, 53684, 54331, 54963, 55577, 56175, 56755, 57319, 57864, 58393, 58903, 59395, 59870, 60326, 60763, 61183, 61583, 61965, 62328, 62672, 62997, 63302, 63589, 63856, 64103, 64331, 64540, 64729, 64898, 65047, 65177, 65286, 65376, 65446, 65496, 65526 };
// WZD seg034:00E7
// GEMINI
/* Tangent values for angles 45 to 89 degrees.
 * Represents tan(angle) * 256.
 * Values quickly exceed 255, requiring 16-bit words.
 */
const unsigned short Tangents_45_89[45] = {
      256,   265,   274,   284,   294,   305,
      316,   327,   339,   352,   365,   379,
      394,   409,   426,   443,   461,   481,
      502,   524,   548,   574,   603,   633,
      666,   703,   743,   787,   837,   892,
      955,  1026,  1108,  1204,  1317,  1451,
     1616,  1821,  2085,  2435,  2926,  3661,
     4885,  7331, 14669
};
// WZD seg034:0141
const unsigned short angle_to_cos[] = { 65526, 65496, 65446, 65376, 65286, 65176, 65047, 64898, 64729, 64540, 64331, 64103, 63856, 63589, 63302, 62997, 62672, 62328, 61965, 61583, 61183, 60763, 60326, 59870, 59395, 58903, 58392, 57864, 57319, 56755, 56175, 55577, 54963, 54331, 53683, 53019, 52339, 
51643, 50930, 50203, 49460, 48702, 47929, 47142, 46340, 45525, 44695, 43852, 42995, 42125, 41243, 40347, 39440, 38520, 37589, 36647, 35693, 34728, 33753, 32767, 31772, 30767, 29752, 28728, 27696, 26655, 25606, 24550, 23485, 22414, 21336, 20251, 19160, 18063, 16961, 15854, 14742, 13625, 12504, 11379, 10251, 9120, 7986, 6850, 5711, 4571, 3429, 2286, 1143, 0 };

/*
    BEGIN:  1oom
*/
/* floor(0x100 * tan((2 * PI * (i + FIRST)) / 360)) */
static const uint8_t tbl_math_tan_0[] = {
    0x00,0x04,0x08,0x0d,0x11,0x16,0x1a,0x1f,0x23,0x28,0x2d,0x31,0x36,0x3b,0x3f,0x44,0x49,0x4e,0x53,0x58,0x5d,0x62
};
static const uint8_t tbl_math_tan_22[] = {
    0x67,0x6c,0x71,0x77,0x7c,0x82,0x88,0x8d,0x93,0x99,0x9f,0xa6,0xac,0xb3,0xb9,0xc0,0xc8,0xcf,0xd6,0xde,0xe6,0xee,0xf7
};
static const uint16_t tbl_math_tan_45[] = {
    0x0100,0x0109,0x0112,0x011c,0x0126,0x0131,0x013c,0x0147,0x0153,0x0160,0x016d,0x017b,0x018a,0x0199,0x01aa,0x01bb,
    0x01cd,0x01e1,0x01f6,0x020c,0x0224,0x023e
};
static const uint16_t tbl_math_tan_67[] = {
    0x025b,0x0279,0x029a,0x02bf,0x02e7,0x0313,0x0345,0x037c,0x03bb,0x0402,0x0454,0x04b4,0x0525,0x05ab,0x0650,0x071d,
    0x0825,0x0983,0x0b6e,0x0e4d,0x1315,0x1ca3,0x394d
};
/* The values of these two differ slightly, probably not important */
static const uint16_t tbl_math_sin[] = {
    0x0000,0x0477,0x08ef,0x0d65,0x11db,0x164f,0x1ac2,0x1f32,0x23a0,0x280c,0x2c74,0x30d8,0x3539,0x3996,0x3dee,0x4242,
    0x4690,0x4ad8,0x4f1b,0x5358,0x578e,0x5bbe,0x5fe6,0x6407,0x681f,0x6c30,0x7039,0x7438,0x782f,0x7c1c,0x8000,0x83d9,
    0x87a8,0x8b6d,0x8f27,0x92d5,0x9679,0x9a10,0x9d9c,0xa11b,0xa48d,0xa7f3,0xab4c,0xae97,0xb1d5,0xb505,0xb826,0xbb3a,
    0xbe3e,0xc134,0xc41b,0xc6f3,0xc9bb,0xcc73,0xcf1b,0xd1b4,0xd43b,0xd6b3,0xd919,0xdb6f,0xddb3,0xdfe7,0xe208,0xe419,
    0xe617,0xe803,0xe9de,0xeba6,0xed5b,0xeeff,0xf08f,0xf20d,0xf378,0xf4d0,0xf615,0xf746,0xf865,0xf970,0xfa67,0xfb4b,
    0xfc1c,0xfcd9,0xfd82,0xfe17,0xfe99,0xff06,0xff60,0xffa6,0xffd8,0xfff6
};
static const uint16_t tbl_math_cos[] = {
    0xfff6,0xffd8,0xffa6,0xff60,0xff06,0xfe98,0xfe17,0xfd82,0xfcd9,0xfc1c,0xfb4b,0xfa67,0xf970,0xf865,0xf746,0xf615,
    0xf4d0,0xf378,0xf20d,0xf08f,0xeeff,0xed5b,0xeba6,0xe9de,0xe803,0xe617,0xe418,0xe208,0xdfe7,0xddb3,0xdb6f,0xd919,
    0xd6b3,0xd43b,0xd1b3,0xcf1b,0xcc73,0xc9bb,0xc6f2,0xc41b,0xc134,0xbe3e,0xbb39,0xb826,0xb504,0xb1d5,0xae97,0xab4c,
    0xa7f3,0xa48d,0xa11b,0x9d9b,0x9a10,0x9678,0x92d5,0x8f27,0x8b6d,0x87a8,0x83d9,0x7fff,0x7c1c,0x782f,0x7438,0x7038,
    0x6c30,0x681f,0x6406,0x5fe6,0x5bbd,0x578e,0x5358,0x4f1b,0x4ad8,0x468f,0x4241,0x3dee,0x3996,0x3539,0x30d8,0x2c73,
    0x280b,0x23a0,0x1f32,0x1ac2,0x164f,0x11db,0x0d65,0x08ee,0x0477,0x0000
};
/*
    END:  1oom
*/



/*
    WIZARDS.EXE  seg034
*/

// WZD s34p01
// drake178: MATH_Inverse_Tangent()
// MoO2  Module: angle  Get_Angle()
// 1oom:  util_math.c  util_math_calc_angle()
/*
; returns the angle corresponding to a given pair of
; adjacent and opposite side lengths using only integer
; maths (byte fraction) and a lookup table; X_Dist
; represents the adjacent, while Y_Dist the opposite,
; as in the game this is used with coordinates
*/
/*
1oom
*/
// int16_t Get_Angle(int16_t dx, int16_t dy)
// {
//     if(dx >= 0)
//     {
//         if(dy >= 0)
//         {
//             return calc_angle_do(dx, dy);
//         }
//         else
//         {
//             return (360 - calc_angle_do(dx, -dy));
//         }
//     }
//     else
//     {
//         if(dy >= 0)
//         {
//             return (180 - calc_angle_do(-dx, dy));
//         }
//         else
//         {
//             return (180 + calc_angle_do(-dx, -dy));
//         }
//     }
// }
/*
GEMINI
This assembly code is an implementation of atan2() (arctangent with two arguments). It calculates the angle of a 2D vector (X, Y) from $0^\circ$ to $359^\circ$.
While the underlying math is definitively an Inverse Tangent (atan2), naming it Get_Angle() tells you exactly what the function is used for in the context of the game engine.
*/
/* Main Function: Returns a full 0-359 degree angle based on X and Y distances */
int16_t Get_Angle(int x_dist, int y_dist) {
    int angle;
    if (x_dist >= 0) {
        if (y_dist >= 0) {
            /* Quadrant 1 (Bottom-Right, assuming Y goes down on screen) */
            angle = Get_Base_Angle(y_dist, x_dist);
        } else {
            /* Quadrant 4 (Top-Right) */
            angle = Get_Base_Angle(-y_dist, x_dist);
            angle = 360 - angle;
        }
    } else {
        if (y_dist >= 0) {
            /* Quadrant 2 (Bottom-Left) */
            angle = Get_Base_Angle(y_dist, -x_dist);
            angle = 180 - angle;
        } else {
            /* Quadrant 3 (Top-Left) */
            angle = Get_Base_Angle(-y_dist, -x_dist);
            angle += 180;
        }
    }
    return angle;
}


// WZD s34p02
// drake178: Get_Base_Angle()
// 1oom:  util_math.c  calc_angle_do()
/*
; returns the angle corresponding to a given pair of
; adjacent (passed in bx) and opposite (passed in ax)
; side lengths using integer maths and a lookup table
; for quadrant I (both lengths positive)
*/
/*
1oom
*/
// int16_t calc_angle_do(int16_t dx /* _BX_ */, int16_t dy)
// {
//     uint16_t slope;
//     int16_t angle;
//     if(dx == 0)
//     {
//         return 90;
//     }
//     slope = (dy << 8) / dx;
//     if(slope < 256)
//     {
//         if(slope >= 103)
//         {
//             angle = 22;
//             for(int i = 0; i < 23; ++i)
//             {
//                 if(slope < tbl_math_tan_22[i])
//                 {
//                     goto out2;
//                 }
//                 ++angle;
//             }
//             angle = 44;
//             out2:
//             --angle;
//         }
//         else
//         {
//             angle = 0;
//             for(int i = 0; i < 22; ++i)
//             {
//                 if(slope < tbl_math_tan_0[i])
//                 {
//                     goto out1;
//                 }
//                 ++angle;
//             }
//             angle = 22;
//             out1:
//             --angle;
//         }
//     }
//     else
//     {
//         if(slope >= 603)  /* ¿ BUGBUG Kilgore typo - 0x25B, not 0x258 ? */
//         {
//             angle = 0;
//             for(int i = 0; i < 23; ++i)
//             {
//                 if(slope < tbl_math_tan_67[i])
//                 {
//                     goto out4;
//                 }
//                 ++angle;
//             }
//             return 90;
//             out4:
//             --angle;
//             angle += 67;
//         }
//         else
//         {
//             angle = 0;
//             for(int i = 0; i < 22; ++i)
//             {
//                 if(slope < tbl_math_tan_45[i])
//                 {
//                     goto out3;
//                 }
//                 ++angle;
//             }
//             return 66;
//             out3:
//             --angle;
//             angle += 45;
//         }
//     }
//     return angle;
// }
// GEMINI
/* Helper Function: Calculates the angle in Quadrant 1 (0 to 90 degrees) */
int16_t Get_Base_Angle(unsigned int y, unsigned int x) {
    unsigned int ratio;
    int i;

    /* Prevent divide-by-zero: perfectly vertical means 90 degrees */
    if (x == 0) {
        return 90;
    }

    /* Fixed-point math: (Y * 256) / X
     * The assembly achieves this by shifting the AX register left by 8 bits 
     * before dividing by BX.
     */
    ratio = (y << 8) / x;

    /* The assembly optimizes the lookup by splitting the arrays in half 
     * so it never has to scan more than 23 values to find the angle. 
     */
    if (ratio < 256) {
        /* Angle is between 0 and 44 degrees */
        if (ratio < 103) {
            /* Scan 0 to 21 */
            for (i = 0; i <= 21; i++) {
                if (ratio < Tangents_0_44[i]) return i - 1 < 0 ? 0 : i - 1;
            }
            return 21;
        } else {
            /* Scan 22 to 44 */
            for (i = 22; i <= 44; i++) {
                if (ratio < Tangents_0_44[i]) return i - 1;
            }
            return 44;
        }
    } else {
        /* Angle is between 45 and 89 degrees */
        if (ratio < 603) {
            /* Scan 45 to 66 */
            for (i = 0; i <= 21; i++) {
                if (ratio < Tangents_45_89[i]) return i + 45 - 1;
            }
            return 66; /* 45 + 21 */
        } else {
            /* Scan 67 to 89 */
            for (i = 22; i <= 44; i++) {
                if (ratio < Tangents_45_89[i]) return i + 45 - 1;
            }
            return 89;
        }
    }
}


// WZD s34p03
/*
GEMINI
*/
/* Assuming angle_to_cos is a global array of 91 unsigned shorts */
// extern unsigned short angle_to_cos[91];
int16_t Cos(int16_t angle, int16_t radius) {
    int sign = 1;
    unsigned short multiplier;
    long result;

    /* Fold the 360-degree angle into the 0-90 degree quadrant */
    if (angle > 90 && angle <= 180) {
        angle = 180 - angle;
        sign = -1;
    } else if (angle > 180 && angle <= 270) {
        angle = angle - 180;
        sign = -1;
    } else if (angle > 270 && angle <= 360) {
        angle = 360 - angle;
        /* sign remains positive */
    }

    /* Look up the fixed-point fraction (0 to 65535) */
    multiplier = angle_to_cos[angle];

    /* Multiply and shift right by 16 (equivalent to assembly's "mov ax, dx") */
    result = ((long)radius * multiplier) >> 16;

    if (sign == -1) {
        return (int)(-result);
    }
    return (int)result;
}


// WZD s34p04
// drake178: Sin()
/*
*/
/*
GEMINI
*/
/* Assuming angle_to_cos is a global array of unsigned shorts */
// extern unsigned short angle_to_cos[90]; 
int16_t Sin(int16_t angle, int16_t radius) {
    int sign = 1; /* 1 for positive, -1 for negative */
    unsigned short multiplier;
    long result;

    /* 1. Fold the 360-degree angle into the 0-90 degree quadrant */
    if (angle > 90 && angle <= 180) {
        /* Quadrant 2: Sine is positive, angle counts backward from 180 */
        angle = 180 - angle;
    } else if (angle > 180 && angle <= 270) {
        /* Quadrant 3: Sine is negative, angle counts forward from 180 */
        angle = angle - 180;
        sign = -1;
    } else if (angle > 270 && angle <= 360) {
        /* Quadrant 4: Sine is negative, angle counts backward from 360 */
        angle = 360 - angle;
        sign = -1;
    }

    /* 2. The 90-Degree Shortcut */
    /* If the angle is exactly 90 (or 270, which folds to 90), sin(90) = 1.0. 
       1.0 * radius is just radius. The assembly skips the math here. */
    if (angle == 90) {
        if (sign == -1) {
            return -radius;
        }
        return radius;
    }

    /* 3. Look up the fixed-point fraction (0 to 65535) */
    multiplier = angle_to_cos[angle];

    /* 4. Multiply and take the upper 16 bits (divide by 65536) */
    /* Note: In C, standard signed long math handles negative radii natively, 
       whereas the assembly had to manually do abs(radius) to use the 'mul' instruction. */
    result = ((long)radius * multiplier) >> 16;

    /* 5. Apply the quadrant sign */
    if (sign == -1) {
        return (int)(-result);
    }
    
    return (int)result;
}


// WZD s34p05
// drake178: VGA_DrawTexture()
/*
*/
/*
draw rotated?
*/
// WZD s34p06
// drake178: VGA_DrawTexture_R()
/*
*/
/*
remap draw rotated?
*/
