// _s24p06c.c DBG_IsDisabled
// in ST_DBG.H

#include "ST_DBG.H"

int DBG_IsDisabled(void)
{
    return g_Debug_Disabled;
}
