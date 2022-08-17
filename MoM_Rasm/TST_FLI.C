
#include "MGC_DEF.H"  // gsa_MAINSCRN_0_AnimatedLogo
#include "MOM_DEF.H"  // Quit()
#include "ST_EMM.H"  // EMM_Startup
#include "ST_FLIC.H"  // FLIC_Draw_XY()
//#include "ST_LBX.H"
//#include "ST_SA.H"
#include "ST_VGA.H"  // DEFAULT_FONTS_FILE; VGA_SetModeY()

#include "STU_DBG.H"


/* ╔══════════════════════════════════════════════════════════════════╗
╔══╝ Test: Draw FLIC Frame                                            ║
╠═════════════════════════════════════════════════════════════════════╣
║                                                                     ║
║  Set Video Mode                                                     ║
║  Load LBX                                                           ║
║                                                                     ║
║                                                                     ║
║                                                                     ║
║                                                                     ║
║                                                                     ║
║                                                                     ║
║                                                                  ╔══╝
╚══════════════════════════════════════════════════════════════════╝ */
/*
    // _s01p04c.c   void GAME_LoadMainImages(void)
    //gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
    //gsa_VORTEX_1_MenuContinue = LBXE_LoadSingle(g_LbxNm_VORTEX, 1);
    //gsa_VORTEX_2_MenuHallOfFame = LBXE_LoadSingle(g_LbxNm_VORTEX, 2);
    //gsa_VORTEX_5_MenuLoadGame = LBXE_LoadSingle(g_LbxNm_VORTEX, 5);
    //gsa_VORTEX_4_MenuNewGame = LBXE_LoadSingle(g_LbxNm_VORTEX, 4);
    //gsa_VORTEX_3_MenuQuitToDOS = LBXE_LoadSingle(g_LbxNm_VORTEX, 3);
    //gsa_MAINSCRN_5_ScreenBottom = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 5);

    // _s01p06c.c   void SCREEN_Menu_Draw(void)
    //FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);
    //FLIC_Draw_XY(0, 41, gsa_MAINSCRN_5_ScreenBottom);

    // _s28p11a.c   void FLIC_Draw_XY(int Left, int Top, unsigned int FlicHdr_SgmtAddr)
    //FLIC_Draw_EMM_C(tmp_SI, tmp_DI, FlicHdr_SgmtAddr, Frame_Index);


|-> EMM_Load_LBX_File_1() |-> EMM_Load_LBX_File(argEmmRsrvd=TRUE) |-> EMM_GetHandle(argEmmRsrvd=TRUE)
<-| EMM_CheckHandleOpen() { g_EMM_Reserved = g_EMM_Table.EmmRsrvd }
<-| EMM_LBX_Load_Entry() { if ( g_LBX_EmmRsvd == 1 ) { SAMB_data = EMM_LBX_HdrOnly(EmmHndl, EmmHndlNm, LbxEntry, SAMB_head, LoadType); }
LBXE_LoadSingle(LbxName=MAINSCRN, LbxEntryIndex=0)
LBX_Load_Entry(LbxName=MAINSCRN, LbxEntry=0, SAMB_head=0x0000, LoadType=0, LbxHdrFmt=0)
EMM_LBX_Load_Entry(EmmHndlNm=MAINSCRN, LbxEntry=0, SAMB_head=0x0000, LoadType=0, FormatType=0)

DEBUG: [_s12p08c.c, 103]: BEGIN: EMM_MapnRead(Dst_Ofst = 0x8930, Dst_Sgmt = 0x0000, Src_Ofst = 0x0006, Src_Sgmt = 0x0000, nbytes = 2, EmmHandle = 6)

DEBUG: [_s13p12c.c, 34] BEGIN: EMM_LBX_HdrOnly(EmmHndl=6, EmmHndlNm=MAINSCRN, LbxEntry=0, SAMB_head=0x0000, LoadType=0)

LBX_Load_Entry()
    SAMB_data = EMM_LBX_Load_Entry(LbxName, LbxEntry, SAMB_head, LoadType, tmp_LbxHdrFmt);
        EmmHndl = EMM_CheckHandleOpen(EmmHndlNm);


*/

void test_draw_flic_frame()
{
    Debug_Log_Startup();

    STGE_Init_Draw();

    /*
        Confirm STGE State:

            EMM:
                g_EMM_PageFrame_Base_Address, g_EMM_OK, g_EMM_Pages_Reserved, g_EMM_Open_Handles, g_EMM_Table[], g_EmmHndlNbr_YOMOMA, g_EmmHndlNbr_VGAFILEH, g_EmmHndlNbr_EMMDATAH, g_EMMDATAH_Level
            
            VGA:
                g_PaletteLbxFileName, gsa_FontStyleData, gsa_BorderStyleData, gsa_PaletteLbxEntry, gsa_Palette, gsa_PaletteFlags, gsa_PaletteSaved, gsa_ReplacementColors, gsa_VGAFILEH_Header, gsa_IntensityScaleTable
                UU_g_VGA_TextDraw_Initd, gfp_VGA_TextLine_Lefts, gfp_VGA_TextLine_Rights, gfp_VGA_TextLine_Tops, gfp_VGA_TextLine_Starts
                g_RenderScreenPage, g_DrawScreenPage_SgmtAddr

    */
    dlvfprintf("DEBUG: [%s, %d] g_EMM_PageFrame_Base_Address: 0x%04X\n", __FILE__, __LINE__, g_EMM_PageFrame_Base_Address);
    dlvfprintf("DEBUG: [%s, %d] g_RSP_Idx: %u, gsa_DSP_Addr: 0x%04X\n", __FILE__, __LINE__, g_RSP_Idx, gsa_DSP_Addr);

    EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);

    gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
    gsa_MAINSCRN_5_ScreenBottom = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 5);
    // //gsa_VORTEX_1_MenuContinue = LBXE_LoadSingle(g_LbxNm_VORTEX, 1);
    // //gsa_VORTEX_2_MenuHallOfFame = LBXE_LoadSingle(g_LbxNm_VORTEX, 2);
    // //gsa_VORTEX_3_MenuQuitToDOS = LBXE_LoadSingle(g_LbxNm_VORTEX, 3);
    // //gsa_VORTEX_4_MenuNewGame = LBXE_LoadSingle(g_LbxNm_VORTEX, 4);
    // //gsa_VORTEX_5_MenuLoadGame = LBXE_LoadSingle(g_LbxNm_VORTEX, 5);
    //dlvfprintf("DEBUG: [%s, %d] gsa_MAINSCRN_0_AnimatedLogo: 0x%04X\n", __FILE__, __LINE__, gsa_MAINSCRN_0_AnimatedLogo);
    //dlvfprintf("DEBUG: [%s, %d] gsa_MAINSCRN_5_ScreenBottom: 0x%04X\n", __FILE__, __LINE__, gsa_MAINSCRN_5_ScreenBottom);

    //FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);
    //FLIC_Draw_XY(0, 41, gsa_MAINSCRN_5_ScreenBottom);

    // // //FLIC_Draw_XY(Left=0, Top=0, FlicHdr_SgmtAddr=0x1C54)
    // // //FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);
    // // //FLIC_Draw_EMM_C(ScreenPage_X=0, ScreenPage_Y=0, SAMB_data_FLIC_HDR=0x1C54, Frame_Index=3)
    // FLIC_Draw_EMM_C(0, 0, gsa_MAINSCRN_0_AnimatedLogo, 0);
    // // ?!? VGA_LoadPalette(2, -1, 0); ?!? VGA_DAC_Write(); ?!? VGA_SetDrawFrame(); ?!? VGA_PageFlip(); || GUI_SimplePageFlip(); ?!? VGA_SetDirectDraw ?!?

    // getch();
    // VGA_DAC_Write();
    // getch();
    // VGA_PageFlip();
    // dlvfprintf("DEBUG: [%s, %d] g_RenderScreenPage: %u, g_DrawScreenPage_SgmtAddr: 0x%04X\n", __FILE__, __LINE__, g_RenderScreenPage, g_DrawScreenPage_SgmtAddr);
    // getch();

    // draws and renders:  FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo); VGA_DAC_Write(); VGA_PageFlip();
    // palette wrong, but draws and renders:  FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo); VGA_PageFlip();
    // flashes on the DAC update, but draws and renders:  FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo); VGA_PageFlip(); VGA_DAC_Write();
    
    // does not render:  FLIC_Draw_EMM_C(0, 0, gsa_MAINSCRN_0_AnimatedLogo, 0); VGA_DAC_Write(); VGA_PageFlip();

    // draws and renders:  FLIC_LoadPalette(gsa_MAINSCRN_0_AnimatedLogo, 0); FLIC_Draw_EMM_C(0, 0, gsa_MAINSCRN_0_AnimatedLogo, 0); VGA_DAC_Write(); VGA_PageFlip();

    // // seems to work fine
    // FLIC_LoadPalette(gsa_MAINSCRN_0_AnimatedLogo, 0);
    // FLIC_Draw_EMM_C(0, 0, gsa_MAINSCRN_0_AnimatedLogo, 0);
    // VGA_DAC_Write();
    // VGA_PageFlip();
    // getch();
    // // seems to work fine
    // FLIC_Draw_EMM_C(0, 0, gsa_MAINSCRN_0_AnimatedLogo, 0);
    // FLIC_Draw_EMM_C(0, 41, gsa_MAINSCRN_5_ScreenBottom, 0);
    // VGA_DAC_Write();
    // VGA_PageFlip();
    // getch();

    FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);  // Frame Index 0
    FLIC_Draw_XY(0, 41, gsa_MAINSCRN_5_ScreenBottom);
    VGA_DAC_Write();
    VGA_PageFlip();
    getch();
    //FLIC_ResetFrame(gsa_MAINSCRN_0_AnimatedLogo);
    //Title_Frame_Index = FLIC_GetCurFrame(gsa_MAINSCRN_0_AnimatedLogo);
    //FLIC_SetFrame(gsa_MAINSCRN_0_AnimatedLogo, 0);

    FLIC_ResetFrame(gsa_MAINSCRN_0_AnimatedLogo);
    FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);  // Frame Index 0
    FLIC_Draw_XY(0, 41, gsa_MAINSCRN_5_ScreenBottom);
    VGA_DAC_Write();
    VGA_PageFlip();
    getch();

    FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);  // Frame Index 1
    VGA_PageFlip();
    getch();

    FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo);  // Frame Index 2 - scrambled
    VGA_PageFlip();
    getch();


    VGA_SetTextMode();

    Debug_Log_Shutdown();
}
