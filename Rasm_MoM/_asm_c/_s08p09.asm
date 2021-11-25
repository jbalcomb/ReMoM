TITLE _s08p09.asm AllocMem_EntrySizeParas

.MODEL LARGE, C

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg008 byte public 'CODE' use16
;    assume cs:seg008
;    ;org 7
;    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC AllocMem_EntrySizeParas

proc AllocMem_EntrySizeParas far

    sgmt_addr = word ptr -2
    nparas    = word ptr 6

    push bp
    mov bp, sp
    sub sp, 2

    mov ax, [bp+nparas]
    xor dx, dx
    mov cl, 4
    call LXLSH@

    add ax, 10h ; 16
    adc dx, 0
    push dx
    push ax
    call _farmalloc
    pop cx
    pop cx

    mov [gfp_SAMB+2], dx
    mov [gfp_SAMB], ax
    mov ax, [gfp_SAMB]
    or ax, [gfp_SAMB+2]
    jnz @@Success

@@Failure
    xor ax, ax

@@JmpDone:
    jmp @@Done

@@Success:
    mov ax, [gfp_SAMB+2]
    inc ax
    mov [bp+sgmt_addr], ax

    call RAM_Update_WorstFree

    mov ax, [bp+sgmt_addr]
    jmp @@JmpDone

@@Done:
    mov sp, bp
    pop bp
    ret

endp AllocMem_EntrySizeParas

;ends seg008

end
