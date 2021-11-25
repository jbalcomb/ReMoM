// _s10p10c.c LBX_EntryLoader

/*

gLbxHeader
LbxHdrFmtTyp
|-> EMM_LBX_Load
Gloabls - Curr. vs. Prev.
LoadType
DOS Read - Chunks 32K B / 2K PR

*/

/*
char *LBX_Alloc_Seg = ...ST Mem Blk...farmalloc;
char *LBX_Data_Seg = LBX_Alloc_Seg + LbxEntrySize;

Pointer arithmetic is fairly simple to understand. If you have a pointer to the first element of an array then p + 1 points to the second element and so on regardless of size of each element. So even if you had an array of ints, or an arbitrary structure MyData it would hold true.

MyData data[100];
MyData *p1 = data;;  // same as &data[0]
MyData *p2 = p1 + 1; // same as &data[1]
MyData *p3 = p2 + 1; // same as &data[2]
MyData *p4 = p2 - 1; // same as &data[0] again

If your array is unsigned char then you just add however many bytes offset you wish to go into the array, e.g.

unsigned char data[16384];
unsigned char *offset = data + 512; // same as &data[512]
*offset = 5; // same as data[512] = 5;

Alternatively if the notation is confusing, you can always refer to it as it is shown in comments above, e.g. &data[512]

*/

#include "ST_HEAD.H"
#include "ST_LBX.H"


unsigned int LBX_EntryLoader(char *LbxName, int LbxEntryIndex, unsigned int AllocSeg, int LoadType, int FormatType)
{
// LBX_NameWithPath= byte ptr -6Ch
// LBX_FileName= byte ptr -30h
// Entry_Size_Bytes= dword ptr -1Ch
// Next_Entry_In_File= dword ptr -18h
// Entry_Off_In_File= dword ptr -14h
// File_Offset= dword ptr -10h
// Bytes_To_Copy= word ptr -0Ch
// Load_To_Seg= word ptr -0Ah
// Entry_Size_Paras= word ptr -8
// varLbxFileFormat= word ptr -6
// MemberOffset= word ptr -4
// LBX_Data_Seg= word ptr -2

    char LBX_FilePath[60];
    char LBX_FileName[20];
    unsigned int LBX_Data_Seg;
    unsigned int Load_To_Seg;
    int Bytes_To_Copy;
    int varLbxFileFormat;
    long varLbxFileOffset;
    int varLbxFileOffset_Lo;
    int varLbxFileOffset_Hi;
    int EntryOffsetTable;
    //int Entry_Offset_Begin_Lo;
    //int Entry_Offset_Begin_Hi;
    //int Entry_Offset_End_Lo;
    //int Entry_Offset_End_Hi;
    long Entry_Offset_Begin;
    long Entry_Offset_End;
    long Entry_Size_Bytes;
    long Entry_Size_Paras;
    int tmpMemBlkSizeParas;
    int tmpMemBlkUsedParas;
    int tmpMemBlkFreeParas;
    int tmpBytesRead;

    printf("DEBUG: BEGIN: LBX_EntryLoader()\n");

    if ( LbxEntryIndex < 0 )
    {
        printf("DEBUG: Error: [%s, %d]\n", __FILE__, __LINE__);
        LBX_Error(LbxName, 0x01, LbxEntryIndex);  // LBXErr_not_found
    }

    if (g_LBX_Header_Allocd == 0)
    {
        g_LBX_Header_Allocd = 1;
        //g_LBX_Header = AllocMem_LbxHeader(32);  // 32 paragraphs = 32 * 16 = 512 bytes
        gsa_LBX_Header = SA_Allocate_MemBlk(SZ_LBX_HDR_PR);  // 32 paragraphs = 32 * 16 = 512 bytes
    }

    printf("DEBUG: LbxName: %s()\n");
    StripFileExt(LbxName);
    printf("DEBUG: LbxName: %s()\n");

    if ( (FormatType == 0) || (UU_g_LBX_HdrFmt == 2) )
    {
        varLbxFileFormat = 0;
    }
    else
    {
        varLbxFileFormat = 1;
    }

    LBX_Data_Seg = EMM_LBX_EntryLoader(LbxName, LbxEntryIndex, AllocSeg, LoadType, varLbxFileFormat);

    if ( LBX_Data_Seg == 0 )
    {
    
    /*
        BEGIN: Globals - Current vs. Previous
    */
        /*
            If LBX_LastLoadHnd != -1
            And LbxName == LBX_LastLoadName
            And varLbxFileFormat == LBX_LastLoadFmt
            And LbxEntryIndex < LBX_LastLoadECount
        */
        if ( (g_LBX_FileHandle != -1) && (stricmp(LbxName, g_LBX_Name) != 0) && (varLbxFileFormat == g_LBX_FormatType))
        {
            g_LBX_FormatType = varLbxFileFormat;

            if ( g_LBX_FileHandle != -1 )
            {
                lbx_close(g_LBX_FileHandle);
            }
            
            strcpy(g_LBX_Name, LbxName);

            strcpy(LBX_FileName, LbxName);
            strcat(LBX_FileName, g_LBX_FileExtension);
            g_LBX_FileHandle = lbx_open(LBX_FileName);

            if ( g_LBX_FileHandle == 0 )
            {
                if ( UU_g_LBX_FilePath == 0 )
                {
                    printf("DEBUG: Error: [%s, %d]\n", __FILE__, __LINE__);
                    LBX_Error(LbxName, 0x01, LbxEntryIndex);  // LBXErr_not_found
                }
                else
                {
                    strcpy(LBX_FilePath, UU_g_LBX_FilePath);
                    strcat(LBX_FilePath, LBX_FileName);
                    g_LBX_FileHandle = lbx_open(LBX_FileName);
                    if ( g_LBX_FileHandle == 0 )
                    {
                        printf("DEBUG: Error: [%s, %d]\n", __FILE__, __LINE__);
                        LBX_Error(LbxName, 0x01, LbxEntryIndex);  // LBXErr_not_found
                    }
                }
            }

            if ( g_LBX_FormatType == 0 )  // gLBX_FormatType ~= varLbxFileFormat ~= argLbxFrmtTyp
            {
                //////varLbxFileOffset = 0;
                ////varLbxFileOffset_Hi = 0;
                ////varLbxFileOffset_Lo = 0;
                //varLbxFileOffset = (0 << 8) | 0;
                varLbxFileOffset = 0;
            }
            else
            {
                //////varLbxFileOffset = 512;
                ////varLbxFileOffset_Hi = 0;
                ////varLbxFileOffset_Lo = 512;
                //varLbxFileOffset = (0 << 8) | 512;
                varLbxFileOffset = 512;
            }
            
            ////if ( DISK_LSeek(varLbxFileOffset, gLBX_FileHandle) == 0)
            //if ( DISK_LSeek(varLbxFileOffset_Lo, varLbxFileOffset_Hi, gLBX_FileHandle) == 0)
            if ( lbx_seek(varLbxFileOffset, g_LBX_FileHandle) == 0)
            {
                printf("DEBUG: Error: [%s, %d]\n", __FILE__, __LINE__);
                LBX_Error(LbxName, 0x02, LbxEntryIndex);  // LBXErr_corrupted
            }

            // unsigned _dos_read(int handle, void far *buf, unsigned *nread);
            lbx_read_sgmt(gsa_LBX_Header, 512, g_LBX_FileHandle);

            if ( farpeekw(gsa_LBX_Header, 2) != 0xFEAD )  // LBX_HEADER_OFST_MAGSIG ... LBX_FileSig
            {
                printf("DEBUG: Error: [%s, %d]\n", __FILE__, __LINE__);
                LBX_Error(LbxName, 0x07, LbxEntryIndex);  // LBXErr_bad_header
            }

            g_LBX_EntryCount = farpeekw(gsa_LBX_Header, 0);

        }

        /*
            END: Globals - Current vs. Previous
        */

        
        if ( g_LBX_EntryCount < LbxEntryIndex )
        {
            printf("DEBUG: Error: [%s, %d]\n", __FILE__, __LINE__);
            LBX_Error(LbxName, 0x08, LbxEntryIndex);  // LBXErr_entries_exceeded
        }

        EntryOffsetTable = LbxEntryIndex * 4 + 8;                       // Entry Index * Entry Offset Size + Header Preamble Size
        Entry_Offset_Begin = farpeekdw(gsa_LBX_Header, EntryOffsetTable);
        EntryOffsetTable = EntryOffsetTable + 4;                        // ... + Entry Offset Size
        Entry_Offset_End = farpeekdw(gsa_LBX_Header, EntryOffsetTable);

        Entry_Size_Bytes = Entry_Offset_End - Entry_Offset_Begin;

        //DISK_LSeek(Entry_Offset_Begin_Lo, Entry_Offset_Begin_Hi, gLBX_FileHandle);
        if ( lbx_seek(Entry_Offset_Begin, g_LBX_FileHandle) == 0 )
        {
            printf("DEBUG: Error: [%s, %d]\n", __FILE__, __LINE__);
            LBX_Error(LbxName, 0x02, LbxEntryIndex);  // LBXErr_corrupted
        }

        Entry_Size_Paras = Entry_Size_Bytes / 16;


        /*
            BEGIN: LBX Load Type
        */
        if ( LoadType == 0 )
        {
            //LBX_Data_Seg = AllocMem_EntrySize(Entry_Size_Paras);
            LBX_Data_Seg = SA_Allocate_MemBlk(Entry_Size_Paras);

            if ( LBX_Data_Seg == 0 )
            {
                printf("DEBUG: Error: [%s, %d]\n", __FILE__, __LINE__);
                LBX_Error(LbxName, 0x03, LbxEntryIndex);  // LBXErr_low_RAM
            }
        }
        else
        {
            /* BEGIN: Load Type 1 or 2 */
            if ( LoadType == 1 )
            {
                if ( SA_Alloc_Validate(AllocSeg) )
                {
                    printf("DEBUG: Error: [%s, %d]\n", __FILE__, __LINE__);
                    LBX_Error(LbxName, 0x02, LbxEntryIndex);  // LBXErr_corrupted
                }

                tmpMemBlkSizeParas = farpeekw(AllocSeg, 8); // LBX_Alloc_Space_Header.Size_Paras
                tmpMemBlkSizeParas = tmpMemBlkSizeParas - 1;

                if (tmpMemBlkSizeParas < Entry_Size_Paras)
                {
                    printf("DEBUG: Error: [%s, %d]\n", __FILE__, __LINE__);
                    tmpMemBlkSizeParas = farpeekw(AllocSeg, 8); // LBX_Alloc_Space_Header.Size_Paras
                    tmpMemBlkSizeParas = Entry_Size_Paras - tmpMemBlkSizeParas;
                    tmpMemBlkSizeParas = tmpMemBlkSizeParas + 1;
                    LBX_Error(LbxName, 0x04, LbxEntryIndex, tmpMemBlkSizeParas);  // LBXErr_alloc_fail
                }

                LBX_Data_Seg = AllocSeg + 1;

                tmpMemBlkUsedParas = Entry_Size_Paras + 1;
            }

            if ( LoadType == 2 )
            {
                if ( SA_Alloc_Validate(AllocSeg) )
                {
                    printf("DEBUG: Error: [%s, %d]\n", __FILE__, __LINE__);
                    LBX_Error(LbxName, 0x02, LbxEntryIndex);  // LBXErr_corrupted
                }

                tmpMemBlkFreeParas = SA_Alloc_GetFree(AllocSeg);
                if ( tmpMemBlkFreeParas < Entry_Size_Paras)
                {
                    printf("DEBUG: Error: [%s, %d]\n", __FILE__, __LINE__);
                    tmpMemBlkFreeParas = SA_Alloc_GetFree(AllocSeg);
                    tmpMemBlkFreeParas = Entry_Size_Paras - tmpMemBlkFreeParas;
                    LBX_Error(LbxName, 0x05, LbxEntryIndex, tmpMemBlkFreeParas);  // LBXErr_reload_fail
                }

                tmpMemBlkUsedParas = farpeekw(AllocSeg, 10);  // LBX_Alloc_Space_Header.Used_Paras

                LBX_Data_Seg = AllocSeg + tmpMemBlkUsedParas;

                tmpMemBlkUsedParas = farpeekw(AllocSeg, 10);  // LBX_Alloc_Space_Header.Used_Paras

                tmpMemBlkUsedParas = Entry_Size_Paras + tmpMemBlkUsedParas;
            }
            /* END: Load Type 1 or 2 */
            farpokew(AllocSeg, 10, tmpMemBlkUsedParas);  // LBX_Alloc_Space_Header.Used_Paras

        }

        /*
            END: LBX Load Type
        */
    
        /*
            BEGIN: Read Entry Data
        */
        Load_To_Seg = LBX_Data_Seg;
        Bytes_To_Copy = 32768;
        while ( Entry_Size_Bytes >= 32768 )
        {
            Entry_Size_Bytes = Entry_Size_Bytes - 32768;
            tmpBytesRead = lbx_read_sgmt(Load_To_Seg, Bytes_To_Copy, g_LBX_FileHandle);
            if (tmpBytesRead == 0 )
            {
                printf("DEBUG: Error: [%s, %d]\n", __FILE__, __LINE__);
                LBX_Error(LbxName, 0x02, LbxEntryIndex, tmpMemBlkFreeParas);  // LBXErr_corrupted
            }
            Load_To_Seg = Load_To_Seg + 2048;  // 2048 paragraphs = 2048 * 16 = 32768
        }
        if (Entry_Size_Bytes > 0)
        {
            Bytes_To_Copy = Entry_Size_Bytes;
            tmpBytesRead = lbx_read_sgmt(Load_To_Seg, Bytes_To_Copy, g_LBX_FileHandle);
            if (tmpBytesRead == 0 )
            {
                printf("DEBUG: Error: [%s, %d]\n", __FILE__, __LINE__);
                LBX_Error(LbxName, 0x02, LbxEntryIndex, tmpMemBlkFreeParas);  // LBXErr_corrupted
            }
        }

        /*
            END: Read Entry Data
        */

    }

    Update_MemFreeWorst_KB();

    printf("DEBUG: END: LBX_EntryLoader()\n");

    return LBX_Data_Seg;
}
