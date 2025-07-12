#ifndef MOX_BITS_H
#define MOX_BITS_H

#include "MOX_TYPE.H"



#define LO_WORD(l)               ( (uint16_t) ( ( ( (uint32_t)(l) )       ) & 0xFFFF) )
#define HI_WORD(l)               ( (uint16_t) ( ( ( (uint32_t)(l) ) >> 16 ) & 0xFFFF) )
#define LO_BYTE(w)               ( ( uint8_t) ( ( ( (uint16_t)(w) )       ) & 0xFF  ) )
#define HI_BYTE(w)               ( ( uint8_t) ( ( ( (uint16_t)(w) ) >>  8 ) & 0xFF  ) )
#define LO_NIBBLE(b)             ( ( uint8_t) ( ( ( ( uint8_t)(b) )       ) & 0x0F  ) )
#define HI_NIBBLE(b)             ( ( uint8_t) ( ( ( ( uint8_t)(b) ) >>  4 ) & 0xF0  ) )

/*
Per The Googs...
Search Labs | AI Overview

…

The C macros for extracting the high and low nibbles from a byte are:
HI_NIBBLE(b): Defined as (((b) >> 4) & 0x0F)
LO_NIBBLE(b): Defined as ((b) & 0x0F) 
In these macros, b must be a variable or constant of an integral data type. Only the least-significant byte of b is used. For example, HI_NIBBLE(0xAB)==0xA and LO_NIBBLE(0xAB)==0xB. 
A nibble is four consecutive binary digits, or half of an 8-bit byte. It can be either the first four bits or the last four bits of a byte. The upper nibble is the most significant half of a number, while the lower nibble is the least significant half. 

***Generative AI is experimental.***
*/

// C:\STU\developp\1oom\src\bits.h

// DEDU  ¿ should case to (void *) ?
#define GET_1B_OFS(_ptr_,_ofs_) (                              \
  ( (uint8_t)  ( (uint8_t) *( (uint8_t *)(_ptr_) + (_ofs_) + 0) )       )   \
)

#define GET_2B_OFS(_ptr_,_ofs_) (                              \
  ( (uint16_t) ( (uint8_t) *( (uint8_t *)(_ptr_) + (_ofs_) + 0) )       )   \
| ( (uint16_t) ( (uint8_t) *( (uint8_t *)(_ptr_) + (_ofs_) + 1) ) <<  8 )   \
)

#define GET_3B_OFS(_ptr_,_ofs_) (                              \
  ( (uint32_t) ( (uint8_t) *( (uint8_t *)(_ptr_) + (_ofs_) + 0) )       )   \
| ( (uint32_t) ( (uint8_t) *( (uint8_t *)(_ptr_) + (_ofs_) + 1) ) <<  8 )   \
| ( (uint32_t) ( (uint8_t) *( (uint8_t *)(_ptr_) + (_ofs_) + 2) ) << 16 )   \
)
    
#define GET_4B_OFS(_ptr_,_ofs_) (                                           \
  ( (uint32_t) ( (uint8_t) *( (uint8_t *)(_ptr_) + (_ofs_) + 0) )       )   \
| ( (uint32_t) ( (uint8_t) *( (uint8_t *)(_ptr_) + (_ofs_) + 1) ) <<  8 )   \
| ( (uint32_t) ( (uint8_t) *( (uint8_t *)(_ptr_) + (_ofs_) + 2) ) << 16 )   \
| ( (uint32_t) ( (uint8_t) *( (uint8_t *)(_ptr_) + (_ofs_) + 3) ) << 24 )   \
)

#define SET_1B_OFS(_ptr_,_ofs_,_val_) (                                     \
*( (uint8_t *)(_ptr_) + (_ofs_) + 0 ) = ( (uint8_t)(_val_)        )         \
)

#define SET_2B_OFS(_ptr_,_ofs_,_val_) (                                     \
*( (uint8_t *)(_ptr_) + (_ofs_) + 0 ) = ( (uint8_t)((_val_)     ) ),        \
*( (uint8_t *)(_ptr_) + (_ofs_) + 1 ) = ( (uint8_t)((_val_) >> 8) )         \
)

#define SET_3B_OFS(_ptr_,_ofs_,_val_) (                                     \
*( (uint8_t *)(_ptr_) + (_ofs_) + 0 ) = ( (uint8_t)((_val_)      ) ),       \
*( (uint8_t *)(_ptr_) + (_ofs_) + 1 ) = ( (uint8_t)((_val_) >>  8) ),       \
*( (uint8_t *)(_ptr_) + (_ofs_) + 2 ) = ( (uint8_t)((_val_) >> 16) ),       \
)

#define SET_4B_OFS(_ptr_,_ofs_,_val_) (                                     \
*( (uint8_t *)(_ptr_) + (_ofs_) + 0 ) = ( (uint8_t)((_val_)      ) ),       \
*( (uint8_t *)(_ptr_) + (_ofs_) + 1 ) = ( (uint8_t)((_val_) >>  8) ),       \
*( (uint8_t *)(_ptr_) + (_ofs_) + 2 ) = ( (uint8_t)((_val_) >> 16) ),       \
*( (uint8_t *)(_ptr_) + (_ofs_) + 3 ) = ( (uint8_t)((_val_) >> 24) )        \
)



// C:\STU\developp\1oom\src\bits.h
// #if defined WORDS_BIGENDIAN && defined ALLOW_UNALIGNED_ACCESS
// #define SET_BE_16(_p_,_v_) ((*((uint16_t *)(_p_))) = (_v_))
// #define GET_BE_16(_p_) (*((uint16_t const *)(_p_)))
// #define SET_BE_32(_p_,_v_) ((*((uint32_t *)(_p_))) = (_v_))
// #define GET_BE_32(_p_) (*((uint16_t const *)(_p_)))
// #else
#define SET_BE_16(_p_,_v_) (((uint8_t *)(_p_))[1] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[0] = (((_v_) >> 8) & 0xffu))
#define GET_BE_16(_p_) (((uint16_t)(((uint8_t const *)(_p_))[1])) | (((uint16_t)(((uint8_t const *)(_p_))[0])) << 8))
#define SET_BE_32(_p_,_v_) (((uint8_t *)(_p_))[3] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[2] = (((_v_) >> 8) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 16) & 0xffu), ((uint8_t *)(_p_))[0] = (((_v_) >> 24) & 0xffu))
#define GET_BE_32(_p_) (((uint32_t)GET_BE_16((_p_) + 2)) | (((uint32_t)GET_BE_16((_p_))) << 16))
// #endif
#define SET_BE_24(_p_, _v_) (((uint8_t *)(_p_))[2] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 8) & 0xffu), ((uint8_t *)(_p_))[0] = (((_v_) >> 16) & 0xffu))
#define GET_BE_24(_p_) ((((uint32_t)GET_BE_16((_p_))) << 8) | ((uint32_t)*(((uint8_t const *)(_p_)) + 2)))



#endif /* MOX_BITS_H */
