TITLE _s09p03.asm lbx_seek
; BCpp31LR, pg347/361
; #include <io.h>
; long lseek(int handle, long offset, int fromwhere);

.MODEL LARGE, C

;segment dseg para public 'DATA' use16
;ends dseg

.CODE
;segment seg009 byte public 'CODE' use16
;    assume cs:seg009
;    ;org 2
;    assume es:nothing, ss:nothing, ds:dseg, fs:nothing, gs:nothing

PUBLIC lbx_seek

proc lbx_seek far

    foffset = dword ptr 6
    ;foffset_Lo = word ptr 6
    ;foffset_Hi = word ptr 8
    fhandle =  word ptr 0Ah

    push bp
    mov  bp, sp
    push si
    push di
    push es
    push ds

    mov ax, 4200h
    mov bx, [bp+fhandle]
    mov cx, word ptr [bp+foffset+2]
    mov dx, word ptr [bp+foffset]
    ;mov cx, [bp+foffset_Hi]
    ;mov dx, [bp+foffset_Lo]
    int 21h

jb @@Error
    mov ax, 0FFFFh
    pop ds
    pop es
    pop di
    pop si
    pop bp
    ret

@@Error:
    mov ax, 0
    pop ds
    pop es
    pop di
    pop si
    pop bp
    ret

endp lbx_seek

;ends seg009

end
