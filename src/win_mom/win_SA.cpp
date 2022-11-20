
#include "win_TYPE.hpp"

#include "win_SA.hpp"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

#include <malloc.h>
#include <stdlib.h>     /* itoa() */

SAMB_ptr g_SAMB;                        // MGC dseg:A5C0  WZD dseg:E5CA


/*
    MAGIC.EXE  seg008
*/

// s08p07
SAMB_ptr Allocate_Space(int size)
{
    long lsize;
    SAMB_ptr SAMB_head;
    lsize = (size + 1) * 16;
    g_SAMB = (SAMB_ptr)malloc(lsize);
    // if(g_SAMB == NULL) { Allocation_Error(1, size); }
    SAMB_head = g_SAMB + 12;

    SA_SET_MEMSIG1(SAMB_head);
    SA_SET_MEMSIG2(SAMB_head);
    SA_SET_SIZE(SAMB_head,size);
    SA_SET_USED(SAMB_head,1);

    return SAMB_head;
}


// s08p08/p09
SAMB_ptr Allocate_Space_No_Header(int size)
{
    long lsize;
    SAMB_ptr SAMB_data;
    lsize = (size + 1) * 16;
    g_SAMB = (SAMB_ptr)malloc(lsize);
    // if(g_SAMB == NULL) { Allocation_Error(1, size); }
    SAMB_data = g_SAMB + 12;

    return SAMB_data;
}
