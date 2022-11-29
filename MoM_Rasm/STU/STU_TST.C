
#include "Mox_TYPE.H"

#include "ST_DEF.H"  /* {ST_FALSE,ST_TRUE}, {ST_FAILURE,ST_SUCCESS} */

#include "ST_EMM.H"     /* EMM_Pages_Reserved */
#include "ST_FLIC.H"  /* s_FLIC_HDR */
#include "ST_SA.H"  /* SAMB_addr, SAMB_ptr, etc. */
#include "ST_VGA.H"  /* p_Palette, etc. */

#include "STU_DBG.H"
#include "STU_TST.H"
#include "STU_VGA.H"

#include <ALLOC.H>      /* coreleft(), farcoreleft(), malloc(), farmalloc(), free(), farfree() */    
#include <STDARG.H>     /* va_list; va_arg(), va_end(), va_start() */
#include <STDIO.H>      /* FILE; fclose(), fopen(), fread(), fwrite(), printf(); */
#include <STDLIB.H>     /* itoa() */
#include <STRING.H>     /* strcat(), strcpy() */


/*
    EMM - Uninitialized
        ? ALL Xref seg012 and seg013 ?
*/
//dseg:A5E0 UU_EMMData_PrevLvl dw 0
extern unsigned int g_EMM_Overlay_Handle;           // dseg:A5E2
extern unsigned int EMMDATAH_Level;                 // dseg:A5E4
extern unsigned int EmmHndlNbr_EMMDATAH;            // dseg:A5E6
// ? dseg:A5E8 UU_EMM_Handle dw 0
extern unsigned int g_EmmHndlNbr_VGAFILEH;          // dseg:A5EA
extern unsigned int g_LBX_EmmRsvd;                  // dseg:A5EC
extern unsigned int EmmHndlNbr_YOMOMA;              // dseg:A5EE
extern unsigned int EMM_OK;                         // dseg:A5F0
extern EMM_Record EMM_Table[];                      // dseg:A5F2 g_EMS_Table EMS_Record 28h dup(<0>)
extern unsigned int g_EMM_MinKB;                    // dseg:A7D2


char Test_Log_FileName[] = "TEST.LOG";
FILE * Test_Log_File = NULL;
char Test_Log_ISO8601_DateTime[21] = "1583-01-01T00:00:00Z";  // earliest possible, compliant ISO-8601 DateTime/timestamp

// DELETE struct s_TST_LBX TST_LBX[101];


void Test_Log_Startup(void)
{
    Test_Log_File = fopen(Test_Log_FileName, "w");
    if(Test_Log_File == NULL)
    {
        printf("TEST [FATAL]: Unable to open log file: %s", Test_Log_FileName);
        exit(1);
    }
    get_datetime(&Test_Log_ISO8601_DateTime);
    fprintf(Test_Log_File, "[%s] Test: %s\n", Test_Log_ISO8601_DateTime, "BEGIN: Test Log");
    fflush(Test_Log_File);
}

void Test_Log_Shutdown(void)
{
    get_datetime(&Test_Log_ISO8601_DateTime);
    fprintf(Test_Log_File, "[%s] TEST: %s\n", Test_Log_ISO8601_DateTime, "END: Test Log");
    fflush(Test_Log_File);
    fclose(Test_Log_File);
}

// "tst" as in "test"; "prn" as in "print"/"printf";
void tst_prn(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(Test_Log_File, fmt, args);
    va_end(args);
    /*
        flush output buffer to disk, in case of crash
        fysnc() flush kernel buffers, as well as userspace buffers, on a POSIX system (i.e., NOT Windows)
    */
   fflush(Test_Log_File);
}

void wait_for_esc(void)
{
    int flag_done;
    int input_control_index;
    int _escape_field;
    int _quit_field;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: wait_for_esc()\n", __FILE__, __LINE__);
#endif

    flag_done = ST_FALSE;

    _quit_field = CTRL_CreateHotkey('Q', -1);
    _escape_field = CTRL_CreateHotkey(27, -1);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] _quit_field: %d\n", __FILE__, __LINE__, _quit_field);
    dbg_prn("DEBUG: [%s, %d] _escape_field: %d\n", __FILE__, __LINE__, _escape_field);
#endif

    while ( flag_done == ST_FALSE )
    {
        input_control_index = IN_GetInput();
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] input_control_index: %d\n", __FILE__, __LINE__, input_control_index);
#endif
        if ( (input_control_index == _quit_field) || (input_control_index == _escape_field) )
        {
            flag_done = ST_TRUE;
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: wait_for_esc()\n", __FILE__, __LINE__);
#endif

}

void reset_EMM(void)
{
    unsigned int EMM_Handle_Count;
    unsigned int EMM_Handle_Number;
    int itr_EMM_Handle_Count;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: reset_EMM()\n", __FILE__, __LINE__);
#endif

    _AH = 0x4B;             // INT 67,4B - Get Handle Count
    geninterrupt(0x67);     // EMS INT
    EMM_Handle_Count = _BX;

    for ( itr_EMM_Handle_Count = 1; itr_EMM_Handle_Count < EMM_Handle_Count; itr_EMM_Handle_Count++ )
    {
        EMM_Handle_Number = itr_EMM_Handle_Count;
        _DX = EMM_Handle_Number;
        _AH = 0x45;         // INT 67,45 - Release Handle and Memory Pages
        geninterrupt(0x67); // EMS INT
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: reset_EMM()\n", __FILE__, __LINE__);
#endif
}

void reset_ST_EMM(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: reset_ST_EMM()\n", __FILE__, __LINE__);
#endif

    reset_EMM();

    EMM_OK = 0;
    EmmHndlNbr_YOMOMA = 0;
    g_EmmHndlNbr_VGAFILEH = 0;
    EmmHndlNbr_EMMDATAH = 0;
    EMMDATAH_Level = 0;
    EMM_Pages_Reserved = 40;
    g_EMM_Open_Handles = 0;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: reset_ST_EMM()\n", __FILE__, __LINE__);
#endif
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
struct s_TST_LBX TST_LBX_MAINSCRN_005 = {
    "MAINSCRN.LBX",     /*  char LBX_File_Name[13]                */
    "MAINSCRN",         /*  char LBX_Name[9]                      */
    0,                  /*  unsigned int LBX_Type                 */
    0,                  /*  int LBX_Entry                         */
    "MAINSCRN_005",     /*  char LBX_Entry_Name[13]               */
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
LBX_Entry_Name = "MAINSCRN_005"
LBX_File_Size_B = 
LBX_File_Size_PR = 
LBX_File_Size_PG = 
EMM_Logical_Page_Count
*/

// DELETE void dl_TST_LBX_MAINSCRN_000(void)
// DELETE {
// DELETE     dbg_prn("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_File_Name: %s\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_File_Name);
// DELETE     dbg_prn("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_Name: %s\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_Name);
// DELETE     dbg_prn("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_Type: %u\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_Type);
// DELETE     dbg_prn("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_Entry: %d\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_Entry);
// DELETE     dbg_prn("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_Entry_Name: %s\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_Entry_Name);
// DELETE     dbg_prn("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.EMM_Table_Index: %d\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.EMM_Table_Index);
// DELETE     dbg_prn("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_File_Size_B: %ld\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_File_Size_B);
// DELETE     dbg_prn("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_File_Size_PR: %ld\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_File_Size_PR);
// DELETE     dbg_prn("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.LBX_File_Size_PG: %ld\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.LBX_File_Size_PG);
// DELETE     dbg_prn("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.EMM_Logical_Page_Count: %d\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.EMM_Logical_Page_Count);
// DELETE     dbg_prn("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.Segment_Address: 0x%04X\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.Segment_Address);
// DELETE     dbg_prn("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.EMM_Handle_Number: %d\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.EMM_Handle_Number);
// DELETE     dbg_prn("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.EMM_Logical_Page: %d\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.EMM_Logical_Page);
// DELETE     dbg_prn("DEBUG: [%s, %d] TST_LBX_MAINSCRN_000.EMM_Logical_Page_Offset: 0x%04X\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.EMM_Logical_Page_Offset);
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



int validate_EMM_Startup(void)
{
    int test_status;
    int itr_EMM_Open_Handles;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: validate_EMM_Startup()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED

    if( EMM_OK == ST_FALSE )                                      { test_status = -1; }
    if( EmmHndlNbr_YOMOMA != 1 )                                  { test_status = -1; }
    if( g_EmmHndlNbr_VGAFILEH != 2 )                              { test_status = -1; }
    if( EmmHndlNbr_EMMDATAH != 3 )                                { test_status = -1; }
    if( EMMDATAH_Level != 0 )                                     { test_status = -1; }
    if( EMM_Pages_Reserved != 149 )                             { test_status = -1; }
    if( g_EMM_Open_Handles != 3 )                                 { test_status = -1; }
    if( EMM_GetFreePageCount() != 1962 )                          { test_status = -1; }
    if( EMM_GetActiveHandleCount() != 4)                          { test_status = -1; }
    if( EMM_Table[0].eEmmHndlNbr != 1 )                           { test_status = -1; }
    if( strcmp(EMM_Table[0].eEmmHndlNm, "YO MOMA") != 0 )         { test_status = -1; }
    if( EMM_Table[0].eEmmRsrvd != 0 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(EMM_Table[0].eEmmHndlNbr) != 1 )   { test_status = -1; }
    if( EMM_Table[1].eEmmHndlNbr != 2 )                           { test_status = -1; }
    if( strcmp(EMM_Table[1].eEmmHndlNm, "VGAFILEH") != 0 )        { test_status = -1; }
    if( EMM_Table[1].eEmmRsrvd != 1 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(EMM_Table[1].eEmmHndlNbr) != 5 )   { test_status = -1; }
    if( EMM_Table[2].eEmmHndlNbr != 3 )                           { test_status = -1; }
    if( strcmp(EMM_Table[2].eEmmHndlNm, "EMMDATAH") != 0 )        { test_status = -1; }
    if( EMM_Table[2].eEmmRsrvd != 1 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(EMM_Table[2].eEmmHndlNbr) != 4 )   { test_status = -1; }

    if( test_status != -1 )  // TEST_FAILURE
    {
        test_status = 1;  // TEST_SUCCESS
    }
#ifdef STU_DEBUG
    else
    {
        dbg_prn("DEBUG: [%s, %d] ( EMM_OK == ST_FALSE ): %s\n", __FILE__, __LINE__, (( EMM_OK == ST_FALSE ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( EmmHndlNbr_YOMOMA != 1 ): %s\n", __FILE__, __LINE__, (( EmmHndlNbr_YOMOMA != 1 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( g_EmmHndlNbr_VGAFILEH != 2 ): %s\n", __FILE__, __LINE__, (( g_EmmHndlNbr_VGAFILEH != 2 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( EmmHndlNbr_EMMDATAH != 3 ): %s\n", __FILE__, __LINE__, (( EmmHndlNbr_EMMDATAH != 3 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( EMMDATAH_Level != 0 ): %s\n", __FILE__, __LINE__, (( EMMDATAH_Level != 0 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( EMM_Pages_Reserved != 149 ): %s\n", __FILE__, __LINE__, (( EMM_Pages_Reserved != 149 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( g_EMM_Open_Handles != 3 ): %s\n", __FILE__, __LINE__, (( g_EMM_Open_Handles != 3 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( EMM_GetFreePageCount() != 1962 ): %s\n", __FILE__, __LINE__, (( EMM_GetFreePageCount() != 1962 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( EMM_GetActiveHandleCount() != 4): %s\n", __FILE__, __LINE__, (( EMM_GetActiveHandleCount() != 4) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( EMM_Table[0].eEmmHndlNbr != 1 ): %s\n", __FILE__, __LINE__, (( EMM_Table[0].eEmmHndlNbr != 1 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( strcmp(EMM_Table[0].eEmmHndlNm, \"YO MOMA\") != 0 ): %s\n", __FILE__, __LINE__, (( strcmp(EMM_Table[0].eEmmHndlNm, "YO MOMA") != 0 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( EMM_Table[0].eEmmRsrvd != 0 ): %s\n", __FILE__, __LINE__, (( EMM_Table[0].eEmmRsrvd != 0 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( EMM_GetHandlePageCount(EMM_Table[0].eEmmHndlNbr) != 1 ): %s\n", __FILE__, __LINE__, (( EMM_GetHandlePageCount(EMM_Table[0].eEmmHndlNbr) != 1 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( EMM_Table[1].eEmmHndlNbr != 2 ): %s\n", __FILE__, __LINE__, (( EMM_Table[1].eEmmHndlNbr != 2 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( strcmp(EMM_Table[1].eEmmHndlNm, \"VGAFILEH\") != 0 ): %s\n", __FILE__, __LINE__, (( strcmp(EMM_Table[1].eEmmHndlNm, "VGAFILEH") != 0 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( EMM_Table[1].eEmmRsrvd != 1 ): %s\n", __FILE__, __LINE__, (( EMM_Table[1].eEmmRsrvd != 1 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( EMM_GetHandlePageCount(EMM_Table[1].eEmmHndlNbr) != 5 ): %s\n", __FILE__, __LINE__, (( EMM_GetHandlePageCount(EMM_Table[1].eEmmHndlNbr) != 5 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( EMM_Table[2].eEmmHndlNbr != 3 ): %s\n", __FILE__, __LINE__, (( EMM_Table[2].eEmmHndlNbr != 3 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( strcmp(EMM_Table[2].eEmmHndlNm, \"EMMDATAH\") != 0 ): %s\n", __FILE__, __LINE__, (( strcmp(EMM_Table[2].eEmmHndlNm, "EMMDATAH") != 0 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( EMM_Table[2].eEmmRsrvd != 1 ): %s\n", __FILE__, __LINE__, (( EMM_Table[2].eEmmRsrvd != 1 ) ? "FAIL" : "PASS"));
        dbg_prn("DEBUG: [%s, %d] ( EMM_GetHandlePageCount(EMM_Table[2].eEmmHndlNbr) != 4 ): %s\n", __FILE__, __LINE__, (( EMM_GetHandlePageCount(EMM_Table[2].eEmmHndlNbr) != 4 ) ? "FAIL" : "PASS"));
    }
#endif
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] EMM_OK: %u\n", __FILE__, __LINE__, EMM_OK);
//     dbg_prn("DEBUG: [%s, %d] EmmHndlNbr_YOMOMA: %u\n", __FILE__, __LINE__, EmmHndlNbr_YOMOMA);
//     dbg_prn("DEBUG: [%s, %d] g_EmmHndlNbr_VGAFILEH: %u\n", __FILE__, __LINE__, g_EmmHndlNbr_VGAFILEH);
//     dbg_prn("DEBUG: [%s, %d] EmmHndlNbr_EMMDATAH: %u\n", __FILE__, __LINE__, EmmHndlNbr_EMMDATAH);
//     dbg_prn("DEBUG: [%s, %d] EMMDATAH_Level: %u\n", __FILE__, __LINE__, EMMDATAH_Level);
//     dbg_prn("DEBUG: [%s, %d] g_EMM_Pages_Reserved: %d\n", __FILE__, __LINE__, g_EMM_Pages_Reserved);
//     dbg_prn("DEBUG: [%s, %d] g_EMM_Open_Handles: %u\n", __FILE__, __LINE__, g_EMM_Open_Handles);
//     dbg_prn("DEBUG: [%s, %d] EMM_GetFreePageCount(): %u\n", __FILE__, __LINE__, EMM_GetFreePageCount());
//     dbg_prn("DEBUG: [%s, %d] EMM_GetActiveHandleCount(): %u\n", __FILE__, __LINE__, EMM_GetActiveHandleCount());
//     dbg_prn("DEBUG: [%s, %d] EMM_Table[0].eEmmHndlNbr: %u\n", __FILE__, __LINE__, EMM_Table[0].eEmmHndlNbr);
//     dbg_prn("DEBUG: [%s, %d] EMM_Table[0].eEmmHndlNm: %s\n", __FILE__, __LINE__, EMM_Table[0].eEmmHndlNm);
//     dbg_prn("DEBUG: [%s, %d] EMM_Table[0].eEmmRsrvd: %d\n", __FILE__, __LINE__, EMM_Table[0].eEmmRsrvd);
//     dbg_prn("DEBUG: [%s, %d] EMM_GetHandlePageCount(EMM_Table[0].eEmmHndlNbr: %u\n", __FILE__, __LINE__, EMM_GetHandlePageCount(EMM_Table[0].eEmmHndlNbr));
//     dbg_prn("DEBUG: [%s, %d] EMM_Table[1].eEmmHndlNbr: %u\n", __FILE__, __LINE__, EMM_Table[1].eEmmHndlNbr);
//     dbg_prn("DEBUG: [%s, %d] EMM_Table[1].eEmmHndlNm: %s\n", __FILE__, __LINE__, EMM_Table[1].eEmmHndlNm);
//     dbg_prn("DEBUG: [%s, %d] EMM_Table[1].eEmmRsrvd: %d\n", __FILE__, __LINE__, EMM_Table[1].eEmmRsrvd);
//     dbg_prn("DEBUG: [%s, %d] EMM_GetHandlePageCount(EMM_Table[1].eEmmHndlNbr: %u\n", __FILE__, __LINE__, EMM_GetHandlePageCount(EMM_Table[1].eEmmHndlNbr));
//     dbg_prn("DEBUG: [%s, %d] EMM_Table[2].eEmmHndlNbr: %u\n", __FILE__, __LINE__, EMM_Table[2].eEmmHndlNbr);
//     dbg_prn("DEBUG: [%s, %d] EMM_Table[2].eEmmHndlNm: %s\n", __FILE__, __LINE__, EMM_Table[2].eEmmHndlNm);
//     dbg_prn("DEBUG: [%s, %d] EMM_Table[2].eEmmRsrvd: %d\n", __FILE__, __LINE__, EMM_Table[2].eEmmRsrvd);
//     dbg_prn("DEBUG: [%s, %d] EMM_GetHandlePageCount(EMM_Table[2].eEmmHndlNbr: %u\n", __FILE__, __LINE__, EMM_GetHandlePageCount(EMM_Table[2].eEmmHndlNbr));
// #endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: validate_EMM_Startup( test_status = %s)\n", __FILE__, __LINE__, (test_status == 1 ? "TEST_SUCCESS" : "TEST_FAILURE"));
#endif

    return test_status;
}

int validate_Load_Font_File(void)
{
    int test_status;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: validate_Load_Font_File()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED



    if( test_status != -1 )  // TEST_FAILURE
    {
        test_status = 1;  // TEST_SUCCESS
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: validate_Load_Font_File( test_status = %s)\n", __FILE__, __LINE__, (test_status == 1 ? "TEST_SUCCESS" : "TEST_FAILURE"));
#endif

    return test_status;

}

void validate_FontStyleData(void)
{

}

void validate_BorderStyleData(void)
{
    
}

int validate_Palette_0(void)
{
    int test_status;
    int itr_Palette;

    for ( itr_Palette = 0; itr_Palette < 256; itr_Palette++)
    {
        if ( *(p_Palette + itr_Palette) != 0 )
        {
            dbg_prn("DEBUG: [%s, %d] FAILURE: ( *(p_Palette + itr_Palette) != 0 )\n", __FILE__, __LINE__);
            dbg_prn("DEBUG: [%s, %d] FAILURE: p_Palette[%d]: 0x%02X;\n", __FILE__, __LINE__, itr_Palette, *(p_Palette + itr_Palette));
            goto Failure;
        }
    }

Success:
    test_status = 1;  // TEST_SUCCESS
    goto Done;
Failure:
    test_status = -1;  // TEST_FAILURE
    goto Done;
Done:
    return test_status;
}

/*
    Validate that the 'Shadow Palette' is set properly
      after FLIC_Load_Palette() on MAINSCRN_000, Frame 0
    MAINSCRN_000_00-Palette.BIN  (MAINSC~2.BIN)

*/
int validate_Palette_M00(void)
{
    int test_status;
    FILE * fp;
    int itr_Palette;
    unsigned char baito1;
    unsigned char baito2;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: validate_Palette_M00()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED

    fp = fopen("MAINSC~2.BIN", "rb");

    if (fp == NULL)
    {
        dbg_prn("DEBUG: [%s, %d] ( fp = fopen(\"MAINSC~2.BIN\", \"rb\") ) == NULL\n", __FILE__, __LINE__);
        abort();
    }

    for ( itr_Palette = 0; itr_Palette < 256; itr_Palette++)
    {
        fread(&baito1, 1, 1, fp);
        baito2 = *(p_Palette + itr_Palette);
        // dbg_prn("DEBUG: [%s, %d]: baito:  0x%02X  0x%02X\n", __FILE__, __LINE__, baito1, baito2);
        if( baito2 != baito1 )
        {
            dbg_prn("DEBUG: [%s, %d]: baito[%d]:  0x%02X  0x%02X\n", __FILE__, __LINE__, itr_Palette, baito1, baito2);
            goto Failure;
        }
    }

Success:
    test_status = 1;  // TEST_SUCCESS
    goto Done;
Failure:
    test_status = -1;  // TEST_FAILURE
    goto Done;
Done:
    fclose(fp);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: validate_Palette_M00( test_status = %s)\n", __FILE__, __LINE__, (test_status == 1 ? "TEST_SUCCESS" : "TEST_FAILURE"));
#endif
    return test_status;
}

int validate_PaletteFlags_1(void)
{
    int itr_PaletteFlags;
    int test_status;
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: validate_PaletteFlags_1()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED

    for ( itr_PaletteFlags = 0; itr_PaletteFlags < 256; itr_PaletteFlags++)
    {
        if ( *(p_PaletteFlags + itr_PaletteFlags) != 1 )
        {
            dbg_prn("DEBUG: [%s, %d] FAILURE: ( *(p_PaletteFlags + itr_PaletteFlags) != 1 )\n", __FILE__, __LINE__);
            dbg_prn("DEBUG: [%s, %d] FAILURE: p_PaletteFlags: 0x%02X;\n", __FILE__, __LINE__, *(p_PaletteFlags + itr_PaletteFlags));
            goto Failure;
        }
    }

Success:
    test_status = 1;  // TEST_SUCCESS
    goto Done;
Failure:
    test_status = -1;  // TEST_FAILURE
    goto Done;
Done:
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: validate_PaletteFlags_1( test_status = %s)\n", __FILE__, __LINE__, (test_status == 1 ? "TEST_SUCCESS" : "TEST_FAILURE"));
#endif
    return test_status;
}


int validate_PaletteFlags_M00(void)
{
    int itr_PaletteFlags;
    int test_status;
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: validate_PaletteFlags_M00()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED

    for(itr_PaletteFlags = 0; itr_PaletteFlags < 256; itr_PaletteFlags++)
    {
        if( *(p_PaletteFlags + itr_PaletteFlags) != 1 )
        {
            dbg_prn("DEBUG: [%s, %d] FAILURE: ( *(p_PaletteFlags + itr_PaletteFlags) != 1 )\n", __FILE__, __LINE__);
            dbg_prn("DEBUG: [%s, %d] FAILURE: p_PaletteFlags: 0x%02X;\n", __FILE__, __LINE__, *(p_PaletteFlags + itr_PaletteFlags));
            goto Failure;
        }
    }

Success:
    test_status = 1;  // TEST_SUCCESS
    goto Done;
Failure:
    test_status = -1;  // TEST_FAILURE
    goto Done;
Done:
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: validate_PaletteFlags_M00( test_status = %s)\n", __FILE__, __LINE__, (test_status == 1 ? "TEST_SUCCESS" : "TEST_FAILURE"));
#endif
    return test_status;
}

int validate_PaletteLbxEntry(void)
{
    int test_status;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: validate_PaletteLbxEntry()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED



    if ( test_status != -1 )  // TEST_FAILURE
    {
        test_status = 1;  // TEST_SUCCESS
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: validate_PaletteLbxEntry( test_status = %s)\n", __FILE__, __LINE__, (test_status == 1 ? "TEST_SUCCESS" : "TEST_FAILURE"));
#endif

    return test_status;

}

int validate_PaletteLbxEntry_2(SAMB_ptr sad1_PaletteLbxEntry)
{
    int test_status;
    FILE * fp;
    unsigned char baito1;
    unsigned char baito2;
    int itr_PaletteLbxEntry;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: validate_PaletteLbxEntry_2( sad1_PaletteLbxEntry = %p)\n", __FILE__, __LINE__, sad1_PaletteLbxEntry);
#endif

    test_status = 0;  // TEST_UNDEFINED

    // fp = fopen("FONTS_002.BIN", "rb");
    fp = fopen("FONTS_~3.BIN", "rb");
    // fp = fopen(FONTS_LBX_002, "rb");

    if (fp == NULL)
    {
        dbg_prn("DEBUG: [%s, %d] ( fp = fopen(\"FONTS_002.BIN\", \"rb\") ) == NULL\n", __FILE__, __LINE__);
        abort();
    }

    itr_PaletteLbxEntry = 0;
    while(itr_PaletteLbxEntry < 5472)
    {
        fread(&baito1, 1, 1, fp);
        baito2 = *(sad1_PaletteLbxEntry + itr_PaletteLbxEntry);
        // dbg_prn("DEBUG: [%s, %d]: baito:  0x%02X  0x%02X\n", __FILE__, __LINE__, baito1, baito2);

        if( baito2 != baito1 )
        {
            test_status = -1;  // TEST_FAILURE
            break;
        }

        itr_PaletteLbxEntry++;
    }

    if ( test_status != -1 )  // TEST_FAILURE
    {
        test_status = 1;  // TEST_SUCCESS
    }

    fclose(fp);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: validate_PaletteLbxEntry_2( test_status = %s)\n", __FILE__, __LINE__, (test_status == 1 ? "TEST_SUCCESS" : "TEST_FAILURE"));
#endif

    return test_status;
}

//int validate_FLIC_Header_FP(SAMB_ptr fp_FLIC_Header)
int validate_FLIC_Header_FP(struct s_FLIC_HDR _FAR * fp_FLIC_Header)
{
    int test_status;
    SAMB_addr sa_FLIC_Header;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: validate_FLIC_Header_FP( fp_FLIC_Header == %Fp)\n", __FILE__, __LINE__, fp_FLIC_Header);
#endif
    sa_FLIC_Header = FP_SEG(fp_FLIC_Header);
    test_status = validate_FLIC_Header(sa_FLIC_Header);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: validate_FLIC_Header_FP( test_status == %s)\n", __FILE__, __LINE__, (test_status == 1 ? "TEST_SUCCESS" : "TEST_FAILURE"));
#endif
    return test_status;
}
int validate_FLIC_Header(SAMB_addr sa_FLIC_Header)
{
    int test_status;
    struct s_FLIC_HDR * fp_FLIC_Header;
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: validate_FLIC_Header( sa_FLIC_Header = 0x%04X)\n", __FILE__, __LINE__, sa_FLIC_Header);
#endif

    test_status = 0;  // TEST_UNDEFINED

    fp_FLIC_Header = (struct s_FLIC_HDR *) MK_FP(sa_FLIC_Header, 0);

    tst_prn("TEST: [%s, %d] fp_FLIC_Header->Width: %d\n",                   __FILE__, __LINE__, fp_FLIC_Header->Width);
    tst_prn("TEST: [%s, %d] fp_FLIC_Header->Height: %d\n",                  __FILE__, __LINE__, fp_FLIC_Header->Height);
    tst_prn("TEST: [%s, %d] fp_FLIC_Header->Current_Frame: %d\n",           __FILE__, __LINE__, fp_FLIC_Header->Current_Frame);
    tst_prn("TEST: [%s, %d] fp_FLIC_Header->Frame_Count: %d\n",             __FILE__, __LINE__, fp_FLIC_Header->Frame_Count);
    tst_prn("TEST: [%s, %d] fp_FLIC_Header->Loop_Frame: %d\n",              __FILE__, __LINE__, fp_FLIC_Header->Loop_Frame);
    tst_prn("TEST: [%s, %d] fp_FLIC_Header->EMM_Handle_Number: %d\n",       __FILE__, __LINE__, fp_FLIC_Header->EMM_Handle_Number);
    tst_prn("TEST: [%s, %d] fp_FLIC_Header->EMM_Logical_Page_Number: %d\n", __FILE__, __LINE__, fp_FLIC_Header->EMM_Logical_Page_Number);
    tst_prn("TEST: [%s, %d] fp_FLIC_Header->EMM_Logical_Page_Offset: %d\n", __FILE__, __LINE__, fp_FLIC_Header->EMM_Logical_Page_Offset);
    tst_prn("TEST: [%s, %d] fp_FLIC_Header->Palette_Header_Offset: %d\n",   __FILE__, __LINE__, fp_FLIC_Header->Palette_Header_Offset);

    if ( sa_FLIC_Header == TST_LBX_MAINSCRN_000.Segment_Address )
    {
        // ASSERT()
        if (
            (fp_FLIC_Header->Width != 320) ||
            (fp_FLIC_Header->Height != 41) ||
            (fp_FLIC_Header->Current_Frame != 0) ||
            (fp_FLIC_Header->Frame_Count != 20) ||
            (fp_FLIC_Header->Loop_Frame != 0) ||
            (fp_FLIC_Header->EMM_Handle_Number != 4) ||
            (fp_FLIC_Header->EMM_Logical_Page_Number != 0) ||
            (fp_FLIC_Header->EMM_Logical_Page_Offset != 704) ||
            (fp_FLIC_Header->Palette_Header_Offset != 102)
        )
        {
            tst_prn("TEST: [%s, %d] if ( ... ) { test_status = -1 }\n", __FILE__, __LINE__);
            test_status = -1;  // TEST_FAILURE
        }
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->Width == 320)                    %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->Width == 320) ? "PASS" : "FAIL" ));
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->Height == 41)                    %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->Height == 41) ? "PASS" : "FAIL" ));
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->Current_Frame == 0)              %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->Current_Frame == 0) ? "PASS" : "FAIL" ));
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->Frame_Count == 20)               %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->Frame_Count == 20) ? "PASS" : "FAIL" ));
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->Loop_Frame == 0)                 %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->Loop_Frame == 0) ? "PASS" : "FAIL" ));
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->EMM_Handle_Number == 4)          %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->EMM_Handle_Number == 4) ? "PASS" : "FAIL" ));
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->EMM_Logical_Page_Number == 0)    %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->EMM_Logical_Page_Number == 0) ? "PASS" : "FAIL" ));
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->EMM_Logical_Page_Offset == 704)  %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->EMM_Logical_Page_Offset == 704) ? "PASS" : "FAIL" ));
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->Palette_Header_Offset == 102)    %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->Palette_Header_Offset == 102) ? "PASS" : "FAIL" ));
    }
    else if ( sa_FLIC_Header == TST_LBX_MAINSCRN_005.Segment_Address )
    {
        // ASSERT()
        if (
            (fp_FLIC_Header->Width != 320) ||
            (fp_FLIC_Header->Height != 159) ||
            (fp_FLIC_Header->Current_Frame != 0) ||
            (fp_FLIC_Header->Frame_Count != 1) ||
            (fp_FLIC_Header->Loop_Frame != 0) ||
            (fp_FLIC_Header->EMM_Handle_Number != 4) ||
            (fp_FLIC_Header->EMM_Logical_Page_Number != 9) ||
            (fp_FLIC_Header->EMM_Logical_Page_Offset != 6416) ||
            (fp_FLIC_Header->Palette_Header_Offset != 0)
        )
        {
            tst_prn("TEST: [%s, %d] if ( ... ) { test_status == TEST_FAILURE }\n", __FILE__, __LINE__);
            test_status = -1;  // TEST_FAILURE
        }
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->Width == 320):                    %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->Width == 320) ? "PASS" : "FAIL" ));
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->Height == 159):                   %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->Height == 159) ? "PASS" : "FAIL" ));
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->Current_Frame == 0):              %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->Current_Frame == 0) ? "PASS" : "FAIL" ));
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->Frame_Count == 1):                %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->Frame_Count == 1) ? "PASS" : "FAIL" ));
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->Loop_Frame == 0):                 %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->Loop_Frame == 0) ? "PASS" : "FAIL" ));
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->EMM_Handle_Number == 4):          %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->EMM_Handle_Number == 4) ? "PASS" : "FAIL" ));
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->EMM_Logical_Page_Number == 9):    %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->EMM_Logical_Page_Number == 9) ? "PASS" : "FAIL" ));
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->EMM_Logical_Page_Offset == 6416):  %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->EMM_Logical_Page_Offset == 6416) ? "PASS" : "FAIL" ));
        tst_prn("TEST: [%s, %d] (fp_FLIC_Header->Palette_Header_Offset == 0):    %s\n", __FILE__, __LINE__, ( (fp_FLIC_Header->Palette_Header_Offset == 0) ? "PASS" : "FAIL" ));
    }
    else
    {
        tst_prn("TEST: [%s, %d] UNKNOWN FLIC HEADER\n", __FILE__, __LINE__);
        test_status = -1;  // TEST_FAILURE
    }

    if ( test_status != -1 )  // TEST_FAILURE
    {
        test_status = 1;  // TEST_SUCCESS
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: validate_FLIC_Header( test_status == %s)\n", __FILE__, __LINE__, (test_status == 1 ? "TEST_SUCCESS" : "TEST_FAILURE"));
#endif
    return test_status;
}

int validate_MAINSCRN_LBX_EMM(void)
{
    int test_status;
    int itr_EMM_Open_Handles;
    unsigned int EMM_Handle_Number;
    unsigned char EMM_Logical_Page;
    unsigned int EMM_Physical_Page;
    FILE * fp;
    unsigned char baito1;
    unsigned char baito2;
    long sz_MainscrnLbx;
    long itr_MainscrnLbx;
    long sz_EmmWindow;
    long itr_EmmWindow;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: validate_MAINSCRN_LBX_EMM()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED

    for(itr_EMM_Open_Handles = 0; itr_EMM_Open_Handles < g_EMM_Open_Handles; itr_EMM_Open_Handles++)
    {
        if( strcmp(EMM_Table[itr_EMM_Open_Handles].eEmmHndlNm,"MAINSCRN") == 0 )
        {
            test_status = 1;
            // dbg_prn("DEBUG: [%s, %d]: itr_EMM_Open_Handles: %d\n", __FILE__, __LINE__, itr_EMM_Open_Handles);
            EMM_Handle_Number = EMM_Table[itr_EMM_Open_Handles].eEmmHndlNbr;
        }
    }


    if(test_status == 1)
    {
        
        fp = fopen("MAINSCRN.LBX", "rb");

        if (fp == NULL)
        {
            dbg_prn("DEBUG: [%s, %d] ( fp = fopen(\"MAINSCRN.LBX\", \"rb\") ) == NULL\n", __FILE__, __LINE__);
            abort();
        }

        sz_MainscrnLbx = 196511;
        sz_EmmWindow = 65536;

        EMM_Logical_Page = 0;

        itr_MainscrnLbx = 0;
        while(itr_MainscrnLbx < sz_MainscrnLbx)
        {
            EMM_Physical_Page = 0;
            dbg_prn("DEBUG: [%s, %d]: EMM_Physical_Page: %u  EMM_Logical_Page: %u\n", __FILE__, __LINE__, EMM_Physical_Page, EMM_Logical_Page);
            EMM_MAP_PAGE(EMM_Physical_Page++,EMM_Handle_Number,EMM_Logical_Page++);
            EMM_MAP_PAGE(EMM_Physical_Page++,EMM_Handle_Number,EMM_Logical_Page++);
            EMM_MAP_PAGE(EMM_Physical_Page++,EMM_Handle_Number,EMM_Logical_Page++);
            EMM_MAP_PAGE(EMM_Physical_Page++,EMM_Handle_Number,EMM_Logical_Page++);
            itr_EmmWindow = 0;
            while(itr_EmmWindow < sz_EmmWindow)
            {
                fread(&baito1, 1, 1, fp);
                baito2 = *((unsigned char *) MK_FP(EMM_PageFrameBaseAddress, itr_EmmWindow));
                // dbg_prn("DEBUG: [%s, %d]: baito:  0x%02X  0x%02X\n", __FILE__, __LINE__, baito1, baito2);
                if( baito2 != baito1 )
                {
                    test_status = -1;  // TEST_FAILURE
                    break;
                    // dbg_prn("DEBUG: [%s, %d]: baito:  0x%02X  0x%02X\n", __FILE__, __LINE__, baito1, baito2);
                    // dbg_prn("DEBUG: [%s, %d]: EMM_Physical_Page: %u  EMM_Logical_Page: %u\n", __FILE__, __LINE__, EMM_Physical_Page, EMM_Logical_Page);
                    // dbg_prn("DEBUG: [%s, %d]: itr_MainscrnLbx: %ld  itr_EmmWindow: %ld\n", __FILE__, __LINE__, itr_MainscrnLbx, itr_EmmWindow);

                }
                itr_EmmWindow++;
                itr_MainscrnLbx++;
                if(itr_MainscrnLbx >= sz_MainscrnLbx)
                {
                    break;
                }
            }
        }
    }


    if ( test_status != -1 )  // TEST_FAILURE
    {
        test_status = 1;  // TEST_SUCCESS
    }

    fclose(fp);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: validate_MAINSCRN_LBX_EMM( test_status = %s)\n", __FILE__, __LINE__, (test_status == 1 ? "TEST_SUCCESS" : "TEST_FAILURE"));
#endif

    return test_status;
}

/*
DEBUG: [stu_tst.c, 349] EMM_OK: 1
DEBUG: [stu_tst.c, 350] EmmHndlNbr_YOMOMA: 1
DEBUG: [stu_tst.c, 351] g_EmmHndlNbr_VGAFILEH: 2
DEBUG: [stu_tst.c, 352] EmmHndlNbr_EMMDATAH: 3
DEBUG: [stu_tst.c, 353] EMMDATAH_Level: 0
DEBUG: [stu_tst.c, 354] g_EMM_Pages_Reserved: 10
DEBUG: [stu_tst.c, 355] g_EMM_Open_Handles: 6
DEBUG: [stu_tst.c, 356] EMM_GetFreePageCount(): 1941
DEBUG: [stu_tst.c, 357] EMM_GetActiveHandleCount(): 7

...

DEBUG: [stu_tst.c, 363] EMM_Table[3].eEmmHndlNbr: 4
DEBUG: [stu_tst.c, 361] EMM_Table[3].eEmmHndlNm: MAINSCRN
DEBUG: [stu_tst.c, 362] EMM_Table[3].eEmmRsrvd: 1
DEBUG: [stu_tst.c, 364] EMM_GetHandlePageCount(4): 12
DEBUG: [stu_tst.c, 363] EMM_Table[4].eEmmHndlNbr: 5
DEBUG: [stu_tst.c, 361] EMM_Table[4].eEmmHndlNm: WIZARDS
DEBUG: [stu_tst.c, 362] EMM_Table[4].eEmmRsrvd: 1
DEBUG: [stu_tst.c, 364] EMM_GetHandlePageCount(5): 8
DEBUG: [stu_tst.c, 363] EMM_Table[5].eEmmHndlNbr: 6
DEBUG: [stu_tst.c, 361] EMM_Table[5].eEmmHndlNm: SPELLDAT
DEBUG: [stu_tst.c, 362] EMM_Table[5].eEmmRsrvd: 1
DEBUG: [stu_tst.c, 364] EMM_GetHandlePageCount(6): 1
DEBUG: [stu_tst.c, 374] END: validate_GAME_LoadMainImages( test_status = TEST_SUCCESS)

*/
int validate_GAME_LoadMainImages(void)
{
    int test_status;
    int itr_EMM_Open_Handles;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: validate_GAME_LoadMainImages()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] EMM_OK: %d\n", __FILE__, __LINE__, EMM_OK);
//     dbg_prn("DEBUG: [%s, %d] EmmHndlNbr_YOMOMA: %d\n", __FILE__, __LINE__, EmmHndlNbr_YOMOMA);
//     dbg_prn("DEBUG: [%s, %d] g_EmmHndlNbr_VGAFILEH: %d\n", __FILE__, __LINE__, g_EmmHndlNbr_VGAFILEH);
//     dbg_prn("DEBUG: [%s, %d] EmmHndlNbr_EMMDATAH: %d\n", __FILE__, __LINE__, EmmHndlNbr_EMMDATAH);
//     dbg_prn("DEBUG: [%s, %d] EMMDATAH_Level: %d\n", __FILE__, __LINE__, EMMDATAH_Level);  // ? NIU ?
//     dbg_prn("DEBUG: [%s, %d] g_EMM_Pages_Reserved: %d\n", __FILE__, __LINE__, g_EMM_Pages_Reserved);
//     dbg_prn("DEBUG: [%s, %d] g_EMM_Open_Handles: %d\n", __FILE__, __LINE__, g_EMM_Open_Handles);
//     dbg_prn("DEBUG: [%s, %d] EMM_GetFreePageCount(): %u\n", __FILE__, __LINE__, EMM_GetFreePageCount());
//     dbg_prn("DEBUG: [%s, %d] EMM_GetActiveHandleCount(): %u\n", __FILE__, __LINE__, EMM_GetActiveHandleCount());
// 
//     for ( itr_EMM_Open_Handles = 0; itr_EMM_Open_Handles < g_EMM_Open_Handles; itr_EMM_Open_Handles++ )
//     {
//         dbg_prn("DEBUG: [%s, %d] EMM_Table[%d].eEmmHndlNbr: %u\n", __FILE__, __LINE__, itr_EMM_Open_Handles, EMM_Table[itr_EMM_Open_Handles].eEmmHndlNbr);
//         dbg_prn("DEBUG: [%s, %d] EMM_Table[%d].eEmmHndlNm: %s\n", __FILE__, __LINE__, itr_EMM_Open_Handles, EMM_Table[itr_EMM_Open_Handles].eEmmHndlNm);
//         dbg_prn("DEBUG: [%s, %d] EMM_Table[%d].eEmmRsrvd: %d\n", __FILE__, __LINE__, itr_EMM_Open_Handles, EMM_Table[itr_EMM_Open_Handles].eEmmRsrvd);
//         dbg_prn("DEBUG: [%s, %d] EMM_GetHandlePageCount(%d): %u\n", __FILE__, __LINE__, EMM_Table[itr_EMM_Open_Handles].eEmmHndlNbr, EMM_GetHandlePageCount(EMM_Table[itr_EMM_Open_Handles].eEmmHndlNbr));
//     }
// #endif

    /*
        BEGIN: EMM_Startup()
    */
    if( EMM_OK == ST_FALSE )                                      { test_status = -1; }
    if( EmmHndlNbr_YOMOMA != 1 )                                  { test_status = -1; }
    if( g_EmmHndlNbr_VGAFILEH != 2 )                              { test_status = -1; }
    if( EmmHndlNbr_EMMDATAH != 3 )                                { test_status = -1; }
    if( EMMDATAH_Level != 0 )                                     { test_status = -1; }
    // if( g_EMM_Pages_Reserved != 31 )                                { test_status = -1; }
    // if( g_EMM_Open_Handles != 3 )                                   { test_status = -1; }
    // if( EMM_GetFreePageCount() != 1962 )                            { test_status = -1; }
    // if( EMM_GetActiveHandleCount() != 4)                            { test_status = -1; }
    if( EMM_Table[0].eEmmHndlNbr != 1 )                           { test_status = -1; }
    if( strcmp(EMM_Table[0].eEmmHndlNm, "YO MOMA") != 0 )         { test_status = -1; }
    if( EMM_Table[0].eEmmRsrvd != 0 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(EMM_Table[0].eEmmHndlNbr) != 1 )   { test_status = -1; }
    if( EMM_Table[1].eEmmHndlNbr != 2 )                           { test_status = -1; }
    if( strcmp(EMM_Table[1].eEmmHndlNm, "VGAFILEH") != 0 )        { test_status = -1; }
    if( EMM_Table[1].eEmmRsrvd != 1 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(EMM_Table[1].eEmmHndlNbr) != 5 )   { test_status = -1; }
    if( EMM_Table[2].eEmmHndlNbr != 3 )                           { test_status = -1; }
    if( strcmp(EMM_Table[2].eEmmHndlNm, "EMMDATAH") != 0 )        { test_status = -1; }
    if( EMM_Table[2].eEmmRsrvd != 1 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(EMM_Table[2].eEmmHndlNbr) != 4 )   { test_status = -1; }
    /*
        BEGIN: EMM_Startup()
    */
    /*
        BEGIN: Changes...
    */
    if( EMM_Pages_Reserved != 10 )                                { test_status = -1; }
    if( g_EMM_Open_Handles != 6 )                                   { test_status = -1; }
    if( EMM_GetFreePageCount() != 1941 )                            { test_status = -1; }
    if( EMM_GetActiveHandleCount() != 7)                            { test_status = -1; }
    /*
        END: Changes...
    */
    /*
        BEGIN: Additions...
    */
    if( EMM_Table[3].eEmmHndlNbr != 4 )                           { test_status = -1; }
    if( strcmp(EMM_Table[3].eEmmHndlNm, "MAINSCRN") != 0 )        { test_status = -1; }
    if( EMM_Table[3].eEmmRsrvd != 1 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(EMM_Table[3].eEmmHndlNbr) != 12 )  { test_status = -1; }
    if( EMM_Table[4].eEmmHndlNbr != 5 )                           { test_status = -1; }
    if( strcmp(EMM_Table[4].eEmmHndlNm, "WIZARDS") != 0 )         { test_status = -1; }
    if( EMM_Table[4].eEmmRsrvd != 1 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(EMM_Table[4].eEmmHndlNbr) != 8 )   { test_status = -1; }
    if( EMM_Table[5].eEmmHndlNbr != 6 )                           { test_status = -1; }
    if( strcmp(EMM_Table[5].eEmmHndlNm, "SPELLDAT") != 0 )        { test_status = -1; }
    if( EMM_Table[5].eEmmRsrvd != 1 )                             { test_status = -1; }
    if( EMM_GetHandlePageCount(EMM_Table[5].eEmmHndlNbr) != 1 )   { test_status = -1; }
    /*
        END: Additions...
    */

    if ( test_status != -1 )  // TEST_FAILURE
    {
        test_status = 1;  // TEST_SUCCESS
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: validate_GAME_LoadMainImages( test_status = %s)\n", __FILE__, __LINE__, (test_status == 1 ? "TEST_SUCCESS" : "TEST_FAILURE"));
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

/*
    Video Back Buffer (VBB)

    ST_VGA.C/.H
    byte far * video_back_buffer;  // segment (far) pointer
    video_back_buffer = SA_Allocate_Space_MemBlk(4000);  // farmalloc((4000 + 1) * 16)
    Segment Address : {0,...,63999}
    VGA [{A000,A400}],[{0,1,2,3}],[{0,...,15999}]

    Sequencer Registers
        Sequencer Address Register
        Sequencer Data Register
        Address Register - port 3C4h
        Data Register    - port 3C5h

    Map Mask Register (Index 02h)
    Memory Plane Write Enable
        Bits 3-0 of this field correspond to planes 3-0 of the VGA display memory.
        If a bit is set, then write operations will modify the respective plane of display memory.
        If a bit is not set then write operations will not affect the respective plane of display memory.

        WriteMapMasks[4] = {0x01, 0x02, 0x04, 0x08};
        {0001,0010,0100,1000}
        x % 4
        Enable Write on ONLY memory plane 0 or 1 or 2 or 3
    ?
        by definition, the X coordinate simultaneously determines the memory address and the write mask


*/
void test_VBB_Pattern_Write(void)
{
    int itr_x;
    int itr_y;
    int itr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_VBB_Pattern_Write()\n", __FILE__, __LINE__);
#endif

    // video_back_buffer = SA_Allocate_MemBlk(4000);
    video_back_buffer = farmalloc(64000);
    if(video_back_buffer == NULL) { abort(); }
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] video_back_buffer: %p\n", __FILE__, __LINE__, video_back_buffer);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] video_back_buffer[0]: %02X\n", __FILE__, __LINE__, video_back_buffer[0]);
    dbg_prn("DEBUG: [%s, %d] video_back_buffer[1]: %02X\n", __FILE__, __LINE__, video_back_buffer[1]);
    dbg_prn("DEBUG: [%s, %d] video_back_buffer[63998]: %02X\n", __FILE__, __LINE__, video_back_buffer[63998]);
    dbg_prn("DEBUG: [%s, %d] video_back_buffer[63999]: %02X\n", __FILE__, __LINE__, video_back_buffer[63999]);
#endif

    memset(video_back_buffer, 0, 64000);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] video_back_buffer[0]: %02X\n", __FILE__, __LINE__, video_back_buffer[0]);
    dbg_prn("DEBUG: [%s, %d] video_back_buffer[1]: %02X\n", __FILE__, __LINE__, video_back_buffer[1]);
    dbg_prn("DEBUG: [%s, %d] video_back_buffer[63998]: %02X\n", __FILE__, __LINE__, video_back_buffer[63998]);
    dbg_prn("DEBUG: [%s, %d] video_back_buffer[63999]: %02X\n", __FILE__, __LINE__, video_back_buffer[63999]);
#endif

    for(itr_y = 0; itr_y < 200; itr_y++)
    {
        for(itr_x = 0; itr_x < 320; itr_x++)
        {
            // video_back_buffer[((itr_y * 320) + itr_x)] = GREEN;
            // video_back_buffer[((itr_y * 320) + itr_x)] = (((itr_y * 320) + itr_x) % 256);
            video_back_buffer[((itr_y * 320) + itr_x)] = 0x00;
        }
    }
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] video_back_buffer[0]: %02X\n", __FILE__, __LINE__, video_back_buffer[0]);
    dbg_prn("DEBUG: [%s, %d] video_back_buffer[1]: %02X\n", __FILE__, __LINE__, video_back_buffer[1]);
    dbg_prn("DEBUG: [%s, %d] video_back_buffer[63998]: %02X\n", __FILE__, __LINE__, video_back_buffer[63998]);
    dbg_prn("DEBUG: [%s, %d] video_back_buffer[63999]: %02X\n", __FILE__, __LINE__, video_back_buffer[63999]);
#endif

    video_back_buffer[0] = MAGENTA;
    video_back_buffer[63999] = MAGENTA;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] video_back_buffer[0]: %02X\n", __FILE__, __LINE__, video_back_buffer[0]);
    dbg_prn("DEBUG: [%s, %d] video_back_buffer[1]: %02X\n", __FILE__, __LINE__, video_back_buffer[1]);
    dbg_prn("DEBUG: [%s, %d] video_back_buffer[63998]: %02X\n", __FILE__, __LINE__, video_back_buffer[63998]);
    dbg_prn("DEBUG: [%s, %d] video_back_buffer[63999]: %02X\n", __FILE__, __LINE__, video_back_buffer[63999]);
#endif

    // for(itr = 0; itr < 32; itr++)
    // {
    //     *(video_back_buffer + itr) = BLUE;
    // }

    for(itr = 0; itr < 32; itr++)
    {
        *(video_back_buffer + (0 * 320) + itr) = BLUE;
    }
    
    for(itr = 0; itr < 32; itr++)
    {
        *(video_back_buffer + (19 * 320) + itr) = GREEN;
    }
    
    for(itr = 1; itr < 19; itr++)
    {
        *(video_back_buffer + (itr * 320) + 0) = RED;
    }
    
    for(itr = 1; itr < 19; itr++)
    {
        *(video_back_buffer + (itr * 320) + 31) = YELLOW;
    }



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_VBB_Pattern_Write()\n", __FILE__, __LINE__);
#endif
}
void test_VBB_Pattern_Read(void)
{

}
void test_VBB_Pattern_Draw(void)
{
    int itr_x;
    int itr_y;
    byte mask;      /* VGA Memory Plane Write Map Mask */
    byte * vram;    /* {A000,A400} + Offset to Scan-Line + Offset in Scan-Line */
    // byte * VGA = (byte *)0xA0000000L;        /* this points to video memory. */

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_VBB_Pattern_Write()\n", __FILE__, __LINE__);
#endif

    // // mask = STU_VGA_WriteMapMasks[(x_start & 0x03)];  // 0b00000011  ~== x modulo 4  (x % 4, x|4)
    // mask = 0x01;
    // outportb(SC_INDEX, SC_MAPMASK);   /* 0x03C4, 0x02   - output the index of the desired Data Register to the Address Register */
    // outportb(SC_DATA, mask);          /* 0x03C5, 0b0001 - write the value to the Data register */

    // vram = 0xA000 + 0 + 0;
    // // DEBUG: [stu_tst.c, 1088] vram: 0000:A000
    // vram = MK_FP((0xA000 + 0), 0);
    // // DEBUG: [stu_tst.c, 1091] vram: A000:0000
    // vram = ( (void *) ( ((unsigned long) (0xA000 + 0) << 16) | (0) ) );
    // // DEBUG: [stu_tst.c, 1093] vram: A000:0000
    // vram = VGA;
    // // DEBUG: [stu_tst.c, 1096] vram: A000:0000
    // vram = &VGA[20*320];
    // // DEBUG: [stu_tst.c, 1104] vram: A000:1900
    // vram = (VGA + (20*320));
    // // DEBUG: [stu_tst.c, 1100] vram: A000:1900
    // vram = (VGA + 65535);
    // // DEBUG: [stu_tst.c, 1101] vram: A000:FFFF
    // vram = (VGA + 65536);
    // // DEBUG: [stu_tst.c, 1106] vram: A000:0000
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] vram: %p\n", __FILE__, __LINE__, vram);
// #endif

/*
    Holly Fer Molly, this actually works - draws the first 32 bytes of the VBB.

    vram = ( (void *) ( ((unsigned long) (0xA000 + 0) << 16) | (0) ) );

    outportb(SC_INDEX, SC_MAPMASK);

    mask = 0x01;
    outportb(SC_DATA, mask);
    *(vram + ((0 * 320) + 0)) = *(video_back_buffer + 0);
    *(vram + ((0 * 320) + 1)) = *(video_back_buffer + 4);
    *(vram + ((0 * 320) + 2)) = *(video_back_buffer + 8);
    *(vram + ((0 * 320) + 3)) = *(video_back_buffer + 12);
    *(vram + ((0 * 320) + 4)) = *(video_back_buffer + 16);
    *(vram + ((0 * 320) + 5)) = *(video_back_buffer + 20);
    *(vram + ((0 * 320) + 6)) = *(video_back_buffer + 24);
    *(vram + ((0 * 320) + 7)) = *(video_back_buffer + 28);

    mask = 0x02;
    outportb(SC_DATA, mask);
    *(vram + ((0 * 320) + 0)) = *(video_back_buffer + 1);
    *(vram + ((0 * 320) + 1)) = *(video_back_buffer + 5);
    *(vram + ((0 * 320) + 2)) = *(video_back_buffer + 9);
    *(vram + ((0 * 320) + 3)) = *(video_back_buffer + 13);
    *(vram + ((0 * 320) + 4)) = *(video_back_buffer + 17);
    *(vram + ((0 * 320) + 5)) = *(video_back_buffer + 21);
    *(vram + ((0 * 320) + 6)) = *(video_back_buffer + 25);
    *(vram + ((0 * 320) + 7)) = *(video_back_buffer + 29);

    mask = 0x04;
    outportb(SC_DATA, mask);
    *(vram + ((0 * 320) + 0)) = *(video_back_buffer + 2);
    *(vram + ((0 * 320) + 1)) = *(video_back_buffer + 6);
    *(vram + ((0 * 320) + 2)) = *(video_back_buffer + 10);
    *(vram + ((0 * 320) + 3)) = *(video_back_buffer + 14);
    *(vram + ((0 * 320) + 4)) = *(video_back_buffer + 18);
    *(vram + ((0 * 320) + 5)) = *(video_back_buffer + 22);
    *(vram + ((0 * 320) + 6)) = *(video_back_buffer + 26);
    *(vram + ((0 * 320) + 7)) = *(video_back_buffer + 30);

    mask = 0x08;
    outportb(SC_DATA, mask);
    *(vram + ((0 * 320) + 0)) = *(video_back_buffer + 3);
    *(vram + ((0 * 320) + 1)) = *(video_back_buffer + 7);
    *(vram + ((0 * 320) + 2)) = *(video_back_buffer + 11);
    *(vram + ((0 * 320) + 3)) = *(video_back_buffer + 15);
    *(vram + ((0 * 320) + 4)) = *(video_back_buffer + 19);
    *(vram + ((0 * 320) + 5)) = *(video_back_buffer + 23);
    *(vram + ((0 * 320) + 6)) = *(video_back_buffer + 27);
    *(vram + ((0 * 320) + 7)) = *(video_back_buffer + 31);

*/
/*
    Algorithm
        set mask
        draw every 4th byte from the VBB
        offset into VGA VRAM
            row = (y * 320)
            col = x
        offset into VBB
            ? row * row length + col ?

    *(vram + ((  0 * 320) +  0)) = *(video_back_buffer + (  0 * 320) + (  0 * 4));
    *(vram + ((  0 * 320) +  1)) = *(video_back_buffer + (  0 * 320) + (  0 * 4));
    // ...
    *(vram + ((  0 * 320) + 79)) = *(video_back_buffer + (  0 * 320) + (319 * 4));
    *(vram + ((  1 * 320) +  0)) = *(video_back_buffer + (  1 * 320) + (  0 * 4));
    // ...
    *(vram + ((199 * 320) + 79)) = *(video_back_buffer + (199 * 320) + (319 * 4));
                            /\
                              needs to go up by 1 each iteration, but only to 79

*/

    vram = ( (void *) ( ((unsigned long) (0xA000 + 0) << 16) | (0) ) );

    outportb(SC_INDEX, SC_MAPMASK);

    mask = 0x01;
    outportb(SC_DATA, mask);
    for(itr_y = 0; itr_y < 200; itr_y++)
    {
        for(itr_x = 0; itr_x < 320; itr_x++)
        {
            *(vram + ((itr_y * (320/4)) + (itr_x % 80))) = *(video_back_buffer + (((itr_y * 320) + ((itr_x % 80) * 4)) + 0));
#ifdef STU_DEBUG
    // dbg_prn("DEBUG: [%s, %d] ((itr_y * 320) = (%d * 320) = %d\n", __FILE__, __LINE__, itr_y, (itr_y * 320));
    // dbg_prn("DEBUG: [%s, %d] (itr_x % 80) = (%d % 80) = %d\n", __FILE__, __LINE__, itr_x, (itr_x % 80));
//     dbg_prn("DEBUG: [%s, %d] ((itr_y * 320) + (itr_x mod 80)) = ((%d * 320) + (%d mod 80)) = ((%d) + (%d)) = (%d)\n", __FILE__, __LINE__, itr_y, itr_x, (itr_y * 320), (itr_x % 80), ((itr_y * 320) + (itr_x % 80)));
//     dbg_prn(
//         "DEBUG: [%s, %d] ((itr_y * 320) + ((itr_x mod 80) * 4) + 0) = ((%d * 320) + ((%d mod 80) * 4) + 0) = ((%d) + ((%d) * 4) + 0) = ((%d) + ((%d)) + 0) = ((%d)) + 0) = (%d)\n",
//          __FILE__, __LINE__,
//          itr_y,  itr_x,
//          (itr_y * 320), (itr_x % 80),
//          (itr_y * 320), ((itr_x % 80) * 4),
//          ((itr_y * 320) + ((itr_x % 80) * 4)),
//          (((itr_y * 320) + ((itr_x % 80) * 4)) + 0)
//         );

#endif
        }
    }
/*
((itr_y * 320) + (itr_x mod 80)) = ((0 * 320) + (0 mod 80)) = ((0) + (0)) = (0)
((itr_y * 320) + ((itr_x mod 80) * 4) + 0) = ((0 * 320) + ((0 mod 80) * 4) + 0) = ((0) + ((0) * 4) + 0) = ((0) + ((0)) + 0) = ((0)) + 0) = (0)
((itr_y * 320) + (itr_x mod 80)) = ((0 * 320) + (1 mod 80)) = ((0) + (1)) = (1)
((itr_y * 320) + ((itr_x mod 80) * 4) + 0) = ((0 * 320) + ((1 mod 80) * 4) + 0) = ((0) + ((1) * 4) + 0) = ((0) + ((4)) + 0) = ((4)) + 0) = (4)
((itr_y * 320) + (itr_x mod 80)) = ((0 * 320) + (2 mod 80)) = ((0) + (2)) = (2)
((itr_y * 320) + ((itr_x mod 80) * 4) + 0) = ((0 * 320) + ((2 mod 80) * 4) + 0) = ((0) + ((2) * 4) + 0) = ((0) + ((8)) + 0) = ((8)) + 0) = (8)
((itr_y * 320) + (itr_x mod 80)) = ((0 * 320) + (3 mod 80)) = ((0) + (3)) = (3)
((itr_y * 320) + ((itr_x mod 80) * 4) + 0) = ((0 * 320) + ((3 mod 80) * 4) + 0) = ((0) + ((3) * 4) + 0) = ((0) + ((12)) + 0) = ((12)) + 0) = (12)
((itr_y * 320) + (itr_x mod 80)) = ((0 * 320) + (4 mod 80)) = ((0) + (4)) = (4)
((itr_y * 320) + ((itr_x mod 80) * 4) + 0) = ((0 * 320) + ((4 mod 80) * 4) + 0) = ((0) + ((4) * 4) + 0) = ((0) + ((16)) + 0) = ((16)) + 0) = (16)
((itr_y * 320) + (itr_x mod 80)) = ((0 * 320) + (5 mod 80)) = ((0) + (5)) = (5)
((itr_y * 320) + ((itr_x mod 80) * 4) + 0) = ((0 * 320) + ((5 mod 80) * 4) + 0) = ((0) + ((5) * 4) + 0) = ((0) + ((20)) + 0) = ((20)) + 0) = (20)
((itr_y * 320) + (itr_x mod 80)) = ((0 * 320) + (6 mod 80)) = ((0) + (6)) = (6)
((itr_y * 320) + ((itr_x mod 80) * 4) + 0) = ((0 * 320) + ((6 mod 80) * 4) + 0) = ((0) + ((6) * 4) + 0) = ((0) + ((24)) + 0) = ((24)) + 0) = (24)
((itr_y * 320) + (itr_x mod 80)) = ((0 * 320) + (7 mod 80)) = ((0) + (7)) = (7)
((itr_y * 320) + ((itr_x mod 80) * 4) + 0) = ((0 * 320) + ((7 mod 80) * 4) + 0) = ((0) + ((7) * 4) + 0) = ((0) + ((28)) + 0) = ((28)) + 0) = (28)
*/
    mask = 0x02;
    outportb(SC_DATA, mask);
    for(itr_y = 0; itr_y < 200; itr_y++)
    {
        for(itr_x = 0; itr_x < 320; itr_x++)
        {
            *(vram + ((itr_y * (320/4)) + (itr_x % 80))) = *(video_back_buffer + (((itr_y * 320) + ((itr_x % 80) * 4)) + 1));
        }
    }

    mask = 0x04;
    outportb(SC_DATA, mask);
    for(itr_y = 0; itr_y < 200; itr_y++)
    {
        for(itr_x = 0; itr_x < 320; itr_x++)
        {
            *(vram + ((itr_y * (320/4)) + (itr_x % 80))) = *(video_back_buffer + (((itr_y * 320) + ((itr_x % 80) * 4)) + 2));
        }
    }

    mask = 0x08;
    outportb(SC_DATA, mask);
    for(itr_y = 0; itr_y < 200; itr_y++)
    {
        for(itr_x = 0; itr_x < 320; itr_x++)
        {
            *(vram + ((itr_y * (320/4)) + (itr_x % 80))) = *(video_back_buffer + (((itr_y * 320) + ((itr_x % 80) * 4)) + 3));
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_VBB_Pattern_Write()\n", __FILE__, __LINE__);
#endif
}

void test_VBB_Pattern_Print(void)
{

}
void test_VBB_Pattern_Dump(void)
{

}
