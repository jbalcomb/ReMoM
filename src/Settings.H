/*
    WIZARDS.EXE
        ovr125
    MAGIC.EXE

WZD
MGC

MoO2
    Module: FILEDEF
    Module: INITGAME
    Module: HAROLD
    Module: LOADSAVE
    Module: RUSS

*/

#ifndef SETTINGS_H
#define SETTINGS_H

#include "MoM.H"

#ifdef __cplusplus
extern "C" {
#endif



// 1D2h 466d bytes
struct s_MAGIC_SET
{
    /* Settings Screen */
    /*  00 */ uint16_t SoundFX;
    /*  02 */ uint16_t BG_Music;
    /*  04 */ uint16_t Event_Music;
    /*  06 */ uint16_t City_Spell_Events;
    /*  08 */ uint16_t Ovl_Spell_Events;
    /*  0A */ uint16_t Summoning_Events;
    /*  0C */ uint16_t EoT_Summary;
    /*  0E */ uint16_t Raze_City;
    /*  10 */ uint16_t Random_Events;
    /*  12 */ uint16_t EoT_Wait;
    /*  14 */ uint16_t Strategic_Combat;
    /*  16 */ uint16_t Auto_Unit_Info;
    /*  18 */ uint16_t Enemy_Moves;
    /*  1A */ uint16_t Enemy_Spells;
    /*  1C */ uint16_t Spell_Book_Order;
    /*  1E */ uint16_t Spell_Anims;
    /*  20 */ uint16_t Show_Node_Owners;
    /*  22 */ uint16_t Expanding_Help;

    /*  24 */ uint16_t Snd_Channels;
    /*  26 */ uint16_t Input_Type;

    /*  28 */ uint16_t Difficulty;
    /*  2A */ uint16_t Opponents;
    /*  2C */ uint16_t LandSize;
    /*  2E */ uint16_t MagicPower;

    /*  30 */ uint16_t Have_Save[NUM_SAVES];
    /*  40 */ char Save_Names[NUM_SAVES][LEN_SAVE_DESCRIPTION];

    /*  E0 */ uint16_t Movement_Anims;

    /*  E2 */ char hof_names[NUM_HOF_ENTRIES][LEN_HOF_NAME];
    /* 1AA */ uint16_t hof_scores[NUM_HOF_ENTRIES];
    /* 1BE */ uint16_t HallofFame_Races[NUM_HOF_ENTRIES];
};



extern uint8_t _magic_set[];
extern struct s_MAGIC_SET magic_set;



#define SETTINGS_FILE_NAME "MAGIC.SET"
#define SETTINGS_FILE_SIZE 466



void Read_MAGIC_SET(void);
void Write_MAGIC_SET(void);



/*
    WIZARDS.EXE  ovr125
*/

// WZD o125p01
void Settings_Screen(void);

// WZD o125p02
void Settings_Screen_Draw(void);

// WZD o125p03
void Load_MAGIC_SET(void);

// WZD o125p04
void Set_Default_Game_Settings(void);

// WZD o125p05
void Set_Settings_Screen_Help_List(void);



#ifdef __cplusplus
}
#endif

#endif  /* SETTINGS_H */
