
/*
Unable to resolve configuration with compilerPath "J:/STU/DBWD/BORLANDC/BIN/BCC.EXE"
*/

#include "ST_HEAD.H"
#include "MOM_HEAD.H"
#include "MGC_HEAD.H"

/*
Hardware Settings
Game Settings
Sound, Music, and Speech Drivers and Settings
Saved Games
Initialize Hardware
Disable Debug
Initialize Tables
Set ESC Override
Load Palette, Set Palette
Play Intro (if !"JENNY")
Load Main Images
Load TerrStat
Prepare Credits
Clear Screen
Main Menu Music
Load Palette, Set Palette
Main Menu
*/


//int main(int argc, char *argv[])
//int main(void)
int mgc_main(void)
{
    printf("DEBUG: BEGIN: main()\n");

    g_EMM_Pages_Reserved = EMM_PAGES_REQUIRED;

    /*
        CONFIG.MOM
    */
    /*
        MAGIC.SET
    */
    /*
        MIDI Driver
        DSP Driver
    */
    /*
        SAVE.GAM
    */

    EMM_SetMinKB(EMM_MIN_KB);
    RAM_SetMinKB(RAM_MIN_KB);

    /*

        Hardware_Init()
        EMM, AIL
    */

    //Settings.InputType = 1;
    
    // _s14p01c.c
    //int __cdecl __far Hardware_Init(int Input_Type, int Snd_Chnls, int FontFile@, int M_Drv, int M_IO, int M_IRQ, int M_DMA, int D_Drv, int D_IO, int D_IRQ, int D_DMA)
    Hardware_Init(1, 2, GAME_FONT_FILE, 0, 0, 0, 0, 0, 0, 0, 0);  // Defaults for 'No Sound'

    /*
        DBG_Disable();
    */

    /*
        j_LBX_Tables_Init();
    */

    /*
        GUI_Set_ESC_Override();
    */

    /*
        VGA_LoadPalette(0, -1);
        VGA_DAC_Write();
    */

    /*
        if not "JENNY", then Play_Intro()
    */

    /*
        GAME_LoadMainImages()
    */
    /*
        GAME_LoadTerrstat()
    */
    /*
        GAME_LoadSpellData
    */

    /*
        Main Menu
    */
    /*
        j_GAME_PrepareCredits()
        VGA_DrawFilledRect(0, 0, 319, 199, 0)
        VGA_SetDirectDraw()
        VGA_DrawFilledRect(0, 0, 319, 199, 0)
        VGA_SetDrawFrame()
        SND_Stop_Music()
        MainMenu_Music_Seg = LBXE_LoadSingle("music", 104) // MUSIC_Main_Menu 68h
        if (Settings.BG_Music == 1)
        {
            SND_PlayFile(MainMenu_Music_Seg)
        }
        VGA_LoadPalette(0, -1)
        VGA_DAC_Write()

        GAME_MainMenu()
    */

    /*
        j_GAME_Save(8)
    */

    //GAME_QuitWithReport();
    GAME_QuitProgram("Thank you for playing Master of Magic!$");

    printf("DEBUG: END: main()\n");
    return 0;
}
