
TODO  fixup "tmpcurrpal"

Load_Font_File()
Load_Palette()
Apply_Palette()



Load_Font_File()
    font_style_data
    border_style_data
    font_header
    palette_block
    // MoO2  current_palette
    p_Palette              = Allocate_Space(64);     //  64 PR, 1024 B
    // 1oom: lbxpal_palette = p_Palette;
    current_palette = p_Palette;
    p_PaletteFlags         = p_Palette + (48 * SZ_PARAGRAPH_B);  // ~== p_PaletteFlags = &p_Palette[768];
    palette_flags = p_PaletteFlags;
    // TODO  UU_DAC_Save_Seg = Allocate_Space(48);  // in MoO1, also unused, maybe debug code
    remap_color_palettes = (uint8_t *)Allocate_Space(384);  // 384 PR, 6144 B  (24 * 256  ~'remap color tables')
    // TODO  file_animation_header = (struct s_FLIC_HDR *)Allocate_Space(2);
    Intensity_Scale_Tbl = Allocate_Space(96);  // 96 PR, 1536 B

Load_Palette()
    font_colors
    UU_gsa_Palette_Data
    mouse_palette
    remap_colors


palette_block is (just) the SAMB for the palette LBX entry




Fonts.C
void Load_Font_File(char * font_file)
    palette_block          = Allocate_Space(348);    // 348 PR, 5568 B
    // MoO2  current_palette
    p_Palette              = Allocate_Space(64);     //  64 PR, 1024 B
    // 1oom: lbxpal_palette = p_Palette;
    current_palette = p_Palette;
    p_PaletteFlags         = p_Palette + (48 * SZ_PARAGRAPH_B);  // ~== p_PaletteFlags = &p_Palette[768];
    palette_flags = p_PaletteFlags;
    for(itr = 0; itr < 768; itr++)
    {
        *(p_Palette + itr) = 0;
    }
    for(itr = 0; itr < 256; itr++)
    {
        *(p_PaletteFlags + itr) = ST_TRUE;
    }

...allocate 1024 bytes - 768 for colors, 256 for flags...
...set all colors to ST_TRANSPARENT, set all flags to ST_TRUE...


Fonts.C
void Load_Palette(int entry, int start_color, int end_color)
    palette_data = LBX_Reload(font_name, entry+2, palette_block);
    // 1oom: lbxpal_palette_inlbx = palette_data;
