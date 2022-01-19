// _s34p82c.c GUI_1TickRedraw
// ST_GUI.H

#include "ST_GUI.H"

#include "STU_DBG.H"


void GUI_Redraw_WaitOne(void)
{
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: GUI_Redraw_WaitOne()\n", __FILE__, __LINE__);
#endif

    if ( g_GUI_RedrawFn_Present == 0 )
    {
        HERE("( g_GUI_RedrawFn_Present == 0 )");
        VGA_DAC_Write();
        GUI_SimplePageFlip();
    }
    else
    {
        HERE("( g_GUI_RedrawFn_Present != 0 )");
        CLK_SaveCounter();
        VGA_SetDrawFrame();
        GUI_CallRedrawFn();
        VGA_DAC_Write();
        GUI_SimplePageFlip();  // _s34p85c.c
        CLK_Wait(1);
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: GUI_Redraw_WaitOne()\n", __FILE__, __LINE__);
#endif
}
