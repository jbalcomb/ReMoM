// _s09p05a.c lbx_read_sgmt
// ST_LBX.H

#include "ST_HEAD.H"

int  lbx_read_sgmt(unsigned int sgmt_addr, int nbytes, int fhandle)
{
    // printf("DEBUG: [%s, %d]: BEGIN: lbx_read_sgmt(sgmt_addr == 0x%04X  nbytes == %d  fhandle == %d)\n", __FILE__, __LINE__, sgmt_addr, nbytes, fhandle);

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
    // // NOTE: printf() clobbers AX, CX, and DX
    // printf("DEBUG: END: lbx_read_sgmt()\n");
    return(ST_SUCCESS);
Error:
//asm mov ax, 0
asm pop ds
//asm pop es
//asm pop di
//asm pop si

    // printf("DEBUG: END: lbx_read_sgmt()\n");
    return(ST_FAILURE);
}
