
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

#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoX_SA.H"

#include <malloc.h>
#include <stdlib.h>     /* itoa() */


extern int RAM_Min_KB;



// s07p04
int Check_Allocation(SAMB_ptr SAMB_head);

// s08p07
SAMB_ptr Allocate_Space(signed int size);
// s08p08
SAMB_ptr Allocate_Space_No_Header(signed int size);
// s08p15
unsigned int Get_Free_Blocks(SAMB_ptr SAMB_head);
// _s08p19
void Allocation_Error(int error_num, int blocks);

SAMB_ptr g_SAMB;  // rename - g_header, tmp_header_ptr, _header_ptr, ?!?

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



/*
    MAGIC.EXE  seg007
*/

// s07p04
int Check_Allocation(SAMB_ptr SAMB_head)
{
    int is_valid;
    
    if (SA_Get_MemSig1(SAMB_head) != SA_MEMSIG1 || SA_Get_MemSig2(SAMB_head) != SA_MEMSIG2)
    {
        is_valid = ST_FAILURE;
    }
    else
    {
        is_valid = ST_SUCCESS;
    }
    printf("MemSig1: 0x%04X\n", SA_Get_MemSig1(SAMB_head));
    printf("MemSig2: 0x%04X\n", SA_Get_MemSig2(SAMB_head));
    printf("Size: %d\n", SA_Get_Size(SAMB_head));
    printf("Used: %0d\n", SA_Get_Used(SAMB_head));

    return is_valid;
}


/*
    MAGIC.EXE  seg008
*/

// s08p07
SAMB_ptr Allocate_Space(signed int size)
{
    signed long lsize;
    SAMB_ptr SAMB_head;
    lsize = (size + 1) * 16;
    g_SAMB = (SAMB_ptr) malloc(lsize);
    if ( g_SAMB == NULL ) { Allocation_Error(0x01, size); }
    SAMB_head = (SAMB_ptr) PTR_ADD_PARAGRAPH(g_SAMB,1);
    SA_Set_MemSig1(SAMB_head);
    SA_Set_MemSig2(SAMB_head);
    SA_Set_Size(SAMB_head,size);
    SA_Set_Used(SAMB_head,1);
    // Check_Free();
    return SAMB_head;
}

// s08p08
SAMB_ptr Allocate_Space_No_Header(signed int size)
{
    signed long lsize;
    SAMB_ptr SAMB_data;
    lsize = (size + 1) * 16;
    g_SAMB = (SAMB_ptr) malloc(lsize);
    if ( g_SAMB == NULL ) { Allocation_Error(0x01, size); }
    SAMB_data = (SAMB_ptr) PTR_ADD_PARAGRAPH(g_SAMB,1);
    // Update_MemFreeWorst_KB();
    return SAMB_data;
}

// s08p15
unsigned int Get_Free_Blocks(SAMB_ptr SAMB_head)
{
    return SA_Get_Size(SAMB_head) - SA_Get_Used(SAMB_head);
}

// _s08p19
void Allocation_Error(int error_num, int blocks)
{
    char buffer[120] = {0};
    char buffer2[20] = {0};

    if ( DBG_IsDisabled() != 0 )
    {
        switch(error_num)
        {
            case 1:
                strcpy(buffer, cnst_Alloc_Error11);
                strcat(buffer, cnst_Alloc_Error12);
                break;
            case 2:
                strcpy(buffer, cnst_Alloc_Error21);
                strcat(buffer, cnst_Alloc_Error22);
                strcat(buffer, cnst_Alloc_Error23);
                break;
            case 3:
                strcpy(buffer, cnst_Alloc_Error3);
                strcat(buffer, cnst_Alloc_Error22);
                strcat(buffer, cnst_Alloc_Error12);
                break;
            case 4:
                strcpy(buffer, cnst_Alloc_Error21);
                strcat(buffer, cnst_Alloc_Error4);
                strcat(buffer, cnst_Alloc_Error22);
                strcat(buffer, cnst_Alloc_Error23);
                break;
        }
        itoa(blocks, buffer2, 10);
        strcat(buffer, buffer2);
        strcat(buffer, cnst_Alloc_Error13);
    }
    else
    {
        strcpy(buffer, cnst_Alloc_Error51);
        itoa(RAM_Min_KB, buffer2, 10);
        strcat(buffer, buffer2);
        strcat(buffer, cnst_Alloc_Error52);
    }

    Exit_With_Message(buffer);
}
