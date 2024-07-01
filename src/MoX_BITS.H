#ifndef MOX_BITS_H
#define MOX_BITS_H

#include "MoX_TYPE.H"



#define GET_1B_OFS(_ptr_,_ofs_) (                              \
  ( (uint8_t)  ( (uint8_t) *( (_ptr_) + (_ofs_) + 0) )       ) \
)

#define GET_2B_OFS(_ptr_,_ofs_) (                              \
  ( (uint16_t) ( (uint8_t) *( (_ptr_) + (_ofs_) + 0) )       ) \
| ( (uint16_t) ( (uint8_t) *( (_ptr_) + (_ofs_) + 1) ) <<  8 ) \
)

#define GET_4B_OFS(_ptr_,_ofs_) (                              \
  ( (uint32_t) ( (uint8_t) *( (_ptr_) + (_ofs_) + 0) )       ) \
| ( (uint32_t) ( (uint8_t) *( (_ptr_) + (_ofs_) + 1) ) <<  8 ) \
| ( (uint32_t) ( (uint8_t) *( (_ptr_) + (_ofs_) + 2) ) << 16 ) \
| ( (uint32_t) ( (uint8_t) *( (_ptr_) + (_ofs_) + 3) ) << 24 ) \
)

#define SET_1B_OFS(_ptr_,_ofs_,_val_) (        \
*( (_ptr_) + (_ofs_) + 0 ) = ( (_val_)      )  \
)

#define SET_2B_OFS(_ptr_,_ofs_,_val_) (        \
*( (_ptr_) + (_ofs_) + 0 ) = ( (_val_)      ), \
*( (_ptr_) + (_ofs_) + 1 ) = ( (_val_) >> 8 )  \
)

#define SET_4B_OFS(_ptr_,_ofs_,_val_) (        \
*( (_ptr_) + (_ofs_) + 0 ) = ( (_val_)      ), \
*( (_ptr_) + (_ofs_) + 1 ) = ( (_val_) >>  8), \
*( (_ptr_) + (_ofs_) + 2 ) = ( (_val_) >> 16), \
*( (_ptr_) + (_ofs_) + 3 ) = ( (_val_) >> 24)  \
)


#endif /* MOX_BITS_H */
