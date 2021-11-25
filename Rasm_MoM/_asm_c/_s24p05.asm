; _s24p05.asm DBG_Disable

.MODEL LARGE, C

.CODE
;segment    seg024 byte public 'CODE' use16
;    assume cs:seg024
;    ;org 2
;    assume es:nothing, ss:nothing, ds:dseg,    fs:nothing, gs:nothing

PUBLIC DBG_Disable

proc DBG_Disable far

    push bp
    mov  bp, sp

    mov [DBG_Disabled], 1

    pop bp
    ret

endp DBG_Disable

;ends seg024

end
