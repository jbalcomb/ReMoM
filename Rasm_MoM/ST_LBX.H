#ifndef ST_LBX_H
#define ST_LBX_H


#define LBXHEADER {if(!g_LBX_Header_Allocd){g_LBX_Header_Allocd = 1;gsa_LBX_Header = SA_Allocate_MemBlk(SZ_LBX_HDR_PR);}}


#define SZ_LBX_HDR_B 512
#define SZ_LBX_HDR_PR 32
#define SZ_LBX_HDR_PRE_B 8
#define SZ_LBX_HDR_OFST_B 4
#define SZ_32K_B 32768
#define SZ_32K_PR 2048
#define SZ_PARAGRAPH_B 16

#define LBX_MAGSIG 0xFEAD


// LBX Globals - Initialized
extern unsigned int g_LBX_Header_Allocd;        // dseg:3E66
extern int g_LBX_FileHandle;                    // dseg:3E68
extern unsigned int UU_g_LBX_HdrFmt;            // dseg:3E6A
extern char UU_g_LBX_FilePath[];                // dseg:3E6C
extern char g_LBX_FileExtension[];              // dseg:3E9E
// LBX Globals - Unitialized
extern unsigned int g_LBX_EntryCount;           // dseg:A5C6
extern unsigned int g_LBX_FormatType;           // dseg:A5C8
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
    seg007
*/
unsigned char farpeekb(unsigned int sgmt, unsigned int ofst);                // _s07p05
unsigned int farpeekw(unsigned int sgmt, unsigned int ofst);                 // _s07p06
unsigned long farpeekdw(unsigned int sgmt, unsigned int ofst);               // _s07p07
void farpokeb(unsigned int sgmt, unsigned int ofst, unsigned char value);    // _s07p08
void farpokew(unsigned int sgmt, unsigned int ofst, unsigned int value);     // _s07p09
//void farpokedw(unsigned int sgmt, unsigned int ofst, unsigned long val);   // _s07p10

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
unsigned int LBXE_LoadSingle(char *LbxName, int LbxEntryIndex);                                                // _s10p01.asm

//unsigned int LBX_EntryLoader(char *LbxFileName, int LbxEntryIndex, int AllocSeg, int LoadType, int Fortmattype);    // _s10p10c.c
unsigned int LBX_EntryLoader(char *LbxName, int LbxEntryIndex, unsigned int AllocSeg, int LoadType, int FormatType);

void ExtractFileBase(char *LbxFileName);   // _s10p16.asm
void RAM_SetMinKB(int RAM_MinKB);       // _s10p17c.c

#ifdef __cplusplus
}
#endif

#endif /* ST_LBX_H */
