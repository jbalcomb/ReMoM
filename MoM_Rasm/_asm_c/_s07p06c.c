// _s07p06c.c farpeekw
// ST_LBX.H

#include "ST_DEF.H"

unsigned int farpeekw(unsigned int sgmt, unsigned int ofst)
{
    unsigned int ret_val;
    /*
    mov si, ofst
    mov ax, sgmt
    mov ds, ax
    lodsw
    */
    ret_val = *( (unsigned int *) MK_FP(sgmt, ofst) );

    return ret_val;
}
