// _s13p13c.c EMM_LBX_Load_Record
// ST_EMM.H
// ~= c&p EMM_LBX_Load_Entry()

#include "ST_HEAD.H"

#include "ST_EMM.H"
#include "ST_LBX.H"
#include "ST_SA.H"


unsigned int EMM_LBX_Load_Record(char *EmmHndlNm, int LbxEntry, unsigned int SAMB_head, int LoadType, int RecFirst, int RecCount, int RecSize)
{
    //unsigned int tmp_SAMB_head;
    sgmt_addr SAMB_data;
    unsigned int EmmHndl;
    unsigned int LbxEntryCount;
    long EntryTableOffset;
    unsigned int tmp_EntryTableOffset_Sgmt;
    unsigned int tmp_EntryTableOffset_Ofst;
    static long DataOffset_Begin;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    static long DataOffset_End;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    unsigned long DataSize_Bytes;   // LBXENTRYOFFSET()
    unsigned int tmp_DataOffsetBegin_Sgmt;
    unsigned int tmp_DataOffsetBegin_Ofst;
    static int Record_Count;        // LBXRECORDOFFSET()
    static int Record_Size;         // LBXRECORDOFFSET()
    unsigned long DataSize_Paras;   // LBXLOADTYPE()
    unsigned int tmp_SAMB_Size;     // LBXLOADTYPE()
    sgmt_addr tmp_SAMB_data;        // LBXREADDATA()
    unsigned int ReadNbytes;        // LBXREADDATA()
    void * pSAMB_head;

//    printf("DEBUG: [%s, %d]: BEGIN: EMM_LBX_Load_Record(EmmHndlNm = %s, LbxEntry = %d, SAMB_head = 0x%04X, LoadType = %d, RecFirst = %d, RecCount = %d, RecSize = %d)\n", __FILE__, __LINE__, EmmHndlNm, LbxEntry, SAMB_head, LoadType, RecFirst, RecCount, RecSize);

    //tmp_SAMB_head = SAMB_head;
    SAMB_data = ST_FAILURE;

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

    /*
        BEGIN: Load Type
    */
    DataSize_Paras = ((DataSize_Bytes / SZ_PARAGRAPH_B) + 1);
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

Error:
//    printf("DEBUG: [%s, %d]: END: EMM_LBX_Load_Record(EmmHndlNm = %s, LbxEntry = %d, SAMB_head = 0x%04X, LoadType = %d, RecFirst = %d, RecCount = %d, RecSize = %d) { SAMB_data = 0x%04X }\n", __FILE__, __LINE__, EmmHndlNm, LbxEntry, SAMB_head, LoadType, RecFirst, RecCount, RecSize, SAMB_data);
    return ST_FAILURE;
Exit:
//    printf("DEBUG: [%s, %d]: END: EMM_LBX_Load_Record(EmmHndlNm = %s, LbxEntry = %d, SAMB_head = 0x%04X, LoadType = %d, RecFirst = %d, RecCount = %d, RecSize = %d) { SAMB_data = 0x%04X }\n", __FILE__, __LINE__, EmmHndlNm, LbxEntry, SAMB_head, LoadType, RecFirst, RecCount, RecSize, SAMB_data);
    return SAMB_data;
}
