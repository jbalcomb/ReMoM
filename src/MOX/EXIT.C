/*
    WIZARDS.EXE
        seg005
        seg006
    MoO2:
        Module: exit
*/

#include "EXIT.H"

#include "EMM.H"
#include "Mouse.H"
#include "SOUND.H"

#include <stdlib.h>
#include <stdio.h>



// MoO1 ORION dseg:5DDA                         BEGIN:  seg016 - Initialized Data
// MoO1 ORION dseg:5DDA
// MoO1 ORION dseg:5DDA 00 00                   fh_ERROR_LOG    dw 0                    ; DATA XREF: DBG_Open_ERROR_LOG+13w
// MoO1 ORION dseg:5DDA                                                                 ; DBG_Open_ERROR_LOG+22r ...
// MoO1 ORION dseg:5DDC 00 00                   fh2_ERROR_LOG   dw 0                    ; DATA XREF: DBG_Open_ERROR_LOG+1Dw
// MoO1 ORION dseg:5DDC                                                                 ; DBG_Close_ERROR_LOG+1Cr ...
// MoO1 ORION dseg:5DDE 45 52 52 4F 52 2E 4C 4F+str_ERROR_LOG      db 'ERROR.LOG',0        ; DATA XREF: DBG_Open_ERROR_LOG+Eo
// MoO1 ORION dseg:5DDE 47 00
// MoO1 ORION dseg:5DDE                         END:  seg016 - Initialized Data



int MemFreeWorst_KB = 64000;                    // dseg:3CC2  0xFA00  ? Bytes vs. Paragrphs ? 62.5 KB vs 4000 Pr ? sizeof VGA 320x200 ?
char * cnst_Quit_Report1 = "Data";              // dseg:3CC4
char * cnst_Quit_Report2 = " Free: ";           // dseg:3CC9
char * cnst_Quit_Report3 = " bytes   Memory";   // dseg:3CD1
char * cnst_Quit_Report4 = "k   Worst";         // dseg:3CE1
char * cnst_Quit_Report5 = "k   EMM: ";         // dseg:3CEB
char * cnst_Quit_Report6 = " blocks";           // dseg:3CF5



// s05p01
// TODO  void Exit_With_Size(void)
// TODO  {
// TODO      char temp_string[20];
// TODO      char string[120];
// TODO      unsigned int Current_FreeNearHeap_B;
// TODO      unsigned int Worst_LargestFreeBlock_KB;
// TODO      unsigned int Current_LargestFreeBlock_KB;
// TODO      
// TODO      Current_LargestFreeBlock_KB = dos_memfree_kb();
// TODO      Worst_LargestFreeBlock_KB = MemFreeWorst_KB;
// TODO      Current_FreeNearHeap_B = coreleft();
// TODO      strcpy(string, cnst_Quit_Report1);
// TODO      strcat(string, cnst_Quit_Report2);
// TODO      ltoa(Current_FreeNearHeap_B, temp_string, 10);  // Data Free: bytes
// TODO      strcat(string, temp_string);
// TODO      strcat(string, cnst_Quit_Report3);
// TODO      strcat(string, cnst_Quit_Report2);
// TODO      itoa(Current_LargestFreeBlock_KB, temp_string, 10);  // "Memory Free: k"
// TODO      strcat(string, temp_string);
// TODO      strcat(string, cnst_Quit_Report4);
// TODO      strcat(string, cnst_Quit_Report2);
// TODO      itoa(Worst_LargestFreeBlock_KB, temp_string, 10);  // "Worst Free k"
// TODO      strcat(string, temp_string);
// TODO      strcat(string, cnst_Quit_Report5);
// TODO      itoa(EMM_Pages_Reserved, temp_string, 10);  // "EMM: blocks"
// TODO      strcat(string, temp_string);
// TODO      strcat(string, cnst_Quit_Report6);
// TODO  
// TODO      Exit_With_Message(string);
// TODO  }


// WZD s05p02
// drake178: ¿ ?
/*
shuts down the mouse, sound, and EMM systems, returns
the VGA to text mode, and quits the game with the
passed message - using a direct DOS interrupt rather
than the standard library exit functions
*/
/*

*/
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
// GAME_EXE_Swap()

// WZD s05p05
// Update_MemFreeWorst_KB()

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
    printf("%s\n", string);
    getch();
    exit(EXIT_FAILURE);

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
void DBG_Open_ERROR_LOG__STUB(void)
{

}

// WZD s06p06
// drake178: ¿ ?
// AKA  s06p06_Empty_pFxn()
// MoO1 ORION seg006 STARMAP seg016
void DBG_Close_ERROR_LOG__STUB(void)
{

}
// seg006:007A DBG_Open_ERROR_LOG proc far             ; CODE XREF: init_fonts_sound_mouse_etc+7P
// seg006:007A                                         ; seg014:00AFP
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
// seg006:00AA DBG_Close_ERROR_LOG proc far            ; CODE XREF: Exit_With_Message+17P
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
// seg006:00D7 loc_4687:                               ; CODE XREF: DBG_Close_ERROR_LOG+10j
// seg006:00D7                 pop     di
// seg006:00D8                 pop     si
// seg006:00D9                 pop     ds
// seg006:00DA                 pop     es
// seg006:00DB                 retf
// seg006:00DB DBG_Close_ERROR_LOG endp
// seg006:00DB
// seg006:00DB seg006          ends
