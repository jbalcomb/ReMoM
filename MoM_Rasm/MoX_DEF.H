#ifndef win32_MOM_DEF_H
#define win32_MOM_DEF_H


#define ST_TRUE  1
#define ST_FALSE 0
#define ST_UNDEFINED -1

#define ST_SUCCESS -1
#define ST_FAILURE  0

#ifndef NULL
#define NULL 0
#endif /* NULL */

#if defined( __DOS16__ )
#if !defined( __DOS_H )
#include <DOS.H>
#endif
#if !defined( __DEFS_H )
#include <_defs.h>
#endif
#undef MK_FP
#undef FP_SEG
#undef FP_OFF
#define MK_FP(_sgmt_,_ofst_) ( (void *) ( ((unsigned long) (_sgmt_) << 16) | (_ofst_) ) )
#define FP_SEG(_fp_) ( (unsigned int) ( (unsigned long)((void *)(_fp_)) >> 16) )
#define FP_OFF(_fp_) ( (unsigned int) (_fp_) )
#define PTR_INC_PARAGRAPH(_ptr_) ( (unsigned char *) MK_FP( ( FP_SEG( (_ptr_) ) + 1), 0) )
#define PTR_DEC_PARAGRAPH(_ptr_) ( (unsigned char *) MK_FP( ( FP_SEG( (_ptr_) ) - 1), 0) )
#define PTR_ADD_PARAGRAPH(_ptr_,_paras_) ( (unsigned char *) MK_FP( ( FP_SEG( (_ptr_) ) + (nparas)), 0) )
#define PTR_SUB_PARAGRAPH(_ptr_,_paras_) ( (unsigned char *) MK_FP( ( FP_SEG( (_ptr_) ) - (nparas)), 0) )
#endif
#if defined( __WIN32__ )
#define PTR_INC_PARAGRAPH(_ptr_) ( (unsigned char *) (_ptr_) + 16 )  // unsigned char * offset = data + 512; // same as &data[512]
#define PTR_DEC_PARAGRAPH(_ptr_) ( (unsigned char *) (_ptr_) - 16 )
#define PTR_ADD_PARAGRAPH(_ptr_,_paras_) ( (unsigned char *) (_ptr_) + (16 * nparas) )
#define PTR_SUB_PARAGRAPH(_ptr_,_paras_) ( (unsigned char *) (_ptr_) - (16 * nparas) )
#endif


#endif /* MoX_ST_HEAD_H */
