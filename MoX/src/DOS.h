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

// WZD s03p05
int32_t LOF(char * file_name);

// UU_DISK_GetFileDateTime()
// UU_DISK_CheckDrive()
// UU_DISK_GetCurrentDir()



/*
    WZD  seg004
*/

// WZD s04p01
int16_t DIR(char * match_string, char * found_file);

// WZD s04p02
// int32_t Disk_Free(void)




/*
    WIZARDS.EXE  seg007
*/

// WZD s07p01
// UU_SAMB_DOS_Allocate_Memory

// WZD s07p02
// UU_DOS_MemAlloc

// WZD s07p03
// UU_DOS_MemFree

// WZD s07p04
int16_t Check_Allocation(SAMB_ptr SAMB_head);

// WZD s07p05
uint8_t farpeekb(void * ptr, int16_t ofst);

// WZD s07p06
uint16_t farpeekw(void * ptr, int16_t ofst);

// WZD s07p07
uint32_t farpeekdw(void * ptr, int16_t ofst);

// WZD s07p08
void farpokeb(void * ptr, int16_t ofst, uint8_t value);

// WZD s07p09
void farpokew(void * ptr, int16_t ofst, uint16_t value);

// WZD s07p10
void farpokedw(void * ptr, int16_t ofst, uint32_t value);



#ifdef __cplusplus
}
#endif

#endif /* DOS_H */
