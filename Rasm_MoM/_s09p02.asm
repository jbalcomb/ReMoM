TITLE _s09p02.asm lbx_close

.MODEL LARGE, C

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg009 byte public 'CODE' use16
;    assume cs:seg009
;    ;org 2
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC lbx_close

proc lbx_close far

    fhandle = word ptr 6

    push bp
    mov  bp, sp
    push si
    push di
    push es
    push ds
    
    mov ah, 3Eh
    mov bx, [bp+fhandle]
    int 21h

    pop ds
    pop es
    pop di
    pop si
    pop bp
    ret

endp lbx_close

;ends seg009

end
