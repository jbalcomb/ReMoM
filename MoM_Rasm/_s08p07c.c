// _s08p07c.c SA_Allocate_Space
// in ST_SA.H

/*
https://stanislavs.org/helppc/int_21-48.html
INT 21,48 - Allocate Memory
	AH = 48h
	BX = number of memory paragraphs requested
	on return:
	AX = segment address of allocated memory block (MCB + 1para)
	   = error code if CF set  (see DOS ERROR CODES)
	BX = size in paras of the largest block of memory available
	     if CF set, and AX = 08 (Not Enough Mem)
	CF = 0 if successful
	   = 1 if error
	- returns segment address of allocated memory block AX:0000
	- each allocation requires a 16 byte overhead for the MCB
	- returns maximum block size available if error

BCpp31LR, pg18/32
#include <dos.h>
int allocmem(unsigned size, unsigned *segp);
unsigned _dos_allocmem(unsigned size, unsigned *segp);

BCpp31LR, pg155/168
#include <alloc.h>
void far *farmalloc(unsigned long nbytes);

BCpp31LR, pg184/198
#include <dos.h>
unsigned FP OFF(void far *p);
unsigned FP SEG(void far *p);


    
    printf("DEBUG: farcoreleft() %lu\n", farcoreleft());
    printf("DEBUG: farheapcheck() %d\n", farheapcheck());
    / *
        _HEAPEMPTY is returned if there is no heap (value 1).
        _HEAPOK is returned if the heap is verified (value 2).
        _HEAPCORRUPT is returned if the heap has been corrupted (value -1).
    * /

*/

#include "ST_HEAD.H"

sgmt_addr SA_Allocate_Space(unsigned int nparas)
{
    sgmt_addr SAMB;

//    printf("DEBUG: [%s, %d] BEGIN: SA_Allocate_Space(nparas = %u)\n", __FILE__, __LINE__, nparas);

    gfp_tmpSAMB = (char _FAR *) farmalloc(((unsigned long)nparas * 16) + 16);
    //gfp_tmpSAMB = NULL;  // force failure, for testing
    if ( gfp_tmpSAMB == NULL )
    {
        //printf("DEBUG: Error: farmalloc(%lu) [%s, %d]\n", nbytes, __FILE__, __LINE__);
        SA_Alloc_Error(1, nparas); // Alloc Error #1: Allocation Too Small
    }
//    printf("DEBUG: gfp_tmpSAMB: %Fp  0x%08X\n", gfp_tmpSAMB, gfp_tmpSAMB);
    
    SAMB = FP_SEG(gfp_tmpSAMB) + 1;

    farpokew(SAMB, 4, SA_MEMSIG1);  // SAMB.MemSig1
    farpokew(SAMB, 6, SA_MEMSIG2);  // SAMB.MemSig2
    farpokew(SAMB, 8, nparas);      // SAMB.Size
    farpokew(SAMB, 10, 1);          // SAMB.Used
    
    // printf("DEBUG: [%s %d]: farpokew(SAMB = 0x%04X, 4 = 0x%04X, SA_MEMSIG1 = 0x%04X)\n", __FILE__, __LINE__, SAMB, 4, SA_MEMSIG1);
    // printf("DEBUG: [%s %d]: farpokew(SAMB = 0x%04X, 6 = 0x%04X, SA_MEMSIG2 = 0x%04X)\n", __FILE__, __LINE__, SAMB, 6, SA_MEMSIG2);
    // printf("DEBUG: [%s %d]: farpokew(SAMB = 0x%04X, 8 = 0x%04X, nparas = 0x%04X)\n", __FILE__, __LINE__, SAMB, 8, nparas);
    // printf("DEBUG: [%s %d]: farpokew(SAMB = 0x%04X, 10 = 0x%04X, 1 = 0x%04X)\n", __FILE__, __LINE__, SAMB, 10, 1);

    // printf("DEBUG: [%s %d]: farpeekw(SAMB, 4): 0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB, 4));
    // printf("DEBUG: [%s %d]: farpeekw(SAMB, 6): 0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB, 6));
    // printf("DEBUG: [%s %d]: farpeekw(SAMB, 8): 0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB, 8));
    // printf("DEBUG: [%s %d]: farpeekw(SAMB, 10): 0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB, 10));

    Update_MemFreeWorst_KB();

//    printf("DEBUG: END: SA_Allocate_Space()\n");
    return SAMB;
}
