; _s18p02.asm VGA_DrawChar
; ST_TXT.H

.MODEL LARGE, C

EXTRN sa_FontStyleData:WORD
EXTRN gsa_DSP_Addr:WORD

EXTRN VGA_DrawGlyph:PROC

.CODE
;segment seg018 byte public 'CODE' use16
; 	 assume cs:seg018
; 	 ;org 0Ch
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_DrawChar

; Font_ColorIndex_2 dw 0
; Font_ColorIndex_3 dw 0
; Current_Font_Index dw 0
VGA_GlyphDraw_X dw 0
; VGA_GlyphDraw_IMGHgt dw 0
VGA_GlyphDraw_Next_X dw 0
; VGA_GlyphDraw_Offset dw 0
; VGA_GlyphDraw_Cols dw 0
; VGA_GlyphDraw_VTrim dw 0
; VGA_GlyphDraw_FHgt dw 0
; VGA_GlyphDraw_DHgt dw 0
; VGA_GlyphDraw_Top dw 0
; VGA_GlyphDraw_Buffer db 50h dup(0)

proc VGA_DrawChar

    Left  = word ptr 6
    Top   = word ptr 8
    Char  = word ptr 0Ah

    push bp
    mov  bp, sp
    sub  sp, 2
    push es
    push ds
    push si
    push di

    mov ax, [bp+Left]
    mov [cs:VGA_GlyphDraw_X], ax
    mov bx, [bp+Char]
    xor bh, bh
    cmp bx, 20h
    js short loc_1AD12
    cmp bx, 7Eh
    jns short loc_1AD12
    mov ax, [bp+Top]
    mov cx, ax
    shl ax, 1
    shl ax, 1
    add ax, cx
    add ax, [gsa_DSP_Addr]
    mov es, ax

    mov ax, [sa_FontStyleData]
    mov ds, ax
    assume ds:nothing

    sub bx, 20h
    ;mov dl, [bx+FONT_HEADER.Glyph_Widths]
    mov dl, [bx+4Ah]  ; 4Ah 74d  FONT_HEADER.Glyph_Widths
    xor dh, dh
    ;mov ax, [ds:FONT_HEADER.Horz_Spacing]
    mov ax, [ds:48h]  ; 48h 72d  FONT_HEADER.Horz_Spacing
    add ax, dx
    add ax, [cs:VGA_GlyphDraw_X]
    mov [cs:VGA_GlyphDraw_Next_X], ax
    shl bx, 1
    ;mov si, [bx+FONT_HEADER.Glyph_Offsets]
    mov si, [bx+0AAh]  ; AAh 170d  FONT_HEADER.Glyph_Offsets
    mov bx, [cs:VGA_GlyphDraw_X]
    ;push cs
    ;call near ptr VGA_DrawGlyph
    ;nop
    call VGA_DrawGlyph

    mov ax, [cs:VGA_GlyphDraw_Next_X]

    pop di
    pop si
    pop ds
    ;assume ds:dseg
    assume ds:DGROUP
    pop es
    mov sp, bp
    pop bp
    ret

loc_1AD12:
    mov ax, [cs:VGA_GlyphDraw_X]

    pop di
    pop si
    pop ds
    pop es
    mov sp, bp
    pop bp
    ret

endp VGA_DrawChar

;ends seg018

end
