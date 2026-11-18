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

#include "Fonts.h"
#include "MOX_DEF.h"
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
    OG is hand-written asm; Get_Base_Angle() takes its arguments in registers (AX = opposite, BX = adjacent).
    Returns 0 .. 360: a small negative y_dist with x_dist >= 0 gives 360 - 0 = 360, not 0.
*/
int16_t Get_Angle(int16_t x_dist, int16_t y_dist)
{
    if(x_dist >= 0)
    {
        if(y_dist < 0)
        {
            return (360 - Get_Base_Angle(-y_dist, x_dist));
        }
        return Get_Base_Angle(y_dist, x_dist);
    }
    else
    {
        if(y_dist < 0)
        {
            return (Get_Base_Angle(-y_dist, -x_dist) + 180);
        }
        return (180 - Get_Base_Angle(y_dist, -x_dist));
    }
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
/*
    OG is hand-written asm; called with AX = y_dist (opposite), BX = x_dist (adjacent), both already non-negative.
    ratio = (y_dist * 256) / x_dist; Tangents_0_44[] and Tangents_45_89[] hold tan(0) .. tan(44) and tan(45) .. tan(89) in the same 8.8 fixed point.
    OGBUG: a ratio of 247 .. 255 (44 to 45 degrees) returns 43; that scan's fall-through sets di = 44 and then decrements it.
    A ratio of 14669 and up (89 to 90 degrees) returns 90, through the same exit as x_dist == 0.
*/
int16_t Get_Base_Angle(uint16_t y_dist, uint16_t x_dist)
{
    // uint16_t ratio;
    uint32_t ratio;  /* CLAUDE */  // OG 'div bx' traps (divide overflow) when y_dist >= (256 * x_dist); a 32-bit ratio runs off the tables to the 90 exit instead
    int16_t angle;              // _DI_
    int16_t itr;                // _CX_

    if(x_dist == 0)
    {
        return 90;
    }

    ratio = (((uint32_t)y_dist << 8) / x_dist);

    if(ratio < 256)
    {
        if(ratio < 103)
        {
            angle = 0;
            for(itr = 0; itr < 22; itr++)
            {
                if(ratio < Tangents_0_44[itr])
                {
                    return (angle - 1);
                }
                angle++;
            }
            angle = 22;
            return (angle - 1);
        }
        else
        {
            angle = 22;
            for(itr = 0; itr < 23; itr++)
            {
                if(ratio < Tangents_0_44[(22 + itr)])
                {
                    return (angle - 1);
                }
                angle++;
            }
            angle = 44;
            return (angle - 1);
        }
    }
    else
    {
        if(ratio < 603)
        {
            angle = 0;
            for(itr = 0; itr < 22; itr++)
            {
                if(ratio < Tangents_45_89[itr])
                {
                    return ((angle - 1) + 45);
                }
                angle++;
            }
            return 66;
        }
        else
        {
            angle = 0;
            for(itr = 0; itr < 23; itr++)
            {
                if(ratio < Tangents_45_89[(22 + itr)])
                {
                    return ((angle - 1) + 67);
                }
                angle++;
            }
            return 90;
        }
    }
}


// WZD s34p03
// drake178: Cos()
// MoO2  Cos()
/*
    OG is hand-written asm; the tables are in the code segment.
    angle_to_cos[] holds cos(1) .. cos(90), so it is read at [angle - 1]; angle 0 returns radius unscaled.
    The multiply is unsigned (mul), on |radius|, keeping the high word, so the result rounds toward zero; the sign of radius is put back after.
    Expects 0 .. 360: a negative angle, or one past 360, indexes before the table.
*/
int16_t Cos(int16_t angle, int16_t radius)
{
    int16_t negate;             // _DX_ / _CX_
    uint16_t multiplier;        // _AX_ (lodsw)
    int16_t result;             // _AX_

    negate = ST_FALSE;

    if(angle >= 91)
    {
        if(angle < 181)
        {
            angle = (180 - angle);
            negate = ST_TRUE;
        }
        else if(angle < 271)
        {
            angle = (angle - 180);
            negate = ST_TRUE;
        }
        else
        {
            angle = (360 - angle);
        }
    }

    if(angle == 0)
    {
        result = radius;
    }
    else
    {
        multiplier = angle_to_cos[(angle - 1)];
        if(radius >= 0)
        {
            result = (int16_t)(((uint32_t)(uint16_t)radius * multiplier) >> 16);
        }
        else
        {
            result = -(int16_t)(((uint32_t)(uint16_t)-radius * multiplier) >> 16);
        }
    }

    if(negate != ST_FALSE)
    {
        result = -result;
    }

    return result;
}


// WZD s34p04
// drake178: Sin()
// MoO2  Sin()
/*
    OG is hand-written asm; the tables are in the code segment.
    angle_to_sin[] holds sin(0) .. sin(89), so it is read at [angle]; angle 90 returns radius unscaled.
    The multiply is unsigned (mul), on |radius|, keeping the high word, so the result rounds toward zero; the sign of radius is put back after.
    Expects 0 .. 360: a negative angle, or one past 360, indexes before the table.
*/
int16_t Sin(int16_t angle, int16_t radius)
{
    int16_t negate;             // _DX_ / _CX_
    uint16_t multiplier;        // _AX_ (lodsw)
    int16_t result;             // _AX_

    negate = ST_FALSE;

    if(angle >= 91)
    {
        if(angle < 181)
        {
            angle = (180 - angle);
        }
        else if(angle < 271)
        {
            angle = (angle - 180);
            negate = ST_TRUE;
        }
        else
        {
            angle = (360 - angle);
            negate = ST_TRUE;
        }
    }

    if(angle == 90)
    {
        result = radius;
    }
    else
    {
        multiplier = angle_to_sin[angle];
        if(radius >= 0)
        {
            result = (int16_t)(((uint32_t)(uint16_t)radius * multiplier) >> 16);
        }
        else
        {
            result = -(int16_t)(((uint32_t)(uint16_t)-radius * multiplier) >> 16);
        }
    }

    if(negate != ST_FALSE)
    {
        result = -result;
    }

    return result;
}


// WZD s34p05
// drake178: VGA_DrawTexture()
// MoO2  Texture_Map_Block()  (32-bit edge-list rewrite; same role)
/*
    OG is hand-written asm; its state lives in code-segment variables (Skip_Width, Min_Write_Offset, Max_Write_Offset, Y_Vrt_Slope, Y_Down, Y_Hrz_Slope, Y_Left, Vrt_Read_Skip, Up_Error, Horz_Left_Error, Horz_Down_Error, Col_Height_Frac).
    The addressing is kept in VGA Mode Y terms: di is a 16-bit offset into one 80-byte-per-row VGA plane, and the plane is the low two bits of x. Only the write is translated to the linear page buffer.
    That keeps the asm's 16-bit wraparound: a column height of 0 runs 'loop' 65536 times, and 65536 rows of 'di += 80' wrap the whole plane, so the column is drawn top to bottom.
    The clip test runs after 'inc di', so it tests di + 1 against MinOff / MaxOff.
*/
void Texture_Map_Block(TextureRenderParams * params)
{
    uint16_t skip_width;
    uint16_t min_write_offset;
    uint16_t max_write_offset;
    uint16_t up_error;
    uint16_t horz_left_error;
    uint16_t horz_down_error;
    uint16_t col_height_frac;
    uint16_t width;
    uint16_t dest;              // _DI_
    uint16_t source_offset;     // _SI_
    uint8_t plane_mask;         // _AH_
    uint16_t column_dest;
    uint16_t column_source_offset;
    uint16_t row_count;         // _CX_
    uint16_t y_hrz_error;       // _DX_
    uint16_t y_vrt_error;       // _BX_
    uint8_t color;              // _AL_
    int16_t plane;

    skip_width = params->skip_width;
    min_write_offset = params->min_off;
    max_write_offset = params->max_off;

    dest = ((params->start_y * 80) + (params->start_x >> 2));
    plane = (params->start_x & 3);
    plane_mask = (1 << plane);
    source_offset = params->read_off;

    up_error = 128;
    horz_left_error = 128;
    horz_down_error = 128;

    col_height_frac = (params->col1_hgt << 8);

    width = params->width;

    for(;;)
    {
        column_dest = dest;
        column_source_offset = source_offset;

        if((int16_t)skip_width < 0)
        {
            row_count = (col_height_frac >> 8);
            y_hrz_error = 0;
            y_vrt_error = 0;
            do
            {
                color = params->pict_data[source_offset];
                dest++;
                if(
                    (color != 0)
                    &&
                    (dest >= min_write_offset)
                    &&
                    (dest < max_write_offset)
                )
                {
                    dest--;
                    // stosb  ES:DI  plane (map mask) / di
                    if(dest < (80 * SCREEN_HEIGHT))  /* CLAUDE */  // VGA memory past the visible page is not in the linear page buffer
                    {
                        current_video_page[(((dest / 80) * SCREEN_WIDTH) + ((dest % 80) * 4) + plane)] = color;  /* CLAUDE */
                    }
                    dest++;
                }
                dest += 79;
                source_offset += params->v_rskip;
                y_hrz_error += params->y_hslope;
                if((y_hrz_error >> 8) != 0)
                {
                    y_hrz_error &= 0x00FF;
                    source_offset += params->y_left;
                }
                y_vrt_error += params->y_vslope;
                if((y_vrt_error >> 8) != 0)
                {
                    y_vrt_error &= 0x00FF;
                    source_offset += params->y_down;
                }
            } while(--row_count != 0);
        }

        skip_width--;

        dest = column_dest;
        source_offset = column_source_offset;

        if(--width == 0)
        {
            break;
        }

        dest += params->ch_incr;
        up_error += params->up_slope;
        if((up_error >> 8) != 0)
        {
            up_error &= 0x00FF;
            dest += params->nl;
        }

        col_height_frac += params->ch_slope;

        source_offset += params->h_rskip;
        horz_left_error += params->x_hslope;
        if((horz_left_error >> 8) != 0)
        {
            horz_left_error &= 0x00FF;
            source_offset += params->x_left;
        }
        horz_down_error += params->x_vslope;
        if((horz_down_error >> 8) != 0)
        {
            horz_down_error &= 0x00FF;
            source_offset += params->x_down;
        }

        plane_mask <<= 1;
        plane++;
        if((int8_t)(plane_mask - 9) >= 0)
        {
            plane_mask = 1;
            plane = 0;
            dest++;
        }
    }
}


// WZD s34p06
// drake178: VGA_DrawTexture_R()
// MoO2  DNE  (remap variant; named after MoO2's Remap_ prefix convention)
/*
    Same engine as Texture_Map_Block(), plus the remap colors: 0 is transparent, 1-231 are written as is, and 232 and up select block (color - 232) of remap_color_palettes
    (256 bytes each), indexed by the pixel already on the page, so the picture tints or shades whatever is under it.
    The high nibble of AH carries the plane index for the read-map select (3CEh index 4), so the read and the write hit the same pixel; here that is the same linear offset.
*/
void Remap_Texture_Map_Block(TextureRenderParams * params)
{
    uint16_t skip_width;
    uint16_t min_write_offset;
    uint16_t max_write_offset;
    uint16_t up_error;
    uint16_t horz_left_error;
    uint16_t horz_down_error;
    uint16_t col_height_frac;
    uint16_t width;
    uint16_t dest;              // _DI_
    uint16_t source_offset;     // _SI_
    uint8_t plane_mask;         // _AH_ (low nibble)
    uint16_t column_dest;
    uint16_t column_source_offset;
    uint16_t row_count;         // _CX_
    uint16_t y_hrz_error;       // _DX_
    uint16_t y_vrt_error;       // _BX_
    uint8_t color;              // _AL_
    int16_t plane;              // _AH_ (high nibble)
    int32_t page_offset;  /* CLAUDE */

    skip_width = params->skip_width;
    min_write_offset = params->min_off;
    max_write_offset = params->max_off;

    dest = ((params->start_y * 80) + (params->start_x >> 2));
    plane = (params->start_x & 3);
    plane_mask = (1 << plane);
    source_offset = params->read_off;

    up_error = 128;
    horz_left_error = 128;
    horz_down_error = 128;

    col_height_frac = (params->col1_hgt << 8);

    width = params->width;

    for(;;)
    {
        column_dest = dest;
        column_source_offset = source_offset;

        if((int16_t)skip_width < 0)
        {
            row_count = (col_height_frac >> 8);
            y_hrz_error = 0;
            y_vrt_error = 0;
            do
            {
                color = params->pict_data[source_offset];
                dest++;
                if(
                    (color != 0)
                    &&
                    (dest >= min_write_offset)
                    &&
                    (dest < max_write_offset)
                )
                {
                    dest--;
                    // stosb  ES:DI  plane (map mask / read map) / di
                    if(dest < (80 * SCREEN_HEIGHT))  /* CLAUDE */  // VGA memory past the visible page is not in the linear page buffer
                    {
                        page_offset = (((dest / 80) * SCREEN_WIDTH) + ((dest % 80) * 4) + plane);  /* CLAUDE */
                        if(color >= 232)
                        {
                            current_video_page[page_offset] = remap_color_palettes[(((color - 232) * 256) + current_video_page[page_offset])];
                        }
                        else
                        {
                            current_video_page[page_offset] = color;
                        }
                    }
                    dest++;
                }
                dest += 79;
                source_offset += params->v_rskip;
                y_hrz_error += params->y_hslope;
                if((y_hrz_error >> 8) != 0)
                {
                    y_hrz_error &= 0x00FF;
                    source_offset += params->y_left;
                }
                y_vrt_error += params->y_vslope;
                if((y_vrt_error >> 8) != 0)
                {
                    y_vrt_error &= 0x00FF;
                    source_offset += params->y_down;
                }
            } while(--row_count != 0);
        }

        skip_width--;

        dest = column_dest;
        source_offset = column_source_offset;

        if(--width == 0)
        {
            break;
        }

        dest += params->ch_incr;
        up_error += params->up_slope;
        if((up_error >> 8) != 0)
        {
            up_error &= 0x00FF;
            dest += params->nl;
        }

        col_height_frac += params->ch_slope;

        source_offset += params->h_rskip;
        horz_left_error += params->x_hslope;
        if((horz_left_error >> 8) != 0)
        {
            horz_left_error &= 0x00FF;
            source_offset += params->x_left;
        }
        horz_down_error += params->x_vslope;
        if((horz_down_error >> 8) != 0)
        {
            horz_down_error &= 0x00FF;
            source_offset += params->x_down;
        }

        plane_mask <<= 1;
        plane++;
        if((int8_t)(plane_mask - 9) >= 0)
        {
            plane_mask = 1;
            plane = 0;
            dest++;
        }
    }
}
