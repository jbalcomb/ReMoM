// _s34p01c.c GUI_ProcessInput()
// ST_GUI.H

#include "ST_CTRL.H"
#include "ST_GUI.H"

#include "STU_DBG.H"

/*
    ? Fall-Through Path ?
        if KBD_CheckBuffer() == 0
            MOUSE_INTSet_MvOnly()
            if MOUSE_GetButtonStatus() == 0
                if MOUSE_GetClickRec1 == 0
                    MOUSE_GetClickRec1()
                    CRP_MOUSE_GetClickRec2()
                    return 0;

    ? Return Value ?
        ? control table index of selected control ?

*/


int IN_ProcessInput(void)
{
    char InputCode;
    int CtrlIdx;
    int Unused_Local;
    int Control_Bottom;
    int Control_Right;
    int Control_Top;
    int Control_Left;
    int Accepted_Char;
    int Mouse_Btn_Clicked;
    int Y_Pos;
    int X_Pos;
    unsigned int tmp_MidX;
    unsigned int tmp_MidY;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: IN_ProcessInput()\n", __FILE__, __LINE__);
#endif

    CtrlIdx = 0;
    g_CTRL_FocusedControl = -1;
    Mouse_Btn_Clicked = 0;
    g_CTRL_MouseFocusCtrl = 0;
    X_Pos = MD_GetX();
    Y_Pos = MD_GetY();
    g_CRSR_Offset = CR_GetOffset();
    
    if ( KBD_CheckBuffer() != 0 )  // _s33p16  int KBD_CheckBuffer(void)
    {
        HERE("( KBD_CheckBuffer() != 0 )");
        InputCode = UI_ProcessInput_KD(&CtrlIdx);  // _s34p22c.c  int GUI_NoTriggerKeys(int *CtrlIdx)
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] InputCode: %d\n", __FILE__, __LINE__, InputCode);
#endif
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] CtrlIdx: %d\n", __FILE__, __LINE__, CtrlIdx);
#endif

        if ( InputCode != 0 )
        {
            // ...F11, DbgQuit
            // ...F12, DbgScrnDmp
            // ...ESC, EscapeOverride...return -1
            // ...backtick, DbgIsDisabled, DbgControlHighlight
            // ...tilde, DbgIsDisabled, DbgHelpHighlight
            // if ( Control Type != AltKey Control )  ... else MOUSE_INTSet_MvBtns(); return CtrlIdx;
            //     if ( CtrlIdx != 0 )  ... else Y/N KP_Enter Y/N InEditSelect ...
            //         if ( HotKey == InputCode )  ... else Y/N KP_Enter Y/N InEditSelect ...
            //             if ( Control Type != Slidebar )
            //                 CTRL_SetFocus(CtrlIdx, MidX, MidY)
            //                 MOUSE_INTSet_MvBtns()
            //                     if ( Control Type == ToggleButton )  ... if ( Param2 != 0 ) { Param2 = 0 } else { Param2 = 1 } ... SCRN_Redraw_WaitOne(); UI_FcsdCtrl = -1
            //                         if ( Control Type == LockableButton )  ... (Param2==0)?(Param2 = 1;SCRN_Redraw_WaitOne();g_CTRL_FocusedControl=-1;return(CtrlIdx);)
            //                                                                               :(SCRN_Redraw_WaitOne();g_CTRL_FocusedControl=-1;return 0;)
            //                             if ( Control Type == ClickLink ) {g_CTRL_FocusedControl=-1;return(Param0-Parent);}
            //                                                              else{SCRN_Redraw_WaitOne();g_CTRL_FocusedControl=-1;return(CtrlIdx)}

            if ( gfp_CTRL_Control_Table[CtrlIdx].Ctrl_Type != Ctrl_AltString )
            {
                if ( CtrlIdx != 0 )
                {
                    if ( gfp_CTRL_Control_Table[CtrlIdx].Hotkey == InputCode )
                    {
                        if ( gfp_CTRL_Control_Table[CtrlIdx].Ctrl_Type != Ctrl_Slidebar )
                        {
                            // TODO(JimBalcomb): see if this odd mid-point calculation is just compile-time confusion due to the struct array indexing
                            tmp_MidX = gfp_CTRL_Control_Table[CtrlIdx].Left + ((gfp_CTRL_Control_Table[CtrlIdx].Right - gfp_CTRL_Control_Table[CtrlIdx].Left) / 2);
                            tmp_MidX = gfp_CTRL_Control_Table[CtrlIdx].Top + ((gfp_CTRL_Control_Table[CtrlIdx].Bottom - gfp_CTRL_Control_Table[CtrlIdx].Top) / 2);
                            CTRL_SetFocus(CtrlIdx, tmp_MidX, tmp_MidY);
                            MD_INT_SetMvBtns();
                            if ( gfp_CTRL_Control_Table[CtrlIdx].Ctrl_Type == Ctrl_ToggleButton )
                            {
                                if ( gfp_CTRL_Control_Table[CtrlIdx].Param2 != 0 )
                                {
                                    gfp_CTRL_Control_Table[CtrlIdx].Param2 = 0;
                                }
                                else
                                {
                                    gfp_CTRL_Control_Table[CtrlIdx].Param2 = 1;
                                }
                                SCRN_Redraw_WaitOne();
                                g_CTRL_FocusedControl = -1;
                                return CtrlIdx;
                            }
                            else if ( gfp_CTRL_Control_Table[CtrlIdx].Ctrl_Type == Ctrl_LockableButton )
                            {
                                if ( gfp_CTRL_Control_Table[CtrlIdx].Param2 == 0 )
                                {
                                    gfp_CTRL_Control_Table[CtrlIdx].Param2 = 1;
                                    SCRN_Redraw_WaitOne();
                                    g_CTRL_FocusedControl = -1;
                                    return CtrlIdx;
                                }
                                else
                                {
                                    SCRN_Redraw_WaitOne();
                                    g_CTRL_FocusedControl = -1;
                                    return 0;
                                }
                            }
                            else if ( gfp_CTRL_Control_Table[CtrlIdx].Ctrl_Type == Ctrl_ClickLink )
                            {
                                g_CTRL_FocusedControl = -1;
                                return gfp_CTRL_Control_Table[CtrlIdx].Param0;  // Parent
                            }
                            else
                            {
                                SCRN_Redraw_WaitOne();
                                g_CTRL_FocusedControl = -1;
                                return CtrlIdx;
                            }
                        }
                    }  /* if ( gfp_CTRL_Control_Table[CtrlIdx].Hotkey == InputCode ) */
                }  /* if ( CtrlIdx != 0 ) */
            }  /* if ( gfp_CTRL_Control_Table[CtrlIdx].Ctrl_Type != Ctrl_AltString ) */
        }  /* if ( InputCode != 0 ) */
    }  /* if ( KBD_CheckBuffer() != 0 ) */
    else
    {
        HERE("( KBD_CheckBuffer() == 0 )");
    }
    // else
    // {
    //     //MOUSE_INTSet_MvOnly();  // _s33p10
    //                               // _s33p11         MOUSE_INTSet_MvBtns

    //     // _s33p05.asm  int MD_GetButtonStatus(void)
    //     if ( MD_GetButtonStatus() != 0 )
    //     {

    //     }
    //     else
    //     {

    //     }

    // }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: IN_ProcessInput()\n", __FILE__, __LINE__);
#endif
    return CtrlIdx;
}
