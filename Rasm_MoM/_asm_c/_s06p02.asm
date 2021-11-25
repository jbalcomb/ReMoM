TITLE _s06p02.asm DOS_QuitWithMsg

.MODEL LARGE, C

.DATA

segment seg006 byte public 'CODE' use16
    assume cs:seg006
    assume ds:@data
    ;org 0Eh
    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC DOS_QuitWithMsg

; IDA: void __cdecl __far DOS_QuitWithMsg(char *argStringPtr)
proc DOS_QuitWithMsg far

    Message = word ptr 6

    push bp
    mov bp, sp

    mov si, [bp+Message]                ; move, into SI, the first byte at the address stored on the stack at BP+6 (~= uint16 Message = 6;)

FindNull:
    lodsb
    cmp al, 0
    jnz short FindNull

    dec si
    mov byte ptr [si], 0Dh
    inc si
    mov byte ptr [si], 0Ah
    inc si
    mov byte ptr [si], 0Dh
    inc si
    mov byte ptr [si], 0Ah
    inc si
    mov byte ptr [si], '$'
    inc si

    mov dx, [bp+Message]
    mov ah, 9
    int 21h

    mov ah, 4Ch
    int 21h

    pop bp
    ret

endp DOS_QuitWithMsg

ends seg006

end
