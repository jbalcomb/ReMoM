/*
    Overland
    Combat

    Spell Cast Category
    Spell Target Type

*/
#ifndef SPELLDAT_H
#define SPELLDAT_H



/*
    (Overland) Spell Target Type  (STT)

vs. Combat Spell Target Type  (CSTT)
*/
enum e_OVERLAND_SPELL_TARGET_TYPE
{
    stt_Friendly_Unit   = 0,
    stt_Friendly_Group  = 1,
    stt_Enemy_Unit      = 2,
    stt_Enemy_Group     = 3,
    stt_Map_Square      = 4,
    stt_Friendly_City   = 5,
    stt_Enemy_City      = 6,
    stt_Magic_Node      = 7
};
enum e_COMBAT_SPELL_TARGET_TYPE
{
    cstt_Tile_NoUnit   =  1,
    cstt_Tile          =  2,
    cstt_Wall          =  3,
    cstt_Tile_NoUnitA  =  4,
    cstt_Tile_NoUnitD  =  5,

    cstt_EnemyUnit     = 10,
    cstt_EnemyNU       = 11,

    cstt_FriendlyUnit  = 20,
    cstt_FriendlyNU    = 21,

    cstt_FriendlyHero  = 23,

    cstt_DispelMagic   = 30
};



#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif

#endif  /* SPELLDAT_H */
