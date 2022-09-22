#ifndef ST_DEF_H
#define ST_DEF_H


#ifndef DEBUG
#define HERE
#endif


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


#ifdef __cplusplus
}
#endif

#endif /* ST_DEF_H */
