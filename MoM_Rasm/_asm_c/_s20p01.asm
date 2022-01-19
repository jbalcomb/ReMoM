; _s20p01.asm VGA_LoadPalette

.MODEL LARGE, C

.CODE
;segment seg020 byte public 'CODE' use16
;    assume cs:seg020
;    ;org 0Ah
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_LoadPalette

proc VGA_LoadPalette

    First_Index = word ptr -4
    Color_Count = word ptr -2
    Palette_Index = word ptr 6
    First_Color = word ptr 8 ; would be the 1st color if not -1
    Last_Color = word ptr 0Ah ; would be the last color (unused in MoM)

    push bp
    mov  bp, sp
    sub  sp, 4
    push si
    push di

    push [LBX_Palette_Seg@]
    mov ax, [bp+Palette_Index]
    add ax, 2
    push ax
    mov ax, offset Font_FileName
    push ax
    call LBXE_LoadReplace

    add sp, 6
    mov [Palette_Color_Data@], ax
    mov ax, [Palette_Color_Data@]
    add ax, 30h
    mov [Palette_Font_Colors@], ax
    mov ax, [Palette_Font_Colors@]
    add ax, 10h
    mov [Unused_Palette_Data@], ax
    mov ax, [Unused_Palette_Data@]
    add ax, 10h
    mov [Palette_Cursor_Data@], ax
    mov ax, [Palette_Cursor_Data@]
    add ax, 100h
    mov [Shading_Colors@], ax
    ;db 83h,7Eh,8,0FFh ; <BAD>cmp [bp+First_Color], 0FFFFh
    cmp [bp+First_Color], -1
    jnz short loc_1BED2

    mov [bp+First_Index], 0
    mov [bp+Color_Count], 100h
    jmp short loc_1BEE2

loc_1BED2:
    mov ax, [bp+First_Color]
    mov [bp+First_Index], ax
    mov ax, [bp+Last_Color]
    sub ax, [bp+First_Color]
    inc ax
    mov [bp+Color_Count], ax

loc_1BEE2:
    push si
    push di
    push es
    push ds
    mov di, [bp+First_Index]
    mov bx, di
    add bx, 300h
    mov ax, di
    shl di, 1
    add di, ax
    mov si, di
    mov cx, [bp+Color_Count]
    mov ax, [Palette_Color_Data@]
    mov dx, [VGA_DAC_Segment]
    mov ds, ax
    mov es, dx

loc_1BF05:
    lodsb
    cmp al, [es:di]
    jz short loc_1BF0F

    mov [byte ptr es:bx], 1

loc_1BF0F:
    stosb
    lodsb
    cmp al, [es:di]
    jz short loc_1BF1A

    mov [byte ptr es:bx], 1

loc_1BF1A:
    stosb
    lodsb
    cmp al, [es:di]
    jz short loc_1BF25

    mov [byte ptr es:bx], 1

loc_1BF25:
    stosb
    inc bx
    loop loc_1BF05

    pop ds
    pop es
    pop di
    pop si

    xor ax, ax
    push ax
    xor ax, ax
    push ax
    call VGA_SetFont

    pop cx
    pop cx
    ;db 83h,7Eh,8,0FFh ; <BAD>cmp [bp+First_Color], 0FFFFh
    cmp [bp+First_Color], -1
    jnz short loc_1BF49

    mov ax, 0FFh
    push ax
    xor ax, ax
    push ax
    jmp short loc_1BF4F

loc_1BF49:
    push [bp+Last_Color]
    push [bp+First_Color]

loc_1BF4F:
    nop
    push cs
    call near ptr VGA_SetDACChanged

    pop cx
    pop cx
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp VGA_LoadPalette

;ends seg020

end
