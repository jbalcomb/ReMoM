// _s07p07c.c farpeekdw
// ST_LBX.H

#include <DOS.H>

#include "ST_HEAD.H"

unsigned long farpeekdw(unsigned int sgmt, unsigned int ofst)
{
    unsigned int tmp_AX;
    unsigned int tmp_DX;
    unsigned long tmpDoubleWord;

    //printf("DEBUG: %s %d BEGIN: farpeekdw(sgmt = 0x%04X, ofst = 0x%04X)\n", __FILE__, __LINE__, sgmt, ofst);

//sgmt = word ptr 6
//ofst = word ptr 8
//push bp
//mov  bp, sp
asm push ds
asm push si
asm mov si, ofst
asm mov ax, sgmt
asm mov ds, ax
asm lodsw
asm mov bx, ax
asm lodsw
asm mov dx, ax
asm mov ax, bx
asm pop si
asm pop ds
//pop bp
//ret
    tmp_AX = _AX;
    // printf("tmp_AX == 0x%04X\n", tmp_AX);
    tmp_DX = _DX;
    // printf("tmp_DX == 0x%04X\n", tmp_DX);
    tmpDoubleWord = (unsigned long) ( ((unsigned long)tmp_DX) << 16 ) + tmp_AX;

    //printf("DEBUG: %s %d END: farpeekdw(sgmt = 0x%04X, ofst = 0x%04X) { tmp_AX = 0x%04X, tmp_DX = 0x%04X, tmp_Word = 0x%08lX }\n", __FILE__, __LINE__, sgmt, ofst, tmp_AX, tmp_DX, tmpDoubleWord);
    return tmpDoubleWord;
}
