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

#define PTR_INC_PARAGRAPH(_ptr_) ( (unsigned char *) (_ptr_) + 16 )  // unsigned char * offset = data + 512; // same as &data[512]
#define PTR_DEC_PARAGRAPH(_ptr_) ( (unsigned char *) (_ptr_) - 16 )
#define PTR_ADD_PARAGRAPH(_ptr_,_paras_) ( (unsigned char *) (_ptr_) + (16 * _paras_) )
#define PTR_SUB_PARAGRAPH(_ptr_,_paras_) ( (unsigned char *) (_ptr_) - (16 * _paras_) )


#define GET_2B(_ptr_) ( (unsigned int)                                                *((_ptr_) + 1) << 8 | *((_ptr_) + 0) )

#define GET_4B(_ptr_) ( (unsigned long) *((_ptr_) + 3) << 24 | *((_ptr_) + 2) << 16 | *((_ptr_) + 1) << 8 | *((_ptr_) + 0) )

#define SET_2B(_ptr_,_val_) (                                                                 *((_ptr_) + 1) = (_val_) >> 8; *((_ptr_) + 0) = (_val_); )

#define SET_4B(_ptr_,_val_) ( *((_ptr_) + 3) = (_val_) >> 24; *((_ptr_) + 2) = (_val_) >> 16; *((_ptr_) + 1) = (_val_) >> 8; *((_ptr_) + 0) = (_val_); )


// replaces farpeekb()
#define GET_1B_OFS(_ptr_,_ofs_) ( (unsigned char)                                                                                             *((_ptr_) + _ofs_ + 0) )

// replaces farpeekw()
#define GET_2B_OFS(_ptr_,_ofs_) ( (unsigned int)                                                                *((_ptr_) + _ofs_ + 1) << 8 | *((_ptr_) + _ofs_ + 0) )

// replaces farpeekdw()
#define GET_4B_OFS(_ptr_,_ofs_) ( (unsigned long) *((_ptr_) + _ofs_ + 3) << 24 | *((_ptr_) + _ofs_ + 2) << 16 | *((_ptr_) + _ofs_ + 1) << 8 | *((_ptr_) + _ofs_ + 0) )

// replaces farpokeb()
#define SET_1B_OFS(_ptr_,_ofs_,_val_) ( (unsigned char)                                                                                             *((_ptr_) + _ofs_ + 0) )

// replaces farpokew()
#define SET_2B_OFS(_ptr_,_ofs_,_val_) ( *((_ptr_) + (_ofs_) + 1) = (_val_) >> 8, *((_ptr_) + (_ofs_) + 0) = (_val_) )

// replaces farpokedw()
#define SET_4B_OFS(_ptr_,_ofs_,_val_) ( (unsigned long) *((_ptr_) + _ofs_ + 3) << 24 | *((_ptr_) + _ofs_ + 2) << 16 | *((_ptr_) + _ofs_ + 1) << 8 | *((_ptr_) + _ofs_ + 0) )

#endif /* MoX_DEF_H */
