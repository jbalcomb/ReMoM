// _s19p11c.c VGA_GetFontHeight
// ST_VGA.H

#include "ST_VGA.H"


int VGA_GetFontHeight(void)
{
    int Font_Height;
    
    Font_Height= farpeekw(gsa_FontStyleData, 16); // FONT_HEADER.Font_Height

    return Font_Height;
}
