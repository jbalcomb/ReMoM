# ST Cursor



20220805:
What am I still missing?
1st calls to Restore, Store, Draw
TST code for individual draw functions



20220811:
    ScreenPage Address
    Sets DS - Save; Sets ES - Draw, Restore
    ? Screen Page Index ? "active"/"visible"  
        RSP:
            mov  ax, [g_RSP_Idx]
        DSP:
            mov  ax, 1
            sub  ax, [g_RSP_Idx]
        RSP & DSP:
            mov  ah, al
            xor  al, al
            shl  ax, 1
            shl  ax, 1
            add  ax, e_GRAPHICS_MODE_VIDEO_RAM
        Draw RSP & DSP
            mov  dx, cx  ; Y_Pos
            shl  dx, 1
            shl  dx, 1
            add  dx, cx
            ...
            add  ax, dx
        So, Draw includes the Offset to Scan-Line, Save and Restore do not

##### CRL_Draw_RSP  
```
mov  ax, [g_RSP_Idx]
mov  ah, al
xor  al, al
shl  ax, 1
shl  ax, 1
add  ax, dx
add  ax, e_GRAPHICS_MODE_VIDEO_RAM
mov  es, ax
```
##### CRL_Draw_DSP  
```
mov  ax, 1
sub  ax, [g_RSP_Idx]
mov  ah, al
xor  al, al
shl  ax, 1
shl  ax, 1
add  ax, e_GRAPHICS_MODE_VIDEO_RAM
add  ax, dx
mov  es, ax
```
##### CRL_Save_RSP  
```
mov  ax, [g_RSP_Idx]
mov  ah, al
sub  al, al
shl  ax, 1
shl  ax, 1
add  ax, e_GRAPHICS_MODE_VIDEO_RAM
mov  ds, ax
```
##### CRL_Save_DSP  
```
mov  ax, 1
sub  ax, [g_RSP_Idx]
mov  ah, al
xor  al, al
shl  ax, 1
shl  ax, 1
add  ax, e_GRAPHICS_MODE_VIDEO_RAM
mov  ds, ax
```
##### CR_Restore_RSP  
```
mov  ax, [g_RSP_Idx]
mov  ah, al
sub  al, al
shl  ax, 1
shl  ax, 1
add  ax, e_GRAPHICS_MODE_VIDEO_RAM
mov  es, ax
```
##### CR_Restore_RSP  
```
mov  ax, 1
sub  ax, [g_RSP_Idx]
mov  ah, al
xor  al, al
shl  ax, 1
shl  ax, 1
add  ax, e_GRAPHICS_MODE_VIDEO_RAM
mov  es, ax
```

    Loops: Planes, Width, Height
        planes = 0; while(planes < 4) { ...; planes++; ... }
            itr_width = 0; while(itr_width < Draw_Width) { ..., itr_width++; ... }
                itr_height = 0; while(itr_height != 0) { ...; itr_height--; }


```c
int planes;
int itr_width;
int itr_height;

planes = 0;
while (planes < 4)
{
    itr_width = 0;

    while(itr_width < Draw_Width)
    {
        itr_height = 0;
        while(itr_height != 0)
        {
            CR_Save_RSP[Dst_Ofst] = pSrc;
            Dst_Ofst += 2;
            pSrc += 78;
            itr_height--;
        }
        itr_width++;
    }

    planes++;
    Src_Ofst = ScreenPage_Ofst;
}
```

### Pre/Post Head-Body-Tail
Src, Dst
CR_Save_RSP[ ScreenPage_Ofst, Draw_Width, Draw_Height ]
outportb(GC_INDEX, GC_READMAP);

Calcs Src_Sgmt, Sets _DS
Calcs Row_Offset
Calcs Draw_Width, using Row_Offset  >--------------------------|
Calcs Column_Offset                                            |
Calcs ScreenPage_Offset (Row_Offset + Column_Offset)  >---|    |
Sets CR_BG->ScreenPage_Offset  <--------------------------|    |  
                                                          |    |
Sets CR_BG->Draw_Width  <-------------------------------- | ---|  
Assigns Draw_Width  <------------------------------------ | ---|  
                                                          |
Calcs Draw_Height  >---------|                            |
Sets CR_BG->Draw_Height  <---|                            |
Assigns Draw_Height  <-------|                            |
                                                          |
Assigns ScreenPage_Offset  <------------------------------|


### Loops: Planes/Masks, Width, Height

16x16 pixels
16x16 bytes
plane loop on four planes
width loop by two bytes per plane
height loop on row count, column-wise
width of 16 pixels is 4 pixels per 4 planes
so, per plane, 2 reads of 2 bytes

#### Loop Planes/Masks
    Plane: 4 iterations - {0,1,2,3}
        plane = 0; while(plane != 4) { ...; plane++; ... }
    Mask: 4 iterations - {1,2,4,8}/{0001,0010,0100,1000}
        mask =    1; while(mask !=    16) { ...; mask  *= 2; ... }
        mask = 0001; while(mask != 10000) { ...; mask <<= 1; ... }

#### Loop Width
itr_width = 0; while(itr_width < Draw_Width) { ..., itr_width++; ... }

#### Loop Height
    reads/writes words/2 bytes  (LODSW/STOSW)
    itr_height = 0; while(itr_height != 0) { ...; itr_height--; }
    two bytes, at a specific address in VRAM, on a given plane
    VRAM[offset][plane] & VRAM[offset+1][plane]
    | 0 | 1 | 2 | 3 | 0 | 1 | 2 | 3 |
      X               X
this loops 16 times, for the cursor height...

inside a loop of 8 times, for the cursor width in words
inside a loop of 4 times, for the planes

| 0 | 1 | 2 | 3 | 0 | 1 | 2 | 3 |
  X               X














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
Should I not simply trace through the debugger until the first time g_MD_CursorDraw is 1?
Or, can I just add debug code that has flags for when it would all come together?
    What would that look like?
        Mouse Enabled, Cursor Draw Enabled, call to Draw Cursor?
        Y/N/M MD INT Handler is installed? is MvOnly or MVBtns?

*** g_MD_CDraw only controls whether the MD INT Handler ***
```c
if (g_MD_CDraw != 0)
{
    if (g_GUI_WindowCount >= 2)
    {
        GUI_FindWindow(X_Pos, Y_Pos);
    }
    CRL_Restore_RSP(X_Pos, Y_Pos);
    CRL_Save_RSP(X_Pos, Y_Pos);
    CRH_Draw_RSP(X_Pos, Y_Pos);
}
```



?
CRH/CRL Cursor Draw only checks the Cursor Type, not g_MD_CursorDraw
~= if (g_MD_CursorDraw == TRUE && g_CRSR_Curr != NONE) { do actaully draw cursor }
?
And... What about g_CRSR_HaveSave?
CR Restore does not check g_CRSR_HaveSave
CR Save check g_CRSR_Curr and g_CR_HaveSave
So, all this does is indicate if we have ever saved the cursor area background?
But, ... it's not set to TRUE if the Cursor Type is not NONE?!? So, not quite ...
...
1oom calls it cursor_i0_bg_stored
Whats the business with cursor_i0 and cursor_i1?
How/Is it elated to:
```c
typedef struct ui_cursor_area_s {
    uint8_t cursor_i;
    uint8_t mouseoff;
    uint16_t x0;
    uint16_t y0;
    uint16_t x1;    /* inclusive */
    uint16_t y1;    /* inclusive */
} ui_cursor_area_t;
ui_cursor_area_t ui_cursor_area_all_i0;
ui_cursor_area_t ui_cursor_area_all_i1;
```
```c
ui_cursor_area_t ui_cursor_area_all_i0 = { 0, 0, 0, 0, 319, 199 };
ui_cursor_area_t ui_cursor_area_all_i1 = { 1, 0, 0, 0, 319, 199 };
```
```c
void ui_cursor_init(int scale)
{
    ui_cursor_init_do(scale, &ui_cursor_area_all_i0);
    ui_cursor_init_do(scale, &ui_cursor_area_all_i1);
``
?


Hrrmmm... the MD_INT_Handler is GUI_Window-aware...





## Init / Start-Up

Data Segment
dseg:A7F2 00 00                   gsa_gsa_PaletteLbxEntry dw 0            ; as returned by LBXE_LoadReplace when loading a palette through VGA_LoadPalette
dseg:A7F4 00 00                   gsa_PaletteLbxEntry dw 0                ; 15Ch LBX_Alloc_Space paragraphs
...
dseg:A81A 00 00                   gsa_Cursor_Array dw 0                   ; 500h into the palette entry  ; 16 * (16 * 16) cursor images, 100h bytes each

unsigned int gsa_gsa_PaletteLbxEntry;     // dseg:A7F2
unsigned int gsa_PaletteLbxEntry;         // dseg:A7F4  ; alloc in VGA_DAC_Init
unsigned int gsa_Cursor_Array;            // dseg:A81A  ; 500h into the palette entry

s14p03    void VGA_DAC_Init(char *PaletteLbxFileName)
gsa_PaletteLbxEntry = SA_Allocate_Space(348);      // 348 paragraphs = 386 * 16 bytes = 5,568 bytes

s20p01    void VGA_LoadPalette(int Palette_Index, int First_Color, int Last_Color)
gsa_gsa_PaletteLbxEntry = LBXE_LoadReplace(g_PaletteLbxFileName, Palette_Index+2, gsa_PaletteLbxEntry);  |-> LBX_EntryLoader(g_PaletteLbxFileName, Palette_Index+2, gsa_PaletteLbxEntry, 1, 0);

### Story-Time:
```
Pre-defined / Known-In-Advanced - The structure, size, and contents of the LBX records in FONTS.LBX
Allocated, on launch, in VGA_DAC_Init(); Loaded, in main(), in VGA_LoadPalette();
Using LBXE_LoadReplace() and passing the preallocated SAMB_head means it knows to only every have one record loaded at a time.
VGA_LoadPalette() using the file name set by VGA_DAC_Init(), which is passed from Hardware_Init()

```
?
_s34p65c.c IN_Init
GUI_SetWindows(1, g_GUI_VirtualScreen);  // _s33p01  ST_GUI.H
?





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
    |-> GUI_CursorCopy_Bkup2Main  
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
