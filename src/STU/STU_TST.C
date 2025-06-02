/*
    SimTex Universe (STU)
    Test

*/

#include "../MOX/MOX_TYPE.H"

#include "../MOM_DEF.H"

#include "../Spellbook.H"


void TST_Learn_Spell(int16_t player_idx, int16_t spell_idx)
{
    int16_t InRealm_Index;
    int16_t Spell_Realm;

    Spell_Realm = ((spell_idx - 1) / NUM_SPELLS_PER_MAGIC_REALM);
    InRealm_Index = ((spell_idx - 1) % NUM_SPELLS_PER_MAGIC_REALM);
    _players[player_idx].spells_list[((Spell_Realm * NUM_SPELLS_PER_MAGIC_REALM) + InRealm_Index)] = 2;  /* S_Known */

}

void TST_Patch_Game_Data(void)
{
    int16_t player_idx;
    int16_t spell_idx;
    int16_t InRealm_Index;
    int16_t Spell_Realm;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: TST_Patch_Game_Data()\n", __FILE__, __LINE__);
#endif


    player_idx = HUMAN_PLAYER_IDX;


    _players[player_idx].spell_casting_skill = 32767;  // ¿ MAX_SINT32 ?

    _players[player_idx].mana_reserve = 65535;  // ¿ MAX_UINT16 ?

    
    TST_Learn_Spell(player_idx, spl_Floating_Island);

    TST_Learn_Spell(player_idx, spl_Raise_Dead);

    TST_Learn_Spell(player_idx, spl_Animate_Dead);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: TST_Patch_Game_Data()\n", __FILE__, __LINE__);
#endif
}
