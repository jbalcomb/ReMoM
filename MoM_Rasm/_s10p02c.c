// _s10p02c.c LBXE_LoadReplace
// ST_LBX.H

#include "ST_HEAD.H"

#include "STU_DBG.H"


unsigned int LBXE_LoadReplace(char *LbxName, int LbxEntryIndex, unsigned int SAMB_head)
{
    int LoadType;
    int LbxHdrFmt;

    if ( (strcmp(LbxName, "FONTS.LBX") == 0) && (LbxEntryIndex == 2) )
    {
        dlvfprintf("DEBUG: [%s, %d] BEGIN: LBXE_LoadReplace(LbxName=%s, LbxEntryIndex=%d, SAMB_head=0x%04X)\n", __FILE__, __LINE__, LbxName, LbxEntryIndex, SAMB_head);
    }

    LoadType = 1;
    LbxHdrFmt = 0;

    return LBX_Load_Entry(LbxName, LbxEntryIndex, SAMB_head, LoadType, LbxHdrFmt);
}
