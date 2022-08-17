TITLE _s33p39.asm VGA_DrawCursor_RSP

.MODEL LARGE, C

EXTRN gsa_Cursor_Array:WORD
EXTRN g_RSP_Idx:WORD

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC CRL_Draw_RSP

; BX = X_Pos
; CX = Y_Pos
; DX = g_CRSR_Curr

proc CRL_Draw_RSP

    Draw_Height = word ptr -4
    gsa_Cursor = word ptr -2

    push bp
    mov  bp, sp
    sub  sp, 4
    push es
    push ds
    push si
    push di

    mov ax, seg DGROUP
    mov ds, ax
    assume ds:DGROUP

    cmp dx, 0
    jnz DoDrawCursor

    pop di
    pop si
    pop ds
    pop es
    mov sp, bp
    pop bp
    ret

DoDrawCursor:
    dec dx
    shl dx, 1
    shl dx, 1
    shl dx, 1
    shl dx, 1
    add dx, [gsa_Cursor_Array]
    mov [bp+gsa_Cursor], dx

    mov dx, cx  ; Y_Pos
    shl dx, 1
    shl dx, 1
    add dx, cx  ; (Y_Pos * 4) + Y_Pos ~== (Y_Pos * 5)
    mov ax, [g_RSP_Idx]
    mov ah, al
    xor al, al
    shl ax, 1
    shl ax, 1
    add ax, 0A000h  ; AX = {A000,A4000} + ? (Y_Pos * 5) ?
    add ax, dx
    mov es, ax

    mov ax, 0C8h  ; 200d  SCREEN_HEIGHT_PIXELS
    sub ax, cx
    jbe @@Done

    cmp ax, 11h  ; 17d
    js SetHeightCalcMask

    mov ax, 10h

SetHeightCalcMask:
    mov [bp+Draw_Height], ax
    mov ax, bx
    mov ah, 1
    and al, 3
    jz RegMask_RowOffset_ClipHeight

    mov cl, al
    xor ch, ch

ShiftMaskBitIntoPosition:
    shl ah, 1
    loop ShiftMaskBitIntoPosition

RegMask_RowOffset_ClipHeight:
    mov dx, 3C4h ;SC_INDEX
    mov al, 2 ;e_SC_MAPMASK
    out dx, al

    mov dx, bx
    shr dx, 1
    shr dx, 1

    mov cx, 140h  ; 320d
    sub cx, bx
    jbe @@Done

    cmp cx, 11h  ; 17d
    js ItrLine_SrcSgmtOfst

    mov cx, 10h  ; 16d

ItrLine_SrcSgmtOfst:
    mov bl, cl
    mov bh, byte ptr [bp+Draw_Height]

    mov si, 0

    mov cx, [bp+gsa_Cursor]
    mov ds, cx

LoopPlanes:
    mov di, dx
    mov dx, 3C5h ;SC_DATA
    mov al, ah
    out dx, al
    
    mov dx, di
    mov cl, bh
    xor ch, ch

LoopLines:
    lodsb
    cmp al, 0
    jnz WritePixel

SkipPixel:
    add di, 50h  ; 80d  line width stride
    loop LoopLines

JmpNextBitmapLine:
    jmp NextBitmapLine
    ; ? nop | align 2 ?

WritePixel:
    stosb
    add di, 4Fh  ; 79d  (1 + 79 = 80)
    loop LoopLines

NextBitmapLine:
    mov cl, 10h  ; 16d
    sub cl, bh
    xor ch, ch
    add si, cx
    dec bl
    jz @@Done
    
    shl ah, 1
    cmp ah, 9
    js NextPlane

    inc dx
    mov ah, 1

NextPlane:
    jmp LoopPlanes

@@Done:
    pop di
    pop si
    pop ds
    pop es
    mov sp, bp
    pop bp
    ret

endp CRL_Draw_RSP

;ends seg033

end
