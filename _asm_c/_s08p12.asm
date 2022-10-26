TITLE _s08p12.asm SA_Alloc_First

.MODEL LARGE, C

.CODE
;segment seg008 byte public 'CODE' use16
;    assume cs:seg008
;    ;org 7
;    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC SA_Alloc_First

proc SA_Alloc_First

    paras_total = word ptr -2
    SAMB        = word ptr 6
    nparas      = word ptr 8

    push bp
    mov  bp, sp
    sub  sp, 2
    push si
    push di

    mov si, [bp+nparas]
    mov ax, si
    inc ax
    mov si, ax
    mov ax, [bp+SAMB]
    inc ax
    mov di, ax
    mov ax, s_SAMB.Size_Paras
    push ax
    push [bp+SAMB]
    call farpeekw
    pop cx
    pop cx
    mov [bp+paras_total], ax
    push [bp+SAMB]
    call SA_Alloc_Validate
    pop cx
    or ax, ax
    jnz short loc_14A3A
    push si
    mov ax, Alloc_Space_corrupted
    push ax
    nop
    push cs
    call near ptr SA_Alloc_Error
    pop cx
    pop cx
loc_14A3A:
    cmp [bp+paras_total], si
    jnb short loc_14A50
    mov ax, si
    sub ax, [bp+paras_total]
    push ax
    mov ax, Alloc_Next_too_small
    push ax
    nop
    push cs
    call near ptr SA_Alloc_Error
    pop cx
    pop cx
loc_14A50:
    mov ax, si
    inc ax
    push ax
    mov ax, s_SAMB.Used_Paras
    push ax
    push [bp+SAMB]
    call farpokew
    add sp, 6
    mov ax, LBX_MemSig1
    push ax
    mov ax, s_SAMB.MemSig1
    push ax
    push di
    call farpokew
    add sp, 6
    mov ax, LBX_MemSig2
    push ax
    mov ax, s_SAMB.MemSig2
    push ax
    push di
    call farpokew
    add sp, 6
    mov ax, si
    dec ax
    push ax
    mov ax, s_SAMB.Size_Paras
    push ax
    push di
    call farpokew
    add sp, 6
    mov ax, 1
    push ax
    mov ax, s_SAMB.Used_Paras
    push ax
    push di
    call farpokew
    add sp, 6
    mov ax, 1
    push ax
    mov ax, s_SAMB.Marked_Paras
    push ax
    push di
    call farpokew
    add sp, 6
    mov ax, di
    inc ax
    
    jmp short $+2
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp SA_Alloc_First

;ends seg008

end
