/*
    MAGIC.EXE
        seg004
    WIZARDS.EXE
        seg004

MoO2  Module: dos
*/
/*
    ¿ ?

    WIZARDS.EXE
        seg003
        seg004
        seg005
        seg006

*/

#ifndef DOS_H
#define DOS_H

#include "MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */



/*
    WIZARDS.EXE  seg003
*/

// UU_Return_Ignore()
// UU_Botched_DISK_GetSpace()
// UU_DISK_GetDefaultDrive()
// UU_DISK_DeleteFile()
// LOF()
// UU_DISK_GetFileDateTime()
// UU_DISK_CheckDrive()
// UU_DISK_GetCurrentDir()

// WZD s03p05
int32_t LOF(char * file_name);



/*
    WZD  seg004
*/

// WZD s04p01
int16_t DIR(char * match_string, char * found_file);

// WZD s04p02
// int32_t Disk_Free(void)



/*
    WIZARDS.EXE  seg005
*/



/*
    WIZARDS.EXE  seg006
*/

// WZD s06p01
// VGA_SetTextMode()

// WZD s06p02
// DOS_QuitWithMsg()

// WZD s06p03
// UU_DOS_PrintString2()

// WZD s06p04
// RAM_GetFreeBlockSize()

// WZD s06p05
// s06p05_Empty_pFxn()
void DBG_Open_ERROR_LOG(void);

// WZD s06p06
// s06p06_Empty_pFxn()
void DBG_Close_ERROR_LOG(void);



#ifdef __cplusplus
}
#endif

#endif /* DOS_H */
