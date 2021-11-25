#ifndef MOM_DEF_H
#define MOM_DEF_H

#ifdef DEBUG
int DEBUG 1;
#endif  /* DEBUG */


#define EMM_PAGES_REQUIRED 158
#define EMM_MIN_KB 2700  // 168 pages ? 168 = 158 + (1 + 5 + 4) ?
#define RAM_MIN_KB 583

#define MOM_FONT_FILE "FONTS.LBX"
#define GAME_FONT_FILE MOM_FONT_FILE

typedef struct
    {
        char name[20];
    } SaveNameStruct;

typedef struct
    {
        int SoundFX;
        int BG_Music;
        int Event_Music;
        int City_Spell_Events;
        int Ovl_Spell_Events;
        int Summoning_Events;
        int EoT_Summary;
        int Raze_City;
        int Random_Events;
        int EoT_Wait;
        int Strategic_Combat;
        int Auto_Unit_Info;
        int Enemy_Moves;
        int Enemy_Spells;
        int Spell_Book_Order;
        int Spell_Anims;
        int Show_Node_Owners;
        int Expanding_Help;
        int SoundChannels;
        int InputType;
        int Difficulty;
        int Opponents;
        int LandSize;
        int MagicPower;
        int Using_Saves[8];
        SaveNameStruct Save_Names[8];
        int Movement_Anims;
        SaveNameStruct HallofFame_Names[10];
        int HallofFame_Scores[10];
        int HallofFame_Races[10];
    } SettingsStruct;

extern SettingsStruct Settings;


extern unsigned int g_MemFreeWorst_KB;  // dseg:3CC2
extern char *cnst_Quit_Report1;         // dseg:3CC4
extern char *cnst_Quit_Report2;         // dseg:3CC9
extern char *cnst_Quit_Report3;         // dseg:3CD1
extern char *cnst_Quit_Report4;         // dseg:3CE1
extern char *cnst_Quit_Report5;         // dseg:3CEB
extern char *cnst_Quit_Report6;         // dseg:3CF5








#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

// _s05p01c.c
void GAME_QuitWithReport(void);
// _s05p02c.c
void GAME_QuitProgram(char *quitmsg);
// _s05p05c.c
void Update_MemFreeWorst_KB(void);

// _s06p01c.c
void VGA_SetTextMode(void);
// _s06p02c.c
void DOS_QuitWithMsg(char *quitmsg);        // IDA: void __cdecl __far DOS_QuitWithMsg(char *argStringPtr)
// _s06p04
unsigned int dos_memfree_kb(void);

/*
    seg014
*/
// _s14p01
//void Hardware_Init(char *FontFileName);
void Hardware_Init(int argInputType, int argSoundChannels, char *argFontFileName, int M_Drv, int M_IO, int M_IRQ, int M_DMA, int D_Drv, int D_IO, int D_IRQ, int D_DMA);
// _s14p03
void VGA_DAC_Init(char *FontFileName);


#ifdef __cplusplus
}
#endif


#endif /* MOM_DEF_H */
