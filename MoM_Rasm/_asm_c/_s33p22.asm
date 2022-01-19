TITLE _s33p22.asm MOUSE_GetClickX

.MODEL LARGE, C

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MOUSE_GetClickX

proc MOUSE_GetClickX far
push ds
mov ax, seg dseg
mov ds, ax
mov ax, [MOUSE_ClickX]
pop ds
retf

endp MOUSE_GetClickX


;ends seg033

end
