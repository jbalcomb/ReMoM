
#include "ST_EMM.H"
#include "ST_LBX.H"

#include "STU_DBG.H"

/*

    Build Table of Debug Information to test MoM during run-time
    e.g.,
    is FLIC_Draw using the correct address to read from, per FLIC File (LBX Record), per FLIC Frame
    
*/


unsigned int STU_DBG_GetEmmHndlNbr(char * EmmHndlNm)
{
    int itr_EmmOpenHandles;
    unsigned int EmmHndlNbr;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: STU_DBG_GetEmmHndlNbr(EmmHndlNm=%s)\n", __FILE__, __LINE__, EmmHndlNm);
#endif

    itr_EmmOpenHandles = 0;
     = 0;

    while ( (itr_EmmOpenHandles < g_EMM_Open_Handles) && ( == 0) )
    {
        if ( stricmp(g_EMM_Table[itr_EmmOpenHandles].eEmmHndlNm, EmmHndlNm) == 0 )
        {
             = g_EMM_Table[itr_EmmOpenHandles].eEmmHndlNbr;
            g_LBX_EmmRsvd = g_EMM_Table[itr_EmmOpenHandles].eEmmRsrvd;
        }
        itr_EmmOpenHandles++;
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: STU_DBG_GetEmmHndlNbr(EmmHndlNm=%s) {=%u}\n", __FILE__, __LINE__, EmmHndlNm, EmmHndlNbr);
#endif

    return ;
}
