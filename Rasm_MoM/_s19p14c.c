// _s19p14c.c VGA_TextDraw_Init

#include "ST_SA.H"
#include "ST_VGA.H"


void VGA_TextDraw_Init(void)
{
    unsigned int tmp_SI;

    printf("DEBUG: BEGIN: VGA_TextDraw_Init()\n");

    UU_g_VGA_TextDraw_Initd = 1;

    tmp_SI = SA_Allocate_Space(8);          // 8 paragraphs = 8 * 16 bytes = 128 bytes
    gfp_VGA_TextLine_Lefts = SA_MK_FP0(tmp_SI);
    printf("DEBUG: gfp_VGA_TextLine_Lefts: %Fp\n", gfp_VGA_TextLine_Lefts);

    tmp_SI = SA_Allocate_Space(8);          // 8 paragraphs = 8 * 16 bytes = 128 bytes
    gfp_VGA_TextLine_Rights = SA_MK_FP0(tmp_SI);
    printf("DEBUG: gfp_VGA_TextLine_Rights: %Fp\n", gfp_VGA_TextLine_Rights);

    tmp_SI = SA_Allocate_Space(8);          // 8 paragraphs = 8 * 16 bytes = 128 bytes
    gfp_VGA_TextLine_Tops = SA_MK_FP0(tmp_SI);
    printf("DEBUG: gfp_VGA_TextLine_Tops: %Fp\n", gfp_VGA_TextLine_Tops);

    tmp_SI = SA_Allocate_Space(8);          // 8 paragraphs = 8 * 16 bytes = 128 bytes
    gfp_VGA_TextLine_Starts = SA_MK_FP0(tmp_SI);
    printf("DEBUG: gfp_VGA_TextLine_Starts: %Fp\n", gfp_VGA_TextLine_Starts);

    printf("DEBUG: END: VGA_TextDraw_Init()\n");    
}
