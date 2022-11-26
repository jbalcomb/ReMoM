#ifndef ST_DEF_H
#define ST_DEF_H


// #if defined(__FARFUNCS__)
// #if     LDATA
// #if     !defined(__HUGE__)

/*
    Hey, Visual Studio Code. Sshhh....
*/
// defined in VS Code Configuration ... #define __VSCODE__ 1
#ifdef __VSCODE__
#define asm _asm
//unsigned char _AL_;
//extern unsigned char _AL_;
//#define _AL _AL_
extern unsigned char _AL;
extern unsigned char _AL;
extern unsigned char _AH;
extern unsigned int _AX;
extern unsigned char _BL;
extern unsigned char _BH;
extern unsigned int _BX;
extern unsigned char _CL;
extern unsigned char _CH;
extern unsigned int _CX;
extern unsigned char _DL;
extern unsigned char _DH;
extern unsigned int _DX;
extern unsigned int _SI;
extern unsigned int _DI;
extern unsigned int _DS;
extern unsigned int _ES;
#endif

#ifndef STU_DEBUG
#define DLOG
#endif

#ifndef __WIN32__
static int delete = 0;
#endif

#define ST_TRUE  1
#define ST_FALSE 0
#define ST_UNDEFINED -1

#define ST_SUCCESS -1
#define ST_FAILURE  0



// // /j/STU/DBWD/developc/flilib/flisrc ... aatypes.h
// #ifndef NULL
// #define NULL ((void *)0L)
// #endif /* NULL */
#ifndef NULL
#define NULL 0
#endif /* NULL */


// Define macro to swap two numbers
// #define SWAP(x, y, temp) temp = x; x = y; y = temp;  ... macro expansion(temp = x; x = y; y = temp;)
// x = x + y - ( y = x );
// #define SWAP(x, y, T) do { T SWAP = x; x = y; y = SWAP; } while (0)  ...  invoke: SWAP(a, b, int);
#define SWAP(x, y) (x ^= y ^= x ^= y)

// #ifndef __WIN32__
#if !defined( __DOS_H )
#include <DOS.H>
#endif
#if !defined( __DEFS_H )
#include <_defs.h>
#endif

#undef MK_FP
#undef FP_SEG
#undef FP_OFF
#define MK_FP(_sgmt_,_ofst_)    ( (void *) ( ((unsigned long) (_sgmt_) << 16) | (_ofst_) ) )
#define FP_SEG(_fp_)            ( (unsigned int) ( (unsigned long)((void *)(_fp_)) >> 16) )
#define FP_OFF(_fp_)            ( (unsigned int) (_fp_) )
// #endif  /* #ifndef __WIN32__ */

// #if defined( __DOS16__ )
#define PTR_INC_PARAGRAPH(_ptr_) ( (unsigned char *) MK_FP( ( FP_SEG( (_ptr_) ) + 1), 0) )
#define PTR_DEC_PARAGRAPH(_ptr_) ( (unsigned char *) MK_FP( ( FP_SEG( (_ptr_) ) - 1), 0) )
#define PTR_ADD_PARAGRAPH(_ptr_,_paras_) ( (unsigned char *) MK_FP( ( FP_SEG( (_ptr_) ) + (_paras_)), 0) )
#define PTR_SUB_PARAGRAPH(_ptr_,_paras_) ( (unsigned char *) MK_FP( ( FP_SEG( (_ptr_) ) - (_paras_)), 0) )
// #endif  /* #if defined( __DOS16__ ) */

// Far Pointer - Add Relative Offset
#define ADD_REL(_ptr_,_rel_) ( (unsigned char _FAR *) MK_FP( FP_SEG((_ptr_)), (FP_OFF((_ptr_)) + (_rel_)) ) )
#define FP_ADD_REL(_ptr_,_rel_) ( (unsigned char _FAR *) MK_FP( FP_SEG((_ptr_)), (FP_OFF((_ptr_)) + (_rel_)) ) )

// you can't &fp a Far Pointer - you only get the offset portion - ~== FP_OFF() || (unsigned int)(fp)
// ? ~== (segment * 16) + offset ?
// no need for (... << 16), case to unsigned long puts the segment portion in the upper half
#define AddressOf(_farptr_) ( ((unsigned long)(_farptr_)) | ((unsigned int)(_farptr_)) )


// #ifdef __SEGMENTED_MEMORY__

// unsigned char * MK_PTR( unsigned char * _ptr_, unsigned short int _ofs_)
// if/when/ever need to add the far pointer's current offset portion to the segment portion?
#define MK_PTR(_ptr_,_ofst_)  ( (unsigned char *) MK_FP(FP_SEG((_ptr_)),(FP_OFF((_ptr_))+(_ofst_))) )

#define MK_PTR_SGMT(_ptr_,_sgmt_,_ofst_)  ( MK_FP( FP_SEG((_ptr_)) + (_sgmt_), (_ofst_) ) )

#define MK_PTR_OFST(_ptr_,_ofst_)         ( MK_FP( FP_SEG((_ptr_)), (FP_OFF((_ptr_))+(_ofst_))) )

// e.g., pointer to VRAM
// the draw functions need to take the pointer to VRAM and add the screen-page offset to the segment portion of the far pointer
//   and set the scanline offset in the offset portion of the far pointer
// LM:  byte * p_VRAM;  p_Vbuf = p_VRAM + (y * pitch + x);
// SM:  byte * fp_VRAM = 0xA000;  fp_Vbuf = MK_FP( FP_SEG(fp_VRAM) + (y * 80 /16), (x / 4)  )
// Use-Cases:
// Use-Case 1: have a Far Pointer, need to add to SGMT and set OFST
// Use-Case 2: have a Far Pointer, need to add to OFST


// #ifdef __LINEAR_MEMORY__

// #define MK_PTR(_ptr_,_ofs_)  ( (unsigned char *) ( (_ptr_) + (_ofs_) )  // ~++ &_ptr_[_ofs_]



#define GET_LE_16(_ptr_) ( (((unsigned int)(((unsigned char _FAR *)(_ptr_))[1])) << 8) | ((unsigned int)(((unsigned char _FAR *)(_ptr_))[0])) )

#define GET_LE_32(_ptr_) ( (((unsigned long)((unsigned char _FAR *)(_ptr_))[3]) << 24) | (((unsigned long)((unsigned char _FAR *)(_ptr_))[2]) << 16) | (((unsigned long)((unsigned char _FAR *)(_ptr_))[1]) <<  8) |  ((unsigned long)((unsigned char _FAR *)(_ptr_))[0]) )


#define GET_2B(_ptr_) ( (unsigned int)                                                *((_ptr_) + 1) << 8 | *((_ptr_) + 0) )

#define GET_4B(_ptr_) ( (unsigned long) *((_ptr_) + 3) << 24 | *((_ptr_) + 2) << 16 | *((_ptr_) + 1) << 8 | *((_ptr_) + 0) )

// cast _ptr_ to (unsigned char *), index/add offset, set low byte of value with value and high byte cleared, set high byte of value with high byte shifted low and high byte cleared

#define SET_2B(_ptr_,_val_) (                                                                 *((_ptr_) + 1) = (_val_) >> 8; *((_ptr_) + 0) = (_val_); )

#define SET_4B(_ptr_,_val_) ( *((_ptr_) + 3) = (_val_) >> 24; *((_ptr_) + 2) = (_val_) >> 16; *((_ptr_) + 1) = (_val_) >> 8; *((_ptr_) + 0) = (_val_); )

// ( (uint8_t *) (_p_) )[0] = ((_v_) & 0xffu )
// ( (uint8_t *) (_p_) )[1] = (((_v_) >> 8) & 0xffu )
// `_val_ >> 8` vs. `(_val_) >> 8` :: protect _val_ from macro expansion operator precence issues
// `_val_ >> 8` vs. `(_val_ >> 8) & 0xFF` :: explcitly clear the high byte AFTER the shift ? because sign-bit or 1's complement platforms ?


#define FP_GET_2B_OFS(_farptr_,_ofs_) ( GET_2B( (unsigned char far *)MK_FP(FP_SEG(_farptr_),FP_OFF(_farptr_)+_ofs_) ) )

#define FP_GET_4B_OFS(_farptr_,_ofs_) ( GET_4B( (unsigned char far *)MK_FP(FP_SEG(_farptr_),FP_OFF(_farptr_)+_ofs_) ) )

// replaces farpeekb()
#define GET_1B_OFS(_ptr_,_ofs_) ( (unsigned char)                                                                                             *((_ptr_) + _ofs_ + 0) )

// replaces farpeekw()
// #define GET_2B_OFS(_ptr_,_ofs_) ( (unsigned int)                                                                *((_ptr_) + _ofs_ + 1) << 8 | *((_ptr_) + _ofs_ + 0) )
#define GET_2B_OFS(_ptr_,_ofs_) ( (unsigned int)                                                                (*((_ptr_) + _ofs_ + 1) << 8) | (*((_ptr_) + _ofs_ + 0)) )

// replaces farpeekdw()
#define GET_4B_OFS(_ptr_,_ofs_) ( (unsigned long) *((_ptr_) + _ofs_ + 3) << 24 | *((_ptr_) + _ofs_ + 2) << 16 | *((_ptr_) + _ofs_ + 1) << 8 | *((_ptr_) + _ofs_ + 0) )

// replaces farpokeb()
#define SET_1B_OFS(_ptr_,_ofs_,_val_) ( (unsigned char)                                                                                             *((_ptr_) + _ofs_ + 0) )

// replaces farpokew()
#define SET_2B_OFS(_ptr_,_ofs_,_val_) ( *((_ptr_) + (_ofs_) + 1) = (_val_) >> 8, *((_ptr_) + (_ofs_) + 0) = (_val_) )

// replaces farpokedw()
#define SET_4B_OFS(_ptr_,_ofs_,_val_) ( (unsigned long) *((_ptr_) + _ofs_ + 3) << 24 | *((_ptr_) + _ofs_ + 2) << 16 | *((_ptr_) + _ofs_ + 1) << 8 | *((_ptr_) + _ofs_ + 0) )



#define	DOS_INT			        0x21
#define PRINT_CHAR              0x02        // DL = character to output
#define PRINT_STRING            0x09        // DS:DX = pointer to string ending in "$"
#define	ALLOCMEM                0x48        // CF = {success, failure}; AX = segment address or error code; BX = size in paragraphs (16 B) of the largest block of memory available


extern unsigned int g_LFSR_LO_bits;         // dseg:431E  3568h  13672d  0011 0101 0110 1000b
extern unsigned int g_LFSR_HO_bits;         // dseg:4320


#endif /* ST_DEF_H */
