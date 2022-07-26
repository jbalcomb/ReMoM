// _s12p02c.c EMM_GetHandleCount

#include "ST_HEAD.H"
#include "MOM_DEF.H"
#include "ST_EMM.H"


unsigned int EMM_GetActiveHandleCount(void)
{

    _AH = EMS_GETHANDLES;
    geninterrupt(EMS_INT);

    return(_BX);
}
