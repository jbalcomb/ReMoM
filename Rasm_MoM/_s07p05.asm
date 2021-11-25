TITLE _s07p.asm 

.MODEL LARGE, C

.CODE
;segment seg007 byte public 'CODE' use16
;assume cs:seg007
;;org 0Ah
;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC farpeekb

proc farpeekb far

    Sgmt = word ptr 6
    Ofst = word ptr 8

    push bp
    mov bp, sp
    push ds
    push si

    mov si, [bp+Ofst]
    mov ax, [bp+Sgmt]
    mov ds, ax
    lodsb
    xor ah, ah

    pop si
    pop ds
    pop bp
    ret

endp farpeekb

;ends seg007

end
