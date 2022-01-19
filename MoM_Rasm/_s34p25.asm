TITLE _s34p25.asm GUI_MouseOverControl()
; ST_GUI.H

.MODEL LARGE, C

EXTRN gfp_GUI_Control_Table:DWORD
EXTRN g_GUI_Control_Count:WORD
EXTRN g_GUI_CursorOffset:WORD

EXTRN GUI_FindWindow:PROC
EXTRN GUI_GetCursorOffset:PROC
EXTRN MOUSE_GetX:PROC
EXTRN MOUSE_GetY:PROC

.CODE
;segment seg034 byte public 'CODE' use16
; 	 assume cs:seg034
; 	 ;org 0Eh
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_MouseOverControl

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

proc GUI_MouseOverControl

    Control_Bottom  = word ptr -0Ch
    Control_Right   = word ptr -0Ah
    Control_Top     = word ptr -8
    Control_Left    = word ptr -6
    Mouse_At        = word ptr -4
    Y_Pos           = word ptr -2

    push bp
    mov  bp, sp
    sub  sp, 0Ch
    push si
    push di

    call MOUSE_GetX
    mov di, ax
    call MOUSE_GetY
    mov [bp+Y_Pos], ax

    mov [bp+Mouse_At], 0

    push [bp+Y_Pos]
    push di
    call GUI_FindWindow
    pop cx
    pop cx

    call GUI_GetCursorOffset
    mov [g_GUI_CursorOffset], ax

    mov si, 1
    jmp loc_27AE7

loc_27A60:
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
    mov ax, di
    add ax, [g_GUI_CursorOffset]
    cmp ax, [bp+Control_Left]
    jl short loc_27AE2
    mov ax, di
    add ax, [g_GUI_CursorOffset]
    cmp ax, [bp+Control_Right]
    jg short loc_27AE2
    mov ax, [bp+Y_Pos]
    add ax, [g_GUI_CursorOffset]
    cmp ax, [bp+Control_Top]
    jl short loc_27AE2
    mov ax, [bp+Y_Pos]
    add ax, [g_GUI_CursorOffset]
    cmp ax, [bp+Control_Bottom]
    jg short loc_27AE2
    mov [bp+Mouse_At], si
    jmp short loc_27AF0

loc_27AE2:
    mov ax, si
    inc ax
    mov si, ax

loc_27AE7:
    cmp si, [g_GUI_Control_Count]
    jge short loc_27AF0
    jmp loc_27A60

loc_27AF0:
    mov ax, [bp+Mouse_At]

    jmp short $+2

    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp GUI_MouseOverControl

;ends seg034

end
