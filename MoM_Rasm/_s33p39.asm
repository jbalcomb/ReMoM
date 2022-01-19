TITLE _s33p39.asm VGA_DrawCursor_RSP

.MODEL LARGE, C

EXTRN gsa_Palette_Cursor_Data:WORD
EXTRN g_RenderScreenPage:WORD

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_DrawCursor_RSP

proc VGA_DrawCursor_RSP

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
    jnz short loc_24ADC

    pop di
    pop si
    pop ds
    pop es
    mov sp, bp
    pop bp
    ret

loc_24ADC:
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

    mov ax, [g_RenderScreenPage]
    mov ah, al
    xor al, al
    shl ax, 1
    shl ax, 1
    add ax, dx
    add ax, 0A000h
    mov es, ax

    mov ax, 0C8h
    sub ax, cx
    jbe short @@Done
    cmp ax, 11h
    js short loc_24B15
    mov ax, 10h

loc_24B15:
    mov [bp+Draw_Height], ax
    mov ax, bx
    mov ah, 1
    and al, 3
    jz short loc_24B28
    mov cl, al
    xor ch, ch

loc_24B24:
    shl ah, 1
    loop loc_24B24

loc_24B28:
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
    js short loc_24B43
    mov cx, 10h

loc_24B43:
    mov bl, cl
    mov bh, byte ptr [bp+Draw_Height]
    mov si, 0
    mov cx, [bp+Cursor_Image_Segment]
    mov ds, cx

loc_24B50:
    mov di, dx
    mov dx, 3C5h
    mov al, ah
    out dx, al
    
    mov dx, di
    mov cl, bh
    xor ch, ch

loc_24B5E:
    lodsb
    cmp al, 0
    jnz short loc_24B6B
    add di, 50h
    loop loc_24B5E

    jmp short loc_24B71
    nop

loc_24B6B:
    stosb
    add di, 4Fh
    loop loc_24B5E

loc_24B71:
    mov cl, 10h
    sub cl, bh
    xor ch, ch
    add si, cx
    dec bl
    jz short @@Done
    
    shl ah, 1
    cmp ah, 9
    js short loc_24B87
    inc dx
    mov ah, 1

loc_24B87:
    jmp short loc_24B50

@@Done:
    pop di
    pop si
    pop ds
    pop es
    mov sp, bp
    pop bp
    ret

endp VGA_DrawCursor_RSP

;ends seg033

end
