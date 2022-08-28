TITLE   _s15p01.ASM VGA_SetModeY
; NOTE(Jimbalcomb,20220119): I found a copy of almost exactly this same code is a very old forum
; but, I have since lost track of it. :(..
; FWIW, it well predates Abrash's DDJ publication.
; NOTE(JimBalcomb,20220721): I am fairly certain I found the above reference based on old time limited search on google for some odd/noteable piece of the assembly code.
; maybe/maybe not, the and or stuff, maybe/maybe not in hex vs. binary

.MODEL LARGE, C

INCLUDE VGA_HW.INC

segment	seg015 byte public 'CODE' use16
    assume cs:seg015

PUBLIC VGA_SetModeY

proc VGA_SetModeY far

    push	si
    push	di
    push	es
    push	ds

    mov	al, 13h                             ; Mode 13h; 320x200 256 color graphics (MCGA,VGA)
    mov ah, 00h                             ; Set Video Mode
    int	10h                                 ; Video BIOS

    mov	dx, SC_INDEX
    mov	al, SC_MEMORY_MODE
    out	dx, al

    mov	dx, SC_DATA
    in	al, dx
    and	al, 11110111b                       ; Clear Bit 3: Enable - Chain 4
    or	al, 00000100b                       ; Set Bit 2: Disable - Odd/Even Host Memory Write Adressing
    out	dx, al

    ; NOTE(JimBalcomb,20220721): I can not recall what the issue was such that I chose/needed to comment out this chunk of code
    ; mov	ax, VIDEO_RAM
    ; mov	es, ax
    ; sub	di, di
    ; mov	ax, di
    ; mov	cx, 8000h                           ; 32,768 times
    ; cld
    ; rep stosw

    mov	dx, CRTC_INDEX
    mov	al, CRTC_UNDERLINE_LOCATION
    out	dx, al

    mov	dx, CRTC_DATA
    in	al, dx
    and	al, 10111111b                       ; Clear Bit 6: Disable - Double-Word Addressing
    out	dx, al

    mov	dx, CRTC_DATA
    mov	al, CRTC_MODE_CONTROL
    out	dx, al

    mov	dx, CRTC_INDEX
    in	al, dx
    or	al, 01000000b                       ; Set Bit 6: byte address mode
    out	dx, al

    mov	dx, SC_INDEX
    mov	al, SC_MAP_MASK
    out	dx, al

    pop	ds
    pop	es
    pop	di
    pop	si

    ret

endp VGA_SetModeY

ends seg015

end
