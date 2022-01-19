// _s34p73c.c GUI_SetFocus
// ST_GUI.H

#include "ST_GUI.H"


void GUI_SetFocus(int Ctrl_Index, int X_Pos, int Y_Pos)
{
    int tmp_SI;
    int tmp_DI;

    tmp_SI = Ctrl_Index;

    tmp_DI = X_Pos;

    if ( (tmp_DI >= 0) && (tmp_DI < 320) && (Y_Pos >= 0) && (Y_Pos < 200) )
    {
        if ( g_MOUSE_Hardware  == 0)
        {
            GUI_RestoreCursorArea_RSP();
            VGA_SetDirectDraw();
            g_GUI_FocusedControl = tmp_SI;
            GUI_DrawControl(tmp_SI, 1);
            g_GUI_FocusedControl = -1;
            VGA_SetDrawFrame();
            GUI_SaveCursorArea_RSP(tmp_DI, Y_Pos);
            GUI_DrawCursor_RSP(tmp_DI, Y_Pos);
            MOUSE_MoveCursor(tmp_DI, Y_Pos);
        }
        else
        {
            MOUSE_Disable_CDraw();

            if ( tmp_SI != g_GUI_FocusedControl )
            {
                if ( g_GUI_FocusedControl != -1 )
                {
                    /*
                        Redraw Previous & Draw Current
                    */
                    if ( g_GUI_FocusedControl != tmp_SI)
                    {
                        GUI_RestoreCursorArea_RSP();

                        /*
                            Yay/Nay Redraw Previous
                            Yay:
                                Fall-Through:
                                    previous control is not Ctrl_MStateButton
                                    previous control is Ctrl_MStateButton but current control is not Ctrl_MStateButton
                                    previous control is Ctrl_DialogLine and current control is Ctrl_DialogLine
                                    current control is Selectable

                            Nay:
                                previous control is Ctrl_MStateButton but current control is not Ctrl_MStateButton
                                previous control is Ctrl_DialogLine but current control is not Ctrl_MStateButton
                                current control is not Selectable

                        */

                        if ( (gfp_GUI_Control_Table[g_GUI_FocusedControl].Ctrl_Type != Ctrl_MStateButton)
                            && (gfp_GUI_Control_Table[tmp_SI].Ctrl_Type == Ctrl_MStateButton) )
                        {
                            if ( (gfp_GUI_Control_Table[g_GUI_FocusedControl].Ctrl_Type != Ctrl_DialogLine)
                            && (gfp_GUI_Control_Table[tmp_SI].Ctrl_Type == Ctrl_DialogLine) )
                            {
                                if ( gfp_GUI_Control_Table[tmp_SI].Selectable == 1 )
                                {
                                    GUI_DrawControl(g_GUI_FocusedControl, 0);
                                }
                            }
                        }

                        // @@DrawControl:
                        g_GUI_FocusedControl = tmp_SI;
                        GUI_DrawControl(tmp_SI, 1);

                        if ( gfp_GUI_Control_Table[tmp_SI].Ctrl_Type != Ctrl_EditBox )
                        {
                            tmp_DI = MOUSE_GetX();
                            Y_Pos = MOUSE_GetY();
                        }

                        GUI_SaveCursorArea_RSP(tmp_DI, Y_Pos);
                        GUI_DrawCursor_RSP(tmp_DI, Y_Pos);
                        MOUSE_MoveCursor(tmp_DI, Y_Pos);

                    }
                }
                else
                {
                    GUI_RestoreCursorArea_RSP();
                    g_GUI_FocusedControl == tmp_SI;
                    GUI_DrawControl(tmp_SI, 1);

                    if ( gfp_GUI_Control_Table[tmp_SI].Ctrl_Type != Ctrl_EditBox )
                    {
                        tmp_DI = MOUSE_GetX();
                        Y_Pos = MOUSE_GetY();
                    }
                    
                    GUI_SaveCursorArea_RSP(tmp_DI, Y_Pos);
                    GUI_DrawCursor_RSP(tmp_DI, Y_Pos);
                    MOUSE_MoveCursor(tmp_DI, Y_Pos);

                }
            }

            MOUSE_CDraw_Restore();
        }
    }
}
