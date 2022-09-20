// _s28p16c.c FLIC_GetCurFrame
// ST_FLIC.H

// #include "ST_TYPE.H"

#include "ST_FLIC.H"

#include "STU_DBG.H"


word FLIC_GetCurFrame(SAMB_addr sa_FLIC_Header)
{
    word Current_Frame;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: FLIC_GetCurFrame(sa_FLIC_Header=0x%04X)\n", __FILE__, __LINE__, sa_FLIC_Header);
#endif

    Current_Frame = farpeekw(sa_FLIC_Header, FlicHdr_CurrentFrame);

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: FLIC_GetCurFrame(sa_FLIC_Header=0x%04X, Current_Frame=%u)\n", __FILE__, __LINE__, sa_FLIC_Header, Current_Frame);
#endif

    return Current_Frame;
}
