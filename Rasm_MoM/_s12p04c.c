// _s12p04c.c EMM_GetHandlePageCount

#include "ST_HEAD.H"

// IDA: int __cdecl __far EMM_GetHandlePageCount(int EMM_Handle)
unsigned int EMM_GetHandlePageCount(unsigned int EmmHndlNbr)
{
    _DX = EmmHndlNbr;
    _AH = EMS_GETHNDLPAGECNT;
    geninterrupt(EMM_INT);

    if (_AH)
    {
        printf("DEBUG: _AH: 0x%02X\n", _AH);
        return(0);
    }
    else
    {
        return(_BX);
    }
}
