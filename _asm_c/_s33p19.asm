TITLE _s33p19.asm MOUSE_GetY

.MODEL LARGE, C

EXTRN g_MOUSE_CurrentY:WORD

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MOUSE_GetY

proc MOUSE_GetY

    push ds

;    mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax

    mov ax, [g_MOUSE_CurrentY]

    pop ds
    ret

endp MOUSE_GetY

;ends seg033

end
