TITLE _s08p07.asm LBX_Alloc_Space

.MODEL LARGE

INCLUDE ST_LBX.INC

EXTRN LBX_MemSig1:WORD
EXTRN LBX_MemSig2:WORD
EXTRN LBX_Alloc_Offset:WORD
EXTRN LBX_Alloc_Segment:WORD
EXTRN LBX_Alloc_Space_Header:ST_Alloc_Space_Header_STRUC

EXTRN LXLSH:PROC
EXTRN farmalloc:PROC
EXTRN farpokew:PROC
EXTRN LBX_Alloc_Error:PROC
EXTRN RAM_Update_WorstFreeBlock_KB:PROC

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg008 byte public 'CODE' use16
;   assume cs:seg008
;   ;org 7
;   assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC LBX_Alloc_Space

proc LBX_Alloc_Space far

    segp = word ptr -2
    size = word ptr 6

    push bp
    mov bp, sp
    sub sp, 2
    push si
    mov si, [bp+size]
    mov ax, si
    xor dx, dx
    mov cl, 4
    call LXLSH

    add ax, 10h
    adc dx, 0
    push dx
    push ax
    call farmalloc
    pop cx
    pop cx

    mov [LBX_Alloc_Segment], dx
    mov [LBX_Alloc_Offset], ax
    mov ax, [LBX_Alloc_Offset]
    or ax, [LBX_Alloc_Segment]
    jnz short @@FptrNotNull

@@Error:
    push si
    mov ax, 01h ; enum Alloc_too_small
    push ax
    ;nop
    ;push cs
    call LBX_Alloc_Error
    add sp, 4

@@FptrNotNull:
    mov ax, [LBX_Alloc_Segment]
    inc ax
    mov [bp+segp], ax

    mov ax, LBX_MemSig1
    push ax
    mov ax, LBX_Alloc_Space_Header.MemSig1
    push ax
    push [bp+segp]
    call farpokew
    add sp, 6

    mov ax, LBX_MemSig2
    push ax
    mov ax, LBX_Alloc_Space_Header.MemSig2
    push ax
    push [bp+segp]
    call farpokew
    add sp, 6

    push si
    mov ax, LBX_Alloc_Space_Header.Size_Paras
    push ax
    push [bp+segp]
    call farpokew
    add sp, 6

    mov ax, 1
    push ax
    mov ax, LBX_Alloc_Space_Header.Used_Paras
    push ax
    push [bp+segp]
    call farpokew
    add sp, 6

    call RAM_Update_WorstFreeBlock_KB

    mov ax, [bp+segp]
    ;jmp short $+2

    pop si
    mov sp, bp
    pop bp
    ret

endp LBX_Alloc_Space

;ends seg008

end
