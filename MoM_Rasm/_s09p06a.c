// _s09p06a.c lbx_read_ofst
// ST_LBX.H

#include "ST_HEAD.H"

int  lbx_read_ofst(unsigned int dst_ofst, int nbytes, int fhandle)
{
    // printf("DEBUG: [%s, %d]: BEGIN: lbx_read_ofst(dst_ofst == 0x%04X  nbytes == %d  fhandle == %d)\n", __FILE__, __LINE__, dst_ofst, nbytes, fhandle);

// asm push si
// asm push di
// asm push es
asm push ds
asm mov ah, 3Fh
asm mov bx, fhandle
asm mov cx, nbytes
asm mov dx, dst_ofst
asm int 21h
asm jb Error
// asm mov ax, 0FFFFh
asm pop ds
// asm pop es
// asm pop di
// asm pop si
    // printf("DEBUG: END: lbx_read_ofst()\n");
    return(ST_SUCCESS);

Error:
// asm mov ax, 0
asm pop ds
// asm pop es
// asm pop di
// asm pop si
    // printf("DEBUG: END: lbx_read_ofst()\n");
    return(ST_FAILURE);
}
