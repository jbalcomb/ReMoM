// _s13p16c.c EMM_Shutdown
// ST_EMM.H

#include "ST_EMM.H"

void EMM_Shutdown(void)
{
    unsigned int itrOpenHandles = 0;
//    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_Shutdown()\n", __FILE__, __LINE__);

//    dlvfprintf("DEBUG: gEMM_Open_Handles: %u\n", g_EMM_Open_Handles);

    while (itrOpenHandles < g_EMM_Open_Handles)
    {
//        dlvfprintf("DEBUG: gEMM_Open_Handles: %u;  itrOpenHandles: %u\n", g_EMM_Open_Handles, itrOpenHandles);
        EMM_ReleaseHandle(g_EMM_Table[itrOpenHandles].eEmmHndlNbr);
        itrOpenHandles++;
    }

    if (g_EMM_Overlay_Handle != 0)
    {
        EMM_ReleaseHandle(g_EMM_Overlay_Handle);
    }

//    dlvfprintf("DEBUG: [%s, %d] END: EMM_Shutdown()\n", __FILE__, __LINE__);
}
