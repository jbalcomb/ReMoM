TITLE _s33p.asm 

.MODEL LARGE, C

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC 

proc MOUSE_GetClickedBtns far
push ds
mov ax, seg dseg
mov ds, ax
mov ax, [MOUSE_ClickBtns]
pop ds
retf

endp MOUSE_GetClickedBtns



;ends seg033

end
