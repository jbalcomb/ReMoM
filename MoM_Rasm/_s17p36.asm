TITLE _s17p36.asm VGA_DrawStyledString
; ST_TXT.H

.MODEL LARGE, C

EXTRN gsa_FontStyleData:WORD
EXTRN g_VGA_Outline_Color:WORD
EXTRN g_VGA_Skip_AA_Pixels:WORD

EXTRN farpeekb:PROC
EXTRN farpokeb:PROC
EXTRN VGA_DrawString:PROC
EXTRN VGA_FontColorSelect:PROC

.CODE
;segment seg017 byte public 'CODE' use16
;	 assume cs:seg017
;	 ;org 0Ah
;	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_DrawStyledString

Shadow_BtmRight     EQU 1
Shadow_TopLeft      EQU 2
Shadow_BtmRight_2px EQU 3
Outline_Full        EQU 4
Outline_Plus_BR2px  EQU 5

proc VGA_DrawStyledString

    Loop_Var       = word ptr -4
    Outline_Style  = word ptr -2
    Left           = word ptr 6
    Top            = word ptr 8
    String         = word ptr 0Ah
    JstfyTo        = word ptr 0Ch

    push bp
    mov  bp, sp
    sub  sp, 4
    push si
    push di

    mov si, [bp+Left]
    mov di, [bp+Top]
    ;mov ax, FONT_HEADER.Outline_Style
    mov ax, 12h  ; 12h 18d  FONT_HEADER.Outline_Style
    push ax
    push [gsa_FontStyleData]
    call farpeekb
    pop cx
    pop cx

    mov [bp+Outline_Style], ax
    cmp [bp+Outline_Style], 0
    jnz short @@FontHasOutlineStyle
    jmp loc_19B29

@@FontHasOutlineStyle:
    mov [bp+Loop_Var], 0
    jmp short @@Loop16X

loc_19988:
    push [g_VGA_Outline_Color]
    push [bp+Loop_Var]
    push [gsa_FontStyleData]
    call farpokeb
    add sp, 6

    inc [bp+Loop_Var]

@@Loop16X:
    cmp [bp+Loop_Var], 10h
    jl short loc_19988
    mov [g_VGA_Skip_AA_Pixels], 1
    cmp [bp+Outline_Style], Shadow_TopLeft
    jz short loc_199F5
    push [bp+JstfyTo]
    xor ax, ax
    push ax
    push [bp+String]
    mov ax, di
    inc ax
    push ax
    mov ax, si
    inc ax
    push ax
    nop
    push cs
    call near ptr VGA_DrawString
    add sp, 0Ah

    push [bp+JstfyTo]
    xor ax, ax
    push ax
    push [bp+String]
    mov ax, di
    inc ax
    push ax
    push si
    nop
    push cs
    call near ptr VGA_DrawString
    add sp, 0Ah

    push [bp+JstfyTo]
    xor ax, ax
    push ax
    push [bp+String]
    push di
    mov ax, si
    inc ax
    push ax
    nop
    push cs
    call near ptr VGA_DrawString
    add sp, 0Ah

loc_199F5:
    cmp [bp+Outline_Style], Shadow_BtmRight
    jz short loc_19A46
    cmp [bp+Outline_Style], Shadow_BtmRight_2px
    jz short loc_19A46
    push [bp+JstfyTo]
    xor ax, ax
    push ax
    push [bp+String]
    push di
    mov ax, si
    dec ax
    push ax
    nop
    push cs
    call near ptr VGA_DrawString
    add sp, 0Ah

    push [bp+JstfyTo]
    xor ax, ax
    push ax
    push [bp+String]
    mov ax, di
    dec ax
    push ax
    mov ax, si
    dec ax
    push ax
    nop
    push cs
    call near ptr VGA_DrawString
    add sp, 0Ah

    push [bp+JstfyTo]
    xor ax, ax
    push ax
    push [bp+String]
    mov ax, di
    dec ax
    push ax
    push si
    nop
    push cs
    call near ptr VGA_DrawString
    add sp, 0Ah

loc_19A46:
    cmp [bp+Outline_Style], Shadow_BtmRight_2px
    jz short loc_19A52
    cmp [bp+Outline_Style], Outline_Plus_BR2px
    jnz short loc_19AA5

loc_19A52:
    push [bp+JstfyTo]
    xor ax, ax
    push ax
    push [bp+String]
    mov ax, di
    add ax, 2
    push ax
    mov ax, si
    add ax, 2
    push ax
    nop
    push cs
    call near ptr VGA_DrawString
    add sp, 0Ah

    push [bp+JstfyTo]
    xor ax, ax
    push ax
    push [bp+String]
    mov ax, di
    add ax, 2
    push ax
    mov ax, si
    inc ax
    push ax
    nop
    push cs
    call near ptr VGA_DrawString
    add sp, 0Ah

    push [bp+JstfyTo]
    xor ax, ax
    push ax
    push [bp+String]
    mov ax, di
    inc ax
    push ax
    mov ax, si
    add ax, 2
    push ax
    nop
    push cs
    call near ptr VGA_DrawString
    add sp, 0Ah

loc_19AA5:
    cmp [bp+Outline_Style], Shadow_BtmRight_2px
    jle short loc_19ADD
    push [bp+JstfyTo]
    xor ax, ax
    push ax
    push [bp+String]
    mov ax, di
    dec ax
    push ax
    mov ax, si
    inc ax
    push ax
    nop
    push cs
    call near ptr VGA_DrawString
    add sp, 0Ah

    push [bp+JstfyTo]
    xor ax, ax
    push ax
    push [bp+String]
    mov ax, di
    inc ax
    push ax
    mov ax, si
    dec ax
    push ax
    nop
    push cs
    call near ptr VGA_DrawString
    add sp, 0Ah

loc_19ADD:
    cmp [bp+Outline_Style], Outline_Plus_BR2px
    jnz short loc_19B13
    push [bp+JstfyTo]
    xor ax, ax
    push ax
    push [bp+String]
    push di
    mov ax, si
    add ax, 2
    push ax
    nop
    push cs
    call near ptr VGA_DrawString
    add sp, 0Ah

    push [bp+JstfyTo]
    xor ax, ax
    push ax
    push [bp+String]
    mov ax, di
    add ax, 2
    push ax
    push si
    nop
    push cs
    call near ptr VGA_DrawString
    add sp, 0Ah

loc_19B13:
    ;mov ax, FONT_HEADER.Color_Index
    mov ax, 13h  ; 13h 19d  FONT_HEADER.Color_Index
    push ax
    push [gsa_FontStyleData]
    call farpeekb
    pop cx
    pop cx

    push ax
    nop
    push cs
    call near ptr VGA_FontColorSelect
    pop cx

loc_19B29:
    mov [g_VGA_Skip_AA_Pixels], 0
    push [bp+JstfyTo]
    mov ax, 1
    push ax
    push [bp+String]
    push di
    push si
    nop
    push cs
    call near ptr VGA_DrawString
    add sp, 0Ah

    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp VGA_DrawStyledString

;ends seg017

end
