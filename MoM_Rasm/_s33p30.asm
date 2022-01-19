TITLE _s33p30.asm GUI_SaveCursorArea_RSP

.MODEL LARGE, C

EXTRN g_GUI_CurrentCursor:WORD
EXTRN g_GUI_HaveAreaSave:WORD
EXTRN g_RenderScreenPage:WORD
EXTRN g_Cursor_Save_Main:WORD

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC GUI_SaveCursorArea_RSP

proc GUI_SaveCursorArea_RSP

    Width_Bytes  = byte ptr -6
    Height_Lines = word ptr -4
    ScreenPage_Offset   = word ptr -2
    X_Pos        = word ptr 6
    Y_Pos        = word ptr 8

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

    cmp [g_GUI_CurrentCursor], 0
    jnz short loc_24673

    cmp [g_GUI_HaveAreaSave], 0
    jz short loc_2466D

    pop ds
    pop es
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

loc_2466D:
    mov [g_GUI_HaveAreaSave], 1
    
loc_24673:
    mov ax, ds
    ;assume ds:dseg
    assume ds:DGROUP
    mov es, ax
    ;assume es:dseg
    assume es:DGROUP

    mov ax, [g_RenderScreenPage]
    mov ah, al
    sub al, al
    shl ax, 1
    shl ax, 1
    add ax, 0A000h
    mov ds, ax
    mov si, [bp+X_Pos]
    shr si, 1
    shr si, 1
    mov ax, si
    mov bx, 3
    add ax, bx
    sub ax, 50h
    js short loc_2469C
    sub bx, ax

loc_2469C:
    mov ax, [bp+Y_Pos]
    mov cx, 50h
    mul cx
    add si, ax
    sub di, di
;    mov [word ptr es:g_Cursor_Save_Main.ScreenPage_Offset+di], si
    mov [word ptr es:g_Cursor_Save_Main+di], si
    add di, 2
    mov es:g_Cursor_Save_Main[di], bx
    add di, 2
    mov [bp+Width_Bytes], bl
    mov ax, [bp+Y_Pos]
    mov bx, 10h
    add ax, bx
    sub ax, 0C8h
    js short loc_246CA
    sub bx, ax

loc_246CA:
    mov es:g_Cursor_Save_Main[di], bx
    add di, 2
    mov [bp+Height_Lines], bx
    mov [bp+ScreenPage_Offset], si
    sub bl, bl
    mov al, 4
    mov dx, 3CEh
    out dx, al
    inc dx

@@LoopsPlanes:
    mov al, bl
    out dx, al
    sub bh, bh

@@LoopLines:
    mov cx, [bp+Height_Lines]

@@LoopWords:
    lodsw
    mov es:g_Cursor_Save_Main[di], ax
    add di, 2
    add si, 4Eh
    loop @@LoopWords

    mov si, [bp+ScreenPage_Offset]
    inc bh
    sub ah, ah
    mov al, bh
    shl ax, 1
    add si, ax
    cmp bh, [bp+Width_Bytes]
    jnz short @@LoopLines
    inc bl
    mov si, [bp+ScreenPage_Offset]
    cmp bl, 4
    jnz short @@LoopsPlanes

    pop ds
    pop es
    assume es:nothing
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp GUI_SaveCursorArea_RSP

;ends seg033

end
