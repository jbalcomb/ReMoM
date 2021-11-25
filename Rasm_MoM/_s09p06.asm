TITLE _s09p.asm lbx_read_ofst

.MODEL LARGE, C

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg009 byte public 'CODE' use16
;    assume cs:seg009
;    ;org 2
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC lbx_read_ofst

proc lbx_read_ofst far

    buffer  = word ptr 6
    nbytes  = word ptr 8
    fhandle = word ptr 0Ah

    push bp
    mov bp, sp
    push si
    push di
    push es
    push ds
    mov ah, 3Fh
    mov bx, [bp+fhandle]
    mov cx, [bp+nbytes]
    mov dx, [bp+buffer]
    int 21h

    jb @@Error

    mov ax, 0FFFFh
    pop ds
    pop es
    pop di
    pop si
    pop bp
    ret

@@Error:
    mov ax, 0
    pop ds
    pop es
    pop di
    pop si
    pop bp
    ret

endp lbx_read_ofst

;ends seg009

end
