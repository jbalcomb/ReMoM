TITLE _s07p04.asm LBX_Alloc_Validate

.MODEL LARGE, C

EXTRN LBX_Alloc_Space_Header:s_LBX_Alloc_Space_Header
EXTRN LBX_MemSig1:WORD
EXTRN LBX_MemSig2:WORD

.CODE
;segment seg007 byte public 'CODE' use16
;assume cs:seg007
;;org 0Ah
;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC LBX_Alloc_Validate

proc LBX_Alloc_Validate far

    LBX_Alloc_Seg = word ptr 6

    push bp
    mov bp, sp
    push ds
    push si

    mov ax, [bp+LBX_Alloc_Seg]
    mov ds, ax
    mov si, LBX_Alloc_Space_Header.MemSig1
    
    lodsw
    cmp ax, LBX_MemSig1
    jnz short @@error

    lodsw
    cmp ax, LBX_MemSig2
    jnz short @@error

    mov ax, 0FFFFh
    pop si
    pop ds
    pop bp
    ret

@@error:
    mov ax, 0
    pop si
    pop ds
    pop bp
    ret

endp LBX_Alloc_Validate

;ends seg007

end
