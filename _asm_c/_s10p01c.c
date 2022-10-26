// _s10p01c.c LBXE_LoadSingle
// ST_LBX.H

#include "ST_HEAD.H"

#include "STU_DBG.H"


unsigned int LBXE_LoadSingle(char *LbxName, int LbxEntryIndex)
{
    sgmt_addr SAMB_head;
    int LoadType;
    int LbxHdrFmt;

#ifdef DEBUG
    if ( (strcmp(LbxName, "MAINSCRN") == 0) && (LbxEntryIndex == 0) )
    {
        dlvfprintf("DEBUG: [%s, %d] BEGIN: LBXE_LoadSingle(LbxName=%s, LbxEntryIndex=%d)\n", __FILE__, __LINE__, LbxName, LbxEntryIndex);
    }
#endif

    SAMB_head = 0;
    LoadType = 0;
    LbxHdrFmt = 0;

    return LBX_Load_Entry(LbxName, LbxEntryIndex, SAMB_head, LoadType, LbxHdrFmt);
}
