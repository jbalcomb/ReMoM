; _s21p01.asm VGA_DAC_Write
; ST_VGA.H

.MODEL LARGE, C


EXTRN gsa_Palette:WORD

.CODE
;segment seg021 byte public 'CODE' use16
;    assume cs:seg021
;    ;org 8
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_DAC_Write

proc VGA_DAC_Write far

    push si
    push di
    push es
    push ds

    mov ax, [gsa_Palette]
    mov ds, ax
    
    mov si, 0

    mov bx, 0
    mov di, 300h
    mov cx, 0

loc_1C509:
    cmp cx, 0
    jnz short loc_1C51E

    mov dx, 3DAh ; e_INPUT_STATUS ; 3DAh - Read: Input Status #1 Register

    mov cx, 80h

@@Wait_VBL1:
    in al, dx
    and al, 08h ; 1000b ; Test bit 3 - Verticle Retrace
    jnz @@Wait_VBL1

@@Wait_VBL2:
    in al, dx
    and al, 08h ; 1000b ; Test bit 3 - Verticle Retrace
    jz @@Wait_VBL2

loc_1C51E:
    add si, 3
    cmp byte ptr [di], 0
    jz short loc_1C539

    mov dx, 3C8h ; e_PALETTE_INDEX
    sub si, 3
    cli
    mov al, bl
    out dx, al
    inc dx ; 3C9h ; e_PALETTE_DATA
    lodsb
    out dx, al
    lodsb
    out dx, al
    lodsb
    out dx, al
    dec cx
    sti

loc_1C539:
    inc di
    inc bx
    cmp bh, 0
    jz short loc_1C509

    mov ax, ds
    mov es, ax
    ;assume es:dseg
    mov di, 300h
    mov cx, 80h
    mov ax, 0
    rep stosw

    pop ds
    pop es
    ;assume es:nothing
    pop di
    pop si
    ret

endp VGA_DAC_Write

;ends seg021

end
