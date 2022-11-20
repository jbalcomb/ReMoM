#ifndef WIN_FONTS_HPP
#define WIN_FONTS_HPP


extern byte_ptr p_Palette;                             // MGC dseg:A7DE    alloc in Load_Font_File()
extern byte_ptr p_PaletteFlags;                        // MGC dseg:A7D6    alloc in Load_Font_File()
extern byte_ptr p_Palette_XBGR;                        // STU/Win32


void Load_Font_File(char * font_file);
void Load_Palette(int entry, int start_color, int end_color);
void Set_Palette_Changes(int start_color, int end_color);


#endif /* WIN_FONTS_HPP */
