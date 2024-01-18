







Main_Screen_Draw_Summary_Window()

```c
    colors[0] = 198;
    colors[1] = 198;
    colors[2] = 198;
    Set_Font_Colors_15(0, &colors[0])

    Set_Outline_Color(0);

    if(gold >= 0)
    {
        Set_Font_Style_Outline(0, 4, 0, 0);
        Set_Alias_Color(184);
    }
    else
    {
        if(cycle_incomes == ST_UNDEFINED)
        {
            Set_Font_Style_Outline(0, 2, 0, 0);  // disabled
            Set_Alias_Color(0);
        }
        else
        {
            Set_Font_Style_Outline(0, 15, 0, 0);  // enabled
            Set_Alias_Color(0);
        }
    }
```

all font style_num 0
colors {4,0,0}, {2,0,0}, {15,0,0}
¿ normal, highlight, special ?

I don't know what 2,4,15 are.
I don't know what it means to set the highligh and special to 0.

How does setting the color block have anything to do with blinking?


cycle_incomes: {-1,0} {disable,enable}

The text (anti-)alias color is the first color in each color block of 16.



// WZD o64p06
void OVL_DisableIncmBlink(void)
    cycle_incomes = -1;
// WZD o64p07
void OVL_EnableIncmBlink(void)
    cycle_incomes = 0;


## Set_Font_Style_Outline()

```c
Set_Font(style_num, Color_1, Color_2, Color_3);
farpokeb(font_style_data, s_FONT_HEADER.shadow_flag, 4);
```
```c
Set_Font(style_num, Color_1, Color_2, Color_3);
font_header->shadow_flag = 4;  // enum e_Font_Shadow { e_Font_Shadow_Outline }
```
    function (0 bytes) Set_Font_Style_Outline
    Address: 01:00120D79
        Num params: 2
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (2 bytes) style_num
            pointer (4 bytes) colors

    function (0 bytes) Set_Font_Style
    Address: 01:00120BB5
        Num params: 2
        Return type: void (1 bytes) 
        signed integer (4 bytes) 
        pointer (4 bytes) 
        Locals:
            signed integer (4 bytes) style_num
            pointer (4 bytes) colors
            signed integer (4 bytes) i

    struct (9052 bytes) font_header
    Address: 02:001ABE7C
            array (8 bytes) current_colors
            Num elements:    8, Type:                unsigned integer (4 bytes) 
            signed integer (2 bytes) height
            signed integer (2 bytes) shadow_flag
            array (8 bytes) normal_colors
            Num elements:    8, Type:                unsigned integer (4 bytes) 
            array (8 bytes) highlight_colors
            Num elements:    8, Type:                unsigned integer (4 bytes) 
            array (8 bytes) special_colors
            Num elements:    8, Type:                unsigned integer (4 bytes) 
            signed integer (2 bytes) current_base_height
            signed integer (2 bytes) current_line_feed
            signed integer (2 bytes) current_vertical_spacing
            signed integer (2 bytes) current_horizontal_spacing
            array (256 bytes) current_font_widths
            Num elements:  256, Type:                unsigned integer (4 bytes) 
            array (1024 bytes) current_data_offsets
            Num elements:  256, Type:                unsigned integer (4 bytes) 
            array (16 bytes) base_height
            Num elements:    8, Type:                unsigned integer (4 bytes) 
            array (16 bytes) horizontal_spacing
            Num elements:    8, Type:                unsigned integer (4 bytes) 
            array (16 bytes) vertical_spacing
            Num elements:    8, Type:                unsigned integer (4 bytes) 
            array (1536 bytes) font_widths
            Num elements:    6, Type:                unsigned integer (4 bytes) 
            array (6144 bytes) data_offsets
            Num elements:    6, Type:                unsigned integer (4 bytes) 

