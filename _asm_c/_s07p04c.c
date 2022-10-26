// _s07p04c.c Alloc_Seg

#include "ST_SA.H"

//IDA: int __cdecl __far Alloc_Seg(unsigned int LBX_Alloc_Seg)
int SA_Alloc_Validate(unsigned int SAMB)
{
    int is_valid;
    // unsigned int *SgmtPtr;
    unsigned int _FAR * SgmtPtr;
    unsigned int word3;
    unsigned int word4;

    // printf("DEBUG: %s %d BEGIN: SA_Alloc_Validate(SAMB = 0x%04X)\n", __FILE__, __LINE__, SAMB);

    // // // SgmtPtr = SAMB;
    // // // // // // // // *SgmtPtr = SAMB; ... word1 = SgmtPtr[0]; ... gives you the value of SAMB
    // // // // // // // printf("DEBUG: [%s %d]: SgmtPtr: 0x%04X\n", __FILE__, __LINE__, SgmtPtr); // ... gives you the value of SAMB
    // // // // // // // printf("DEBUG: [%s %d]: *SgmtPtr: 0x%04X\n", __FILE__, __LINE__, *SgmtPtr); // ~== word1 = SgmtPtr[0];
    // // // // // // // printf("DEBUG: [%s %d]: &SgmtPtr: 0x%04X\n", __FILE__, __LINE__, &SgmtPtr); // of course, gives you the address of the variable SgmtPtr
    // // // // // // word1 = SgmtPtr[0];
    // // // // // // word2 = SgmtPtr[2];
    // // // // // // Those were the wrong offsets - But, still does not return the correct values.
    // // // // // // word1 = SgmtPtr[4];  // s_SAMB.MemSig1
    // // // // // // word2 = SgmtPtr[6];  // s_SAMB.MemSig2
    // // // // // word3 = (unsigned int)SgmtPtr[4];  // s_SAMB.MemSig1
    // // // // // word4 = (unsigned int)SgmtPtr[6];  // s_SAMB.MemSig2
    // // // // word3 = (unsigned int)*SgmtPtr[4];  // s_SAMB.MemSig1 ... invalid indirection
    // // // // word4 = (unsigned int)*SgmtPtr[6];  // s_SAMB.MemSig2 ... invalid indirection
    // // SgmtPtr = (unsigned int *)SAMB;
    // // word3 = SgmtPtr[4];  // s_SAMB.MemSig1
    // // word4 = SgmtPtr[6];  // s_SAMB.MemSig2
    // SgmtPtr = (unsigned int far *)SAMB;
    // word3 = SgmtPtr[4];  // s_SAMB.MemSig1
    // word4 = SgmtPtr[6];  // s_SAMB.MemSig2
    // printf("DEBUG: [%s %d]: word3: 0x%04X\n", __FILE__, __LINE__, word3);
    // printf("DEBUG: [%s %d]: word4: 0x%04X\n", __FILE__, __LINE__, word4);

    // printf("DEBUG: [%s %d]: farpeekw(SAMB, 4): 0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB, 4));
    // printf("DEBUG: [%s %d]: farpeekw(SAMB, 6): 0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB, 6));
    // printf("DEBUG: [%s %d]: farpeekw(SAMB, 8): 0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB, 8));
    // printf("DEBUG: [%s %d]: farpeekw(SAMB, 10): 0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB, 10));

    // I gave up.

    word3 = farpeekw(SAMB, 4);
    word4 = farpeekw(SAMB, 6);
    // printf("DEBUG: [%s %d]: word3: 0x%04X\n", __FILE__, __LINE__, word3);
    // printf("DEBUG: [%s %d]: word4: 0x%04X\n", __FILE__, __LINE__, word4);

    if (word3 != LBX_MemSig1 || word4 != LBX_MemSig2)
    {
        //return 0;
        is_valid = 0;  // FALSE / FAILURE
        // printf("DEBUG: [%s %d]: is_valid: 0x%04X\n", __FILE__, __LINE__, is_valid);
    }
    else
    {
        //return -1;
        is_valid = -1;  // TRUE / SUCCESS
        // printf("DEBUG: [%s %d]: is_valid: 0x%04X\n", __FILE__, __LINE__, is_valid);
    }

    // printf("DEBUG: %s %d END: SA_Alloc_Validate()\n", __FILE__, __LINE__);
    return is_valid;
}
