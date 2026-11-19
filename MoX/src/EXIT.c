/*
    WIZARDS.EXE
        seg005
        seg006
    MoO2:
        Module: exit
*/

#include "../../STU/src/STU_LOG.h"
#include "../../ext/stu_compat.h"

#include "EXIT.h"

#include "EMS/EMS.h"
#include "Mouse.h"
#include "SOUND.h"

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#define _CRT_NONSTDC_NO_DEPRECATE
#include <conio.h>
#include <io.h>      /* CLAUDE: _isatty */
#endif



// MoO1 ORION dseg:5DDA                         BEGIN:  seg016 - Initialized Data
// MoO1 ORION dseg:5DDA
// MoO1 ORION dseg:5DDA 00 00                   fh_ERROR_LOG    dw 0                    ; DATA XREF: DBG_Open_ERROR_LOG+13^w
// MoO1 ORION dseg:5DDA                                                                 ; DBG_Open_ERROR_LOG+22^r ...
// MoO1 ORION dseg:5DDC 00 00                   fh2_ERROR_LOG   dw 0                    ; DATA XREF: DBG_Open_ERROR_LOG+1D^w
// MoO1 ORION dseg:5DDC                                                                 ; DBG_Close_ERROR_LOG+1C^r ...
// MoO1 ORION dseg:5DDE 45 52 52 4F 52 2E 4C 4F+str_ERROR_LOG      db 'ERROR.LOG',0        ; DATA XREF: DBG_Open_ERROR_LOG+E^o
// MoO1 ORION dseg:5DDE 47 00
// MoO1 ORION dseg:5DDE                         END:  seg016 - Initialized Data



// dseg:3CC2
uint16_t MemFreeWorst_KB = 64000;  // (conventional memory is at most 640 KB)

// dseg:3CC4  cnst_Quit_Report1 = "Data";              
// dseg:3CC9  cnst_Quit_Report2 = " Free: ";           
// dseg:3CD1  cnst_Quit_Report3 = " bytes   Memory";   
// dseg:3CE1  cnst_Quit_Report4 = "k   Worst";         
// dseg:3CEB  cnst_Quit_Report5 = "k   EMM: ";         
// dseg:3CF5  cnst_Quit_Report6 = " blocks";           

/* EOG_HACK */  // DONT  // WZD dseg:3CFD
/* EOG_HACK */  // DONT  char cnst_ProgSwap_Error1[] = "Error- game file ";
/* EOG_HACK */  // DONT  // WZD dseg:3D0F
/* EOG_HACK */  // DONT  char cnst_ProgSwap_Error2[] = " could not be found";




/*
    WIZARDS.EXE  seg005
*/

// WZD s05p01
/*
    Debug exit (F11_Key(), non-release builds only): exits with a memory report.
    "Data Free: <near heap> bytes   Memory Free: <largest DOS block>k   Worst Free: <smallest seen>k   EMM: <reserved pages> blocks"
    MAGIC.EXE has the same function (MGC s05p01).
*/
void Exit_With_Size(void)
{
    char number_string[20] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char message[120] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint16_t near_heap_free_bytes = 0;
    uint16_t worst_free_kb = 0;
    uint16_t current_free_kb = 0;
    // current_free_kb = dos_memfree_kb();
    /* HACK */ current_free_kb = 0;  /* asm: call Dos_Free -- INT 21h AH=48h BX=FFFFh, the largest free DOS block in paragraphs >> 6 = KB; there is no DOS memory to ask */
    worst_free_kb = MemFreeWorst_KB;
    // near_heap_free_bytes = coreleft();
    /* HACK */ near_heap_free_bytes = 0;  /* asm: call coreleft -- Borland RTL, the free bytes in the near heap; there is no near heap */
    stu_strcpy(message, "Data");
    stu_strcat(message, " Free: ");
    stu_ltoa(near_heap_free_bytes, number_string, 10);
    stu_strcat(message, number_string);
    stu_strcat(message, " bytes   Memory");
    stu_strcat(message, " Free: ");
    stu_itoa(current_free_kb, number_string, 10);
    stu_strcat(message, number_string);
    stu_strcat(message, "k   Worst");
    stu_strcat(message, " Free: ");
    stu_itoa(worst_free_kb, number_string, 10);
    stu_strcat(message, number_string);
    stu_strcat(message, "k   EMM: ");
    stu_itoa(EMM_Pages_Reserved, number_string, 10);
    stu_strcat(message, number_string);
    stu_strcat(message, " blocks");
    Exit_With_Message(message);
}


// WZD s05p02
// TODO  void (*Exit_With_Message)(char* string);
void Exit_With_Message(char * string)
{
    Stop_Music__STUB();
    Reset_System_Mouse();
    Audio_Uninit__STUB();
    EMM_ReleaseAll__STUB();
    DBG_Close_ERROR_LOG__STUB();
    Reset_Video();
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
// void Exit_With_Value(int16_t value);


// WZD s05p04
/* EOG_HACK */  // DONT  void Respawn(char * exe_name, char * argv0, char * argv1, char * argv2)
/* EOG_HACK */  // DONT  {
/* EOG_HACK */  // DONT      char exit_message[82];
/* EOG_HACK */  // DONT      int handle_index;
/* EOG_HACK */  // DONT      Stop_Music__STUB();
/* EOG_HACK */  // DONT      if(!Check_Default_Video_Page())
/* EOG_HACK */  // DONT      {
/* EOG_HACK */  // DONT          Copy_On_To_Off_Page();
/* EOG_HACK */  // DONT          Toggle_Pages();
/* EOG_HACK */  // DONT      }
/* EOG_HACK */  // DONT      Reset_System_Mouse();
/* EOG_HACK */  // DONT      Audio_Uninit__STUB();
/* EOG_HACK */  // DONT      fcloseall();
/* EOG_HACK */  // DONT      for(handle_index = 5; handle_index < 30; handle_index++)
/* EOG_HACK */  // DONT      {
/* EOG_HACK */  // DONT          /* INT 21h AH=3Eh: Close file handle */
/* EOG_HACK */  // DONT          union REGS regs;
/* EOG_HACK */  // DONT          regs.h.ah = 0x3E;
/* EOG_HACK */  // DONT          regs.x.bx = (unsigned int)handle_index;
/* EOG_HACK */  // DONT          int86(0x21, &regs, &regs);
/* EOG_HACK */  // DONT      }
/* EOG_HACK */  // DONT      EMM_Release_Resd();
/* EOG_HACK */  // DONT      execl(exe_name, exe_name, argv0, argv1, argv2, (char *)0);
/* EOG_HACK */  // DONT      stu_strcpy(exit_message, cnst_ProgSwap_Error1);
/* EOG_HACK */  // DONT      stu_strcat(exit_message, exe_name);
/* EOG_HACK */  // DONT      stu_strcat(exit_message, cnst_ProgSwap_Error2);
/* EOG_HACK */  // DONT      Exit_With_Message(exit_message);
/* EOG_HACK */  // DONT  }


// WZD s05p05
// Update_MemFreeWorst_KB()


// WZD s05p06
// UU_VGA_B800Dump()


// WZD s05p07
// UU_VGA_TextModeString()



/*
    WIZARDS.EXE  seg006
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
    LOG_INFO(LOG_CAT_EXIT, "%s", string);

#ifdef _WIN32
    // getch();  not without _CRT_INTERNAL_NONSTDC_NAMES
    // _getch();  // 6031 Return value ignored
    // char ch = _getch();
    /* CLAUDE: only pause for a keypress at an interactive console.  Under a
       windowless / redirected-stdin run (DeMoM, CTest, CI) _getch() blocks
       forever waiting for a key that never arrives, turning every fatal
       error into a hang instead of a clean exit-with-message. */
    if(_isatty(_fileno(stdin)))
    {
        #pragma warning(suppress : 6031)  // 6031 Return value ignored
        _getch();
    }

#endif

    exit(EXIT_FAILURE);
    // Exception thrown at 0x00007FF9012D698B (ntdll.dll) in 010_ReMoM.exe: 
    // 0xC0000005: Access violation reading location 0xFFFFFFFFFFFFFFF8.
    
    // PostQuitMessage();
    // ...crashes SDL2

    // return;
    // exit(EXIT_SUCCESS);

}

// WZD s06p03
// UU_DOS_PrintString2()
// 1oom  dos_write_str()

// WZD s06p04
// RAM_GetFreeBlockSize()
// 1oom  get_free_mem_k()

// WZD s06p05
// drake178: ¿ ?
// AKA s06p05_Empty_pFxn()
// MoO1 ORION seg006 STARMAP seg016
/*
XREF:
    Init_Drivers()
    UU_Legacy_Startup()
*/
void DBG_Open_ERROR_LOG__STUB(void)
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

// WZD s06p06
// drake178: ¿ ?
// AKA  s06p06_Empty_pFxn()
// MoO1 ORION seg006 STARMAP seg016
void DBG_Close_ERROR_LOG__STUB(void)
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
// seg006:007A DBG_Open_ERROR_LOG proc far             ; CODE XREF: init_fonts_sound_mouse_etc+7vP
// seg006:007A                                         ; seg014:00AFvP
// seg006:007A                 push    es
// seg006:007B                 push    ds
// seg006:007C                 push    si
// seg006:007D                 push    di
// seg006:007E                 pop     di
// seg006:007F                 pop     si
// seg006:0080                 pop     ds
// seg006:0081                 pop     es
// seg006:0082                 retf
// seg006:0083 ; ---------------------------------------------------------------------------
// seg006:0083                 mov     ah, 3Ch
// seg006:0085                 mov     cx, 0
// seg006:0088                 mov     dx, offset str_ERROR_LOG ; "ERROR.LOG"
// seg006:008B                 int     21h             ; DOS - 2+ - CREATE A FILE WITH HANDLE (CREAT)
// seg006:008B                                         ; CX = attributes for file
// seg006:008B                                         ; DS:DX -> ASCIZ filename (may include drive and path)
// seg006:008D                 mov     fh_ERROR_LOG, ax
// seg006:0090                 mov     ah, 45h
// seg006:0092                 mov     bx, 2
// seg006:0095                 int     21h             ; DOS - 2+ - CREATE DUPLICATE HANDLE (DUP)
// seg006:0095                                         ; BX = file handle to duplicate
// seg006:0097                 mov     fh2_ERROR_LOG, ax
// seg006:009A                 mov     ah, 46h
// seg006:009C                 mov     bx, fh_ERROR_LOG
// seg006:00A0                 mov     cx, 2
// seg006:00A3                 int     21h             ; DOS - 2+ - FORCE DUPLICATE HANDLE (FORCDUP,DUP2)
// seg006:00A3                                         ; BX = existing file handle, CX = new file handle
// seg006:00A5                 pop     di
// seg006:00A6                 pop     si
// seg006:00A7                 pop     ds
// seg006:00A8                 pop     es
// seg006:00A9                 retf
// seg006:00A9 DBG_Open_ERROR_LOG endp
// seg006:00A9
// seg006:00AA
// seg006:00AA ; =============== S U B R O U T I N E =======================================
// seg006:00AA
// seg006:00AA
// seg006:00AA DBG_Close_ERROR_LOG proc far            ; CODE XREF: Exit_With_Message+17^P
// seg006:00AA                 push    es
// seg006:00AB                 push    ds
// seg006:00AC                 push    si
// seg006:00AD                 push    di
// seg006:00AE                 pop     di
// seg006:00AF                 pop     si
// seg006:00B0                 pop     ds
// seg006:00B1                 pop     es
// seg006:00B2                 retf
// seg006:00B3 ; ---------------------------------------------------------------------------
// seg006:00B3                 mov     bx, fh_ERROR_LOG
// seg006:00B7                 cmp     bx, 0
// seg006:00BA                 jz      short loc_4687
// seg006:00BC                 mov     ah, 3Eh
// seg006:00BE                 mov     bx, fh_ERROR_LOG
// seg006:00C2                 int     21h             ; DOS - 2+ - CLOSE A FILE WITH HANDLE
// seg006:00C2                                         ; BX = file handle
// seg006:00C4                 mov     ah, 46h
// seg006:00C6                 mov     bx, fh2_ERROR_LOG
// seg006:00CA                 mov     cx, 2
// seg006:00CD                 int     21h             ; DOS - 2+ - FORCE DUPLICATE HANDLE (FORCDUP,DUP2)
// seg006:00CD                                         ; BX = existing file handle, CX = new file handle
// seg006:00CF                 mov     ah, 3Eh
// seg006:00D1                 mov     bx, fh2_ERROR_LOG
// seg006:00D5                 int     21h             ; DOS - 2+ - CLOSE A FILE WITH HANDLE
// seg006:00D5                                         ; BX = file handle
// seg006:00D7
// seg006:00D7 loc_4687:                               ; CODE XREF: DBG_Close_ERROR_LOG+10^j
// seg006:00D7                 pop     di
// seg006:00D8                 pop     si
// seg006:00D9                 pop     ds
// seg006:00DA                 pop     es
// seg006:00DB                 retf
// seg006:00DB DBG_Close_ERROR_LOG endp
// seg006:00DB
// seg006:00DB seg006          ends
