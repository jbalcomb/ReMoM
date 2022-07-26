// _s13p15c.c EMM_CheckHandleOpen

#include "ST_EMM.H"
#include "ST_LBX.H"

#include "STU_DBG.H"

/*

    Return:
        0   Failure
        EMM Handle Number
*/
unsigned int EMM_CheckHandleOpen(char *EmmHndlNm)
{
    int itrEmmOpenHandles;
    unsigned int EmmHndl;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: EMM_CheckHandleOpen(EmmHndlNm=%s)\n", __FILE__, __LINE__, EmmHndlNm);
#endif

    itrEmmOpenHandles = 0;
    EmmHndl = 0;
    while ( (itrEmmOpenHandles < g_EMM_Open_Handles) && (EmmHndl == 0) )
    {
        if ( stricmp(g_EMM_Table[itrEmmOpenHandles].eEmmHndlNm, EmmHndlNm) == 0 )
        {
            EmmHndl = g_EMM_Table[itrEmmOpenHandles].eEmmHndlNbr;
            g_LBX_EmmRsvd = g_EMM_Table[itrEmmOpenHandles].eEmmRsrvd;
        }
        itrEmmOpenHandles++;
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: EMM_CheckHandleOpen(EmmHndlNm=%s) {EmmHndl=%u}\n", __FILE__, __LINE__, EmmHndlNm, EmmHndl);
#endif

    return EmmHndl;
}
