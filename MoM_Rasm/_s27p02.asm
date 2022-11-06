TITLE _s27p02.asm FLIC_Draw_R
; ST_FLIC.H

.MODEL LARGE, C

;ScreenPage_X
;ScreenPage_Y

EXTRN EMM_PageFrameBaseAddress:WORD
EXTRN gsa_DSP_Addr:WORD
;VGA_WriteMapMasks3
;EXTRN g_VGA_WriteMapMasks:WORD
EXTRN sa_ReplacementColors:WORD

EXTRN EMM_MapMulti4:PROC

.DATA
VGA_WriteMapMasks3 db 01h,02h,04h,08h

.CODE
;segment seg027 byte public 'CODE' use16
;    assume cs:seg027
;    ;org 0Ch
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing


PUBLIC FLIC_Draw_R


e_GC_INDEX   EQU 03CEh
e_GC_DATA    EQU 03CFh
e_GC_READMAP EQU   04h
e_SC_INDEX   EQU 03C4h
e_SC_DATA    EQU 03C5h
e_SC_MAPMASK EQU   02h

s_FLIC_HDR struc ; (sizeof=0x10)
fh_Width dw ?
fh_Height dw ?
fh_Current_Frame dw ?
fh_Frame_Count dw ?
fh_Loop_Frame dw ?
fh_EMM_Handle_Number db ?
fh_EMM_Logical_Page_Number db ?
fh_EMM_Logical_Page_Offset dw ?
fh_Palette_Data_Offset dw ?
s_FLIC_HDR ends



; draws an LBX image into the current draw segment,
; column by column, decoding its RLE along the way,
; and processing any shaded colors by reading the
; current screen pixel and replacing with it from
; the predefined Replacement_Colors@ block

; int __cdecl __far FLIC_Draw_R(int ScreenPage_X, int ScreenPage_Y, int FlicWidth, int Img_Off, int Img_Seg)
proc FLIC_Draw_R

    ScreenPage_X  = word ptr 6
    ScreenPage_Y  = word ptr 8
    FlicWidth     = word ptr 0Ah
    Img_Off       = word ptr 0Ch
    Img_Seg       = word ptr 0Eh

    push bp
    mov bp, sp
    push si
    push di
    push es
    push ds
    mov ax, [bp+ScreenPage_Y]
    mov bx, ax
    shl ax, 1
    shl ax, 1
    add ax, bx ; * 5 as a segment address = * 80 total bytes which,
    ; since each byte is 4 pixels, equals the draw row
    add ax, [gsa_DSP_Addr]
    mov es, ax
    mov ax, [bp+ScreenPage_X]
    mov bx, ax
    shr bx, 1
    shr bx, 1
    mov di, bx ; X / 4 = pixel offset in video memory
    ; X mod 4 = plane index of the pixel
    and ax, 3
    mov bl, al
    shl bl, 1
    shl bl, 1
    shl bl, 1
    shl bl, 1
    mov si, offset VGA_WriteMapMasks3 ; should use dseg:41d0
    add si, ax
    lodsb
    mov ah, al
    or ah, bl ; low nibble = map mask for the first pixel
    ; high nibble = read map for the same
    mov si, [bp+Img_Off]
    mov cx, [bp+FlicWidth]
    mov bx, [bp+Img_Seg]
    mov ds, bx
    mov bx, cx

loc_1E8F3:
    push di
    mov dx, 3CEh
    mov al, 4
    out dx, al ; EGA: graph 1 and 2 addr reg:
    ; read map select.
    ; Data bits 0-2 select map # for read mode 00.
    inc dx
    mov al, ah
    shr al, 1
    shr al, 1
    shr al, 1
    shr al, 1 ; set the read plane for the pixel column
    out dx, al ; EGA port: graphics controller data register
    mov dx, 3C4h
    mov al, 2
    out dx, al ; EGA: sequencer address reg
    ; map mask: data bits 0-3 enable writes to bit planes 0-3
    inc dx
    mov al, ah
    and al, 0Fh ; set the write plane for the pixel column
    out dx, al ; EGA port: sequencer data register
    lodsb ; byte #1
    cmp al, 0FFh ; empty column (transparent)
    jz short loc_1E958
    mov dh, al
    lodsb ; byte #2
    mov dl, al ; dx = encoded column data length in bytes (15 bits,
    ; the sign bit indicates whether there are repeats)
    cmp dh, 0
    jns short loc_1E924 ; chunk byte #2 (skip count)
    jmp loc_1E9B5

loc_1E924: ; chunk byte #2 (skip count)
    mov cl, [si+1]
    xor ch, ch
    mov al, cl
    shl cx, 1
    shl cx, 1
    add cl, al
    adc ch, 0
    shl cx, 1
    shl cx, 1
    shl cx, 1
    shl cx, 1
    add di, cx
    lodsb ; chunk byte #1 (process count)
    mov cl, al
    xor ch, ch
    inc si
    dec dx
    dec dx
    sub dx, cx

loc_1E948:
    lodsb
    cmp al, 0E8h
    jnb short loc_1E97E
    stosb
    add di, 4Fh ; same plane, one pixel down
    loop loc_1E948
    cmp dx, 1
    jns short loc_1E924 ; chunk byte #2 (skip count)

loc_1E958:
    pop di
    dec bx
    jz short loc_1E978
    mov cl, ah
    and cl, 0F0h ; read plane
    add cl, 10h ; -> next
    and ah, 0Fh ; write plane
    shl ah, 1 ; -> next
    cmp ah, 9 ; wrapped around?
    js short loc_1E973
    mov cl, 0 ; reset the read map,
    mov ah, 1 ; reset the write mask,
    inc di ; and increase the memory offset

loc_1E973:
    or ah, cl
    jmp loc_1E8F3

loc_1E978:
    pop ds
    pop es
    pop di
    pop si
    pop bp
    ret

loc_1E97E:
    sub al, 0E8h
    push si
    push ds
    ;mov si, seg dseg
    mov si, seg DGROUP
    mov ds, si
    mov si, ax
    and si, 0FFh
    shl si, 1
    shl si, 1
    shl si, 1
    shl si, 1
    add si, [sa_ReplacementColors]         ; 180h LBX_Alloc_Space paragraphs
                                            ; each 256 byte (100h) block represents a different entry
                                            ;
                                            ; within a block, every byte contains a color index
                                            ; closest to the combination of the original color
                                            ; at that index, and the color and percent specified
                                            ; from the palette image when the block is filled out
                                            ;
                                            ; FLIC_Draw_R and other _R functions use these to
                                            ; replace bytes read from a source image file
    mov ds, si ; ds = paragraph of the block defined by (al-$E8)
    mov al, [es:di]
    mov si, ax
    and si, 0FFh
    lodsb
    pop ds
    pop si
    stosb
    add di, 4Fh ; same plane, one pixel down
    loop loc_1E948
    cmp dx, 1
    js short loc_1E958
    jmp loc_1E924 ; chunk byte #2 (skip count)

loc_1E9B5:
    and dx, 7FFFh

loc_1E9B9: ; chunk byte #2 (skip count)
    mov cl, [si+1]
    xor ch, ch
    mov al, cl
    shl cx, 1
    shl cx, 1
    add cl, al
    adc ch, 0
    shl cx, 1
    shl cx, 1
    shl cx, 1
    shl cx, 1
    add di, cx
    lodsb ; chunk byte #1 (process count)
    mov cl, al
    xor ch, ch
    inc si
    dec dx
    dec dx
    sub dx, cx

loc_1E9DD:
    lodsb
    cmp al, 0E0h
    jnb short loc_1EA31
    cmp al, 0E8h
    jnb short loc_1E9FA
    stosb
    add di, 4Fh ; same plane, one pixel down
    loop loc_1E9DD
    cmp dx, 1
    jns short loc_1E9B9 ; chunk byte #2 (skip count)
    jmp loc_1E958
    pop ds
    pop es
    pop di
    pop si
    pop bp
    ret

loc_1E9FA:
    sub al, 0E8h
    push si
    push ds
    ;mov si, seg dseg
    mov si, seg DGROUP
    mov ds, si
    mov si, ax
    and si, 0FFh
    shl si, 1
    shl si, 1
    shl si, 1
    shl si, 1
    add si, [sa_ReplacementColors]         ; 180h LBX_Alloc_Space paragraphs
                                            ; each 256 byte (100h) block represents a different entry
                                            ;
                                            ; within a block, every byte contains a color index
                                            ; closest to the combination of the original color
                                            ; at that index, and the color and percent specified
                                            ; from the palette image when the block is filled out
                                            ;
                                            ; FLIC_Draw_R and other _R functions use these to
                                            ; replace bytes read from a source image file
    mov ds, si ; ds = paragraph of the block defined by (al-$E8)
    mov al, [es:di]
    mov si, ax
    and si, 0FFh
    lodsb
    pop ds
    pop si
    stosb
    add di, 4Fh ; same plane, one pixel down
    loop loc_1E9DD
    cmp dx, 1
    jns short loc_1E9B5
    jmp loc_1E958

loc_1EA31:
    and al, 1Fh
    inc al
    push cx
    mov cl, al
    xor ch, ch
    lodsb
    cmp al, 0E8h
    jnb short loc_1EA54

loc_1EA3F:
    stosb
    add di, 4Fh ; same plane, one pixel down
    loop loc_1EA3F
    pop cx
    dec cx
    loop loc_1E9DD
    cmp dx, 1
    jns short loc_1EA51
    jmp loc_1E958

loc_1EA51: ; chunk byte #2 (skip count)
    jmp loc_1E9B9

loc_1EA54:
    sub al, 0E8h
    push si
    push ds
    ;mov si, seg dseg
    mov si, seg DGROUP
    mov ds, si
    mov si, ax
    and si, 0FFh
    shl si, 1
    shl si, 1
    shl si, 1
    shl si, 1
    add si, [sa_ReplacementColors]         ; 180h LBX_Alloc_Space paragraphs
                                            ; each 256 byte (100h) block represents a different entry
                                            ;
                                            ; within a block, every byte contains a color index
                                            ; closest to the combination of the original color
                                            ; at that index, and the color and percent specified
                                            ; from the palette image when the block is filled out
                                            ;
                                            ; FLIC_Draw_R and other _R functions use these to
                                            ; replace bytes read from a source image file
    mov ds, si ; ds = paragraph of the block defined by (al-$E8)

loc_1EA71:
    mov al, [es:di]
    mov si, ax
    and si, 0FFh
    movsb
    add di, 4Fh ; same plane, one pixel down
    loop loc_1EA71
    
    pop ds
    pop si
    pop cx
    dec cx
    dec cx
    jz short loc_1EA8A
    jmp loc_1E9DD

loc_1EA8A:
    cmp dx, 1
    jns short loc_1EA51
    jmp loc_1E958

    pop ds
    pop es
    pop di
    pop si
    pop bp
    ret

endp FLIC_Draw_R



tmpFlicHdrEmmLogicalPageOffset dw 0
tmpFlicHdrEmmHandleNumber dw 0
tmpFlicHdrWidth dw 0



;ends seg027

end
