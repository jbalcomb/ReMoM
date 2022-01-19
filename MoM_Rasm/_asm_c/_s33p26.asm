TITLE _s33p26.asm CRP_MOUSE_GetSecClick

.MODEL LARGE, C

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC CRP_MOUSE_GetSecClick

proc CRP_MOUSE_GetSecClick far
push ds
mov ax, seg dseg
mov ds, ax
mov ax, [MOUSE_ClickRec2]
sub bx, bx
mov [MOUSE_ClickRec2], bx
pop ds
retf

endp CRP_MOUSE_GetSecClick



;ends seg033

end
