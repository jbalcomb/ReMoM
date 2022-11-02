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


#define SA_Get_MemSig1(_SAMB_head_) ( GET_2B_OFS((_SAMB_head_),  4) )

#define SA_Get_MemSig2(_SAMB_head_) ( GET_2B_OFS((_SAMB_head_),  6) )

#define SA_Get_Size(_SAMB_head_)    ( GET_2B_OFS((_SAMB_head_),  8) )

#define SA_Get_Used(_SAMB_head_)    ( GET_2B_OFS((_SAMB_head_), 10) )

#define SA_Set_MemSig1(_SAMB_head_)        ( SET_2B_OFS((_SAMB_head_),  4, SA_MEMSIG1) )

#define SA_Set_MemSig2(_SAMB_head_)        ( SET_2B_OFS((_SAMB_head_),  6, SA_MEMSIG2) )

#define SA_Set_Size(_SAMB_head_,_nparas_)  ( SET_2B_OFS((_SAMB_head_),  8, (_nparas_)) )

#define SA_Set_Used(_SAMB_head_,_nparas_)  ( SET_2B_OFS((_SAMB_head_), 10, (_nparas_)) )


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

// s07p04
int Check_Allocation(SAMB_ptr SAMB_head);

/*
    seg008
*/

// s08p07
SAMB_ptr Allocate_Space(int size);

// s08p08/09
SAMB_ptr Allocate_Space_No_Header(int size);

// SAMB_ptr SA_Alloc_First(SAMB_ptr SAMB_head, int nparas);                // s08p12
// SAMB_ptr SA_Alloc_Next(SAMB_ptr SAMB_head, int nparas);                 // s08p14

// s08p15
unsigned int Get_Free_Blocks(SAMB_ptr SAMB_head);

// SAMB_ptr SA_MK_FP0(SAMB_addr sgmt_addr);                                // s08p16

// s08p19
void Allocation_Error(int error_num, int blocks);


#ifdef __cplusplus
}
#endif

#endif /* MOX_SA_H */
