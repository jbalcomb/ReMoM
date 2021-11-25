// _s12p03c.c EMM_GetFreePageCnt

#include "ST_HEAD.H"

unsigned int EMM_GetFreePageCount(void)
{
    _AH = EMS_GETPAGES;
    geninterrupt(EMM_INT);
    
    /* if _AH is non-zero, there was an error */
    //return(regs.h.ah ? ret: 0);

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
