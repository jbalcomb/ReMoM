TITLE _s08p.asm SA_MK_FP0 MK_FAR

.MODEL LARGE, C

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg008 byte public 'CODE' use16
;   assume cs:seg008
;   ;org 7
;   ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC SA_MK_FP0

; int far *__cdecl __far MK_FAR(unsigned int sgmt_addr)
proc SA_MK_FP0 far

    tmp_sgmt_addr = word ptr -6
    fptr_ofst = word ptr -4
    fptr_sgmt = word ptr -2
    sgmt_addr = word ptr 6

    push bp
    mov bp, sp
    sub sp, 6

    mov ax, [bp+sgmt_addr]
    mov [bp+tmp_sgmt_addr], ax
    
    mov ax, [bp+tmp_sgmt_addr]
    mov [bp+fptr_sgmt], ax

    mov [bp+fptr_ofst], 0

    mov dx, [bp+fptr_sgmt]
    mov ax, [bp+fptr_ofst]

    jmp short $+2

    mov sp, bp
    pop bp
    ret

endp SA_MK_FP0

;ends seg008

end
