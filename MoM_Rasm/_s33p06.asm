TITLE _s33p06.asm MD_WaitRelease
; in ST_GUI.H

.MODEL LARGE, C

.CODE
;segment seg033 byte public 'CODE' use16
;    assume cs:seg033
;    ;org 6
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC MD_WaitRelease

proc MD_WaitRelease

    push si
    push di
    push es
    push ds
    
WaitNotPressed:
    mov ax, 3
    mov bx, 0
    mov cx, 0
    mov dx, 0
    cli
    int 33h
    sti
    and bx, 3                               ; ( button_status_bitfield & 0x00000011 ) == ( left button pressed || right button pressed )
    jnz WaitNotPressed

    mov ax, bx                              ; ? Why would you return BX on purpose, after waiting for it to be 0 ? Also, this is effectively return ST_FAILURE ?
    
    pop ds
    pop es
    pop di
    pop si
    ret

endp MD_WaitRelease

;ends seg033

end
