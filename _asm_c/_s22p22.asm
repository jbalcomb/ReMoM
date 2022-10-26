TITLE _s22p22.asm MEM_Copy_Far
; ST_DEF.H

.MODEL LARGE, C

.CODE
;segment seg022 byte public 'CODE' use16
;    assume cs:seg022
;    ;org 0Ah
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MEM_Copy_Far

proc MEM_Copy_Far

    fptr_Dst = dword ptr 6
    fptr_Src = dword ptr 0Ah
    nbytes = word ptr 0Eh

    push bp
    mov  bp, sp
    push si
    push di

    cmp [bp+nbytes], 0
    jnz short @@CheckDstSgmt

    xor ax, ax
    jmp short @@Done

@@CheckDstSgmt:
    cmp word ptr [bp+fptr_Dst+2], 0
    jnz short @@CheckSrcSgmt

    mov ax, ds
    mov word ptr [bp+fptr_Dst+2], ax

@@CheckSrcSgmt:
    cmp word ptr [bp+fptr_Src+2], 0
    jnz short @@CheckOddNbytes

    mov ax, ds
    mov word ptr [bp+fptr_Src+2], ax

@@CheckOddNbytes:
    test [bp+nbytes], 1
    jz short @@CopyEven

@@CopyOdd:
    push ds
    push es
    push si
    push di
    
    mov ax, word ptr [bp+fptr_Dst+2]
    mov es, ax
    mov di, word ptr [bp+fptr_Dst]
    mov bx, word ptr [bp+fptr_Src+2]
    mov si, word ptr [bp+fptr_Src]
    mov cx, [bp+nbytes]
    mov ds, bx
    rep movsb

    pop di
    pop si
    pop es
    pop ds
    jmp short @@Done

@@CopyEven:
    push ds
    push es
    push si
    push di

    mov ax, word ptr [bp+fptr_Dst+2]
    mov es, ax
    mov di, word ptr [bp+fptr_Dst]
    mov bx, word ptr [bp+fptr_Src+2]
    mov si, word ptr [bp+fptr_Src]
    mov cx, [bp+nbytes]
    shr cx, 1
    mov ds, bx
    rep movsw

    pop di
    pop si
    pop es
    pop ds

@@Done:
    pop di
    pop si
    pop bp
    ret

endp MEM_Copy_Far

;ends seg022

end
