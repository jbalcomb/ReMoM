// _s34p65c.c GUI_Init
// ST_GUI.H

#include "ST_HEAD.H"

#include "ST_GUI.H"


void GUI_Init(int Input_Type)
{
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: GUI_Init(Input_Type=%d)\n", __FILE__, __LINE__, Input_Type);
#endif

    gfp_GUI_Control_Table = SA_MK_FP0(SA_Allocate_Space(357));  // 357 paragraphs = 367 * 16 = 5712 bytes  (? 150*38=5700 ? + 12 ?)

    /*
        Input_Type: {0, 1, 2, n}
    */
    if ( Input_Type == 1 )
    {
        GUI_SetWindows(1, g_GUI_VirtualScreen);  // _s33p01  ST_GUI.H
        g_MOUSE_Hardware = MOUSE_Init();  // _s33p07  int MOUSE_Init(void)  ST_GUI.H
        if ( g_MOUSE_Hardware != ST_FAILURE )
        {
            g_MOUSE_Hardware = 1;  // ST_TRUE
        }
        else
        {
//            dlvfprintf("This feature not yet implemented.");
            // CRP_MOUSE_SetUsable();
            // g_MOUSE_Emu_X = 158;
            // g_MOUSE_Emu_Y = 100;
            // MOUSE_MoveCursor(158, 100);
        }
    }
    else
    {
//        dlvfprintf("This feature not yet implemented.");
    }
    
    VGA_SaveCursorArea_RSP(158, 100);
    g_GUI_Delay = 0;
    g_GUI_FocusedControl = -1;  // ST_UNDEFINED
    g_GUI_EscapeOverride = 0;
    GUI_Clear();  // _s34p57  ST_GUI.H
    
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: GUI_Init()\n", __FILE__, __LINE__);
#endif
}
