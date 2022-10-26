TITLE _s34p79.asm GUI_DisableRedraw

.MODEL LARGE, C

.CODE
;segment seg034 byte public 'CODE' use16
; 	 assume cs:seg034
; 	 ;org 0Eh
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_DisableRedraw

proc GUI_DisableRedraw far
push bp
mov bp, sp
mov [GUI_RedrawFn_Present], 0
pop bp
retf
endp GUI_DisableRedraw

;ends seg034

end
