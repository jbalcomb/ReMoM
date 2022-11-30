
/*
    "SPACE ALLOC" - STARLORD.EXE
*/

/*
    MoO2
    Module: allocate
    Allocate_Space
    Allocate_Space_No_Header
    Allocate_First_Block
    Allocate_Next_Block
    Allocation_Error
    Reallocation_Error

    Allocate_Space
        Num params: 1
        Return type: pointer (4 bytes) 
            signed integer (4 bytes) size
            pointer (4 bytes) name
            pointer (4 bytes) header
            signed integer (4 bytes) lsize

*/

#include "MoX_TYPE.H"   /* MoX_SAMB_ptr, etc. */
#include "MoX_FAR.H"    /* MoX_MK_FP(), etc. */
#include "MoX_DEF.H"    /* MoX_ST_FAILURE, etc. */
#include "MoX_BITS.H"   /* MoX_GET_2B_OFS() etc. */

#include "MoX_SA.H"

#include "ST_DEF.H"

#include "ST_EXIT.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

#if defined(__DOS16__)
#include <ALLOC.H>      /* coreleft(), farcoreleft(), malloc(), farmalloc(), free(), farfree() */
#include <STDLIB.H>     /* itoa() */
#include <STRING.H>     /* strcat(), strcpy() */
#endif
#if defined(__WIN32__)
#include <malloc.h>
#include <stdlib.h>     /* itoa() */
#include <string.h>     /* strcat(), strcpy() */
#endif


extern MoX_word MoX_RAM_Min_KB;
extern int16_t RAM_Min_KB;



MoX_SAMB_ptr MoX_tmp_SAMB_head;  // rename - ?pTmpSAMB?, g_header, tmp_header_ptr, _header_ptr, ?!?


char * str_allocation_errors[] =
{
    "Near Allocation too large by ",
    " bytes",
    "Insufficient memory. You need at least ",
    "K free. Try removing all TSR's.",
    "Dynamic allocation too small for Allocate_Space()",
    " of ",
    " blocks",
    "Failed to reload",
    " Allocate_Next_Block()",
    ": Short by ",
    "Allocation space has been corrupted for",
    " (EMM) "
};


/*
    MAGIC.EXE  seg007
*/

// s07p04
MoX_word Check_Allocation(MoX_SAMB_ptr SAMB_head)
{
    MoX_word is_valid;
    
    if (MoX_SA_GET_MEMSIG1(SAMB_head) != MoX_SA_MEMSIG1 || MoX_SA_GET_MEMSIG2(SAMB_head) != MoX_SA_MEMSIG2)
    {
        is_valid = MoX_ST_FAILURE;
    }
    else
    {
        is_valid = MoX_ST_SUCCESS;
    }
#ifdef STU_DEBUG
    dbg_prn("MemSig1: 0x%04X\n", MoX_SA_GET_MEMSIG1(SAMB_head));
    dbg_prn("MemSig2: 0x%04X\n", MoX_SA_GET_MEMSIG2(SAMB_head));
    dbg_prn("Size: %d\n",        MoX_SA_GET_SIZE(SAMB_head));
    dbg_prn("Used: %0d\n",       MoX_SA_GET_USED(SAMB_head));
#endif

    return is_valid;
}


/*
    MAGIC.EXE  seg008
*/

// MGC s08p07
MoX_SAMB_ptr Allocate_Space(MoX_word size)
{
    S32 lsize;
    MoX_SAMB_ptr SAMB_head;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: MoX_Allocate_Space( size = %u )\n", __FILE__, __LINE__, size);
#endif

    lsize = (size + 1) * 16;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] lsize: %ld )\n", __FILE__, __LINE__, lsize);
#endif
    MoX_tmp_SAMB_head = (MoX_SAMB_ptr) malloc(lsize);
    if ( MoX_tmp_SAMB_head == NULL ) { Allocation_Error(0x01, size); }
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] MoX_tmp_SAMB_head: %p\n", __FILE__, __LINE__, MoX_tmp_SAMB_head);
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] FP_SEG(MoX_tmp_SAMB_head): 0x%04X\n", __FILE__, __LINE__, FP_SEG(MoX_tmp_SAMB_head));
    dbg_prn("DEBUG: [%s, %d] MK_FP(FP_SEG(MoX_tmp_SAMB_head), 0): %Fp\n", __FILE__, __LINE__, MK_FP(FP_SEG(MoX_tmp_SAMB_head), 0));
    dbg_prn("DEBUG: [%s, %d] MK_FP((FP_SEG(MoX_tmp_SAMB_head) + 1), 0): %Fp\n", __FILE__, __LINE__, MK_FP((FP_SEG(MoX_tmp_SAMB_head) + 1), 0));
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] MoX_FP_SEG(MoX_tmp_SAMB_head): 0x%04X\n", __FILE__, __LINE__, MoX_FP_SEG(MoX_tmp_SAMB_head));
    dbg_prn("DEBUG: [%s, %d] MoX_MK_FP(MoX_FP_SEG(MoX_tmp_SAMB_head), 0): %Fp\n", __FILE__, __LINE__, (MoX_SAMB_ptr)MoX_MK_FP(MoX_FP_SEG(MoX_tmp_SAMB_head), 0));
    dbg_prn("DEBUG: [%s, %d] MoX_MK_FP((MoX_FP_SEG(MoX_tmp_SAMB_head) + 1), 0): %Fp\n", __FILE__, __LINE__, (MoX_SAMB_ptr)MoX_MK_FP((MoX_FP_SEG(MoX_tmp_SAMB_head) + 1), 0));
#endif
    // SAMB_head = (MoX_SAMB_ptr) MoX_PTR_ADD_PARAGRAPH(MoX_tmp_SAMB_head,1);
#if defined(__DOS16__)
    SAMB_head = (MoX_SAMB_ptr)MoX_MK_FP((MoX_FP_SEG(MoX_tmp_SAMB_head) + 1), 0);
#endif
#if defined(__WIN32__)
    SAMB_head = MoX_tmp_SAMB_head + 12;  // 16-byte paragraph - 4-byte malloc header
#endif

    MoX_SA_SET_MEMSIG1(SAMB_head);
    MoX_SA_SET_MEMSIG2(SAMB_head);
    MoX_SA_SET_SIZE(SAMB_head,size);
    MoX_SA_SET_USED(SAMB_head,1);

    // MoX_Check_Free();
#if defined(__DOS16__)
    Update_MemFreeWorst_KB();
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: MoX_Allocate_Space( size = %u ) { SAMB_head = %p }\n", __FILE__, __LINE__, size, SAMB_head);
#endif

    return SAMB_head;
}

// s08p08
MoX_SAMB_ptr Allocate_Space_No_Header(MoX_word size)
{
    S32 lsize;
    MoX_SAMB_ptr SAMB_data;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: MoX_Allocate_Space_No_Header( size = %u )\n", __FILE__, __LINE__, size);
#endif

    lsize = (size + 1) * 16;
    MoX_tmp_SAMB_head = (MoX_SAMB_ptr) malloc(lsize);
    if ( MoX_tmp_SAMB_head == NULL ) { Allocation_Error(0x01, size); }
    // SAMB_head = (MoX_SAMB_ptr) MoX_PTR_ADD_PARAGRAPH(MoX_tmp_SAMB_head,1);
#if defined(__DOS16__)
    SAMB_data = (MoX_SAMB_ptr)MoX_MK_FP((MoX_FP_SEG(MoX_tmp_SAMB_head) + 1), 0);
#endif
#if defined(__WIN32__)
    SAMB_data = MoX_tmp_SAMB_head + 12;  // 16-byte paragraph - 4-byte malloc header
#endif
    
    // MoX_Check_Free();
#if defined(__DOS16__)
    Update_MemFreeWorst_KB();
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: MoX_Allocate_Space_No_Header( size = %u ) { SAMB_data = %p }\n", __FILE__, __LINE__, size, SAMB_data);
#endif

    return SAMB_data;
}

// MGC s08p15
MoX_word Get_Free_Blocks(MoX_SAMB_ptr SAMB_head)
{
    return MoX_SA_GET_SIZE(SAMB_head) - MoX_SA_GET_USED(SAMB_head);
}

// MGC s08p19
void Allocation_Error(MoX_word error_num, MoX_word blocks)
{
    char buffer[120] = {0};
    char buffer2[20] = {0};

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: MoX_Allocation_Error( error_num = %d, blocks = %d)\n", __FILE__, __LINE__, error_num, blocks);
#endif

    // if(MoX_Check_Release_Version() == MoX_ST_TRUE)
    if(DBG_IsDisabled() == MoX_ST_TRUE)
    {
        strcpy(buffer, "Insufficient memory. You need at least ");
        // itoa(MoX_RAM_Min_KB, buffer2, 10);
        itoa(RAM_Min_KB, buffer2, 10);
        strcat(buffer, buffer2);
        strcat(buffer, "K free. Try removing all TSR's.");
    }
    else
    {
        switch(error_num)
        {
            case 1:
                strcpy(buffer, str_allocation_errors[4]);
                strcat(buffer, str_allocation_errors[5]);
                break;
            case 2:
                strcpy(buffer, str_allocation_errors[7]);
                strcat(buffer, str_allocation_errors[8]);
                strcat(buffer, str_allocation_errors[9]);
                break;
            case 3:
                strcpy(buffer, str_allocation_errors[10]);
                strcat(buffer, str_allocation_errors[8]);
                strcat(buffer, str_allocation_errors[5]);
                break;
            case 4:
                strcpy(buffer, str_allocation_errors[7]);
                strcat(buffer, str_allocation_errors[11]);
                strcat(buffer, str_allocation_errors[8]);
                strcat(buffer, str_allocation_errors[9]);
                break;
        }
        itoa(blocks, buffer2, 10);
        strcat(buffer, buffer2);
        strcat(buffer, str_allocation_errors[6]);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: MoX_Allocation_Error( error_num = %d, blocks = %d)\n", __FILE__, __LINE__, error_num, blocks);
#endif

    // MoX_Exit_With_Message(buffer);
    Exit(buffer);
}
