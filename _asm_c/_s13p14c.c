// _s13p14c.c EMM_LBXR_DirectLoad
// ST_EMM.H
// ~= c&p EMM_LBX_Load_Record()

#include "ST_HEAD.H"

#include "ST_EMM.H"
#include "ST_LBX.H"
#include "ST_SA.H"


int EMM_LBXR_DirectLoad(char *EmmHndlNm, int LbxEntry, unsigned int SAMB_data, int RecFirst, int RecCount, int RecSize)
{
    unsigned int tmp_LbxEntry;
    sgmt_addr UU_SAMB_data;
    unsigned int EmmHndl;
    unsigned int LbxEntryCount;
    long EntryTableOffset;
    unsigned int tmp_EntryTableOffset_Sgmt;
    unsigned int tmp_EntryTableOffset_Ofst;
    static long DataOffset_Begin;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    static long DataOffset_End;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    unsigned long DataSize_Bytes;
    unsigned int tmp_DataOffsetBegin_Sgmt;
    unsigned int tmp_DataOffsetBegin_Ofst;
    static int Record_Count;        // LBXRECORDOFFSET()
    static int Record_Size;         // LBXRECORDOFFSET()
    unsigned long DataSize_Paras;   // LBXLOADTYPE()
    unsigned int ReadNbytes;        // LBXREADDATA()

    printf("DEBUG: [%s, %d]: BEGIN: EMM_LBXR_DirectLoad(EmmHndlNm = %s, LbxEntry = %d, SAMB_data = 0x%04X, RecFirst = %d, RecCount = %d, RecSize = %d)\n", __FILE__, __LINE__, EmmHndlNm, LbxEntry, SAMB_data, RecFirst, RecCount, RecSize);

    tmp_LbxEntry = LbxEntry;

    UU_SAMB_data = ST_FAILURE;

    EMMLBXHANDLE()

    EMMLBXENTRYCOUNT()

    /*
        BEGIN: LBX Header Offset
    */
    EntryTableOffset = ((4 * LbxEntry) + 8);
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
        BEGIN: Records - Offset Begin, Size
    */
    EMMLBXRECORDOFFSET()
    /*
        END: Records - Offset Begin, Size
    */

    DataSize_Paras = ((DataSize_Bytes / SZ_PARAGRAPH_B) + 1);
    // ...UnUsed, but as seen elsewhere for use in 'Load Type'

    /*
        BEGIN: Read Data
    */
    // EMMLBXREADDATA()
    ReadNbytes = DataSize_Bytes % SZ_32K_B;
    tmp_DataOffsetBegin_Sgmt = (unsigned int)(DataOffset_Begin >> 16);
    tmp_DataOffsetBegin_Ofst = (unsigned int)(DataOffset_Begin & 0x0000FFFF);
    EMM_MapnRead(0, SAMB_data, tmp_DataOffsetBegin_Sgmt, tmp_DataOffsetBegin_Ofst, ReadNbytes, EmmHndl);
    /*
        END: Read Data
    */

    tmp_DataOffsetBegin_Sgmt = (unsigned int)(DataOffset_Begin >> 16);         // Hi-Word of Double-Word
    tmp_DataOffsetBegin_Ofst = (unsigned int)(DataOffset_Begin & 0x0000FFFF);  // Lo-Word of Double-Word
    EMM_MapnRead(0, SAMB_data, tmp_DataOffsetBegin_Ofst, tmp_DataOffsetBegin_Sgmt, 16, EmmHndl);

Error:
    printf("DEBUG: [%s, %d]: END: EMM_LBXR_DirectLoad(EmmHndlNm = %s, LbxEntry = %d, SAMB_data = 0x%04X, RecFirst = %d, RecCount = %d, RecSize = %d) { SAMB_data = 0x%04X }\n", __FILE__, __LINE__, EmmHndlNm, LbxEntry, SAMB_data, RecFirst, RecCount, RecSize, SAMB_data);
    return ST_FAILURE;
Exit:
    printf("DEBUG: [%s, %d]: END: EMM_LBXR_DirectLoad(EmmHndlNm = %s, LbxEntry = %d, SAMB_data = 0x%04X, RecFirst = %d, RecCount = %d, RecSize = %d) { SAMB_data = 0x%04X }\n", __FILE__, __LINE__, EmmHndlNm, LbxEntry, SAMB_data, RecFirst, RecCount, RecSize, SAMB_data);
    return ST_SUCCESS;
}
