// _s07p07c.c farpeekdw
// ST_LBX.H

#include <DOS.H>

#include "ST_HEAD.H"

unsigned long farpeekdw(unsigned int sgmt, unsigned int ofst)
{
    unsigned int tmp_AX;
    unsigned int tmp_DX;
    unsigned long tmpDoubleWord;

    tmp_AX = *( (unsigned int _FAR * )MK_FP(sgmt, ofst));
    tmp_DX = *( (unsigned int _FAR * )MK_FP(sgmt, ofst+2));

    tmpDoubleWord = (unsigned long) ( ((unsigned long)tmp_DX) << 16 ) + tmp_AX;

    return tmpDoubleWord;
}
