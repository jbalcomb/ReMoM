TITLE _s12p10.asm EMM_GetPageFrame

.MODEL LARGE, C

EXTRN g_EMM_PageFrame_Base_Address:WORD

.CODE
;segment seg012 byte public 'CODE' use16
;    assume cs:seg012
;    ;org 6
;    ;assume es:nothing, ss:nothing, ds:dseg

proc EMM_GetPageFrame far

    push ds

    mov ax, seg dseg
    mov ds, ax
    mov ax, [g_EMM_PageFrame_Base_Address]

    pop ds
    ret

endp EMM_GetPageFrame

;ends seg012

end
