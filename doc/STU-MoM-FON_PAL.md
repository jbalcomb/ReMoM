
MoM / MoX
Font
Palette

Export BMP
Export Animator Pro - Font, Palette, Animation

FONTS.LBX



256 color palette, as rectangles of solid color
Color-Map Index; Red, Green, Blue values, 6-bpp, as Hexidecima - 1 chracter per nibble
MoM Font Style Number 1 - width = 3, height = 5





void Load_Font_File(char * font_file)
    strcpy(font_name, font_file);
    font_style_data = LBX_Load(font_file, 0);
    border_style_data = LBX_Load(font_file, 1);
    font_header = (struct s_FONT_HEADER *)font_style_data;
    palette_block          = Allocate_Space(348);    // 348 paragraphs = 386 * 16 bytes = 5568 bytes
    p_Palette              = Allocate_Space(64);     //  64 paragraphs =  64 * 16 bytes = 1024 bytes
    p_PaletteFlags         = p_Palette + (48 * 16);  // ~== p_PaletteFlags = &p_Palette[768];
    // TODO  UU_DAC_Save_Seg = Allocate_Space(48);  // 48 PR, 768 B
    // Replacement_Colors = Allocate_Space(384);  // 384 paragraphs = 384 * 16 = 6,144 bytes  (24 * 256  ~'remap color tables')
    remap_color_palettes = Allocate_Space(384);
    // TODO  gsa_VGAFILEH_Header = Allocate_Space(2);
    Intensity_Scale_Tbl = Allocate_Space(96);  // 96 paragraphs = 96 * 16 = 1,536 bytes  

void Load_Palette(int entry, int start_color, int end_color)
   int color_start;
    int color_count;
    int itr;
    uint8_t font_color_block;
    uint8_t color_index;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Load_Palette(entry = %d, start_color = %d, end_color = %d)\n", __FILE__, __LINE__, entry, start_color, end_color);
#endif

    palette_data = LBX_Reload(font_name, entry+2, palette_block);
    // 1oom: lbxpal_palette_inlbx = palette_data;

    // font_colors = (palette_data + (16 * (48)));  // 768
    font_colors = &palette_data[768];

    // UU_gsa_Palette_Data = (palette_data + (16 * (48 + 16)));  // 400h

    mouse_palette = (palette_data + (16 * (48 + 16 + 16)));  // 1280

    // array of 24 color fractions (B-G-R-Percent)
    // used to set shaded replacement colors for each color index
    // gsa_ShadingColors = (palette_data + (16 * (48 + 16 + 16 + 256)));  // 5376  (24 * 4 = 96) 0x60  6 PR
    remap_colors = (palette_data + (16 * (48 + 16 + 16 + 256)));
    // 1oom
    // lbxpal_ctableparam = Pal + 0x1500;
    // 1oom: lbxpal_ctableparam = palette_data + 0x1500;

    if(start_color == ST_UNDEFINED)
    {
        color_start = 0;
        color_count = 256;
    }
    else
    {
        color_start = start_color;
        color_count = (end_color - start_color) + 1;
    }

    for(itr = 0; itr < (color_count * 3); itr++)
    {
        *(p_Palette + (color_start * 3) + itr) = *(palette_data + (color_start * 3) + itr);
    }
    Set_Font(0, 0, 0, 0);
    if(start_color == ST_UNDEFINED)
    {
        Set_Palette_Changes(0, 255);
    }
    else
    {
        Set_Palette_Changes(start_color, end_color);
    }



7 "palettes"

FONTS.LBX
entry_name  entry_description
No File     Fonts Style Data
No File     Border Style Data
EMPERATO    Main game palete
LOADSAVE    Load/Save palette
ARCANUS     Magic Castle View
WIZLAB F    conquest
TEST FLC    spell mastery
BACKGRND    you win screen...
LOSE FLC    you lose screen...




## Export BMP
initialize Microsoft (MS) Device-Independent-Bitmap (DIB) Structure (BITMAPINFO)
allocate memory for BMP image bytes
load font
load palette
loop through colors - 256 = 16 x 16 rectangles
print text labels
write out BMP file
