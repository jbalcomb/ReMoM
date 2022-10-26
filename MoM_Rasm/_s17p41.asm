TITLE _s17p41.asm VGA_FontColor3Toggle
; ST_TXT.H

.MODEL LARGE, C

EXTRN sa_FontStyleData:WORD

EXTRN farpeekb:PROC
EXTRN VGA_FontColorSelect:PROC

.CODE
;segment seg017 byte public 'CODE' use16
;	 assume cs:seg017
;	 ;org 0Ah
;	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_FontColor3Toggle

proc VGA_FontColor3Toggle

    push bp
    mov  bp, sp

    ;mov ax, FONT_HEADER.Color_Index
    mov ax, 14h  ; 14h 20d  FONT_HEADER.Color_Index
    push ax
    push [sa_FontStyleData]
    call farpeekb
    pop cx
    pop cx

    cmp ax, 2
    jnz short loc_19D6C
    xor ax, ax
    jmp short loc_19D6F

loc_19D6C:
    mov ax, 2

loc_19D6F:
    push ax
    ;nop
    ;push cs
    ;call near ptr VGA_FontColorSelect
    call VGA_FontColorSelect
    pop cx

    pop bp
    ret

endp VGA_FontColor3Toggle

;ends seg017

end
