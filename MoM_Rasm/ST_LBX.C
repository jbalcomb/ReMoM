
#include "ST_LBX.H"

#include "MoX_TYPE.H"
#include "ST_DEF.H"

#include "ST_EMM.H"
#include "ST_EXIT.H"    /* Update_MemFreeWorst_KB() */
#include "ST_SA.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

// J:\STU\DBWD\BORLANDC\INCLUDE\
#include <stdio.h>      /* FILE */
#include <fcntl.h>      /* O_BINARY, O_RDONLY */
#include<sys\stat.h>    /* ? */
#include <IO.H>         /* filelength(), lseek(), read(); SEEK_CUR, SEEK_END, SEEK_SET */
    // SEEK_SET (0)  File beginning; SEEK_CUR (1)  Current file pointer; SEEK_END (2)  End-of-file
//#ifdef STU_DEBUG
    #include <errno.h>
//#endif


/*
    LBX Globals - Initialized
*/
unsigned int g_LBX_Header_Allocd = 0;           // dseg:3E66
int g_LBX_FileHandle = DOS_UnusedFileHandle;    // dseg:3E68  DOS Unused File Handle  == -1
unsigned int UU_g_LBX_HdrFmtOvrRd = 0;            // dseg:3E6A
char UU_g_LBX_FilePath[50] = {0};               // dseg:3E6C
char LBX_File_Extension[] = ".LBX";            // dseg:3E9E

char * cnst_LBXErr_Common1 = ".LBX [entry ";                                         // MGC dseg:3EA3
char * cnst_LBXErr_Common2 = "] ";                                                   // MGC dseg:3EB0
char * cnst_LBX_Error1 = " could not be found.";                                     // MGC dseg:3EB3
char * cnst_LBX_Error2 = " has been corrupted.";                                     // MGC dseg:3EC8
char * cnst_LBX_Error31 = "Insufficient memory. You need at least ";                 // MGC dseg:3EDD
char * cnst_LBX_Error32 = "K free. Try removing all TSR's.";                         // MGC dseg:3F05
char * cnst_LBX_Error4 = " was not properly allocated or has been corrupted.";       // MGC dseg:3F25
char * cnst_LBX_Error51 = " failed to reload. Allocation too small by ";             // MGC dseg:3F58
char * cnst_LBX_Error52 = " pages";                                                  // MGC dseg:3F84
char * cnst_LBX_Error7 = " is not an LBX file";                                      // MGC dseg:3F8B
char * cnst_LBX_Error8 = " exceeds number of LBX entries";                           // MGC dseg:3F9F
char * cnst_LBX_Error9 = " has an incorrect record size";                            // MGC dseg:3FBE
char * cnst_LBX_ErrorA = " exceeds number of defined records";                       // MGC dseg:3FDC
char * cnst_LBX_ErrorB = " cannot be reloaded into EMS w/o being first released.";   // MGC dseg:3FFF
char * cnst_LBX_ErrorC = " EMM loading error. Insufficient EMM.";                    // MGC dseg:4036
char * cnst_LBX_ErrorD = " Only pictures may be loaded into reserved EMM";           // MGC dseg:405C
char * cnst_LBX_ErrorE = " (Reserved EMM) ";                                         // MGC dseg:408B
char * cnst_LBX_ErrorF1 = " LBX to";                                                 // MGC dseg:409C
char * cnst_LBX_ErrorG = " Vga file animation frames cannot exceed 65536 bytes";     // MGC dseg:40A4
/*
    LBX Globals - Unitialized
*/
unsigned int RAM_Min_KB;                    // MGC dseg:A5C4  ; set to 583 in _main
MoX_word MoX_RAM_Min_KB;
unsigned int g_LBX_EntryCount;              // MGC dseg:A5C6
unsigned int UU_g_LBX_HdrFmt;               // MGC dseg:A5C8
SAMB_addr gsa_LBX_Header;                   // MGC dseg:A5CA
char g_LBX_Name[16];                        // MGC dseg:A5CC
unsigned int g_LBX_EmmRsvd;                 // MGC dseg:A5EC



/*
    MGC  seg009
*/

// _s09p01
int lbx_open(char *fname)
{
    int fhandle;

    fhandle = open(fname, O_RDONLY|O_BINARY);

    if (fhandle == -1)
    {
        fhandle = 0;
    }

    return fhandle;
}

// _s09p02
int lbx_close(int fhandle)
{
    return close(fhandle);  // returns SUCCESS: 0, FAILURE: -1
}

// _s09p03
int lbx_seek(long foffset, int fhandle)
{
    int st_status;

    if ( lseek(fhandle, foffset, SEEK_SET) == -1L )  //  long lseek(int handle, long offset, int fromwhere);  returns FAILURE: -1L
    {
        st_status = 0;  // ST_FAILURE
    }
    else
    {
        st_status = -1;  // ST_SUCCESS
    }

    return st_status;
}

// _s09p04
long lbx_size(int fhandle)
{
    long foffset;

    foffset = lseek(fhandle, 0, SEEK_END);

    if ( foffset == -1L )
    {
        foffset = 0;  // ST_FAILURE
    }

    return foffset;
}

// _s09p05
int lbx_read_sgmt(unsigned int dst_sgmt, int nbytes, int fhandle)
{
    void * buf;
    int st_status;

    buf = MK_FP(dst_sgmt, 0);

    if ( read(fhandle, buf, nbytes) == -1 )
    {
        st_status = 0;  // ST_FAILURE
    }
    else
    {
        st_status = -1;  // ST_SUCCESS
    }

    return st_status;
}

// _s09p06
int lbx_read_ofst(unsigned int dst_ofst, int nbytes, int fhandle)
{
    void * buf;
    int st_status;

    buf = MK_FP(_DS, dst_ofst);  // without this FP, it gets the right bytes but does not assign them properly ? because all pointers are FAR in LARGE memory model ? override NEAR ?

    if ( read(fhandle, buf, nbytes) == -1 )
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
int lbx_read(void _FAR * fpDst, int nbytes, int fhandle)
{
    int st_status;
//#ifdef STU_DEBUG
    int baitos;
    char * strerrbuf;
    int itr_nbytes;
//#endif

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: BEGIN: lbx_read(fpDst = 0x%p, nbytes = %d, fhandle = %d)\n", __FILE__, __LINE__, fpDst, nbytes, fhandle);
// #endif

    if ( (baitos = read(fhandle, fpDst, nbytes)) == -1 )
    {

// #ifdef STU_DEBUG
//         strerrbuf = strerror(errno);
//         dlvfprintf("DEBUG: [%s, %d]: strerrbuf: %s\n", __FILE__, __LINE__, strerrbuf);
// #endif

        st_status = 0;  // ST_FAILURE
    }
    else
    {

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: baitos: %d\n", __FILE__, __LINE__, baitos);
// #endif

// #ifdef STU_DEBUG
//     for ( itr_nbytes = 0; itr_nbytes < nbytes; itr_nbytes++ )
//     {
//         dlvfprintf("DEBUG: [%s, %d]: buf[%d]: 0x%02X\n", __FILE__, __LINE__, itr_nbytes, *((unsigned char *)fpDst + itr_nbytes));
//     }
// #endif

        st_status = -1;  // ST_SUCCESS
    }

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: END: lbx_read(fpDst = 0x%p, nbytes = %d, fhandle = %d) { st_status = %d }\n", __FILE__, __LINE__, fpDst, nbytes, fhandle, st_status);
// #endif

    return st_status;
}

// _s09p07
//void strcpy(char _FAR * dst, const char _FAR * src)
void strcpyfar(unsigned int dst_ofst, unsigned int dst_sgmt, unsigned int src_ofst, unsigned int src_sgmt)
{
    char * fp_dst;
    char * fp_src;
    int itr_str;

    if ( dst_sgmt == 0 )
    {
        dst_sgmt = _DS;
    }

    if ( src_sgmt == 0 )
    {
        src_sgmt = _DS;
    }

    fp_dst = MK_FP(dst_sgmt, dst_ofst);
    fp_src = MK_FP(src_sgmt, src_ofst);

    itr_str = 0;
    while( fp_src[itr_str] != '\0' )
    {
        *fp_dst++ = *fp_src++;
        itr_str++;
    }
}



/*
    MGC  seg010
*/


// _s10p01
SAMB_addr LBXE_LoadSingle(char *LbxName, int LbxEntryIndex)
{
    SAMB_addr SAMB_head;
    int LoadType;
    int LbxHdrFmt;
    SAMB_addr SAMB_data;

    SAMB_head = 0;
    LoadType = 0;
    LbxHdrFmt = 0;

    SAMB_data = LBX_Load_Entry(LbxName, LbxEntryIndex, SAMB_head, LoadType, LbxHdrFmt);

    return SAMB_data;
}
SAMB_ptr LBXE_LoadSingle_LM(char *LbxName, int LbxEntryIndex)
{
    SAMB_ptr SAMB_head;
    int LoadType;
    int LbxHdrFmt;
    SAMB_ptr SAMB_data;

    SAMB_head = 0;
    LoadType = 0;
    LbxHdrFmt = 0;

    SAMB_data = (SAMB_ptr) MK_FP( LBX_Load_Entry(LbxName, LbxEntryIndex, (SAMB_addr)FP_SEG(SAMB_head), LoadType, LbxHdrFmt), 0 );

    return SAMB_data;
}


// _s10p02
SAMB_addr LBXE_LoadReplace(char *LbxName, int LbxEntryIndex, SAMB_addr SAMB_head)
{
    int LoadType;
    int LbxHdrFmt;
    SAMB_addr SAMB_data;

    LoadType = 1;
    LbxHdrFmt = 0;

    SAMB_data = LBX_Load_Entry(LbxName, LbxEntryIndex, SAMB_head, LoadType, LbxHdrFmt);

    return SAMB_data;
}
SAMB_ptr LBXE_LoadReplace_LM(char *LbxName, int LbxEntryIndex, SAMB_ptr SAMB_head)
{
    int LoadType;
    int LbxHdrFmt;
    SAMB_ptr SAMB_data;

    LoadType = 1;
    LbxHdrFmt = 0;

    SAMB_data = (SAMB_ptr)MK_FP(LBX_Load_Entry(LbxName, LbxEntryIndex, (SAMB_addr)FP_SEG(SAMB_head), LoadType, LbxHdrFmt),0);

    return SAMB_data;
}


// _s10p03
SAMB_addr LBXE_LoadAppend(char *LbxName, int LbxEntryIndex, SAMB_addr SAMB_head)
{
    int LoadType;
    int LbxHdrFmt;
    SAMB_addr SAMB_data;

    LoadType = 2;
    LbxHdrFmt = 0;

    SAMB_data = LBX_Load_Entry(LbxName, LbxEntryIndex, SAMB_head, LoadType, LbxHdrFmt);

    return SAMB_data;
}
SAMB_ptr LBXE_LoadAppend_LM(char *LbxName, int LbxEntryIndex, SAMB_ptr SAMB_head)
{
    int LoadType;
    int LbxHdrFmt;
    SAMB_ptr SAMB_data;

    LoadType = 2;
    LbxHdrFmt = 0;

    SAMB_data = (SAMB_ptr)MK_FP(LBX_Load_Entry(LbxName, LbxEntryIndex, (SAMB_addr)FP_SEG(SAMB_head), LoadType, LbxHdrFmt),0);

    return SAMB_data;
}


// _s10p04
SAMB_addr LBXR_LoadSingle(char *LbxName, int LbxEntryIndex, int RecFirst, int RecCount, int RecSize)
{
    SAMB_addr SAMB_head;
    int LoadType;
    SAMB_addr SAMB_data;

    SAMB_head = 0;
    LoadType = 0;

    SAMB_data = LBX_Load_Record(LbxName, LbxEntryIndex, SAMB_head, LoadType, RecFirst, RecCount, RecSize);

    return SAMB_data;
}
SAMB_ptr LBXR_LoadSingle_LM(char *LbxName, int LbxEntryIndex, int RecFirst, int RecCount, int RecSize)
{
    SAMB_ptr SAMB_head;
    int LoadType;
    SAMB_ptr SAMB_data;

    SAMB_head = 0;
    LoadType = 0;

    SAMB_data = (SAMB_ptr)MK_FP(LBX_Load_Record(LbxName, LbxEntryIndex, (SAMB_addr)FP_SEG(SAMB_head), LoadType, RecFirst, RecCount, RecSize),0);

    return SAMB_data;
}


// _s10p05
SAMB_addr LBXR_LoadReplace(char *LbxName, int LbxEntryIndex, SAMB_addr SAMB_head, int RecFirst, int RecCount, int RecSize)
{
    int LoadType;
    SAMB_addr SAMB_data;

    LoadType = 1;

    SAMB_data = LBX_Load_Record(LbxName, LbxEntryIndex, SAMB_head, LoadType, RecFirst, RecCount, RecSize);

    return SAMB_data;
}
SAMB_ptr LBXR_LoadReplace_LM(char *LbxName, int LbxEntryIndex, SAMB_ptr SAMB_head, int RecFirst, int RecCount, int RecSize)
{
    int LoadType;
    SAMB_ptr SAMB_data;

    LoadType = 1;

    SAMB_data = (SAMB_ptr)MK_FP(LBX_Load_Record(LbxName, LbxEntryIndex, (SAMB_addr)FP_SEG(SAMB_head), LoadType, RecFirst, RecCount, RecSize),0);

    return SAMB_data;
}


// _s10p06
SAMB_addr LBXR_LoadAppend(char *LbxName, int LbxEntryIndex, SAMB_addr SAMB_head, int RecFirst, int RecCount, int RecSize)
{
    int LoadType;
    SAMB_addr SAMB_data;

    LoadType = 2;

    SAMB_data = LBX_Load_Record(LbxName, LbxEntryIndex, SAMB_head, LoadType, RecFirst, RecCount, RecSize);

    return SAMB_data;
}
SAMB_ptr LBXR_LoadAppend_LM(char *LbxName, int LbxEntryIndex, SAMB_ptr SAMB_head, int RecFirst, int RecCount, int RecSize)
{
    int LoadType;
    SAMB_ptr SAMB_data;

    LoadType = 2;

    SAMB_data = (SAMB_ptr)MK_FP(LBX_Load_Record(LbxName, LbxEntryIndex, (SAMB_addr)FP_SEG(SAMB_head), LoadType, RecFirst, RecCount, RecSize),0);

    return SAMB_data;
}


// UU // _s10p07
// UU SAMB_addr UU_LBXE_LoadSingle2(char *LbxName, int LbxEntryIndex)
// UU {
// UU     SAMB_addr SAMB_head;
// UU     int LoadType;
// UU     int LbxHdrFmt;
// UU     SAMB_addr SAMB_data;
// UU 
// UU     SAMB_head = 0;
// UU     LoadType = 0;
// UU     LbxHdrFmt = 1;
// UU 
// UU     SAMB_data = LBX_Load_Entry(LbxName, LbxEntryIndex, SAMB_head, LoadType, LbxHdrFmt);
// UU 
// UU     return SAMB_data;
// UU }
// UU 
// UU 
// UU // _s10p08
// UU SAMB_addr UU_LBXE_LoadReplace2(char *LbxName, int LbxEntryIndex, SAMB_addr SAMB_head)
// UU {
// UU     int LoadType;
// UU     int LbxHdrFmt;
// UU     SAMB_addr SAMB_data;
// UU 
// UU     LoadType = 1;
// UU     LbxHdrFmt = 1;
// UU 
// UU     SAMB_data = LBX_Load_Entry(LbxName, LbxEntryIndex, SAMB_head, LoadType, LbxHdrFmt);
// UU 
// UU     return SAMB_data;
// UU }
// UU 
// UU 
// UU // _s10p09
// UU SAMB_addr UU_LBXE_LoadAppend2(char *LbxName, int LbxEntryIndex, SAMB_addr SAMB_head)
// UU {
// UU     int LoadType;
// UU     int LbxHdrFmt;
// UU     SAMB_addr SAMB_data;
// UU 
// UU     LoadType = 2;
// UU     LbxHdrFmt = 1;
// UU 
// UU     SAMB_data = LBX_Load_Entry(LbxName, LbxEntryIndex, SAMB_head, LoadType, LbxHdrFmt);
// UU 
// UU     return SAMB_data;
// UU }


// _s10p10
SAMB_addr LBX_Load_Entry(char *LbxName, int LbxEntry, SAMB_addr SAMB_head, int LoadType, int LbxHdrFmt)
{
    char *tmp_LbxName;
    int tmp_LbxEntry;
    SAMB_addr SAMB_data;
    int tmp_LbxHdrFmt;
    char tmp_LbxFileName[20];
    char tmp_LbxFilePathName[60];
    unsigned long tmp_LbxHdrOfst;
    unsigned int EntryTableOffset;  // LBXENTRYOFFSET()
    unsigned long DataOffset_Begin; // LBXENTRYOFFSET()
    unsigned long DataOffset_End;   // LBXENTRYOFFSET()
    unsigned long DataSize_Bytes;   // LBXENTRYOFFSET()
    unsigned long DataSize_Paras;   // LBXLOADTYPE()
    unsigned int tmp_SAMB_Size;     // LBXLOADTYPE()
    SAMB_addr tmp_SAMB_data;        // LBXREADDATA()
    unsigned int ReadNbytes;        // LBXREADDATA()
    SAMB_ptr pSAMB_head;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: LBX_Load_Entry(LbxName=%s, LbxEntry=%d, SAMB_head=0x%04X, LoadType=%d, LbxHdrFmt=%d)\n", __FILE__, __LINE__, LbxName, LbxEntry, SAMB_head, LoadType, LbxHdrFmt);
#endif

// //     if
// //     (
// //         ((strcmp(LbxName, "FONTS.LBX") == 0) && (LbxEntry == 2)) ||
// //         ((strcmp(LbxName, "MAINSCRN") == 0) && (LbxEntry == 0))
// //     )
// //     {
// //         dlvfprintf("DEBUG: [%s, %d] BEGIN: LBX_Load_Entry(LbxName=%s, LbxEntry=%d, SAMB_head=0x%04X, LoadType=%d, LbxHdrFmt=%d)\n", __FILE__, __LINE__, LbxName, LbxEntry, SAMB_head, LoadType, LbxHdrFmt);
// //     }
// #endif

// #ifdef TEST
//     // DELETE //TST_LBX[Get_LBX_Name_Index(LbxName)];
//     // DELETE Populate_TST_LBX("LBX_Load_Entry", LbxName, LbxEntry, SAMB_head, LoadType, LbxHdrFmt);
// #endif

    tmp_LbxEntry = LbxEntry;
    tmp_LbxName = LbxName;
    
    LBXTESTNEGATIVEENTRY()

    // LBXHEADER()
    // if( g_LBX_Header_Allocd == 0 ) { g_LBX_Header_Allocd = 1; gsa_LBX_Header = FP_SGMT(SA_Allocate_MemBlk(SZ_LBX_HDR_PR)); }
    // if( g_LBX_Header_Allocd == 0 ) { g_LBX_Header_Allocd = 1; gsa_LBX_Header = FP_SGMT(SA_Allocate_MemBlk(SZ_LBX_HDR_PR)); }
    // if( g_LBX_Header_Allocd == 0 ) { g_LBX_Header_Allocd = 1; gsa_LBX_Header = (unsigned int) ( (unsigned long)((void _FAR *)(SA_Allocate_MemBlk(SZ_LBX_HDR_PR))) >> 16); }
    if( g_LBX_Header_Allocd == 0 ) { g_LBX_Header_Allocd = 1; gsa_LBX_Header = (unsigned int) ( (unsigned long)((void *)(SA_Allocate_MemBlk(SZ_LBX_HDR_PR))) >> 16); }
    


    ExtractFileBase(LbxName);

    LBXHEADEROFFSET()

    SAMB_data = EMM_LBX_Load_Entry(LbxName, LbxEntry, SAMB_head, LoadType, tmp_LbxHdrFmt);

    if ( SAMB_data == ST_FAILURE )
    {
        /*
            BEGIN: Current vs. Previous
        */
        if ( (g_LBX_FileHandle == -1) || (stricmp(LbxName, g_LBX_Name) != 0) || (tmp_LbxHdrFmt != UU_g_LBX_HdrFmt))
        {
            /*
                BEGIN: Current != Previous
            */
            //HERE("Curr. != Prev.");
            UU_g_LBX_HdrFmt = tmp_LbxHdrFmt;
            if ( g_LBX_FileHandle != -1 ) { lbx_close(g_LBX_FileHandle); }
            strcpy(g_LBX_Name, LbxName);
            strcpy(tmp_LbxFileName, LbxName);
            strcat(tmp_LbxFileName, LBX_File_Extension);
            g_LBX_FileHandle = lbx_open(tmp_LbxFileName);
            if ( (g_LBX_FileHandle == ST_FAILURE) && (UU_g_LBX_FilePath == ST_NULL) ) { LBX_Error(LbxName, 0x01, LbxEntry, NULL);  }
            else
            {
                strcpy(tmp_LbxFilePathName, UU_g_LBX_FilePath );
                strcat(tmp_LbxFilePathName, tmp_LbxFileName);
                g_LBX_FileHandle = lbx_open(tmp_LbxFilePathName);
                if ( g_LBX_FileHandle == ST_FAILURE ) { LBX_Error(LbxName, 0x01, LbxEntry, NULL);  }
            }

            /*
                BEGIN: LBX Header Offset
            */
            if ( UU_g_LBX_HdrFmt == 0 )
            {
                tmp_LbxHdrOfst = 0;
            }
            else
            {
                tmp_LbxHdrOfst = 512;
            }
            /*
                END: LBX Header Offset
            */

            if ( lbx_seek(tmp_LbxHdrOfst, g_LBX_FileHandle) == ST_FAILURE )
            {
                // HERE("LBX_Error: has been corrupted  ( lbx_seek(tmp_LbxHdrOfst, g_LBX_FileHandle) == ST_FAILURE )");
                LBX_Error(tmp_LbxName, 0x02, tmp_LbxEntry, NULL);  /* LBXErr_corrupted */
            }

            lbx_read_sgmt(gsa_LBX_Header, SZ_LBX_HDR_B, g_LBX_FileHandle);

            if ( farpeekw(gsa_LBX_Header, 2) != LBX_MAGSIG )
            {
                LBX_Error(tmp_LbxName, 0x07, tmp_LbxEntry, NULL);  /* LBXErr_bad_header */
            }

            g_LBX_EntryCount = farpeekw(gsa_LBX_Header, 0);
            
            /*
                END: Current != Previous
            */
        }
        else
        {
            //HERE("Curr. == Prev.");
        }

        // (g_LBX_EntryCount < LbxEntryIndex) ~== (!(LbxEntryIndex >= g_LBX_EntryCount)) ~== (!((LbxEntryIndex - g_LBX_EntryCount) < 0))
        if ( g_LBX_EntryCount < LbxEntry )
        {
            LBX_Error(tmp_LbxName, 0x08, tmp_LbxEntry, NULL); /* LBXErr_entries_exceeded */
        }
        /*
            END: Current vs. Previous
        */

        /*
            BEGIN: Entry - Offset Begin, End, Size
        */
        LBXENTRYOFFSET()
        /*
            END: Entry - Offset Begin, End, Size
        */

        /*
            BEGIN: Allocation Type
        */
        DataSize_Paras = 1 + (DataSize_Bytes / SZ_PARAGRAPH_B);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] DataSize_Paras: %lu\n", __FILE__, __LINE__, DataSize_Paras);
#endif
        LBXLOADTYPE()  // sets SAMB_data and tmp_SAMB_Size
        /*
            END: Allocation Type
        */
        tmp_SAMB_data = SAMB_data;
        /*
            BEGIN: Read Data
        */
        LBXREADDATA()
        /*
            END: Read Data
        */

    }  /* if ( SAMB_data == 0 ) */

    Update_MemFreeWorst_KB();

#ifdef STU_DEBUG    
    dbg_prn("DEBUG: [%s, %d] END: LBX_Load_Entry(LbxName=%s, LbxEntry=%d, SAMB_head=0x%04X, LoadType=%d, LbxHdrFmt=%d) { SAMB_data = 0x%04X }\n", __FILE__, __LINE__, LbxName, LbxEntry, SAMB_head, LoadType, LbxHdrFmt, SAMB_data);
#endif

    return SAMB_data;
}


// _s10p11
// ~= c&p LBX_Load_Entry()
SAMB_addr LBX_Load_Record(char *LbxName, int LbxEntry, SAMB_addr SAMB_head, int LoadType, int RecFirst, int RecCount, int RecSize)
{
    char *tmp_LbxName;
    int tmp_LbxEntry;
    SAMB_addr SAMB_data;
    int tmp_LbxHdrFmt;
    char tmp_LbxFileName[20];
    char tmp_LbxFilePathName[60];
    unsigned long tmp_LbxHdrOfst;
    // TODO(JimBalcomb): fixup the mix of static and unsigned longs
    unsigned int EntryTableOffset;  // LBXENTRYOFFSET()
    unsigned long DataOffset_Begin; // LBXENTRYOFFSET()
    unsigned long DataOffset_End;   // LBXENTRYOFFSET()
    unsigned long DataSize_Bytes;   // LBXENTRYOFFSET()
    static int Record_Count;        // LBXRECORDOFFSET()
    static int Record_Size;         // LBXRECORDOFFSET()
    unsigned long DataSize_Paras;   // LBXLOADTYPE()
    unsigned int tmp_SAMB_Size;     // LBXLOADTYPE()
    SAMB_addr tmp_SAMB_data;        // LBXREADDATA()
    unsigned int ReadNbytes;        // LBXREADDATA()
    SAMB_ptr pSAMB_head;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: LBX_Load_Record(LbxName = %s, LbxEntry = %d, SAMB_head = 0x%04X, LoadType = %d, RecFirst = %d, RecCount = %d, RecSize = %d)\n", __FILE__, __LINE__, LbxName, LbxEntry, SAMB_head, LoadType, RecFirst, RecCount, RecSize);
#endif

    tmp_LbxName = LbxName;
    tmp_LbxEntry = LbxEntry;

    LBXTESTNEGATIVEENTRY()

    LBXHEADER()

    ExtractFileBase(LbxName);

    SAMB_data = EMM_LBX_Load_Record(tmp_LbxName, tmp_LbxEntry, SAMB_head, LoadType, RecFirst, RecCount, RecSize);

    /*
        BEGIN: LBX Format Type / Header Offset
    */
    tmp_LbxHdrFmt = 0;
    /*
        END: LBX Format Type / Header Offset
    */

    if ( SAMB_data == ST_FAILURE )
    {
        // HERE("LBX not in EMM");

        /*
            BEGIN: Current vs. Previous
        */
        if ( (g_LBX_FileHandle == -1) || (stricmp(LbxName, g_LBX_Name) != 0) || (tmp_LbxHdrFmt != UU_g_LBX_HdrFmt))
        {
            /*
                BEGIN: Current != Previous
            */
//            printf("DEBUG: [%s, %d]: Curr. != Prev.\n", __FILE__, __LINE__);

            UU_g_LBX_HdrFmt = tmp_LbxHdrFmt;
            
            if ( g_LBX_FileHandle != -1 )
            {
                lbx_close(g_LBX_FileHandle);
            }

            strcpy(g_LBX_Name, LbxName);
            strcpy(tmp_LbxFileName, LbxName);
            strcat(tmp_LbxFileName, LBX_File_Extension);
            g_LBX_FileHandle = lbx_open(tmp_LbxFileName);
            
            if ( (g_LBX_FileHandle == ST_FAILURE) && (UU_g_LBX_FilePath == ST_NULL) )
            {
                LBX_Error(LbxName, 0x01, LbxEntry, NULL);  /* LBXErr_not_found */
            }
            else
            {
                strcpy(tmp_LbxFilePathName, UU_g_LBX_FilePath );
                strcat(tmp_LbxFilePathName, tmp_LbxFileName);
                g_LBX_FileHandle = lbx_open(tmp_LbxFilePathName);

                if ( g_LBX_FileHandle == ST_FAILURE )
                {
                    LBX_Error(LbxName, 0x01, LbxEntry, NULL);  /* LBXErr_not_found */
                }
            }

            // TODO(JimBalcom): this is wierd and not in LBX_Load_Entry
            if ( g_LBX_FileHandle == 0 )
            {
                LBX_Error(LbxName, 0x01, LbxEntry, NULL);  /* LBXErr_not_found */
            }

            /*
                BEGIN: LBX Header Offset
            */
            tmp_LbxHdrOfst = 0;
            /*
                END: LBX Header Offset
            */

            if ( lbx_seek(tmp_LbxHdrOfst, g_LBX_FileHandle) == ST_FAILURE )
            {
                LBX_Error(tmp_LbxName, 0x02, tmp_LbxEntry, NULL);  /* LBXErr_corrupted */
            }

            // HERE("lbx_read_sgmt(gsa_LBX_Header, SZ_LBX_HDR_B, g_LBX_FileHandle);");
            lbx_read_sgmt(gsa_LBX_Header, SZ_LBX_HDR_B, g_LBX_FileHandle);

            if ( farpeekw(gsa_LBX_Header, 2) != LBX_MAGSIG )
            {
                LBX_Error(tmp_LbxName, 0x07, tmp_LbxEntry, NULL);  /* LBXErr_bad_header */
            }

            g_LBX_EntryCount = farpeekw(gsa_LBX_Header, 0);
            
            /*
                END: Current != Previous
            */
        }
        else
        {
//            printf("DEBUG: [%s, %d]: Curr. == Prev.\n", __FILE__, __LINE__);
        }
        // (g_LBX_EntryCount < LbxEntryIndex) ~== (!(LbxEntryIndex >= g_LBX_EntryCount)) ~== (!((LbxEntryIndex - g_LBX_EntryCount) < 0))
        if ( g_LBX_EntryCount < LbxEntry )
        {
            LBX_Error(tmp_LbxName, 0x08, tmp_LbxEntry, NULL); /* LBXErr_entries_exceeded */
        }
        /*
            END: Current vs. Previous
        */

        /*
            BEGIN: Entry - Offset Begin, End, Size
        */
        LBXENTRYOFFSET()
        /*
            END: Entry - Offset Begin, End, Size
        */

        /*
            BEGIN: Records - Offset Begin, Size
        */
        LBXRECORDOFFSET()
        /*
            END: Records - Offset Begin, Size
        */

        /*
            BEGIN: Allocation Type
        */
        DataSize_Paras = 1 + (DataSize_Bytes / SZ_PARAGRAPH_B);
        LBXLOADTYPE()
        /*
            END: Allocation Type
        */
        tmp_SAMB_data = SAMB_data;
        /*
            BEGIN: Read Data
        */
        LBXREADDATA()
        /*
            END: Read Data
        */

    }  /* if ( SAMB_data == 0 ) */

    Update_MemFreeWorst_KB();

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: LBX_Load_Record(LbxName = %s, LbxEntry = %d, SAMB_head = 0x%04X, LoadType = %d, RecFirst = %d, RecCount = %d, RecSize = %d) { SAMB_data = 0x%04X }\n", __FILE__, __LINE__, LbxName, LbxEntry, SAMB_head, LoadType, RecFirst, RecCount, RecSize), SAMB_data;
#endif
    return SAMB_data;
}


// _s10p12
// ~= c&p LBX_Load_Record()
unsigned int LBXR_DirectLoader(char *LbxName, int LbxEntry, unsigned int SAMB_data, int RecFirst, int RecCount, int RecSize)
{
    char *tmp_LbxName;
    int tmp_LbxEntry;
    int tmp_LbxHdrFmt;
    char tmp_LbxFileName[20];
    char tmp_LbxFilePathName[60];
    unsigned long tmp_LbxHdrOfst;
    unsigned int EntryTableOffset;  // LBXENTRYOFFSET()
    unsigned long DataOffset_Begin; // LBXENTRYOFFSET()
    unsigned long DataOffset_End;   // LBXENTRYOFFSET()
    unsigned long DataSize_Bytes;   // LBXENTRYOFFSET()
    static int Record_Count;        // LBXRECORDOFFSET()
    static int Record_Size;         // LBXRECORDOFFSET()
    unsigned int ReadNbytes;        // LBXREADDATA()
    void * pSAMB_head;

    tmp_LbxName = LbxName;
    tmp_LbxEntry = LbxEntry;

    LBXTESTNEGATIVEENTRY()

    LBXHEADER()

    ExtractFileBase(LbxName);

    SAMB_data = EMM_LBXR_DirectLoad(tmp_LbxName, tmp_LbxEntry, SAMB_data, RecFirst, RecCount, RecSize);

    /*
        BEGIN: LBX Format Type / Header Offset
    */
    tmp_LbxHdrFmt = 0;
    /*
        END: LBX Format Type / Header Offset
    */

    if ( SAMB_data == ST_FAILURE )
    {

        /*
            BEGIN: Current vs. Previous
        */
        if ( (g_LBX_FileHandle == -1) || (stricmp(LbxName, g_LBX_Name) != 0) || (tmp_LbxHdrFmt != UU_g_LBX_HdrFmt))
        {
            /*
                BEGIN: Current != Previous
            */
            //printf("DEBUG: [%s, %d]: Curr. != Prev.\n", __FILE__, __LINE__);

            UU_g_LBX_HdrFmt = tmp_LbxHdrFmt;
            
            if ( g_LBX_FileHandle != -1 )  // #define DOS_UNUSED_FILE_HANDLE -1
            {
                lbx_close(g_LBX_FileHandle);
            }

            strcpy(g_LBX_Name, LbxName);
            strcpy(tmp_LbxFileName, LbxName);
            strcat(tmp_LbxFileName, LBX_File_Extension);
            g_LBX_FileHandle = lbx_open(tmp_LbxFileName);
            
            if ( (g_LBX_FileHandle == ST_FAILURE) && (UU_g_LBX_FilePath == ST_NULL) )
            {
                LBX_Error(LbxName, 0x01, LbxEntry, NULL);  /* LBXErr_not_found */
            }
            else
            {
                strcpy(tmp_LbxFilePathName, UU_g_LBX_FilePath );
                strcat(tmp_LbxFilePathName, tmp_LbxFileName);
                g_LBX_FileHandle = lbx_open(tmp_LbxFilePathName);

                if ( g_LBX_FileHandle == ST_FAILURE )
                {
                    LBX_Error(LbxName, 0x01, LbxEntry, NULL);  /* LBXErr_not_found */
                }
            }

            // TODO(JimBalcom): this is wierd and not in LBX_Load_Entry
            if ( g_LBX_FileHandle == 0 )
            {
                LBX_Error(LbxName, 0x01, LbxEntry, NULL);  /* LBXErr_not_found */
            }

            /*
                BEGIN: LBX Header Offset
            */
            tmp_LbxHdrOfst = 0;
            /*
                END: LBX Header Offset
            */

            if ( lbx_seek(tmp_LbxHdrOfst, g_LBX_FileHandle) == ST_FAILURE )
            {
                LBX_Error(tmp_LbxName, 0x02, tmp_LbxEntry, NULL);  /* LBXErr_corrupted */
            }

            lbx_read_sgmt(gsa_LBX_Header, SZ_LBX_HDR_B, g_LBX_FileHandle);

            if ( farpeekw(gsa_LBX_Header, 2) != LBX_MAGSIG )
            {
                LBX_Error(tmp_LbxName, 0x07, tmp_LbxEntry, NULL);  /* LBXErr_bad_header */
            }

            g_LBX_EntryCount = farpeekw(gsa_LBX_Header, 0);
            
            /*
                END: Current != Previous
            */
        }
        else
        {
            // printf("DEBUG: [%s, %d]: Curr. == Prev.\n", __FILE__, __LINE__);
        }
        // (g_LBX_EntryCount < LbxEntryIndex) ~== (!(LbxEntryIndex >= g_LBX_EntryCount)) ~== (!((LbxEntryIndex - g_LBX_EntryCount) < 0))
        if ( g_LBX_EntryCount < LbxEntry )
        {
            LBX_Error(tmp_LbxName, 0x08, tmp_LbxEntry, NULL); /* LBXErr_entries_exceeded */
        }
        /*
            END: Current vs. Previous
        */

        /*
            BEGIN: Entry - Offset Begin, End, Size
        */
        LBXENTRYOFFSET()
        /*
            END: Entry - Offset Begin, End, Size
        */

        /*
            BEGIN: Records - Offset Begin, Size
        */
        LBXRECORDOFFSET()
        /*
            END: Records - Offset Begin, Size
        */

        /*
            BEGIN: Read Data
        */
        // TODO(JimBalcomb): figure out what makes 60000 make sense
        // TODO(JimBalcomb): figure out the mix of non-EMM lbx_read_sgmt() but EMM ReadNbytes = tmp_DataSize_Bytes % divisor;
        // TODO(JimBalcomb): figure out if tmp_SAMB_data is required elsewhere, as it is not used here
        ReadNbytes = DataSize_Bytes % 60000;
        if ( lbx_read_sgmt(SAMB_data, ReadNbytes, g_LBX_FileHandle) == ST_FAILURE )
        {
            // HERE("LBX entry corrupted");
            LBX_Error(LbxName, 0x02, LbxEntry, NULL);  /* LBXErr_corrupted */
        }
        /*
            END: Read Data
        */

    }  /* if ( SAMB_data == 0 ) */

    Update_MemFreeWorst_KB();

    return SAMB_data;
}


// _s10p13
// ~= c&p EMM_LBX_Load_Entry()
unsigned int LBX_GetEntryData(char *LbxName, int LbxEntry, unsigned long *LbxEntryOffset, unsigned long *LbxEntrySize, int LbxHdrFmt)
{
    char *tmp_LbxName;
    int tmp_LbxEntry;
    SAMB_addr SAMB_data;
    int tmp_LbxHdrFmt;
    char tmp_LbxFileName[20];
    char tmp_LbxFilePathName[60];
    unsigned long tmp_LbxHdrOfst;
    unsigned int EntryTableOffset;
    unsigned long LbxEntryOffset_End;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: LBX_GetEntryData(LbxName = %s, LbxEntry = %d, *LbxEntryOffset, *LbxEntrySize, FormatType = %d)\n", __FILE__, __LINE__, LbxName, LbxEntry, LbxHdrFmt);
#endif

    tmp_LbxEntry = LbxEntry;
    tmp_LbxName = LbxName;
    
    LBXTESTNEGATIVEENTRY()

    // LBXHEADER()
    // if( g_LBX_Header_Allocd == 0 ) { g_LBX_Header_Allocd = 1; gsa_LBX_Header = FP_SEG(SA_Allocate_MemBlk(SZ_LBX_HDR_PR)); }
    // if( g_LBX_Header_Allocd == 0 ) { g_LBX_Header_Allocd = 1; gsa_LBX_Header = FP_SGMT(SA_Allocate_MemBlk(SZ_LBX_HDR_PR)); }
    LBXHEADER()

    ExtractFileBase(LbxName);

    LBXHEADEROFFSET()


        /*
            BEGIN: Current vs. Previous
        */
        if ( (g_LBX_FileHandle == -1) || (stricmp(LbxName, g_LBX_Name) != 0) || (tmp_LbxHdrFmt != UU_g_LBX_HdrFmt))
        {
            /*
                BEGIN: Current != Previous
            */
            // dlvfprintf("DEBUG: [%s, %d]: Curr. != Prev.\n", __FILE__, __LINE__);

            UU_g_LBX_HdrFmt = tmp_LbxHdrFmt;
            
            if ( g_LBX_FileHandle != -1 )
            {
                lbx_close(g_LBX_FileHandle);
            }

            strcpy(g_LBX_Name, LbxName);
            strcpy(tmp_LbxFileName, LbxName);
            strcat(tmp_LbxFileName, LBX_File_Extension);
            g_LBX_FileHandle = lbx_open(tmp_LbxFileName);
           
            if ( (g_LBX_FileHandle == ST_FAILURE) && (UU_g_LBX_FilePath == ST_NULL) )
            {
                // HERE("LBX_Error: could not be found  ( (g_LBX_FileHandle == ST_FAILURE) && (UU_g_LBX_FilePath == ST_NULL) )");
                LBX_Error(LbxName, 0x01, LbxEntry, NULL);  /* LBXErr_not_found */
            }
            else
            {
                strcpy(tmp_LbxFilePathName, UU_g_LBX_FilePath );
                strcat(tmp_LbxFilePathName, tmp_LbxFileName);
                g_LBX_FileHandle = lbx_open(tmp_LbxFilePathName);

                if ( g_LBX_FileHandle == ST_FAILURE )
                {
                    // HERE("LBX_Error: could not be found  ( g_LBX_FileHandle == ST_FAILURE )");
                    LBX_Error(LbxName, 0x01, LbxEntry, NULL);  /* LBXErr_not_found */
                }
            }

            /*
                BEGIN: LBX Header Offset
            */
            if ( UU_g_LBX_HdrFmt == 0 )
            {
                tmp_LbxHdrOfst = 0;
            }
            else
            {
                tmp_LbxHdrOfst = 512;
            }
            /*
                END: LBX Header Offset
            */

            if ( lbx_seek(tmp_LbxHdrOfst, g_LBX_FileHandle) == ST_FAILURE )
            {
                LBX_Error(tmp_LbxName, 0x02, tmp_LbxEntry, NULL);  /* LBXErr_corrupted */
            }

            lbx_read_sgmt(gsa_LBX_Header, SZ_LBX_HDR_B, g_LBX_FileHandle);

            if ( farpeekw(gsa_LBX_Header, 2) != LBX_MAGSIG )
            {
                LBX_Error(tmp_LbxName, 0x07, tmp_LbxEntry, NULL);  /* LBXErr_bad_header */
            }

            g_LBX_EntryCount = farpeekw(gsa_LBX_Header, 0);
            
            /*
                END: Current != Previous
            */
        }
        else
        {
            // dlvfprintf("DEBUG: [%s, %d]: Curr. == Prev.\n", __FILE__, __LINE__);
        }
        // (g_LBX_EntryCount < LbxEntryIndex) ~== (!(LbxEntryIndex >= g_LBX_EntryCount)) ~== (!((LbxEntryIndex - g_LBX_EntryCount) < 0))
        if ( g_LBX_EntryCount < LbxEntry )
        {
            LBX_Error(tmp_LbxName, 0x08, tmp_LbxEntry, NULL); /* LBXErr_entries_exceeded */
        }
        /*
            END: Current vs. Previous
        */

    EntryTableOffset = ((4 * LbxEntry) + 8);
    *LbxEntryOffset = farpeekdw(gsa_LBX_Header, EntryTableOffset);
    LbxEntryOffset_End = farpeekdw(gsa_LBX_Header, EntryTableOffset + 4);
    *LbxEntrySize = LbxEntryOffset_End - *LbxEntrySize;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: LBX_GetEntryData(LbxName = %s, LbxEntry = %d, *LbxEntryOffset, *LbxEntrySize, FormatType = %d)\n", __FILE__, __LINE__, LbxName, LbxEntry, LbxHdrFmt);
#endif

    return g_LBX_FileHandle;
}

// _s10p14
LBX_SetPath(char * LBX_Path)
{
    strcpy(UU_g_LBX_FilePath, LBX_Path);
}

// s10p15
void LBX_Error(char * LBX_Name, int LBX_Error_Number, int LBX_Index, int pages)
{
    int LBX_Error_Index;
    char str_itoa[20];
    char str_errmsg[120];

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: LBX_Error( LBX_Name = %s, LBX_Error_Number = %d, LBX_Index = %d, pages = %d)\n", __FILE__, __LINE__, LBX_Name, LBX_Error_Number, LBX_Index, pages);
#endif

#ifdef STU_DEBUG
    // strcpy(str_errmsg, "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");  // 120 characters
    strcpy(str_errmsg, "0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_");  // 110 characters
    dbg_prn("DEBUG: [%s, %d] str_errmsg: %s\n", __FILE__, __LINE__, str_errmsg);
#endif

    strcpy(str_errmsg, LBX_Name);
    itoa(LBX_Index, str_itoa, 10);
    strcat(str_errmsg, cnst_LBXErr_Common1);        // char *cnst_LBXErr_Common1 = ".LBX [entry ";                                         // MGC dseg:3EA3
    strcat(str_errmsg, str_itoa);
    strcat(str_errmsg, cnst_LBXErr_Common2);        // char *cnst_LBXErr_Common2 = "] ";                                                   // MGC dseg:3EB0
/*
char *cnst_LBX_Error7 = " is not an LBX file";                                      // MGC dseg:3F8B
char *cnst_LBX_Error8 = " exceeds number of LBX entries";                           // MGC dseg:3F9F
char *cnst_LBX_Error9 = " has an incorrect record size";                            // MGC dseg:3FBE
char *cnst_LBX_ErrorA = " exceeds number of defined records";                       // MGC dseg:3FDC
char *cnst_LBX_ErrorB = " cannot be reloaded into EMS w/o being first released.";   // MGC dseg:3FFF
char *cnst_LBX_ErrorC = " EMM loading error. Insufficient EMM.";                    // MGC dseg:4036
char *cnst_LBX_ErrorD = " Only pictures may be loaded into reserved EMM";           // MGC dseg:405C
char *cnst_LBX_ErrorE = " (Reserved EMM) ";                                         // MGC dseg:408B
char *cnst_LBX_ErrorF1 = " LBX to";                                                 // MGC dseg:409C
char *cnst_LBX_ErrorG = " Vga file animation frames cannot exceed 65536 bytes";     // MGC dseg:40A4
*/
/*
switch case jump table
seg010:10F5
off_16015           ; jump table for switch statement
offset loc_15EE2    case 0x00
offset loc_15EF4    case 0x01
offset loc_15EF9    case 0x02
offset loc_15F32    case 0x03
offset loc_15F37    case 0x04
offset loc_16006    case default
offset loc_15F70    case 0x06
offset loc_15F76    case 0x07
offset loc_15F7C    case 0x08
offset loc_15F82    case 0x09
offset loc_15F88    case 0x0A
offset loc_15FA9    case 0x0B
offset loc_15FAF    case 0x0C
offset loc_15FB5    case 0x0D
offset loc_15FD3    case 0x0E
offset loc_15FF7    case 0x0F
*/
    LBX_Error_Index = LBX_Error_Number - 1;

    switch (LBX_Error_Index)
    {
        case 0:
            strcat(str_errmsg, cnst_LBX_Error1);    // char *cnst_LBX_Error1 = " could not be found.";                                     // MGC dseg:3EB3
            break;
        case 1:
            strcat(str_errmsg, cnst_LBX_Error2);    // char *cnst_LBX_Error2 = " has been corrupted.";                                     // MGC dseg:3EC8
            break;
        case 2:
            strcpy(str_errmsg, cnst_LBX_Error31);   // char *cnst_LBX_Error31 = "Insufficient memory. You need at least ";                 // MGC dseg:3EDD
            itoa(RAM_Min_KB, str_itoa, 10);
            strcat(str_errmsg, str_itoa);
            strcat(str_errmsg, cnst_LBX_Error32);   // char *cnst_LBX_Error32 = "K free. Try removing all TSR's.";                         // MGC dseg:3F05
            break;
        case 3:
            strcat(str_errmsg, cnst_LBX_Error4);    // char *cnst_LBX_Error4 = " was not properly allocated or has been corrupted.";       // MGC dseg:3F25
            break;
        case 4:
            strcat(str_errmsg, cnst_LBX_Error51);   // char *cnst_LBX_Error51 = " failed to reload. Allocation too small by ";             // MGC dseg:3F58
            itoa(pages, str_itoa, 10);
            strcat(str_errmsg, str_itoa);
            strcat(str_errmsg, cnst_LBX_Error52);   // char *cnst_LBX_Error52 = " pages";                                                  // MGC dseg:3F84
            break;
        //case 5:
        //    break;
        case 6:
            strcat(str_errmsg, cnst_LBX_Error7);
            break;
        case 7:
            strcat(str_errmsg, cnst_LBX_Error8);
            break;
        case 8:
            strcat(str_errmsg, cnst_LBX_Error9);
            break;
        case 9:
            strcat(str_errmsg, cnst_LBX_ErrorA);
            break;
        case 10:
            strcpy(str_errmsg, LBX_Name);
            strcat(str_errmsg, LBX_File_Extension);
            strcat(str_errmsg, cnst_LBX_ErrorB);
            break;
        case 11:
            strcat(str_errmsg, cnst_LBX_ErrorC);
            break;
        case 12:
            strcat(str_errmsg, cnst_LBX_ErrorD);
            break;
        case 13:
            strcat(str_errmsg, cnst_LBX_ErrorC);
            strcat(str_errmsg, cnst_LBX_ErrorE);
        case 14:
            strcat(str_errmsg, cnst_LBX_ErrorF1);
            strcat(str_errmsg, cnst_LBX_ErrorC);
            strcat(str_errmsg, cnst_LBX_ErrorE);
            break;
        case 15:
            strcat(str_errmsg, cnst_LBX_ErrorG);
            break;
    }

Done:

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] str_errmsg: %s\n", __FILE__, __LINE__, str_errmsg);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: LBX_Error( LBX_Name = %s, LBX_Error_Number = %d, LBX_Index = %d, pages = %d)\n", __FILE__, __LINE__, LBX_Name, LBX_Error_Number, LBX_Index, pages);
#endif

    Exit(str_errmsg);
}

/*
    J:\STU\DBWD\developc\1oom-master\src\lbx.c
        char buf[32];
        const char *p = filename;
        char *q = buf;
        while (*p) {
            *q++ = toupper(*p++);
        }
        *q = 0;
*/
// _s10p16
void ExtractFileBase(char * LbxFileName)
{
    int itr_filename;

    itr_filename = 0;

    while (LbxFileName[itr_filename] != '\0')
    {

        if ( LbxFileName[itr_filename] >= 'a' )
        {
            LbxFileName[itr_filename] = LbxFileName[itr_filename] - 32;  // {'a' - 32 = 'A', ..., 'z' - 32 = 'Z'}
        }

        if ( LbxFileName[itr_filename] == '.' )
        {
            LbxFileName[itr_filename] = '\0';
        }
        
        itr_filename++;
    }
}

// s10p17
void RAM_SetMinKB(int Min_KB)
{
    RAM_Min_KB = Min_KB;
}
