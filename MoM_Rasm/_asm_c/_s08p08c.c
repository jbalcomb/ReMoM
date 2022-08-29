// _s08p08c.c SA_Allocate_MemBlk_LbxHeader
// in ST_SA.H

#include "ST_HEAD.H"

#include "ST_SA.H"

// unsigned int SA_Allocate_MemBlk_LbxHeader(unsigned int nparas)
unsigned int SA_Allocate_MemBlk(unsigned int nparas)
{
    sgmt_addr SAMB_data;

//    printf("DEBUG: [%s, %d] BEGIN: SA_Allocate_MemBlk(nparas = %d)\n", __FILE__, __LINE__, nparas);

    gfp_tmpSAMB = (char _FAR *) farmalloc(((unsigned long)nparas * 16) + 16);
    //gfp_tmpSAMB = NULL;  // DEBUG: force failure, for testing
    if ( gfp_tmpSAMB == NULL )
    {
//        printf("DEBUG: FATAL: [%s, %d] gfp_tmpSAMB: %Fp,  farmalloc(%lu) \n", __FILE__, __LINE__, gfp_tmpSAMB, (((unsigned long)nparas * 16) + 16));
        SA_Alloc_Error(1, nparas); // Alloc Error #1: Allocation Too Small
    }
    //printf("DEBUG: gfp_tmpSAMB: %Fp  0x%08X\n", gfp_tmpSAMB, gfp_tmpSAMB);
    
    SAMB_data = FP_SEG(gfp_tmpSAMB) + 1;

    Update_MemFreeWorst_KB();

//    printf("DEBUG: [%s, %d] END: SA_Allocate_MemBlk() { SAMB_data = 0x%04X}\n", __FILE__, __LINE__, SAMB_data);
    return SAMB_data;
}
