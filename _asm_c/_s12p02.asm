TITLE _s12p02.asm EMS_GetHandleCount

.MODEL LARGE

segment dseg para public 'DATA' use16
ends dseg

segment seg012 byte public 'CODE' use16
    assume cs:seg012
    ;org 6
    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC C EMS_GetHandleCount

proc EMS_GetHandleCount far

    push es
    push ds
    push si
    push di
    
    mov ah, 4Bh
    int 67h
    cmp ah, 0
    jnz @@GotErrorCode
    mov ax, bx

    pop di
    pop si
    pop ds
    pop es
    ret

@@GotErrorCode:
    mov ax, 0

    pop di
    pop si
    pop ds
    pop es
    ret

endp EMS_GetHandleCount

ends seg012

end
