/*
    WIZARDS.EXE
        ovr130
*/

#include "NEXTTURN.H"
#include "Spells130.H"

#include "MOX/MOX_DEF.H"
#include "MOX/MOX_TYPE.H"

#include "MOX/MOM_Data.H"

#include "Terrain.H"



/*
    WIZARDS.EXE  ovr130
*/


// WZD o130p01
/*

XREF:
    j_UNIT_SetEquipSlots()
        WIZ_HireHero()

e.g.,
WIZ_HireHero()
    |-> Hero_Slot_Types(unit_type_idx, _players[player_idx].Heroes[hero_slot_idx].Item_Slots)

Page 28  (PDF Page 33)
Note that the weapons and armor a hero can be equipped with depend on
the hero’s type; some heroes are mages, others are warriors, etc.
*/
void Hero_Slot_Types(int16_t unit_type_idx, int16_t item_slots[])
{

    if(_unit_type_table[unit_type_idx].hero_type == ht_IDK2)
    {
        item_slots[0] = ist_SwordStaff_Slot;
        item_slots[1] = ist_Armor_Slot;
        item_slots[2] = ist_Misc_Slot;
    }
    else if(_unit_type_table[unit_type_idx].hero_type == ht_Mage)
    {
        item_slots[0] = ist_Staff_Slot;
        item_slots[1] = ist_Misc_Slot;
        item_slots[2] = ist_Misc_Slot;
    }
    else if(_unit_type_table[unit_type_idx].hero_type == ht_IDK1)
    {
        item_slots[0] = ist_Bow_Slot;
        item_slots[1] = ist_Armor_Slot;
        item_slots[2] = ist_Misc_Slot;
    }
    else  /* ht_Warrior */
    {
        item_slots[0] = ist_Sword_Slot;
        item_slots[1] = ist_Armor_Slot;
        item_slots[2] = ist_Misc_Slot;
    }

}


// WZD o130p02
// drake178: sub_AF331()
//Cast_DeathWish()

// WZD o130p03
// drake178: sub_AF56D()
//IDK_Spell_GreatUnsummoning()

// WZD o130p04
// drake178: sub_AF7C1()
//Cast_BlackWind()

// WZD o130p05
/*
171  spl_Drain_Power
Drain Power:
Death. Instant. Casting Cost: 50 mana. Uncommon.
Drains from 50 to 150 points of magic power from a target wizard’s mana reserve.
*/
int16_t Apply_Drain_Power(int16_t player_idx)
{
    int16_t itr = 0;
    int16_t return_value = 0;  // _SI_

    return_value = 0;

    for(itr = 0; itr < 10; itr++)
    {

        // 10 * Random(20) = {10, ..., 200}
        return_value += Random(20);

    }

    if(_players[player_idx].mana_reserve < return_value)
    {

        _players[player_idx].mana_reserve = return_value;

    }

    _players[player_idx].mana_reserve -= return_value;

    return return_value;

}


// WZD o130p06
// drake178: sAFA06_Anim_EarthLore()
//sAFA06_Anim_EarthLore()

// WZD o130p07
// drake178: sub_AFB7F()
//Cast_SpellBinding()

// WZD o130p08
// drake178: sub_AFCA8()
//Cast_ChangeTerain()

// WZD o130p09
// drake178: sub_B01F7()
//Cast_RaiseVolcano()

// WZD o130p10
// drake178: sub_B0692()
//Cast_Transmute()

// WZD o130p11
// drake178: sub_B0C07()
//Cast_Corruption()

// WZD o130p12
// drake178: sub_B1108()
//Cast_WordOfRecall()

// WZD o130p13
// drake178: sB1280_Anim_EnchantRoad()
//Cast_EnchantRoad()

// WZD o130p14
// drake178: sub_B148C()
//Cast_SpellWard()

// WZD o130p15
// drake178: sub_B1843()
/*
    turns off bonus from Holy Weapon / High Prayer
*/
void Cast_HolyArms(int16_t player_idx)
{
    int16_t itr = 0;  // _DI_

    for(itr = 0; itr < _units; itr++)
    {

        if(
            (_UNITS[itr].owner_idx == player_idx)
            &&
            ((_UNITS[itr].enchantments & USA_HOLYBONUS) != 0)
        )
        {

            _UNITS[itr].enchantments ^= USA_HOLYBONUS;

        }

    }

}


// WZD o130p16
// drake178: CTY_Consecration()
/*
; processes the effect of Consecration - removes all
; negative city enchantments and all corruption from
; within a 2-tile radius of the city
;
; BUG: will cleanse the corner tiles that are outside
; the catchment area
*/
/*

Consecration:
Life. City Enchantment. Casting Cost: 400 mana;
Upkeep: 8 mana/turn. Very Rare.
Dispels all negative enchantments on a city and protects the city
from further chaos and death enchantments (whether city-specific
or global).
*/
void Apply_Spell_Consecration(int16_t city_idx)
{
    int16_t cur_wx = 0;
    int16_t city_wp = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;  // _DI_

    _CITIES[city_idx].enchantments[CHAOS_RIFT] = 0;
    _CITIES[city_idx].enchantments[EVIL_PRESENCE] = 0;
    _CITIES[city_idx].enchantments[CURSED_LANDS] = 0;
    _CITIES[city_idx].enchantments[FAMINE] = 0;
    _CITIES[city_idx].enchantments[PESTILENCE] = 0;
    _CITIES[city_idx].enchantments[CHAOS_RIFT] = 0;

    city_wp = _CITIES[city_idx].wp;

    for(itr_wx = (_CITIES[city_idx].wx - 2); itr_wx < (_CITIES[city_idx].wx + 2); itr_wx++)
    {

        if(itr_wx < 0)
        {
            cur_wx = (itr_wx + WORLD_WIDTH);
        }
        else if(itr_wx > WORLD_WIDTH)
        {
            cur_wx = (itr_wx - WORLD_WIDTH);
        }
        else
        {
            cur_wx = itr_wx;
        }

        for(itr_wy = (_CITIES[city_idx].wy - 2); itr_wy < (_CITIES[city_idx].wy + 2); itr_wy++)
        {

            if(
                (itr_wy >= 0)
                &&
                (itr_wy <= WORLD_HEIGHT)
            )
            {

                if(Square_Has_Corruption(cur_wx, itr_wy, city_wp) != ST_FALSE)
                {
                    Square_Clear_Corruption(cur_wx, itr_wy, city_wp);
                }

            }

        }

    }

    _page_flip_effect = 3;

}


// WZD o130p17
void Apply_Subversion(int16_t player_idx)
{
    int16_t itr = 0;  // _CX_

    for(itr = 0; itr < _num_players; itr++)
    {

        if(itr == 0)
        {
            continue;
        }

        if(itr != player_idx)
        {

            if(_players[itr].Dipl.Contacted[player_idx] == ST_TRUE)
            {

                _players[itr].Dipl.Visible_Rel[player_idx] -= 25;

                if(_players[itr].Dipl.Visible_Rel[player_idx] < -100)
                {

                    _players[itr].Dipl.Visible_Rel[player_idx] = -100;

                }

                _players[itr].Dipl.Visible_Rel[player_idx] = _players[player_idx].Dipl.Visible_Rel[itr];

                _players[player_idx].peace_duration[itr] = 0;

                _players[itr].peace_duration[player_idx] = 0;

            }

        }

    }

}


// WZD o130p18
// drake178: sub_B1ABE()
void Cast_NaturesAwareness(int16_t player_idx)
{

    if(player_idx == HUMAN_PLAYER_IDX)
    {

        Allocate_Reduced_Map();
        Cheat_Reveal();
        All_AI_Players_Contacted();
        Release_Block(_screen_seg);

    }

}
