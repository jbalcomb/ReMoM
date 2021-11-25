TITLE   _s10p01.asm LBXE_LoadSingle

.MODEL LARGE, C

EXTRN   LBX_EntryLoader:PROC ; _s10p10.asm

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg010 byte public 'CODE' use16
;    assume cs:seg010
;    ;org 6
;    ;assume es:nothing, ss:nothing, ds:dseg

PUBLIC LBXE_LoadSingle

proc LBXE_LoadSingle far

    FileName = word ptr  6
    Entry_Index = word ptr  8

    push bp
    mov  bp, sp
    
    mov  ax, 0                          ;xor ax, ax
    push ax
    mov  ax, 0                          ;xor ax, ax
    push ax
    mov  ax, 0                          ;xor ax, ax
    push ax
    push [bp+Entry_Index]
    push [bp+FileName]
;    ;nop
;    ;push cs
    call LBX_EntryLoader
    add  sp, 0Ah
    
    pop  bp
    
    ret

endp LBXE_LoadSingle

;ends seg010

end
