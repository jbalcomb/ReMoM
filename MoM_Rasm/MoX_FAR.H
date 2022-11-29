#ifndef MoX_FAR_H
#define MoX_FAR_H

#include "MoX_TYPE.H"


#define MoX_MK_FP(_sgmt_,_ofst_)    ( (void *) ( ((MoX_dword) (_sgmt_) << 16) | (_ofst_) ) )
#define MoX_FP_SEG(_fp_)            ( (MoX_word) ((MoX_dword)((void *)(_fp_)) >> 16) )
#define MoX_FP_OFF(_fp_)            ( (MoX_word) (_fp_) )

// #define MoX_MK_FP(_sgmt_,_ofst_)    ( (void *) ( ((unsigned long) (_sgmt_) << 16) | (_ofst_) ) )
// #define MoX_FP_SEG(_fp_)            ( (unsigned int) ( (unsigned long)((void *)(_fp_)) >> 16) )
// #define MoX_FP_OFF(_fp_)            ( (unsigned int) (_fp_) )


#endif  /* MoX_FAR_H */
