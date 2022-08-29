// _s07p09c.c  farpokew
// ST_SA.H
//;BCpp31LR, pg 393/408
//;void poke(unsigned segment, unsigned offset, int value);

#include "ST_HEAD.H"


void farpokew(unsigned int sgmt, unsigned int ofst, unsigned short val)
{
    /*
    mov ax, [bp+sgmt]
    mov es, ax
    mov di, [bp+ofst]
    mov ax, [bp+value]
    stosw
    */

    ( *( (unsigned short _FAR * )MK_FP(sgmt, ofst)) = val );
}
