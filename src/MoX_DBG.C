
#include "MoX_DBG.H"

#include "MoX_TYPE.H"
#include "MoX_DEF.H"


/*
    Initialized Data
*/
// uint16_t release_version = MoX_ST_FALSE;                  // dseg:434E    XREF: DBG_Quit; DBG_ScreenDump; DBG_Disable; DBG_IsDisabled

#ifdef STU_DEBUG
uint16_t release_version = ST_FALSE;
#else
uint16_t release_version = ST_TRUE;                  // dseg:434E    XREF: DBG_Quit; DBG_ScreenDump; DBG_Disable; DBG_IsDisabled
#endif

// WZD s24p05
void Release_Version(void)
{
    release_version = ST_TRUE;
}

// WZD s24p06
int Check_Release_Version(void)
{
    return release_version;
}
