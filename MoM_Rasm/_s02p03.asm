; _s02p03.asm CLK_Wait
; in MOM_DEF.H

.MODEL LARGE, C

EXTRN g_TIMER_Count_Lo:WORD

.CODE
;segment seg002 byte public 'CODE' use16
;    assume cs:seg002
;    ;org 2
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC CLK_Wait


proc CLK_Wait

    nticks = word ptr 6

    push bp
    mov  bp, sp
    sub  sp, 2
    push es
    push ds
    push si
    push di

    jmp short @@WaitTicks
    nop
@@EarlyExit:
    pop di
    pop si
    pop ds
    pop es
    mov sp, bp
    pop bp
    ret

@@WaitTicks:
    mov ax, 0
    mov es, ax
    assume es:nothing

    mov ax, [es:46Ch] ; [es:e_DAILY_TIMER_COUNTER_lw]
    sub ax, [word ptr g_TIMER_Count_Lo]
    js short @@EarlyExit

    cmp ax, 4
    jns short @@EarlyExit

    sub ax, [bp+nticks]
    js short @@WaitTicks
    
    pop di
    pop si
    pop ds
    pop es
    assume es:nothing
    mov sp, bp
    pop bp
    ret

endp CLK_Wait

;ends seg002

end
