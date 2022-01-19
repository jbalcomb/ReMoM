TITLE _s33p18.asm MOUSE_GetX

.MODEL LARGE, C

EXTRN g_MOUSE_CurrentX:WORD

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MOUSE_GetX

proc MOUSE_GetX

    push ds

;    mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax

    mov ax, [g_MOUSE_CurrentX]

    pop ds
    ret

endp MOUSE_GetX

;ends seg033

end
