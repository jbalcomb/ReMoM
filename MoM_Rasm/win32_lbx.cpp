
/*
##### seg009    ST_LBX.H    ALL ASM
_s09p01     lbx_open                    int  lbx_open(char *fname);
_s09p02     lbx_close                   int  lbx_close(int fhandle);
_s09p03     lbx_seek                    long lbx_seek(long foffset, int fhandle);
_s09p05a.c  lbx_read_sgmt               int  lbx_read_sgmt(unsigned int dst_sgmt, int nbytes, int fhandle);
_s09p06a.c  lbx_read_ofst               int  lbx_read_ofst(unsigned int dst_ofst, int nbytes, int fhandle);
_s09p07     strcpyfar                   void strcpyfar(unsigned int dstoff, unsigned int dstseg, unsigned int srcoff, unsigned int srcseg);

##### seg010    ST_LBX.H
_s10p01         LBXE_LoadSingle             sgmt_addr LBXE_LoadSingle(char *LbxFileName, int LbxEntryIndex);
_s10p02         LBXE_LoadReplace            sgmt_addr LBXE_LoadReplace(char *LbxFileName, int LbxEntryIndex, unsigned int SAMB);
_s10p03         LBXE_LoadAppend             sgmt_addr LBXE_LoadAppend(char *LbxFileName, int LbxEntryIndex, unsigned int SAMB);
_s10p04c.c      LBXR_LoadSingle

_s10p10c.c      LBX_Load_Entry              sgmt_addr LBX_EntryLoader()       LBX_EntryLoader
_s10p11c.c      LBX_Load_Load_Record              LBX_RecordLoader
[]_s10p12c.c      LBXR_DirectLoader
[]_s10p13c.c      LBX_GetEntryData

_s10p15c.c      LBX_Error
_s10p16.asm     ExtractFileBase             ExtractFileBase(char * FileName)  STR_StripFileExt
_s10p17c.c      RAM_SetMinKB                RAM_SetMinKB(int RAM_MinKbytes)

*/
// extern uint8_t *lbxfile_item_get(lbxfile_e file_id, uint16_t entry_id);

unsigned int LBXE_LoadSingle(char *LbxName, int LbxEntryIndex)
{
    unsigned int SAMB_head;
    int LoadType;
    int LbxHdrFmt;

    SAMB_head = 0;
    LoadType = 0;
    LbxHdrFmt = 0;

    return LBX_Load_Entry(LbxName, LbxEntryIndex, SAMB_head, LoadType, LbxHdrFmt);
}

unsigned int LBX_Load_Entry(char *LbxName, int LbxEntry, unsigned int SAMB_head, int LoadType, int LbxHdrFmt)
{
    unsigned int SAMB_data;
    SAMB_data = 0;
    return SAMB_data;
}
