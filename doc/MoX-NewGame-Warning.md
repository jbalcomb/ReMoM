

// MoO2  GENDRAW  Warning_Box_()

MoX
GENDRAW.c
/*
NEWGAME.LBX, 044  WARNBACK   Warning box top
NEWGAME.LBX, 045  WARNBACK   Warning box buttom
// MGC  dseg:90C2 00 00                                           _warning2_bottom_seg dw 0               ; DATA XREF: Warning_Message+68w ...
// MGC  dseg:90C2                                                                                         ; appended into the LBX_Sandbox_Segment
// MGC  dseg:90C4 00 00                                           _warning2_top_seg dw 0                  ; DATA XREF: Warning_Message+51w ...
// MGC  dseg:90C4                                                                                         ; appended into the LBX_Sandbox_Segment
// MGC  dseg:90C6 00 00                                           _warning_bottom_seg dw 0                ; DATA XREF: Newgame_Screen_6__WIP+BFw ...
// MGC  dseg:90C6                                                                                         ; appended into the LBX_Sandbox_Segment
// MGC  dseg:90C8 00 00                                           _warning_top_seg dw 0                   ; DATA XREF: Newgame_Screen_6__WIP+A8w ...
// MGC  dseg:90C8                                                                                         ; LBXE_LoadReplace into the LBX_Sandbox_Segment
*/
// WZD dseg:CB02
SAMB_ptr IMG_GUI_RedMsg2Btm;
// WZD dseg:CB04
SAMB_ptr IMG_GUI_RedMessage2;
// WZD dseg:CB06
SAMB_ptr IMG_GUI_RedMsg1Btm;
// WZD dseg:CB08
SAMB_ptr IMG_GUI_RedMessage1;



## _warning_top_seg
XREF
Down w Newgame_Screen_6__WIP+A8 mov     [_warning_top_seg], ax          ; LBXE_LoadReplace into the LBX_Sandbox_Segment
Down w GAME_New_Screen_4+AF     mov     [_warning_top_seg], ax          ; LBXE_LoadReplace into the LBX_Sandbox_Segment
Down w GAME_New_Screen_5+10E    mov     [_warning_top_seg], ax          ; LBXE_LoadReplace into the LBX_Sandbox_Segment
Down w Warning_Message+23       mov     [_warning_top_seg], ax          ; LBXE_LoadReplace into the LBX_Sandbox_Segment
Down r GUI_DrawRedMessage+50    push    [_warning_top_seg]              ; LBXE_LoadReplace into the LBX_Sandbox_Segment


## Warning_Message
XREF
Up J j_GUI_ShowRedMessage  jmp     Warning_Message                   ; loads and displays the provided message in a red
Up p UU_GUI_WarningType1+E call    near ptr Warning_Message          ; loads and displays the provided message in a red
Up p GUI_WarningType0+E    call    near ptr Warning_Message          ; loads and displays the provided message in a red

proc NX_j_GUI_ShowRedMessage far
jmp     Warning_Message                   ; loads and displays the provided message in a red

## j_GUI_WarningType0
XREF
Down P Newgame_Screen_6__WIP+246 call    j_GUI_WarningType0                ; displays the passed message in a red warning dialog
Down P GAME_New_Screen_4+6D4     call    j_GUI_WarningType0                ; displays the passed message in a red warning dialog
Down P GAME_New_Screen_5+706     call    j_GUI_WarningType0                ; displays the passed message in a red warning dialog

...

; int __cdecl __far NX_j_UU_GUI_WarningType1(char *Msg@)
proc NX_j_UU_GUI_WarningType1 far
jmp     UU_GUI_WarningType1  



NEWGAME.LBX, 044  WARNBACK   Warning box top
NEWGAME.LBX, 045  WARNBACK   Warning box buttom


e.g.
"""
You can not select a
Myrran race unless you
have the Myrran special.
"""
