TITLE _s09p01.asm lbx_open

.MODEL LARGE, C

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg009 byte public 'CODE' use16
;    assume cs:seg009
;    ;org 2
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC lbx_open

proc lbx_open far

    fname = word ptr 6

    push bp
    mov  bp, sp
    push si
    push di
    push es
    push ds
    
    mov  ax, 3D00h
    mov  dx, [bp+fname]
    int  21h

    jb @@Error

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

endp lbx_open

;ends seg009

end
