TITLE _s33p32.asm VGA_SaveCursorArea

.MODEL LARGE, C

INCLUDE ST_GUI.INC

EXTRN g_DisplayScreenPage:BYTE

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_SaveCursorArea

proc VGA_SaveCursorArea far

    Width_Bytes = byte ptr -6
    Height_Lines = word ptr -4
    VGA_Offset = word ptr -2
    X_Pos = word ptr 6
    Y_Pos = word ptr 8

    push bp
    mov bp, sp
    sub sp, 6
    push si
    push di
    push es
    push ds
    mov ax, seg dseg
    mov ds, ax
    mov ax, ds
    mov es, ax
    assume es:dseg
    mov ax, [g_DisplayScreenPage]
    mov ah, al
    sub al, al                              ; (_AX & 0xFF00)
    shl ax, 1
    shl ax, 1                               ; (_AX << 2) == (_AX * 2^2) == (_AX * 4)
    add ax, 0A000h                          ; VGA - Graphics Mode - Video Memory Segment Address / Base
    mov ds, ax
    
    mov si, [bp+X_Pos]
    shr si, 1
    shr si, 1                               ; (_SI >> 2) == (_SI / 2^2) == (_SI / 4)
    mov ax, si
    mov bx, 3
    add ax, bx                              ; 
    sub ax, 50h                             ; _AX = (_AX - 80)  SCREEN_WIDTH
    js short loc_24831                      ; Jump If (80 > _AX)  Jump near if sign (SF=1)

    sub bx, ax                              ; _BX = (_BX - _AX) ... But, we just made _AX be negative, so _BX = (_BX + _AX)

loc_24831:
    mov ax, [bp+Y_Pos]
    mov cx, 50h                             ; SCREEN_WIDTH
    mul cx                                  ; _AX = (_AX * _CX)
    add si, ax
    sub di, di
    mov [word ptr es:Cursor_Save_Main.VGA_Offset+di], si
    add di, 2
    mov es:Cursor_Save_Main[di], bx
    add di, 2
    mov [bp+Width_Bytes], bl
    mov ax, [bp+Y_Pos]
    mov bx, 10h
    add ax, bx
    sub ax, 0C8h
    js short loc_2485F
    sub bx, ax
loc_2485F:
    mov es:Cursor_Save_Main[di], bx
    add di, 2
    mov [bp+Height_Lines], bx
    mov [bp+VGA_Offset], si
    sub bl, bl
    mov al, 4
    mov dx, 3CEh                            ; GC_INDEX
    out dx, al
    inc dx                                  ; GC_DATA
loc_24876:
    mov al, bl
    out dx, al
    sub bh, bh
loc_2487B:
    mov cx, [bp+Height_Lines]
loc_2487E:
    lodsw
    mov word ptr es:Cursor_Save_Main.Saved_Image[di-6], ax
    add di, 2
    add si, 4Eh
    loop loc_2487E
    mov si, [bp+VGA_Offset]
    inc bh
    sub ah, ah
    mov al, bh
    shl ax, 1
    add si, ax
    cmp bh, [bp+Width_Bytes]
    jnz short loc_2487B
    inc bl
    mov si, [bp+VGA_Offset]
    cmp bl, 4
    jnz short loc_24876

    pop ds
    pop es
    pop di
    pop si
    mov sp, bp
    pop bp
    retf

endp VGA_SaveCursorArea

;ends seg033

end
