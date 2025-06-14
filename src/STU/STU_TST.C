/*
    SimTex Universe (STU)
    Test

*/

#include "STU_CHK.H"

#include "../MOX/MOX_DAT.H"

#include "../MOM_DEF.H"
#include "../Spellbook.H"
#include "../NEXTTURN.H"

#ifdef STU_DEBUG
#include "STU/STU_DBG.H"
#endif


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



    // scc_Unresistable_Spell  14
    TST_Learn_Spell(player_idx, spl_Mind_Storm);  //   67
    TST_Learn_Spell(player_idx, spl_Web);         //    5
    // scc_Resistable_Spell  13
    // TST_Learn_Spell(player_idx, spl_Black_Sleep);       //  165
    // TST_Learn_Spell(player_idx, spl_Confusion);         //   46
    // TST_Learn_Spell(player_idx, spl_Creature_Binding);  //   73
    // TST_Learn_Spell(player_idx, spl_Vertigo);           //   53
    // TST_Learn_Spell(player_idx, spl_Weakness);          //  162
    // scc_Mundane_Curse  16
    // TST_Learn_Spell(player_idx, spl_Possession);        //  172
    // TST_Learn_Spell(player_idx, spl_Shatter);           //   88

    // scc_Combat_Destroy_Unit  12
    // TST_Learn_Spell(player_idx, spl_Disintegrate);   //  112
    // TST_Learn_Spell(player_idx, spl_Dispel_Evil);    //  134
    // TST_Learn_Spell(player_idx, spl_Petrify);        //   22
    // TST_Learn_Spell(player_idx, spl_Word_Of_Death);  //  192

    // scc_Combat_Banish  23
    // TST_Learn_Spell(player_idx, spl_Banish);  //  64

    // scc_Combat_Counter_Magic  21
    // TST_Learn_Spell(player_idx, spl_Counter_Magic);  //  48

    // Target Wizard
    // TST_Learn_Spell(player_idx, spl_Cruel_Unminding);  // 191
    // TST_Learn_Spell(player_idx, spl_Drain_Power);      // 171
    // TST_Learn_Spell(player_idx, spl_Spell_Blast);      //  58
    // TST_Learn_Spell(player_idx, spl_Subversion);       // 177

    // Spell Bind
    // TST_Learn_Spell(player_idx, spl_Spell_Binding);  // 76

    // Dispel, Disenchant, Disjunct
    // _players[0].Globals[DETECT_MAGIC     ] = (0 + 1);  // 206  spl_Detect_Magic
    // _players[1].Globals[AWARENESS        ] = (1 + 1);  // 209  spl_Awareness
    // _players[1].Globals[DETECT_MAGIC     ] = (1 + 1);  // 206  spl_Detect_Magic
    // // ¿ global enchantments that do not affect other players ? ...negatively? e.g., AURA_OF_MAJESTY
    // _players[2].Globals[AWARENESS        ] = (2 + 1);  // 209  spl_Awareness
    // _players[2].Globals[CHAOS_SURGE      ] = (2 + 1);  // 116  spl_Chaos_Surge
    // _players[2].Globals[CRUSADE          ] = (2 + 1);  // 158  spl_Crusade
    // _players[2].Globals[DOOM_MASTERY     ] = (2 + 1);  // 117  spl_Doom_Mastery
    // _players[2].Globals[HERB_MASTERY     ] = (2 + 1);  //  38  spl_Herb_Mastery
    // _players[2].Globals[JUST_CAUSE       ] = (2 + 1);  // 127  spl_Just_Cause
    // _players[2].Globals[NATURES_AWARENESS] = (2 + 1);  //  34  spl_Natures_Awareness
    // _players[3].Globals[AWARENESS        ] = (3 + 1);  // 209  spl_Awareness
    // _players[4].Globals[AWARENESS        ] = (4 + 1);  // 209  spl_Awareness
    // TST_Learn_Spell(player_idx, spl_Dispel_Magic);
    // TST_Learn_Spell(player_idx, spl_Dispel_Magic_True);
    // TST_Learn_Spell(player_idx, spl_Disenchant_Area);
    // TST_Learn_Spell(player_idx, spl_Disenchant_True);
    // TST_Learn_Spell(player_idx, spl_Disjunction);  // 210
    // TST_Learn_Spell(player_idx, spl_Disjunction_True);

    // Direct Damage - Fixed
    // TST_Learn_Spell(player_idx, spl_Doom_Bolt);
    // TST_Learn_Spell(player_idx, spl_Fire_Storm);
    // TST_Learn_Spell(player_idx, spl_Ice_Storm);
    // TST_Learn_Spell(player_idx, spl_Star_Fires);
    // TST_Learn_Spell(player_idx, spl_Warp_Lightning);

    // Direct Damage - Variable
    // TST_Learn_Spell(player_idx, spl_Fire_Bolt);
    // TST_Learn_Spell(player_idx, spl_Fireball);
    // TST_Learn_Spell(player_idx, spl_Ice_Bolt);
    // TST_Learn_Spell(player_idx, spl_Life_Drain);
    // TST_Learn_Spell(player_idx, spl_Lightning_Bolt);
    // TST_Learn_Spell(player_idx, spl_Psionic_Blast);

    // Overland Unit Enchantments
    // Overland Unit Enchantments - Normal Units Only
    // TST_Learn_Spell(player_idx, spl_Berserk);
    // TST_Learn_Spell(player_idx, spl_Black_Channels);
    // TST_Learn_Spell(player_idx, spl_Bless);
    // TST_Learn_Spell(player_idx, spl_Cloak_Of_Fear);
    // TST_Learn_Spell(player_idx, spl_Eldritch_Weapon);
    // TST_Learn_Spell(player_idx, spl_Elemental_Armor);
    // TST_Learn_Spell(player_idx, spl_Endurance);
    // TST_Learn_Spell(player_idx, spl_Flame_Blade);
    // TST_Learn_Spell(player_idx, spl_Flight);
    // TST_Learn_Spell(player_idx, spl_Giant_Strength);
    // TST_Learn_Spell(player_idx, spl_Guardian_Wind);
    // TST_Learn_Spell(player_idx, spl_Haste);
    // TST_Learn_Spell(player_idx, spl_Heroism);
    // TST_Learn_Spell(player_idx, spl_Holy_Armor);
    // TST_Learn_Spell(player_idx, spl_Holy_Weapon);
    // TST_Learn_Spell(player_idx, spl_Immolation);
    // TST_Learn_Spell(player_idx, spl_Invisibility);
    // TST_Learn_Spell(player_idx, spl_Invulnerability);
    // TST_Learn_Spell(player_idx, spl_Iron_Skin);
    // TST_Learn_Spell(player_idx, spl_Lionheart);
    // TST_Learn_Spell(player_idx, spl_Magic_Immunity);
    // TST_Learn_Spell(player_idx, spl_Path_Finding);
    // TST_Learn_Spell(player_idx, spl_Planar_Travel);
    // TST_Learn_Spell(player_idx, spl_Regeneration);
    // TST_Learn_Spell(player_idx, spl_Resist_Elements);
    // TST_Learn_Spell(player_idx, spl_Resist_Magic);
    // TST_Learn_Spell(player_idx, spl_Righteousness);
    // TST_Learn_Spell(player_idx, spl_Spell_Lock);
    // TST_Learn_Spell(player_idx, spl_Stone_Skin);
    // TST_Learn_Spell(player_idx, spl_True_Sight);
    // TST_Learn_Spell(player_idx, spl_Water_Walking);
    // TST_Learn_Spell(player_idx, spl_Wind_Walking);
    // TST_Learn_Spell(player_idx, spl_Wraith_Form);



    // TST_Learn_Spell(player_idx, spl_Floating_Island);

    // TST_Learn_Spell(player_idx, spl_Animate_Dead);
    // TST_Learn_Spell(player_idx, spl_Raise_Dead);

    // City Enchantment - Positive
    // City Enchantment - Negative
    // TST_Learn_Spell(player_idx, spl_Consecration);
    // TST_Learn_Spell(player_idx, spl_Cursed_Lands);
    // TST_Learn_Spell(player_idx, spl_Famine);
    // TST_Learn_Spell(player_idx, spl_Flying_Fortress);
    // TST_Learn_Spell(player_idx, spl_Gaias_Blessing);
    // TST_Learn_Spell(player_idx, spl_Inspirations);
    // TST_Learn_Spell(player_idx, spl_Natures_Eye);
    // TST_Learn_Spell(player_idx, spl_Prosperity);
    // TST_Learn_Spell(player_idx, spl_Spell_Ward);
    // TST_Learn_Spell(player_idx, spl_Stream_Of_Life);
    // TST_Learn_Spell(player_idx, spl_Summoning_Circle);
    // TST_Learn_Spell(player_idx, spl_Wall_Of_Darkness);
    // TST_Learn_Spell(player_idx, spl_Wall_Of_Fire);
    // TST_Learn_Spell(player_idx, spl_Wall_Of_Stone);

    // TST_Learn_Spell(player_idx, spl_Awareness);
    // TST_Learn_Spell(player_idx, spl_Aura_Of_Majesty);
    // TST_Learn_Spell(player_idx, spl_Holy_Arms);
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
