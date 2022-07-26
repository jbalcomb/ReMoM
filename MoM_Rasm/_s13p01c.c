// _s13p01c.c EMM_Startup
// in ST_EMM.H

#include "ST_HEAD.H"

#include "STU_DBG.H"


/*
    checks for EMS
    checks if any active handles are named "YO MOMA"
    sets that handle number or gets a new one and updates g_EMM_Open_Handles
    sets g_EMM_OK
    adds additional handles to g_EMM_Table and updates g_EMM_Open_Handles
    gets handles for VGAFILEH and EMMDATAH (and sets g_EMMDATAH_Level=0)
*/
void EMM_Startup(void)
{
    int status_emm_init;
    int itr_active_handles;
    char varTmpStr100[100];
    int varEmmOpenHandleCount;
    unsigned int itr_EmmHndlNbrs;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_Startup()\n", __FILE__, __LINE__);
#endif

    // if ( !EMM_Init() )
    // {
    //     EMM_BuildEmmErrStr(&varTmpStr100);
    //     Quit(varTmpStr100);
    // }
    status_emm_init = EMM_Init();  // return: {0xFF,0x00}/{-1,0}/{ST_FAILURE,ST_SUCCESS}
    if ( status_emm_init == 0x00)
    {
        EMM_BuildEmmErrStr(varTmpStr100);
        Quit(varTmpStr100);
    }

    varEmmOpenHandleCount = EMM_GetActiveHandleCount();

// - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

    g_EmmHndlNbr_YOMOMA = 0;

    for (itr_active_handles = 1; ((itr_active_handles <= varEmmOpenHandleCount) && (g_EmmHndlNbr_YOMOMA == 0)); itr_active_handles++)
    {
        if ( EMM_GetHandleName(varTmpStr100, itr_active_handles) )
        {
            if ( stricmp(g_EmmHndlNm_YOMOMA1, varTmpStr100) )
            {
                g_EmmHndlNbr_YOMOMA = itr_active_handles;
            }
        }
    }

// - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

    g_EMM_Open_Handles = 0;

    if (g_EmmHndlNbr_YOMOMA != 0)
    {
        strcpy(g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm, g_EmmHndlNm_YOMOMA1);
        g_EMM_Table[g_EMM_Open_Handles].eEmmRsrvd = 0;
        g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNbr = g_EmmHndlNbr_YOMOMA;
        g_EMM_Open_Handles = 1;
    }
    else
    {
        g_EmmHndlNbr_YOMOMA = EMM_GetHandle(1, g_EmmHndlNm_YOMOMA2, 0);  //   // 1 page/16KB, EmmRsvd=FALSE  NOTE: EMM_GetHandle() increments g_EMM_Open_Handles
    }

// - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

    if ( g_EmmHndlNbr_YOMOMA == 0 )
    {
        EMM_BuildEmmErrStr(varTmpStr100);
        Quit(varTmpStr100);
    }

    g_EMM_OK = 1;

// - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

    itr_EmmHndlNbrs = g_EmmHndlNbr_YOMOMA + 1;

    while (itr_EmmHndlNbrs < 50 && g_EMM_Open_Handles < 40)
    {
        if (EMM_GetHandleName(varTmpStr100, itr_EmmHndlNbrs) != 0 && varTmpStr100[0] != 0)
        {
            g_EMM_Table[g_EMM_Open_Handles].eEmmRsrvd = 0;
            strcpy(g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm, varTmpStr100);
            g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNbr = itr_EmmHndlNbrs;
            g_EMM_Open_Handles++;
        }
        itr_EmmHndlNbrs++;
    }

// - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

    if ( EMM_GetFreePageCount() < g_EMM_Pages_Reserved )  // returns the unallocated pages count (not the total pages count)
    {
        EMM_BuildEmmErrStr(varTmpStr100);
        Quit(varTmpStr100);
    }

    g_EmmHndlNbr_VGAFILEH = EMM_GetHandle(5, g_EmmHndlNm_VGAFILEH, 1);  // 5 pages/80KB, EmmRsvd=TRUE
    g_EmmHndlNbr_EMMDATAH = EMM_GetHandle(4, g_EmmHndlNm_EMMDATAH, 1);  // 4 pages/64KB, EmmRsvd=TRUE
    g_EMMDATAH_Level = 0;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: EMM_Startup()\n", __FILE__, __LINE__);
#endif

}
