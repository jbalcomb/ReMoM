/*
    Items

    WIZARDS.EXE
        ovr109
*/

#include "../../MoX/src/GENDRAW.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/Util.h"  /* Clear_Structure(); Swap_Short() */

#include "City_ovr55.h"  /* Player_Add_Mana() */
#include "Items.h"
#include "ItemScrn.h"
#include "MainScr.h"
#include "MOM_SCR.h"
#include "SBookScr.h"
#include "UNITTYPE.h"  /* srat_Thrown */



// WZD dseg:5AEA                                                 BEGIN:  ovr109

// WZD dseg:5AEA
char cnst_MESSAGE_File[] = "message";

// WZD dseg:5AEA                                                 END:  ovr109





// WZD o109p01
/*
; processes the passed list of items being awarded to
; the selected wizard - either through the items screen
; if it's the human player, or AI_Process_Item_Pool otherwise
; has some BUGs in both branches

hard-coded to 'Main Screen'

Combat()
    |-> Player_Process_Item_Pool(combat_attacker_player_idx, Item_Count, &Item_List[0])

*/
void Player_Process_Item_Pool(int16_t player_idx, int16_t item_count, int16_t item_list[])
{
    if(item_count > 0)
    {
        if(player_idx == HUMAN_PLAYER_IDX)
        {

            Allocate_Reduced_Map();

            Full_Draw_Main_Screen();

            LBX_Load_Data_Static(cnst_MESSAGE_File, 0, (SAMB_ptr)GUI_NearMsgString, 67, 1, 150);

            Warn0(GUI_NearMsgString);
            
            Dissolve_Main_Screen();

            Process_Item_Pool(item_count, item_list);

            current_screen = scr_Main_Screen;

            Dissolve_Main_Screen();
        }
        else
        {
            AI_Process_Item_Pool(player_idx, item_count, item_list);  /* CLAUDE */  /* asm: push [bp+item_list] / push _SI_item_count / push _DI_player_idx / call AI_ProcessItems__STUB - was a TODO */
        }
    }
}


// WZD o109p02
/*
    sets 'cost' to 0
    NOTE: Destroy_Item() sets cost to -1

    ¿ 'cost' is (Base) 'Cost' ?
    ¿ what does it mean for this to be 0 ?
*/
void Remove_Item(int16_t item_idx)
{
    _ITEMS[item_idx].cost = 0;
}


// WZD o109p03
// drake178: AI_ProcessItems()
/*
AI (and neutral) share of an item pool, from Player_Process_Item_Pool()
    neutral player: every item is removed
    otherwise: the pool and every item the player's heroes carry go into one list, the heroes' slots are emptied,
    the heroes are sorted by unit type (highest first), each gets AI_Equip_Hero_Best_Items() and then AI_Equip_Hero_Empty_Slots(),
    and each item nobody took is removed and pays the player half its cost in mana
*/
void AI_Process_Item_Pool(int16_t player_idx, int16_t item_count, int16_t item_list[])
{
    int16_t pool_count = 0;
    int16_t itr_inner = 0;
    int16_t hero_count = 0;
    int16_t pool_list[40] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t hero_list[NUM_HEROES] = { 0, 0, 0, 0, 0, 0 };
    int16_t itr = 0;  // _DI_

    if(player_idx == NEUTRAL_PLAYER_IDX)
    {

        for(itr = 0; itr < item_count; itr++)
        {

            Remove_Item(item_list[itr]);

        }

        return;

    }

    hero_count = 0;

    pool_count = 0;

    for(itr = 0; itr < item_count; itr++)
    {

        pool_list[pool_count] = item_list[itr];

        pool_count++;

    }

    for(itr = 0; itr < NUM_HEROES; itr++)
    {

        if(_players[player_idx].Heroes[itr].unit_idx > ST_UNDEFINED)
        {

            hero_list[hero_count] = _players[player_idx].Heroes[itr].unit_idx;

            hero_count++;

            for(itr_inner = 0; itr_inner < NUM_HERO_ITEMS; itr_inner++)
            {

                if(_players[player_idx].Heroes[itr].Items[itr_inner] > ST_UNDEFINED)
                {

                    pool_list[pool_count] = _players[player_idx].Heroes[itr].Items[itr_inner];

                    pool_count++;

                    _players[player_idx].Heroes[itr].Items[itr_inner] = ST_UNDEFINED;

                }

            }

        }

    }

    /* bubble sort, highest unit type first */
    if(hero_count > 1)
    {

        for(itr_inner = 1; itr_inner < hero_count; itr_inner++)
        {

            for(itr = 0; itr < (hero_count - itr_inner); itr++)
            {

                if(_UNITS[hero_list[itr]].type < _UNITS[hero_list[(itr + 1)]].type)
                {

                    Swap_Short(&hero_list[itr], &hero_list[(itr + 1)]);

                }

            }

        }

    }

    for(itr = 0; itr < hero_count; itr++)
    {

        AI_Equip_Hero_Best_Items(player_idx, _UNITS[hero_list[itr]].Hero_Slot, &pool_list[0], &pool_count);

    }

    for(itr = 0; itr < hero_count; itr++)
    {

        AI_Equip_Hero_Empty_Slots(player_idx, _UNITS[hero_list[itr]].Hero_Slot, &pool_list[0], &pool_count);

    }

    for(itr = 0; itr < pool_count; itr++)
    {

        Player_Add_Mana(player_idx, (_ITEMS[pool_list[itr]].cost / 2));

        Remove_Item(pool_list[itr]);

    }

}


// WZD o109p04
// drake178: AI_EquipHeroFirst()
/*
first equipment pass for an AI hero: for each of its 3 slots, takes the costliest item in the pool that fits the slot type
    an axe is worth 0 unless the hero's unit type has a thrown ranged attack
    a bow is worth +2000 in a Bow slot
    an item with Spell Skill or Spell Save is worth 0 to a hero with no casting skill (Sword, Bow and Misc slots)
the item goes into the slot and is removed from the pool (item_list[], *item_count)
the caller (AI_Process_Item_Pool()) has emptied every slot into the pool first; AI_Equip_Hero_Empty_Slots() fills what is left
*/
void AI_Equip_Hero_Best_Items(int16_t player_idx, int16_t hero_slot_idx, int16_t item_list[], int16_t * item_count)
{
    int16_t itr_hero_items = 0;
    int16_t highest_value = 0;
    int16_t list_idx = 0;
    int16_t pool_count = 0;
    int16_t item_value = 0;
    int16_t unit_type = 0;
    int16_t itr_items = 0;  // _DI_

    pool_count = *item_count;

    for(itr_hero_items = 0; itr_hero_items < NUM_HERO_ITEMS; itr_hero_items++)
    {

        unit_type = _UNITS[_players[player_idx].Heroes[hero_slot_idx].unit_idx].type;

        switch(_players[player_idx].Heroes[hero_slot_idx].Item_Slots[itr_hero_items])
        {

            case ist_Sword_Slot:
            {

                list_idx = ST_UNDEFINED;

                highest_value = 0;

                for(itr_items = 0; itr_items < pool_count; itr_items++)
                {

                    if(
                        (_ITEMS[item_list[itr_items]].type == it_Sword)
                        ||
                        (_ITEMS[item_list[itr_items]].type == it_Mace)
                        ||
                        (_ITEMS[item_list[itr_items]].type == it_Axe)
                    )
                    {

                        item_value = _ITEMS[item_list[itr_items]].cost;

                        if(
                            (_ITEMS[item_list[itr_items]].type == it_Axe)
                            &&
                            (_unit_type_table[unit_type].Ranged_Type != srat_Thrown)
                        )
                        {

                            item_value = 0;

                        }

                        if(
                            (
                                (_ITEMS[item_list[itr_items]].spell_skill > 0)
                                ||
                                (_ITEMS[item_list[itr_items]].spell_save > 0)
                            )
                            &&
                            (_HEROES2[player_idx]->heroes[unit_type].Casting_Skill == 0)
                        )
                        {

                            item_value = 0;

                        }

                        if(item_value > highest_value)
                        {

                            list_idx = itr_items;

                            highest_value = item_value;

                        }

                    }

                }

            } break;

            case ist_Bow_Slot:
            {

                list_idx = ST_UNDEFINED;

                highest_value = 0;

                for(itr_items = 0; itr_items < pool_count; itr_items++)
                {

                    if(
                        (_ITEMS[item_list[itr_items]].type == it_Sword)
                        ||
                        (_ITEMS[item_list[itr_items]].type == it_Mace)
                        ||
                        (_ITEMS[item_list[itr_items]].type == it_Axe)
                        ||
                        (_ITEMS[item_list[itr_items]].type == it_Bow)
                    )
                    {

                        item_value = _ITEMS[item_list[itr_items]].cost;

                        if(_ITEMS[item_list[itr_items]].type == it_Bow)
                        {

                            item_value += 2000;

                        }

                        if(
                            (_ITEMS[item_list[itr_items]].type == it_Axe)
                            &&
                            (_unit_type_table[unit_type].Ranged_Type != srat_Thrown)
                        )
                        {

                            item_value = 0;

                        }

                        if(
                            (
                                (_ITEMS[item_list[itr_items]].spell_skill > 0)
                                ||
                                (_ITEMS[item_list[itr_items]].spell_save > 0)
                            )
                            &&
                            (_HEROES2[player_idx]->heroes[unit_type].Casting_Skill == 0)
                        )
                        {

                            item_value = 0;

                        }

                        if(item_value > highest_value)
                        {

                            list_idx = itr_items;

                            highest_value = item_value;

                        }

                    }

                }

            } break;

            case ist_SwordStaff_Slot:
            {

                list_idx = ST_UNDEFINED;

                highest_value = 0;

                for(itr_items = 0; itr_items < pool_count; itr_items++)
                {

                    if(
                        (_ITEMS[item_list[itr_items]].type == it_Sword)
                        ||
                        (_ITEMS[item_list[itr_items]].type == it_Mace)
                        ||
                        (_ITEMS[item_list[itr_items]].type == it_Axe)
                        ||
                        (_ITEMS[item_list[itr_items]].type == it_Staff)
                        ||
                        (_ITEMS[item_list[itr_items]].type == it_Wand)
                    )
                    {

                        item_value = _ITEMS[item_list[itr_items]].cost;

                        if(
                            (_ITEMS[item_list[itr_items]].type == it_Axe)
                            &&
                            (_unit_type_table[unit_type].Ranged_Type != srat_Thrown)
                        )
                        {

                            item_value = 0;

                        }

                        if(item_value > highest_value)
                        {

                            list_idx = itr_items;

                            highest_value = item_value;

                        }

                    }

                }

            } break;

            case ist_Staff_Slot:
            {

                list_idx = ST_UNDEFINED;

                highest_value = 0;

                for(itr_items = 0; itr_items < pool_count; itr_items++)
                {

                    if(
                        (_ITEMS[item_list[itr_items]].type == it_Staff)
                        ||
                        (_ITEMS[item_list[itr_items]].type == it_Wand)
                    )
                    {

                        item_value = _ITEMS[item_list[itr_items]].cost;

                        if(item_value > highest_value)
                        {

                            list_idx = itr_items;

                            highest_value = item_value;

                        }

                    }

                }

            }
            /* OGBUG  no break: the Staff slot falls into the Armor case, which resets list_idx and picks the costliest shield or mail - so an AI hero's Staff slot gets armor (asm: jl loc_881B3 runs straight on into loc_88221) */
            case ist_Armor_Slot:
            {

                list_idx = ST_UNDEFINED;

                highest_value = 0;

                for(itr_items = 0; itr_items < pool_count; itr_items++)
                {

                    if(
                        (_ITEMS[item_list[itr_items]].type == it_Shield)
                        ||
                        (_ITEMS[item_list[itr_items]].type == it_Chainmail)
                        ||
                        (_ITEMS[item_list[itr_items]].type == it_Platemail)
                    )
                    {

                        item_value = _ITEMS[item_list[itr_items]].cost;

                        if(item_value > highest_value)
                        {

                            list_idx = itr_items;

                            highest_value = item_value;

                        }

                    }

                }

            } break;

            case ist_Misc_Slot:
            {

                list_idx = ST_UNDEFINED;

                highest_value = 0;

                for(itr_items = 0; itr_items < pool_count; itr_items++)
                {

                    if(_ITEMS[item_list[itr_items]].type == it_Misc)
                    {

                        item_value = _ITEMS[item_list[itr_items]].cost;

                        if(
                            (
                                (_ITEMS[item_list[itr_items]].spell_skill > 0)
                                ||
                                (_ITEMS[item_list[itr_items]].spell_save > 0)
                            )
                            &&
                            (_HEROES2[player_idx]->heroes[unit_type].Casting_Skill == 0)
                        )
                        {

                            item_value = 0;

                        }

                        if(item_value > highest_value)
                        {

                            list_idx = itr_items;

                            highest_value = item_value;

                        }

                    }

                }

            } break;

        }

        if(list_idx > ST_UNDEFINED)
        {

            _players[player_idx].Heroes[hero_slot_idx].Items[itr_hero_items] = item_list[list_idx];

            Clear_Structure(list_idx, (uint8_t *)item_list, sizeof(item_list[0]), pool_count);

            pool_count--;

        }

    }

    *item_count = pool_count;

}


// WZD o109p05
// drake178: AI_EquipRemainder()
/*
second equipment pass for an AI hero: each slot still empty (Items[] < 0) gets the costliest item left in the pool that fits the slot type - no value adjustments
the item goes into the slot and is removed from the pool (item_list[], *item_count)
*/
void AI_Equip_Hero_Empty_Slots(int16_t player_idx, int16_t hero_slot_idx, int16_t item_list[], int16_t * item_count)
{
    int16_t itr_hero_items = 0;
    int16_t highest_value = 0;
    int16_t list_idx = 0;
    int16_t pool_count = 0;
    int16_t item_value = 0;
    int16_t unit_type = 0;
    int16_t itr_items = 0;  // _DI_

    pool_count = *item_count;

    for(itr_hero_items = 0; itr_hero_items < NUM_HERO_ITEMS; itr_hero_items++)
    {

        unit_type = _UNITS[_players[player_idx].Heroes[hero_slot_idx].unit_idx].type;  /* asm: stored, never read in this function */

        if(_players[player_idx].Heroes[hero_slot_idx].Items[itr_hero_items] < 0)
        {

            switch(_players[player_idx].Heroes[hero_slot_idx].Item_Slots[itr_hero_items])
            {

                case ist_Sword_Slot:
                {

                    list_idx = ST_UNDEFINED;

                    highest_value = 0;

                    for(itr_items = 0; itr_items < pool_count; itr_items++)
                    {

                        if(
                            (_ITEMS[item_list[itr_items]].type == it_Sword)
                            ||
                            (_ITEMS[item_list[itr_items]].type == it_Mace)
                            ||
                            (_ITEMS[item_list[itr_items]].type == it_Axe)
                        )
                        {

                            item_value = _ITEMS[item_list[itr_items]].cost;

                            if(item_value > highest_value)
                            {

                                list_idx = itr_items;

                                highest_value = item_value;

                            }

                        }

                    }

                } break;

                case ist_Bow_Slot:
                {

                    list_idx = ST_UNDEFINED;

                    highest_value = 0;

                    for(itr_items = 0; itr_items < pool_count; itr_items++)
                    {

                        if(
                            (_ITEMS[item_list[itr_items]].type == it_Sword)
                            ||
                            (_ITEMS[item_list[itr_items]].type == it_Mace)
                            ||
                            (_ITEMS[item_list[itr_items]].type == it_Axe)
                            ||
                            (_ITEMS[item_list[itr_items]].type == it_Bow)
                        )
                        {

                            item_value = _ITEMS[item_list[itr_items]].cost;

                            if(item_value > highest_value)
                            {

                                list_idx = itr_items;

                                highest_value = item_value;

                            }

                        }

                    }

                } break;

                case ist_SwordStaff_Slot:
                {

                    list_idx = ST_UNDEFINED;

                    highest_value = 0;

                    for(itr_items = 0; itr_items < pool_count; itr_items++)
                    {

                        if(
                            (_ITEMS[item_list[itr_items]].type == it_Sword)
                            ||
                            (_ITEMS[item_list[itr_items]].type == it_Mace)
                            ||
                            (_ITEMS[item_list[itr_items]].type == it_Axe)
                            ||
                            (_ITEMS[item_list[itr_items]].type == it_Staff)
                            ||
                            (_ITEMS[item_list[itr_items]].type == it_Wand)
                        )
                        {

                            item_value = _ITEMS[item_list[itr_items]].cost;

                            if(item_value > highest_value)
                            {

                                list_idx = itr_items;

                                highest_value = item_value;

                            }

                        }

                    }

                } break;

                case ist_Staff_Slot:
                {

                    list_idx = ST_UNDEFINED;

                    highest_value = 0;

                    for(itr_items = 0; itr_items < pool_count; itr_items++)
                    {

                        if(
                            (_ITEMS[item_list[itr_items]].type == it_Staff)
                            ||
                            (_ITEMS[item_list[itr_items]].type == it_Wand)
                        )
                        {

                            item_value = _ITEMS[item_list[itr_items]].cost;

                            if(item_value > highest_value)
                            {

                                list_idx = itr_items;

                                highest_value = item_value;

                            }

                        }

                    }

                } break;

                case ist_Armor_Slot:
                {

                    list_idx = ST_UNDEFINED;

                    highest_value = 0;

                    for(itr_items = 0; itr_items < pool_count; itr_items++)
                    {

                        if(
                            (_ITEMS[item_list[itr_items]].type == it_Shield)
                            ||
                            (_ITEMS[item_list[itr_items]].type == it_Chainmail)
                            ||
                            (_ITEMS[item_list[itr_items]].type == it_Platemail)
                        )
                        {

                            item_value = _ITEMS[item_list[itr_items]].cost;

                            if(item_value > highest_value)
                            {

                                list_idx = itr_items;

                                highest_value = item_value;

                            }

                        }

                    }

                } break;

                case ist_Misc_Slot:
                {

                    list_idx = ST_UNDEFINED;

                    highest_value = 0;

                    for(itr_items = 0; itr_items < pool_count; itr_items++)
                    {

                        if(_ITEMS[item_list[itr_items]].type == it_Misc)
                        {

                            item_value = _ITEMS[item_list[itr_items]].cost;

                            if(item_value > highest_value)
                            {

                                list_idx = itr_items;

                                highest_value = item_value;

                            }

                        }

                    }

                } break;

            }

            if(list_idx > ST_UNDEFINED)
            {

                _players[player_idx].Heroes[hero_slot_idx].Items[itr_hero_items] = item_list[list_idx];

                Clear_Structure(list_idx, (uint8_t *)item_list, sizeof(item_list[0]), pool_count);

                pool_count--;

            }

        }

    }

    *item_count = pool_count;

}
