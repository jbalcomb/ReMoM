#include <DOS.H>


/*

LBX
    Entry
    Records

LBX Type
    0 "pictures" / "vga animation frames"

LBX Entry Size
    Entry Offset Table = 8 + (4 * Entry Index)
    Entry Offset Begin = 8 + (4 * Entry Index) + 0
    Entry Offset End   = 8 + (4 * Entry Index) + 4
    Entry Size = Entry Offset End - Entry Offset Begin

Read Chunk Size
    32K
    16K
    ...
    Bytes_To_Copy = 32768
    cmp 
*/

/*

Load When?
Load What?
Load Where?

...need a font...
LBX File Name:      FONTS.LBX
LBX Entry Index:    0

...need a palette...
LBX File Name:      FONTS.LBX
LBX Entry Index:    2

To be more efficient, is the currenly requested resource in the same file as the previously requested resource?

To be even more efficient, is it already loaded into memory
How would I know?

What am I doing? Reading data (bytes) from a file into memory.
Where in the file should I start reading? How many bytes should I read?
Hrmm...
How about using an array/table that you can index with the Entry Index and be provide a starting point and length/size/count...
...or a starting point and an ending and then the count can be calculated?


Copy Data:
    From File, To Memory
    From Memory, To Memory
    From Memory, To File
    ~= Read, Copy, Write

    DOS API - Interrupt 21h
        Function 3Fh
            Read From File or Device Using Handle
                BX = File Handle; CX = Number of Bytes; DS:DX = pointer to buffer;
                AX = number of bytes read
        Function 40h
            Write To File or Device Using Handle
                BX = File Handle; CX = Number of Bytes; DS:DX = pointer to buffer;

    Microsoft C, Borland Turbo C, Borland C++
        fread()
        fwrite()

    Asm, Inline Asm, C, or C++

*/

/*
    Advanced MS-DOS Programming
        The Microsoft(R) Guide for Assembly Language and C Programmers
            By Ray Duncan
            1st 1986, 2nd 1988
    Chapter 11  Memory Management
    Line 8756

    0000:0000        0
    0009:FFFF   655359  ( 1024 * 640 = 655360)
    ...this 640 KB area of RAM is sometimes referred to as conventional memory

    The transient-program area (TPA), sometimes called the memory arena, is the remainder of memory above the operating-system area.
    The memory arena is dynamically allocated in blocks called arena entries.
    Each arena entry has a special control structure called an arena header, and all of the arena headers are chained together.

    Memory-Allocation Functions

    .COM (memory-image) files
    Because .COM programs contain no file header that can pass segment and memory-use information to MS-DOS,
    MS-DOS simply assumes the worst case and gives such a program everything.
    MS-DOS will load the program as long as there is an available memory block as large as the size of the file
      plus 256 bytes for the PSP
      and 2 bytes for the stack.

    the linker sets two fields in a .EXE file's header to inform MS-DOS about the program's memory requirements
        MIN_ALLOC, defines the minimum number of paragraphs required by the program, in addition to those for the code, data, and stack segments
        MAX_ALLOC, defines the maximum number of paragraphs of additional memory the program would use if they were available
    
    ...program needs additional memory space for an I/O buffer or an array...
    ...call Int 21H Function 48H (Allocate Memory Block) with the desired number of paragraphs...
    ...block of unallocated memory...
    ...MS-DOS returns the segment address of the base of the assigned area

    When the MS-DOS memory manager is searching the chain of arena headers to
    satisfy a memory-allocation request, it can use one of the following
    strategies:
    þ  First fit: Use the arena entry at the lowest address that is large
        enough to satisfy the request.
    þ  Best fit: Use the smallest arena entry that will satisfy the request,
        regardless of its location.
    þ  Last fit: Use the arena entry at the highest address that is large
        enough to satisfy the request.
    The default MS-DOS allocation strategy is first fit.

*/

/*
    LBX Entry Size
        (LBX Entry Offset End - LBX Entry Offset Begin)
    
    LBX Entry Offset Begin
        LBX Entry Offset Table[ ( 8 + ( LBX Entry Index * 4 ) ) ]
    LBX Entry Offset End
        LBX Entry Offset Table[ ( 8 + ( LBX Entry Index * 4 ) + 4 ) ]
    
    unsigned long * ptrLbxOfstTbl;
    ptrLbxOfstTbl = ptrLbxBuf + ( 8 + ( LBX Entry Index * 4 ) )
    LbxEntOfstBgn = *ptrLbxOfstTbl;
    ptrLbxOfstTbl += 4;
    LbxEntOfstEnd = *ptrLbxOfstTbl;

*/

#define LD_BTN_GAME() LBX_Load("MAINSCRN.LBX", 13, 0, 0, 0)

char * gFontEntryData;
char * gPaletteEntryData;

void lbx_open(char *fname, int *fhandle);

void LBX_Load(char *name, int idx, int arg1, int arg2, int arg3)
{

}

/*
    DOS API - Interrupt 21h
        Function 3Dh
            Open File Using Handle
                AL = access mode; DS:DX = pointer to ASCIIZ file name;
                AX = file handle

*/
void lbx_open(char *fname, int *fhandle)
{
    _AL = 0x00;
    _DX = fname;
    _AH = 0x3D;
    geninterrupt(0x21);
    fhandle = _AX;
}
/*
    DOS API - Interrupt 21h
        Function 3Eh
            Close File Using Handle
                BX = file handle

*/
void lbx_close(int *fhandle)
{
    _BX = fhandle;
    _AH = 0x3E;
    geninterrupt(0x21);
}
/*
    DOS API - Interrupt 21h
        Function 3Fh
            Read From File or Device Using Handle
                BX = File Handle; CX = Number of Bytes; DS:DX = pointer to buffer;
                AX = number of bytes read
dosadvnc.txt
    Records of any length, up to an entire segment (65,535 bytes), can be read to any memory address in one operation.
*/
void lbx_read(void *buffer, int nbytes, int fhandle)
{
    _BX = fhandle;
    _CX = nbytes;
    _DX = buffer;
    _AH = 0x3F;
    geninterrupt(0x21);
}

/*
    BCpp31LR, pg 100/123
    div()   Divides two integers, returning quotient and remainder.
        #inc1ude <stdlib.h>
        div_t div(int numer, int denom);
    
    BCpp31LR, pg 329/344
    ldiv()  Divides two longs, returning quotient and remainder.
        #include <stdlib.h>
        ldiv_t ldiv(long int numer, long int denom);
    ldiv divides two longs and returns both the quotient and the remainder as an ldiv_t type.
    numer and denom are the numerator and denominator, respectively.
    The ldiv_t type is a structure of longs defined (with typedef) in stdlib.h as follows:
        typedef struct {
            long int quot;  / * quotient * /
            long int rem;   / * remainder * /
        } ldiv_t;
    ldiv_t lx;
    lx = ldiv(100000L, 30000L);
    printf("100000 div 30000 = %ld remainder %ld\n", lx.quot, lx.rem);
*/
/*
loc_1528B:                              ; Logical Exclusive OR
   xor  ax, ax
   mov  dx, 16
   push ax                                  ; long int denom - high word
   push dx                                  ; long int denom - low word     (AKA divisor)
   push [word ptr bp+Entry_Size_Bytes+2]    ; long int numer - high word
   push [word ptr bp+Entry_Size_Bytes]      ; long int numer - low word     (AKA dividend)
   call LDIV@
   inc  ax
   mov  [bp+Entry_Size_Paras], ax
*/

void read_chunks(int nbytes)
{
    int chunk_size = 32768;
    int chunk_count;

    chunk_count = nbytes / chunk_size;

}

