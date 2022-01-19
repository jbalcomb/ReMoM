TITLE _s17p39.asm VGA_UseFontColor1
; ST_TXT.H

.MODEL LARGE, C

EXTRN VGA_FontColorSelect:PROC

.CODE
;segment seg017 byte public 'CODE' use16
;	 assume cs:seg017
;	 ;org 0Ah
;	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_UseFontColor1

proc VGA_UseFontColor1

    push bp
    mov  bp, sp

    xor ax, ax
    push ax
    ;nop
    ;push cs
    ;call near ptr VGA_FontColorSelect
    call VGA_FontColorSelect
    pop cx

    pop bp
    ret

endp VGA_UseFontColor1


;ends seg017

end
