// _s28p16c.c FLIC_GetCurFrame
// ST_FLIC.H

#include "ST_FLIC.H"
#include "STU_DBG.H"


unsigned int FLIC_GetCurFrame(unsigned int FlicHdr_SgmtAddr)
{
    unsigned int Current_Frame;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: FLIC_GetCurFrame(FlicHdr_SgmtAddr=0x%04X)\n", __FILE__, __LINE__, FlicHdr_SgmtAddr);
#endif

    Current_Frame = farpeekw(FlicHdr_SgmtAddr, FlicHdr_CurrentFrame);

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: FLIC_GetCurFrame(FlicHdr_SgmtAddr=0x%04X, Current_Frame=%u)\n", __FILE__, __LINE__, FlicHdr_SgmtAddr, Current_Frame);
#endif

    return Current_Frame;
}
