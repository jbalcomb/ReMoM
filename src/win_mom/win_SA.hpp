#ifndef WIN_SA_HPP
#define WIN_SA_HPP

#include "win_BITS.hpp"

/*
    "SPACE ALLOC", per STARLORD.EXE
*/



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

#define SA_GET_MEMSIG1(_SAMB_head_) ( GET_2B_OFS((_SAMB_head_),  4) )

#define SA_GET_MEMSIG2(_SAMB_head_) ( GET_2B_OFS((_SAMB_head_),  6) )

#define SA_GET_SIZE(_SAMB_head_)    ( GET_2B_OFS((_SAMB_head_),  8) )

#define SA_GET_USED(_SAMB_head_)    ( GET_2B_OFS((_SAMB_head_), 10) )

#define SA_SET_MEMSIG1(_SAMB_head_)        ( SET_2B_OFS((_SAMB_head_),  4, SA_MEMSIG1) )

#define SA_SET_MEMSIG2(_SAMB_head_)        ( SET_2B_OFS((_SAMB_head_),  6, SA_MEMSIG2) )

#define SA_SET_SIZE(_SAMB_head_,_nparas_)  ( SET_2B_OFS((_SAMB_head_),  8, (_nparas_)) )

#define SA_SET_USED(_SAMB_head_,_nparas_)  ( SET_2B_OFS((_SAMB_head_), 10, (_nparas_)) )


SAMB_ptr Allocate_Space(int size);
SAMB_ptr Allocate_Space_No_Header(int size);


#endif /* WIN_SA_HPP */
