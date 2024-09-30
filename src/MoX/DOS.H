/*
    ¿ ?

    WIZARDS.EXE
        seg003
        seg004
        seg005
        seg006

*/

#ifndef MOX_DOS_H
#define MOX_DOS_H

#include "MoX_Lib.H"



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



/*
    WIZARDS.EXE  seg004
*/

// WZD s04p01
// DIR()

// WZD s04p02
// // int32_t UU_DISK_GetFreeSpace()



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


#endif /* MOX_DOS_H */
