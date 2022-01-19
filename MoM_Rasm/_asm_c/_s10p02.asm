TITLE _s10p02.asm LBXE_LoadReplace

.MODEL LARGE, C

.CODE
; segment seg010 byte public 'CODE' use16
; assume cs:seg010
; ;org 6
; ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC LBXE_LoadReplace

proc LBXE_LoadReplace

    FileName@= word ptr 6
    Entry_Index= word ptr 8
    LBX_Alloc_Seg= word ptr 0Ah

    push bp
    mov bp, sp
    xor ax, ax
    push ax
    mov ax, 1
    push ax
    push [bp+LBX_Alloc_Seg]
    push [bp+Entry_Index]
    push [bp+FileName@]
    nop
    push cs
    call near ptr LBX_EntryLoader
    add sp, 0Ah
    pop bp
    ret

endp LBXE_LoadReplace

;ends seg010

end
