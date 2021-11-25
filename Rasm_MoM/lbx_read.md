
lbx_read_sgmt vs. lbx_read_ofst vs. _dos_read

|dos_read|lbx_read_sgmt|lbx_read_ofst|
|:-------|:------------|:------------|
| void far *buf   | buffer = word ptr 6    | buffer = word ptr 6    |
| unsigned len    | nbytes = word ptr 8    | nbytes = word ptr 8    |
| int fd          | fhandle = word ptr 0Ah | fhandle = word ptr 0Ah |
|                 |                        |                        |
| mov ah,3fh      | mov ah, 3Fh            | mov ah, 3Fh            |
| mov bx,fd       | mov bx, [bp+fhandle]   | mov bx, [bp+fhandle]   |
| mov cx,len      | mov cx, [bp+nbytes]    | mov cx, [bp+nbytes]    |
| lds dx, buf     | mov dx, [bp+buffer]    | mov dx, [bp+buffer]    |
|                 | mov ds, dx             |                        |
|                 | mov dx, 0              |                        |
| int 21h         | int 21h                | int 21h                |


unsigned  _dos_read( int fd, void far *buf, unsigned len, unsigned *nread )
{
asm     push    ds
asm     mov     ah,3fh
asm     mov     bx,fd
asm     mov     cx,len
asm     lds     dx, buf
asm     int     21h
asm     pop     ds
asm     jc      _dos_readFailed

asm     LES_    bx, nread
asm     mov     ES_ [bx], ax
        return  (0);

_dos_readFailed:
        return (__DOSerror(_AX));
}

proc lbx_read_sgmt far

    buffer  = word ptr 6
    nbytes  = word ptr 8
    fhandle = word ptr 0Ah

    push bp
    mov  bp, sp
    push si
    push di
    push es
    push ds

    mov ah, 3Fh
    mov bx, [bp+fhandle]
    mov cx, [bp+nbytes]
    mov dx, [bp+buffer]
    mov ds, dx
    mov dx, 0
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

endp lbx_read_sgmt

proc lbx_read_ofst far

    buffer  = word ptr 6
    nbytes  = word ptr 8
    fhandle = word ptr 0Ah

    push bp
    mov bp, sp
    push si
    push di
    push es
    push ds
    
    mov ah, 3Fh
    mov bx, [bp+fhandle]
    mov cx, [bp+nbytes]
    mov dx, [bp+buffer]
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

endp lbx_read_ofst
