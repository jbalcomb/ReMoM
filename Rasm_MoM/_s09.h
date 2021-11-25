
#ifndef _s09_H
#define _s09_H


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
long lbx_size(int fhandle);                                     // _s09p04.asm  UU_DISK_GetFileSize
int  lbx_read_sgmt(unsigned buffer, int nbytes, int fhandle);   // _s09p05.asm  DISK_ReadToSeg
int  lbx_read_ofst(unsigned buffer, int nbytes, int fhandle);   // _s09p06.asm  DISK_ReadToData
void strcpyfar(unsigned int dstoff, unsigned int dstseg, unsigned int srcoff, unsigned int srcseg); //_s09p07.asm STR_CopyFar

#ifdef __cplusplus
}
#endif

#endif /* _s09_H */
