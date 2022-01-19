TITLE _s26p05.asm VGA_Copy_RSP_DSP
; ST_VGA.H

.MODEL LARGE, C

EXTRN GUI_GetCursorIndex:PROC
EXTRN VGA_Copy_RSP_DSP_YM:PROC
EXTRN VGA_Copy_RSP_DSP_NM:PROC

.CODE
;segment seg026 byte public 'CODE' use16
;    assume cs:seg026
;    ;org 0Ah
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_Copy_RSP_DSP

proc VGA_Copy_RSP_DSP

    push ds
    push es
    push si
    push di

    call GUI_GetCursorIndex

    cmp ax, 0
    jz short @@NoCursor

    ;push cs
    ;call near ptr GUI_CopyDispToDraw
    ;nop
    call VGA_Copy_RSP_DSP_YM

    pop di
    pop si
    pop es
    pop ds
    ret

@@NoCursor:
    ;push cs
    ;call near ptr VGA_CopyDispToDraw
    ;nop
    call VGA_Copy_RSP_DSP_NM

    pop di
    pop si
    pop es
    pop ds
    ret

endp VGA_Copy_RSP_DSP

;ends seg026

end
