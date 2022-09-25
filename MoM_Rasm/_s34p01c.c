// _s34p01c.c GUI_ProcessInput()
// ST_GUI.H

/*
    BCpp31PG, pg 175/190
        #pragma warn -yyy
    BCpp31UG, pg 162/171
    BCpp30UG, pg 158/167
        -waus*  Identifier is assigned a value that is never used.
        -wccc*  Condition is always true/false.
        -weff*  Code has no effect.
        -wpar*  Parameter parameter is never used.
        -wuse   Identifier is declared but never used.
        -wrpt*  Nonportable pointer conversion.
*/
#pragma warn -aus
#pragma warn -eff
#pragma warn -par

#include "ST_CTRL.H"
#include "ST_GUI.H"

#ifdef DEBUG
#include "STU_DBG.H"
#endif

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
    int MD_ButtonStatus;
    int Y_Pos;
    int X_Pos;
    unsigned int tmp_MidX;
    unsigned int tmp_MidY;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: IN_ProcessInput()\n", __FILE__, __LINE__);
#endif

    CtrlIdx = 0;
    g_CTRL_Focused = -1;
    MD_ButtonStatus = 0;
    g_CTRL_MouseFocusCtrl = 0;
    X_Pos = MD_GetX();
    Y_Pos = MD_GetY();
    g_CRSR_Offset = CR_GetOffset();
    
    if ( KBD_CheckBuffer() != 0 )  // _s33p16  int KBD_CheckBuffer(void)
    {
        // HERE("( KBD_CheckBuffer() != 0 )");
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
            //                         if ( Control Type == LockableButton )  ... (Param2==0)?(Param2 = 1;SCRN_Redraw_WaitOne();g_CTRL_Focused=-1;return(CtrlIdx);)
            //                                                                               :(SCRN_Redraw_WaitOne();g_CTRL_Focused=-1;return 0;)
            //                             if ( Control Type == ClickLink ) {g_CTRL_Focused=-1;return(Param0-Parent);}
            //                                                              else{SCRN_Redraw_WaitOne();g_CTRL_Focused=-1;return(CtrlIdx)}

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
                                g_CTRL_Focused = -1;
                                return CtrlIdx;
                            }
                            else if ( gfp_CTRL_Control_Table[CtrlIdx].Ctrl_Type == Ctrl_LockableButton )
                            {
                                if ( gfp_CTRL_Control_Table[CtrlIdx].Param2 == 0 )
                                {
                                    gfp_CTRL_Control_Table[CtrlIdx].Param2 = 1;
                                    SCRN_Redraw_WaitOne();
                                    g_CTRL_Focused = -1;
                                    return CtrlIdx;
                                }
                                else
                                {
                                    SCRN_Redraw_WaitOne();
                                    g_CTRL_Focused = -1;
                                    return 0;
                                }
                            }
                            else if ( gfp_CTRL_Control_Table[CtrlIdx].Ctrl_Type == Ctrl_ClickLink )
                            {
                                g_CTRL_Focused = -1;
                                return gfp_CTRL_Control_Table[CtrlIdx].Param0;  // Parent
                            }
                            else
                            {
                                SCRN_Redraw_WaitOne();
                                g_CTRL_Focused = -1;
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
        // HERE("( KBD_CheckBuffer() == 0 )");
        /*
            ProgramPath:
            KBD_CheckBuffer() == 0
        */

        MD_INT_SetMvOnly();  // ? turns off registering new clicks, therefore holding onto this/the existing click ?

        if ( MD_GetButtonStatus() == 0 )
        {
            // HERE("( MD_GetButtonStatus() == 0 )");
            // TODO(JimBalcomb,20220818): Y/N/M add support for ClickRec/MouseEmulation
        }
        else
        {
            // HERE("( MD_GetButtonStatus() != 0 )");
            /*
                ProgramPath:
                    KBD_CheckBuffer() == 0
                    MD_INT_SetMvOnly()
                    MD_GetButtonStatus() != 0
            */
            // HERE("MD_ButtonStatus = MD_GetButtonStatus();");
            MD_ButtonStatus = MD_GetButtonStatus();
            
            #ifdef DEBUG
            dlvfprintf("DEBUG: [%s, %d] MD_GetButtonStatus() |-> MD_ButtonStatus: %d\n", __FILE__, __LINE__, MD_ButtonStatus);
            #endif

            if ( MD_ButtonStatus != 2 )  // ? MD LEFT MOUSE BUTTON CLICK
            {
                // HERE("( MD_ButtonStatus != 2 )");
                /*
                    ProgramPath:
                        KBD_CheckBuffer() == 0
                        MD_INT_SetMvOnly()
                        MD_GetButtonStatus() != 0
                        MD_ButtonStatus = MD_GetButtonStatus()
                        MD_ButtonStatus != 2
                */
                if ( MD_GetButtonStatus() == 0 )
                {
                    // TODO(JimBalcomb,20220818): figure out why this is being checked again and where this program path goes
                    // HERE("( MD_GetButtonStatus() == 0 )");
                    /*
                        ProgramPath:
                            KBD_CheckBuffer() == 0
                            MD_INT_SetMvOnly()
                            MD_GetButtonStatus() != 0
                            MD_ButtonStatus = MD_GetButtonStatus()
                            MD_ButtonStatus != 2
                            MD_GetButtonStatus() == 0
                    */
                }  /* if ( MD_GetButtonStatus() == 0 ) */
                else
                {
                    // HERE("( MD_GetButtonStatus() != 0 )");
                    /*
                        ProgramPath:
                            KBD_CheckBuffer() == 0
                            MD_INT_SetMvOnly()
                            MD_GetButtonStatus() != 0
                            MD_ButtonStatus = MD_GetButtonStatus()
                            MD_ButtonStatus != 2
                            MD_GetButtonStatus() != 0
                    */
                    X_Pos = MD_GetX();
                    #ifdef DEBUG
                    dlvfprintf("DEBUG: [%s, %d] MD_GetX() |-> X_Pos: %d\n", __FILE__, __LINE__, X_Pos);
                    #endif
                    Y_Pos = MD_GetY();
                    #ifdef DEBUG
                    dlvfprintf("DEBUG: [%s, %d] MD_GetY() |-> Y_Pos: %d\n", __FILE__, __LINE__, Y_Pos);
                    #endif
                    g_GUI_Cursor_Offset = CR_GetOffset();
                    #ifdef DEBUG
                    dlvfprintf("DEBUG: [%s, %d] CR_GetOffset() |-> g_GUI_Cursor_Offset: %d\n", __FILE__, __LINE__, g_GUI_Cursor_Offset);
                    #endif
                    CtrlIdx = 0;
                    //Unused_Local = -1;  // ? related to, at the beginning, `g_CTRL_Focused = -1;` ?
                    InputCode = 0;
                    CtrlIdx = GUI_MouseOverControl();
                    #ifdef DEBUG
                    dlvfprintf("DEBUG: [%s, %d] GUI_MouseOverControl() |-> CtrlIdx: %d\n", __FILE__, __LINE__, CtrlIdx);
                    #endif
                    /*
                        ? big break/branch here ? whether the click is meaningful ?
                    */
                    if ( CtrlIdx == 0 )
                    {
                        // HERE("( CtrlIdx == 0 )");
                        /*
                            ProgramPath:
                                KBD_CheckBuffer() == 0
                                MD_INT_SetMvOnly()
                                MD_GetButtonStatus() != 0
                                MD_ButtonStatus = MD_GetButtonStatus()
                                MD_ButtonStatus != 2
                                MD_GetButtonStatus() != 0
                                CtrlIdx == 0
                        */

                        if ( g_CTRL_Focused != -1 )
                        {
                            // HERE("( g_CTRL_Focused != -1 )");

                            //if ( gfp_CTRL_Control_Table[g_CTRL_Focused].Ctrl_Type == Ctrl_Slidebar )
                            //    SCRN_CallRedrawFn()
                            //if ( ( gfp_CTRL_Control_Table[g_CTRL_Focused].Ctrl_Type != Ctrl_MStateButton ) && ( gfp_CTRL_Control_Table[g_CTRL_Focused].Ctrl_Type != Ctrl_DialogLine ) && ( gfp_CTRL_Control_Table[g_CTRL_Focused].Ctrl_Type != Ctrl_EditSelect ) )
                            //{
                            //    MD_CDraw_Disable(); CRL_Restore_RSP(); GUI_DrawControl(g_CTRL_Focused,0); CRL_Save_RSP(X_Pos,Y_Pos); CRH_Draw_RSP(X_Pos,Y_Pos); MD_MoveCursor(X_Pos,Y_Pos); MD_CDraw_Restore();
                            //}
                            g_CTRL_Focused = -1;
                        }
                        else
                        {
                            // HERE("( g_CTRL_Focused == -1 )");
                        }

                        // CRP_MD_SetClick_CilckRec2(X_Pos,Y_Pos);

                    }  /* if ( CtrlIdx == 0 ) */
                    else
                    {
                        // HERE("( CtrlIdx != 0 )");
                        /*
                            ProgramPath:
                                KBD_CheckBuffer() == 0
                                MD_INT_SetMvOnly()
                                MD_GetButtonStatus() != 0
                                MD_ButtonStatus = MD_GetButtonStatus()
                                MD_ButtonStatus != 2
                                MD_GetButtonStatus() != 0
                                CtrlIdx != 0
                        */

                        if ( CtrlIdx != g_CTRL_Focused )
                        {
                            // HERE("( CtrlIdx != g_CTRL_Focused )");

                        }
                        else
                        {
                            // HERE("( CtrlIdx == g_CTRL_Focused )");

                        }

                    }  /* else ( CtrlIdx != 0 ) */

                }  /* else ( MD_GetButtonStatus() != 0 ) */
                /*
                    @@PostClickProcess
                */
                //if ( gfp_CTRL_Control_Table[g_CTRL_MouseFocusCtrl].Ctrl_Type == Ctrl_Slidebar )
                //    SCRN_CallRedrawFn()
                g_CTRL_MouseFocusCtrl = 0;
                if ( CtrlIdx == 0 )
                {
                    // HERE("( CtrlIdx == 0 )");

                }
                else
                {
                    // HERE("( CtrlIdx != 0 )");
                    // @@PostClickProcess_CtrlIdx
                    /*
                    MD_GetClickRec1();
                    CRP_MD_GetClickRec2();
                    GUI_Processed_LastX = X_Pos;
                    GUI_Processed_LastY = Y_Pos;
                    GUI_Processed_Btns = MD_ButtonStatus;
                    */
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
                    switch(gfp_CTRL_Control_Table[CtrlIdx].Ctrl_Type)
                    {
                        case Ctrl_ClickButton:
                        {
                            // HERE("case Ctrl_ClickButton");
                            break;
                        }

                    }

                }

                //@@SetMdiMvBtns:
                MD_INT_SetMvBtns();
                g_CTRL_Focused == -1;
                if ( MD_ButtonStatus == 0 )
                {
                    // HERE("( MD_ButtonStatus == 0 )");
                    // @@JmpButtonStatusZero  @@ReturnFalse
                    goto ReturnFalse;

                }
                else
                {
                    // HERE("( MD_ButtonStatus != 0 )");
                    // @@Return_InputControlIndex

                }

            }  /* if ( MD_ButtonStatus != 2 ) */
            else
            {
                // HERE("( MD_ButtonStatus == 2 )");
                // TODO(JimBalcomb,20220818): add support for right-click/ContextHelp
            }  /* else ( MD_ButtonStatus == 2 ) */

        }  /* else ( MD_GetButtonStatus() != 0 ) */

    }  /* else ( KBD_CheckBuffer() == 0 ) */

    goto Done;

ReturnNegAX:
    // ? AX could be Param0 ? rather than CtrlIdx ?
    CtrlIdx = (CtrlIdx * -1);
    goto Done;


ReturnFalse:
    CtrlIdx = 0;
    goto Done;

Done:

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: IN_ProcessInput()\n", __FILE__, __LINE__);
#endif

    return CtrlIdx;
}
