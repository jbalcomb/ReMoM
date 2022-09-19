#ifndef STU_TST_H
#define STU_TST_H

#include "ST_TYPE.H"

extern char Debug_Log_FileName[];
extern FILE *Debug_Log_File;
extern char DBG_LBX_Filename[];

struct s_TST_LBX
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
};
typedef struct s_TST_LBX TST_LBX;
extern struct s_TST_LBX TST_LBX_MAINSCRN_000;



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


void dl_TST_LBX_MAINSCRN_000(void);

void validate_FontStyleData(void);
void validate_BorderStyleData(void);
void validate_Palette_0(void);
int validate_PaletteFlags_1(void);
void validate_PaletteLbxEntry(void);


#ifdef __cplusplus
}
#endif

#endif /* STU_TST_H */
