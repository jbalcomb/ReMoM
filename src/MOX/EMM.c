/*
    WIZARDS.EXE
        seg012
        seg013
*/

#include "EMM.h"

#include "MOX_BASE.h"
#include "MOX_TYPE.h"

#include <stdlib.h>     /* abs(); itoa(); */
#include <string.h>     /* memcpy() memset(), strcat(), strcpy(), stricmp() */



// WZD dseg:760C                                                 BEGIN:  seg012 - Initialized Data

// ; contains the segment address of the EMS page frame
// WZD dseg:760C
SAMB_ptr EMS_PFBA = 0;
// WZD dseg:760E 45 4D 4D 58 58 58 58 30 00                      EMM_DevName db 'EMMXXXX0',0             ; DATA XREF: EMM_DetectDevice:loc_16AA0o

// WZD dseg:7617
struct s_EMM_L2P_MAP_RECORD EMM_Log2Phys_Map[4] =
{
               { 0, 0 },
               { 0, 1 },
               { 0, 2 },
               { 0, 3 }
};

// WZD dseg:7627 00                                              db    0

// WZD dseg:7628
int16_t EMM_Pages_Reserved = 40;

// WZD dseg:762A
int16_t EMM_Open_Handles = 0;

// WZD dseg:762C 59 4F 20 4D 4F 4D 41 00                         cnst_EMM_Hnd1 db 'YO MOMA',0            ; DATA XREF: EMM_Startup+3Ao ...
// WZD dseg:7634 59 4F 20 4D 4F 4D 41 00                         cnst_EMM_Hnd12 db 'YO MOMA',0           ; DATA XREF: EMM_Startup+6Ao

// WZD dseg:763C 00                                              db    0
// WZD dseg:763D 00                                              db    0

// WZD dseg:763D                                                 END:  seg012 - Initialized Data



// WZD dseg:763E                                                 BEGIN:  seg013 - Initialized Data

// WZD dseg:763E 56 47 41 46 49 4C 45 48 00                      str_VGAFILEH__seg013 db 'VGAFILEH',0    ; DATA XREF: EMM_Startup+164o
// WZD dseg:7647 45 4D 4D 44 41 54 41 48 00                      str_EMMDATAH__seg013 db 'EMMDATAH',0    ; DATA XREF: EMM_Startup+17Bo
// WZD dseg:7650 2E 4C 42 58 00                                  str_LBX_extension__seg013 db '.LBX',0   ; DATA XREF: EMM_Load_LBX_File+A3o ...
// WZD dseg:7655 45 4D 4D 20 72 65 73 65 72 76 65 64 20 65 78 63+cnst_EMMErr_ResOut1 db 'EMM reserved exceeded by ',0
// WZD dseg:766F 20 62 6C 6F 63 6B 73 20 5B 00                   cnst_EMMErr_ResOut2 db ' blocks [',0    ; DATA XREF: EMM_GetHandle+12Co
// WZD dseg:7679 2E 4C 42 58 5D 00                               cnst_EMMErr_ResOut3 db '.LBX]',0        ; DATA XREF: EMM_GetHandle+147o
// WZD dseg:767F 20 72 65 6C 6F 61 64 65 64 20 69 6E 74 6F 20 45+cnst_EMMErr_Reload1 db ' reloaded into EMM, diff size =',0
// WZD dseg:769F 20 20 00                                        cnst_EMMErr_Space db '  ',0             ; DATA XREF: EMM_GetHandle+1ECo ...
// WZD dseg:76A2 45 4D 4D 20 32 6C 6F 6E 67 00                   UU_cnst_EMMErr_2long db 'EMM 2long',0   ; DATA XREF: UU_EMM_LBX_Load2Hnd+A3o
// WZD dseg:76AC
char cnst_EMMErr_TooLow1[] = "You must have at least ";
// WZD dseg:76C4
char cnst_EMMErr_TooLow2[] = "K of expanded memory.";

// WZD dseg:76C4                                                 END:  seg013 - Initialized Data



// WZD dseg:E5EA                                                 BEGIN:  seg013 - Unitialized Data

// WZD dseg:E5EA 00 00                                           UU_EMMData_PrevLvl dw 0                 ; DATA XREF: UU_EMM_Data_Mark+6w ...

// WZD dseg:E5EC
int16_t g_EmmHndl_OVERXYZ;

// WZD dseg:E5EE 00 00                                           EMM_Data_Level dw 0                     ; DATA XREF: EMM_Startup+18Ew ...
// WZD dseg:E5F0 00 00                                           _EMMDATAH_seg dw 0                      ; DATA XREF: EMM_Startup+18Bw ...
// WZD dseg:E5F2 00 00                                           dw 0

// WZD dseg:E5F4
byte_ptr _VGAFILEH_seg;

// WZD dseg:E5F6 00 00                                           g_EmmRsvd dw 0                          ; DATA XREF: EMM_LBX_EntryLoader:@@EmmHndlNmExistsr ...
// WZD dseg:E5F8 00 00                                           EmmHndlNbr_YOMOMA dw 0                  ; DATA XREF: EMM_Startup+1Bw ...
// WZD dseg:E5FA 00 00                                           EMM_OK dw 0                             ; DATA XREF: EMM_Startup:loc_16E33w ...

// WZD dseg:E5FC
struct s_EMM_RECORD EMM_Table[40];

// WZD dseg:E7DC
int16_t EMM_MinKB;

// WZD dseg:E7DC                                                 END:  seg013 - Unitialized Data



/*
    WIZARDS.EXE seg012
*/

// WZD s12p01
// EMM_DetectDevice()

// WZD s12p02
// EMM_GetHandleCount()

// WZD s12p03
// EMM_GetFreePageCnt()

// WZD s12p04
// EMM_GetPageCount()

// WZD s12p05
// EMM_MakeNamedHandle()

// WZD s12p06
// EMM_GetHandleName()

// WZD s12p07
void EMM_ReleaseHandle__SUTB(int16_t handle)
{
    if(handle != 0)
    {
// mov     ah, 45h
// mov     dx, [bp+EMM_Handle]
// int     67h                             ;  - LIM EMS - RELEASE HANDLE AND MEMORY
//                                         ; DX = EMM handle
//                                         ; Return: AH = status
        
    }
}

// WZD s12p08
// EMM_MapnRead()

// WZD s12p09
// EMM_MapnWrite()

// WZD s12p10
// EMM_GetPageFrame()

// WZD s12p11
// EMM_MapFourPages()

// WZD s12p12
void EMM_Map4Pages(int emsFirst, SAMB_ptr emsHandle)
{
    EMS_PFBA = (emsHandle + (emsFirst * SZ_EMM_LOGICAL_PAGE));
// mov     bx, [bp+emsFirst]
// mov     dx, [bp+emsHandle]
// mov     ax, seg dseg
// mov     ds, ax
// mov     [ems_mapping_array.Logical_Page], bx
// inc     bx
// mov     [ems_mapping_array.Logical_Page+4], bx
// inc     bx
// mov     [ems_mapping_array.Logical_Page+8], bx
// inc     bx
// mov     [ems_mapping_array.Logical_Page+0Ch], bx
// inc     bx                              ; ¿ trailing increment in a for-loop ?
// mov     cx, 4                           ; number of entries in array
// mov     si, offset ems_mapping_array
// mov     ax, 5000h
// int     67h                             ;  - LIM EMS 4.0 - MAP/UNMAP MULTIPLE HANDLE PAGES
//                                         ; AL = 00h / 01h, DX = handle, CX = number of entries in array
//                                         ; DS:SI -> mapping array
//                                         ; Return: AH = status
}


/*
    WIZARDS.EXE seg013
*/

// WZD s13p01
void EMM_Startup(void)
{



}


// WZD s13p02
// EMM_Load_LBX_File_0()

// WZD s13p03
// EMM_Load_LBX_File_1()

// WZD s13p04
// EMM_Load_LBX_File()

// WZD s13p05
// RP_EMM_LoadLBX2Hnd()

// WZD s13p06
// RP_EMM_LoadLBX2Hnd2()

// WZD s13p07
// UU_EMM_LoadLBX_Fail()

// WZD s13p08
// UU_EMM_CustLoadLBX()

// WZD s13p09
// UU_EMM_CustLoadLBX2()

// WZD s13p10
// UU_EMM_CustomLBXLoad()

// WZD s13p11
// EMM_LBX_EntryLoader()

// WZD s13p12
// EMM_LBX_Load_Picture_Header()

// WZD s13p13
// EMM_LBX_RecLoader()

// WZD s13p14
// EMM_LBXR_DirectLoad()

// WZD s13p15
// EMM_CheckHandleOpen()

// s13p16
void EMM_ReleaseAll__STUB(void)
{
    int16_t itr = 0;  // _SI_
    for(itr = 0; itr < EMM_Open_Handles; itr++)
    {
        EMM_ReleaseHandle__SUTB(EMM_Table[itr].handle);
    }
    if(g_EmmHndl_OVERXYZ != 0)
    {
        EMM_ReleaseHandle__SUTB(g_EmmHndl_OVERXYZ);
    }
}


// WZD s13p17
// EMM_Release_Resd()

// WZD s13p18
// UU_EMM_ReleaseHnd()

// WZD s13p19
// UU_EMM_Available()

// WZD s13p20
// EMM_GetHandle()

// WZD s13p21
// RP_EMM_VGAWrite_fP5()

// WZD s13p22
// RP_EMM_VGARead_fP5()

// WZD s13p23
// RP_EMM_VGAWrite_P5()

// WZD s13p24
// RP_EMM_VGARead_P5()

// WZD s13p25
// EMM_Map_DataH()

// WZD s13p26
// EMM_EMMDATAH_AllocFirst()

// WZD s13p27
// EMM_EMMDATAH_AllocNext()

// WZD s13p28
// UU_EMM_Data_Mark()

// WZD s13p29
// UU_EMM_Data_Undo()

// WZD s13p30
// UU_EMM_LBX_Load2Hnd()

// WZD s13p31
void EMM_Set_Minimum(int amount)
{
    EMM_MinKB = amount;
}

// WZD s13p32
void EMM_GetLowString(char * string)
{
    char temp[20];
    itoa(EMM_MinKB, temp, 10);
    strcpy(string, cnst_EMMErr_TooLow1);
    strcat(string, temp);
    strcat(string, cnst_EMMErr_TooLow2);
}
