TITLE _s33p25.asm MOUSE_GetClick

.MODEL LARGE, C

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MOUSE_GetClick

proc MOUSE_GetClick

    push ds

    mov ax, seg dseg
    mov ds, ax

    mov ax, [MOUSE_ClickRec1]
    sub bx, bx
    mov [MOUSE_ClickRec1], bx
    
    pop ds
    ret

endp MOUSE_GetClick


;ends seg033

end
