# ST Cursor

_s33p30         CRL_Save_RSP                                                    GUI_SaveCursorArea_RSP
_s33p31         CRL_Save_DSP                                                    GUI_SaveCursorArea_DSP
_s33p32         IN_CRL_Save_RSP                                                 VGA_SaveCursorArea_RSP
_s33p33         CRL_Copy_DSP2RSP                                                GUI_CursorCopy_Bkup2Main
_s33p34         CRL_Restore_RSP                                                 GUI_RestoreCursorArea_RSP
_s33p35         CRL_Restore_DSP                                                 GUI_RestoreCursorArea_DSP
// UU _s33p36      UU_CRL_Restore_SP3                                           UU_VGA_RestoreCursorArea_SP3
_s33p37         CRH_Draw_RSP |-> CRL_Draw_RSP                                   GUI_DrawCursor_RSP
_s33p38         CRH_Draw_DSP |-> CRL_Draw_DSP                                   GUI_DrawCursor_DSP
_s33p39         CRL_Draw_RSP                                                    VGA_DrawCursor_RSP
_s33p40         CRL_Draw_DSP                                                    VGA_DrawCursor_DSP

FAQ-Me:
Q: The outer-most boundary/context here is **Per Screen**?
A:
    ∵ GUI_SetWindows(1,*g_GUI_VirtualScreen)   g_GUI_VirtualScreen = {Crsr_None, 0, 0, 0, 319, 199}

...on Start-Up...MD_INT_Handler installed, set to 'cursor position change', which turns on g_MD_Draw, but g_MD_Draw is set back to off before interrupts are re-enabled
...? odd that it sets g_MD_Draw directly, rather than using MD_CDraw_Disable() ? which would have set g_MD_CDraw_Save to 1, because the MD_INT_Handler directly sets g_MD_Draw to 1 ?
...so, movement is tracked, but MD_INT_Handler doesn't start drawing the cursor until ? something enables g_MD_CDraw ?
... ? that something would be MD_INT_SetMvOnly() or MD_INT_SetMvBtns() ?
... ? which are only called by GUI_ProcessInput() and G_GUI_PressAnyKey() ?
      e.g.,
          ProgramPath:
          KBD_CheckBuffer == 0
          MDI_Mv()
          @@DoTheMouse:                        ; sets the mouse interrupt handler to be called on
          call MD_INT_SetMvOnly                ; movement only, enables cursor drawing, and returns
          call MOUSE_GetButtonStatus           ; returns button_status

          ProgramPath:
          KBD_CheckBuffer == 0
          MDI_Mv()
          MD_GetButtonStatus() == 0
          MOUSE_GetClickRec1() == 0
          @@NayKdInptCd_MdSetMvBtns:           ; sets the mouse interrupt handler to be called on
          call MD_INT_SetMvBtns                ; both movement and button presses, enables cursor
Anywhoodle, ...
Should I not simply trace through the debugger until the first time g_MD_CDraw is 1?
Or, can I just add debug code that has flags for when it would all come together?
    What would that look like?
        Mouse Enabled, Cursor Draw Enabled, call to Draw Cursor?
        Y/N/M MD INT Handler is installed? is MvOnly or MVBtns?





Hrrmmm... the MD_INT_Handler is GUI_Window-aware...



Only Calls / Short-Chains:
    GUI_Init() | -> VGA_SaveCursorArea_RSP()
    GUI_SimplePageFlip() |-> GUI_CursoCopy_Bkup2Main()
    GUI_DrawCursor_RSP() |-> VGA_DrawCursor_RSP()
    GUI_DrawCursor_DSP() |-> VGA_DrawCursor_DSP()



First Calls:
    main() |-> Hardware_Init() |-> GUI_Init() |-> VGA_SaveCursorArea_RSP(158,100)
    main() |-> GAME_MainMenu() |-> SCREEN_Menu() |-> GUI_SaveCursorArea_RSP(MOUSE_GetX,MOUSE_GetY)

    ? GUI_GetInput() ?
        ... |-> GUI_DrawCursor_RSP()
        ... |-> GUI_RestoreCursorArea_RSP()
        ... |-> MD_CDraw_Disable()
        ... |-> MD_CDraw_Restore()

main() |-> GAME_MainMenu() |-> SCREEN_Menu() |-> GUI_SimplePageFlip()
    |-> MD_CDraw_Disable         
    |-> MOUSE_GetX               
    |-> MOUSE_GetY               
    |-> GUI_DrawControls
    |-> GUI_FindWindow           
    |-> GUI_SaveCursorArea_DSP   
    |-> GUI_DrawCursor_DSP       
    |-> VGA_PageFlip             
    |-> GUI_RestoreCursorArea_DSP
    |-> GUI_CursoCopy_Bkup2Main  
    |-> MD_CDraw_Restore         



GUI_DrawCursor_DSP
    GUI_SimplePageFlip

GUI_DrawCursor_RSP
        Up   P VGA_Copy_RSP_DSP_YM+99           call GUI_DrawCursor_RSP              ; overlays the mouse cursor image at the specified
        Up   p MOUSE_INT_Handler+94             call near ptr GUI_DrawCursor_RSP     ; overlays the mouse cursor image at the specified
        Down p UU_GUI_ProcBufferClick:loc_24D9D call near ptr GUI_DrawCursor_RSP     ; overlays the mouse cursor image at the specified
        Down P GUI_ProcessInput+9E              call GUI_DrawCursor_RSP              ; overlays the mouse cursor image at the specified
        Down P GUI_ProcessInput+C09             call GUI_DrawCursor_RSP              ; overlays the mouse cursor image at the specified
        Down P CRP_GUI_KeyInputOnly+30          call GUI_DrawCursor_RSP              ; overlays the mouse cursor image at the specified
        Down P CRP_GUI_KeyInputOnly+B5          call GUI_DrawCursor_RSP              ; overlays the mouse cursor image at the specified
        Down P UI_ProcessInput_KD+2CE           call GUI_DrawCursor_RSP              ; overlays the mouse cursor image at the specified
        Down P GUI_MouseEmuMove+10B             call GUI_DrawCursor_RSP              ; overlays the mouse cursor image at the specified
        Down P G_GUI_PressAnyKey+9C             call GUI_DrawCursor_RSP              ; overlays the mouse cursor image at the specified
        Down P G_GUI_PressAnyKey+11D            call GUI_DrawCursor_RSP              ; overlays the mouse cursor image at the specified
        Down P GUI_ProcessDirKey+293            call GUI_DrawCursor_RSP              ; overlays the mouse cursor image at the specified
        Down P GUI_MoveDlgHighlight+3DA         call GUI_DrawCursor_RSP              ; overlays the mouse cursor image at the specified
        Down P GUI_SetFocus+93                  call GUI_DrawCursor_RSP              ; overlays the mouse cursor image at the specified
        Down P GUI_SetFocus+179                 call GUI_DrawCursor_RSP              ; overlays the mouse cursor image at the specified
        Down P GUI_SetFocus+1C5                 call GUI_DrawCursor_RSP              ; overlays the mouse cursor image at the specified

GUI_CursoCopy_Bkup2Main
XREFs:
    only called by GUI_SimplePageFlip()



## Cursor Index - Current & Previous

    GUI_FindWindow() sets Previous to Current and sets new Current; also sets g_GUI_CursorOffset
    GUI_SetWindows() sets Current to the top-level *Window*


g_GUI_CurrentCursor
    GUI_SetWindows+3E        mov  [g_GUI_CurrentCursor], ax       
    GUI_FindWindow+C         mov  ax, [g_GUI_CurrentCursor]       
    GUI_FindWindow+53        mov  [g_GUI_CurrentCursor], ax       
    GUI_GetCursorIndex+6     mov  ax, [g_GUI_CurrentCursor]       
    GUI_SaveCursorArea_RSP+F cmp  [g_GUI_CurrentCursor], Crsr_None
    GUI_SaveCursorArea_DSP+F cmp  [g_GUI_CurrentCursor], 0        
    GUI_DrawCursor_RSP+C     mov  dx, [g_GUI_CurrentCursor]       
    GUI_DrawCursor_DSP+C     mov  dx, [g_GUI_CurrentCursor]       

g_GUI_PrevCursor
    GUI_FindWindow+F               mov  [g_GUI_PrevCursor], ax       
    GUI_SaveCursorArea_DSP+16      cmp  [g_GUI_PrevCursor], 0        
    GUI_RestoreCursorArea_RSP+F    cmp  [g_GUI_PrevCursor], 0        
    GUI_RestoreCursorArea_DSP+F    cmp  [g_GUI_PrevCursor], Crsr_None
    UU_VGA_RestoreCursorArea_SP3+F cmp  [g_GUI_PrevCursor], 0        





Cursor_Save_Main
Cursor_Save_Bkup
1200 bytes, 600 words
? RSP & DSP ?
dseg:43B2    Cursor_Save_Main    db 4B0h dup(0)
dseg:4862    Cursor_Save_Bkup    db 4B0h dup(0)

XREFs:
Cursor_Save_Bkup
    GUI_SaveCursorArea_DSP
    GUI_CursoCopy_Bkup2Main
Cursor_Save_Main
    GUI_SaveCursorArea_RSP
    VGA_SaveCursorArea_RSP
    GUI_CursoCopy_Bkup2Main
    GUI_RestoreCursorArea_RSP
    GUI_RestoreCursorArea_DSP
    UU_VGA_RestoreCursorArea_SP3


Cursor_Save_Main
GUI_SaveCursorArea_RSP+60                mov  [word ptr es:Cursor_Save_Main+di], si
GUI_SaveCursorArea_RSP+68                mov  es:Cursor_Save_Main[di], bx     ; Width in Addresses
GUI_SaveCursorArea_RSP:loc_246CA         mov  es:Cursor_Save_Main[di], bx
GUI_SaveCursorArea_RSP+A2                mov  es:Cursor_Save_Main[di], ax
VGA_SaveCursorArea_RSP+44                mov  [word ptr es:Cursor_Save_Main+di], si
VGA_SaveCursorArea_RSP+4C                mov  es:Cursor_Save_Main[di], bx     ; Width_Bytes
VGA_SaveCursorArea_RSP:loc_2485F         mov  es:Cursor_Save_Main[di], bx     ; Height_Lines
VGA_SaveCursorArea_RSP+86                mov  word ptr es:Cursor_Save_Main.Saved_Image[di-6], ax
GUI_CursoCopy_Bkup2Main+B                mov  di, offset Cursor_Save_Main
GUI_RestoreCursorArea_RSP+20             mov  di, [word ptr Cursor_Save_Main+si]
GUI_RestoreCursorArea_RSP+2A             mov  ax, word ptr Cursor_Save_Main[si]
GUI_RestoreCursorArea_RSP+34             mov  ax, word ptr Cursor_Save_Main[si]
GUI_RestoreCursorArea_RSP:@@LoopWords    mov  ax, word ptr Cursor_Save_Main[si]

GUI_RestoreCursorArea_DSP+34             mov  di, [word ptr Cursor_Save_Main+si]
GUI_RestoreCursorArea_DSP+3E             mov  ax, Cursor_Save_Main[si]
GUI_RestoreCursorArea_DSP+48             mov  ax, Cursor_Save_Main[si]
GUI_RestoreCursorArea_DSP:@@LoopWords    mov  ax, Cursor_Save_Main[si]

UU_VGA_RestoreCursorArea_SP3+20          mov  di, [word ptr Cursor_Save_Main+si]
UU_VGA_RestoreCursorArea_SP3+2A          mov  ax, Cursor_Save_Main[si]
UU_VGA_RestoreCursorArea_SP3+34          mov  ax, Cursor_Save_Main[si]
UU_VGA_RestoreCursorArea_SP3:@@LoopWords mov  ax, Cursor_Save_Main[si]

Cursor_Save_Bkup
GUI_SaveCursorArea_DSP+6B        mov  [word ptr es:Cursor_Save_Bkup+di], si
GUI_SaveCursorArea_DSP+73        mov  es:Cursor_Save_Bkup[di], bx
GUI_SaveCursorArea_DSP:loc_247A8 mov  es:Cursor_Save_Bkup[di], bx
GUI_SaveCursorArea_DSP+AD        mov  es:Cursor_Save_Bkup[di], ax

GUI_CursoCopy_Bkup2Main+E        mov  si, offset Cursor_Save_Bkup
