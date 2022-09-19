
#include "ST_VGA.H"

#include "STU_DBG.H"
#include "STU_TST.H"


char Test_Log_FileName[] = "TEST.LOG";
FILE * Test_Log_File = NULL;
char Test_Log_ISO8601_DateTime[21] = "1583-01-01T00:00:00Z";  // earliest possible, compliant ISO-8601 DateTime/timestamp

struct s_TST_LBX TST_LBX_MAINSCRN_000 = {
    "MAINSCRN.LBX",     /*  char LBX_File_Name[13]                */
    "MAINSCRN",         /*  char LBX_Name[9]                      */
    0,                  /*  unsigned int LBX_Type                 */
    0,                  /*  int LBX_Entry                         */
    "MAINSCRN_000",     /*  char LBX_Entry_Name[13]               */
    0,                  /*  int EMM_Table_Index                   */
    0,                  /*  long LBX_File_Size_B                  */
    0,                  /*  long LBX_File_Size_PR                 */
    0,                  /*  long LBX_File_Size_PG                 */
    0,                  /*  int EMM_Logical_Page_Count            */
    0,                  /*  sgmt_addr Segment_Address             */
    0,                  /*  unsigned char EMM_Handle_Number       */
    0,                  /*  unsigned char EMM_Logical_Page        */
    0                   /*  unsigned int EMM_Logical_Page_Offset  */
};
/*
LBX_File_Name = "MAINSCRN.LBX"
LBX_Name = "MAINSCRN"
LBX_Type = 0
LBX_Entry = 0
LBX_Entry_Name = "MAINSCRN_000"
LBX_File_Size_B = 196511
LBX_File_Size_PR = 13;  // (196511 /    16 = 12281.9375)
LBX_File_Size_PG = 12;  // (196511 / 16384 =    11.99407958984375)
EMM_Logical_Page_Count
*/

void dl_TST_LBX_MAINSCRN_000(void)
{
    dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_File_Name: %s\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_File_Name);
    dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_Name: %s\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_Name);
    dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_Type: %u\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_Type);
    dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_Entry: %d\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_Entry);
    dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_Entry_Name: %s\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_Entry_Name);
    dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.EMM_Table_Index: %d\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.EMM_Table_Index);
    dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_File_Size_B: %ld\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_File_Size_B);
    dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_File_Size_PR: %ld\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_File_Size_PR);
    dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_File_Size_PG: %ld\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_File_Size_PG);
    dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.EMM_Logical_Page_Count: %d\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.EMM_Logical_Page_Count);
    dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.Segment_Address: 0x%04X\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.Segment_Address);
    dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.EMM_Handle_Number: %d\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.EMM_Handle_Number);
    dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.EMM_Logical_Page: %d\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.EMM_Logical_Page);
    dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.EMM_Logical_Page_Offset: 0x%04X\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.EMM_Logical_Page_Offset);
}

void validate_FontStyleData(void)
{

}

void validate_BorderStyleData(void)
{
    
}

void validate_Palette_0(void)
{
    int itr_Palette;

    for ( itr_Palette = 0; itr_Palette < 256; itr_Palette++)
    {
        if ( *((BYTE *)(gsa_Palette + itr_Palette)) != 0 )
        {
            dlvfprintf("DEBUG: [%s, %d] FAILURE: ( *((BYTE *)(gsa_Palette + itr_PaletteFlags)) != 1 )\n", __FILE__, __LINE__);
            dlvfprintf("DEBUG: [%s, %d] FAILURE: gsa_Palette: 0x%02X;\n", __FILE__, __LINE__, *((BYTE *)(gsa_Palette + itr_Palette)));
            exit(1);
        }
    }
    
}

int validate_PaletteFlags_1(void)
{
    int itr_PaletteFlags;
    BYTE * pPaletteFlags;
    int test_status;
    
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: validate_PaletteFlags_1()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED

    dlvfprintf("DEBUG: [%s, %d] gsa_PaletteFlags: 0x%04X\n", __FILE__, __LINE__, gsa_PaletteFlags);
    pPaletteFlags = (BYTE *)MK_FP(gsa_PaletteFlags,0);
    dlvfprintf("DEBUG: [%s, %d] pPaletteFlags: %p\n", __FILE__, __LINE__, pPaletteFlags);

    for ( itr_PaletteFlags = 0; itr_PaletteFlags < 256; itr_PaletteFlags++)
    {
        // // dlvfprintf("DEBUG: [%s, %d] (gsa_PaletteFlags + itr_PaletteFlags): 0x%04X\n", __FILE__, __LINE__, (gsa_PaletteFlags + itr_PaletteFlags));
        // // dlvfprintf("DEBUG: [%s, %d] ((BYTE *)(gsa_PaletteFlags + itr_PaletteFlags)): %p\n", __FILE__, __LINE__, ((BYTE *)(gsa_PaletteFlags + itr_PaletteFlags)));
        // // dlvfprintf("DEBUG: [%s, %d] *((BYTE *)(gsa_PaletteFlags + itr_PaletteFlags)): 0x%02X\n", __FILE__, __LINE__, *((BYTE *)(gsa_PaletteFlags + itr_PaletteFlags)));
        // // dlvfprintf("DEBUG: [%s, %d] ((BYTE *)MK_FP(gsa_PaletteFlags,0) + itr_PaletteFlags): %p\n", __FILE__, __LINE__, ((BYTE *)MK_FP(gsa_PaletteFlags,0) + itr_PaletteFlags));
        // // dlvfprintf("DEBUG: [%s, %d] ((BYTE *)MK_FP(gsa_PaletteFlags,0) + itr_PaletteFlags): %Fp\n", __FILE__, __LINE__, ((BYTE *)MK_FP(gsa_PaletteFlags,0) + itr_PaletteFlags));
        // // dlvfprintf("DEBUG: [%s, %d] *((BYTE *)MK_FP(gsa_PaletteFlags,0) + itr_PaletteFlags): 0x%02X\n", __FILE__, __LINE__, *((BYTE *)MK_FP(gsa_PaletteFlags,0) + itr_PaletteFlags));
        // 
        // dlvfprintf("DEBUG: [%s, %d] (pPaletteFlags + itr_PaletteFlags): %p\n", __FILE__, __LINE__, (pPaletteFlags + itr_PaletteFlags));
        // dlvfprintf("DEBUG: [%s, %d] gsa_PaletteFlags: 0x%02X;\n", __FILE__, __LINE__, *(pPaletteFlags + itr_PaletteFlags));
        // // e.g., { 28A5:0000, ..., 28A5:00FF}

        // if ( *((BYTE *)(gsa_PaletteFlags + itr_PaletteFlags)) != 1 )
        if ( *(pPaletteFlags + itr_PaletteFlags) != 1 )
        {
            dlvfprintf("DEBUG: [%s, %d] FAILURE: ( *((BYTE *)(gsa_PaletteFlags + itr_PaletteFlags)) != 1 )\n", __FILE__, __LINE__);
            // dlvfprintf("DEBUG: [%s, %d] FAILURE: gsa_PaletteFlags: 0x%02X;\n", __FILE__, __LINE__, *((BYTE *)(gsa_PaletteFlags + itr_PaletteFlags)));
            dlvfprintf("DEBUG: [%s, %d] FAILURE: gsa_PaletteFlags: 0x%02X;\n", __FILE__, __LINE__, *(pPaletteFlags + itr_PaletteFlags));
            test_status = -1;  // TEST_FAILURE
            // exit(1);
            break;
        }
    }

    if ( test_status != -1 )  // TEST_FAILURE
    {
        test_status = 1;  // TEST_SUCCESS
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: validate_PaletteFlags_1( test_status = %s)\n", __FILE__, __LINE__, (test_status ? "TEST_SUCCESS" : "TEST_FAILURE"));
#endif

    return test_status;
}

void validate_PaletteLbxEntry(void)
{
    
}
