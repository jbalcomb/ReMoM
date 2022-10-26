TITLE _s34p.asm 

.MODEL LARGE, C

.CODE
;segment seg034 byte public 'CODE' use16
; 	 assume cs:seg034
; 	 ;org 0Eh
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC
proc UU_GUI_SetDlgTitleHelp far
Help@= word ptr 6
push bp
mov bp, sp
mov ax, [bp+Help@]
mov [GUI_EmptyTitleHelp], ax
pop bp
retf
endp UU_GUI_SetDlgTitleHelp

;ends seg034

end
