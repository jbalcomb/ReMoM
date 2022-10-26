TITLE _s13p25.asm EMM_Map_DataH

.MODEL LARGE, C

.CODE
;segment seg013 byte public 'CODE' use16
;    assume cs:seg013
;    ;org 7
;    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC EMM_Map_DataH

proc EMM_Map_DataH

    push bp
    mov bp, sp

    xor ax, ax
    push ax
    push [EMM_DATA_H]
    call EMM_MapFourPages

    pop cx
    pop cx
    pop bp
    ret

endp EMM_Map_DataH

;ends seg013

end
