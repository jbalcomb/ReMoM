TITLE _s34p.asm 

.MODEL LARGE, C

.CODE
;segment seg034 byte public 'CODE' use16
; 	 assume cs:seg034
; 	 ;org 0Eh
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC

proc UU_GUI_ExtClear far
push bp
mov bp, sp
push cs
call near ptr GUI_Clear
mov ax, 1
push ax
push cs
call near ptr GUI_SetDelay
pop cx
pop bp
retf
endp UU_GUI_ExtClear

;ends seg034

end
