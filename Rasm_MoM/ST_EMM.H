#ifndef ST_EMM_H
#define ST_EMM_H

//typedef void _seg * memptr;

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

#define VGAFILEH "VGAFILEH"

/*
In EMS_Startup, there is a loop for 50 iterations, with a check for `EMS_Open_Handles < 40`.
I do not know what the 50 is doing or why it is doing it.
The 40 seems to be the number of 'EMM Handes' that MoM/MGC /requires/.
Also, that is the value to which g_EMM_Pages_Reserved and g_EMM_Table[] is set.
But, GAME_QuitWithReport() reports this as "EMM: [g_EMM_Pages_Reserved] blocks"
And, g_EMM_Pages_Reserved is set to 158 (9Eh) as the beginning of main().
EMM_LoadLBX checks (g_EMM_Open_Handles < 40)

*/
#define MOM_EMM_HANDLES 40  // 28h



struct s_EMM_Record {
    char eEmmHndlNm[9];
    char eEmmRsrvd;
    unsigned int eEmmHndlNbr;
};
typedef struct s_EMM_Record EMM_Record;


//; seg012:0006  EMM_Logical_Page dw 0                   ; DATA XREF: EMM_MapnRead, EMM_MapnWrite
extern unsigned int EMM_Logical_Page;

/*
    EMM - Uninitialized
        ? ALL Xref seg012 and seg013 ?
*/

extern unsigned int g_EMM_PageFrame_Base_Address;    // dseg:40E4
extern char g_EMM_device_name[];                      // dseg:40E6
extern unsigned int g_EMM_Pages_Reserved;            // dseg:4100 ; set to 158 at the start of _main
extern unsigned int g_EMM_Open_Handles;              // dseg:4102
extern char *g_EmmHndlNm_YOMOMA1;                     // dseg:4104 "YO MOMA"
extern char *g_EmmHndlNm_YOMOMA2;                     // dseg:410C "YO MOMA"
extern char *g_EmmHndlNm_VGAFILEH;                    // dseg:4116 "VGAFILEH"
extern char *g_EmmHndlNm_EMMDATAH;                    // dseg:411F "EMMDATAH"
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
extern unsigned int g_EMM_Data_Level;                 // dseg:A5E4
extern unsigned int g_EmmHndlNbr_EMMDATAH;            // dseg:A5E6
// ? dseg:A5E8 UU_EMM_Handle dw 0
extern unsigned int g_EmmHndlNbr_VGAFILEH;            // dseg:A5EA
extern unsigned int g_LBX_EmmRsvd;              // dseg:A5EC
extern unsigned int g_EmmHndlNbr_YOMOMA;              // dseg:A5EE
extern unsigned int g_EMM_OK;                         // dseg:A5F0
extern EMM_Record g_EMM_Table[]; // dseg:A5F2 g_EMS_Table EMS_Record 28h dup(<0>)
extern unsigned int g_EMM_MinKB;                      // dseg:A7D2

/*
TBD:
cnst_EMM_FigHndName db 'FIGUREX',0
cnst_EMM_TileHndName db	'TILEXXX',0
cnst_EMM_ContH_Name db 'CONTXXX',0

RAM_WorstFreeBlocks dw 0FA00h
cnst_Quit_Report1 db 'Data',0
cnst_Quit_Report2 db ' Free: ',0
cnst_Quit_Report3 db ' bytes   Memory',0
cnst_Quit_Report4 db 'k   Worst',0
cnst_Quit_Report5 db 'k   EMM: ',0
cnst_Quit_Report6 db ' blocks',0

EMM_Log2Phys_Map EMM_L2P_Map_Record <0,	0>
EMM_L2P_Map_Record <0,	    1>
EMM_L2P_Map_Record <0,	    2>
EMM_L2P_Map_Record <0,	    3>
...
align 2
...
bb    0
db    0
...
cnst_LBX_Ext2 db '.LBX',0
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
int EMM_Init(void);                                                                  // _s12p01.asm
unsigned int EMM_GetActiveHandleCount(void);                                         // _s12p02c.c
unsigned int EMM_GetFreePageCount(void);                                             // _s12p03c.c
unsigned int EMM_GetHandlePageCount(unsigned int EmsHndlNbr);                        // _s12p04.asm
// IDA: int __cdecl __far EMM_MakeNamedHandle(int argPageCount, const char *argHandleName)
unsigned int EMM_MakeNamedHandle(unsigned int argPageCount, unsigned char far * argHandleName);    // _s12p05
//extern unsigned int StringToUpper(unsigned char far * DestFarString, unsigned char far * SourceFarString);
unsigned int EMM_GetHandleName(char *EmmHndlNm, unsigned int EmmHndlNbr);           // _s12p06
void EMM_ReleaseHandle(unsigned int EMM_Handle);                                    // _s1207c.c

/*
    seg013
*/
void EMM_Startup(void);                                                             // _s13p01c.c
unsigned int EMM_LBX_EntryLoader(char *LbxFileName, int LbxEntryIndex, int AllocSeg, int LoadType, int FormatType);  // _s13p11c.c
unsigned int EMM_LBX_HdrOnly(int EmmHndl, char *EmmHndlNm, int LbxEntryIndex, int AllocSeg, int LoadType); // _s13p12c.c
unsigned int EMM_CheckHandleOpen(char *EmmHndlNm);                                  // _s13p15c.c
void EMM_Shutdown(void);                                                            // _s13p16c.c
// argEmmRsvdFlag can not be unsigned, as it is compared to -1
unsigned int EMM_GetOrCreateHandle(unsigned int argEmmLogicalPageCount, char *argEmmHandleName, int argEmmRsvdFlag);  // _s13p20.asm
void EMM_SetMinKB(int varEMM_MinKB);                                                 // _s13p31
void EMM_BuildEmmErrStr(char *strbuf);                                               // _s13p32c.c
unsigned int EMM_MapnRead(unsigned int Dst_Ofst, unsigned int Dst_Sgmt, unsigned int Src_Ofst, unsigned int Src_Sgmt, int nbytes, int EMM_Handle); //_s12p08.asm

#ifdef __cplusplus
}
#endif

#endif /* ST_EMM_H */
