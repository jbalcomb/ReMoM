
#include <STRING.H>

#include "ST_FLIC.H"
#include "ST_VGA.H"

#include "STU_DBG.H"
#include "STU_TST.H"


char Test_Log_FileName[] = "TEST.LOG";
FILE * Test_Log_File = NULL;
char Test_Log_ISO8601_DateTime[21] = "1583-01-01T00:00:00Z";  // earliest possible, compliant ISO-8601 DateTime/timestamp

// DELETE struct s_TST_LBX TST_LBX[101];

// "tst" as in "test"; "prn" as in "print"/"printf";
void tst_prn(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(Debug_Log_File, fmt, args);
    va_end(args);
    /*
        flush output buffer to disk, in case of crash
        fysnc() flush kernel buffers, as well as userspace buffers, on a POSIX system (i.e., NOT Windows)
    */
   fflush(Debug_Log_File);
}


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

// DELETE void dl_TST_LBX_MAINSCRN_000(void)
// DELETE {
// DELETE     dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_File_Name: %s\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_File_Name);
// DELETE     dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_Name: %s\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_Name);
// DELETE     dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_Type: %u\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_Type);
// DELETE     dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_Entry: %d\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_Entry);
// DELETE     dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_Entry_Name: %s\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_Entry_Name);
// DELETE     dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.EMM_Table_Index: %d\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.EMM_Table_Index);
// DELETE     dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_File_Size_B: %ld\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_File_Size_B);
// DELETE     dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_File_Size_PR: %ld\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_File_Size_PR);
// DELETE     dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_File_Size_PG: %ld\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_File_Size_PG);
// DELETE     dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.EMM_Logical_Page_Count: %d\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.EMM_Logical_Page_Count);
// DELETE     dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.Segment_Address: 0x%04X\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.Segment_Address);
// DELETE     dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.EMM_Handle_Number: %d\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.EMM_Handle_Number);
// DELETE     dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.EMM_Logical_Page: %d\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.EMM_Logical_Page);
// DELETE     dlvfprintf("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.EMM_Logical_Page_Offset: 0x%04X\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.EMM_Logical_Page_Offset);
// DELETE }
// DELETE 
// DELETE Populate_TST_LBX(const char * LBX_Load_Function, char * LBX_Name, int LBX_Entry_Index, SAMB_addr SAMB_head, int Load_Type, int LBX_Header_Format)
// DELETE {
// DELETE     struct s_TST_LBX * p_TST_LBX;
// DELETE 
// DELETE     if ( strcmp(LBX_Name, "MAINSCRN") && LBX_Entry_Index == 0 ) { p_TST_LBX = &TST_LBX_MAINSCRN_000; }
// DELETE 
// DELETE     strcpy(p_TST_LBX->LBX_Load_Function, LBX_Load_Function);
// DELETE     
// DELETE }
// DELETE 
// DELETE void Set_LBX_Load_Function(const char * LBX_Load_Function, const char * LBX_Name, unsigned int LBX_Entry_Index)
// DELETE {
// DELETE     int LBX_Name_Index;
// DELETE 
// DELETE     LBX_Name_Index = Get_LBX_Name_Index(LBX_Name);
// DELETE 
// DELETE     strcpy(TST_LBX[LBX_Name_Index].LBX_Load_Function, LBX_Load_Function);
// DELETE 
// DELETE }

/*
    Pre  EMM_Startup():
        ?
    Post EMM_Startup():
        g_EMM_OK == ST_TRUE
        g_EmmHndlNbr_YOMOMA != 0
        g_EmmHndlNbr_VGAFILEH != 0
        g_EmmHndlNbr_EMMDATAH != 0
        g_EMM_Open_Handles == 3
        g_EMM_Pages_Reserved == 31
        EMM_Table[]
        EMM_Table[0].eEmmHndlNbr
        EMM_Table[0].eEmmHndlNm
        EMM_Table[0].eEmmRsrvd
        EMM_Table[1].eEmmHndlNbr
        EMM_Table[1].eEmmHndlNm
        EMM_Table[1].eEmmRsrvd
        EMM_Table[2].eEmmHndlNbr
        EMM_Table[2].eEmmHndlNm
        EMM_Table[2].eEmmRsrvd
        EMM_Table[3].eEmmHndlNbr
        EMM_Table[3].eEmmHndlNm
        EMM_Table[3].eEmmRsrvd
        EMM_Table[4].eEmmHndlNbr
        EMM_Table[4].eEmmHndlNm
        EMM_Table[4].eEmmRsrvd

DEBUG: [st_emm.c, 258] EMM_PageFrameBaseAddress: 0xE000 }

DEBUG: [tst_mgc.c, 436] g_EMM_OK: 1
DEBUG: [tst_mgc.c, 437] g_EmmHndlNbr_YOMOMA: 1
DEBUG: [tst_mgc.c, 438] g_EmmHndlNbr_VGAFILEH: 2
DEBUG: [tst_mgc.c, 439] g_EmmHndlNbr_EMMDATAH: 3
DEBUG: [tst_mgc.c, 441] g_EMM_Pages_Reserved: 31
DEBUG: [tst_mgc.c, 442] g_EMM_Open_Handles: 3
DEBUG: [tst_mgc.c, 443] EMM_GetFreePageCount(): 1962
DEBUG: [tst_mgc.c, 444] EMM_GetActiveHandleCount(): 4

DEBUG: [tst_mgc.c, 447] g_EMM_Table[0].eEmmHndlNm: YO MOMA
DEBUG: [tst_mgc.c, 448] g_EMM_Table[0].eEmmRsrvd: 0
DEBUG: [tst_mgc.c, 449] g_EMM_Table[0].eEmmHndlNbr: 1
DEBUG: [tst_mgc.c, 450] EMM_GetHandlePageCount(1): 1
DEBUG: [tst_mgc.c, 447] g_EMM_Table[1].eEmmHndlNm: VGAFILEH
DEBUG: [tst_mgc.c, 448] g_EMM_Table[1].eEmmRsrvd: 1
DEBUG: [tst_mgc.c, 449] g_EMM_Table[1].eEmmHndlNbr: 2
DEBUG: [tst_mgc.c, 450] EMM_GetHandlePageCount(2): 5
DEBUG: [tst_mgc.c, 447] g_EMM_Table[2].eEmmHndlNm: EMMDATAH
DEBUG: [tst_mgc.c, 448] g_EMM_Table[2].eEmmRsrvd: 1
DEBUG: [tst_mgc.c, 449] g_EMM_Table[2].eEmmHndlNbr: 3
DEBUG: [tst_mgc.c, 450] EMM_GetHandlePageCount(3): 4

*/
void validate_EMM_Startup(void)
{
    int test_status;
    int itr_EMM_Open_Handles;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: validate_EMM_Startup()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED

    if( g_EMM_OK != ST_SUCCESS )                                    { test_status = -1; }
    if( g_EmmHndlNbr_YOMOMA != 1 )                                  { test_status = -1; }
    if( g_EmmHndlNbr_VGAFILEH != 2 )                                { test_status = -1; }
    if( g_EmmHndlNbr_EMMDATAH != 3 )                                { test_status = -1; }
    if( g_EMMDATAH_Level != 0 )                                     { test_status = -1; }
    if( g_EMM_Pages_Reserved != 31 )                                { test_status = -1; }
    if( g_EMM_Open_Handles != 3 )                                   { test_status = -1; }
    if( EMM_GetFreePageCount() != 1962 )                            { test_status = -1; }
    if( EMM_GetActiveHandleCount() != 4)                            { test_status = -1; }
    if( g_EMM_Table[0].eEmmHndlNbr != 1 )                           { test_status = -1; }
    if( strcmp(g_EMM_Table[0].eEmmHndlNm, "YO MOMA") != 0 )         { test_status = -1; }
    if( g_EMM_Table[0].eEmmRsrvd != 0 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(g_EMM_Table[0].eEmmHndlNbr) != 1 )   { test_status = -1; }
    if( g_EMM_Table[1].eEmmHndlNbr != 2 )                           { test_status = -1; }
    if( strcmp(g_EMM_Table[1].eEmmHndlNm, "VGAFILEH") != 0 )        { test_status = -1; }
    if( g_EMM_Table[1].eEmmRsrvd != 1 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(g_EMM_Table[1].eEmmHndlNbr) != 5 )   { test_status = -1; }
    if( g_EMM_Table[2].eEmmHndlNbr != 3 )                           { test_status = -1; }
    if( strcmp(g_EMM_Table[2].eEmmHndlNm, "EMMDATAH") != 0 )        { test_status = -1; }
    if( g_EMM_Table[2].eEmmRsrvd != 1 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(g_EMM_Table[2].eEmmHndlNbr) != 4 )   { test_status = -1; }

    if( test_status != -1 )  // TEST_FAILURE
    {
        test_status = 1;  // TEST_SUCCESS
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: validate_EMM_Startup( test_status = %s)\n", __FILE__, __LINE__, (test_status ? "TEST_SUCCESS" : "TEST_FAILURE"));
#endif

    return test_status;
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

int validate_FLIC_Header(SAMB_data sa_FLIC_Header)
{
    int test_status;
    struct s_FLIC_HDR * fp_FLIC_Header;
    
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: validate_FLIC_Header( sa_FLIC_Header = 0x%04X)\n", __FILE__, __LINE__, sa_FLIC_Header);
#endif

    test_status = 0;  // TEST_UNDEFINED

    fp_FLIC_Header = (struct s_FLIC_HDR *)MK_FP(sa_FLIC_Header, 0);

    tst_prn("DEBUG: [%s, %d] fp_FLIC_Header->Width: %d\n",                   __FILE__, __LINE__, fp_FLIC_Header->Width);
    tst_prn("DEBUG: [%s, %d] fp_FLIC_Header->Height: %d\n",                  __FILE__, __LINE__, fp_FLIC_Header->Height);
    tst_prn("DEBUG: [%s, %d] fp_FLIC_Header->Current_Frame: %d\n",           __FILE__, __LINE__, fp_FLIC_Header->Current_Frame);
    tst_prn("DEBUG: [%s, %d] fp_FLIC_Header->Frame_Count: %d\n",             __FILE__, __LINE__, fp_FLIC_Header->Frame_Count);
    tst_prn("DEBUG: [%s, %d] fp_FLIC_Header->Loop_Frame: %d\n",              __FILE__, __LINE__, fp_FLIC_Header->Loop_Frame);
    tst_prn("DEBUG: [%s, %d] fp_FLIC_Header->EMM_Handle_Number: %d\n",       __FILE__, __LINE__, fp_FLIC_Header->EMM_Handle_Number);
    tst_prn("DEBUG: [%s, %d] fp_FLIC_Header->EMM_Logical_Page_Number: %d\n", __FILE__, __LINE__, fp_FLIC_Header->EMM_Logical_Page_Number);
    tst_prn("DEBUG: [%s, %d] fp_FLIC_Header->EMM_Logical_Page_Offset: %d\n", __FILE__, __LINE__, fp_FLIC_Header->EMM_Logical_Page_Offset);
    tst_prn("DEBUG: [%s, %d] fp_FLIC_Header->Palette_Header_Offset: %d\n",   __FILE__, __LINE__, fp_FLIC_Header->Palette_Header_Offset);

    if ( sa_FLIC_Header == TST_LBX_MAINSCRN_000.Segment_Address )
    {
        // ASSERT()
        if (
            (fp_FLIC_Header->Width != 320) ||
            (fp_FLIC_Header->Height != 41) ||
            (fp_FLIC_Header->Current_Frame != 0) ||
            (fp_FLIC_Header->Frame_Count != 20) ||
            (fp_FLIC_Header->Loop_Frame != 0) ||
            (fp_FLIC_Header->EMM_Handle_Number == 0) ||
            (fp_FLIC_Header->EMM_Logical_Page_Number != 0) ||
            (fp_FLIC_Header->EMM_Logical_Page_Offset != 704) ||
            (fp_FLIC_Header->Palette_Header_Offset != 102)
        )
        {
            test_status = -1;  // TEST_FAILURE
        }
        tst_prn("DEBUG: [%s, %d] (fp_FLIC_Header->Width == 320) %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->Width == 320) ? "TRUE" : "FALSE" ));
    }

    if ( test_status != -1 )  // TEST_FAILURE
    {
        test_status = 1;  // TEST_SUCCESS
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: validate_FLIC_Header( test_status = %s)\n", __FILE__, __LINE__, (test_status ? "TEST_SUCCESS" : "TEST_FAILURE"));
#endif

    return test_status;
}

/*
DEBUG: [stu_tst.c, 349] g_EMM_OK: 1
DEBUG: [stu_tst.c, 350] g_EmmHndlNbr_YOMOMA: 1
DEBUG: [stu_tst.c, 351] g_EmmHndlNbr_VGAFILEH: 2
DEBUG: [stu_tst.c, 352] g_EmmHndlNbr_EMMDATAH: 3
DEBUG: [stu_tst.c, 353] g_EMMDATAH_Level: 0
DEBUG: [stu_tst.c, 354] g_EMM_Pages_Reserved: 10
DEBUG: [stu_tst.c, 355] g_EMM_Open_Handles: 6
DEBUG: [stu_tst.c, 356] EMM_GetFreePageCount(): 1941
DEBUG: [stu_tst.c, 357] EMM_GetActiveHandleCount(): 7

...

DEBUG: [stu_tst.c, 363] g_EMM_Table[3].eEmmHndlNbr: 4
DEBUG: [stu_tst.c, 361] g_EMM_Table[3].eEmmHndlNm: MAINSCRN
DEBUG: [stu_tst.c, 362] g_EMM_Table[3].eEmmRsrvd: 1
DEBUG: [stu_tst.c, 364] EMM_GetHandlePageCount(4): 12
DEBUG: [stu_tst.c, 363] g_EMM_Table[4].eEmmHndlNbr: 5
DEBUG: [stu_tst.c, 361] g_EMM_Table[4].eEmmHndlNm: WIZARDS
DEBUG: [stu_tst.c, 362] g_EMM_Table[4].eEmmRsrvd: 1
DEBUG: [stu_tst.c, 364] EMM_GetHandlePageCount(5): 8
DEBUG: [stu_tst.c, 363] g_EMM_Table[5].eEmmHndlNbr: 6
DEBUG: [stu_tst.c, 361] g_EMM_Table[5].eEmmHndlNm: SPELLDAT
DEBUG: [stu_tst.c, 362] g_EMM_Table[5].eEmmRsrvd: 1
DEBUG: [stu_tst.c, 364] EMM_GetHandlePageCount(6): 1
DEBUG: [stu_tst.c, 374] END: validate_GAME_LoadMainImages( test_status = TEST_SUCCESS)

*/
int validate_GAME_LoadMainImages(void)
{
    int test_status;
    int itr_EMM_Open_Handles;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: validate_GAME_LoadMainImages()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED

// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] g_EMM_OK: %d\n", __FILE__, __LINE__, g_EMM_OK);
//     dlvfprintf("DEBUG: [%s, %d] g_EmmHndlNbr_YOMOMA: %d\n", __FILE__, __LINE__, g_EmmHndlNbr_YOMOMA);
//     dlvfprintf("DEBUG: [%s, %d] g_EmmHndlNbr_VGAFILEH: %d\n", __FILE__, __LINE__, g_EmmHndlNbr_VGAFILEH);
//     dlvfprintf("DEBUG: [%s, %d] g_EmmHndlNbr_EMMDATAH: %d\n", __FILE__, __LINE__, g_EmmHndlNbr_EMMDATAH);
//     dlvfprintf("DEBUG: [%s, %d] g_EMMDATAH_Level: %d\n", __FILE__, __LINE__, g_EMMDATAH_Level);  // ? NIU ?
//     dlvfprintf("DEBUG: [%s, %d] g_EMM_Pages_Reserved: %d\n", __FILE__, __LINE__, g_EMM_Pages_Reserved);
//     dlvfprintf("DEBUG: [%s, %d] g_EMM_Open_Handles: %d\n", __FILE__, __LINE__, g_EMM_Open_Handles);
//     dlvfprintf("DEBUG: [%s, %d] EMM_GetFreePageCount(): %u\n", __FILE__, __LINE__, EMM_GetFreePageCount());
//     dlvfprintf("DEBUG: [%s, %d] EMM_GetActiveHandleCount(): %u\n", __FILE__, __LINE__, EMM_GetActiveHandleCount());
// 
//     for ( itr_EMM_Open_Handles = 0; itr_EMM_Open_Handles < g_EMM_Open_Handles; itr_EMM_Open_Handles++ )
//     {
//         dlvfprintf("DEBUG: [%s, %d] g_EMM_Table[%d].eEmmHndlNbr: %u\n", __FILE__, __LINE__, itr_EMM_Open_Handles, g_EMM_Table[itr_EMM_Open_Handles].eEmmHndlNbr);
//         dlvfprintf("DEBUG: [%s, %d] g_EMM_Table[%d].eEmmHndlNm: %s\n", __FILE__, __LINE__, itr_EMM_Open_Handles, g_EMM_Table[itr_EMM_Open_Handles].eEmmHndlNm);
//         dlvfprintf("DEBUG: [%s, %d] g_EMM_Table[%d].eEmmRsrvd: %d\n", __FILE__, __LINE__, itr_EMM_Open_Handles, g_EMM_Table[itr_EMM_Open_Handles].eEmmRsrvd);
//         dlvfprintf("DEBUG: [%s, %d] EMM_GetHandlePageCount(%d): %u\n", __FILE__, __LINE__, g_EMM_Table[itr_EMM_Open_Handles].eEmmHndlNbr, EMM_GetHandlePageCount(g_EMM_Table[itr_EMM_Open_Handles].eEmmHndlNbr));
//     }
// #endif

    /*
        BEGIN: EMM_Startup()
    */
    if( g_EMM_OK != ST_SUCCESS )                                    { test_status = -1; }
    if( g_EmmHndlNbr_YOMOMA != 1 )                                  { test_status = -1; }
    if( g_EmmHndlNbr_VGAFILEH != 2 )                                { test_status = -1; }
    if( g_EmmHndlNbr_EMMDATAH != 3 )                                { test_status = -1; }
    if( g_EMMDATAH_Level != 0 )                                     { test_status = -1; }
    // if( g_EMM_Pages_Reserved != 31 )                                { test_status = -1; }
    // if( g_EMM_Open_Handles != 3 )                                   { test_status = -1; }
    // if( EMM_GetFreePageCount() != 1962 )                            { test_status = -1; }
    // if( EMM_GetActiveHandleCount() != 4)                            { test_status = -1; }
    if( g_EMM_Table[0].eEmmHndlNbr != 1 )                           { test_status = -1; }
    if( strcmp(g_EMM_Table[0].eEmmHndlNm, "YO MOMA") != 0 )         { test_status = -1; }
    if( g_EMM_Table[0].eEmmRsrvd != 0 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(g_EMM_Table[0].eEmmHndlNbr) != 1 )   { test_status = -1; }
    if( g_EMM_Table[1].eEmmHndlNbr != 2 )                           { test_status = -1; }
    if( strcmp(g_EMM_Table[1].eEmmHndlNm, "VGAFILEH") != 0 )        { test_status = -1; }
    if( g_EMM_Table[1].eEmmRsrvd != 1 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(g_EMM_Table[1].eEmmHndlNbr) != 5 )   { test_status = -1; }
    if( g_EMM_Table[2].eEmmHndlNbr != 3 )                           { test_status = -1; }
    if( strcmp(g_EMM_Table[2].eEmmHndlNm, "EMMDATAH") != 0 )        { test_status = -1; }
    if( g_EMM_Table[2].eEmmRsrvd != 1 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(g_EMM_Table[2].eEmmHndlNbr) != 4 )   { test_status = -1; }
    /*
        BEGIN: EMM_Startup()
    */
    /*
        BEGIN: Changes...
    */
    if( g_EMM_Pages_Reserved != 10 )                                { test_status = -1; }
    if( g_EMM_Open_Handles != 6 )                                   { test_status = -1; }
    if( EMM_GetFreePageCount() != 1941 )                            { test_status = -1; }
    if( EMM_GetActiveHandleCount() != 7)                            { test_status = -1; }
    /*
        END: Changes...
    */
    /*
        BEGIN: Additions...
    */
    if( g_EMM_Table[3].eEmmHndlNbr != 4 )                           { test_status = -1; }
    if( strcmp(g_EMM_Table[3].eEmmHndlNm, "MAINSCRN") != 0 )        { test_status = -1; }
    if( g_EMM_Table[3].eEmmRsrvd != 1 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(g_EMM_Table[3].eEmmHndlNbr) != 12 )  { test_status = -1; }
    if( g_EMM_Table[4].eEmmHndlNbr != 5 )                           { test_status = -1; }
    if( strcmp(g_EMM_Table[4].eEmmHndlNm, "WIZARDS") != 0 )         { test_status = -1; }
    if( g_EMM_Table[4].eEmmRsrvd != 1 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(g_EMM_Table[4].eEmmHndlNbr) != 8 )   { test_status = -1; }
    if( g_EMM_Table[5].eEmmHndlNbr != 6 )                           { test_status = -1; }
    if( strcmp(g_EMM_Table[5].eEmmHndlNm, "SPELLDAT") != 0 )        { test_status = -1; }
    if( g_EMM_Table[5].eEmmRsrvd != 1 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(g_EMM_Table[5].eEmmHndlNbr) != 1 )   { test_status = -1; }
    /*
        END: Additions...
    */

    if ( test_status != -1 )  // TEST_FAILURE
    {
        test_status = 1;  // TEST_SUCCESS
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: validate_GAME_LoadMainImages( test_status = %s)\n", __FILE__, __LINE__, (test_status ? "TEST_SUCCESS" : "TEST_FAILURE"));
#endif

    return test_status;
}

/*
    Test
        FLIC_Draw_XY()
        MAINSCRN.LBX
        Entry 0
        FLIC Frame 0

    TST_LBX_MAINSCRN_000


*/
void test_pre__FLIC_Draw_XY__MAINSCRN_000_000(void)
{

}
void test_post__FLIC_Draw_XY__MAINSCRN_000_000(void)
{

}
