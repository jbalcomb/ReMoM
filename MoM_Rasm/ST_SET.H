#ifndef ST_CRSR_H
#define ST_CRSR_H

#include "MoX_TYPE.H"    /* word */

// 14h 20d bytes
struct s_SAVENAME
{
    char Name[20];
};

// 1D2h 466d bytes
struct s_MAGIC_SET
{
    /* 00*/ word SoundFX;
    /* 02*/ word BG_Music;
    /* 04*/ word Event_Music;
    /* 06*/ word City_Spell_Events;
    /* 08*/ word Ovl_Spell_Events;
    /* 0A*/ word Summoning_Events;
    /* 0C*/ word EoT_Summary;
    /* 0E*/ word Raze_City;
    /* 10*/ word Random_Events;
    /* 12*/ word EoT_Wait;
    /* 14*/ word Strategic_Combat;
    /* 16*/ word Auto_Unit_Info;
    /* 18*/ word Enemy_Moves;
    /* 1A*/ word Enemy_Spells;
    /* 1C*/ word Spell_Book_Order;
    /* 1E*/ word Spell_Anims;
    /* 20*/ word Show_Node_Owners;
    /* 22*/ word Expanding_Help;
    /* 24*/ word Snd_Channels;
    /* 26*/ word Input_Type;
    /* 28*/ word Difficulty;
    /* 2A*/ word Opponents;
    /* 2C*/ word LandSize;
    /* 2E*/ word MagicPower;
    /* 30*/ word Using_Saves[8];
    /* 40*/ struct s_SAVENAME Save_Names[8];
    /* E0*/ word Movement_Anims;
    /* E2*/ struct s_SAVENAME HallofFame_Names[10];
    /*1AA*/ word HallofFame_Scores[10];
    /*1BE*/ word HallofFame_Races[10];
};

struct s_MAGIC_SET magic_set;


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


void Load_MAGIC_SET(void);


#ifdef __cplusplus
}
#endif

#endif /* ST_CRSR_H */
