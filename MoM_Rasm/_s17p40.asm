TITLE _s17p40.asm VGA_FontColor2Toggle
; ST_TXT.H

.MODEL LARGE, C

EXTRN gsa_FontStyleData:WORD

EXTRN farpeekb:PROC
EXTRN VGA_FontColorSelect:PROC

.CODE
;segment seg017 byte public 'CODE' use16
;	 assume cs:seg017
;	 ;org 0Ah
;	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_FontColor2Toggle

proc VGA_FontColor2Toggle

    push bp
    mov bp, sp
    ;mov ax, FONT_HEADER.Color_Index
    mov ax, 14h  ; 14h 20d  FONT_HEADER.Color_Index
    push ax
    push [gsa_FontStyleData]
    call farpeekb
    pop cx
    pop cx

    cmp ax, 1
    jnz short loc_19D45
    xor ax, ax
    jmp short loc_19D48

loc_19D45:
    mov ax, 1

loc_19D48:
    push ax
    ;nop
    ;push cs
    ;call near ptr VGA_FontColorSelect
    call VGA_FontColorSelect
    pop cx

    pop bp
    ret

endp VGA_FontColor2Toggle


;ends seg017

end
