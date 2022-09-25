
#include "MoX_SA.hpp"

#include "MoX_ST_HEAD.hpp"


// s07p04
/*
    pSAMB_head:
        pointer to 'SAMB Header', as returned by SA_Allocate_Space()  (~AKA 'Load Type' 1)
    Return:
        ST_STATUS: ST_SUCCESS, ST_FAILURE
*/
int SA_Alloc_Validate(SAMB_ptr pSAMB_head)
{
    unsigned int memsig1;
    unsigned int memsig2;
    int is_valid;
 
    memsig1 = ((unsigned int)*((unsigned char *)pSAMB_head + 4)) | (unsigned int)((unsigned int)*((unsigned char *)pSAMB_head + 5) << 8);
    memsig2 = ((unsigned int)*((unsigned char *)pSAMB_head + 6)) | (unsigned int)((unsigned int)*((unsigned char *)pSAMB_head + 7) << 8);

    if (memsig1 != SA_MEMSIG1 || memsig2 != SA_MEMSIG2)
    {
        is_valid = ST_FAILURE;
    }
    else
    {
        is_valid = ST_SUCCESS;
    }

    return is_valid;
}
