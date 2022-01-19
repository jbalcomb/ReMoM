
#include <DOS.H>

#include "ST_LBX.H"

#include "TST_MGC.H"

#include "STU_UTIL.H"

/*

LBX_EntryLoader

Alloc Global LBX Header
Extract FileName Base
Cache from EMM
Curr == Prev
Open, Read Header, Test MagSig, Test Entry Count
Entry Size Bytes = Entry Offset End - Entry Offset Begin
Entry Size Paras = Entry Size Bytes / 16
Load Type {0, 1, 2, n} ? New, Replace, Append
...
void _segp * LBX_Data_Seg = AllocMem_EntrySizeParas()
void _segp * Load_To_Seg = LBX_Data_Seg


*/

#define GETNEWBLOCK {if(!mmfree)MML_ClearBlock();mmnew=mmfree;mmfree=mmfree->next;}

//#define LBXHEADER {if(!g_LBX_Header_Allocd){g_LBX_Header_Allocd = 1;gsa_LBX_Header = AllocMem_LbxHeader(32);}}
//#define LBXHEADER {if(!g_LBX_Header_Allocd){g_LBX_Header_Allocd = 1;gsa_LBX_Header = SA_Allocate_MemBlk(32);}}

unsigned int LbxLdEt1(char * LBX_Name, int LBX_Entry)
{
    char LBX_FileName[20];
    char LBX_FileName2[20];
    char LBX_FileName3[20];
    char LBX_FileName4[20];
    int result;
    int tmpReturn;
    unsigned int tmpLbxMagSig;
    unsigned char far * fptrUchar;
    unsigned int far * fptrUint;
    unsigned int itr;
    int tmpLbxEntryCount;
    unsigned long varLbxFileOffset;
    unsigned int EntryTableOffset;
    long Entry_Offset_Begin;
    long Entry_Offset_End;
    long Entry_Size_Bytes;
    int Entry_Size_Paras;
    unsigned int EntryData_SgmtAddr;
    unsigned int LoadData_SgmtAddr;
    unsigned int nbytes_read;


    printf("DEBUG: BEGIN: LbxLdEt1()\n");

    // if ( g_LBX_Header_Allocd == 0 )
    // {
    //     g_LBX_Header_Allocd = 1;
    //     gsa_LBX_Header = AllocMem_LbxHeader(32);
    // }
    LBXHEADER()
    printf("DEBUG: gsa_LBX_Header: 0x%08X\n", gsa_LBX_Header);
    wait_for_any_key();
    
    printf("DEBUG: LBX_Name: %s\n", LBX_Name);
    ExtractFileBase(LBX_Name);
    printf("DEBUG: LBX_Name: %s\n", LBX_Name);

    strcpy(g_LBX_Name, LBX_Name);
    printf("DEBUG: g_LBX_Name: %s\n", g_LBX_Name);
    
    strcpy(LBX_FileName, LBX_Name);
    printf("DEBUG: LBX_FileName: %s\n", LBX_FileName);
    strcat(LBX_FileName, g_LBX_FileExtension);
    printf("DEBUG: LBX_FileName: %s\n", LBX_FileName);

    // strcpy(LBX_FileName2, LBX_Name);
    // printf("DEBUG: LBX_FileName2: %s\n", LBX_FileName2);
    // strcat(LBX_FileName2, ".LBX");
    // printf("DEBUG: LBX_FileName2: %s\n", LBX_FileName2);


    // strcpy(LBX_FileName3, "BACKGRND");
    // printf("DEBUG: LBX_FileName3: %s\n", LBX_FileName3);
    // strcat(LBX_FileName3, g_LBX_FileExtension);
    // printf("DEBUG: LBX_FileName3: %s\n", LBX_FileName3);

    // strcpy(LBX_FileName4, "BACKGRND");
    // printf("DEBUG: LBX_FileName4: %s\n", LBX_FileName4);
    // strcat(LBX_FileName4, ".LBX");
    // printf("DEBUG: LBX_FileName4: %s\n", LBX_FileName4);

    // printf("DEBUG: g_LBX_FileHandle: 0x%08X\n", g_LBX_FileHandle);
    // result = dos_open_file_handle("BACKGRND.LBX");
    // printf("DEBUG: result: %d\n", result);
    // result = dos_open_file_handle(LBX_FileName);
    // printf("DEBUG: result: %d\n", result);
    // result = dos_open_file_handle(LBX_FileName2);
    // printf("DEBUG: result: %d\n", result);
    // result = dos_open_file_handle(LBX_FileName3);
    // printf("DEBUG: result: %d\n", result);
    // result = dos_open_file_handle(LBX_FileName4);
    // printf("DEBUG: result: %d\n", result);

    g_LBX_FileHandle = lbx_open(LBX_FileName);
    printf("DEBUG: g_LBX_FileHandle: 0x%08X\n", g_LBX_FileHandle);
    
    //varLbxFileOffset = 512;
    varLbxFileOffset = 0;

    tmpReturn = lbx_seek(varLbxFileOffset, g_LBX_FileHandle);
    if ( tmpReturn == 0 )
    {
        printf("DEBUG: FAILURE: lbx_seek(%lu, %d) == %d\n", varLbxFileOffset, g_LBX_FileHandle, tmpReturn);
    }
    else
    {
        printf("DEBUG: SUCCESS: lbx_seek(%lu, %d) == %d\n", varLbxFileOffset, g_LBX_FileHandle, tmpReturn);
    }
    
    tmpReturn = lbx_read_sgmt(gsa_LBX_Header, 512, g_LBX_FileHandle);
    if ( tmpReturn == 0 )
    {
        printf("DEBUG: FAILURE: lbx_read_sgmt(gsa_LBX_Header, 512, g_LBX_FileHandle)\n");
    }
    else
    {
        printf("DEBUG: SUCCESS: lbx_read_sgmt(gsa_LBX_Header, 512, g_LBX_FileHandle)\n");
    }
    
    fptrUchar = MK_FP(gsa_LBX_Header, 0);
    fptrUint = MK_FP(gsa_LBX_Header, 0);
    for ( itr = 0; itr < 8; itr++ )
    {
        printf("%02X\n", fptrUchar[itr]);
    }
    for ( itr = 0; itr < 2; itr++ )
    {
        printf("%04X\n", fptrUint[itr]);
    }

    tmpLbxMagSig = farpeekw(gsa_LBX_Header, 2);
    printf("DEBUG: tmpLbxMagSig == 0x%04X\n", tmpLbxMagSig);

    if ( tmpLbxMagSig != 0xFEAD )
    {
        printf("DEBUG: FAILURE: LBX MagSig\n");
    }
    else
    {
        printf("DEBUG: SUCCESS: LBX MagSig\n");
    }

    g_LBX_EntryCount = farpeekw(gsa_LBX_Header, 0);
    printf("DEBUG: g_LBX_EntryCount == 0x%04X\n", g_LBX_EntryCount);

    if ( g_LBX_EntryCount < LBX_Entry )
    {
        printf("DEBUG: FAILURE: LBX Entry Count\n");
    }
    else
    {
        printf("DEBUG: SUCCESS: LBX Entry Count\n");
    }

    EntryTableOffset = ( 8 + ( LBX_Entry * 4) );
    printf("DEBUG: EntryTableOffset == 0x%04X\n", EntryTableOffset);

    Entry_Offset_Begin = farpeekdw(gsa_LBX_Header, EntryTableOffset);
    printf("DEBUG: Entry_Offset_Begin == 0x%08X\n", Entry_Offset_Begin);

    EntryTableOffset = EntryTableOffset + 4;
    printf("DEBUG: EntryTableOffset == 0x%04X\n", EntryTableOffset);

    Entry_Offset_End = farpeekdw(gsa_LBX_Header, EntryTableOffset);
    printf("DEBUG: Entry_Offset_End == 0x%08X\n", Entry_Offset_End);

    Entry_Size_Bytes = Entry_Offset_End - Entry_Offset_Begin;
    printf("DEBUG: Entry_Size_Bytes == %ld\n", Entry_Size_Bytes);

    tmpReturn = lbx_seek(Entry_Offset_Begin, g_LBX_FileHandle);
    if ( tmpReturn == 0 )
    {
        printf("DEBUG: FAILURE: lbx_seek(%lu, %d) == %d\n", Entry_Offset_Begin, g_LBX_FileHandle, tmpReturn);
    }
    else
    {
        printf("DEBUG: SUCCESS: lbx_seek(%lu, %d) == %d\n", Entry_Offset_Begin, g_LBX_FileHandle, tmpReturn);
    }
    
    Entry_Size_Paras = 1 + (Entry_Size_Bytes / 16);
    printf("DEBUG: Entry_Size_Paras == %d\n", Entry_Size_Paras);

    //EntryData_SgmtAddr = AllocMem_EntrySize(Entry_Size_Paras);
    //EntryData_SgmtAddr = AllocMem_LbxHeader(Entry_Size_Paras);
    EntryData_SgmtAddr = SA_Allocate_MemBlk(Entry_Size_Paras);

    LoadData_SgmtAddr = EntryData_SgmtAddr;

    nbytes_read = 32768;  // chunk size bytes

    printf("DEBUG: %ld > %u == %d\n", Entry_Size_Bytes, nbytes_read, ( Entry_Size_Bytes > nbytes_read ));
    while ( Entry_Size_Bytes > nbytes_read )
    {
        printf("DEBUG: %ld > %u\n", Entry_Size_Bytes, nbytes_read);
        Entry_Size_Bytes = Entry_Size_Bytes - nbytes_read;
        lbx_read_sgmt(LoadData_SgmtAddr, nbytes_read, g_LBX_FileHandle);
        LoadData_SgmtAddr = LoadData_SgmtAddr + 2048;  // chunk size paragraphs
    }

    printf("DEBUG: %ld > %u == %d\n", Entry_Size_Bytes, 0, ( Entry_Size_Bytes > 0 ));
    if ( Entry_Size_Bytes > 0 )
    {
        nbytes_read = Entry_Size_Bytes;
        lbx_read_sgmt(LoadData_SgmtAddr, nbytes_read, g_LBX_FileHandle);
    }

    // //Update_MinFreePages();
    // RAM_Update_WorstFreeBlock_KB();

    printf("DEBUG: zxcvzxcv\n");
    
    printf("DEBUG: END: LbxLdEt1()\n");
    return EntryData_SgmtAddr;
}

int dos_open_file_handle(char *fname)
{
asm push ds
asm mov  ax, 3D00h
//asm mov  dx, fname
asm lds  dx, fname
asm int  21h
asm jc _openFailed
asm pop ds
    return _AX;
_openFailed:
asm pop ds
    return (0);
}
/*
J:\STU\DBWD\BORLANDC\CRTL\CLIB\DOSALLOC.C
... _dos_allocmem - allocates DOS memory segment (MSC compatible)
*/
unsigned _dos_allocmem(unsigned size, unsigned *segp)
{
    _BX = size;
    _AH = 0x48;
    geninterrupt(0x21);
    if (_FLAGS & 1)                     /* if carry set, error */
    {
        *segp = _BX;                    /* return max. block size */
        //return (__DOSerror(_AX));       /* set errno */
        return (_AX);
    }
    else
    {
        *segp = _AX;                    /* return block address */
        return (0);
    }
}
