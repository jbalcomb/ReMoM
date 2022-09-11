// _s01p01 main
// in MGC_DEF.H

/*
Unable to resolve configuration with compilerPath "J:/STU/DBWD/BORLANDC/BIN/BCC.EXE"
*/

#include "MGC_HEAD.H"

#include "ST_GUI.H"
#include "ST_VGA.H"

#include "STU_DBG.H"

/*
    ? Tell the Compiler that the Linker will find these later ?
*/
extern unsigned int EMM_PageFrame_Base_Address;   // dseg:40E4

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
int MGC_Main(void)
{
    //DBGLOG("DEBUG: [%s, %d] BEGIN: MGC_Main()\n", __FILE__, __LINE__);
    //TRACE(("message %d\n", 1));
    // dlvfprintf("DEBUG: [%s, %d] BEGIN: MGC_Main()\n", __FILE__, __LINE__);
    
    g_EMM_Pages_Reserved = EMM_PAGES_REQUIRED;

    /*
        CONFIG.MOM
    */
    // LoadConfigMom()

    /*
        MAGIC.SET
    */
    //LoadMagicSet()

    /*
        MIDI Driver
        DSP Driver
    */
    /*
        SAVE.GAM
        for ( itr_SaveGamNbr = 1; itr_SaveGamNbr < 9; itr_SaveGamNbr++ )
    */

    EMM_SetMinKB(EMM_MIN_KB);
    RAM_SetMinKB(RAM_MIN_KB);

    //Settings.InputType = 1;
    // _s14p01c.c
    //int __cdecl __far Hardware_Init(int Input_Type, int Snd_Chnls, char *FontFile, int M_Drv, int M_IO, int M_IRQ, int M_DMA, int D_Drv, int D_IO, int D_IRQ, int D_DMA)
    Hardware_Init(1, 2, GAME_FONT_FILE, 0, 0, 0, 0, 0, 0, 0, 0);  // Defaults for 'No Sound'
    /*
        |-> EMM_Startup()
        |-> VGA_SetModeY()
        |-> VGA_DAC_Init() |-> ... LBXE_LoadSingle(), SA_Allocate_Space(), VGA_TextDraw_Init()
        |-> SND_Init()
        |-> IN_Init() |-> MD_Init()
        |-> RNG_TimerSeed()
        |-> VGA_Set_DSP_Addr()
    */

    DBG_Disable();

    /*
        j_LBX_Tables_Init();  // stub057
            |-> LBX_Tables_Init(6100);  // ovr057
    */
    // TODO(JimBalcomb,20220818): move whatever this 6100 is to a manifest constant
    MoM_Tables_Init(6100);
    
    IN_SetEscOverride();

    //VGA_LoadPalette(0, -1);  // argument missing in dasm
    VGA_LoadPalette(0, -1, 0);
    VGA_DAC_Write();

    /*
        if not "JENNY", then Play_Intro()
    */

    GAME_LoadMainImages();
    GAME_Load_TERRSTAT_0();
    GAME_Load_SPELLDAT_0();

    /*
        Main Menu
    */

    /*
        Main Menu - Credits
    */
    // //j_GAME_PrepareCredits()
    // GAME_PrepareCredits();


    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Draw Screen-Page
    VGA_SetDirectDraw();
    VGA_DrawFilledRect(0, 0, 319, 199, 0);  // ~= Clear Screen, on Render Screen-Page
    VGA_Set_DSP_Addr();

    /*
        SND_Stop_Music()
        MainMenu_Music_Seg = LBXE_LoadSingle("music", 104) // MUSIC_Main_Menu 68h
        if (Settings.BG_Music == 1)
        {
            SND_PlayFile(MainMenu_Music_Seg)
        }
    */

    //VGA_LoadPalette(0, -1);  // argument missing in dasm
    VGA_LoadPalette(0, -1, 0);  // EMPERATO
    VGA_DAC_Write();



    GAME_MainMenu();  // MGC_DEF.H  _s01p03c.c




    /*
        SAVEGAM_Save(8)
    */

    //GAME_QuitWithReport();
    Quit("Thank you for playing Master of Magic!$");


    //DBGLOG("DEBUG: [%s, %d] END: MGC_Main()\n", __FILE__, __LINE__);
    // dlvfprintf("DEBUG: [%s, %d] END: MGC_Main()\n", __FILE__, __LINE__);

    return 0;
}
