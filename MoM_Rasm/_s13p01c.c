// _s13p01c.c EMM_Startup
// in ST_EMM.H

#include "ST_HEAD.H"

void EMM_Startup(void)
{
    int itr_active_handles;
    char varTmpStr100[100];
    int varEmmOpenHandleCount;
    int varItrEmmHndlNbrs;
//    printf("DEBUG: %s %d BEGIN: EMM_Startup()\n", __FILE__, __LINE__);

    if ( !EMM_Init() )
    {
        EMM_BuildEmmErrStr(&varTmpStr100);
        Quit(varTmpStr100);
    }

    varEmmOpenHandleCount = EMM_GetActiveHandleCount();

// - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

    g_EmmHndlNbr_YOMOMA = 0;

    itr_active_handles = 1;
    while (itr_active_handles < varEmmOpenHandleCount && g_EmmHndlNbr_YOMOMA == 0)
    {
//        printf("DEBUG: (%d, %d, %d) itr_active_handles < varEmmOpenHandleCount && EmmHndlNbr_YOMOMA == 0\n", itr_active_handles, varEmmOpenHandleCount, g_EmmHndlNbr_YOMOMA);
        if ( EMM_GetHandleName(varTmpStr100, itr_active_handles) )
        {
            if ( stricmp(g_EmmHndlNm_YOMOMA1, varTmpStr100) )
            {
//                printf("DEBUG: Found Existing YO MOMA\n");
                g_EmmHndlNbr_YOMOMA = itr_active_handles;
            }
        }
        itr_active_handles++;
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
        // unsigned int EMM_GetOrCreateHandle(unsigned int argEmmLogicalPageCount, char *argEmmHandleName, int argEmmRsvdFlag);
        g_EmmHndlNbr_YOMOMA = EMM_GetHandle(1, g_EmmHndlNm_YOMOMA2, 0);
    }

// - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

    if ( g_EmmHndlNbr_YOMOMA == 0 )
    {
        EMM_BuildEmmErrStr(varTmpStr100);
        Quit(varTmpStr100);
    }

    g_EMM_OK = 1;

    itr_active_handles = g_EmmHndlNbr_YOMOMA + 1;

// - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

    while (varItrEmmHndlNbrs < 50 && g_EMM_Open_Handles < 40)
    {

        _AX = EMM_GetHandleName(varTmpStr100, varItrEmmHndlNbrs);

        if (_AX == 0 || varTmpStr100 == 0)
        {
            continue;
        }
        else
        {
            g_EMM_Table[g_EMM_Open_Handles].eEmmRsrvd = 0;
            strcpy(g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm, varTmpStr100);
            g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNbr = varItrEmmHndlNbrs;
            g_EMM_Open_Handles++;
        }
        varItrEmmHndlNbrs++;
    }

// - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

    _AX = EMM_GetFreePageCount();  // returns the unallocated pages count (not the total pages count)

    if ( _AX < g_EMM_Pages_Reserved )
    {
        EMM_BuildEmmErrStr(varTmpStr100);
        Quit(varTmpStr100);
    }

    g_EmmHndlNbr_VGAFILEH = EMM_GetHandle(5, g_EmmHndlNm_VGAFILEH, 1);
    g_EmmHndlNbr_EMMDATAH = EMM_GetHandle(4, g_EmmHndlNm_EMMDATAH, 1);

    g_EMMDATAH_Level = 0;

//    printf("DEBUG: %s %d END: EMM_Startup()\n", __FILE__, __LINE__);
}
