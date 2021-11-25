/*
    BCpp31PG, pg 175/190
        #pragma warn -yyy
    BCpp31UG, pg 162/171
    BCpp30UG, pg 158/167
        -waus*  Identifier is assigned a value that is never used.
        -wccc*  Condition is always true/false.
        -weff*  Code has no effect.
        -wpar*  Parameter parameter is never used.
        -wuse   Identifier is declared but never used.
        -wrpt*  Nonportable pointer conversion.
*/
#pragma warn -aus
#pragma warn -par

//#include <stdlib.h>
#include <ALLOC.H>  /* coreleft(), farcoreleft(), malloc(), farmalloc(), free(), farfree() */
#include <DOS.H>

#include "MGC_HEAD.H"
#include "MOM_HEAD.H"
#include "ST_HEAD.H"
#include "ST_LBX.H"

#include "TST_MGC.H"


#undef __STDC__
#undef __OS2__
#undef _Windows
#undef __TINY__
#undef __SMALL__
#undef __MEDIUM__
#undef __COMPACT__
//#define __LARGE__
#undef __HUGE__

typedef unsigned char  byte;
typedef unsigned short word;  // root42 has unsigned int
typedef unsigned long  dword;

//typedef _seg * memptr;  // Wolf3d ID_MM.H


#define FONT_FILE_NAME "FONTS.LBX"
char *g_FontFileName = "FONTS.LBX";

// typedef struct SAMB_struct SAMB;
// typedef far * SAMB fp_SAMB;
// s_SAMB far * gfp_SAMB;

// extern byte far gamepal;
// extern	pictabletype	_seg *pictable;
// extern	pictabletype	_seg *picmtable;
// extern	spritetabletype _seg *spritetable;


void test_set_mem_min_ram(void);
void test_set_mem_min_emm(void);
/*
    seg009
*/
void test_lbx_open(void);
void test_lbx_close(void);
void test_lbx_seek(void);
void test_lbx_size(void);
void test_lbx_read_sgmt(void);
void test_lbx_read_ofst(void);
void test_strcpyfar(void);

void test_coreleft(void);
void test_farcoreleft(void);
void test_dos_allocmem(void);
void test_dos_freemem(void);
//void test_allocmem(void);
//void test_freemem(void);
void test_malloc(void);
void test_farmalloc(void);

void test_pointers(void);
unsigned int __FAR * make_ui_fptr(unsigned long ul_fptr_address);
char __FAR * SgmtAddr_FarPtr1(unsigned int segment_address);
char __FAR * SgmtAddr_FarPtr2(unsigned int segment_address);
char __FAR * SgmtAddr_FarPtr3(unsigned int segment_address);
void __FAR * MK_FP0(unsigned int sgmt_addr);

void test_LbxLdEt1(void);

void test_asciz_character_array(void);

void test_SA_Allocate_Space(void);
void test_SA_Allocate_MemBlk(void);

void test_Hardware_Init(void);
void test_VGA_DAC_Init(void);

/*
    J:\STU\DBWD\developc\Animator\Vpaint\COMMON\ptr.h
    norm_pointer() / enorm_pointer() / pj_enorm_pointer()
    J:\STU\DBWD\developc\Animator\Vpaint\PJ\inc\compiler.h
    J:\STU\DBWD\developc\Animator\Vpaint\PJ\util\ptrenorm.c
    J:\STU\DBWD\developc\Animator\Vpaint\V\peekpoke.asm

*/
typedef long intptr_t;
//void *make_ptr(void *pt);
////void *enorm_pointer(void *p);
////void *norm_pointer(void *p);
//unsigned int ptr_offset(int offset, int seg);
//unsigned int ptr_seg(int offset, int seg);
//void *ptr_next_seg(void *p);
//long pt_to_long(unsigned offset, unsigned seg);
//void *long_to_pt(unsigned long l);
void *make_ptr();
void *norm_pointer(void *p);
unsigned int ptr_offset();
unsigned int ptr_seg();
void *ptr_next_seg(void *p);
long pt_to_long();
void *long_to_pt();


int main(void)
{

    // test_lbx_open();
    // test_lbx_close();
    // test_lbx_seek();
    // test_lbx_size();

//void test_lbx_read_sgmt(void);
//void test_lbx_read_ofst(void);
//void test_strcpyfar(void);

    // test_coreleft();
    // test_farcoreleft();
    // test_dos_allocmem();
    // test_dos_freemem();
    // test_malloc();
    // test_farmalloc();

    //test_pointers();

    //test_LbxLdEt1();

    //test_asciz_character_array();

    // test_SA_Allocate_Space();
    // test_SA_Allocate_MemBlk();

    test_Hardware_Init();
    //test_VGA_DAC_Init();

    return 0;
}

void test_set_mem_min_ram(void)
{

}

void test_set_mem_min_emm(void)
{

}
// _s09p01
void test_lbx_open(void)
{
    int fhandle;

    fhandle = 0;

    fhandle = lbx_open(g_FontFileName);

    if ( fhandle == 0 )
    {
        printf("DEBUG: FAILURE: lbx_open() fhandle == %d\n", fhandle);
    }
    else
    {
        printf("DEBUG: SUCCESS: lbx_open() fhandle == %d\n", fhandle);
    }
}
// _s09p01
void test_lbx_close(void)
{
    int fhandle;
    int result;
    char result_hi;

    fhandle = 0;
    result = 0;
    result_hi = 0;

    fhandle = lbx_open(g_FontFileName);

    result = lbx_close(fhandle);

    result_hi = (char)(result >> 8);

    if ( result_hi == 0x3E )
    {
        printf("DEBUG: SUCCESS: lbx_close() result_hi == 0x%02X\n", result_hi);
    }
    else
    {
        printf("DEBUG: FAILURE: lbx_close() result_hi == 0x%02X\n", result_hi);
    }
}

void test_lbx_seek(void)
{
    int fhandle;
    int result;
    char result_hi;
    char result_lo;
    char *ptr; 

    fhandle = 0;
    result = 0;

    fhandle = lbx_open(g_FontFileName);

    result = lbx_seek(512, fhandle);

    if ( result == -1 )
    {
        printf("DEBUG: SUCCESS: lbx_seek() result == %d\n", result);
    }
    else
    {
        printf("DEBUG: FAILURE: lbx_seek() result == %d\n", result);
    }

    lbx_close(fhandle);
}

void test_lbx_size(void)
{
    int fhandle;
    long filelength;

    fhandle = 0;
    filelength = 0;

    fhandle = lbx_open(g_FontFileName);

    filelength = lbx_size(fhandle);

    if ( filelength == 0 )
    {
        printf("DEBUG: FAILURE: lbx_size() filelength == %ld\n", filelength);
    }
    else
    {
        printf("DEBUG: SUCCESS: lbx_size() filelength == %ld\n", filelength);
    }

    lbx_close(fhandle);
}

void test_lbx_read_sgmt(void)
{
int fhandle;
    long filelength;

    fhandle = 0;
    filelength = 0;

    fhandle = lbx_open(g_FontFileName);

    //filelength = lbx_read_sgmt(fhandle);

    if ( filelength == 0 )
    {
        printf("DEBUG: FAILURE: lbx_size() filelength == %ld\n", filelength);
    }
    else
    {
        printf("DEBUG: SUCCESS: lbx_size() filelength == %ld\n", filelength);
    }

    lbx_close(fhandle);
}

void test_lbx_ofst(void)
{

}

void test_strcpyfar(void)
{

}

/*
    BCpp31LR, pg 89/102
        #include <alloc.h>
        unsigned long coreleft(void);
            printf("The difference between the highest allocated block and\n");
            printf("the top of the heap is: %lu bytes\n", (unsigned long) coreleft());
*/
void test_coreleft(void)
{
    unsigned long heap_size;

    heap_size = coreleft();

    printf("DEBUG: test_coreleft() heap_size == %lu\n", heap_size);
    printf("DEBUG: test_coreleft() heap_size == 0x%08X\n", heap_size);

}
/*
    BCpp31LR, pg 147/160
        #include <alloc.h>
        unsigned long farcoreleft(void);
            printf("The difference between the highest allocated block in the far\n");
            printf("heap and the top of the far heap is: %lu bytes\n", farcoreleft());
*/
void test_farcoreleft(void)
{
    unsigned long farheap_size;

    farheap_size = farcoreleft();
    
    printf("DEBUG: test_farcoreleft() farheap_size == %lu\n", farheap_size);
    printf("DEBUG: test_farcoreleft() farheap_size == 0x%08X\n", farheap_size);
}
/*
    J:\STU\DBWD\BORLANDC\CRTL\CLIB\DOSALLOC.C
    BCpp31LR, pg 18/29
        #include <dos.h>
        unsigned _dos_allocmem(unsigned size, unsigned *segp);
            unsigned int segp;
            unsigned int size = 64; / * (64*16) = 1024 bytes * /
            if (_dos_allocmem(size, &segp) == 0)
                printf("Allocated memory at segment: %x\n", segp);
            else {
                perror("Unable to allocate block.");
                printf("Maximum no. of paragraphs"
                       " available is %u\n", segp);
                return 1;
            }
*/
void test_dos_allocmem(void)
{
    unsigned int size;
    unsigned int segp;
    unsigned int result;

    size = 64; /* (64 x 16) = 1024 bytes */

    result = _dos_allocmem(size, &segp);

    if ( result == 0 )
    {
        printf("DEBUG: SUCCESS: _dos_allocmem() result == %d\n", result);
        printf("DEBUG: _dos_allocmem() segp == 0x%04X\n", segp);
    }
    else
    {
        printf("DEBUG: FAILURE: _dos_allocmem() result == %d\n", result);
        printf("DEBUG: _dos_allocmem() segp == 0x%04X\n", segp);
    }
}
/*
    J:\STU\DBWD\BORLANDC\CRTL\CLIB\DOSFREE.C
    BCpp31LR, pg 191/206
        #include <dos.h>
        unsigned _dos_freemem(unsigned segx);
        unsigned int size, segp, err;
        size = 64; / * (64 x 16) = 1024 bytes * /
        err = _dos_allocmem(size, &segp);
        if (err == 0)
            printf ("Allocated memory at segment: %x\n", segp);
        else {
            perror("Unable to allocate block");
            printf("Maximum no. of paragraphs available is %u\n", segp);
            return 1;
        }
        _dos_freemem(segp);
*/
void test_dos_freemem(void)
{
    unsigned int size;
    unsigned int segp;
    unsigned int result;

    size = 64; /* (64 x 16) = 1024 bytes */

    _dos_allocmem(size, &segp);

    printf("DEBUG: _dos_freemem() segp == 0x%04X\n", segp);

    result = _dos_freemem(segp);

    if ( result == 0 )
    {
        printf("DEBUG: SUCCESS: _dos_freemem() result == %d\n", result);
        printf("DEBUG: _dos_freemem() segp == 0x%04X\n", segp);
    }
    else
    {
        printf("DEBUG: FAILURE: _dos_freemem() result == %d\n", result);
        printf("DEBUG: _dos_freemem() segp == 0x%04X\n", segp);
    }
}
/*
    BCpp31LR, pg 351/366
        #include <stdlib.h> or #include<alloc.h>
        void *malloc(size_t size);
            char *str;
            / * allocate memory for string * /
            if ((str = (char *) malloc(10)) == NULL) {
                printf("Not enough memory to allocate buffer\n");
                exit(1); / * terminate program if out of memory * /
            }
            / * free memory * /
            free(str);
...
...heap...dynamic allocation...variable-sized block of memory...
*/
void test_malloc(void)
{
    size_t size;
    char *MemBlk;

    //size = 32768;  // 32768 bytes, 32 KB, 2048 paragraphs, 0.5 pages
    //size = 1024;
    size = 1;

    MemBlk = (char *) malloc(size);

    printf("DEBUG: test_malloc() MemBlk == 0x%08X\n", MemBlk);

    if ( MemBlk == NULL )
    {
        printf("DEBUG: FAILURE: test_malloc() MemBlk == %lu\n", MemBlk);
        printf("DEBUG: test_malloc() MemBlk == 0x%08X\n", MemBlk);
    }
    else
    {
        printf("DEBUG: SUCCESS: test_malloc() MemBlk == %p\n", MemBlk);
        printf("DEBUG: test_malloc() MemBlk == 0x%08X\n", MemBlk);
    }

    free(MemBlk);
}
/*
    BCpp31LR, pg 155/168
        #include <alloc.h>
        void far *farmalloc(unsigned long nbytes);
            char far *fptr;
            / * allocate memory for the far pointer * /
            fptr = (char far *) farmalloc(10);
            movedata (FP_SEG(str), FP_OFF(str), FP_SEG(fptr), FP_OFF(fptr), strlen(str));
*/
void test_farmalloc(void)
{
    unsigned long nbytes;
    char __FAR *MemBlk;

    //nbytes = 32768;  // 32768 bytes, 32 KB, 2048 paragraphs, 0.5 pages
    //nbytes = 1024;
    //nbytes = 1;
    nbytes = ( ( 32 * 16 ) + 16 );
    //nbytes = 16;
    
    MemBlk = (char __FAR *) farmalloc(nbytes);
    
    printf("DEBUG: MemBlk = (char __FAR *) farmalloc(%lu); [%s, %d]\n", nbytes, __FILE__, __LINE__);
    
    if ( MemBlk == NULL )
    {
        printf("DEBUG: FAILURE: test_farmalloc(%lu) MemBlk == %Fp\n", nbytes, MemBlk);
    }
    else
    {
        printf("DEBUG: SUCCESS: test_farmalloc() MemBlk == %Fp\n", MemBlk);
    }

    farfree(MemBlk);
}
/*
    FP_OFF, FP_SEG
    BCpp31LR, pg 184/

    MK_FP
    BCpp31LR, pg 364/379
        #include <dos.h>
        void far * MK_FP(unsigned seg, unsigned ofs);
            unsigned int far *screen;
            screen = MK_FP(0xB800, 0);
*/
void test_pointers(void)
{
    unsigned int nparas;
    unsigned int sgmt_addr;
    unsigned int result;
    char __FAR * FarPtr1;
    char __FAR * FarPtr2;
    char __FAR * FarPtr3;
    char __FAR * FarPtr4;

    /*
        segment size: 64K, 1024*64, 65536, 0x10000
        MAX WORD (2 bytes) segment size 65535, 0xFFFF, 4095 paragraphs
        ~= AX:0000 - AX:FFFF
        DOS Allocate Memory
            AX = segment address of allocated memory block (MCB + 1 paragraph (16 bytes))
            - each allocation requires a 16 byte overhead for the MCB
                MCB - DOS Memory Control Block Format
                ...MCB chain
        
    */
    nparas = 64; /* (64 x 16) = 1024 bytes */
    nparas = 4095; // 65535 bytes == MAX UNSIGNED INTEGER

    result = _dos_allocmem(nparas, &sgmt_addr);

    printf("DOS Maximum Memory Available: %u Paragraphs, %u Bytes\n", sgmt_addr, (sgmt_addr * 16));

    nparas = sgmt_addr;

    result = _dos_allocmem(nparas, &sgmt_addr);

    if ( result == 0 )
    {
        printf("DEBUG: SUCCESS: _dos_allocmem() result == %d\n", result);
        printf("DEBUG: _dos_allocmem() sgmt_addr == 0x%04X\n", sgmt_addr);
    }
    else
    {
        printf("DEBUG: FAILURE: _dos_allocmem() result == %d\n", result);
        printf("DEBUG: _dos_allocmem() sgmt_addr == 0x%04X\n", sgmt_addr);
    }

    FarPtr1 = SgmtAddr_FarPtr1(sgmt_addr);
    FarPtr2 = SgmtAddr_FarPtr2(sgmt_addr);
    FarPtr3 = SgmtAddr_FarPtr3(sgmt_addr);
    FarPtr4 = MK_FP0(sgmt_addr);

}
/*
    void far * fp => unsigned long fp_addr
    unsigned long fp_addr => void far * fp
    void far * fp => unsigned long fp_addr_sgmt
    void far * fp => unsigned long fp_addr_ofst

    MK_FP(segment_address,0)

*/
char __FAR * SgmtAddr_FarPtr1(unsigned int segment_address)
{
    char __FAR * FarPtr;
    FarPtr = (char __FAR *) ( (long) segment_address << 16 );
    return FarPtr;
}
char __FAR * SgmtAddr_FarPtr2(unsigned int segment_address)
{
    char __FAR * FarPtr;
    long farptr_sgmt;

    farptr_sgmt = ( (long) segment_address << 16 );

    FarPtr = (char __FAR *) (farptr_sgmt | 0);

    return FarPtr;
}
char __FAR * SgmtAddr_FarPtr3(unsigned int segment_address)
{
    char __FAR * FarPtr;
    long farptr_sgmt;
    unsigned int temp;
    
    temp = segment_address;

    FarPtr = (char __FAR *) ( (long) temp << 16 );

    return FarPtr;
}
unsigned int __FAR * make_ui_fptr(unsigned long ul_fptr_address)
{
    unsigned int __FAR *ui_fptr;

    ui_fptr = (unsigned int __FAR *) ul_fptr_address;

    return ui_fptr;
}
void __FAR * MK_FP0(unsigned int sgmt_addr)
{
    return MK_FP(sgmt_addr,0);
}

/*
        unsigned FP OFF(void far *p);
        unsigned FP SEG(void far *p);
        void far * MK_FP<unsigned seg, unsigned ofs);
    J:\STU\DBWD\BORLANDC\INCLUDE\DOS.H
    Line 474
    #define MK_FP( seg,ofs )( (void _seg * )( seg ) +( void near * )( ofs ))
    #define FP_SEG( fp )( (unsigned )( void _seg * )( void far * )( fp ))
    #define FP_OFF( fp )( (unsigned )( fp ))

    int  inline _Cdecl peek( unsigned __segment, unsigned __offset )  
                            { return( *( (int  far* )MK_FP( __segment, __offset )) ); }

    #define peek( a,b )( *( (int  far* )MK_FP( (a ),( b )) ))

    void inline _Cdecl poke( unsigned __segment, unsigned __offset, int __value )  
                            {( *( (int  far* )MK_FP( __segment, __offset )) = __value ); }

    #define poke( a,b,c )( *( (int  far* )MK_FP( (a ),( b )) ) =( int )( c ))

*/
/*
    J:\STU\DBWD\developc\Animator\Vpaint\COMMON\ptr.c
*/
/* fool C into thinking a long is a pointer */
void * make_ptr(void *pt)
{
	return pt;
}

#if !defined(__TURBOC__)
void * enorm_pointer(void *p)
{
	intptr_t x = (intptr_t)p;
	return (void *) ((x + 1) & ~0x1);
}
#endif /* __TURBOC__ */

unsigned int ptr_offset(int offset, int seg)
{
	return offset;
}

unsigned int ptr_seg(int offset, int seg)
{
	return seg;
}

// Error: Too few parameters in call to 'ptr_seg' in function ptr_next_seg
// Error: Extra parameter in call to make_ptr in function ptr_next_seg
//void * ptr_next_seg(void *p)
//{
//	return make_ptr(0, ptr_seg(p) + 1);
//}

long pt_to_long(unsigned offset, unsigned seg)
{
	long result;

	result = seg;
	result <<= 4;
	result += offset;
	return result;
}

// Error: Type mismatch in parameter 'pt' in call to 'make_ptr' in function long_to_pt
// Error: Extra parameter in call to make_ptr in function long_to_pt
//void * long_to_pt(unsigned long l)
//{
//	unsigned segment, offset;
//
//	offset = (l&15);
//	l >>= 4;
//	segment = l;
//	return make_ptr(offset, segment);
//}
/*
    https://github.com/yzh/yzhack/blob/master/japanese/emalloc.c
*/
/* 正規化されたポインタを作成する */ // JP->EN /* Create a normalized pointer */
static 	void * mk_fp ( unsigned  int seg, unsigned  int off)
{
	unsigned long temp;

	temp = (((unsigned long)seg) << 4) + ((unsigned long)off);
	return (void *)(((temp & 0xffff0L) << 12) + (temp & 0x0000fL));
}
// _s08p16
void __FAR * MK_FAR_0(unsigned int Seg_Address)
{
    //unsigned int Return_Segment;
    //unsigned int Return_Offset;
    //unsigned int Segment_Pointer;
    //Segment_Pointer = Seg_Address;
    //Return_Segment = Segment_Pointer;
    //Return_Offset = 0;
    ///*
    //    _DX = Return_Segment
    //    _AX = Return_Offset
    //    return DX:AX
    //*/
    unsigned int __FAR * segp;
    segp = MK_FP(Seg_Address, 0);
    return segp;
}

void test_LbxLdEt1(void)
{
    unsigned int LBX_EntryData_SgmtAddr;
    //LbxLdEt1(FONT_FILE_NAME);
    //LbxLdEt1(g_FontFileName);
    LBX_EntryData_SgmtAddr = LbxLdEt1("BACKGRND.LBX", 0);

    printf("DEBUG: LBX_EntryData_SgmtAddr == 0x%08X\n", LBX_EntryData_SgmtAddr);

    

}

void test_asciz_character_array(void)
{
    char chracter_array[20];

    strcpy(chracter_array, "BACKGRND");
    strcat(chracter_array, ".LBX");
    
}

void test_SA_Allocate_Space(void)
{
    unsigned int sgmt_addr;

    sgmt_addr = SA_Allocate_Space(32);

    printf("DEBUG: test_SA_Allocate_Space(): sgmt_addr == 0x%08X\n", sgmt_addr);

}
void test_SA_Allocate_MemBlk(void)
{
    unsigned int sgmt_addr;

    sgmt_addr = SA_Allocate_MemBlk(32);

    printf("DEBUG: test_SA_Allocate_MemBlk(): sgmt_addr == 0x%08X\n", sgmt_addr);

}

void test_Hardware_Init(void)
{
    Hardware_Init(1, 2, GAME_FONT_FILE, 0, 0, 0, 0, 0, 0, 0, 0);  // Defaults for 'No Sound'
}

void test_VGA_DAC_Init(void)
{
    VGA_DAC_Init(GAME_FONT_FILE);

}
