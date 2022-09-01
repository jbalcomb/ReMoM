//#include <ALLOC.H>  /* coreleft(), farcoreleft(), malloc(), farmalloc(), free(), farfree() */
//#include <DOS.H>

#include <malloc.h>
#include <STDIO.H>      /* printf() */
#include <STRING.H>
//#include <fcntl.h>      /* O_BINARY, O_RDONLY */
#include <fcntl.h>      /* _O_BINARY, _O_RDONLY */
//#include <stdio.h> 
//#include <string.h>
//#include <unistd.h> 
//#include <iostream>
#include <io.h>
// ? corecrt.h ? _open()
// C:\Program Files (x86)\Windows Kits\10\include\10.0.19041.0\ucrt\corecrt_io.h(392): note: see declaration of '_open'
/*
        inline int __CRTDECL _open(
            _In_z_ char const* const _FileName,
            _In_   int         const _OFlag,
            _In_   int         const _PMode = 0
            )
        {
            int _FileHandle;
            // Last parameter passed as 0 because we don't want to validate pmode from _open
            errno_t const _Result = _sopen_dispatch(_FileName, _OFlag, _SH_DENYNO, _PMode, &_FileHandle, 0);
            return _Result ? -1 : _FileHandle;
        }

*/

#include "_s10p16c.c"

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

/*
    MGC  ST_LBX
*/
unsigned int g_LBX_Header_Allocd = 0;           // MGC  dseg:3E66
int g_LBX_FileHandle = -1;                      // MGC  dseg:3E68  DOS_UnusedFileHandle
unsigned int UU_g_LBX_HdrFmtOvrRd = 0;          // MGC  dseg:3E6A
char UU_g_LBX_FilePath[50] = {0};               // MGC  dseg:3E6C
char g_LBX_FileExtension[] = ".LBX";            // MGC  dseg:3E9E
unsigned int g_LBX_EntryCount;                  // MGC  dseg:A5C6
unsigned int UU_g_LBX_HdrFmt;                   // MGC  dseg:A5C8
unsigned int gsa_LBX_Header;                    // MGC  dseg:A5CA
char g_LBX_Name[16];                            // MGC  dseg:A5CC
unsigned int g_LBX_EmmRsvd;                     // MGC  dseg:A5EC

char Font_File[] = "FoNtS.LbX";


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
    void * SAMB_head;
    SAMB_head = malloc((nparas * 16) + 1);

    *((unsigned int *)SAMB_head +  4) = 0x12FA;  // SA_MEMSIG1
    *((unsigned int *)SAMB_head +  6) = 0x4ECF;  // SA_MEMSIG2
    *((unsigned int *)SAMB_head +  8) = nparas;  // Size (in Paragraphs)
    *((unsigned int *)SAMB_head + 10) = 1;       // Used (in Paragraphs)
    
    return SAMB_head;
}

void * SA_Allocate_MemBlk(unsigned int nparas)
{
    void * SAMB_head;
    SAMB_head = malloc((nparas * 16) + 1);
    
    return SAMB_head;
}

int SA_Alloc_Validate(void * SAMB_head)
{
    int is_valid;
    unsigned int memsig1;
    unsigned int memsig2;

    // if ( header1->padding[i] != BLK_DATA )

    // memsig1 = farpeekw(SAMB_head, 4);
    // memsig2 = farpeekw(SAMB_head, 6);
    memsig1 = *((unsigned int *)SAMB_head +  4);
    memsig2 = *((unsigned int *)SAMB_head +  6);

    if (memsig1 != 0x12FA || memsig2 != 0x4ECF)  // SA_MEMSIG1  ||  SA_MEMSIG2
    {
        is_valid = 0;  // ST_FAILURE
    }
    else
    {
        is_valid = -1;  // ST_SUCCESS
    }

    return is_valid;

}

/*
Microsoft directly supports POSIX-style low-level IO calls such as open(), read(), , write(), and close(); although with what appears to be a misleading "deprecated" characterization.
The required header is <io.h>.
The calls correspond to functions named with a preceeding underscore, so open() maps to _open().
*/
// _s09p01
int lbx_open(char *fname)
{
    int fhandle;

    //fhandle = open(fname, O_RDONLY|O_BINARY);
    fhandle = _open(fname, _O_RDONLY|_O_BINARY);
    printf("fhandle: %d\n", fhandle);

    if (fhandle == -1)
    {
        fhandle = 0;
    }

    return fhandle;
}

unsigned int LBX_Load_Entry(char *LbxName, int LbxEntry, unsigned int SAMB_head, int LoadType, int LbxHdrFmt)
{
    unsigned int SAMB_data;
    char tmp_LbxFileName[20];

    SAMB_data = 0;
    // ...
    // gsa_LBX_Header = SA_Allocate_MemBlk(32);  // SZ_LBX_HDR_PR
    // ...
    ExtractFileBase(LbxName);
    // ...
    // SAMB_data = EMM_LBX_Load_Entry(LbxName, LbxEntry, SAMB_head, LoadType, tmp_LbxHdrFmt);
    /*    BEGIN: Current vs. Previous    */
    strcpy(g_LBX_Name, LbxName);
    strcpy(tmp_LbxFileName, LbxName);
    strcat(tmp_LbxFileName, g_LBX_FileExtension);
    printf("tmp_LbxFileName: %s\n", tmp_LbxFileName);
    g_LBX_FileHandle = lbx_open(tmp_LbxFileName);
    printf("g_LBX_FileHandle: %d\n", g_LBX_FileHandle);


    return SAMB_data;
}

unsigned int LBXE_LoadSingle(char *LbxName, int LbxEntryIndex)
{
    unsigned int SAMB_head;
    int LoadType;
    int LbxHdrFmt;
    unsigned int SAMB_data;

    SAMB_head = 0;
    LoadType = 0;
    LbxHdrFmt = 0;

    SAMB_data = LBX_Load_Entry(LbxName, LbxEntryIndex, SAMB_head, LoadType, LbxHdrFmt);

    return SAMB_data;
}

void TST_SMLM(void)
{
    unsigned char * dynamically_allocated_array;
    unsigned char * lbx_header;
    int is_valid;
    unsigned int sa_FontStyleData;

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

    sa_FontStyleData = LBXE_LoadSingle(Font_File, 0);

    //strcpyfar(unsigned int dst_ofst, unsigned int dst_sgmt, unsigned int src_ofst, unsigned int src_sgmt)
    //printf("%s",fp_dst_strcpyfar);

}
