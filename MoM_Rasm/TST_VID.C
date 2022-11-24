
/*
    Tests for the Game Engine Video Sub-System
    TST_VID
    STGE_VID
    ST_VID
    MoX_VID
    ?Hardware?VGA Video Card?~SDL?~GDI?
    ?"Graphics"?
    Graphics Primitives
        Draw
            Point, Line, Box
            Fill
            Nay-Fill/Yay-Fill
            Pattern
            Masks/Masked
            Remapped Colors
            Color Cycling
            Anti-Aliasing
            Shading
            Blending

Fonts
Palettes
VGA
FLIC
"pictures", "animations"


*/

/*
    Hot-Mess of re-re-re-naming identifiers...

g_LbxNm_MAINSCRN

mainscrn_lbx_file

unsigned int gsa_MAINSCRN_0_AnimatedLogo;   // dseg:52E8
gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);


gsa_MAINSCRN_0_AnimatedLogo
sa_MAINSCRN_000
fmainmenu_top

? from: to: ?
ST_VGA.C/.H
seg001.C/.H

*/


void test_VGA_VRAM(void);

void test_VGA_SetDirectDraw(void);
void test_VGA_Set_DSP_Addr(void);
void test_PAL_Load_Palette(void);
void test_Load_Font_File(void);

void test_FLIC_Load_Palette(void);
void test_FLIC_Draw_EMM(void);
void test_FLIC_Draw_XY(void);

void test_FLIC_Draw_Back(void);
void test_FLIC_Draw_Frame_Back(void);
void test_FLIC_Draw_Frame_2BMP(void);

void test_STU_Export_Palette_XBGR(void);
void test_STU_Export_VBB_To_BMP(void);






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
    // FLIC_Draw_EMM_C(Left, Top, sa_FLIC_Header, Frame_Index);
    FLIC_Draw_Frame_EMM(Left, Top, sa_FLIC_Header, Frame_Index);
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
    // gsa_MAINSCRN_0_AnimatedLogo  = LBXE_LoadSingle(mainscrn_lbx_file, 0);
    // gsa_MAINSCRN_5_ScreenBottom  = LBXE_LoadSingle(mainscrn_lbx_file, 5);
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




void test_FLIC_Draw_Back(void)
{
    int MenuArea_X_Left;
    int MenuArea_Y_Top;
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_FLIC_Draw_Back()\n", __FILE__, __LINE__);
#endif

    MenuArea_X_Left = 123;
    MenuArea_Y_Top = 141;

    video_back_buffer = SA_Allocate_MemBlk(4000);
    video_back_buffer_seg = FP_SEG(video_back_buffer);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] video_back_buffer: %p\n", __FILE__, __LINE__, video_back_buffer);
    dbg_prn("DEBUG: [%s, %d] video_back_buffer_seg: 0x%04X\n", __FILE__, __LINE__, video_back_buffer_seg);
#endif
    memset(video_back_buffer, CYAN, 320*200);

    Load_Font_File(GAME_FONT_FILE);

    PAL_Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View

    mainmenu_top = LBXE_LoadSingle_LM(mainscrn_lbx_file, 0);   // TESTSCRN.LBX  320 x 40
    mainmenu_bot = LBXE_LoadSingle_LM(mainscrn_lbx_file, 5);   // TESTSCRN.LBX  320 x 160
    mainmenu_c   = LBXE_LoadSingle_LM(vortex_lbx_file, 1);  // "Continue"
    mainmenu_h   = LBXE_LoadSingle_LM(vortex_lbx_file, 2);
    mainmenu_q   = LBXE_LoadSingle_LM(vortex_lbx_file, 3);  // "Quit To DOS"
    mainmenu_n   = LBXE_LoadSingle_LM(vortex_lbx_file, 4);
    mainmenu_l   = LBXE_LoadSingle_LM(vortex_lbx_file, 5);

    // // FLIC_Load_Palette(fp_FLIC_File, current_frame_index);
    // FLIC_Load_Palette(mainmenu_top, 0);
    
    // FLIC_Draw_Back(0,0,0,mainmenu_l,video_back_buffer);



    VGA_SetModeY();
    // g_RSP_Idx = 0;
    // gsa_DSP_Addr = 0xA000;

    VGA_Set_DSP_Addr();
    // g_RSP_Idx = 0;
    // gsa_DSP_Addr = 0xA400;

//     Fill_Video_Back_Buffer(CYAN, video_back_buffer);
//     // Draw_Video_Back_Buffer(0, 0, 320, 200, video_back_buffer);
//     test_VBB_Pattern_Draw();
//     // VGA_PageFlip();
//     getch();
// 
//     Fill_VRAM(GREEN);
//     VGA_PageFlip();
//     getch();

    VGA_DrawFilledRect(0,0,319,199,MAGENTA);
    // getch();
    VGA_PageFlip();
    getch();

    FLIC_Draw(MenuArea_X_Left, MenuArea_Y_Top + 48, mainmenu_q);
    // getch();
    VGA_PageFlip();
    // getch();
    VGA_DAC_Write();
    getch();

    // FLIC_Draw_Back(32, 20, 0, mainmenu_q, video_back_buffer);
    // getch();
    // Draw_Video_Back_Buffer(0, 0, 320, 200, video_back_buffer);
    // getch();

    // Fill_Video_Back_Buffer(CYAN, video_back_buffer);
    // getch();
    // Draw_Video_Back_Buffer(0, 0, 320, 200, video_back_buffer);
    // getch();
    // VGA_PageFlip();
    // getch();

    // FLIC_Draw_Back(MenuArea_X_Left, MenuArea_Y_Top + 48, 0, mainmenu_q, video_back_buffer);
    // getch();

    FLIC_Draw_Back(32, 20, 0, mainmenu_q, video_back_buffer);
    // Draw_Video_Back_Buffer(0, 0, 320, 200, video_back_buffer);
    test_VBB_Pattern_Draw();
    // VGA_PageFlip();
    getch();

    FLIC_Draw_Back(0, 0, 0, mainmenu_bot, video_back_buffer);
    test_VBB_Pattern_Draw();
    getch();

    FLIC_Draw_Back(0, 0, 0, mainmenu_bot, video_back_buffer);
    FLIC_Draw_Back(0, 161, 0, mainmenu_top, video_back_buffer);
    FLIC_Draw_Back(12, 12, 0, mainmenu_q, video_back_buffer);  // q
    FLIC_Draw_Back(12, 24, 0, mainmenu_c, video_back_buffer);  // c
    FLIC_Draw_Back(12, 36, 0, mainmenu_h, video_back_buffer);  // h
    FLIC_Draw_Back(12, 48, 0, mainmenu_n, video_back_buffer);  // n
    FLIC_Draw_Back(12, 60, 0, mainmenu_l, video_back_buffer);  // l
    // Draw_Video_Back_Buffer(0, 0, 320, 200, video_back_buffer);
    test_VBB_Pattern_Draw();
    // VGA_PageFlip();
    getch();

    // Fill_VRAM(GREEN);  // How slow is it?!? It's Sofa-King Slow!!
    // VGA_PageFlip();
    // getch();


    VGA_SetTextMode();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_FLIC_Draw_Back()\n", __FILE__, __LINE__);
#endif

}




void test_FLIC_Draw_Frame_Back(void)
{
    int test_status;
    static struct s_FLIC_HDR PS_FLIC_Header;  // persistent, local
    int x;
    int y;
    int w;
    int current_frame_index;
    SAMB_ptr video_back_buffer;
    unsigned int dos_largest_memory_block;
    unsigned long int current_coreleft;
    unsigned long int current_farcoreleft;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_FLIC_Draw_Frame_Back()\n", __FILE__, __LINE__);
#endif

#ifdef STU_DEBUG
    _BX = 0xFFFF;
    _AH = 0x48;             // ALLOCMEM
    geninterrupt(0x21);     // DOS_INT
    dos_largest_memory_block = _BX;  // BX = size in paras of the largest block of memory available
    dbg_prn("DEBUG: [%s, %d] dos_largest_memory_block PR: %u\n", __FILE__, __LINE__, dos_largest_memory_block);
    dbg_prn("DEBUG: [%s, %d] dos_largest_memory_block B: %u\n", __FILE__, __LINE__, (dos_largest_memory_block * 16));
    dbg_prn("DEBUG: [%s, %d] dos_largest_memory_block KB: %u\n", __FILE__, __LINE__, (dos_largest_memory_block * 16 / 64));
#endif
#ifdef STU_DEBUG
    current_coreleft = (unsigned long) coreleft();
    current_farcoreleft = (unsigned long) farcoreleft();
    dbg_prn("current_coreleft: %lu\n", current_coreleft);
    dbg_prn("current_farcoreleft: %lu\n", current_farcoreleft);
#endif
    // printf("The difference between the highest allocated block and\n");
    // printf("the top of the heap is: %lu bytes\n", (unsigned long) coreleft());
    // printf("The difference between the highest allocated block in the far\n");
    // printf("heap and the top of the far heap is: %lu bytes\n", farcoreleft());

    test_status = 0;  // TEST_UNDEFINED

    if(!g_Load_Font_File_tested) { test_Load_Font_File(); }
    if(!g_Load_Font_File_validated) { test_status = -1; }  // TEST_FAILURE

    PAL_Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View


    mainmenu_top = LBXE_LoadSingle_LM(mainscrn_lbx_file, 0);
    // mainmenu_bot = LBXE_LoadSingle_LM(mainscrn_lbx_file, 5);
    mainmenu_c = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 1); // 70 paragraphs
    mainmenu_h = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 2);
    mainmenu_q = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 3);
    mainmenu_n = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 4);
    mainmenu_l = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 5);


    memcpy((void *)&PS_FLIC_Header, (const void *)mainmenu_l, sizeof(PS_FLIC_Header));

    // video_back_buffer = SA_Allocate_Space(4000);  // 4000 paragraphs = 64000 bytes / 16 bytes per paragraph

    current_frame_index = 0;
    x = 0;
    y = 0;


    // VGA_SetModeY();  // HERE: g_RSP_Idx = 0; gsa_DSP_Addr = 0xA000;
    // VGA_DrawFilledRect(0,0,319,199,5);
    // getch();

    // FLIC_Draw_Frame_Back(x, y, w, mainmenu_l, video_back_buffer);

    // SCREEN_Menu()  seg001.C, Line 540
    // seg020.C  void PAL_Load_Palette(int entry, int start_color, int end_color)
    PAL_Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View

    FLIC_Load_Palette(mainmenu_l, current_frame_index);



    // VGA_PageFlip();
    // getch();

    // VGA_DAC_Write();
    // getch();

    // FLIC_Load_Palette(MK_FP(sa_FLIC_Header,0), Frame_Index);  // s21p07
    // getch();

    // VGA_DAC_Write();
    // getch();
    
    // VGA_PageFlip();
    // getch();

    // // BAD!! STU_VGA_RAM_Dump_1();
    // STU_VGA_RAM_Dump();

    // VGA_SetTextMode();

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_FLIC_Draw_EMM()\n", __FILE__, __LINE__);
#endif
}




void test_FLIC_Draw_Frame_2BMP(void)
{

}




void test_STU_Export_Palette_XBGR(void)
{
    Load_Font_File(GAME_FONT_FILE);
    PAL_Load_Palette(2,-1,0);
    STU_Export_Palette_XBGR();
}




void test_STU_Export_VBB_To_BMP(void)
{
//     int MenuArea_X_Left;
//     int MenuArea_Y_Top;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_STU_Export_VBB_To_BMP()\n", __FILE__, __LINE__);
#endif

    // Load_Font_File(GAME_FONT_FILE);
    // PAL_Load_Palette(2,-1,0);
    // VGA_DAC_Write();
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: test_FLIC_Draw_Back()\n", __FILE__, __LINE__);
#endif

//     MenuArea_X_Left = 123;
//     MenuArea_Y_Top = 141;

    video_back_buffer = SA_Allocate_MemBlk(4000);
    video_back_buffer_seg = FP_SEG(video_back_buffer);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] video_back_buffer: %p\n", __FILE__, __LINE__, video_back_buffer);
    dbg_prn("DEBUG: [%s, %d] video_back_buffer_seg: 0x%04X\n", __FILE__, __LINE__, video_back_buffer_seg);
#endif
    // memset(video_back_buffer, CYAN, 320*200);
    // STU_Export_VBB_To_BIN();
    // Load_Font_File(GAME_FONT_FILE);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] p_Palette_XBGR: %p\n", __FILE__, __LINE__, p_Palette_XBGR);
#endif
    // STU_Export_Palette_XBGR();
    // STU_Export_VBB_To_BMP();
    // STU_Export_VBB_To_BMP32();

    // Fill_Video_Back_Buffer(MAGENTA, video_back_buffer);
    // STU_Export_VBB_To_BMP();

    // Load_Font_File(GAME_FONT_FILE);
    // PAL_Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View
    // STU_Export_Palette_XBGR();


    // Load_Font_File(GAME_FONT_FILE);
    // PAL_Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View
    // mainmenu_top = LBXE_LoadSingle_LM("TESTSCRN.LBX", 0);   // TESTSCRN.LBX  320 x 40
    // FLIC_Draw_Back(0, 0, 0, mainmenu_bot, video_back_buffer);
    // STU_Export_Palette_XBGR();

    // Load_Font_File(GAME_FONT_FILE);
    // PAL_Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View
    // mainmenu_top = LBXE_LoadSingle_LM("TESTSCRN.LBX", 0);   // TESTSCRN.LBX  320 x 40
    // mainmenu_bot = LBXE_LoadSingle_LM(mainscrn_lbx_file, 5);   // TESTSCRN.LBX  320 x 160
    // mainmenu_c = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 1);       // 
    // mainmenu_h = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 2);       // 
    // mainmenu_q = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 3);       // Quit To DOS
    // mainmenu_n = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 4);       // 
    // mainmenu_l = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 5);       // 
    // FLIC_Draw_Back(0, 0, 0, mainmenu_bot, video_back_buffer);
    // FLIC_Draw_Back(0, 161, 0, mainmenu_top, video_back_buffer);
    // FLIC_Draw_Back(12, 12, 0, mainmenu_q, video_back_buffer);  // q
    // FLIC_Draw_Back(12, 24, 0, mainmenu_c, video_back_buffer);  // c
    // FLIC_Draw_Back(12, 36, 0, mainmenu_h, video_back_buffer);  // h
    // FLIC_Draw_Back(12, 48, 0, mainmenu_n, video_back_buffer);  // n
    // FLIC_Draw_Back(12, 60, 0, mainmenu_l, video_back_buffer);  // l
    // STU_Export_Palette_XBGR();
    // STU_Export_VBB_To_BIN();
    // STU_Export_VBB_To_BMP();
    // STU_Export_VBB_To_BMP32();




    VGA_SetModeY();      // g_RSP_Idx = 0;  gsa_DSP_Addr = 0xA000;
    VGA_Set_DSP_Addr();  // g_RSP_Idx = 0;  gsa_DSP_Addr = 0xA400;

    Load_Font_File(GAME_FONT_FILE);
    PAL_Load_Palette(2, -1, 0); // ARCANUS - Magic Castle View
    mainmenu_top = LBXE_LoadSingle_LM("TESTSCRN.LBX", 0);   // TESTSCRN.LBX  320 x 40
    mainmenu_bot = LBXE_LoadSingle_LM(mainscrn_lbx_file, 5);   // TESTSCRN.LBX  320 x 160
    mainmenu_c = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 1);       // 
    mainmenu_h = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 2);       // 
    mainmenu_q = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 3);       // Quit To DOS
    mainmenu_n = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 4);       // 
    mainmenu_l = LBXE_LoadSingle_LM(g_LbxNm_VORTEX, 5);       // 
    FLIC_Draw_Back(0, 0, 0, mainmenu_bot, video_back_buffer);
    FLIC_Draw_Back(0, 161, 0, mainmenu_top, video_back_buffer);
    FLIC_Draw_Back(12, 12, 0, mainmenu_q, video_back_buffer);  // q
    FLIC_Draw_Back(12, 24, 0, mainmenu_c, video_back_buffer);  // c
    FLIC_Draw_Back(12, 36, 0, mainmenu_h, video_back_buffer);  // h
    FLIC_Draw_Back(12, 48, 0, mainmenu_n, video_back_buffer);  // n
    FLIC_Draw_Back(12, 60, 0, mainmenu_l, video_back_buffer);  // l
    test_VBB_Pattern_Draw();
    getch();
    VGA_DAC_Write();
    getch();
    VGA_PageFlip();
    getch();
    STU_Export_Palette_XBGR();
    STU_Export_VBB_To_BIN();
    STU_Export_VBB_To_BMP();
    STU_Export_VBB_To_BMP32();


    VGA_SetTextMode();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: test_STU_Export_VBB_To_BMP()\n", __FILE__, __LINE__);
#endif
}
