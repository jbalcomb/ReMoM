// _s10p03c.c LBXE_LoadAppend
// ST_LBX.H

unsigned int LBXE_LoadAppend(char *LbxName, int LbxEntryIndex, unsigned int SAMB)
{
    return LBX_Load_Entry(LbxName, LbxEntryIndex, SAMB, 2, 0);
}
