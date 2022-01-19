TITLE _s34p01.asm GUI_ProcessInput

.MODEL LARGE, C

EXTRN g_DBG_ControlHighlight:WORD
EXTRN g_DBG_HelpHighlight:WORD
EXTRN gfp_GUI_Control_Table:DWORD
EXTRN g_GUI_Active_EditSlct:WORD
EXTRN g_GUI_ClickActivate:WORD
EXTRN g_GUI_Control_Count:WORD
EXTRN g_GUI_Cursor_Offset:WORD
EXTRN g_GUI_DialogDirections:WORD
EXTRN g_GUI_EditAnimStage:WORD
EXTRN g_GUI_EditCursorOn:WORD
EXTRN g_GUI_EditString:BYTE
EXTRN g_GUI_EscapeOverride:WORD
EXTRN g_GUI_FocusedControl:WORD
EXTRN g_GUI_Help_Available:WORD
EXTRN g_GUI_InEditSelect:WORD
EXTRN g_GUI_MouseFocusCtrl:WORD
EXTRN g_GUI_Processed_LastX:WORD
EXTRN g_GUI_Processed_LastY:WORD
EXTRN g_GUI_Processed_Btns:WORD

EXTRN DBG_IsDisabled:PROC
;EXTRN DBG_ScreenDump:PROC
;EXTRN DBG_Quit:PROC
EXTRN GUI_CallRedrawFn:PROC
;EXTRN GUI_ContextBasedHelp:PROC
EXTRN GUI_DrawCursor_RSP:PROC
EXTRN GUI_DrawControl:PROC
EXTRN GUI_GetCursorOffset:PROC
EXTRN GUI_FindWindow:PROC
EXTRN GUI_MouseOverControl:PROC
EXTRN GUI_NoTriggerKeys:PROC
EXTRN GUI_Redraw_WaitOne:PROC
EXTRN GUI_Redraw_WaitTimer:PROC
EXTRN GUI_RestoreCursorArea_RSP:PROC
EXTRN GUI_SaveCursorArea_RSP:PROC
EXTRN GUI_SetFocus:PROC
;EXTRN GUI_SlideDown:PROC
;EXTRN GUI_SlideUp:PROC
EXTRN KBD_CheckBuffer:PROC
EXTRN MOUSE_CDraw_Restore:PROC
EXTRN MOUSE_Disable_CDraw:PROC
EXTRN Mouse_GetButtonStatus:PROC
EXTRN MOUSE_GetClickedBtns:PROC
EXTRN MOUSE_GetClickRec1:PROC
EXTRN CRP_MOUSE_GetClickRec2:PROC
EXTRN MOUSE_GetClickX:PROC
EXTRN MOUSE_GetClickY:PROC
EXTRN MOUSE_GetX:PROC
EXTRN MOUSE_GetY:PROC
EXTRN MOUSE_INTSet_MvBtns:PROC
EXTRN MOUSE_INTSet_MvOnly:PROC
EXTRN MOUSE_MoveCursor:PROC
;EXTRN CRP_MOUSE_SetSecClick:PROC  ; TODO(JimBalcomb): Legacy - 
EXTRN strcpy:PROC


.CODE
;segment seg034 byte public 'CODE' use16
; 	 assume cs:seg034
; 	 ;org 0Eh
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_ProcessInput

GUI_CTRL struc; (sizeof=0x26)
Left dw ?
Top dw ?
Right dw ?
Bottom dw ?
Ctrl_Type dw ?
UU_Help dw ?
Font_Index dw ?
ColorSet1 dw ?
Rect_FX dw ?
Selectable dw ?
ColorSet2 dw ?
Param0 dw ?                             ; Text@                           (6:Min_Val, 9:Parent)
Param1 dw ?                             ; Trig_State     (6:Max_Val, 9/A:Index, B:IMG, C:CellW)
Param2 dw ?                             ; State@        (4/B:BG_Color, 6:StSeg, 8:Len, C:CellH)
Param3 dw ?                             ; FullFrames    (4:Align, 6:MinSlide, A:CBlock, C:Col@)
Param4 dw ?                             ; MaxSlide                           (4:CStyle, C:Row@)
Param5 dw ?                             ; IMG_Seg                        (4/B:MaxLen, 6:Orient)
Param6 dw ?                             ; CorsorColors@                    (6:StOff, B:Outline)
Hotkey db ?
Unused_25h db ?
GUI_CTRL ends

; enum GUI_Controls
Ctrl_ClickButton  = 0
Ctrl_ToggleButton  = 1
Ctrl_LockableButton  = 2
Ctrl_MStateButton  = 3
Ctrl_EditBox  = 4
Ctrl_ImageLabel  = 5
Ctrl_Slidebar  = 6
Ctrl_Label  = 7
Ctrl_AltString  = 8
Ctrl_ClickLink  = 9
Ctrl_DialogLine  = 0Ah
Ctrl_EditSelect  = 0Bh
Ctrl_ClickGrid  = 0Ch

; enum Key_Press
KP_Enter  = 0Ch
KP_Esc    = 1Bh
KP_F11    = 91h
KP_F12    = 92h


proc GUI_ProcessInput

    Input_Key            = byte ptr -15h
    Input_Control_Index  = word ptr -14h
    Unused_Local         = word ptr -12h
    Control_Bottom       = word ptr -10h
    Control_Right        = word ptr -0Eh
    Control_Top          = word ptr -0Ch
    Control_Left         = word ptr -0Ah
    Accepted_Char        = word ptr -8
    Mouse_Btn_Clicked    = word ptr -6
    Y_Pos                = word ptr -4
    X_Pos                = word ptr -2

    push bp
    mov  bp, sp
    sub  sp, 16h
    push si
    push di

    mov [bp+Input_Control_Index], 0
    mov [g_GUI_FocusedControl], -1
    mov [bp+Mouse_Btn_Clicked], 0
    mov [g_GUI_MouseFocusCtrl], 0

    call MOUSE_GetX
    mov [bp+X_Pos], ax
    call MOUSE_GetY
    mov [bp+Y_Pos], ax

    call GUI_GetCursorOffset
    mov [g_GUI_Cursor_Offset], ax

    call KBD_CheckBuffer
    or ax, ax
    jnz short loc_24DF0
    jmp loc_254D2

loc_24DF0:
    lea ax, [bp+Input_Control_Index]
    push ax
    ;nop
    ;push cs
    ;call near ptr GUI_NoTriggerKeys
    call GUI_NoTriggerKeys
    pop cx
    mov [bp+Input_Key], al
    cmp [bp+Input_Key], 0
    jnz short @@Check_DBG_KP_F11

loc_24E03:
    call MOUSE_INTSet_MvBtns

loc_24E08:
    xor ax, ax

loc_24E0A:
    jmp loc_25D07

@@Check_DBG_KP_F11:
    cmp [bp+Input_Key], KP_F11
    jnz short @@Check_DBG_KP_F12
    ;call DBG_Quit
    ; TODO(JimBalcomb): implement DBG_Quit()

@@Check_DBG_KP_F12:
    cmp [bp+Input_Key], KP_F12
    jnz short @@Check_KP_ESC

    ; call MOUSE_Disable_CDraw
    ; call GUI_RestoreCursorArea_RSP
    ; ;call DBG_ScreenDump
    ; ; TODO(JimBalcomb): implement DBG_ScreenDump()
    ; call MOUSE_GetY
    ; push ax
    ; call MOUSE_GetX
    ; push ax
    ; call GUI_SaveCursorArea_RSP
    ; pop cx
    ; pop cx
    ; call MOUSE_GetY
    ; push ax
    ; call MOUSE_GetX
    ; push ax
    ; call GUI_DrawCursor_RSP
    ; pop cx
    ; pop cx
    ; call MOUSE_GetY
    ; push ax
    ; call MOUSE_GetX
    ; push ax
    ; call MOUSE_MoveCursor
    ; pop cx
    ; pop cx
    ; call MOUSE_CDraw_Restore

@@Check_KP_ESC:
    cmp [bp+Input_Key], KP_Esc
    jnz short loc_24E7B

    cmp [g_GUI_EscapeOverride], 0
    jnz short loc_24E7B
    jmp loc_25C54

loc_24E7B:
    cmp [bp+Input_Key], '`'
    jnz short loc_24E9F

    call DBG_IsDisabled
    or ax, ax
    jnz short loc_24E9F
    cmp [g_DBG_ControlHighlight], 0
    jnz short loc_24E99
    mov [g_DBG_ControlHighlight], 1
    jmp short loc_24E9F

loc_24E99:
    mov [g_DBG_ControlHighlight], 0

loc_24E9F:
    cmp [bp+Input_Key], '~'
    jnz short loc_24EC3
    
    call DBG_IsDisabled
    or ax, ax
    jnz short loc_24EC3
    cmp [g_DBG_HelpHighlight], 0
    jnz short loc_24EBD
    mov [g_DBG_HelpHighlight], 1
    jmp short loc_24EC3

loc_24EBD:
    mov [g_DBG_HelpHighlight], 0

loc_24EC3:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_AltString
    jnz short loc_24EE0
    call MOUSE_INTSet_MvBtns
    jmp loc_25CF9

loc_24EE0:
    cmp [bp+Input_Control_Index], 0
    jnz short loc_24EE9
    jmp loc_2507A

loc_24EE9:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov al, [es:bx+GUI_CTRL.Hotkey]
    cmp al, [bp+Input_Key]
    jz short loc_24F03
    jmp loc_2507A

loc_24F03:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_Slidebar
    jnz short loc_24F1B
    jmp loc_24E03

loc_24F1B:
    cmp [bp+Input_Control_Index], 0
    jnz short loc_24F24
    jmp loc_25069

loc_24F24:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Right]
    push ax
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    pop ax
    sub ax, [es:bx+GUI_CTRL.Left]
    cwd
    sub ax, dx
    sar ax, 1
    push ax
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Left]
    pop dx
    add ax, dx
    mov [bp+X_Pos], ax
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Bottom]
    push ax
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    pop ax
    sub ax, [es:bx+GUI_CTRL.Top]
    cwd
    sub ax, dx
    sar ax, 1
    push ax
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Top]
    pop dx
    add ax, dx
    mov [bp+Y_Pos], ax
    push [bp+Y_Pos]
    push [bp+X_Pos]
    push [bp+Input_Control_Index]
    ;nop
    ;push cs
    ;call near ptr GUI_SetFocus
    call GUI_SetFocus
    add sp, 6
    call MOUSE_INTSet_MvBtns
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Ctrl_Type]
    cmp ax, Ctrl_ToggleButton
    jz short loc_25020
    cmp ax, Ctrl_LockableButton
    jz short loc_24FE4
    cmp ax, Ctrl_ClickLink
    jz short loc_2501D
    jmp loc_25067

loc_24FE4:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    cmp word ptr [bx], 0
    jnz short loc_25013
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    mov word ptr [bx], 1
    jmp short loc_2501B

loc_25013:
    ;nop
    ;push cs
    ;call near ptr GUI_Redraw_WaitOne
    call GUI_Redraw_WaitOne
    jmp loc_25C2B

loc_2501B:
    jmp short loc_25069

loc_2501D:
    jmp loc_25C61

loc_25020:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    cmp word ptr [bx], 0
    jnz short loc_2504F
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    mov word ptr [bx], 1
    jmp short loc_25065

loc_2504F:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    mov word ptr [bx], 0

loc_25065:
    jmp short loc_25069

loc_25067:
    jmp short $+2

loc_25069:
    ;nop
    ;push cs
    ;call near ptr GUI_Redraw_WaitOne
    call GUI_Redraw_WaitOne
    mov [g_GUI_FocusedControl], -1
    jmp loc_25CF9
    jmp loc_254D2

loc_2507A:
    cmp [bp+Input_Key], KP_Enter
    jz short loc_25083
    jmp loc_2537F

loc_25083:
    cmp [g_GUI_InEditSelect], 0
    jz short loc_250B9
    mov si, [g_GUI_Active_EditSlct]
    mov ax, offset g_GUI_EditString
    push ax
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    push [es:bx+GUI_CTRL.Param0]
    call strcpy
    pop cx
    pop cx
    cmp [g_GUI_ClickActivate], 0
    jnz short loc_250B6
    ;nop
    ;push cs
    ;call near ptr GUI_Redraw_WaitOne
    call GUI_Redraw_WaitOne

loc_250B6:
    jmp loc_253C9

loc_250B9:
    ;nop
    ;push cs
    ;call near ptr GUI_MouseOverControl
    call GUI_MouseOverControl
    mov si, ax
    or si, si
    jnz short loc_250C7
    jmp loc_24E08

loc_250C7:
    or si, si
    jg short loc_250CE
    jmp loc_25222

loc_250CE:
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_EditSelect
    jz short loc_250E5
    jmp loc_251AB

loc_250E5:
    cmp [g_GUI_InEditSelect], 0
    jnz short loc_25121
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    push [es:bx+GUI_CTRL.Param0]
    mov ax, offset g_GUI_EditString
    push ax
    call strcpy
    pop cx
    pop cx
    mov [g_GUI_EditAnimStage], 0
    mov [g_GUI_EditCursorOn], 0
    mov [g_GUI_InEditSelect], 1
    mov [g_GUI_Active_EditSlct], si
    jmp loc_251A9

loc_25121:
    cmp [g_GUI_Active_EditSlct], si
    jz short loc_25155
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    push [es:bx+GUI_CTRL.Param0]
    mov ax, offset g_GUI_EditString
    push ax
    call strcpy
    pop cx
    pop cx
    mov [g_GUI_EditAnimStage], 0
    mov [g_GUI_EditCursorOn], 0
    mov [g_GUI_Active_EditSlct], si
    jmp short loc_251A9

loc_25155:
    xor di, di
    jmp short loc_2515A

loc_25159:
    inc di

loc_2515A:
    cmp [g_GUI_EditString+di], 0
    jz short loc_25174
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Param5], di
    jg short loc_25159

loc_25174:
    cmp g_GUI_EditString[di-1], '_'
    jnz short loc_2517C
    dec di

loc_2517C:
    mov [g_GUI_EditString+di], 0
    mov [g_GUI_EditAnimStage], 0
    mov [g_GUI_Active_EditSlct], -1
    mov ax, offset g_GUI_EditString
    push ax
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    push [es:bx+GUI_CTRL.Param0]
    call strcpy
    pop cx
    pop cx

loc_251A9:
    jmp short loc_25222

loc_251AB:
    cmp [g_GUI_InEditSelect], 0
    jz short loc_25222
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_EditSelect
    jz short loc_25222
    xor di, di
    jmp short loc_251CB

loc_251CA:
    inc di

loc_251CB:
    cmp [g_GUI_EditString+di], 0
    jz short loc_251E6
    mov ax, [g_GUI_Active_EditSlct]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Param5], di
    jg short loc_251CA

loc_251E6:
    cmp g_GUI_EditString[di-1], '_'
    jnz short loc_251EE
    dec di

loc_251EE:
    mov [g_GUI_EditString+di], 0
    mov ax, offset g_GUI_EditString
    push ax
    mov ax, [g_GUI_Active_EditSlct]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    push [es:bx+GUI_CTRL.Param0]

loc_25209:
    call strcpy
    pop cx
    pop cx
    mov [g_GUI_EditAnimStage], 0
    mov [g_GUI_InEditSelect], 0
    mov [g_GUI_Active_EditSlct], -1

loc_25222:
    or si, si
    jnz short loc_25229
    jmp loc_2531A

loc_25229:
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_Slidebar
    jz short loc_25260
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_EditSelect
    jz short loc_25260
    push [bp+Y_Pos]
    push [bp+X_Pos]
    push si
    ;nop
    ;push cs
    ;call near ptr GUI_SetFocus
    call GUI_SetFocus
    add sp, 6

loc_25260:
    call MOUSE_INTSet_MvBtns
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Ctrl_Type]
    cmp ax, Ctrl_ToggleButton
    jz short loc_252C5
    cmp ax, Ctrl_LockableButton
    jz short loc_25288
    cmp ax, Ctrl_ClickLink
    jz short loc_252BA
    jmp loc_25309

loc_25288:
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    cmp word ptr [bx], 0
    jnz short loc_252B5
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    mov word ptr [bx], 1
    jmp short loc_252B8

loc_252B5:
    jmp loc_25C2B

loc_252B8:
    jmp short loc_2530B

loc_252BA:
    mov [g_GUI_FocusedControl], -1
    mov ax, si
    jmp loc_25C6A

loc_252C5:
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    cmp word ptr [bx], 0
    jnz short loc_252F2
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    mov word ptr [bx], 1
    jmp short loc_25307

loc_252F2:
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    mov word ptr [bx], 0

loc_25307:
    jmp short loc_2530B

loc_25309:
    jmp short $+2

loc_2530B:
    cmp [g_GUI_ClickActivate], 0
    jnz short loc_25317
    ;nop
    ;push cs
    ;call near ptr GUI_Redraw_WaitOne
    call GUI_Redraw_WaitOne

loc_25317:
    jmp loc_253C9

loc_2531A:
    cmp [g_GUI_DialogDirections], 0
    jz short loc_2537F
    mov si, 1
    jmp short loc_25379

loc_25326:
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_DialogLine
    jnz short loc_25378
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    mov ax, [bx]
    push ax
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    pop ax
    cmp ax, [es:bx+GUI_CTRL.Param1]
    jnz short loc_25378
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Selectable], 0
    jz short loc_25378
    jmp short loc_253C9

loc_25378:
    inc si

loc_25379:
    cmp si, [g_GUI_Control_Count]
    jl short loc_25326

loc_2537F:
    cmp [bp+Input_Key], '+'
    jz short loc_2538B
    cmp [bp+Input_Key], '-'
    jnz short loc_253D4

loc_2538B:
    ;nop
    ;push cs
    ;call near ptr GUI_MouseOverControl
    call GUI_MouseOverControl
    mov si, ax
    or si, si
    jz short loc_253D4
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_Slidebar
    jnz short loc_253D4
    cmp [bp+Input_Key], '+'
    jnz short loc_253B7
    push si
    ; ;nop
    ; ;push cs
    ; ;call near ptr GUI_SlideUp
    ; call GUI_SlideUp
    ; pop cx
    ; TODO(JimBalcomb): implement GUI_SlideUp()

loc_253B7:
    cmp [bp+Input_Key], '-'
    jnz short loc_253C4
    push si
    ; ;nop
    ; ;push cs
    ; ;call near ptr GUI_SlideDown
    ; call GUI_SlideDown
    ; pop cx
    ; TODO(JimBalcomb): implement GUI_SlideDown()

loc_253C4:
    call MOUSE_INTSet_MvBtns

loc_253C9:
    mov [g_GUI_FocusedControl], -1
    mov ax, si
    jmp loc_24E0A

loc_253D4:
    cmp [g_GUI_InEditSelect], 0
    jg short loc_253DE
    jmp loc_254C9

loc_253DE:
    cmp [bp+Input_Key], 0Bh
    jnz short loc_253E7
    jmp loc_25490

loc_253E7:
    mov [bp+Accepted_Char], 0
    cmp [bp+Input_Key], '@'
    jle short loc_253F8
    cmp [bp+Input_Key], ']'
    jl short loc_25410

loc_253F8:
    cmp [bp+Input_Key], '-'
    jle short loc_25404
    cmp [bp+Input_Key], ';'
    jl short loc_25410

loc_25404:
    cmp [bp+Input_Key], ' '
    jz short loc_25410
    cmp [bp+Input_Key], '-'
    jnz short loc_25417

loc_25410:
    mov [bp+Accepted_Char], 1
    jmp short loc_25428

loc_25417:
    cmp [bp+Input_Key], '`'
    jle short loc_25428
    cmp [bp+Input_Key], '{'
    jge short loc_25428
    mov [bp+Accepted_Char], 1

loc_25428:
    cmp [bp+Accepted_Char], 1
    jnz short loc_2548E
    xor si, si
    jmp short loc_25433

loc_25432:
    inc si

loc_25433:
    cmp [g_GUI_EditString+si], '_'
    jz short loc_25458
    cmp [g_GUI_EditString+si], 0
    jz short loc_25458
    mov ax, [g_GUI_Active_EditSlct]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Param5]
    dec ax
    cmp ax, si
    jg short loc_25432

loc_25458:
    mov ax, [g_GUI_Active_EditSlct]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Param5]
    dec ax
    cmp ax, si
    jle short loc_2547D

    mov al, [bp+Input_Key]
    mov [g_GUI_EditString+si], al
    mov g_GUI_EditString[si+1], 0
    jmp short loc_25482

loc_2547D:
    mov [g_GUI_EditString+si], 0

loc_25482:
    mov [g_GUI_EditCursorOn], 0
    mov [g_GUI_EditAnimStage], 0
    loc_2548E:
    jmp short loc_254C9

loc_25490:
    xor si, si
    jmp short loc_25495

loc_25494:
    inc si

loc_25495:
    cmp [g_GUI_EditString+si], 0
    jz short loc_254BA
    cmp [g_GUI_EditString+si], '_'
    jz short loc_254BA
    mov ax, [g_GUI_Active_EditSlct]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Param5]
    dec ax
    cmp ax, si
    jg short loc_25494

loc_254BA:
    or si, si
    jle short loc_254C9
    mov g_GUI_EditString[si-1], 0
    mov [g_GUI_EditAnimStage], 0

loc_254C9:
    mov [g_GUI_FocusedControl], -1
    jmp loc_24E03

loc_254D2:
    call MOUSE_INTSet_MvOnly
    call Mouse_GetButtonStatus
    or ax, ax
    jz short loc_254E3
    jmp loc_258A9

loc_254E3:
    call MOUSE_GetClickRec1
    or ax, ax
    jnz short loc_254F2
    jmp loc_24E03
    jmp loc_258A6

loc_254F2:
    call MOUSE_GetClickedBtns
    mov [bp+Mouse_Btn_Clicked], ax
    cmp [bp+Mouse_Btn_Clicked], 2
    jnz short loc_25527

    cmp [g_GUI_Help_Available], 0
    jz short loc_25513

    ; ;nop
    ; ;push cs
    ; ;call near ptr GUI_ContextBasedHelp
    ; call GUI_ContextBasedHelp
    mov ax, 0
    ; TODO(JimBalcomb): implement GUI_ContextBasedHelp()
    or ax, ax
    jnz short loc_25513
    jmp loc_258C7

loc_25513:
    cmp [g_GUI_EscapeOverride], 0
    jnz short loc_25527

loc_2551A:
    call MOUSE_GetClickRec1
    call CRP_MOUSE_GetClickRec2
    jmp loc_25C54

loc_25527:
    call MOUSE_GetClickX
    mov [bp+X_Pos], ax
    call MOUSE_GetClickY
    mov [bp+Y_Pos], ax
    mov [bp+Input_Control_Index], 0
    mov [bp+Unused_Local], -1
    mov [bp+Input_Key], 0
    push [bp+Y_Pos]
    push [bp+X_Pos]
    call GUI_FindWindow
    pop cx
    pop cx
    call GUI_GetCursorOffset
    mov [g_GUI_Cursor_Offset], ax
    mov si, 1
    jmp loc_255E9

loc_25560:
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Left]
    mov [bp+Control_Left], ax
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Top]
    mov [bp+Control_Top], ax
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Right]
    mov [bp+Control_Right], ax
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Bottom]
    mov [bp+Control_Bottom], ax
    mov ax, [bp+X_Pos]
    add ax, [g_GUI_Cursor_Offset]
    cmp ax, [bp+Control_Left]
    jl short loc_255E4
    mov ax, [bp+X_Pos]
    add ax, [g_GUI_Cursor_Offset]
    cmp ax, [bp+Control_Right]
    jg short loc_255E4
    mov ax, [bp+Y_Pos]
    add ax, [g_GUI_Cursor_Offset]
    cmp ax, [bp+Control_Top]
    jl short loc_255E4
    mov ax, [bp+Y_Pos]
    add ax, [g_GUI_Cursor_Offset]
    cmp ax, [bp+Control_Bottom]
    jg short loc_255E4
    mov [bp+Input_Control_Index], si
    jmp short loc_255F2

loc_255E4:
    mov ax, si
    inc ax
    mov si, ax

loc_255E9:
    cmp si, [g_GUI_Control_Count]
    jge short loc_255F2
    jmp loc_25560

loc_255F2:
    cmp [bp+Input_Control_Index], 0
    jz short loc_25629
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_EditSelect
    jz short loc_25629
    mov ax, [bp+Input_Control_Index]
    mov [g_GUI_MouseFocusCtrl], ax
    push [bp+Y_Pos]
    push [bp+X_Pos]
    push [bp+Input_Control_Index]
    ;nop
    ;push cs
    ;call near ptr GUI_SetFocus
    call GUI_SetFocus
    add sp, 6
    ;nop
    ;push cs
    ;call near ptr GUI_Redraw_WaitOne
    call GUI_Redraw_WaitOne

loc_25629:
    cmp [bp+Input_Control_Index], 0
    jnz short loc_25632
    jmp loc_25777

loc_25632:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_EditSelect
    jz short loc_2564A
    jmp loc_2571A

loc_2564A:
    cmp [g_GUI_InEditSelect], 0
    jnz short loc_25689
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    push [es:bx+GUI_CTRL.Param0]
    mov ax, offset g_GUI_EditString
    push ax
    call strcpy
    pop cx
    pop cx
    mov [g_GUI_EditAnimStage], 0
    mov [g_GUI_EditCursorOn], 0
    mov [g_GUI_InEditSelect], 1
    mov ax, [bp+Input_Control_Index]
    mov [g_GUI_Active_EditSlct], ax
    jmp loc_25718

loc_25689:
    mov ax, [g_GUI_Active_EditSlct]
    cmp ax, [bp+Input_Control_Index]
    jz short loc_256C2
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    push [es:bx+GUI_CTRL.Param0]
    mov ax, offset g_GUI_EditString
    push ax
    call strcpy
    pop cx
    pop cx
    mov [g_GUI_EditAnimStage], 0
    mov [g_GUI_EditCursorOn], 0
    mov ax, [bp+Input_Control_Index]
    mov [g_GUI_Active_EditSlct], ax
    jmp short loc_25718

loc_256C2:
    xor di, di
    jmp short loc_256C7

loc_256C6:
    inc di

loc_256C7:
    cmp [g_GUI_EditString+di], 0
    jz short loc_256E2
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Param5], di
    jg short loc_256C6

loc_256E2:
    cmp g_GUI_EditString[di-1], '_'
    jnz short loc_256EA
    dec di

loc_256EA:
    mov [g_GUI_EditString+di], 0
    mov [g_GUI_EditAnimStage], 0
    mov [g_GUI_Active_EditSlct], -1
    mov ax, offset g_GUI_EditString
    push ax
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    push [es:bx+GUI_CTRL.Param0]
    call strcpy
    pop cx
    pop cx

loc_25718:
    jmp short loc_25777

loc_2571A:
    cmp [g_GUI_InEditSelect], 0
    jz short loc_25777
    xor di, di
    jmp short loc_25726

loc_25725:
    inc di

loc_25726:
    cmp [g_GUI_EditString+di], 0
    jz short loc_25741
    mov ax, [g_GUI_Active_EditSlct]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Param5], di
    jg short loc_25725

loc_25741:
    cmp g_GUI_EditString[di-1], '_'
    jnz short loc_25749
    dec di

loc_25749:
    mov [g_GUI_EditString+di], 0
    ; TODO(JimBalcomb): figure out why this on has "offset" and the next one does not
    mov ax, offset g_GUI_EditString
    push ax
    mov ax, [g_GUI_Active_EditSlct]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    push [es:bx+GUI_CTRL.Param0]
    call strcpy
    pop cx
    pop cx
    mov [g_GUI_InEditSelect], 0
    mov [g_GUI_Active_EditSlct], -1

loc_25777:
    mov [g_GUI_FocusedControl], -1
    cmp [bp+Input_Control_Index], 0
    jz short loc_2579A

    call CRP_MOUSE_GetClickRec2
    mov ax, [bp+X_Pos]
    mov [g_GUI_Processed_LastX], ax
    mov ax, [bp+Y_Pos]
    mov [g_GUI_Processed_LastY], ax
    mov ax, [bp+Mouse_Btn_Clicked]
    mov [g_GUI_Processed_Btns], ax

loc_2579A:
    cmp [bp+Input_Control_Index], 0
    jnz short loc_257A3
    jmp loc_2588D

loc_257A3:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Ctrl_Type]
    dec bx
    cmp bx, 8
    jbe short loc_257BE
    jmp loc_2588B

loc_257BE:
    shl bx, 1
    jmp [cs:off_25D20+bx]

loc_257C5:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    cmp word ptr [bx], 0
    jnz short loc_257F4
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    mov word ptr [bx], 1
    jmp short loc_257F7

loc_257F4:
    jmp loc_25C26

loc_257F7:
    jmp loc_2588D

loc_257FA:
    cmp [bp+Mouse_Btn_Clicked], 2
    jnz short loc_2582E
    call MOUSE_INTSet_MvBtns
    mov [g_GUI_FocusedControl], -1
    cmp [g_GUI_EscapeOverride], 0
    jz short loc_25829

loc_25812:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Param0]
    jmp loc_25D02
    jmp short loc_2582C

loc_25829:
    jmp loc_25C54

loc_2582C:
    jmp short loc_25831

loc_2582E:
    jmp loc_25C5C

loc_25831:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    cmp word ptr [bx], 0
    jnz short loc_25860
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    mov word ptr [bx], 1
    jmp short loc_25876

loc_25860:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    mov word ptr [bx], 0

loc_25876:
    jmp short loc_2588D

loc_25878:
    push [bp+Y_Pos]
    push [bp+X_Pos]
    push [bp+Input_Control_Index]
    ;nop
    ;push cs
    ;call near ptr GUI_SetFocus
    call GUI_SetFocus
    add sp, 6
    jmp short loc_2588D

loc_2588B:
    jmp short $+2

loc_2588D:
    call MOUSE_INTSet_MvBtns
    mov [g_GUI_MouseFocusCtrl], 0
    cmp [bp+Mouse_Btn_Clicked], 2
    jnz short loc_258A3
    jmp loc_25CFF
    jmp short loc_258A6

loc_258A3:
    jmp loc_25CF9

loc_258A6:
    jmp loc_25BAA

loc_258A9:
    call Mouse_GetButtonStatus
    mov [bp+Mouse_Btn_Clicked], ax
    cmp [bp+Mouse_Btn_Clicked], 2
    jnz short loc_258EF
    cmp [g_GUI_Help_Available], 0
    jz short loc_258D4
    ; ;nop
    ; ;push cs
    ; ;call near ptr GUI_ContextBasedHelp
    ; call GUI_ContextBasedHelp
    mov ax, 0
    ; TODO(JimBalcomb): implement GUI_ContextBasedHelp()
    or ax, ax
    jnz short loc_258D4

loc_258C7:
    call MOUSE_GetClickRec1
    call CRP_MOUSE_GetClickRec2
    jmp loc_24E08

loc_258D4:
    cmp [g_GUI_EscapeOverride], 0
    jnz short loc_258EF
    jmp short loc_258E2

loc_258DD:
    ;nop
    ;push cs
    ;call near ptr GUI_Redraw_WaitOne
    call GUI_Redraw_WaitOne

loc_258E2:
    call Mouse_GetButtonStatus
    cmp ax, 2
    jz short loc_258DD
    jmp loc_2551A

loc_258EF:
    jmp loc_25B84

loc_258F2:
    call MOUSE_GetX
    mov [bp+X_Pos], ax
    call MOUSE_GetY
    mov [bp+Y_Pos], ax
    call GUI_GetCursorOffset
    mov [g_GUI_Cursor_Offset], ax
    mov [bp+Input_Control_Index], 0
    mov [bp+Unused_Local], -1
    mov [bp+Input_Key], 0
    ;nop
    ;push cs
    ;call near ptr GUI_MouseOverControl
    call GUI_MouseOverControl
    mov [bp+Input_Control_Index], ax
    cmp [bp+Input_Control_Index], 0
    jz short loc_25929
    jmp loc_259E6

loc_25929:
    ;db 83h,3Eh,22h,4Dh,0FFh ; <BAD>cmp [g_GUI_FocusedControl], -1
    cmp [g_GUI_FocusedControl], -1
    jnz short loc_25933
    jmp loc_259D6

loc_25933:
    mov ax, [g_GUI_FocusedControl]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_Slidebar
    jnz short loc_2594D
    nop
    push cs
    call near ptr GUI_CallRedrawFn

loc_2594D:
    mov ax, [g_GUI_FocusedControl]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_MStateButton
    jz short loc_259D0
    mov ax, [g_GUI_FocusedControl]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_DialogLine
    jz short loc_259D0
    mov ax, [g_GUI_FocusedControl]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_EditSelect
    jz short loc_259D0
    call MOUSE_Disable_CDraw
    call GUI_RestoreCursorArea_RSP
    xor ax, ax
    push ax
    push [g_GUI_FocusedControl]
    nop
    push cs
    call near ptr GUI_DrawControl
    pop cx
    pop cx
    push [bp+Y_Pos]
    push [bp+X_Pos]
    call GUI_SaveCursorArea_RSP
    pop cx
    pop cx
    push [bp+Y_Pos]
    push [bp+X_Pos]
    call GUI_DrawCursor_RSP
    pop cx
    pop cx
    push [bp+Y_Pos]
    push [bp+X_Pos]
    call MOUSE_MoveCursor
    pop cx
    pop cx
    call MOUSE_CDraw_Restore

loc_259D0:
    mov [g_GUI_FocusedControl], 0FFFFh

loc_259D6:
    ;push [bp+Y_Pos]
    ;push [bp+X_Pos]
    ;call CRP_MOUSE_SetSecClick
    ;pop cx
    ;pop cx
    jmp loc_25B90

loc_259E6:
    mov ax, [bp+Input_Control_Index]
    cmp ax, [g_GUI_FocusedControl]
    jnz short loc_259F2
    jmp loc_25A78

loc_259F2:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_EditBox
    jz short loc_25A78
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_EditSelect
    jz short loc_25A78
    ;db 83h,3Eh,22h,4Dh,0FFh ; <BAD>cmp [g_GUI_FocusedControl], -1
    cmp [g_GUI_FocusedControl], -1
    jz short loc_25A4D
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_ClickGrid
    jnz short loc_25A4D
    mov ax, [g_GUI_FocusedControl]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_ClickGrid
    jz short loc_25A78

loc_25A4D:
    mov ax, [g_GUI_FocusedControl]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_Slidebar
    jnz short loc_25A67
    nop
    push cs
    call near ptr GUI_CallRedrawFn

loc_25A67:
    push [bp+Y_Pos]
    push [bp+X_Pos]
    push [bp+Input_Control_Index]
    nop
    push cs
    call near ptr GUI_SetFocus
    add sp, 6

loc_25A78:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_EditSelect
    jnz short loc_25B0A
    cmp [g_GUI_InEditSelect], 0
    jnz short loc_25ACB
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    push [es:bx+GUI_CTRL.Param0]
    mov ax, offset g_GUI_EditString
    push ax
    call strcpy
    pop cx
    pop cx
    mov [g_GUI_EditAnimStage], 0
    mov [g_GUI_EditCursorOn], 0
    mov [g_GUI_InEditSelect], 1
    mov ax, [bp+Input_Control_Index]
    mov [g_GUI_Active_EditSlct], ax
    jmp short loc_25B08

loc_25ACB:
    mov ax, [g_GUI_Active_EditSlct]
    cmp ax, [bp+Input_Control_Index]
    jz short loc_25B08
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    push [es:bx+GUI_CTRL.Param0]
    mov ax, offset g_GUI_EditString
    push ax
    call strcpy
    pop cx
    pop cx
    mov [g_GUI_EditAnimStage], 0
    mov [g_GUI_EditCursorOn], 0
    mov [g_GUI_InEditSelect], 1
    mov ax, [bp+Input_Control_Index]
    mov [g_GUI_Active_EditSlct], ax

loc_25B08:
    jmp short loc_25B67

loc_25B0A:
    cmp [g_GUI_InEditSelect], 0
    jz short loc_25B67
    xor di, di
    jmp short loc_25B16

loc_25B15:
    inc di

loc_25B16:
    cmp [g_GUI_EditString+di], 0
    jz short loc_25B31
    mov ax, [g_GUI_Active_EditSlct]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+20h], di
    jg short loc_25B15

loc_25B31:
    cmp g_GUI_EditString[di-1], '_'
    jnz short loc_25B39
    dec di

loc_25B39:
    mov [g_GUI_EditString+di], 0
    mov ax, offset g_GUI_EditString
    push ax
    mov ax, [g_GUI_Active_EditSlct]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    push [es:bx+GUI_CTRL.Param0]
    call strcpy
    pop cx
    pop cx
    mov [g_GUI_InEditSelect], 0
    mov [g_GUI_Active_EditSlct], -1

loc_25B67:
    mov ax, [bp+Input_Control_Index]
    mov [g_GUI_MouseFocusCtrl], ax
    cmp [g_GUI_ClickActivate], 0
    jz short loc_25B76
    jmp short loc_25B90

loc_25B76:
    call Mouse_GetButtonStatus
    or ax, ax
    jz short loc_25B84
    nop
    push cs
    call near ptr GUI_Redraw_WaitTimer

loc_25B84:
    call Mouse_GetButtonStatus
    or ax, ax
    jz short loc_25B90
    jmp loc_258F2

loc_25B90:
    mov ax, [g_GUI_MouseFocusCtrl]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_Slidebar
    jnz short loc_25BAA
    nop
    push cs
    call near ptr GUI_CallRedrawFn

loc_25BAA:
    mov [g_GUI_MouseFocusCtrl], 0
    cmp [bp+Input_Control_Index], 0
    jnz short loc_25BB9
    jmp loc_25CD8

loc_25BB9:
    call MOUSE_GetClickRec1
    call CRP_MOUSE_GetClickRec2
    mov ax, [bp+X_Pos]
    mov [g_GUI_Processed_LastX], ax
    mov ax, [bp+Y_Pos]
    mov [g_GUI_Processed_LastY], ax
    mov ax, [bp+Mouse_Btn_Clicked]
    mov [g_GUI_Processed_Btns], ax
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Ctrl_Type]
    dec bx
    cmp bx, 8
    jbe short loc_25BF0
    jmp loc_25CD6

loc_25BF0:
    shl bx, 1
    jmp [cs:off_25D0E+bx]

loc_25BF7:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    cmp word ptr [bx], 0
    jnz short loc_25C26
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    mov word ptr [bx], 1
    jmp short loc_25C34

loc_25C26:
    call MOUSE_INTSet_MvBtns

loc_25C2B:
    mov [g_GUI_FocusedControl], -1
    jmp loc_24E08

loc_25C34:
    jmp loc_25CD8

loc_25C37:
    cmp [bp+Mouse_Btn_Clicked], 2
    jnz short loc_25C5C
    call MOUSE_INTSet_MvBtns
    mov [g_GUI_FocusedControl], -1
    cmp [g_GUI_EscapeOverride], 0
    jz short loc_25C54
    jmp loc_25812
    jmp short loc_25C5A

loc_25C54:
    mov ax, -1
    jmp loc_24E0A

loc_25C5A:
    jmp short loc_25C7C

loc_25C5C:
    call MOUSE_INTSet_MvBtns
    loc_25C61:
    mov [g_GUI_FocusedControl], -1
    mov ax, [bp+Input_Control_Index]

loc_25C6A:
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Param0]
    jmp loc_24E0A

loc_25C7C:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    cmp word ptr [bx], 0
    jnz short loc_25CAB
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    mov word ptr [bx], 1
    jmp short loc_25CC1

loc_25CAB:
    mov ax, [bp+Input_Control_Index]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param2]
    mov word ptr [bx], 0

loc_25CC1:
    jmp short loc_25CD8

loc_25CC3:
    push [bp+Y_Pos]
    push [bp+X_Pos]
    push [bp+Input_Control_Index]
    nop
    push cs
    call near ptr GUI_SetFocus
    add sp, 6
    jmp short loc_25CD8

loc_25CD6:
    jmp short $+2

loc_25CD8:
    call MOUSE_INTSet_MvBtns
    mov [g_GUI_FocusedControl], -1
    mov ax, [bp+Mouse_Btn_Clicked]
    or ax, ax
    jz short loc_25CF6
    cmp ax, 1
    jz short loc_25CF9
    cmp ax, 2
    jz short loc_25CFF
    jmp short loc_25D07

loc_25CF6:
    jmp loc_24E08

loc_25CF9:
    mov ax, [bp+Input_Control_Index]
    jmp loc_24E0A

loc_25CFF:
    mov ax, [bp+Input_Control_Index]

loc_25D02:
    neg ax
    jmp loc_24E0A

loc_25D07:
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp GUI_ProcessInput

align 2
off_25D0E dw offset loc_25C7C
dw offset loc_25BF7
dw offset loc_25CD6
dw offset loc_25CC3
dw offset loc_25CD6
dw offset loc_25CD6
dw offset loc_25CD6
dw offset loc_25CD6
dw offset loc_25C37
off_25D20 dw offset loc_25831
dw offset loc_257C5
dw offset loc_2588B
dw offset loc_25878
dw offset loc_2588B
dw offset loc_2588B
dw offset loc_2588B
dw offset loc_2588B
dw offset loc_257FA

;ends seg034

end
