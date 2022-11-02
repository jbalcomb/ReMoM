#ifndef MOX_LBX_H
#define MOX_LBX_H

#include "MoX_TYPE.H"   /* SAMB_ptr */
#include "MoX_DEF.H"    /* GET_2B() */



// // #define DOS_UnusedFileHandle 0xFFFF  // Warning: Constant out of range in comparison ...
// #define DOS_UnusedFileHandle -1
// #define ST_NULL 0
// 
#define SZ_LBX_HDR_B 512
#define SZ_LBX_HDR_PR 32  // 32 paragraphs * 16 bytes per paragaph = 512 bytes
// #define SZ_LBX_HDR_PRE_B 8
// #define SZ_LBX_HDR_OFST_B 4
// #define SZ_16K_B 16384
#define SZ_32K_B 32768
// #define SZ_32K_PR 2048
#define SZ_PARAGRAPH_B 16
// 
#define LBX_MAGSIG 0xFEAD
// 
// #define LBX_TYPE_PICTURES 0

#define GET_LBX_HDR_COUNT(_pLbxHeader_)     ( GET_2B((_pLbxHeader_) + 0) )
#define GET_LBX_HDR_MAGSIG(_pLbxHeader_)    ( GET_2B((_pLbxHeader_) + 2) )
#define GET_LBX_HDR_RESERVED(_pLbxHeader_)  ( GET_2B((_pLbxHeader_) + 4) )
#define GET_LBX_HDR_TYPE(_pLbxHeader_)      ( GET_2B((_pLbxHeader_) + 6) )


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

SAMB_ptr MoX_Explore_LBX_Load_Entry(char * LBX_Name, int LBX_Index, SAMB_ptr SAMB_head, int LoadType, int LbxHdrFmt);

/*
    seg009
*/


/*
    seg010
*/

// MGC  s10p01
SAMB_ptr Farload(char * lbx_name, int entry_num);
// MGC  s10p02
SAMB_ptr Far_Reload(char * lbx_name, int entry_num, SAMB_ptr base_seg);
// MGC  s10p02
SAMB_ptr Far_Reload_Next(char * lbx_name, int entry_num, SAMB_ptr base_seg);

// MGC  s10p10
SAMB_ptr Farload_Library(char * lbx_name, signed int entry_num, SAMB_ptr base_seg, signed int allocation_type);
SAMB_ptr MoX_LBX_Load_Entry(char * LBX_Name, int LBX_Index, SAMB_ptr SAMB_head, int LoadType, int LbxHdrFmt);

// MGC  s10p15
void Error_Handler(char * file_name, int error_num, int entry_num, int pages);


#ifdef __cplusplus
}
#endif

#endif /* MOX_LBX_H */
