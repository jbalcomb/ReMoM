// _s08p08c.c SA_Allocate_MemBlk_LbxHeader
// in ST_SA.H

#include "ST_HEAD.H"

// unsigned int SA_Allocate_MemBlk_LbxHeader(unsigned int nparas)
unsigned int SA_Allocate_MemBlk(unsigned int nparas)
{
    sgmt_addr SAMB;

    printf("DEBUG: BEGIN: SA_Allocate_MemBlk()\n");

    gfp_tmpSAMB = (char far *) farmalloc(((unsigned long)nparas * 16) + 16);
    //gfp_tmpSAMB = NULL;  // force failure, for testing
    if ( gfp_tmpSAMB == NULL )
    {
        //printf("DEBUG: Error: farmalloc(%lu) [%s, %d]\n", nbytes, __FILE__, __LINE__);
        SA_Alloc_Error(1, nparas); // Alloc Error #1: Allocation Too Small
    }
    printf("DEBUG: gfp_tmpSAMB: %Fp  0x%016X\n", gfp_tmpSAMB, gfp_tmpSAMB);
    
    SAMB = FP_SEG(gfp_tmpSAMB) + 1;

    Update_MemFreeWorst_KB();

    printf("DEBUG: END: SA_Allocate_MemBlk()\n");
    return SAMB;
}
