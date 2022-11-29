#ifndef MOX_TYPE_H
#define MOX_TYPE_H


typedef unsigned char * MoX_byte_ptr;
typedef unsigned char * MoX_SAMB_ptr;

typedef unsigned char      byte;
typedef unsigned int       word;
typedef unsigned long int  dword;
typedef unsigned int sgmt_addr;
typedef unsigned int SAMB_addr;
typedef unsigned char * byte_ptr;
typedef unsigned char * SAMB_ptr;

#if defined(__DOS16__)
typedef unsigned int MoX_sgmt_addr;
typedef unsigned int MoX_ofst_addr;
typedef unsigned int MoX_SAMB_addr;
#endif

#if defined(__DOS16__)
typedef unsigned char      MoX_byte;
typedef unsigned int       MoX_word;
typedef unsigned long int  MoX_dword;
typedef signed char        S8;
typedef signed int         S16;
typedef signed long int    S32;
typedef unsigned char      U8;
typedef unsigned int       U16;
typedef unsigned long int  U32;
typedef signed char        int8_t;
typedef signed int         int16_t;
typedef signed long int    int32_t;
typedef unsigned char      uint8_t;
typedef unsigned int       uint16_t;
typedef unsigned long int  uint32_t;
#endif

#if defined(__WIN32__)
typedef unsigned char       MoX_byte;
typedef unsigned short int  MoX_word;
typedef unsigned int        MoX_dword;
typedef signed char         S8;
typedef signed short int    S16;
typedef signed int          S32;
typedef unsigned char       U8;
typedef unsigned short int  U16;
typedef unsigned int        U32;
// #include <stdint.h>
#define _STDINT
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
#endif


#endif /* MOX_TYPE_H */
