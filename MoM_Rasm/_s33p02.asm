TITLE _s33p.asm GUI_FindWindow
; ST_GUI.H

.MODEL LARGE, C

EXTRN g_CRSR_Curr:WORD
EXTRN g_CRSR_Offset:WORD
EXTRN g_CRSR_Prev:WORD
EXTRN g_GUI_WindowCount:WORD
EXTRN g_GUI_Windows:WORD

;s_GUI_WINDOW

;struc GUI_WINDOW ; (sizeof=0xC, standard type)
;Cursor_Index dw ?                       ; enum Cursor_Types
;Cursor_Offset dw ?
;Left dw ?
;Top dw ?
;Right dw ?
;Bottom dw ?
;ends GUI_WINDOW

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_FindWindow

proc GUI_FindWindow

    X_Pos = word ptr 6
    Y_Pos = word ptr 8

    push bp
    mov  bp, sp
    push ds
    push es

;    mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax
    mov es, ax
;    assume es:dseg
    assume es:DGROUP

    mov ax, [g_CRSR_Curr]
    mov [g_CRSR_Prev], ax

    mov bx, [g_GUI_Windows]
    mov cx, [g_GUI_WindowCount]
    dec cx                                  ; `:` index = {0, ..., count-1}

    jz short loc_23FCD
    mov dx, cx
    mov ax, 0Ch                             ; sizeof s_GUI_WINDOW
    mul dx
    add bx, ax

loc_23FA8:
;    mov ax, [bx+GUI_WINDOW.Left]
    mov ax, [bx+4]
    cmp [bp+X_Pos], ax
    jb short loc_23FC8
;    mov ax, [bx+GUI_WINDOW.Top]
    mov ax, [bx+6]
    cmp [bp+Y_Pos], ax
    jb short loc_23FC8
;    mov ax, [bx+GUI_WINDOW.Right]
    mov ax, [bx+8]
    cmp ax, [bp+X_Pos]
    jb short loc_23FC8
;    mov ax, [bx+GUI_WINDOW.Bottom]
    mov ax, [bx+0Ah]
    cmp ax, [bp+Y_Pos]
    jnb short loc_23FCD

loc_23FC8:
    sub bx, 0Ch                             ; sizeof s_GUI_WINDOW
    loop loc_23FA8

loc_23FCD:
;    mov ax, [bx+GUI_WINDOW.Cursor_Offset]
    mov ax, [bx+2]
    mov [g_CRSR_Offset], ax
;    mov ax, [bx+GUI_WINDOW.Cursor_Index]
    mov ax, [bx]
    mov [g_CRSR_Curr], ax

    pop es
    assume es:nothing
    pop ds
    pop bp
    ret

endp GUI_FindWindow

;ends seg033

end
