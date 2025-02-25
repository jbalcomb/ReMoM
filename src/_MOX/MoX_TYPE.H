#ifndef MOX_TYPE_H
#define MOX_TYPE_H



typedef unsigned char * byte_ptr;
typedef unsigned char * SAMB_ptr;
typedef long long       SAMB_INT;  // TODO  finish this out - ~ pointer to pict or int for -1/INVALID, but 16-bit DOS 2-byte vs. 64-bit Win 8-byte (short vs. long long)

typedef unsigned char       byte;
typedef unsigned short int  word;
typedef unsigned int        dword;

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



#endif /* MOX_TYPE_H */
