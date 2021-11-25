; _s24p06.asm DBG_IsDisabled

.MODEL LARGE, C

.CODE
;segment    seg024 byte public 'CODE' use16
;    assume cs:seg024
;    ;org 2
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC DBG_IsDisabled

proc DBG_IsDisabled far

    push bp
    mov  bp, sp

    mov ax, [DBG_Disabled]
    jmp short $+2

    pop    bp
    ret

endp DBG_IsDisabled

;ends seg024

end
