// _s19p14c.c VGA_TextDraw_Init
// ST_VGA.H

#include "ST_SA.H"
#include "ST_VGA.H"


void VGA_TextDraw_Init(void)
{
    UU_g_VGA_TextDraw_Initd = 1;
    gfp_VGA_TextLine_Lefts = SA_MK_FP0(SA_Allocate_Space(8));   // 8 paragraphs = 8 * 16 bytes = 128 bytes
    gfp_VGA_TextLine_Rights = SA_MK_FP0(SA_Allocate_Space(8));  // 8 paragraphs = 8 * 16 bytes = 128 bytes
    gfp_VGA_TextLine_Tops = SA_MK_FP0(SA_Allocate_Space(8));    // 8 paragraphs = 8 * 16 bytes = 128 bytes
    gfp_VGA_TextLine_Starts = SA_MK_FP0(SA_Allocate_Space(8));  // 8 paragraphs = 8 * 16 bytes = 128 bytes
}
