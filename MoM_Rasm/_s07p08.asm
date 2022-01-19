TITLE _s07p.asm 
;BCpp31LR, pg 394/409
;void pokeb(unsigned segment, unsigned offset, char valueue);

.MODEL LARGE, C

.CODE
;segment seg007 byte public 'CODE' use16
;assume cs:seg007
;;org 0Ah
;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC farpokeb

proc farpokeb far

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
    stosb

    pop di
    pop es
    pop bp
    ret

endp farpokeb

;ends seg007

end
