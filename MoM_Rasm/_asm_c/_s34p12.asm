TITLE _s34p12.asm GUI_SetHelp

.MODEL LARGE, C

.CODE
;segment seg034 byte public 'CODE' use16
;    assume cs:seg034
;    ;org 0Eh
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_SetHelp

proc GUI_SetHelp far
Entry_Tbl@= word ptr 6
Entry_Count= word ptr 8
push bp
mov bp, sp
mov ax, [bp+Entry_Tbl@]
mov [HLP_Entry_Table@], ax
mov [GUI_Help_Available], 1
mov ax, [bp+Entry_Count]
mov [HLP_Entry_Count], ax
pop bp
retf
endp GUI_SetHelp

;ends seg034

end
