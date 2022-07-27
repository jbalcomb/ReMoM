TITLE _s26p07.asm VGA_Copy_RSP_DSP_NM
; ST_VGA.H

.MODEL LARGE, C

EXTRN g_RSP_Idx:WORD

EXTRN MD_CDraw_Restore:PROC
EXTRN MD_CDraw_Disable:PROC

.CODE
;segment seg026 byte public 'CODE' use16
;    assume cs:seg026
;    ;org 0Ah
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_Copy_RSP_DSP_NM

proc VGA_Copy_RSP_DSP_NM

    push ds
    push es
    push si
    push di

    call MD_CDraw_Disable

    mov ax, [g_RSP_Idx]
    mov ah, al
    xor al, al
    shl ax, 1
    shl ax, 1
    add ax, 0A000h
    mov ds, ax

    mov bx, 0A400h
    sub bx, ax
    add bx, 0A000h
    mov es, bx

    mov dx, 3C4h
    mov al, 2
    out dx, al

    inc dx
    mov al, 0Fh
    out dx, al

    sub si, si
    sub di, di
    mov ax, 8
    mov dx, 3CEh
    out dx, al

    inc dx
    xor al, al
    out dx, al

    mov cx, 3E80h
    rep movsb

    mov al, 0FFh
    out dx, al

    call MD_CDraw_Restore

    pop di
    pop si
    pop es
    pop ds
    ret

endp VGA_Copy_RSP_DSP_NM

;ends seg026

end
