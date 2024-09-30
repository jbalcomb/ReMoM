/*
    ¿ ?

    WIZARDS.EXE
        seg003
        seg004
        seg005
        seg006

*/

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
// int32_t UU_DISK_GetFreeSpace()



/*
    WIZARDS.EXE  seg005
*/

/*
    WIZARDS.EXE  seg006
    MoO1  STARMAP.EXE  seg016
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
// AKA  s06p05_Empty_pFxn()
/*

XREF:
    Init_Drivers()
    UU_Legacy_Startup()

*/
void DBG_Open_ERROR_LOG(void)
{

/*
MoM

push    es
push    ds
push    si
push    di
pop     di
pop     si
pop     ds
pop     es
retf
*/

/*
MoO1

push    es
push    ds
push    si
push    di
pop     di
pop     si
pop     ds
pop     es
retf
; ---------------------------------------------------------------------------
mov     ah, 3Ch
mov     cx, 0
mov     dx, offset aError_log ; "ERROR.LOG"
int     21h             ; DOS - 2+ - CREATE A FILE WITH HANDLE (CREAT)  ; CX = attributes for file  ; DS:DX -> ASCIZ filename (may include drive and path)
mov     fh_ERROR_LOG, ax
mov     ah, 45h
mov     bx, 2
int     21h             ; DOS - 2+ - CREATE DUPLICATE HANDLE (DUP)  ; BX = file handle to duplicate
mov     fh2_ERROR_LOG, ax
mov     ah, 46h
mov     bx, fh_ERROR_LOG
mov     cx, 2
int     21h             ; DOS - 2+ - FORCE DUPLICATE HANDLE (FORCDUP,DUP2)  ; BX = existing file handle, CX = new file handle
pop     di
pop     si
pop     ds
pop     es
retf
*/
}

// WZD s06p06
// AKA  s06p06_Empty_pFxn()
/*
XRE:
    Exit_With_Message()
    UU_Exit_With_Value()
    UU_VGA_B800Dump()

*/
void DBG_Close_ERROR_LOG(void)
{
/*
MoM

push    es
push    ds
push    si
push    di
pop     di
pop     si
pop     ds
pop     es
retf

*/

/*
MoO1

push    es
push    ds
push    si
push    di
pop     di
pop     si
pop     ds
pop     es
retf
; ---------------------------------------------------------------------------
mov     bx, fh_ERROR_LOG
cmp     bx, 0
jz      short loc_171ED
mov     ah, 3Eh
mov     bx, fh_ERROR_LOG
int     21h             ; DOS - 2+ - CLOSE A FILE WITH HANDLE  ; BX = file handle
mov     ah, 46h
mov     bx, fh2_ERROR_LOG
mov     cx, 2
int     21h             ; DOS - 2+ - FORCE DUPLICATE HANDLE (FORCDUP,DUP2)  ; BX = existing file handle, CX = new file handle
mov     ah, 3Eh
mov     bx, fh2_ERROR_LOG
int     21h             ; DOS - 2+ - CLOSE A FILE WITH HANDLE  ; BX = file handle
loc_171ED:
pop     di
pop     si
pop     ds
pop     es
retf
*/
}
