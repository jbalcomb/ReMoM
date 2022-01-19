; _s18p06.asm VGA_DrawGlyph_NoAA
; ST_TXT.H

.MODEL LARGE, C

.CODE
;segment seg018 byte public 'CODE' use16
; 	 assume cs:seg018
; 	 ;org 0Ch
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_DrawGlyph_NoAA

; Font_ColorIndex_2 dw 0
; Font_ColorIndex_3 dw 0
; Current_Font_Index dw 0
; VGA_GlyphDraw_X dw 0
; VGA_GlyphDraw_IMGHgt dw 0
; VGA_GlyphDraw_Next_X dw 0
; VGA_GlyphDraw_Offset dw 0
VGA_GlyphDraw_Cols dw 0
; VGA_GlyphDraw_VTrim dw 0
; VGA_GlyphDraw_FHgt dw 0
; VGA_GlyphDraw_DHgt dw 0
; VGA_GlyphDraw_Top dw 0
; VGA_GlyphDraw_Buffer db 50h dup(0)

proc VGA_DrawGlyph_NoAA

    mov [cs:VGA_GlyphDraw_Cols], dx
    mov dx, 3C4h
    mov al, 2
    out dx, al

    mov di, bx
    shr di, 1
    shr di, 1

    mov ah, 1

    and bx, 3
    jz short loc_1AEDD
    mov cx, bx

loc_1AED9:
    shl ah, 1
    loop loc_1AED9

loc_1AEDD:
    mov dx, 3C5h
    mov al, ah
    out dx, al

    mov dx, di

loc_1AEE5:
    lodsb
    cmp al, 0
    js short loc_1AF0E
    mov cl, al
    xor ch, ch
    shr cx, 1
    shr cx, 1
    shr cx, 1
    shr cx, 1
    mov bl, al
    and bx, 0Fh
    jz short loc_1AF07
    mov al, [bx]

loc_1AEFF:
    stosb
    add di, 4Fh
    loop loc_1AEFF
    jmp short loc_1AEE5

loc_1AF07:
    add di, 50h
    loop loc_1AEFF
    jmp short loc_1AEE5

loc_1AF0E:
    and al, 7Fh
    jz short loc_1AF2A
    mov bl, al
    xor bh, bh
    mov cx, bx
    shl bx, 1
    shl bx, 1
    add bx, cx
    shl bx, 1
    shl bx, 1
    shl bx, 1
    shl bx, 1
    add di, bx
    jmp short loc_1AEE5

loc_1AF2A:
    mov di, dx
    shl ah, 1
    cmp ah, 9
    js short loc_1AF36
    mov ah, 1
    inc di

loc_1AF36:
    mov dx, 3C5h
    mov al, ah
    out dx, al

    mov dx, di
    dec [cs:VGA_GlyphDraw_Cols]
    jnz short loc_1AEE5

    ret

endp VGA_DrawGlyph_NoAA

;ends seg018

end
