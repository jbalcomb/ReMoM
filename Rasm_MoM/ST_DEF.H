#ifndef ST_DEF_H
#define ST_DEF_H


#ifdef FAR
#define __FAR far
#else
#define __FAR
#endif


#define	DOS_INT			        0x21
#define PRINT_CHAR              0x02        // DL = character to output
#define PRINT_STRING            0x09        // DS:DX = pointer to string ending in "$"
#define	ALLOCMEM                0x48        // CF = {success, failure}; AX = segment address or error code; BX = size in paragraphs (16 B) of the largest block of memory available


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


#ifdef __cplusplus
}
#endif

#endif /* ST_DEF_H */
