TITLE _s34p13.asm GUI_ClearHelp

.MODEL LARGE, C

.CODE
;segment seg034 byte public 'CODE' use16
;    assume cs:seg034
;    ;org 0Eh
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_ClearHelp

proc GUI_ClearHelp far
push bp
mov bp, sp
mov [GUI_Help_Available], 0
mov [HLP_Entry_Count], 0
mov [HLP_Entry_Table@], 0
pop bp
retf
endp GUI_ClearHelp


;ends seg034

end
