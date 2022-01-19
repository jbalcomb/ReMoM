#ifndef ST_EMM_H
#define ST_EMM_H



/*
J:\STU\DBWD\developc\wolf3d\WOLFSRC\ID_PM.C
    /////////////////////////////////////////////////////////////////////////////
    //
    //	EMS Management code
    //
    /////////////////////////////////////////////////////////////////////////////

    //
    //	PML_MapEMS() - Maps a logical page to a physical page
    //
    void
    PML_MapEMS(word logical,word physical)
    {
        _AL = physical;
        _BX = logical;
        _DX = EMSHandle;
        _AH = EMS_MAPPAGE;
    asm	int	EMS_INT

        if (_AH)
            Quit("PML_MapEMS: Page mapping failed");
    }

    //
    //	PML_StartupEMS() - Sets up EMS for Page Mgr's use
    ...
        _AH = EMS_ALLOCPAGES;
        _BX = EMSAvail;
        geninterrupt(EMS_INT);
        if (_AH)
            goto error;
        EMSHandle = _DX;
        ...
        EMSPresent = true;			// We have EMS
    error:
        return(EMSPresent);
*/
/*
J:\STU\DBWD\developc\wolf3d\WOLFSRC\ID_MM.H
*/
//--------

#define	EMS_INT			0x67

#define	EMS_STATUS		0x40
#define	EMS_GETFRAME	0x41
#define	EMS_GETPAGES	0x42
#define	EMS_ALLOCPAGES	0x43
#define	EMS_MAPPAGE		0x44
#define	EMS_FREEPAGES	0x45
#define	EMS_VERSION		0x46

#define	EMM_INT			    0x67

#define	EMS_STATUS		    0x40
#define	EMS_GETFRAME	    0x41
#define	EMS_GETPAGES	    0x42    // Get Page Counts (LIM EMS 3.2+);      BX = unallocated pages, DX = total pages
//Get Handle and Allocate Pages
#define	EMS_ALLOCPAGES	    0x43    // DX = EMM handle
#define	EMS_MAPPAGE		    0x44
#define	EMS_FREEPAGES	    0x45
#define	EMS_VERSION		    0x46
#define	EMS_GETHANDLES      0x4B    // BX = number of currently active EMM handles (0-256)
#define	EMS_GETHNDLPAGECNT  0x4C    // Get Page Count for Handle (LIM EMS); BX = total allocated page count for handle (1-512)
#define EMS_GET_PAGE_COUNT_FOR_ALL_HANDLES  0x4D    // BX = number of active EMM handles (0..255); ES:DI = pointer to an array of entries
#define EMS_GET_SET_HANDLE_NAME 0x53    // AL = 00 get handle name; AL = 01 set handle name;
#define EMS_GET_HANDLE_NAME 5300h   // ES:DI = pointer to 8byte handle name array
#define EMS_SET_HANDLE_NAME 5301h   // DX = EMM handle; DS:SI = pointer to 8byte handle name

/*
unsigned int EMS_STATUS_Code[165] = {
    0,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    36,
    1,
    2,
    36,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    15,
    16,
    17,
    18,
    19,
    20,
    21,
    22,
    23,
    24,
    36,
    25,
    26,
    27,
    28,
    29,
    30,
    36,
    31,
    32,
    33,
    34
};

// https://www.stanislavs.org/helppc/int_67-40.html
char EMS_STATUS_Message[36][113] = {
    "success",
    "internal software error",
    "hardware malfunction",
    "unallocated or invalid handle",
    "undefined function requested",
    "no handles available",
    "error in save or restore of mapping context",
    "more pages requested than physically exist",
    "more pages requested than currently available",
    "zero pages requested",
    "invalid logical page number",
    "illegal physical page number",
    "context stack out of memory",
    "context save failed, handle already has context stack",
    "context restore failed, no context stack for handle",
    "invalid subfunction",
    "undefined attribute type",
    "feature not supported",
    "success, portion of the source region was overwritten",
    "length of source or destination region exceeds length of region allocated to either source or destination handle",
    "conventional and expanded memory regions overlap",
    "offset within logical page exceeds size of logical page",
    "region length exceeds 1M",
    "source and destination regions have same handle and overlap",
    "memory source or destination type undefined",
    "specified DMA register set not supported ??? specified alternate map register set not supported",
    "all DMA register sets currently allocated ??? all alternate map register sets currently allocated",
    "alternate DMA sets not supported ??? alternate map register sets not supported",
    "undefined or unallocated DMA register set ??? undefined or unallocated alternate map register set",
    "dedicated DMA channels not supported",
    "specified dedicated DMA channel not supported",
    "duplicate handle name",
    "attempted to wrap around 1Mb conventional address space",
    "contents of partial page map corrupted or count of mappable segments exceeds total mappable segments",
    "operating system denied access",
    "INVALID EMM_STATUS Code!!"
};
*/


#define VGAFILEH "VGAFILEH"

/*
In EMS_Startup, there is a loop for 50 iterations, with a check for `EMS_Open_Handles < 40`.
I do not know what the 50 is doing or why it is doing it.
The 40 seems to be the number of 'EMM Handes' that MoM/MGC /requires/.
Also, that is the value to which g_EMM_Pages_Reserved and g_EMM_Table[] is set.
But, GAME_QuitWithReport() reports this as "EMM: [g_EMM_Pages_Reserved] blocks"
And, g_EMM_Pages_Reserved is set to 158 (9Eh) as the beginning of main().
EMM_LoadLBX checks (g_EMM_Open_Handles < 40)
...there is a check elsewhere on g_EMM_Pages_Reserved and the Total EMM Pages Free/Available
...g_EMM_Pages_Reserved seems to actually be the EMM Pages Available of the pre-defined EMM Pages Required
*/
#define MOM_EMM_HANDLES 40  // 28h



struct s_EMM_Record {
    char eEmmHndlNm[9];
    char eEmmRsrvd;
    unsigned int eEmmHndlNbr;
};
typedef struct s_EMM_Record EMM_Record;

// struc EMS_L2P_Map_Record ; (sizeof=0x4)
// LogicalPageNbr dw ?
// PhysicalPageNbr dw ?
// ends EMS_L2P_Map_Record
struct s_EMS_L2P_Map_Record //; (sizeof=0x4)
{
    int LogicalPageNbr;
    int PhysicalPageNbr;
};


/*
    ? `static` at module-scope ?
        seg012:0006  EMM_Logical_Page dw 0                   ; DATA XREF: EMM_MapnRead, EMM_MapnWrite
*/
extern unsigned int EMM_Logical_Page;


extern char EmmHndlNm_FIGUREX[];            // dseg:3846
extern char EmmHndlNm_TILEXXX[];            // dseg:384E
extern char EmmHndlNm_CONTXXX[];            // dseg:3856

extern unsigned int g_EmmHndl_CONTXXX;      // dseg:68F6
extern unsigned int g_EmmHndl_TILEXXX;      // dseg:9484
extern unsigned int g_EmmHndl_FIGUREX;      // dseg:9486


/*
    EMM - Initialized
        ? ALL Xref seg012 and seg013 ?
*/

extern unsigned int g_EMM_PageFrame_Base_Address;   // dseg:40E4
extern char g_EMM_device_name[];                    // dseg:40E6

// dseg:40EF g_EMM_Log2Phys_Map EMS_L2P_Map_Record <0, 0> ; This is not in ORION.
// dseg:40EF EMS_L2P_Map_Record <0,      1>
// dseg:40EF EMS_L2P_Map_Record <0,      2>
// dseg:40EF EMS_L2P_Map_Record <0,      3>
// dseg:40FF align 2
extern unsigned int g_EMM_Log2Phys_Map[]; // dseg:40EF

extern unsigned int g_EMM_Pages_Reserved;           // dseg:4100 ; set to 158 at the start of _main
extern unsigned int g_EMM_Open_Handles;             // dseg:4102
extern char *g_EmmHndlNm_YOMOMA1;                   // dseg:4104  "YO MOMA"
extern char *g_EmmHndlNm_YOMOMA2;                   // dseg:410C  "YO MOMA"
extern char *g_EmmHndlNm_VGAFILEH;                  // dseg:4116  "VGAFILEH"
extern char *g_EmmHndlNm_EMMDATAH;                  // dseg:411F  "EMMDATAH"
extern char cnst_LBX_Ext2[];                        // dseg:4128
extern char *cnst_EmmErr_ResOut1;                   // dseg:412D
extern char *cnst_EmmErr_ResOut2;                   // dseg:4147
extern char *cnst_EmmErr_ResOut3;                   // dseg:4151
extern char *cnst_EmmErr_Reload1;                   // dseg:4157
extern char *cnst_EmmErr_Space;                     // dseg:4177

/*
    EMM - Uninitialized
        ? ALL Xref seg012 and seg013 ?
*/

//dseg:A5E0 UU_EMMData_PrevLvl dw 0
extern unsigned int g_EMM_Overlay_Handle;            // dseg:A5E2
extern unsigned int g_EMMDATAH_Level;                 // dseg:A5E4
extern unsigned int g_EmmHndlNbr_EMMDATAH;            // dseg:A5E6
// ? dseg:A5E8 UU_EMM_Handle dw 0
extern unsigned int g_EmmHndlNbr_VGAFILEH;            // dseg:A5EA
extern unsigned int g_LBX_EmmRsvd;              // dseg:A5EC
extern unsigned int g_EmmHndlNbr_YOMOMA;              // dseg:A5EE
extern unsigned int g_EMM_OK;                         // dseg:A5F0
extern EMM_Record g_EMM_Table[];            // dseg:A5F2 g_EMS_Table EMS_Record 28h dup(<0>)
extern unsigned int g_EMM_MinKB;                      // dseg:A7D2

/*
TBD:
RAM_WorstFreeBlocks dw 0FA00h
cnst_Quit_Report1 db 'Data',0
cnst_Quit_Report2 db ' Free: ',0
cnst_Quit_Report3 db ' bytes   Memory',0
cnst_Quit_Report4 db 'k   Worst',0
cnst_Quit_Report5 db 'k   EMM: ',0
cnst_Quit_Report6 db ' blocks',0

...
align 2
...
bb    0
db    0
...
; char UU_cnst_EMMErr_2long[]
UU_cnst_EMMErr_2long db	'EMM 2long',0

*/


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg012
*/
// _s12p01.asm
int EMM_Init(void);
// _s12p02c.c
unsigned int EMM_GetActiveHandleCount(void);
// _s12p03c.c
unsigned int EMM_GetFreePageCount(void);
// _s12p04.asm
unsigned int EMM_GetHandlePageCount(unsigned int EmsHndlNbr);
// _s12p05
unsigned int EMM_MakeNamedHandle(unsigned int argPageCount, unsigned char far * argHandleName);
// _s12p06
unsigned int EMM_GetHandleName(char *EmmHndlNm, unsigned int EmmHndlNbr);
// _s1207c.c
void EMM_ReleaseHandle(unsigned int EMM_Handle);
// _s12p08.asm
void EMM_MapnRead(unsigned int Dst_Ofst, unsigned int Dst_Sgmt, unsigned int Src_Ofst, unsigned int Src_Sgmt, int nbytes, int EMM_Handle);
// _s12p10.asm
unsigned int EMM_GetPageFrame(void);
// _s12p11a.c
void EMM_Map4(int EmmHandle, int EmmLogicalPage);
// _s12p12a.c
void EMM_MapMulti4(int argFirstLogicalPage, int argEmsHandle);

/*
    seg013
*/
// _s13p01c.c
void EMM_Startup(void);
// _s13p03c.c
void EMM_Load_LBX_File_1(char *LbxFileName);
// _s13p04c.c
int EMM_Load_LBX_File(char *LbxFileName, int Reserved);
// _s13p11c.c
unsigned int EMM_LBX_Load_Entry(char *LbxFileName, int LbxEntryIndex, unsigned int SAMB_head, int LoadType, int FormatType);
// _s13p12c.c
unsigned int EMM_LBX_HdrOnly(int EmmHndl, char *EmmHndlNm, int LbxEntryIndex, unsigned int SAMB_head, int LoadType);
// _s13p13c.c
unsigned int EMM_LBX_Load_Record(char *EmmHndlNm, int LbxEntry, unsigned int SAMB_head, int LoadType, int RecFirst, int RecCount, int RecSize);
// _s13p13c.c
//EMM_LBX_Load_Record
//_S13p14c.c
//EMM_LBX_DirectLoad

// _s13p15c.c
unsigned int EMM_CheckHandleOpen(char *EmmHndlNm);
// _s13p16c.c
void EMM_Shutdown(void);
// argEmmRsvdFlag can not be unsigned, as it is compared to -1
// _s13p25c.c
void EMM_Map4_EMMDATAH(void);
// _s13p26c.c
unsigned int EMM_EMMDATAH_AllocFirst(int nparas);
// _s13p27c.c
unsigned int EMM_EMMDATAH_AllocNext(int nparas);
// _s13p20c.c
unsigned int EMM_GetHandle(unsigned int EmmLogicalPageCount, char *EmmHandleName, int EmmRsvdFlag);
// _s13p31
void EMM_SetMinKB(int EmmMinKB);
// _s13p32c.c
void EMM_BuildEmmErrStr(char *strbuf);

#ifdef __cplusplus
}
#endif

#endif /* ST_EMM_H */
