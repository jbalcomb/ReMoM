// _s13p11c.c EMM_LBX_EntryLoader

#include "ST_EMM.H"
#include "ST_LBX.H"


unsigned int EMM_LBX_EntryLoader(char *EmmHndlNm, int LbxEntryIndex, int AllocSeg, int LoadType, int FormatType)
{
// Header_Offset= dword ptr -18h
// Total_Bytes_Left= dword ptr -14h
// Entry_Offset= dword ptr -10h
// Amt_Paragraphs= word ptr -0Ch
// Load_Segment= word ptr -0Ah
// Bytes_To_Copy= word ptr -8
// Resource_Type= word ptr -6
// Entry_Count= word ptr -4
// EMM_Handle= word ptr -2

    unsigned int EmmHndl;
    unsigned int LbxEntryCount;
    unsigned int LbxFileType;
    // Bytes_To_Copy= word ptr -8
    // Load_Segment= word ptr -0Ah
    // Amt_Paragraphs= word ptr -0Ch
    long EntryOffsetBegin;
    long EntryOffsetEnd;
    long EntryTableOffset;
    
    unsigned int tmpSegmentAddress;
    unsigned int tmpHeaderOffsetLo;

    EmmHndl =  EMM_CheckHandleOpen(EmmHndlNm);

    if ( EmmHndl == 0 )
    {
        return 0;
    }

    if ( g_LBX_EmmRsvd == 1 )
    {
        EMM_MapnRead(LbxFileType, 0, 6, 0, 2, EmmHndl);  // ? read 2 bytes at offset 6 ? ...in the LBX Header Preamble ? ...which is 'LBX File Type' ?

        if ( LbxFileType != 0 )
        {
            LBX_Error(EmmHndlNm, 0x0D, LbxEntryIndex);  // LBXErr_cantload_reserved
        }

        //tmpSegmentAddress = EMM_LBX_HdrOnly(EmmHndl, EmmHndlNm, LbxEntryIndex, AllocSeg, LoadType, FormatType);
        tmpSegmentAddress = EMM_LBX_HdrOnly(EmmHndl, EmmHndlNm, LbxEntryIndex, AllocSeg, LoadType);

        return tmpSegmentAddress;
    }
    
    EMM_MapnRead(LbxEntryCount, 0, 0, 0, 2, EmmHndl);  // ? read 2 bytes at offset 0 ? ...in the LBX Header Preamble ? ...which is 'LBX Entry Count' ?

    if ( LbxEntryCount < LbxEntryIndex )
    {
        //LBX_Error(LbxName, 0x08, LbxEntryIndex);  // LBXErr_entries_exceeded
        return 0;
    }

//   if ( FormatType != 0 )
//   {
//       tmpHeaderOffsetLo = LbxEntryIndex * 4 + 520;  // ? 520 = 512 + 8 ?
//   }
//   else
//   {
//       tmpHeaderOffsetLo = LbxEntryIndex * 4 + 8;  // Entry Index * Entry Offset Size + Header Preamble Size
//   }
//
//    EntryTableOffset = (0 << 8) | (tmpHeaderOffsetLo);
//
//    EMM_MapnRead(&EntryOffsetBegin, 0, EntryTableOffset, EntryTableOffset, 4, EmmHndl);
//
//    EntryTableOffset = (0 << 8) | (tmpHeaderOffsetLo + 4);
//
//    EMM_MapnRead(&EntryOffsetEnd, 0, EntryTableOffset, EntryTableOffset, 4, EmmHndl);

    return 0;
}
