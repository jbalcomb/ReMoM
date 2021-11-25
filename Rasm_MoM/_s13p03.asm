TITLE _s13p.asm EMM_LoadResLBX

.MODEL LARGE, C

.CODE
;segment seg013 byte public 'CODE' use16
;    assume cs:seg013
;    ;org 7
;    assume es:nothing, ss:nothing, ds:dseg

PUBLIC EMM_LoadResLBX

proc EMM_LoadResLBX far

    LBX_Name = word ptr 6

    push bp
    mov  bp, sp

    mov ax, 1
    push ax
    push [bp+LBX_Name]
    nop
    push cs
    call near ptr EMM_Load_LBX
    pop cx
    pop cx

    pop bp
    retf

endp EMM_LoadResLBX

;ends seg013

end
