TITLE _s34p26.asm GUI_MousedControl()
; ST_GUI.H

.MODEL LARGE, C

EXTRN gfp_GUI_Control_Table:DWORD
EXTRN g_GUI_CursorOffset:WORD
EXTRN g_GUI_FocusedControl:WORD

EXTRN GUI_FindWindow:PROC
EXTRN GUI_GetCursorOffset:PROC
EXTRN GUI_MouseOverControl:PROC
EXTRN MOUSE_GetX:PROC
EXTRN MOUSE_GetY:PROC

.CODE
;segment seg034 byte public 'CODE' use16
; 	 assume cs:seg034
; 	 ;org 0Eh
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_MousedControl

;struc GUI_CTRL ; (sizeof=0x26)
GUI_CTRL struc
Left        dw ?
Top         dw ?
Right       dw ?
Bottom      dw ?
Ctrl_Type   dw ?
UU_Help     dw ?
Font_Index  dw ?
ColorSet1   dw ?
Rect_FX     dw ?
Selectable  dw ?
ColorSet2   dw ?
Param0      dw ?                             ; Text@                           (6:Min_Val, 9:Parent)
Param1      dw ?                             ; Trig_State     (6:Max_Val, 9/A:Index, B:IMG, C:CellW)
Param2      dw ?                             ; State@        (4/B:BG_Color, 6:StSeg, 8:Len, C:CellH)
Param3      dw ?                             ; FullFrames    (4:Align, 6:MinSlide, A:CBlock, C:Col@)
Param4      dw ?                             ; MaxSlide                           (4:CStyle, C:Row@)
Param5      dw ?                             ; IMG_Seg                        (4/B:MaxLen, 6:Orient)
Param6      dw ?                             ; CorsorColors@                    (6:StOff, B:Outline)
Hotkey      db ?
Unused_25h  db ?
;ends GUI_CTRL
GUI_CTRL ends

; enum GUI_Controls
Ctrl_ClickButton     EQU 00h
Ctrl_ToggleButton    EQU 01h
Ctrl_LockableButton  EQU 02h
Ctrl_MStateButton    EQU 03h
Ctrl_EditBox         EQU 04h
Ctrl_ImageLabel      EQU 05h
Ctrl_Slidebar        EQU 06h
Ctrl_Label           EQU 07h
Ctrl_AltString       EQU 08h
Ctrl_ClickLink       EQU 09h
Ctrl_DialogLine      EQU 0Ah
Ctrl_EditSelect      EQU 0Bh
Ctrl_ClickGrid       EQU 0Ch

proc GUI_MousedControl

    Y_Pos = word ptr -4
    X_Pos = word ptr -2

    push bp
    mov  bp, sp
    sub  sp, 4
    push si

    call MOUSE_GetX
    mov [bp+X_Pos], ax
    call MOUSE_GetY
    mov [bp+Y_Pos], ax

    xor si, si
    push [bp+Y_Pos]
    push [bp+X_Pos]
    call GUI_FindWindow
    pop cx
    pop cx

    call GUI_GetCursorOffset
    mov [g_GUI_CursorOffset], ax
    ;push cs
    ;call near ptr GUI_MouseOverControl
    call GUI_MouseOverControl
    mov si, ax
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_ClickLink
    jnz short @@CheckDialogLine

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
    mov bx, [es:bx+GUI_CTRL.Param2]
    pop ax
    mov [bx], ax
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov ax, [es:bx+GUI_CTRL.Param0]

@@JmpDone:
    jmp @@Done

@@CheckDialogLine:
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_DialogLine
    jnz short @@Check_ClickGrid

    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Selectable], 0
    jnz short @@Check_ClickGrid

@@Return_FAILURE:
    xor ax, ax
    jmp short @@JmpDone

@@Check_ClickGrid:
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_ClickGrid
    jz short loc_27BC3
    jmp @@Return_SI

loc_27BC3:
    ;db 83h,3Eh,22h,4Dh,0FFh ; <BAD>cmp [g_GUI_FocusedControl], 0FFFFh
    cmp [g_GUI_FocusedControl], 0FFFFh
    jz short loc_27BE2
    mov ax, [g_GUI_FocusedControl]
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    cmp [es:bx+GUI_CTRL.Ctrl_Type], Ctrl_ClickGrid
    jnz short loc_27BE2
    jmp @@Return_SI

loc_27BE2:
    call MOUSE_GetX
    push ax
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    pop ax
    sub ax, [es:bx+GUI_CTRL.Left]
    push ax
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    pop ax
    cwd
    idiv [es:bx+GUI_CTRL.Param1]
    push ax
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param3]
    pop ax
    mov [bx], ax
    call MOUSE_GetY
    push ax
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    pop ax
    sub ax, [es:bx+GUI_CTRL.Top]
    push ax
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    pop ax
    cwd
    idiv [es:bx+GUI_CTRL.Param2]
    push ax
    mov ax, si
    mov dx, 26h
    imul dx
    les bx, [gfp_GUI_Control_Table]
    add bx, ax
    mov bx, [es:bx+GUI_CTRL.Param4]
    pop ax
    mov [bx], ax

@@Return_SI:
    mov ax, si
    jmp @@JmpDone

@@Done:
    pop si
    mov sp, bp
    pop bp
    ret

endp GUI_MousedControl

;ends seg034

end
