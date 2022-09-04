
#include "ST_HEAD.H"

#include "ST_LBX.H"
#include "ST_SA.H"

#include <fcntl.h>      /* O_BINARY, O_RDONLY */
#include<sys\stat.h>    /* ? */
#include <io.h>         /* filelength(); SEEK_CUR, SEEK_END, SEEK_SET */
//SEEK_SET (0)  File beginning
//SEEK_CUR (1)  Current file pointer
//SEEK_END (2)  End-of-file


unsigned int g_LBX_Header_Allocd = 0;           // dseg:3E66
int g_LBX_FileHandle = DOS_UnusedFileHandle;    // dseg:3E68  DOS Unused File Handle  == -1
unsigned int UU_g_LBX_HdrFmtOvrRd = 0;            // dseg:3E6A
char UU_g_LBX_FilePath[50] = {0};               // dseg:3E6C
char g_LBX_FileExtension[] = ".LBX";            // dseg:3E9E

char *cnst_LBXErr_Common1 = ".LBX [entry ";                                         //dseg:3EA3
char *cnst_LBXErr_Common2 = "] ";                                                   //dseg:3EB0
char *cnst_LBX_Error1 = " could not be found.";                                     //dseg:3EB3
char *cnst_LBX_Error2 = " has been corrupted.";                                     //dseg:3EC8
char *cnst_LBX_Error31 = "Insufficient memory. You need at least ";                 //dseg:3EDD
char *cnst_LBX_Error32 = "K free. Try removing all TSR's.";                         //dseg:3F05
char *cnst_LBX_Error4 = " was not properly allocated or has been corrupted.";       //dseg:3F25
char *cnst_LBX_Error51 = " failed to reload. Allocation too small by ";             //dseg:3F58
char *cnst_LBX_Error52 = " pages";                                                  //dseg:3F84
char *cnst_LBX_Error7 = " is not an LBX file";                                      //dseg:3F8B
char *cnst_LBX_Error8 = " exceeds number of LBX entries";                           //dseg:3F9F
char *cnst_LBX_Error9 = " has an incorrect record size";                            //dseg:3FBE
char *cnst_LBX_ErrorA = " exceeds number of defined records";                       //dseg:3FDC
char *cnst_LBX_ErrorB = " cannot be reloaded into EMS w/o being first released.";   //dseg:3FFF
char *cnst_LBX_ErrorC = " EMM loading error. Insufficient EMM.";                    //dseg:4036
char *cnst_LBX_ErrorD = " Only pictures may be loaded into reserved EMM";           //dseg:405C
char *cnst_LBX_ErrorE = " (Reserved EMM) ";                                         //dseg:408B
char *cnst_LBX_ErrorF1 = " LBX to";                                                 //dseg:409C
char *cnst_LBX_ErrorG = " Vga file animation frames cannot exceed 65536 bytes";     //dseg:40A4

unsigned int g_LBX_EntryCount;              // dseg:A5C6
unsigned int UU_g_LBX_HdrFmt;               // dseg:A5C8
unsigned int gsa_LBX_Header;                // dseg:A5CA
char g_LBX_Name[16];                        // dseg:A5CC
unsigned int g_LBX_EmmRsvd;                 // dseg:A5EC



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
    int bytes;
    //void * buf;
    //buf = malloc();
    //read(fhandle, buf, 10)
    void _FAR * buf;
    //unsigned int ofst;
    int st_status;

    buf = MK_FP(dst_sgmt, 0);
    //ofst = 0;
    //buf = (void *)((long)(sgmt << 16) | ofst);

    if ((bytes = read(fhandle, buf, nbytes)) == -1) {
        st_status = 0;  // ST_FAILURE
    }
    else
    {
        st_status = -1;  // ST_SUCCESS
    }

    return st_status;
}

// _s09p06
int  lbx_read_ofst(unsigned int dst_ofst, int nbytes, int fhandle)
{
    int bytes;
    //void * buf;
    //buf = malloc();
    //read(fhandle, buf, 10)
    void _FAR * buf;
    //unsigned int ofst;
    int st_status;

    buf = MK_FP(_DS, dst_ofst);
    //ofst = 0;
    //buf = (void *)((long)(sgmt << 16) | ofst);

    if ((bytes = read(fhandle, buf, nbytes)) == -1) {
        st_status = 0;  // ST_FAILURE
    }
    else
    {
        st_status = -1;  // ST_SUCCESS
    }

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
unsigned int LBXE_LoadSingle(char *LbxName, int LbxEntryIndex)
{
    sgmt_addr SAMB_head;
    int LoadType;
    int LbxHdrFmt;
    unsigned int SAMB_data;

    SAMB_head = 0;
    LoadType = 0;
    LbxHdrFmt = 0;

    SAMB_data = LBX_Load_Entry(LbxName, LbxEntryIndex, SAMB_head, LoadType, LbxHdrFmt);

    return SAMB_data;
}

// _s10p02
unsigned int LBXE_LoadReplace(char *LbxName, int LbxEntryIndex, unsigned int SAMB_head)
{
    int LoadType;
    int LbxHdrFmt;
    unsigned int SAMB_data;

    LoadType = 1;
    LbxHdrFmt = 0;

    SAMB_data = LBX_Load_Entry(LbxName, LbxEntryIndex, SAMB_head, LoadType, LbxHdrFmt);

    return SAMB_data;
}

// _s10p03
unsigned int LBXE_LoadAppend(char *LbxName, int LbxEntryIndex, unsigned int SAMB_head)
{
    int LoadType;
    int LbxHdrFmt;
    unsigned int SAMB_data;

    LoadType = 2;
    LbxHdrFmt = 0;

    SAMB_data = LBX_Load_Entry(LbxName, LbxEntryIndex, SAMB_head, LoadType, LbxHdrFmt);

    return SAMB_data;
}

// _s10p04
unsigned int LBXR_LoadSingle(char *LbxName, int LbxEntryIndex, int RecFirst, int RecCount, int RecSize)
{
    unsigned int SAMB_head;
    int LoadType;
    unsigned int SAMB_data;

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
    g_RAM_Min_KB = RAM_MinKB;
}
