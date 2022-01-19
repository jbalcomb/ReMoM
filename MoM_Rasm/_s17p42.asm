TITLE _s17p42.asm VGA_FontColorSelect
; ST_TXT.H

.MODEL LARGE, C

EXTRN gsa_FontStyleData:WORD

EXTRN farpeekw:PROC
EXTRN farpokeb:PROC
EXTRN farpokew:PROC

.CODE
;segment seg017 byte public 'CODE' use16
;	 assume cs:seg017
;	 ;org 0Ah
;	 assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_FontColorSelect

proc VGA_FontColorSelect

    Color_Set_Pointer  = word ptr -2
    ColorSet_Index     = word ptr 6

    push bp
    mov  bp, sp
    sub  sp, 2
    push si
    push di

    mov di, [bp+ColorSet_Index]
    mov ax, di
    mov cl, 4
    shl ax, cl
    ;add ax, FONT_HEADER.Colors_1
    add ax, 14h  ; 14h 20d  FONT_HEADER.Colors_1
    mov [bp+Color_Set_Pointer], ax
    xor si, si
    jmp short loc_19DB5

loc_19D93:
    mov ax, si
    add ax, [bp+Color_Set_Pointer]
    push ax
    push [gsa_FontStyleData]
    call farpeekw
    pop cx
    pop cx
    push ax
    push si
    push [gsa_FontStyleData]
    call farpokew
    add sp, 6
    add si, 2

loc_19DB5:
    cmp si, 10h
    jl short loc_19D93
    push di
    ;mov ax, FONT_HEADER.Color_Index
    mov ax, 13h  ; 13h 19d  FONT_HEADER.Color_Index
    push ax
    push [gsa_FontStyleData]
    call farpokeb
    add sp, 6

    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp VGA_FontColorSelect


;ends seg017

end
