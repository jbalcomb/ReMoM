TITLE _s33p14.asm MOUSE_CDraw_Restore

.MODEL LARGE, C

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MOUSE_CDraw_Restore

proc MOUSE_CDraw_Restore far
push ds
push bx
mov bx, seg dseg
mov ds, bx
mov bx, [MOUSE_CDraw_Save]
mov [MOUSE_CursorDraw], bx
pop bx
pop ds
retf

endp MOUSE_CDraw_Restore

;ends seg033

end
