#ifndef MOX_BITS_H
#define MOX_BITS_H

#include "MoX_TYPE.H"


#define MoX_GET_1B_OFS(_ptr_,_ofs_) ( (MoX_byte)                                                                                              *((_ptr_) + _ofs_ + 0) )
#define MoX_GET_2B_OFS(_ptr_,_ofs_) ( (MoX_word)                                                                *((_ptr_) + _ofs_ + 1) << 8 | *((_ptr_) + _ofs_ + 0) )
#define MoX_GET_4B_OFS(_ptr_,_ofs_) ( (MoX_dword) *((_ptr_) + _ofs_ + 3) << 24 | *((_ptr_) + _ofs_ + 2) << 16 | *((_ptr_) + _ofs_ + 1) << 8 | *((_ptr_) + _ofs_ + 0) )
#define MoX_SET_1B_OFS(_ptr_,_ofs_,_val_) (                                                                                                                                     *((_ptr_) + (_ofs_) + 0) = (_val_) )
#define MoX_SET_2B_OFS(_ptr_,_ofs_,_val_) (                                                                                          *((_ptr_) + (_ofs_) + 1) = ((_val_) >> 8), *((_ptr_) + (_ofs_) + 0) = (_val_) )
#define MoX_SET_4B_OFS(_ptr_,_ofs_,_val_) ( *((_ptr_) + (_ofs_) + 3) = ((_val_) >> 24), *((_ptr_) + (_ofs_) + 2) = ((_val_) >> 16),  *((_ptr_) + (_ofs_) + 1) = ((_val_) >> 8), *((_ptr_) + (_ofs_) + 0) = (_val_) )


#endif /* MOX_BITS_H */
