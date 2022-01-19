TITLE _s07p07.asm farpeekdw

.MODEL LARGE, C

.CODE
;segment seg007 byte public 'CODE' use16
;assume cs:seg007
;;org 0Ah
;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC farpeekdw

proc farpeekdw far

    sgmt = word ptr 6
    ofst = word ptr 8

    push bp
    mov  bp, sp
    push ds
    push si

    mov si, [bp+ofst]
    mov ax, [bp+sgmt]
    mov ds, ax
    lodsw
    mov bx, ax
    lodsw
    mov dx, ax
    mov ax, bx

    pop si
    pop ds
    pop bp
    ret

endp farpeekdw

;ends seg007

end
