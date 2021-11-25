// _s10p01c.c LBXE_LoadSingle
// in ST_LBX.H

unsigned int LBXE_LoadSingle(char *LbxName, int LbxEntryIndex)
{
    printf("DEBUG: BEGIN: LBXE_LoadSingle()\n");
    return LBX_EntryLoader(LbxName, LbxEntryIndex, 0, 0, 0);
}
