#ifndef UNITTYPE_H
#define UNITTYPE_H

#include "MoX_TYPE.H"

// sizeof=0x24
struct s_UNIT_TYPE
{
    /* 00 */ char ** Name;                   /* ; offset (00036AA0) */
    /* 02 */ int8_t Melee;                  /* ; base 10 */
    /* 03 */ int8_t Ranged;                 /* ; base 10 */
    /* 04 */ int8_t Ranged_Type;
    /* 05 */ int8_t Ammo;                   /* ; base 10 */
    /* 06 */ int8_t To_Hit;                 /* ; base 10 */
    /* 07 */ int8_t Defense;                /* ; base 10 */
    /* 08 */ int8_t Resist;                 /* ; base 10 */
    /* 09 */ int8_t Move_Halves;            /* ; base 10 */
    /* 0A */ int16_t Cost;                  /* ; base 10 */
    /* 0C */ int8_t Upkeep;                 /* ; base 10 */
    /* 0D */ int8_t Race;                   /* ; enum Race_Code */
    /* 0E */ int8_t Bldng1_or_Portrait;     /* ; enum BLD_TYPE */
    /* 0F */ int8_t Bldng2_or_HeroType;     /* ; enum BLD_TYPE */
    /* 10 */ SAMB_ptr pict_seg;             /* ; segment pointers to LBX_Alloc_Space headers for reserved EMM file links */
    /* 12 */ int8_t Hits;                   /* ; base 10 */
    /* 13 */ int8_t Sight;                  /* ; base 10 */
    /* 14 */ int8_t Transport;              /* ; base 10 */
    /* 15 */ int8_t Figures;                /* ; base 10 */
    /* 16 */ int8_t Construction;           /* ; base 10 */
    /* 17 */ int8_t Spec_Att_Attr;          /* ; base 10 */
    /* 18 */ uint16_t Move_Flags;            /* ; enum MOVEFLAGS */
    /* 1A */ uint16_t Attribs_1;             /* ; enum ATTRIB_1 */
    /* 1C */ uint16_t Attribs_2;             /* ; enum ATTRIB_2 */
    /* 1E */ uint16_t Abilities;             /* ; enum ABL_FLAGS */
    /* 20 */ uint16_t Attack_Flags;          /* ; enum ATK_FLAGS */
    /* 22 */ int16_t Sound;
};




/*

    BEGIN: STU-MoXer :: Generate_Unit_Type_Data()

*/
extern char * _unit_type_names[];
extern struct s_UNIT_TYPE _unit_type_table[];



#endif  /* UNITTYPE_H */
