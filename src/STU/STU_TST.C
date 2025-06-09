/*
    SimTex Universe (STU)
    Test

*/

#include "../MOX/MOX_DAT.H"
#include "../MOX/MOX_TYPE.H"

#include "../MOM_DEF.H"

#include "../Spellbook.H"


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

    
    TST_Cheat_Power(player_idx);

    // City Enchantment
    TST_Learn_Spell(player_idx, spl_Consecration);
    TST_Learn_Spell(player_idx, spl_Cursed_Lands);
    TST_Learn_Spell(player_idx, spl_Famine);
    TST_Learn_Spell(player_idx, spl_Flying_Fortress);
    TST_Learn_Spell(player_idx, spl_Gaias_Blessing);
    TST_Learn_Spell(player_idx, spl_Inspirations);
    TST_Learn_Spell(player_idx, spl_Natures_Eye);
    TST_Learn_Spell(player_idx, spl_Prosperity);
    TST_Learn_Spell(player_idx, spl_Spell_Ward);
    TST_Learn_Spell(player_idx, spl_Stream_Of_Life);
    TST_Learn_Spell(player_idx, spl_Summoning_Circle);
    TST_Learn_Spell(player_idx, spl_Wall_Of_Darkness);
    TST_Learn_Spell(player_idx, spl_Wall_Of_Fire);
    TST_Learn_Spell(player_idx, spl_Wall_Of_Stone);

    // TST_Learn_Spell(player_idx, spl_Animate_Dead);
    // TST_Learn_Spell(player_idx, spl_Awareness);
    // TST_Learn_Spell(player_idx, spl_Aura_Of_Majesty);
    // TST_Learn_Spell(player_idx, spl_Floating_Island);
    // TST_Learn_Spell(player_idx, spl_Holy_Arms);
    // TST_Learn_Spell(player_idx, spl_Raise_Dead);
    // TST_Learn_Spell(player_idx, spl_Natures_Awareness);

    // Cityscape_Window() - Farbackground
    // _CITIES[0].enchantments[FLYING_FORTRESS] = ST_TRUE;
    // _CITIES[0].enchantments[CURSED_LANDS] = ST_TRUE;
    // _CITIES[0].enchantments[FAMINE] = ST_TRUE;
    // _CITIES[0].enchantments[GAIAS_BLESSING] = ST_TRUE;

    // Cityscape_Draw_Background()
    // _CITIES[0].enchantments[CHAOS_RIFT] = ST_TRUE;
    // _CITIES[0].enchantments[CLOUD_OF_SHADOW] = ST_TRUE;
    // _CITIES[0].enchantments[HEAVENLY_LIGHT] = ST_TRUE;
    
    // Cityscape_Draw_Foreground()
    // _CITIES[0].enchantments[DEATH_WARD] = ST_TRUE;
    // _CITIES[0].enchantments[CHAOS_WARD] = ST_TRUE;
    // _CITIES[0].enchantments[NATURE_WARD] = ST_TRUE;
    // _CITIES[0].enchantments[LIFE_WARD] = ST_TRUE;
    // _CITIES[0].enchantments[SORCERY_WARD] = ST_TRUE;
    // _CITIES[0].enchantments[NATURES_EYE] = ST_TRUE;
    // _CITIES[0].enchantments[INSPIRATIONS] = ST_TRUE;
    // _CITIES[0].enchantments[PROSPERITY] = ST_TRUE;
    // _CITIES[0].enchantments[CONSECRATION] = ST_TRUE;
    // _CITIES[0].enchantments[WALL_OF_DARKNESS] = ST_TRUE;
    // _CITIES[0].enchantments[WALL_OF_FIRE] = ST_TRUE;
Capture_Cities_Data();



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: TST_Patch_Game_Data()\n", __FILE__, __LINE__);
#endif
}
