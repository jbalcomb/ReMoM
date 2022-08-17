
#include "MGC_DEF.H"

#include "ST_VGA.H"
#pragma warn -aus
#pragma warn -par

//#include <stdlib.h>
#include <ALLOC.H>  /* coreleft(), farcoreleft(), malloc(), farmalloc(), free(), farfree() */
#include <DOS.H>

//#include "MGC_HEAD.H"
#include "MGC_DEF.H"
//#include "MOM_HEAD.H"
#include "MOM_DEF.H"
#include "ST_HEAD.H"

#include "ST_CRSR.H"
#include "ST_CTRL.H"
#include "ST_FLIC.H"
#include "ST_GUI.H"
#include "ST_LBX.H"
#include "ST_SA.H"
#include "ST_SCRN.H"
#include "ST_VGA.H"

#include "TST_MGC.H"

#include "STU_DBG.H"
#include "STU_VGA.H"


        /* ╔══════════════════════════════════════════════════════════════════╗
        ╔══╝  Test - Cursor                                                   ║
        ╠═════════════════════════════════════════════════════════════════════╣
        ║                                                                     ║
        ║  Draw, Store, Restore                                               ║
        ║  ~ Show / Hide                                                      ║
        ║                                                                     ║
        ║  Render-ScreenPage, Draw-ScreenPage                                 ║
        ║                                                                     ║
        ║                                                                     ║
        ║                                                                  ╔══╝
        ╚══════════════════════════════════════════════════════════════════╝ */

/*
g_CRSR_Save_RSP
g_CRSR_Save_DSP

gsa_PaletteLbxEntry
void VGA_DAC_Init(char *PaletteLbxFileName)
    strcpy(g_PaletteLbxFileName, PaletteLbxFileName);
    gsa_FontStyleData = LBXE_LoadSingle(PaletteLbxFileName, 0);
    gsa_BorderStyleData = LBXE_LoadSingle(PaletteLbxFileName, 1);
    gsa_PaletteLbxEntry = SA_Allocate_Space(348);      // 348 paragraphs = 386 * 16 bytes = 5,568 bytes


_s20p01c.c      VGA_LoadPalette             void VGA_LoadPalette(int Palette_Index, int First_Color, int Last_Color)
void VGA_LoadPalette(int Palette_Index, int First_Color, int Last_Color)

    gsa_gsa_PaletteLbxEntry = LBXE_LoadReplace(g_PaletteLbxFileName, Palette_Index+2, gsa_PaletteLbxEntry);
                             // |-> LBX_EntryLoader(g_PaletteLbxFileName, Palette_Index+2, gsa_PaletteLbxEntry, 1, 0);

                                                              //                0x0000  3*265 palette / color-map
    gsa_Palette_Font_Colors = gsa_gsa_PaletteLbxEntry +  48;  //  48 pr  768 b  0x0300  16 arrays of 16 colors
    UU_gsa_Palette_Data     = gsa_Palette_Font_Colors +  16;  //  16 pr  256 b  0x0400  ? UnUsed ? DNE in MoO1 ?
    gsa_Cursor_Array        = UU_gsa_Palette_Data     +  16;  //  16 pr  256 b  0x0500  16 16x16 cursor bitmap images
    gsa_ShadingColors       = gsa_Cursor_Array        + 256;  // 256 pr 4096 b  0x1500  5,376 bytes array of 24 color fractions (B-G-R-Percent)

*/

void test_cursor(void)
{
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: test_cursor()\n", __FILE__, __LINE__);
#endif

    STGE_Startup();

    // s14p03    void VGA_DAC_Init(char *PaletteLbxFileName)
    gsa_PaletteLbxEntry = SA_Allocate_Space(348);      // 348 paragraphs = 386 * 16 bytes = 5,568 bytes

    // VGA_LoadPalette(0, -1, 0);
    // VGA_DAC_Write();

    GUI_SetWindows(1, g_GUI_MainMenuWindow);
    
    // s20p01    void VGA_LoadPalette(int Palette_Index, int First_Color, int Last_Color)
    gsa_gsa_PaletteLbxEntry = LBXE_LoadReplace(g_PaletteLbxFileName, 0+2, gsa_PaletteLbxEntry);
    //gsa_Cursor_Array = UU_gsa_Palette_Data + 16;
    gsa_Cursor_Array = gsa_gsa_PaletteLbxEntry + 48 + 16 + 16;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: test_cursor()\n", __FILE__, __LINE__);
#endif
}



/*
_s33p37         GUI_DrawCursor_RSP |-> CRL_Draw_RSP
_s33p38         GUI_DrawCursor_DSP |-> VGA_DrawCursor_DSP
_s33p39         CRL_Draw_RSP
_s33p40         VGA_DrawCursor_DSP
*/
void test_CR_Draw_C(void)
{
    int ScreenPage_Idx = g_RSP_Idx;
    struct s_GUI_WINDOW test_scrn00[1] = { 0, 0, 0, 0, 319, 199};
    struct s_GUI_WINDOW test_scrn01[1] = { 1, 0, 0, 0, 319, 199};
    struct s_GUI_WINDOW test_scrn02[1] = { 2, 0, 0, 0, 319, 199};
    struct s_GUI_WINDOW test_scrn03[1] = { 3, 0, 0, 0, 319, 199};
    struct s_GUI_WINDOW test_scrn04[1] = { 4, 0, 0, 0, 319, 199};
    struct s_GUI_WINDOW test_scrn05[1] = { 5, 0, 0, 0, 319, 199};
    struct s_GUI_WINDOW test_scrn06[1] = { 6, 0, 0, 0, 319, 199};
    struct s_GUI_WINDOW test_scrn07[1] = { 7, 0, 0, 0, 319, 199};
    struct s_GUI_WINDOW test_scrn08[1] = { 8, 0, 0, 0, 319, 199};
    struct s_GUI_WINDOW test_scrn09[1] = { 9, 0, 0, 0, 319, 199};
    struct s_GUI_WINDOW test_scrn10[1] = {10, 0, 0, 0, 319, 199};
    struct s_GUI_WINDOW test_scrn11[1] = {11, 0, 0, 0, 319, 199};
    struct s_GUI_WINDOW test_scrn12[1] = {12, 0, 0, 0, 319, 199};
    struct s_GUI_WINDOW test_scrn13[1] = {13, 0, 0, 0, 319, 199};
    struct s_GUI_WINDOW test_scrn14[1] = {14, 0, 0, 0, 319, 199};
    struct s_GUI_WINDOW test_scrn15[1] = {15, 0, 0, 0, 319, 199};
    struct s_GUI_WINDOW test_scrn16[1] = {16, 0, 0, 0, 319, 199};

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: test_CR_Draw_C()\n", __FILE__, __LINE__);
#endif

    //STGE_Init_Draw();
    STGE_Startup();

//struct s_GUI_WINDOW g_GUI_MainMenuWindow[1] = {1, 0, 0, 319, 199, 0};    // dseg:205E
    GUI_SetWindows(1, g_GUI_MainMenuWindow);
    //g_GUI_WindowCount = argWindowCount = 1
    //g_GUI_Windows = argWindowArray g_GUI_MainMenuWindow
    // g_CR_CursorOffset = GUI_WINDOW.Cursor_Offset = 0
    // g_CR_Curr = GUI_WINDOW.Cursor_Index = 1
    // g_CRSR_Prev = 0
    dlvfprintf("DEBUG: [%s, %d] g_GUI_WindowCount: %d\n", __FILE__, __LINE__, g_GUI_WindowCount);
    dlvfprintf("DEBUG: [%s, %d] g_GUI_Windows->Cursor_Index: %d\n", __FILE__, __LINE__, g_GUI_Windows->Cursor_Index);
    dlvfprintf("DEBUG: [%s, %d] g_GUI_Windows->Cursor_Offset: %d\n", __FILE__, __LINE__, g_GUI_Windows->Cursor_Offset);
    dlvfprintf("DEBUG: [%s, %d] g_GUI_Windows->Left: %d\n", __FILE__, __LINE__, g_GUI_Windows->Left);
    dlvfprintf("DEBUG: [%s, %d] g_GUI_Windows->Top: %d\n", __FILE__, __LINE__, g_GUI_Windows->Top);
    dlvfprintf("DEBUG: [%s, %d] g_GUI_Windows->Right: %d\n", __FILE__, __LINE__, g_GUI_Windows->Right);
    dlvfprintf("DEBUG: [%s, %d] g_GUI_Windows->Bottom: %d\n", __FILE__, __LINE__, g_GUI_Windows->Bottom);
    dlvfprintf("DEBUG: [%s, %d] g_CRSR_Offset: %d\n", __FILE__, __LINE__, g_CRSR_Offset);
    dlvfprintf("DEBUG: [%s, %d] g_CRSR_Curr: %d\n", __FILE__, __LINE__, g_CRSR_Curr);
    dlvfprintf("DEBUG: [%s, %d] g_CRSR_Prev: %d\n", __FILE__, __LINE__, g_CRSR_Prev);
    GUI_FindWindow(32,20);
    dlvfprintf("DEBUG: [%s, %d] g_CRSR_Offset: %d\n", __FILE__, __LINE__, g_CRSR_Offset);
    dlvfprintf("DEBUG: [%s, %d] g_CRSR_Curr: %d\n", __FILE__, __LINE__, g_CRSR_Curr);
    dlvfprintf("DEBUG: [%s, %d] g_CRSR_Prev: %d\n", __FILE__, __LINE__, g_CRSR_Prev);

    //CR_Draw_C(32, 20, Crsr_Normal);  // 1oom: ui_cursor_draw
    //CR_Draw_C(32, 20, ScreenPage_Idx);  // 1oom: ui_cursor_draw

    //g_CRSR_Curr = Crsr_Normal;
    //CR_Draw_C(32, 20, 0);
    //HERE("g_CRSR_Curr = 2; CR_Draw_C(64, 20, 0);");
    //g_CRSR_Curr = 2;
    //CR_Draw_C(64, 20, 0);
    //g_CRSR_Curr = 3;
    //CR_Draw_C(96, 20, 0);
    //g_CRSR_Curr = 4;
    //CR_Draw_C(128, 20, 0);
    //g_CRSR_Curr = 5;
    //CR_Draw_C(160, 20, 0);
    //g_CRSR_Curr = 6;
    //CR_Draw_C(192, 20, 0);
    //g_CRSR_Curr = 7;
    //CR_Draw_C(224, 20, 0);
    //g_CRSR_Curr = 8;
    //CR_Draw_C(256, 20, 0);
    //g_CRSR_Curr = 9;
    //CR_Draw_C(32, 52, 0);
    //g_CRSR_Curr = 10;
    //CR_Draw_C(64, 52, 0);
    //g_CRSR_Curr = 11;
    //CR_Draw_C(96, 52, 0);
    //g_CRSR_Curr = 12;
    //CR_Draw_C(128, 52, 0);
    //g_CRSR_Curr = 13;
    //CR_Draw_C(160, 52, 0);
    //g_CRSR_Curr = 14;
    //CR_Draw_C(192, 52, 0);
    //g_CRSR_Curr = 15;
    //CR_Draw_C(224, 52, 0);
    //g_CRSR_Curr = 16;
    //CR_Draw_C(256, 52, 0);
    //getch();

    g_CRSR_Curr = Crsr_Normal;
    g_CRSR_Prev = Crsr_Normal;
    g_CRSR_HaveSave = ST_FALSE;

    CR_Draw_C(32, 20, 0);
    getch();
    CR_Store_C(32, 20, 0);
    getch();
    VGA_PageFlip();
    getch();
    CR_Restore_C(1);
    getch();
    VGA_PageFlip();  // ? eyeball the placement and pixel matchup ?
    getch();

    //VGA_DrawFilledRect(0, 0, 319, 199, 0);
    //VGA_SetDirectDraw();
    //VGA_DrawFilledRect(0, 0, 319, 199, 0);
    //VGA_Set_DSP_Addr();
    //VGA_PageFlip();
    //getch();
    //CR_Restore_C(0);
    //getch();
    //CR_Restore_C(1);
    //getch();
    //VGA_PageFlip();
    //getch();

    VGA_SetTextMode();

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: test_CR_Draw_C()\n", __FILE__, __LINE__);
#endif

}
