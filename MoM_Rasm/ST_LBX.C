
// J:\STU\DBWD\BORLANDC\INCLUDE\
#include <stdio.h>      /* FILE */
#include <fcntl.h>      /* O_BINARY, O_RDONLY */
#include<sys\stat.h>    /* ? */
#include <IO.H>         /* filelength(), read(); SEEK_CUR, SEEK_END, SEEK_SET */
    // SEEK_SET (0)  File beginning; SEEK_CUR (1)  Current file pointer; SEEK_END (2)  End-of-file
//#ifdef DEBUG
    #include <errno.h>
//#endif

#include "ST_HEAD.H"
#include "ST_TYPE.H"

#include "ST_LBX.H"
//#include "ST_SA.H"


/*
    LBX Globals - Initialized
*/
unsigned int g_LBX_Header_Allocd = 0;           // dseg:3E66
int g_LBX_FileHandle = DOS_UnusedFileHandle;    // dseg:3E68  DOS Unused File Handle  == -1
unsigned int UU_g_LBX_HdrFmtOvrRd = 0;            // dseg:3E6A
char UU_g_LBX_FilePath[50] = {0};               // dseg:3E6C
char g_LBX_FileExtension[] = ".LBX";            // dseg:3E9E

char *cnst_LBXErr_Common1 = ".LBX [entry ";                                         // MGC dseg:3EA3
char *cnst_LBXErr_Common2 = "] ";                                                   // MGC dseg:3EB0
char *cnst_LBX_Error1 = " could not be found.";                                     // MGC dseg:3EB3
char *cnst_LBX_Error2 = " has been corrupted.";                                     // MGC dseg:3EC8
char *cnst_LBX_Error31 = "Insufficient memory. You need at least ";                 // MGC dseg:3EDD
char *cnst_LBX_Error32 = "K free. Try removing all TSR's.";                         // MGC dseg:3F05
char *cnst_LBX_Error4 = " was not properly allocated or has been corrupted.";       // MGC dseg:3F25
char *cnst_LBX_Error51 = " failed to reload. Allocation too small by ";             // MGC dseg:3F58
char *cnst_LBX_Error52 = " pages";                                                  // MGC dseg:3F84
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
/*
    LBX Globals - Unitialized
*/
unsigned int RAM_Min_KB;                    // MGC dseg:A5C4  ; set to 583 in _main
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
    void _FAR * buf;
    int st_status;
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: lbx_read_sgmt(dst_sgmt = 0x%04X, nbytes = %d, fhandle = %d)\n", __FILE__, __LINE__, dst_sgmt, nbytes, fhandle);
#endif
    buf = MK_FP(dst_sgmt, 0);
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: buf: %p\n", __FILE__, __LINE__, buf);
#endif
    if (read(fhandle, buf, nbytes) == -1) {
        st_status = 0;  // ST_FAILURE
    }
    else
    {
        st_status = -1;  // ST_SUCCESS
    }
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: lbx_read_sgmt(dst_sgmt = 0x%04X, nbytes = %d, fhandle = %d)\n", __FILE__, __LINE__, dst_sgmt, nbytes, fhandle);
#endif
    return st_status;
}

// _s09p06
int lbx_read_ofst(unsigned int dst_ofst, int nbytes, int fhandle)
{
    // void * buf;
    void _FAR * buf;
    int st_status;
//#ifdef DEBUG
    int baitos;
    char * strerrbuf;
    int itr_nbytes;
//#endif
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: lbx_read_ofst(dst_ofst = 0x%04X, nbytes = %d, fhandle = %d)\n", __FILE__, __LINE__, dst_ofst, nbytes, fhandle);
#endif

    //buf = (void *)dst_ofst;
    buf = MK_FP(_DS, dst_ofst);  // without this FP, it gets the right bytes but does not assign them properly ? because all pointers are FAR in LARGE memory model ? override NEAR ?
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: buf: %p\n", __FILE__, __LINE__, buf);
#endif
    if ( (baitos = read(fhandle, buf, nbytes)) == -1 )
    {
//#ifdef DEBUG
        strerrbuf = strerror(errno);
        dlvfprintf("DEBUG: [%s, %d]: strerrbuf: %s\n", __FILE__, __LINE__, strerrbuf);
//#endif
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: buf: %p\n", __FILE__, __LINE__, buf);
#endif
        st_status = 0;  // ST_FAILURE
    }
    else
    {
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: baitos: %d\n", __FILE__, __LINE__, baitos);
#endif
//#ifdef DEBUG
    for ( itr_nbytes = 0; itr_nbytes < nbytes; itr_nbytes++ )
    {
        dlvfprintf("DEBUG: [%s, %d]: buf[%d]: 0x%02X\n", __FILE__, __LINE__, itr_nbytes, *((unsigned char *)buf + itr_nbytes));
    }
//#endif
        st_status = -1;  // ST_SUCCESS
    }
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: lbx_read_ofst(dst_ofst = 0x%04X, nbytes = %d, fhandle = %d) { st_status = %d }\n", __FILE__, __LINE__, dst_ofst, nbytes, fhandle, st_status);
#endif
    return st_status;
}

// _s09p05  lbx_read_sgmt
// _s09p06  lbx_read_ofst
int lbx_read(void _FAR * fpDst, int nbytes, int fhandle)
{
    int st_status;
//#ifdef DEBUG
    int baitos;
    char * strerrbuf;
    int itr_nbytes;
//#endif
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: lbx_read(fpDst = 0x%p, nbytes = %d, fhandle = %d)\n", __FILE__, __LINE__, fpDst, nbytes, fhandle);
#endif

    if ( (baitos = read(fhandle, fpDst, nbytes)) == -1 )
    {
//#ifdef DEBUG
        strerrbuf = strerror(errno);
        dlvfprintf("DEBUG: [%s, %d]: strerrbuf: %s\n", __FILE__, __LINE__, strerrbuf);
//#endif
        st_status = 0;  // ST_FAILURE
    }
    else
    {
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: baitos: %d\n", __FILE__, __LINE__, baitos);
#endif
//#ifdef DEBUG
    for ( itr_nbytes = 0; itr_nbytes < nbytes; itr_nbytes++ )
    {
        dlvfprintf("DEBUG: [%s, %d]: buf[%d]: 0x%02X\n", __FILE__, __LINE__, itr_nbytes, *((unsigned char *)fpDst + itr_nbytes));
    }
//#endif
        st_status = -1;  // ST_SUCCESS
    }
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: lbx_read(fpDst = 0x%p, nbytes = %d, fhandle = %d) { st_status = %d }\n", __FILE__, __LINE__, fpDst, nbytes, fhandle, st_status);
#endif
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

// _s10p05

// _s10p06

// _s10p07

// _s10p08

// _s10p09

// _s10p10

// _s10p11

// _s10p12

// _s10p13

// _s10p14

// _s10p15
void LBX_Error(char * name, int errno, int entry, int pages)
{
    char cnv[20];
    char errmsg[120];

    strcpy(errmsg, name);
    itoa(entry, cnv, 10);
    strcat(errmsg, cnst_LBXErr_Common1);
    strcat(errmsg, cnv);
    strcat(errmsg, cnst_LBXErr_Common2);
    
    switch (errno)
    {
        case 1:
            strcat(errmsg, cnst_LBX_Error1);
            break;
        case 2:
            strcat(errmsg, cnst_LBX_Error2);
            break;
        case 3:
            strcpy(errmsg, cnst_LBX_Error31);
            itoa(RAM_Min_KB, cnv, 10);
            strcat(errmsg, cnv);
            strcat(errmsg, cnst_LBX_Error32);
            break;
        case 4:
            strcat(errmsg, cnst_LBX_Error4);
            break;
        case 5:
            strcat(errmsg, cnst_LBX_Error51);
            itoa(pages, cnv, 10);
            strcat(errmsg, cnv);
            strcat(errmsg, cnst_LBX_Error52);
            break;
        //case 6:
        //    break;
        case 7:
            strcat(errmsg, cnst_LBX_Error7);
            break;
        case 8:
            strcat(errmsg, cnst_LBX_Error8);
            break;
        case 9:
            strcat(errmsg, cnst_LBX_Error9);
            break;
        case 10:
            strcat(errmsg, cnst_LBX_ErrorA);
            break;
        case 11:
            strcpy(errmsg, name);
            strcat(errmsg, g_LBX_FileExtension);
            strcat(errmsg, cnst_LBX_ErrorB);
            break;
        case 12:
            strcat(errmsg, cnst_LBX_ErrorC);
            break;
        case 13:
            strcat(errmsg, cnst_LBX_ErrorD);
            break;
        case 14:
            strcat(errmsg, cnst_LBX_ErrorC);
            strcat(errmsg, cnst_LBX_ErrorE);
        case 15:
            strcat(errmsg, cnst_LBX_ErrorF1);
            strcat(errmsg, cnst_LBX_ErrorC);
            strcat(errmsg, cnst_LBX_ErrorE);
            break;
        case 16:
            strcat(errmsg, cnst_LBX_ErrorG);
            break;
    }

    Quit(errmsg);
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

// _s10p17
void RAM_SetMinKB(int RAM_MinKB)
{
    RAM_Min_KB = RAM_MinKB;
}
