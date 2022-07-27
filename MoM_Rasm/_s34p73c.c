// _s34p73c.c GUI_SetFocus
// ST_GUI.H

#include "ST_CTRL.H"
#include "ST_GUI.H"


void CTRL_SetFocus(int Ctrl_Index, int X_Pos, int Y_Pos)
{
    int tmp_SI;
    int tmp_DI;

    tmp_SI = Ctrl_Index;

    tmp_DI = X_Pos;

    if ( (tmp_DI >= 0) && (tmp_DI < 320) && (Y_Pos >= 0) && (Y_Pos < 200) )
    {
        if ( g_MD_Hardware == 0)
        {
            CRL_Restore_RSP();
            VGA_SetDirectDraw();
            g_CTRL_FocusedControl = tmp_SI;
            CTRL_DrawControl(tmp_SI, 1);
            g_CTRL_FocusedControl = -1;
            VGA_Set_DSP_Addr();
            CRL_Save_RSP(tmp_DI, Y_Pos);
            CRH_Draw_RSP(tmp_DI, Y_Pos);
            MD_MoveCursor(tmp_DI, Y_Pos);
        }
        else
        {
            MD_CDraw_Disable();

            if ( tmp_SI != g_CTRL_FocusedControl )
            {
                if ( g_CTRL_FocusedControl != -1 )
                {
                    /*
                        Redraw Previous & Draw Current
                    */
                    if ( g_CTRL_FocusedControl != tmp_SI)
                    {
                        CRL_Restore_RSP();

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

                        if ( (gfp_CTRL_Control_Table[g_CTRL_FocusedControl].Ctrl_Type != Ctrl_MStateButton)
                            && (gfp_CTRL_Control_Table[tmp_SI].Ctrl_Type == Ctrl_MStateButton) )
                        {
                            if ( (gfp_CTRL_Control_Table[g_CTRL_FocusedControl].Ctrl_Type != Ctrl_DialogLine)
                            && (gfp_CTRL_Control_Table[tmp_SI].Ctrl_Type == Ctrl_DialogLine) )
                            {
                                if ( gfp_CTRL_Control_Table[tmp_SI].Selectable == 1 )
                                {
                                    CTRL_DrawControl(g_CTRL_FocusedControl, 0);
                                }
                            }
                        }

                        // @@DrawControl:
                        g_CTRL_FocusedControl = tmp_SI;
                        CTRL_DrawControl(tmp_SI, 1);

                        if ( gfp_CTRL_Control_Table[tmp_SI].Ctrl_Type != Ctrl_EditBox )
                        {
                            tmp_DI = MD_GetX();
                            Y_Pos = MD_GetY();
                        }

                        CRL_Save_RSP(tmp_DI, Y_Pos);
                        CRH_Draw_RSP(tmp_DI, Y_Pos);
                        MD_MoveCursor(tmp_DI, Y_Pos);

                    }
                }
                else
                {
                    CRL_Restore_RSP();
                    g_CTRL_FocusedControl == tmp_SI;
                    CTRL_DrawControl(tmp_SI, 1);

                    if ( gfp_CTRL_Control_Table[tmp_SI].Ctrl_Type != Ctrl_EditBox )
                    {
                        tmp_DI = MD_GetX();
                        Y_Pos = MD_GetY();
                    }
                    
                    CRL_Save_RSP(tmp_DI, Y_Pos);
                    CRH_Draw_RSP(tmp_DI, Y_Pos);
                    MD_MoveCursor(tmp_DI, Y_Pos);

                }
            }

            MD_CDraw_Restore();
        }
    }
}
