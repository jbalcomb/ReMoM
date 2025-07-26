/*
    SimTex Universe (STU)
    Test

*/

#include "STU_CHK.h"
#include "STU_DBG.h"

#include "../MOX/MOX_DAT.h"

#include "../MOM_DEF.h"
#include "../Spellbook.h"
#include "../NEXTTURN.h"



void TST_Cheat_Power(int16_t player_idx)
{

    // Alt-PWR Cheat_Power()
    // _players[itr1].mana_reserve = 10000;
    // _players[itr1].gold_reserve = 10000;
    // _players[itr1].spell_casting_skill = 10000;
    // _players[itr1].Nominal_Skill = Player_Base_Casting_Skill(itr1);
    // _players[itr1].Skill_Left = _players[itr1].Nominal_Skill;

    _players[player_idx].gold_reserve = 32767;  // MAX_SINT32

    // NOWORKIE  "-1", because of Print_Integer_Right()  _players[player_idx].mana_reserve = 65535;  // MAX_UINT16
    _players[player_idx].mana_reserve = 32767;  // MAX_SINT32

    _players[player_idx].spell_casting_skill = 32767;  // MAX_SINT32
    _players[player_idx].Nominal_Skill = Player_Base_Casting_Skill(player_idx);  // 181
    _players[player_idx].Skill_Left = _players[player_idx].Nominal_Skill;

}

void TST_Learn_Spell(int16_t player_idx, int16_t spell_idx)
{
    int16_t spell_realm_idx = 0;
    int16_t spell_realm = 0;

    spell_realm = ((spell_idx - 1) / NUM_SPELLS_PER_MAGIC_REALM);
    spell_realm_idx = ((spell_idx - 1) % NUM_SPELLS_PER_MAGIC_REALM);
    _players[player_idx].spells_list[((spell_realm * NUM_SPELLS_PER_MAGIC_REALM) + spell_realm_idx)] = sls_Known;

}

void TST_Patch_Game_Data(void)
{
    int16_t player_idx = 0;
    int16_t spell_idx = 0;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: TST_Patch_Game_Data()\n", __FILE__, __LINE__);
#endif

    player_idx = HUMAN_PLAYER_IDX;
    
    // TST_Cheat_Power(player_idx);

    // TST_Learn_Spell(player_idx, spl_Dispel_Magic);

    // _CITIES[0].enchantments[FLYING_FORTRESS] = ST_TRUE;
    // _CITIES[0].enchantments[CHAOS_WARD] = ST_TRUE;
    // _players[0].Globals[DETECT_MAGIC     ] = (0 + 1);  // 206  spl_Detect_Magic
    // _players[1].Globals[AWARENESS        ] = (1 + 1);  // 209  spl_Awareness

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: TST_Patch_Game_Data()\n", __FILE__, __LINE__);
#endif
}
