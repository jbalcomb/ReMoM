; _s20p05.asm VGA_SetDACChanged

.MODEL LARGE, C

.CODE
;segment seg020 byte public 'CODE' use16
;    assume cs:seg020
;    ;org 0Ah
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC VGA_SetDACChanged

proc VGA_SetDACChanged

    First_Color= word ptr 6
    Last_Color= word ptr 8

    push bp
    mov  bp, sp
    push si

    mov si, [bp+First_Color]
    jmp short loc_1C0B2
loc_1C0A0:
    mov ax, 1
    push ax
    push si
    push [VGA_DACmod_Segment@]
    call POKE_Byte
    add sp, 6
    inc si
loc_1C0B2:
    cmp si, [bp+Last_Color]
    jle short loc_1C0A0

    pop si
    pop bp
    ret

endp VGA_SetDACChanged

;ends seg020

end
