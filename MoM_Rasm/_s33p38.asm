TITLE _s33p38.asm CRH_Draw_DSP

.MODEL LARGE, C

EXTRN g_CRSR_Curr:WORD

EXTRN CRL_Draw_DSP:PROC

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC CRH_Draw_DSP

proc CRH_Draw_DSP

    X_Pos = word ptr 6
    Y_Pos = word ptr 8

    push bp
    mov  bp, sp
    push si
    push di
    push es
    push ds

    ;mov ax, seg dseg
    mov ax, seg DGROUP
    mov ds, ax
    ;assume ds:dseg
    assume ds:DGROUP

    mov dx, [g_CRSR_Curr]
    cmp dx, 0  ; e_Crsr_None
    jz short @@Done

    mov bx, [bp+X_Pos]
    mov cx, [bp+Y_Pos]
    ;push cs
    ;call near ptr CRL_Draw_DSP
    ;nop
    call CRL_Draw_DSP

@@Done:
    pop ds
    pop es
    pop di
    pop si
    pop bp
    ret

endp CRH_Draw_DSP
;ends seg033

end
