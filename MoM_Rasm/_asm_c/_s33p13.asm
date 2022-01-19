TITLE _s33p13.asm MOUSE_Disable_CDraw

.MODEL LARGE, C

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MOUSE_Disable_CDraw

proc MOUSE_Disable_CDraw far
push ds
push bx
mov bx, seg dseg
mov ds, bx
mov bx, [MOUSE_CursorDraw]
mov [MOUSE_CDraw_Save], bx
sub bx, bx
mov [MOUSE_CursorDraw], bx
pop bx
pop ds
retf

endp MOUSE_Disable_CDraw

;ends seg033

end
