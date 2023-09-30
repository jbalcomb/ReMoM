#ifndef MOX_LBX_H
#define MOX_LBX_H

#include "MoX_TYPE.H"

#include "MoX_BITS.H"   /* GET_2B_OFS() etc. */


#include <STDIO.H>      /* FILE; fclose(), fopen(), fread(), fseek(); */



#define SZ_32K_B            32768
#define SZ_PARAGRAPH_B         16

#define SZ_LBX_HDR_B          512
#define SZ_LBX_HDR_PR          32  // 32 paragraphs * 16 bytes per paragaph = 512 bytes

#define LBX_MAGSIG              0xFEAD

// #define LBX_FILE_TYPE_PICTURES   0

#define LBX_HDR_POS_NUM_ENTRIES                 0x00
#define LBX_HDR_POS_MAGIC_NUMBER                0x02
// #define LBX_HDR_POS_FILE_TYPE                   0x04
// #define LBX_HDR_POS_DUMMY                       0x06
// #define LBX_HDR_POS_OFFSETS                     0x08

#define LBX_GET_NUM_ENTRIES(_ptr_)   ( GET_2B_OFS( (_ptr_), LBX_HDR_POS_NUM_ENTRIES  ) )
#define LBX_GET_MAGIC_NUMBER(_ptr_)  ( GET_2B_OFS( (_ptr_), LBX_HDR_POS_MAGIC_NUMBER ) )
// #define LBX_GET_FILETYPE(_ptr_)          ( GET_2B_OFS( (_ptr_), LBX_HDR_POS_FILE_TYPE    ) )
// #define LBX_GET_DUMMY(_ptr_)             ( GET_2B_OFS( (_ptr_), LBX_HDR_POS_DUMMY        ) )
// #define LBX_GET_OFFSETS(_ptr_)           ( GET_2B_OFS( (_ptr_), LBX_HDR_POS_OFFSETS      ) )

#define LBX_GET_ENTRY_START(_lbx_header_,_entry_num_)  ( GET_4B_OFS( (_lbx_header_), ( 8 + ((_entry_num_) * 4)    ) ) )
#define LBX_GET_ENTRY_END(_lbx_header_,_entry_num_)    ( GET_4B_OFS( (_lbx_header_), ( 8 + ((_entry_num_) * 4) + 4) ) )
// #define LBX_GET_ENTRY_LENGTH(_lbx_header_,_entry_num_) ( LBX_GET_ENTRY_END( (_lbx_header_), (_entry_num_) ) - LBX_GET_ENTRY_START( (_lbx_header_), (_entry_num_) ) )


extern int16_t lbxload_lbx_header_flag;
extern FILE * lbxload_fptr;
extern char lbxload_lbx_file_extension[];
extern int16_t lbxload_num_entries;
extern SAMB_ptr lbxload_lbx_header;
extern char lbxload_lbx_name[];



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    WZD  seg010
    MGC  seg010
*/

// WZD s10p01
// MoO2: Farload()
SAMB_ptr LBX_Load(char * lbx_name, int16_t entry_num);
// WZD s10p02
// MoO2: Far_Reload()
SAMB_ptr LBX_Reload(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head);
// WZD s10p03
// MoO2: Far_Reload_Next()
SAMB_ptr LBX_Reload_Next(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head);

// WZD s10p04
// MoO2: Farload_Data()
SAMB_ptr LBX_Load_Data(char * lbx_name, int16_t entry_num, int16_t start_rec, int16_t num_recs, int16_t record_size);
// WZD s10p05
// MoO2: Far_Reload_Data()
SAMB_ptr LBX_Reload_Data(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, int16_t start_rec, int16_t num_recs, int16_t record_size);
// WZD s10p06
// MoO2: Far_Reload_Next_Data()
SAMB_ptr LBX_Reload_Next_Data(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, int16_t start_rec, int16_t num_recs, int16_t record_size);


// WZD s10p10
// ¿ MoO2: Farload_Library() ?
SAMB_ptr LBX_Load_Entry(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, int16_t allocation_type);

// WZD s10p11
// MoO2: Farload_Library_Data()
SAMB_ptr LBX_Load_Library_Data(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, int16_t allocation_type, uint16_t start_rec, uint16_t num_recs, uint16_t record_size);

// WZD s10p12
    // LBXR_DirectLoader( LBX_FileName@, Entry_Index, LoadTo@, FirstRec, RecCount, RecSize )
    // Main_Screen_Help() LBXR_DirectLoader("helpentry", 1, &_help_entries, 0, 25, 10);
// MoO2: Farload_Data_Static() ?  |-> Farload_Library_Data() ... in MoO2, they just added Allocation Type: 3 - No Allocation / Pre-Allocated
// TODO  void LBX_Load_Data_Static(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, uint16_t start_rec, uint16_t num_recs, uint16_t record_size);
void LBX_Load_Data_Static(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, uint16_t start_rec, uint16_t num_recs, uint16_t record_size);

// WZD s10p13
// ¿ MoO2: Get_Farload_Data_Record_Count() ?
// LBX_GetEntryData()

// WZD s10p15
void Error_Handler(char * file_name, int16_t error_num, int16_t entry_num, int16_t pages);

// WZD s10p16
void File_Name_Base(char * file_name);


#ifdef __cplusplus
}
#endif

#endif /* MOX_LBX_H */
