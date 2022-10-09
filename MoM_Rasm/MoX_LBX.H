#ifndef MOX_LBX_H
#define MOX_LBX_H

#include "MoX_TYPE.H"   /* SAMB_ptr */
#include "MoX_DEF.H"    /* GET_2B() */


char *cnst_LBXErr_Common1 = ".LBX [entry ";                                         // MGC dseg:3EA3
char *cnst_LBXErr_Common2 = "] ";                                                   // MGC dseg:3EB0
char *cnst_LBX_Error1 = " could not be found.";                                     // MGC dseg:3EB3
char *cnst_LBX_Error2 = " has been corrupted.";                                     // MGC dseg:3EC8
char *cnst_LBX_Error31 = "Insufficient memory. You need at least ";                 // MGC dseg:3EDD
char *cnst_LBX_Error32 = "K free. Try removing all TSR's.";                         // MGC dseg:3F05
char *cnst_LBX_Error4 = " was not properly allocated or has been corrupted.";       // MGC dseg:3F25
char *cnst_LBX_Error51 = " failed to reload. Allocation too small by ";             // MGC dseg:3F58
char *cnst_LBX_Error52 = " pages";                                                  // MGC dseg:3F84
char *cnst_LBX_Error7 = " is not an LBX file";                                      // MGC dseg:3F8B
char *cnst_LBX_Error8 = " exceeds number of LBX entries";                           // MGC dseg:3F9F
char *cnst_LBX_Error9 = " has an incorrect record size";                            // MGC dseg:3FBE
char *cnst_LBX_ErrorA = " exceeds number of defined records";                       // MGC dseg:3FDC
char *cnst_LBX_ErrorB = " cannot be reloaded into EMS w/o being first released.";   // MGC dseg:3FFF
char *cnst_LBX_ErrorC = " EMM loading error. Insufficient EMM.";                    // MGC dseg:4036
char *cnst_LBX_ErrorD = " Only pictures may be loaded into reserved EMM";           // MGC dseg:405C
char *cnst_LBX_ErrorE = " (Reserved EMM) ";                                         // MGC dseg:408B
char *cnst_LBX_ErrorF1 = " LBX to";                                                 // MGC dseg:409C
char *cnst_LBX_ErrorG = " Vga file animation frames cannot exceed 65536 bytes";     // MGC dseg:40A4


// // #define DOS_UnusedFileHandle 0xFFFF  // Warning: Constant out of range in comparison ...
// #define DOS_UnusedFileHandle -1
// #define ST_NULL 0
// 
#define SZ_LBX_HDR_B 512
#define SZ_LBX_HDR_PR 32  // 32 paragraphs * 16 bytes per paragaph = 512 bytes
// #define SZ_LBX_HDR_PRE_B 8
// #define SZ_LBX_HDR_OFST_B 4
// #define SZ_16K_B 16384
// #define SZ_32K_B 32768
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

// s10p01
SAMB_ptr MoX_LBXE_LoadSingle(char * LBX_Name, int LBX_Index);

// _s10p10
SAMB_ptr MoX_LBX_Load_Entry(char * LBX_Name, int LBX_Index, SAMB_ptr SAMB_head, int LoadType, int LbxHdrFmt);

// s10p15
void LBX_Error(char * LBX_Name, int LBX_Error_Number, int LBX_Index, int pages);


#ifdef __cplusplus
}
#endif

#endif /* MOX_LBX_H */
