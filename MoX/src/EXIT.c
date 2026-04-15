/*
    WIZARDS.EXE
        seg005
        seg006
    MoO2:
        Module: exit
*/

#include "../../ext/stu_compat.h"

#include "Mouse.h"
#include "SOUND.h"
#include "EMS/EMS.h"

#include "EXIT.h"

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#define _CRT_NONSTDC_NO_DEPRECATE
#include <conio.h>
#endif



// MoO1 ORION dseg:5DDA                         BEGIN:  seg016 - Initialized Data
// MoO1 ORION dseg:5DDA
// MoO1 ORION dseg:5DDA 00 00                   fh_ERROR_LOG    dw 0                    ; DATA XREF: DBG_Open_ERROR_LOG+13w
// MoO1 ORION dseg:5DDA                                                                 ; DBG_Open_ERROR_LOG+22r ...
// MoO1 ORION dseg:5DDC 00 00                   fh2_ERROR_LOG   dw 0                    ; DATA XREF: DBG_Open_ERROR_LOG+1Dw
// MoO1 ORION dseg:5DDC                                                                 ; DBG_Close_ERROR_LOG+1Cr ...
// MoO1 ORION dseg:5DDE 45 52 52 4F 52 2E 4C 4F+str_ERROR_LOG      db 'ERROR.LOG',0        ; DATA XREF: DBG_Open_ERROR_LOG+Eo
// MoO1 ORION dseg:5DDE 47 00
// MoO1 ORION dseg:5DDE                         END:  seg016 - Initialized Data


// AKA MemFreeWorst_KB
int max_far_size = 64000;                    // dseg:3CC2  0xFA00  ? Bytes vs. Paragrphs ? 62.5 KB vs 4000 Pr ? sizeof VGA 320x200 ?
char * msg_size_1 = "Data";              // dseg:3CC4
char * msg_size_2_4_6 = " Free: ";           // dseg:3CC9
char * msg_size_3 = " bytes   Memory";   // dseg:3CD1
char * msg_size_5 = "k   Worst";         // dseg:3CE1
char * msg_size_7 = "k   EMM: ";         // dseg:3CEB
char * msg_size_8 = " blocks";           // dseg:3CF5



// s05p01
void Exit_With_Size(void)
{
    char temp_string[LEN_TEMP_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char string[LEN_EXIT_MESSAGE] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t data_free = 0;
    int16_t worst_free = 0;
    int16_t current_free = 0;

    current_free = Dos_Free();
    worst_free = max_far_size;
    /* HACK */  data_free = 1; // ¿ emulate conventional memory w/ interruprs to manufacture need for coreleft() ?
    stu_strcpy(string, msg_size_1);
    stu_strcat(string, msg_size_2_4_6);
    stu_ltoa(data_free, temp_string, 10);  // Data Free: bytes
    stu_strcat(string, temp_string);
    stu_strcat(string, msg_size_3);
    stu_strcat(string, msg_size_2_4_6);
    stu_itoa(current_free, temp_string, 10);  // "Memory Free: k"
    stu_strcat(string, temp_string);
    stu_strcat(string, msg_size_5);
    stu_strcat(string, msg_size_2_4_6);
    stu_itoa(worst_free, temp_string, 10);  // "Worst Free k"
    stu_strcat(string, temp_string);
    stu_strcat(string, msg_size_7);
    stu_itoa(EMM_Pages_Reserved, temp_string, 10);  // "EMM: blocks"
    stu_strcat(string, temp_string);
    stu_strcat(string, msg_size_8);

    Exit_With_Message(string);
}


// WZD s05p02
void Exit_With_Message(char * string)
{
    // MoO2  Reset_System()
    Stop_Music__STUB();
    Reset_System_Mouse();
    Audio_Uninit__STUB();
    EMM_Release_All();
    DBG_Close_ERROR_LOG();
    Reset_Video();
    // MoO2   printf_(printf_fmt_string_newline, string); exit_(1);
    Quit_With_Message(string);
}
/*
    MoO2
void Exit_With_Message(char * string)
{
    Reset_System();
    printf_(printf_fmt_string_newline, string);  // ; "%s\n"
    exit_(EXIT_FAILURE)
}
void Reset_System(void)
{
    if(IDK_system_reset == 0)
    {
        Reset_System_Mouse();
        Reset_Video();
        Disable_Keyboard();
        Audio_Uninit();
        Net_Uninit();
    }
    IDK_system_reset = 1;
}
*/



// WZD s05p03
// drake178: ¿ ?
// MoO1  ¿ ?
// Mo2  Module: exit  Exit_With_Value()
void Exit_With_Value(int16_t value)
{
    char string[40] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    // MoO2   itoa_(value, string, 10);
    // MoO2  Reset_System()
    stu_itoa(value, string, 10);
    Stop_Music__STUB();
    Reset_System_Mouse();
    Audio_Uninit__STUB();
    EMM_Release_All();
    DBG_Close_ERROR_LOG();
    Reset_Video();
    // MoO2   printf_(printf_fmt_string_newline, string); exit_(1);
    Quit_With_Message(string);
}
/*
Exit_With_Value     proc near
string              = byte ptr -2Ch
value               = dword ptr -4
push 50h
call __CHK
push ebx
push ecx
push edx
push esi
push edi
push ebp
mov ebp, esp
sub esp, 2Ch
mov [ebp+value], eax
mov ebx, 10
lea edx, [ebp+string]
mov eax, [ebp+value]
call itoa_
call Reset_System
lea eax, [ebp+string]
push eax
mov eax, offset printf_fmt_string_newline                                       ; "%s\n"
push eax
call printf_
add esp, 8
mov eax, 1
call exit_
mov esp, ebp
pop ebp
pop edi
pop esi
pop edx
pop ecx
pop ebx
retn
endp
*/


// WZD s05p04
// GAME_EXE_Swap()

// WZD s05p05
// AKA Update_MemFreeWorst_KB()
void Check_Free(void)
{
    int16_t size = 0;
    size = Dos_Free();
    if(size > max_far_size)
    {
        max_far_size = size;
    }
}

// WZD s05p06
// UU_VGA_B800Dump()

// WZD s05p07
// UU_VGA_TextModeString()



/*
    WIZARDS.EXE  seg006
*/

// WZD s06p01
// drake178: VGA_SetTextMode()
/*
switches the graphics to text mode 03 and clears
the screen
*/
/*
sets the VGA video mode to text mode 03h
(this is the normal/default text mode)
*/
void Reset_Video(void)
{
// mov     ax, 3
// int     10h                             ; - VIDEO - SET VIDEO MODE
//                                         ; AL = mode
}
/*
    MoO2
    #include <i86.h>
    void Reset_Video(void)
    {
        mov     [ebp+in_regs], 3
        lea     ebx, [ebp+out_regs]
        lea     edx, [ebp+in_regs]
        mov     eax, 10h
        call    int386_
    }
*/


// WZD s06p02
// drake178: ¿ ?
// AKA QuitToDOS
/*
exits to DOS with the provided message
same as UU_DOS_PrintString,
  except DOS QUIT WITH EXIT CODE
*/
/*
Hrmm... Asm, BC/BCpp, or MS C code?
This has the customary C function pre & postamble.
Is it meaningful that is does not push and pop SI?
...
didn't see anything is MSC that looks like this was a library function
...
probably should be named something MS-DOS specific
maybe also something specifc to getting the two blank lines following - 
*/
void Quit_With_Message(char * string)
{
// mov     si, [bp+string]
// FindNull:
// lodsb
// cmp     al, 0
// jnz     short FindNull
// dec     si
// mov     [byte ptr si], 0Dh
// inc     si
// mov     [byte ptr si], 0Ah
// inc     si
// mov     [byte ptr si], 0Dh
// inc     si
// mov     [byte ptr si], 0Ah
// inc     si
// mov     [byte ptr si], '$'
// inc     si
// mov     dx, [bp+string]
// mov     ah, 9
// int     21h                               ; DOS - PRINT STRING
//                                           ; DS:DX -> string terminated by "$"
// mov     ah, 4Ch
// int     21h                               ; DOS - 2+ - QUIT WITH EXIT CODE (EXIT)
//                                           ; AL = exit code
    // char * ptr;
    // ptr = string;
    // while(*ptr++ != '\0') {  }
    // ptr--;
    // *ptr++ = '0x0D';
    // *ptr++ = '0x0A';
    // *ptr++ = '0x0D';
    // *ptr++ = '0x0A';
    // *ptr++ = '$';

    // ~== //MoO2
    // ORION2.LE  dseg02:00172670 25 73 0A 00                                     printf_fmt_string_newline db '%s',0Ah,0
    // mov     eax, [ebp+string]
    // push    eax
    // mov     eax, offset printf_fmt_string_newline ; "%s\n"
    // push    eax
    // call    printf_
    // add     esp, 8
    // mov     eax, 1
    // call    exit_

    printf("%s\n\n", string);

/* HACK */  #ifdef _WIN32
/* HACK */      // getch();  not without _CRT_INTERNAL_NONSTDC_NAMES
/* HACK */      // _getch();  // 6031 Return value ignored
/* HACK */      // char ch = _getch();
/* HACK */      #pragma warning(suppress : 6031)  // 6031 Return value ignored
/* HACK */      _getch();
/* HACK */  
/* HACK */  #endif

    exit(EXIT_FAILURE);
    // Exception thrown at 0x00007FF9012D698B (ntdll.dll) in 010_ReMoMber.exe: 
    // 0xC0000005: Access violation reading location 0xFFFFFFFFFFFFFFF8.
    
    // PostQuitMessage();
    // ...crashes SDL2

    // return;
    // exit(EXIT_SUCCESS);

}

// WZD s06p03
// UU_DOS_PrintString2()
// 1oom  dos_write_str()
/*
No *Quit*, but same print string functionality as Quit_With_Message(), byte-for-byte
*/
void Print_Message(char * string)
{
    printf("%s\n\n", string);
}

// WZD s06p04
// AKA RAM_GetFreeBlockSize()
// 1oom  get_free_mem_k()
// ¿ Linear_Free(), because B to KB ?
int16_t Dos_Free(void)
{
// mov     ah, 48h
// mov     bx, 0FFFFh
// int     21h                             ; DOS - 2+ - ALLOCATE MEMORY
//                                         ; BX = number of 16-byte paragraphs desired
// mov     ax, bx                          ; largest available block size
// shr     ax, 1
// shr     ax, 1
// shr     ax, 1
// shr     ax, 1
// shr     ax, 1
// shr     ax, 1
    /* HACK */  return 640;
}

// WZD s06p05
// drake178: ¿ ?
// AKA  s06p05_Empty_pFxn()
// MoO1 ORION seg006 STARMAP seg016
/*
XREF:
    Init_Drivers()
    UU_Legacy_Startup()
*/
void DBG_Open_ERROR_LOG(void)
{

}
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
    // save a copy of the file handle for STDERR
    mov     ah, 45h
    mov     bx, 2
    int     21h             ; DOS - 2+ - CREATE DUPLICATE HANDLE (DUP)  ; BX = file handle to duplicate
    mov     fh2_ERROR_LOG, ax
    // overwrite the file handle for STDERR
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


// WZD s06p06
// drake178: ¿ ?
// AKA  s06p06_Empty_pFxn()
// MoO1 ORION seg006 STARMAP seg016
/*
XREF:
    Exit_With_Message()
    UU_Exit_With_Value()
    UU_VGA_B800Dump()
*/
void DBG_Close_ERROR_LOG(void)
{

}
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
    // restore the file handle for STDERR
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
