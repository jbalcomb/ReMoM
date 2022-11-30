#ifndef MOX_SA_H
#define MOX_SA_H

/*
    "SPACE ALLOC" - STARLORD.EXE
*/

#include "MoX_TYPE.H"

#include "MoX_BITS.H"   /* MoX_GET_2B_OFS() etc. */


#define MoX_SAMB_RESERVED1   0
#define MoX_SAMB_RESERVED2   2
#define MoX_SAMB_MEMSIG1     4
#define MoX_SAMB_MEMSIG2     6
#define MoX_SAMB_SIZE        8
#define MoX_SAMB_USED       10
#define MoX_SAMB_unknown    12
#define MoX_SAMB_MARKED     14

#define MoX_SA_MEMSIG1      0x12FA
#define MoX_SA_MEMSIG2      0x4ECF

#define MoX_SA_GET_MEMSIG1(_SAMB_head_) ( MoX_GET_2B_OFS((_SAMB_head_),  4) )
#define MoX_SA_GET_MEMSIG2(_SAMB_head_) ( MoX_GET_2B_OFS((_SAMB_head_),  6) )
#define MoX_SA_GET_SIZE(_SAMB_head_)    ( MoX_GET_2B_OFS((_SAMB_head_),  8) )
#define MoX_SA_GET_USED(_SAMB_head_)    ( MoX_GET_2B_OFS((_SAMB_head_), 10) )
#define MoX_SA_SET_MEMSIG1(_SAMB_head_)        ( MoX_SET_2B_OFS((_SAMB_head_),  4, MoX_SA_MEMSIG1) )
#define MoX_SA_SET_MEMSIG2(_SAMB_head_)        ( MoX_SET_2B_OFS((_SAMB_head_),  6, MoX_SA_MEMSIG2) )
#define MoX_SA_SET_SIZE(_SAMB_head_,_nparas_)  ( MoX_SET_2B_OFS((_SAMB_head_),  8, (_nparas_)) )
#define MoX_SA_SET_USED(_SAMB_head_,_nparas_)  ( MoX_SET_2B_OFS((_SAMB_head_), 10, (_nparas_)) )


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    MGC  seg007
*/

// MGC s07p04
int16_t Check_Allocation(MoX_SAMB_ptr SAMB_head);

/*
    MGC  seg008
*/

// MGC s08p07
MoX_SAMB_ptr Allocate_Space(uint16_t size);

// MGC s08p08/09
MoX_SAMB_ptr Allocate_Space_No_Header(uint16_t size);

// MGC s08p12
MoX_SAMB_ptr Allocate_First_Block(MoX_SAMB_ptr SAMB_head, uint16_t size);

// MGC s08p14
MoX_SAMB_ptr Allocate_Next_Block(MoX_SAMB_ptr SAMB_head, uint16_t size);

// MGC s08p15
uint16_t Get_Free_Blocks(MoX_SAMB_ptr SAMB_head);

// MGC s08p19
void Allocation_Error(MoX_word error_num, uint16_t blocks);


#ifdef __cplusplus
}
#endif

#endif /* MOX_SA_H */
