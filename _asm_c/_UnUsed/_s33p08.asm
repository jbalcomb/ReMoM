TITLE _s33p.asm 

.MODEL LARGE, C

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC 

proc CRP_MOUSE_SetUsable far
push ds
mov ax, seg dseg
mov ds, ax
mov [MOUSE_Usable], 1
pop ds
retf

endp CRP_MOUSE_SetUsable

;ends seg033

end
