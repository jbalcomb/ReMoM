#ifndef MOX_SET_H
#define MOX_SET_H

#include "MoX_TYPE.H"
#include "MoX_BASE.H"



#define SETTINGS_FILE_NAME "MAGIC.SET"
#define SETTINGS_FILE_SIZE 466



// 1D2h 466d bytes
struct s_MAGIC_SET
{
    /* Settings Screen */
    /*  00 */ uint16_t sound_effects;
    /*  02 */ uint16_t background_music;
    /*  04 */ uint16_t event_music;
    /*  06 */ uint16_t City_Spell_Events;
    /*  08 */ uint16_t Ovl_Spell_Events;
    /*  0A */ uint16_t Summoning_Events;
    /*  0C */ uint16_t end_of_turn_summary;
    /*  0E */ uint16_t Raze_City;
    /*  10 */ uint16_t random_events;
    /*  12 */ uint16_t EoT_Wait;
    /*  14 */ uint16_t strategic_combat_only;
    /*  16 */ uint16_t auto_unit_information;  /* Settings Screen: "Auto Unit Information" */
    /*  18 */ uint16_t Enemy_Moves;
    /*  1A */ uint16_t Enemy_Spells;
    /*  1C */ uint16_t spell_book_ordering;  /* HELP: "SPELL BOOK ORDERING" "Controls whether spells on your spell book are ordered in categories versus one long list." */
    /*  1E */ uint16_t spell_animations;
    /*  20 */ uint16_t Show_Node_Owners;
    /*  22 */ uint16_t Expanding_Help;

    /*  24 */ uint16_t sound_channels;
    /*  26 */ uint16_t input_type;

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



#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

#endif  /* MOX_SET_H */
