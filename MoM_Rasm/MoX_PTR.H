#ifndef MOX_PTR_H
#define MOX_PTR_H

#include "MoX_TYPE.H"


// e.g., ...
//     // SAMB_head = (MoX_SAMB_ptr) MoX_PTR_ADD_PARAGRAPH(MoX_tmp_SAMB_head,1);
// #if defined(__DOS16__)
//     SAMB_head = (MoX_SAMB_ptr)MoX_MK_FP((MoX_FP_SEG(MoX_tmp_SAMB_head) + 1), 0);
// #endif
// #if defined(__WIN32__)
//     SAMB_head = MoX_tmp_SAMB_head + 12;  // 16-byte paragraph - 4-byte malloc header
// #endif


#ifdef __DOS16__
#define PTR_ADD_PARAGRAPH(_ptr_,_paras_) ( (unsigned char *) MK_FP( ( FP_SEG( (_ptr_) ) + (_paras_)), 0) )
#endif

#ifdef __WIN32__
#define PTR_ADD_PARAGRAPH(_ptr_,_paras_) ( (unsigned char *) (_ptr_) + (16 * _paras_) )
#endif


#define MoX_PTR_INC_PARAGRAPH(_ptr_) ( (MoX_byte *) (_ptr_) + 16 )
#define MoX_PTR_DEC_PARAGRAPH(_ptr_) ( (MoX_byte *) (_ptr_) - 16 )
#define MoX_PTR_ADD_PARAGRAPH(_ptr_,_paras_) ( (MoX_byte *) (_ptr_) + (16 * _paras_) )
#define MoX_PTR_SUB_PARAGRAPH(_ptr_,_paras_) ( (MoX_byte *) (_ptr_) - (16 * _paras_) )


#endif /* MOX_PTR_H */
