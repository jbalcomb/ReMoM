/*
    WIZARDS.EXE
        seg008
*/

/*
    "SPACE ALLOC" - STARLORD.EXE
*/

/*
    MoO2
    Module: allocate
    Allocate_Space
    Allocate_Space_No_Header
    Allocate_First_Block
    Allocate_Next_Block
    Allocation_Error
    Reallocation_Error

    Allocate_Space
        Num params: 1
        Return type: pointer (4 bytes) 
            signed integer (4 bytes) size
            pointer (4 bytes) name
            pointer (4 bytes) header
            signed integer (4 bytes) lsize

*/

#include "MoX_TYPE.H"   /* SAMB_ptr, etc. */
#include "MoX_BITS.H"   /* GET_2B_OFS() etc. */
#include "MoX_BASE.H"    /* ST_FAILURE, etc. */

#include "MoX_DBG.H"    /* Check_Release_Version() */

#include "Allocate.H"

#include <malloc.h>     /* malloc() */
#include <stdlib.h>     /* itoa() */
#include <string.h>     /* strcat(), strcpy() */



//                                          ¿ BEGIN: Allocate.C - Initialized Data ?
// WZD dseg:727C
// drake178: LBX_NearBuf_Used
int16_t near_buffer_used = 256;

// WZD dseg:727E
// char cnst_Alloc_Error01[] = "Near Allocation too large by ";
// WZD dseg:729C
// char cnst_Alloc_Error02[] = " bytes";

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
//                                          ¿ END: Allocate.C - Initialized Data ?
// WZD dseg:7602 72 00                                           cstr_R__2 db 'r',0                      ; DATA XREF: DISK_OpenCreate:loc_169A8o



// WZD dseg:D496 00 00                                           save_active dw 0                        ; DATA XREF: Load_Screen+D1w ...
//                                          ¿ BEGIN: Allocate.C - Uninitialized Data ?

// WZD dseg:D498
// drake178: LBX_NearBuf_Mark
int16_t near_buffer_mark;

// ¿ ? // WZD dseg:D49A
// ¿ ? char Temp_String[100];
// ¿ ? // WZD dseg:D4FE
// ¿ ? char Tmp_Conv_Str_1[20];
// ¿ ? // WZD dseg:D512
// ¿ ? char Tmp_Conv_Str_2[30];
// ¿ ? // WZD dseg:D530
// ¿ ? char Tmp_Conv_Str_3[106];
// ¿ ? // WZD dseg:D59A
// ¿ ? char LBX_Near_Buffer[4144];
// WZD dseg:D49A
char near_buffer[4400];
#define NEAR_BUFFER_SIZE 4400
#define NEAR_BUFFER_RESERVED 256
#define NEAR_BUFFER_UNRESERVED 4144

// WZD dseg:E5CA 00 00 00 00                                     fp_tmpSAMB dd 0                         ; DATA XREF: Allocate_Space+28w ...
// WZD dseg:E5CE 00 00                                           RAM_MinKbytes dw 0                      ; DATA XREF: Allocation_Error+2Cr ...
//                                          ¿ END: Allocate.C - Uninitialized Data ? ; set to 583 in _main
// WZD dseg:E5D0 00 00                                           LBX_LastLoadECount dw 0                 ; DATA XREF: LBX_Load_Entry+1C6w ...

/*
Notes from MGC Dasm

// MGC dseg:948A
    ; int GUI_Help_NearAlloc
    GUI_Help_NearAlloc dw 0

// MGC dseg:948C
    ; unsigned int gsa_GUI_Credits_Text_IMG
    gsa_GUI_Credits_Text_IMG dw 0
        ; MGC_DEF.H C35h LBX_Alloc_First paragraphs in the sandbox, now formatted for a 280x122 image

// MGC dseg:948E
    g_NearHeap_Mark dw 0

// MGC dseg:9490
    g_Temp_String db 100 dup(  0)
        ; 100 bytes ~= char TempString[100]

// MGC dseg:94F4
    Tmp_Conv_Str_1 db 20 dup(0)
        ; 20 bytes ~= char Tmp_Conv_Str_1[20]

// MGC dseg:9508
    Tmp_Conv_Str_2 db 30 dup(0)
        ; 30 bytes ~= char Tmp_Conv_Str_2[30]

// MGC dseg:9526
    Tmp_Conv_Str_3 db 106 dup(0)
        ; 106 bytes ~= char Tmp_Conv_Str_3[106]
        ; ...maybe sizeof/max set at compile-time?
        ; ...Nein! 2021118
        ; 256 - (100 + 20 + 30)
        ; probably not actually (specifically) 106
        ; just left-over or some unseeable other

// MGC dseg:9590
    SA_NearHeap_Buffer db 4144 dup(0)
        ; 4144 B (?4K + 48B?) (4.046875 KB)
        ;
        ; elsewhere/later...
        ; there is a check for 4400 (256 + 4144)
        ; and a read of 4000
        ;
*/


SAMB_ptr tmp_SAMB_head;  // rename - ?pTmpSAMB?, g_header, tmp_header_ptr, _header_ptr, ?!?


char * str_allocation_errors[] =
{
    "Near Allocation too large by ",
    " bytes",
    "Insufficient memory. You need at least ",
    "K free. Try removing all TSR's.",
    "Dynamic allocation too small for Allocate_Space()",
    " of ",
    " blocks",
    "Failed to reload",
    " Allocate_Next_Block()",
    ": Short by ",
    "Allocation space has been corrupted for",
    " (EMM) "
};


/*
    MAGIC.EXE  seg007
*/

// s07p04
int16_t Check_Allocation(SAMB_ptr SAMB_head)
{
    int16_t is_valid;

    if (SA_GET_MEMSIG1(SAMB_head) != SA_MEMSIG1 || SA_GET_MEMSIG2(SAMB_head) != SA_MEMSIG2)
    {
        is_valid = ST_FAILURE;
    }
    else
    {
        is_valid = ST_SUCCESS;
    }

    return is_valid;
}


/*
    MAGIC.EXE  seg008
*/

// WZD s08p01
// drake178: LBX_NearAlloc_Mark()
void Near_Allocate_Mark(void)
{
    near_buffer_mark = near_buffer_used;
}

// WZD s08p02
// drake178: LBX_NearAlloc_Undo()
void Near_Allocate_Undo(void)
{
    near_buffer_used = near_buffer_mark;
}

// WZD s08p03
// drake178: LBX_NearAlloc_Reset()
void Near_Allocate_Reset(void)
{
    near_buffer_used = NEAR_BUFFER_RESERVED;
    near_buffer_mark = NEAR_BUFFER_RESERVED;
}

// WZD s08p04
// drake178: LBX_NearAlloc_First()
byte_ptr Near_Allocate_First(int16_t size)
{

    if(size > NEAR_BUFFER_UNRESERVED) /* ¿ 4400 - 256 ? */
    {
        Near_Allocation_Error(size - NEAR_BUFFER_SIZE + NEAR_BUFFER_RESERVED);
    }
    near_buffer_used = size + NEAR_BUFFER_RESERVED;
    near_buffer_mark = size + NEAR_BUFFER_RESERVED;
    return (byte_ptr)&near_buffer[NEAR_BUFFER_RESERVED];
}

// WZD s08p05
// drake178: LBX_NearAlloc_Next()
byte_ptr Near_Allocate_Next(int16_t size)
{
    int16_t tmp_near_buffer_used;

    if((NEAR_BUFFER_SIZE - near_buffer_used) < size)
    {
        Near_Allocation_Error(size - NEAR_BUFFER_SIZE + near_buffer_used);
    }
    tmp_near_buffer_used = near_buffer_used;
    near_buffer_used += size;
    return (byte_ptr)&near_buffer[tmp_near_buffer_used];
}

// WZD s08p06
// drake178: LBX_NearAlloc_Error()
void Near_Allocation_Error(int16_t size)
{
    strcpy(near_buffer, "Near Allocation too large by ");  // cnst_Alloc_Error01[] = "Near Allocation too large by "
    // itoa(size, Tmp_Conv_Str_1, 10);
#pragma warning(suppress : 4996)
    itoa(size, &near_buffer[100], 10);
    // strcat(near_buffer, Tmp_Conv_Str_1);
    strcat(near_buffer, &near_buffer[100]);
    strcat(near_buffer, " bytes");  // cnst_Alloc_Error02[] = " bytes"

    // TODO  Exit_With_Message((char *)&near_buffer[0]);
}


// WZD s08p07
SAMB_ptr Allocate_Space(uint16_t size)
{
    int32_t lsize;
    SAMB_ptr SAMB_head;

    lsize = (size + 1) * 16;

    tmp_SAMB_head = (SAMB_ptr) malloc(lsize);
    if(tmp_SAMB_head == NULL) { Allocation_Error(1, size); }
    SAMB_head = tmp_SAMB_head + 12;  // 16-byte paragraph - 4-byte malloc header

#pragma warning(suppress : 6011)
    SA_SET_MEMSIG1(SAMB_head);
#pragma warning(suppress : 6011)
    SA_SET_MEMSIG2(SAMB_head);
#pragma warning(suppress : 6011)
    SA_SET_SIZE(SAMB_head,size);
#pragma warning(suppress : 6011)
    SA_SET_USED(SAMB_head,1);

    // Update_MemFreeWorst_KB();

    return SAMB_head;
}

// WZD s08p08 / s08p09
SAMB_ptr Allocate_Space_No_Header(uint16_t size)
{
    int32_t lsize;
    SAMB_ptr SAMB_data;

    lsize = (size + 1) * 16;

    tmp_SAMB_head = (SAMB_ptr) malloc(lsize);
    if(tmp_SAMB_head == NULL) { Allocation_Error(1, size); }
    SAMB_data = tmp_SAMB_head + 12;  // 16-byte paragraph - 4-byte malloc header

    // Update_MemFreeWorst_KB();

    return SAMB_data;
}


// WZD s08p10
// drake178: LBX_Alloc_Mark
void Mark_Block(SAMB_ptr block)
{
    int16_t used;
    // MSDOS  used = farpeekw(block, SAMB_HDR_POS_USED);
    // MSDOS  farpokew(block, SMB_HDR_POS_MARK);
    used = SA_GET_USED(block);
    SA_SET_MARK(block, used);
}


// WZD s08p11
// drake178: LBX_Alloc_Undo()
void Release_Block(SAMB_ptr block)
{
    int16_t mark;
    // MSDOS  used = farpeekw(block, SAMB_HDR_POS_MARK);
    // MSDOS  farpokew(block, SMB_HDR_POS_USED);
    mark = SA_GET_MARK(block);
    SA_SET_USED(block, mark);

}


// WZD s08p12
SAMB_ptr Allocate_First_Block(SAMB_ptr SAMB_head, uint16_t size)
{
    SAMB_ptr sub_SAMB_head;
    SAMB_ptr sub_SAMB_data;

    if(Check_Allocation(SAMB_head) == ST_FAILURE) { Allocation_Error(0x03, size+1); }
    if(SA_GET_SIZE(SAMB_head) < size) { Allocation_Error(0x02, size+1); }

    // block header + sub block header + sub block size
    SA_SET_USED(SAMB_head,1+(1+size));

    sub_SAMB_head = SAMB_head + 16;  // ~== &SAMB_head[16]

    SA_SET_MEMSIG1(sub_SAMB_head);
    SA_SET_MEMSIG2(sub_SAMB_head);
    SA_SET_SIZE(sub_SAMB_head,size);
    SA_SET_USED(sub_SAMB_head,1);
    SA_SET_MARK(sub_SAMB_head,1);

    sub_SAMB_data = sub_SAMB_head + 16;  // ~== &sub_SAMB_head[16]

    return sub_SAMB_data;
}

// WZD s08p13
// drake178: LBX_Alloc_Flush()
void Reset_First_Block(SAMB_ptr block)
{
    // MSDOS  SAMB_ptr header;
    // MSDOS  uint16_t size;
    // MSDOS  header = block + (1 * 16);  // SZ_SEGMENT
    // MSDOS  size = farpeekw(block, SAMB_HDR_POS_SIZE);
    // MSDOS  farpokew(block, SAMB_HDR_POS_USED, 1);

    SA_SET_USED(block, 1);
}

// WZD s08p14
SAMB_ptr Allocate_Next_Block(SAMB_ptr SAMB_head, uint16_t size)
{
    SAMB_ptr sub_SAMB_head;
    SAMB_ptr sub_SAMB_data;
    uint16_t old_used;
    uint16_t new_used;
    
    if(Check_Allocation(SAMB_head) == ST_FAILURE) { Allocation_Error(0x03, size); }
    if(Get_Free_Blocks(SAMB_head) < size+1) { Allocation_Error(0x02, size + 1); }

    old_used = SA_GET_USED(SAMB_head);

    new_used = (SA_GET_USED(SAMB_head) + size + 1);

    SA_SET_USED(SAMB_head, new_used);
    
    sub_SAMB_head = SAMB_head + (old_used * 16);  // ~== &SAMB_head[SAMB->used]

    SA_SET_MEMSIG1(sub_SAMB_head);
    SA_SET_MEMSIG2(sub_SAMB_head);
    SA_SET_SIZE(sub_SAMB_head,size);
    SA_SET_USED(sub_SAMB_head,1);
    // SA_SET_MARK(sub_SAMB_head,1);

    sub_SAMB_data = sub_SAMB_head + 16;  // ~== &sub_SAMB_head[16]

    return sub_SAMB_data;
}


// WZD s08p15
// drake178: LBX_Alloc_GetFree()
// ¿ MoO2  Module: allocate  Get_Remaining_Block_Space() ?
uint16_t Get_Free_Blocks(SAMB_ptr SAMB_head)
{
    return SA_GET_SIZE(SAMB_head) - SA_GET_USED(SAMB_head);
}

// WZD s08p16
// WZD s08p17
// WZD s08p18

// WZD s08p19
void Allocation_Error(uint16_t error_num, uint16_t blocks)
{
    char buffer[120] = {0};
    char buffer2[20] = {0};

    if(Check_Release_Version() == ST_TRUE)
    {
        strcpy(buffer, "Insufficient memory. You need at least ");
        itoa(640, buffer2, 10);
        strcat(buffer, buffer2);
        strcat(buffer, "K free. Try removing all TSR's.");
    }
    else
    {
        switch(error_num)
        {
            case 1:
                strcpy(buffer, str_allocation_errors[4]);
                strcat(buffer, str_allocation_errors[5]);
                break;
            case 2:
                strcpy(buffer, str_allocation_errors[7]);
                strcat(buffer, str_allocation_errors[8]);
                strcat(buffer, str_allocation_errors[9]);
                break;
            case 3:
                strcpy(buffer, str_allocation_errors[10]);
                strcat(buffer, str_allocation_errors[8]);
                strcat(buffer, str_allocation_errors[5]);
                break;
            case 4:
                strcpy(buffer, str_allocation_errors[7]);
                strcat(buffer, str_allocation_errors[11]);
                strcat(buffer, str_allocation_errors[8]);
                strcat(buffer, str_allocation_errors[9]);
                break;
        }
        itoa(blocks, buffer2, 10);
        strcat(buffer, buffer2);
        strcat(buffer, str_allocation_errors[6]);
    }

    // TODO  Exit_With_Message(buffer);
}
