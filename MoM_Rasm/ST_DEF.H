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

static int delete = 0;

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


#define ADD_REL(_ptr_,_rel_) ( (unsigned char _FAR *) MK_FP( FP_SEG((_ptr_)), (FP_OFF((_ptr_)) + (_rel_)) ) )


// you can't &fp a Far Pointer - you only get the offset portion - ~== FP_OFF() || (unsigned int)(fp)
// ? ~== (segment * 16) + offset ?
// no need for (... << 16), case to unsigned long puts the segment portion in the upper half
#define AddressOf(_farptr_) ( ((unsigned long)(_farptr_)) | ((unsigned int)(_farptr_)) )


// Eh? Pointer vs. Address? 1oom uses address
// #define GET_LE_16(_ptr_) ( (_ptr_[1] <<  8) |  _ptr_[0] )
// #define GET_LE_32(_ptr_) ( (_ptr_[3] << 24) | (_ptr_[2] << 16) | (_ptr_[1] << 8) | _ptr_[0] )
// ((unsigned int)((unsigned char *)_addr_)[1]) ... cast the address to a byte pointer, index the value, cast the value to the desired data type/size, left-shift into position
// #define GET_LE_16(_addr_) ( (((unsigned int)((unsigned char *)(_addr_))[1]) <<  8) |  ((unsigned int)((unsigned char *)(_addr_))[0]) )
// #define GET_LE_32(_addr_) ( (((unsigned long)((unsigned char *)(_addr_))[3]) << 24) | (((unsigned long)((unsigned char *)(_addr_))[2]) << 16) | (((unsigned long)((unsigned char *)(_addr_))[1]) <<  8) |  ((unsigned long)((unsigned char *)(_addr_))[0]) )
// // #define FPGET_WORD(_fp_,_rel_) ( *( (word _FAR *) MK_FP( FP_SEG(_fp_), (FP_OFF(_fp_) + (_rel_)) ) ) )

// //#ifdef __DOS16__

#define FPGET_WORD(_fp_) ( *((word _FAR *)MK_FP(FP_SEG(_fp_), FP_OFF(_fp_))) )

#define GET_LE_16(_ptr_) ( (((unsigned int)(((unsigned char _FAR *)(_ptr_))[1])) << 8) | ((unsigned int)(((unsigned char _FAR *)(_ptr_))[0])) )

#define GET_LE_32(_ptr_) ( (((unsigned long)((unsigned char _FAR *)(_ptr_))[3]) << 24) | (((unsigned long)((unsigned char _FAR *)(_ptr_))[2]) << 16) | (((unsigned long)((unsigned char _FAR *)(_ptr_))[1]) <<  8) |  ((unsigned long)((unsigned char _FAR *)(_ptr_))[0]) )
// //#endif

/*
    HOW-TO: Access multi-byte values through a (unsigned char far *)

    unsigned char far * src;
    unsigned char baito;
    baito = *src;
    unsigned char far * src;
    unsigned int wordo;
    wordo = (unsigned int *)src;


void lbxgfx_apply_palette(uint8_t *data)
{
    if (lbxgfx_has_palette(data)) {
        uint8_t *p = lbxgfx_get_palptr(data);
        int first = lbxgfx_get_palfirst(data);
        int num = lbxgfx_get_palnum(data);
        lbxpal_set_palette(p, first, num);
    }
}
#define lbxgfx_get_paloffs(_data_) GET_LE_16(&((_data_)[0x0e]))
#define lbxgfx_get_paldataoffs(_data_) GET_LE_16(&((_data_)[lbxgfx_get_paloffs(_data_)+0x00]))
#define lbxgfx_get_palfirst(_data_) GET_LE_16(&((_data_)[lbxgfx_get_paloffs(_data_)+0x02]))
#define lbxgfx_get_palnum(_data_) GET_LE_16(&((_data_)[lbxgfx_get_paloffs(_data_)+0x04]))
#define lbxgfx_get_palptr(_data_) ((_data_) + lbxgfx_get_paldataoffs(_data_))
...
#define lbxgfx_get_palptr(_data_) ( (_data_) + lbxgfx_get_paldataoffs(_data_))
#define lbxgfx_get_palptr(_data_) ( (_data_) + GET_LE_16(&((_data_)[lbxgfx_get_paloffs(_data_)+0x00])) )
#define lbxgfx_get_palptr(_data_) ( (_data_) + GET_LE_16(&( (_data_)[ GET_LE_16(&((_data_)[0x0e])) + 0x00 ] )) )
So, ...
    lbxgfx_get_palptr takes a pointer and returns a pointer
    lbxgfx_get_paldataoffs takes a pointer and returns an unsigned integer
    GET_LE_16 takes an address and returns an unsigned integer
    GET_LE_16 takes an address, casts it to a pointer, dereferences it to get a value, and then casts that value to an unsigned integer

e.g.,
0xAD 0xFE
LBX_Header
LBX_Header[2] == 0xAD
LBX_Header[3] == 0xFE
∵ Little-Endian ∴ (unsigned int) LBX_Header[2] == 0xADFE
0xFEAD == LBX_Header[3], LBX_Header[2]
unsigned int is 2 bytes
LBX_Header[2] == 0xAD == 10101101
LBX_Header[3] == 0xFE == 11111110
unsigned int
00000000 00000000
(unsigned char) LBX_Header[2] == 10101101
(unsigned char) LBX_Header[3] == 11111110
(unsigned int)  LBX_Header[3] == 00000000 11111110
((unsigned int)  LBX_Header[3]) << 8 == 11111110 00000000
(((unsigned int)  LBX_Header[3]) << 8) | ((unsigned char) LBX_Header[2]) == 11111110 10101101
...
byte pointer to LBX_Header
get byte at index 3
explicitly cast 1-byte value to 2-byte value
left-shift the low-byte into the high-byte
get byte at index 2
OR the low-byte of the 2-byte value with the byte at index 2  ( 00000000 | 10101101 == 10101101)

unsigned int(b[1])<<8 | unsigned int(b[0])

#define GET_LE_16(_p_) (((uint16_t)(((uint8_t const *)(_p_))[0])) | (((uint16_t)(((uint8_t const *)(_p_))[1])) << 8))
...
static int GetLE16(const uint8_t* const data) {
  return (data[0] << 0) | (data[1] << 8);
}
...
unsigned int GET_LE_16( unsigned char * fp_Data)
{
    return ( (fp_Data[1] << 8) | fp_Data[0] );
}
#define GET_LE_16(_ptr_) ( (_ptr_[1] <<  8) |  _ptr_[0] )
#define GET_LE_24(_ptr_) ( (_ptr_[2] << 16) | (_ptr_[1] <<  8) |  _ptr_[0] )
#define GET_LE_32(_ptr_) ( (_ptr_[3] << 24) | (_ptr_[2] << 16) | (_ptr_[1] << 8) | _ptr_[0] )
Eh? GTG?!?




s21p07  FLIC_LoadPalette()
fptr_Src = (unsigned int *)MK_FP(SrcSgmt, SrcOfst);
fh_palette_data_offset = fptr_Src[0];  // Offset to FLIC Palette Data

s20p01  VGA_LoadPalette()
fptr_DstByte = (unsigned char *)MK_FP(DstSgmt, DstOfst);
fptr_SrcByte = (unsigned char *)MK_FP(SrcSgmt, SrcOfst);
for(itr_Color_Count = 0; itr_Color_Count < (Color_Count * 3); itr_Color_Count++)
    *fptr_DstByte++ = *fptr_SrcByte++;

*/

// ST_SA.C
// //#define SET_LE_16(_p_,_v_) ( ((uint8_t *)(_p_))[0] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 8) & 0xffu) )
// //#define SET_LE_32(_p_,_v_) ( ((uint8_t *)(_p_))[0] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 8) & 0xffu), ((uint8_t *)(_p_))[2] = (((_v_) >> 16) & 0xffu), ((uint8_t *)(_p_))[3] = (((_v_) >> 24) & 0xffu))

// //LBX_EntryCount = farpeekw(LBX_Header, 0);
// // LBX_EntryCount = *((unsigned int *)LBX_Header + 0);
// // //#define GET_LE_16(_p_) (((uint16_t)(((uint8_t const *)(_p_))[0])) | (((uint16_t)(((uint8_t const *)(_p_))[1])) << 8))
// // LBX_EntryCount = (((uint16_t)(((uint8_t const *)(LBX_Header))[0])) | (((uint16_t)(((uint8_t const *)(LBX_Header))[1])) << 8));
// LBX_EntryCount = ((unsigned int)*((unsigned char *)LBX_Header + 0)) | (unsigned int)((unsigned int)*((unsigned char *)LBX_Header + 1) << 8);

// #define GET_LE_16(_p_) (((uint16_t)(((uint8_t const *)(_p_))[0])) | (((uint16_t)(((uint8_t const *)(_p_))[1])) << 8))
// #define GET_LE_32(_p_) (((uint32_t)GET_LE_16((_p_))) | (((uint32_t)GET_LE_16((_p_) + 2)) << 16))
// #define lbxgfx_get_w(_data_) GET_LE_16(&((_data_)[0x00]))
// #define lbxgfx_get_h(_data_) GET_LE_16(&((_data_)[0x02]))
// #define lbxgfx_get_frame(_data_) GET_LE_16(&((_data_)[0x04]))
// #define lbxgfx_set_frame(_data_, _v_) SET_LE_16(&((_data_)[0x04]), (_v_))
// #define lbxgfx_set_frame_0(_data_) lbxgfx_set_frame((_data_), 0)
// #define lbxgfx_get_frames(_data_) GET_LE_16(&((_data_)[0x06]))
// #define lbxgfx_get_frames2(_data_) GET_LE_16(&((_data_)[0x08]))
// #define lbxgfx_get_ehandle(_data_) ((_data_)[0x0a])
// #define lbxgfx_get_epage(_data_) ((_data_)[0x0b])
// #define lbxgfx_set_epage(_data_, _v_) ((_data_)[0x0b] = (_v_))
// #define lbxgfx_get_offs0c(_data_) GET_LE_16(&((_data_)[0x0c]))
// #define lbxgfx_get_paloffs(_data_) GET_LE_16(&((_data_)[0x0e]))
// #define lbxgfx_has_palette(_data_) (lbxgfx_get_paloffs(_data_) != 0)
// #define lbxgfx_get_indep(_data_) ((_data_)[0x10])
// #define lbxgfx_get_format(_data_) ((_data_)[0x11])
// #define lbxgfx_get_paldataoffs(_data_) GET_LE_16(&((_data_)[lbxgfx_get_paloffs(_data_)+0x00]))
// #define lbxgfx_get_palfirst(_data_) GET_LE_16(&((_data_)[lbxgfx_get_paloffs(_data_)+0x02]))
// #define lbxgfx_get_palnum(_data_) GET_LE_16(&((_data_)[lbxgfx_get_paloffs(_data_)+0x04]))
// #define lbxgfx_get_paloffs06(_data_) GET_LE_16(&((_data_)[lbxgfx_get_paloffs(_data_)+0x06]))
// #define lbxgfx_get_palptr(_data_) ((_data_) + lbxgfx_get_paldataoffs(_data_))
// #define lbxgfx_get_frameoffs0(_data_, _frame_) (GET_LE_32(&((_data_)[0x12 + 4 * (_frame_)])))
// #define lbxgfx_get_frameoffs1(_data_, _frame_) (GET_LE_32(&((_data_)[0x16 + 4 * (_frame_)])))
// #define lbxgfx_get_frameptr(_data_, _frame_) ((_data_) + lbxgfx_get_frameoffs0((_data_), (_frame_)))
// #define lbxgfx_get_frameclearflag(_data_, _frame_) (*lbxgfx_get_frameptr((_data_), (_frame_)))



#define	DOS_INT			        0x21
#define PRINT_CHAR              0x02        // DL = character to output
#define PRINT_STRING            0x09        // DS:DX = pointer to string ending in "$"
#define	ALLOCMEM                0x48        // CF = {success, failure}; AX = segment address or error code; BX = size in paragraphs (16 B) of the largest block of memory available


extern unsigned int g_LFSR_LO_bits;         // dseg:431E  3568h  13672d  0011 0101 0110 1000b
extern unsigned int g_LFSR_HO_bits;         // dseg:4320


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    seg004
*/

// _s04p01c.c
// int __cdecl __far DISK_FileFind(char *FileName_Ptr, char *Read_Buffer)


#ifdef __cplusplus
}
#endif

#endif /* ST_DEF_H */
