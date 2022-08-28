// _s08p16c.c SA_MK_FP0 MK_FAR

#include "ST_HEAD.H"


// int far *__cdecl __far MK_FAR(unsigned int sgmt_addr)
void _FAR * SA_MK_FP0(unsigned int sgmt_addr)
{
/*
    tmp_sgmt_addr = word ptr -6
    fptr_ofst = word ptr -4
    fptr_sgmt = word ptr -2
    sgmt_addr = word ptr 6
    mov ax, [bp+sgmt_addr]
    mov [bp+tmp_sgmt_addr], ax
    mov ax, [bp+tmp_sgmt_addr]
    mov [bp+fptr_sgmt], ax
    mov [bp+fptr_ofst], 0
    mov dx, [bp+fptr_sgmt]
    mov ax, [bp+fptr_ofst]
*/
    void _FAR * fp;
    fp = (void _FAR *)MK_FP(sgmt_addr,0);
    return fp;
}
