#ifndef WIN_LBX_HPP
#define WIN_LBX_HPP

#include "win_BITS.hpp"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


#define SZ_LBX_HDR_B                            512
#define SZ_LBX_HDR_PR                            32  // 32 paragraphs * 16 bytes per paragaph = 512 bytes

#define LBX_MAGSIG                              0xFEAD
// 
// #define LBX_FILE_TYPE_PICTURES   0

#define LBX_HDR_POS_NUM_ENTRIES                 0x00
#define LBX_HDR_POS_MAGIC_NUMBER                0x02
#define LBX_HDR_POS_FILE_TYPE                   0x04
#define LBX_HDR_POS_DUMMY                       0x06
#define LBX_HDR_POS_OFFSETS                     0x08

#define LBX_GET_NUMENTRIES(_ptr_)   ( GET_2B_OFS( (_ptr_), LBX_HDR_POS_NUM_ENTRIES  ) )
#define LBX_GET_MAGICNUMBER(_ptr_)  ( GET_2B_OFS( (_ptr_), LBX_HDR_POS_MAGIC_NUMBER ) )
#define LBX_GET_FILETYPE(_ptr_)     ( GET_2B_OFS( (_ptr_), LBX_HDR_POS_FILE_TYPE    ) )
#define LBX_GET_DUMMY(_ptr_)        ( GET_2B_OFS( (_ptr_), LBX_HDR_POS_DUMMY        ) )
#define LBX_GET_OFFSETS(_ptr_)      ( GET_2B_OFS( (_ptr_), LBX_HDR_POS_OFFSETS      ) )

#define LBX_GET_ENTRY_START(_lbx_header_,_entry_num_)  ( GET_4B_OFS( (_lbx_header_), ( 8 + ((_entry_num_) * 4)    ) ) )
#define LBX_GET_ENTRY_END(_lbx_header_,_entry_num_)    ( GET_4B_OFS( (_lbx_header_), ( 8 + ((_entry_num_) * 4) + 4) ) )
#define LBX_GET_ENTRY_LENGTH(_lbx_header_,_entry_num_) ( LBX_GET_ENTRY_END( (_lbx_header_), (_entry_num_) ) - LBX_GET_ENTRY_START( (_lbx_header_), (_entry_num_) ) )


SAMB_ptr LBX_Load(char * lbx_name, short entry_num);
SAMB_ptr LBX_Reload(char * lbx_name, short entry_num, SAMB_ptr SAMB_head);
SAMB_ptr LBX_Reload_Next(char * lbx_name, short entry_num, SAMB_ptr SAMB_head);

void File_Name_Base(char * file_name);


#endif /* WIN_LBX_HPP */
