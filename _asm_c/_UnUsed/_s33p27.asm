TITLE _s33p.asm 

.MODEL LARGE, C

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC 

proc CRP_MOUSE_SetSecClick far

X_Pos= word ptr 6
Y_Pos= word ptr 8

push bp
mov bp, sp
push ds
mov ax, seg dseg
mov ds, ax
mov [MOUSE_ClickRec2], 1
mov ax, [bp+X_Pos]
mov [MOUSE_ClickX], ax
mov ax, [bp+Y_Pos]
mov [MOUSE_ClickY], ax
pop ds
pop bp
retf

endp CRP_MOUSE_SetSecClick


;ends seg033

end
