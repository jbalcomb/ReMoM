


Allocate
Help

¿ Exit ?
¿ LBX_Load ?
¿ EMM ?



...used for messages vs. used for error messages?
...passed to Exit_With_Message()?
not always?
most common?
related to MS-DOS?
from the MS-C era?
in MoO1?
not in MoO2?

¿ Assembly language STRUC ?
    ¿ just looks like an `offset` + an *offset*?
uint8_t near_buffer[4400]
uint8_t * itoa_buffer = (uint8_t *)&near_buffer[100]


100 + 20 + 30 + 106 = 256
near_buffer_used is initialized to 256
UU_Near_Allocate_Reset() sets near_buffer_used and near_buffer_mark to 256
256 * 16 byte paragraphs is 4096
4144 - 4096 = 48 / 16 = 3 ¿ three block headers ?



Near_Allocate_First()
    if(size > 4144)
        Near_Allocation_Error(size - 4400 + 256);
            "Near allocation too large by (nbytes) bytes" message
            4144 - 4400 = -256 + 256 = 0
            4145 - 4400 = -256 + 256 = 1
    near_buffer_used = 4144 + 256 = 4400
    near_buffer_mark = 4144 + 256 = 4400
    return &near_buffer[256]  ~== &SA_NearHeap_Buffer[0]


Howzerbout if ... ?
char near_buffer[4400]
first 256 bytes are *reserved*
100, 20, 30, ¿106?

usage would suggest that it *knows* that the final string is <= 100 characters
so, ¿ ? + 20 + 30 + ¿ ? ... longest error message ... Tmp_Conv_Str_3 has to <= ¿ ? characters ... atleast 100 - (20 + 30) = 50 ?

page_count max length as string?
32 MB LIM-EMS v4 = 32 * 1024 * 1024 = 33554432 / 16 KB pages = 33554432 / 16384 = 2048 = 4 characters




WZD
dseg:D498                                                 ¿ BEGIN: Allocate.C - Uninitialized Data ?
dseg:D498 00 00                                           near_buffer_mark dw 0                   ; DATA XREF: Near_Allocate_Mark+6w ...
dseg:D49A                                                 ; char Temp_String[]
dseg:D49A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+Temp_String db 100 dup(0)               ; DATA XREF: Near_Allocate_First+2Co ...
dseg:D4FE 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+Tmp_Conv_Str_1 db 20 dup(0)             ; DATA XREF: Near_Allocation_Error+16o ...
dseg:D512 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+Tmp_Conv_Str_2 db 30 dup(0)             ; DATA XREF: EMM_GetHandle+1A2o ...
dseg:D530 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+Tmp_Conv_Str_3 db 106 dup(  0)          ; DATA XREF: EMM_GetHandle+1B3o ...
dseg:D59A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+LBX_Near_Buffer db 4144 dup(  0)
dseg:D59A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+
dseg:D59A 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+
dseg:E5CA 00 00 00 00                                     fp_tmpSAMB dd 0                         ; DATA XREF: Allocate_Space+28w ...
dseg:E5CE 00 00                                           RAM_MinKbytes dw 0                      ; DATA XREF: Allocation_Error+2Cr ...
dseg:E5CE                                                 ¿ END: Allocate.C - Uninitialized Data ? ; set to 583 in _main


### Temp_String
XREF:
    Near_Allocate_First+2C      mov     ax, offset Temp_String    
    Near_Allocate_Next+2F       mov     ax, offset Temp_String    
    Near_Allocation_Error+7     mov     ax, offset Temp_String    
    Near_Allocation_Error+29    mov     ax, offset Temp_String    
    Near_Allocation_Error+3E    mov     ax, offset Temp_String    
    EMM_GetHandle+112           mov     ax, offset Temp_String    
    EMM_GetHandle+121           mov     ax, offset Temp_String    
    EMM_GetHandle+130           mov     ax, offset Temp_String    
    EMM_GetHandle+13C           mov     ax, offset Temp_String    
    EMM_GetHandle+14B           mov     ax, offset Temp_String    
    EMM_GetHandle+156           mov     ax, offset Temp_String    
    EMM_GetHandle+1C3           mov     ax, offset Temp_String    
    EMM_GetHandle+1D2           mov     ax, offset Temp_String    
    EMM_GetHandle+1E1           mov     ax, offset Temp_String    
    EMM_GetHandle+1F0           mov     ax, offset Temp_String    
    EMM_GetHandle+1FF           mov     ax, offset Temp_String    
    EMM_GetHandle+20E           mov     ax, offset Temp_String    
    EMM_GetHandle+21D           mov     ax, offset Temp_String    
    EMM_GetHandle+228           mov     ax, offset Temp_String    
    VGA_DrawFarString+C         mov     ax, offset Temp_String    
    VGA_DrawFarString+18        mov     ax, offset Temp_String    
    VGA_DrawCenteredFar+C       mov     ax, offset Temp_String    
    VGA_DrawCenteredFar+18      mov     ax, offset Temp_String    
    UU_VGA_DrawRtAlgFar+C       mov     ax, offset Temp_String    
    UU_VGA_DrawRtAlgFar+18      mov     ax, offset Temp_String    
    VGA_WndDrawFarString+C      mov     ax, offset Temp_String    
    VGA_WndDrawFarString+18     mov     ax, offset Temp_String    
    UU_VGA_WndDrawCntrdFar+C    mov     ax, offset Temp_String    
    UU_VGA_WndDrawCntrdFar+18   mov     ax, offset Temp_String    
    UU_VGA_WndDrawRtAlgFar+C    mov     ax, offset Temp_String    
    UU_VGA_WndDrawRtAlgFar+18   mov     ax, offset Temp_String    
    UU_DBG_TblDrawString+1A     mov     ax, offset Temp_String    
    UU_DBG_TblDrawString+26     mov     ax, offset Temp_String    
    UU_STR_CopyToNearLBX+F      add     ax, offset Temp_String    
    LBX_DrawFarString+C         mov     ax, offset Temp_String    
    LBX_DrawFarString+1B        mov     ax, offset Temp_String    
    UU_LBX_DrawRtAlgFar+C       mov     ax, offset Temp_String    
    UU_LBX_DrawRtAlgFar+1B      mov     ax, offset Temp_String    
    UU_LBX_DrawCenteredFar+C    mov     ax, offset Temp_String    
    UU_LBX_DrawCenteredFar+1B   mov     ax, offset Temp_String    
    UU_LBX_DrawJustifiedFar+C   mov     ax, offset Temp_String    
    UU_LBX_DrawJustifiedFar+1E  mov     ax, offset Temp_String    
    IDK_EoT_s52B09+127          mov     ax, offset Temp_String    
    IDK_EoT_s52B09+133          mov     ax, offset Temp_String    
    CMB_CounterMessage+176      cmp     [bp+di+Temp_String-1], 's'
    UU_DBG_GetKnownSpells+3B    mov     ax, offset Temp_String    
    UU_DBG_GetKnownSpells+47    mov     ax, offset Temp_String    
    CRP_DBG_SpellTargetError+27 mov     ax, offset Temp_String    
    CRP_DBG_SpellTargetError+37 mov     ax, offset Temp_String    
    CRP_DBG_SpellTargetError+42 mov     ax, offset Temp_String    
    Draw_Help_Entry_+88         mov     ax, offset Temp_String    
    Draw_Help_Entry_+517        mov     ax, offset Temp_String    


### Tmp_Conv_Str_1
Near_Allocation_Error()
    SDL_itoa(nbytes, Tmp_Conv_Str_1, 10)
    strcat(Temp_String, Tmp_Conv_Str_1)
EMM_GetHandle+184        mov     ax, offset Tmp_Conv_Str_1
EMM_GetHandle+1DD        mov     ax, offset Tmp_Conv_Str_1


### Tmp_Conv_Str_2
EMM_GetHandle+1A2 mov     ax, offset Tmp_Conv_Str_2
EMM_GetHandle+1FB mov     ax, offset Tmp_Conv_Str_2


### Tmp_Conv_Str_3
EMM_GetHandle+1B3 mov     ax, offset Tmp_Conv_Str_3
EMM_GetHandle+219 mov     ax, offset Tmp_Conv_Str_3


### LBX_Near_Buffer
No XREFS
