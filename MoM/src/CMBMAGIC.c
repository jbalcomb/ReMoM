/*
    Combat

    WIZARDS.EXE
        ovr112  Combat Spellbook ~ CMBTSPBK?  ¿ CMBMAGIC ?

*/

#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"

#include "../../ext/stu_compat.h"

#include "../../MoX/src/angle.h"
#include "../../MoX/src/EXIT.h"
#include "../../MoX/src/paragrph.h"
#include "../../MoX/src/Timer.h"
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/GENDRAW.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/Help.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_BASE.h"
#include "../../MoX/src/MOX_DAT.h"  /* _players, _screen_seg */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_SET.h"  /* magic_set */
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/special.h"
#include "../../MoX/src/Util.h"

#include "DIPLODEF.h"
#include "City_ovr55.h"
#include "CITYCALC.h"
#include "DIPLOMAC.h"
#include "Items.h"
#include "Lair.h"
#include "LOADER.h"
#include "MainScr.h"
#include "MainScr_Maps.h"
#include "MOM_DBG.h"
#include "NEXTTURN.h"
#include "RACETYPE.h"
#include "SBookScr.h"
#include "Spellbook.h"  /* spl_Doom_Bolt;  Combat_Spellbook_Build() */
#include "SPELLDEF.h"
#include "Spells131.h"
#include "Spells133.h"
#include "SPLMASTR.h"
#include "Terrain.h"
#include "TerrType.h"
#include "UnitMove.h"   // WTFMATE
#include "UnitStat.h"
#include "UNITTYPE.h"   /* rag_Missile */
#include "UnitView.h"
#include "WZD_o059.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>     /* memcpy() memset(), stu_strcat(), stu_strcpy(), stricmp() */

#include "CMBMAGIC.h"
#include "CMBTAI.h"
#include "CMBTDEF.h"
#include "CMBTMVPT.h"
#include "Combat.h"



// WZD dseg:5AF2                                                 BEGIN:  ovr112 - Initialized Data

// WZD dseg:5AF2
char cnst_Counter_Magic[] = "Counter Magic";
// WZD dseg:5AFF
char str_empty_string__ovr112[] = "";
// WZD dseg:5B00
char hlpentry_lbx_file__ovr112[] = "hlpentry";
// WZD dseg:5B09
char str_hotkey_F__ovr112[] = "F";
// WZD dseg:5B0B
char str_hotkey_B__ovr112[] = "B";
// WZD dseg:5B0D 1B 00                                           str_hotkey_ESC__ovr112 db 1Bh,0              ; DATA XREF: Combat_Spellbook_Screen+6E0o
// WZD dseg:5B0F
char message_lbx_file__ovr112[] = "message";
// WZD dseg:5B17 00                                              align 2

// WZD dseg:5B17                                                 END:  ovr112 - Initialized Data



// WZD dseg:C896                                                 ¿ BEGIN:  ovr112 ?

/*
NIU_Who_Has_More_Leadership()  ...in ovr098
CMB_LoadResources__WIP()
Strategic_Combat_Allocate()
BU_Apply_Battlefield_Effects__WIP()
Init_Battlefield_Effects()

CMB_PrepareTurn__WIP()
CMB_LoadResources__WIP()
Strategic_Combat_Allocate()
BU_Apply_Battlefield_Effects__WIP()
Init_Battlefield_Effects()

CMB_LoadResources__WIP()
Strategic_Combat_Allocate()
BU_Apply_Battlefield_Effects__WIP()
Init_Battlefield_Effects()
UNIT_ConvSpellATK()
UNIT_ConvSpellATK()


Holy Bonus, Leadership, Prayer Master
"...only the best applies..."
"...only the highest bonus applies..."
"...Prayer Master or Resistance to All..."
    attack strength (swords), defense (shields) and resistance (crosses) of all friendly units in combat by bonus level of holy bonus

¿ indexed by PLAYER_NUM() ?

*/
// WZD dseg:C896
// WZD dseg:C89A
// WZD dseg:C89E
int16_t * _battlefield_leadership;
int16_t * _battlefield_resistall;
int16_t * _battlefield_holybonus;

// WZD dseg:C8A2
int16_t * CMB_IDK_4PR;
// WZD dseg:C8A6
int16_t _combat_spell_target_type;
// WZD dseg:C8A8
int16_t * CMB_NearDispel_UCs;
// WZD dseg:C8AA
int16_t * CMB_NearDispel_UEs;
// WZD dseg:C8AC
uint16_t _combat_caster_idx;

// WZD dseg:C8AC                                                 ¿ END:  ovr112 ?




/*
    WIZARDS.EXE  ovr112
*/

// WZD o112p01
void Combat_Compose_Spellbook_Background(void)
{
    Combat_Compose_Background();  // ... |-> Copy_Off_To_Back();
    Set_Page_Off();
    Combat_Screen_Draw();
    FLIC_Draw(16, 10, _spellbook_small_seg);
    Copy_Off_To_Back();
}


// WZD o112p02
/*
~== Spellbook_Screen_Draw()
    |-> SmlBook_Draw(16, 10);
*/
void Combat_Spellbook_Screen_Draw(void)
{
    Copy_Back_To_Off();
    Combat_Spellbook_Draw(16, 10, _combat_caster_idx);
}


// WZD o112p03
/**
 * @brief Resolves a combat spell cast for a human player, AI player, or battle unit ability.
 *
 * @details
 * This is the main combat-spell dispatch path. It determines which spell is being
 * cast, optionally opens the combat spellbook for a human caster, applies legality
 * checks, handles Counter Magic / combat dispel interception, selects a target, and
 * then applies the spell effect through Cast_Spell_On_Battle_Unit().
 *
 * The function also handles several special cases:
 * - Battle-unit spell-like abilities such as Doom Bolt, Fireball, Web, Healing, and
 *   Summon Demon.
 * - Human cast flow through the combat spellbook and target selection screens.
 * - AI combat casting, including item-based spell use.
 * - Mana, skill, and movement-point adjustments after a cast resolves or is countered.
 *
 * @param caster_idx
 * The casting entity. Values below CASTER_IDX_BASE identify a battle unit index;
 * values at or above CASTER_IDX_BASE identify a player casting directly.
 * @param wx
 * The world X coordinate of the combat location. Used when the cast flow needs to
 * resolve or re-validate battlefield context.
 * @param wy
 * The world Y coordinate of the combat location. Used when the cast flow needs to
 * resolve or re-validate battlefield context.
 * @param wp
 * The plane index for the combat location.
 *
 * @return The combat cast status.
 * @return ST_FALSE if the spell cannot be initiated or a summon fails.
 * @return ST_TRUE for early-success paths such as a resolved summon ability.
 * @return 2 when a spell is selected and resolved, including countered casts.
 *
 * @note This routine mutates global combat state, battle-unit state, player mana,
 * and combat UI back buffers as part of the cast flow.
 * @note The function preserves several legacy quirks documented inline, including
 * known cost, counter, and targeting inconsistencies.
 */
int16_t Combat_Cast_Spell(int16_t caster_idx, int16_t wx, int16_t wy, int16_t wp)
{
    int16_t battle_unit_mana = 0;
    int16_t is_spell_like_ability = 0;  /* set to 1 if the human player uses a Spell Ability */
    int16_t cast_status = 0;
    int16_t counter_player_idx = 0;
    int16_t base_mana_cost = 0;
    int16_t extra_mana_cost = 0;
    int16_t player_idx = 0;
    int16_t target_cgy = 0;
    int16_t counter_magic_idx__target_cgx = 0;  // AKA counter_magic_idx, target_cgx, counter_realm, cost_multiplier
    int16_t counter_realm = 0;
    int16_t cost_multiplier = 0;
    int16_t Target = 0;
    int16_t selected_page_slot = 0;
    int16_t effective_cost = 0;
    int16_t overland_spell_idx_saved = 0;
    int16_t has_castable_spell = 0;  // AKA saved_casting_cost_remaining
    int16_t saved_casting_cost_remaining = 0;
    int16_t spell_idx = 0;
    int16_t available_mana_pool = 0;  // AKA total_mana_cost
    int16_t total_mana_cost = 0;
    cast_status = ST_TRUE;
    is_spell_like_ability = ST_FALSE;
    _combat_caster_idx = caster_idx;
    if(caster_idx >= CASTER_IDX_BASE)
    {
        player_idx = (caster_idx - CASTER_IDX_BASE);
    }
    else
    {
        player_idx = battle_units[caster_idx].controller_idx;
        battle_unit_mana = battle_units[caster_idx].mana;
    }
    if(caster_idx == (CASTER_IDX_BASE + NEUTRAL_PLAYER_IDX))
    {
        return cast_status;
    }
    if(
        (player_idx == HUMAN_PLAYER_IDX)
        &&
        (_auto_combat_flag == ST_FALSE)
    )
    {
        spell_idx = spl_NONE;
        is_spell_like_ability = ST_FALSE;
        /*
            BEGIN:  Caster is Battle Unit
        */
        if(caster_idx < CASTER_IDX_BASE)
        {
            if((battle_units[caster_idx].Attribs_2 & USA_DOOMBOLT) != 0)
            {
                spell_idx = spl_Doom_Bolt;
                is_spell_like_ability = ST_TRUE;
            }
            if((battle_units[caster_idx].Attribs_2 & USA_FIREBALL) != 0)
            {
                spell_idx = spl_Fireball;
                is_spell_like_ability = ST_TRUE;
            }
            if((battle_units[caster_idx].Attribs_2 & USA_WEB) != 0)
            {
                spell_idx = spl_Web;
                is_spell_like_ability = ST_TRUE;
            }
            if((battle_units[caster_idx].Attribs_2 & USA_HEALING) != 0)
            {
                spell_idx = spl_Healing;
                is_spell_like_ability = ST_TRUE;
            }
            if((battle_units[caster_idx].Attribs_2 & (USA_SUMMON_DEMON_1 | USA_SUMMON_DEMON_2)) != 0)
            {
                if(_units < MAX_UNIT_COUNT)
                {
                    BU_SummonDemon__SEGRAX(caster_idx);
                    battle_units[caster_idx].movement_points = 0;
                    return ST_TRUE;
                }
                else
                {
                    return ST_FALSE;
                }
            }

        }
        /*
            END:  Caster is Battle Unit
        */
        // HERE:  caster is player or handled unit abilities
        // spell_idx could be Doom Bolt, Fireball, Web, or Healing
        if(spell_idx == spl_NONE)
        {
            has_castable_spell = Combat_Spellbook_Build(caster_idx);
            if(has_castable_spell == ST_FALSE)
            {
                return ST_FALSE;
            }
            do {
                Combat_Compose_Spellbook_Background();  // ... |-> Copy_Off_To_Back();
                // selected_page_slot@  index on page of selected spell
                // ...gets passed to Combat_Spellbook_Mana_Adder_Screen()
                // g_combat_spellbook_left_page is the associated page number
                spell_idx = Combat_Spellbook_Screen(caster_idx, &selected_page_slot);
                // prep for going right back to the combat screen
                if(
                    (spell_idx <= 0)
                    ||
                    (spell_data_table[spell_idx].type < scc_Infusable_Spell)
                )
                {
                    _page_flip_effect = pfe_Dissolve;
                    Combat_Compose_Spellbook_Background();  // ... |-> Copy_Off_To_Back();
                    // where's the rest of the screen update?
                }
            } while(Do_Legal_Spell_Check__WIP(spell_idx) != ST_FALSE);
            // ...not illegal...
            // not doing xtramana popup, so go right back to the combat screen
            if(
                (spell_data_table[spell_idx].type < scc_Infusable_Spell)
                ||
                (is_spell_like_ability == ST_TRUE)  // ; conflicting condition - will always jump
            )
            {
                Combat_Compose_Background();  // ... |-> Copy_Off_To_Back();
                Set_Page_Off();
                Combat_Screen_Draw();
                PageFlip_FX();
            }
        }
    }
    else
    {
        /* AI / Auto Combat Control path */
        AI_SetCombatRealms();
        if(caster_idx < CASTER_IDX_BASE &&
            battle_units[caster_idx].Item_Charges > 0 &&
            battle_units[caster_idx].action == BUA_UseItem)
        {
            spell_idx = _ITEMS[_players[player_idx].Heroes[_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot].Items[0]].embed_spell_idx;
        }
        else
        {
            spell_idx = AI_SelectCmbtSpell(caster_idx);
        }
        if(spell_idx > 0)
        {
            if(caster_idx < CASTER_IDX_BASE)
            {
                battle_units[caster_idx].target_battle_unit_idx = caster_idx;
                if(battle_units[caster_idx].controller_idx != HUMAN_PLAYER_IDX)
                {
                    _ai_immobile_counter = ST_UNDEFINED;
                }
            }
        }
    }
    /*
        BEGIN:  
    */
    if(spell_idx != spl_NONE)
    {
        /*
            BEGIN:  ¿ effective_cost ?
        */
        if(
            (spell_data_table[spell_idx].type < scc_Infusable_Spell)
            ||
            (is_spell_like_ability != ST_FALSE)
        )
        {
            if(caster_idx > CASTER_IDX_BASE)
            {
                // ; BUG: causes counters to use the effective cost
                available_mana_pool = Casting_Cost(player_idx, spell_idx, 1);
            }
            else
            {
                /* OGBUG  ignores Evil Omens */
                available_mana_pool = spell_data_table[spell_idx].casting_cost;
            }
            effective_cost = available_mana_pool;
        }
        else  /* (spell_data_table[spell_idx].type >= scc_Infusable_Spell) && (is_spell_like_ability == ST_FALSE) */
        {
            if(
                (player_idx == HUMAN_PLAYER_IDX)
                &&
                (_auto_combat_flag == ST_FALSE)
            )
            {
                saved_casting_cost_remaining = _players[HUMAN_PLAYER_IDX].casting_cost_remaining;
                overland_spell_idx_saved = _players[HUMAN_PLAYER_IDX].casting_spell_idx;
                _players[HUMAN_PLAYER_IDX].casting_spell_idx = spell_idx;
                if(caster_idx >= CASTER_IDX_BASE)
                {
                    _players[HUMAN_PLAYER_IDX].casting_cost_remaining = Casting_Cost(HUMAN_PLAYER_IDX, spell_idx, 1);
                }
                else
                {
                    /* OGBUG  ignores Evil Omens */
                    _players[HUMAN_PLAYER_IDX].casting_cost_remaining = spell_data_table[spell_idx].casting_cost;
                }
                available_mana_pool = Combat_Spellbook_Mana_Adder_Screen(spell_idx, selected_page_slot, caster_idx);
                if(caster_idx >= CASTER_IDX_BASE)
                {
                    effective_cost = (available_mana_pool - ((available_mana_pool * Casting_Cost_Reduction(player_idx, spell_idx)) / 100));
                }
                else
                {
                    effective_cost = available_mana_pool;
                }
                _players[HUMAN_PLAYER_IDX].casting_cost_remaining = saved_casting_cost_remaining;
                _players[HUMAN_PLAYER_IDX].casting_spell_idx = overland_spell_idx_saved;
                Combat_Compose_Background();  // ... |-> Copy_Off_To_Back();
                Set_Page_Off();
                Combat_Screen_Draw();
                PageFlip_FX();
            }
            else  /* (player_idx != HUMAN_PLAYER_IDX) || (_auto_combat_flag != ST_FALSE) */
            {
                if(caster_idx >= CASTER_IDX_BASE)
                {
                    //  ; BUG: ignores the base casting cost
                    available_mana_pool = _players[player_idx].Cmbt_Skill_Left;
                    // ; BUG: ignores casting cost modifiers
                    if(spell_data_table[spell_idx].casting_cost < available_mana_pool)
                    {
                        available_mana_pool = spell_data_table[spell_idx].casting_cost;
                    }
                    extra_mana_cost = Combat_Casting_Cost_Multiplier(player_idx);
                    // ; BUG: ignores casting cost modifiers
                    if(((_players[player_idx].mana_reserve * 10) / extra_mana_cost) < spell_data_table[spell_idx].casting_cost)
                    {
                        // ; BUG: ignores casting cost modifiers
                        available_mana_pool = (((_players[player_idx].mana_reserve * 10) / extra_mana_cost) - spell_data_table[spell_idx].casting_cost);
                    }
                }
                else
                {
                    /* OGBUG  ignores Evil Omens */
                    /* OGBUG  infusable item charges will be automatically countered if the user has no mana */
                    available_mana_pool = (battle_unit_mana - spell_data_table[spell_idx].casting_cost);
                    if(spell_data_table[spell_idx].casting_cost < available_mana_pool)
                    {
                        available_mana_pool = spell_data_table[spell_idx].casting_cost;
                    }
                }
                base_mana_cost = spell_data_table[spell_idx].casting_cost;
                extra_mana_cost = 0;
                // ; BUG: Banish has an effective gain of 1/15 mana, not 5
                if(
                    (spell_idx == spl_Life_Drain)
                    ||
                    (spell_idx == spl_Banish)
                )
                {
                    extra_mana_cost = ((Random(((available_mana_pool / 5) + 1)) - 1) * 5);
                }
                else if(spell_idx == spl_Counter_Magic)
                {
                    extra_mana_cost = ((Random(((available_mana_pool / 5) + 1)) - 1) * 5);
                }
                else
                {
                    extra_mana_cost = available_mana_pool;
                }
                total_mana_cost = (base_mana_cost + extra_mana_cost);
                if(caster_idx > CASTER_IDX_BASE)
                {
                    /* OGBUG  ignores Evil Omens */
                    effective_cost = (total_mana_cost - ((total_mana_cost * Casting_Cost_Reduction(player_idx, spell_idx)) / 100));
                }
                else
                {
                    effective_cost = total_mana_cost;
                }
            }  /* (player_idx != HUMAN_PLAYER_IDX) || (_auto_combat_flag != ST_FALSE) */
        }
        /*
            END:  ¿ effective_cost ?
        */
        /*
            BEGIN:  ¿ Counter Magic ?
        */
        if(
            (spell_idx != spl_Web)
            ||
            (is_spell_like_ability != ST_TRUE)
        )
        {
            if(player_idx == _combat_attacker_player)
            {
                // counter_magic_idx__target_cgx = combat_enchantments[COUNTER_MAGIC_ATTKR];
                counter_magic_idx__target_cgx = COUNTER_MAGIC_ATTKR;
                counter_player_idx = _combat_defender_player;
            }
            else
            {
                // counter_magic_idx__target_cgx = combat_enchantments[COUNTER_MAGIC_DFNDR];
                counter_magic_idx__target_cgx = COUNTER_MAGIC_DFNDR;
                counter_player_idx = _combat_attacker_player;
            }
            if(combat_enchantments[counter_magic_idx__target_cgx] > 0)
            {
                if(Combat_Spell_Dispel_Attempt(combat_enchantments[counter_magic_idx__target_cgx], available_mana_pool, player_idx, spell_data_table[spell_idx].magic_realm) != ST_FALSE)
                {
                    Set_Page_Off();
                    Combat_Screen_Draw();
                    PageFlip_FX();
                    Copy_On_To_Off_Page();
                    Combat_Spell_Counter_Message(caster_idx, counter_player_idx, spell_idx, cnst_Counter_Magic);  // "Counter Magic"
                    if(caster_idx >= CASTER_IDX_BASE)
                    {
                        // ; BUG: this variable is still in use and needed below!
                        // This code is the same as the 'pay the cost' for when it actually gets cast, below?
                        // This bug could be because it was a macro?
                        counter_magic_idx__target_cgx = Combat_Casting_Cost_Multiplier((caster_idx - CASTER_IDX_BASE));
                        _players[(caster_idx - CASTER_IDX_BASE)].Cmbt_Skill_Left -= effective_cost;
                        _players[(caster_idx - CASTER_IDX_BASE)].mana_reserve -= (counter_magic_idx__target_cgx / 10);
                    }
                    else
                    {
                        if(is_spell_like_ability != ST_TRUE)
                        {
                            // ; BUG: this may not be the hero's original owner
                            if(
                                (battle_units[caster_idx].Item_Charges > 0)
                                &&
                                (_ITEMS[_players[player_idx].Heroes[_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot].Items[0]].embed_spell_idx == spell_idx)
                            )
                            {
                                battle_units[caster_idx].Item_Charges -= 1;
                            }
                            else
                            {
                                effective_cost -= battle_unit_mana;
                            }
                            if(battle_unit_mana < 0)
                            {
                                battle_unit_mana = 0;
                            }
                        }
                        else
                        {
                            if(spell_idx == spl_Doom_Bolt)
                            {
                                battle_units[caster_idx].Attribs_2 ^= USA_DOOMBOLT;
                            }
                            if(spell_idx == spl_Web)
                            {
                                battle_units[caster_idx].Attribs_2 ^= USA_WEB;
                            }
                            if(spell_idx == spl_Fireball)
                            {
                                battle_units[caster_idx].Attribs_2 ^= USA_FIREBALL;
                            }
                            if(spell_idx == spl_Healing)
                            {
                                battle_units[caster_idx].Attribs_2 ^= USA_HEALING;
                            }
                        }
                        battle_units[caster_idx].movement_points = 0;
                    }
                    cast_status = 2;
                    spell_idx = ST_UNDEFINED;
                }
                combat_enchantments[counter_magic_idx__target_cgx] -= 5;
                if(combat_enchantments[counter_magic_idx__target_cgx] < 0)
                {
                    combat_enchantments[counter_magic_idx__target_cgx] = 0;
                    Update_Combat_Enchantments_Icon_And_Help();
                }
            }  /* if(combat_enchantments[counter_magic_idx__target_cgx] > 0) */
            if(battlefield->center_square_structure == CS_SorceryNode)
            {
                counter_realm = sbr_Sorcery;
            }
            else if(battlefield->center_square_structure == CS_ChaosNode)
            {
                counter_realm = sbr_Chaos;
            }
            else if(battlefield->center_square_structure == CS_NatureNode)
            {
                counter_realm = sbr_Nature;
            }
            else
            {
                counter_realm = ST_UNDEFINED;
            }
            if(
                (player_idx < _num_players)
                &&
                (_players[player_idx].node_mastery > 0)
            )
            {
                counter_realm = ST_UNDEFINED;
            }
            if(
                (counter_realm > ST_UNDEFINED)
                &&
                (spell_idx > ST_UNDEFINED)
                &&
                (spell_data_table[spell_idx].magic_realm != counter_realm)
                &&
                (Combat_Spell_Dispel_Attempt(50, available_mana_pool, player_idx, spell_data_table[spell_idx].magic_realm) != ST_FALSE)
            )            
            {
                Set_Page_Off();
                Combat_Screen_Draw();
                PageFlip_FX();
                Copy_On_To_Off_Page();
                Combat_Spell_Counter_Message(caster_idx, 5, spell_idx, str_empty_string__ovr112);
                if(caster_idx >= CASTER_IDX_BASE)
                {
                    cost_multiplier = Combat_Casting_Cost_Multiplier((caster_idx - CASTER_IDX_BASE));
                    _players[(caster_idx - CASTER_IDX_BASE)].Cmbt_Skill_Left -= effective_cost;
                    _players[(caster_idx - CASTER_IDX_BASE)].mana_reserve -= (cost_multiplier / 10);
                }
                else
                {
                    if(is_spell_like_ability != ST_TRUE)
                    {
                        // ; BUG: this may not be the hero's original owner
                        if(
                            (battle_units[caster_idx].Item_Charges > 0)
                            &&
                            (_ITEMS[_players[player_idx].Heroes[_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot].Items[0]].embed_spell_idx == spell_idx)
                        )
                        {
                            battle_units[caster_idx].Item_Charges -= 1;
                        }
                        else
                        {
                            effective_cost -= battle_unit_mana;
                        }
                        if(battle_unit_mana < 0)
                        {
                            battle_unit_mana = 0;
                        }
                    }
                    else
                    {
                        if(spell_idx == spl_Doom_Bolt)
                        {
                            battle_units[caster_idx].Attribs_2 ^= USA_DOOMBOLT;
                        }
                        if(spell_idx == spl_Web)
                        {
                            battle_units[caster_idx].Attribs_2 ^= USA_WEB;
                        }
                        if(spell_idx == spl_Fireball)
                        {
                            battle_units[caster_idx].Attribs_2 ^= USA_FIREBALL;
                        }
                        if(spell_idx == spl_Healing)
                        {
                            battle_units[caster_idx].Attribs_2 ^= USA_HEALING;
                        }
                    }
                    battle_units[caster_idx].movement_points = 0;
                }
                cast_status = 2;
                // spell_idx = ST_UNDEFINED;
                spell_idx = 0;
            }
        }
        /*
            END:  ¿ Counter Magic ?
        */
    }
    /*
        END:  
    */
    // ovr112:0A6D
    // @@Target_And_Effect:
    if(spell_idx > spl_NONE)
    {
        if(
            (spell_data_table[spell_idx].type == scc_Battlefield_Spell)
            ||
            (spell_data_table[spell_idx].type == scc_Combat_Counter_Magic)
            ||
            (spell_data_table[spell_idx].type == scc_Disenchants)
            ||
            (spell_data_table[spell_idx].type == scc_City_Enchantment_Positive)
            ||
            (spell_idx == spl_Animate_Dead)
        )
        {
            Target = 99;
            counter_magic_idx__target_cgx = 0;
            target_cgy = 0;
        }
        else
        {
            if(
                (player_idx == HUMAN_PLAYER_IDX)
                &&
                (_auto_combat_flag == ST_FALSE)
            )
            {
                Target = Combat_Spell_Target_Screen(spell_idx, &counter_magic_idx__target_cgx, &target_cgy);
            }
            else
            {
                Target = AITP_Combat_Spell(spell_idx, player_idx, &counter_magic_idx__target_cgx, &target_cgy);
                if(Target != 99)
                {
                    counter_magic_idx__target_cgx = battle_units[Target].cgx;
                    target_cgy = battle_units[Target].cgy;
                }
            }
        }
        if(Target != 999)
        {
            // ... |-> Tactical_Combat_Draw() |-> CMB_DrawMap__WIP() |-> Copy_Back_To_Off()  // 'combat background' from Combat_Screen_Compose_Background()
            // So, ... What's in back-page here?
            // Maybe, maybe not, we called the Combat_Spellbook_Mana_Adder_Screen()? 
            // Maybe, maybe not, we called the Combat_Spell_Target_Screen()?
            Cast_Spell_On_Battle_Unit(spell_idx, Target, caster_idx, counter_magic_idx__target_cgx, target_cgy, available_mana_pool, ST_TRUE, ST_NULL, ST_NULL);
            cast_status = 2;
            if(caster_idx >= CASTER_IDX_BASE)
            {
                counter_magic_idx__target_cgx = Combat_Casting_Cost_Multiplier((caster_idx - CASTER_IDX_BASE));
                _players[(caster_idx - CASTER_IDX_BASE)].Cmbt_Skill_Left -= effective_cost;
                _players[(caster_idx - CASTER_IDX_BASE)].mana_reserve -= ((effective_cost * counter_magic_idx__target_cgx) / 10);
            }
            else  /* caster_idx < CASTER_IDX_BASE */
            {
                if(is_spell_like_ability != ST_TRUE)
                {
                    // ; BUG: this may not be the hero's original owner
                    if(
                        (battle_units[caster_idx].Item_Charges > 0)
                        &&
                        (_ITEMS[_players[player_idx].Heroes[_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot].Items[0]].embed_spell_idx == spell_idx)
                    )
                    {
                        battle_units[caster_idx].Item_Charges -= 1;
                    }
                    else
                    {
                        effective_cost -= battle_unit_mana;
                    }
                    if(battle_unit_mana < 0)
                    {
                        battle_unit_mana = 0;
                    }
                }
                else
                {
                    if(spell_idx == spl_Doom_Bolt)
                    {
                        battle_units[caster_idx].Attribs_2 ^= USA_DOOMBOLT;
                    }
                    if(spell_idx == spl_Web)
                    {
                        battle_units[caster_idx].Attribs_2 ^= USA_WEB;
                    }
                    if(spell_idx == spl_Fireball)
                    {
                        battle_units[caster_idx].Attribs_2 ^= USA_FIREBALL;
                    }
                    if(spell_idx == spl_Healing)
                    {
                        battle_units[caster_idx].Attribs_2 ^= USA_HEALING;
                    }
                }
                battle_units[caster_idx].movement_points = 0;
            }  /* caster_idx < CASTER_IDX_BASE */
        }
    }
// AFTER:
//     spell_idx > 0
//     Target
//     Effect
    if(caster_idx < CASTER_IDX_BASE)
    {
        battle_units[caster_idx].mana = (int8_t)battle_unit_mana;
        if(
            (battle_unit_mana < 3)
            ||
            (battle_units[caster_idx].ammo == 0)
        )
        {
            // ; BUG: also removes short range attacks
            battle_units[caster_idx].ranged = 0;
            battle_units[caster_idx].ranged_type = ST_UNDEFINED;
        }
    }
    return cast_status;
}


// WZD o112p04
// drake178: CMB_ShowSpellbook()
/*
; displays the combat spellbook, allowing a spell to
; be selected for casting by the specified entity
; (player or unit)
; returns the index of the selected spell, and sets the
; passed pointer's value to the clicked control's index
; which indicates its current screen slot
*/
/*

¿ ~== Spellbook_Screen() ?
¿ ~== SmlBook_Draw() ?
*/
int16_t Combat_Spellbook_Screen(int16_t caster_idx, int16_t * selected_spell)
{
    int16_t spellbook_pages[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t spell_cost = 0;
    int16_t hotkey_b = 0;
    int16_t hotkey_f = 0;
    int16_t spellbook_page_spell_index = 0;
    int16_t spell_idx = 0;
    int16_t hotkey_esc = 0;
    int16_t y_start = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t itr = 0;
    int16_t x_start = 0;
    Load_Palette_From_Animation(_spellbook_small_seg);
    Reset_Cycle_Palette_Color();
    Mark_Block(_screen_seg);
    spl_anim_compose_seg = Allocate_Next_Block(_screen_seg, 1090);
    // {Nature, Sorcery, Chaos, Life, Death, Arcane}
    for(itr = 0; itr < NUM_MAGIC_REALMS; itr++)
    {
        spellbook_symbols_bitm[itr] = Allocate_Next_Block(_screen_seg, 4);  // 4 PR  64 B
        Draw_Picture_To_Bitmap(_spellbook_small_symbols[itr], spellbook_symbols_bitm[itr]);
    }
    g_spellbook_mode = 1;
    g_spellbook_left_page = g_combat_spellbook_left_page;
    Assign_Auto_Function(Combat_Spellbook_Screen_Draw, 2);
    LBX_Load_Data_Static(hlpentry_lbx_file__ovr112, 0, (SAMB_ptr)&_help_entries[0], 0, 15, 10);
    Set_Help_List(_help_entries, 15);
    Set_Outline_Color(ST_TRANSPARENT);
    Set_Font_Style_Shadow_Down(0, 3, 0, 0);
    Clear_Fields();
    x_start = 16;
    y_start = 12;
    for(itr = 0; itr < 6; itr++)
    {
        spellbook_pages[(itr + 0)] = Add_Hidden_Field((x_start + 16), (y_start + (itr * 22) + 17), (x_start + 137), (y_start + (itr * 22) + 34), (int16_t)str_empty_string__ovr112[0], ST_UNDEFINED);
    }
    for(itr = 0; itr < 6; itr++)
    {
        spellbook_pages[(itr + 6)] = Add_Hidden_Field((x_start + 148), (y_start + (itr * 22) + 17), (x_start + 268), (y_start + (itr * 22) + 34), (int16_t)str_empty_string__ovr112[0], ST_UNDEFINED);
    }
    hotkey_esc = Add_Hidden_Field(x_start + 159, y_start + 154, x_start + 177, y_start + 183, ST_UNDEFINED, ST_UNDEFINED);
    hotkey_f   = Add_Hidden_Field(x_start + 259, y_start +   2, x_start + 272, y_start +  15, (int16_t)str_hotkey_F__ovr112[0], ST_UNDEFINED);
    hotkey_b   = Add_Hidden_Field(x_start +  13, y_start +   2, x_start +  26, y_start +  14, (int16_t)str_hotkey_B__ovr112[0], ST_UNDEFINED);
    leave_screen = ST_FALSE;
    while(leave_screen == ST_FALSE)
    {
        input_field_idx = Get_Input();
        /*
            Hot-Key ESCAPE
        */
        if(input_field_idx == hotkey_esc)
        {
            Play_Left_Click__DUPE();
            leave_screen = ST_TRUE;
            spell_idx = 0;
        }
        /*
            Help Fields
        */
        for(itr = 0; itr < 6; itr++)
        {
            if(m_spellbook_pages[g_spellbook_left_page].count > itr)
            {
                _help_entries[(3 + itr)].help_idx = abs(m_spellbook_pages[g_spellbook_left_page].spell[itr]);
            }
            else
            {
                _help_entries[(3 + itr)].help_idx = ST_UNDEFINED;
            }
        }
        for(itr = 0; itr < 6; itr++)
        {
            if(m_spellbook_pages[(g_spellbook_left_page + 1)].count > itr)
            {
                _help_entries[(9 + itr)].help_idx = abs(m_spellbook_pages[(g_spellbook_left_page + 1)].spell[itr]);
            }
            else
            {
                _help_entries[(9 + itr)].help_idx = ST_UNDEFINED;
            }
        }
        /*
            Hot-Key Page Forward
        */
        if(input_field_idx == hotkey_f)
        {
            if((m_spellbook_page_count - 2) > g_spellbook_left_page)
            {
                Release_Block(_screen_seg);
                Play_Left_Click__DUPE();
                // Spellbook_Screen()  SmlBook_PageTurn(1, 0, 0);
                SmlBook_PageTurn(1, 1, caster_idx);
                g_spellbook_left_page += 2;
                Mark_Block(_screen_seg);
                spl_anim_compose_seg = Allocate_Next_Block(_screen_seg, 1090);
                for(itr = 0; itr < NUM_MAGIC_REALMS; itr++)
                {
                    spellbook_symbols_bitm[itr] = Allocate_Next_Block(_screen_seg, 4);  // 4 PR  64 B
                    Draw_Picture_To_Bitmap(_spellbook_small_symbols[itr], spellbook_symbols_bitm[itr]);
                }
            }
        }
        /*
            Hot-Key Page Backward
        */
        if(input_field_idx == hotkey_b)
        {
            if(g_spellbook_left_page > 1)
            {
                Release_Block(_screen_seg);
                Play_Left_Click__DUPE();
                // Spellbook_Screen()  SmlBook_PageTurn(0, 0, 0);
                SmlBook_PageTurn(0, 1, caster_idx);
                g_spellbook_left_page -= 2;
                Mark_Block(_screen_seg);
                spl_anim_compose_seg = Allocate_Next_Block(_screen_seg, 1090);
                for(itr = 0; itr < NUM_MAGIC_REALMS; itr++)
                {
                    spellbook_symbols_bitm[itr] = Allocate_Next_Block(_screen_seg, 4);  // 4 PR  64 B
                    Draw_Picture_To_Bitmap(_spellbook_small_symbols[itr], spellbook_symbols_bitm[itr]);
                }
            }
        }
        /*
            BEGIN:  Left-Click Spellbook Page Spell Fields
        */
        for(itr = 0; itr < 12; itr++)
        {
            if(spellbook_pages[itr] == input_field_idx)
            {
                if(itr < 6)
                {
                    if(m_spellbook_pages[g_spellbook_left_page].count > itr)
                    {
                        Play_Left_Click();
                        spellbook_page_spell_index = itr;
                        spell_idx = m_spellbook_pages[g_spellbook_left_page].spell[itr];
                    }
                    else
                    {
                        spell_idx = ST_UNDEFINED;
                    }
                }
                else  /* (itr >= 6) */
                {
                    if(m_spellbook_pages[(g_spellbook_left_page + 1)].count > (itr - 6))
                    {
                        Play_Left_Click();
                        spell_idx = m_spellbook_pages[(g_spellbook_left_page + 1)].spell[(itr - 6)];
                        spellbook_page_spell_index = itr;
                    }
                    else
                    {
                        spell_idx = ST_UNDEFINED;
                    }
                }
                if(caster_idx < (CASTER_IDX_BASE - 1))
                {
                    spell_cost = spell_data_table[abs(spell_idx)].casting_cost;
                }
                else
                {
                    spell_cost = Casting_Cost(HUMAN_PLAYER_IDX, abs(spell_idx), 1);
                }
                if(
                    (spell_data_table[abs(spell_idx)].type == scc_Summoning)
                    &&
                    (_combat_structure == cs_OceanTerrainType)
                    &&
                    ((_unit_type_table[spell_data_table[abs(spell_idx)].unit_type].Move_Flags & MV_FLYING) == 0)
                    &&
                    ((_unit_type_table[spell_data_table[abs(spell_idx)].unit_type].Abilities & UA_NONCORPOREAL) == 0)
                )
                {
                    spell_idx = ST_UNDEFINED;
                }
                if(
                    (
                        (spell_data_table[abs(spell_idx)].type == scc_City_Enchantment_Positive)
                        ||
                        (abs(spell_idx) == spl_Wall_Of_Stone)
                    )
                    &&
                    (
                        (_combat_attacker_player == HUMAN_PLAYER_IDX)
                        ||
                        (_combat_environ != 1)  /* move onto Enemy City */
                    )
                )
                {
                    spell_idx = ST_UNDEFINED;
                }
                // ; BUG: this may not be the unit's original owner
                if(
                    (spell_cost > g_spellbook_cast_mana_limit)
                    &&
                    (
                        (caster_idx > (CASTER_IDX_BASE - 1))
                        ||
                        (_ITEMS[_players[battle_units[caster_idx].controller_idx].Heroes[_UNITS[battle_units[caster_idx].unit_idx].Hero_Slot].Items[0]].embed_spell_idx != spell_idx)
                    )
                )
                {
                    spell_idx = ST_UNDEFINED;
                }
                if(spell_idx > ST_UNDEFINED)
                {
                    leave_screen = ST_TRUE;
                }
            }
        }
        /*
            END:  Left-Click Spellbook Page Spell Fields
        */
        if(leave_screen == ST_FALSE)
        {
            /* DEDU  ¿ Macro // Same as in preamble ? */
            Clear_Fields();
            x_start = 16;
            y_start = 12;
            for(itr = 0; itr < 6; itr++)
            {
                spellbook_pages[(itr + 0)] = Add_Hidden_Field((x_start + 16), (y_start + (itr * 22) + 17), (x_start + 137), (y_start + (itr * 22) + 34), (int16_t)str_empty_string__ovr112[0], ST_UNDEFINED);
            }
            for(itr = 0; itr < 6; itr++)
            {
                spellbook_pages[(itr + 6)] = Add_Hidden_Field((x_start + 148), (y_start + (itr * 22) + 17), (x_start + 268), (y_start + (itr * 22) + 34), (int16_t)str_empty_string__ovr112[0], ST_UNDEFINED);
            }
            hotkey_esc = Add_Hidden_Field(x_start + 159, y_start + 154, x_start + 177, y_start + 183,                     ST_UNDEFINED, ST_UNDEFINED);
            hotkey_f   = Add_Hidden_Field(x_start + 259, y_start +   2, x_start + 272, y_start +  15, (int16_t)str_hotkey_F__ovr112[0], ST_UNDEFINED);
            hotkey_b   = Add_Hidden_Field(x_start +  13, y_start +   2, x_start +  26, y_start +  14, (int16_t)str_hotkey_B__ovr112[0], ST_UNDEFINED);
            Set_Page_Off();
            Combat_Spellbook_Screen_Draw();
            PageFlip_FX();
        }
    }
    g_combat_spellbook_left_page = g_spellbook_left_page;
    Deactivate_Auto_Function();
    Deactivate_Help_List();
    Near_Allocate_Undo();
    Release_Block(_screen_seg);
    Clear_Fields();
    *selected_spell = spellbook_page_spell_index;
    Combat_Compose_Background();  // ... |-> Copy_Off_To_Back();
    return spell_idx;
}


// WZD o112p05
// drake178: CMB_CheckSpellErrors()
/*
; checks if there are any conditions in play that would
; prevent the specified spell from being cast without
; even selecting a target
; returns 1 and displays a warning if the spell can't
; be cast, or returns 0 if it can
;
; contains multiple BUGs and misses many conditions
; that could have been evaluated here
*/
/*

¿ BUGBUG no legal check for 'Dispel Evil' ?
*/
int16_t Do_Legal_Spell_Check__WIP(int16_t spell_idx)
{
    int16_t Controlled_Units = 0;
    int16_t illegal = 0;
    int16_t itr = 0;  // _SI_
    int16_t IDK = 0;  // _DI_

    illegal = ST_FALSE;

    if(spell_idx == 666)
    {
        return ST_TRUE;
    }

    if(spell_idx <= spl_NONE)
    {
        return ST_FALSE;
    }

    if(spell_idx == spl_Magic_Vortex)
    {
        if(_vortex_count == 10)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, (SAMB_ptr)&GUI_NearMsgString[0], 76, 1, 150);  // "Only ten vortexes can be in any area at once"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
    }

    if(spell_idx == spl_Word_Of_Recall)
    {
        if(_players[HUMAN_PLAYER_IDX].casting_spell_idx == spl_Spell_Of_Return)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, (SAMB_ptr)&GUI_NearMsgString[0], 85, 1, 150);  // "Word of Recall and Recall Hero may not be cast while you are banished"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
    }

    if(spell_idx == spl_Recall_Hero)
    {
        IDK = ST_FALSE;
        if(_players[HUMAN_PLAYER_IDX].casting_spell_idx == spl_Spell_Of_Return)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, (SAMB_ptr)&GUI_NearMsgString[0], 85, 1, 150);  // "Word of Recall and Recall Hero may not be cast while you are banished"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
        else if(
                (_combat_wx == _players[HUMAN_PLAYER_IDX].summon_wx)
                &&
                (_combat_wy == _players[HUMAN_PLAYER_IDX].summon_wy)
                &&
                (_combat_wp == _players[HUMAN_PLAYER_IDX].summon_wp)
        )
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, (SAMB_ptr)&GUI_NearMsgString[0], 77, 1, 150);  // "Recall Hero returns your heroes to your summoning circle and this battle is at your summoning circle"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
        else
        {
            for(itr = 0; ((itr < _combat_total_unit_count) && (IDK == ST_FALSE)); itr++)
            {
                if(
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (_UNITS[battle_units[itr].unit_idx].Hero_Slot > ST_UNDEFINED)
                )
                {
                    IDK = ST_TRUE;
                }
                if(IDK == ST_FALSE)
                {
                    LBX_Load_Data_Static(message_lbx_file__ovr112, 0, (SAMB_ptr)&GUI_NearMsgString[0], 78, 1, 150);  // "There are no heroes left to recall"
                    Warn1(GUI_NearMsgString);
                    illegal = ST_TRUE;
                }
            }
        }
    }

    if(spell_idx == spl_Star_Fires)
    {
        IDK = ST_FALSE;
        for(itr = 0; ((itr < _combat_total_unit_count) && (IDK == ST_FALSE)); itr++)
        {
                if(
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (
                        (battle_units[itr].race == rt_Death)
                        ||
                        (battle_units[itr].race == rt_Chaos)
                    )
                )
                {
                    IDK = ST_TRUE;
                }
        }
        if(IDK == ST_FALSE)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, (SAMB_ptr)&GUI_NearMsgString[0], 79, 1, 150);  // "There are no Chaos or Death units to throw this spell on"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
    }

    // ; BUG: the former does not recognize confused units as
    // ; not belonging to the player, while the latter counts
    // ; uninvolved, recalled, and fleeing units as valid
    // ; targets
    if(spell_idx == spl_Animate_Dead)
    {
        IDK = ST_FALSE;
        Controlled_Units = 0;
        for(itr = 0; ((itr < _combat_total_unit_count) && (IDK == ST_FALSE)); itr++)
        {
                if(
                    (battle_units[itr].status == bus_Active)
                    &&
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                )
                {
                    Controlled_Units++;
                }
                if(
                    (battle_units[itr].status > bus_Active)
                    &&
                    (battle_units[itr].status != bus_Gone)
                    &&
                    (battle_units[itr].race != rt_Death)
                    &&
                    ((battle_units[itr].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)
                    &&
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (_UNITS[battle_units[itr].unit_idx].Hero_Slot == ST_UNDEFINED)
                    &&
                    (_UNITS[battle_units[itr].unit_idx].wp != 9)
                )
                {
                    IDK = ST_TRUE;
                }
        }
        if(IDK == ST_FALSE)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, (SAMB_ptr)&GUI_NearMsgString[0], 80, 1, 150);  // "There are no dead units that can be animated"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
        else if(Controlled_Units == MAX_STACK)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, (SAMB_ptr)&GUI_NearMsgString[0], 84, 1, 150);  // "You may only control 9 units in combat at one time"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
    }

    // ; BUG: the former does not recognize confused units as
    // ; not belonging to the player, while the latter counts
    // ; uninvolved, recalled, and fleeing units as valid
    // ; targets
    /*
        Check if there are any (valid) *dead* units.
            invalid: status <= bus_Active (0)  ¿ ?
            invalid: controller is not Current/Human Player
            invalid: race >= rt_Arcane  ¿ ?
            
    */
    if(spell_idx == spl_Raise_Dead)
    {
        IDK = ST_FALSE;
        Controlled_Units = 0;
        for(itr = 0; ((itr < _combat_total_unit_count) && (IDK == ST_FALSE)); itr++)
        {
                if(
                    (battle_units[itr].status == bus_Active)
                    &&
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                )
                {
                    Controlled_Units++;
                }
                if(
                    (battle_units[itr].status > bus_Active)
                    &&
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (battle_units[itr].race < rt_Arcane)
                    &&
                    (battle_units[itr].status != bus_Gone)
                    &&
                    (_UNITS[battle_units[itr].unit_idx].wp != 9)
                )
                {
                    IDK = ST_TRUE;
                }
        }
        if(IDK == ST_FALSE)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, (SAMB_ptr)&GUI_NearMsgString[0], 81, 1, 150);  // "There are no dead units that can be raised from the dead"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
        else if(Controlled_Units == MAX_STACK)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, (SAMB_ptr)&GUI_NearMsgString[0], 84, 1, 150);  // "You may only control 9 units in combat at one time"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
    }

    // ; BUG: the exclusions fail to include Mass Healing,
    // ; which then can't be cast with True Light active
    if(
        (spell_data_table[spell_idx].type == scc_Battlefield_Spell)
        ||
        (spell_data_table[spell_idx].type == scc_Combat_Counter_Magic)
        ||
        (spell_idx == spl_Holy_Word)
        ||
        (spell_idx == spl_Death_Spell)
        ||
        (spell_idx == spl_Flame_Strike)
        ||
        (spell_idx == spl_Call_Chaos)
    )
    {
        if(_combat_attacker_player == HUMAN_PLAYER_IDX)
        {
            IDK = 0;
        }
        else
        {
            IDK = 1;
        }
        if(combat_enchantments[(spell_data_table[spell_idx].Param0 + IDK)] > 0)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, (SAMB_ptr)&GUI_NearMsgString[0], 82, 1, 150);  // "That combat enchantment is already in effect"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
    }

    // ; BUG: fails to check if the units are active or not
    if(spell_data_table[spell_idx].type == scc_Unit_Enchantment_Normal_Only)
    {
        IDK = ST_FALSE;
        for(itr = 0; ((itr < _combat_total_unit_count) && (IDK == ST_FALSE)); itr++)
        {
                if(
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (battle_units[itr].race < rt_Arcane)
                )
                {
                    IDK = ST_TRUE;
                }
        }
        if(IDK == ST_FALSE)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, (SAMB_ptr)&GUI_NearMsgString[0], 83, 1, 150);  // "There are no normal units to throw this spell on"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
    }
    if(spell_data_table[spell_idx].type == scc_Mundane_Curse)
    {
        IDK = ST_FALSE;
        for(itr = 0; ((itr < _combat_total_unit_count) && (IDK == ST_FALSE)); itr++)
        {
                if(
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (battle_units[itr].race < rt_Arcane)
                )
                {
                    IDK = ST_TRUE;
                }
        }
        if(IDK == ST_FALSE)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, (SAMB_ptr)&GUI_NearMsgString[0], 83, 1, 150);  // "There are no normal units to throw this spell on"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
    }

    // ; should also include Possession and Creature Binding
    if(spell_data_table[spell_idx].type == scc_Summoning)
    {
        if(_units == MAX_UNIT_COUNT)
        {
            LBX_Load_Data_Static(message_lbx_file__ovr112, 0, (SAMB_ptr)&GUI_NearMsgString[0], 50, 1, 150);  // "Maximum number of units exceeded"
            Warn1(GUI_NearMsgString);
            illegal = ST_TRUE;
        }
        else
        {
            Controlled_Units = 0;
            for(itr = 0; itr < _combat_total_unit_count; itr++)
            {
                if(
                    (battle_units[itr].controller_idx == HUMAN_PLAYER_IDX)
                    &&
                    (battle_units[itr].status == bus_Active)
                )
                {
                    Controlled_Units++;
                }
            }
            if(Controlled_Units == MAX_STACK)
            {
                LBX_Load_Data_Static(message_lbx_file__ovr112, 0, (SAMB_ptr)&GUI_NearMsgString[0], 84, 1, 150);  // "You may only control 9 units in combat at one time"
                Warn1(GUI_NearMsgString);
                illegal = ST_TRUE;
            }
        }
    }

    return illegal;

}


// WZD o112p06
// drake178: SPL_GetResistMod()
/*
; returns the resistance modifier for the specified
; spell, assigned through code rather than tables
*/
/*

Are these all Combat-Only spells?
Why is this not just in SPELLDAT?
Barring that, why is this not in that function that updates the static game data?

XREF:
    j_Spell_Resistance_Modifier()
        AITP_Combat_Spell()
        AITP_Combat_Spell()
        Cast_Spell_On_Battle_Unit()

RESISTANCE TO SPELLS
...resistance to magic and poison (as symbolized by the number of crosses they have on their unit statistics window)
...base resistance...modifiers to resistance (spells and special unit abilities)...
...makes a resistance roll of 1 to 10. The roll is compared to the creature’s modified (if applicable) resistance.

Note that "saves" decrease the target's ability to resist the spell.
Each reduction has the effect of negating one enemy resistance (i.e., cross) or, in other words, reducing the enemy’s chance to save by 10% per point.

*/
int16_t Spell_Resistance_Modifier(int16_t spell_idx)
{
    int16_t resist_mod = 0;  // _DX_

    resist_mod = 0;

    // ¿ WTF ?  ; this is drake creating the database from the
    // ¿ WTF ?  ; wrong file... it is actually -1 ($FFFF) in v1.31
    // ¿ WTF ?  if(spell_idx == spl_Warp_Creature) { resist_mod = -15; }

    if(spell_idx == spl_Shatter)           { resist_mod =  0; }
    if(spell_idx == spl_Warp_Creature)     { resist_mod = -1; }
    if(spell_idx == spl_Weakness)          { resist_mod = -2; }
    if(spell_idx == spl_Black_Sleep)       { resist_mod = -2; }
    if(spell_idx == spl_Possession)        { resist_mod = -1; }
    if(spell_idx == spl_Black_Wind)        { resist_mod = -1; }
    if(spell_idx == spl_Terror)            { resist_mod =  1; }
    if(spell_idx == spl_Wrack)             { resist_mod =  1; }
    if(spell_idx == spl_Word_Of_Death)     { resist_mod = -5; }
    if(spell_idx == spl_Death_Spell)       { resist_mod = -2; }
    if(spell_idx == spl_Death_Wish)        { resist_mod =  0; }
    if(spell_idx == spl_Dispel_Evil)       { resist_mod = -4; }
    if(spell_idx == spl_Holy_Word)         { resist_mod = -2; }
    if(spell_idx == spl_Petrify)           { resist_mod =  0; }
    if(spell_idx == spl_Vertigo)           { resist_mod =  0; }
    if(spell_idx == spl_Banish)            { resist_mod = -3; }
    if(spell_idx == spl_Confusion)         { resist_mod = -4; }
    if(spell_idx == spl_Stasis)            { resist_mod = -5; }
    if(spell_idx == spl_Creature_Binding)  { resist_mod = -2; }
    if(spell_idx == spl_Great_Unsummoning) { resist_mod = -3; }

    return resist_mod;

}


// WZD o112p07
/*    combat = asting cost multiplier; )
    Cost for Magic Reserve, not Casting Skill

¿ maximum distance is 30, because world is 60 and it wraps ?
¿ 30 is 300% AKA 3x (*3.0) ?
¿ Neutral Player is hard-coded to 10% ?
*/
int16_t Combat_Casting_Cost_Multiplier(int16_t player_idx)
{
    int16_t delta_y;
    int16_t modifier;  // _SI_
    int16_t delta_x;  // _SI_  DNE in Dasm
    int16_t distance;  // _DI_

    if(player_idx == NEUTRAL_PLAYER_IDX)
    {
        modifier = 1;
    }
    else
    {

        if(_FORTRESSES[player_idx].wp != _combat_wp)
        {
            distance = (WORLD_WIDTH / 2);
        }
        else
        {
            delta_y = abs(_FORTRESSES[player_idx].wy - _combat_wy);

            delta_x = abs(_FORTRESSES[player_idx].wx - _combat_wx);

            if(delta_x > (WORLD_WIDTH / 2))
            {
                delta_x = (WORLD_WIDTH - delta_x);
            }

            if(delta_x > (WORLD_WIDTH / 2))
            {
                delta_x = (WORLD_WIDTH - delta_x);
            }

            if(delta_x < delta_y)
            {
                distance = delta_x;
            }
            else
            {
                distance = delta_y;
            }
        }

        if(distance == 0)
        {
            modifier = 5;
        }
        else
        {

            if(_players[player_idx].channeler > 0)
            {
                distance = 2;
            }

            modifier = 10;

            if(distance > 5)
            {
                modifier += 5;
            }

            if(distance > 10)
            {
                modifier += 5;
            }

            if(distance > 15)
            {
                modifier += 5;
            }

            if(distance > 20)
            {
                modifier += 5;
            }

        }

    }

    return modifier;
}


// WZD o112p08
// drake178: AITP_EarthToMud()
/*
Earth to Mud target picker: scans every combat map square and scores it by proximity (Chebyshev-ish, see below) to ground-bound units - enemy units within 2 squares add
(3 - distance), own units within 2 squares subtract (3 - distance).  Returns 99 with the best square in *target_cgx / *target_cgy, or -1 on an ocean battlefield / no positive
square.  Flying, merging, teleporting, and non-corporeal units are ignored on both sides.
NOTE  the distance test only scores the LARGER of the two axis deltas: when delta-x > delta-y only delta-x is range-checked, otherwise only delta-y - a faithful quirk, not a
      reconstruction shortcut
*/
int16_t AITP_EarthToMud(int16_t player_idx, int16_t * target_cgx, int16_t * target_cgy)
{
    int16_t best_result = 0;      /* Best_Result */
    int16_t cgy = 0;              /* Y_Loop_Var */
    int16_t cgx = 0;              /* X_Loop_Var */
    int16_t retn_value = 0;       /* Retn_Value */
    int16_t delta_cgy = 0;        /* Y_Dist */
    int16_t delta_cgx = 0;        /* X_Dist */
    int16_t square_value = 0;     /* _DI_ */
    int16_t battle_unit_idx = 0;  /* _SI_ */
    struct s_BATTLE_UNIT * bu_ptr = NULL;

    retn_value = ST_UNDEFINED;
    best_result = 0;

    /* no mud on an ocean battlefield */
    if(_combat_structure == cs_OceanTerrainType)
    {
        return ST_UNDEFINED;
    }

    for(cgx = 0; cgx < COMBAT_GRID_WIDTH; cgx++)
    {
        for(cgy = 0; cgy < COMBAT_GRID_HEIGHT; cgy++)
        {
            square_value = 0;
            for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
            {
                bu_ptr = &battle_units[battle_unit_idx];

                if(bu_ptr->controller_idx != player_idx)
                {
                    /* enemy unit: mud near it is good */
                    if(
                        (bu_ptr->status == bus_Active)
                        &&
                        ((bu_ptr->Move_Flags & MV_FLYING) == 0)
                        &&
                        ((bu_ptr->Move_Flags & MV_MERGING) == 0)
                        &&
                        ((bu_ptr->Move_Flags & MV_TELEPORT) == 0)
                        &&
                        ((bu_ptr->Abilities & UA_NONCORPOREAL) == 0)
                    )
                    {
                        delta_cgx = abs(cgx - bu_ptr->cgx);
                        delta_cgy = abs(cgy - bu_ptr->cgy);
                        if(delta_cgx > delta_cgy)
                        {
                            if(delta_cgx <= 2)
                            {
                                square_value += (3 - delta_cgx);
                            }
                        }
                        else
                        {
                            if(delta_cgy <= 2)
                            {
                                square_value += (3 - delta_cgy);
                            }
                        }
                    }
                }
                else  /* (bu_ptr->controller_idx == player_idx) */
                {
                    /* own unit: mud near it is bad */
                    if(
                        (bu_ptr->status == bus_Active)
                        &&
                        ((bu_ptr->Move_Flags & MV_FLYING) == 0)
                        &&
                        ((bu_ptr->Move_Flags & MV_MERGING) == 0)
                        &&
                        ((bu_ptr->Move_Flags & MV_TELEPORT) == 0)
                        &&
                        ((bu_ptr->Abilities & UA_NONCORPOREAL) == 0)
                    )
                    {
                        delta_cgx = abs(cgx - bu_ptr->cgx);
                        delta_cgy = abs(cgy - bu_ptr->cgy);
                        if(delta_cgx > delta_cgy)
                        {
                            if(delta_cgx <= 2)
                            {
                                square_value -= (3 - delta_cgx);
                            }
                        }
                        else
                        {
                            if(delta_cgy <= 2)
                            {
                                square_value -= (3 - delta_cgy);
                            }
                        }
                    }
                }
            }

            if(square_value > best_result)
            {
                best_result = square_value;
                *target_cgx = cgx;
                *target_cgy = cgy;
                retn_value = 99;
            }
        }
    }

    return retn_value;
}


// WZD o112p09
// drake178: AITP_Disrupt()
/*
Disrupt target picker: only useful for the attacker against a walled city; probes five wall squares - the gate (8,13), then (8,11), (8,10), (7,13), (7,10) - and returns 99 with
the first one still standing in *target_cgx / *target_cgy, else -1.
OGBUG  the wall-state probes index walls[cgy][cgx] without rebasing to walls[cgy - 10][cgx - 5], so the reads land past the end of s_BATTLEFIELD (Dasm raw offsets 15B6h..15D0h) in
       leftover _screen_seg arena memory - the "uninitialized wall structure offsets" bug noted at the dispatch call site
*/
int16_t AITP_Disrupt(int16_t player_idx, int16_t * target_cgx, int16_t * target_cgy)
{
    int16_t retn_value = 0;  /* _DX_ */

    retn_value = ST_UNDEFINED;

    if(_combat_defender_player != player_idx)  /* the defender owns the walls */
    {
        if(battlefield->walled == 1)
        {
            /* OGBUG  each probe below reads walls[cgy][cgx] instead of the
             *   rebased walls[cgy - 10][cgx - 5], so it lands past the end of
             *   walls[4][4] in leftover _screen_seg memory (Dasm raw offsets
             *   15B6h..15D0h).  We preserve the OG OOB read via pointer
             *   arithmetic on a flat int16_t* so -Warray-bounds doesn't fire.
             *   OG (preserved):    ((int16_t*)&walls[0][0])[cgy * 4 + cgx]
             *   Correct rebased:   battlefield->walls[cgy - 10][cgx - 5]  */
            int16_t * walls_flat = (int16_t *)&battlefield->walls[0][0];
            if(walls_flat[13 * 4 + 8] == 1)       /* OGBUG 15D0h; rebased: walls[3][3] */
            {
                *target_cgx = 8;
                *target_cgy = 13;
                retn_value = 99;
            }
            else if(walls_flat[11 * 4 + 8] == 1)  /* OGBUG 15C0h; rebased: walls[1][3] */
            {
                *target_cgx = 8;
                *target_cgy = 11;
                retn_value = 99;
            }
            else if(walls_flat[10 * 4 + 8] == 1)  /* OGBUG 15B8h; rebased: walls[0][3] */
            {
                *target_cgx = 8;
                *target_cgy = 10;
                retn_value = 99;
            }
            else if(walls_flat[13 * 4 + 7] == 1)  /* OGBUG 15CEh; rebased: walls[3][2] */
            {
                *target_cgx = 7;
                *target_cgy = 13;
                retn_value = 99;
            }
            else if(walls_flat[10 * 4 + 7] == 1)  /* OGBUG 15B6h; rebased: walls[0][2] */
            {
                *target_cgx = 7;
                *target_cgy = 10;
                retn_value = 99;
            }
        }
    }

    return retn_value;
}


// WZD o112p10
// drake178: AITP_CracksCall()
/*
Cracks Call target picker: picks the strongest visible enemy ground unit (not flying, merging, or non-corporeal), with a +30 bonus for a unit standing on an intact wall square
when attacking a walled city.  If no unit qualifies but the city is walled, falls back to the same five wall-square probes as AITP_Disrupt() and returns 99 with the square.
OGBUG  both the in-loop wall test and the fallback probes index walls[cgy][cgx] without rebasing (see AITP_Disrupt())
NOTE  unlike AITP_EarthToMud(), MV_TELEPORT units are NOT excluded
*/
int16_t AITP_CracksCall(int16_t player_idx, int16_t * target_cgx, int16_t * target_cgy)
{
    int16_t unit_threat = 0;      /* Unit_Threat */
    int16_t retn_value = 0;       /* Retn_Value */
    int16_t highest_threat = 0;   /* Highest_Threat */
    int16_t battle_unit_idx = 0;  /* _DI_ */
    struct s_BATTLE_UNIT * bu_ptr = NULL;

    highest_threat = 0;
    retn_value = ST_UNDEFINED;

    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        unit_threat = ST_UNDEFINED;
        bu_ptr = &battle_units[battle_unit_idx];

        if(
            (bu_ptr->controller_idx != player_idx)
            &&
            (bu_ptr->status == bus_Active)
            &&
            ((bu_ptr->Move_Flags & MV_FLYING) == 0)
            &&
            ((bu_ptr->Move_Flags & MV_MERGING) == 0)
            &&
            ((bu_ptr->Abilities & UA_NONCORPOREAL) == 0)
        )
        {
            if(!Target_Is_Visible(battle_unit_idx)) continue;  /* skips the best-threat compare, as in the Dasm */
            unit_threat = Effective_Battle_Unit_Strength(battle_unit_idx);
            if(
                (_combat_defender_player != player_idx)
                &&
                (battlefield->walled == 1)
                &&
                (bu_ptr->cgx >= 5)
                &&
                (bu_ptr->cgx <= 7)
                &&
                (bu_ptr->cgy >= 11)
                &&
                (bu_ptr->cgy <= 13)
                &&
                (battlefield->walls[bu_ptr->cgy][bu_ptr->cgx] == 1)  /* OGBUG  unrebased walls index */
            )
            {
                unit_threat += 30;  /* drop the wall out from under it */
            }
        }

        if(unit_threat > highest_threat)
        {
            highest_threat = unit_threat;
            retn_value = battle_unit_idx;
            *target_cgx = bu_ptr->cgx;
            *target_cgy = bu_ptr->cgy;
        }
    }

    /* fallback: no unit target, but the city is walled - target a standing wall square instead */
    if(
        (_combat_defender_player != player_idx)
        &&
        (battlefield->walled == 1)
        &&
        (retn_value == ST_UNDEFINED)
    )
    {
        /* OGBUG  same unrebased walls[cgy][cgx] pattern as AITP_Disrupt() —
         *   see the header comment there for details.  Preserved via
         *   pointer arithmetic on a flat int16_t*.
         *   OG (preserved):    ((int16_t*)&walls[0][0])[cgy * 4 + cgx]
         *   Correct rebased:   battlefield->walls[cgy - 10][cgx - 5]  */
        int16_t * walls_flat = (int16_t *)&battlefield->walls[0][0];
        if(walls_flat[13 * 4 + 8] == 1)       /* OGBUG 15D0h; rebased: walls[3][3] */
        {
            *target_cgx = 8;
            *target_cgy = 13;
            retn_value = 99;
        }
        else if(walls_flat[11 * 4 + 8] == 1)  /* OGBUG 15C0h; rebased: walls[1][3] */
        {
            *target_cgx = 8;
            *target_cgy = 11;
            retn_value = 99;
        }
        else if(walls_flat[10 * 4 + 8] == 1)  /* OGBUG 15B8h; rebased: walls[0][3] */
        {
            *target_cgx = 8;
            *target_cgy = 10;
            retn_value = 99;
        }
        else if(walls_flat[13 * 4 + 7] == 1)  /* OGBUG 15CEh; rebased: walls[3][2] */
        {
            *target_cgx = 7;
            *target_cgy = 13;
            retn_value = 99;
        }
        else if(walls_flat[10 * 4 + 7] == 1)  /* OGBUG 15B6h; rebased: walls[0][2] */
        {
            *target_cgx = 7;
            *target_cgy = 10;
            retn_value = 99;
        }
    }

    return retn_value;
}


// WZD o112p11
// drake178: UU_AITP_WordofRecall()

// WZD o112p12
// drake178: AITP_RecallHero()
/*
Recall Hero target picker: picks the own active hero in the most danger - effective strength scaled up by max-HP / current-HP - unless the battle is at the caster's own Fortress
(never recall the Fortress defense).  Returns the battle unit index or -1.
*/
int16_t AITP_RecallHero(int16_t player_idx)
{
    int16_t current_hp = 0;       /* Current_HP */
    int16_t max_hp = 0;           /* Max_HP */
    int16_t picked_target = 0;    /* Target_Index */
    int16_t highest_danger = 0;   /* Highest_Danger */
    int16_t unit_danger = 0;      /* _DI_ */
    int16_t battle_unit_idx = 0;  /* _SI_ */
    struct s_BATTLE_UNIT * bu_ptr = NULL;

    highest_danger = 0;
    picked_target = ST_UNDEFINED;

    if(
        (_FORTRESSES[player_idx].wx == _combat_wx)
        &&
        (_FORTRESSES[player_idx].wy == _combat_wy)
        &&
        (_FORTRESSES[player_idx].wp == _combat_wp)
    )
    {
        return picked_target;  /* -1: never recall out of the own-Fortress battle */
    }

    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        unit_danger = -1;
        bu_ptr = &battle_units[battle_unit_idx];

        if(bu_ptr->controller_idx != player_idx) continue;
        if(bu_ptr->status != bus_Active) continue;
        if(_UNITS[bu_ptr->unit_idx].Hero_Slot <= ST_UNDEFINED) continue;

        unit_danger = Effective_Battle_Unit_Strength(battle_unit_idx);
        max_hp = (bu_ptr->figure_max * bu_ptr->hits);
        current_hp = ((bu_ptr->figure_cnt * bu_ptr->hits) - bu_ptr->front_figure_damage);
        if(current_hp != 0)
        {
            /* Dasm: `imul Max_HP` followed by `cwd` - the product is truncated to 16 bits before the divide */
            unit_danger = ((int16_t)(unit_danger * max_hp) / current_hp);
        }

        if(unit_danger > highest_danger)
        {
            highest_danger = unit_danger;
            picked_target = battle_unit_idx;
        }
    }

    return picked_target;
}
