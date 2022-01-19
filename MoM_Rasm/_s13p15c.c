// _s13p15c.c EMM_CheckHandleOpen

#include "ST_EMM.H"
#include "ST_LBX.H"

unsigned int EMM_CheckHandleOpen(char *EmmHndlNm)
{
    int itrEmmOpenHandles;
    unsigned int EmmHndl;

//    printf("DEBUG: [%s, %d]: BEGIN: EMM_CheckHandleOpen(EmmHndlNm = %s)\n", __FILE__, __LINE__, EmmHndlNm);

    itrEmmOpenHandles = 0;
    EmmHndl = 0;

    // printf("DEBUG: [%s, %d]: itrEmmOpenHandles = %d,  EmmHndl = %u, g_EMM_Open_Handles = %d\n", __FILE__, __LINE__, itrEmmOpenHandles, EmmHndl, g_EMM_Open_Handles);

    while ( (itrEmmOpenHandles < g_EMM_Open_Handles) && (EmmHndl == 0) )
    {
        // printf("DEBUG: [%s, %d]: EmmHndlNm = %s,  g_EMM_Table[%d].eEmmHndlNm = %s\n", __FILE__, __LINE__, EmmHndlNm, itrEmmOpenHandles, g_EMM_Table[itrEmmOpenHandles].eEmmHndlNm);

        if ( stricmp(g_EMM_Table[itrEmmOpenHandles].eEmmHndlNm, EmmHndlNm) == 0 )
        {
            EmmHndl = g_EMM_Table[itrEmmOpenHandles].eEmmHndlNbr;
            g_LBX_EmmRsvd = g_EMM_Table[itrEmmOpenHandles].eEmmRsrvd;
            // printf("DEBUG: [%s, %d]: SUCCESS: EmmHndl = %d,  g_LBX_EmmRsvd = %d\n", __FILE__, __LINE__, EmmHndl, g_LBX_EmmRsvd);
        }

        itrEmmOpenHandles++;
    }

//    printf("DEBUG: [%s, %d]: END: EMM_CheckHandleOpen() {EmmHndl = %u}\n", __FILE__, __LINE__, EmmHndl);
    return EmmHndl;
}
