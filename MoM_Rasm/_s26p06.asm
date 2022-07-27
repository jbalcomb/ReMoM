TITLE _s26p06.asm VGA_Copy_RSP_DSP_YM
; ST_VGA.H

.MODEL LARGE, C

EXTRN g_RSP_Idx:WORD

EXTRN MD_CDraw_Restore:PROC
EXTRN MD_CDraw_Disable:PROC
EXTRN MD_GetX:PROC
EXTRN MD_GetY:PROC
EXTRN MD_MoveCursor:PROC
EXTRN GUI_FindWindow:PROC
EXTRN CRH_Draw_RSP:PROC
EXTRN CRL_Save_RSP:PROC
EXTRN CRL_Restore_RSP:PROC
EXTRN CRL_Restore_DSP:PROC

.CODE
;segment seg026 byte public 'CODE' use16
;    assume cs:seg026
;    ;org 0Ah
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_Copy_RSP_DSP_YM

proc VGA_Copy_RSP_DSP_YM

    Data_Seg     = word ptr -0Ah
    Top          = word ptr -6
    Left         = word ptr -4
    tmp_RSP_Addr  = word ptr -2

    push bp
    mov  bp, sp
    sub  sp, 0Ah
    push ds
    push es
    push si
    push di

    mov ax, ds
    mov [bp+Data_Seg], ax

    call MD_CDraw_Disable

    call MD_GetX
    mov [bp+Left], ax

    call MD_GetY
    mov [bp+Top], ax

    mov ax, [g_RSP_Idx]
    mov ah, al
    xor al, al
    shl ax, 1
    shl ax, 1
    add ax, 0A000h
    mov ds, ax
    ;assume ds:nothing

    mov [bp+tmp_RSP_Addr], ax

    mov bx, 0A400h
    sub bx, ax
    add bx, 0A000h
    mov es, bx
    sub si, si
    sub di, di

loc_1E58D:
    mov dx, 3C4h
    mov al, 2
    out dx, al
    inc dx
    mov al, 0Fh
    out dx, al
    mov ax, 8
    mov dx, 3CEh
    out dx, al
    inc dx
    mov al, 0
    out dx, al
    mov cx, 0FA0h
    rep movsb

    mov al, 0FFh
    out dx, al

    mov ax, [bp+Data_Seg]
    mov ds, ax
    ;assume ds:dseg
    assume ds:DGROUP

    call CRL_Restore_DSP

    call MD_GetY
    push ax
    call MD_GetX
    cmp ax, [bp+Left]
    jnz short loc_1E5CB
    pop bx
    cmp bx, [bp+Top]
    jz short loc_1E5ED
    push bx

loc_1E5CB:
    push ax
    call GUI_FindWindow
    call CRL_Restore_RSP
    call CRL_Save_RSP
    call GUI_FindWindow
    call CRH_Draw_RSP
    call MD_MoveCursor
    add sp, 4

loc_1E5ED:
    mov ds, [bp+tmp_RSP_Addr]
    cmp si, 3E80h
    jb short loc_1E58D

    mov ax, [bp+Data_Seg]
    mov ds, ax
    ;assume ds:dseg
    assume ds:DGROUP

    call CRL_Restore_DSP
    
    call MD_CDraw_Restore

    pop di
    pop si
    pop es
    pop ds
    mov sp, bp
    pop bp
    ret

endp VGA_Copy_RSP_DSP_YM

;ends seg026

end
