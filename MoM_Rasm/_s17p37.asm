TITLE _s17p37.asm VGA_DrawString
; ST_TXT.H

.MODEL LARGE, C

EXTRN g_VGA_Skip_AA_Pixels:WORD
EXTRN g_VGA_TextDraw_Y:WORD
EXTRN g_VGA_TextDraw_X:WORD
EXTRN gsa_FontStyleData:WORD

EXTRN farpeekw:PROC
EXTRN VGA_DrawChar:PROC
EXTRN VGA_DrawChar_NoAA:PROC
EXTRN VGA_FontColor2Toggle:PROC
EXTRN VGA_FontColor3Toggle:PROC
EXTRN VGA_GetStringWidth:PROC
EXTRN VGA_UseFontColor1:PROC

.CODE
;segment seg017 byte public 'CODE' use16
;	 assume cs:seg017
;	 ;org 0Ah
;	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_DrawString

proc VGA_DrawString

    Switch_Char       = word ptr -10h
    Char_Index        = word ptr -0Eh
    Space_Extension   = word ptr -0Ch
    Pixels_to_Spread  = word ptr -0Ah
    Spaces_Drawn      = word ptr -8
    Spaces_on_Line    = word ptr -6
    Char              = word ptr -4
    Char_Index_2      = word ptr -2
    Left              = word ptr 6
    Top               = word ptr 8
    String            = word ptr 0Ah
    Colors            = word ptr 0Ch
    JstfyTo           = word ptr 0Eh

    push bp
    mov  bp, sp
    sub  sp, 10h
    push si
    push di

    mov si, [bp+String]

    mov di, [bp+JstfyTo]

    mov [bp+Char_Index_2], 0

    mov ax, [bp+Left]
    mov [g_VGA_TextDraw_X], ax

    mov ax, [bp+Top]
    mov [g_VGA_TextDraw_Y], ax
    
    or di, di
    jnz short loc_19B6F
    ;db 0E9h,7Fh,0 ; <BAD>jmp loc_19BEE
    jmp loc_19BEE

loc_19B6F:
    mov [bp+Spaces_Drawn], 0
    mov [bp+Pixels_to_Spread], 0
    mov [bp+Char_Index], 0
    mov [bp+Spaces_on_Line], 0
    jmp short loc_19B93

loc_19B85:
    mov bx, [bp+Char_Index]
    ;cmp [byte ptr bx+si], ' '
    cmp byte ptr [bx+si], ' '
    jnz short loc_19B90
    inc [bp+Spaces_on_Line]

loc_19B90:
    inc [bp+Char_Index]

loc_19B93:
    mov bx, [bp+Char_Index]
    ;cmp [byte ptr bx+si], 0
    cmp byte ptr [bx+si], 0
    jz short loc_19BC3
    mov bx, [bp+Char_Index]
    ;cmp [byte ptr bx+si], 0Dh
    cmp byte ptr [bx+si], 0Dh
    jz short loc_19BC3
    mov bx, [bp+Char_Index]
    ;cmp [byte ptr bx+si], 14h
    cmp byte ptr [bx+si], 14h
    jz short loc_19BC3
    mov bx, [bp+Char_Index]
    ;cmp [byte ptr bx+si], 19h
    cmp byte ptr [bx+si], 19h
    jz short loc_19BC3
    mov bx, [bp+Char_Index]
    ;cmp [byte ptr bx+si], 15h
    cmp byte ptr [bx+si], 15h
    jz short loc_19BC3
    mov bx, [bp+Char_Index]
    ;cmp [byte ptr bx+si], 1Dh
    cmp byte ptr [bx+si], 1Dh
    jnz short loc_19B85

loc_19BC3:
    cmp [bp+Spaces_on_Line], 0
    jnz short loc_19BCB
    xor di, di

loc_19BCB:
    push si
    call VGA_GetStringWidth
    pop cx
    sub di, ax
    or di, di
    jg short loc_19BDC
    xor di, di
    jmp short loc_19BEE

loc_19BDC:
    mov ax, di
    cwd
    idiv [bp+Spaces_on_Line]
    mov [bp+Pixels_to_Spread], dx
    mov ax, di
    cwd
    idiv [bp+Spaces_on_Line]
    mov [bp+Space_Extension], ax

loc_19BEE:
    jmp loc_19CD4

loc_19BF1:
    mov ax, [bp+Char]
    mov [bp+Switch_Char], ax
    mov cx, 9
    mov bx, offset word_19CED

loc_19BFD:
    mov ax, [cs:bx]
    cmp ax, [bp+Switch_Char]
    jz short loc_19C0C
    add bx, 2
    loop loc_19BFD
    jmp short loc_19C80

loc_19C0C:
    jmp [word ptr cs:bx+12h]

loc_19C10:
    cmp [bp+Colors], 0
    jz short loc_19C1B
    nop
    push cs
    call near ptr VGA_UseFontColor1

loc_19C1B:
    jmp loc_19CD1

loc_19C1E:
    cmp [bp+Colors], 0
    jz short loc_19C29
    nop
    push cs
    call near ptr VGA_FontColor2Toggle

loc_19C29:
    jmp loc_19CD1

loc_19C2C:
    cmp [bp+Colors], 0
    jz short loc_19C37
    nop
    push cs
    call near ptr VGA_FontColor3Toggle

loc_19C37:
    jmp loc_19CD1

loc_19C3A:
    cmp [bp+Colors], 0
    jz short loc_19C45
    nop
    push cs
    call near ptr VGA_FontColor2Toggle

loc_19C45:
    jmp loc_19CD1

loc_19C48:
    ;mov ax, FONT_HEADER.Line_Height
    mov ax, 44h  ; 44h 68d  FONT_HEADER.Line_Height
    push ax
    push [gsa_FontStyleData]
    call farpeekw
    pop cx
    pop cx
    add [g_VGA_TextDraw_Y], ax
    mov ax, [bp+Left]
    mov [g_VGA_TextDraw_X], ax
    jmp short loc_19CD1

loc_19C63:
    mov bx, [bp+Char_Index_2]
    add bx, si
    mov al, [bx+1]
    mov ah, 0
    mov dx, [bp+Left]
    add dx, ax
    mov [g_VGA_TextDraw_X], dx
    inc [bp+Char_Index_2]
    jmp short loc_19CD1

loc_19C7B:
    mov ax, [g_VGA_TextDraw_X]
    jmp short loc_19CE6

loc_19C80:
    cmp [g_VGA_Skip_AA_Pixels], 0
    jnz short loc_19C99
    push [bp+Char]
    push [g_VGA_TextDraw_Y]
    push [g_VGA_TextDraw_X]
    call VGA_DrawChar
    jmp short loc_19CA9

loc_19C99:
    push [bp+Char]
    push [g_VGA_TextDraw_Y]
    push [g_VGA_TextDraw_X]
    call VGA_DrawChar_NoAA

loc_19CA9:
    add sp, 6
    mov [g_VGA_TextDraw_X], ax
    or di, di
    jz short loc_19CCF
    cmp [bp+Char], ' '
    jnz short loc_19CCF
    mov ax, [bp+Space_Extension]
    add [g_VGA_TextDraw_X], ax
    mov ax, [bp+Spaces_Drawn]
    cmp ax, [bp+Pixels_to_Spread]
    jge short loc_19CCC
    inc [g_VGA_TextDraw_X]

loc_19CCC:
    inc [bp+Spaces_Drawn]

loc_19CCF:
    jmp short $+2

loc_19CD1:
    inc [bp+Char_Index_2]

loc_19CD4:
    mov bx, [bp+Char_Index_2]
    mov al, [bx+si]
    cbw
    mov [bp+Char], ax
    or ax, ax
    jz short loc_19CE4
    jmp loc_19BF1

loc_19CE4:
    jmp short loc_19C7B

loc_19CE6:
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp VGA_DrawString

db 0
word_19CED dw 1, 2, 3, 4
dw 0Dh, 14h, 15h, 19h
dw 1Dh
off_19CFF dw offset loc_19C10
dw offset loc_19C1E
dw offset loc_19C2C
dw offset loc_19C3A
dw offset loc_19C48
dw offset loc_19C48
dw offset loc_19C7B
dw offset loc_19C63
dw offset loc_19C63

;ends seg017

end
