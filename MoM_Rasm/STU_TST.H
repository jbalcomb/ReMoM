#ifndef STU_TST_H
#define STU_TST_H

#include "ST_TYPE.H"


#define TST_HERE(argString) { \
    do { \
        fprintf(Debug_Log_File, "TEST: [%s, %d]: %s \n", __FILE__, __LINE__, argString ); \
        fflush(Debug_Log_File); \
    } while(0); \
}

#define TSTPRN(x) do { if (TEST) tst_prn x; } while (0)
// Usage: TRACE(("message %d\n", var));


extern char Debug_Log_FileName[];
extern FILE *Debug_Log_File;
extern char DBG_LBX_Filename[];

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
    // struct s_FLIC_HEADER;
    SAMB_data SAMB_data_FlicHeader;
    // DELETE char LBX_Load_Function[32];
};
//typedef struct s_TST_LBX TST_LBX;
extern struct s_TST_LBX TST_LBX_MAINSCRN_000;

// DELETE extern struct s_TST_LBX TST_LBX[101];


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

void tst_prn(const char *fmt, ...);

// DELETE void dl_TST_LBX_MAINSCRN_000(void);
// DELETE 
// DELETE Populate_TST_LBX(const char * LBX_Load_Function, char * LBX_Name, int LBX_Entry_Index, SAMB_addr SAMB_head, int Load_Type, int LBX_Header_Format);
// DELETE 
// DELETE void Set_LBX_Load_Function(const char * LBX_Load_Function, const char * LBX_Name, unsigned int LBX_Entry_Index);

void validate_EMM_Startup(void);
void validate_FontStyleData(void);
void validate_BorderStyleData(void);
void validate_Palette_0(void);
int  validate_PaletteFlags_1(void);
void validate_PaletteLbxEntry(void);
int  validate_FLIC_Header(SAMB_data sa_FLIC_Header);

int  validate_GAME_LoadMainImages();

void test_pre__FLIC_Draw_XY__MAINSCRN_000_000(void);
void test_post__FLIC_Draw_XY__MAINSCRN_000_000(void);


#ifdef __cplusplus
}
#endif

#endif /* STU_TST_H */
