TITLE _s12p06.asm EMM_GetHandleName

.MODEL LARGE, C

segment seg012 byte public 'CODE' use16
    assume cs:seg012

PUBLIC EMM_GetHandleName

proc EMM_GetHandleName far

    argEmmHandleName = word ptr 6        ; ~= argEmmHandleName:FAR PTR BYTE
    argEmmHandleNumber = word ptr 8

    push bp
    mov  bp, sp
    push es
    push ds
    push si
    push di

    mov ax, 5300h
    mov dx, [bp+argEmmHandleNumber]
    mov di, [bp+argEmmHandleName]
    mov bx, ds
    mov es, bx
    assume es:@data
    int 67h
    ; 		ES:DI = pointer to 8byte handle name array
	;       AH = status  (see EMS STATUS)

    cmp ah, 0  ; 00  success
    jnz short @@error

    mov di, [bp+argEmmHandleName]
    add di, 8
    mov al, 0
    stosb                               ; ~= argEmmHandleName[8] = '\0'

    mov ax, 0FFFFh

    pop di
    pop si
    pop ds
    pop es
    pop bp
    ret

@@error:
    mov ax, 0

    pop di
    pop si
    pop ds
    pop es
    pop bp
    ret

endp EMM_GetHandleName

ends seg012

end
