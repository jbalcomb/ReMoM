#ifndef ALLOCATE_H
#define ALLOCATE_H

/*
    "SPACE ALLOC" - STARLORD.EXE
*/



#include "MoX_TYPE.H"   /* SAMB_ptr, etc. */
#include "MoX_BITS.H"   /* MoX_GET_2B_OFS() etc. */


enum e_SA_Allocation_Type
{
    sa_Single,
    sa_First,
    sa_Next,
    sa_Invalid
};


#define SAMB_RESERVED1   0
#define SAMB_RESERVED2   2
#define SAMB_MEMSIG1     4
#define SAMB_MEMSIG2     6
#define SAMB_SIZE        8
#define SAMB_USED       10
#define SAMB_unknown    12
#define SAMB_MARK       14

#define SA_MEMSIG1      0x12FA
#define SA_MEMSIG2      0x4ECF

#define SA_GET_MEMSIG1(_SAMB_head_) ( GET_2B_OFS((_SAMB_head_), SAMB_MEMSIG1) )
#define SA_GET_MEMSIG2(_SAMB_head_) ( GET_2B_OFS((_SAMB_head_), SAMB_MEMSIG2) )
#define SA_GET_SIZE(_SAMB_head_)    ( GET_2B_OFS((_SAMB_head_), SAMB_SIZE)    )
#define SA_GET_USED(_SAMB_head_)    ( GET_2B_OFS((_SAMB_head_), SAMB_USED)    )
#define SA_SET_MEMSIG1(_SAMB_head_)        ( SET_2B_OFS((_SAMB_head_), SAMB_MEMSIG1, SA_MEMSIG1) )
#define SA_SET_MEMSIG2(_SAMB_head_)        ( SET_2B_OFS((_SAMB_head_), SAMB_MEMSIG2, SA_MEMSIG2) )
#define SA_SET_SIZE(_SAMB_head_,_nparas_)  ( SET_2B_OFS((_SAMB_head_), SAMB_SIZE, (_nparas_))    )
#define SA_SET_USED(_SAMB_head_,_nparas_)  ( SET_2B_OFS((_SAMB_head_), SAMB_USED, (_nparas_))    )
#define SA_SET_MARK(_SAMB_head_,_nparas_)  ( SET_2B_OFS((_SAMB_head_), SAMB_MARK, (_nparas_))    )


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    MGC  seg007
*/

// MGC s07p04
int16_t Check_Allocation(SAMB_ptr SAMB_head);

/*
    MGC  seg008
*/

// MGC s08p07
SAMB_ptr Allocate_Space(uint16_t size);

// MGC s08p08/09
SAMB_ptr Allocate_Space_No_Header(uint16_t size);

// MGC s08p12
SAMB_ptr Allocate_First_Block(SAMB_ptr SAMB_head, uint16_t size);

// MGC s08p14
SAMB_ptr Allocate_Next_Block(SAMB_ptr SAMB_head, uint16_t size);

// MGC s08p15
uint16_t Get_Free_Blocks(SAMB_ptr SAMB_head);

// MGC s08p19
void Allocation_Error(uint16_t error_num, uint16_t blocks);


#ifdef __cplusplus
}
#endif

#endif /* ALLOCATE_H */
