TITLE _s12p07.asm EMM_ReleaseHandle

.MODEL LARGE, C

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg012 byte public 'CODE' use16
;    assume cs:seg012
;    ;org 6
;    ;assume es:nothing, ss:nothing, ds:dseg

proc EMM_ReleaseHandle far

    EMM_Handle = word ptr 6

    push bp
    mov bp, sp
    push es
    push ds
    push si
    push di

    cmp [bp+EMM_Handle], 0
    jz short loc_162B2

    mov ah, 45h
    mov dx, [bp+EMM_Handle]
    int 67h

loc_162B2:
    pop di
    pop si
    pop ds
    pop es
    pop bp
    ret

endp EMM_ReleaseHandle

;ends seg012

end
