


NOTE(JimBalcomb,20231226): 
just cleaned up/out everything in the code leftover from way back when I did the crazy color math for the invisible effect
pretty sure most of my notes are actually in scribbles on paper in a folder in the purple MoM_Rasm project box.
ATM, I can't recall much of what I learned or what is or isn't legit OG-MoM.
I know I took a lot from MoO2, 1oom, and Animator, and I'm pretty sure it's all split out into more managable chunks.
Seems to all still be working just fine.
I just added in Update_Remap_Gray_Palette(), but didn't change anything, I'd just neglected to uncomment it.





Update_Remap_Gray_Palette()
    Create_Remap_Palette(0, 0, 0, 0, 50);

Calculate_Remap_Colors()
    Create_Remap_Palette(0, 0, 0, 0, 50);
    for(itr_blocks = 1; itr_blocks < 24; itr_blocks++)
    {
        block = itr_blocks;
        block_ofst = block * 4;  // sizeof: uint8_t r,g,b,%
        red     = GET_1B_OFS(remap_colors, block_ofst + 0);
        green   = GET_1B_OFS(remap_colors, block_ofst + 1);
        blue    = GET_1B_OFS(remap_colors, block_ofst + 2);
        percent = GET_1B_OFS(remap_colors, block_ofst + 3);
        Create_Remap_Palette(block, red, green, blue, percent);
    }

Update_Remap_Color_Range(first_set, last_set)
    if(first_set == 0)
    {
        Create_Remap_Palette(0, 0, 0, 0, 50);  // same as default in Calculate_Remap_Colors()
        first_set = 1;
    }
    for(itr_blocks = first_set; itr_blocks <= last_set; itr_blocks++)  // NOTE: LTE, therefore "inclusive"
    {
        block = itr_blocks;
        block_ofst = block * 4;  // sizeof: uint8_t r,g,b,%
        red     = GET_1B_OFS(remap_colors, block_ofst + 0);
        green   = GET_1B_OFS(remap_colors, block_ofst + 1);
        blue    = GET_1B_OFS(remap_colors, block_ofst + 2);
        percent = GET_1B_OFS(remap_colors, block_ofst + 3);
        Create_Remap_Palette(block, red, green, blue, percent);
    }



Create_Remap_Palette()



seg020

seg021




// WZD s20p02
// drake178: VGA_ResetShade0()
// MoO2: ~ Calculate_Remap_Gray_Palettes_()


// WZD s20p03
// drake178: VGA_SetShades_Grey0()
// MoO2: ~ Update_Glass_Remap_Colors()
// fills out the Replacement_Colors@ table by combining Shading_Colors@ with any changed palette colors table 0 is set to shades of 50% black instead of whatever the palette originally had (the same)
// MoO2: Calculate_Remap_Gray_Palettes_
    // 1oom: lbxpal_build_colortables();
    // MoO2: Calculate_Picture_Remap_Colors() |-> Create_Picture_Remap_Palette()
// ¿ remaps colors based on the on the data in the screen buffer ? ¿ for animations, pictures, bitmaps, and sprites ?
// drake178: Replacement_Colors@ table by combining
// drake178: Shading_Colors@
// gsa_ShadingColors ~== glass_colors
// Replacement_Colors ~== glass_remap_colors[16][256]


// WZD s20p04
// drake178: VGA_SetShades_Range()
// MoO2: ~ Update_Glass_Remap_Color_Range()
// fills out the Replacement_Colors@ table by combining Shading_Colors@ with any changed palette colors,
//   but only for the color sets indicated by the confines table 0 is set to shades of 50% black instead of whatever the palette originally had, if included


// WZD s21p06
// drake178: VGA_ShadeColorMatch
// MoO2: 
// 1oom: lbxpal_build_colortable()
// ANI: make_cfit_table()
/*
    dst red = (src1 red * vpercent) + (src2 red * percent)
    e.g.,
        25% of src1 and 75% of src2
        25% of red parameter and 75% of screen red
        ¿ 25% of full saturation ~== "brighten" ?

    passed in color is color2 / 2nd portion of mix/blend

    calculates color3 from color1 and color2
    finds closest color in current palette
*/

/*
    Create_Remap_Palette()
        Create_Remap_Palette_0()
        Create_Remap_Palette_()
            Find_Closest_Color()
        Create_Remap_Palette_1()
*/
void Create_Remap_Palette(int16_t block, uint8_t red, uint8_t green, uint8_t blue, uint8_t percent)

void Create_Remap_Palette_0(int16_t block)

/*
    takes red, green, blue from the glass_colors{} parameters
    does not use the percent
    sets the entire remap palette to the closest color in the current palette
*/
void Create_Remap_Palette_1(int16_t block, uint8_t red, uint8_t green, uint8_t blue)


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


; drake178: VGA_ShadeColorMatch
; MoO2: ~ Update_Glass_Remap_Colors
; 1oom: lbxpal_build_colortable()
; fills out a replacement color table finding closest
; matches to the indicated percent of the supplied RGB
; color plus the remaining percent of the original
; palette colors
;
; WARNING: ignores all colors that are not marked as
; changed, both as source and as target
; Attributes: bp-based frame

; int __cdecl __far Create_Remap_Palette(int block, int red, int green, int blue, int percent)
proc Create_Remap_Palette far

block= word ptr  6
red= word ptr  8
green= word ptr  0Ah
blue= word ptr  0Ch
percent= word ptr  0Eh

_SI_color_map_idx = si


/*
    Animator
        "match-color red", "...green", "...blue"
        "get [red,green,blue] from current table slot"

*/
uint8_t Find_Closest_Color(uint8_t red, uint8_t green, uint8_t blue)



// TODO  // 1oom: uint8_t lbxpal_find_closest(uint8_t r, uint8_t g, uint8_t b)
// TODO  uint8_t Find_Closest_Color(uint8_t r, uint8_t g, uint8_t b)
// TODO  {
// TODO      uint8_t min_c = 0;
// TODO      int min_dist = 10000;
// TODO      // uint8_t *p = lbxpal_palette;
// TODO      uint8_t *p = p_Palette;
// TODO      for (int i = 0; i < 256; ++i) {
// TODO          int dist;
// TODO          dist = abs(r - *p++);
// TODO          dist += abs(g - *p++);
// TODO          dist += abs(b - *p++);
// TODO          if (dist < min_dist) {
// TODO              min_dist = dist;
// TODO              min_c = i;
// TODO              if (dist == 0) {
// TODO                  break;
// TODO              }
// TODO          }
// TODO      }
// TODO      return min_c;
// TODO  }

