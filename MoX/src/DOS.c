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

#include "Mouse.h"
#include "MOX_BASE.h"
#include "MOX_TYPE.h"

#include <stdio.h>

#include "DOS.h"



/*
MoO2
Module: dos

    struct (36 bytes) tmbuf
    Address: 02:001A5804
            signed integer (4 bytes) tm_sec
            signed integer (4 bytes) tm_min
            signed integer (4 bytes) tm_hour
            signed integer (4 bytes) tm_mday
            signed integer (4 bytes) tm_mon
            signed integer (4 bytes) tm_year
            signed integer (4 bytes) tm_wday
            signed integer (4 bytes) tm_yday
            signed integer (4 bytes) tm_isdst

    struct (43 bytes) fileinfo
    Address: 02:001A5828
            array (21 bytes) reserved
            Num elements:   21, Type:                unsigned integer (4 bytes) 
            char (1 bytes) attrib
            unsigned integer (2 bytes) wr_time
            unsigned integer (2 bytes) wr_date
            unsigned integer (4 bytes) size
            array (13 bytes) name
            Num elements:   13, Type:                unsigned integer (4 bytes) 

    function (0 bytes) DIR
    Address: 01:001114D7
        Num params: 2
        Return type: signed integer (4 bytes) 
        pointer (4 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) match_string
            pointer (4 bytes) found_file
            unsigned integer (4 bytes) rc

    function (0 bytes) DIR_DIR
    Address: 01:00111551
        Num params: 2
        Return type: signed integer (4 bytes) 
        pointer (4 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) match_string
            pointer (4 bytes) found_file
            unsigned integer (4 bytes) rc
            unsigned integer (4 bytes) attr

    function (0 bytes) LOF
    Address: 01:00111610
        Num params: 1
        Return type: signed integer (4 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) file_name
            unsigned integer (4 bytes) rc

    function (0 bytes) Time_Stamp
    Address: 01:00111660
        Num params: 1
        Return type: pointer (4 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) file_name
            unsigned integer (4 bytes) rc
            signed integer (4 bytes) time

    function (0 bytes) Disk_Free
    Address: 01:00111763
        Num params: 0
        Return type: signed integer (4 bytes) 
        Locals:
            struct (8 bytes) disk_info
                    unsigned integer (2 bytes) total_clusters
                    unsigned integer (2 bytes) avail_clusters
                    unsigned integer (2 bytes) sectors_per_cluster
                    unsigned integer (2 bytes) bytes_per_sector
            unsigned integer (4 bytes) size

    function (0 bytes) Delete_File
    Address: 01:001117BF
        Num params: 1
        Return type: void (1 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) filename

    function (0 bytes) Current_Dir
    Address: 01:001117EB
        Num params: 1
        Return type: void (1 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) filename

    function (0 bytes) File_Exists
    Address: 01:0011181C
        Num params: 1
        Return type: signed integer (4 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) filename
            struct (43 bytes) ffblk
                    array (21 bytes) reserved
                    Num elements:   21, Type:                        unsigned integer (4 bytes) 
                    char (1 bytes) attrib
                    unsigned integer (2 bytes) wr_time
                    unsigned integer (2 bytes) wr_date
                    unsigned integer (4 bytes) size
                    array (13 bytes) name
                    Num elements:   13, Type:                        unsigned integer (4 bytes) 

    function (0 bytes) Get_Open_File_Count
    Address: 01:0011186A
        Num params: 0
        Return type: signed integer (4 bytes) 
        void (1 bytes) 
        Locals:
            pointer (4 bytes) fptr
            array (60 bytes) full_file_path
            Num elements:   60, Type:                unsigned integer (4 bytes) 
            signed integer (4 bytes) file_count

*/



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
/*
MoO2
Module: dos
    function (0 bytes) LOF
    Address: 01:00111610
        Num params: 1
        Return type: signed integer (4 bytes) 
        pointer (4 bytes) 
        Locals:
            pointer (4 bytes) file_name
            unsigned integer (4 bytes) rc
*/
int32_t LOF(char * file_name)
{
    FILE * file_pointer = 0;
    signed int long position = 0;
    uint32_t size = 0;
    Save_Mouse_State();
    file_pointer = fopen(file_name, "rb");
    if(
        (file_pointer != NULL)
        &&
        (fseek(file_pointer, 0, SEEK_END) == 0)
        &&
        ((position = ftell(file_pointer)) != -1L)
    )
    {
        size = (uint32_t)position;
    }
    else
    {
        size = 0;  // AKA ST_FAILURE, not -1 for ST_SUCCESS
    }
    if(file_pointer != NULL)
    {
        fclose(file_pointer);
    }
    Restore_Mouse_State();
    return size;
}


/*
    WIZARDS.EXE  seg004
*/

// WZD s04p01
/*
    returns ST_SUCCESS(-1), ST_FAILURE(1)
OG-MoM MS-DOS version uses 'Find First' and 'Find Next'
*/
int16_t DIR(char * match_string, char * found_file)
{
    int16_t st_status = 0;
    FILE * file_pointer = 0;
    Save_Mouse_State();
    file_pointer = fopen(match_string, "rb");
    if(file_pointer == NULL)
    {
        found_file[0] = '\0';
        st_status = ST_FAILURE;
    }
    else
    {
        fclose(file_pointer);
        while(*match_string) { *(found_file++) = *(match_string++); }
        *found_file = '\0';
        st_status = ST_SUCCESS;
    }
    Restore_Mouse_State();
    return st_status;
}


// WZD s04p02
// drake178: UU_DISK_GetFreeSpace()
/*
; unused in MoM
; wrapper for INT 21h - AH 36h (Get Disk Space), that
; also multiplies the registers to return the final
; value in bytes (of the free space only), or -1
*/
/*
*/
/*
MoO2
Module: dos
    function (0 bytes) Disk_Free
    Address: 01:00111763
        Num params: 0
        Return type: signed integer (4 bytes) 
        Locals:
            struct (8 bytes) disk_info
                    unsigned integer (2 bytes) total_clusters
                    unsigned integer (2 bytes) avail_clusters
                    unsigned integer (2 bytes) sectors_per_cluster
                    unsigned integer (2 bytes) bytes_per_sector
            unsigned integer (4 bytes) size
*/
// int32_t Disk_Free(void)
// {
// push    es
// push    di
// push    ds
// push    si
// mov     dx, 0
// mov     ah, 36h
// int     21h                             ; DOS - 2+ - GET DISK SPACE
//                                         ; DL = drive code (0 = default, 1 = A, 2 = B, etc.)
// cmp     ax, 0FFFFh
// jz      short loc_14B47
// // ; sectors per cluster * number of available clusters * number of bytes per sector
// mul     cx
// mul     bx
// jmp     short @@Done
// nop
// loc_14B47:
// mov     dx, -1
// mov     ax, -1
// @@Done:
// pop     si
// pop     ds
// pop     di
// pop     es
// retf
// }



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
