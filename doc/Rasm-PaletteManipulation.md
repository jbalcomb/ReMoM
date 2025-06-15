
Where did I put my notes?





tinting is a type of color panipulation
brighten - add white
darken - add black


R,G,B

...apply a tint...

new red = current red + (255 - current red) * tint factor





_AX_ = 100
_AX_ - percent
if _AX_ > 0
if _AX_ - 100 is negative


e.g.,
    percent == 40
    vpercent = (100 -  40) == 60
     60 - 100 = 40

    percent == 100
    vpercent = (100 - 100) ==  0
      0 - 100 = -100

    percent == 40
    vpercent = (100 -   0) == 100
    100 - 100 = 0



C:\STU\devel\ReMoO1\src\lbxpal.c
static void lbxpal_build_colortable(int ctablei, uint8_t pr, uint8_t pg, uint8_t pb, uint8_t percent)
            temp_scale = (v * 256) / 100;
            temp_mul = (percent * 256) / 100;
            r_add = (pr * temp_mul) >> 8;
            g_add = (pg * temp_mul) >> 8;
            b_add = (pb * temp_mul) >> 8;
                r = (((*p++ * temp_scale) >> 8) + r_add) & 0xff;
                g = (((*p++ * temp_scale) >> 8) + g_add) & 0xff;
                b = (((*p++ * temp_scale) >> 8) + b_add) & 0xff;



multiplier = (100 - percent) * 256 / 100

100 - 40 = 60
60 * 256 = 15360
15360 / 100 = 153.6

scale = percent * 256 / 100

40 * 256 = 10240
10240 / 100 = 102.4

new red = red * scale

63 * 102 = 6426

00011001 00011010
mov     al, ah
xor     ah, ah
00000000 00011001
25
~== >> 8








// WZD s21p03
// drake178: VGA_ShadeScreen()
// WZD s21p04
// drake178: VGA_SlideColors()

// WZD s21p06
// drake178: VGA_ShadeColorMatch
// MoO2: 
// 1oom: lbxpal_build_colortable()
// ANI: make_cfit_table()
void Create_Remap_Palette(int16_t block, uint8_t red, uint8_t green, uint8_t blue, uint8_t percent)
/*
    builds a color-map index table of indicies that remaps existing color to other existing colors that match based on the parameters

    blend/mix two colors
        ~ alpha-blending
    dst color = src1 color * (100 - percent) + src2 color * percent
    color2 passed in
    color1 from current palette

    e.g., {63,63,63,25} ... add 25% of full saturation, for each of R, G, B

    NOTE: The Assembly code has a lot of instances of only using the hi- or lo-byte.

*/
void Create_Remap_Palette_(int16_t block, uint8_t red, uint8_t green, uint8_t blue, uint8_t percent)
...
    color2_red = red;
    color2_grn = green;
    color2_blu = blue;

    vpercent = (100 - percent);

    color2_multiplier = (( percent << 8) / 100) & 0x00FF;  // ¿ Fixed_Point Math ~== << 8 ?
    color1_multiplier = ((vpercent << 8) / 100) & 0x00FF;  // ¿ Fixed_Point Math ~== << 8 ?

    // MoO2: tint_red = glass_colors[].red * glass_colors[].percent
    color2_red_portion = ((color2_red * color2_multiplier) >> 8);
    color2_grn_portion = ((color2_grn * color2_multiplier) >> 8);
    color2_blu_portion = ((color2_blu * color2_multiplier) >> 8);

