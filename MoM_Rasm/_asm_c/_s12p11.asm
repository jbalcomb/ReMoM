TITLE _s12p11.asm EMM_MapFourPages
; ST_EMM.H

.MODEL LARGE, C

.CODE
;segment seg012 byte public 'CODE' use16
;    assume cs:seg012
;    ;org 6
;    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC EMM_MapFourPages

proc EMM_MapFourPages

    argEmmHandle = word ptr 6
    argFirstLogicalPage = word ptr 8

    push bp
    mov  bp, sp
    push es
    push ds

    mov ax, 4400h                           ; AH - Map Memory; AL - Physical Page Number
    mov dx, [bp+argEmmHandle]               ; DX - EMM Handle Number
    mov bx, [bp+argFirstLogicalPage]        ; BX - Logical Page Number
    int 67h                                 ; LIM EMS Interrupt (ISR)

    mov ax, 4401h
    mov bx, [bp+argFirstLogicalPage]
    inc bx
    mov dx, [bp+argEmmHandle]
    int 67h

    mov ax, 4402h
    mov bx, [bp+argFirstLogicalPage]
    inc bx
    inc bx
    mov dx, [bp+argEmmHandle]
    int 67h

    mov ax, 4403h
    mov bx, [bp+argFirstLogicalPage]
    inc bx
    inc bx
    inc bx
    mov dx, [bp+argEmmHandle]
    int 67h

    pop ds
    pop es
    pop bp
    ret

endp EMM_MapFourPages

;ends seg012

end
