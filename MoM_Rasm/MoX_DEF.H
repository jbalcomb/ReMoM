#ifndef MoX_DEF_H
#define MoX_DEF_H


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

/*
    get multi-bytes value from single-byte pointer
*/
// ( (unsigned int) ((unsigned char *)(g_LBX_Header))[3] << 8 ) | ( (unsigned int) ((unsigned char *)(g_LBX_Header))[2] );
// g_LBX_Header[2] | g_LBX_Header[3] << 8;
// *(g_LBX_Header + 2) | *(g_LBX_Header + 3) << 8;
#define GET_2B(_ptr_) ( (unsigned int)*((_ptr_) + 1) << 8 | (unsigned int)*((_ptr_) + 0) )
#define GET_4B(_ptr_) ( (unsigned int)*((_ptr_) + 1) << 8 | (unsigned int)*((_ptr_) + 0) )
// #define SET_2B(_p_,_v_) (((uint8_t *)(_p_))[0] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 8) & 0xffu))
// #define SET_4B(_p_,_v_) (((uint8_t *)(_p_))[0] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 8) & 0xffu), ((uint8_t *)(_p_))[2] = (((_v_) >> 16) & 0xffu), ((uint8_t *)(_p_))[3] = (((_v_) >> 24) & 0xffu))

// #define GET_LE_16(_ptr_) ( (_ptr_[1] <<  8) |  _ptr_[0] )
// #define GET_LE_32(_ptr_) ( (_ptr_[3] << 24) | (_ptr_[2] << 16) | (_ptr_[1] << 8) | _ptr_[0] )
// ((unsigned int)((unsigned char *)_addr_)[1]) ... cast the address to a byte pointer, index the value, cast the value to the desired data type/size, left-shift into position
// #define GET_LE_16(_addr_) ( (((unsigned int)((unsigned char *)(_addr_))[1]) <<  8) |  ((unsigned int)((unsigned char *)(_addr_))[0]) )
// #define GET_LE_32(_addr_) ( (((unsigned long)((unsigned char *)(_addr_))[3]) << 24) | (((unsigned long)((unsigned char *)(_addr_))[2]) << 16) | (((unsigned long)((unsigned char *)(_addr_))[1]) <<  8) |  ((unsigned long)((unsigned char *)(_addr_))[0]) )
// // #define FPGET_WORD(_fp_,_rel_) ( *( (word _FAR *) MK_FP( FP_SEG(_fp_), (FP_OFF(_fp_) + (_rel_)) ) ) )

    // extern uint8_t *bytes;
    // uint32_t myInt1 = bytes[0] + (bytes[1] << 8) + (bytes[2] << 16) + (bytes[3] << 24);

// #define GET_LE_16(_p_) (((uint16_t)(((uint8_t const *)(_p_))[0])) | (((uint16_t)(((uint8_t const *)(_p_))[1])) << 8))
// #define GET_LE_32(_p_) (((uint32_t)GET_LE_16((_p_))) | (((uint32_t)GET_LE_16((_p_) + 2)) << 16))
// #define GET_LE_24(_p_) (((uint32_t)GET_LE_16((_p_))) | (((uint32_t)*(((uint8_t const *)(_p_)) + 2)) << 16))
// #define SET_LE_16(_p_,_v_) (((uint8_t *)(_p_))[0] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 8) & 0xffu))
// #define SET_LE_32(_p_,_v_) (((uint8_t *)(_p_))[0] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 8) & 0xffu), ((uint8_t *)(_p_))[2] = (((_v_) >> 16) & 0xffu), ((uint8_t *)(_p_))[3] = (((_v_) >> 24) & 0xffu))

// https://e2e.ti.com/support/microcontrollers/msp-low-power-microcontrollers-group/msp430/f/msp-low-power-microcontroller-forum/408577/pointer-uint16_t
// #include "stdint.h" 
// unsigned char buffer[6] = {0x00,0x12,0x34,0x56,0x78,0x90}; 
// int main(void)
// {
//     unsigned int var_uint;
//     var_uint = *((unsigned int *)&buffer[1]); // get address of offset by array indexing, cast to unsigned int pointer, dereference pointer to get value
//     //value in var_int is 0x1200
//     return 0; 
// }

// https://stackoverflow.com/questions/3872704/what-is-the-correct-way-to-offset-a-pointer
// If your array is unsigned char then you just add however many bytes offset you wish to go into the array, e.g.
// unsigned char data[16384];
// unsigned char * offset = data + 512; // same as &data[512]
// *offset = 5; // same as data[512] = 5;
// // ? *(data + 512) = 5; // same as data[512] = 5;


// ?!? something about casting both sides being safer because of sign extended differences in specifications, compilers, platforms, etc. ?!?
// ?!? casting to (void *) then (unsigned char *) because any pointer of type can be cast to (void *) and a void pointer can be cast to a point of any type ?!?
// ...which is just protecting against passing in a pointer that is not already a point of type unsigned char?
// ...cause whatever, but the only real concerns would be what the compiler spits out and being sure to document the why and what-for?
// #define READ_INT(address) (((unsigned char *)(void*)address)[0]|(((int)((unsigned char *)(void*)address)[1])<<8))

// as it stands, this works just fine
//     LBX_MagSig = ( (unsigned int) ((unsigned char *)(g_LBX_Header))[3] << 8 ) | ( (unsigned int) ((unsigned char *)(g_LBX_Header))[2] );
// as it stands, this works just fine
//     LBX_MagSig =   g_LBX_Header[2]   |   g_LBX_Header[3]   << 8;  // 0xFEAD
// ~== LBX_MagSig = *(g_LBX_Header + 2) | *(g_LBX_Header + 3) << 8;

// 06 00 AD FE 00 00 00 00 C0 02 00 00 86 3A 02 00 B6 40 02 00 33 48 02 00 43 4F 02 00 10 59 02 00 9F FF 02 00
// 06 00 
// AD FE 
// 00 00 
// 00 00 
// C0 02 00 00 
// 86 3A 02 00 
// B6 40 02 00 
// 33 48 02 00 
// 43 4F 02 00 
// 10 59 02 00 
// 9F FF 02 00
// 9F 
// FF 
// 02 
// 00
// unsigned char * data;
// Ofs_Beg 8 + (4 * LBX_Index)
// Ofs_End 8 + (4 * LBX_Index + 1)
// data + Ofs_End





#endif /* MoX_DEF_H */
