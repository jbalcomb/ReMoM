// _s07p04c.c Alloc_Seg

#include "ST_SA.H"

//IDA: int __cdecl __far Alloc_Seg(unsigned int LBX_Alloc_Seg)
int SA_Alloc_Validate(unsigned int SAMB)
{
    unsigned int *SgmtPtr;
    unsigned int word1;
    unsigned int word2;

    SgmtPtr = SAMB;

    word1 = SgmtPtr[0];
    word2 = SgmtPtr[2];

    if (word1 != LBX_MemSig1 || word2 != LBX_MemSig2)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
