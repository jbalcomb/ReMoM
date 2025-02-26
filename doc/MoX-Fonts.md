


TODO  move Print() notes here to MoX-Print.md




## Load_Font_File()

void Load_Font_File(char * font_file)



## Load_Palette()

void Load_Palette(int entry, int start_color, int end_color)

    palette_data = LBX_Reload(font_name, entry+2, palette_block);
    font_colors = &palette_data[768];
    // UU_gsa_Palette_Data = (palette_data + (16 * (48 + 16)));  // 400h
    mouse_palette = (palette_data + (16 * (48 + 16 + 16)));  // 1280
    remap_colors = (palette_data + (16 * (48 + 16 + 16 + 256)));  // FONTS.LBX, 2; @0x1500  5376
    for(itr = 0; itr < (color_count * 3); itr++)
        *(p_Palette + (color_start * 3) + itr) = *(palette_data + (color_start * 3) + itr);
    Set_Font_Style(0, 0, 0, 0);
    if(start_color == ST_UNDEFINED)
        Set_Palette_Changes(0, 255);
    else
        Set_Palette_Changes(start_color, end_color);








Fonts
Text
Colors
Outline
Alias

font_colors


font_style_data
font_header
struct s_FONT_HEADER
current_colors[16];
normal_colors[16];
highlight_colors[16];
special_colors[16];


## ¿ data for 8 fonts ?



e.g.,

main()
    Init_Drivers(VIDEO_MODE, MOM_FONT_FILE);
    |-> Load_Font_File(font_file);

    Load_Palette(0, -1, 0);
    |-> Set_Font(0, 0, 0, 0);
    // VGA_SetShades_Grey0()
    // Set_Button_Down_Offsets(1, 1);
    // Cycle_Palette_Color(198, 40, 0, 0, 63, 0, 0, 1);  // (color_num, red_min, green_min, blue_min, red_max, green_max, blue_max, step_value)
    Apply_Palette();

Main_Screen()
    Set_Outline_Color(0);
    Reset_Cycle_Palette_Color();

Main_Screen_Draw()


Main_Screen_Draw_Summary_Window()
    uint8_t colors[16] = {0};
    colors[0] = 198;
    colors[1] = 198;
    colors[2] = 198;
    Set_Font_Colors_15(0, colors);
    Set_Outline_Color(ST_BLACK);
    if(gold >= 0)
    {
        Set_Font_Style_Outline(0, 4, 0, 0);
        Set_Alias_Color(184);
    }
    else
    {
        if(cycle_incomes == ST_UNDEFINED)
        {
            Set_Font_Style_Outline(0, 2, 0, 0);
        }
        else
        {
            Set_Font_Style_Outline(0, 15, 0, 0);
        }
    }

    gold = 142;

    Print_Integer_Right(273, 101, gold);
    Print(278, 101, cstr_Gold);

Print_Integer_Right()
    Print_Right()
        Get_String_Width()
        Print()
            Print_Display()
                Print_String()

Print() sets the full_flag to FALSE, for Print_Display()
only other call to Print_Display() is from Print_Full(), which sets the full_flag to 'right_side'
Print_Display() calls Print_String(), passes the full_flag and sets the change_color_ok_flag to TRUE

Print_Display() handles the shadow/outline
    sets all the current_colors to the outline_color
    sets the draw_alias_flag



## Alias + Shadow/Outline
How does it get to not having a shadow/outline on the corner pixels?

e.g.,
Main_Screen_Draw_Summary_Window()
    gold = 142;
    Print_Integer_Right(273, 101, gold);
    Print(278, 101, cstr_Gold);

BEGIN: Get_String_Width(string = 142)
END: Get_String_Width(string = 142) { width = 9 }

BEGIN: Print_Display(x = 265, y = 101, string = 142, full_flag = 0)

outline_style: 4
(outline_style != 0)
(outline_style != 2)
(outline_style != 1 && outline_style != 3)
(outline_style > 3)

BEGIN: Print_String(x = 265, y = 101, string = 142, change_color_ok_flag = 1, full_flag = 0)
string: 142
character: 49
(draw_alias_flag == ST_FALSE)
BEGIN: Print_Character_No_Alias(x = 265, y = 101, char_num = 49)
BEGIN: Print_Character_No_Alias_ASM(x_start = 265, y_start = 101, width = 1, font_data_offset = 000001D5B8F77597)


Print_Display()
    sets draw_alias_flag to ST_TRUE
    for the call to Print_String()
    for the shadow/outline
    given the flag set, Print_String()
        calls Print_Character(), instead of Print_Character_No_Alias()
    Print_Character() calls Print_Character_ASM()
    Print_Character_ASM() should skip the pixels that have values for aliasing

    so, ...
        in the Font Glyph, there is a color-map value that is used for the aliasing
        and that same color-map value is what the shadow/outline uses

Thrrrrp!
Can't seem to find the perspective that will let my brain make sense of
    `draw_alias_flag == ST_TRUE` calls Print_Character_No_Alias()
    which means "...skip the pixels..."


Meh.
For the skip a pixel in the Print_Character_No_Alias_ASM()
the first one that can be tested for is the upper left corner of the 2 in 142
which would be in the portion that does the outline for (x + 1) (y - 1)
How's that look in the debug?
BEGIN: Main_Screen_Draw_Summary_Window()
Print_Right(): string: 142
            // TODO  DLOG("(outline_style > 3)");
            Print_String(x + 1, y - 1, string, 0, full_flag);  // overdraw right + top
DEBUG: [C:\devel\STU-MoM_Rasm\src\Fonts.C, 433]: BEGIN: Print_String(x = 266, y = 100, string = 142, change_color_ok_flag = 0, full_flag = 0)







Print_Integer_Right(273, 101, gold);
    Print_Right()
        Get_String_Width()
        Print()
            Print_Display()
                Print_String()



Print_Character_ASM()

    ALWAYS uses CURRENT_COLORS

¿ state of CURRENT_COLORS for Print_Integer_Right(273, 101, gold); ?

Load_Font_File()
    font_header/font_style_data is all zeroes

Load_Palette(0, -1, 0); |-> Set_Font(0, 0, 0, 0);
    current, normal, highlight, and special are all font_colors[block 0]
    ...
    palette_data = LBX_Reload(font_name, entry+2, palette_block);
    font_colors = &palette_data[768];
        font_colors[0][0]: 0x07
        font_colors[0][1]: 0x0F


¿ any other touches between Load_Palette() and Main_Screen_Draw_Summary_Window() ?


Main_Screen_Draw_Summary_Window()

    colors[0] = 198;
    colors[1] = 198;
    colors[2] = 198;
    Set_Font_Colors_15(0, colors);
        |-> Set_Font(font_idx, 15, 0, 0);
            sets font_colors[15] to
                font_colors[15][0]: 0xC6
                font_colors[15][1]: 0xC6
                font_colors[15][2]: 0xC6
                font_colors[15][3]: 0x00
                font_colors[15][4]: 0x00
                font_colors[15][5]: 0x00
                font_colors[15][6]: 0x00
                font_colors[15][7]: 0x00
                font_colors[15][8]: 0x00
                font_colors[15][9]: 0x00
                font_colors[15][10]: 0x00
                font_colors[15][11]: 0x00
                font_colors[15][12]: 0x00
                font_colors[15][13]: 0x00
                font_colors[15][14]: 0x00
                font_colors[15][15]: 0x00
            sets current_colors and normal_colors to font_colors[15]
            sets Font Style to Font Style Number 0

    Set_Outline_Color(ST_BLACK);

    if(gold >= 0)

        Set_Font_Style_Outline(0, 4, 0, 0);
            sets current and normal to font_colors block 4
                font_colors[4][0]: 0xB4
                font_colors[4][1]: 0xB4
                font_colors[4][2]: 0xB2
                font_colors[4][3]: 0xB0
                font_colors[4][4]: 0xD1
                font_colors[4][5]: 0xB2
                font_colors[4][6]: 0xB4
                font_colors[4][7]: 0xB4
                font_colors[4][8]: 0xB5
                font_colors[4][9]: 0xB6
                font_colors[4][10]: 0xB6
                font_colors[4][11]: 0xB6
                font_colors[4][12]: 0xB7
                font_colors[4][13]: 0x0E
                font_colors[4][14]: 0x0F
                font_colors[4][15]: 0x1C

        Set_Alias_Color(184);  // 184d  B8h  ~"dark gold"
            sets font_colors[itr][0] to alias color
            sets current,normal,highlight,special[0] to alias color

So, ...
    at the call to Print_Integer_Right(273, 101, gold);
    current_colors is
        current_colors[0]: 0xB8
        current_colors[1]: 0xB4
        current_colors[2]: 0xB2
        current_colors[3]: 0xB0
        current_colors[4]: 0xD1
        current_colors[5]: 0xB2
        current_colors[6]: 0xB4
        current_colors[7]: 0xB4
        current_colors[8]: 0xB5
        current_colors[9]: 0xB6
        current_colors[10]: 0xB6
        current_colors[11]: 0xB6
        current_colors[12]: 0xB7
        current_colors[13]: 0x0E
        current_colors[14]: 0x0F
        current_colors[15]: 0x1C


DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3004]: current_colors[0]: 0xB4
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3004]: current_colors[1]: 0xB4
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3004]: current_colors[2]: 0xB2
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3004]: current_colors[3]: 0xB0
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3004]: current_colors[4]: 0xD1
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3004]: current_colors[5]: 0xB2
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3004]: current_colors[6]: 0xB4
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3004]: current_colors[7]: 0xB4
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3004]: current_colors[8]: 0xB5
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3004]: current_colors[9]: 0xB6
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3004]: current_colors[10]: 0xB6
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3004]: current_colors[11]: 0xB6
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3004]: current_colors[12]: 0xB7
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3004]: current_colors[13]: 0x0E
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3004]: current_colors[14]: 0x0F
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3004]: current_colors[15]: 0x1C

DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3013]: current_colors[0]: 0xB4
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3013]: current_colors[1]: 0xB4
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3013]: current_colors[2]: 0xB2
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3013]: current_colors[3]: 0xB0
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3013]: current_colors[4]: 0xD1
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3013]: current_colors[5]: 0xB2
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3013]: current_colors[6]: 0xB4
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3013]: current_colors[7]: 0xB4
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3013]: current_colors[8]: 0xB5
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3013]: current_colors[9]: 0xB6
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3013]: current_colors[10]: 0xB6
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3013]: current_colors[11]: 0xB6
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3013]: current_colors[12]: 0xB7
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3013]: current_colors[13]: 0x0E
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3013]: current_colors[14]: 0x0F
DEBUG: [C:\devel\STU-MoM_Rasm\src\MainScr.C, 3013]: current_colors[15]: 0x1C

Heh. I just hadn't gottent to adding the code to set current,normal,highlight,special.
Still, why does it update by the time it gets to Food and Mana?
¿ cause it does update the font_colors[] blocks and then that gets copied in ?
Yep. on the next call to Set_Font_Style_Outline()
