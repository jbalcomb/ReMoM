TITLE _s27.asm FLIC_Draw_...
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

PUBLIC FLIC_Draw
PUBLIC FLIC_Draw_R
PUBLIC FLIC_Draw_EMM
PUBLIC FLIC_EMM_MapNextPages
PUBLIC FLIC_Draw_EMM_R

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
; column by column, decoding its RLE along the way

; int __cdecl __far FLIC_Draw(int ScreenPage_X, int ScreenPage_Y, int FlicWidth, int Img_Off, int Img_Seg)

proc FLIC_Draw

    ScreenPage_X = word ptr 6
    ScreenPage_Y = word ptr 8
    FlicWidth = word ptr 0Ah
    Img_Off = word ptr 0Ch
    Img_Seg = word ptr 0Eh

    push bp
    mov  bp, sp
    push si
    push di
    push es
    push ds

    mov dx, 3C4h
    mov al, 2
    out dx, al                              ; EGA: sequencer address reg
                                            ; map mask: data bits 0-3 enable writes to bit planes 0-3
    mov ax, [bp+ScreenPage_Y]
    mov bx, ax
    shl ax, 1
    shl ax, 1
    add ax, bx                              ; * 5 as a segment address = * 80 total bytes which,
                                            ; since each byte is 4 pixels, equals the draw row
    add ax, [gsa_DSP_Addr]
    mov es, ax
    mov ax, [bp+ScreenPage_X]
    mov bx, ax
    shr bx, 1
    shr bx, 1
    mov di, bx                              ; X / 4 = pixel offset in video memory
    and ax, 3                               ; X mod 4 = plane index of the pixel
    mov si, offset VGA_WriteMapMasks3
    add si, ax
    lodsb
    mov ah, al                              ; ah = map mask for the first pixel
    mov si, [bp+Img_Off]
    mov cx, [bp+FlicWidth]
    mov bx, [bp+Img_Seg]
    mov ds, bx
    mov bx, cx

loc_1E7ED:
    push di
    mov dx, 3C5h
    mov al, ah ; set the write plane for the pixel column
    out dx, al ; EGA port: sequencer data register
    lodsb ; byte #1
    cmp al, 0FFh ; empty column (transparent)
    jz short loc_1E832
    mov dh, al
    lodsb ; byte #2
    mov dl, al ; dx = encoded column data length in bytes (15 bits,
    ; the sign bit indicates whether there are repeats)
    cmp dh, 0
    js short loc_1E842

loc_1E803: ; chunk byte #2 (skip count)
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

loc_1E827:
    movsb
    add di, 4Fh ; same plane, one pixel down
    loop loc_1E827
    cmp dx, 1
    jns short loc_1E803 ; chunk byte #2 (skip count)

loc_1E832:
    pop di
    dec bx
    jz short @@Done
    shl ah, 1 ; next write plane
    cmp ah, 9 ; wrapping around?
    js short loc_1E840
    mov ah, 1 ; reset, and
    inc di ; increase the write address

loc_1E840:
    jmp short loc_1E7ED

loc_1E842:
    and dx, 7FFFh

loc_1E846: ; chunk byte #2 (skip count)
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

loc_1E86A:
    lodsb
    cmp al, 0E0h
    jnb short loc_1E87C
    stosb
    add di, 4Fh ; same plane, one pixel down
    loop loc_1E86A
    cmp dx, 1
    jns short loc_1E846 ; chunk byte #2 (skip count)
    jmp short loc_1E832

loc_1E87C:
    and al, 1Fh
    inc al
    push cx
    mov cl, al
    xor ch, ch
    lodsb

loc_1E886:
    stosb
    add di, 4Fh ; same plane, one pixel down
    loop loc_1E886 ; repeat the same color
    pop cx
    dec cx
    loop loc_1E86A
    cmp dx, 1
    jns short loc_1E846 ; chunk byte #2 (skip count)
    pop di
    dec bx
    jz short @@Done
    shl ah, 1 ; next write plane
    cmp ah, 9 ; wrapping around?
    js short loc_1E8A3
    mov ah, 1 ; reset, and
    inc di ; increase the write address

loc_1E8A3:
    jmp loc_1E7ED

@@Done:
    pop ds
    pop es
    pop di
    pop si
    pop bp
    ret

endp FLIC_Draw


; draws an LBX image into the current draw segment,
; column by column, decoding its RLE along the way,
; and processing any shaded colors by reading the
; current screen pixel and replacing with it from
; the predefined Replacement_Colors@ block

; int __cdecl __far FLIC_Draw_R(int ScreenPage_X, int ScreenPage_Y, int FlicWidth, int Img_Off, int Img_Seg)
proc FLIC_Draw_R

    ScreenPage_X= word ptr 6
    ScreenPage_Y= word ptr 8
    FlicWidth= word ptr 0Ah
    Img_Off= word ptr 0Ch
    Img_Seg= word ptr 0Eh

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

; draws an LBX image into the current draw segment,
; column by column, decoding its RLE along the way,
; and processing any shaded colors by reading the
; current screen pixel and replacing it from the
; predefined Replacement_Colors@ block
; maps the image into the EMS page frame, but will
; fail on images > 32k if they start 3 pages in

; int __cdecl __far FLIC_Draw_EMM(int ScreenPage_X, int ScreenPage_Y, int Img_Seg, int Frame_Index)

proc FLIC_Draw_EMM

    ScreenPage_X = word ptr 6
    ScreenPage_Y = word ptr 8
    Img_Seg = word ptr 0Ah
    Frame_Index = word ptr 0Ch

    push bp
    mov  bp, sp
    push si
    push di
    push es
    push ds

    mov ax, [bp+Img_Seg]
    mov ds, ax
    assume ds:nothing
    mov es, ax

    mov ax, ds:s_FLIC_HDR.fh_Width
    mov [cs:tmpFlicHdrWidth], ax

    mov si, s_FLIC_HDR.fh_EMM_Handle_Number

    lodsb  ; AX = [DS:SI]
    xor ah, ah
    mov dx, ax
    lodsb
    xor ah, ah
    mov bx, ax
    lodsw
    mov si, ax
    mov di, si

    push dx                                 ; argEmsHandle
    push bx                                 ; argFirstLogicalPage
    call EMM_MapMulti4                      ; maps in four consecutive logical pages from the
                                            ; passed handle, starting with the one specified
                                            ; uses a different EMM function than seg012:0251
                                            ; preserves all register values
    add sp, 4

    ;mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax
    ;assume ds:dseg
    assume ds:DGROUP

    mov ax, [bp+Frame_Index]
    shl ax, 1
    shl ax, 1
    add si, ax
    add si, 12h  ; 18
    mov ax, [EMM_PageFrameBaseAddress] ; contains the segment address of the EMS page frame
    mov ds, ax
    lodsw ; frame image offset low word
    mov dx, ax
    lodsw ; frame image offset high word
    inc dx
    adc ax, 0
    shl dx, 1
    rcl ax, 1
    shl dx, 1
    rcl ax, 1
    shr dx, 1
    shr dx, 1
    add al, [es:s_FLIC_HDR.fh_EMM_Logical_Page_Number]
    adc ah, 0
    add dx, [es:s_FLIC_HDR.fh_EMM_Logical_Page_Offset]
    cmp dx, 0C000h  ; 49152
    jb short loc_1EB13
    sub dx, 0C000h  ; 49152
    inc ax
    inc ax
    inc ax

loc_1EB13:
    mov [cs:tmpFlicHdrEmmLogicalPageOffset], ax
    mov bx, 11h  ; 17
    add bx, di
    mov bl, [bx]
    xor bh, bh
    mov di, bx
    mov si, dx
    mov bx, ax
    mov dl, [es:s_FLIC_HDR.fh_EMM_Handle_Number]
    xor dh, dh
    mov [cs:tmpFlicHdrEmmHandleNumber], dx
    push bx ; BUG: the code to load further pages only triggers
    ; on the last page, mapping only 2 here means that if
    ; the frame is larger than 32k, it will not be loaded
    ; properly unless it starts below 48k in the set, as
    ; in that case the original 4 page mapping will still
    ; be in place from above by accident
    push dx
    mov ax, 4400h
    int 67h ; - LIM EMS - MAP MEMORY
    ; AL = physical page number (0-3)
    ; BX = logical page number, DX = handle
    ; Return: AH = status
    mov ax, 4401h
    pop dx
    pop bx
    inc bx
    int 67h ; - LIM EMS - MAP MEMORY
    ; AL = physical page number (0-3)
    ; BX = logical page number, DX = handle
    ; Return: AH = status
    cmp di, 0
    jz short @@NayShading3
    push [bp+Img_Seg]
    push [bp+ScreenPage_Y]                  ; ScreenPage_Y
    push [bp+ScreenPage_X]                  ; ScreenPage_X
    ;push cs
    ;call near ptr FLIC_Draw_EMM_R       ; draws an LBX image into the current draw segment,
                                            ; column by column, decoding its RLE along the way,
                                            ; and processing any shaded colors by reading the
                                            ; current screen pixel and replacing with it from
                                            ; the predefined Replacement_Colors@ block
                                            ;
                                            ; the image must already be mapped into the EMS page
                                            ; frame, and si needs to point to the image data
    ;nop
    call FLIC_Draw_EMM_R
    add sp, 6

    pop ds
    pop es
    pop di
    pop si
    pop bp
    ret

@@NayShading3:
    ;mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax
    mov dx, 3C4h
    mov al, 2
    out dx, al ; EGA: sequencer address reg
    ; map mask: data bits 0-3 enable writes to bit planes 0-3
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
    mov bx, offset VGA_WriteMapMasks3 ; should use dseg:41d0
    add bx, ax
    mov ah, [bx] ; write mask for the first pixel
    mov bx, [EMM_PageFrameBaseAddress] ; contains the segment address of the EMS page frame
    mov ds, bx
    mov bx, [cs:tmpFlicHdrWidth]

@@Loop48KB3:
    push di
    cmp si, 0C000h
    jb short @@LoopColumn3
    call FLIC_EMM_MapNextPages ; changes the EMM page mapping such that the previous
    ; last page now becomes the first one
    ; preserves ax and bx, reduces si by $C000 (3 pages)

@@LoopColumn3:
    mov dx, 3C5h
    mov al, ah
    out dx, al ; EGA port: sequencer data register
    lodsb ; byte #1
    cmp al, 0FFh ; empty column (transparent)
    jz short @@SkipColumn3
    mov dh, al
    lodsb ; byte #2
    mov dl, al ; dx = encoded column data length in bytes (15 bits,
    ; the sign bit indicates whether there are repeats)
    cmp dh, 0
    js short @@ULC_Run_Count3

@@ULC_Copy3: ; chunk byte #2 (skip count)
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

@@ULC_Copy_Do3:
    movsb
    add di, 4Fh ; Line-Delta same plane, one pixel down
    loop @@ULC_Copy_Do3
    cmp dx, 1
    jns short @@ULC_Copy3 ; chunk byte #2 (skip count)

@@SkipColumn3:
    pop di
    dec bx
    jz short @@Done3
    shl ah, 1 ; next write plane
    cmp ah, 9 ; wrapping around?
    js short @@EndOfColumn_YaySkip3
    mov ah, 1 ; reset, and
    inc di ; increase the write address

@@EndOfColumn_YaySkip3:
    jmp short @@Loop48KB3

@@ULC_Run_Count3:
    and dx, 7FFFh

@@ULC_Run3: ; chunk byte #2 (skip count)
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

loc_1EC1F:
    lodsb
    cmp al, 0E0h
    jnb short loc_1EC31
    stosb
    add di, 4Fh ; Line-Delta same plane, one pixel down
    loop loc_1EC1F
    cmp dx, 1
    jns short @@ULC_Run3 ; chunk byte #2 (skip count)
    jmp short @@SkipColumn3

loc_1EC31:
    and al, 1Fh
    inc al
    push cx
    mov cl, al
    xor ch, ch
    lodsb

loc_1EC3B:
    stosb
    add di, 4Fh ; Line-Delta same plane, one pixel down
    loop loc_1EC3B
    pop cx
    dec cx
    loop loc_1EC1F
    cmp dx, 1
    jns short @@ULC_Run3 ; chunk byte #2 (skip count)
    pop di
    dec bx
    jz short @@Done3
    shl ah, 1 ; next write plane
    cmp ah, 9 ; wrapping around?
    js short @@EndOfColumn_NaySkip3
    mov ah, 1 ; reset, and
    inc di ; increase the write address

@@EndOfColumn_NaySkip3:
    jmp @@Loop48KB

@@Done3:
    pop ds
    pop es
    pop di
    pop si
    pop bp
    ret

endp FLIC_Draw_EMM


; changes the EMM page mapping such that the previous
; last page now becomes the first one
; preserves ax and bx, reduces si by $C000 (3 pages)

; int FLIC_EMM_MapNextPages(void)

proc FLIC_EMM_MapNextPages

    push bx
    mov cx, ax
    mov bx, [cs:tmpFlicHdrEmmLogicalPageOffset]
    inc bx
    inc bx
    inc bx
    mov [cs:tmpFlicHdrEmmLogicalPageOffset], bx
    mov dx, [cs:tmpFlicHdrEmmHandleNumber]
    push dx                                 ; argEmsHandle
    push bx                                 ; argFirstLogicalPage
    call EMM_MapMulti4                      ; maps in four consecutive logical pages from the
                                            ; passed handle, starting with the one specified
                                            ; uses a different EMM function than seg012:0251
                                            ; preserves all register values
    add sp, 4
    sub si, 0C000h                          ; 3 * sizeof(EMM Logical Page)  3*16KB = 48KB
    mov ax, cx
    pop bx
    ret

endp FLIC_EMM_MapNextPages


; draws an LBX image into the current draw segment,
; column by column, decoding its RLE along the way,
; and processing any shaded colors by reading the
; current screen pixel and replacing with it from
; the predefined Replacement_Colors@ block
;
; the image must already be mapped into the EMS page
; frame, and si needs to point to the image data

; int __cdecl __far FLIC_Draw_EMM_R(int ScreenPage_X, int ScreenPage_Y)

proc FLIC_Draw_EMM_R

    ScreenPage_X= word ptr 6
    ScreenPage_Y= word ptr 8

    push bp
    mov  bp, sp

    ;mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax

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
    mov cl, al
    shl cl, 1
    shl cl, 1
    shl cl, 1
    shl cl, 1
    mov bx, offset VGA_WriteMapMasks3 ; should use dseg:41d0
    add bx, ax
    mov al, [bx]
    mov ah, al
    or ah, cl ; low nibble = map mask for the first pixel
    ; high nibble = read map for the same
    mov bx, [EMM_PageFrameBaseAddress] ; contains the segment address of the EMS page frame
    mov ds, bx
    mov bx, [cs:tmpFlicHdrWidth]
@@Loop48KB:
    push di
    cmp si, 0C000h
    jb short loc_1ECD9
    call FLIC_EMM_MapNextPages ; changes the EMM page mapping such that the previous
    ; last page now becomes the first one
    ; preserves ax and bx, reduces si by $C000 (3 pages)
loc_1ECD9:
    mov dx, e_GC_INDEX
    mov al, e_GC_READMAP
    out dx, al ; EGA: graph 1 and 2 addr reg:
    ; read map select.
    ; Data bits 0-2 select map # for read mode 00.
    inc dx ; _DX = GC_DATA
    mov al, ah
    shr al, 1
    shr al, 1
    shr al, 1
    shr al, 1 ; set the read plane for the pixel column
    out dx, al ; EGA port: graphics controller data register
    mov dx, e_SC_INDEX
    mov al, e_SC_MAPMASK
    out dx, al ; EGA: sequencer address reg
    ; map mask: data bits 0-3 enable writes to bit planes 0-3
    inc dx ; _DX = SC_DATA
    mov al, ah
    and al, 0Fh ; set the write plane for the pixel column
    out dx, al ; EGA port: sequencer data register
    lodsb ; byte #1
    cmp al, 0FFh ; empty column (transparent)
    jz short loc_1ED3D
    mov dh, al
    lodsb ; byte #2
    mov dl, al ; dx = encoded column data length in bytes (15 bits,
    ; the sign bit indicates whether there are repeats)
    cmp dh, 0
    jns short loc_1ED09 ; chunk byte #2 (skip count)
    jmp loc_1ED96
loc_1ED09: ; chunk byte #2 (skip count)
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
loc_1ED2D:
    lodsb
    cmp al, 0E8h
    jnb short loc_1ED5F
    stosb
    add di, 4Fh ; same plane, one pixel down
    loop loc_1ED2D
    cmp dx, 1
    jns short loc_1ED09 ; chunk byte #2 (skip count)
loc_1ED3D:
    pop di
    dec bx
    jz short loc_1ED5D
    mov cl, ah
    and cl, 0F0h ; read plane
    add cl, 10h ; -> next
    and ah, 0Fh ; write plane
    shl ah, 1 ; -> next
    cmp ah, 9 ; wrapped around?
    js short loc_1ED58
    mov cl, 0 ; reset the read map,
    mov ah, 1 ; reset the write mask,
    inc di ; and increase the memory offset
loc_1ED58:
    or ah, cl
    jmp @@Loop48KB
loc_1ED5D:
    pop bp
    ret

loc_1ED5F:
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
    add si, [sa_ReplacementColors] ; 180h LBX_Alloc_Space paragraphs
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
    add di, 4Fh ; 'O' ; same plane, one pixel down
    loop loc_1ED2D
    cmp dx, 1
    js short loc_1ED3D
    jmp loc_1ED09 ; chunk byte #2 (skip count)
loc_1ED96:
    and dx, 7FFFh
loc_1ED9A: ; chunk byte #2 (skip count)
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
loc_1EDBE:
    lodsb
    cmp al, 0E0h
    jnb short loc_1EE0E
    cmp al, 0E8h
    jnb short loc_1EDD7
    stosb
    add di, 4Fh ; same plane, one pixel down
    loop loc_1EDBE
    cmp dx, 1
    jns short loc_1ED9A ; chunk byte #2 (skip count)
    jmp loc_1ED3D
    pop bp
    ret

loc_1EDD7:
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
    add si, [sa_ReplacementColors] ; 180h LBX_Alloc_Space paragraphs
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
    loop loc_1EDBE
    cmp dx, 1
    jns short loc_1ED96
    jmp loc_1ED3D
loc_1EE0E:
    and al, 1Fh
    inc al
    push cx
    mov cl, al
    xor ch, ch
    lodsb
    cmp al, 0E8h
    jnb short loc_1EE31
@@LoopColumn:
    stosb
    add di, 4Fh ; same plane, one pixel down
    loop @@LoopColumn
    pop cx
    dec cx
    loop loc_1EDBE
    cmp dx, 1
    jns short loc_1EE2E
    jmp loc_1ED3D
loc_1EE2E: ; chunk byte #2 (skip count)
    jmp loc_1ED9A
loc_1EE31:
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
    add si, [sa_ReplacementColors] ; 180h LBX_Alloc_Space paragraphs
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
loc_1EE4E:
    mov al, [es:di]
    mov si, ax
    and si, 0FFh
    movsb
    add di, 4Fh ; same plane, one pixel down
    loop loc_1EE4E

    pop ds
    pop si
    pop cx
    dec cx
    dec cx
    jz short loc_1EE67
loc_1EE64:
    jmp loc_1EDBE
loc_1EE67:
    cmp dx, 1
loc_1EE6A:
    jns short loc_1EE2E
loc_1EE6C:
    jmp loc_1ED3D
    pop bp
    ret

endp FLIC_Draw_EMM_R

;ends seg027

end
