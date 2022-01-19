TITLE _s33p40.asm VGA_DrawCursor_DSP

.MODEL LARGE, C

EXTRN gsa_Palette_Cursor_Data:WORD
EXTRN g_RenderScreenPage:WORD

.CODE
;segment seg033 byte public 'CODE' use16
; assume cs:seg033
; ;org 6
; assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_DrawCursor_DSP

proc VGA_DrawCursor_DSP

    Draw_Height = word ptr -4
    Cursor_Image_Segment = word ptr -2

    push bp
    mov  bp, sp
    sub  sp, 4
    push es
    push ds
    push si
    push di

    ;mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax
    ;assume ds:dseg
    assume ds:DGROUP

    cmp dx, 0
    jnz short loc_24BAD

    pop di
    pop si
    pop ds
    pop es
    mov sp, bp
    pop bp
    ret

loc_24BAD:
    dec dx
    shl dx, 1
    shl dx, 1
    shl dx, 1
    shl dx, 1
    add dx, [gsa_Palette_Cursor_Data]
    mov [bp+Cursor_Image_Segment], dx

    mov dx, cx
    shl dx, 1
    shl dx, 1
    add dx, cx
    mov ax, 1
    sub ax, [g_RenderScreenPage]
    mov ah, al
    xor al, al
    shl ax, 1
    shl ax, 1
    add ax, 0A000h
    add ax, dx
    mov es, ax
    mov ax, 0C8h
    sub ax, cx
    jbe short @@Done

    cmp ax, 11h
    js short loc_24BEA

    mov ax, 10h

loc_24BEA:
    mov [bp+Draw_Height], ax
    mov ax, bx
    mov ah, 1
    and al, 3
    jz short loc_24BFD

    mov cl, al
    xor ch, ch

loc_24BF9:
    shl ah, 1
    loop loc_24BF9

loc_24BFD:
    mov dx, 3C4h
    mov al, 2
    out dx, al
    mov dx, bx
    shr dx, 1
    shr dx, 1
    mov cx, 140h
    sub cx, bx
    jbe short @@Done

    cmp cx, 11h
    js short loc_24C18

    mov cx, 10h

loc_24C18:
    mov bl, cl
    ;mov bh, [byte ptr bp+Draw_Height]
    mov bh, byte ptr [bp+Draw_Height]
    mov si, 0
    mov cx, [bp+Cursor_Image_Segment]
    mov ds, cx

loc_24C25:
    mov di, dx
    mov dx, 3C5h
    mov al, ah
    out dx, al
    mov dx, di
    mov cl, bh
    xor ch, ch

loc_24C33:
    lodsb
    cmp al, 0
    jnz short loc_24C40
    add di, 50h
    loop loc_24C33
    jmp short loc_24C46
    ;align 2
loc_24C40:
    stosb
    add di, 4Fh
    loop loc_24C33

loc_24C46:
    mov cl, 10h
    sub cl, bh
    xor ch, ch
    add si, cx
    dec bl
    jz short @@Done

    shl ah, 1
    cmp ah, 9
    js short loc_24C5C

    inc dx
    mov ah, 1

loc_24C5C:
    jmp short loc_24C25

@@Done:
    pop di
    pop si
    pop ds
    pop es
    mov sp, bp
    pop bp
    ret

endp VGA_DrawCursor_DSP

;ends seg033

end
