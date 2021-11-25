TITLE _s12p08.asm EMM_MapnRead
; https://stackoverflow.com/questions/4976636/whats-the-purpose-of-the-rotate-instructions-rol-rcl-on-x86
; a good C compiler will emit rol opcodes when compiling code which tries to compute a rotation with the C operators (i.e. (x << 12) | (x >> 20)).
; << and >> are shifts. But for a 32-bit value x, the whole expression (x << 12) | (x >> 20), consisting of two shifts (one left, one right) and a bitwise OR, has the same effect than a rotation of a 32-bit word (here, by 12 bits to the left).
;
; ~= movedata(), but for EMM
; void movedata(unsigned srcseg, unsigned srcoff, unsigned dstseg, unsigned dstoff, size_t n);

.MODEL LARGE, C

EXTRN g_EMM_PageFrame_Base_Address:WORD

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg012 byte public 'CODE' use16
;    assume cs:seg012
;    ;org 6
;    ;assume es:nothing, ss:nothing, ds:dseg
; seg012:0006  EMM_Logical_Page dw 0                   ; DATA XREF: EMM_MapnRead, EMM_MapnWrite
EMM_Logical_Page dw 0

PUBLIC EMM_MapnRead

proc EMM_MapnRead far

    Dst_Ofst    = word ptr 6
    Dst_Sgmt    = word ptr 8
    Src_Ofst    = word ptr 0Ah
    Src_Sgmt    = word ptr 0Ch
    nbytes      = word ptr 0Eh
    EMM_Handle  = word ptr 10h

    push bp
    mov bp, sp
    sub sp, 2
    push es
    push ds
    push si
    push di

    cmp [bp+Dst_Sgmt], 0
    jnz @@DotIt

    mov ax, ds
    mov [bp+Dst_Sgmt], ax

@@DotIt:
    ;
    ; Math to translate 64K to 16K Logical Page Size?
    ;
    mov ax, [word ptr bp+Src_Ofst]
    mov dx, [word ptr bp+Src_Sgmt]
    shl ax, 1                           ; Shift Left - Multiply r/m16 by 2, once
    rcl dx, 1                           ; Rotate Bits Left with CF
    shl ax, 1
    rcl dx, 1
    mov bx, dx
    mov [cs:EMM_Logical_Page], bx
    shr ax, 1
    shr ax, 1
    mov si, ax
    mov ax, 4400h
    mov dx, [bp+EMM_Handle]
    int 67h

    mov ax, 4401h
    mov bx, [cs:EMM_Logical_Page]
    inc bx
    mov dx, [bp+EMM_Handle]
    int 67h

    mov ax, 4402h
    mov bx, [cs:EMM_Logical_Page]
    inc bx
    inc bx
    mov dx, [bp+EMM_Handle]
    int 67h

    mov ax, 4403h
    mov bx, [cs:EMM_Logical_Page]
    inc bx
    inc bx
    inc bx
    mov dx, [bp+EMM_Handle]
    int 67h

    mov ax, [bp+Dst_Sgmt]
    mov es, ax
    mov di, [bp+Dst_Ofst]
    mov cx, [bp+nbytes]
    mov ax, [g_EMM_PageFrame_Base_Address]
    mov ds, ax
    rep movsb                           ; Move CX bytes from DS:[SI] to ES:[DI]

    pop di
    pop si
    pop ds
    pop es
    mov sp, bp
    pop bp
    ret

endp EMM_MapnRead

;ends seg012

end
