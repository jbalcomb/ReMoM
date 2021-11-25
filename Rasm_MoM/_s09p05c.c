// _s09p05c.c lbx_read_sgmt

int  lbx_read_sgmt(unsigned int sgmt_addr, int nbytes, int fhandle)
{
    printf("DEBUG: BEGIN: lbx_read_sgmt()\n");
    printf("sgmt_addr == 0x%08X  nbytes == %d  fhandle == %d\n", sgmt_addr, nbytes, fhandle);
//asm push bp
//asm mov  bp, sp
//asm push si
//asm push di
//asm push es
asm push ds
asm mov ah, 3Fh
asm mov bx, fhandle
asm mov cx, nbytes
asm mov dx, sgmt_addr
asm mov ds, dx
asm mov dx, 0
asm int 21h
asm jb Error
//asm mov ax, 0FFFFh
asm pop ds
//asm pop es
//asm pop di
//asm pop si
//asm pop bp
//asm ret
    printf("DEBUG: END: lbx_read_sgmt()\n");
    return (-1);
Error:
//asm mov ax, 0
asm pop ds
//asm pop es
//asm pop di
//asm pop si
//asm pop bp
//asm ret
    printf("DEBUG: END: lbx_read_sgmt()\n");
    return (0);

}