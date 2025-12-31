#ifndef RACETYPE_H
#define RACETYPE_H

#include "../../MoX/src/MOX_TYPE.h"



/*
21 race type  {0,...,20}

at some point, 46 + rt is used to index SPELLSCR
but, the names and descriptions are incongruent/misaligned

*/
// drake178: enum Race_Code
enum e_RACE_TYPE
{
    rt_Barbarian  = 0,
    rt_Beastmen   = 1,
    rt_Dark_Elf   = 2,
    rt_Draconian  = 3,
    rt_Dwarf      = 4,
    rt_Gnoll      = 5,
    rt_Halfling   = 6,
    rt_High_Elf   = 7,
    rt_High_Man   = 8,
    rt_Klackon    = 9,
    rt_Lizardman  = 10,
    rt_Nomad      = 11,
    rt_Orc        = 12,
    rt_Troll      = 13,
    rt_Standard   = 14,
    rt_Arcane     = 15,     /* SPELLSCR.LBX, 059  "SUMMGREY"    "" */
    rt_Nature     = 16,     /* SPELLSCR.LBX, 060  "SUMGREEN"    "" */
    rt_Sorcery    = 17,     /* SPELLSCR.LBX, 061  "SUMMBLUE"    "" */
    rt_Chaos      = 18,     /* SPELLSCR.LBX, 062  "SUMMRED"     "" */
    rt_Life       = 19,     /* SPELLSCR.LBX, 063  "SUMWHITE"    "" */
    rt_Death      = 20      /* SPELLSCR.LBX, 064  "SUMMPURP"    "" */
};



// sizeof=0x18  24
struct s_RACE_TYPE
{
    /* 00 */ char ** name;
    /* 02 */ int16_t cant_build_count;
    /* 04 */ int16_t cant_build[7];  // enum BLD_TYPE
    /* 12 */ int16_t outpost_growth_rate;  // base 10
    /* 14 */ int16_t Growth_Mod;
    /* 16 */ int16_t house_type;  /* {0: standard, 1: tree house, 2: mound} */
    /* 18 */
};

// HOUSES   tree
// HOUSES   rocks
// HOUSES   house
// HOUSES   tree house
// HOUSES   mound


extern char * _race_type_names[21];

extern struct s_RACE_TYPE _race_type_table[14];



#endif  /* RACETYPE_H */