#ifndef MOX_SA_H
#define MOX_SA_H

#include "MoX_TYPE.H"

#define SAMB_RESERVED1   0
#define SAMB_RESERVED2   2
#define SAMB_MEMSIG1     4
#define SAMB_MEMSIG2     6
#define SAMB_SIZE        8
#define SAMB_USED       10
#define SAMB_unknown    12
#define SAMB_MARKED     14

#define SA_MEMSIG1      0x12FA
#define SA_MEMSIG2      0x4ECF

extern SAMB_ptr pTmpSAMB;


// #define GET_SAMB_USED(_SAMB_Head_) ( GET_LE_16((_SAMB_Head_)) )
// #define GET_SAMB_SIZE(_SAMB_Head_) ( GET_LE_16((_SAMB_Head_)) )
// #define SET_SAMB_USED(_SAMB_Head_) ( SET_LE_16((_SAMB_Head_),_value_) )


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg007
*/
// //int SA_Alloc_Validate(sgmt_addr SAMB_head)                                  // s07p04
// //int SA_Alloc_Validate(SAMB_addr pSAMB_head)                                 // s07p04
// int SA_Alloc_Validate(SAMB_ptr pSAMB_head);  // returns ST_STATUS: SUCCESS -1, FAILURE 0
// unsigned char farpeekb(unsigned int sgmt, unsigned int ofst);               // s07p05
// unsigned int farpeekw(unsigned int sgmt, unsigned int ofst);                // s07p06
// unsigned long farpeekdw(unsigned int sgmt, unsigned int ofst);              // s07p07
// void farpokeb(unsigned int sgmt, unsigned int ofst, unsigned char value);   // s07p08
// void farpokew(unsigned int sgmt, unsigned int ofst, unsigned short value);  // s07p09
// // UU? void farpokedw(unsigned int sgmt, unsigned int ofst, unsigned long val);  // s07p10

/*
    seg008
*/
// SAMB_ptr SA_Allocate_Space(unsigned int nparas);                        // s08p07
SAMB_ptr SA_Allocate_MemBlk(unsigned int nparas);                       // s08p08/09
// // unsigned int SA_Allocate_MemBlk_LbxHeader(unsigned int nparas);      // s08p08
// // unsigned int SA_Allocate_MemBlk_EntrySize(unsigned int nparas);      // s08p09
// SAMB_ptr SA_Alloc_First(SAMB_ptr SAMB_head, int nparas);                // s08p12
// SAMB_ptr SA_Alloc_Next(SAMB_ptr SAMB_head, int nparas);                 // s08p14
// unsigned int SA_GetFree(SAMB_ptr SAMB_head);                            // s08p15
// SAMB_ptr SA_MK_FP0(SAMB_addr sgmt_addr);                                // s08p16
void SA_Alloc_Error(int SA_Error_Number, int blocks);                              // s08p19

#ifdef __cplusplus
}
#endif

#endif /* MOX_SA_H */
