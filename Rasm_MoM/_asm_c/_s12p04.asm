TITLE _s12p04.asm EMS_GetHandlePageCount

.MODEL LARGE, C

;segment seg012 byte public 'CODE' use16
.CODE
    ;assume cs:seg012
    ;org 6
    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC EMS_GetHandlePageCount

; returns the amount of pages allocated to the handle,
; or 0 on errors
proc EMS_GetPageCount far

    EMM_Handle = word ptr 6

    push bp
    mov bp, sp
    push es
    push ds
    push si
    push di

    mov ah, 4Ch
    mov dx, [bp+EMM_Handle]
    int 67h
    cmp ah, 0
    jnz error
    mov ax, bx

    pop di
    pop si
    pop ds
    pop es
    pop bp
    ret

error:
    mov ax, 0

    pop di
    pop si
    pop ds
    pop es
    pop bp
    ret

endp EMS_GetHandlePageCount

;ends seg012

end
