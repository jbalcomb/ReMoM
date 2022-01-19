TITLE _s34p82.asm GUI_1TickRedraw

.MODEL LARGE, C

.CODE
;segment seg034 byte public 'CODE' use16
; 	 assume cs:seg034
; 	 ;org 0Eh
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_1TickRedraw

proc GUI_1TickRedraw far
push bp
mov bp, sp
cmp [GUI_RedrawFn_Present], 0
jz short loc_2E2AB
call CLK_SaveCounter
call VGA_SetDrawFrame
push cs
call near ptr GUI_CallRedrawFn
call VGA_DAC_Write
nop
push cs
call near ptr GUI_SimplePageFlip
mov ax, 1
push ax
call CLK_Wait
pop cx
jmp short loc_2E2B5
loc_2E2AB:
call VGA_DAC_Write
nop
push cs
call near ptr GUI_SimplePageFlip
loc_2E2B5:
pop bp
retf
endp GUI_1TickRedraw

;ends seg034

end
