TITLE _s12p10.asm EMM_GetPageFrame
; ST_EMM.H

.MODEL LARGE, C

EXTRN g_EMM_PageFrame_Base_Address:WORD

.CODE
;segment seg012 byte public 'CODE' use16
;    assume cs:seg012
;    ;org 6
;    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC EMM_GetPageFrame

proc EMM_GetPageFrame

    push ds

    ;mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax

    mov ax, [g_EMM_PageFrame_Base_Address]

    pop ds
    ret

endp EMM_GetPageFrame

;ends seg012

end
