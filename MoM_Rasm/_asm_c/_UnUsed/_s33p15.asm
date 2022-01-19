TITLE _s33p.asm 

.MODEL LARGE, C

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC 

proc UU_MOUSE_Usable far
push ds
mov ax, seg dseg
mov ds, ax
mov ax, [MOUSE_DriverPresent]
cmp ax, 0
jz short loc_242A8

pop ds
retf

loc_242A8:
mov ax, [MOUSE_Usable]
pop ds
retf

endp UU_MOUSE_Usable

;ends seg033

end
