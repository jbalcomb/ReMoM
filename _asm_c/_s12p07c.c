// _s12p07c.c EMM_ReleaseHandle

#include "ST_HEAD.H"


void EMM_ReleaseHandle(unsigned int EMM_Handle)
{
//    printf("DEBUG: %s %d BEGIN: EMM_ReleaseHandle()\n", __FILE__, __LINE__);

    if (EMM_Handle != 0)
    {
        _DX = EMM_Handle;
        _AH = EMS_FREEPAGES;
        geninterrupt(EMS_INT);
    }

//    printf("DEBUG: %s %d END: EMM_ReleaseHandle()\n", __FILE__, __LINE__);
}
