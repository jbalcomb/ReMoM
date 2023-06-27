
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

    if (SA_GET_MEMSIG1(SAMB_head) != SA_MEMSIG1 || SA_GET_MEMSIG2(SAMB_head) != SA_MEMSIG2)
    {
        is_valid = ST_FAILURE;
    }
    else
    {
        is_valid = ST_SUCCESS;
    }

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

    lsize = (size + 1) * 16;

    tmp_SAMB_head = (SAMB_ptr) malloc(lsize);
    if(tmp_SAMB_head == NULL) { Allocation_Error(1, size); }
    SAMB_head = tmp_SAMB_head + 12;  // 16-byte paragraph - 4-byte malloc header

    SA_SET_MEMSIG1(SAMB_head);
    SA_SET_MEMSIG2(SAMB_head);
    SA_SET_SIZE(SAMB_head,size);
    SA_SET_USED(SAMB_head,1);

    // Update_MemFreeWorst_KB();

    return SAMB_head;
}

// s08p08
SAMB_ptr Allocate_Space_No_Header(uint16_t size)
{
    int32_t lsize;
    SAMB_ptr SAMB_data;

    lsize = (size + 1) * 16;

    tmp_SAMB_head = (SAMB_ptr) malloc(lsize);
    if(tmp_SAMB_head == NULL) { Allocation_Error(1, size); }
    SAMB_data = tmp_SAMB_head + 12;  // 16-byte paragraph - 4-byte malloc header

    // Update_MemFreeWorst_KB();

    return SAMB_data;
}


// MGC s08p12
SAMB_ptr Allocate_First_Block(SAMB_ptr SAMB_head, uint16_t size)
{
    SAMB_ptr sub_SAMB_head;
    SAMB_ptr sub_SAMB_data;

    if(Check_Allocation(SAMB_head) == ST_FALSE) { Allocation_Error(0x03, size+1); }
    if(SA_GET_SIZE(SAMB_head) < size) { Allocation_Error(0x02, size+1); }

    // block header + sub block header + sub block size
    SA_SET_USED(SAMB_head,1+(1+size));

    sub_SAMB_head = SAMB_head + 16;  // ~== &SAMB_head[16]

    SA_SET_MEMSIG1(sub_SAMB_head);
    SA_SET_MEMSIG2(sub_SAMB_head);
    SA_SET_SIZE(sub_SAMB_head,size);
    SA_SET_USED(sub_SAMB_head,1);
    SA_SET_MARK(sub_SAMB_head,1);

    sub_SAMB_data = sub_SAMB_head + 16;  // ~== &sub_SAMB_head[16]

    return sub_SAMB_data;
}

// MGC s08p14
SAMB_ptr Allocate_Next_Block(SAMB_ptr SAMB_head, uint16_t size)
{
    SAMB_ptr sub_SAMB_head;
    SAMB_ptr sub_SAMB_data;
    uint16_t old_used;
    uint16_t new_used;
    
    if(Check_Allocation(SAMB_head) == ST_FALSE) { Allocation_Error(0x03, size); }
    if(Get_Free_Blocks(SAMB_head) < size+1) { Allocation_Error(0x02, size + 1); }

    old_used = SA_GET_USED(SAMB_head);

    new_used = (SA_GET_USED(SAMB_head) + size + 1);

    SA_SET_USED(SAMB_head, new_used);
    
    sub_SAMB_head = SAMB_head + (old_used * 16);  // ~== &SAMB_head[SAMB->used]

    SA_SET_MEMSIG1(sub_SAMB_head);
    SA_SET_MEMSIG2(sub_SAMB_head);
    SA_SET_SIZE(sub_SAMB_head,size);
    SA_SET_USED(sub_SAMB_head,1);
    // SA_SET_MARK(sub_SAMB_head,1);

    sub_SAMB_data = sub_SAMB_head + 16;  // ~== &sub_SAMB_head[16]

    return sub_SAMB_data;
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

    if(Check_Release_Version() == ST_TRUE)
    {
        strcpy(buffer, "Insufficient memory. You need at least ");
#pragma warning(suppress : 4996)
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
#pragma warning(suppress : 4996)
        itoa(blocks, buffer2, 10);
        strcat(buffer, buffer2);
        strcat(buffer, str_allocation_errors[6]);
    }

    // TODO  Exit_With_Message(buffer);
}
