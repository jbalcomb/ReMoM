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


// seg034:0006 00 04 08 0D 11 16 1A 1F 23 28 2D 31 36 3B 3F 44+Tangents_0_44 db 0, 4, 8, 13, 17, 22, 26, 31, 35, 40, 45, 49, 54, 59, 63, 68, 73, 78, 83, 88, 93, 98, 103, 108, 113, 119, 124, 130, 136, 141, 147, 153, 159, 166, 172, 179, 185, 192, 200, 207, 214, 222, 230, 238, 247
// seg034:0006 49 4E 53 58 5D 62 67 6C 71 77 7C 82 88 8D 93 99+                                        ; DATA XREF: MATH_Inverse_Tan_QI+27t ...
// seg034:0033 00 00 77 04 EF 08 65 0D DB 11 4F 16 C2 1A 32 1F+Sin_Multipliers dw 0, 1143, 2287, 3429, 4571, 5711, 6850, 7986, 9120, 10252, 11380, 12504, 13625, 14742, 15854, 16962, 18064, 19160, 20251, 21336, 22414, 23486, 24550, 25607, 26655, 27696, 28729, 29752, 30767, 31772, 32768, 33753, 34728, 35693, 36647, 37589, 38521, 39440
// seg034:0033 A0 23 0C 28 74 2C D8 30 39 35 96 39 EE 3D 42 42+                                        ; DATA XREF: MATH_Int_Sin+4Eo
// seg034:0033 90 46 D8 4A 1B 4F 58 53 8E 57 BE 5B E6 5F 07 64+dw 40348, 41243, 42125, 42995, 43852, 44695, 45525, 46341, 47142, 47930, 48702, 49460, 50203, 50931, 51643, 52339, 53019, 53684, 54331, 54963, 55577, 56175, 56755, 57319, 57864, 58393, 58903, 59395, 59870, 60326, 60763, 61183, 61583, 61965, 62328, 62672, 62997
// seg034:0033 1F 68 30 6C 39 70 38 74 2F 78 1C 7C 00 80 D9 83+dw 63302, 63589, 63856, 64103, 64331, 64540, 64729, 64898, 65047, 65177, 65286, 65376, 65446, 65496, 65526
// seg034:00E7 00 01 09 01 12 01 1C 01 26 01 31 01 3C 01 47 01+Tangents_45_89 dw 256, 265, 274, 284, 294, 305, 316, 327, 339, 352, 365, 379, 394, 409, 426, 443, 461, 481, 502, 524, 548, 574, 603, 633, 666, 703, 743, 787, 837, 892, 955, 1026, 1108, 1204, 1317, 1451, 1616, 1821, 2085, 2435, 2926, 3661, 4885, 7331, 14669
// seg034:00E7 53 01 60 01 6D 01 7B 01 8A 01 99 01 AA 01 BB 01+                                        ; DATA XREF: MATH_Inverse_Tan_QI+65t ...
// seg034:0141 F6 FF D8 FF A6 FF 60 FF 06 FF 98 FE 17 FE 82 FD+Cos_Multipliers dw 65526, 65496, 65446, 65376, 65286, 65176, 65047, 64898, 64729, 64540, 64331, 64103, 63856, 63589, 63302, 62997, 62672, 62328, 61965, 61583, 61183, 60763, 60326, 59870, 59395, 58903, 58392, 57864, 57319, 56755, 56175, 55577, 54963, 54331, 53683, 53019, 52339
// seg034:0141 D9 FC 1C FC 4B FB 67 FA 70 F9 65 F8 46 F7 15 F6+                                        ; DATA XREF: MATH_Int_Cos+4Fo
// seg034:0141 D0 F4 78 F3 0D F2 8F F0 FF EE 5B ED A6 EB DE E9+dw 51643, 50930, 50203, 49460, 48702, 47929, 47142, 46340, 45525, 44695, 43852, 42995, 42125, 41243, 40347, 39440, 38520, 37589, 36647, 35693, 34728, 33753, 32767, 31772, 30767, 29752, 28728, 27696, 26655, 25606, 24550, 23485, 22414, 21336, 20251, 19160, 18063
// seg034:0141 03 E8 17 E6 18 E4 08 E2 E7 DF B3 DD 6F DB 19 D9+dw 16961, 15854, 14742, 13625, 12504, 11379, 10251, 9120, 7986, 6850, 5711, 4571, 3429, 2286, 1143, 0

// seg034:0006 00 04 08 0D 11 16 1A 1F 23 28 2D 31 36 3B 3F 44+Tangents_0_44 db 0, 4, 8, 0Dh, 11h, 16h, 1Ah, 1Fh, 23h, 28h, 2Dh, 31h, 36h, 3Bh, 3Fh, 44h, 49h, 4Eh, 53h, 58h, 5Dh, 62h, 67h, 6Ch, 71h, 77h, 7Ch, 82h, 88h, 8Dh, 93h, 99h, 9Fh, 0A6h, 0ACh, 0B3h, 0B9h, 0C0h, 0C8h, 0CFh, 0D6h, 0DEh, 0E6h, 0EEh, 0F7h
// seg034:0006 49 4E 53 58 5D 62 67 6C 71 77 7C 82 88 8D 93 99+                                        ; DATA XREF: MATH_Inverse_Tan_QI+27t ...
// seg034:0033 00 00 77 04 EF 08 65 0D DB 11 4F 16 C2 1A 32 1F+Sin_Multipliers dw 0, 477h, 8EFh, 0D65h, 11DBh, 164Fh, 1AC2h, 1F32h, 23A0h, 280Ch, 2C74h, 30D8h, 3539h, 3996h, 3DEEh, 4242h, 4690h, 4AD8h, 4F1Bh, 5358h, 578Eh, 5BBEh, 5FE6h, 6407h, 681Fh, 6C30h, 7039h, 7438h, 782Fh, 7C1Ch, 8000h, 83D9h, 87A8h, 8B6Dh, 8F27h, 92D5h, 9679h, 9A10h
// seg034:0033 A0 23 0C 28 74 2C D8 30 39 35 96 39 EE 3D 42 42+                                        ; DATA XREF: MATH_Int_Sin+4Eo
// seg034:0033 90 46 D8 4A 1B 4F 58 53 8E 57 BE 5B E6 5F 07 64+dw 9D9Ch, 0A11Bh, 0A48Dh, 0A7F3h, 0AB4Ch, 0AE97h, 0B1D5h, 0B505h, 0B826h, 0BB3Ah, 0BE3Eh, 0C134h, 0C41Bh, 0C6F3h, 0C9BBh, 0CC73h, 0CF1Bh, 0D1B4h, 0D43Bh, 0D6B3h, 0D919h, 0DB6Fh, 0DDB3h, 0DFE7h, 0E208h, 0E419h, 0E617h, 0E803h, 0E9DEh, 0EBA6h, 0ED5Bh, 0EEFFh
// seg034:0033 1F 68 30 6C 39 70 38 74 2F 78 1C 7C 00 80 D9 83+dw 0F08Fh, 0F20Dh, 0F378h, 0F4D0h, 0F615h, 0F746h, 0F865h, 0F970h, 0FA67h, 0FB4Bh, 0FC1Ch, 0FCD9h, 0FD82h, 0FE17h, 0FE99h, 0FF06h, 0FF60h, 0FFA6h, 0FFD8h, 0FFF6h
// seg034:00E7 00 01 09 01 12 01 1C 01 26 01 31 01 3C 01 47 01+Tangents_45_89 dw 100h, 109h, 112h, 11Ch, 126h, 131h, 13Ch, 147h, 153h, 160h, 16Dh, 17Bh, 18Ah, 199h, 1AAh, 1BBh, 1CDh, 1E1h, 1F6h, 20Ch, 224h, 23Eh, 25Bh, 279h, 29Ah, 2BFh, 2E7h, 313h, 345h, 37Ch, 3BBh, 402h, 454h, 4B4h, 525h, 5ABh, 650h, 71Dh, 825h, 983h, 0B6Eh, 0E4Dh
// seg034:00E7 53 01 60 01 6D 01 7B 01 8A 01 99 01 AA 01 BB 01+                                        ; DATA XREF: MATH_Inverse_Tan_QI+65t ...
// seg034:00E7 CD 01 E1 01 F6 01 0C 02 24 02 3E 02 5B 02 79 02+dw 1315h, 1CA3h, 394Dh
// seg034:0141 F6 FF D8 FF A6 FF 60 FF 06 FF 98 FE 17 FE 82 FD+Cos_Multipliers dw 0FFF6h, 0FFD8h, 0FFA6h, 0FF60h, 0FF06h, 0FE98h, 0FE17h, 0FD82h, 0FCD9h, 0FC1Ch, 0FB4Bh, 0FA67h, 0F970h, 0F865h, 0F746h, 0F615h, 0F4D0h, 0F378h, 0F20Dh, 0F08Fh, 0EEFFh, 0ED5Bh, 0EBA6h, 0E9DEh, 0E803h, 0E617h, 0E418h, 0E208h, 0DFE7h, 0DDB3h, 0DB6Fh, 0D919h
// seg034:0141 D9 FC 1C FC 4B FB 67 FA 70 F9 65 F8 46 F7 15 F6+                                        ; DATA XREF: MATH_Int_Cos+4Fo
// seg034:0141 D0 F4 78 F3 0D F2 8F F0 FF EE 5B ED A6 EB DE E9+dw 0D6B3h, 0D43Bh, 0D1B3h, 0CF1Bh, 0CC73h, 0C9BBh, 0C6F2h, 0C41Bh, 0C134h, 0BE3Eh, 0BB39h, 0B826h, 0B504h, 0B1D5h, 0AE97h, 0AB4Ch, 0A7F3h, 0A48Dh, 0A11Bh, 9D9Bh, 9A10h, 9678h, 92D5h, 8F27h, 8B6Dh, 87A8h, 83D9h, 7FFFh, 7C1Ch, 782Fh, 7438h, 7038h, 6C30h, 681Fh
// seg034:0141 03 E8 17 E6 18 E4 08 E2 E7 DF B3 DD 6F DB 19 D9+dw 6406h, 5FE6h, 5BBDh, 578Eh, 5358h, 4F1Bh, 4AD8h, 468Fh, 4241h, 3DEEh, 3996h, 3539h, 30D8h, 2C73h, 280Bh, 23A0h, 1F32h, 1AC2h, 164Fh, 11DBh, 0D65h, 8EEh, 477h, 0

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

*/
int16_t Get_Angle(int16_t dx, int16_t dy)
{
    if(dx >= 0)
    {
        if(dy >= 0)
        {
            return calc_angle_do(dx, dy);
        }
        else
        {
            return (360 - calc_angle_do(dx, -dy));
        }
    }
    else
    {
        if(dy >= 0)
        {
            return (180 - calc_angle_do(-dx, dy));
        }
        else
        {
            return (180 + calc_angle_do(-dx, -dy));
        }
    }
}


// WZD s34p02
// drake178: MATH_Inverse_Tan_QI()
// 1oom:  util_math.c  calc_angle_do()
/*
; returns the angle corresponding to a given pair of
; adjacent (passed in bx) and opposite (passed in ax)
; side lengths using integer maths and a lookup table
; for quadrant I (both lengths positive)
*/
/*

TODO  someday, maybe, find this in the old code archive
*/
int16_t calc_angle_do(int16_t dx /* _BX_ */, int16_t dy)
{
    uint16_t slope;
    int16_t angle;

    if(dx == 0)
    {
        return 90;
    }

    slope = (dy << 8) / dx;

    if(slope < 256)
    {
        if(slope >= 103)
        {
            angle = 22;
            for(int i = 0; i < 23; ++i)
            {
                if(slope < tbl_math_tan_22[i])
                {
                    goto out2;
                }
                ++angle;
            }
            angle = 44;
            out2:
            --angle;
        }
        else
        {
            angle = 0;
            for(int i = 0; i < 22; ++i)
            {
                if(slope < tbl_math_tan_0[i])
                {
                    goto out1;
                }
                ++angle;
            }
            angle = 22;
            out1:
            --angle;
        }
    }
    else
    {
        if(slope >= 603)  /* ¿ BUGBUG Kilgore typo - 0x25B, not 0x258 ? */
        {
            angle = 0;
            for(int i = 0; i < 23; ++i)
            {
                if(slope < tbl_math_tan_67[i])
                {
                    goto out4;
                }
                ++angle;
            }
            return 90;
            out4:
            --angle;
            angle += 67;
        }
        else
        {
            angle = 0;
            for(int i = 0; i < 22; ++i)
            {
                if(slope < tbl_math_tan_45[i])
                {
                    goto out3;
                }
                ++angle;
            }
            return 66;
            out3:
            --angle;
            angle += 45;
        }
    }
// TODO  #ifdef FEATURE_MODEBUG
// TODO      if ((angle < 0) || (angle >= 180)) {
// TODO          LOG_DEBUG((3, "%s: dx:%i dy:%i -> slope:%i angle:%i\n", dx, dy, slope, angle));
// TODO      }
// TODO  #endif
    return angle;
}

// WZD s34p03
// drake178: 
/*
*/
/*

*/
// WZD s34p04
// drake178: 
/*
*/
/*

*/
// WZD s34p05
// drake178: 
/*
*/
/*

*/
// WZD s34p06
// drake178: 
/*
*/
/*

*/
