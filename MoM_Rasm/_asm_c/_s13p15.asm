TITLE _s13p15.asm EMM_CheckHandleOpen

.MODEL LARGE, C

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg013 byte public 'CODE' use16
;    assume cs:seg013
;    ;org 7
;    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC EMM_CheckHandleOpen

proc EMM_CheckHandleOpen far

    EMM_HandleName@= word ptr 6

    push bp
    mov bp, sp
    push si
    push di

    xor di, di
    xor si, si
    jmp short loc_175F4

loc_175BC:
    push [bp+EMM_HandleName@]
    mov ax, si
    mov dx, 0Ch
    imul dx
    add ax, offset EMM_Table.HandleName
    push ax
    call stricmp
    pop cx
    pop cx

    or ax, ax
    jnz short loc_175F3

    mov ax, si
    mov dx, 0Ch
    imul dx
    mov bx, ax
    mov di, [EMM_Table.Handle+bx]

    mov ax, si
    mov dx, 0Ch
    imul dx
    mov bx, ax
    mov al, [EMM_Table.Reserved+bx]
    cbw                                 ; The CBW (convert byte to word) instruction copies the sign (bit 7) in the source operand into every bit in the AH register.
    mov [LBX_LastLoadReserved], ax

loc_175F3:
    inc si

loc_175F4:
    cmp si, [EMM_Open_Handles]
    jge short loc_175FE

    or di, di
    jz short loc_175BC

loc_175FE:
    mov ax, di
    jmp short $+2

    pop di
    pop si
    pop bp
    ret

endp EMM_CheckHandleOpen

;ends seg013

end
