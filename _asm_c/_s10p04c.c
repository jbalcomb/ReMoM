// _s10p04c.c LBXR_LoadSingle
// ST_LBX.H

unsigned int LBXR_LoadSingle(char *LbxName, int LbxEntryIndex, int RecFirst, int RecCount, int RecSize)
{
    unsigned int SAMB_head = 0;
    int LoadType = 0;

    LBX_Load_Record(LbxName, LbxEntryIndex, SAMB_head, LoadType, RecFirst, RecCount, RecSize);

}
