TITLE _s09p07.asm strcpyfar

.MODEL LARGE, C

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg009 byte public 'CODE' use16
;    assume cs:seg009
;    ;org 2
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC strcpyfar

proc strcpyfar far

    dstoff  = word ptr 6
    dstseg  = word ptr 8
    srcoff  = word ptr 0Ah
    srcseg  = word ptr 0Ch

    push bp
    mov  bp, sp
    push si
    push di
    push es
    push ds
    
    cmp [bp+dstseg], 0
    jnz @@SkipSeg

    mov ax, ds
    mov [bp+dstseg], ax

@@SkipSeg:
    cmp [bp+srcseg], 0
    jnz @@SetRegs

    mov ax, ds
    mov [bp+srcseg], ax

@@SetRegs:
    mov ax, [bp+dstseg]
    mov es, ax
    mov si, [bp+srcoff]
    mov di, [bp+dstoff]
    mov ax, [bp+srcseg]
    mov ds, ax

@@NextByte:
    lodsb
    stosb
    cmp al, 0
    jnz @@NextByte

    pop ds
    pop es
    pop di
    pop si
    pop bp
    ret

endp strcpyfar

;ends seg009

end
