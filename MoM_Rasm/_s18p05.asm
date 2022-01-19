; _s18p05.asm VGA_DrawGlyph
; ST_TXT.H

.MODEL LARGE, C

.CODE
;segment seg018 byte public 'CODE' use16
; 	 assume cs:seg018
; 	 ;org 0Ch
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_DrawGlyph

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

proc VGA_DrawGlyph

    mov [cs:VGA_GlyphDraw_Cols], dx
    mov dx, 3C4h
    mov al, 2
    out dx, al

    mov di, bx
    shr di, 1
    shr di, 1
    mov ah, 1
    and bx, 3
    jz short loc_1AE5F
    mov cx, bx

loc_1AE5B:
    shl ah, 1
    loop loc_1AE5B

loc_1AE5F:
    mov dx, 3C5h
    mov al, ah
    out dx, al

    mov dx, di

loc_1AE67:
    lodsb
    cmp al, 0
    js short loc_1AE87
    mov cl, al
    xor ch, ch
    shr cx, 1
    shr cx, 1
    shr cx, 1
    shr cx, 1
    mov bl, al
    and bx, 0Fh
    mov al, [bx]

loc_1AE7F:
    stosb
    add di, 4Fh
    loop loc_1AE7F
    jmp short loc_1AE67

loc_1AE87:
    and al, 7Fh
    jz short loc_1AEA3
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
    jmp short loc_1AE67

loc_1AEA3:
    mov di, dx
    shl ah, 1
    cmp ah, 9
    js short loc_1AEAF
    mov ah, 1
    inc di

loc_1AEAF:
    mov dx, 3C5h
    mov al, ah
    out dx, al

    mov dx, di
    dec [cs:VGA_GlyphDraw_Cols]
    jnz short loc_1AE67

    ret

endp VGA_DrawGlyph

;ends seg018

end
