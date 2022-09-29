#ifndef ST_DEF_H
#define ST_DEF_H


// /*
//     Hey, Visual Studio Code. Sshhh....
// */
// #define __VSCODE__ 1
// #ifdef __VSCODE__
// #define asm _asm
// unsigned int _AH;
// unsigned int _AL;
// unsigned int _AX;
// unsigned int _BH;
// unsigned int _BL;
// unsigned int _BX;
// unsigned int _CH;
// unsigned int _CL;
// unsigned int _CX;
// unsigned int _DH;
// unsigned int _DL;
// unsigned int _DX;
// unsigned int _SI;
// unsigned int _DI;
// unsigned int _DS;
// unsigned int _ES;
// #endif

static int delete = 0;

#define ST_TRUE  1
#define ST_FALSE 0
#define ST_UNDEFINED -1

#define ST_SUCCESS -1
#define ST_FAILURE  0


#if !defined( __DOS_H )
#include <DOS.H>
#endif

#undef MK_FP

#undef FP_SEG

#undef FP_OFF

#define MK_FP(_sgmt_,_ofst_) ( (void *) ( ((unsigned long) (_sgmt_) << 16) | (_ofst_) ) )

#define FP_SEG(_fp_) ( (unsigned int) ( (unsigned long)((void *)(_fp_)) >> 16) )

#define FP_OFF(_fp_) ( (unsigned int) (_fp_) )



// // /j/STU/DBWD/developc/flilib/flisrc ... aatypes.h
// #ifndef NULL
// #define NULL ((void *)0L)
// #endif /* NULL */
#ifndef NULL
#define NULL 0
#endif /* NULL */


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


/*
    seg022
*/

// _s22p07
void RNG_TimerSeed(void);

// _s22p22
//int MEM_Copy_Far(char _FAR *fptr_Dst, char _FAR *fptr_Src, int nbytes);
int ST_MoveData(unsigned int destoff, unsigned int destseg, unsigned int srcoff, unsigned int srcseg, unsigned int nbytes);
// int ST_MoveData_SA(unsigned int dst_ofst, unsigned int dst_sgmt, unsigned int src_ofst, unsigned int src_sgmt, unsigned int nbytes);
// int ST_MoveData_FP(void _FAR * dst, void _FAR * src, size_t nbytes);


#ifdef __cplusplus
}
#endif

#endif /* ST_DEF_H */
