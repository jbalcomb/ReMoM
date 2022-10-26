TITLE _s33p23.asm MOUSE_GetClickY

.MODEL LARGE, C

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MOUSE_GetClickY

proc MOUSE_GetClickY far
push ds
mov ax, seg dseg
mov ds, ax
mov ax, [MOUSE_ClickY]
pop ds
retf

endp MOUSE_GetClickY


;ends seg033

end
