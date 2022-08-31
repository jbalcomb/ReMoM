//#include <ALLOC.H>  /* coreleft(), farcoreleft(), malloc(), farmalloc(), free(), farfree() */
//#include <DOS.H>

#include <malloc.h>
#include <STDIO.H>      /* printf() */

/*
    Segmented Memory vs. Linear Memory

    MS DOS 16-bit
    MS Win 32/64-bit
*/
/*
    https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/malloc?view=msvc-170
    Syntax
    C
    void *malloc(size_t size);
    Parameters
        size        Bytes to allocate.
    Return Value    void pointer to the allocated space or NULL, if there is insufficient memory available.
*/

#define BYTES_PER_PARAGRAPH                 16

//Intel's C++ Compiler for Windows also defines _MSC_VER. If this is a concern, use #if _MSC_VER && !__INTEL_COMPILER
//#if _MSC_VER && !__INTEL_COMPILER
//char * gfp_tmpSAMB;
//#else
//char _FAR * gfp_tmpSAMB;
//#endif
#undef _FAR
#if _MSC_VER && !__INTEL_COMPILER
#define _FAR
#else
#define _FAR far
#endif

char _FAR * gfp_tmpSAMB;

// // _s08p07
// sgmt_addr SA_Allocate_Space(unsigned int nparas)
// {
//     sgmt_addr SAMB;
// 
//     gfp_tmpSAMB = (char _FAR *) farmalloc(((unsigned long)nparas * 16) + 16);
// 
//     if ( gfp_tmpSAMB == NULL )
//     {
//         SA_Alloc_Error(1, nparas); // Alloc Error #1: Allocation Too Small
//     }
//     
//     SAMB = FP_SEG(gfp_tmpSAMB) + 1;
// 
//     farpokew(SAMB, 4, SA_MEMSIG1);  // SAMB.MemSig1
//     farpokew(SAMB, 6, SA_MEMSIG2);  // SAMB.MemSig2
//     farpokew(SAMB, 8, nparas);      // SAMB.Size
//     farpokew(SAMB, 10, 1);          // SAMB.Used
//     
//     //Update_MemFreeWorst_KB();
// 
//     return SAMB;
// }
void * SA_Allocate_Space(unsigned int nparas)
{
    void * samb;
    samb = malloc((nparas * 16) + 1);

    *((unsigned int *)samb +  4) = 0x12FA;  // SA_MEMSIG1
    *((unsigned int *)samb +  6) = 0x4ECF;  // SA_MEMSIG2
    *((unsigned int *)samb +  8) = nparas;
    *((unsigned int *)samb + 10) = 1;
    
    return samb;
}

int SA_Alloc_Validate(void * samb)
{
    int is_valid;
    unsigned int word3;
    unsigned int word4;

    // if ( header1->padding[i] != BLK_DATA )

    // word3 = farpeekw(SAMB_head, 4);
    // word4 = farpeekw(SAMB_head, 6);
    word3 = *((unsigned int *)samb +  4);
    word4 = *((unsigned int *)samb +  6);

    if (word3 != 0x12FA || word4 != 0x4ECF)  // SA_MEMSIG1  ||  SA_MEMSIG2
    {
        is_valid = 0;  // ST_FAILURE
    }
    else
    {
        is_valid = -1;  // ST_SUCCESS
    }

    return is_valid;

}

void TST_SMLM(void)
{
    unsigned char * dynamically_allocated_array;
    unsigned char * lbx_header;
    int is_valid;

    dynamically_allocated_array = (unsigned char *)malloc(4096);
    if( dynamically_allocated_array == NULL )
        printf("FAILURE: \n");
    else
    {
        printf( "SUCCESS: \n" );
    }
    free(dynamically_allocated_array);

    lbx_header = (unsigned char *)SA_Allocate_Space(1);

    is_valid = SA_Alloc_Validate(lbx_header);

    if( is_valid == 0 )
        printf("FAILURE: \n");
    else if( is_valid == -1 )
    {
        printf( "SUCCESS: \n" );
    }
    else
    {
        printf( "ERROR: \n" );
    }

}
