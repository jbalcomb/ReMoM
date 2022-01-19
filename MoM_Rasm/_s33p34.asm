TITLE _s33p.asm GUI_RestoreCursorArea_RSP
; ST_GUI.H

.MODEL LARGE, C

EXTRN g_GUI_PrevCursor:WORD
EXTRN g_Cursor_Save_Main:WORD
EXTRN g_RenderScreenPage:WORD

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_RestoreCursorArea_RSP

proc GUI_RestoreCursorArea_RSP

    Width_Bytes = byte ptr -6
    Height_Lines = word ptr -4
    ScreenPage_Offset = word ptr -2

    push bp
    mov  bp, sp
    sub  sp, 6
    push si
    push di
    push es
    push ds

;    mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax
    ;assume ds:dseg
    assume ds:DGROUP

    cmp [g_GUI_PrevCursor], 0
    jnz short @@HavePrevCursor

    pop ds
    pop es
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

@@HavePrevCursor:
    sub si, si
;    mov di, [word ptr g_Cursor_Save_Main.ScreenPage_Offset+si]
    mov di, [word ptr g_Cursor_Save_Main+si]
    add si, 2
    mov [bp+ScreenPage_Offset], di
    mov ax, word ptr g_Cursor_Save_Main[si]
    add si, 2
    mov [bp+Width_Bytes], al
    mov ax, word ptr g_Cursor_Save_Main[si]
    add si, 2
    mov [bp+Height_Lines], ax

    mov ax, [g_RenderScreenPage]
    mov ah, al
    sub al, al
    shl ax, 1
    shl ax, 1
    add ax, 0A000h
    mov es, ax

    mov bl, 1
    mov dx, 3C4h
    mov al, 2
    out dx, al
    inc dx

@@LoopsPlanes:
    mov al, bl
    out dx, al
    sub bh, bh

@@LoopLines:
    mov cx, [bp+Height_Lines]

@@LoopWords:
    mov ax, word ptr g_Cursor_Save_Main[si]
    stosw
    add si, 2
    add di, 4Eh
    loop @@LoopWords

    mov di, [bp+ScreenPage_Offset]
    inc bh
    sub ah, ah
    mov al, bh
    shl ax, 1
    add di, ax
    cmp bh, [bp+Width_Bytes]
    jnz short @@LoopLines
    shl bl, 1
    mov di, [bp+ScreenPage_Offset]
    cmp bl, 10h
    jnz short @@LoopsPlanes

    pop ds
    pop es
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp GUI_RestoreCursorArea_RSP

;ends seg033

end
