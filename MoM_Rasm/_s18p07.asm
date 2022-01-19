; _s18p07.asm VGA_GetStringWidth
; ST_TXT.H

.MODEL LARGE, C

EXTRN gsa_FontStyleData:WORD

.CODE
;segment seg018 byte public 'CODE' use16
; 	 assume cs:seg018
; 	 ;org 0Ch
; 	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_GetStringWidth

proc VGA_GetStringWidth

    String = word ptr 6

    push bp
    mov  bp, sp
    push es
    push ds
    push si
    push di

    mov si, [bp+String]

    mov ax, [gsa_FontStyleData]
    mov es, ax

    ;mov bx, FONT_HEADER.Horz_Spacing
    mov bx, 48h     ; 48h 72d  FONT_HEADER.Horz_Spacing
    mov cx, [es:bx]
    mov dx, 0
    mov bh, 0
    mov ah, 0

@@LoopChars:
    lodsb
    sub al, 20h
    js short loc_1AF77
    cmp al, 5Eh
    jns short @@LoopChars
    mov bl, al
    ;mov al, [es:bx+FONT_HEADER.Glyph_Widths]
    mov al, [es:bx+4Ah]  ; 4Ah 74d  FONT_HEADER.Glyph_Widths
    add dx, ax
    add dx, cx
    jmp short @@LoopChars

loc_1AF77:
    add al, 20h
    jz short @@Done
    cmp al, 0Dh
    jz short @@Done
    cmp al, 14h
    jz short @@Done
    cmp al, 15h
    jz short @@Done
    cmp al, 19h
    jz short @@Done
    cmp al, 1Dh
    jz short @@Done
    jmp short @@LoopChars

@@Done:
    mov ax, dx
    sub ax, cx

    pop di
    pop si
    pop ds
    pop es
    pop bp
    ret

endp VGA_GetStringWidth

;ends seg018

end
