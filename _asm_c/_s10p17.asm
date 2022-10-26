TITLE _s10p17.asm RAM_SetMinKB

.MODEL LARGE

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg010 byte public 'CODE' use16
;    assume cs:seg010
;    ;org 6
;    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing


proc RAM_SetMinKB far

    RAM_MinKbytes = word ptr 6

    push bp
    mov bp, sp
    mov ax, [bp+RAM_MinKbytes]
    mov [RAM_MinKbytes], ax
    pop bp
    ret

endp RAM_SetRAM_SetMinKBMinKbytes

;ends seg010

end
