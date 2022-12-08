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
    MGC  seg010
    WZD  seg010
*/

// MGC s10p01
SAMB_ptr LBX_Load(char * lbx_name, int16_t entry_num);
// MGC s10p02
SAMB_ptr LBX_Reload(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head);
// MGC s10p03
SAMB_ptr LBX_Reload_Next(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head);

// MGC s10p10
SAMB_ptr LBX_Load_Entry(char * lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, int16_t allocation_type);

// MGC s10p15
void Error_Handler(char * file_name, int16_t error_num, int16_t entry_num, int16_t pages);

// MGC s10p16
void File_Name_Base(char * file_name);


#ifdef __cplusplus
}
#endif

#endif /* MOX_LBX_H */
