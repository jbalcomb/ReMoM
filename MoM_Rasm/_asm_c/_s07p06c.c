// _s07p06c.c farpeekw
// ST_LBX.H

#include <DOS.H>

#include "ST_HEAD.H"

unsigned int farpeekw(unsigned int sgmt, unsigned int ofst)
{
    /*
    mov si, ofst
    mov ax, sgmt
    mov ds, ax
    lodsw
    */
    
    return(*( (unsigned int _FAR * )MK_FP(sgmt, ofst)));
}
