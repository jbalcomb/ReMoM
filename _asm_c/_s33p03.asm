TITLE _s33p03.asm GUI_GetCursorIndex

.MODEL LARGE, C

.CODE
;segment seg033 byte public 'CODE' use16
; assume cs:seg033
; ;org 6
; assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_GetCursorIndex

proc GUI_GetCursorIndex far
push ds
mov ax, seg dseg
mov ds, ax
mov ax, [GUI_CurrentCursor]
pop ds
retf

endp GUI_GetCursorIndex

;ends seg033

end
