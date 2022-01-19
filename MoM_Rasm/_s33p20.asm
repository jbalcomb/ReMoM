TITLE _s33p20.asm MOUSE_MoveCursor
; ST_GUI.H

.MODEL LARGE, C

EXTRN g_MouseDriverInstalled:WORD
EXTRN g_MOUSE_CurrentX:WORD
EXTRN g_MOUSE_CurrentY:WORD

EXTRN MOUSE_MoveCursorINT:PROC

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MOUSE_MoveCursor

proc MOUSE_MoveCursor

    X_Pos = word ptr 6
    Y_Pos = word ptr 8

    push bp
    mov  bp, sp
    push ds

    ;mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax

    mov ax, [bp+X_Pos]
    mov [g_MOUSE_CurrentX], ax

    mov ax, [bp+Y_Pos]
    mov [g_MOUSE_CurrentY], ax

    cmp [g_MouseDriverInstalled], 0
    jz short @@Done

    push ax
    push [bp+X_Pos]
    ;push cs
    ;call near ptr MOUSE_MoveCursorINT
    ;nop
    call MOUSE_MoveCursorINT
    add sp, 4

@@Done:
    pop ds
    pop bp
    ret

endp MOUSE_MoveCursor



;ends seg033

end
