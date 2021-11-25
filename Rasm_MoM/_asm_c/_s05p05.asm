TITLE _s05p05.asm RAM_Update_WorstFree

.MODEL LARGE, C

EXTRN RAM_GetFreeBlockSize:PROC

segment dseg para public 'DATA' use16
ends dseg


segment seg005 byte public 'CODE' use16
    assume cs:seg005
    ;org 0Eh
    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing


proc RAM_Update_WorstFree far

    push bp
    mov bp, sp
    push si

    call RAM_GetFreeBlockSize

    mov si, ax
    cmp si, [RAM_WorstFreeBlocks]
    jnb short @@NotNewWorst

    mov [RAM_WorstFreeBlocks], si

    @@NotNewWorst:
    pop si
    pop bp
    ret

endp RAM_Update_WorstFree

ends seg005

end
