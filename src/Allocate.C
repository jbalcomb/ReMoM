
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

#include "MoX_TYPE.H"   /* SAMB_ptr, etc. */
#include "MoX_DEF.H"    /* ST_FAILURE, etc. */
#include "MoX_BITS.H"   /* GET_2B_OFS() etc. */

#include "MoX_DBG.H"    /* Check_Release_Version() */

#include "Allocate.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

#include <malloc.h>     /* malloc() */
#include <stdlib.h>     /* itoa() */
#include <string.h>     /* strcat(), strcpy() */



SAMB_ptr tmp_SAMB_head;  // rename - ?pTmpSAMB?, g_header, tmp_header_ptr, _header_ptr, ?!?


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
int16_t Check_Allocation(SAMB_ptr SAMB_head)
{
    int16_t is_valid;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Check_Allocation( SAMB_head = %p )\n", __FILE__, __LINE__, SAMB_head);
#endif

    if (SA_GET_MEMSIG1(SAMB_head) != SA_MEMSIG1 || SA_GET_MEMSIG2(SAMB_head) != SA_MEMSIG2)
    {
        is_valid = ST_FAILURE;
    }
    else
    {
        is_valid = ST_SUCCESS;
    }

#ifdef STU_DEBUG
    dbg_prn("MemSig1: 0x%04X\n", SA_GET_MEMSIG1(SAMB_head));
    dbg_prn("MemSig2: 0x%04X\n", SA_GET_MEMSIG2(SAMB_head));
    dbg_prn("Size: %d\n",        SA_GET_SIZE(SAMB_head));
    dbg_prn("Used: %0d\n",       SA_GET_USED(SAMB_head));
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Check_Allocation( SAMB_head = %p ) { is_valid = %d }\n", __FILE__, __LINE__, SAMB_head, is_valid);
#endif

    return is_valid;
}


/*
    MAGIC.EXE  seg008
*/

// MGC s08p07
SAMB_ptr Allocate_Space(uint16_t size)
{
    int32_t lsize;
    SAMB_ptr SAMB_head;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Allocate_Space( size = %u )\n", __FILE__, __LINE__, size);
#endif

    lsize = (size + 1) * 16;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] lsize: %ld\n", __FILE__, __LINE__, lsize);
#endif

    tmp_SAMB_head = (SAMB_ptr) malloc(lsize);
    if(tmp_SAMB_head == NULL) { Allocation_Error(1, size); }
    SAMB_head = tmp_SAMB_head + 12;  // 16-byte paragraph - 4-byte malloc header

    SA_SET_MEMSIG1(SAMB_head);
    SA_SET_MEMSIG2(SAMB_head);
    SA_SET_SIZE(SAMB_head,size);
    SA_SET_USED(SAMB_head,1);

    // Update_MemFreeWorst_KB();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Allocate_Space( size = %u ) { SAMB_head = %p }\n", __FILE__, __LINE__, size, SAMB_head);
#endif

    return SAMB_head;
}

// s08p08
SAMB_ptr Allocate_Space_No_Header(uint16_t size)
{
    int32_t lsize;
    SAMB_ptr SAMB_data;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Allocate_Space_No_Header( size = %u )\n", __FILE__, __LINE__, size);
#endif

    lsize = (size + 1) * 16;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] lsize: %ld\n", __FILE__, __LINE__, lsize);
#endif

    tmp_SAMB_head = (SAMB_ptr) malloc(lsize);
    if(tmp_SAMB_head == NULL) { Allocation_Error(1, size); }
    SAMB_data = tmp_SAMB_head + 12;  // 16-byte paragraph - 4-byte malloc header

    // Update_MemFreeWorst_KB();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: Allocate_Space_No_Header( size = %u ) { SAMB_data = %p }\n", __FILE__, __LINE__, size, SAMB_data);
#endif

    return SAMB_data;
}


// MGC s08p12
SAMB_ptr Allocate_First_Block(SAMB_ptr SAMB_head, uint16_t size)
{
    SAMB_ptr SAMB_data;

    SAMB_data = ST_NULL;

    return SAMB_data;
}

// MGC s08p14
SAMB_ptr Allocate_Next_Block(SAMB_ptr SAMB_head, uint16_t size)
{
    SAMB_ptr SAMB_data;

    SAMB_data = ST_NULL;

    return SAMB_data;
}


// MGC s08p15
uint16_t Get_Free_Blocks(SAMB_ptr SAMB_head)
{
    return SA_GET_SIZE(SAMB_head) - SA_GET_USED(SAMB_head);
}

// MGC s08p19
void Allocation_Error(uint16_t error_num, uint16_t blocks)
{
    char buffer[120] = {0};
    char buffer2[20] = {0};

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: MoX_Allocation_Error( error_num = %d, blocks = %d)\n", __FILE__, __LINE__, error_num, blocks);
#endif

    if(Check_Release_Version() == ST_TRUE)
    {
        strcpy(buffer, "Insufficient memory. You need at least ");
        itoa(640, buffer2, 10);
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

    // Exit_With_Message(buffer);
}
