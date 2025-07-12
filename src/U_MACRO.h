
/*
    Accessor Macros - MoX & MoM

Required Local Variables:
    _UNITS[]    unit_idx  ... ¿ itr_units `< _units`

SEE: C:\STU\devel\ReMoM\doc\Rasm-Accessor-Macros.md
*/


/*

*/




/*

    Unit Macros

*/

/*

¿ combat_data vs. global_combat_data ?

¿ s_BATTLE_UNIT vs s_CMBT_DATA ?

*/
#define BU_LUCKY()      { ((strategic_unit->Attribs_1 & 0x0400) != 0) }
#define BU_CASTER_20()  { ((strategic_unit->Attribs_1 & 0x2000) != 0) }
#define BU_CASTER_40()  { ((strategic_unit->Attribs_1 & 0x4000) != 0) }

