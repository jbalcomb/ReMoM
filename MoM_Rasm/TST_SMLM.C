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
//unsigned int g_LBX_Header_Allocd = 0;           // MGC  dseg:3E66
//int g_LBX_FileHandle = -1;                      // MGC  dseg:3E68  DOS_UnusedFileHandle
//unsigned int UU_g_LBX_HdrFmtOvrRd = 0;          // MGC  dseg:3E6A
//char UU_g_LBX_FilePath[50] = {0};               // MGC  dseg:3E6C
//char g_LBX_FileExtension[] = ".LBX";            // MGC  dseg:3E9E
//unsigned int g_LBX_EntryCount;                  // MGC  dseg:A5C6
//unsigned int UU_g_LBX_HdrFmt;                   // MGC  dseg:A5C8
//unsigned int gsa_LBX_Header;                    // MGC  dseg:A5CA
//char g_LBX_Name[16];                            // MGC  dseg:A5CC
//unsigned int g_LBX_EmmRsvd;                     // MGC  dseg:A5EC
unsigned int LBX_Header_Allocd = 0;
FILE * LBX_FilePointer;
char LBX_FileExtension[] = ".LBX";
unsigned int LBX_EntryCount;
void * LBX_Header;
char LBX_Name[16];
unsigned int LBX_EmmRsvd;

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
    SAMB_head = malloc( (16 * (nparas + 1)) );

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
FILE * lbx_open(char *file_name)
{
    FILE * fp;

    fp = fopen(file_name, "rb");

    if (fp == NULL)
    {
        fp = 0;
    }

    return fp;
}
// _s09p03
int lbx_seek(long foffset, FILE * fp)
{
    int st_status;

    if ( fseek(fp, foffset, SEEK_SET) != 0 )
    {
        st_status = 0;  // ST_FAILURE
    }
    else
    {
        st_status = -1;  // ST_SUCCESS
    }

    return st_status;
}

// _s09p05  lbx_read_sgmt
// _s09p06  lbx_read_ofst
int lbx_read(void * buff, int nbytes, FILE * fp)
{
    //size_t items_read;  // vcruntime.h  #ifdef _WIN64 typedef unsigned __int64 size_t; #else typedef unsigned int size_t;
    unsigned int items_read;
    int st_status;

    if ((items_read = fread(buff, 1, nbytes, fp)) != nbytes)
    {
        st_status = 0;  // ST_FAILURE
    }
    else
    {
        st_status = -1;  // ST_SUCCESS
    }

    return st_status;
}

// _s10p16
void ExtractFileBase(char * LbxFileName)
{
    int itr_filename;
    itr_filename = 0;
    while (LbxFileName[itr_filename] != '\0')
    {
        if ( LbxFileName[itr_filename] >= 'a' )
            LbxFileName[itr_filename] = LbxFileName[itr_filename] - 32;  // {'a' - 32 = 'A', ..., 'z' - 32 = 'Z'}
        if ( LbxFileName[itr_filename] == '.' )
            LbxFileName[itr_filename] = '\0';
        itr_filename++;
    }
}

// _s10p10
void * LBX_Load_Entry(char * LbxName, int LbxEntry, void * SAMB_head, int LoadType, int LbxHdrFmt)
{
    void * SAMB_data;
    char LbxFileName[20];
    unsigned int EntryTableOffset;
    unsigned long DataOffset_Begin; // LBXENTRYOFFSET()
    unsigned long DataOffset_End;   // LBXENTRYOFFSET()
    unsigned long DataSize_Bytes;   // LBXENTRYOFFSET()
    unsigned long DataSize_Paras;   // LBXLOADTYPE()
    //unsigned int tmp_SAMB_Size;     // LBXLOADTYPE()
    unsigned char * tmp_SAMB_data;        // LBXREADDATA()
    unsigned int ReadNbytes;        // LBXREADDATA()

    SAMB_data = 0;  // NULL
    // ...
    LBX_Header = SA_Allocate_MemBlk(32);  // SZ_LBX_HDR_PR  (512 bytes / 16 bytes per paragraph = 32 paragraphs)
    // ...
    ExtractFileBase(LbxName);
    // ...
    // SAMB_data = EMM_LBX_Load_Entry(LbxName, LbxEntry, SAMB_head, LoadType, tmp_LbxHdrFmt);

    /*    BEGIN: Current vs. Previous    */
    strcpy(LBX_Name, LbxName);
    strcpy(LbxFileName, LbxName);
    strcat(LbxFileName, LBX_FileExtension);
    LBX_FilePointer = lbx_open(LbxFileName);
    lbx_seek(0, LBX_FilePointer);
    lbx_read(LBX_Header, 512, LBX_FilePointer);
    //if ( farpeekw(gsa_LBX_Header, 2) != LBX_MAGSIG )
    //LBX_EntryCount = farpeekw(LBX_Header, 0);
    // LBX_EntryCount = *((unsigned int *)LBX_Header + 0);
    // //#define GET_LE_16(_p_) (((uint16_t)(((uint8_t const *)(_p_))[0])) | (((uint16_t)(((uint8_t const *)(_p_))[1])) << 8))
    // LBX_EntryCount = (((uint16_t)(((uint8_t const *)(LBX_Header))[0])) | (((uint16_t)(((uint8_t const *)(LBX_Header))[1])) << 8));
    LBX_EntryCount = ((unsigned int)*((unsigned char *)LBX_Header + 0)) | (unsigned int)((unsigned int)*((unsigned char *)LBX_Header + 1) << 8);
    printf("LBX_EntryCount: %u\n", LBX_EntryCount);
    //if ( g_LBX_EntryCount < LbxEntry )
    /*    END: Current vs. Previous    */

    ////#define LBXENTRYOFFSET()
    EntryTableOffset = (8 + (4 * LbxEntry));
    printf("EntryTableOffset: 0x%04X\n", EntryTableOffset);
    //DataOffset_Begin = farpeekdw(LBX_Header, EntryTableOffset);
    //DataOffset_End = farpeekdw(LBX_Header, EntryTableOffset + 4);
    DataOffset_Begin = ((unsigned long)*((unsigned char *)LBX_Header + EntryTableOffset + 0)) | (unsigned long)((unsigned long)*((unsigned char *)LBX_Header + EntryTableOffset + 1) << 8) | (unsigned long)((unsigned long)*((unsigned char *)LBX_Header + EntryTableOffset + 2) << 16) | (unsigned long)((unsigned long)*((unsigned char *)LBX_Header + EntryTableOffset + 3) << 24);
    printf("DataOffset_Begin: 0x%08X\n", DataOffset_Begin);

    DataOffset_End = ((unsigned long)*((unsigned char *)LBX_Header + EntryTableOffset + 4 + 0)) | (unsigned long)((unsigned long)*((unsigned char *)LBX_Header + EntryTableOffset + 4 + 1) << 8) | (unsigned long)((unsigned long)*((unsigned char *)LBX_Header + EntryTableOffset + 4 + 2) << 16) | (unsigned long)((unsigned long)*((unsigned char *)LBX_Header + EntryTableOffset + 4 + 3) << 24);
    printf("DataOffset_End: 0x%08X\n", DataOffset_End);

    DataSize_Bytes = DataOffset_End - DataOffset_Begin;
    printf("DataSize_Bytes: %u\n", DataSize_Bytes);

    lbx_seek(DataOffset_Begin, LBX_FilePointer);

    DataSize_Paras = 1 + (DataSize_Bytes / 16);  // SZ_PARAGRAPH_B
    printf("DataSize_Paras: %u\n", DataSize_Paras);

    //if ( LoadType == 0 )
    SAMB_data = SA_Allocate_MemBlk(DataSize_Paras);

    tmp_SAMB_data = (unsigned char *)SAMB_data;

    //#define LBXREADDATA() {
    #define SZ_32K_B 32768
    #define SZ_32K_PR 2048
    ReadNbytes = SZ_32K_B;
    while ( DataSize_Bytes >= SZ_32K_B )
    {
        DataSize_Bytes -= SZ_32K_B;
        lbx_read(tmp_SAMB_data, ReadNbytes, LBX_FilePointer);
        tmp_SAMB_data += SZ_32K_PR;
    }
    if ( DataSize_Bytes > 0 )
    {
        ReadNbytes = DataSize_Bytes;
        lbx_read(tmp_SAMB_data, ReadNbytes, LBX_FilePointer);
    }

    return SAMB_data;
}

void * LBXE_LoadSingle(char *LbxName, int LbxEntryIndex)
{
    void * SAMB_head;
    int LoadType;
    int LbxHdrFmt;
    void * SAMB_data;

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
    void * FontStyleData;

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

    FontStyleData = LBXE_LoadSingle(Font_File, 0);

    //strcpyfar(unsigned int dst_ofst, unsigned int dst_sgmt, unsigned int src_ofst, unsigned int src_sgmt)
    //printf("%s",fp_dst_strcpyfar);

}
