// _s34p71c.c GUI_DrawControls
// ST_GUI.H

#include "ST_CTRL.H"
#include "ST_GUI.H"
#include "ST_VGA.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


/*
    ? (some) Controls have their own draw function ?
    
*/

void CTRL_DrawControls(void)
{
    int Max_Y;
    int Min_Y;
    int Max_X;
    int Min_X;
// Text_Change= word ptr -0Ch
// State_Pointer= dword ptr -0Ah
// Slider_State= word ptr -6
// Half_V_Spacing= word ptr -4
    int Half_Font_Height;

    int tmp_SI;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: CTRL_DrawControls()\n", __FILE__, __LINE__);
#endif

    Min_X = g_VGA_Min_X;
    Max_X = g_VGA_Max_X;
    Min_Y = g_VGA_Min_Y;
    Max_Y = g_VGA_Max_Y;

    VGA_ResetDrawWindow();
    VGA_Set_DSP_Addr();

    for ( tmp_SI = 1; tmp_SI < g_CTRL_Control_Count; tmp_SI++ )
    {
        if ( (tmp_SI == g_CTRL_Focused) && (gfp_CTRL_Control_Table[tmp_SI].Ctrl_Type != Ctrl_EditBox) && (gfp_CTRL_Control_Table[tmp_SI].Ctrl_Type != Ctrl_ClickGrid) )
        {
            CTRL_DrawControl(tmp_SI, 1);
        }

        //  0: Ctrl_ClickButton
        //  1: Ctrl_ToggleButton
        //  2: Ctrl_LockableButton
        //  3: Ctrl_MStateButton
        //  4: Ctrl_EditBox
        //  5: Ctrl_ImageLabel
        //  6: Ctrl_Slidebar
        //  7: Ctrl_Label
        //  8: Ctrl_AltString
        //  9: Ctrl_ClickLink
        // 10: Ctrl_DialogLine
        // 11: Ctrl_EditSelect
        // 12: Ctrl_ClickGrid
        switch (gfp_CTRL_Control_Table[tmp_SI].Ctrl_Type)
        {
            case Ctrl_ClickButton:
            {
#ifdef STU_DEBUG
                dbg_prn("DEBUG: [%s, %d] case Ctrl_ClickButton\n", __FILE__, __LINE__);
#endif
                FLIC_Reset_CurrentFrame(gfp_CTRL_Control_Table[tmp_SI].Param5);
                // FLIC_Draw_XY(gfp_CTRL_Control_Table[tmp_SI].Left, gfp_CTRL_Control_Table[tmp_SI].Top, gfp_CTRL_Control_Table[tmp_SI].Param5);
                FLIC_Draw(gfp_CTRL_Control_Table[tmp_SI].Left, gfp_CTRL_Control_Table[tmp_SI].Top, (SAMB_ptr)MK_FP(gfp_CTRL_Control_Table[tmp_SI].Param5,0));
                VGA_SetFont(gfp_CTRL_Control_Table[tmp_SI].Font_Index, gfp_CTRL_Control_Table[tmp_SI].ColorSet1, 0, 0);
                Half_Font_Height = ( (VGA_GetFontHeight() - 1) / 2);
                VGA_DrawCentered(
                    ((gfp_CTRL_Control_Table[tmp_SI].Right - gfp_CTRL_Control_Table[tmp_SI].Left) + gfp_CTRL_Control_Table[tmp_SI].Left),
                    (((gfp_CTRL_Control_Table[tmp_SI].Bottom - gfp_CTRL_Control_Table[tmp_SI].Top) / 2) + gfp_CTRL_Control_Table[tmp_SI].Top - Half_Font_Height),
                    gfp_CTRL_Control_Table[tmp_SI].Param0
                );
                break;
            }
            case Ctrl_ToggleButton:
            {
#ifdef STU_DEBUG
                dbg_prn("DEBUG: [%s, %d] case Ctrl_ToggleButton\n", __FILE__, __LINE__);
#endif
                break;
            }
            case Ctrl_LockableButton:
            {
#ifdef STU_DEBUG
                dbg_prn("DEBUG: [%s, %d] case Ctrl_LockableButton\n", __FILE__, __LINE__);
#endif
                break;
            }
            case Ctrl_MStateButton:
            {
#ifdef STU_DEBUG
                dbg_prn("DEBUG: [%s, %d] case Ctrl_MStateButton\n", __FILE__, __LINE__);
#endif
                break;
            }
            case Ctrl_EditBox:
            {
#ifdef STU_DEBUG
                dbg_prn("DEBUG: [%s, %d] case Ctrl_EditBox\n", __FILE__, __LINE__);
#endif
                break;
            }
            case Ctrl_ImageLabel:
            {
#ifdef STU_DEBUG
                dbg_prn("DEBUG: [%s, %d] case Ctrl_ImageLabel\n", __FILE__, __LINE__);
#endif
                break;
            }
            case Ctrl_Slidebar:
            {
#ifdef STU_DEBUG
                dbg_prn("DEBUG: [%s, %d] case Ctrl_Slidebar\n", __FILE__, __LINE__);
#endif
                break;
            }
            case Ctrl_Label:
            {
#ifdef STU_DEBUG
                dbg_prn("DEBUG: [%s, %d] case Ctrl_Label\n", __FILE__, __LINE__);
#endif
                break;
            }
            case Ctrl_AltString:
            {
#ifdef STU_DEBUG
                dbg_prn("DEBUG: [%s, %d] case Ctrl_AltString\n", __FILE__, __LINE__);
#endif
                break;
            }
            case Ctrl_ClickLink:
            {
#ifdef STU_DEBUG
                dbg_prn("DEBUG: [%s, %d] case Ctrl_ClickLink\n", __FILE__, __LINE__);
#endif
                break;
            }
            case Ctrl_DialogLine :
            {
#ifdef STU_DEBUG
                dbg_prn("DEBUG: [%s, %d] case Ctrl_DialogLine\n", __FILE__, __LINE__);
#endif
                break;
            }
            case Ctrl_EditSelect :
            {
#ifdef STU_DEBUG
                dbg_prn("DEBUG: [%s, %d] case Ctrl_EditSelect\n", __FILE__, __LINE__);
#endif
                break;
            }
            case Ctrl_ClickGrid :
            {
#ifdef STU_DEBUG
                dbg_prn("DEBUG: [%s, %d] case Ctrl_ClickGrid\n", __FILE__, __LINE__);
#endif
                break;
            }
        }
    }

    /*
        Debug Code - Highlight Help Control Boundarie
    */
    // if ( (g_DBG_HelpHighlight != 0) && (g_HLP_Entry_Count != 0) )
    // {
    //     for ( tmp_SI = 0; tmp_SI < g_HLP_Entry_Count; tmp_SI++ )
    //     {
    //         // if ( g_HLP_Entry_Table + (tmp_SI * 5) != -1 )
    //         // {
    //         //     // mov  ax, 180
    //         //     // push ax                              ; Color
    //         //     // mov  ax, di
    //         //     // add  ax, 2
    //         //     // shl  ax, 1
    //         //     // mov  bx, [g_HLP_Entry_Table]
    //         //     // add  bx, ax
    //         //     // push [word ptr bx]                   ; Y_2
    //         //     // mov  ax, di
    //         //     // add  ax, 3
    //         //     // shl  ax, 1
    //         //     // mov  bx, [g_HLP_Entry_Table]
    //         //     // add  bx, ax
    //         //     // push [word ptr bx]                   ; X_2
    //         //     // mov  ax, di
    //         //     // add  ax, 2
    //         //     // shl  ax, 1
    //         //     // mov  bx, [g_HLP_Entry_Table]
    //         //     // add  bx, ax
    //         //     // push [word ptr bx]                   ; Y_1
    //         //     // mov  ax, di
    //         //     // inc  ax
    //         //     // shl  ax, 1
    //         //     // mov  bx, [g_HLP_Entry_Table]
    //         //     // add  bx, ax
    //         //     // push [word ptr bx]                   ; X_1
    //         //     // call VGA_WndDrawLine
    //         // }
    //     }
    // }

Done:
    VGA_SetDrawWindow(Min_X, Min_Y, Max_X, Max_Y);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: CTRL_DrawControls()\n", __FILE__, __LINE__);
#endif
}
