TITLE _s07p09.asm  farpokew
;BCpp31LR, pg 393/408
;void poke(unsigned segment, unsigned offset, int value);

.MODEL LARGE, C

.CODE
;segment seg007 byte public 'CODE' use16
;assume cs:seg007
;;org 0Ah
;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC farpokew

proc farpokew far

    sgmt    = word ptr 6
    ofst    = word ptr 8
    value   = word ptr 0Ah

    push bp
    mov  bp, sp
    push es
    push di

    mov ax, [bp+sgmt]
    mov es, ax
    mov di, [bp+ofst]
    mov ax, [bp+value]
    stosw

    pop di
    pop es
    pop bp
    ret

endp farpokew

;ends seg007

end
