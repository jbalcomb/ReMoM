// _s13p12c.c EMM_LBX_HdrOnly

/*

*/


unsigned int EMM_LBX_HdrOnly(int EMM_Handle, char *EMM_Handle_Name, int LBX_Entry_Index, unsigned int AllocSeg, int Load_Type)
{
    unsigned int tmp_SI;
    unsigned int tmp_DI;
    int LBX_Entry_Count;
    unsigned int EntryTableOffset;
    long Entry_Offset_Begin;
    long Entry_Offset_End;

    int Header_Size;

    tmp_SI = AllocSeg;
    tmp_DI = 0;

    if ( EMM_MapnRead(&LBX_Entry_Index, 0, 0, 0, 2, EMM_Handle) < LBX_Entry_Index )
    {
        LBX_Error(EMM_Handle_Name, 0x08, LBX_Entry_Index);  // LBXErr_entries_exceeded
    }

    EntryTableOffset = ( 8 + ( LBX_Entry_Index * 4) );

    EMM_MapnRead(&Entry_Offset_Begin, 0, EntryTableOffset, 4, EMM_Handle);

    Header_Size = 1;

    if ( Load_Type == 0 )
    {
        //tmp_DI = AllocMem_EntrySize(Header_Size);
        tmp_DI = SA_Allocate_MemBlk(Header_Size);

        if ( tmp_DI == 0 )
        {
            LBX_Error(EMM_Handle_Name, 0x03, LBX_Entry_Index);  // LBXErr_low_RAM
        }
    }

    return AllocSeg;
}
