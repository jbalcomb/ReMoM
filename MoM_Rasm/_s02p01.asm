; _s02p01.asm CLK_SaveCounter
; in MOM_DEF.H

.MODEL LARGE, C

EXTRN g_TIMER_Count_Lo:WORD
EXTRN g_TIMER_Count_Hi:WORD

.CODE
;segment seg002 byte public 'CODE' use16
;    assume cs:seg002
;    ;org 2
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC CLK_SaveCounter

proc CLK_SaveCounter

    push es

    mov ax, 0
    mov es, ax
    assume es:nothing

    mov ax, [es:46Ch]
    mov [word ptr g_TIMER_Count_Lo], ax   ; Int 0's/ticks/milli/microsends
    mov ax, [es:46Eh]
    mov [word ptr g_TIMER_Count_Hi], ax ; hour counter

    pop es
    assume es:nothing
    ret

endp CLK_SaveCounter

;ends seg002

end
