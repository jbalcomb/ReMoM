
#include "ST_TYPE.H"
#include "ST_DEF.H"     /* FP_SEG(), FP_OFF(); */
#include "MGC_DEF.H"

/*
    EMM_Pages_Reserved = EMM_PAGES_REQUIRED;
    EMM_SetMinKB(EMM_MIN_KB);
    RAM_SetMinKB(RAM_MIN_KB);
*/
#include "ST_EMM.H"     /* EMM_Pages_Reserved */
#include "ST_DBG.H"     /* DBG_IsDisabled(), DBG_ScreenDump() */
#include "ST_FLIC.H"
#include "ST_LBX.H"     /* LBX_Error(), LBXE_LoadSingle() */
#include "ST_SA.H"      /* SA_Allocate_MemBlk(), SA_Allocate_Space(); */
#include "ST_VGA.H"     /* font_name */

#include "seg001.H"     /* GAME_LoadMainImages(); */
#include "seg014.H"     /* Hardware_Init(), Load_Font_File(); */
#include "seg019.H"     /* VGA_TextDraw_Init() */
#include "seg020.H"     /* palette_block, palette_data; PAL_Load_Palette() */
#include "seg021.H"     /* FLIC_Load_Palette(); */
#include "seg022.H"     /* ST_MoveData() */
#include "seg028.H"     /* FLIC_Draw_XY(); */
/* VGA_TextDraw_Init() */
/* IN_Init() */
/* MD_Init() */
/* MoM_Tables_Init() */
/* VGA_Set_DSP_Addr() */
/* VGA_DAC_Write() */

#include "STU_DBG.H"    /* DLOG(); Debug_Log_Startup(), Debug_Log_Shutdown(); */
#include "STU_TST.H"    /* TLOG(); Test_Log_Startup(), Test_Log_Shutdown();*/
#include "STU_VGA.H"    /* STU_VGA_RAM_Dump(); */

#include <ASSERT.H>     /* NDEBUG; assert(); */
#include <CONIO.H>      /* getch() */
#include <STDLIB.H>     /* abort(), getenv(); */
#include <STRING.H>     /* strcmp() */


/*
    BCpp31PG, pg 175/190
        #pragma warn -yyy
    BCpp31UG, pg 162/171
    BCpp30UG, pg 158/167
        -waus*  Identifier is assigned a value that is never used.
        -wccc*  Condition is always true/false.
        -weff*  Code has no effect.
        -wpar*  Parameter parameter is never used.
        -wuse   Identifier is declared but never used.
        -wrpt*  Nonportable pointer conversion.
*/
#pragma warn -aus
#pragma warn -eff
#pragma warn -par


/*
    EMM - Uninitialized
        ? ALL Xref seg012 and seg013 ?
*/
//dseg:A5E0 UU_EMMData_PrevLvl dw 0
extern unsigned int g_EMM_Overlay_Handle;           // dseg:A5E2
extern unsigned int EMMDATAH_Level;                 // dseg:A5E4
extern unsigned int EmmHndlNbr_EMMDATAH;            // dseg:A5E6
// ? dseg:A5E8 UU_EMM_Handle dw 0
extern unsigned int g_EmmHndlNbr_VGAFILEH;          // dseg:A5EA
extern unsigned int g_LBX_EmmRsvd;                  // dseg:A5EC
extern unsigned int EmmHndlNbr_YOMOMA;              // dseg:A5EE
extern unsigned int EMM_OK;                         // dseg:A5F0
extern EMM_Record EMM_Table[];                      // dseg:A5F2 g_EMS_Table EMS_Record 28h dup(<0>)
extern unsigned int g_EMM_MinKB;                    // dseg:A7D2


unsigned char g_EMM_tested = 0;
unsigned char g_EMM_validated = 0;
unsigned char g_Load_Font_File_tested = 0;
unsigned char g_Load_Font_File_validated = 0;
unsigned char g_MAINSCRN_LBX_EMM_tested = 0;
unsigned char g_MAINSCRN_LBX_EMM_validated = 0;
unsigned char g_FLIC_Load_Palette_tested = 0;
unsigned char g_FLIC_Load_Palette_validated = 0;


void test_MGC_Main(void);
void test_VGA_VRAM(void);
void test_Load_MAINSCRN_LBX_EMM(void);
void test_Load_MAINSCRN_000(void);
void test_Load_MAINSCRN_005(void);
void test_VGA_SetDirectDraw(void);
void test_VGA_Set_DSP_Addr(void);
void test_PAL_Load_Palette(void);
void test_Load_Font_File(void);
void test_EMM_Init(void);
void test_EMM_Startup(void);
void test_EMM_Load_LBX_File(void);
void test_GAME_LoadMainImages(void);
void test_FLIC_Load_Palette(void);
void test_FLIC_Draw_EMM(void);
void test_FLIC_Draw_XY(void);
void test_SA_Error(void);
void test_LBX_Error(void);


int main(void)
{
    char * cstr_ComSpec;
    Debug_Log_Startup();
    Test_Log_Startup();
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: TST_MGC main()\n", __FILE__, __LINE__);
#endif

    /* get comspec environment parameter */
    cstr_ComSpec = getenv("COMSPEC");
    /* display comspec parameter */
    dbg_prn("Command processor: %s\n", cstr_ComSpec);
    // flags = fnsplit(s,drive,dir,file,ext);
    if( strcmp(cstr_ComSpec,"Z:\\COMMAND.COM") == 0)
    {
        dbg_prn("Running in DOXBox...\n");
    }

#if defined(__VSCODE__)
    abort();
#endif

    // test_VGA_SetDirectDraw();
    // test_VGA_Set_DSP_Addr();

    // test_PAL_Load_Palette();  // TEST_SUCCESS, for FONTS.LBX,2

    // test_Load_Font_File();

    // test_EMM_Init();  // TEST_SUCCESS
    // test_EMM_Startup();  // TEST_SUCCESS
    // test_EMM_Load_LBX_File();

    //test_GAME_LoadMainImages();

    // test_Load_MAINSCRN_LBX_EMM();  // TEST_SUCCESS

    // test_Load_MAINSCRN_000();  // TEST_SUCCESS
    // test_Load_MAINSCRN_005();
    // 
    // // test_VGA_VRAM();
    // 
    // // test_FLIC_Load_Palette();
    // test_FLIC_Draw_EMM();
    // // test_FLIC_Draw_XY();

    test_MGC_Main();

    // test_SA_Error();
    // test_LBX_Error();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: TST_MGC main()\n", __FILE__, __LINE__);
#endif
    Test_Log_Shutdown();
    Debug_Log_Shutdown();
    return 0;
}

void test_MGC_Main(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_MGC_Main()\n", __FILE__, __LINE__);
#endif

    // MGC main()
    DLOG("CALL: EMM_Pages_Reserved = EMM_PAGES_REQUIRED;");
    EMM_Pages_Reserved = EMM_PAGES_REQUIRED;
    // MGC main()
    DLOG("CALL: EMM_SetMinKB(EMM_MIN_KB);");
    EMM_SetMinKB(EMM_MIN_KB);
    // MGC main()
    DLOG("CALL: RAM_SetMinKB(RAM_MIN_KB);");
    RAM_SetMinKB(RAM_MIN_KB);

    // MGC main() |-> Hardware_Init()
    DLOG("CALL: EMM_Startup();");
    EMM_Startup();
    // MGC main() |-> Hardware_Init()
    DLOG("CALL: VGA_SetModeY();");
    VGA_SetModeY();
    // MGC main() |-> Hardware_Init()
    // s14p03
    DLOG("CALL: Load_Font_File(GAME_FONT_FILE);");
    Load_Font_File(GAME_FONT_FILE);  // "FONTS.LBX"
        // |-> ... LBXE_LoadSingle(FONTS.LBX,0), SA_Allocate_MemBlk()
        // |-> ... LBXE_LoadSingle(FONTS.LBX,1), SA_Allocate_MemBlk()
        // |-> VGA_TextDraw_Init()

    // MGC main() |-> Hardware_Init()
    DLOG("CALL: IN_Init(1);");
    IN_Init(1);  // INPUT_TYPE_KEYBOARD_AND_MOUSE
        // |-> MD_Init()
    // MGC main() |-> Hardware_Init()
    DLOG("CALL: VGA_Set_DSP_Addr();");
    VGA_Set_DSP_Addr();
    // MGC main()
    DLOG("CALL: MoM_Tables_Init(6100);");
    MoM_Tables_Init(6100);
    // MGC main()
    // s20p01
    DLOG("CALL: PAL_Load_Palette(0, -1, 0);");
    PAL_Load_Palette(0, -1, 0);
    // MGC main()
    DLOG("CALL: VGA_DAC_Write();");
    VGA_DAC_Write();
    // MGC main()
    DLOG("CALL: GAME_LoadMainImages();");
    GAME_LoadMainImages();
    // MGC main()
    DLOG("CALL: GAME_Load_TERRSTAT_0();");
    GAME_Load_TERRSTAT_0();
    // MGC main()
    DLOG("CALL: GAME_Load_SPELLDAT_0();");
    GAME_Load_SPELLDAT_0();
    // MGC main()
    DLOG("CALL: VGA_DrawFilledRect(0, 0, 319, 199, 0);");
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Draw Screen-Page
    // MGC main()
    DLOG("CALL: VGA_SetDirectDraw();");
    VGA_SetDirectDraw();
    // MGC main()
    DLOG("CALL: VGA_DrawFilledRect(0, 0, 319, 199, 0);");
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Render Screen-Page
    // MGC main()
    DLOG("CALL: VGA_Set_DSP_Addr();");
    VGA_Set_DSP_Addr();
    // MGC main()
    DLOG("CALL: PAL_Load_Palette(0, -1, 0);");
    PAL_Load_Palette(0, -1, 0);  // EMPERATO
    // MGC main()
    DLOG("CALL: VGA_DAC_Write();");
    VGA_DAC_Write();

    // MGC main()
    DLOG("CALL: GAME_MainMenu();");
    GAME_MainMenu();  // MGC_DEF.H  _s01p03c.c
    // ...
    //          Screen_Action = SCREEN_Menu();  // MGC_DEF.H  _s01p05c.c
    // MGC main() |-> GAME_MainMenu() |-> SCREEN_Menu()
    //             SCREEN_Menu_Draw();  // MGC_DEF.H  _s01p06c.c
    //             SCRN_SimplePageFlip();
    // MGC main() |-> GAME_MainMenu() |-> SCREEN_Menu() |-> SCREEN_Menu_Draw()
    //     FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);  // NOTE(JimBalcomb): This is the first call to FLIC_Draw_XY()
    //     FLIC_Draw_XY(0, 41, gsa_MAINSCRN_5_ScreenBottom);
    //     FLIC_Draw_XY(MenuArea_X_Left, (MenuArea_Y_Top + 12), gsa_VORTEX_5_MenuLoadGame);
    //     FLIC_Draw_XY(MenuArea_X_Left, (MenuArea_Y_Top + (12 * Continue_Move_Down)), gsa_VORTEX_1_MenuContinue);
    //     FLIC_Draw_XY(MenuArea_X_Left, (MenuArea_Y_Top + 24), gsa_VORTEX_4_MenuNewGame);
    //     FLIC_Draw_XY(MenuArea_X_Left, (MenuArea_Y_Top + 36), gsa_VORTEX_2_MenuHallOfFame);
    //     FLIC_Draw_XY(MenuArea_X_Left, (MenuArea_Y_Top + 48), gsa_VORTEX_3_MenuQuitToDOS);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_MGC_Main()\n", __FILE__, __LINE__);
#endif
    // Quit();
    // VGA_SetTextMode();
}

/*
    Test
        Write & Read VGA Graphics Mode Video Memory (VRAM)

    Sequencer Address Register and the Sequencer Data Register
    Sequencer Registers
    Map Mask Register (Index 02h)
        Memory Plane Write Enable

    Screen-Page Index
    Screen-Page Segment Address

*/
void test_VGA_VRAM(void)
{
//     // #define CLRSCR0() { VGA_DrawFilledRect(0,0,319,199,0); VGA_SetDrawAddress(); }
//     // #define CLRSCR1() { VGA_DrawFilledRect(0,0,319,199,0); VGA_SetDirectDraw();  }
//     // VGA_DrawFilledRect(0,0,319,199,0);
//     // VGA_SetDrawAddress();
//     // VGA_SetDirectDraw();
// 
//     Load_Font_File(GAME_FONT_FILE);
//     // unsigned int gsa_DSP_Addr = 0xA000;                 // dseg:41C4
//     // int g_RSP_Idx = 0;                                  // dseg:41C6
//     VGA_Set_DSP_Addr();  // Sets Screen-Page Segment Addres, based on Screen-Page Index    gsa_DSP_Addr = VRAM_BASE + ( (1 - g_RSP_Idx) << 10 );
//     // FLIC_Draw_EMM() uses gsa_DSP_Addr to set the Dst_Sgmt ... Also, Offset to Scan-Line
//     // NOTE: VGA_PageFlip() is the place that g_RSP_Idx is changed - it sets it to (1 - g_RSP_Idx)
//     PAL_Load_Palette(0, -1, 0);  // requires Load_Font_File(), LBXE_LoadReplace(), LBX_Load_Entry(), SA_Allocate_MemBlk(), SA_Allocate_Space(), 
//     VGA_DAC_Write();
//         // outportb( 0x3C8, itrVgaDacColors );
//         // outportb( 0x3C9, ptr_Palette[ofstPalette++] );
//         // outportb( 0x3C9, ptr_Palette[ofstPalette++] );
//         // outportb( 0x3C9, ptr_Palette[ofstPalette++] );
// 
// // asm     mov dx, 0x03C4 //; SC_INDEX
// // asm     mov al, 0x02  //; SC_MAP_MASK
// // asm     out dx, al
// 
// // asm mov dx, 0x03C5 //; SC_DATA
// // asm mov al, bl
// // asm out dx, al
// 
//     VGA_SetModeY();
//     getch();
//     // VGA_DrawFilledRect(0,0,319,199,0);
//     VGA_Set_DSP_Addr();  // gsa_DSP_Addr = VRAM_BASE + ( (1 - g_RSP_Idx) << 10 );
//     // VGA_DrawFilledRect(0,0,319,199,0);
//     VGA_SetDirectDraw();  // gsa_DSP_Addr = VRAM_BASE + ( (g_RSP_Idx) << 10 );
//     // ? Set the *Draw* to the Back-Buffer
//     // ? Set the *Draw* to the Front-Buffer
// 
//     // ; NOTE(JimBalcomb,20220721): I can not recall what the issue was such that I chose/needed to comment out this chunk of code
//     // ; mov	ax, VIDEO_RAM
//     // ; mov	es, ax
//     // ; sub	di, di
//     // ; mov	ax, di
//     // ; mov	cx, 8000h                           ; 32,768 times ... 65,536 zeros
//     // ; rep stosw  ; WORD [ES:DI] = AX;
// 
// 
//     VGA_SetTextMode();


    VGA_SetModeY();
    // g_RSP_Idx = 0;
    // gsa_DSP_Addr = 0xA000;
    VGA_DrawFilledRect(0,0,319,199,5);
    getch();
    // BAD!! STU_VGA_RAM_Dump_1();
    STU_VGA_RAM_Dump();
    
    VGA_SetTextMode();
}

void test_VGA_SetDirectDraw(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_VGA_SetDirectDraw()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dbg_prn("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dbg_prn("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    g_RSP_Idx = 0;
    VGA_SetDirectDraw();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dbg_prn("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dbg_prn("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    g_RSP_Idx = 1;
    VGA_SetDirectDraw();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dbg_prn("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dbg_prn("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    if ( g_RSP_Idx == 0 )
    {
        if ( gsa_DSP_Addr != 0xA000 )
        {
            dbg_prn("TEST: [%s, %d] FAILURE: ( (g_RSP_Idx == 0 ) && (gsa_DSP_Addr != 0xA000) )\n", __FILE__, __LINE__);
            dbg_prn("TEST: [%s, %d] FAILURE: g_RSP_Idx: %d; gsa_DSP_Addr: 0x%04X;\n", __FILE__, __LINE__, g_RSP_Idx, gsa_DSP_Addr);
            exit(1);
        }
        else
        {
            dbg_prn("TEST: [%s, %d] SUCCESS: ( (g_RSP_Idx == 0 ) && (gsa_DSP_Addr == 0xA000) )\n", __FILE__, __LINE__);
        }
    }
    else
    {
        dbg_prn("TEST: [%s, %d] SKIP: g_RSP_Idx != 0\n", __FILE__, __LINE__);
    }

    if ( g_RSP_Idx == 1 )
    {
        if ( gsa_DSP_Addr != 0xA400 )
        {
            dbg_prn("DEBUG: [%s, %d] FAILURE: ( (g_RSP_Idx == 1 ) && (gsa_DSP_Addr != 0xA400) )\n", __FILE__, __LINE__);
            dbg_prn("DEBUG: [%s, %d] FAILURE: g_RSP_Idx: %d; gsa_DSP_Addr: 0x%04X;\n", __FILE__, __LINE__, g_RSP_Idx, gsa_DSP_Addr);
            exit(1);
        }
        else
        {
            dbg_prn("DEBUG: [%s, %d] SUCCESS: ( (g_RSP_Idx == 1 ) && (gsa_DSP_Addr == 0xA400) )\n", __FILE__, __LINE__);
        }
    }
    else
    {
        dbg_prn("TEST: [%s, %d] SKIP: g_RSP_Idx != 1\n", __FILE__, __LINE__);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_VGA_SetDirectDraw()\n", __FILE__, __LINE__);
#endif
}

void test_Load_MAINSCRN_LBX_EMM(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_Load_MAINSCRN_LBX_EMM()\n", __FILE__, __LINE__);
#endif

    if (!g_EMM_tested) { test_EMM_Startup(); }
    if (!g_EMM_validated) {  abort(); }

    // MGC main() |-> GAME_LoadMainImages()
    EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);
    g_MAINSCRN_LBX_EMM_tested = 1;
    if( validate_MAINSCRN_LBX_EMM() ) { g_MAINSCRN_LBX_EMM_validated = 1; }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_Load_MAINSCRN_LBX_EMM()\n", __FILE__, __LINE__);
#endif
}

/*
unsigned int gsa_MAINSCRN_0_AnimatedLogo;   // dseg:52E8
gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
*/
void test_Load_MAINSCRN_000(void)
{
    SAMB_data sa_MAINSCRN_000;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_Load_MAINSCRN_000()\n", __FILE__, __LINE__);
#endif

    if(!g_EMM_tested) { test_EMM_Startup(); }
    if(!g_EMM_validated) {  abort(); }

    // GAME_LoadMainImages()
    // EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);
    if(!g_MAINSCRN_LBX_EMM_tested) { test_Load_MAINSCRN_LBX_EMM(); }
    if(!g_MAINSCRN_LBX_EMM_validated) { abort(); }

    sa_MAINSCRN_000 = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
    TST_LBX_MAINSCRN_000.Segment_Address = sa_MAINSCRN_000;

    validate_FLIC_Header(sa_MAINSCRN_000);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_Load_MAINSCRN_000()\n", __FILE__, __LINE__);
#endif
}

/*
unsigned int gsa_MAINSCRN_0_AnimatedLogo;   // dseg:52E8
gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
*/
void test_Load_MAINSCRN_005(void)
{
    SAMB_data sa_MAINSCRN_005;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_Load_MAINSCRN_005()\n", __FILE__, __LINE__);
#endif

    if (!g_EMM_tested) { test_EMM_Startup(); }
    if (!g_EMM_validated) { abort(); }

    // GAME_LoadMainImages()
    // EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);
    if(!g_MAINSCRN_LBX_EMM_tested) { test_Load_MAINSCRN_LBX_EMM(); }
    if(!g_MAINSCRN_LBX_EMM_validated) { abort(); }

    sa_MAINSCRN_005 = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 5);
    TST_LBX_MAINSCRN_005.Segment_Address = sa_MAINSCRN_005;

    validate_FLIC_Header(sa_MAINSCRN_005);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_Load_MAINSCRN_005()\n", __FILE__, __LINE__);
#endif
}

/*
    _s26p02c.c  void VGA_Set_DSP_Addr(void)
    
    ST_VGA.H
        extern unsigned int gsa_DSP_Addr;                   // dseg:41C4
        extern int g_RSP_Idx;                               // dseg:41C6
    ST_VGA.C
        unsigned int gsa_DSP_Addr = 0xA000;                 // dseg:41C4
        int g_RSP_Idx = 0;                                  // dseg:41C6
    
    Pre-Initialized
        screen-page index 0; screen-page adress 0xA000;
    In-Game
        g_RSP_Idx changed in VGA_PageFlip()
        gsa_DSP_Addr changed in VGA_Set_DSP_Addr() and VGA_SetDirectDraw()
        VGA_PageFlip() sets the (render) screen-page index as (1 - screen-page index)
        VGA_Set_DSP_Addr() sets the screen-page address based on (1 - screen-page index)
        VGA_SetDirectDraw() sets the screen-page address based on the screen-page index, rather than (1 - screen-page index)

*/
void test_VGA_Set_DSP_Addr(void)
{

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_VGA_Set_DSP_Addr()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dbg_prn("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dbg_prn("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    g_RSP_Idx = 0;
    VGA_Set_DSP_Addr();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dbg_prn("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dbg_prn("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    g_RSP_Idx = 1;
    VGA_Set_DSP_Addr();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dbg_prn("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dbg_prn("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    if ( g_RSP_Idx == 0 )
    {
        if ( gsa_DSP_Addr != 0xA400 )
        {
            dbg_prn("TEST: [%s, %d] FAILURE: ( (g_RSP_Idx == 0 ) && (gsa_DSP_Addr != 0xA400) )\n", __FILE__, __LINE__);
            dbg_prn("TEST: [%s, %d] FAILURE: g_RSP_Idx: %d; gsa_DSP_Addr: 0x%04X;\n", __FILE__, __LINE__, g_RSP_Idx, gsa_DSP_Addr);
            exit(1);
        }
        else
        {
            dbg_prn("TEST: [%s, %d] SUCCESS: ( (g_RSP_Idx == 0 ) && (gsa_DSP_Addr == 0xA400) )\n", __FILE__, __LINE__);
        }
    }
    else
    {
        dbg_prn("TEST: [%s, %d] SKIP: g_RSP_Idx != 0\n", __FILE__, __LINE__);
    }

    if ( g_RSP_Idx == 1 )
    {
        if ( gsa_DSP_Addr != 0xA000 )
        {
            dbg_prn("DEBUG: [%s, %d] FAILURE: ( (g_RSP_Idx == 1 ) && (gsa_DSP_Addr != 0xA000) )\n", __FILE__, __LINE__);
            dbg_prn("DEBUG: [%s, %d] FAILURE: g_RSP_Idx: %d; gsa_DSP_Addr: 0x%04X;\n", __FILE__, __LINE__, g_RSP_Idx, gsa_DSP_Addr);
            exit(1);
        }
        else
        {
            dbg_prn("DEBUG: [%s, %d] SUCCESS: ( (g_RSP_Idx == 1 ) && (gsa_DSP_Addr == 0xA000) )\n", __FILE__, __LINE__);
        }
    }
    else
    {
        dbg_prn("TEST: [%s, %d] SKIP: g_RSP_Idx != 1\n", __FILE__, __LINE__);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_VGA_Set_DSP_Addr()\n", __FILE__, __LINE__);
#endif

}

void test_PAL_Load_Palette(void)
{
    char font_file[11] = "FONTS.LBX";
    char font_name[16];
    SAMB_ptr sad1_PaletteLbxEntry;
    SAMB_ptr sah1_PaletteLbxEntry;
    int entry;
    int start_color;
    int end_color;
    // int i;

    // ~== Load_Font_File()
    strcpy(font_name, font_file);
    sah1_PaletteLbxEntry    = SA_Allocate_Space(348);           // 348 paragraphs = 386 * 16 bytes = 5568 bytes
    p_Palette               = SA_Allocate_Space(64);            //  64 paragraphs =  64 * 16 bytes = 1024 bytes
    p_PaletteFlags          = p_Palette + (48 * 16);            // ~== p_PaletteFlags = &p_Palette[768];
    // for ( i = 0; i < 768; i++)
    // {
    //     *(p_Palette + i) = 0;
    // }
    // for ( i = 0; i < 256; i++)
    // {
    //     *(p_PaletteFlags + i) = 0;
    // }

    dbg_prn("DEBUG: [%s, %d] sah1_PaletteLbxEntry: %p\n", __FILE__, __LINE__, sah1_PaletteLbxEntry);
    dbg_prn("DEBUG: [%s, %d] p_Palette: %p\n", __FILE__, __LINE__, p_Palette);
    dbg_prn("DEBUG: [%s, %d] p_PaletteFlags: %p\n", __FILE__, __LINE__, p_PaletteFlags);

    // ~== s20p01 PAL_Load_Palette()
    // void PAL_Load_Palette(int Palette_Index, int First_Color, int Last_Color)
    // MGC main() |-> PAL_Load_Palette(0, -1, 0);  // EMPERATO
    // MoO2  Module: fonts  Load_Palette(signed integer (2 bytes) entry, signed integer (2 bytes) start_color, signed integer (2 bytes) end_color)
    entry = 0;
    start_color = -1;
    end_color = 0;
    sad1_PaletteLbxEntry  = (SAMB_ptr)MK_FP(LBXE_LoadReplace(font_name, entry+2, (SAMB_addr)FP_SEG(sah1_PaletteLbxEntry)),0);

    validate_PaletteLbxEntry_2(sad1_PaletteLbxEntry);

    // s20p05 VGA_SetDACChanged()
    // if ( start_color == -1 ) { VGA_SetDACChanged(0, 255); } else { VGA_SetDACChanged(start_color, end_color); }

    dbg_prn("DEBUG: [%s, %d] sad1_PaletteLbxEntry: 0x%04X\n", __FILE__, __LINE__, sad1_PaletteLbxEntry);

    // ~== VGA_DAC_Write()

}


/*
    MGC main()
        |-> Hardware_Init(1, 2, GAME_FONT_FILE, 0, 0, 0, 0, 0, 0, 0, 0);  // Defaults for 'No Sound'
            |-> Load_Font_File()
    
    Load_Font_File()
        |-> ... LBXE_LoadSingle(), SA_Allocate_Space(), VGA_TextDraw_Init()
    
*/
void test_Load_Font_File(void)
{
    int test_status;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_Load_Font_File()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED

    dbg_prn("DEBUG: [%s, %d] font_name: %s\n", __FILE__, __LINE__, GAME_FONT_FILE);

    dbg_prn("DEBUG: [%s, %d] font_name: %s\n", __FILE__, __LINE__, font_name);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_FontStyleData: 0x%04X\n", __FILE__, __LINE__, gsa_FontStyleData);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_BorderStyleData: 0x%04X\n", __FILE__, __LINE__, gsa_BorderStyleData);
    // dbg_prn("DEBUG: [%s, %d] sah1_PaletteLbxEntry: %p\n", __FILE__, __LINE__, sah1_PaletteLbxEntry);
    dbg_prn("DEBUG: [%s, %d] palette_block: %p\n", __FILE__, __LINE__, palette_block);
    dbg_prn("DEBUG: [%s, %d] p_Palette: %p\n", __FILE__, __LINE__, p_Palette);
    dbg_prn("DEBUG: [%s, %d] p_PaletteFlags: %p\n", __FILE__, __LINE__, p_PaletteFlags);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_PaletteSaved: 0x%04X\n", __FILE__, __LINE__, gsa_PaletteSaved);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_ReplacementColors: 0x%04X\n", __FILE__, __LINE__, gsa_ReplacementColors);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_VGAFILEH_Header: 0x%04X\n", __FILE__, __LINE__, gsa_VGAFILEH_Header);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_IntensityScaleTable: 0x%04X\n", __FILE__, __LINE__, gsa_IntensityScaleTable);

    // SM2LM dbg_prn("DEBUG: [%s, %d] UU_g_VGA_TextDraw_Initd: %u\n", __FILE__, __LINE__, UU_g_VGA_TextDraw_Initd);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gfp_VGA_TextLine_Lefts: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Lefts);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gfp_VGA_TextLine_Rights: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Rights);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gfp_VGA_TextLine_Tops: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Tops);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gfp_VGA_TextLine_Starts: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Starts);


    Load_Font_File(GAME_FONT_FILE);


    dbg_prn("DEBUG: [%s, %d] font_name: %s\n", __FILE__, __LINE__, font_name);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_FontStyleData: 0x%04X\n", __FILE__, __LINE__, gsa_FontStyleData);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_BorderStyleData: 0x%04X\n", __FILE__, __LINE__, gsa_BorderStyleData);
    // dbg_prn("DEBUG: [%s, %d] sah1_PaletteLbxEntry: %p\n", __FILE__, __LINE__, sah1_PaletteLbxEntry);
    dbg_prn("DEBUG: [%s, %d] palette_block: %p\n", __FILE__, __LINE__, palette_block);
    dbg_prn("DEBUG: [%s, %d] p_Palette: %p\n", __FILE__, __LINE__, p_Palette);
    dbg_prn("DEBUG: [%s, %d] p_PaletteFlags: %p\n", __FILE__, __LINE__, p_PaletteFlags);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_PaletteSaved: 0x%04X\n", __FILE__, __LINE__, gsa_PaletteSaved);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_ReplacementColors: 0x%04X\n", __FILE__, __LINE__, gsa_ReplacementColors);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_VGAFILEH_Header: 0x%04X\n", __FILE__, __LINE__, gsa_VGAFILEH_Header);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gsa_IntensityScaleTable: 0x%04X\n", __FILE__, __LINE__, gsa_IntensityScaleTable);

    // SM2LM dbg_prn("DEBUG: [%s, %d] UU_g_VGA_TextDraw_Initd: %u\n", __FILE__, __LINE__, UU_g_VGA_TextDraw_Initd);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gfp_VGA_TextLine_Lefts: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Lefts);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gfp_VGA_TextLine_Rights: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Rights);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gfp_VGA_TextLine_Tops: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Tops);
    // SM2LM dbg_prn("DEBUG: [%s, %d] gfp_VGA_TextLine_Starts: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Starts);

    // ASSERT()
    if ( strcmp(font_name, "FONTS.LBX") != 0 )
    {
        dbg_prn("DEBUG: [%s, %d] FAILURE: ( strcmp(font_name, \"FONTS.LBX\") != 0 )\n", __FILE__, __LINE__);
        dbg_prn("DEBUG: [%s, %d] FAILURE: font_name: %s;\n", __FILE__, __LINE__, font_name);
        exit(1);
    }
    else
    {
        dbg_prn("DEBUG: [%s, %d] SUCCESS: ( strcmp(font_name, \"FONTS.LBX\") == 0 )\n", __FILE__, __LINE__);
    }

    // validate_Palette_0();
    test_status = validate_PaletteFlags_1();
    tst_prn("TEST: [%s, %d] test_status: %d\n", __FILE__, __LINE__, test_status);
    // TSTPRN(("TEST: [%s, %d] test_status: %d\n", __FILE__, __LINE__, test_status));  // WTF!?! "Warning: Condition is always true in function..."
    if ( test_status == -1 )  // TEST_FAILURE
    {
        dbg_prn("DEBUG: [%s, %d] FAILURE: validate_PaletteFlags_1()\n", __FILE__, __LINE__);
        exit(1);
    }

    g_Load_Font_File_tested = 1;
    if ( validate_Load_Font_File() ) { g_Load_Font_File_validated = 1; }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_Load_Font_File()\n", __FILE__, __LINE__);
#endif
}


void test_EMM_Init(void)
{
    int result;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_EMM_Init()\n", __FILE__, __LINE__);
#endif

    result = EMM_Init();

    if ( result == ST_SUCCESS )
    {
        dbg_prn("DEBUG: [%s, %d] SUCCESS: \n", __FILE__, __LINE__);
    }
    if ( result == ST_FAILURE )
    {
        dbg_prn("DEBUG: [%s, %d] FAILURE: \n", __FILE__, __LINE__);
    }
    if ( ( result != ST_SUCCESS ) && ( result != ST_FAILURE ) )
    {
        dbg_prn("DEBUG: [%s, %d] INVALID: \n", __FILE__, __LINE__);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_EMM_Init()\n", __FILE__, __LINE__);
#endif
}

void test_EMM_Startup(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_EMM_Startup()\n", __FILE__, __LINE__);
#endif
    // MGC main()
    EMM_Pages_Reserved = EMM_PAGES_REQUIRED;
    // EMM_SetMinKB(EMM_MIN_KB);
    // RAM_SetMinKB(RAM_MIN_KB);
    // MGC main() |-> Hardware_Init() |-> EMM_Startup()
    EMM_Startup();
    g_EMM_tested = 1;
    if ( validate_EMM_Startup() ) { g_EMM_validated = 1; }
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_EMM_Startup()\n", __FILE__, __LINE__);
#endif
}

/*
    Test:
        s13p03          void EMM_Load_LBX_File_1(char * LbxFileName);
        s13p04          int EMM_Load_LBX_File(char * LbxFileName, int Reserved);
    Prerequisites:
        EMM_Startup() |-> EMM_Init() {EMM_PageFrameBaseAddress}

*/
void test_EMM_Load_LBX_File(void)
{

    int Title_Frame_Index;  // _s01p06c.c  SCREEN_Menu_Draw()
    struct s_FLIC_HDR * pFlicHeader; // _s21p07c.c  FLIC_Load_Palette()
    SAMB_addr SAMB_data;
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_EMM_Load_LBX_File()\n", __FILE__, __LINE__);
#endif

    // main() |-> Hardware_Init() |->
    EMM_Startup();

#ifdef STU_TEST
    if ( EMM_PageFrameBaseAddress != 0xE000 )
    {
        dbg_prn("DEBUG: [%s, %d] FAILURE: EMM_Init()\n", __FILE__, __LINE__);
        dbg_prn("DEBUG: [%s, %d] EMM_PageFrameBaseAddress: 0x%04X }\n", __FILE__, __LINE__, EMM_PageFrameBaseAddress);
        exit(1);
    }
    if (
        ( EMM_OK != ST_TRUE) ||
        ( EmmHndlNbr_YOMOMA == 0 ) ||
        ( g_EmmHndlNbr_VGAFILEH == 0 ) ||
        ( EmmHndlNbr_EMMDATAH == 0 ) ||
        ( g_EMM_Open_Handles != 3 ) ||
        ( EMM_Pages_Reserved != 31 )
    )
    {
        dbg_prn("DEBUG: [%s, %d] FAILURE: EMM_Startup()\n", __FILE__, __LINE__);
        dbg_prn("DEBUG: [%s, %d] EMM_OK: %d\n", __FILE__, __LINE__, EMM_OK);
        dbg_prn("DEBUG: [%s, %d] EmmHndlNbr_YOMOMA: %d\n", __FILE__, __LINE__, EmmHndlNbr_YOMOMA);
        dbg_prn("DEBUG: [%s, %d] g_EmmHndlNbr_VGAFILEH: %d\n", __FILE__, __LINE__, g_EmmHndlNbr_VGAFILEH);
        dbg_prn("DEBUG: [%s, %d] EmmHndlNbr_EMMDATAH: %d\n", __FILE__, __LINE__, EmmHndlNbr_EMMDATAH);
        dbg_prn("DEBUG: [%s, %d] g_EMM_Open_Handles: %d\n", __FILE__, __LINE__, g_EMM_Open_Handles);
        dbg_prn("DEBUG: [%s, %d] EMM_Pages_Reserved: %d\n", __FILE__, __LINE__, EMM_Pages_Reserved);
        exit(1);
    }
#endif

    // MGC main() |->
    // void GAME_LoadMainImages(void)
    EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);
        // |-> EMM_Load_LBX_File(LbxFileName=MAINSCRN, EmmRsvd=1)

    gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
        // |-> LBX_Load_Entry(LbxName=MAINSCRN, LbxEntry=0, SAMB_head=0x0000, LoadType=0, LbxHdrFmt=0)
        // |-> EMM_LBX_Load_Entry(EmmHndlNm=MAINSCRN, LbxEntry=0, SAMB_head=0x0000, LoadType=0, FormatType=0)
        // |-> EMM_LBX_FLIC_Header(EmmHndl=7, EmmHndlNm=MAINSCRN, LbxEntry=0, SAMB_head=0x0000, LoadType=0)
        // |-> SA_Allocate_MemBlk(nparas = 1)

    // ...
    // ...
    // ...

    // DBG_MAINSCRN_000 = gsa_MAINSCRN_0_AnimatedLogo;
    TST_LBX_MAINSCRN_000.Segment_Address = gsa_MAINSCRN_0_AnimatedLogo;
    // dbg_prn("DEBUG: [%s, %d] DBG_MAINSCRN_000: 0x%04X\n", __FILE__, __LINE__, DBG_MAINSCRN_000);
    dbg_prn("DEBUG: [%s, %d] MAINSCRN_000.Segment_Address: 0x%04X\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.Segment_Address);


    pFlicHeader = (struct s_FLIC_HDR *)MK_FP(gsa_MAINSCRN_0_AnimatedLogo, 0);
    //pFlicHeader = (void _FAR *) ( ((unsigned long) (gsa_MAINSCRN_0_AnimatedLogo) << 16) | (0) ) )
    dbg_prn("DEBUG: [%s, %d] pFlicHeader: %p\n", __FILE__, __LINE__, pFlicHeader);
    
    dbg_prn("DEBUG: [%s, %d] Width = %d\n", __FILE__, __LINE__, pFlicHeader->Width);

    SAMB_data = gsa_MAINSCRN_0_AnimatedLogo;
    dbg_prn("DEBUG: [%s, %d] SAMB_data = gsa_MAINSCRN_0_AnimatedLogo: (0x%04X = 0x%04X) == %s\n", __FILE__, __LINE__, SAMB_data, gsa_MAINSCRN_0_AnimatedLogo, ((SAMB_data == gsa_MAINSCRN_0_AnimatedLogo) ? "TRUE" : "FALSE"));
    dbg_prn("DEBUG: [%s, %d] Width = %d\n", __FILE__, __LINE__,                    farpeekw(SAMB_data, FlicHdr_Width));
    dbg_prn("DEBUG: [%s, %d] Height = %d\n", __FILE__, __LINE__,                   farpeekw(SAMB_data, FlicHdr_Height));
    dbg_prn("DEBUG: [%s, %d] CurrentFrame = %d\n", __FILE__, __LINE__,             farpeekw(SAMB_data, FlicHdr_CurrentFrame));
    dbg_prn("DEBUG: [%s, %d] FrameCount = %d\n", __FILE__, __LINE__,               farpeekw(SAMB_data, FlicHdr_FrameCount));
    dbg_prn("DEBUG: [%s, %d] LoopFrame = %d\n", __FILE__, __LINE__,                farpeekw(SAMB_data, FlicHdr_LoopFrame));
    dbg_prn("DEBUG: [%s, %d] EmmHandleNumber = %d\n", __FILE__, __LINE__,          farpeekb(SAMB_data, FlicHdr_EmmHandleNumber));
    dbg_prn("DEBUG: [%s, %d] EmmLogicalPageIndex = %d\n", __FILE__, __LINE__,      farpeekb(SAMB_data, FlicHdr_EmmLogicalPageIndex));
    dbg_prn("DEBUG: [%s, %d] EmmLogicalPageOffset = 0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_EmmLogicalPageOffset));
    dbg_prn("DEBUG: [%s, %d] PaletteDataOffset = 0x%04X\n", __FILE__, __LINE__,    farpeekw(SAMB_data, FlicHdr_PaletteDataOffset));

    // int SCREEN_Menu(void)
    FLIC_Reset_CurrentFrame(gsa_MAINSCRN_0_AnimatedLogo);

    // SCREEN_Menu() |-> SCREEN_Menu_Draw() |->
    Title_Frame_Index = FLIC_Get_CurrentFrame(gsa_MAINSCRN_0_AnimatedLogo);
    FLIC_Set_CurrentFrame(gsa_MAINSCRN_0_AnimatedLogo, 0);
    DLOG("CALL: VGA_SetModeY();");
    VGA_SetModeY();
    FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);
        // ST_MoveData(destoff=0xB47C, destseg=0x0000, srcoff=0x0000, srcseg=0x20D1, nbytes=16)
        // FLIC_Load_Palette(FlicHdr_SgmtAddr=0x20D1, Frame_Index=0)
        // FLIC_Draw_EMM_C(ScreenPage_X=0, ScreenPage_Y=0, SAMB_data_FLIC_HDR=0x20D1, Frame_Index=0)
    // void FLIC_Load_Palette(SAMB_addr FlicHdr_SgmtAddr, int Frame_Index)

    DLOG("CALL: VGA_SetTextMode();");
    VGA_SetTextMode();
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_EMM_Load_LBX_File()\n", __FILE__, __LINE__);
#endif
}

void test_GAME_LoadMainImages(void)
{
    int itr_EMM_Table_Index;
    WORD EMM_Handle_Number;
    BYTE EMM_Physical_Page_Number;
    WORD EMM_Logical_Page_Number;
    WORD LBX_EntryCount;
    WORD LBX_MagSig_Hi;
    WORD LBX_MagSig_Lo;
    WORD LBX_Type;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_GAME_LoadMainImages()\n", __FILE__, __LINE__);
#endif

    EMM_Startup();
    GAME_LoadMainImages();
    validate_GAME_LoadMainImages();

    /*
        Test what, where, how?
            ?
            EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);
            ?
            gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
            ?
            Did MAINSCRN.LBX load into EMM properly?
            ? 

    */
    // TST_LBX_MAINSCRN_000.LBX_File_Name: MAINSCRN.LBX
    // TST_LBX_MAINSCRN_000.LBX_Name: MAINSCRN
    // TST_LBX_MAINSCRN_000.EMM_Table_Index: 0
    // TST_LBX_MAINSCRN_000.LBX_File_Size_B: 196511
    // TST_LBX_MAINSCRN_000.LBX_File_Size_PR: 0
    // TST_LBX_MAINSCRN_000.LBX_File_Size_PG: 12
    // TST_LBX_MAINSCRN_000.EMM_Logical_Page_Count: 11
    // TST_LBX_MAINSCRN_000.EMM_Handle_Number: 4
    dbg_prn("DEBUG: [%s, %d] g_EMM_Open_Handles: %u \n", __FILE__, __LINE__, g_EMM_Open_Handles);
    dbg_prn("DEBUG: [%s, %d] g_EMM_Pages_Reserved: %d \n", __FILE__, __LINE__, EMM_Pages_Reserved);
    // same logic as in EMM_CheckHandleOpen()
    for ( itr_EMM_Table_Index = 0; itr_EMM_Table_Index < g_EMM_Open_Handles; itr_EMM_Table_Index++ )
    {
        if ( stricmp(EMM_Table[itr_EMM_Table_Index].eEmmHndlNm, TST_LBX_MAINSCRN_000.LBX_Name) == 0 )
        {
            dbg_prn("DEBUG: [%s, %d] EMM_Table[%d].eEmmHndlNm: %s \n", __FILE__, __LINE__, itr_EMM_Table_Index, EMM_Table[itr_EMM_Table_Index].eEmmHndlNm);
            dbg_prn("DEBUG: [%s, %d] EMM_Table[%d].eEmmHndlNbr: %u \n", __FILE__, __LINE__, itr_EMM_Table_Index, EMM_Table[itr_EMM_Table_Index].eEmmHndlNbr);
            dbg_prn("DEBUG: [%s, %d] EMM_Table[%d].eEmmRsrvd: %d \n", __FILE__, __LINE__, itr_EMM_Table_Index, EMM_Table[itr_EMM_Table_Index].eEmmRsrvd);
        }
    }
    //TST_LBX_MAINSCRN_000.EMM_Table_Index
    //EMM_Table[itr_EMM_Table_Index].eEmmHndlNbr

    // s12p10  EMM_GetPageFrame()
    // s12p08  EMM_MapnRead()  s12p11  EMM_Map4()  s12p12  EMM_MapMulti4()
    // EMM_MAP_PAGE(_epp_,_ehn_,_elp_)
    EMM_Handle_Number = EMM_Table[TST_LBX_MAINSCRN_000.EMM_Table_Index].eEmmHndlNbr;
    EMM_Physical_Page_Number = 0;
    EMM_Logical_Page_Number = 0;
    // EMM_MAP_PAGE(0,4,0);
    // EMM_MAP_PAGE(EMM_Physical_Page_Number, EMM_Handle_Number, EMM_Logical_Page_Number);
    EMM_Map4(EMM_Handle_Number, EMM_Logical_Page_Number);
    dbg_prn("DEBUG: [%s, %d] EMM_PageFrameBaseAddress: 0x%04X\n", __FILE__, __LINE__, EMM_PageFrameBaseAddress);
    // dbg_prn("DEBUG: [%s, %d] LBX_EntryCount: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 0));
    // dbg_prn("DEBUG: [%s, %d] LBX_MagSig_Hi: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 2));
    // dbg_prn("DEBUG: [%s, %d] LBX_MagSig_Lo: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 4));
    // dbg_prn("DEBUG: [%s, %d] LBX_Type: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 6));
    LBX_EntryCount = (WORD)(EMM_PageFrameBaseAddress + 0);
    LBX_MagSig_Hi  = (WORD)(EMM_PageFrameBaseAddress + 2);
    LBX_MagSig_Lo  = (WORD)(EMM_PageFrameBaseAddress + 4);
    LBX_Type       = (WORD)(EMM_PageFrameBaseAddress + 6);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_GAME_LoadMainImages()\n", __FILE__, __LINE__);
#endif
}


/*  s21p07  void FLIC_Load_Palette(SAMB_addr sa_FLIC_Header, int Frame_Index);  */
void test_FLIC_Load_Palette(void)
{
    int test_status;
    //  void FLIC_Draw_XY(int Left, int Top, SAMB_addr sa_FLIC_Header)
    //      static struct s_FLIC_HDR FLIC_Header;
    //      int Frame_Index;
    SAMB_addr sa_FLIC_Header;
    static struct s_FLIC_HDR PS_FLIC_Header;
    int Frame_Index;
    struct s_FLIC_HDR _FAR * pPS_FLIC_Header;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_FLIC_Load_Palette()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED

    // TST_LBX_MAINSCRN_000.Segment_Address = gsa_MAINSCRN_0_AnimatedLogo;
    tst_prn("TEST: [%s, %d] TST_LBX_MAINSCRN_000.Segment_Address: 0x%04X\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.Segment_Address);
    sa_FLIC_Header = TST_LBX_MAINSCRN_000.Segment_Address;
    tst_prn("TEST: [%s, %d] sa_FLIC_Header: 0x%04X\n", __FILE__, __LINE__, sa_FLIC_Header);

    if(!validate_FLIC_Header(sa_FLIC_Header)) { test_status = -1; }  // TEST_FAILURE

    if(!g_Load_Font_File_tested) { test_Load_Font_File(); }
    if(!g_Load_Font_File_validated) { test_status = -1; }  // TEST_FAILURE

    // FLIC_Draw_XY()
    // int ST_MoveData(unsigned int destoff, unsigned int destseg, unsigned int srcoff, unsigned int srcseg, unsigned int nbytes);
    ST_MoveData((unsigned int)&PS_FLIC_Header, 0, 0, sa_FLIC_Header, sizeof(PS_FLIC_Header));
    //  ?  memcpy(); memmove() |-> movmem();  movedata();  ?

    // FLIC_Draw_XY()
    Frame_Index = PS_FLIC_Header.Current_Frame;

    // FLIC_Draw_XY()
    if ( PS_FLIC_Header.Palette_Header_Offset != 0 )
    {
        TLOG("CALL: FLIC_Load_Palette(sa_FLIC_Header, Frame_Index);");
        // FLIC_Load_Palette(sa_FLIC_Header, Frame_Index);  // s21p07
        // FLIC_Load_Palette_FP_EMM(sa_FLIC_Header, Frame_Index);
        FLIC_Load_Palette(MK_FP(sa_FLIC_Header,0), Frame_Index);  // s21p07
        if(!validate_PaletteFlags_M00()) { test_status = -1; }  // TEST_FAILURE
        if(!validate_Palette_M00()) { test_status = -1; }  // TEST_FAILURE
    }

    g_FLIC_Load_Palette_tested = 1;
    // if ( validate_FLIC_Load_Palette() ) { g_FLIC_Load_Palette_validated = 1; }
    if ( test_status != -1 ) { g_FLIC_Load_Palette_validated = 1; }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_FLIC_Load_Palette()\n", __FILE__, __LINE__);
#endif
}

/*  s27p03  void FLIC_Draw_EMM_C(int ScreenPage_X, int ScreenPage_Y, SAMB_addr SAMB_data_FLIC_HDR, int Frame_Index)  */
void test_FLIC_Draw_EMM(void)
{
    int test_status;
    //  void FLIC_Draw_XY(int Left, int Top, SAMB_addr sa_FLIC_Header)
    //      static struct s_FLIC_HDR FLIC_Header;
    //      int Frame_Index;
    int Left;
    int Top;
    SAMB_addr sa_FLIC_Header;
    static struct s_FLIC_HDR PS_FLIC_Header;
    int Frame_Index;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_FLIC_Draw_EMM()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED

    // TST_LBX_MAINSCRN_000.Segment_Address = gsa_MAINSCRN_0_AnimatedLogo;
    tst_prn("TEST: [%s, %d] TST_LBX_MAINSCRN_000.Segment_Address: 0x%04X\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.Segment_Address);
    sa_FLIC_Header = TST_LBX_MAINSCRN_000.Segment_Address;
    tst_prn("TEST: [%s, %d] sa_FLIC_Header: 0x%04X\n", __FILE__, __LINE__, sa_FLIC_Header);

    if(!validate_FLIC_Header(sa_FLIC_Header)) { test_status = -1; }  // TEST_FAILURE

    if(!g_Load_Font_File_tested) { test_Load_Font_File(); }
    if(!g_Load_Font_File_validated) { test_status = -1; }  // TEST_FAILURE

    // FLIC_Draw_XY()
    // int ST_MoveData(unsigned int destoff, unsigned int destseg, unsigned int srcoff, unsigned int srcseg, unsigned int nbytes);
    ST_MoveData((unsigned int)&PS_FLIC_Header, 0, 0, sa_FLIC_Header, sizeof(PS_FLIC_Header));
    //  ?  memcpy(); memmove() |-> movmem();  movedata();  ?

    // FLIC_Draw_XY()
    // Frame_Index = PS_FLIC_Header.Current_Frame;
    Frame_Index = 0;

    // main() |-> GAME_MainMenu() |-> SCREEN_Menu() |-> SCREEN_Menu_Draw() |-> FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);
    // FLIC_Draw_EMM_C(Left, Top, sa_FLIC_Header, Frame_Index);
    Left = 0;
    Top = 0;
    // FLIC_Draw_EMM_C(Left, Top, sa_FLIC_Header, Frame_Index);
    // 
    // // no-workie DBG_ScreenDump();
    // 
    // // // STU_VGA_DAC_Dump("MENUDAC.BIN");  // STU_VGA.C/.H
    // // // // VGA_Set_DSP_Addr();
    // // // VGA_PageFlip();
    // // VGA_DAC_Write();

    VGA_SetModeY();
    // g_RSP_Idx = 0;
    // gsa_DSP_Addr = 0xA000;
    VGA_DrawFilledRect(0,0,319,199,5);
    getch();
    FLIC_Draw_EMM_C(Left, Top, sa_FLIC_Header, Frame_Index);
    // VGA_PageFlip();
    getch();
    VGA_DAC_Write();
    getch();
    FLIC_Load_Palette(MK_FP(sa_FLIC_Header,0), Frame_Index);  // s21p07
    getch();
    VGA_DAC_Write();
    getch();
    // VGA_PageFlip();
    // getch();
    // BAD!! STU_VGA_RAM_Dump_1();
    STU_VGA_RAM_Dump();

    VGA_SetTextMode();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_FLIC_Draw_EMM()\n", __FILE__, __LINE__);
#endif
}

/*
    Test
        FLIC_Draw_XY()
*/
/*  s29p11  void FLIC_Draw_XY(int Left, int Top, SAMB_addr sa_FLIC_Header)  */
void test_FLIC_Draw_XY(void)
{
    //  void FLIC_Draw_XY(int Left, int Top, SAMB_addr sa_FLIC_Header)
    //      static struct s_FLIC_HDR FLIC_Header;
    //      int Frame_Index;
    int Left;
    int Top;
    SAMB_addr sa_FLIC_Header;
    static struct s_FLIC_HDR PS_FLIC_Header;
    int Frame_Index;
    struct s_FLIC_HDR _FAR * pPS_FLIC_Header;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_FLIC_Draw_XY()\n", __FILE__, __LINE__);
#endif

    // GAME_LoadMainImages()
    // gsa_MAINSCRN_0_AnimatedLogo  = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
    // gsa_MAINSCRN_5_ScreenBottom  = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 5);
    // gsa_VORTEX_1_MenuContinue    = LBXE_LoadSingle(g_LbxNm_VORTEX, 1);
    // gsa_VORTEX_2_MenuHallOfFame  = LBXE_LoadSingle(g_LbxNm_VORTEX, 2);
    // gsa_VORTEX_3_MenuQuitToDOS   = LBXE_LoadSingle(g_LbxNm_VORTEX, 3);
    // gsa_VORTEX_4_MenuNewGame     = LBXE_LoadSingle(g_LbxNm_VORTEX, 4);
    // gsa_VORTEX_5_MenuLoadGame    = LBXE_LoadSingle(g_LbxNm_VORTEX, 5);

    // TST_LBX_MAINSCRN_000.Segment_Address = gsa_MAINSCRN_0_AnimatedLogo;
    tst_prn("TEST: [%s, %d] TST_LBX_MAINSCRN_000.Segment_Address: 0x%04X\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.Segment_Address);
    sa_FLIC_Header = TST_LBX_MAINSCRN_000.Segment_Address;
    tst_prn("TEST: [%s, %d] sa_FLIC_Header: 0x%04X\n", __FILE__, __LINE__, sa_FLIC_Header);
    validate_FLIC_Header(sa_FLIC_Header);

    test_Load_Font_File();

    // FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);
    // e0s0 FLIC_Draw_A(Left, Top, PS_FLIC_Header.Width, FLIC_Frame_Ofst, FLIC_Frame_Sgmt);

    // FLIC_Draw_XY(32, 20, gsa_VORTEX_3_MenuQuitToDOS);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_FLIC_Draw_XY()\n", __FILE__, __LINE__);
#endif
}

// s08p19  void SA_Alloc_Error(int SA_Error_Number, int blocks);
void test_SA_Error(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_SA_Error()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] Debug_Disabled: %d\n", __FILE__, __LINE__, Debug_Disabled);
#endif
    Debug_Disabled = 0;
    SA_Alloc_Error(1,111);
    Debug_Disabled = 1;
    SA_Alloc_Error(1,111);
    SA_Alloc_Error(2,111);
    SA_Alloc_Error(3,111);
    SA_Alloc_Error(4,111);
    // DNE SA_Alloc_Error(5,0);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_SA_Error()\n", __FILE__, __LINE__);
#endif
}
// s10p15  void LBX_Error(char * name, int LBX_Error_Number, int entry, int pages)
void test_LBX_Error(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_LBX_Error()\n", __FILE__, __LINE__);
#endif
    LBX_Error("MAINSCRN",  1, 0, 111);
    LBX_Error("MAINSCRN",  2, 0, 111);
    LBX_Error("MAINSCRN",  3, 0, 111);
    LBX_Error("MAINSCRN",  4, 0, 111);
    LBX_Error("MAINSCRN",  5, 0, 111);
    LBX_Error("MAINSCRN",  6, 0, 111);
    LBX_Error("MAINSCRN",  7, 0, 111);
    LBX_Error("MAINSCRN",  8, 0, 111);
    LBX_Error("MAINSCRN",  9, 0, 111);
    LBX_Error("MAINSCRN", 10, 0, 111);
    LBX_Error("MAINSCRN", 11, 0, 111);
    LBX_Error("MAINSCRN", 12, 0, 111);
    LBX_Error("MAINSCRN", 13, 0, 111);
    LBX_Error("MAINSCRN", 14, 0, 111);
    LBX_Error("MAINSCRN", 15, 0, 111);
    LBX_Error("MAINSCRN", 16, 0, 111);
    // DNE LBX_Error("MAINSCRN", 17, 0, 111);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_LBX_Error()\n", __FILE__, __LINE__);
#endif
}
