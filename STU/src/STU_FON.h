#ifndef STU_FON_H
#define STU_FON_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif



/*
    MoM font constants (from s_FONT_HEADER in MoX/src/Fonts.h)
    FONTS.LBX contains 8 font styles, each covering 96 printable characters (ASCII 32-127).
*/
#define FON_STYLE_COUNT     8
#define FON_CHAR_COUNT      96
#define FON_FIRST_CHAR      32      /* space */
#define FON_LAST_CHAR       127     /* DEL / last printable */
#define FON_COLOR_SET_COUNT 16



/*
    Extracted glyph: one character from one font style.
    The pixel data is an array of (width * height) bytes, each byte being a colour-map index.
    A zero pixel means transparent / background.
*/
typedef struct s_FON_GLYPH
{
    int style;          /* font style index 0..7 */
    int char_code;      /* ASCII code 32..127 */
    int width;          /* glyph width in pixels */
    int height;         /* glyph height in pixels */
    uint8_t * pixels;   /* row-major pixel data, width * height bytes (colour-map indices) */
} FON_GLYPH;

/*
    Extracted font style: all 96 glyphs for one of the 8 styles.
*/
typedef struct s_FON_STYLE
{
    int style;                          /* font style index 0..7 */
    int height;                         /* pixel height for this style */
    int base_height;                    /* baseline offset */
    int horizontal_spacing;             /* inter-character spacing */
    int vertical_spacing;               /* inter-line spacing */
    uint8_t widths[FON_CHAR_COUNT];     /* per-character widths */
    FON_GLYPH glyphs[FON_CHAR_COUNT];  /* extracted glyphs */
} FON_STYLE;



/*
    Dump a text-art representation of all glyphs in a font style to stdout.
    Useful for quick visual verification without needing a graphics viewer.
*/
void Dump_Font_Style_Text(int style_index);

/*
    Dump all 8 font styles.
*/
void Dump_All_Font_Styles_Text(void);

/*
    Export glyphs for one font style as individual BMP files into out_dir.
    Files are named e.g. "style0_char065_A.bmp".
    Returns 0 on success, non-zero on failure.
*/
int Export_Font_Style_BMP(int style_index, const char * out_dir);

/*
    Export all 8 font styles as BMP files.
    Returns 0 on success, non-zero on first failure.
*/
int Export_All_Font_Styles_BMP(const char * out_dir);

/*
    TODO  future: convert a font style to a TrueType font (.ttf).
    This is a longer-term goal for the community.
    Returns 0 on success, non-zero on failure.
*/
/* int Export_Font_Style_TTF(int style_index, const char * out_path); */



#ifdef __cplusplus
}
#endif

#endif /* STU_FON_H */
