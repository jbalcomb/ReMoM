TITLE _s06p04.asm RAM_GetFreeBlockSize

.MODEL LARGE, C

;unsigned _dos_allocmem(unsigned size, unsigned *segp);
segment seg006 byte public 'CODE' use16
    assume cs:seg006
    ;org 0Eh
    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC RAM_GetFreeBlockSize

proc RAM_GetFreeBlockSize far

    push es
    push ds
    push si
    push di

    mov ah, 48h
    mov bx, 0FFFFh                      ; size in 'paragraphs' (16B)
    int 21h

    mov ax, bx                          ; maximum block size available
    shr ax, 1                           ; AX = AX / 2^1
    shr ax, 1                           ; AX = AX / 2^2
    shr ax, 1                           ; AX = AX / 2^3
    shr ax, 1                           ; AX = AX / 2^4
    shr ax, 1                           ; AX = AX / 2^5
    shr ax, 1                           ; AX = AX / 2^6 (64)

    pop di
    pop si
    pop ds
    pop es
    ret

endp RAM_GetFreeBlockSize

ends seg006

end
