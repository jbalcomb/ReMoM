#include "MoX_DBG.H"


#ifdef STU_DEBUG
int Debug_Disabled = 0;
#else
int Debug_Disabled = 1;                  // dseg:434E    XREF: DBG_Quit; DBG_ScreenDump; DBG_Disable; DBG_IsDisabled
#endif

// s24p05
void DBG_Disable(void)
{
    Debug_Disabled = 1;
}

// s24p06
int DBG_IsDisabled(void)
{
    return Debug_Disabled;
}
