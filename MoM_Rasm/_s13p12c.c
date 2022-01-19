// _s13p12c.c EMM_LBX_HdrOnly
// ST_EMM.H

#include "ST_HEAD.H"

#include "ST_EMM.H"
#include "ST_FLIC.H"
#include "ST_LBX.H"

#include "STU_DBG.H"


unsigned int EMM_LBX_HdrOnly(int EmmHndl, char *EmmHndlNm, int LbxEntry, unsigned int SAMB_head, int LoadType)
{
    sgmt_addr SAMB_data;
    static unsigned int LbxEntryCount;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    long EntryTableOffset;
    unsigned int tmp_EntryTableOffset_Sgmt;
    unsigned int tmp_EntryTableOffset_Ofst;
    static long DataOffset_Begin;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    unsigned int tmp_DataOffsetBegin_Sgmt;
    unsigned int tmp_DataOffsetBegin_Ofst;
    unsigned long DataSize_Paras;   // LBXLOADTYPE()
    unsigned int tmp_SAMB_Size;     // LBXLOADTYPE()
    unsigned char Entry_Begin_Logical_Page_Number;
    unsigned int Entry_Begin_Logical_Page_Offset;

//    printf("DEBUG: [%s, %d]: BEGIN: EMM_LBX_HdrOnly(EmmHndl = %d, EmmHndlNm = %s, LbxEntry = %d, SAMB_head = 0x%04X, LoadType = %d)\n", __FILE__, __LINE__, EmmHndl, EmmHndlNm, LbxEntry, SAMB_head, LoadType);
    
    if ( (strcmp(EmmHndlNm, "MAINSCRN") == 0) && (LbxEntry == 0) )
    {
        dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_LBX_HdrOnly(EmmHndl=%d, EmmHndlNm=%s, LbxEntry=%d, SAMB_head=0x%04X, LoadType=%d)\n", __FILE__, __LINE__, EmmHndl, EmmHndlNm, LbxEntry, SAMB_head, LoadType);
    }

    SAMB_data = ST_FAILURE;

    /*
        ? same code as in EMM_LBX_EntryLoader() ?
            EMM_MapnRead - LbxEntryCount
            EMM_MapnRead - EntryOffsetBegin
    */
    EMM_MapnRead((unsigned int)&LbxEntryCount, 0, 0, 0, 2, EmmHndl);

    if ( LbxEntryCount < LbxEntry )
    {
        LBX_Error(EmmHndlNm, 0x08, LbxEntry);  // LBXErr_entries_exceeded
    }

    /*
        BEGIN: LBX Header Offset
    */
    EntryTableOffset = ((4 * LbxEntry) + 8);
    /*
        END: LBX Header Offset
    */

    tmp_EntryTableOffset_Sgmt = (unsigned int)(EntryTableOffset >> 16);         // Hi-Word of Double-Word
    tmp_EntryTableOffset_Ofst = (unsigned int)(EntryTableOffset & 0x0000FFFF);  // Lo-Word of Double-Word
    EMM_MapnRead((unsigned int)&DataOffset_Begin, 0, tmp_EntryTableOffset_Ofst, tmp_EntryTableOffset_Sgmt, 4, EmmHndl);
    
    /*
        BEGIN: Load Type
    */
    DataSize_Paras = 1;  // sizeof FLIC_HDR
    EMMLBXLOADTYPE()
    /*
        END: Load Type
    */

    if ( (strcmp(EmmHndlNm, "MAINSCRN") == 0) && (LbxEntry == 0) )
    {
        dlvfprintf("DEBUG: [%s, %d] SAMB_data=0x%04X\n", __FILE__, __LINE__, SAMB_data);
    }

    Entry_Begin_Logical_Page_Number = DataOffset_Begin / 16384;  // e_EMM_Page_Size (0x4000)
    Entry_Begin_Logical_Page_Offset = DataOffset_Begin % 16384;  // e_EMM_Page_Size (0x4000)

    /*
        BEGIN: Read Data
    */
    tmp_DataOffsetBegin_Sgmt = (unsigned int)(DataOffset_Begin >> 16);         // Hi-Word of Double-Word
    tmp_DataOffsetBegin_Ofst = (unsigned int)(DataOffset_Begin & 0x0000FFFF);  // Lo-Word of Double-Word
    EMM_MapnRead(0, SAMB_data, tmp_DataOffsetBegin_Ofst, tmp_DataOffsetBegin_Sgmt, 16, EmmHndl);
    /*
        END: Read Data
    */

    if ( (strcmp(EmmHndlNm, "MAINSCRN") == 0) && (LbxEntry == 0) )
    {
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_Width=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_Width));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_Height=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_Height));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_CurrentFrame=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_CurrentFrame));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_FrameCount=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_FrameCount));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_LoopFrame=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_LoopFrame));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmHandleNumber=%d\n", __FILE__, __LINE__, farpeekb(SAMB_data, FlicHdr_EmmHandleNumber));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmLogicalPageIndex=%d\n", __FILE__, __LINE__, farpeekb(SAMB_data, FlicHdr_EmmLogicalPageIndex));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmLogicalPageOffset=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_EmmLogicalPageOffset));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_PaletteDataOffset=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_PaletteDataOffset));
    }

    // TODO(JimBalcomb): add defines for offsets and struct for LBX_FLIC_HDR
    farpokeb(SAMB_data, 10, (char)EmmHndl);                          // 10 0x0A LBX_FLIC_HDR.EMM_Handle_Number
    farpokeb(SAMB_data, 11, (char)Entry_Begin_Logical_Page_Number);  // 11 0x0B LBX_FLIC_HDR.EMM_Logical_Page_Number
    farpokew(SAMB_data, 12, Entry_Begin_Logical_Page_Offset);        // 12 0x0C LBX_FLIC_HDR.EMM_Logical_Page_Offset

    if ( (strcmp(EmmHndlNm, "MAINSCRN") == 0) && (LbxEntry == 0) )
    {
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_Width=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_Width));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_Height=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_Height));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_CurrentFrame=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_CurrentFrame));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_FrameCount=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_FrameCount));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_LoopFrame=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_LoopFrame));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmHandleNumber=%d\n", __FILE__, __LINE__, farpeekb(SAMB_data, FlicHdr_EmmHandleNumber));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmLogicalPageIndex=%d\n", __FILE__, __LINE__, farpeekb(SAMB_data, FlicHdr_EmmLogicalPageIndex));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmLogicalPageOffset=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_EmmLogicalPageOffset));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_PaletteDataOffset=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_PaletteDataOffset));
    }

//    printf("DEBUG: [%s, %d]: END: EMM_LBX_HdrOnly(EmmHndl = %d, EmmHndlNm = %s, LbxEntry = %d, SAMB_head = 0x%04X, LoadType = %d) { SAMB_data = 0x%04X }\n", __FILE__, __LINE__, EmmHndl, EmmHndlNm, LbxEntry, SAMB_head, LoadType, SAMB_data);
    return SAMB_data;
}
