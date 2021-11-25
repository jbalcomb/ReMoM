TITLE _s13p31.asm EMM_SetMinKB

.MODEL LARGE, C

EXTRN g_EMM_MinKB:WORD ; NOTE: ONLY Xref'd from EMM_SetMinKB() and EMM_GetLowString()

.CODE
;segment seg013 byte public 'CODE' use16
;    assume cs:seg013
;    ;org 7
;    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC EMM_SetMinKB

; ? sets the global variable 'EMM_MinKBytes' ?
; ? ST programming paradigm ?
; NOTE: ONLY called from main()
proc EMM_SetMinKB far

    varEMM_MinKB = word ptr 6

    push bp
    mov bp, sp

    mov ax, [bp+varEMM_MinKB]
    mov [g_EMM_MinKB], ax

    pop bp
    ret

endp EMM_SetMinKB

;ends seg013

end
