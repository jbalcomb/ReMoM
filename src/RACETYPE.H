#ifndef RACETYPE_H
#define RACETYPE_H

#include "MoX_TYPE.H"



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