TITLE _s26p03.asm VGA_IsDefaultDrawScreenPage
; ST_VGA.H

.MODEL LARGE, C

EXTRN g_DrawScreenPage_SgmtAddr:WORD

.CODE
;segment seg026 byte public 'CODE' use16
;    assume cs:seg026
;    ;org 0Ah
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_IsDefaultDrawScreenPage

proc VGA_IsDefaultDrawScreenPage

    mov ax, [g_DrawScreenPage_SgmtAddr]
    cmp ax, 0A000h ; Screen-Page 0 SgmtAddr
    jz short @@Return_FALSE

@@Return_TRUE:
    mov ax, 1 ; ST_TRUE
    ret

@@Return_FALSE:
    sub ax, ax ; ST_FALSE
    ret

endp VGA_IsDefaultDrawScreenPage

;ends seg026

end
