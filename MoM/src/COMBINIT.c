/*
    Combat

    WIZARDS.EXE
        ovr116  MoO2  COMBINIT

*/



#include <string.h>                  /* memcpy() */

#include "../../MoX/src/MOM_DAT.h"   /* _UNITS, _ITEMS, _HEROES2, _num_players, global_battle_unit, spell_data_table, cnt_* */
#include "../../MoX/src/MOM_DEF.h"   /* UE_*, USA_*, UA_*, MV_*, IP_*, ip_*, Att_*, UM_*, CC_*, it_*, NUM_HERO_ITEMS, CHAOS_SURGE, HOLY_ARMS, CHARM_OF_LIFE, CRUSADE */
#include "../../MoX/src/MOX_BASE.h"  /* ST_TRUE, ST_FALSE, ST_UNDEFINED */
#include "../../MoX/src/MOX_BITS.h"  /* GET_4B_OFS(), which MOX_DEF.h's ITEM_POWER() expands to */
#include "../../MoX/src/MOX_DAT.h"   /* _players */
#include "../../MoX/src/MOX_DEF.h"   /* SETMIN(), ITEM_POWER(), UNITOWNER(), HERO_* */
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/random.h"    /* Random() */

#include "CITYCALC.h"                /* Unit_Gold_Upkeep() */
#include "NEXTTURN.h"                /* Unit_Moves2() */
#include "RACETYPE.h"                /* rt_* */
#include "UNITTYPE.h"                /* _unit_type_table, struct s_UNIT_TYPE, rag_*, srat_* */

#include "Combat.h"                  /* struct s_BATTLE_UNIT, bue_*, bus_*, bua_*, BU_CASTER_*(), BU_LUCKY(), *_ATTKR / *_DFNDR, combat_enchantments */
#include "CMBMAGIC.h"

#include "COMBINIT.h"



/*
    WIZARDS.EXE  ovr116
*/

// WZD o116p01
uint32_t Battle_Unit_Item_Stats(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit)
{
    uint32_t battle_unit_item_enchantments = 0;
    int16_t unit_owner_idx = 0;
    int16_t hero_slot_idx = 0;
    int16_t item_idx = 0;
    int16_t itr_items = 0;
    battle_unit->item_enchantments = 0;
    battle_unit->melee_attack_attributes = 0;
    battle_unit->ranged_attack_attributes = 0;
    unit_owner_idx = _UNITS[unit_idx].owner_idx;
    hero_slot_idx = _UNITS[unit_idx].Hero_Slot;
    if(hero_slot_idx == ST_UNDEFINED) { return 0; }
    battle_unit_item_enchantments = 0;
    for(itr_items = 0; itr_items < NUM_HERO_ITEMS; itr_items++)
    {
        if(_players[unit_owner_idx].Heroes[hero_slot_idx].Items[itr_items] != ST_UNDEFINED)
        {
            item_idx = _players[unit_owner_idx].Heroes[hero_slot_idx].Items[itr_items];
            Item_Powers_To_Unit_Enchantments(item_idx, battle_unit);
            if(_ITEMS[item_idx].type == it_Shield)
            {
                battle_unit->Abilities = (battle_unit->Abilities | UA_LARGESHIELD);
            }
            if(battle_unit->mana_max > 0)
            {
                battle_unit->mana_max += _ITEMS[item_idx].spell_skill;
            }
            battle_unit->defense          += _ITEMS[item_idx].defense;
            battle_unit->Gold_Defense     += _ITEMS[item_idx].defense;
            battle_unit->movement_points  += _ITEMS[item_idx].moves2;
            battle_unit->resist           += _ITEMS[item_idx].resistance;
            battle_unit->Gold_Resist      += _ITEMS[item_idx].resistance;
            // if it_Sword, it_Mace, it_Axe, it_Misc ...
            if(
                (_ITEMS[item_idx].type <= it_Axe)
                ||
                (_ITEMS[item_idx].type == it_Misc)
            )
            {
                battle_unit->melee         += _ITEMS[item_idx].attack;
                battle_unit->Gold_Melee    += _ITEMS[item_idx].attack;
                battle_unit->melee_tohit  += _ITEMS[item_idx].tohit;
                if(ITEM_POWER(item_idx,ip_Flaming))
                {
                    battle_unit->melee += 3;
                    battle_unit->Gold_Melee += 3;
                }
                if(ITEM_POWER(item_idx,ip_Giant_Strength))
                {
                    battle_unit->melee += 1;
                    battle_unit->Gold_Melee += 1;
                }
                // ip_Vampiric,   ip_Lightning, ip_Destruction, ip_Chaos,    ip_Death,     ip_Power_Drain, ip_Holy_Avenger, ip_Phantasmal, ip_Stoning
                // Att_LifeSteal, Att_ArmorPrc, Att_Destruct,   Att_DoomDmg, Att_DthTouch, Att_PwrDrain,   Att_DsplEvil,    Att_Illusion,  Att_StnTouch
                Item_Powers_To_Attack_Attributes(&battle_unit->melee_attack_attributes, item_idx);

            }
            if(
                ((_ITEMS[item_idx].type == it_Bow)   && ((battle_unit->ranged_type / 10) == rag_Missile))
                ||
                ((_ITEMS[item_idx].type == it_Staff) && ((battle_unit->ranged_type / 10) == rag_Magic))
                ||
                ((_ITEMS[item_idx].type == it_Wand)  && ((battle_unit->ranged_type / 10) == rag_Magic))
                ||
                ((_ITEMS[item_idx].type == it_Axe)   && (battle_unit->ranged_type == srat_Thrown))
                ||
                (_ITEMS[item_idx].type == it_Misc)
            )
            {
                battle_unit->ranged         += _ITEMS[item_idx].attack;
                battle_unit->Gold_Ranged    += _ITEMS[item_idx].attack;
                battle_unit->ranged_tohit  += _ITEMS[item_idx].tohit;
                if(ITEM_POWER(item_idx,ip_Flaming))
                {
                    battle_unit->ranged += 3;
                    battle_unit->Gold_Ranged += 3;
                }
                if(ITEM_POWER(item_idx,ip_Giant_Strength) && (_ITEMS[item_idx].type == it_Axe) && (battle_unit->ranged_type == srat_Thrown))
                {
                    battle_unit->ranged += 1;
                    battle_unit->Gold_Ranged += 1;
                }
                Item_Powers_To_Attack_Attributes(&battle_unit->ranged_attack_attributes, item_idx);
            }
        }
    }
    battle_unit_item_enchantments |= battle_unit->item_enchantments;
    return battle_unit_item_enchantments;
}


// WZD o116p02
/*
    ¿ "Spell-Specific Artifact Enchantments" ?
*/
void Item_Powers_To_Unit_Enchantments(int16_t item_idx, struct s_BATTLE_UNIT * battle_unit)
{
    uint32_t item_enchantments = 0;
    item_enchantments = 0;
    if(ITEM_POWER(item_idx,ip_Cloak_Of_Fear))
    {
        item_enchantments |= UE_CLOAK_OF_FEAR;
    }
    if(ITEM_POWER(item_idx,ip_Wraith_Form))
    {
        item_enchantments |= UE_WRAITH_FORM;
    }
    if(ITEM_POWER(item_idx,ip_Regeneration))
    {
        item_enchantments |= UE_REGENERATION;
    }
    if(ITEM_POWER(item_idx,ip_Pathfinding))
    {
        item_enchantments |= UE_PATH_FINDING;
    }
    if(ITEM_POWER(item_idx,ip_Water_Walking))
    {
        item_enchantments |= UE_WATER_WALKING;
    }
    if(ITEM_POWER(item_idx,ip_Resist_Elements))
    {
        item_enchantments |= UE_RESIST_ELEMENTS;
    }
    if(ITEM_POWER(item_idx,ip_Elemental_Armour))
    {
        item_enchantments |= UE_ELEMENTAL_ARMOR;
    }
    if(ITEM_POWER(item_idx,ip_Endurance))
    {
        item_enchantments |= UE_ENDURANCE;
    }
    if(ITEM_POWER(item_idx,ip_Invisibility))
    {
        item_enchantments |= UE_INVISIBILITY;
    }
    if(ITEM_POWER(item_idx,ip_Flight))
    {
        item_enchantments |= UE_FLIGHT;
    }
    if(ITEM_POWER(item_idx,ip_Resist_Magic))
    {
        item_enchantments |= UE_RESIST_MAGIC;
    }
    if(ITEM_POWER(item_idx,ip_Guardian_Wind))
    {
        item_enchantments |= UE_GUARDIAN_WIND;
    }
    if(ITEM_POWER(item_idx,ip_Magic_Immunity))
    {
        item_enchantments |= UE_MAGIC_IMMUNITY;
    }
    if(ITEM_POWER(item_idx,ip_True_Sight))
    {
        item_enchantments |= UE_TRUE_SIGHT;
    }
    if(ITEM_POWER(item_idx,ip_Bless))
    {
        item_enchantments |= UE_BLESS;
    }
    if(ITEM_POWER(item_idx,ip_Lion_Heart))
    {
        item_enchantments |= UE_LION_HEART;
    }
    if(ITEM_POWER(item_idx,ip_Planar_Travel))
    {
        item_enchantments |= UE_PLANAR_TRAVEL;
    }
    if(ITEM_POWER(item_idx,ip_Righteousness))
    {
        item_enchantments |= UE_RIGHTEOUSNESS;
    }
    if(ITEM_POWER(item_idx,ip_Invulnerability))
    {
        item_enchantments |= UE_INVULNERABILITY;
    }
    if(ITEM_POWER(item_idx,ip_Holy_Avenger))
    {
        item_enchantments |= UE_BLESS;
    }
    battle_unit->item_enchantments |= item_enchantments;
}


// WZD o116p03
void Item_Powers_To_Attack_Attributes(uint16_t * attack_flags, int16_t item_idx)
{
    if(ITEM_POWER(item_idx,ip_Vampiric))
    {
        *attack_flags |= Att_LifeSteal;
    }
    if(ITEM_POWER(item_idx,ip_Lightning))
    {
        *attack_flags |= Att_ArmorPrc;
    }
    if(ITEM_POWER(item_idx,ip_Destruction))
    {
        *attack_flags |= Att_Destruct;
    }
    if(ITEM_POWER(item_idx,ip_Chaos))
    {
        *attack_flags |= Att_DoomDmg;
    }
    if(ITEM_POWER(item_idx,ip_Death))
    {
        *attack_flags |= Att_DthTouch;
    }
    if(ITEM_POWER(item_idx,ip_Power_Drain))
    {
        *attack_flags |= Att_PwrDrain;
    }
    if(ITEM_POWER(item_idx,ip_Holy_Avenger))
    {
        *attack_flags |= Att_DsplEvil;
    }
    if(ITEM_POWER(item_idx,ip_Phantasmal))
    {
        *attack_flags |= Att_Illusion;
    }
    if(ITEM_POWER(item_idx,ip_Stoning))
    {
        *attack_flags |= Att_StnTouch;
    }
}


// WZD o116p04
int16_t Unit_Hit_Points(int16_t unit_idx)
{
    int16_t unit_level = 0;
    int16_t charm_of_life_bonus = 0;
    int16_t hit_points = 0;
    unit_level = _UNITS[unit_idx].Level;
    if(
        ((_UNITS[unit_idx].enchantments & UE_HEROISM) != 0)
        &&
        (unit_level < 3)
    )
    {
        unit_level = 3;
    }
    hit_points = _unit_type_table[_UNITS[unit_idx].type].Hits;
    if((_UNITS[unit_idx].enchantments & UE_BLACK_CHANNELS) != 0)
    {
        hit_points += 1;
    }
    if((_UNITS[unit_idx].enchantments & UE_LION_HEART) != 0)
    {
        hit_points += 3;
    }
    /*
        BEGIN: Unit is Hero-Unit
    */
    if(_UNITS[unit_idx].Hero_Slot > ST_UNDEFINED)
    {
        if(unit_level > 0)
        {
            hit_points += 1;
        }
        if(HERO_CONSTITUTION(_UNITS[unit_idx].owner_idx, _UNITS[unit_idx].type))
        {
            hit_points += (unit_level + 1);
        }
        if(HERO_CONSTITUTION2(_UNITS[unit_idx].owner_idx, _UNITS[unit_idx].type))
        {
            hit_points += (((unit_level + 1) * 3) / 2);
        }
        if(unit_level > 1)
        {
            hit_points += 1;
        }
        if(unit_level > 3)
        {
            hit_points += 1;
        }
    }
    /*
        END: Unit is Hero-Unit
    */
    if(unit_level > 2)
    {
        hit_points += 1;
    }
    if(unit_level > 4)
    {
        hit_points += 1;
    }
    if(unit_level > 5)
    {
        hit_points += 1;
    }
    if(unit_level > 6)
    {
        hit_points += 1;
    }
    if(unit_level > 7)
    {
        hit_points += 1;
    }
    if(_players[_UNITS[unit_idx].owner_idx].Globals[CHARM_OF_LIFE] > 0)
    {
        charm_of_life_bonus = hit_points / 4;
        if(charm_of_life_bonus < 1)
        {
            charm_of_life_bonus = 1;
        }
        hit_points += charm_of_life_bonus;
    }
    return hit_points;
}

// WZD o116p05
int16_t Battle_Unit_Hit_Points(struct s_BATTLE_UNIT * battle_unit)
{
    int16_t charm_of_life_bonus = 0;
    int16_t unit_level = 0;
    uint32_t battle_unit_enchantments = 0;
    int16_t unit_idx = 0;
    int16_t hit_points = 0;
    unit_idx = battle_unit->unit_idx;
    unit_level = _UNITS[unit_idx].Level;
    battle_unit_enchantments = _UNITS[unit_idx].enchantments | battle_unit->enchantments | battle_unit->item_enchantments;
    if(
        ((battle_unit_enchantments & UE_HEROISM) != 0)
        &&
        (unit_level < 3)
    )
    {
        unit_level = 3;
    }
    if(
        (_players[battle_unit->controller_idx].Globals[CRUSADE] > 0)
        &&
        ((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_FANTASTIC) == 0)
    )
    {
        unit_level++;
    }
    hit_points = _unit_type_table[_UNITS[unit_idx].type].Hits;
    if((battle_unit_enchantments & UE_BLACK_CHANNELS) != 0)
    {
        hit_points += 1;
        battle_unit->Gold_Hits += 1;
    }
    if((battle_unit_enchantments & UE_LION_HEART) != 0)
    {
        hit_points += 3;
        battle_unit->Gold_Hits += 3;
    }
    if(_UNITS[unit_idx].Hero_Slot > ST_UNDEFINED)
    {
        if(unit_level > 0)
        {
            hit_points += 1;
        }
        if(HERO_CONSTITUTION(_UNITS[unit_idx].owner_idx, _UNITS[unit_idx].type))
        {
            hit_points += (unit_level + 1);
        }
        if(HERO_CONSTITUTION2(_UNITS[unit_idx].owner_idx, _UNITS[unit_idx].type))
        {
            hit_points += (((unit_level + 1) * 3) / 2);
        }
        if(unit_level > 1)
        {
            hit_points += 1;
        }
        if(unit_level > 3)
        {
            hit_points += 1;
        }
    }  /* (_UNITS[unit_idx].Hero_Slot > ST_UNDEFINED) */
    if(unit_level > 2)
    {
        hit_points += 1;
    }
    if(unit_level > 4)
    {
        hit_points += 1;
    }
    if(unit_level > 5)
    {
        hit_points += 1;
    }
    if(unit_level > 6)
    {
        hit_points += 1;
    }
    if(unit_level > 7)
    {
        hit_points += 1;
    }
    if(_players[_UNITS[unit_idx].owner_idx].Globals[CHARM_OF_LIFE] > 0)
    {
        charm_of_life_bonus = (hit_points / 4);
        SETMIN(charm_of_life_bonus, 1);
        hit_points += charm_of_life_bonus;
    }
    hit_points += battle_unit->Extra_Hits;
    battle_unit->Gold_Hits += battle_unit->Extra_Hits;
    return hit_points;
}


// WZD o116p06
// MoO2  Module: COMBINIT  Load_Combat_Ship_()
void Load_Battle_Unit(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit)
{
    int16_t item_charges = 0;
    int16_t itr = 0;
    memcpy(battle_unit, &_unit_type_table[_UNITS[unit_idx].type].Melee, sizeof(struct s_UNIT_TYPE));
    battle_unit->combat_effects = 0;
    battle_unit->melee_tohit = 0;
    battle_unit->ranged_tohit = 0;
    battle_unit->tohit = 0;  // set in Battle_Unit_Regular_Stats()
    battle_unit->toblock = 0;
    battle_unit->Weapon_Plus1 = 0;
    battle_unit->melee_attack_attributes = 0;
    battle_unit->ranged_attack_attributes = 0;
    battle_unit->item_enchantments = 0;
    battle_unit->Extra_Hits = 0;
    battle_unit->unit_idx = unit_idx;
    battle_unit->Web_HP = 0;
    battle_unit->Gold_Melee = 0;
    battle_unit->Gold_Ranged = 0;
    battle_unit->Gold_Defense = 0;
    battle_unit->Gold_Resist = 0;
    battle_unit->Gold_Hits = 0;
    battle_unit->Grey_Melee = 0;
    battle_unit->Grey_Ranged = 0;
    battle_unit->Grey_Defense = 0;
    battle_unit->Grey_Resist = 0;
    battle_unit->bufpi = ST_UNDEFINED;
    battle_unit->status = bus_Active;
    battle_unit->controller_idx = _UNITS[unit_idx].owner_idx;
    for(itr = 0; itr < 3; itr++)
    {
        battle_unit->damage[itr] = 0;
    }
    battle_unit->enchantments = 0;
    battle_unit->Suppression = 0;
    battle_unit->mana_max = 0;
    battle_unit->item_charges = 0;
    battle_unit->target_battle_unit_idx = ST_UNDEFINED;
    battle_unit->Poison_Strength = 0;
    battle_unit->upkeep = (int8_t)Unit_Gold_Upkeep(unit_idx);
    if((battle_unit->attack_attributes & Att_Poison) != 0)
    {
        battle_unit->Poison_Strength = battle_unit->Spec_Att_Attrib;
        if(battle_unit->ranged_type != srat_MultiGaze)
        {
            battle_unit->Spec_Att_Attrib = 0;
        }
    }
    Battle_Unit_Regular_Stats(battle_unit);
    battle_unit->mana = battle_unit->mana_max;
    battle_unit->damage[0] = _UNITS[unit_idx].Damage;
    battle_unit->front_figure_damage = (_UNITS[unit_idx].Damage % battle_unit->hits);
    battle_unit->figure_cnt = (battle_unit->figure_max - (_UNITS[unit_idx].Damage / battle_unit->hits));
    battle_unit->Atk_FigLoss = 0;
    battle_unit->confusion_state = 0;
    battle_unit->gibs = 0;
    battle_unit->Unknown_5A = 0;  // NOTE: Per the Dasm, duplicate `battle_unit->Unknown_5A = 0;`
    battle_unit->Unknown_5A = 0;
    battle_unit->Melee_Anim = 0;
    battle_unit->outline_magic_realm = 0;
    battle_unit->move_anim_ctr = 0;
    battle_unit->mid_move = ST_FALSE;
    battle_unit->action = bua_Ready;
    battle_unit->animate_idle = ST_FALSE;
    battle_unit->figure_effect = 0;
    battle_unit->animate_move_as_idle = 0;
    /* OGBUG: only Items[0]'s embedded spell is ever usable */
    if(_UNITS[unit_idx].Hero_Slot > ST_UNDEFINED)
    {
        if(_players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[0] > ST_UNDEFINED)
        {
            item_charges = (int8_t)_ITEMS[_players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[0]].embed_spell_cnt;
        }
        else
        {
            item_charges = 0;
        }
        if(item_charges > 0)
        {
            battle_unit->item_charges = (int8_t)item_charges;
        }
    }
}

// WZD o116p07
/*
¿ MoO2  Module: COMBINIT  Load_Combat_Ship_() ?
    Bonus_For_Regular_Mods_()
    Bonus_For_Special_Mods_()
*/
void Battle_Unit_Regular_Stats(struct s_BATTLE_UNIT * battle_unit)
{
    int16_t weapon_quality = 0;
    int8_t niu_variable = 0;
    int8_t unit_mutations = 0;
    uint32_t battle_unit_enchantments = 0;
    int16_t chaos_surge = 0;
    int16_t unit_idx = 0;
    int16_t itr_players = 0;
    unit_idx = battle_unit->unit_idx;
    unit_mutations = _UNITS[unit_idx].mutations;
    /* OGBUG: this reset block is missing battle_unit->Weapon_Plus1 = 0; */
    battle_unit->tohit = _unit_type_table[_UNITS[battle_unit->unit_idx].type].To_Hit;
    battle_unit->melee_tohit = 0;
    battle_unit->ranged_tohit = 0;
    battle_unit->toblock = 0;
    battle_unit->Gold_Melee = 0;
    battle_unit->Gold_Ranged = 0;
    battle_unit->Gold_Defense = 0;
    battle_unit->Gold_Resist = 0;
    battle_unit->Gold_Hits = 0;
    battle_unit->Grey_Melee = 0;
    battle_unit->Grey_Ranged = 0;
    battle_unit->Grey_Defense = 0;
    battle_unit->Grey_Resist = 0;
    battle_unit->resist = _unit_type_table[_UNITS[unit_idx].type].Resist;
    battle_unit->defense = _unit_type_table[_UNITS[unit_idx].type].Defense;
    battle_unit->melee = _unit_type_table[_UNITS[unit_idx].type].Melee;
    battle_unit->ranged = _unit_type_table[_UNITS[unit_idx].type].Ranged;
    if(BU_CASTER_40())
    {
        battle_unit->mana_max = 40;
    }
    if(BU_CASTER_20())
    {
        battle_unit->mana_max = 20;
    }
    Battle_Unit_Level_Stats(unit_idx, battle_unit);
    if(BU_LUCKY())
    {
        battle_unit->tohit += 1;
        battle_unit->toblock += 1;
        battle_unit->resist += 1;
        battle_unit->Gold_Resist += 1;
    }
    /*
        Unit is a Hero Unit
    */
    if(_UNITS[unit_idx].Hero_Slot > ST_UNDEFINED)
    {
        if(battle_unit->Weapon_Plus1 == 0)
        {
            battle_unit->Weapon_Plus1 = 1;
        }
        Battle_Unit_Hero_Skill_Stats(unit_idx, battle_unit);
        Battle_Unit_Item_Stats(unit_idx, battle_unit);
    }
    battle_unit_enchantments = battle_unit->item_enchantments | _UNITS[unit_idx].enchantments;
    weapon_quality = (_UNITS[unit_idx].mutations & 0x03);
    if(weapon_quality > 0)
    {
        if(battle_unit->melee > 0)
        {
            battle_unit->melee += (weapon_quality - 1);
            battle_unit->Gold_Melee += (weapon_quality - 1);
            battle_unit->melee_tohit += 1;
        }
        if(
            ((battle_unit->ranged_type / 10) == rag_Missile)
            ||
            ((battle_unit->ranged_type / 10) == rag_Boulder)
            ||
            (battle_unit->ranged_type == srat_Thrown)
        )
        {
            battle_unit->ranged += (weapon_quality - 1);
            battle_unit->Gold_Ranged += (weapon_quality - 1);
            battle_unit->ranged_tohit += 1;
        }
            battle_unit->defense += (weapon_quality - 1);
            battle_unit->Gold_Defense += (weapon_quality - 1);
            battle_unit->Weapon_Plus1 = (weapon_quality + 1);
    }
    /* OGBUG: should check itr_players - the HUMAN_PLAYER_IDX is a typo */
    chaos_surge = ST_FALSE;
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        if(_players[HUMAN_PLAYER_IDX].Globals[CHAOS_SURGE] > 0)
        {
            chaos_surge = ST_TRUE;
        }
    }
    if(chaos_surge == ST_TRUE)
    {
        if(battle_unit->race == rt_Chaos)
        {
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged += 2;
                battle_unit->Gold_Ranged += 2;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee += 2;
                battle_unit->Gold_Melee += 2;
            }
        }
    }
    if(
        (
            (_players[_UNITS[unit_idx].owner_idx].Globals[HOLY_ARMS] > 0)
            &&
            !(battle_unit->Abilities & UA_FANTASTIC)
            &&
            !(unit_mutations & UM_UNDEAD)
        )
        ||
        ((battle_unit_enchantments & UE_HOLY_WEAPON) != 0)
    )
    {
        battle_unit->enchantments |= UE_HOLY_WEAPON;
        battle_unit->melee_tohit += 1;
        if(
            (battle_unit->ranged_type == srat_Thrown)
            ||
            ((battle_unit->ranged_type / 10) == rag_Boulder)
            ||
            ((battle_unit->ranged_type / 10) == rag_Missile)
        )
        {
            battle_unit->ranged_tohit += 1;
        }
        if(battle_unit->Weapon_Plus1 == 0)
        {
            battle_unit->Weapon_Plus1 = 1;
        }
    }
    /* OGBUG: "makes generic units ignore weapon immunity" AKA "gives generic units non-normal weapons" */
    /* 'Chaos Channels' and 'Black Channels' change the race of a Normal/Non-Fantastic Unit Type */
    if(
        ((battle_unit->Abilities & UA_FANTASTIC) != 0)
        ||
        (battle_unit->race > rt_Troll)
    )
    {
        if(battle_unit->Weapon_Plus1 == 0)
        {
            battle_unit->Weapon_Plus1 = 1;
        }
    }
    if((battle_unit->Abilities & UA_NONCORPOREAL) != 0)
    {
        battle_unit->Move_Flags |= MV_SWIMMING;
    }
    Apply_Enchantment_And_Mutation_Effects(battle_unit, battle_unit_enchantments, unit_mutations);
    battle_unit->movement_points = (int8_t)Unit_Moves2(unit_idx);
    battle_unit_enchantments = battle_unit->item_enchantments | _UNITS[unit_idx].enchantments;
    /* OGBUG: could cause a display discrepancy by overriding Wind Mastery malus overland ... only if an opposing Wind Mastery has reduced the moves of a ship with Flight */
    if(battle_unit->movement_points < 3)  /* OGBUG: tests moves, sets moves2 */
    {
        if((battle_unit_enchantments & UE_FLIGHT) != 0)
        {
            battle_unit->movement_points = 6;
        }
    }
    battle_unit->hits = (int8_t)Unit_Hit_Points(unit_idx);
}


// WZD o116p08
void Apply_Enchantment_And_Mutation_Effects(struct s_BATTLE_UNIT * battle_unit, uint32_t battle_unit_enchantments, uint8_t unit_mutations)
{
    if((battle_unit_enchantments & UE_WATER_WALKING) != 0)
    {
        battle_unit->Move_Flags |= MV_SWIMMING;
    }
    if((battle_unit_enchantments & UE_TRUE_SIGHT) != 0)
    {
        battle_unit->Attribs_1 |= USA_IMMUNITY_ILLUSION;
    }
    if((battle_unit_enchantments & UE_INVULNERABILITY) != 0)
    {
        battle_unit->Attribs_1 |= USA_IMMUNITY_WEAPON;
    }
    if((battle_unit_enchantments & UE_FLIGHT) != 0)
    {
        battle_unit->Move_Flags |= MV_FLYING;
    }
    if((battle_unit_enchantments & UE_WRAITH_FORM) != 0)
    {
        battle_unit->Abilities |= UA_NONCORPOREAL;
        battle_unit->Attribs_1 |= USA_IMMUNITY_WEAPON;
        battle_unit->Move_Flags |= MV_SWIMMING;
    }
    if((unit_mutations & UM_UNDEAD) != 0)
    {
        battle_unit->race = rt_Death;
        battle_unit->Abilities |= UA_FANTASTIC;
    }
    if((battle_unit_enchantments & UE_BLACK_CHANNELS) != 0)
    {
        if(battle_unit->melee > 0)        
        {
            battle_unit->melee += 2;
            battle_unit->Gold_Melee += 2;
        }
        if(battle_unit->ranged_type != rat_UNDEF)
        {
            battle_unit->ranged += 1;
            battle_unit->Gold_Ranged += 1;
        }
        battle_unit->defense += 1;
        battle_unit->Gold_Defense += 1;
        battle_unit->resist += 1;
        battle_unit->Gold_Resist += 1;
        battle_unit->race = rt_Death;
        battle_unit->Abilities |= UA_FANTASTIC;
        battle_unit->Attribs_1 |= (USA_IMMUNITY_ILLUSION | USA_IMMUNITY_COLD | USA_IMMUNITY_POISON);
    }
    if((battle_unit_enchantments & UE_IRON_SKIN) != 0)
    {
        battle_unit->defense += 5;
        battle_unit->Gold_Defense += 5;
    }
    else if((battle_unit_enchantments & UE_STONE_SKIN) != 0)
    {
        battle_unit->defense += 1;
        battle_unit->Gold_Defense += 1;
    }
    if((battle_unit_enchantments & UE_GUARDIAN_WIND) != 0)
    {
        battle_unit->Attribs_1 |= USA_IMMUNITY_MISSILES;
    }
    if((battle_unit_enchantments & UE_MAGIC_IMMUNITY) != 0)
    {
        battle_unit->Attribs_1 |= USA_IMMUNITY_MAGIC;
    }
    if((battle_unit_enchantments & UE_FLAME_BLADE) != 0)
    {
        if(battle_unit->melee > 0)
        {
            battle_unit->melee += 2;
            battle_unit->Gold_Melee += 2;
        }
        if(
            (battle_unit->ranged_type == srat_Thrown)
            ||
            ((battle_unit->ranged_type / 10) == rag_Missile)
        )
        {
            battle_unit->ranged += 2;
            battle_unit->Gold_Ranged += 2;
        }
        if(battle_unit->Weapon_Plus1 == 0)
        {
            battle_unit->Weapon_Plus1 = 1;
        }
    }
    if((battle_unit_enchantments & UE_GIANT_STRENGTH) != 0)
    {
        if(battle_unit->melee > 0)
        {
            battle_unit->melee += 1;
            battle_unit->Gold_Melee += 1;
        }
        if(battle_unit->ranged_type == srat_Thrown)
        {
            battle_unit->ranged += 1;
            battle_unit->Gold_Ranged += 1;
        }
    }
    if((battle_unit_enchantments & UE_IMMOLATION) != 0)
    {
        battle_unit->Attribs_2 |= USA_IMMOLATION;
    }
    if((battle_unit_enchantments & UE_ELDRITCH_WEAPON) != 0)
    {
        battle_unit->melee_attack_attributes |= Att_EldrWeap;
        if(
            ((battle_unit->ranged_type / 10) == rag_Missile)
            ||
            (battle_unit->ranged_type == srat_Thrown)
        )
        {
            battle_unit->ranged_attack_attributes |= Att_EldrWeap;
        }
        if(battle_unit->Weapon_Plus1 == 0)
        {
            battle_unit->Weapon_Plus1 = 1;
        }
    }
    if((unit_mutations & CC_ARMOR) != 0)
    {
        battle_unit->defense += 3;
        battle_unit->Gold_Defense += 3;
        battle_unit->race = rt_Chaos;
    }
    if((unit_mutations & CC_FLIGHT) != 0)
    {
        battle_unit->Move_Flags |= MV_FLYING;
        battle_unit->race = rt_Chaos;
    }
    if((unit_mutations & CC_BREATH) != 0)
    {
        battle_unit->ranged = 2;
        battle_unit->ranged_type = srat_FireBreath;
        battle_unit->race = rt_Chaos;
    }
    if((battle_unit_enchantments & UE_LION_HEART) != 0)
    {
        if(battle_unit->melee > 0)
        {
            battle_unit->melee += 3;
            battle_unit->Gold_Melee += 3;
        }
        if(
            ((battle_unit->ranged_type / 10) == rag_Missile)
            ||
            ((battle_unit->ranged_type / 10) == rag_Boulder)
            ||
            (battle_unit->ranged_type == srat_Thrown)
        )
        {
            battle_unit->ranged += 3;
            battle_unit->Gold_Ranged += 3;
        }
        battle_unit->resist += 3;
        battle_unit->Gold_Resist += 3;
    }
    if((battle_unit_enchantments & UE_HOLY_ARMOR) != 0)
    {
        battle_unit->defense += 2;
        battle_unit->Gold_Defense += 2;
    }
    /* OGBUG: undead are also supposed to gain Cold, Poison, and Illusions Immunities (native Death units have all of these anyway) */
    if(battle_unit->race == rt_Death)
    {
        battle_unit->Attribs_1 |= USA_IMMUNITY_DEATH;
    }
    if((battle_unit_enchantments & UE_BERSERK) != 0)
    {
        if(battle_unit->melee > 0)
        {
            battle_unit->Gold_Melee += battle_unit->melee;
            battle_unit->melee *= 2;
        }
        battle_unit->Grey_Defense = battle_unit->defense;
        battle_unit->defense = -20;
    }
}


// WZD o116p09
/*
OGBUG: ignores Crusade, Warlord, and experience points in general when considering Heroism, which is either what should happen overland too, or not here either
~ CITYCALC.c Calc_Unit_Level()
~ MoO2  Module: ERICNET  Calc_Ship_Level_()
*/
void Battle_Unit_Level_Stats(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit)
{
    int16_t unit_level = 0;
    unit_level = _UNITS[unit_idx].Level;
    if(
        ((_UNITS[unit_idx].enchantments & UE_HEROISM) != 0)
        ||
        ((battle_unit->enchantments & UE_HEROISM) != 0)
    )
    {
        SETMIN(unit_level, 3);
    }
    if(_UNITS[unit_idx].Hero_Slot > ST_UNDEFINED)
    {
        if(unit_level > 0)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
            battle_unit->defense++;
        }
        if(unit_level > 1)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }
        if(unit_level > 2)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
            battle_unit->defense++;
        }
        if(unit_level > 3)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }
        if(unit_level > 4)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
            battle_unit->defense++;
        }
        if(unit_level > 5)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }
        if(unit_level > 6)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
            battle_unit->defense++;
        }
        if(unit_level > 7)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }
    }
    else
    {
        if(unit_level > 0)
        {
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }
        if(unit_level > 1)
        {
            battle_unit->resist++;
            battle_unit->defense++;
        }
        if(unit_level > 2)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }
        if(unit_level > 3)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            battle_unit->defense++;
        }
        if(unit_level > 4)
        {
            battle_unit->tohit++;
            battle_unit->resist++;
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
            }
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
            }
        }
    }
}


// WZD o116p10
void Battle_Unit_Hero_Skill_Stats(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit)
{
    int16_t hero_owner_idx = 0;
    int16_t hero_type = 0;
    hero_owner_idx = _UNITS[unit_idx].owner_idx;
    hero_type = _UNITS[unit_idx].type;
    if(
        (HERO_NOBLE(hero_owner_idx, hero_type))
        ||
        (_players[hero_owner_idx].famous > ST_FALSE)
    )
    {
        battle_unit->upkeep = 0;
    }
    if(HERO_AGILITY(hero_owner_idx, hero_type))
    {
        battle_unit->defense = (battle_unit->defense + (_UNITS[unit_idx].Level + 1));
    }
    if(HERO_AGILITY2(hero_owner_idx,hero_type))
    {
        battle_unit->defense = (battle_unit->defense + (((_UNITS[unit_idx].Level + 1) * 3) / 2));
    }
    if(HERO_BLADEMASTER(hero_owner_idx,hero_type))
    {
        battle_unit->tohit = (battle_unit->tohit + ((_UNITS[unit_idx].Level + 1) / 2));
    }
    if(HERO_BLADEMASTER2(hero_owner_idx,hero_type))
    {
        battle_unit->tohit = (battle_unit->tohit + (((_UNITS[unit_idx].Level + 1) * 3) / 4));
    }
    if(HERO_MIGHT(hero_owner_idx,hero_type))
    {
        battle_unit->melee = (battle_unit->melee + (_UNITS[unit_idx].Level + 1));
    }
    if(HERO_MIGHT2(hero_owner_idx,hero_type))
    {
        battle_unit->melee = (battle_unit->melee + (((_UNITS[unit_idx].Level + 1) * 3) / 2));
    }
    if(
        (HERO_ARCANE_POWER(hero_owner_idx,hero_type))
        &&
        ((battle_unit->ranged_type / 10) == rag_Magic)
    )
    {
        battle_unit->ranged = (battle_unit->ranged + (_UNITS[unit_idx].Level + 1));
    }
    if(
        (HERO_ARCANE_POWER2(hero_owner_idx,hero_type))
        &&
        ((battle_unit->ranged_type / 10) == rag_Magic)
    )
    {
        battle_unit->ranged = (battle_unit->ranged + (((_UNITS[unit_idx].Level + 1) * 3) / 2));
    }
    if(_HEROES2[hero_owner_idx]->heroes[hero_type].Casting_Skill > 0)
    {
        battle_unit->mana_max = ((((_HEROES2[hero_owner_idx]->heroes[hero_type].Casting_Skill + 1) * (_UNITS[unit_idx].Level + 1)) * 5) / 2);
    }
    else
    {
        battle_unit->mana_max = 0;
    }
    if(HERO_LUCKY(hero_owner_idx,hero_type))
    {
        battle_unit->tohit += 1;
        battle_unit->toblock += 1;
        battle_unit->resist += 1;
        battle_unit->Gold_Resist += 1;
    }
}


// WZD o116p11
/*
Page 98  (PDF Page 103)
STRUCTURES INFLUENCING COMBAT
Magic Nodes
Magic nodes can affect combat in one of two ways.
They may increase (by one) the attack strengths (swords and ranged weapons), defenses (shields) and resistance (crosses) of all creatures from the same magic realm as the node
  (i.e., a nature node would exert this positive effect on all creatures of nature such as sprites and basilisks)
  when these creatures are fighting in any area controlled by the node.
*/
/*
¿ MoO2  Module: COMBINIT  Load_Combat_Ship_() ?
    Bonus_For_Regular_Mods_()
    Bonus_For_Special_Mods_()
*/
void Battle_Unit_Special_Stats(struct s_BATTLE_UNIT * battle_unit)
{
    int16_t magic_node_structure = 0;
    int16_t item_idx = 0;
    int16_t mutation_flags = 0;
    uint32_t enchantments = 0;
    int16_t unit_idx = 0;
    int16_t itr_hero_items = 0;
    unit_idx = battle_unit->unit_idx;
    magic_node_structure = ST_FALSE;
    if(
        (
            (_combat_node_type == cnt_Sorcery)
            &&
            (battle_unit->race == rt_Sorcery)
        )
        ||
        (
            (_combat_node_type == cnt_Chaos)
            &&
            (battle_unit->race == rt_Chaos)
        )
        ||
        (
            (_combat_node_type == cnt_Nature)
            &&
            (battle_unit->race == rt_Nature)
        )
    )
    {
        magic_node_structure = ST_TRUE;
    }
    if(magic_node_structure == ST_TRUE)
    {
        battle_unit->melee += 2;
        battle_unit->Gold_Melee += 2;
        if(battle_unit->ranged > 0)
        {
            battle_unit->ranged += 2;
            battle_unit->Gold_Ranged += 2;
        }
        battle_unit->resist += 2;
        battle_unit->Gold_Resist += 2;
        battle_unit->defense += 2;
        battle_unit->Gold_Defense += 2;
    }
    if(battle_unit->melee > 0 && battle_unit->race < rt_Arcane)
    {
        battle_unit->melee += _battlefield_leadership[battle_unit->controller_idx];
        battle_unit->Gold_Melee += _battlefield_leadership[battle_unit->controller_idx];
    }
    if(battle_unit->ranged > 0 && battle_unit->race < rt_Arcane && (battle_unit->ranged_type / 10) != rag_Magic)
    {
        battle_unit->ranged += _battlefield_leadership[battle_unit->controller_idx] / 2;
        battle_unit->Gold_Ranged += _battlefield_leadership[battle_unit->controller_idx] / 2;
    }
    if(battle_unit->melee > 0)
    {
        battle_unit->melee += _battlefield_holybonus[battle_unit->controller_idx];
        battle_unit->Gold_Melee += _battlefield_holybonus[battle_unit->controller_idx];
    }
    battle_unit->resist += _battlefield_resistall[battle_unit->controller_idx];
    battle_unit->Gold_Resist += _battlefield_resistall[battle_unit->controller_idx];
    battle_unit->defense += _battlefield_holybonus[battle_unit->controller_idx];
    battle_unit->Gold_Defense += _battlefield_holybonus[battle_unit->controller_idx];
    if(_UNITS[unit_idx].Hero_Slot > ST_UNDEFINED)
    {
        for(itr_hero_items = 0; itr_hero_items < NUM_HERO_ITEMS; itr_hero_items++)
        {
            item_idx = _players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[itr_hero_items];
            if(item_idx > ST_UNDEFINED)
            {
                if(_ITEMS[item_idx].Powers & IP_HASTE)
                {
                    battle_unit->combat_effects |= bue_Haste;
                }
            }
        }
    }
    mutation_flags = _UNITS[battle_unit->unit_idx].mutations;
    if(
        (combat_enchantments[HIGH_PRAYER_ATTKR] == 1 && battle_unit->controller_idx == _combat_attacker_player)
        ||
        (combat_enchantments[HIGH_PRAYER_DFNDR] == 1 && battle_unit->controller_idx == _combat_defender_player)
    )
    {
        battle_unit->tohit++;
        battle_unit->toblock++;
        battle_unit->resist++;
        battle_unit->Gold_Resist++;
        if(battle_unit->melee > 0)
        {
            battle_unit->melee += 2;
            battle_unit->Gold_Melee += 2;
        }
        battle_unit->resist += 2;
        battle_unit->Gold_Resist += 2;
        battle_unit->defense += 2;
        battle_unit->Gold_Defense += 2;
    }
    else if(
        (combat_enchantments[PRAYER_ATTKR] == 1 && battle_unit->controller_idx == _combat_attacker_player)
        ||
        (combat_enchantments[PRAYER_DFNDR] == 1 && battle_unit->controller_idx == _combat_defender_player)
    )
    {
        battle_unit->tohit++;
        battle_unit->toblock++;
        battle_unit->resist++;
        battle_unit->Gold_Resist++;
    }
    if(battle_unit->race == rt_Life)
    {
        if(combat_enchantments[TRUE_LIGHT_ATTKR] != 0 || combat_enchantments[TRUE_LIGHT_DFNDR] != 0)
        {
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
                battle_unit->Gold_Melee++;
            }
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
                battle_unit->Gold_Ranged++;
            }
            battle_unit->resist++;
            battle_unit->Gold_Resist++;
            battle_unit->defense++;
            battle_unit->Gold_Defense++;
        }
        if(combat_enchantments[DARKNESS_ATTKR] != 0 || combat_enchantments[DARKNESS_DFNDR] != 0)
        {
            battle_unit->melee--;
            battle_unit->Grey_Melee++;
            battle_unit->ranged--;
            battle_unit->Grey_Ranged++;
            battle_unit->resist--;
            battle_unit->Grey_Resist++;
            battle_unit->defense--;
            battle_unit->Grey_Defense++;
        }
    }
    if(battle_unit->race == rt_Death)
    {
        if(combat_enchantments[DARKNESS_ATTKR] != 0 || combat_enchantments[DARKNESS_DFNDR] != 0)
        {
            if(battle_unit->melee > 0)
            {
                battle_unit->melee++;
                battle_unit->Gold_Melee++;
            }
            if(battle_unit->ranged > 0)
            {
                battle_unit->ranged++;
                battle_unit->Gold_Ranged++;
            }
            battle_unit->resist++;
            battle_unit->Gold_Resist++;
            battle_unit->defense++;
            battle_unit->Gold_Defense++;
        }
        if(combat_enchantments[TRUE_LIGHT_ATTKR] != 0 || combat_enchantments[TRUE_LIGHT_DFNDR] != 0)
        {
            battle_unit->melee--;
            battle_unit->Grey_Melee++;
            battle_unit->ranged--;
            battle_unit->Grey_Ranged++;
            battle_unit->resist--;
            battle_unit->Grey_Resist++;
            battle_unit->defense--;
            battle_unit->Grey_Defense++;
        }
    }
    enchantments = _UNITS[battle_unit->unit_idx].enchantments | battle_unit->enchantments;
    if(
        ((combat_enchantments[METAL_FIRES_ATTKR] != 0 && battle_unit->controller_idx == _combat_attacker_player)
        ||
        (combat_enchantments[METAL_FIRES_DFNDR] != 0 && battle_unit->controller_idx != _combat_attacker_player))
        &&
        !(battle_unit->Abilities & UA_FANTASTIC)
        &&
        !(enchantments & UE_FLAME_BLADE))
    {
        if(battle_unit->melee > 0)
        {
            battle_unit->melee++;
            battle_unit->Gold_Melee++;
        }
        if((battle_unit->ranged_type / 10 == rag_Missile) || (battle_unit->ranged_type == srat_Thrown))
        {
            battle_unit->ranged++;
            battle_unit->Gold_Ranged++;
        }
        if(battle_unit->Weapon_Plus1 == 0)
        {
            battle_unit->Weapon_Plus1 = 1;
        }
    }
    if((combat_enchantments[MASS_INVISIBILITY_ATTKR] != 0 && battle_unit->controller_idx == _combat_attacker_player) ||
        (combat_enchantments[MASS_INVISIBILITY_DFNDR] != 0 && battle_unit->controller_idx != _combat_attacker_player))
    {
        battle_unit->enchantments |= UE_INVISIBILITY;
    }
    if((combat_enchantments[WARP_REALITY_ATTKR] != 0 || combat_enchantments[WARP_REALITY_DFNDR] != 0) &&
        battle_unit->race != rt_Chaos)
    {
        battle_unit->tohit -= 2;
    }
    if((combat_enchantments[BLACK_PRAYER_ATTKR] != 0 && battle_unit->controller_idx != _combat_attacker_player) ||
        (combat_enchantments[BLACK_PRAYER_DFNDR] != 0 && battle_unit->controller_idx == _combat_attacker_player))
    {
        battle_unit->melee--;
        battle_unit->ranged--;
        battle_unit->resist -= 2;
        battle_unit->Grey_Melee++;
        battle_unit->Grey_Ranged++;
        battle_unit->Grey_Resist += 2;
        battle_unit->defense--;
        battle_unit->Grey_Defense++;
    }
    if(battle_unit->combat_effects & bue_Mind_Twist)
    {
        battle_unit->tohit--;
        battle_unit->resist--;
        battle_unit->Gold_Resist++;
    }
    if(battle_unit->combat_effects & bue_Vertigo)
    {
        battle_unit->tohit -= 2;
        battle_unit->defense--;
        battle_unit->Grey_Defense++;
    }
    if(battle_unit->combat_effects & bue_Weakness)
    {
        battle_unit->melee -= 2;
        battle_unit->Grey_Melee += 2;
        if((battle_unit->ranged_type / 10 == rag_Missile) || (battle_unit->ranged_type / 10 == srat_Thrown))
        {
            battle_unit->ranged -= 2;
            battle_unit->Grey_Ranged += 2;
        }
    }
    if(battle_unit->combat_effects & bue_Mind_Storm)
    {
        battle_unit->melee -= 5;
        battle_unit->ranged -= 5;
        battle_unit->resist -= 5;
        battle_unit->defense -= 5;
        battle_unit->Grey_Defense += 5;
        battle_unit->Grey_Melee += 5;
        battle_unit->Grey_Ranged += 5;
        battle_unit->Grey_Resist += 5;
    }
    enchantments = (_UNITS[battle_unit->unit_idx].enchantments ^ battle_unit->enchantments) & battle_unit->enchantments;
    Apply_Enchantment_And_Mutation_Effects(battle_unit, enchantments, mutation_flags);
    if(battle_unit->combat_effects & bue_Warped_Attack)
    {
        battle_unit->Grey_Melee += (battle_unit->melee + 1) / 2;
        battle_unit->melee -= (battle_unit->melee + 1) / 2;
    }
    if(battle_unit->combat_effects & bue_Warped_Defense)
    {
        battle_unit->Grey_Defense += (battle_unit->defense + 1) / 2;
        battle_unit->defense -= (battle_unit->defense + 1) / 2;
    }
    if(battle_unit->combat_effects & bue_Warped_Resist)
    {
        battle_unit->Grey_Resist += battle_unit->resist;
        battle_unit->resist = 0;
    }
    if(battle_unit->combat_effects & bue_Shatter)
    {
        if(battle_unit->melee > 1)
        {
            battle_unit->melee = 1;
            battle_unit->Grey_Melee = battle_unit->melee - 1;
        }
        if(battle_unit->ranged > 1)
        {
            battle_unit->ranged = 1;
            battle_unit->Grey_Ranged = battle_unit->ranged - 1;
        }
    }
    if(battle_unit->melee < 0)
    {
        battle_unit->melee = 0;
    }
    if(battle_unit->ranged < 0)
    {
        battle_unit->ranged = 0;
    }
    if(battle_unit->defense < 0)
    {
        battle_unit->defense = 0;
    }
    if((battle_unit->combat_effects & bue_Web) && (battle_unit->Move_Flags & MV_FLYING))
    {
        battle_unit->Move_Flags ^= MV_FLYING;
    }
    battle_unit[0].hits = (int8_t)Battle_Unit_Hit_Points(&battle_unit[0]);
}


// WZD o116p12
int16_t NIU_Unit_Has_Spell_Enchantment(int16_t unit_idx, int16_t spell_idx)
{
    uint32_t enchantments = 0;
    int16_t has_it = 0;
    Load_Battle_Unit(unit_idx, global_battle_unit);
    enchantments = _UNITS[unit_idx].enchantments | global_battle_unit->item_enchantments;
    if((enchantments & spell_data_table[spell_idx].enchantments) != 0)
    {
        has_it = ST_TRUE;
    }
    else
    {
        has_it = ST_FALSE;
    }
    return has_it;
}
