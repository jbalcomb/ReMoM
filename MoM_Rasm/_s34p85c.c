// _s34p85c.c GUI_SimplePageFlip
// ST_GUI.H

#include "ST_GUI.H"

#include "ST_CRSR.H"
#include "ST_SCRN.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


// MoO2  ? Toggle_Pages() ? vs. VGA_PageFlip() ?
void SCRN_SimplePageFlip(void)
{
    int mouse_x;
    int mouse_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: SCRN_SimplePageFlip()\n", __FILE__, __LINE__);
#endif

    MD_CDraw_Disable();

    mouse_x = MD_GetX();
    mouse_y = MD_GetY();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] mouse_x: %d\n", __FILE__, __LINE__, mouse_x);
    dbg_prn("DEBUG: [%s, %d] mouse_y: %d\n", __FILE__, __LINE__, mouse_y);
#endif

    // NOTE(JimBalcomb,20221110): Presently, this feels like where what has been drawn is everything, so the draw-screen-page can be saved/exported/dumped.
    // STU_Export_DSP_To_BMP();


    CTRL_DrawControls();  // MGC s34p71c  ST_CTRL.H

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] g_CRSR_Prev: %d\n", __FILE__, __LINE__, g_CRSR_Prev);
    dbg_prn("DEBUG: [%s, %d] g_CRSR_Curr: %d\n", __FILE__, __LINE__, g_CRSR_Curr);
    dbg_prn("DEBUG: [%s, %d] g_GUI_Windows: %d\n", __FILE__, __LINE__, g_GUI_Windows);
    dbg_prn("DEBUG: [%s, %d] g_GUI_WindowCount: %d\n", __FILE__, __LINE__, g_GUI_WindowCount);
    dbg_prn("DEBUG: [%s, %d] g_CRSR_Offset: %d\n", __FILE__, __LINE__, g_CRSR_Offset);
#endif

    GUI_FindWindow(mouse_x, mouse_y);  // MGC s33p02  ST_GUI.H

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] g_CRSR_Prev: %d\n", __FILE__, __LINE__, g_CRSR_Prev);
    dbg_prn("DEBUG: [%s, %d] g_CRSR_Curr: %d\n", __FILE__, __LINE__, g_CRSR_Curr);
    dbg_prn("DEBUG: [%s, %d] g_GUI_Windows: %d\n", __FILE__, __LINE__, g_GUI_Windows);
    dbg_prn("DEBUG: [%s, %d] g_GUI_WindowCount: %d\n", __FILE__, __LINE__, g_GUI_WindowCount);
    dbg_prn("DEBUG: [%s, %d] g_CRSR_Offset: %d\n", __FILE__, __LINE__, g_CRSR_Offset);
#endif

    CRL_Save_DSP(mouse_x, mouse_y);  // MGC s33p31
    CRH_Draw_DSP(mouse_x, mouse_y);  // MGC s33p38 |-> CRL_Draw_DSP() s33p40
    //CRH_Draw_DSP_A(mouse_x, mouse_y);
    //CRH_Draw_DSP_C(mouse_x, mouse_y);

    VGA_PageFlip();  // MGC _s26p04a.c
    
    CRL_Restore_DSP();   // MGC s33p35
    CRL_Copy_DSP2RSP();  // MGC _s33p33.asm

    MD_CDraw_Restore();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: SCRN_SimplePageFlip()\n", __FILE__, __LINE__);
#endif
}
