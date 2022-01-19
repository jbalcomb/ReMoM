TITLE _s02p05.asm VGA_WaitSync
; MOM_DEF.H

.MODEL LARGE, C

.CODE
;segment seg002 byte public 'CODE' use16
;    assume cs:seg002
;    ;org 2
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_WaitSync

proc VGA_WaitSync

    push es

    mov dx, 3DAh ; e_INPUT_STATUS

@@waitnosync:
    in al, dx
    and al, 08h ; e_VRETRACE
    jnz @@waitnosync
    
@@waitsync:
    in al, dx
    and al, 08h ; e_VRETRACE
    jz @@waitsync

    pop es
    ret

endp VGA_WaitSync

;ends seg002

end
