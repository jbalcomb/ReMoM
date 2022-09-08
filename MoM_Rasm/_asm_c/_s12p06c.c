// _s12p06c.c EMM_GetHandleName
// ST_EMM.H

#include <DOS.H>

#include "ST_EMM.H"


unsigned int EMM_GetHandleName(char * EmmHndlNm, unsigned int EmmHndlNbr)
{
    union  REGS  inregs;
    union  REGS  outregs;
    struct SREGS segregs;
    //int isr_result;
    int ret_val;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: EMM_GetHandleName(EmmHndlNm=0, EmmHndlNbr=%u)\n", __FILE__, __LINE__, EmmHndlNbr);
#endif

    inregs.x.dx = EmmHndlNbr;
    segregs.es = FP_SEG(EmmHndlNm);
    inregs.x.di = FP_OFF(EmmHndlNm);
    inregs.h.ah = 0x53;  // INT 67,53 Get/Set Handle Name (LIM EMS 4.0+)
    inregs.h.al = 0x00;  // 0 = Get, 1 = Set
    int86x(EMS_INT, &inregs, &outregs, &segregs);

    if (outregs.h.ah != 0x00)
    {
        ret_val = 0x00;
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: EMM_GetHandleName(EmmHndlNm=0, EmmHndlNbr=%u) {ret_val=%d}\n", __FILE__, __LINE__, EmmHndlNbr, ret_val);
#endif
    }
    else
    {
        EmmHndlNm[8] = '\0';
        ret_val = 0xFF;
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: EMM_GetHandleName(EmmHndlNm=%s, EmmHndlNbr=%u) {ret_val=%d\n", __FILE__, __LINE__, EmmHndlNm, EmmHndlNbr, ret_val);
#endif
    }

    return ret_val;
}
