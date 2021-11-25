// _s13p15c.c EMM_CheckHandleOpen

#include "ST_EMM.H"
#include "ST_LBX.H"

unsigned int EMM_CheckHandleOpen(char *EmmHndlNm)
{
    int itrEmmOpenHandles = 0;
    unsigned int EmmHndl = 0;

    while ( (itrEmmOpenHandles < g_EMM_Open_Handles) && (EmmHndl == 0) )
    {

        if ( stricmp(g_EMM_Table[itrEmmOpenHandles].eEmmHndlNm, EmmHndlNm) )
        {
            EmmHndl = g_EMM_Table[itrEmmOpenHandles].eEmmHndlNbr;
            g_LBX_EmmRsvd = g_EMM_Table[itrEmmOpenHandles].eEmmRsrvd;
        }

        itrEmmOpenHandles++;
    }

    return EmmHndl;
}
