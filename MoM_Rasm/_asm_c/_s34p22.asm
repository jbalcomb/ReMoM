TITLE _s34p22.asm GUI_NoTriggerKeys

.MODEL LARGE, C

EXTRN g_GUI_ActiveStringTrig:WORD
EXTRN g_GUI_Control_Count:WORD
EXTRN g_GUI_Last_Key_Control:WORD
EXTRN gfp_GUI_Control_Table:DWORD
EXTRN g_MOUSE_Emu_Y:WORD
EXTRN g_MOUSE_Emu_X:WORD
EXTRN g_GUI_Cursor_Offset:WORD

EXTRN strcpy:PROC
EXTRN GUI_DrawCursor_RSP:PROC
EXTRN GUI_FindWindow:PROC
EXTRN GUI_GetCursorOffset:PROC
;EXTRN GUI_ProcessDirKey:PROC
EXTRN GUI_RestoreCursorArea_RSP:PROC
EXTRN GUI_SaveCursorArea_RSP:PROC
EXTRN KBD_GetKey:PROC
EXTRN MOUSE_CDraw_Restore:PROC
EXTRN MOUSE_Disable_CDraw:PROC
EXTRN MOUSE_MoveCursor:PROC


.CODE
;segment seg034 byte public 'CODE' use16
;    assume cs:seg034
;    ;org 0Eh
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_NoTriggerKeys

GUI_CTRL struc ; (sizeof=0x26)
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

; KP_Left, KP_Right, KP_Up, KP_Down, KP_PgUp, KP_Home, KP_End, KP_PgDn
; enum Key_Press
KP_Left  = 1
KP_Right  = 2
KP_Up  = 3
KP_Down  = 4
KP_PgUp  = 19h
KP_Home  = 1Ah
KP_End  = 1Ch
KP_PgDn  = 1Dh


proc GUI_NoTriggerKeys

    Local_String= byte ptr -28h
    Input_Key_Extended= word ptr -0Ah
    Original_Key= byte ptr -8
    Capitalized_Key= byte ptr -7
    Reset_String_Triggers= word ptr -6
    Loop_Var= word ptr -4
    Control_Index= word ptr -2
    Retn_Pointer= word ptr 6
    
    push bp
    mov  bp, sp
    sub  sp, 28h
    push si
    push di
    
    mov di, [bp+Retn_Pointer]

    call KBD_GetKey
    mov [bp+Capitalized_Key], al
    mov ax, [g_GUI_Last_Key_Control]
    cmp ax, [g_GUI_Control_Count]
    jl short loc_266BD
    mov [g_GUI_Last_Key_Control], 0

loc_266BD:
    mov ax, [g_GUI_Last_Key_Control]
    inc ax
    mov si, ax
    mov al, [bp+Capitalized_Key]
    mov [bp+Original_Key], al
    cmp [bp+Capitalized_Key], 60h
    jle short loc_266DD
    cmp [bp+Capitalized_Key], 7Bh
    jge short loc_266DD
    mov al, [bp+Capitalized_Key]
    add al, 0E0h
    mov [bp+Capitalized_Key], al

loc_266DD:
    cmp [g_GUI_ActiveStringTrig], 0
    jz short loc_2671E
    mov si, [g_GUI_ActiveStringTrig]
    mov al, [bp+Capitalized_Key]
    add al, 0A1h
    mov [bp+Capitalized_Key], al
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov al, [es:bx+GUI_CTRL.Hotkey]
    cmp al, [bp+Capitalized_Key]
    jnz short loc_26710
    mov al, [bp+Capitalized_Key]
    add al, 5Fh
    mov [bp+Capitalized_Key], al
    jmp short loc_2671C

loc_26710:
    mov al, [bp+Capitalized_Key]
    add al, 5Fh
    mov [bp+Capitalized_Key], al
    mov si, [g_GUI_Control_Count]

loc_2671C:
    jmp short loc_26722

loc_2671E:
    mov si, [g_GUI_Control_Count]

loc_26722:
    cmp si, [g_GUI_Control_Count]
    jz short loc_2672B
    jmp loc_2683B

loc_2672B:
    mov [g_GUI_ActiveStringTrig], 0
    mov ax, [g_GUI_Last_Key_Control]
    inc ax
    mov si, ax
    jmp short loc_26783

loc_26739:
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_AltString
    jnz short loc_26782
    mov al, [bp+Capitalized_Key]
    add al, 0A1h
    mov [bp+Capitalized_Key], al
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov al, [es:bx+GUI_CTRL.Hotkey]
    cmp al, [bp+Capitalized_Key]
    jnz short loc_26777
    mov al, [bp+Capitalized_Key]
    add al, 5Fh
    mov [bp+Capitalized_Key], al
    jmp short loc_267B3
    jmp short loc_26780

loc_26777:
    mov al, [bp+Capitalized_Key]
    add al, 5Fh
    mov [bp+Capitalized_Key], al
    inc si

loc_26780:
    jmp short loc_26783

loc_26782:
    inc si

loc_26783:
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov al, [es:bx+GUI_CTRL.Hotkey]
    cmp al, [bp+Capitalized_Key]
    jnz short loc_267AD
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_AltString
    jnz short loc_267B3

loc_267AD:
    cmp si, [g_GUI_Control_Count]
    jnz short loc_26739

loc_267B3:
    cmp si, [g_GUI_Control_Count]
    jz short loc_267BC
    db 0E9h,7Fh,0 ; <BAD>jmp loc_2683B

loc_267BC:
    mov si, 1
    jmp short loc_2680B

loc_267C1:
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_AltString
    jnz short loc_2680A
    mov al, [bp+Capitalized_Key]
    add al, 0A1h
    mov [bp+Capitalized_Key], al
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov al, [es:bx+GUI_CTRL.Hotkey]
    cmp al, [bp+Capitalized_Key]
    jnz short loc_267FF
    mov al, [bp+Capitalized_Key]
    add al, 5Fh
    mov [bp+Capitalized_Key], al
    jmp short loc_2683B
    jmp short loc_26808

loc_267FF:
    mov al, [bp+Capitalized_Key]
    add al, 5Fh
    mov [bp+Capitalized_Key], al
    inc si

loc_26808:
    jmp short loc_2680B

loc_2680A:
    inc si

loc_2680B:
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov al, [es:bx+GUI_CTRL.Hotkey]
    cmp al, [bp+Capitalized_Key]
    jnz short loc_26835
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_AltString
    jnz short loc_2683B

loc_26835:
    cmp si, [g_GUI_Control_Count]
    jnz short loc_267C1

loc_2683B:
    mov [g_GUI_Last_Key_Control], si
    mov [bp+Reset_String_Triggers], 1
    cmp si, [g_GUI_Control_Count]
    jl short loc_2684D
    jmp loc_26A4C

loc_2684D:
    mov [di], si
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Left], 140h
    jl short loc_26866
    jmp loc_26975

loc_26866:
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Top], 0C8h
    jl short loc_2687E
    jmp loc_26975

loc_2687E:
    call MOUSE_Disable_CDraw
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Right]
    push ax
    mov ax, si
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
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Left]
    pop dx
    add ax, dx
    mov [g_MOUSE_Emu_X], ax
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Bottom]
    push ax
    mov ax, si
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
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Top]
    pop dx
    add ax, dx
    mov [g_MOUSE_Emu_Y], ax
    cmp [g_MOUSE_Emu_X], 140h
    jge short loc_26970
    cmp [g_MOUSE_Emu_Y], 0C8h
    jge short loc_26970
    push [g_MOUSE_Emu_Y]
    push [g_MOUSE_Emu_X]
    call GUI_FindWindow
    pop cx
    pop cx

    call GUI_GetCursorOffset
    mov [g_GUI_Cursor_Offset], ax

    mov ax, [g_MOUSE_Emu_X]
    sub ax, [g_GUI_Cursor_Offset]
    mov [g_MOUSE_Emu_X], ax
    mov ax, [g_MOUSE_Emu_Y]
    sub ax, [g_GUI_Cursor_Offset]
    mov [g_MOUSE_Emu_Y], ax

    push [g_MOUSE_Emu_Y]
    push [g_MOUSE_Emu_X]
    call MOUSE_MoveCursor
    pop cx
    pop cx

    call GUI_RestoreCursorArea_RSP
    push [g_MOUSE_Emu_Y]
    push [g_MOUSE_Emu_X]
    call GUI_SaveCursorArea_RSP
    pop cx
    pop cx

    push [g_MOUSE_Emu_Y]
    push [g_MOUSE_Emu_X]
    call GUI_DrawCursor_RSP
    pop cx
    pop cx

loc_26970:
    call MOUSE_CDraw_Restore

loc_26975:
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_AltString
    jz short loc_2698C
    jmp loc_26A4A

loc_2698C:
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    push [es:bx+GUI_CTRL.Param0]
    lea ax, [bp+Local_String]
    push ax
    call strcpy
    pop cx
    pop cx
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    inc [es:bx+GUI_CTRL.Param1]
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Param1]
    push ax
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    pop ax
    cmp ax, [es:bx+GUI_CTRL.Param2]
    jl short loc_26A0E
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov [es:bx+GUI_CTRL.Param1], 0
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov al, [bp+Local_String]
    mov [es:bx+GUI_CTRL.Hotkey], al
    mov [g_GUI_ActiveStringTrig], 0
    jmp short loc_26A45

loc_26A0E:
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param1]
    lea ax, [bp+Local_String]
    add bx, ax
    mov al, [bx]
    push ax
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    pop ax
    mov [es:bx+GUI_CTRL.Hotkey], al
    mov [g_GUI_ActiveStringTrig], si
    mov word ptr [di], 0
    mov [bp+Capitalized_Key], 0

loc_26A45:
    mov [bp+Reset_String_Triggers], 0

loc_26A4A:
    jmp short loc_26AAA

loc_26A4C:
    mov ax, [di]
    mov [bp+Control_Index], ax
    mov al, [bp+Capitalized_Key]
    cbw
    mov [bp+Input_Key_Extended], ax
    mov cx, 8
    mov bx, offset word_26B47

loc_26A5E:
    mov ax, [cs:bx]
    cmp ax, [bp+Input_Key_Extended]
    jz short loc_26A6D
    add bx, 2
    loop loc_26A5E
    jmp short loc_26AA3

loc_26A6D:
    jmp [word ptr cs:bx+10h]

loc_26A71:
    mov ax, 4Ch

loc_26A74:
    push ax
    ; ;nop
    ; ;push cs
    ; ;call near ptr GUI_ProcessDirKey
    ; call GUI_ProcessDirKey
    ; pop cx
    mov ax, 1
    ; TODO(JimBalcomb): implement GUI_ProcessDirKey()
    mov [bp+Control_Index], ax
    jmp short loc_26AA5

loc_26A80:
    mov ax, 52h
    jmp short loc_26A74

loc_26A85:
    mov ax, 55h
    jmp short loc_26A74

loc_26A8A:
    mov ax, 44h
    jmp short loc_26A74

loc_26A8F:
    mov ax, 57h
    jmp short loc_26A74

loc_26A94:
    mov ax, 58h
    jmp short loc_26A74

loc_26A99:
    mov ax, 59h
    jmp short loc_26A74

loc_26A9E:
    mov ax, 5Ah
    jmp short loc_26A74

loc_26AA3:
    jmp short $+2

loc_26AA5:
    mov ax, [bp+Control_Index]
    mov [di], ax

loc_26AAA:
    cmp [bp+Reset_String_Triggers], 0
    jz short loc_26B1E
    mov [bp+Loop_Var], 0
    jmp short loc_26B15
    loc_26AB7:
    mov ax, [bp+Loop_Var]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_AltString
    jnz short loc_26B12
    mov ax, [bp+Loop_Var]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    push [es:bx+GUI_CTRL.Param0]
    lea ax, [bp+Local_String]
    push ax
    call strcpy
    pop cx
    pop cx
    mov ax, [bp+Loop_Var]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov [es:bx+GUI_CTRL.Param1], 0
    mov ax, [bp+Loop_Var]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov al, [bp+Local_String]
    mov [es:bx+GUI_CTRL.Hotkey], al

loc_26B12:
    inc [bp+Loop_Var]

loc_26B15:
    mov ax, [bp+Loop_Var]
    cmp ax, [g_GUI_Control_Count]
    jl short loc_26AB7

loc_26B1E:
    mov ax, [di]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov al, [es:bx+GUI_CTRL.Hotkey]
    cmp al, [bp+Capitalized_Key]
    jz short loc_26B3A
    mov al, [bp+Original_Key]
    mov [bp+Capitalized_Key], al

loc_26B3A:
    mov al, [bp+Capitalized_Key]
    
    cbw
    
    jmp short $+2
    
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp GUI_NoTriggerKeys

db 0
word_26B47 dw KP_Left,KP_Right, KP_Up,KP_Down
dw KP_PgUp,KP_Home,KP_End,KP_PgDn
off_26B57 dw offset loc_26A71
dw offset loc_26A80
dw offset loc_26A85
dw offset loc_26A8A
dw offset loc_26A94
dw offset loc_26A8F
dw offset loc_26A99
dw offset loc_26A9E

;ends seg034

end
