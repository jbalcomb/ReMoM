// _s07p08c.c  farpokeb
// ST_SA.H
//;BCpp31LR, pg 394/409
//;void pokeb(unsigned segment, unsigned offset, char value);

#include "ST_HEAD.H"


void farpokeb(unsigned int sgmt, unsigned int ofst, unsigned char val)
{
    /*
    mov ax, [bp+sgmt]
    mov es, ax
    mov di, [bp+ofst]
    mov ax, [bp+value]
    stosb
    */

    ( *( (unsigned char _FAR * )MK_FP(sgmt, ofst)) = val );
}
