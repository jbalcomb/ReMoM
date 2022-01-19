TITLE _s33p38.asm GUI_DrawCursor_DSP

.MODEL LARGE, C

EXTRN g_GUI_CurrentCursor:WORD

EXTRN VGA_DrawCursor_DSP:PROC

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_DrawCursor_DSP

proc GUI_DrawCursor_DSP

    X_Pos = word ptr 6
    Y_Pos = word ptr 8

    push bp
    mov  bp, sp
    push si
    push di
    push es
    push ds

    ;mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax
    ;assume ds:dseg
    assume ds:DGROUP

    mov dx, [g_GUI_CurrentCursor]
    cmp dx, 0
    jz short @@Done

    mov bx, [bp+X_Pos]
    mov cx, [bp+Y_Pos]
    push cs
    call near ptr VGA_DrawCursor_DSP
    nop

@@Done:
    pop ds
    pop es
    pop di
    pop si
    pop bp
    ret

endp GUI_DrawCursor_DSP
;ends seg033

end
