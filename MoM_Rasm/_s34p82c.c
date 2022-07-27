// _s34p82c.c GUI_1TickRedraw
// ST_GUI.H

#include "ST_GUI.H"
#include "ST_SCRN.H"

#include "STU_DBG.H"


void SCRN_Redraw_WaitOne(void)
{
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: SCRN_Redraw_WaitOne()\n", __FILE__, __LINE__);
#endif

    if ( g_SCRN_RedrawFn_Present == 0 )
    {
        HERE("( g_SCRN_RedrawFn_Present == 0 )");
        VGA_DAC_Write();
        SCRN_SimplePageFlip();
    }
    else
    {
        HERE("( g_SCRN_RedrawFn_Present != 0 )");
        CLK_SaveCounter();
        VGA_Set_DSP_Addr();
        SCRN_CallRedrawFn();
        VGA_DAC_Write();
        SCRN_SimplePageFlip();  // _s34p85c.c
        CLK_Wait(1);
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: SCRN_Redraw_WaitOne()\n", __FILE__, __LINE__);
#endif
}
