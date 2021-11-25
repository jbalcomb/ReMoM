// _s24p05c.c DBG_Disable
// in ST_DBG.H

#include "ST_DBG.H"

void DBG_Disable(void)
{
    g_Debug_Disabled = 1;
}
