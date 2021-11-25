// _s07p06c.c farpeekw

unsigned int farpeekw(unsigned int sgmt, unsigned int ofst)
{
    unsigned int tmpWord;
    printf("DEBUG: BEGIN: farpeekw()\n");
    printf("sgmt == 0x%08X  ofst == 0x%08X\n", sgmt, ofst);
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
    // tmpWord = _AX;
    // printf("_AX == 0x%04X\n", _AX);
    // //tmpWord = _AX;
    // printf("tmpWord == 0x%04X\n", tmpWord);
    // printf("_AX == 0x%04X\n", _AX);
    // printf("DEBUG: END: farpeekw()\n");
    // // return (_AX);
    // return tmpWord;
    return (_AX);
}
