TITLE _s08p15.asm LBX_Alloc_GetFree

.MODEL LARGE, C

EXTRN farpeekw:PROC

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg008 byte public 'CODE' use16
;    assume cs:seg008
;    ;org 7
;    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC LBX_Alloc_GetFree

proc LBX_Alloc_GetFree far

    LBX_Free_Paras  = word ptr -2
    LBX_Alloc_Seg   = word ptr 6

    push bp
    mov  bp, sp
    sub  sp, 2
    push si

    mov si, [bp+LBX_Alloc_Seg]
    ;mov ax, LBX_Alloc_Space_Header.Size_Paras
    mov ax, 08h
    push ax
    push si
    call farpeekw
    pop cx
    pop cx

    push ax
    ;mov ax, LBX_Alloc_Space_Header.Used_Paras
    mov ax, 0Ah
    push ax
    push si
    call farpeekw
    pop cx
    pop cx

loc_14BEC:
    pop dx
    sub dx, ax
    mov [bp+LBX_Free_Paras], dx
    mov ax, [bp+LBX_Free_Paras]
    jmp short $+2

    pop si
    mov sp, bp
    pop bp
    ret

endp LBX_Alloc_GetFree

;ends seg008

end
