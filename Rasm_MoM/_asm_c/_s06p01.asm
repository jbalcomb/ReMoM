TITLE _s06p01.asm VGA_SetTextMode

.MODEL LARGE

segment dseg para public 'DATA' use16
ends dseg

segment seg006 byte public 'CODE' use16
    assume cs:seg006
    ;org 0Eh
    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC _VGA_SetTextMode

proc _VGA_SetTextMode far

    mov ax, 03h                         ; Video Mode 03 80x25 16 color text (CGA,EGA,MCGA,VGA)
    int 10h                             ; Video BIOS
    
    ret

endp _VGA_SetTextMode

ends seg006

end
