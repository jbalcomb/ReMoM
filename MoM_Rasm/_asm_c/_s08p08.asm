TITLE  _s08p08.asm LBX_farmalloc
; SEE:  BCpp31LR,pg18/29; allocmem,_dos_allocmem
;       https://stanislavs.org/helppc/int_21-48.html
;       https://stanislavs.org/helppc/memory_control_block.html
;       BCpp31LR,pg155/168; farmalloc

.MODEL LARGE, C

EXTRN LBX_Alloc_Segment:WORD
EXTRN LBX_Alloc_Offset:WORD

EXTRN farmalloc:PROC
EXTRN LXLSH@:PROC ; BORLANDC\CRTL\CLIB\H_LLSH.ASM

;segment dseg para public 'DATA' use16
;ends dseg

;segment seg008 byte public 'CODE' use16
;    assume cs:seg008
;    ;org 7
;    ;assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC ST_AllocMem

proc ST_AllocMem far

    segp = word ptr -2
    size = word ptr 6

    push bp
    mov bp, sp
    sub sp, 2

    mov ax, [bp+size]
    mov dx, 0
    mov cl, 4
    call LXLSH@
    add ax, 10h
    adc dx, 0
    push dx
    push ax
    call farmalloc
    add sp, 04h                         ;pop cx, pop cx

    mov [LBX_Alloc_Segment], dx
    mov [LBX_Alloc_Offset], ax

    mov ax, [LBX_Alloc_Offset]
    or ax, [LBX_Alloc_Segment]
    jnz short @@FptrNotNull

@@error:
    mov ax, 0

@@JmpDone:
    jmp short @@Done

@@FptrNotNull:
    mov ax, [LBX_Alloc_Segment]
    inc ax
    mov [bp+segp], ax

    call RAM_Update_WorstFree

    mov ax, [bp+segp]
    jmp short @@JmpDone

@@Done:
    mov sp, bp
    pop bp
    ret

endp ST_AllocMem

;ends seg008

end
