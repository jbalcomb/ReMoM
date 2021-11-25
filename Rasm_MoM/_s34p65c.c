// _s34p65c.c GUI_Init
// in ST_GUI.H

#include "ST_GUI.H"


void GUI_Init(int Input_Type)
{

    gfp_GUI_Control_Table = SA_MK_FP0(SA_Allocate_Space(357));  // 357 paragraphs = 367 * 16 = 5712 bytes

    /*
        Input_Type: {0, 1, 2, n}
    */
    if ( Input_Type == 1 )
    {
        GUI_SetWindows(1, g_GUI_VirtualScreen);
        g_MOUSE_Hardware = MOUSE_Init();
        if ( g_MOUSE_Hardware != 0 )
        {
            g_MOUSE_Hardware = 1;
        }
        else
        {
            printf("This feature not yet implemented.");
            // CRP_MOUSE_SetUsable();
            // g_MOUSE_Emu_X = 158;
            // g_MOUSE_Emu_Y = 100;
            // MOUSE_MoveCursor(158, 100);
        }
    }
    else
    {
        printf("This feature not yet implemented.");
    }

    VGA_SaveCursorArea(158, 100);
    g_GUI_Delay = 0;
    g_GUI_FocusedControl = -1;
    g_GUI_EscapeOverride = 0;
    GUI_Clear();
}
