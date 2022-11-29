#ifndef STU_TST_H
#define STU_TST_H

#include "MoX_TYPE.H"

#include "ST_SA.H"

#include <STDIO.H>   /* FILE; */


#define TEST_FAILURE   -1
#define TEST_SUCCESS    1
#define TEST_UNDEFINED  0


// FONTS.LBX
#define FONTS_LBX_000    "FONTS_~1.BIN"  // FONTS_000.BIN
#define FONTS_LBX_001    "FONTS_~2.BIN"  // FONTS_001.BIN
#define FONTS_LBX_002    "FONTS_~3.BIN"  // FONTS_002.BIN
// MAINSCRN.LBX
#define MAINSCRN_LBX_000 "MAINSC~1.BIN"  // MAINSCRN_000.BIN
#define MAINSCRN_LBX_000_000_PAL "MAINSC~1.BIN"  // MAINSCRN_000_00-Palette.BIN
#define MAINSCRN_LBX_005 "MAINSC~2.BIN"  // MAINSCRN_005.BIN


extern char Test_Log_FileName[];
extern FILE * Test_Log_File;

// "PF" as in ? printf() || print format || Pass/Fail ?
#define PF_TEST_STATUS() { (test_status == 1 ? "TEST_SUCCESS" : "TEST_FAILURE") }

#define TLOG(_str_) { do { fprintf(Test_Log_File, "TEST: [%s, %d]: %s\n", __FILE__, __LINE__, (_str_)); fflush(Test_Log_File); } while(0); }

// #define TSTPRN(x) do { if (TEST) tst_prn x; } while (0)
// // Usage: TRACE(("message %d\n", var));



struct s_TST_LBX  // sizeof 109  ... * 101 = 11,009 bytes ... * 127 = 1,398,143 bytes  :(..
{
    char LBX_File_Name[13];
    char LBX_Name[9];
    unsigned int LBX_Type;
    int LBX_Entry;
    char LBX_Entry_Name[13];
    int EMM_Table_Index;
    long LBX_File_Size_B;
    long LBX_File_Size_PR;
    long LBX_File_Size_PG;
    int EMM_Logical_Page_Count;
    unsigned int Segment_Address;
    unsigned char EMM_Handle_Number;
    unsigned char EMM_Logical_Page;
    unsigned int EMM_Logical_Page_Offset;
    // // struct s_FLIC_HEADER;
    unsigned int SAMB_data_FlicHeader;
    // // DELETE char LBX_Load_Function[32];
};
//typedef struct s_TST_LBX TST_LBX;
extern struct s_TST_LBX TST_LBX_MAINSCRN_000;
extern struct s_TST_LBX TST_LBX_MAINSCRN_005;

// DELETE extern struct s_TST_LBX TST_LBX[101];


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

void Test_Log_Startup(void);
void Test_Log_Shutdown(void);
void tst_prn(const char * fmt, ...);

void wait_for_esc(void);

void reset_EMM(void);
void reset_ST_EMM(void);

void validate_FontStyleData(void);
void validate_BorderStyleData(void);
int validate_Palette_0(void);
int validate_Palette_M00(void);
int validate_PaletteFlags_1(void);
int validate_PaletteFlags_M00(void);
int validate_PaletteLbxEntry(void);
int validate_PaletteLbxEntry_2(SAMB_ptr sad1_PaletteLbxEntry);

// int validate_FLIC_Header_FP(SAMB_ptr fp_FLIC_Header);
int validate_FLIC_Header_FP(struct s_FLIC_HDR _FAR * fp_FLIC_Header);
int validate_FLIC_Header(SAMB_addr sa_FLIC_Header);

int validate_MAINSCRN_LBX_EMM(void);

// int validate_Hardware_Init()
int validate_EMM_Startup(void);
int validate_Load_Font_File(void);
// int validate_MoM_Tables_Init(6100);
int validate_GAME_LoadMainImages();
// int validate_GAME_Load_TERRSTAT_0();
// int validate_GAME_Load_SPELLDAT_0();

void test_pre__FLIC_Draw_XY__MAINSCRN_000_000(void);
void test_post__FLIC_Draw_XY__MAINSCRN_000_000(void);

void test_VBB_Pattern_Write(void);
void test_VBB_Pattern_Read(void);
void test_VBB_Pattern_Draw(void);
void test_VBB_Pattern_Print(void);
void test_VBB_Pattern_Dump(void);


#ifdef __cplusplus
}
#endif

#endif /* STU_TST_H */
