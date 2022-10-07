
#include "MoX_SA.H"

#include "MoX_TYPE.H"
#include "MoX_DEF.H"    /* PTR_INC_PARAGRAPH() */

#include "MoX_MoM.H"    /*  */

#include "MoX_DBG.H"    /* DBG_IsDisabled() */
#include "MoX_EXIT.H"   /* Exit() */


#if defined(__DOS16__)
#include <MALLOC.H>
#include <STDLIB.H>     /* itoa() */
#include <STRING.H>     /* strcat(), strcpy(); */
#endif
#if defined(__WIN32__)
#include <malloc.h>
#include <stdlib.h>     /* itoa() */
#endif


#ifdef STU_DEBUG
#include "STU_DBG.H"  /* DLOG() */
#endif


unsigned char * pTmpSAMB;


char *cnst_Alloc_Error01 = "Near Allocation too large by ";                     // dseg:3D56
char *cnst_Alloc_Error02 = " bytes";                                            // dseg:3D74
char *cnst_Alloc_Error51 = "Insufficient memory. You need at least ";           // dseg:3D7B
char *cnst_Alloc_Error52 = "K free. Try removing all TSR's.";                   // dseg:3DA3
char *cnst_Alloc_Error11 = "Dynamic allocation too small for Allocate_Space()"; // dseg:3DC3
char *cnst_Alloc_Error12 = " of ";                                              // dseg:3DF5
char *cnst_Alloc_Error13 = " blocks";                                           // dseg:3DFA
char *cnst_Alloc_Error21 = "Failed to reload";                                  // dseg:3E02
char *cnst_Alloc_Error22 = " Allocate_Next_Block()";                            // dseg:3E13
char *cnst_Alloc_Error23 = ": Short by ";                                       // dseg:3E2A
char *cnst_Alloc_Error3 = "Allocation space has been corrupted for";            // dseg:3E36
char *cnst_Alloc_Error4 = " (EMM) ";                                            // dseg:3E5E


// // s07p04
// /*
//     pSAMB_head:
//         pointer to 'SAMB Header', as returned by SA_Allocate_Space()  (~AKA 'Load Type' 1)
//     Return:
//         ST_STATUS: ST_SUCCESS, ST_FAILURE
// */
// int SA_Alloc_Validate(SAMB_ptr pSAMB_head)
// {
//     unsigned int memsig1;
//     unsigned int memsig2;
//     int is_valid;
//  
//     memsig1 = ((unsigned int)*((unsigned char *)pSAMB_head + 4)) | (unsigned int)((unsigned int)*((unsigned char *)pSAMB_head + 5) << 8);
//     memsig2 = ((unsigned int)*((unsigned char *)pSAMB_head + 6)) | (unsigned int)((unsigned int)*((unsigned char *)pSAMB_head + 7) << 8);
// 
//     if (memsig1 != SA_MEMSIG1 || memsig2 != SA_MEMSIG2)
//     {
//         is_valid = ST_FAILURE;
//     }
//     else
//     {
//         is_valid = ST_SUCCESS;
//     }
// 
//     return is_valid;
// }


// s08p08
SAMB_ptr SA_Allocate_MemBlk(unsigned int nparas)
{
    SAMB_ptr pSAMB_data;

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: BEGIN: SA_Allocate_MemBlk(nparas = %u)\n", __FILE__, __LINE__, nparas);
// #endif

    pTmpSAMB = (SAMB_ptr) malloc(((unsigned long)nparas * 16) + 16);

    if ( pTmpSAMB == NULL )
    {
        SA_Alloc_Error(0x01, nparas); // Alloc Error #1: Allocation Too Small
    }
    
// #ifdef __DOS16__
//     //pSAMB_data = (pTmpSAMB + 16);
//     //pSAMB_data = MK_FP((FP_SEG(pTmpSAMB) + 1),FP_OFF(pTmpSAMB));
//     pSAMB_data = (SAMB_ptr) MK_FP((FP_SEG(pTmpSAMB) + 1),0);  // add 1 segment/paragraph
// #endif
// #ifdef __WIN32__
//     pSAMB_data = pTmpSAMB + 16;  // add 16 bytes
// #endif
    pSAMB_data = (SAMB_ptr) PTR_INC_PARAGRAPH(pTmpSAMB);  // add 1 paragraph / 16 bytes

    //Update_MemFreeWorst_KB();

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: END: SA_Allocate_MemBlk(nparas = %u) { pSAMB_data = %p }\n", __FILE__, __LINE__, nparas, pSAMB_data);
// #endif

    return pSAMB_data;
}


// _s08p19
void SA_Alloc_Error(int SA_Error_Number, int blocks)
{
    int SA_Error_Index;
    char str_itoa[20];
    char str_errmsg[120];

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: SA_Alloc_Error( SA_Error_Number = %d, blocks = %d)\n", __FILE__, __LINE__, SA_Error_Number, blocks);
#endif

#ifdef STU_DEBUG
    // strcpy(str_errmsg, "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");  // 120 characters
    strcpy(str_errmsg, "0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_");  // 110 characters
    dbg_prn("DEBUG: [%s, %d] str_errmsg: %s\n", __FILE__, __LINE__, str_errmsg);
#endif

    if ( DBG_IsDisabled() != 0 )
    {
        // DLOG("( DBG_IsDisabled() != 0 )");
        SA_Error_Index = SA_Error_Number - 1;
        switch(SA_Error_Index)
        {
            case 0:
                strcpy(str_errmsg, cnst_Alloc_Error11);
                strcat(str_errmsg, cnst_Alloc_Error12);
                itoa(blocks, str_itoa, 10);
                break;
            case 1:
                strcpy(str_errmsg, cnst_Alloc_Error21);
                strcat(str_errmsg, cnst_Alloc_Error22);
                strcat(str_errmsg, cnst_Alloc_Error23);
                itoa(blocks, str_itoa, 10);
                break;
            case 2:
                strcpy(str_errmsg, cnst_Alloc_Error3);
                strcat(str_errmsg, cnst_Alloc_Error22);
                strcat(str_errmsg, cnst_Alloc_Error12);
                itoa(blocks, str_itoa, 10);
                break;
            case 3:
                strcpy(str_errmsg, cnst_Alloc_Error21);
                strcat(str_errmsg, cnst_Alloc_Error4);
                strcat(str_errmsg, cnst_Alloc_Error22);
                strcat(str_errmsg, cnst_Alloc_Error23);
                itoa(blocks, str_itoa, 10);
                break;
        }
        strcat(str_errmsg, str_itoa);
        strcat(str_errmsg, cnst_Alloc_Error13);
        goto Done;
    }
    else
    {
        // DLOG("( DBG_IsDisabled() == 0 )");
        strcpy(str_errmsg, cnst_Alloc_Error51);
        itoa(RAM_Min_KB, str_itoa, 10);
        strcat(str_errmsg, str_itoa);
        strcat(str_errmsg, cnst_Alloc_Error52);
        goto Done;
    }

Done:

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] str_errmsg: %s\n", __FILE__, __LINE__, str_errmsg);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: SA_Alloc_Error( SA_Error_Number = %d, blocks = %d)\n", __FILE__, __LINE__, SA_Error_Number, blocks);
#endif
    Exit(str_errmsg);
}
