/*
    SimTex Universe (STU)
    Test

*/

#include "../MOX/MOX_TYPE.H"

#include "../MOM_DEF.H"

#include "../Spellbook.H"



void TST_Patch_Game_Data(void)
{
    int16_t player_idx;
    int16_t spell_idx;
    int16_t InRealm_Index;
    int16_t Spell_Realm;

    player_idx = HUMAN_PLAYER_IDX;

    spell_idx = spl_Floating_Island;

    Spell_Realm = ((spell_idx - 1) / NUM_SPELLS_PER_MAGIC_REALM);

    InRealm_Index = ((spell_idx - 1) % NUM_SPELLS_PER_MAGIC_REALM);

    _players[player_idx].spells_list[((Spell_Realm * NUM_SPELLS_PER_MAGIC_REALM) + InRealm_Index)] = 2;  /* S_Known */

    _players[player_idx].spell_casting_skill = 32767;  // ¿ MAX_SINT32 ?

    _players[player_idx].mana_reserve = 65535;  // ¿ MAX_UINT16 ?

}
