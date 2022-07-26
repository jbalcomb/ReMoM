// _s13p12c.c EMM_LBX_HdrOnly
// ST_EMM.H

#include "ST_HEAD.H"

#include "ST_EMM.H"
#include "ST_FLIC.H"
#include "ST_LBX.H"

#include "STU_DBG.H"

/*
    ? only for LBX Type 0: pictures
    ? only Reserved EMM
    ∴
    ∵
    reads sizeof(LBX_FLIC_HDR)
    populates LBX_FLIC_HDR.EMM_Handle_Number, .EMM_Logical_Page_Number, and .EMM_Logical_Page_Offset
*/
unsigned int EMM_LBX_FLIC_Header(int EmmHndl, char *EmmHndlNm, int LbxEntry, unsigned int SAMB_head, int LoadType)
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
    unsigned char *ptr_Byte;
    unsigned char _FAR *fptr_Dst;
#ifdef DEBUG
    unsigned char bDebugDetail = 0;
#endif

#ifdef DEBUG
    if
    (
        ( (strcmp(EmmHndlNm, "MAINSCRN") == 0) && (LbxEntry == 0) ) ||
        ( (strcmp(EmmHndlNm, "MAINSCRN") == 0) && (LbxEntry == 5) )
    )
    {
        bDebugDetail = 1;
    }
#endif

#ifdef DEBUG
    //if (bDebugDetail)
    //{
        dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_LBX_FLIC_Header(EmmHndl=%d, EmmHndlNm=%s, LbxEntry=%d, SAMB_head=0x%04X, LoadType=%d)\n", __FILE__, __LINE__, EmmHndl, EmmHndlNm, LbxEntry, SAMB_head, LoadType);
    //}
#endif
    
    SAMB_data = ST_FAILURE;

    /*
        ? same code as in EMM_LBX_EntryLoader() ?
            LbxEntryCount     =  EMM_MapnRead
            EntryOffsetBegin  =  EMM_MapnRead
    */
    EMM_MapnRead((unsigned int)&LbxEntryCount, 0, 0, 0, 2, EmmHndl);

    if ( LbxEntryCount < LbxEntry )
    {
        LBX_Error(EmmHndlNm, 0x08, LbxEntry);  // LBXErr_entries_exceeded
    }

    /*
        BEGIN: LBX Header Offset
    */
    EntryTableOffset = (8 + (4 * LbxEntry));
    /*
        END: LBX Header Offset
    */

    /*
        BEGIN: LBX Record Offset
    */
    tmp_EntryTableOffset_Sgmt = (unsigned int)(EntryTableOffset >> 16);         // Hi-Word of Double-Word
    tmp_EntryTableOffset_Ofst = (unsigned int)(EntryTableOffset & 0x0000FFFF);  // Lo-Word of Double-Word
    EMM_MapnRead((unsigned int)&DataOffset_Begin, 0, tmp_EntryTableOffset_Ofst, tmp_EntryTableOffset_Sgmt, 4, EmmHndl);
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] (unsigned int)(DataOffset_Begin >> 16): 0x%04X\n", __FILE__, __LINE__, (unsigned int)(DataOffset_Begin >> 16));
    dlvfprintf("DEBUG: [%s, %d] (unsigned int)(DataOffset_Begin & 0x0000FFFF): 0x%04X\n", __FILE__, __LINE__, (unsigned int)(DataOffset_Begin & 0x0000FFFF));
#endif

#ifdef DEBUG
    if (bDebugDetail)
    {
        dlvfprintf("DEBUG: [%s, %d] EntryTableOffset=0x%04X\n", __FILE__, __LINE__, EntryTableOffset);
        dlvfprintf("DEBUG: [%s, %d] tmp_EntryTableOffset_Sgmt=0x%04X\n", __FILE__, __LINE__, tmp_EntryTableOffset_Sgmt);
        dlvfprintf("DEBUG: [%s, %d] tmp_EntryTableOffset_Ofst=0x%04X\n", __FILE__, __LINE__, tmp_EntryTableOffset_Ofst);
        dlvfprintf("DEBUG: [%s, %d] DataOffset_Begin=0x%08lX (%ld) (%lu)\n", __FILE__, __LINE__, DataOffset_Begin, DataOffset_Begin, DataOffset_Begin);
    }
    if (bDebugDetail)
    {
        if
        (
            ( strcmp(EmmHndlNm, "MAINSCRN") == 0 ) &&
            ( LbxEntry == 0 ) &&
            ( DataOffset_Begin != 0x000002C0 )
        )
        {
            dlvfprintf("DEBUG: [%s, %d] FATAL: MAINSCXRN_000: DataOffset_Begin != 0x000002C0 { 0x%08lX }\n", __FILE__, __LINE__, DataOffset_Begin);
        }
        if
        (
            ( strcmp(EmmHndlNm, "MAINSCRN") == 0 ) &&
            ( LbxEntry == 5 ) &&
            ( DataOffset_Begin != 0x00025910 )
        )
        {
            dlvfprintf("DEBUG: [%s, %d] FATAL: MAINSCXRN_005: DataOffset_Begin != 00025910 { 0x%08lX }\n", __FILE__, __LINE__, DataOffset_Begin);
        }
    }
#endif
    /*
        END: LBX Record Offset
    */

    /*
        BEGIN: Load Type
    */
    DataSize_Paras = 1;  // sizeof FLIC_HDR
    EMMLBXLOADTYPE()
    /*
        END: Load Type
    */

#ifdef DEBUG
    if (bDebugDetail)
    {
        dlvfprintf("DEBUG: [%s, %d] SAMB_data=0x%04X\n", __FILE__, __LINE__, SAMB_data);
    }
#endif

    Entry_Begin_Logical_Page_Number = DataOffset_Begin / EMS_PAGE_SIZE;  // 16384 e_EMM_Page_Size (0x4000)
    Entry_Begin_Logical_Page_Offset = DataOffset_Begin % EMS_PAGE_SIZE;  // 16384 e_EMM_Page_Size (0x4000)

#ifdef DEBUG
    if (bDebugDetail)
    {
        // MAINSCRN,0
        // DEBUG: [_s13p12c.c, 115] Entry_Begin_Logical_Page_Number=0 (0x00)
        // DEBUG: [_s13p12c.c, 116] Entry_Begin_Logical_Page_Offset=704 (0x02C0)
        // MAINSCRN,5
        // DEBUG: [_s13p12c.c, 115] Entry_Begin_Logical_Page_Number=9 (0x09)
        // DEBUG: [_s13p12c.c, 116] Entry_Begin_Logical_Page_Offset=6416 (0x1910)
        dlvfprintf("DEBUG: [%s, %d] Entry_Begin_Logical_Page_Number=%u (0x%02X)\n", __FILE__, __LINE__, Entry_Begin_Logical_Page_Number, Entry_Begin_Logical_Page_Number);
        dlvfprintf("DEBUG: [%s, %d] Entry_Begin_Logical_Page_Offset=%u (0x%04X)\n", __FILE__, __LINE__, Entry_Begin_Logical_Page_Offset, Entry_Begin_Logical_Page_Offset);
    }
#endif

    /*
        BEGIN: Read Data - ST FLIC Header
    */
    tmp_DataOffsetBegin_Sgmt = (unsigned int)(DataOffset_Begin >> 16);         // Hi-Word of Double-Word
    tmp_DataOffsetBegin_Ofst = (unsigned int)(DataOffset_Begin & 0x0000FFFF);  // Lo-Word of Double-Word
    EMM_MapnRead(0, SAMB_data, tmp_DataOffsetBegin_Ofst, tmp_DataOffsetBegin_Sgmt, 16, EmmHndl);
    /*
        END: Read Data - ST FLIC Header
    */

//dlvfprintf("DEBUG: [%s, %d] g_LBX_EmmRsvd: %d, EmmHndlNm: %s, LbxEntry: %d, SgmtAddr: 0x%04X, FlicHdr_EmmHandleNumber: %u, FlicHdr_EmmLogicalPageIndex: %u, FlicHdr_EmmLogicalPageOffset: 0x%04X)\n", __FILE__, __LINE__, g_LBX_EmmRsvd, EmmHndlNm, LbxEntry, SAMB_data, EmmHndl, Entry_Begin_Logical_Page_Number, Entry_Begin_Logical_Page_Offset);

#ifdef DEBUG
    if (bDebugDetail)
    {
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_Width=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_Width));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_Height=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_Height));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_CurrentFrame=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_CurrentFrame));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_FrameCount=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_FrameCount));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_LoopFrame=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_LoopFrame));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmHandleNumber=%d\n", __FILE__, __LINE__, farpeekb(SAMB_data, FlicHdr_EmmHandleNumber));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmLogicalPageIndex=%d\n", __FILE__, __LINE__, farpeekb(SAMB_data, FlicHdr_EmmLogicalPageIndex));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmLogicalPageOffset=0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_EmmLogicalPageOffset));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_PaletteDataOffset=0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_PaletteDataOffset));
    }
#endif

    // TODO(JimBalcomb): add defines for offsets and struct for LBX_FLIC_HDR
    farpokeb(SAMB_data, 10, (char)EmmHndl);                          // 10 0x0A LBX_FLIC_HDR.EMM_Handle_Number
    farpokeb(SAMB_data, 11, (char)Entry_Begin_Logical_Page_Number);  // 11 0x0B LBX_FLIC_HDR.EMM_Logical_Page_Number
    farpokew(SAMB_data, 12, Entry_Begin_Logical_Page_Offset);        // 12 0x0C LBX_FLIC_HDR.EMM_Logical_Page_Offset

#ifdef DEBUG
    if (bDebugDetail)
    {
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_Width=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_Width));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_Height=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_Height));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_CurrentFrame=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_CurrentFrame));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_FrameCount=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_FrameCount));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_LoopFrame=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_LoopFrame));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmHandleNumber=%d\n", __FILE__, __LINE__, farpeekb(SAMB_data, FlicHdr_EmmHandleNumber));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmLogicalPageIndex=%d\n", __FILE__, __LINE__, farpeekb(SAMB_data, FlicHdr_EmmLogicalPageIndex));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmLogicalPageOffset=0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_EmmLogicalPageOffset));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_PaletteDataOffset=0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_PaletteDataOffset));
    }
#endif

#ifdef DEBUG
    //if (bDebugDetail)
    //{
        dlvfprintf("DEBUG: [%s, %d] END: EMM_LBX_FLIC_Header(EmmHndl=%d, EmmHndlNm=%s, LbxEntry=%d, SAMB_head=0x%04X, LoadType=%d) {SAMB_data=0x%04X}\n", __FILE__, __LINE__, EmmHndl, EmmHndlNm, LbxEntry, SAMB_head, LoadType, SAMB_data);
    //}
#endif
    return SAMB_data;
}
