// _s01p06c.c SCREEN_Menu_Draw()
// MGC_DEF.H

#include "MOM_DEF.H"
#include "MGC_DEF.H"

#include "STU_DBG.H"


void SCREEN_Menu_Draw(void)
{
    int Title_Frame_Index;
    int Loop_Var;
    int Continue_Move_Down;
    int MouseOver_ControlIndex;
    int MenuArea_X_Left;
    int MenuArea_Y_Top;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: SCREEN_Menu_Draw()\n", __FILE__, __LINE__);
#endif

    MenuArea_X_Left = 123;
    MenuArea_Y_Top = 141;

    HERE("GUI_MousedControl()");
    MouseOver_ControlIndex = GUI_MousedControl();
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] MouseOver_ControlIndex: %d\n", __FILE__, __LINE__, MouseOver_ControlIndex);
#endif

    VGA_SetDrawFrame();
    VGA_DrawFilledRect(0, 0, 319, 199, 0);

    HERE("FLIC_GetCurFrame()");
    Title_Frame_Index = FLIC_GetCurFrame(gsa_MAINSCRN_0_AnimatedLogo);
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] Title_Frame_Index: %d\n", __FILE__, __LINE__, Title_Frame_Index);
#endif

    HERE("FLIC_SetFrame()");
    FLIC_SetFrame(gsa_MAINSCRN_0_AnimatedLogo, 0);

    // TODO(JimBalcomb): figure out and fix the logic for looping the 13 frames of the "MOM animated logo"
    // for ( Loop_Var = 0; Loop_Var <= Title_Frame_Index; Loop_Var++ )
    // {
    //     HERE("VGA_DrawLBXImage()");
    //     VGA_DrawLBXImage(0, 0, gsa_MAINSCRN_0_AnimatedLogo);  // NOTE(JimBalcomb): This is the first call to VGA_DrawLBXImage()
    // }
    HERE("VGA_DrawLBXImage(0, 0, gsa_MAINSCRN_0_AnimatedLogo);");
    VGA_DrawLBXImage(0, 0, gsa_MAINSCRN_0_AnimatedLogo);  // NOTE(JimBalcomb): This is the first call to VGA_DrawLBXImage()
    // NOTE(JimBalcomb): this LBX Entry/FLIC includes a 256-color palette chunk

    HERE("VGA_DrawLBXImage(0, 41, gsa_MAINSCRN_5_ScreenBottom);");
    VGA_DrawLBXImage(0, 41, gsa_MAINSCRN_5_ScreenBottom);

    if ( g_ScreenChangeFade == 0 )
    {
        // j_GAME_DrawCredits()
    }

    Continue_Move_Down = 0;

    if ( g_GAME_HaveSaves == 0 )
    {
        Continue_Move_Down = 1;
    }
    else
    {
        if ( MouseOver_ControlIndex == GUI_Load_Lbl_Index )
        {
            HERE("FLIC_SetFrame()");
            FLIC_ResetFrame(gsa_VORTEX_5_MenuLoadGame);
        }
        else
        {
            HERE("FLIC_ResetFrame()");
            FLIC_SetFrame(gsa_VORTEX_5_MenuLoadGame, 1);
        }

        HERE("VGA_DrawLBXImage()");
        VGA_DrawLBXImage(MenuArea_X_Left, (MenuArea_Y_Top + 12), gsa_VORTEX_5_MenuLoadGame);

    }

    if ( g_GAME_HaveContSave != 0 )
    {
        if ( MouseOver_ControlIndex == GUI_Continue_Label )
        {
            HERE("FLIC_ResetFrame()");
            FLIC_ResetFrame(gsa_VORTEX_1_MenuContinue);
        }
        else
        {
            HERE("FLIC_SetFrame()");
            FLIC_SetFrame(gsa_VORTEX_1_MenuContinue, 1);
        }

        HERE("VGA_DrawLBXImage()");
        VGA_DrawLBXImage(MenuArea_X_Left, (MenuArea_Y_Top + (12 * Continue_Move_Down)), gsa_VORTEX_1_MenuContinue);
    }

    if ( MouseOver_ControlIndex == GUI_NewGame_Label )
    {
        HERE("FLIC_ResetFrame()");
        FLIC_ResetFrame(gsa_VORTEX_4_MenuNewGame);
    }
    else
    {
        HERE("FLIC_SetFrame()");
        FLIC_SetFrame(gsa_VORTEX_4_MenuNewGame, 1);
    }

    HERE("VGA_DrawLBXImage()");
    VGA_DrawLBXImage(MenuArea_X_Left, (MenuArea_Y_Top + 24), gsa_VORTEX_4_MenuNewGame);

    if ( MouseOver_ControlIndex == GUI_HoF_Lbl_Index )
    {
        HERE("FLIC_ResetFrame()");
        FLIC_ResetFrame(gsa_VORTEX_2_MenuHallOfFame);
    }
    else
    {
        HERE("FLIC_SetFrame()");
        FLIC_SetFrame(gsa_VORTEX_2_MenuHallOfFame, 1);
    }

    HERE("VGA_DrawLBXImage()");
    VGA_DrawLBXImage(MenuArea_X_Left, (MenuArea_Y_Top + 36), gsa_VORTEX_2_MenuHallOfFame);

    if ( MouseOver_ControlIndex == GUI_Quit_Lbl_Index )
    {
        HERE("FLIC_ResetFrame()");
        FLIC_ResetFrame(gsa_VORTEX_3_MenuQuitToDOS);
    }
    else
    {
        HERE("FLIC_SetFrame()");
        FLIC_SetFrame(gsa_VORTEX_3_MenuQuitToDOS, 1);
    }

    HERE("VGA_DrawLBXImage()");
    VGA_DrawLBXImage(MenuArea_X_Left, (MenuArea_Y_Top + 48), gsa_VORTEX_3_MenuQuitToDOS);

    // // VGA_LoadPalette(2, -1, 0);
    // // VGA_DAC_Write();
    // // STU_VGA_DAC_Dump("MENUDAC.BIN");
    // // //VGA_SetDrawFrame();
    // // VGA_PageFlip();
    // // DEBUG(JimBalcomb): doing all of this here does make the screen render correctly
    // VGA_LoadPalette(2, -1, 0);
    // VGA_DAC_Write();
    // DEBUG(JimBalcomb): doing just these two here does make the screen render correctly
    VGA_DAC_Write();
    // DEBUG(JimBalcomb): just VGA_DAC_Write() here does make the screen render correctly
    // so, ...  VGA_DrawLBXImage(0, 0, gsa_MAINSCRN_0_AnimatedLogo) has 256-clor custom palette...
    ///... and, calls FLIC_LoadPalette() to load that
    // ... and, FLIC_LoadPalette() sets the palette update flags
    // so, VGA_DAC_Write() needs to be called to /apply/ the palette
    // the calls to VGA_DAC_Write() are in GUI_Redraw_WaitOne() and GUI_Redraw_WaitTimer()
    // the calls to GUI_Redraw_WaitOne() and GUI_Redraw_WaitTimer() are in GUI_ProcessInput()


    // mov  ax, [CRP_Unused_Anim_Var]
    // inc  ax
    // mov  bx, 20
    // cwd
    // idiv bx
    // mov  [CRP_Unused_Anim_Var], dx

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: SCREEN_Menu_Draw()\n", __FILE__, __LINE__);
#endif
}
