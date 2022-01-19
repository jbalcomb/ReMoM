// _s07p06c.c farpeekw
// ST_LBX.H

#include <DOS.H>

#include "ST_HEAD.H"

unsigned int farpeekw(unsigned int sgmt, unsigned int ofst)
{
    unsigned int tmp_Word;
    unsigned char _FAR * tmp_fptr_SgmtOfst;
    
    //printf("DEBUG: %s %d BEGIN: farpeekw(sgmt = 0x%04X, ofst = 0x%04X)\n", __FILE__, __LINE__, sgmt, ofst);

    // tmp_fptr_SgmtOfst = (unsigned char _FAR *) MK_FP(sgmt, ofst);
    // tmp_Word = (unsigned int) *tmp_fptr_SgmtOfst;

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
asm pop si
asm pop ds
//pop bp
//ret
    tmp_Word = _AX;
    // printf("_AX == 0x%04X\n", _AX);
    // //tmpWord = _AX;
    // printf("tmpWord == 0x%04X\n", tmpWord);
    // printf("_AX == 0x%04X\n", _AX);
    // printf("DEBUG: END: farpeekw()\n");
    // // return (_AX);
    // return tmpWord;
    //printf("DEBUG: %s %d END: farpeekw(sgmt = 0x%04X, ofst = 0x%04X) { tmp_Word = 0x%04X }\n", __FILE__, __LINE__, sgmt, ofst, tmp_Word);
    return tmp_Word;
}
