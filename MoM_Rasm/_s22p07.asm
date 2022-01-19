TITLE _s22p07.asm RNG_TimerSeed
; in ST_DEF.H

.MODEL LARGE, C

EXTRN g_LFSR_HO_bits:WORD
EXTRN g_LFSR_LO_bits:WORD

.CODE
;segment seg022 byte public 'CODE' use16
;    assume cs:seg022
;    ;org 0Ah
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC RNG_TimerSeed

proc RNG_TimerSeed far

    Clock_Ticks_Hi = word ptr -4
    Clock_Ticks_Lo = word ptr -2
    
    push bp
    mov bp, sp
    sub sp, 4
    
    push es
    
    mov ax, 0
    mov es, ax
    assume es:nothing

    mov ax, [es:46Ch]
    mov [bp+Clock_Ticks_Lo], ax

    mov ax, [es:46Eh]
    mov [bp+Clock_Ticks_Hi], ax

    pop es
    assume es:nothing

    mov ax, [bp+Clock_Ticks_Lo]
    mov [g_LFSR_LO_bits], ax

    mov ax, [bp+Clock_Ticks_Hi]
    mov [g_LFSR_HO_bits], ax
    
    mov sp, bp
    pop bp
    ret

endp RNG_TimerSeed

;ends seg022

end
