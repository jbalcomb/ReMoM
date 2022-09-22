
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

#include <ASSERT.H>  // NDEBUG; assert();
#include <STDLIB.H>  // abort();
#include <STRING.H>

#include "ST_TYPE.H"

#include "MGC_DEF.H"

#include "ST_EMM.H"
#include "ST_FLIC.H"
#include "ST_LBX.H"
#include "ST_VGA.H"

#include "STU_DBG.H"
#include "STU_TST.H"


void test_Load_MAINSCRN_000(void);

void test_VGA_SetDirectDraw(void);
void test_VGA_Set_DSP_Addr(void);

void test_VGA_DAC_Init(void);

void test_EMM_Init(void);
void test_EMM_Startup(void);
void test_EMM_Load_LBX_File(void);
void test_GAME_LoadMainImages(void);

void test_FLIC_Draw_XY(void);

void test_MGC_Main(void);



int main(void)
{

    Debug_Log_Startup();

    // test_VGA_SetDirectDraw();
    // test_VGA_Set_DSP_Addr();

    // test_VGA_DAC_Init();

    // test_EMM_Init();
    // test_EMM_Startup();
    // test_EMM_Load_LBX_File();

    test_GAME_LoadMainImages();

    // test_Load_MAINSCRN_000();

    Debug_Log_Shutdown();
    return 0;
}

void test_MGC_Main(void)
{
    // MGC main()
    g_EMM_Pages_Reserved = EMM_PAGES_REQUIRED;
    // MGC main()
    EMM_SetMinKB(EMM_MIN_KB);
    // MGC main()
    RAM_SetMinKB(RAM_MIN_KB);
    // MGC main() |-> Hardware_Init()
    EMM_Startup();
    // MGC main() |-> Hardware_Init()
    // s14p03
    VGA_DAC_Init(GAME_FONT_FILE);  // "FONTS.LBX"
        // |-> ... LBXE_LoadSingle(FONTS.LBX,0), SA_Allocate_Space(), VGA_TextDraw_Init()
    // MGC main() |-> Hardware_Init()
    IN_Init(1);  // INPUT_TYPE_KEYBOARD_AND_MOUSE
        // |-> MD_Init()
    // MGC main() |-> Hardware_Init()
    VGA_Set_DSP_Addr();
    // MGC main()
    MoM_Tables_Init(6100);
    // MGC main()
    // s20p01
    VGA_LoadPalette(0, -1, 0);
        // 
    // MGC main()
    VGA_DAC_Write();
    // MGC main()
    GAME_LoadMainImages();
    // MGC main()
    GAME_Load_TERRSTAT_0();
    // MGC main()
    GAME_Load_SPELLDAT_0();
    // MGC main()
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Draw Screen-Page
    // MGC main()
    VGA_SetDirectDraw();
    // MGC main()
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Render Screen-Page
    // MGC main()
    VGA_Set_DSP_Addr();
    // MGC main()
    VGA_LoadPalette(0, -1, 0);  // EMPERATO
    // MGC main()
    VGA_DAC_Write();
    // MGC main()
    // GAME_MainMenu();  // MGC_DEF.H  _s01p03c.c

}

void test_VGA_SetDirectDraw(void)
{

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: test_VGA_SetDirectDraw()\n", __FILE__, __LINE__);
#endif

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dlvfprintf("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dlvfprintf("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    g_RSP_Idx = 0;
    VGA_SetDirectDraw();

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dlvfprintf("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dlvfprintf("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    g_RSP_Idx = 1;
    VGA_SetDirectDraw();

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dlvfprintf("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dlvfprintf("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    if ( g_RSP_Idx == 0 )
    {
        if ( gsa_DSP_Addr != 0xA000 )
        {
            dlvfprintf("TEST: [%s, %d] FAILURE: ( (g_RSP_Idx == 0 ) && (gsa_DSP_Addr != 0xA000) )\n", __FILE__, __LINE__);
            dlvfprintf("TEST: [%s, %d] FAILURE: g_RSP_Idx: %d; gsa_DSP_Addr: 0x%04X;\n", __FILE__, __LINE__, g_RSP_Idx, gsa_DSP_Addr);
            exit(1);
        }
        else
        {
            dlvfprintf("TEST: [%s, %d] SUCCESS: ( (g_RSP_Idx == 0 ) && (gsa_DSP_Addr == 0xA000) )\n", __FILE__, __LINE__);
        }
    }
    else
    {
        dlvfprintf("TEST: [%s, %d] SKIP: g_RSP_Idx != 0\n", __FILE__, __LINE__);
    }

    if ( g_RSP_Idx == 1 )
    {
        if ( gsa_DSP_Addr != 0xA400 )
        {
            dlvfprintf("DEBUG: [%s, %d] FAILURE: ( (g_RSP_Idx == 1 ) && (gsa_DSP_Addr != 0xA400) )\n", __FILE__, __LINE__);
            dlvfprintf("DEBUG: [%s, %d] FAILURE: g_RSP_Idx: %d; gsa_DSP_Addr: 0x%04X;\n", __FILE__, __LINE__, g_RSP_Idx, gsa_DSP_Addr);
            exit(1);
        }
        else
        {
            dlvfprintf("DEBUG: [%s, %d] SUCCESS: ( (g_RSP_Idx == 1 ) && (gsa_DSP_Addr == 0xA400) )\n", __FILE__, __LINE__);
        }
    }
    else
    {
        dlvfprintf("TEST: [%s, %d] SKIP: g_RSP_Idx != 1\n", __FILE__, __LINE__);
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: test_VGA_SetDirectDraw()\n", __FILE__, __LINE__);
#endif

}

/*
unsigned int gsa_MAINSCRN_0_AnimatedLogo;   // dseg:52E8
gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
*/
void test_Load_MAINSCRN_000(void)
{
    SAMB_data sa_MAINSCRN_000;

    // MGC main() and Hardware_Init()
    g_EMM_Pages_Reserved = EMM_PAGES_REQUIRED;
    EMM_SetMinKB(EMM_MIN_KB);
    RAM_SetMinKB(RAM_MIN_KB);
    EMM_Startup();
    validate_EMM_Startup();

    // GAME_LoadMainImages
    EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);
    sa_MAINSCRN_000 = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
    TST_LBX_MAINSCRN_000.Segment_Address = sa_MAINSCRN_000;

    validate_FLIC_Header(sa_MAINSCRN_000);

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

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: test_VGA_Set_DSP_Addr()\n", __FILE__, __LINE__);
#endif

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dlvfprintf("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dlvfprintf("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    g_RSP_Idx = 0;
    VGA_Set_DSP_Addr();

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dlvfprintf("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dlvfprintf("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    g_RSP_Idx = 1;
    VGA_Set_DSP_Addr();

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] VRAM_BASE: 0x%04X\n", __FILE__, __LINE__, VRAM_BASE);
    dlvfprintf("DEBUG: [%s, %d] g_RSP_Idx: %d\n", __FILE__, __LINE__, g_RSP_Idx);
    dlvfprintf("DEBUG: [%s, %d] gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, gsa_DSP_Addr);
#endif

    if ( g_RSP_Idx == 0 )
    {
        if ( gsa_DSP_Addr != 0xA400 )
        {
            dlvfprintf("TEST: [%s, %d] FAILURE: ( (g_RSP_Idx == 0 ) && (gsa_DSP_Addr != 0xA400) )\n", __FILE__, __LINE__);
            dlvfprintf("TEST: [%s, %d] FAILURE: g_RSP_Idx: %d; gsa_DSP_Addr: 0x%04X;\n", __FILE__, __LINE__, g_RSP_Idx, gsa_DSP_Addr);
            exit(1);
        }
        else
        {
            dlvfprintf("TEST: [%s, %d] SUCCESS: ( (g_RSP_Idx == 0 ) && (gsa_DSP_Addr == 0xA400) )\n", __FILE__, __LINE__);
        }
    }
    else
    {
        dlvfprintf("TEST: [%s, %d] SKIP: g_RSP_Idx != 0\n", __FILE__, __LINE__);
    }

    if ( g_RSP_Idx == 1 )
    {
        if ( gsa_DSP_Addr != 0xA000 )
        {
            dlvfprintf("DEBUG: [%s, %d] FAILURE: ( (g_RSP_Idx == 1 ) && (gsa_DSP_Addr != 0xA000) )\n", __FILE__, __LINE__);
            dlvfprintf("DEBUG: [%s, %d] FAILURE: g_RSP_Idx: %d; gsa_DSP_Addr: 0x%04X;\n", __FILE__, __LINE__, g_RSP_Idx, gsa_DSP_Addr);
            exit(1);
        }
        else
        {
            dlvfprintf("DEBUG: [%s, %d] SUCCESS: ( (g_RSP_Idx == 1 ) && (gsa_DSP_Addr == 0xA000) )\n", __FILE__, __LINE__);
        }
    }
    else
    {
        dlvfprintf("TEST: [%s, %d] SKIP: g_RSP_Idx != 1\n", __FILE__, __LINE__);
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: test_VGA_Set_DSP_Addr()\n", __FILE__, __LINE__);
#endif

}

/*
    MGC main()
        |-> Hardware_Init(1, 2, GAME_FONT_FILE, 0, 0, 0, 0, 0, 0, 0, 0);  // Defaults for 'No Sound'
            |-> VGA_DAC_Init()
    
    VGA_DAC_Init()
        |-> ... LBXE_LoadSingle(), SA_Allocate_Space(), VGA_TextDraw_Init()
    
*/
void test_VGA_DAC_Init(void)
{
    int test_status;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: test_VGA_DAC_Init()\n", __FILE__, __LINE__);
#endif

    test_status = 0;  // TEST_UNDEFINED

    dlvfprintf("DEBUG: [%s, %d] g_PaletteLbxFileName: %s\n", __FILE__, __LINE__, GAME_FONT_FILE);

    dlvfprintf("DEBUG: [%s, %d] g_PaletteLbxFileName: %s\n", __FILE__, __LINE__, g_PaletteLbxFileName);
    dlvfprintf("DEBUG: [%s, %d] gsa_FontStyleData: 0x%04X\n", __FILE__, __LINE__, gsa_FontStyleData);
    dlvfprintf("DEBUG: [%s, %d] gsa_BorderStyleData: 0x%04X\n", __FILE__, __LINE__, gsa_BorderStyleData);
    dlvfprintf("DEBUG: [%s, %d] gsa_PaletteLbxEntry: 0x%04X\n", __FILE__, __LINE__, gsa_PaletteLbxEntry);
    dlvfprintf("DEBUG: [%s, %d] gsa_Palette: 0x%04X\n", __FILE__, __LINE__, gsa_Palette);
    dlvfprintf("DEBUG: [%s, %d] gsa_PaletteFlags: 0x%04X\n", __FILE__, __LINE__, gsa_PaletteFlags);
    dlvfprintf("DEBUG: [%s, %d] gsa_PaletteSaved: 0x%04X\n", __FILE__, __LINE__, gsa_PaletteSaved);
    dlvfprintf("DEBUG: [%s, %d] gsa_ReplacementColors: 0x%04X\n", __FILE__, __LINE__, gsa_ReplacementColors);
    dlvfprintf("DEBUG: [%s, %d] gsa_VGAFILEH_Header: 0x%04X\n", __FILE__, __LINE__, gsa_VGAFILEH_Header);
    dlvfprintf("DEBUG: [%s, %d] gsa_IntensityScaleTable: 0x%04X\n", __FILE__, __LINE__, gsa_IntensityScaleTable);

    dlvfprintf("DEBUG: [%s, %d] UU_g_VGA_TextDraw_Initd: %u\n", __FILE__, __LINE__, UU_g_VGA_TextDraw_Initd);
    dlvfprintf("DEBUG: [%s, %d] gfp_VGA_TextLine_Lefts: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Lefts);
    dlvfprintf("DEBUG: [%s, %d] gfp_VGA_TextLine_Rights: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Rights);
    dlvfprintf("DEBUG: [%s, %d] gfp_VGA_TextLine_Tops: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Tops);
    dlvfprintf("DEBUG: [%s, %d] gfp_VGA_TextLine_Starts: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Starts);


    VGA_DAC_Init(GAME_FONT_FILE);


    dlvfprintf("DEBUG: [%s, %d] g_PaletteLbxFileName: %s\n", __FILE__, __LINE__, g_PaletteLbxFileName);
    dlvfprintf("DEBUG: [%s, %d] gsa_FontStyleData: 0x%04X\n", __FILE__, __LINE__, gsa_FontStyleData);
    dlvfprintf("DEBUG: [%s, %d] gsa_BorderStyleData: 0x%04X\n", __FILE__, __LINE__, gsa_BorderStyleData);
    dlvfprintf("DEBUG: [%s, %d] gsa_PaletteLbxEntry: 0x%04X\n", __FILE__, __LINE__, gsa_PaletteLbxEntry);
    dlvfprintf("DEBUG: [%s, %d] gsa_Palette: 0x%04X\n", __FILE__, __LINE__, gsa_Palette);
    dlvfprintf("DEBUG: [%s, %d] gsa_PaletteFlags: 0x%04X\n", __FILE__, __LINE__, gsa_PaletteFlags);
    dlvfprintf("DEBUG: [%s, %d] gsa_PaletteSaved: 0x%04X\n", __FILE__, __LINE__, gsa_PaletteSaved);
    dlvfprintf("DEBUG: [%s, %d] gsa_ReplacementColors: 0x%04X\n", __FILE__, __LINE__, gsa_ReplacementColors);
    dlvfprintf("DEBUG: [%s, %d] gsa_VGAFILEH_Header: 0x%04X\n", __FILE__, __LINE__, gsa_VGAFILEH_Header);
    dlvfprintf("DEBUG: [%s, %d] gsa_IntensityScaleTable: 0x%04X\n", __FILE__, __LINE__, gsa_IntensityScaleTable);

    dlvfprintf("DEBUG: [%s, %d] UU_g_VGA_TextDraw_Initd: %u\n", __FILE__, __LINE__, UU_g_VGA_TextDraw_Initd);
    dlvfprintf("DEBUG: [%s, %d] gfp_VGA_TextLine_Lefts: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Lefts);
    dlvfprintf("DEBUG: [%s, %d] gfp_VGA_TextLine_Rights: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Rights);
    dlvfprintf("DEBUG: [%s, %d] gfp_VGA_TextLine_Tops: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Tops);
    dlvfprintf("DEBUG: [%s, %d] gfp_VGA_TextLine_Starts: %p\n", __FILE__, __LINE__, gfp_VGA_TextLine_Starts);

    // ASSERT()
    if ( strcmpi(g_PaletteLbxFileName, "FONTS.LBX") != 0 )
    {
        dlvfprintf("DEBUG: [%s, %d] FAILURE: ( strcmpi(g_PaletteLbxFileName, \"FONTS.LBX\") != 0 )\n", __FILE__, __LINE__);
        dlvfprintf("DEBUG: [%s, %d] FAILURE: g_PaletteLbxFileName: %s;\n", __FILE__, __LINE__, g_PaletteLbxFileName);
        exit(1);
    }
    else
    {
        dlvfprintf("DEBUG: [%s, %d] SUCCESS: ( strcmpi(g_PaletteLbxFileName, \"FONTS.LBX\") == 0 )\n", __FILE__, __LINE__);
    }

    // validate_Palette_0();
    test_status = validate_PaletteFlags_1();
    tst_prn("TEST: [%s, %d] test_status: %d\n", __FILE__, __LINE__, test_status);
    // TSTPRN(("TEST: [%s, %d] test_status: %d\n", __FILE__, __LINE__, test_status));  // WTF!?! "Warning: Condition is always true in function..."
    if ( test_status == -1 )  // TEST_FAILURE
    {
        dlvfprintf("DEBUG: [%s, %d] FAILURE: validate_PaletteFlags_1()\n", __FILE__, __LINE__);
        exit(1);
    }


#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: test_VGA_DAC_Init()\n", __FILE__, __LINE__);
#endif
}


void test_EMM_Init(void)
{
    int result;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: test_EMM_Init()\n", __FILE__, __LINE__);
#endif

    result = EMM_Init();

    if ( result == ST_SUCCESS )
    {
        dlvfprintf("DEBUG: [%s, %d] SUCCESS: \n", __FILE__, __LINE__);
    }
    if ( result == ST_FAILURE )
    {
        dlvfprintf("DEBUG: [%s, %d] FAILURE: \n", __FILE__, __LINE__);
    }
    if ( ( result != ST_SUCCESS ) && ( result != ST_FAILURE ) )
    {
        dlvfprintf("DEBUG: [%s, %d] INVALID: \n", __FILE__, __LINE__);
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: test_EMM_Init()\n", __FILE__, __LINE__);
#endif
}

void test_EMM_Startup(void)
{
    int itr_open_handles;
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: test_EMM_Startup()\n", __FILE__, __LINE__);
#endif

    EMM_Startup();

    if (
        ( g_EMM_OK == ST_TRUE) &&
        ( g_EmmHndlNbr_YOMOMA != 0 ) &&
        ( g_EmmHndlNbr_VGAFILEH != 0 ) &&
        ( g_EmmHndlNbr_EMMDATAH != 0 ) &&
        ( g_EMM_Open_Handles == 3 ) &&
        ( g_EMM_Pages_Reserved == 31 )
    )
    {
        dlvfprintf("DEBUG: [%s, %d] SUCCESS: \n", __FILE__, __LINE__);
    }
    if ( g_EMM_OK == ST_FALSE )
    {
        dlvfprintf("DEBUG: [%s, %d] FAILURE: \n", __FILE__, __LINE__);
    }
    if ( ( g_EMM_OK != ST_TRUE ) && ( g_EMM_OK != ST_FALSE ) )
    {
        dlvfprintf("DEBUG: [%s, %d] INVALID: \n", __FILE__, __LINE__);
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] g_EMM_OK: %d\n", __FILE__, __LINE__, g_EMM_OK);
    dlvfprintf("DEBUG: [%s, %d] g_EmmHndlNbr_YOMOMA: %d\n", __FILE__, __LINE__, g_EmmHndlNbr_YOMOMA);
    dlvfprintf("DEBUG: [%s, %d] g_EmmHndlNbr_VGAFILEH: %d\n", __FILE__, __LINE__, g_EmmHndlNbr_VGAFILEH);
    dlvfprintf("DEBUG: [%s, %d] g_EmmHndlNbr_EMMDATAH: %d\n", __FILE__, __LINE__, g_EmmHndlNbr_EMMDATAH);
    //dlvfprintf("DEBUG: [%s, %d] g_EMMDATAH_Level: %d\n", __FILE__, __LINE__, g_EMMDATAH_Level);  // ? NIU ?
    dlvfprintf("DEBUG: [%s, %d] g_EMM_Pages_Reserved: %d\n", __FILE__, __LINE__, g_EMM_Pages_Reserved);
    dlvfprintf("DEBUG: [%s, %d] g_EMM_Open_Handles: %d\n", __FILE__, __LINE__, g_EMM_Open_Handles);
    dlvfprintf("DEBUG: [%s, %d] EMM_GetFreePageCount(): %u\n", __FILE__, __LINE__, EMM_GetFreePageCount());
    dlvfprintf("DEBUG: [%s, %d] EMM_GetActiveHandleCount(): %u\n", __FILE__, __LINE__, EMM_GetActiveHandleCount());
    for ( itr_open_handles = 0; itr_open_handles < g_EMM_Open_Handles; itr_open_handles++ )
    {
        dlvfprintf("DEBUG: [%s, %d] g_EMM_Table[%d].eEmmHndlNm: %s\n", __FILE__, __LINE__, itr_open_handles, g_EMM_Table[itr_open_handles].eEmmHndlNm);
        dlvfprintf("DEBUG: [%s, %d] g_EMM_Table[%d].eEmmRsrvd: %d\n", __FILE__, __LINE__, itr_open_handles, g_EMM_Table[itr_open_handles].eEmmRsrvd);
        dlvfprintf("DEBUG: [%s, %d] g_EMM_Table[%d].eEmmHndlNbr: %u\n", __FILE__, __LINE__, itr_open_handles, g_EMM_Table[itr_open_handles].eEmmHndlNbr);
        dlvfprintf("DEBUG: [%s, %d] EMM_GetHandlePageCount(%d): %u\n", __FILE__, __LINE__, g_EMM_Table[itr_open_handles].eEmmHndlNbr, EMM_GetHandlePageCount(g_EMM_Table[itr_open_handles].eEmmHndlNbr));
    }
#endif

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: test_EMM_Startup()\n", __FILE__, __LINE__);
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
    struct s_FLIC_HDR * pFlicHeader; // _s21p07c.c  FLIC_LoadPalette()
    SAMB_addr SAMB_data;
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: test_EMM_Load_LBX_File()\n", __FILE__, __LINE__);
#endif

    // main() |-> Hardware_Init() |->
    EMM_Startup();

#ifdef TEST
    if ( EMM_PageFrameBaseAddress != 0xE000 )
    {
        dlvfprintf("DEBUG: [%s, %d] FAILURE: EMM_Init()\n", __FILE__, __LINE__);
        dlvfprintf("DEBUG: [%s, %d] EMM_PageFrameBaseAddress: 0x%04X }\n", __FILE__, __LINE__, EMM_PageFrameBaseAddress);
        exit(1);
    }
    if (
        ( g_EMM_OK != ST_TRUE) ||
        ( g_EmmHndlNbr_YOMOMA == 0 ) ||
        ( g_EmmHndlNbr_VGAFILEH == 0 ) ||
        ( g_EmmHndlNbr_EMMDATAH == 0 ) ||
        ( g_EMM_Open_Handles != 3 ) ||
        ( g_EMM_Pages_Reserved != 31 )
    )
    {
        dlvfprintf("DEBUG: [%s, %d] FAILURE: EMM_Startup()\n", __FILE__, __LINE__);
        dlvfprintf("DEBUG: [%s, %d] g_EMM_OK: %d\n", __FILE__, __LINE__, g_EMM_OK);
        dlvfprintf("DEBUG: [%s, %d] g_EmmHndlNbr_YOMOMA: %d\n", __FILE__, __LINE__, g_EmmHndlNbr_YOMOMA);
        dlvfprintf("DEBUG: [%s, %d] g_EmmHndlNbr_VGAFILEH: %d\n", __FILE__, __LINE__, g_EmmHndlNbr_VGAFILEH);
        dlvfprintf("DEBUG: [%s, %d] g_EmmHndlNbr_EMMDATAH: %d\n", __FILE__, __LINE__, g_EmmHndlNbr_EMMDATAH);
        dlvfprintf("DEBUG: [%s, %d] g_EMM_Open_Handles: %d\n", __FILE__, __LINE__, g_EMM_Open_Handles);
        dlvfprintf("DEBUG: [%s, %d] g_EMM_Pages_Reserved: %d\n", __FILE__, __LINE__, g_EMM_Pages_Reserved);
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

    DBG_MAINSCRN_000 = gsa_MAINSCRN_0_AnimatedLogo;
    TST_LBX_MAINSCRN_000.Segment_Address = gsa_MAINSCRN_0_AnimatedLogo;
    dlvfprintf("DEBUG: [%s, %d] DBG_MAINSCRN_000: 0x%04X\n", __FILE__, __LINE__, DBG_MAINSCRN_000);
    dlvfprintf("DEBUG: [%s, %d] MAINSCRN_000.Segment_Address: 0x%04X\n", __FILE__, __LINE__, TST_LBX_MAINSCRN_000.Segment_Address);


    pFlicHeader = (struct s_FLIC_HDR *)MK_FP(gsa_MAINSCRN_0_AnimatedLogo, 0);
    //pFlicHeader = (void _FAR *) ( ((unsigned long) (gsa_MAINSCRN_0_AnimatedLogo) << 16) | (0) ) )
    dlvfprintf("DEBUG: [%s, %d] pFlicHeader: %p\n", __FILE__, __LINE__, pFlicHeader);
    
    dlvfprintf("DEBUG: [%s, %d] Width = %d\n", __FILE__, __LINE__, pFlicHeader->Width);

    SAMB_data = gsa_MAINSCRN_0_AnimatedLogo;
    dlvfprintf("DEBUG: [%s, %d] SAMB_data = gsa_MAINSCRN_0_AnimatedLogo: (0x%04X = 0x%04X) == %s\n", __FILE__, __LINE__, SAMB_data, gsa_MAINSCRN_0_AnimatedLogo, ((SAMB_data == gsa_MAINSCRN_0_AnimatedLogo) ? "TRUE" : "FALSE"));
    dlvfprintf("DEBUG: [%s, %d] Width = %d\n", __FILE__, __LINE__,                    farpeekw(SAMB_data, FlicHdr_Width));
    dlvfprintf("DEBUG: [%s, %d] Height = %d\n", __FILE__, __LINE__,                   farpeekw(SAMB_data, FlicHdr_Height));
    dlvfprintf("DEBUG: [%s, %d] CurrentFrame = %d\n", __FILE__, __LINE__,             farpeekw(SAMB_data, FlicHdr_CurrentFrame));
    dlvfprintf("DEBUG: [%s, %d] FrameCount = %d\n", __FILE__, __LINE__,               farpeekw(SAMB_data, FlicHdr_FrameCount));
    dlvfprintf("DEBUG: [%s, %d] LoopFrame = %d\n", __FILE__, __LINE__,                farpeekw(SAMB_data, FlicHdr_LoopFrame));
    dlvfprintf("DEBUG: [%s, %d] EmmHandleNumber = %d\n", __FILE__, __LINE__,          farpeekb(SAMB_data, FlicHdr_EmmHandleNumber));
    dlvfprintf("DEBUG: [%s, %d] EmmLogicalPageIndex = %d\n", __FILE__, __LINE__,      farpeekb(SAMB_data, FlicHdr_EmmLogicalPageIndex));
    dlvfprintf("DEBUG: [%s, %d] EmmLogicalPageOffset = 0x%04X\n", __FILE__, __LINE__, farpeekw(SAMB_data, FlicHdr_EmmLogicalPageOffset));
    dlvfprintf("DEBUG: [%s, %d] PaletteDataOffset = 0x%04X\n", __FILE__, __LINE__,    farpeekw(SAMB_data, FlicHdr_PaletteDataOffset));

    // int SCREEN_Menu(void)
    FLIC_ResetFrame(gsa_MAINSCRN_0_AnimatedLogo);

    // SCREEN_Menu() |-> SCREEN_Menu_Draw() |->
    Title_Frame_Index = FLIC_GetCurFrame(gsa_MAINSCRN_0_AnimatedLogo);
    FLIC_SetFrame(gsa_MAINSCRN_0_AnimatedLogo, 0);
    HERE("CALL: VGA_SetModeY();");
    VGA_SetModeY();
    FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);
        // ST_MoveData(destoff=0xB47C, destseg=0x0000, srcoff=0x0000, srcseg=0x20D1, nbytes=16)
        // FLIC_LoadPalette(FlicHdr_SgmtAddr=0x20D1, Frame_Index=0)
        // FLIC_Draw_EMM_C(ScreenPage_X=0, ScreenPage_Y=0, SAMB_data_FLIC_HDR=0x20D1, Frame_Index=0)
    // void FLIC_LoadPalette(SAMB_addr FlicHdr_SgmtAddr, int Frame_Index)

    HERE("CALL: VGA_SetTextMode();");
    VGA_SetTextMode();
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: test_EMM_Load_LBX_File()\n", __FILE__, __LINE__);
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

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: test_GAME_LoadMainImages()\n", __FILE__, __LINE__);
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
    dlvfprintf("DEBUG: [%s, %d] g_EMM_Open_Handles: %u \n", __FILE__, __LINE__, g_EMM_Open_Handles);
    dlvfprintf("DEBUG: [%s, %d] g_EMM_Pages_Reserved: %d \n", __FILE__, __LINE__, g_EMM_Pages_Reserved);
    // same logic as in EMM_CheckHandleOpen()
    for ( itr_EMM_Table_Index = 0; itr_EMM_Table_Index < g_EMM_Open_Handles; itr_EMM_Table_Index++ )
    {
        if ( stricmp(g_EMM_Table[itr_EMM_Table_Index].eEmmHndlNm, TST_LBX_MAINSCRN_000.LBX_Name) == 0 )
        {
            dlvfprintf("DEBUG: [%s, %d] g_EMM_Table[%d].eEmmHndlNm: %s \n", __FILE__, __LINE__, itr_EMM_Table_Index, g_EMM_Table[itr_EMM_Table_Index].eEmmHndlNm);
            dlvfprintf("DEBUG: [%s, %d] g_EMM_Table[%d].eEmmHndlNbr: %u \n", __FILE__, __LINE__, itr_EMM_Table_Index, g_EMM_Table[itr_EMM_Table_Index].eEmmHndlNbr);
            dlvfprintf("DEBUG: [%s, %d] g_EMM_Table[%d].eEmmRsrvd: %d \n", __FILE__, __LINE__, itr_EMM_Table_Index, g_EMM_Table[itr_EMM_Table_Index].eEmmRsrvd);
        }
    }
    //TST_LBX_MAINSCRN_000.EMM_Table_Index
    //g_EMM_Table[itr_EMM_Table_Index].eEmmHndlNbr

    // s12p10  EMM_GetPageFrame()
    // s12p08  EMM_MapnRead()  s12p11  EMM_Map4()  s12p12  EMM_MapMulti4()
    // EMM_MAP_PAGE(_epp_,_ehn_,_elp_)
    EMM_Handle_Number = g_EMM_Table[TST_LBX_MAINSCRN_000.EMM_Table_Index].eEmmHndlNbr;
    EMM_Physical_Page_Number = 0;
    EMM_Logical_Page_Number = 0;
    // EMM_MAP_PAGE(0,4,0);
    // EMM_MAP_PAGE(EMM_Physical_Page_Number, EMM_Handle_Number, EMM_Logical_Page_Number);
    EMM_Map4(EMM_Handle_Number, EMM_Logical_Page_Number);
    dlvfprintf("DEBUG: [%s, %d] EMM_PageFrameBaseAddress: 0x%04X\n", __FILE__, __LINE__, EMM_PageFrameBaseAddress);
    // dlvfprintf("DEBUG: [%s, %d] LBX_EntryCount: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 0));
    // dlvfprintf("DEBUG: [%s, %d] LBX_MagSig_Hi: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 2));
    // dlvfprintf("DEBUG: [%s, %d] LBX_MagSig_Lo: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 4));
    // dlvfprintf("DEBUG: [%s, %d] LBX_Type: 0x%04X\n", __FILE__, __LINE__, farpeekw(EMM_PageFrameBaseAddress, 6));
    LBX_EntryCount = (WORD)(EMM_PageFrameBaseAddress + 0);
    LBX_MagSig_Hi  = (WORD)(EMM_PageFrameBaseAddress + 2);
    LBX_MagSig_Lo  = (WORD)(EMM_PageFrameBaseAddress + 4);
    LBX_Type       = (WORD)(EMM_PageFrameBaseAddress + 6);


#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: test_GAME_LoadMainImages()\n", __FILE__, __LINE__);
#endif
}

void test_FLIC_Draw_XY(void)
{


    HERE("FLIC_SetFrame(gsa_MAINSCRN_0_AnimatedLogo, 0);");
    FLIC_SetFrame(gsa_MAINSCRN_0_AnimatedLogo, 0);

    HERE("FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);");
    FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);

}