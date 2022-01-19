TITLE _s33p.asm GUI_GetCursorOffset

.MODEL LARGE, C

.CODE
;segment seg033 byte public 'CODE' use16
; assume cs:seg033
; ;org 6
; assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_GetCursorOffset

proc GUI_GetCursorOffset far
push ds
mov ax, seg dseg
mov ds, ax
mov ax, [GUI_CursorOffset]
pop ds
retf

endp GUI_GetCursorOffset

;ends seg033

end
