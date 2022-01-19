TITLE _s34p83.asm GUI_NormalRedraw

.MODEL LARGE, C

.CODE
;segment seg034 byte public 'CODE' use16
; 	 assume cs:seg034
; 	 ;org 0Eh
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_NormalRedraw

proc GUI_NormalRedraw far
push bp
mov bp, sp
cmp [GUI_RedrawFn_Present], 0
jz short loc_2E2E5
call CLK_SaveCounter
call VGA_SetDrawFrame
push cs
call near ptr GUI_CallRedrawFn
call VGA_DAC_Write
nop
push cs
call near ptr GUI_SimplePageFlip
push [GUI_Redraw_Timer]
call CLK_Wait
pop cx
jmp short loc_2E2EF
loc_2E2E5:
call VGA_DAC_Write
nop
push cs
call near ptr GUI_SimplePageFlip
loc_2E2EF:
pop bp
retf
endp GUI_NormalRedraw

;ends seg034

end
