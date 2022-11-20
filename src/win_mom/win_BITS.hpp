#ifndef WIN_BITS_HPP
#define WIN_BITS_HPP


#define GET_1B_OFS(_ptr_,_ofs_) ( (unsigned char)                                                                                                   *((_ptr_) + (_ofs_) + 0) )
#define GET_2B_OFS(_ptr_,_ofs_) ( (unsigned short)                                                                  *((_ptr_) + (_ofs_) + 1) << 8 | *((_ptr_) + (_ofs_) + 0) )
#define GET_4B_OFS(_ptr_,_ofs_) ( (unsigned int)  *((_ptr_) + (_ofs_) + 3) << 24 | *((_ptr_) + (_ofs_) + 2) << 16 | *((_ptr_) + (_ofs_) + 1) << 8 | *((_ptr_) + (_ofs_) + 0) )
#define SET_1B_OFS(_ptr_,_ofs_,_val_) (                                                                                                                              *((_ptr_) + (_ofs_) + 0) = (_val_) )
#define SET_2B_OFS(_ptr_,_ofs_,_val_) (                                                                                     *((_ptr_) + (_ofs_) + 1) = (_val_) >> 8, *((_ptr_) + (_ofs_) + 0) = (_val_) )
#define SET_4B_OFS(_ptr_,_ofs_,_val_) ( *((_ptr_) + (_ofs_) + 3) = (_val_) >> 24, *((_ptr_) + (_ofs_) + 2) = (_val_) >> 16, *((_ptr_) + (_ofs_) + 1) = (_val_) >> 8, *((_ptr_) + (_ofs_) + 0) = (_val_) )


#endif /* WIN_BITS_HPP */
