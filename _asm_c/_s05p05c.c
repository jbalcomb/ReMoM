// _s05p05c.c Update_MemFreeWorst_KB  RAM_Update_WorstFree
// in MOM_DEF.H

#include "MOM_HEAD.H"

void Update_MemFreeWorst_KB(void)
{
    unsigned int MemFree_KB;

    MemFree_KB = dos_memfree_kb();

    if (MemFree_KB < g_MemFreeWorst_KB)
    {
        g_MemFreeWorst_KB = MemFree_KB;
    }

}
