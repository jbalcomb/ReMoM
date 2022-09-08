// _s19p14c.c VGA_TextDraw_Init
// ST_VGA.H

#include "ST_SA.H"
#include "ST_VGA.H"


void VGA_TextDraw_Init(void)
{
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: VGA_TextDraw_Init()\n", __FILE__, __LINE__);
#endif

    UU_g_VGA_TextDraw_Initd = 1;
    gfp_VGA_TextLine_Lefts = SA_Allocate_Space(8);   // 8 paragraphs = 8 * 16 bytes = 128 bytes
    gfp_VGA_TextLine_Rights = SA_Allocate_Space(8);  // 8 paragraphs = 8 * 16 bytes = 128 bytes
    gfp_VGA_TextLine_Tops = SA_Allocate_Space(8);    // 8 paragraphs = 8 * 16 bytes = 128 bytes
    gfp_VGA_TextLine_Starts = SA_Allocate_Space(8);  // 8 paragraphs = 8 * 16 bytes = 128 bytes

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: VGA_TextDraw_Init()\n", __FILE__, __LINE__);
#endif
}
