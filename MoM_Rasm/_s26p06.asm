TITLE _s26p06.asm VGA_Copy_RSP_DSP_YM
; ST_VGA.H

.MODEL LARGE, C

EXTRN g_RenderScreenPage:WORD

EXTRN MOUSE_CDraw_Restore:PROC
EXTRN MOUSE_Disable_CDraw:PROC
EXTRN MOUSE_GetX:PROC
EXTRN MOUSE_GetY:PROC
EXTRN MOUSE_MoveCursor:PROC
EXTRN GUI_FindWindow:PROC
EXTRN GUI_DrawCursor_RSP:PROC
EXTRN GUI_SaveCursorArea_RSP:PROC
EXTRN GUI_RestoreCursorArea_RSP:PROC
EXTRN GUI_RestoreCursorArea_DSP:PROC

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
    tmp_RenderScreenPage_SgmtAddr  = word ptr -2

    push bp
    mov  bp, sp
    sub  sp, 0Ah
    push ds
    push es
    push si
    push di

    mov ax, ds
    mov [bp+Data_Seg], ax

    call MOUSE_Disable_CDraw

    call MOUSE_GetX
    mov [bp+Left], ax

    call MOUSE_GetY
    mov [bp+Top], ax

    mov ax, [g_RenderScreenPage]
    mov ah, al
    xor al, al
    shl ax, 1
    shl ax, 1
    add ax, 0A000h
    mov ds, ax
    ;assume ds:nothing

    mov [bp+tmp_RenderScreenPage_SgmtAddr], ax

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

    call GUI_RestoreCursorArea_DSP

    call MOUSE_GetY
    push ax
    call MOUSE_GetX
    cmp ax, [bp+Left]
    jnz short loc_1E5CB
    pop bx
    cmp bx, [bp+Top]
    jz short loc_1E5ED
    push bx

loc_1E5CB:
    push ax
    call GUI_FindWindow
    call GUI_RestoreCursorArea_RSP
    call GUI_SaveCursorArea_RSP
    call GUI_FindWindow
    call GUI_DrawCursor_RSP
    call MOUSE_MoveCursor
    add sp, 4

loc_1E5ED:
    mov ds, [bp+tmp_RenderScreenPage_SgmtAddr]
    cmp si, 3E80h
    jb short loc_1E58D

    mov ax, [bp+Data_Seg]
    mov ds, ax
    ;assume ds:dseg
    assume ds:DGROUP

    call GUI_RestoreCursorArea_DSP
    
    call MOUSE_CDraw_Restore

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
