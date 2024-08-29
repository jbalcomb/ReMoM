/*
    WIZARDS.EXE
        seg008
*/

#ifndef ALLOCATE_H
#define ALLOCATE_H

/*
    "SPACE ALLOC" - STARLORD.EXE
*/
/*
MoO2
Allocate_Space()
    Allocate_Far_Space()
    Allocate_Dos_Space()
        Allocate_Dos_Data_Space()
~==
Allocate_Near_Space()
¿ minimum == 256 ?
blocks?
256 * 16 = 4096

*/


#include "MoX_TYPE.H"   /* SAMB_ptr, etc. */
#include "MoX_BITS.H"   /* MoX_GET_2B_OFS() etc. */


enum e_SA_Allocation_Type
{
    sa_Single,
    sa_First,
    sa_Next,
    sa_Invalid
};


#define SAMB_RESERVED1   0
#define SAMB_RESERVED2   2
#define SAMB_MEMSIG1     4
#define SAMB_MEMSIG2     6
#define SAMB_SIZE        8
#define SAMB_USED       10
#define SAMB_unknown    12
#define SAMB_MARK       14

#define SA_MEMSIG1      0x12FA
#define SA_MEMSIG2      0x4ECF

#define SA_GET_MEMSIG1(_SAMB_head_) ( GET_2B_OFS((_SAMB_head_), SAMB_MEMSIG1) )
#define SA_GET_MEMSIG2(_SAMB_head_) ( GET_2B_OFS((_SAMB_head_), SAMB_MEMSIG2) )
#define SA_GET_SIZE(_SAMB_head_)    ( GET_2B_OFS((_SAMB_head_), SAMB_SIZE)    )
#define SA_GET_USED(_SAMB_head_)    ( GET_2B_OFS((_SAMB_head_), SAMB_USED)    )
#define SA_GET_MARK(_SAMB_head_)    ( GET_2B_OFS((_SAMB_head_), SAMB_MARK)    )
#define SA_SET_MEMSIG1(_SAMB_head_)        ( SET_2B_OFS((_SAMB_head_), SAMB_MEMSIG1, SA_MEMSIG1) )
#define SA_SET_MEMSIG2(_SAMB_head_)        ( SET_2B_OFS((_SAMB_head_), SAMB_MEMSIG2, SA_MEMSIG2) )
#define SA_SET_SIZE(_SAMB_head_,_nparas_)  ( SET_2B_OFS((_SAMB_head_), SAMB_SIZE, (_nparas_))    )
#define SA_SET_USED(_SAMB_head_,_nparas_)  ( SET_2B_OFS((_SAMB_head_), SAMB_USED, (_nparas_))    )
#define SA_SET_MARK(_SAMB_head_,_nparas_)  ( SET_2B_OFS((_SAMB_head_), SAMB_MARK, (_nparas_))    )



#ifdef __cplusplus
extern "C" {
#endif



// WZD dseg:727C
// drake178: LBX_NearBuf_Used
extern int16_t near_buffer_used;

// WZD dseg:727E
// extern char cnst_Alloc_Error01[];
// WZD dseg:729C
// extern char cnst_Alloc_Error02[];

// WZD dseg:72A3 49 6E 73 75 66 66 69 63 69 65 6E 74 20 6D 65 6D+cnst_Alloc_Error51 db 'Insufficient memory. You need at least ',0
// WZD dseg:72A3 6F 72 79 2E 20 59 6F 75 20 6E 65 65 64 20 61 74+                                        ; DATA XREF: SA_Alloc_Error+14o
// WZD dseg:72CB 4B 20 66 72 65 65 2E 20 54 72 79 20 72 65 6D 6F+cnst_Alloc_Error52 db 'K free. Try removing all TSR',27h,'s.',0
// WZD dseg:72CB 76 69 6E 67 20 61 6C 6C 20 54 53 52 27 73 2E 00                                         ; DATA XREF: SA_Alloc_Error+48o
// WZD dseg:72EB 44 79 6E 61 6D 69 63 20 61 6C 6C 6F 63 61 74 69+cnst_Alloc_Error11 db 'Dynamic allocation too small for Allocate_Space()',0
// WZD dseg:72EB 6F 6E 20 74 6F 6F 20 73 6D 61 6C 6C 20 66 6F 72+                                        ; DATA XREF: SA_Alloc_Error:loc_15607o
// WZD dseg:731D 20 6F 66 20 00                                  cnst_Alloc_Error12 db ' of ',0          ; DATA XREF: SA_Alloc_Error:loc_15628o
// WZD dseg:7322 20 62 6C 6F 63 6B 73 00                         cnst_Alloc_Error13 db ' blocks',0       ; DATA XREF: SA_Alloc_Error+B4o
// WZD dseg:732A 46 61 69 6C 65 64 20 74 6F 20 72 65 6C 6F 61 64+cnst_Alloc_Error21 db 'Failed to reload',0
// WZD dseg:732A 00                                                                                      ; DATA XREF: SA_Alloc_Error:loc_15659o ...
// WZD dseg:733B 20 41 6C 6C 6F 63 61 74 65 5F 4E 65 78 74 5F 42+cnst_Alloc_Error22 db ' Allocate_Next_Block()',0
// WZD dseg:733B 6C 6F 63 6B 28 29 00                                                                    ; DATA XREF: SA_Alloc_Error+D5o ...
// WZD dseg:7352 3A 20 53 68 6F 72 74 20 62 79 20 00             cnst_Alloc_Error23 db ': Short by ',0   ; DATA XREF: SA_Alloc_Error+E4o ...
// WZD dseg:735E 41 6C 6C 6F 63 61 74 69 6F 6E 20 73 70 61 63 65+cnst_Alloc_Error3 db 'Allocation space has been corrupted for',0
// WZD dseg:735E 20 68 61 73 20 62 65 65 6E 20 63 6F 72 72 75 70+                                        ; DATA XREF: SA_Alloc_Error:loc_1569Ao
// WZD dseg:7386 20 28 45 4D 4D 29 20 00                         cnst_Alloc_Error4 db ' (EMM) ',0        ; DATA XREF: SA_Alloc_Error+149o
// WZD dseg:738E 00 00                                           farload_lbx_header_flag dw 0            ; DATA XREF: LBX_Load_Entry:loc_15981r ...
// WZD dseg:7390 FF FF                                           farload_fptr dw 0FFFFh                  ; DATA XREF: LBX_Load_Entry:loc_159DAr ...
// WZD dseg:7392 00 00                                           UU_gLbxLoadFormat dw 0                  ; DATA XREF: LBX_Load_Entry+47r ...
// WZD dseg:7394                                                 ; char secondary_drive_path[]
// WZD dseg:7394 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+secondary_drive_path db 32h dup(0)      ; DATA XREF: LBX_Load_Entry+EDr ...
// WZD dseg:73C6 2E 4C 42 58 00                                  farload_file_extension db '.LBX',0      ; DATA XREF: LBX_Load_Entry+CAo ...
// WZD dseg:73CB 2E 4C 42 58 20 5B 65 6E 74 72 79 20 00          cnst_LBXErr_Common1 db '.LBX [entry ',0 ; DATA XREF: Error_Handler+2Bo
// WZD dseg:73D8 5D 20 00                                        cnst_LBXErr_Common2 db '] ',0           ; DATA XREF: Error_Handler+4Ao
// WZD dseg:73DB 20 63 6F 75 6C 64 20 6E 6F 74 20 62 65 20 66 6F+cnst_LBX_Error1 db ' could not be found.',0
// WZD dseg:73DB 75 6E 64 2E 00                                                                          ; DATA XREF: Error_Handler:loc_16806o
// WZD dseg:73F0 20 68 61 73 20 62 65 65 6E 20 63 6F 72 72 75 70+cnst_LBX_Error2 db ' has been corrupted.',0
// WZD dseg:73F0 74 65 64 2E 00                                                                          ; DATA XREF: Error_Handler:loc_16818o
// WZD dseg:7405 49 6E 73 75 66 66 69 63 69 65 6E 74 20 6D 65 6D+cnst_LBX_Error31 db 'Insufficient memory. You need at least ',0
// WZD dseg:7405 6F 72 79 2E 20 59 6F 75 20 6E 65 65 64 20 61 74+                                        ; DATA XREF: Error_Handler:loc_1681Do
// WZD dseg:742D 4B 20 66 72 65 65 2E 20 54 72 79 20 72 65 6D 6F+cnst_LBX_Error32 db 'K free. Try removing all TSR',27h,'s.',0
// WZD dseg:742D 76 69 6E 67 20 61 6C 6C 20 54 53 52 27 73 2E 00                                         ; DATA XREF: Error_Handler+B7o
// WZD dseg:744D 20 77 61 73 20 6E 6F 74 20 70 72 6F 70 65 72 6C+cnst_LBX_Error4 db ' was not properly allocated or has been corrupted.',0
// WZD dseg:744D 79 20 61 6C 6C 6F 63 61 74 65 64 20 6F 72 20 68+                                        ; DATA XREF: Error_Handler:loc_16856o
// WZD dseg:7480 20 66 61 69 6C 65 64 20 74 6F 20 72 65 6C 6F 61+cnst_LBX_Error51 db ' failed to reload. Allocation too small by ',0
// WZD dseg:7480 64 2E 20 41 6C 6C 6F 63 61 74 69 6F 6E 20 74 6F+                                        ; DATA XREF: Error_Handler:loc_1685Bo
// WZD dseg:74AC 20 70 61 67 65 73 00                            cnst_LBX_Error52 db ' pages',0          ; DATA XREF: Error_Handler+F4o
// WZD dseg:74B3 20 69 73 20 6E 6F 74 20 61 6E 20 4C 42 58 20 66+cnst_LBX_Error7 db ' is not an LBX file',0
// WZD dseg:74B3 69 6C 65 00                                                                             ; DATA XREF: Error_Handler:loc_16894o
// WZD dseg:74C7 20 65 78 63 65 65 64 73 20 6E 75 6D 62 65 72 20+cnst_LBX_Error8 db ' exceeds number of LBX entries',0
// WZD dseg:74C7 6F 66 20 4C 42 58 20 65 6E 74 72 69 65 73 00                                            ; DATA XREF: Error_Handler:loc_1689Ao
// WZD dseg:74E6 20 68 61 73 20 61 6E 20 69 6E 63 6F 72 72 65 63+cnst_LBX_Error9 db ' has an incorrect record size',0
// WZD dseg:74E6 74 20 72 65 63 6F 72 64 20 73 69 7A 65 00                                               ; DATA XREF: Error_Handler:loc_168A0o
// WZD dseg:7504 20 65 78 63 65 65 64 73 20 6E 75 6D 62 65 72 20+cnst_LBX_ErrorA db ' exceeds number of defined records',0
// WZD dseg:7504 6F 66 20 64 65 66 69 6E 65 64 20 72 65 63 6F 72+                                        ; DATA XREF: Error_Handler:loc_168A6o
// WZD dseg:7527 20 63 61 6E 6E 6F 74 20 62 65 20 72 65 6C 6F 61+cnst_LBX_ErrorB db ' cannot be reloaded into EMS w/o being first released.',0
// WZD dseg:7527 64 65 64 20 69 6E 74 6F 20 45 4D 53 20 77 2F 6F+                                        ; DATA XREF: Error_Handler+12Do ...
// WZD dseg:755E 20 45 4D 4D 20 6C 6F 61 64 69 6E 67 20 65 72 72+cnst_LBX_ErrorC db ' EMM loading error. Insufficient EMM.',0
// WZD dseg:755E 6F 72 2E 20 49 6E 73 75 66 66 69 63 69 65 6E 74+                                        ; DATA XREF: Error_Handler:loc_168CDo ...
// WZD dseg:7584 20 4F 6E 6C 79 20 70 69 63 74 75 72 65 73 20 6D+cnst_LBX_ErrorD db ' Only pictures may be loaded into reserved EMM',0
// WZD dseg:7584 61 79 20 62 65 20 6C 6F 61 64 65 64 20 69 6E 74+                                        ; DATA XREF: Error_Handler:loc_168D3o
// WZD dseg:75B3 20 28 52 65 73 65 72 76 65 64 20 45 4D 4D 29 20+cnst_LBX_ErrorE db ' (Reserved EMM) ',0 ; DATA XREF: Error_Handler+14Eo ...
// WZD dseg:75C4 20 4C 42 58 20 74 6F 00                         cnst_LBX_ErrorF1 db ' LBX to',0         ; DATA XREF: Error_Handler:loc_168F7o
// WZD dseg:75CC 20 56 67 61 20 66 69 6C 65 20 61 6E 69 6D 61 74+cnst_LBX_ErrorG db ' Vga file animation frames cannot exceed 65536 bytes',0
// WZD dseg:75CC 69 6F 6E 20 66 72 61 6D 65 73 20 63 61 6E 6E 6F+                                        ; DATA XREF: Error_Handler:loc_1691Bo
// WZD dseg:7601 00                                              db    0
// WZD dseg:7602 72 00                                           cstr_R__2 db 'r',0                      ; DATA XREF: DISK_OpenCreate:loc_169A8o



// WZD dseg:D498
// drake178: LBX_NearBuf_Mark
extern int16_t near_buffer_mark;

// ¿ ? // WZD dseg:D49A
// ¿ ? extern char Temp_String[];
// ¿ ? // WZD dseg:D4FE
// ¿ ? extern char Tmp_Conv_Str_1[];
// ¿ ? // WZD dseg:D512
// ¿ ? extern char Tmp_Conv_Str_2[];
// ¿ ? // WZD dseg:D530
// ¿ ? extern char Tmp_Conv_Str_3[];
// ¿ ? // WZD dseg:D59A
// ¿ ? extern char LBX_Near_Buffer[];
// WZD dseg:D49A
extern char near_buffer[];



/*
    MGC  seg007
*/

// MGC s07p04
int16_t Check_Allocation(SAMB_ptr SAMB_head);

/*
    MGC  seg008
*/

// WZD s08p01
// drake178: LBX_NearAlloc_Mark()
void Near_Allocate_Mark(void);

// WZD s08p02
// drake178: LBX_NearAlloc_Undo()
void Near_Allocate_Undo(void);

// WZD s08p03
// drake178: LBX_NearAlloc_Reset()
void Near_Allocate_Reset(void);

// WZD s08p04
// drake178: LBX_NearAlloc_First()
byte_ptr Near_Allocate_First(int16_t size);

// WZD s08p05
// drake178: LBX_NearAlloc_Next()
byte_ptr Near_Allocate_Next(int16_t size);

// WZD s08p06
// drake178: LBX_NearAlloc_Error()
void Near_Allocation_Error(int16_t size);

// WZD s08p07
SAMB_ptr Allocate_Space(uint16_t size);

// WZD s08p08 / s08p09
SAMB_ptr Allocate_Space_No_Header(uint16_t size);

// WZD s08p10
// drake178: LBX_Alloc_Mark
void Mark_Block(SAMB_ptr block);

// WZD s08p11
// drake178: LBX_Alloc_Undo()
void Release_Block(SAMB_ptr block);

// WZD s08p12
SAMB_ptr Allocate_First_Block(SAMB_ptr SAMB_head, uint16_t size);

// WZD s08p13
// drake178: LBX_Alloc_Flush()
void Reset_First_Block(SAMB_ptr block);

// WZD s08p14
SAMB_ptr Allocate_Next_Block(SAMB_ptr SAMB_head, uint16_t size);

// WZD s08p15
// drake178: LBX_Alloc_GetFree()
// ¿ MoO2  Module: allocate  Get_Remaining_Block_Space() ?
uint16_t Get_Free_Blocks(SAMB_ptr SAMB_head);

// WZD s08p16
// WZD s08p17
// WZD s08p18

// WZD s08p19
void Allocation_Error(uint16_t error_num, uint16_t blocks);


#ifdef __cplusplus
}
#endif

#endif /* ALLOCATE_H */
