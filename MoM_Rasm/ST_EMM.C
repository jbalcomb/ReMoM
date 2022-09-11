
/*
    BCpp31PG, pg 175/190
        #pragma warn -yyy
    BCpp31UG, pg 162/171
    BCpp30UG, pg 158/167
        -waus*  Identifier is assigned a value that is never used.
        -wccc*  Condition is always true/false.
        -weff*  Code has no effect.
        -wpar*  Parameter parameter is never used.
        -wuse   Identifier is declared but never used.
        -wrpt*  Nonportable pointer conversion.
*/
#pragma warn -aus
#pragma warn -eff
#pragma warn -par


#include <DOS.H>

#include <stdlib.h>  /* itoa() */
#include <stdio.h>   /* printf() */
#include <string.h>  /* strcat(), strcpy() */


#include "ST_HEAD.H"
#include "ST_TYPE.H"
#include "MOM_DEF.H"
#include "ST_EMM.H"
#include "ST_FLIC.H"
#include "ST_LBX.H"
#include "ST_SA.H"

#include "STU_DBG.H"


// TODO(jbalcomb): move these scraps of notes
//memptr		bufferseg;

/*
https://fresh2refresh.com/c-programming/c-passing-struct-to-function/
struct student 
{
            int id;
            char name[20];
            float percentage;
};
struct student record; // Global declaration of structure
*/


/*
    ? `static` at module-scope ?
        seg012:0006  EMM_Logical_Page dw 0                   ; DATA XREF: EMM_MapnRead, EMM_MapnWrite
*/
unsigned int EMM_Logical_Page;

char EmmHndlNm_FIGUREX[9] = "FIGUREX";      // dseg:3846
char EmmHndlNm_TILEXXX[9] = "TILEXXX";      // dseg:384E
char EmmHndlNm_CONTXXX[9] = "CONTXXX";      // dseg:3856

unsigned int g_EmmHndl_CONTXXX;             // dseg:68F6
unsigned int g_EmmHndl_TILEXXX;             // dseg:9484
unsigned int g_EmmHndl_FIGUREX;             // dseg:9486


/*
    EMM - Initialized
        ? ALL Xref seg012 and seg013 ?
*/

unsigned int EMM_PageFrame_Base_Address;   // dseg:40E4
char EMM_device_name[9] = "EMMXXXX0";                      // dseg:40E6
// //dseg:40EF EMM_Log2Phys_Map EMM_L2P_Map_Record <0, 0>
// //dseg:40EF                  EMM_L2P_Map_Record <0, 1>
// //dseg:40EF                  EMM_L2P_Map_Record <0, 2>
// //dseg:40EF                  EMM_L2P_Map_Record <0, 3>
// // dseg:40EF g_EMM_Log2Phys_Map EMS_L2P_Map_Record <0, 0> ; This is not in ORION.
// // dseg:40EF EMS_L2P_Map_Record <0,      1>
// // dseg:40EF EMS_L2P_Map_Record <0,      2>
// // dseg:40EF EMS_L2P_Map_Record <0,      3>
// // dseg:40FF align 2
// unsigned int g_EMM_Log2Phys_Map[8] = {0,0,0,1,0,2,0,3}; // dseg:40EF
// C:\devel\STU-MoM_Rasm--rubbish\MoM_Rasm\POCEMS\emstulkt\DISKALL\EMMLIB.H
// MAP_STRUCT far * ptr_map_struct;
//MAP_STRUCT _FAR gfp_map_struct[4] = {{0,0},{0,1},{0,2},{0,3}}; // dseg:40EF
MAP_STRUCT gfp_map_struct[4] = {{0,0},{0,1},{0,2},{0,3}};

// align 2                                                      // dseg:40FF
int g_EMM_Pages_Reserved = 40;                         // dseg:4100 ; set to 158 at the start of _main ? 40 pages = 640KB ? WTF ?
unsigned int g_EMM_Open_Handles = 0;                            // dseg:4102
char *g_EmmHndlNm_YOMOMA1 = "YO MOMA";                          // dseg:4104 "YO MOMA"
char *g_EmmHndlNm_YOMOMA2 = "YO MOMA";                          // dseg:410C "YO MOMA"
char *g_EmmHndlNm_VGAFILEH = "VGAFILEH";                        // dseg:4116 "VGAFILEH"
char *g_EmmHndlNm_EMMDATAH = "EMMDATAH";                        // dseg:411F "EMMDATAH"
char cnst_LBX_Ext2[] = ".LBX";                                  // dseg:4128
char *cnst_EmmErr_ResOut1 = "EMM reserved exceeded by ";        // dseg:412D
char *cnst_EmmErr_ResOut2 = " blocks [";                        // dseg:4147
char *cnst_EmmErr_ResOut3 = ".LBX]";                            // dseg:4151
char *cnst_EmmErr_Reload1 = " reloaded into EMM, diff size =";  // dseg:4157
char *cnst_EmmErr_Space = "  ";                                 // dseg:4177

/*
    EMM - Uninitialized
        ? ALL Xref seg012 and seg013 ?
*/

unsigned int g_EMM_Overlay_Handle;                               // dseg:A5E2
unsigned int g_EMMDATAH_Level;                                    // dseg:A5E4  set to 0 in EMM_Startup
unsigned int g_EmmHndlNbr_VGAFILEH;                               // dseg:A5EA  5 reserved pages
unsigned int g_EmmHndlNbr_EMMDATAH;                               // dseg:A5E6  4 reserved pages
unsigned int g_EmmHndlNbr_YOMOMA;                                 // dseg:A5EE  1 page
unsigned int g_EMM_OK;                                            // dseg:A5F0  set to 1 after creating the YO MOMA handle
//extern EMM_Table EMM_Record 28h dup(<0>)                      // dseg:A5F2  EMM_Table EMM_Record 28h dup(<0>)
EMM_Record g_EMM_Table[MOM_EMM_HANDLES]; // dseg:A5F2 g_EMS_Table EMS_Record 28h dup(<0>)
unsigned int g_EMM_MinKB = 0;                                     // dseg:A7D2  initialized to 0 set to 2700 in _main



/*
    MGC  seg012
*/

// s12p01
/*
; checks for the presence of a LIM EMS 4+ manager, and
; if present, gets and sets the page frame address
; returns -1 if successful, 0 on failure
*/
int EMM_Init(void)
{
    union  REGS  inregs1;
    union  REGS  outregs1;
    struct SREGS segregs1;
    int          result1;
    union  REGS  inregs2;
    union  REGS  outregs2;
    struct SREGS segregs2;
    int          result2;
    union  REGS  inregs3;
    union  REGS  outregs3;
    struct SREGS segregs3;
    int          result3;
    union  REGS  inregs4;
    union  REGS  outregs4;
    struct SREGS segregs4;
    int          result4;
    union  REGS  inregs5;
    union  REGS  outregs5;
    struct SREGS segregs5;
    int          result5;
    union  REGS  inregs6;
    union  REGS  outregs6;
    struct SREGS segregs6;
    int          result6;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_Init()\n", __FILE__, __LINE__);
#endif

    // // //inregs1.x.dx = EMM_device_name;
    // // //inregs1.h.dh = FP_SEG(EMM_device_name);
    // // //inregs1.h.dl = FP_OFF(EMM_device_name);
    // // segregs1.ds = FP_SEG(EMM_device_name);
    // // inregs1.x.dx = FP_OFF(EMM_device_name);
    // // //inregs1.x.dx = (unsigned int)&EMM_device_name;
    // // inregs1.h.ah = 0x3D;  // INT 21,3D - Open File Using Handle
    // // inregs1.h.al = 0x00;  // open access mode: read only
    // // result1 = int86x(DOS_INT, &inregs1, &outregs1, &segregs1);
    // // //#define CF 1 /* Carry flag */
    // asm {
    //     mov dx, offset EMM_device_name
    //     mov ax, 3D00h
    //     int 21h
    // }
    // if ( (_FLAGS & 1) == 0 )
    // {
    //     HERE("FAILURE: INT 21,3D,00 EMM_device_name");
    //     goto Error;
    // }
    inregs1.x.dx = (unsigned int)&EMM_device_name;
    inregs1.h.ah = 0x3D;  // INT 21,3D - Open File Using Handle
    inregs1.h.al = 0x00;  // open access mode: read only
    result1 = int86(DOS_INT, &inregs1, &outregs1);
    if ( outregs1.x.cflag != 0 )
    {
        HERE("FAILURE: INT 21,3D,00 EMM_device_name");
        goto Error;
    }

    inregs2.x.bx = outregs1.x.ax;  // File Handle
    inregs2.h.ah = 0x44;  // INT 21,44 - I/O Control for Devices (IOCTL)
    inregs2.h.al = 0x00;  // function value: IOCTL,0 - Get Device Information
    result2 = int86(DOS_INT, &inregs2, &outregs2);
    if ( outregs2.x.cflag != 0 )
    {
        HERE("FAILURE: INT 21,44,00");
        goto Error;
    }

    if ( outregs2.x.dx & 0x80 == 0 )  // ; test bit 7; 0 == File, 1 == Device
    {
        HERE("FAILURE:  INT 21,44,00: *device* is \"disk file\", not \"character device\" ");
        goto Error;
    }

    inregs3.x.bx = outregs1.x.ax;  // File Handle
    inregs3.h.ah = 0x44;  // INT 21,44 - I/O Control for Devices (IOCTL)
    inregs3.h.al = 0x07;  // IOCTL,7 - Get Output Status
    result3 = int86(DOS_INT, &inregs3, &outregs3);
    //if ( (_FLAGS & 1) == 0 )
    if ( outregs3.x.cflag != 0 )
    {
        HERE("FAILURE: INT 21,44,07");
        goto Error;
    }
    if ( outregs3.h.al == 0 )
    {
        HERE("FAILURE: INT 21,44,07: *device* is not ready");
        goto Error;
    }

    inregs4.x.bx = outregs1.x.ax;  // File Handle
    inregs4.h.ah = 0x3E;  // INT 21,3E - Close File Using Handle
    result4 = int86(DOS_INT, &inregs4, &outregs4);
    if ( outregs4.x.cflag != 0 )
    {
        HERE("FAILURE: INT 21,3E");
        goto Error;
    }

    inregs5.h.ah = 0x46;  // INT 67,46 - Get EMM Version (LIM EMS)
    result5 = int86(EMS_INT, &inregs5, &outregs5);
    if ( outregs5.h.ah != 0 )
    {
        HERE("FAILURE: INT 67,46: (Error) Code not \"success\"");
        goto Error;
    }
    if ( outregs5.h.al < 0x40 )  // BCD; >= LIM EMS Version 4.0
    {
        HERE("FAILURE: LIM EMS Version < 4.0");
        goto Error;
    }

    inregs6.h.ah = 0x41;  // INT 67,41 - Get Page Frame Base Address (LIM EMS 3.0+)
    result6 = int86(EMS_INT, &inregs6, &outregs6);
    if ( outregs6.h.ah != 0 )
    {
        HERE("FAILURE: INT 67,41: (Error) Code not \"success\"");
        goto Error;
    }
    else
    {
        EMM_PageFrame_Base_Address = outregs6.x.bx;  // BX = segment of page frame (PFBA) ...where in the 1Mb memory address the page frame will be mapped
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: EMM_Init() { EMM_PageFrame_Base_Address = 0x%04X }\n", __FILE__, __LINE__, EMM_PageFrame_Base_Address);
#endif

    return -1;  // ST_SUCCESS

Error:
#ifdef DEBUG
    dlvfprintf("DEBUG: %s %d END: EMM_Init()\n", __FILE__, __LINE__);
#endif

    return 0;  // ST_FAILURE
}

// _s12p02c.c EMM_GetHandleCount

#include "ST_HEAD.H"
#include "MOM_DEF.H"
#include "ST_EMM.H"


unsigned int EMM_GetActiveHandleCount(void)
{
    union  REGS  inregs;
    union  REGS  outregs;
    struct SREGS segregs;
    int          result;
    unsigned int emm_handle_count;

    inregs.h.ah = 0x4B;
    int86(EMS_INT, &inregs, &outregs);

    emm_handle_count = outregs.x.bx;

    return(emm_handle_count);
}

// s12p03
unsigned int EMM_GetFreePageCount(void)
{
    union  REGS  inregs;
    union  REGS  outregs;
    struct SREGS segregs;
    int          result;
    unsigned int emm_free_page_count;

    inregs.h.ah = EMS_GETPAGES;
    int86(EMS_INT, &inregs, &outregs);

    if ( outregs.h.ah != 0 )
    {
        emm_free_page_count = 0;
    }
    else
    {
        emm_free_page_count = outregs.x.bx;
    }

    return(emm_free_page_count);
}

// s12p04
unsigned int EMM_GetHandlePageCount(unsigned int EmmHndlNbr)
{
    union  REGS  inregs;
    union  REGS  outregs;
    struct SREGS segregs;
    int          result;
    unsigned int emm_handle_page_count;

    inregs.x.dx - EmmHndlNbr;
    inregs.h.ah = EMS_GETHNDLPAGECNT;
    int86(EMS_INT, &inregs, &outregs);

    if ( outregs.h.ah != 0 )
    {
        emm_handle_page_count = 0;
    }
    else
    {
        emm_handle_page_count = outregs.x.bx;
    }

    return(emm_handle_page_count);
}

// s12p05
/*
; attempts to allocate the indicated amount of pages
; to an EMM handle, and set its name
; returns the handle if successful, 0 otherwise
*/
unsigned int EMM_MakeNamedHandle(unsigned int argPageCount, char _FAR * argHandleName)
{
    union  REGS  inregs1;
    union  REGS  outregs1;
    struct SREGS segregs1;
    int          result1;
    union  REGS  inregs2;
    union  REGS  outregs2;
    struct SREGS segregs2;
    int          result2;
    unsigned int varEmmHandle;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_MakeNamedHandle(argPageCount = %u, argHandleName = %s)\n", __FILE__, __LINE__, argPageCount, argHandleName);
#endif

    inregs1.h.ah = 0x43;  // INT 67,43 - Get Handle and Allocate Pages (LIM EMS 3.2+)
    inregs1.x.bx = argPageCount;
    result1 = int86(EMS_INT, &inregs1, &outregs1);
    if ( outregs1.h.ah != 0 )
    {
        HERE("FAILURE: INT 67,43: (Error) Code not \"success\"");
        dlvfprintf("DEBUG: [%s, %d] outregs1.h.ah: 0x%02X\n", __FILE__, __LINE__, outregs1.h.ah);
        goto Error;
    }
    else
    {
        varEmmHandle = outregs1.x.dx;
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] varEmmHandle: %u\n", __FILE__, __LINE__, varEmmHandle);
#endif
    }
    // ; TODO(JimBalcomb): add code to branch around DOSBox bug - if running in DOSBOX...
    // ;lds si, [bp+argHandleName]
    // les di, [bp+argHandleName]  ; BUG: DOSBox uses the wrong segment and index registers (DOSBox, DOSBox-X, DOSBox-Staging)
    // ;lds si, argHandleName
    // ;;mov dx, si
    // ;;mov ah, 09h
    // ;;int 21h

    inregs2.h.ah = 0x53;  // INT 67,53 Get/Set Handle Name (LIM EMS 4.0+)
    inregs2.h.al = 0x01;  // set handle name
    inregs2.x.dx = varEmmHandle;
    result2 = int86(EMS_INT, &inregs2, &outregs2);
    if ( outregs2.h.ah != 0 )
    {
        HERE("FAILURE: INT 67,53,01: (Error) Code not \"success\"");
        dlvfprintf("DEBUG: [%s, %d] outregs2.h.ah: 0x%02X\n", __FILE__, __LINE__, outregs2.h.ah);
        goto Error;
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: EMM_MakeNamedHandle(argPageCount = %u, argHandleName = %s) { varEmmHandle = %u }\n", __FILE__, __LINE__, argPageCount, argHandleName, varEmmHandle);
#endif

    return varEmmHandle;

Error:
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: EMM_MakeNamedHandle(argPageCount = %u, argHandleName = %s)\n", __FILE__, __LINE__, argPageCount, argHandleName);
#endif

    return 0;  // ST_FAILURE
}

// _s12p06
unsigned int EMM_GetHandleName(char * EmmHndlNm, unsigned int EmmHndlNbr)
{
    union  REGS  inregs;
    union  REGS  outregs;
    struct SREGS segregs;
    //int isr_result;
    int ret_val;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: EMM_GetHandleName(EmmHndlNm = 0, EmmHndlNbr = %u)\n", __FILE__, __LINE__, EmmHndlNbr);
#endif

    inregs.x.dx = EmmHndlNbr;
    segregs.es = FP_SEG(EmmHndlNm);
    inregs.x.di = FP_OFF(EmmHndlNm);
    inregs.h.ah = 0x53;  // INT 67,53 Get/Set Handle Name (LIM EMS 4.0+)
    inregs.h.al = 0x00;  // 0 = Get, 1 = Set
    int86x(EMS_INT, &inregs, &outregs, &segregs);

    if (outregs.h.ah != 0x00)
    {
        ret_val = 0x00;
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: EMM_GetHandleName(EmmHndlNm = 0, EmmHndlNbr = %u) { ret_val = %d }\n", __FILE__, __LINE__, EmmHndlNbr, ret_val);
#endif
    }
    else
    {
        EmmHndlNm[8] = '\0';
        ret_val = 0xFF;
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: EMM_GetHandleName(EmmHndlNm = %s, EmmHndlNbr = %u) { ret_val = %d }\n", __FILE__, __LINE__, EmmHndlNm, EmmHndlNbr, ret_val);
#endif
    }

    return ret_val;
}

// s12p07
void EMM_ReleaseHandle(unsigned int EMM_Handle)
{
    union  REGS  inregs;
    union  REGS  outregs;
    struct SREGS segregs;
    int          result;
//    printf("DEBUG: %s %d BEGIN: EMM_ReleaseHandle()\n", __FILE__, __LINE__);

    if (EMM_Handle != 0)
    {
        inregs.x.dx = EMM_Handle;
        inregs.h.ah = EMS_FREEPAGES;
        int86(EMS_INT, &inregs, &outregs);
    }

//    printf("DEBUG: %s %d END: EMM_ReleaseHandle()\n", __FILE__, __LINE__);
}

// s12p08
/*
    segment:offset  ->  EMM Logical Page
        asm mov ax, [Src_Ofst]
        asm mov dx, [Src_Sgmt]
        asm shl ax, 1
        asm rcl dx, 1
        asm shl ax, 1
        asm rcl dx, 1
        asm mov bx, dx
        asm mov [EmmLogicalPage], bx
        asm shr ax, 1
        asm shr ax, 1
        asm mov si, ax

    e.g., EMM_LBX_EntryLoader -> EMM_MapnRead(LbxFileType, 0, 6, 0, 2, EmmHandle);
    Src_Ofst = 6
    Src_Sgmt = 0
    .:. EMM Logical Page = 0
    SHL 11111111  ==> 11111110 & CF=1
    SHL 01111111  ==> 11111110 & CF=0
    RCL 11111111 & CF=0 ==> 11111110 & CF=1
    RCL 11111111 & CF=1 ==> 11111111 & CF=1
    RCL 01111111 & CF=0 ==> 11111110 & CF=0
    RCL 01111111 & CF=1 ==> 11111111 & CF=0
    .:.
    `:` DX:AX == segment:offset
    SHL AX ; RCL DX
    SHL AX 11111111  ==> 11111110 & CF=1
    RCL DX 00000000  ==> 00000001 & CF=0
    ~= if AX >= 10000000 then AX = (AX * 2) and DX = (DX + 1)
                (32,768)                             (paragraph)
    ...
    Now, why twice? Because 2*32768 = 65536 (64 K) (4 EMM Pages)?

        DX:AX  00000000 00000000 : 00000000 00000000
                                ^^ < ^^
                                    00000000 = 0000     0
                                    01000000 = 4000 16384
                                    10000000 = 8000 32768
                                    11000000 = C000 49152 = (32768 + 16384)
                                00 = 0
                                01 = 1
                                10 = 2
                                11 = 3
    ? In-Game Example ?
    ? need an entry that starts after 64 KB in a LBX that uses EMM_Load_LBX_File()?
    20211217: NOTE(JimBalcomb): MAINSCRN.LBX, Entry Index 5 triggers this, with '0002:5910' (I'm assuming 6 would as well)

                                        EduMat:
                                                J:\STU-EduMat\Asm\rotate\174l7.pdf
                                                https://web.archive.org/web/20200626222103/http://mathforum.org/dr.math/faq/faq.peasant.html
                                                J:\STU\DBWD\developc\wolf3d\WOLFSRC\ID_PM.C
                                                    PML_MapEMS(page / PMEMSSubPage,emspage);
                                                    ...
                                                    #define	PMEMSSubPage	(EMSPageSize / PMPageSize)
                                                    #define	EMSPageSize		16384
                                                    #define	PMPageSize		4096
                                                http://qbrpgs.blarg.ca/qbchronicles/issue4.html
                                                    Page = DataRead& \ 16384            'Determine the EMS page and
                                                    Offset = DataRead& - Page * 16384&  '  offset to load the data into.
                                                    Page = Page + (Slot - 1) * 4        'Adjust the page depending on which slot we're using.
                                                    ...
                                                    DataRead& + BufferSize
                                                    DataRead& = 0                       'We haven't read any data yet...
                                                    BufferSize = 1024
                                                    ...
                                                    So how do we know which logical pages to map to the page frame? Well, because each sound slot is 4 pages, we can use:
                                                    EMS.MapXPages 0, (Slot - 1) * 4, 4, Handle
                                                    (We have to subtract 1 from the slot number because the sound slots are numbered starting at 1, while the EMS pages are numbered starting from 0.)
                                                    SEE: Figure 2-4: Mapping the sound to play to the page frame

*/
// TODO(JimBalcomb): rename 'Src_Sgmt' - it's not quite right, as the actual 'Source Segment' is always EMM_PageFrame_Base_Address
void EMM_MapnRead(unsigned int Dst_Ofst, unsigned int Dst_Sgmt, unsigned int Src_Ofst, unsigned int Src_Sgmt, int nbytes, int EmmHandle)
{
    union  REGS  inregs1;
    union  REGS  outregs1;
    struct SREGS segregs1;
    int          result1;
    union  REGS  inregs2;
    union  REGS  outregs2;
    struct SREGS segregs2;
    int          result2;
    union  REGS  inregs3;
    union  REGS  outregs3;
    struct SREGS segregs3;
    int          result3;
    union  REGS  inregs4;
    union  REGS  outregs4;
    struct SREGS segregs4;
    int          result4;

    int EmmLogicalPage;
    //unsigned int tmp_Src_Ofst;
    unsigned int EmmLogicalPageOfst;
    unsigned int tmp_Src_Sgmt;
    unsigned char _FAR *fptr_Dst;
    unsigned char _FAR *fptr_Src;
    int itr_nbytes;

    // NOTE: printf() clobbers AX, CX, and DX
//#ifdef DEBUG
//    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_MapnRead(Dst_Ofst=0x%04X, Dst_Sgmt=0x%04X, Src_Ofst=0x%04X, Src_Sgmt=0x%04X, nbytes=%d, EmmHandle=%d)\n", __FILE__, __LINE__, Dst_Ofst, Dst_Sgmt, Src_Ofst, Src_Sgmt, nbytes, EmmHandle);
//#endif

    // If a Destination SEGMENT is not specified, assume it is the DATA SEGMENT
    if ( Dst_Sgmt == 0 )
    {
        Dst_Sgmt = _DS;
        //Dst_Sgmt = _DATA;
        //Dst_Sgmt = DGROUP;
        // ? Dst_Sgmt = @data;
    }

    // /*
    //     Shift-Left, twice, Src_Sgmt
    //     Set bits 1 & 0 in Src_Sgmt, based on bits 16 & 15 in Src_Ofst
    // */
    // if ( Src_Ofst & 0xC000 )        // 1100000000000000
    // {
    //     dlvfprintf("DEBUG: [%s, %d]: bit #16 or bit #15 would carry Src_Ofst: 0x%04X\n", __FILE__, __LINE__, Src_Ofst);
    //     // Src_Sgmt = ( (Src_Sgmt << 2) & 0x0003    // 0000000000000011
    //     if ( ( Src_Ofst & 0x8000 ) && ( Src_Ofst & 0x4000 ) )        // 1100000000000000
    //     {
    //         dlvfprintf("DEBUG: [%s, %d]: bit #16 or bit #15 would carry Src_Ofst: 0x%04X\n", __FILE__, __LINE__, Src_Ofst);
    //         // Src_Sgmt = ( (Src_Sgmt << 2) & 0x0003    // 0000000000000011
    //     }
    //     else if ( Src_Ofst & 0x8000 )   // 1000000000000000
    //     {
    //         dlvfprintf("DEBUG: [%s, %d]: bit #16 would carry Src_Ofst: 0x%04X\n", __FILE__, __LINE__, Src_Ofst);
    //         // Src_Sgmt = ( (Src_Sgmt << 2) & 0x0002    // 0000000000000010
    //     }
    //     else if ( Src_Ofst & 0x4000 )   // 0100000000000000
    //     {
    //         dlvfprintf("DEBUG: [%s, %d]: bit #15 would carry Src_Ofst: 0x%04X\n", __FILE__, __LINE__, Src_Ofst);
    //         // Src_Sgmt = ( (Src_Sgmt << 2) & 0x0001    // 0000000000000001
    //     }
    //     // if DEBUG
    //     // getch();
    // }

    // TODO(JimBalcomb,20220724): add debug code here to pop-off when the bits are actually set
    tmp_Src_Sgmt = Src_Sgmt;
    tmp_Src_Sgmt = (tmp_Src_Sgmt << 2);
    if ( (Src_Ofst & 0x8000) != 0 )                        // test bit #16 of Src_Ofst
    {
        tmp_Src_Sgmt = (tmp_Src_Sgmt | 0x0002);     // set bit #2 of Src_Sgmt
    }
    if ( (Src_Ofst & 0x4000) != 0 )                        // test bit #15 of Src_Ofst
    {
        tmp_Src_Sgmt = (tmp_Src_Sgmt | 0x0001);     // set bit #1 of Src_Sgmt
    }
    EmmLogicalPage = tmp_Src_Sgmt;
    EmmLogicalPageOfst = (Src_Ofst & 0x3FFF);       // bits #16 and #15
//#ifdef DEBUG
//    dlvfprintf("DEBUG: [%s, %d] EmmLogicalPage: %d\n", __FILE__, __LINE__, EmmLogicalPage);
//    dlvfprintf("DEBUG: [%s, %d] EmmLogicalPageOfst: 0x%04X\n", __FILE__, __LINE__, EmmLogicalPageOfst);
//#endif

    inregs1.h.al = 0;
    inregs1.x.bx = EmmLogicalPage + 0;
    inregs1.x.dx = EmmHandle;
    inregs1.h.ah = EMS_MAPPAGE;
    int86(EMS_INT, &inregs1, &outregs1);
    inregs2.h.al = 1;
    inregs2.x.bx = EmmLogicalPage + 1;
    inregs2.x.dx = EmmHandle;
    inregs2.h.ah = EMS_MAPPAGE;
    int86(EMS_INT, &inregs2, &outregs2);
    inregs3.h.al = 2;
    inregs3.x.bx = EmmLogicalPage + 2;
    inregs3.x.dx = EmmHandle;
    inregs3.h.ah = EMS_MAPPAGE;
    int86(EMS_INT, &inregs3, &outregs3);
    inregs4.h.al = 3;
    inregs4.x.bx = EmmLogicalPage + 3;
    inregs4.x.dx = EmmHandle;
    inregs4.h.ah = EMS_MAPPAGE;
    int86(EMS_INT, &inregs4, &outregs4);

    /*
    _ES = Dst_Sgmt;
    _DI = Dst_Ofst;
    _DS = EMM_PageFrame_Base_Address;
    _SI = Src_Ofst;
    */
    fptr_Dst = (unsigned char _FAR *) MK_FP(Dst_Sgmt, Dst_Ofst);
    fptr_Src = (unsigned char _FAR *) MK_FP(EMM_PageFrame_Base_Address, EmmLogicalPageOfst);
//#ifdef DEBUG
//    dlvfprintf("DEBUG: [%s, %d] fptr_Dst: %Fp\n", __FILE__, __LINE__, fptr_Dst);
//    dlvfprintf("DEBUG: [%s, %d] fptr_Src: %Fp\n", __FILE__, __LINE__, fptr_Src);
//#endif

    for ( itr_nbytes = 0; itr_nbytes < nbytes; itr_nbytes++ )
    {
        // // *fptr_Dst++ = *fptr_Src++;
        //dlvfprintf("DEBUG: [%s, %d] fptr_Src[%u]: 0x%02X\n", __FILE__, __LINE__, itr_nbytes, fptr_Src[itr_nbytes]);
        fptr_Dst[itr_nbytes] = fptr_Src[itr_nbytes];
        //dlvfprintf("DEBUG: [%s, %d] fptr_Dst[%u]: 0x%02X\n", __FILE__, __LINE__, itr_nbytes, fptr_Dst[itr_nbytes]);
    }

//#ifdef DEBUG
//    dlvfprintf("DEBUG: [%s, %d] END: EMM_MapnRead(Dst_Ofst=0x%04X, Dst_Sgmt=0x%04X, Src_Ofst=0x%04X, Src_Sgmt=0x%04X, nbytes=%d, EmmHandle=%d)\n", __FILE__, __LINE__, Dst_Ofst, Dst_Sgmt, Src_Ofst, Src_Sgmt, nbytes, EmmHandle);
//#endif

}

// _s12p10
unsigned int EMM_GetPageFrame(void)
{
    return EMM_PageFrame_Base_Address;
}

// s12p11
void EMM_Map4(int EmmHandle, int EmmLogicalPage)
{
    union  REGS  inregs1;
    union  REGS  outregs1;
    union  REGS  inregs2;
    union  REGS  outregs2;
    union  REGS  inregs3;
    union  REGS  outregs3;
    union  REGS  inregs4;
    union  REGS  outregs4;

//#ifdef DEBUG
//    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_Map4(EmmHandle=%d, EmmLogicalPage=%d)\n", __FILE__, __LINE__, EmmHandle, EmmLogicalPage);
//#endif

    inregs1.x.dx = EmmHandle;
    inregs1.x.bx = EmmLogicalPage;
    inregs1.h.ah = 0x44;
    inregs1.h.al = 0x00;
    int86(EMS_INT, &inregs1, &outregs1);

    inregs2.x.dx = EmmHandle;
    inregs2.x.bx = EmmLogicalPage + 1;
    inregs2.h.ah = 0x44;
    inregs2.h.al = 0x01;
    int86(EMS_INT, &inregs2, &outregs2);

    inregs3.x.dx = EmmHandle;
    inregs3.x.bx = EmmLogicalPage + 2;
    inregs3.h.ah = 0x44;
    inregs3.h.al = 0x02;
    int86(EMS_INT, &inregs3, &outregs3);

    inregs4.x.dx = EmmHandle;
    inregs4.x.bx = EmmLogicalPage + 3;
    inregs4.h.ah = 0x44;
    inregs4.h.al = 0x03;
    int86(EMS_INT, &inregs4, &outregs4);
    
//#ifdef DEBUG
//    dlvfprintf("DEBUG: [%s, %d] END: EMM_Map4(EmmHandle=%d, EmmLogicalPage=%d)\n", __FILE__, __LINE__, EmmHandle, EmmLogicalPage);
//#endif

}

// s12p12
void EMM_MapMulti4(int argFirstLogicalPage, int argEmmHandle)
{
    union  REGS  inregs;
    union  REGS  outregs;
    struct SREGS segregs;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_MapMulti4(argFirstLogicalPage=%d, argEmmHandle=%d)\n", __FILE__, __LINE__, argFirstLogicalPage, argEmmHandle);
#endif

    gfp_map_struct[0].log_page = argFirstLogicalPage + 0;
    gfp_map_struct[1].log_page = argFirstLogicalPage + 1;
    gfp_map_struct[2].log_page = argFirstLogicalPage + 2;
    gfp_map_struct[3].log_page = argFirstLogicalPage + 3;

    segregs.ds = FP_SEG(gfp_map_struct);
    inregs.x.si = FP_OFF(gfp_map_struct);  // DS:SI = pointer to mapping array
    inregs.x.dx = argEmmHandle;
    inregs.x.cx = 0x04;  // CX = number of entries in array
    inregs.h.al = 0x00;  // 00 Map/unmap pages
    inregs.h.ah = 0x50;  // INT 67,50 - Map/Unmap Multiple Handle Pages (LIM EMS 4.0+)
    int86(EMS_INT, &inregs, &outregs);

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: EMM_MapMulti4(argFirstLogicalPage=%d, argEmmHandle=%d)\n", __FILE__, __LINE__, argFirstLogicalPage, argEmmHandle);
#endif
}





/*
    MGC  seg013
*/

// s13p01
/*
    checks for EMS
    checks if any active handles are named "YO MOMA"
    sets that handle number or gets a new one and updates g_EMM_Open_Handles
    sets g_EMM_OK
    adds additional handles to g_EMM_Table and updates g_EMM_Open_Handles
    gets handles for VGAFILEH and EMMDATAH (and sets g_EMMDATAH_Level=0)
*/
void EMM_Startup(void)
{
    int status_emm_init;
    int itr_active_handles;
    char varTmpStr100[100];
    int varEmmOpenHandleCount;
    unsigned int itr_EmmHndlNbrs;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_Startup()\n", __FILE__, __LINE__);
#endif

    // if ( !EMM_Init() )
    // {
    //     EMM_BuildEmmErrStr(&varTmpStr100);
    //     Quit(varTmpStr100);
    // }
    status_emm_init = EMM_Init();  // return: {0xFF,0x00}/{-1,0}/{ST_FAILURE,ST_SUCCESS}
    if ( status_emm_init == 0x00)
    {
        HERE("if ( status_emm_init == 0x00)");
        EMM_BuildEmmErrStr(varTmpStr100);
        Quit(varTmpStr100);
    }

    varEmmOpenHandleCount = EMM_GetActiveHandleCount();

// - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

    g_EmmHndlNbr_YOMOMA = 0;

    for (itr_active_handles = 1; ((itr_active_handles <= varEmmOpenHandleCount) && (g_EmmHndlNbr_YOMOMA == 0)); itr_active_handles++)
    {
        if ( EMM_GetHandleName(varTmpStr100, itr_active_handles) )
        {
            if ( stricmp(g_EmmHndlNm_YOMOMA1, varTmpStr100) )
            {
                g_EmmHndlNbr_YOMOMA = itr_active_handles;
            }
        }
    }

// - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

    g_EMM_Open_Handles = 0;

    if (g_EmmHndlNbr_YOMOMA != 0)
    {
        strcpy(g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm, g_EmmHndlNm_YOMOMA1);
        g_EMM_Table[g_EMM_Open_Handles].eEmmRsrvd = 0;
        g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNbr = g_EmmHndlNbr_YOMOMA;
        g_EMM_Open_Handles = 1;
    }
    else
    {
        g_EmmHndlNbr_YOMOMA = EMM_GetHandle(1, g_EmmHndlNm_YOMOMA2, 0);  //   // 1 page/16KB, EmmRsvd=FALSE  NOTE: EMM_GetHandle() increments g_EMM_Open_Handles
    }

// - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

    if ( g_EmmHndlNbr_YOMOMA == 0 )
    {
        HERE("if ( g_EmmHndlNbr_YOMOMA == 0 )");
        EMM_BuildEmmErrStr(varTmpStr100);
        Quit(varTmpStr100);
    }

    g_EMM_OK = 1;

// - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

    itr_EmmHndlNbrs = g_EmmHndlNbr_YOMOMA + 1;

    while (itr_EmmHndlNbrs < 50 && g_EMM_Open_Handles < 40)
    {
        if (EMM_GetHandleName(varTmpStr100, itr_EmmHndlNbrs) != 0 && varTmpStr100[0] != 0)
        {
            g_EMM_Table[g_EMM_Open_Handles].eEmmRsrvd = 0;
            strcpy(g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm, varTmpStr100);
            g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNbr = itr_EmmHndlNbrs;
            g_EMM_Open_Handles++;
        }
        itr_EmmHndlNbrs++;
    }

// - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- - --- -

    if ( EMM_GetFreePageCount() < g_EMM_Pages_Reserved )  // returns the unallocated pages count (not the total pages count)
    {
        HERE("if ( EMM_GetFreePageCount() < g_EMM_Pages_Reserved )");
        EMM_BuildEmmErrStr(varTmpStr100);
        Quit(varTmpStr100);
    }

    g_EmmHndlNbr_VGAFILEH = EMM_GetHandle(5, g_EmmHndlNm_VGAFILEH, 1);  // 5 pages/80KB, EmmRsvd=TRUE
    g_EmmHndlNbr_EMMDATAH = EMM_GetHandle(4, g_EmmHndlNm_EMMDATAH, 1);  // 4 pages/64KB, EmmRsvd=TRUE
    g_EMMDATAH_Level = 0;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: EMM_Startup()\n", __FILE__, __LINE__);
#endif

}

// _s13p03
void EMM_Load_LBX_File_1(char *argLbxFileName)
{
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_Load_LBX_File_1(argLbxFileName=%s)\n", __FILE__, __LINE__, argLbxFileName);
#endif

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] CALL: EMM_Load_LBX_File(argLbxFileName=%s, argEmmRsvd=1);\n", __FILE__, __LINE__, argLbxFileName);
#endif
    EMM_Load_LBX_File(argLbxFileName, 1);

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: EMM_Load_LBX_File_1(argLbxFileName=%s)\n", __FILE__, __LINE__, argLbxFileName);
#endif
}

// s13p04
// TODO(JimBalcomb): figure out why signed-long for LbxFielSize gets made negative, like signed-extended cast to int
int EMM_Load_LBX_File(char *LbxFileName, int Reserved)
{
//     char LBX_FileName[20];
//     long LBX_FileSize;
//     strcpy(LBX_FileName, "MAINSCRN.LBX");
//     LBX_FileSize = DOS_GetFileSize(LBX_FileName);
//     printf("DEBUG: [%s, %d]: LBX_FileSize: %ld (0x%08X)\n", __FILE__, __LINE__, LBX_FileSize, LBX_FileSize);
//     printf("DEBUG: [%s, %d]: g_LbxFileSize: %ld (0x%08X)\n", __FILE__, __LINE__, g_LbxFileSize, g_LbxFileSize);
//     if (LBX_FileSize != 196511 )
//     {
//         printf("DEBUG: [%s, %d]: FAILURE: LBX_FileSize: %ld (0x%08X)\n", __FILE__, __LINE__, LBX_FileSize, LBX_FileSize);
//     }
//     else
//     {
//         printf("DEBUG: [%s, %d]: SUCCESS: LBX_FileSize: %ld (0x%08X)\n", __FILE__, __LINE__, LBX_FileSize, LBX_FileSize);
//     }

    //long LbxFileSize;
    //unsigned long LbxFileSize;
    long LbxFileSize;
    long tmp_LbxFileSize;
    char EmmHndlNm[20];
    char EmmHndlFileName[20];
    long UU_varNbytesRead;
    int EmmLogicalPage;
    int ReadNbytes;
    unsigned int tmp_EmmPageFrameSgmtAddr;
    long LbxFileSize16kBlocks;
    int Handle_Exists;
    int EmmHndlNbr;
    int LbxFileHandle;

    int itr_find_zero;
    int itr_pad_zeros;
    int itr_g_EMM_Open_Handles;
    int tmp_EMM_Pages_Available;
    int tmp_EMM_Pages_Required;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_Load_LBX_File(LbxFileName=%s, EmmRsvd=%d)\n", __FILE__, __LINE__, LbxFileName, Reserved);
#endif

    ExtractFileBase(LbxFileName);
    strcpy(EmmHndlNm, LbxFileName);

    // for ( ; argc > 0 && **argv == '-'; argc--, argv++)
    // while( *dest++ = *source++ );
    // for ( itr_find_zero = 0; *(EmmHndlNm + itr_find_zero) != '\0'; itr_find_zero++ )
    itr_find_zero = 0;
    while( EmmHndlNm[itr_find_zero] != '\0' )
    {
        itr_find_zero++;
    }

//    printf("DEBUG: [%s, %d]: itr_find_zero = %d)\n", __FILE__, __LINE__, itr_find_zero);

    for ( itr_pad_zeros = itr_find_zero; itr_pad_zeros < 9; itr_pad_zeros++ )
    {
        *(EmmHndlNm + itr_pad_zeros) = '\0';
    }

//    printf("DEBUG: [%s, %d]: itr_pad_zeros = %d)\n", __FILE__, __LINE__, itr_pad_zeros);

//    printf("DEBUG: [%s, %d]: EmmHndlNm = %s)\n", __FILE__, __LINE__, EmmHndlNm);

    Handle_Exists = 0;

    itr_g_EMM_Open_Handles = 0;
    while ( itr_g_EMM_Open_Handles < g_EMM_Open_Handles )
    {
//        printf("DEBUG: [%s, %d]: g_EMM_Table[itr_g_EMM_Open_Handles].eEmmHndlNm = %s)\n", __FILE__, __LINE__, g_EMM_Table[itr_g_EMM_Open_Handles].eEmmHndlNm);

        if ( stricmp(g_EMM_Table[itr_g_EMM_Open_Handles].eEmmHndlNm, EmmHndlNm) == 0 )
        {
            Handle_Exists = 1;
        }

        itr_g_EMM_Open_Handles++;
    }

    if ( Handle_Exists != 0 )
    {
//        printf("DEBUG: [%s, %d]: FAILURE: if ( Handle_Exists != 0 )\n", __FILE__, __LINE__);
        LBX_Error(EmmHndlNm, 0x0B, 0);  // LBXErr_release_first ... ' cannot be reloaded into EMS w/o being first released.'
    }

    if ( g_EMM_OK == 0 )
    {
//        printf("DEBUG: [%s, %d]: FAILURE: if ( g_EMM_OK == 0 )\n", __FILE__, __LINE__);
        EmmHndlNbr = 0;
        goto Exit;
    }

    if ( !(g_EMM_Open_Handles < 40) )
    {
//        printf("DEBUG: [%s, %d]: FAILURE: if ( !(g_EMM_Open_Handles < 40) )\n", __FILE__, __LINE__);
        goto Exit;
    }

    strcpy(EmmHndlFileName, EmmHndlNm);
    strcat(EmmHndlFileName, cnst_LBX_Ext2);

    // LbxFileSize = DOS_GetFileSize(EmmHndlFileName);  // TODO(JimBalcomb): maybe spend another 5 hours trying to figure out why this is getting 2-byte sign-extended
    // LbxFileSize = C_GetFileSize(EmmHndlFileName);
    // tmp_LbxFileSize = DOS_GetFileSize(EmmHndlFileName);
    LbxFileSize = DOS_GetFileSize(EmmHndlFileName);
    // if ( tmp_LbxFileSize < 0 )
    // {
    //     printf("DEBUG: [%s, %d]: ERROR: tmp_LbxFileSize is NEGATIVE!! %ld (0x%08X)\n", __FILE__, __LINE__, tmp_LbxFileSize, tmp_LbxFileSize);
    //     //LbxFileSize = ((2^31)*2) + tmp_LbxFileSize
    //     //LbxFileSize = ((2^15)*2) + tmp_LbxFileSize;
    //     LbxFileSize = ((long)(tmp_LbxFileSize & 0xFFFF) << 15);
    // }
    // else
    // {
    //     LbxFileSize = tmp_LbxFileSize;
    // }
//    printf("DEBUG: [%s, %d]: LbxFileSize = %ld (0x%08X)\n", __FILE__, __LINE__, LbxFileSize, LbxFileSize);

    if ( LbxFileSize == 0 )
    {
//        printf("DEBUG: [%s, %d]: FAILURE: LbxFileSize = DOS_GetFileSize(EmmHndlFileName)\n", __FILE__, __LINE__);
        goto Exit;
    }

    LbxFileSize16kBlocks = LbxFileSize / 16384; // SZ_16K_B / EMM Page Size
//    printf("DEBUG: [%s, %d]: LbxFileSize16kBlocks = %ld (0x%08X)\n", __FILE__, __LINE__, LbxFileSize16kBlocks, LbxFileSize16kBlocks);

    if ( LbxFileSize % 16384 != 0 )
    {
        LbxFileSize16kBlocks = LbxFileSize16kBlocks + 1;
    }

    if ( Reserved == 0 )
    {
        // TODO(JimBalcomb): double-check and fix this error here ...tmp_EMM_Pages_Available...if ( !(tmp_EMM_Pages_Required < tmp_EMM_Pages_Required) )
        tmp_EMM_Pages_Available = EMM_GetFreePageCount();
        tmp_EMM_Pages_Required = g_EMM_Pages_Reserved + LbxFileSize16kBlocks;
        if ( !(tmp_EMM_Pages_Required < tmp_EMM_Pages_Required) )
        {
//            printf("DEBUG: [%s, %d]: FAILURE: !(tmp_EMM_Pages_Required < tmp_EMM_Pages_Required)\n", __FILE__, __LINE__, tmp_EMM_Pages_Required, tmp_EMM_Pages_Required);
            EmmHndlNbr = 0;
            goto Exit;
        }
    }

   EmmHndlNbr = EMM_GetHandle(LbxFileSize16kBlocks, EmmHndlNm, Reserved);

   if ( EmmHndlNbr == 0 )
   {
//       printf("DEBUG: [%s, %d]: FAILURE: EmmHndlNbr = EMM_GetHandle(LbxFileSize16kBlocks, EmmHndlNm, Reserved)\n", __FILE__, __LINE__);
       goto Exit;
   }

    tmp_EmmPageFrameSgmtAddr = EMM_GetPageFrame();

    EmmLogicalPage = 0;

    if ( tmp_EmmPageFrameSgmtAddr == 0 )
    {
//        printf("DEBUG: [%s, %d]: FAILURE: tmp_EmmPageFrameSgmtAddr = EMM_GetPageFrame()\n", __FILE__, __LINE__);
        goto Exit;
    }

    LbxFileHandle = lbx_open(EmmHndlFileName);

    if ( LbxFileHandle == 0 )
    {
//        printf("DEBUG: [%s, %d]: FAILURE: LbxFileHandle = lbx_open(EmmHndlFileName)\n", __FILE__, __LINE__);
        goto Exit;
    }

    UU_varNbytesRead = 0;

    ReadNbytes = EMS_PAGE_SIZE; // 16384  SZ_16K_B / EMS/EMM Page Size
    tmp_LbxFileSize = LbxFileSize;

    while ( tmp_LbxFileSize >= EMS_PAGE_SIZE )
    {
        tmp_LbxFileSize -= EMS_PAGE_SIZE; // SZ_16K_B / EMM Page Size

        EMM_Map4(EmmHndlNbr, EmmLogicalPage);

        // printf("DEBUG: [%s, %d]: lbx_read_sgmt(0x%04X, %d, %d)\n", __FILE__, __LINE__, tmp_EmmPageFrameSgmtAddr, ReadNbytes, LbxFileHandle);
        lbx_read_sgmt(tmp_EmmPageFrameSgmtAddr, ReadNbytes, LbxFileHandle);

        EmmLogicalPage++;

        UU_varNbytesRead += EMS_PAGE_SIZE;
    }
    if ( tmp_LbxFileSize > 0 )
    {
        ReadNbytes = tmp_LbxFileSize;
        EMM_Map4(EmmHndlNbr, EmmLogicalPage);
        lbx_read_sgmt(tmp_EmmPageFrameSgmtAddr, ReadNbytes, LbxFileHandle);
    }

    lbx_close(LbxFileHandle);

    // TODO(JimBalcomb,20220724): add DEBUG - if MAINSCRN LbxFileSize == 196511
//#ifdef DEBUG
//    dlvfprintf("DEBUG: [%s, %d] EmmLogicalPage: %d\n", __FILE__, __LINE__, EmmLogicalPage);
//    dlvfprintf("DEBUG: [%s, %d] LbxFileSize: %ld\n", __FILE__, __LINE__, LbxFileSize);
//    dlvfprintf("DEBUG: [%s, %d] UU_varNbytesRead: %ld\n", __FILE__, __LINE__, UU_varNbytesRead);
//    dlvfprintf("DEBUG: [%s, %d] tmp_LbxFileSize: %ld\n", __FILE__, __LINE__, tmp_LbxFileSize);
//    dlvfprintf("DEBUG: [%s, %d] UU_varNbytesRead + tmp_LbxFileSize: %ld\n", __FILE__, __LINE__, (UU_varNbytesRead + tmp_LbxFileSize));
//#endif

//#ifdef DEBUG
//    EMM_Map4(EmmHndlNbr, 0);
//    dlvfprintf("DEBUG: [%s, %d] EMM_PageFrame_Base_Address: 0x%04X\n", __FILE__, __LINE__, EMM_PageFrame_Base_Address);
//    dlvfprintf("DEBUG: [%s, %d] LBX_EntryCount: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrame_Base_Address, 0));
//    dlvfprintf("DEBUG: [%s, %d] LBX_MagSig_Hi: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrame_Base_Address, 2));
//    dlvfprintf("DEBUG: [%s, %d] LBX_MagSig_Lo: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrame_Base_Address, 4));
//    dlvfprintf("DEBUG: [%s, %d] LBX_Type: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrame_Base_Address, 6));
//#endif

Exit:

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: EMM_Load_LBX_File(LbxFileName=%s, EmmRsvd=%d)\n", __FILE__, __LINE__, LbxFileName, Reserved);
#endif

    return EmmHndlNbr;
}

// s13p11
// TODO(JimBalcomb): add DEBUG for status/why/which failure
unsigned int EMM_LBX_Load_Entry(char *EmmHndlNm, int LbxEntry, unsigned int SAMB_head, int LoadType, int FormatType)
{
    //unsigned int tmp_SAMB_head;
    sgmt_addr SAMB_data;
    unsigned int EmmHndl;
    unsigned int LbxEntryCount;
    static unsigned int LbxFileType;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    long EntryTableOffset;
    unsigned int tmp_EntryTableOffset_Sgmt;
    unsigned int tmp_EntryTableOffset_Ofst;
    static long DataOffset_Begin;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    static long DataOffset_End;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    unsigned int tmp_DataOffsetBegin_Sgmt;
    unsigned int tmp_DataOffsetBegin_Ofst;
    unsigned long DataSize_Bytes;
    unsigned long DataSize_Paras;
    unsigned int tmp_SAMB_Size;  // used in LBXLOADTYPE/EMMLBXLOADTYPE
    sgmt_addr tmp_SAMB_data;  // result of LBXLOADTYPE/EMMLBXLOADTYPE
    unsigned int ReadNbytes;
    void * pSAMB_head;
    
#ifdef DEBUG
    unsigned char bDebugDetail = 0;
#endif

#ifdef DEBUG
    if
    (
        ( (strcmp(EmmHndlNm, "FONTS") == 0) && (LbxEntry == 0) ) ||
        ( (strcmp(EmmHndlNm, "FONTS") == 0) && (LbxEntry == 1) ) ||
        ( (strcmp(EmmHndlNm, "MAINSCRN") == 0) && (LbxEntry == 0) ) ||
        ( (strcmp(EmmHndlNm, "MAINSCRN") == 0) && (LbxEntry == 5) )
    )
    {
        bDebugDetail = 1;
    }
#endif

#ifdef DEBUG
    //if (bDebugDetail)
    //{
        dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_LBX_Load_Entry(EmmHndlNm=%s, LbxEntry=%d, SAMB_head=0x%04X, LoadType=%d, FormatType=%d)\n", __FILE__, __LINE__, EmmHndlNm, LbxEntry, SAMB_head, LoadType, FormatType);
    //}
#endif

    SAMB_data = ST_FAILURE;

    EMMLBXHANDLE()

    /*
        LBX Type 0: FLIC  ("pictures")
    */
    if ( g_LBX_EmmRsvd == 1 )
    {
        EMM_MapnRead((unsigned int)&LbxFileType, 0, 6, 0, 2, EmmHndl);
        if ( LbxFileType != 0 )
        {
            LBX_Error(EmmHndlNm, 0x0D, LbxEntry);  // LbxErrNbr = 13, LbxErrIdx = 12; 'LBXErr_cantload_reserved': cnst_LBX_ErrorD = " Only pictures may be loaded into reserved EMM"
        }
        SAMB_data = EMM_LBX_FLIC_Header(EmmHndl, EmmHndlNm, LbxEntry, SAMB_head, LoadType);
#ifdef DEBUG
        dlvfprintf("DEBUG: [%s, %d] g_LBX_EmmRsvd: %d, EmmHndlNm: %s, LbxEntry: %d, SgmtAddr: 0x%04X)\n", __FILE__, __LINE__, g_LBX_EmmRsvd, EmmHndlNm, LbxEntry, SAMB_data);
#endif
        goto Exit;
    }

    EMMLBXENTRYCOUNT()

    /*
        BEGIN: LBX Header Offset
    */
    if ( FormatType != 0 )
    {
        EntryTableOffset = ((LbxEntry * 4) + 520);
    }
    else
    {
        EntryTableOffset = ((LbxEntry * 4) + 8);
    }
    // EntryTableOffset = (LbxEntry * 4) + ( (FormatType != 0) ? 520 : 4 );
    /*
        END: LBX Header Offset
    */

    /*
        BEGIN: Entry - Offset Begin, End, Size
    */

    EMMLBXENTRYOFFSET()

    /*
        END: Entry - Offset Begin, End, Size
    */

    /*
        BEGIN: Load Type
    */
    DataSize_Paras = (DataSize_Bytes / SZ_PARAGRAPH_B);
    if ( (DataSize_Paras & 0x0F) != 0 )     // ~== MOD 16
    {
        DataSize_Paras = DataSize_Paras + 1;
    }
    EMMLBXLOADTYPE()
    /*
        END: Load Type
    */
    tmp_SAMB_data = SAMB_data;
    /*
        BEGIN: Read Data
    */
    EMMLBXREADDATA()
    /*
        END: Read Data
    */
    goto Exit;

Error:
    SAMB_data = ST_FAILURE;

Exit:

#ifdef DEBUG
    //if (bDebugDetail)
    //{
        dlvfprintf("DEBUG: [%s, %d] END: EMM_LBX_Load_Entry(EmmHndlNm=%s, LbxEntry=%d, SAMB_head=0x%04X, LoadType=%d, FormatType=%d) { SAMB_data=0x%04X }\n", __FILE__, __LINE__, EmmHndlNm, LbxEntry, SAMB_head, LoadType, FormatType, SAMB_data);
    //}
#endif
    return SAMB_data;
}

// s13p12
/*
    ? only for LBX Type 0: pictures
    ? only Reserved EMM
    ∴
    ∵
    reads sizeof(LBX_FLIC_HDR)
    populates LBX_FLIC_HDR.EMM_Handle_Number, .EMM_Logical_Page_Number, and .EMM_Logical_Page_Offset
*/
unsigned int EMM_LBX_FLIC_Header(int EmmHndl, char *EmmHndlNm, int LbxEntry, unsigned int SAMB_head, int LoadType)
{
    SAMB_addr SAMB_data;
    static unsigned int LbxEntryCount;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    long EntryTableOffset;
    unsigned int tmp_EntryTableOffset_Sgmt;
    unsigned int tmp_EntryTableOffset_Ofst;
    static long DataOffset_Begin;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    unsigned int tmp_DataOffsetBegin_Sgmt;
    unsigned int tmp_DataOffsetBegin_Ofst;
    unsigned long DataSize_Paras;   // LBXLOADTYPE()
    unsigned int tmp_SAMB_Size;     // LBXLOADTYPE()
    unsigned char Entry_Begin_Logical_Page_Number;
    unsigned int Entry_Begin_Logical_Page_Offset;
    unsigned char *ptr_Byte;
    unsigned char _FAR *fptr_Dst;
    void * pSAMB_head;

#ifdef DEBUG
    unsigned char bDebugDetail = 0;
#endif

#ifdef DEBUG
    if
    (
        ( (strcmp(EmmHndlNm, "MAINSCRN") == 0) && (LbxEntry == 0) ) ||
        ( (strcmp(EmmHndlNm, "MAINSCRN") == 0) && (LbxEntry == 5) )
    )
    {
        bDebugDetail = 1;
    }
#endif

#ifdef DEBUG
    //if (bDebugDetail)
    //{
        dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_LBX_FLIC_Header(EmmHndl=%d, EmmHndlNm=%s, LbxEntry=%d, SAMB_head=0x%04X, LoadType=%d)\n", __FILE__, __LINE__, EmmHndl, EmmHndlNm, LbxEntry, SAMB_head, LoadType);
    //}
#endif
    
    SAMB_data = ST_FAILURE;

    /*
        ? same code as in EMM_LBX_EntryLoader() ?
            LbxEntryCount     =  EMM_MapnRead
            EntryOffsetBegin  =  EMM_MapnRead
    */
    EMM_MapnRead((unsigned int)&LbxEntryCount, 0, 0, 0, 2, EmmHndl);

    if ( LbxEntryCount < LbxEntry )
    {
        LBX_Error(EmmHndlNm, 0x08, LbxEntry);  // LBXErr_entries_exceeded
    }

    /*
        BEGIN: LBX Header Offset
    */
    EntryTableOffset = (8 + (4 * LbxEntry));
    /*
        END: LBX Header Offset
    */

    /*
        BEGIN: LBX Record Offset
    */
    tmp_EntryTableOffset_Sgmt = (unsigned int)(EntryTableOffset >> 16);         // Hi-Word of Double-Word
    tmp_EntryTableOffset_Ofst = (unsigned int)(EntryTableOffset & 0x0000FFFF);  // Lo-Word of Double-Word
    EMM_MapnRead((unsigned int)&DataOffset_Begin, 0, tmp_EntryTableOffset_Ofst, tmp_EntryTableOffset_Sgmt, 4, EmmHndl);
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] (unsigned int)(DataOffset_Begin >> 16): 0x%04X\n", __FILE__, __LINE__, (unsigned int)(DataOffset_Begin >> 16));
    dlvfprintf("DEBUG: [%s, %d] (unsigned int)(DataOffset_Begin & 0x0000FFFF): 0x%04X\n", __FILE__, __LINE__, (unsigned int)(DataOffset_Begin & 0x0000FFFF));
#endif

#ifdef DEBUG
    if (bDebugDetail)
    {
        dlvfprintf("DEBUG: [%s, %d] EntryTableOffset=0x%04X\n", __FILE__, __LINE__, EntryTableOffset);
        dlvfprintf("DEBUG: [%s, %d] tmp_EntryTableOffset_Sgmt=0x%04X\n", __FILE__, __LINE__, tmp_EntryTableOffset_Sgmt);
        dlvfprintf("DEBUG: [%s, %d] tmp_EntryTableOffset_Ofst=0x%04X\n", __FILE__, __LINE__, tmp_EntryTableOffset_Ofst);
        dlvfprintf("DEBUG: [%s, %d] DataOffset_Begin=0x%08lX (%ld) (%lu)\n", __FILE__, __LINE__, DataOffset_Begin, DataOffset_Begin, DataOffset_Begin);
    }
    if (bDebugDetail)
    {
        if
        (
            ( strcmp(EmmHndlNm, "MAINSCRN") == 0 ) &&
            ( LbxEntry == 0 ) &&
            ( DataOffset_Begin != 0x000002C0 )
        )
        {
            dlvfprintf("DEBUG: [%s, %d] FATAL: MAINSCXRN_000: DataOffset_Begin != 0x000002C0 { 0x%08lX }\n", __FILE__, __LINE__, DataOffset_Begin);
        }
        if
        (
            ( strcmp(EmmHndlNm, "MAINSCRN") == 0 ) &&
            ( LbxEntry == 5 ) &&
            ( DataOffset_Begin != 0x00025910 )
        )
        {
            dlvfprintf("DEBUG: [%s, %d] FATAL: MAINSCXRN_005: DataOffset_Begin != 00025910 { 0x%08lX }\n", __FILE__, __LINE__, DataOffset_Begin);
        }
    }
#endif
    /*
        END: LBX Record Offset
    */

    /*
        BEGIN: Load Type
    */
    DataSize_Paras = 1;  // sizeof FLIC_HDR
    EMMLBXLOADTYPE()
    /*
        END: Load Type
    */

#ifdef DEBUG
    if (bDebugDetail)
    {
        dlvfprintf("DEBUG: [%s, %d] SAMB_data=0x%04X\n", __FILE__, __LINE__, SAMB_data);
    }
#endif

    Entry_Begin_Logical_Page_Number = DataOffset_Begin / EMS_PAGE_SIZE;  // 16384 e_EMM_Page_Size (0x4000)
    Entry_Begin_Logical_Page_Offset = DataOffset_Begin % EMS_PAGE_SIZE;  // 16384 e_EMM_Page_Size (0x4000)

#ifdef DEBUG
    if (bDebugDetail)
    {
        // MAINSCRN,0
        // DEBUG: [_s13p12c.c, 115] Entry_Begin_Logical_Page_Number=0 (0x00)
        // DEBUG: [_s13p12c.c, 116] Entry_Begin_Logical_Page_Offset=704 (0x02C0)
        // MAINSCRN,5
        // DEBUG: [_s13p12c.c, 115] Entry_Begin_Logical_Page_Number=9 (0x09)
        // DEBUG: [_s13p12c.c, 116] Entry_Begin_Logical_Page_Offset=6416 (0x1910)
        dlvfprintf("DEBUG: [%s, %d] Entry_Begin_Logical_Page_Number=%u (0x%02X)\n", __FILE__, __LINE__, Entry_Begin_Logical_Page_Number, Entry_Begin_Logical_Page_Number);
        dlvfprintf("DEBUG: [%s, %d] Entry_Begin_Logical_Page_Offset=%u (0x%04X)\n", __FILE__, __LINE__, Entry_Begin_Logical_Page_Offset, Entry_Begin_Logical_Page_Offset);
    }
#endif

    /*
        BEGIN: Read Data - ST FLIC Header
    */
    tmp_DataOffsetBegin_Sgmt = (unsigned int)(DataOffset_Begin >> 16);         // Hi-Word of Double-Word
    tmp_DataOffsetBegin_Ofst = (unsigned int)(DataOffset_Begin & 0x0000FFFF);  // Lo-Word of Double-Word
    EMM_MapnRead(0, SAMB_data, tmp_DataOffsetBegin_Ofst, tmp_DataOffsetBegin_Sgmt, 16, EmmHndl);
    /*
        END: Read Data - ST FLIC Header
    */

//dlvfprintf("DEBUG: [%s, %d] g_LBX_EmmRsvd: %d, EmmHndlNm: %s, LbxEntry: %d, SgmtAddr: 0x%04X, FlicHdr_EmmHandleNumber: %u, FlicHdr_EmmLogicalPageIndex: %u, FlicHdr_EmmLogicalPageOffset: 0x%04X)\n", __FILE__, __LINE__, g_LBX_EmmRsvd, EmmHndlNm, LbxEntry, SAMB_data, EmmHndl, Entry_Begin_Logical_Page_Number, Entry_Begin_Logical_Page_Offset);

#ifdef DEBUG
    if (bDebugDetail)
    {
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_Width=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_Width));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_Height=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_Height));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_CurrentFrame=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_CurrentFrame));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_FrameCount=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_FrameCount));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_LoopFrame=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_LoopFrame));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmHandleNumber=%d\n", __FILE__, __LINE__, farpeekb(SAMB_data, FlicHdr_EmmHandleNumber));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmLogicalPageIndex=%d\n", __FILE__, __LINE__, farpeekb(SAMB_data, FlicHdr_EmmLogicalPageIndex));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmLogicalPageOffset=0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_EmmLogicalPageOffset));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_PaletteDataOffset=0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_PaletteDataOffset));
    }
#endif

    // TODO(JimBalcomb): add defines for offsets and struct for LBX_FLIC_HDR
    farpokeb(SAMB_data, 10, (char)EmmHndl);                          // 10 0x0A LBX_FLIC_HDR.EMM_Handle_Number
    farpokeb(SAMB_data, 11, (char)Entry_Begin_Logical_Page_Number);  // 11 0x0B LBX_FLIC_HDR.EMM_Logical_Page_Number
    farpokew(SAMB_data, 12, Entry_Begin_Logical_Page_Offset);        // 12 0x0C LBX_FLIC_HDR.EMM_Logical_Page_Offset

#ifdef DEBUG
    if (bDebugDetail)
    {
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_Width=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_Width));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_Height=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_Height));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_CurrentFrame=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_CurrentFrame));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_FrameCount=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_FrameCount));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_LoopFrame=%d\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_LoopFrame));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmHandleNumber=%d\n", __FILE__, __LINE__, farpeekb(SAMB_data, FlicHdr_EmmHandleNumber));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmLogicalPageIndex=%d\n", __FILE__, __LINE__, farpeekb(SAMB_data, FlicHdr_EmmLogicalPageIndex));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_EmmLogicalPageOffset=0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_EmmLogicalPageOffset));
        dlvfprintf("DEBUG: [%s, %d] FlicHdr_PaletteDataOffset=0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_PaletteDataOffset));
    }
#endif

#ifdef DEBUG
    //if (bDebugDetail)
    //{
        dlvfprintf("DEBUG: [%s, %d] END: EMM_LBX_FLIC_Header(EmmHndl=%d, EmmHndlNm=%s, LbxEntry=%d, SAMB_head=0x%04X, LoadType=%d) {SAMB_data=0x%04X}\n", __FILE__, __LINE__, EmmHndl, EmmHndlNm, LbxEntry, SAMB_head, LoadType, SAMB_data);
    //}
#endif
    return SAMB_data;
}

// s13p13
// ~= c&p EMM_LBX_Load_Entry()
SAMB_addr EMM_LBX_Load_Record(char *EmmHndlNm, int LbxEntry, SAMB_addr SAMB_head, int LoadType, int RecFirst, int RecCount, int RecSize)
{
    SAMB_addr SAMB_data;
    unsigned int EmmHndl;
    unsigned int LbxEntryCount;
    long EntryTableOffset;
    unsigned int tmp_EntryTableOffset_Sgmt;
    unsigned int tmp_EntryTableOffset_Ofst;
    static long DataOffset_Begin;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    static long DataOffset_End;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    unsigned long DataSize_Bytes;   // LBXENTRYOFFSET()
    unsigned int tmp_DataOffsetBegin_Sgmt;
    unsigned int tmp_DataOffsetBegin_Ofst;
    static int Record_Count;        // LBXRECORDOFFSET()
    static int Record_Size;         // LBXRECORDOFFSET()
    unsigned long DataSize_Paras;   // LBXLOADTYPE()
    unsigned int tmp_SAMB_Size;     // LBXLOADTYPE()
    SAMB_addr tmp_SAMB_data;        // LBXREADDATA()
    unsigned int ReadNbytes;        // LBXREADDATA()
    SAMB_ptr pSAMB_head;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: EMM_LBX_Load_Record(EmmHndlNm = %s, LbxEntry = %d, SAMB_head = 0x%04X, LoadType = %d, RecFirst = %d, RecCount = %d, RecSize = %d)\n", __FILE__, __LINE__, EmmHndlNm, LbxEntry, SAMB_head, LoadType, RecFirst, RecCount, RecSize);
#endif

    //tmp_SAMB_head = SAMB_head;
    SAMB_data = ST_FAILURE;

    EMMLBXHANDLE()

    EMMLBXENTRYCOUNT()

    /*
        BEGIN: LBX Header Offset
    */
    EntryTableOffset = ((4 * LbxEntry) + 8);
    /*
        END: LBX Header Offset
    */

    /*
        BEGIN: Entry - Offset Begin, End, Size
    */
    EMMLBXENTRYOFFSET()
    /*
        END: Entry - Offset Begin, End, Size
    */

    /*
        BEGIN: Records - Offset Begin, Size
    */
    EMMLBXRECORDOFFSET()
    /*
        END: Records - Offset Begin, Size
    */

    /*
        BEGIN: Load Type
    */
    DataSize_Paras = ((DataSize_Bytes / SZ_PARAGRAPH_B) + 1);
    if ( (DataSize_Paras & 0x0F) != 0 )     // ~== MOD 16
    {
        DataSize_Paras = DataSize_Paras + 1;
    }
    EMMLBXLOADTYPE()
    /*
        END: Load Type
    */
    tmp_SAMB_data = SAMB_data;
    /*
        BEGIN: Read Data
    */
    EMMLBXREADDATA()
    /*
        END: Read Data
    */

Error:
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: EMM_LBX_Load_Record(EmmHndlNm = %s, LbxEntry = %d, SAMB_head = 0x%04X, LoadType = %d, RecFirst = %d, RecCount = %d, RecSize = %d) { SAMB_data = ST_FAILURE }\n", __FILE__, __LINE__, EmmHndlNm, LbxEntry, SAMB_head, LoadType, RecFirst, RecCount, RecSize);
#endif
    return ST_FAILURE;
Exit:
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: EMM_LBX_Load_Record(EmmHndlNm = %s, LbxEntry = %d, SAMB_head = 0x%04X, LoadType = %d, RecFirst = %d, RecCount = %d, RecSize = %d) { SAMB_data = 0x%04X }\n", __FILE__, __LINE__, EmmHndlNm, LbxEntry, SAMB_head, LoadType, RecFirst, RecCount, RecSize, SAMB_data);
#endif
    return SAMB_data;
}

// s13p14
// ~= c&p EMM_LBX_Load_Record()
int EMM_LBXR_DirectLoad(char *EmmHndlNm, int LbxEntry, unsigned int SAMB_data, int RecFirst, int RecCount, int RecSize)
{
    unsigned int tmp_LbxEntry;
    sgmt_addr UU_SAMB_data;
    unsigned int EmmHndl;
    unsigned int LbxEntryCount;
    long EntryTableOffset;
    unsigned int tmp_EntryTableOffset_Sgmt;
    unsigned int tmp_EntryTableOffset_Ofst;
    static long DataOffset_Begin;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    static long DataOffset_End;  // `static` just to get it put in the 'Data Segment', for EMM_MapnRead()
    unsigned long DataSize_Bytes;
    unsigned int tmp_DataOffsetBegin_Sgmt;
    unsigned int tmp_DataOffsetBegin_Ofst;
    static int Record_Count;        // LBXRECORDOFFSET()
    static int Record_Size;         // LBXRECORDOFFSET()
    unsigned long DataSize_Paras;   // LBXLOADTYPE()
    unsigned int ReadNbytes;        // LBXREADDATA()

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: EMM_LBXR_DirectLoad(EmmHndlNm = %s, LbxEntry = %d, SAMB_data = 0x%04X, RecFirst = %d, RecCount = %d, RecSize = %d)\n", __FILE__, __LINE__, EmmHndlNm, LbxEntry, SAMB_data, RecFirst, RecCount, RecSize);
#endif

    tmp_LbxEntry = LbxEntry;

    UU_SAMB_data = ST_FAILURE;

    EMMLBXHANDLE()

    EMMLBXENTRYCOUNT()

    /*
        BEGIN: LBX Header Offset
    */
    EntryTableOffset = ((4 * LbxEntry) + 8);
    /*
        END: LBX Header Offset
    */

    /*
        BEGIN: Entry - Offset Begin, End, Size
    */
    EMMLBXENTRYOFFSET()
    /*
        END: Entry - Offset Begin, End, Size
    */

    /*
        BEGIN: Records - Offset Begin, Size
    */
    EMMLBXRECORDOFFSET()
    /*
        END: Records - Offset Begin, Size
    */

    DataSize_Paras = ((DataSize_Bytes / SZ_PARAGRAPH_B) + 1);
    // ...UnUsed, but as seen elsewhere for use in 'Load Type'

    /*
        BEGIN: Read Data
    */
    // EMMLBXREADDATA()
    ReadNbytes = DataSize_Bytes % SZ_32K_B;
    tmp_DataOffsetBegin_Sgmt = (unsigned int)(DataOffset_Begin >> 16);
    tmp_DataOffsetBegin_Ofst = (unsigned int)(DataOffset_Begin & 0x0000FFFF);
    EMM_MapnRead(0, SAMB_data, tmp_DataOffsetBegin_Sgmt, tmp_DataOffsetBegin_Ofst, ReadNbytes, EmmHndl);
    /*
        END: Read Data
    */

    tmp_DataOffsetBegin_Sgmt = (unsigned int)(DataOffset_Begin >> 16);         // Hi-Word of Double-Word
    tmp_DataOffsetBegin_Ofst = (unsigned int)(DataOffset_Begin & 0x0000FFFF);  // Lo-Word of Double-Word
    EMM_MapnRead(0, SAMB_data, tmp_DataOffsetBegin_Ofst, tmp_DataOffsetBegin_Sgmt, 16, EmmHndl);

Error:
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: EMM_LBXR_DirectLoad(EmmHndlNm = %s, LbxEntry = %d, SAMB_data = 0x%04X, RecFirst = %d, RecCount = %d, RecSize = %d) { SAMB_data = 0x%04X }\n", __FILE__, __LINE__, EmmHndlNm, LbxEntry, SAMB_data, RecFirst, RecCount, RecSize, SAMB_data);
#endif
    return ST_FAILURE;
Exit:
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: EMM_LBXR_DirectLoad(EmmHndlNm = %s, LbxEntry = %d, SAMB_data = 0x%04X, RecFirst = %d, RecCount = %d, RecSize = %d) { SAMB_data = 0x%04X }\n", __FILE__, __LINE__, EmmHndlNm, LbxEntry, SAMB_data, RecFirst, RecCount, RecSize, SAMB_data);
#endif
    return ST_SUCCESS;
}

// s13p15
/*
    Return:
        0   Failure
        EMM Handle Number
*/
unsigned int EMM_CheckHandleOpen(char *EmmHndlNm)
{
    int itrEmmOpenHandles;
    unsigned int EmmHndl;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: EMM_CheckHandleOpen(EmmHndlNm=%s)\n", __FILE__, __LINE__, EmmHndlNm);
#endif

    itrEmmOpenHandles = 0;
    EmmHndl = 0;
    while ( (itrEmmOpenHandles < g_EMM_Open_Handles) && (EmmHndl == 0) )
    {
        if ( stricmp(g_EMM_Table[itrEmmOpenHandles].eEmmHndlNm, EmmHndlNm) == 0 )
        {
            EmmHndl = g_EMM_Table[itrEmmOpenHandles].eEmmHndlNbr;
            g_LBX_EmmRsvd = g_EMM_Table[itrEmmOpenHandles].eEmmRsrvd;
        }
        itrEmmOpenHandles++;
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: EMM_CheckHandleOpen(EmmHndlNm=%s) {EmmHndl=%u}\n", __FILE__, __LINE__, EmmHndlNm, EmmHndl);
#endif

    return EmmHndl;
}

// s13p16
void EMM_Shutdown(void)
{
    unsigned int itrOpenHandles = 0;
//    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_Shutdown()\n", __FILE__, __LINE__);

//    dlvfprintf("DEBUG: gEMM_Open_Handles: %u\n", g_EMM_Open_Handles);

    while (itrOpenHandles < g_EMM_Open_Handles)
    {
//        dlvfprintf("DEBUG: gEMM_Open_Handles: %u;  itrOpenHandles: %u\n", g_EMM_Open_Handles, itrOpenHandles);
        EMM_ReleaseHandle(g_EMM_Table[itrOpenHandles].eEmmHndlNbr);
        itrOpenHandles++;
    }

    if (g_EMM_Overlay_Handle != 0)
    {
        EMM_ReleaseHandle(g_EMM_Overlay_Handle);
    }

//    dlvfprintf("DEBUG: [%s, %d] END: EMM_Shutdown()\n", __FILE__, __LINE__);
}

// s13p20
unsigned int EMM_GetHandle(unsigned int EmmLogicalPageCount, char *EmmHandleName, int EmmRsvdFlag)
{
    char varTmpStrEmmRsvdErr[6] = {0};
    int varExistingEmmHndlNbr;
    unsigned int varItrOurOpenHandleCount;
    unsigned int varOurOpenHandlePageCount;
    unsigned int varEmmHndlNbr;
    char tmp_EmmHndlNm_byte0;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_GetHandle(EmmLogicalPageCount = %u, EmmHandleName = %s, EmmRsvdFlag = %d)\n", __FILE__, __LINE__, EmmLogicalPageCount, EmmHandleName, EmmRsvdFlag);
#endif

    if (EmmHandleName == NULL)
    {
        varEmmHndlNbr = 0;
        goto Exit;
    }

    /* Loop through our EMM Table, Check for existing handle by name */
    varExistingEmmHndlNbr = -1;
    varItrOurOpenHandleCount = 0;

    while (varItrOurOpenHandleCount < g_EMM_Open_Handles)
    {
        if (strcmp(EmmHandleName, g_EMM_Table[varItrOurOpenHandleCount].eEmmHndlNm) == 0)
        {
            varExistingEmmHndlNbr = varItrOurOpenHandleCount;
        }
        varItrOurOpenHandleCount++;
    }

    /* If we had already created an entry with this name... */
    if (varExistingEmmHndlNbr != -1)
    {
        /* If the reuqested page count is not the same as the existing page count...*/
        varEmmHndlNbr = EMM_GetHandlePageCount(g_EMM_Table[varExistingEmmHndlNbr].eEmmHndlNbr);
        if (varOurOpenHandlePageCount < EmmLogicalPageCount)
        {
            itoa(g_EMM_Table[varExistingEmmHndlNbr].eEmmHndlNbr, Tmp_Conv_Str_1, 10);
            itoa(varEmmHndlNbr, Tmp_Conv_Str_2, 10);
            itoa(EmmLogicalPageCount, Tmp_Conv_Str_3, 10);
            strcpy(Temp_String, EmmHandleName);
            strcat(Temp_String, cnst_EmmErr_Reload1);  //' reloaded into EMM, diff size ='
            strcat(Temp_String, Tmp_Conv_Str_1);
            strcat(Temp_String, cnst_EmmErr_Space);
            strcat(Temp_String, Tmp_Conv_Str_2);
            strcat(Temp_String, cnst_EmmErr_Space);
            strcat(Temp_String, Tmp_Conv_Str_3);
            Quit(Temp_String);
        }
        else
        {
            goto Exit;
        }
    }

    strcpy(g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm, EmmHandleName);

// TODO(JimBalcomb): figure out what this business is with neg(g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm[0])
//     if ( EmmRsvdFlag == 1 )
//     {
//         /*
//             cmp  [bp+EmmRsvdFlag], 1
//             jnz  short @@NotReserved

//             @@Reserved:
//             mov  ax, [g_EMM_Open_Handles]
//             mov  dx, e_sizeof_EMM_RECORD
//             imul dx
//             mov  bx, ax
//             mov  al, [byte ptr EMM_Table.EmmHndlNm+bx]
//             neg  al
//             push ax
//             mov  ax, [g_EMM_Open_Handles]
//             mov  dx, e_sizeof_EMM_RECORD
//             imul dx
//             mov  bx, ax
//             pop  ax
//             mov  [byte ptr EMM_Table.EmmHndlNm+bx], al
//         */
//         /*
//         ? g_EMM_Open_Handles = (
//             _BX = g_EMM_Open_Handles * sizeof EMM_Record
//             _AL = gEMM_Table[].EmmHndlNm + _BX
//             NEG _AL
//             gEMM_Table[g_EMM_Open_Handles].EmmHndlNm)
//         ?
//         */
//         printf("DEBUG: [%s, %d]: g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm: %s\n", __FILE__, __LINE__, g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm);
//         tmp_EmmHndlNm_byte0 = g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm[0];
//         printf("DEBUG: [%s, %d]: tmp_EmmHndlNm_byte0: %c)\n", __FILE__, __LINE__, tmp_EmmHndlNm_byte0);
//         tmp_EmmHndlNm_byte0 = -tmp_EmmHndlNm_byte0;
//         printf("DEBUG: [%s, %d]: tmp_EmmHndlNm_byte0: %c)\n", __FILE__, __LINE__, tmp_EmmHndlNm_byte0);
//         g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm[0] = tmp_EmmHndlNm_byte0;
//         printf("DEBUG: [%s, %d]: g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm: %s\n", __FILE__, __LINE__, g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm);
// getch();
//     }

    varEmmHndlNbr = EMM_MakeNamedHandle(EmmLogicalPageCount, g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm);

    if (varEmmHndlNbr == 0)
    {
        goto Exit;
    }

    strcpy(g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm, EmmHandleName);
    g_EMM_Table[g_EMM_Open_Handles].eEmmRsrvd = EmmRsvdFlag;
    g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNbr = varEmmHndlNbr;
    g_EMM_Open_Handles++;

    if (EmmRsvdFlag == 1)
    {
        g_EMM_Pages_Reserved -= EmmLogicalPageCount;
    }

    if ( g_EMM_Pages_Reserved < 0 )
    {
        // 'EMM reserved exceeded by ' gEMM_Pages_Reserved ' blocks [' EmmHandleName '.LBX]'
        //asm mov ax, [gEMM_Pages_Reserved]
        //asm neg ax
        itoa((g_EMM_Pages_Reserved * -1), varTmpStrEmmRsvdErr, 10);
        strcpy(Temp_String, cnst_EmmErr_ResOut1);
        strcat(Temp_String, varTmpStrEmmRsvdErr);
        strcat(Temp_String, cnst_EmmErr_ResOut2);
        strcat(Temp_String, EmmHandleName);
        strcat(Temp_String, cnst_EmmErr_ResOut3);
        Quit(Temp_String);
    }

Exit:

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: EMM_GetHandle(EmmLogicalPageCount = %u, EmmHandleName = %s, EmmRsvdFlag = %d) { EmmHndlNbr = %u }\n", __FILE__, __LINE__, EmmLogicalPageCount, EmmHandleName, EmmRsvdFlag, varEmmHndlNbr);
#endif

    return varEmmHndlNbr;
}

// s13p25
void EMM_Map4_EMMDATAH(void)
{
    EMM_Map4(g_EmmHndlNbr_EMMDATAH, 0);
}

// s13p26
unsigned int EMM_EMMDATAH_AllocFirst(int nparas)
{
    unsigned int paras_free;
    unsigned int SAMB_head;
    unsigned int SAMB_data;
    int tmp_nparas;
    unsigned int tmp_paras_free;
    unsigned int tmp_g_EMMDATAH_Level;

//    printf("DEBUG: %s %d BEGIN: EMM_EMMDATAH_AllocFirst()\n", __FILE__, __LINE__);

    tmp_nparas = nparas;

    g_EMMDATAH_Level = 0;

    SAMB_head = EMM_PageFrame_Base_Address;
    SAMB_head = SAMB_head + g_EMMDATAH_Level;

    EMM_Map4_EMMDATAH();

    tmp_nparas = tmp_nparas + 1;

    tmp_paras_free = 4090;  // 0FFAh
    tmp_paras_free = tmp_paras_free - g_EMMDATAH_Level;
    paras_free = tmp_paras_free;

    if ( paras_free < tmp_nparas )
    {
        SA_Alloc_Error(0x04, (tmp_nparas - paras_free));  // Alloc_EMM_too_small
    }

    farpokew(SAMB_head, 4, 0x12FA);             // s_SAMB.MemSig1 = e_SAMB_MemSig1
    farpokew(SAMB_head, 6, 0x4ECF);             // s_SAMB.MemSig2 = e_SAMB_MemSig2
    farpokew(SAMB_head, 8, (tmp_nparas - 1));   // s_SAMB.Size_Paras = nparas + 1 - 1
    farpokew(SAMB_head, 10, 1);                 // s_SAMB.Used_Paras = 1
    farpokew(SAMB_head, 14, 1);                 // s_SAMB.Marked_Paras = 1

    tmp_g_EMMDATAH_Level = tmp_nparas;
    tmp_g_EMMDATAH_Level = tmp_g_EMMDATAH_Level + 1;
    g_EMMDATAH_Level = g_EMMDATAH_Level + tmp_g_EMMDATAH_Level;

    SAMB_data = SAMB_head + 1;

//    printf("DEBUG: %s %d END: EMM_EMMDATAH_AllocFirst()\n", __FILE__, __LINE__);
    return SAMB_data;
}

// s13p27
unsigned int EMM_EMMDATAH_AllocNext(int nparas)
{
    unsigned int paras_free;
    unsigned int SAMB_head;
    unsigned int SAMB_data;
    int tmp_nparas;
    unsigned int tmp_paras_free;
    unsigned int tmp_g_EMMDATAH_Level;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_EMMDATAH_AllocNext()\n", __FILE__, __LINE__);
#endif

    tmp_nparas = nparas;

    EMM_Map4_EMMDATAH();

    SAMB_head = EMM_PageFrame_Base_Address;
    SAMB_head = SAMB_head + g_EMMDATAH_Level;

    EMM_Map4_EMMDATAH();

    tmp_nparas = tmp_nparas + 1;

    tmp_paras_free = 4090;  // 0FFAh
    tmp_paras_free = tmp_paras_free - g_EMMDATAH_Level;
    paras_free = tmp_paras_free;

    if ( paras_free < tmp_nparas )
    {
        SA_Alloc_Error(0x04, (tmp_nparas - paras_free));  // Alloc_EMM_too_small
    }

    farpokew(SAMB_head, 4, 0x12FA);             // s_SAMB.MemSig1 = e_SAMB_MemSig1
    farpokew(SAMB_head, 6, 0x4ECF);             // s_SAMB.MemSig2 = e_SAMB_MemSig2
    farpokew(SAMB_head, 8, (tmp_nparas - 1));   // s_SAMB.Size_Paras = nparas + 1 - 1
    farpokew(SAMB_head, 10, 1);                 // s_SAMB.Used_Paras = 1
    farpokew(SAMB_head, 14, 1);                 // s_SAMB.Marked_Paras = 1

    tmp_g_EMMDATAH_Level = tmp_nparas;
    tmp_g_EMMDATAH_Level = tmp_g_EMMDATAH_Level + 1;
    g_EMMDATAH_Level = g_EMMDATAH_Level + tmp_g_EMMDATAH_Level;

    SAMB_data = SAMB_head + 1;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: EMM_EMMDATAH_AllocNext()\n", __FILE__, __LINE__);
#endif

    return SAMB_data;
}

// s13p31
void EMM_SetMinKB(int EMM_Min_KB)
{
    g_EMM_MinKB = EMM_Min_KB;
}

// s13p32
void EMM_BuildEmmErrStr(char *str_buf)
{
    char tmp_string[20];

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: EMM_BuildEmmErrStr()\n", __FILE__, __LINE__);
#endif

    itoa(g_EMM_MinKB, tmp_string, 10);
    strcpy(str_buf, "You must have at least ");
    strcat(str_buf, tmp_string);
    strcat(str_buf, "K of expanded memory.");

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: EMM_BuildEmmErrStr()\n", __FILE__, __LINE__);
#endif
}
