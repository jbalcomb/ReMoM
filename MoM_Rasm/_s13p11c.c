// _s13p11c.c EMM_LBX_Load_Entry
// ST_EMM.H

#include "ST_HEAD.H"

#include "ST_EMM.H"
#include "ST_LBX.H"
#include "ST_SA.H"

#include "STU_DBG.H"

// TODO(JimBalcomb): add DEBUG for status/why/which failure

unsigned int EMM_LBX_Load_Entry(char *EmmHndlNm, int LbxEntry, unsigned int SAMB_head, int LoadType, int FormatType)
{
    //unsigned int tmp_SAMB_head;
    sgmt_addr SAMB_data;
    unsigned int EmmHndl;
    unsigned int LbxEntryCount;
    static unsigned int LbxFileType;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    long EntryTableOffset;
    unsigned int tmp_EntryTableOffset_Sgmt;
    unsigned int tmp_EntryTableOffset_Ofst;
    static long DataOffset_Begin;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    static long DataOffset_End;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    unsigned int tmp_DataOffsetBegin_Sgmt;
    unsigned int tmp_DataOffsetBegin_Ofst;
    unsigned long DataSize_Bytes;
    unsigned long DataSize_Paras;
    unsigned int tmp_SAMB_Size;  // used in LBXLOADTYPE/EMMLBXLOADTYPE
    sgmt_addr tmp_SAMB_data;  // result of LBXLOADTYPE/EMMLBXLOADTYPE
    unsigned int ReadNbytes;
#ifdef DEBUG
    unsigned char bDebugDetail = 0;
#endif

#ifdef DEBUG
    if
    (
        ( (strcmp(EmmHndlNm, "FONTS") == 0) && (LbxEntry == 0) ) ||
        ( (strcmp(EmmHndlNm, "FONTS") == 0) && (LbxEntry == 1) ) ||
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
        dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_LBX_Load_Entry(EmmHndlNm=%s, LbxEntry=%d, SAMB_head=0x%04X, LoadType=%d, FormatType=%d)\n", __FILE__, __LINE__, EmmHndlNm, LbxEntry, SAMB_head, LoadType, FormatType);
    //}
#endif

    SAMB_data = ST_FAILURE;

    EMMLBXHANDLE()

    /*
        LBX Type 0: FLIC  ("pictures")
    */
    if ( g_LBX_EmmRsvd == 1 )
    {
        EMM_MapnRead((unsigned int)&LbxFileType, 0, 6, 0, 2, EmmHndl);
        if ( LbxFileType != 0 )
        {
            LBX_Error(EmmHndlNm, 0x0D, LbxEntry);  // LbxErrNbr = 13, LbxErrIdx = 12; 'LBXErr_cantload_reserved': cnst_LBX_ErrorD = " Only pictures may be loaded into reserved EMM"
        }
        SAMB_data = EMM_LBX_FLIC_Header(EmmHndl, EmmHndlNm, LbxEntry, SAMB_head, LoadType);
        dlvfprintf("DEBUG: [%s, %d] g_LBX_EmmRsvd: %d, EmmHndlNm: %s, LbxEntry: %d, SgmtAddr: 0x%04X)\n", __FILE__, __LINE__, g_LBX_EmmRsvd, EmmHndlNm, LbxEntry, SAMB_data);
        goto Exit;
    }

    EMMLBXENTRYCOUNT()

    /*
        BEGIN: LBX Header Offset
    */
    if ( FormatType != 0 )
    {
        EntryTableOffset = ((LbxEntry * 4) + 520);
    }
    else
    {
        EntryTableOffset = ((LbxEntry * 4) + 8);
    }
    // EntryTableOffset = (LbxEntry * 4) + ( (FormatType != 0) ? 520 : 4 );
    /*
        END: LBX Header Offset
    */

    /*
        BEGIN: Entry - Offset Begin, End, Size
    */

    EMMLBXENTRYOFFSET()

    /*
        END: Entry - Offset Begin, End, Size
    */

    /*
        BEGIN: Load Type
    */
    DataSize_Paras = (DataSize_Bytes / SZ_PARAGRAPH_B);
    if ( (DataSize_Paras & 0x0F) != 0 )     // ~== MOD 16
    {
        DataSize_Paras = DataSize_Paras + 1;
    }
    EMMLBXLOADTYPE()
    /*
        END: Load Type
    */
    tmp_SAMB_data = SAMB_data;
    /*
        BEGIN: Read Data
    */
    EMMLBXREADDATA()
    /*
        END: Read Data
    */
    goto Exit;

Error:
    SAMB_data = ST_FAILURE;

Exit:

#ifdef DEBUG
    //if (bDebugDetail)
    //{
        dlvfprintf("DEBUG: [%s, %d] END: EMM_LBX_Load_Entry(EmmHndlNm=%s, LbxEntry=%d, SAMB_head=0x%04X, LoadType=%d, FormatType=%d) { SAMB_data=0x%04X }\n", __FILE__, __LINE__, EmmHndlNm, LbxEntry, SAMB_head, LoadType, FormatType, SAMB_data);
    //}
#endif
    return SAMB_data;
}
