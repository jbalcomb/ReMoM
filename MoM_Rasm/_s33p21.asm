TITLE _s33p21.asm MD_MoveCursorINT
; ST_GUI.H

.MODEL LARGE, C

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MD_MoveCursorINT

proc MD_MoveCursorINT

    X_Pos = word ptr 6
    Y_Pos = word ptr 8

    push bp
    mov  bp, sp
    push ds

    mov cx, [bp+X_Pos]
    shl cx, 1
    mov dx, [bp+Y_Pos]
    mov ax, 4
    cli
    int 33h
    sti

    pop ds
    pop bp
    ret

endp MD_MoveCursorINT



;ends seg033

end
