TITLE _s08p14.asm SA_Alloc_Next

.MODEL LARGE, C

.CODE
;segment seg008 byte public 'CODE' use16
;    assume cs:seg008
;    ;org 7
;    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC SA_Alloc_Next

proc SA_Alloc_Next far

    subSAMB = word ptr -2
    SAMB    = word ptr 6
    nparas  = word ptr 8

    push bp
    mov  bp, sp
    sub  sp, 2
    push si
    push di

    mov si, [bp+SAMB]
    mov di, [bp+nparas]
    mov ax, di
    inc ax
    mov di, ax
    push si
    call SA_Alloc_Validate
    pop cx
    or ax, ax
    jnz short loc_14B22
    push di
    mov ax, Alloc_Space_corrupted
    push ax
    nop
    push cs
    call near ptr SA_Alloc_Error
    pop cx
    pop cx
loc_14B22:
    push si
    nop
    push cs
    call near ptr SA_GetFree
    pop cx
    cmp ax, di
    jnb short loc_14B44
    push si
    nop
    push cs
    call near ptr SA_GetFree
    pop cx
    mov dx, di
    sub dx, ax
    push dx
    mov ax, Alloc_Next_too_small
    push ax
    nop
    push cs
    call near ptr SA_Alloc_Error
    pop cx
    pop cx
loc_14B44:
    mov ax, s_SAMB.Used_Paras
    push ax
    push si
    call farpeekw
    pop cx
    pop cx
    add ax, si
    mov [bp+subSAMB], ax
    mov ax, s_SAMB.Used_Paras
    push ax
    push si
    call farpeekw
    pop cx
    pop cx
    add ax, di
    push ax
    mov ax, s_SAMB.Used_Paras
    push ax
    push si
    call farpokew
    add sp, 6
    mov ax, LBX_MemSig1
    push ax
    mov ax, s_SAMB.MemSig1
    push ax
    push [bp+subSAMB]
    call farpokew
    add sp, 6
    mov ax, LBX_MemSig2
    push ax
    mov ax, s_SAMB.MemSig2
    push ax
    push [bp+subSAMB]
    call farpokew
    add sp, 6
    mov ax, di
    dec ax
    push ax
    mov ax, s_SAMB.Size_Paras
    push ax
    push [bp+subSAMB]
    call farpokew
    add sp, 6
    mov ax, 1
    push ax
    mov ax, s_SAMB.Used_Paras
    push ax
    push [bp+subSAMB]
    call farpokew
    add sp, 6
    mov ax, [bp+subSAMB]
    inc ax

    jmp short $+2
    pop di
    pop si
    mov sp, bp
    pop bp
    ret

endp SA_Alloc_Next

;ends seg008

end
