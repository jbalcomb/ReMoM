TITLE _s34p08.asm GUI_SetEscOverride

.MODEL LARGE, C

EXTRN g_GUI_EscapeOverride:WORD

.CODE
;segment seg034 byte public 'CODE' use16
;    assume cs:seg034
;    ;org 0Eh
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_SetEscOverride

proc GUI_SetEscOverride far

    push bp
    mov  bp, sp

    mov [g_GUI_EscapeOverride], 1

    pop bp
    retf

endp GUI_SetEscOverride

;ends seg034

end
