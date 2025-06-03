#ifndef MOX_SET_H
#define MOX_SET_H

#include "MOX_TYPE.H"
#include "MOX_BASE.H"



#define SETTINGS_FILE_NAME "MAGIC.SET"
#define SETTINGS_FILE_SIZE 466



// 1D2h 466d bytes
#pragma pack(push)
#pragma pack(2)
struct s_MAGIC_SET
{
    /* Settings Screen */
    /*  00 */ uint16_t sound_effects;           /* "Sound Effects"          */
    /*  02 */ uint16_t background_music;        /* "Background Music"       */
    /*  04 */ uint16_t event_music;             /* "Event Music"            */
    /*  06 */ uint16_t city_spell_events;       /* "City Spell Events"      */
    /*  08 */ uint16_t overland_spell_events;   /* "Overland Spell Events"  */
    /*  0A */ uint16_t summoning_events;        /* "Summoning Events"       */
    /*  0C */ uint16_t end_of_turn_summary;     /* "End of Turn Summary"    */
    /*  0E */ uint16_t raze_city;               /* "Raze City"              */
    /*  10 */ uint16_t random_events;           /* "Random Events"          */
    /*  12 */ uint16_t end_of_turn_wait;        /* "End of Turn Wait"       */
    /*  14 */ uint16_t strategic_combat_only;   /* "Strategic Combat Only"  */
    /*  16 */ uint16_t auto_unit_information;   /* "Auto Unit Information"  */
    /*  18 */ uint16_t enemy_moves;             /* "Enemy Moves"            */
    /*  1A */ uint16_t enemy_spells;            /* "Enemy Spells"           */
    /*  1C */ uint16_t spell_book_ordering;     /* "Spell Book Ordering";  HELP: "SPELL BOOK ORDERING" "Controls whether spells on your spell book are ordered in categories versus one long list." */
    /*  1E */ uint16_t spell_animations;        /* "Spell Animations"       */
    /*  20 */ uint16_t show_node_owners;        /* "Show Node Owners"       */
    /*  22 */ uint16_t expanding_help;          /* "Expanding Help"         */

    /*  24 */ uint16_t sound_channels;
    /*  26 */ uint16_t input_type;

    /*  28 */ uint16_t Difficulty;
    /*  2A */ uint16_t Opponents;
    /*  2C */ uint16_t LandSize;
    /*  2E */ uint16_t MagicPower;

    /*  30 */ uint16_t Have_Save[NUM_SAVES];
    /*  40 */ char Save_Names[NUM_SAVES][LEN_SAVE_DESCRIPTION];

    /*  E0 */ uint16_t movement_animations;     /* "Movement Animations"    */

    /*  E2 */ char hof_names[NUM_HOF_ENTRIES][LEN_HOF_NAME];
    /* 1AA */ uint16_t hof_scores[NUM_HOF_ENTRIES];
    /* 1BE */ uint16_t hof_races[NUM_HOF_ENTRIES];
};
#pragma pack(pop)



#ifdef __cplusplus
extern "C" {
#endif


// TODO  DEDU  Why do these get C++ name mangled?
extern uint8_t _magic_set[];
extern struct s_MAGIC_SET magic_set;



#ifdef __cplusplus
}
#endif

#endif  /* MOX_SET_H */
