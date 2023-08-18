




Create_Remap_Palette()
        Create_Remap_Palette_0()
        Create_Remap_Palette_()
            Find_Closest_Color()
        Create_Remap_Palette_1()

void Create_Remap_Palette_(int16_t block, uint8_t red, uint8_t green, uint8_t blue, uint8_t percent)

color 1
color 2
mix/blend
percent and 100 - percent

MoO2 tint_red = red * percent
¿ "dd_red" ... (palette_red - red) OR (dif_red * dif_red) ?

src1 src2 dst
match color


How to get lighter/brighter for Invisibility?
paramters are 63,63,63,25%
add 25% of 63 to 75% of existing
src1 * (1 - percent) * src2 * percent





Draw_Unit_StatFig()
    // TODO: add contstants for the color index and color count
    for(itr_color_remap = 0; itr_color_remap < 5; itr_color_remap++)
    {
        Replace_Color(UnitDraw_WorkArea, 214 + itr_color_remap, COL_Banners[ ((banner_idx * 5) + itr_color_remap) ]);
    }
// WZD dseg:00AA
//  COL_Banners_P0  105, 106, 107, 108, 109
//  COL_Banners_P1   73,  74,  75,  76,  77
//  COL_Banners_P2  121, 122, 123, 124, 125
//  COL_Banners_P3  201, 165, 203, 166,  45
//  COL_Banners_P4  160, 161, 162, 178, 180
//  COL_Banners_P5   48,  49,  50,  51,  52
uint8_t COL_Banners[] = {
    0x69, 0x6A, 0x6B, 0x6C, 0x6D,
    0x49, 0x4A, 0x4B, 0x4C, 0x4D,
    0x79, 0x7A, 0x7B, 0x7C, 0x7D,
    0xC9, 0xA5, 0xCB, 0xA6, 0x2D,
    0xA0, 0xA1, 0xA2, 0xB2, 0xB4,
    0x30, 0x31, 0x32, 0x33, 0x34
};






related?
VGA_Fade_Out() loops from 0 to 100 in steps of 25



Replace Colors
Remap Colors


Calculate_Remap_Colors()
    |-> Create_Remap_Palette(int16_t block, uint8_t red, uint8_t green, uint8_t blue, uint8_t percent)

¿ "Color Image Quantization for Frame Buffer Display" ?


Create_Remap_Palette(int16_t block, uint8_t red, uint8_t green, uint8_t blue, uint8_t percent)

(100 - percent)
(100 - {0,...,100}) = {100,...0}
¿ light/darken ... color similarity threshold ?

~== percent <   0
~== percent > 100
~== 0 < percent < 100

    if(vpercent > 0)
    {
        if(vpercent < 100)  /* vpercent > 0 && vpercent < 100 */
        {

        }
        else  /* vpercent > 0 && vpercent >= 100 */
        {

        }
    }
    else  /* vpercent <= 0 */
    {

    }

~== percent <   0       map all to one
~== percent > 100       map to self
~== 0 < percent < 100   map each to closest


{63,63,63,25}
percent = 25
vpercent = (100 - percent) = 75

// e.g., 63 * (25 * 256) / 100 = 63 * 6400 / 100 = 403200 / 100 = 4032 / 256 = 15.75
red1   = ((red   * ((percent * 256) / 100)) >> 8);
green1 = ((green * ((percent * 256) / 100)) >> 8);
blue1  = ((blue  * ((percent * 256) / 100)) >> 8);

((percent * 256) / 100))
(25 * 256) / 100 = 6400 / 100 = 64
¿ just fixed point arithmetic trickery ?
(* 256) ~== (>> 8)
¿ mapping percent to value range of palette index / RGB values ?
( 1 * 256) / 100 =   256 / 100 = 2.56
(99 * 256) / 100 = 25344 / 100 = 253.44
...
then multiplied by RGB {0, .., 63}

Something like, ...
    blended red   = alpha * color1 red   + (1 - alpha) * color2 red
    blended green = alpha * color1 green + (1 - alpha) * color2 green
    blended blue  = alpha * color1 blue  + (1 - alpha) * color2 blue

color1 multiplier
color2 multiplier

...

...blend/mix color1 and color2...then, find closest color in current palette...
take percent of color2
take vpercent of color1
using fixed point math and optimal 386 instructions
color2 multiplier
color1 multiplier
intermediate variables
red2b = red2 * color2 multiplier
red1b = red1 * color1 multiplier
red3 = red1b + red2b


logically is equivalent to material on blending two colors
just so happens the 2nd color is always the same color
percent ==    0 ~== reset/default
percent == 100 ~=== IDK, yet.
1 <= percent <= 99 ~== blend/mix passed color as color2, with every color in the current palette, and find the closest existing color in the current palette

Find Closest Color
    diff = ABS(palette - blended)
    if !(diff > threshold)


BEGIN: Create_Remap_Palette(block = 1, red = 3F, green = 3F, blue = 3F, percent = 25)
DBG_Create_Remap_Palette: 1
(vpercent > 0)
(vpercent < 100)
color1_multiplier: 192
color2_multiplier: 64
color2_red_portion: 15
color2_green_portion: 15
color2_blue_portion: 15
color3_red: 33
color3_green: 25
color3_blue: 41










subtracts the passed in color value from the color value in each color in the palette

Why is it checking the Palette Changed Flags?







Draw_Unit_Picture()
    FLIC_Draw(x, y, _unit_colored_backgrounds_seg[unit_colored_backgrounds_idx]);
    Draw_Unit_StatFig(x, y, unit_idx, flag);
        FLIC_Set_CurrentFrame(_unit_type_table[unit_type_idx].pict_seg, 0);
        FLIC_Set_CurrentFrame(_unit_type_table[unit_type_idx].pict_seg, 1);
        Draw_Picture_To_Bitmap(_unit_type_table[unit_type_idx].pict_seg, UnitDraw_WorkArea);
        Replace_Color(UnitDraw_WorkArea, itr_color_remap + 214, COL_Banners[ ((banner_idx * 5) + itr_color_remap) ]);
        *Invisibility*
            Replace_Color_All(UnitDraw_WorkArea, 233); // sets every non-transparent pixel; 233 means use remap colors - (233-232=1) remap color block 1
            Outline_Bitmap_Pixels(UnitDraw_WorkArea, ST_BLACK);
        *Inactive*
            Gray_Scale_Bitmap(UnitDraw_WorkArea, 1);
        Draw_Unit_Enchantment_Outline(unit_idx);
        FLIC_Set_LoopFrame_1(UnitDraw_WorkArea);  // sets for/enables remap colors
        Draw_Picture(x+1, y+1, UnitDraw_WorkArea);
            Draw_Picture_ASM(x, y, SZ_FLIC_HDR, pict_seg, width, height, 0);
                Remap_Draw_Picture_ASM(x_start, y_start, ofst, pict_seg, width, height, skip_x);

        *Actiity Mark*
