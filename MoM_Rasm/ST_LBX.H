#ifndef ST_LBX_H
#define ST_LBX_H

// #include "ST_HEAD.H"


/*

    #define
    function pointer
    Class - overloaded functions
    Class - inline
    intrinsics?
    ?!? #ifdef __FILE__ ?!?

    How/If
        Entry vs. Record
            LBXREADDATA
                lbx_read_sgmt() OR EMM_MapnRead()
        
        LBX_Load vs. EMM_LBX_Load
            LBXENTRYOFFSET
                farpeekdw() OR EMM_MapnRead()
            LBXLOADTYE
                LBX_Error() OR return 0
*/

#define LBXERR01(name,entry) {LBX_Error(name, 0x01, entry);}  /* LBXErr_not_found */
#define LBXERR02(name,entry) {LBX_Error(name, 0x02, entry);}  /* LBXErr_corrupted */
// ...
#define LBXERR07(name,entry) {LBX_Error(name, 0x07, entry);}  /* LBXErr_bad_header */
#define LBXERR08(name,entry) {LBX_Error(name, 0x08, entry);}  /* LBXErr_entries_exceeded */
// ...


#define LBXTESTNEGATIVEENTRY() { \
    if ( !(tmp_LbxEntry >= 0) ) \
    { \
        LBX_Error(tmp_LbxName, 0x01, tmp_LbxEntry);  /* LBXErr_not_found */ \
    } \
}

#define LBXHEADER() { \
    if( g_LBX_Header_Allocd == 0 ) \
    { \
        g_LBX_Header_Allocd = 1; \
        gsa_LBX_Header = SA_Allocate_MemBlk(SZ_LBX_HDR_PR); \
    } \
}

#define LBXHEADEROFFSET() { \
    if ( LbxHdrFmt == 0 ) \
    { \
        tmp_LbxHdrFmt = 0; \
    } \
    else \
    { \
        if ( UU_g_LBX_HdrFmt != 2 ) \
        { \
            tmp_LbxHdrFmt = 1; \
        } \
        else \
        { \
            tmp_LbxHdrFmt = 0; \
        } \
    } \
}

#define EMMLBXHANDLE() { \
    EmmHndl = EMM_CheckHandleOpen(EmmHndlNm); \
    if ( EmmHndl == ST_FAILURE ) \
    { \
        goto Exit; \
    } \
}

#define EMMLBXENTRYCOUNT() { \
    EMM_MapnRead(LbxEntryCount, 0, 0, 0, 2, EmmHndl); \
    if ( LbxEntryCount < LbxEntry ) \
    { \
        goto Error; \
    } \
}
/*
    BEGIN: Current vs. Previous
*/
/* Yay/Nay include TEST-EntryCount? */
/*
    END: Current vs. Previous
*/

/*
    BEGIN: Entry - Offset Begin, End, Size
*/
/* "DataSize" `:` "EntrySize" gets reused as "RecordsSize" */
/* "DataOffset" `:` 'EMS' != "File" */
#define LBXENTRYOFFSET() { \
        EntryTableOffset = ((4 * LbxEntry) + 8); \
        DataOffset_Begin = farpeekdw(gsa_LBX_Header, EntryTableOffset); \
        DataOffset_End = farpeekdw(gsa_LBX_Header, EntryTableOffset + 4); \
        DataSize_Bytes = DataOffset_End - DataOffset_Begin; \
        if ( lbx_seek(DataOffset_Begin, g_LBX_FileHandle) == ST_FAILURE ) \
        { \
            LBX_Error(tmp_LbxName, 0x02, tmp_LbxEntry);  /* LBXErr_corrupted */ \
        } \
}
#define EMMLBXENTRYOFFSET() { \
    EntryTableOffset = ((4 * LbxEntry) + 8); \
    \
    tmp_EntryTableOffset_Sgmt = (unsigned int)(EntryTableOffset >> 16); \
    tmp_EntryTableOffset_Ofst = (unsigned int)(EntryTableOffset & 0x0000FFFF); \
    EMM_MapnRead((unsigned int)&DataOffset_Begin, 0, tmp_EntryTableOffset_Ofst, tmp_EntryTableOffset_Sgmt, 4, EmmHndl); \
    tmp_EntryTableOffset_Sgmt = (unsigned int)((EntryTableOffset+4) >> 16); \
    tmp_EntryTableOffset_Ofst = (unsigned int)((EntryTableOffset+4) & 0x0000FFFF); \
    EMM_MapnRead((unsigned int)&DataOffset_End, 0, tmp_EntryTableOffset_Ofst, tmp_EntryTableOffset_Sgmt, 4, EmmHndl); \
    DataSize_Bytes = DataOffset_End - DataOffset_Begin; \
    if ( !(DataSize_Bytes > 0) ) \
    { \
        goto Exit; \
    } \
}
/*
    END: Entry - Offset Begin, End, Size
*/

/*
    BEGIN: Records - Offset Begin, Size
*/
#define LBXRECORDOFFSET() { \
        lbx_read_ofst((unsigned int)&Record_Count, 2, g_LBX_FileHandle); \
        lbx_read_ofst((unsigned int)&Record_Size, 2, g_LBX_FileHandle); \
        if ( Record_Size ==  0 ) \
        { \
            LBX_Error(g_LBX_Name, 0x09, LbxEntry);  /* LBXErr_recsize_mismatch */ \
        } \
        if ( (RecSize - Record_Size) !=  0 ) \
        { \
            LBX_Error(g_LBX_Name, 0x09, LbxEntry);  /* LBXErr_recsize_mismatch */ \
        }         \
        if ( !(Record_Count <= (RecFirst + RecCount)) ) \
        { \
            LBX_Error(g_LBX_Name, 0x0A, LbxEntry);  /* LBXErr_records_exceeded */ \
        } \
        DataOffset_Begin = 4 + (Record_Size * RecFirst); \
        if ( lbx_seek(DataOffset_Begin, g_LBX_FileHandle) == ST_FAILURE ) \
        { \
            LBX_Error(tmp_LbxName, 0x02, tmp_LbxEntry);  /* LBXErr_corrupted */ \
        } \
        DataSize_Bytes = (Record_Size * RecCount); \
}
#define EMMLBXRECORDOFFSET() { \
    tmp_DataOffsetBegin_Sgmt = (unsigned int)(DataOffset_Begin >> 16); \
    tmp_DataOffsetBegin_Ofst = (unsigned int)(DataOffset_Begin & 0x0000FFFF); \
    EMM_MapnRead((unsigned int)&Record_Count, 0, tmp_DataOffsetBegin_Ofst, tmp_DataOffsetBegin_Sgmt, 2, EmmHndl); \
    tmp_DataOffsetBegin_Sgmt = (unsigned int)((DataOffset_Begin+2) >> 16); \
    tmp_DataOffsetBegin_Ofst = (unsigned int)((DataOffset_Begin+2) & 0x0000FFFF); \
    EMM_MapnRead((unsigned int)&Record_Size, 0, tmp_DataOffsetBegin_Ofst, tmp_DataOffsetBegin_Sgmt, 2, EmmHndl); \
    if ( Record_Size ==  0 ) \
    { \
        goto Error; \
    } \
    if ( (RecSize - Record_Size) !=  0 ) \
    { \
        goto Error; \
    }         \
    if ( !(Record_Count <= (RecFirst + RecCount)) ) \
    { \
        goto Error; \
    } \
    DataOffset_Begin = ((Record_Size * RecFirst) + 4); \
    DataSize_Bytes = (Record_Size * RecCount); \
}
/*
    END: Records - Offset Begin, Size
*/

/*
    BEGIN: Load Type
*/
#define LBXLOADTYPE0() { \
    SAMB_data = SA_Allocate_MemBlk(DataSize_Paras); \
    if ( SAMB_data == 0 ) \
    { \
        Lbx_Error(LbxName, 0x03, LbxEntry); /* LBXErr_low_RAM */ \
    } \
}
/* DataSize_Paras is different (only) in EMM_LBX_Load_Entry */
/* tmp_SAMB_data is different in EMM_LBXR_DirectLoader and EMM_LBX_HdrOnly */
#define LBXLOADTYPE() { \
        /*DataSize_Paras = 1 + (DataSize_Bytes / SZ_PARAGRAPH_B);*/ \
        if ( LoadType == 0 ) \
        { \
            SAMB_data = SA_Allocate_MemBlk(DataSize_Paras); \
            if ( SAMB_data == 0 ) { LBX_Error(LbxName, 0x03, LbxEntry); /* LBXErr_low_RAM */ } \
        } \
        if ( (LoadType == 1) || (LoadType == 2) ) \
        { \
            if ( LoadType == 1 ) \
            { \
                if ( SA_Alloc_Validate(SAMB_head) == 0 ) \
                { \
                    LBX_Error(LbxName, 0x02, LbxEntry); /* LBXErr_corrupted */ \
                } \
                if ( DataSize_Paras > (farpeekw(SAMB_head, SAMB_SIZE) - 1) ) \
                { \
                    LBX_Error(LbxName, 0x04, LbxEntry, (DataSize_Paras - farpeekw(SAMB_head, SAMB_SIZE) + 1) ); /* LBXErr_alloc_short */ \
                } \
                SAMB_data = SAMB_head + 1; \
                tmp_SAMB_Size = DataSize_Paras + 1; \
            } \
            if ( LoadType == 2 ) \
            { \
                if ( SA_Alloc_Validate(SAMB_head) == 0 ) \
                { \
                    LBX_Error(LbxName, 0x02, LbxEntry); /* LBXErr_corrupted */ \
                } \
                if ( DataSize_Paras > SA_GetFree(SAMB_head) ) \
                { \
                    LBX_Error(LbxName, 0x05, LbxEntry, (DataSize_Paras - SA_GetFree(SAMB_head)) ); /* LBXErr_reload_fail */ \
                } \
                SAMB_data = SAMB_head + farpeekw(SAMB_head, SAMB_USED); \
                tmp_SAMB_Size = DataSize_Paras + farpeekw(SAMB_head, SAMB_USED); \
            } \
            farpokew(SAMB_head, SAMB_USED, tmp_SAMB_Size); \
        } \
        /*tmp_SAMB_data = SAMB_data;*/ \
}
#define EMMLBXLOADTYPE() { \
        /*DataSize_Paras = 1 + (DataSize_Bytes / SZ_PARAGRAPH_B);*/ \
        if ( LoadType == 0 ) \
        { \
            SAMB_data = SA_Allocate_MemBlk(DataSize_Paras); \
            if ( SAMB_data == 0 ) { return 0; } \
        } \
        if ( (LoadType == 1) || (LoadType == 2) ) \
        { \
            if ( LoadType == 1 ) \
            { \
                if ( SA_Alloc_Validate(SAMB_head) == 0 ) { return 0; } \
                if ( DataSize_Paras > (farpeekw(SAMB_head, SAMB_SIZE) - 1) ) \
                { \
                    return 0; \
                } \
                SAMB_data = SAMB_head + 1; \
                tmp_SAMB_Size = DataSize_Paras + 1; \
            } \
            if ( LoadType == 2 ) \
            { \
                if ( SA_Alloc_Validate(( (SAMB_head != 0) ? 0 : 1 )) == 0 ) { return 0; } \
                if ( DataSize_Paras > SA_GetFree(SAMB_head) ) \
                { \
                    return 0; \
                } \
                SAMB_data = SAMB_head + farpeekw(SAMB_head, SAMB_USED); \
                tmp_SAMB_Size = DataSize_Paras + farpeekw(SAMB_head, SAMB_USED); \
            } \
            farpokew(SAMB_head, SAMB_USED, tmp_SAMB_Size); \
        } \
        /*tmp_SAMB_data = SAMB_data;*/ \
}
/*
    END: Load Type
*/

/*
    BEGIN: Read Data
*/
/* 'Read Data' is different in EMM_LBXR_DirectLoader and EMM_LBX_HdrOnly
   EMM_LBXR_DirectLoader looks to be the same code as ReadChunkRemainder
   Also, reads directly to SAMB_data, not tmp_SAMB_data */
#define LBXREADSGMT() { \
    if ( lbx_read_sgmt(tmp_SAMB_data, ReadNbytes, g_LBX_FileHandle) == ST_FAILURE ) \
    { \
        LBX_Error(LbxName, 0x02, LbxEntry);  /* LBXErr_corrupted */ \
    } \
}
#define LBXREADDATA() { \
        ReadNbytes = SZ_32K_B; \
        while ( DataSize_Bytes >= SZ_32K_B ) \
        { \
            DataSize_Bytes = DataSize_Bytes - SZ_32K_B; \
            if ( lbx_read_sgmt(tmp_SAMB_data, ReadNbytes, g_LBX_FileHandle) == ST_FAILURE ) \
            { \
                LBX_Error(LbxName, 0x02, LbxEntry);  /* LBXErr_corrupted */ \
            } \
            tmp_SAMB_data = tmp_SAMB_data + SZ_32K_PR; \
        } \
        if ( DataSize_Bytes > 0 ) \
        { \
            ReadNbytes = DataSize_Bytes; \
            if ( lbx_read_sgmt(tmp_SAMB_data, ReadNbytes, g_LBX_FileHandle) == ST_FAILURE ) \
            { \
                LBX_Error(LbxName, 0x02, LbxEntry);  /* LBXErr_corrupted */ \
            } \
        } \
}
#define EMMLBXREADDATA() { \
    ReadNbytes = SZ_32K_B; \
    while ( DataSize_Bytes >= SZ_32K_B ) \
    { \
        DataSize_Bytes = DataSize_Bytes - SZ_32K_B; \
        tmp_DataOffsetBegin_Sgmt = (unsigned int)(DataOffset_Begin >> 16); \
        tmp_DataOffsetBegin_Ofst = (unsigned int)(DataOffset_Begin & 0x0000FFFF); \
        EMM_MapnRead(0, tmp_SAMB_data, tmp_DataOffsetBegin_Sgmt, tmp_DataOffsetBegin_Ofst, ReadNbytes, EmmHndl); \
        DataOffset_Begin = DataOffset_Begin + SZ_32K_B; \
        tmp_SAMB_data = tmp_SAMB_data + SZ_32K_PR; \
    } \
    if ( DataSize_Bytes > 0 ) \
    { \
        ReadNbytes = DataSize_Bytes % SZ_32K_B; \
        tmp_DataOffsetBegin_Sgmt = (unsigned int)(DataOffset_Begin >> 16); \
        tmp_DataOffsetBegin_Ofst = (unsigned int)(DataOffset_Begin & 0x0000FFFF); \
        EMM_MapnRead(0, tmp_SAMB_data, tmp_DataOffsetBegin_Sgmt, tmp_DataOffsetBegin_Ofst, ReadNbytes, EmmHndl); \
    } \
}
/*
    END: Read Data
*/



// #define DOS_UnusedFileHandle 0xFFFF  // Warning: Constant out of range in comparison ...
#define DOS_UnusedFileHandle -1
#define ST_NULL 0

#define SZ_LBX_HDR_B 512
#define SZ_LBX_HDR_PR 32
#define SZ_LBX_HDR_PRE_B 8
#define SZ_LBX_HDR_OFST_B 4
#define SZ_16K_B 16384
#define SZ_32K_B 32768
#define SZ_32K_PR 2048
#define SZ_PARAGRAPH_B 16

#define LBX_MAGSIG 0xFEAD

#define LBX_TYPE_PICTURES 0


// LBX Globals - Initialized
extern unsigned int g_LBX_Header_Allocd;        // dseg:3E66
extern int g_LBX_FileHandle;                    // dseg:3E68
extern unsigned int UU_g_LBX_HdrFmtOvrRd;       // dseg:3E6A
extern char UU_g_LBX_FilePath[];                // dseg:3E6C
extern char g_LBX_FileExtension[];              // dseg:3E9E
// LBX Globals - Unitialized
extern unsigned int g_LBX_EntryCount;           // dseg:A5C6
extern unsigned int UU_g_LBX_HdrFmt;            // dseg:A5C8
extern unsigned int gsa_LBX_Header;             // dseg:A5CA
extern char g_LBX_Name[];                       // dseg:A5CC

// LBX Error (initialized)
extern char *cnst_LBXErr_Common1;               //dseg:3EA3
extern char *cnst_LBXErr_Common2;               //dseg:3EB0
extern char *cnst_LBX_Error1;                   //dseg:3EB3
extern char *cnst_LBX_Error2;                   //dseg:3EC8
extern char *cnst_LBX_Error31;                  //dseg:3EDD
extern char *cnst_LBX_Error32;                  //dseg:3F05
extern char *cnst_LBX_Error4;                   //dseg:3F25
extern char *cnst_LBX_Error51;                  //dseg:3F58
extern char *cnst_LBX_Error52;                  //dseg:3F84
extern char *cnst_LBX_Error7;                   //dseg:3F8B
extern char *cnst_LBX_Error8;                   //dseg:3F9F
extern char *cnst_LBX_Error9;                   //dseg:3FBE
extern char *cnst_LBX_ErrorA;                   //dseg:3FDC
extern char *cnst_LBX_ErrorB;                   //dseg:3FFF
extern char *cnst_LBX_ErrorC;                   //dseg:4036
extern char *cnst_LBX_ErrorD;                   //dseg:405C
extern char *cnst_LBX_ErrorE;                   //dseg:408B
extern char *cnst_LBX_ErrorF1;                  //dseg:409C
extern char *cnst_LBX_ErrorG;                   //dseg:40A4
// dseg:40D9 align 2


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg009
*/
int  lbx_open(char *fname);                                     // _s09p01.asm  DISK_OpenFile
int  lbx_close(int fhandle);                                    // _s09p02.asm  DISK_CloseFile
long lbx_seek(long foffset, int fhandle);                       // _s09p03.asm  DISK_LSeek
long lbx_size(int fhandle);                                     // _s0904.asm   UU_DISK_GetFileSize
int  lbx_read_sgmt(unsigned int sgmt_addr, int nbytes, int fhandle);   // _s09p05.asm  DISK_ReadToSeg
//int  lbx_read_sgmt(void _seg * sgmt_ptr, int nbytes, int fhandle);   // _s09p05.asm  DISK_ReadToSeg
int  lbx_read_ofst(unsigned int buffer, int nbytes, int fhandle);   // _s09p06.asm  DISK_ReadToData
void strcpyfar(unsigned int dstoff, unsigned int dstseg, unsigned int srcoff, unsigned int srcseg); //_s09p07.asm STR_CopyFar

/*
    seg010
*/
// _s10p01
unsigned int LBXE_LoadSingle(char *LbxName, int LbxEntryIndex);                                                
// _s10p02
unsigned int LBXE_LoadReplace(char *LbxName, int LbxEntryIndex, unsigned int SAMB_head);
// _s10p03
unsigned int LBXE_LoadAppend(char *LbxName, int LbxEntryIndex, unsigned int SAMB_head);
// _s10p04c.c
unsigned int LBXR_LoadSingle(char *LbxName, int LbxEntryIndex, int RecFirst, int RecCount, int RecSize);

// _s10p10c.c
unsigned int LBX_Load_Entry(char *LbxName, int LbxEntryIndex, unsigned int SAMB_head, int LoadType, int LbxHdrFmt);
// _s10p11c.c
unsigned int LBX_Load_Record(char *LbxName, int LbxEntryIndex, unsigned int SAMB_head, int LoadType, int RecFirst, int RecCount, int RecSize);
// _s10p12c.c
//unsigned int LBXR_DirectLoader(char *LbxName, int LbxEntry, unsigned int SAMB_head, int RecFirst, int RecCount, int RecSize)
unsigned int LBXR_DirectLoader(char *LbxName, int LbxEntry, unsigned int SAMB_data, int RecFirst, int RecCount, int RecSize);
// _s10p13c.c
unsigned int LBX_GetEntryData(char *LbxName, int LbxEntry, unsigned long *LbxEntryOffset, unsigned long *LbxEntrySize, int LbxHdrFmt);

// _s10p16.asm
void ExtractFileBase(char *LbxFileName);
// _s10p17c.c
void RAM_SetMinKB(int RAM_MinKB);


#ifdef __cplusplus
}
#endif

#endif /* ST_LBX_H */
