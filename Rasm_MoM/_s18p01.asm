; _s18p01.asm VGA_SetFont

.MODEL LARGE, C

struc FONT_HEADER ; (sizeof=0x16A, standard type)
Current_Colors db 16 dup(?)
Font_Height dw ?
Outline_Style db ?
Color_Index db ?
Colors_1 db 16 dup(?)
Colors_2 db 16 dup(?)
Colors_3 db 16 dup(?)
Line_Height dw ?
Vert_Spacing dw ?
Horz_Spacing dw ?
Glyph_Widths db 96 dup(?)
Glyph_Offsets dw 96 dup(?)
ends FONT_HEADER

struc FONT_DATA ; (sizeof=0x4574)
Hdr_Space FONT_HEADER ?
Font_Heights dw 8 dup(?)
Horz_Spacings dw 8 dup(?)
Vert_Spacings dw 8 dup(?)
Glyph_Widths db 768 dup(?)
Glyph_Offsets dw 768 dup(?)
Glyph_Data db 15066 dup(?)
ends FONT_DATA

.CODE
;segment seg018 byte public 'CODE' use16
;    assume cs:seg018
;    ;org 0Ch
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_SetFont

proc VGA_SetFont far

    Font_Color_3 = word ptr -0Ah
    Font_Color_2 = word ptr -8
    Font_Color_1 = word ptr -6
    Font_Index   = word ptr 6
    Color_1      = word ptr 8
    Color_2      = word ptr 0Ah
    Color_3      = word ptr 0Ch

    push bp
    mov  bp, sp
    sub  sp, 0Ah
    push es
    push ds
    push si
    push di

    mov ax, [bp+Color_1]
    cmp ax, 10h
    jb short loc_1AB79
    mov ax, 0
loc_1AB79:
    mov [bp+Font_Color_1], ax
    mov ax, [bp+Color_2]
    cmp ax, 10h
    jb short loc_1AB87
    mov ax, 0
loc_1AB87:
    mov [bp+Font_Color_2], ax
    mov ax, [bp+Color_3]
    cmp ax, 10h
    jb short loc_1AB95
    mov ax, 0
loc_1AB95:
    mov [bp+Font_Color_3], ax
    mov ax, [bp+Font_Color_2]
    mov bx, [bp+Font_Color_3]
    mov cx, [bp+Font_Index]
    mov [cs:Font_ColorIndex_2], ax
    mov [cs:Font_ColorIndex_3], bx
    mov [cs:Current_Font_Index], cx
    mov ax, [bp+Font_Index]
    mov [VGA_Font_Index], ax
    mov ax, [bp+Font_Color_1]
    mov [VGA_Font_ColorIndex1], ax
    mov ax, [bp+Font_Color_2]
    mov [VGA_Font_ColorIndex2], ax
    mov ax, [bp+Font_Color_3]
    mov [VGA_Font_ColorIndex3], ax
    mov ax, [Font_Data_Seg@]
    mov es, ax
    mov di, 0
    mov si, [bp+Font_Color_1]
    mov ax, [Palette_Font_Colors@]
    mov ds, ax
    and si, 0Fh
    shl si, 1
    shl si, 1
    shl si, 1
    shl si, 1
    mov dx, si
    mov cx, 8
    rep movsw
    mov di, FONT_HEADER.Colors_1
    mov si, dx
    mov cx, 8
    rep movsw
    mov si, [cs:Font_ColorIndex_2]
    and si, 0Fh
    shl si, 1
    shl si, 1
    shl si, 1
    shl si, 1
    mov cx, 8
    rep movsw
    mov si, [cs:Font_ColorIndex_3]
    and si, 0Fh
    shl si, 1
    shl si, 1
    shl si, 1
    shl si, 1
    mov cx, 8
    rep movsw
    mov ax, es
    mov ds, ax
    mov di, FONT_HEADER.Font_Height
    mov si, [cs:Current_Font_Index]
    shl si, 1
    add si, FONT_ENTRY.Font_Heights
    movsw
    mov di, FONT_HEADER.Vert_Spacing
    mov si, [cs:Current_Font_Index]
    shl si, 1
    add si, FONT_ENTRY.Vert_Spacings
    lodsw
    stosw
    mov di, FONT_HEADER.Line_Height
    add ax, [ds:FONT_HEADER.Font_Height]
    stosw
    mov di, FONT_HEADER.Horz_Spacing
    mov si, [cs:Current_Font_Index]
    shl si, 1
    add si, FONT_ENTRY.Horz_Spacings
    movsw
    mov di, FONT_HEADER.Outline_Style
    mov ax, 0
    stosw
    mov di, FONT_HEADER.Glyph_Widths
    mov si, [cs:Current_Font_Index]
    mov ax, si
    shl si, 1
    add si, ax
    shl si, 1
    shl si, 1
    shl si, 1
    shl si, 1
    shl si, 1
    add si, FONT_ENTRY.Glyph_Widths
    mov cx, 30h
    rep movsw
    mov di, FONT_HEADER.Glyph_Offsets
    mov si, [cs:Current_Font_Index]
    mov ax, si
    shl si, 1
    add si, ax
    shl si, 1
    shl si, 1
    shl si, 1
    shl si, 1
    shl si, 1
    shl si, 1
    add si, FONT_ENTRY.Glyph_Offsets
    mov cx, 60h
    rep movsw

    pop di
    pop si
    pop ds
    pop es
    mov sp, bp
    pop bp
    ret

endp VGA_SetFont

;ends seg018

end
