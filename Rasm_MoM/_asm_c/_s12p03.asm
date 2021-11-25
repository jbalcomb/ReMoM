TITLE _s12p03.asm EMM_GetFreePageCnt

.MODEL LARGE, C

segment seg012 byte public 'CODE' use16
    assume cs:seg012
    ;org 6
    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC C EMS_GetFreePageCnt

proc EMS_GetFreePageCnt far

    push es
    push ds
    push si
    push di
    
    mov ah, 42h
    int 67h
    cmp ah, 0
    jnz short loc_16200

    mov ax, bx

    pop di
    pop si
    pop ds
    pop es
    ret

loc_16200:
    mov ax, 0

    pop di
    pop si
    pop ds
    pop es
    ret

endp EMS_GetFreePageCnt

ends seg012

end
