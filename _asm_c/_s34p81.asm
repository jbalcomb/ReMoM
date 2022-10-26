TITLE _s34p81.asm GUI_CallRedrawFn

.MODEL LARGE, C

.CODE
;segment seg034 byte public 'CODE' use16
; 	 assume cs:seg034
; 	 ;org 0Eh
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_CallRedrawFn

proc GUI_CallRedrawFn far
push bp
mov bp, sp
cmp [GUI_RedrawFn_Present], 0
jz short loc_2E27B
call [GUI_Redraw_Function]
loc_2E27B:
pop bp
retf
endp GUI_CallRedrawFn

;ends seg034

end
