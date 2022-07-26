¿
∴
∵
≜

Unicode Character “¿” (U+00BF)
Name:	Inverted Question Mark
PostScript Name:	questiondown
HTML Entity:	&#191; &#xBF; &iquest;
UTF-8 Encoding:	0xC2 0xBF

U+2234 ∴
THEREFORE (HTML &#8756; · &there4;, &Therefore;, &therefore;
Alt+8756
 block: Mathematical Operators.
 Category: Math Symbol (Sm) 

Unicode Character “∵” (U+2235)
Name:	Because
Category:	Math Symbol (Sm)
block: Mathematical Operators

If A and B are equal by definition (i.e., A is defined as B), then this is written symbolically as A=B, A:=B, or sometimes A≜B.


IBM PC Drawing Characters

https://jrgraphix.net/r/Unicode/2500-257F

═       2550
║       2551
╔       2554
╗       2557
╚       255A
╝       255D
╠       2560
╣       2563
╦       2566
╩       2569
╬       256C

╒       2552
╓       2553
╕       2555
╖       2556
╘       2558
╙       2559
╛       255B
╜       255C
╞       255E
╟       255F
╡       2561
╢       2562
╤       2564
╥       2565
╧       2567
╨       2568
╪       256A
╫       256B




STU
MoM
Rasm-MoM




// TODO(JimBalcomb): move these rasm deduction notes elsewhere - consolidate into KB
PoC / Test / Research:

C2A:

    Pass char* by Address-Of
        EMS_GetHandleName(char *EmsHndlNm, unsigned int EmsHndlNbr)
        e.g., EMS_GetHandleName(&varTmpStr100, itr);


IDA Pro 5.5 decompilation
    Target: "push/pop bp", "push/pop es, si, ds, di"
    In-Line  Assembly, No Variables, No Arguments, All asm{}, Video BIOS Interrupt
        VGA_SetTextMode
            shows up in IDA55 with pre/postamble push bp; mov bp,sp; ...; pop bp
IDA Pro 5.5 decompilation of BCpp30 PC/DOS Interrupt Calling Functions
    Target: "push/pop bp", "push/pop es, si, ds, di"
    C: asm{}, geninterrupt(), int86(), int86x(), intdos(), intdosx(), intr()
    asm{}:
        ?
    geninterrupt()
        _s12p02c.c  unsigned int EMS_GetHandleCount(void) using geninterrupt; assigns register to variable and returns variable
            shows up in IDA55 with pre/postamble push bp; mov bp,sp; ...; pop bp
            if (_AH) shows up as or ah,ah; jz
            ems_handle_count = _BX; return(ems_handle_count); shows up as mov dx,bx; mov ax,dx, jmp short $+2; ret

    :int86()
        ?
    int86x():
        _s12p03c.c  EMS_GetFreePageCnt(void)    using int86x(), with the return via if else
            shows up as push 42h; push 67h; call lib_func()
        _s12p04c.c  EMS_GetHandlePageCount      using int86x(), with the return via ternary operator
            shows up as push 4Ch; push 67h; call lib_func()
    intdos():
        ?
    intdosx():
        ?
    intr()
        ?


NOTE: a function that has **any** in-line assembly code (`asm ...`) should be named "[...]a.c"



##### seg001    MGC_DEF.H
_s01p01         MGC_MAIN.C                  int MGC_Main(void)                  main                int main(argc, argv)
_s01p03c.c      GAME_MainMenu               void GAME_MainMenu(void)
_s01p04c.c      GAME_LoadMainImages         void GAME_LoadMainImages(void)
_s01p05c.c      SCREEN_Menu                 void SCREEN_Menu(void)              GAME_MainEventLoop
_s01p06c.c      SCREEN_Menu_Draw            void SCREEN_MainMenu_Draw(void)     GAME_DrawMainScreen
_s01p07c.c      GAME_Load_TERRSTAT_0        void GAME_LoadTerrstat(void)
_s01p08c.c      GAME_Load_SPELLDAT_0        void GAME_LoadSpellData(void)

##### seg002    MOM_DEF.H
_s02p01.asm     CLK_SaveCounter             void CLK_SaveCounter(void)
_s02p03.asm     CLK_Wait
_s02p05.asm     VGA_WaitSync

##### seg003    MOM_DEF.H
_s03p05         DISK_GetFileSize            long DISK_GetFileSize(char *fname)
C_GetFileSize(const char *filename)

##### seg004    ST_DEF.H
[]_s04p01c.c    DISK_FileFind               int __cdecl __far DISK_FileFind(char *FileName_Ptr, char *Read_Buffer)

##### seg005    MOM_DEF.H
_s05p01c.c      Quit_MemBug                                                                 GAME_QuitWithReport
_s05p02c.c      Quit                        void Quit(char *quitmsg);                       GAME_QuitProgram
_s05p05c.c      RAM_Update_WorstFreeBlock_KB

##### seg006    MOM_DEF.H   (4-6 ASM)
_s06p01c.c      VGA_SetTextMode             void VGA_SetTextMode(void);
_s06p02c.c      QuitToDOS                   void QuitToDOS(char *quitmsg);                  DOS_QuitWithMsg
_s06p04c.c      dos_memfree_kb              unsigned int dos_memfree_pages(void);           RAM_GetFreeBlockSize_KB

##### seg007    ST_LBX.H    (1-4 UU_DOS_ ASM)
_s07p04c.c      SA_Alloc_Validate                     LBX_Alloc_Validate
_s07p05.asm     farpeekb                    char farpeekb(unsigned int sgmt, unsigned int ofst);
_s07p06c.c      farpeekw                    int farpokew(unsigned int sgmt, unsigned int ofst);
_s07p07c.c      farpeekdw                   long farpokedw(unsigned int sgmt, unsigned int ofst);
_s07p08.asm     farpokeb                    void farpokeb(unsigned int sgmt, unsigned int ofst, char value);
_s07p09.asm     farpokew                    void farpokew(unsigned int sgmt, unsigned int ofst, int value);

##### seg008    ST_SA.H
_s08p07c.c      SA_Allocate_Space           sgmt_addr Allocate_Space(unsigned int size)
_s08p08c.c      SA_Allocate_MemBlk          sgmt_addr SA_Allocate_MemBlk(unsigned int size); ***NOTE: merged _s08p08 & _s08p09  AllocMem_LBX_Header & AllocMem_EntrySize***  
_s08p12c.c      SA_Alloc_First
_s08p14c.c      SA_Alloc_Next
_s08p15c.c      SA_GetFree                  unsigned int SA_GetFree(unsigned int Alloc_Seg) LBX_Alloc_GetFree
_s08p16.asm     SA_MK_FP0                      void far * MK_FAR(sgmt_addr Segment_Address) MK_FAR
_s08p19.asm     SA_Alloc_Error                              LBX_Alloc_Error

##### seg009    ST_LBX.H    ALL ASM
_s09p01     lbx_open                    int  lbx_open(char *fname);
_s09p02     lbx_close                   int  lbx_close(int fhandle);
_s09p03     lbx_seek                    long lbx_seek(long foffset, int fhandle);
_s09p05a.c  lbx_read_sgmt               int  lbx_read_sgmt(unsigned int dst_sgmt, int nbytes, int fhandle);
_s09p06a.c  lbx_read_ofst               int  lbx_read_ofst(unsigned int dst_ofst, int nbytes, int fhandle);
_s09p07     strcpyfar                   void strcpyfar(unsigned int dstoff, unsigned int dstseg, unsigned int srcoff, unsigned int srcseg);

##### seg010    ST_LBX.H
_s10p01         LBXE_LoadSingle             sgmt_addr LBXE_LoadSingle(char *LbxFileName, int LbxEntryIndex);
_s10p02         LBXE_LoadReplace            sgmt_addr LBXE_LoadReplace(char *LbxFileName, int LbxEntryIndex, unsigned int SAMB);
_s10p03         LBXE_LoadAppend             sgmt_addr LBXE_LoadAppend(char *LbxFileName, int LbxEntryIndex, unsigned int SAMB);
_s10p04c.c      LBXR_LoadSingle

_s10p10c.c      LBX_Load_Entry              sgmt_addr LBX_EntryLoader()       LBX_EntryLoader
_s10p11c.c      LBX_Load_Load_Record              LBX_RecordLoader
[]_s10p12c.c      LBXR_DirectLoader
[]_s10p13c.c      LBX_GetEntryData

_s10p15c.c      LBX_Error
_s10p16.asm     ExtractFileBase             ExtractFileBase(char * FileName)  STR_StripFileExt
_s10p17c.c      RAM_SetMinKB                RAM_SetMinKB(int RAM_MinKbytes)

##### seg012    ST_EMM.H
_s12p01.asm     EMM_Init                    int EMM_Init(void);
_s12p02c.c      EMM_GetActiveHandleCount    unsigned int EMM_GetActiveHandleCount(void);
_s12p03c.c      EMM_GetFreePageCount        unsigned int EMM_GetFreePageCount(void);
_s12p04c.c      EMM_GetHandlePageCount
_s12p05.asm     EMM_MakeNamedHandle
_s12p06.asm     EMM_GetHandleName           unsigned int EMM_GetHandleName(char *EmmHndlNm, unsigned int EmmHndlNbr);
_s12p07c.c      EMM_ReleaseHandle
_s12p08c.c      EMM_MapnRead                unsigned int EMM_MapnRead(unsigned int Dst_Ofst, unsigned int Dst_Sgmt, unsigned int Src_Ofst, unsigned int Src_Sgmt, int nbytes, int EMM_Handle)
_s12p10         EMM_GetPageFrame            unsigned int EMM_GetPageFrame(void)
_s12p11c.c      EMM_Map4                    void EMM_Map4(int EmmHandle, int EmmLogicalPage)                EMM_MapFourPages
_s12p12c.c      EMM_MapMulti4

##### seg013    ST_EMM.H
_s13p01c.c      EMM_Startup
_s13p02         UU                          UU_EMM_Load_LBX_0
_s13p03c.c      EMM_Load_LBX_File_1              void EMM_Load_LBX_1(char *argLbxFileName)       EMM_LoadResLBX
_s13p04c.c      EMM_Load_LBX_File                void EMM_Load_LBX(char *argLbxFileName, int argReserved)
...
_s13p11c.c      EMM_LBX_Load_Entry
_s13p12c.c      EMM_LBX_FLIC_Header         unsigned int EMM_LBX_FLIC_Header(int EmmHndl, char *EmmHndlNm, int LbxEntry, unsigned int SAMB_head, int LoadType)   EMM_LBX_HdrOnly
_s13p13c.c      EMM_LBX_Load_Record
[]_S13p14c.c    EMM_LBX_DirectLoad

_s13p15c.c      EMM_CheckHandleOpen         unsigned int EMM_CheckHandleOpen(char *EmmHndlNm)
_s13p16c.c      EMM_Shutdown                        EMM_ReleaseAll
...
_s13p20c.c      EMM_GetHandle       unsigned int EMM_GetHandle(unsigned int argEmmLogicalPageCount, char *argEmmHandleName, int argEmmRsvdFlag)  EMM_GetOrCreateHandle
...
_s13p25c.c      EMM_Map4_EMMDATAH
_s13p26c.c      EMM_EMMDATAH_AllocFirst
_s13p27c.c      EMM_EMMDATAH_AllocNext
...
_s13p31c.c      EMM_SetMinKB                void EMM_SetMinKB(int EMM_Min_KB);
_s13p32c.c      EMM_BuildEmmErrStr          void EMM_BuildEmmErrStr(char *strbuf)

##### seg014    MOM_DEF.H
_s14p01c.c      Hardware_Init               int Hardware_Init(...);
_s14p03c.c      VGA_DAC_Init                void VGA_DAC_Init(char *PaletteLbxFileName);
_s14p04c.c      VGA_SetDrawWindow
_s14p05c.c      VGA_ResetDrawWindow

##### seg015    ST_VGA.H    ALL ASM
_s15p01.asm     VGA_SetModeY                void VGA_SetModeY(void);

##### seg016    ST_VGA.H
_s16p01.asm     VGA_DrawFilledRect          void VGA_DrawFilledRect(int x1, int y1, int x2, int y2, char Color)

##### seg017    ST_TXT.H
_s17p06c.c      VGA_SetOutlineColor         void VGA_SetOutlineColor(int Color_Index)
_s17p24c.c      VGA_DrawCentered
_s17p35c.c      VGA_DrawTextLine
_s17p36         VGA_DrawStyledString
_s17p37         VGA_DrawString

[]_s17p39         VGA_UseFontColor1
[]_s17p40         VGA_FontColor2Toggle
[]_s17p41         VGA_FontColor3Toggle
[]_s17p42         VGA_FontColorSelect

##### seg018    ST_TXT.H    ALL ASM
_s18p01.asm     VGA_SetFont                 void VGA_SetFont(int Font_Index, int Color_1, int Color_2, int Color_3)

[] _s18p02.asm    VGA_DrawChar
[] _s18p03.asm    VGA_DrawChar_NoAA

_s18p07.asm     VGA_GetStringWidth

##### seg019    ST_VGA.H
[?]_s19p05         VGA_GetTextHeight
[?]_s19p06         VGA_GetTextWidth
[]_s19p11         VGA_GetFontHeight
_s19p14         VGA_TextDraw_Init           void VGA_TextDraw_Init(void)


##### seg020    ST_VGA.H
_s20p01c.c      VGA_LoadPalette             void VGA_LoadPalette(int Palette_Index, int First_Color, int Last_Color)
_s20p05c.c      VGA_SetDACChanged           void VGA_SetDACChanged(int First_Color, int Last_Color)

##### seg021    ST_VGA.H    ALL ASM
_s21p01c.c      VGA_DAC_Write               void VGA_DAC_Write(void)
_s21p07c.c      FLIC_LoadPalette            void FLIC_LoadPalette(unsigned int FlicHdr_SgmtAddr, int Frame_Index)       VGA_LoadIMGPalette


##### seg022    ST_DEF.H
_s22p07         RNG_TimerSeed               void RNG_TimerSeed(void)
_s22p08         RNG_Random                  

[]_s22p22       ST_MoveData                 int ST_MoveData(unsigned int destoff, unsigned int destseg, unsigned int srcoff, unsigned int srcseg, unsigned int nbytes)


##### seg024    ST_DBG.H
_s24p05c.c      DBG_Disable                 void DBG_Disable(void);
_s24p06c.c      DBG_IsDisabled              int DBG_IsDisabled(void);


##### seg025    ???
[?] FLIC_VGAFILEH_Draw_R


##### seg026    ST_VGA.H    ALL ASM
_s26p01         VGA_SetDirectDraw           void VGA_SetDirectDraw(void);
_s26p02.asm     VGA_SetDrawFrame            void VGA_SetDrawFrame(void);
_s26p03         VGA_IsDefaultDrawScreenPage  int VGA_IsDefaultDrawScreenPage(void)
_s26p04a.c      VGA_PageFlip                void VGA_PageFlip(void)

[]_s26p05 VGA_Copy_RSP_DSP
[]_s26p06 VGA_Copy_RSP_DSP_YM
[]_s26p07 VGA_Copy_RSP_DSP_NM


##### seg027    ST_FLIC.H    ALL ASM
_s27.asm
_s27p01     FLIC_Draw                   VGA_DrawBitmap
_s27p02     FLIC_Draw_R                 VGA_DrawBitmap_R
_s27p03     FLIC_Draw_EMM               VGA_DrawEMSBitmap
_s27p04     FLIC_EMM_MapNextPages       VGA_MapNextEMMPages
_s27p05     FLIC_Draw_EMM_R             VGA_DrawEMSBitmap_R
_s27a.c
_s27p01     FLIC_Draw_A                   VGA_DrawBitmap
_s27p02     FLIC_Draw_R_A                 VGA_DrawBitmap_R
_s27p03     FLIC_Draw_EMM_A               VGA_DrawEMSBitmap
_s27p04     FLIC_EMM_MapNextPages_A       VGA_MapNextEMMPages
_s27p05     FLIC_Draw_EMM_R_A             VGA_DrawEMSBitmap_R

_s27c.c
_s27p01     FLIC_Draw_C                   VGA_DrawBitmap
_s27p02     FLIC_Draw_R_C                 VGA_DrawBitmap_R
_s27p03     FLIC_Draw_EMM_C               VGA_DrawEMSBitmap
_s27p04     FLIC_EMM_MapNextPages_C       VGA_MapNextEMMPages
_s27p05     FLIC_Draw_EMM_R_C             VGA_DrawEMSBitmap_R


##### seg028    ST_FLIC.H
_s28p02         FLIC_Prepare           void FLIC_Prepare(int Width, int Height, unsigned int SAMB_head);       LBX_Image_Prepare

_s28p11a.c      FLIC_Draw_XY            FLIC_Draw_XY(int Left, int Top, unsigned int FlicHdr_SgmtAddr)         VGA_DrawLBXImage

LBX_IMG_DecPrepare
LBX_IMG_LoadEMSFrame
LBX_IMG_LoadFrame
_s22p22c.c      ST_MoveData             int ST_MoveData(unsigned int destoff, unsigned int destseg, unsigned int srcoff, unsigned int srcseg, unsigned int nbytes)  MEM_Copy_Far



[]_s28p13         FLIC_BuildFrame     void FLIC_BuildFrame(unsigned int FlicHdr_SgmtAddr, unsigned int Target_Seg)  LBX_IMG_BuildFrame

_s28p14c.c      FLIC_SetFrame         void FLIC_SetFrame(unsigned int FlicHdr_SgmtAddr, int Frame_Index)      LBX_IMG_SetFrame
_s28p15         FLIC_ResetFrame       void FLIC_ResetFrame(unsigned int FlicHdr_SgmtAddr)                     LBX_IMG_ResetFrame
_s28p16c.c      FLIC_GetCurFrame      void FLIC_GetCurFrame(unsigned int FlicHdr_SgmtAddr)                    LBX_IMG_GetCurFrame


##### seg029    ???
[?] FLIC_DrawPartImage_R


##### seg031
[?] FLIC_LoadEMSFrame        LBX_IMG_LoadEMSFrame
[?] FLIC_DecPrepare      LBX_IMG_DecPrepare


##### seg033    ST_GUI.H
_s33p01         GUI_SetWindows              
_s33p02         GUI_FindWindow
_s33p03         GUI_GetCursorIndex          int GUI_GetCursorIndex(void)
_s33p04c.c      GUI_GetCursorOffset
_s33p05.asm     MD_GetButtonStatus       int MD_GetButtonStatus(void)     MOUSE_ReadStatus
_s33p06         MOUSE_WaitRelease           int MOUSE_WaitRelease(void)
_s33p07         MOUSE_Init                  int MOUSE_Init(void)
[]_s33p09         MOUSE_Reset
[]_s33p10         MOUSE_INTSet_MvOnly
[]_s33p11         MOUSE_INTSet_MvBtns
_s33p12         MOUSE_INT_Handler         void MOUSE_INT_Handler(void)
_s33p13         MOUSE_Disable_CDraw         void MOUSE_Disable_CDraw(void)
_s33p14         MOUSE_CDraw_Restore         void MOUSE_CDraw_Restore(void)
_s33p16         KBD_CheckBuffer             int KBD_CheckBuffer(void)
_s33p17.asm     KD_GetKeyCode               int KD_GetKeyCode(void)        KBD_GetKey
_s33p18c.c      MOUSE_GetX
_s33p19c.c      MOUSE_GetY

[]_s33p20 MOUSE_MoveCursor
[]_s33p21 MOUSE_MoveCursorINT

[]_s33p22 MOUSE_GetClickX
[]_s33p23 MOUSE_GetClickY

[]_s33p24 MOUSE_GetClickedBtns  int MOUSE_GetClickedBtns(void)

_s33p25c.c      MOUSE_GetClickRec1          int MOUSE_GetClickRec1(void)    MOUSE_GetClick
_s33p26c.c      CRP_MOUSE_GetClickRec2      int MOUSE_GetClickRec2(void)    CRP_MOUSE_GetSecClick
[]_s33p29         MOUSE_SaveClick

_s33p30         GUI_SaveCursorArea_RSP      void GUI_SaveCursorArea_RSP(int X_Pos, int Y_Pos)       GUI_SaveCursorArea          
_s33p31         GUI_SaveCursorArea_DSP      void GUI_SaveCursorArea_DSP(int X_Pos, int Y_Pos)       GUI_SaveDrawCrsrArea          
_s33p32         VGA_SaveCursorArea_RSP      void VGA_SaveCursorArea_RSP(int X_Pos, int Y_Pos)       VGA_SaveCursorArea
_s33p33         GUI_CursorCopy_Bkup2Main     void GUI_CursorCopy_Bkup2Main(void)
_s33p34         GUI_RestoreCursorArea_RSP           VGA_RestoreCursrArea
_s33p35         GUI_RestoreCursorArea_DSP           VGA_RestoreDrawCArea
// UU _s33p36                                       UU_VGA_RestoreCursorArea_SP3
_s33p37         GUI_DrawCursor_RSP |-> VGA_DrawCursor_RSP                 GUI_DisplayCursor
_s33p38         GUI_DrawCursor_DSP |-> VGA_DrawCursor_DSP                GUI_DrawCursor
_s33p39         VGA_DrawCursor_RSP                                        VGA_DisplayCursor
_s33p40         VGA_DrawCursor_DSP                                       VGA_DrawCursor


##### seg034    ST_GUI.H
[]_s34p01         GUI_ProcessInput            _s34p01.asm GUI_ProcessInput
_s34p08         GUI_SetEscOverride          void GUI_SetEscOverride(void)

_s34p12 GUI_SetHelp         void GUI_SetHelp(int Entry_Tbl, int Entry_Count)
_s34p13 GUI_ClearHelp       void GUI_ClearHelp(void)
[?] _s34p14 GUI_ContextBasedHelp

[]_s34p22c.c       GUI_NoTriggerKeys           int GUI_NoTriggerKeys(int *Input_Control_Index)

_s34p25 GUI_MouseOverControl  GUI_MouseOnControl
_s34p26 GUI_MousedControl


[?]_s34p30 GUI_ProcessDirKey


_s34p57         GUI_Clear                   void GUI_Clear(void)
_s34p65         GUI_Init                    void GUI_Init(int Input_Type)
_s34p66c.c      GUI_GetInput                int GUI_GetInput(void)
_s34p67         GUI_SetDelay                void GUI_SetDelay(int Input_Delay)

[]_s34p71 GUI_DrawControls
[]_s34p72 GUI_DrawControl

_s34p73 GUI_SetFocus  void GUI_SetFocus(int Ctrl_Index, int X_Pos, int Y_Pos)

_s34p76c.c      GUI_Set_Redraw_Function           GUI_SetRedrawFn
_s34p79c.c      GUI_DisableRedraw           void GUI_DisableRedraw(void)

_s34p81c.c GUI_CallRedrawFn
_s34p82c.c      GUI_Redraw_WaitOne          void GUI_Redraw_WaitOne(void)       GUI_1TickRedraw
_s34p83c.c      GUI_Redraw_WaitTimer        void GUI_NormalRedraw(void)         GUI_NormalRedraw      

_s34p85c.c      GUI_SimplePageFlip

_s34p47         GUI_CreateClickLabel        GUI_CreateClickLabel(int Left, int Top, int Right, int Bottom, char *varKeyCode, char *Hlp)
_s34p49         GUI_CreateHotkey            GUI_CreateHotkey(char *Hotkey)


##### seg035    ST_SND.H
_s36p02         SND_Init

##### seg036    AIL

##### ovr048    ...SAVE_.GAM Load/Save
[?] _o48p01 SAVEGAM_Save
[?] _o48p02 SAVEGAM_Load

##### ovr049    ...Settings - Screen, ScreenDraw, Load, Reset
[?] _o49p01     SCREEN_Settings
[?] _o49p02     GAME_DrawSettingsScr
[?] _o49p03     GAME_LoadSettings
[?] _o49p04     GAME_ResetSettings
[?] _o49p05     HLP_Load_Settings

##### ovr057    MOM_DEF.H
_o57p01         LBX_Tables_Init             void LBX_Tables_Init(int gfx_buff_nparas)

##### ovr058    ...Intro Animation...

##### ovr060    MGC_DEF.H
[]_o60p01         GAME_DrawCredits
[]_o60p02         GAME_PrepareCredits

##### ovr061    ...Hall Of Fame...
