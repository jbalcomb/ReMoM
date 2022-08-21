TITLE _s33p05.asm MD_GetButtonStatus
; ST_GUI.H

.MODEL LARGE, C

EXTRN g_MouseDriverInstalled:WORD

.CODE
;segment seg033 byte public 'CODE' use16
; assume cs:seg033
; ;org 6
; assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MD_GetButtonStatus

proc MD_GetButtonStatus

    push es
    push ds

    ;mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax

    cmp [g_MouseDriverInstalled], 0
    jz short @@Done

    mov ax, 03h
    mov bx, 0
    mov cx, 0
    mov dx, 0
    cli
    int 33h                                 ; INT 33,3 - Get Mouse Position and Button Status
    sti
    and bx, 3h                              ; mask 00000011 - right button and left button (1 = pressed)
    mov ax, bx

@@Done:
    pop ds
    pop es
    ret

endp MD_GetButtonStatus

;ends seg033

end
