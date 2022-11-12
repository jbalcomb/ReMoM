// _s34p85c.c GUI_SimplePageFlip
// ST_GUI.H

#include "ST_GUI.H"

#include "ST_CRSR.H"
#include "ST_SCRN.H"

// MoO2  ? Toggle_Pages() ? vs. VGA_PageFlip() ?
void SCRN_SimplePageFlip(void)
{
    int mouse_x;
    int mouse_y;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: SCRN_SimplePageFlip()\n", __FILE__, __LINE__);
#endif

    MD_CDraw_Disable();

    mouse_x = MD_GetX();
    mouse_y = MD_GetY();

    // NOTE(JimBalcomb,20221110): Presently, this feels like where what has been drawn is everything, so the draw-screen-page can be saved/exported/dumped.
    // STU_Export_DSP_To_BMP();


    CTRL_DrawControls();  // _s34p71c

    GUI_FindWindow(mouse_x, mouse_y);
    CRL_Save_DSP(mouse_x, mouse_y);
    CRH_Draw_DSP(mouse_x, mouse_y);
    //CRH_Draw_DSP_A(mouse_x, mouse_y);
    //CRH_Draw_DSP_C(mouse_x, mouse_y);

    VGA_PageFlip();  // _s26p04a.c
    
    CRL_Restore_DSP();
    CRL_Copy_DSP2RSP();  // _s33p33.asm

    MD_CDraw_Restore();

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: SCRN_SimplePageFlip()\n", __FILE__, __LINE__);
#endif
}
