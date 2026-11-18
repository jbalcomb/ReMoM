/*
    WIZARDS.EXE
        ovr130
*/

#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/MOX_BASE.h"
#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/MOX_SET.h"
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/GENDRAW.h"
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/Timer.h"
#include "../../MoX/src/Video.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"

#include "City_ovr55.h"
#include "DIPLOMAC.h"
#include "Explore.h"
#include "CITYCALC.h"
#include "MainScr_Maps.h"
#include "../../MoX/src/MOM_DEF.h"
#include "AISPELL.h"
#include "MainScr.h"
#include "OverSpel.h"
#include "SBookScr.h"
#include "SCastScr.h"
#include "Spells128.h"
#include "Spells129.h"
#include "SPELLDEF.h"
#include "NEXTTURN.h"
#include "Spellbook.h"
#include "SPLMASTR.h"
#include "Items.h"  /* Player_Process_Item_Pool() */
#include "Combat.h"  /* Combat_Resistance_Check() */
#include "COMBINIT.h"  /* Load_Battle_Unit() */
#include "../../STU/src/STU_GARB.h"  /* Stack_Garbage() */
#include "Spells137.h"
#include "TerrType.h"
#include "Terrain.h"
#include "UNITTYPE.h"
#include "WZD_o059.h"
#include "WZD_o143.h"

#include <stdlib.h>

#include "Spells130.h"



// WZD dseg:6768                                                 BEGIN:  ovr130 - Initialized Data

// WZD dseg:6768
char aBlackWind[] = "Black Wind";
// WZD dseg:6772
char str_empty_string__ovr130[] = "";
// WZD dseg:6773
char aEarthLore[] = "Earth Lore";
// WZD dseg:677E
char soundfx_lbx_file__ovr130[] = "soundfx";
// WZD dseg:6786
char specfx_lbx_file__ovr130[] = "specfx";
// WZD dseg:678D
char aChangeTerrain[] = "Change Terrain";
// WZD dseg:679C
char message_lbx_file__ovr130[] = "message";
// WZD dseg:67A4
char aRaiseVolcano[] = "Raise Volcano";
// WZD dseg:67B2
char aTransmute[] = "Transmute";
// WZD dseg:67BC
char aCorruption_0[] = "Corruption";
// WZD dseg:67C7
char aWordOfRecall[] = "Word of Recall";
// WZD dseg:67D6
char aEnchantRoad[] = "Enchant Road";
// WZD dseg:67E3
char cnst_NatureWard_3[] = "Nature Ward";
// WZD dseg:67EF
char cnst_SorceryWard_3[] = "Sorcery Ward";
// WZD dseg:67FC
char cnst_ChaosWard_3[] = "Chaos Ward";
// WZD dseg:6807
char cnst_LifeWard_3[] = "Life Ward";
// WZD dseg:6811
char cnst_DeathWard_3[] = "Death Ward";
// WZD dseg:681C
char aSpellWard[] = "Spell Ward";
// WZD dseg:6827
char aSelectASpellWa[] = "Select a Spell Ward To Cast";

// WZD dseg:6843 00                                              align 2

// WZD dseg:6843                                                 END:  ovr130 - Initialized Data



/*
    WIZARDS.EXE  ovr130
*/


// WZD o130p01
/*

XREF:
    j_UNIT_SetEquipSlots()
        Hire_Hero()

e.g.,
Hire_Hero()
    |-> Hero_Slot_Types(unit_type_idx, _players[player_idx].Heroes[hero_slot_idx].Item_Slots)

Page 28  (PDF Page 33)
Note that the weapons and armor a hero can be equipped with depend on
the hero’s type; some heroes are mages, others are warriors, etc.
*/
void Hero_Slot_Types(int16_t unit_type_idx, int16_t item_slots[])
{

    if(_unit_type_table[unit_type_idx].hero_type == ht_Battlecaster)
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
    else if(_unit_type_table[unit_type_idx].hero_type == ht_Archer)
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
/*
Death Wish: every non-fantastic unit not controlled by the caster, and not in a city whose counters stop the spell, must resist (Death, +0) or die
a dying hero's items go to its owner's item pool (Player_Process_Item_Pool())
*/
int16_t Cast_Death_Wish(int16_t player_idx)
{
    byte_ptr unit_is_target = NULL;
    int16_t troop_count = 0;
    int16_t spell_reaches_city = 0;
    int16_t owner_idx = 0;
    int16_t item_count = 0;
    int16_t item_list[NUM_HERO_ITEMS] = { 0, 0, 0 };
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t itr = 0;  // _SI_
    int16_t itr_troops = 0;  // _DI_

    WIZ_GlobalSpellAnim(player_idx, spl_Death_Wish);

    unit_is_target = Near_Allocate_First(_units);

    for(itr_troops = 0; itr_troops < _units; itr_troops++)
    {

        unit_is_target[itr_troops] = ST_TRUE;

    }

    for(itr = 0; itr < _cities; itr++)
    {

        spell_reaches_city = Apply_Automatic_Spell_Counters(spl_Death_Wish, itr, player_idx, ST_FALSE);

        if(spell_reaches_city == ST_FALSE)
        {

            Army_At_City(itr, &troop_count, &troops[0]);

            for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
            {

                unit_is_target[troops[itr_troops]] = ST_FALSE;

            }

        }

    }

    for(itr = 0; itr < _units; itr++)
    {

        if(unit_is_target[itr] != ST_TRUE)
        {

            continue;

        }

        Load_Battle_Unit(itr, global_battle_unit);

        if(global_battle_unit->controller_idx == player_idx)
        {

            continue;

        }

        if((global_battle_unit->Abilities & UA_FANTASTIC) != 0)
        {

            continue;

        }

        item_count = 0;

        owner_idx = _UNITS[itr].owner_idx;

        if(Combat_Resistance_Check(*global_battle_unit, 0, sbr_Death) <= 0)
        {

            continue;

        }

        if(_UNITS[itr].Hero_Slot > ST_UNDEFINED)
        {

            for(itr_troops = 0; itr_troops < NUM_HERO_ITEMS; itr_troops++)
            {

                if(_players[owner_idx].Heroes[_UNITS[itr].Hero_Slot].Items[itr_troops] > ST_UNDEFINED)
                {

                    item_list[item_count] = _players[owner_idx].Heroes[_UNITS[itr].Hero_Slot].Items[itr_troops];

                    item_count++;

                }

                /* OGBUG  empties the slot with 0, a real item index, not ST_UNDEFINED: for The Chosen (ut_Chosen), Kill_Unit() takes its dismissed branch, which calls Remove_Item() on every slot > -1 - so item #0 is freed (cost 0) while someone may still carry it */
                _players[owner_idx].Heroes[_UNITS[itr].Hero_Slot].Items[itr_troops] = 0;

            }

        }

        item_pool_in_process = ST_TRUE;

        m_item_wx = _UNITS[itr].wx;

        m_item_wy = _UNITS[itr].wy;

        m_item_wp = _UNITS[itr].wp;

        Kill_Unit(itr, kt_Normal);

        Player_Process_Item_Pool(owner_idx, item_count, &item_list[0]);

    }

    /* OGBUG  no return value: Cast_Spell_Overland() stores AX in Cast_Successful, and its closing Nature's Wrath test (cmp Cast_Successful, e_ST_TRUE) reads it */
    /* HACK */  return Stack_Garbage();

}


// WZD o130p03
/*
Great Unsummoning: every fantastic unit (any owner, the caster's too) without Spell Lock, and not in a city whose counters stop the spell, must resist (Sorcery, -3) or die
a dying hero's items go to its owner's item pool (Player_Process_Item_Pool())
*/
int16_t Cast_Great_Unsummoning(int16_t player_idx)
{
    byte_ptr unit_is_target = NULL;
    int16_t troop_count = 0;
    int16_t spell_reaches_city = 0;
    int16_t item_count = 0;
    int16_t item_list[NUM_HERO_ITEMS] = { 0, 0, 0 };
    int16_t owner_idx = 0;
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t itr = 0;  // _SI_
    int16_t itr_troops = 0;  // _DI_

    WIZ_GlobalSpellAnim(player_idx, spl_Great_Unsummoning);

    unit_is_target = Near_Allocate_First(_units);

    for(itr_troops = 0; itr_troops < _units; itr_troops++)
    {

        unit_is_target[itr_troops] = ST_TRUE;

    }

    for(itr = 0; itr < _cities; itr++)
    {

        spell_reaches_city = Apply_Automatic_Spell_Counters(spl_Great_Unsummoning, itr, player_idx, ST_FALSE);

        if(spell_reaches_city == ST_FALSE)
        {

            Army_At_City(itr, &troop_count, &troops[0]);

            for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
            {

                unit_is_target[troops[itr_troops]] = ST_FALSE;

            }

        }

    }

    for(itr = 0; itr < _units; itr++)
    {

        if(unit_is_target[itr] != ST_TRUE)
        {

            continue;

        }

        Load_Battle_Unit(itr, global_battle_unit);

        if((global_battle_unit->Abilities & UA_FANTASTIC) == 0)
        {

            continue;

        }

        if((_UNITS[itr].enchantments & UE_SPELL_LOCK) != 0)
        {

            continue;

        }

        item_count = 0;

        owner_idx = _UNITS[itr].owner_idx;

        if(Combat_Resistance_Check(*global_battle_unit, -3, sbr_Sorcery) <= 0)
        {

            continue;

        }

        if(_UNITS[itr].Hero_Slot > ST_UNDEFINED)
        {

            for(itr_troops = 0; itr_troops < NUM_HERO_ITEMS; itr_troops++)
            {

                if(_players[owner_idx].Heroes[_UNITS[itr].Hero_Slot].Items[itr_troops] > ST_UNDEFINED)
                {

                    item_list[item_count] = _players[owner_idx].Heroes[_UNITS[itr].Hero_Slot].Items[itr_troops];

                    item_count++;

                }

                _players[owner_idx].Heroes[_UNITS[itr].Hero_Slot].Items[itr_troops] = 0;

            }

            if(item_count > 0)
            {

                item_pool_in_process = ST_TRUE;

                m_item_wx = _UNITS[itr].wx;

                m_item_wy = _UNITS[itr].wy;

                m_item_wp = _UNITS[itr].wp;

                Player_Process_Item_Pool(owner_idx, item_count, &item_list[0]);

            }

        }

        Kill_Unit(itr, kt_Normal);

    }

    /* OGBUG  no return value: Cast_Spell_Overland() stores AX in Cast_Successful, and its closing Nature's Wrath test (cmp Cast_Successful, e_ST_TRUE) reads it */
    /* HACK */  return Stack_Garbage();

}


// WZD o130p04
int16_t Cast_Black_Wind(int16_t player_idx)
{
    int16_t return_value = 0;
    int16_t scsv5 = 0;
    int16_t scsv4 = 0;
    int16_t scsv3 = 0;
    int16_t scsv2 = 0;
    int16_t scsv1 = 0;
    int16_t itr = 0;  // _SI_

    // DNE  Allocate_Reduced_Map();

    // DNE  Mark_Block(_screen_seg);

    if(player_idx == HUMAN_PLAYER_IDX)
    {

        return_value = Spell_Casting_Screen(stt_Enemy_Group, &scsv1, &scsv2, &scsv3, &scsv4, &scsv5, aBlackWind);

        scsv2 = _UNITS[scsv1].wy;
        scsv3 = _UNITS[scsv1].wp;
        scsv1 = _UNITS[scsv1].wx;

    }
    else
    {

        return_value = Get_Map_Square_Target_For_Spell(stt_Enemy_Group, &scsv1, &scsv2, &scsv3, spl_Black_Wind, player_idx);

    }

    if(return_value == ST_TRUE)
    {

        for(itr = 0; itr < _cities; itr++)
        {

            if(
                (_CITIES[itr].wx == scsv1)
                &&
                (_CITIES[itr].wy == scsv2)
                &&
                (_CITIES[itr].wp == scsv3)
            )
            {

                if(
                    (_CITIES[itr].owner_idx == HUMAN_PLAYER_IDX)
                    ||
                    (player_idx == HUMAN_PLAYER_IDX)
                )
                {

                    return_value = Apply_Automatic_Spell_Counters(spl_Black_Wind, itr, player_idx, ST_TRUE);

                }
                else
                {

                    return_value = Apply_Automatic_Spell_Counters(spl_Black_Wind, itr, player_idx, ST_FALSE);

                }

            }

        }

    }

    if(return_value == ST_TRUE)
    {

        /* OGBUG  AI cast: scsv1 is the target wx (Get_Map_Square_Target_For_Spell()), not a unit_idx, so the owner test reads _UNITS[wx] */
        if(
            (player_idx == HUMAN_PLAYER_IDX)
            ||
            (_UNITS[scsv1].owner_idx == HUMAN_PLAYER_IDX)  // BUGBUGscsv1 is wx here, not unit_idx ...c&p error
            ||
            (Check_Square_Scouted(scsv1, scsv2, scsv3) != ST_FALSE)
        )
        {
            
            g_ai_recompute_needed = ST_TRUE;

            Allocate_Reduced_Map();

            Mark_Block(_screen_seg);

            Spell_Animation_Load_Sound_Effect__WIP(spl_Black_Wind);

            Spell_Animation_Load_Graphics(spl_Black_Wind);

            Spell_Animation_Screen__WIP(scsv1, scsv2, scsv3);

            Full_Draw_Main_Screen();

            Release_Block(_screen_seg);

        }

        Apply_Black_Wind(scsv1, scsv2, scsv3, spl_Black_Wind);

        // BUGBUG  scsv1 is wx here, not unit_idx ...c&p error
        // BUGBUG  should pass spl_Black_Wind, not scsv1 (which is wx) ...c&p error
        /* OGBUG  both paths: scsv1 is the wx by now, so the relation hit goes to the owner of unit #wx */
        Change_Relations(-3, player_idx, _UNITS[scsv1].owner_idx, 8, 0, scsv1);

    }

    return return_value;
    
}


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
    /* OGBUG  10 x Random(20) = 10..200 (the wiki says 50..150) */
    for(itr = 0; itr < 10; itr++)
    {
        // 10 * Random(20) = {10, ..., 200}
        return_value += Random(20);
    }
    if(_players[player_idx].mana_reserve < return_value)
    {
        return_value = _players[player_idx].mana_reserve;
    }
    _players[player_idx].mana_reserve -= return_value;
    return return_value;
}


// WZD o130p06
int16_t Cast_Earth_Lore(int16_t player_idx)
{
    SAMB_ptr soundfx_seg = 0;
    /* HACK */  uint32_t soundfx_seg_size = 0;  // DNE in Dasm
    int16_t itr = 0;
    int16_t curr_wx = 0;
    int16_t return_value = 0;
    int16_t scsv5 = 0;
    int16_t scsv4 = 0;
    int16_t scsv3 = 0;
    int16_t scsv2 = 0;
    int16_t scsv1 = 0;
    int16_t itr_wx = 0;  // _SI_
    int16_t itr_wy = 0;  // _DI_

    return_value = ST_FALSE;

    if(player_idx == HUMAN_PLAYER_IDX)
    {

        Allocate_Reduced_Map();

        Mark_Block(_screen_seg);

        return_value = Spell_Casting_Screen(stt_Map_Square, &scsv1, &scsv2, &scsv3, &scsv4, &scsv5, aCorruption_0);

    }

    // SOUNDFX.LBX, 106  "SILENT V"  "sILENCE"
    soundfx_seg = LBX_Reload_Next(soundfx_lbx_file__ovr130, 106, _screen_seg);
    soundfx_seg_size = lbxload_entry_length;

    if(return_value == ST_TRUE)
    {

        if(player_idx == HUMAN_PLAYER_IDX)
        {

            Update_Remap_Color_Range(1, 4);

            // SPECFX.LBX, 045  "FULGRN2"   "Earth lore"
            Open_File_Animation(specfx_lbx_file__ovr130, 45);

            Spell_Animation_Load_Sound_Effect__WIP(spl_Earth_Lore);

            if(SND_SpellCast != (SAMB_ptr)ST_UNDEFINED)
            {
                Play_Sound(SND_SpellCast, SND_SpellCast_size);
            }

            for(itr = 0; itr < 22; itr++)
            {

                Set_Page_Off();

                Fill(0, 20, 240, 199, ST_TRANSPARENT);

                Reset_Map_Draw();

                Main_Screen_Draw();

                Draw_File_Animation();

                PageFlip_FX();

                Release_Time(2);

            }

            for(itr_wx = _map_x; (_map_x + (MAP_WIDTH - 1)) >= itr_wx; itr_wx++)
            {

                if(itr_wx >= WORLD_WIDTH)
                {

                    curr_wx = (itr_wx - WORLD_WIDTH);

                }
                else
                {

                    curr_wx = itr_wx;

                }

                for(itr_wy = _map_y; (_map_y + (MAP_HEIGHT - 1)) >= itr_wy; itr_wy++)
                {

                    if(itr_wy < WORLD_HEIGHT)
                    {

                        Set_Square_Explored_Flags(curr_wx, itr_wy, scsv3);

                        // DONT  j_empty_fxn_o142p03(curr_wx, itr_wy, scsv3);

                    }

                }

            }

        }

    }

    Play_Sound(soundfx_seg, soundfx_seg_size);

    Dissolve_Main_Screen();

    Release_Block(_screen_seg);

    return return_value;

}


// WZD o130p07
// drake178: sub_AFB7F()
/*
¿ return value ?
    Dasm thinks it should get one, but doesn't show returning one.

*/
// int16_t Cast_Spell_Binding(int16_t player_idx)
void Cast_Spell_Binding(int16_t player_idx)
{
    int16_t var_A = 0;
    uint8_t * ptr_enchantments = 0;
    int16_t target_player_idx = 0;
    int16_t spell_idx = 0;
    int16_t threshold = 0;  // _SI_

    if(player_idx == HUMAN_PLAYER_IDX)
    {

        Global_Enchantment_Screen(spl_Spell_Binding, player_idx);

    }
    else
    {

        var_A = AITP_Disjunction(&target_player_idx, &spell_idx, spl_Spell_Binding, player_idx);

        if(var_A == 1)
        {

            /* OGBUG  AITP_Disjunction() returns the Globals[] slot (0 .. 24) in spell_idx, not a spell index: the dispel cost and realm are read from spell_data_table[slot], and the Globals[] byte cleared on the target and set on the caster is at spell_data_table[slot].oe_idx, a field of an unrelated spell, which can land in another wizard's record or past _players, while the enchantment itself stays */
            threshold = (_players[player_idx].casting_cost_original + Calculate_Dispel_Difficulty(spell_data_table[spell_idx].casting_cost, target_player_idx, spell_data_table[spell_idx].magic_realm));

            threshold = ((_players[player_idx].casting_cost_original * 250) / threshold);

            if(Random(250) <= threshold)
            {

                ptr_enchantments = &_players[target_player_idx].Globals[0];

                // ptr_enchantments[spell_data_table[spell_idx].oe_idx] = 0;
                /* HACK */  if((((int32_t)((uint8_t *)ptr_enchantments - (uint8_t *)&_players[0]) + spell_data_table[spell_idx].oe_idx) >= 0) && (((int32_t)((uint8_t *)ptr_enchantments - (uint8_t *)&_players[0]) + spell_data_table[spell_idx].oe_idx) < (int32_t)sizeof(_players)))  /* CLAUDE */  /* OG: the byte at DS:(Globals + oe_idx) is written wherever it is; ReMoM writes it only while it is inside _players */
                {
                    ptr_enchantments[spell_data_table[spell_idx].oe_idx] = 0;
                }

                ptr_enchantments = &_players[player_idx].Globals[0];

                // ptr_enchantments[spell_data_table[spell_idx].oe_idx] = (player_idx + 1);  // player_num
                /* HACK */  if((((int32_t)((uint8_t *)ptr_enchantments - (uint8_t *)&_players[0]) + spell_data_table[spell_idx].oe_idx) >= 0) && (((int32_t)((uint8_t *)ptr_enchantments - (uint8_t *)&_players[0]) + spell_data_table[spell_idx].oe_idx) < (int32_t)sizeof(_players)))  /* CLAUDE */  /* OG: as above */
                {
                    ptr_enchantments[spell_data_table[spell_idx].oe_idx] = (player_idx + 1);  // player_num
                }

            }

        }

    }

}


// WZD o130p08
int16_t Cast_Change_Terrain(int16_t player_idx)
{
    int16_t saved_map_plane = 0;
    int16_t saved_map_y = 0;
    int16_t saved_map_x = 0;
    int16_t frame_count = 0;
    int16_t terrain_type = 0;
    int16_t status = 0;
    int16_t scsv5 = 0;
    int16_t scsv4 = 0;
    int16_t scsv3 = 0;
    int16_t scsv2 = 0;
    int16_t scsv1 = 0;
    int16_t itr = 0;  // _SI_
    int16_t return_value = 0;  // _DI_

    saved_map_x = _map_x;
    saved_map_y = _map_y;
    saved_map_plane = _map_plane;

    return_value = ST_TRUE;

    if(player_idx != HUMAN_PLAYER_IDX)
    {

        return_value = Get_Map_Square_Target_For_Spell(stt_Map_Square, &scsv1, &scsv2, &scsv3, spl_Change_Terrain, player_idx);

    }
    else
    {

        status = ST_FALSE;

        while((status == ST_FALSE) && (return_value == ST_TRUE))
        {

            return_value = Spell_Casting_Screen(stt_Map_Square, &scsv1, &scsv2, &scsv3, &scsv4, &scsv5, aChangeTerrain);

            if(return_value == ST_TRUE)
            {

                if(
                    (Square_Is_Grasslands(scsv1, scsv2, scsv3) != ST_FALSE)
                    ||
                    (Square_Is_Forest(scsv1, scsv2, scsv3) != ST_FALSE)
                    ||
                    (Square_Is_Desert(scsv1, scsv2, scsv3) != ST_FALSE)
                    ||
                    (Square_Is_Volcano(scsv1, scsv2, scsv3) != ST_FALSE)
                    ||
                    (Square_Is_Hills(scsv1, scsv2, scsv3) != ST_FALSE)
                    ||
                    (Square_Is_Mountain(scsv1, scsv2, scsv3) != ST_FALSE)
                    ||
                    (Square_Is_Swamp(scsv1, scsv2, scsv3) != ST_FALSE)
                )
                {

                    terrain_type = TERRAIN_TYPE(scsv1, scsv2, scsv3);

                    if(
                        (terrain_type != tt_ChaosNode)
                        &&
                        (terrain_type != tt_SorceryNode)
                        &&
                        (terrain_type != tt_NatureNode)
                    )
                    {

                        status = ST_TRUE;

                    }
                    else
                    {

                        LBX_Load_Data_Static(message_lbx_file__ovr130, 0, (SAMB_ptr)&GUI_NearMsgString[0], 34, 1, 150);

                        Warn0(GUI_NearMsgString);

                    }


                }
                else
                {

                    LBX_Load_Data_Static(message_lbx_file__ovr130, 0, (SAMB_ptr)&GUI_NearMsgString[0], 35, 1, 150);

                    Warn0(GUI_NearMsgString);

                }

            }

        }

    }

    if(return_value == ST_TRUE)
    {

        if(
            (
                (player_idx == HUMAN_PLAYER_IDX)
                ||
                (SQUARE_EXPLORED(scsv1, scsv2, scsv3) != ST_FALSE)
            )
            &&
            (magic_set.spell_animations == ST_TRUE)
        )
        {

            g_ai_recompute_needed = ST_TRUE;

            Allocate_Reduced_Map();

            Mark_Block(_screen_seg);

            Spell_Animation_Load_Sound_Effect__WIP(spl_Change_Terrain);

            Spell_Animation_Load_Graphics(spl_Change_Terrain);

            // full draw twice - same off & on
            for(itr = 0; itr < 2; itr++)
            {
                Set_Page_Off();
                Main_Screen_Draw();
                PageFlip_FX();
            }

            if(SND_SpellCast != (SAMB_ptr)ST_UNDEFINED)
            {
                Play_Sound(SND_SpellCast, SND_SpellCast_size);
            }

            frame_count = FLIC_Get_FrameCount(spell_animation_seg);

            SETMAX(frame_count, 12);

            Bring_Square_Into_View(&_map_x, &_map_y, scsv1, scsv2, scsv3);

            _map_plane = scsv3;

            Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

            scsv4 = scsv1;
            scsv5 = scsv2;

            World_To_Screen(_map_x, _map_y, &scsv4, &scsv5);

            for(itr = 0; itr < frame_count; itr++)
            {
                
                Mark_Time();

                if(itr == 8)
                {

                    if(Square_Is_Grasslands(scsv1, scsv2, scsv3) != ST_FALSE)
                    {

                        Set_Terrain_Type_Forest(scsv1, scsv2, scsv3);  // BUGBUG  update move cost maps

                    }
                    else if(
                        (Square_Is_Swamp(scsv1, scsv2, scsv3) != ST_FALSE)
                        ||
                        (Square_Is_Forest(scsv1, scsv2, scsv3) != ST_FALSE)
                        ||
                        (Square_Is_Desert(scsv1, scsv2, scsv3) != ST_FALSE)
                        ||
                        (Square_Is_Hills(scsv1, scsv2, scsv3) != ST_FALSE)
                    )
                    {

                        Set_Terrain_Type_Grasslands(scsv1, scsv2, scsv3);  // BUGBUG  update move cost maps

                    }

                    if(Square_Is_Volcano(scsv1, scsv2, scsv3) != ST_FALSE)
                    {

                        Set_Terrain_Type_Mountain(scsv1, scsv2, scsv3);  // BUGBUG  update move cost maps

                        Volcano_Counts();

                    }
                    else if(Square_Is_Mountain(scsv1, scsv2, scsv3) != ST_FALSE)
                    {

                        Set_Terrain_Type_Hills(scsv1, scsv2, scsv3);  // BUGBUG  update move cost maps

                    }

                }



                Set_Page_Off();

                Reset_Map_Draw();

                Main_Screen_Draw();

                Set_Animation_Frame(spell_animation_seg, itr);

                Clipped_Draw((scsv4 - SQUARE_WIDTH), (scsv5 - SQUARE_HEIGHT), spell_animation_seg);

                PageFlip_FX();

                Release_Time(2);

            }

            Full_Draw_Main_Screen();

            Release_Block(_screen_seg);

        }
        else
        {

            if(Square_Is_Grasslands(scsv1, scsv2, scsv3) != ST_FALSE)
            {

                Set_Terrain_Type_Forest(scsv1, scsv2, scsv3);  // BUGBUG  update move cost maps

            }
            else if(
                (Square_Is_Swamp(scsv1, scsv2, scsv3) != ST_FALSE)
                ||
                (Square_Is_Forest(scsv1, scsv2, scsv3) != ST_FALSE)
                ||
                (Square_Is_Desert(scsv1, scsv2, scsv3) != ST_FALSE)
                ||
                (Square_Is_Hills(scsv1, scsv2, scsv3) != ST_FALSE)
            )
            {

                Set_Terrain_Type_Grasslands(scsv1, scsv2, scsv3);  // BUGBUG  update move cost maps

            }

            if(Square_Is_Volcano(scsv1, scsv2, scsv3) != ST_FALSE)
            {

                Set_Terrain_Type_Mountain(scsv1, scsv2, scsv3);  // BUGBUG  update move cost maps

                Volcano_Counts();

            }
            else if(Square_Is_Mountain(scsv1, scsv2, scsv3) != ST_FALSE)
            {

                Set_Terrain_Type_Hills(scsv1, scsv2, scsv3);  // BUGBUG  update move cost maps

            }

        }

    }

    /*
        AFTER:
            success OR failure
    */

    if(
        (_map_x != saved_map_x)
        ||
        (_map_y != saved_map_y)
        ||
        (_map_plane != saved_map_plane)
    )
    {

        _map_x = saved_map_x;
        _map_y = saved_map_y;
        _map_plane = saved_map_plane;

        Bring_Square_Into_View(&_map_x, &_map_y, _map_x, _map_y, _map_plane);

        Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

    }

    return return_value;

}


// WZD o130p09
int16_t Cast_Raise_Volcano(int16_t player_idx)
{
    int16_t bldg_list[NUM_BUILDINGS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t ctr = 0;
    int16_t list[2] = { 0, 0 };
    int16_t target_is_human = 0;
    int16_t bldg_idx = 0;
    int16_t status = 0;
    int16_t return_value = 0;  // _DI_
    int16_t scsv5 = 0;
    int16_t scsv4 = 0;
    int16_t scsv3 = 0;
    int16_t scsv2 = 0;
    int16_t scsv1 = 0;
    int16_t terrain_type = 0;  // _SI_
    int16_t city_idx = 0;  // _SI_

    return_value = ST_TRUE;

    target_is_human = ST_FALSE;

    if(player_idx != HUMAN_PLAYER_IDX)
    {

        return_value = Get_Map_Square_Target_For_Spell(stt_Map_Square, &scsv1, &scsv2, &scsv3, spl_Raise_Volcano, player_idx);

    }
    else
    {

        status = ST_FALSE;

        while((status == ST_FALSE) && (return_value == ST_TRUE))
        {

            status = ST_TRUE;

            return_value = Spell_Casting_Screen(stt_Map_Square, &scsv1, &scsv2, &scsv3, &scsv4, &scsv5, aRaiseVolcano);

            if(return_value == ST_TRUE)
            {

                if(Square_Is_Sailable(scsv1, scsv2, scsv3) != ST_FALSE)
                {

                        status = ST_FALSE;

                        LBX_Load_Data_Static(message_lbx_file__ovr130, 0, (SAMB_ptr)&GUI_NearMsgString[0], 36, 1, 150);

                        Warn0(GUI_NearMsgString);

                }
                if(Square_Is_Mountain(scsv1, scsv2, scsv3) != ST_FALSE)
                {

                        status = ST_FALSE;

                        LBX_Load_Data_Static(message_lbx_file__ovr130, 0, (SAMB_ptr)&GUI_NearMsgString[0], 37, 1, 150);

                        Warn0(GUI_NearMsgString);

                }
                if(Square_Is_Hills(scsv1, scsv2, scsv3) != ST_FALSE)
                {

                        status = ST_FALSE;

                        LBX_Load_Data_Static(message_lbx_file__ovr130, 0, (SAMB_ptr)&GUI_NearMsgString[0], 38, 1, 150);

                        Warn0(GUI_NearMsgString);

                }
                if(Square_Is_River(scsv1, scsv2, scsv3) != ST_FALSE)
                {

                        status = ST_FALSE;

                        LBX_Load_Data_Static(message_lbx_file__ovr130, 0, (SAMB_ptr)&GUI_NearMsgString[0], 86, 1, 150);

                        Warn0(GUI_NearMsgString);

                }
                terrain_type = TERRAIN_TYPE(scsv1, scsv2, scsv3);

                if(
                    (terrain_type == tt_ChaosNode)
                    ||
                    (terrain_type == tt_SorceryNode)
                    ||
                    (terrain_type == tt_NatureNode)
                )
                {

                    status = ST_FALSE;

                    LBX_Load_Data_Static(message_lbx_file__ovr130, 0, (SAMB_ptr)&GUI_NearMsgString[0], 39, 1, 150);

                    Warn0(GUI_NearMsgString);

                }

            }

        }

    }

    if(return_value == ST_TRUE)
    {

        ctr = 0;

        /* OGBUG  the loop stops at the first nearby human city, so a later city on the target square is never countered or damaged */
        for(city_idx = 0; ((city_idx < _cities) && (target_is_human == ST_FALSE)); city_idx++)
        {

            if(
                (
                    (abs((_CITIES[city_idx].wx - scsv1)) <= 2)
                    ||
                    (abs((_CITIES[city_idx].wx - scsv1)) >= 58)
                )
                &&
                (abs((_CITIES[city_idx].wy - scsv2)) <= 2)
                &&
                (_CITIES[city_idx].wp == scsv3)
            )
            {

                /* OGBUG  list[] has 2 slots: a 3rd nearby city writes list[2], which in the OG frame is target_is_human, so the loop also stops after that city */
                // list[ctr] = city_idx;
                /* HACK */  if(ctr < 2) { list[ctr] = city_idx; } else { target_is_human = city_idx; }

                if(_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX)
                {

                    target_is_human = ST_TRUE;

                }

                ctr++;

            }

            if(
                (_CITIES[city_idx].wx == scsv1)
                &&
                (_CITIES[city_idx].wy == scsv2)
                &&
                (_CITIES[city_idx].wp == scsv3)
            )
            {

                return_value = Apply_Automatic_Spell_Counters(spl_Raise_Volcano, city_idx, player_idx, ((_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX) || (player_idx == HUMAN_PLAYER_IDX)));

                if(return_value == ST_TRUE)
                {

                    Apply_Damage_To_City(city_idx, 0, 15, &bldg_list[0]);

                    if(_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX)
                    {

                        for(bldg_idx = 0; bldg_idx < NUM_BUILDINGS; bldg_idx++)
                        {

                            if(
                                (bldg_list[bldg_idx] > bs_Replaced)
                                &&
                                (MSG_BldLost_Count < 20)
                            )
                            {

                                /* OGBUG  stores the target square's wx as the city index, so the buildings-lost message names the wrong city */
                                MSG_BldLost_Array[MSG_BldLost_Count].city_idx = (int8_t)scsv1;

                                MSG_BldLost_Array[MSG_BldLost_Count].bldg_type_idx = bldg_list[bldg_idx];

                                MSG_BldLost_Count++;

                            }

                        }

                    }

                }

            }

        }

    }

    if(return_value == ST_TRUE)
    {

        if(
            (player_idx == HUMAN_PLAYER_IDX)
            ||
            (target_is_human == ST_TRUE)
            ||
            (SQUARE_EXPLORED(scsv1, scsv2, scsv3) != ST_FALSE)
        )
        {

            g_ai_recompute_needed = ST_TRUE;

            Allocate_Reduced_Map();

            Spell_Animation_Load_Sound_Effect__WIP(spl_Raise_Volcano);

            Spell_Animation_Load_Graphics(spl_Raise_Volcano);

            Spell_Animation_Screen__WIP(scsv1, scsv2, scsv3);

        }

        Set_Terrain_Type_Volcano_With_Owner(scsv1, scsv2, scsv3, player_idx);

        Full_Draw_Main_Screen();

        _players[player_idx].volcanoes++;

        Volcano_Counts();

        if(ctr > 0)
        {

            Change_Relations_For_Bad_City_Spell(player_idx, spl_Raise_Volcano, list[0]);

            if(ctr > 1)
            {

                Change_Relations_For_Bad_City_Spell(player_idx, spl_Raise_Volcano, list[1]);

            }

        }

    }

    Full_Draw_Main_Screen();

    Release_Block(_screen_seg);

    return return_value;

}


// WZD o130p10
/*

coal to gems
iron to gold
silver to mithril

*/
int16_t Cast_Transmute(int16_t player_idx)
{
    int16_t saved_map_plane = 0;
    int16_t saved_map_y = 0;
    int16_t saved_map_x = 0;
    int16_t frame_count = 0;
    int16_t terrain_type = 0;
    int16_t status = 0;
    int16_t scsv5 = 0;
    int16_t scsv4 = 0;
    int16_t scsv3 = 0;
    int16_t scsv2 = 0;
    int16_t scsv1 = 0;
    int16_t did_it = 0;  // _SI_
    int16_t return_value = 0;  // _DI_

    did_it = ST_FALSE;

    Allocate_Reduced_Map();

    Mark_Block(_screen_seg);

    return_value = ST_TRUE;

    if(player_idx != HUMAN_PLAYER_IDX)
    {

        return_value = Get_Map_Square_Target_For_Spell(stt_Map_Square, &scsv1, &scsv2, &scsv3, spl_Transmute, player_idx);

    }
    else
    {

        status = ST_FALSE;

        while((status == ST_FALSE) && (return_value == ST_TRUE))
        {

            return_value = Spell_Casting_Screen(stt_Map_Square, &scsv1, &scsv2, &scsv3, &scsv4, &scsv5, aTransmute);

            if(return_value == ST_TRUE)
            {

                if(
                    (GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) != TS_COAL)
                    &&
                    (GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) != TS_IRON)
                    &&
                    (GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) != TS_SILVER)
                    &&
                    (GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) != TS_GEMS)
                    &&
                    (GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) != TS_GOLD)
                    &&
                    (GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) != TS_MITHRIL)
                )
                {

                    LBX_Load_Data_Static(message_lbx_file__ovr130, 0, (SAMB_ptr)&GUI_NearMsgString[0], 40, 1, 150);

                    Warn0(GUI_NearMsgString);

                }
                else
                {

                    status = ST_TRUE;

                }

            }

        }

    }

    if(return_value == ST_TRUE)
    {

        /* DEDU  unknown which grouping OG intends.  C precedence (&& tighter
           than ||) parses this as:
             human OR ((enemy_spells AND detect_magic AND explored) AND animations)
           — meaning the human-player path bypasses spell_animations entirely.
           Alternative grouping would be:
             (human OR (enemy_spells AND detect_magic AND explored)) AND animations
           — making spell_animations a master toggle for everyone.  Explicit
           parens below preserve the C-precedence interpretation that the
           current binary uses; investigate against OG and regroup if needed. */
        if(
            (player_idx == HUMAN_PLAYER_IDX)
            ||
            (
                (
                    (magic_set.enemy_spells == ST_TRUE)
                    &&
                    (_players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] > ST_FALSE)
                    &&
                    (SQUARE_EXPLORED(scsv1, scsv2, scsv3) != ST_FALSE)
                )
                &&
                (magic_set.spell_animations == ST_TRUE)
            )
        )
        {

            Spell_Animation_Load_Sound_Effect__WIP(spl_Change_Terrain);

            Spell_Animation_Load_Graphics(spl_Change_Terrain);

            Spell_Animation_Screen__WIP(scsv1, scsv2, scsv3);

        }

        if(GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) == TS_MITHRIL)
        {

            SET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3, TS_NONE);

            SET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3, GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) | TS_SILVER);

            did_it = ST_TRUE;

        }

        if(GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) == TS_GOLD)
        {

            SET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3, TS_NONE);

            SET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3, GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) | TS_IRON);

            did_it = ST_TRUE;

        }

        if(GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) == TS_GEMS)
        {

            SET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3, TS_NONE);

            SET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3, GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) | TS_COAL);

            did_it = ST_TRUE;

        }

        if(
            (GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) == TS_COAL)
            &&
            (did_it == ST_FALSE)
        )
        {

            SET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3, TS_NONE);

            SET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3, GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) | TS_GEMS);

            did_it = ST_TRUE;

        }

        if(
            (GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) == TS_IRON)
            &&
            (did_it == ST_FALSE)
        )
        {

            SET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3, TS_NONE);

            SET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3, GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) | TS_GOLD);

            did_it = ST_TRUE;

        }
        if(
            (GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) == TS_SILVER)
            &&
            (did_it == ST_FALSE)
        )
        {

            SET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3, TS_NONE);

            SET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3, GET_TERRAIN_SPECIAL(scsv1, scsv2, scsv3) | TS_MITHRIL);

            did_it = ST_TRUE;

        }

    }

    Dissolve_Main_Screen();

    Release_Block(_screen_seg);

    return return_value;

}


// WZD o130p11
int16_t Cast_Corruption(int16_t player_idx)
{
    int16_t ctr = 0;
    int16_t list[2] = { 0, 0 };
    int16_t saved_map_plane = 0;
    int16_t saved_map_y = 0;
    int16_t saved_map_x = 0;
    SAMB_ptr soundfx_seg = 0;
    uint32_t soundfx_seg_size = 0;
    int16_t target_is_human = 0;
    int16_t frame_count = 0;
    int16_t terrain_type = 0;
    int16_t status = 0;
    int16_t return_value = 0;
    int16_t scsv5 = 0;
    int16_t scsv4 = 0;
    int16_t scsv3 = 0;
    int16_t scsv2 = 0;
    int16_t scsv1 = 0;
    int16_t itr = 0;  // _SI_

    target_is_human = ST_FALSE;

    Allocate_Reduced_Map();

    Mark_Block(_screen_seg);

    saved_map_plane = _map_plane;
    saved_map_x = _map_x;
    saved_map_y = _map_y;

    if(player_idx != HUMAN_PLAYER_IDX)
    {

        return_value = Get_Map_Square_Target_For_Spell(stt_Map_Square, &scsv1, &scsv2, &scsv3, spl_Corruption, player_idx);

    }
    else
    {

        // SOUNDFX.LBX, 106  "SILENT V"  "sILENCE"
        soundfx_seg = LBX_Reload_Next(soundfx_lbx_file__ovr130, 106, _screen_seg);
        soundfx_seg_size = lbxload_entry_length;

        status = ST_FALSE;

        return_value = ST_TRUE;

        while((status == ST_FALSE) && (return_value == ST_TRUE))
        {

            return_value = Spell_Casting_Screen(stt_Map_Square, &scsv1, &scsv2, &scsv3, &scsv4, &scsv5, aCorruption_0);

            if(return_value == ST_TRUE)
            {

                terrain_type = TERRAIN_TYPE(scsv1, scsv2, scsv3);

                if(
                    (terrain_type == tt_ChaosNode)
                    ||
                    (terrain_type == tt_SorceryNode)
                    ||
                    (terrain_type == tt_NatureNode)
                )
                {

                    LBX_Load_Data_Static(message_lbx_file__ovr130, 0, (SAMB_ptr)&GUI_NearMsgString[0], 41, 1, 150);

                    Warn0(GUI_NearMsgString);

                }
                else if((MAP_SQUARE_FLAG(scsv1, scsv2, scsv3) & MSF_CORRUPTION) != 0)
                {

                    LBX_Load_Data_Static(message_lbx_file__ovr130, 0, (SAMB_ptr)&GUI_NearMsgString[0], 42, 1, 150);

                    Warn0(GUI_NearMsgString);

                }
                else if(Square_Is_Sailable(scsv1, scsv2, scsv3) != ST_FALSE)
                {

                    LBX_Load_Data_Static(message_lbx_file__ovr130, 0, (SAMB_ptr)&GUI_NearMsgString[0], 43, 1, 150);

                    Warn0(GUI_NearMsgString);

                }
                else
                {

                    status = ST_TRUE;

                }

            }

        }

    }

    ctr = 0;

    for(itr = 0; itr < _cities; itr++)
    {

        if(
            (
                (abs((_CITIES[itr].wx - scsv1)) <= 2)
                ||
                (abs((_CITIES[itr].wx - scsv1)) >= 58)
            )
            &&
            (abs((_CITIES[itr].wy - scsv2)) <= 2)
            &&
            (_CITIES[itr].wp == scsv3)
        )
        {
            /* OGBUG  list[] has 2 slots: a 3rd, 4th, 5th nearby city writes list[2], [3], [4], which in the OG frame are saved_map_y, saved_map_x, saved_map_plane, so the map view is restored to a city index */
            // list[ctr] = itr;
            /* HACK */  if(ctr < 2) { list[ctr] = itr; }

            if(_CITIES[itr].owner_idx == HUMAN_PLAYER_IDX)
            {

                target_is_human = ST_TRUE;

            }

            ctr++;

        }

        if(
            (_CITIES[itr].wx == scsv1)
            &&
            (_CITIES[itr].wy == scsv2)
            &&
            (_CITIES[itr].wp == scsv3)
        )
        {

            return_value = Apply_Automatic_Spell_Counters(spl_Corruption, itr, player_idx, ((_CITIES[itr].owner_idx == HUMAN_PLAYER_IDX) || (player_idx == HUMAN_PLAYER_IDX)));

        }

    }

    if(return_value == ST_TRUE)
    {

        if(
            (player_idx == HUMAN_PLAYER_IDX)
            ||
            (target_is_human == ST_TRUE)
            ||
            (
                (magic_set.enemy_spells == ST_TRUE)
                &&
                (_players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] > ST_FALSE)
                &&
                (SQUARE_EXPLORED(scsv1, scsv2, scsv3) != ST_FALSE)
            )
        )
        {

            Spell_Animation_Load_Sound_Effect__WIP(spl_Corruption);

            Spell_Animation_Load_Graphics(spl_Corruption);

            // full draw twice - same off & on
            for(itr = 0; itr < 2; itr++)
            {
                Set_Page_Off();
                Main_Screen_Draw();
                PageFlip_FX();
            }

            if(SND_SpellCast != (SAMB_ptr)ST_UNDEFINED)
            {
                Play_Sound(SND_SpellCast, SND_SpellCast_size);
            }

            frame_count = FLIC_Get_FrameCount(spell_animation_seg);

            SETMIN(frame_count, 12);

            Bring_Square_Into_View(&_map_x, &_map_y, scsv1, scsv2, scsv3);

            _map_plane = scsv3;

            // DNE  Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

            scsv4 = scsv1;
            scsv5 = scsv2;

            World_To_Screen(_map_x, _map_y, &scsv4, &scsv5);

            for(itr = 0; itr < frame_count; itr++)
            {
                
                Mark_Time();

                if(itr == 11)
                {

                    SET_MAP_SQUARE_FLAG(scsv1, scsv2, scsv3, MSF_CORRUPTION);

                }

                Set_Page_Off();

                Reset_Map_Draw();

                Main_Screen_Draw();

                Set_Animation_Frame(spell_animation_seg, itr);

                Clipped_Draw((scsv4 - SQUARE_WIDTH), (scsv5 - SQUARE_HEIGHT), spell_animation_seg);

                PageFlip_FX();

                Release_Time(2);

            }

            // DNE  Full_Draw_Main_Screen();

            // DNE  Release_Block(_screen_seg);

        }
        else
        {

            SET_MAP_SQUARE_FLAG(scsv1, scsv2, scsv3, MSF_CORRUPTION);

        }

        if(ctr > 0)
        {

            Change_Relations_For_Bad_City_Spell(player_idx, spl_Corruption, list[0]);

            if(ctr > 1)
            {

                Change_Relations_For_Bad_City_Spell(player_idx, spl_Corruption, list[1]);

            }

        }

    }

    /*
        AFTER:
            success OR failure
    */

    Full_Draw_Main_Screen();

    _map_x = saved_map_x;
    _map_y = saved_map_y;
    _map_plane = saved_map_plane;

    Release_Block(_screen_seg);

    return return_value;

}


// WZD o130p12
int16_t Cast_Word_Of_Recall(int16_t player_idx)
{
    int16_t scsv5 = 0;
    int16_t scsv4 = 0;
    int16_t scsv3 = 0;
    int16_t scsv2 = 0;
    int16_t scsv1 = 0;
    int16_t return_value = 0;  // _DI_

    Allocate_Reduced_Map();

    Mark_Block(_screen_seg);

    if(player_idx == HUMAN_PLAYER_IDX)
    {

        return_value = Spell_Casting_Screen(stt_Friendly_Unit, &scsv1, &scsv2, &scsv3, &scsv4, &scsv5, aWordOfRecall);

    }
    else
    {

        /* OGBUG  spl_Word_Of_Recall is not in AITP_Unit_Enchantment()'s spell table, so it calls Cast_Spell_Target_Error() and returns ST_FALSE: an AI Word of Recall never finds a target (the dedicated picker, AITP_Word_Of_Recall(), has no caller) */
        return_value = AITP_Unit_Enchantment(stt_Friendly_Unit, &scsv1, spl_Word_Of_Recall, player_idx);

    }

    if(return_value == ST_TRUE)
    {

        if(
            (player_idx == HUMAN_PLAYER_IDX)
            ||
            (Check_Square_Scouted(_UNITS[scsv1].wx, _UNITS[scsv1].wy, _UNITS[scsv1].wp) != ST_FALSE)
        )
        {

            Spell_Animation_Load_Sound_Effect__WIP(spl_Word_Of_Recall);

            Spell_Animation_Load_Graphics(spl_Word_Of_Recall);

            Spell_Animation_Screen__WIP(_UNITS[scsv1].wx, _UNITS[scsv1].wy, _UNITS[scsv1].wp);

        }

        _UNITS[scsv1].wx = (int8_t)_players[player_idx].summon_wx;
        _UNITS[scsv1].wy = (int8_t)_players[player_idx].summon_wy;
        _UNITS[scsv1].wp = (int8_t)_players[player_idx].summon_wp;

        Evict_Weakest_Unit(scsv1);

    }

    Full_Draw_Main_Screen();

    Release_Block(_screen_seg);

    return return_value;

}


// WZD o130p13
int16_t Cast_Enchant_Road(int16_t player_idx)
{
    int16_t saved_map_plane = 0;
    int16_t saved_map_y = 0;
    int16_t saved_map_x = 0;
    SAMB_ptr soundfx_seg = 0;
    /* HACK */  uint32_t soundfx_seg_size = 0;  // DNE in Dasm
    int16_t return_value = 0;
    int16_t scsv5 = 0;
    int16_t scsv4 = 0;
    int16_t scsv3 = 0;
    int16_t scsv2 = 0;
    int16_t scsv1 = 0;
    int16_t itr = 0;
    int16_t curr_wx = 0;

    int16_t itr_wx = 0;  // _SI_
    int16_t itr_wy = 0;  // _DI_

    // saved_map_x = _map_x;
    // saved_map_y = _map_y;
    // saved_map_plane = _map_plane;
    /* CLAUDE */  /* REBUG */  /* asm: _map_plane, then _map_x, then _map_y */
    saved_map_plane = _map_plane;
    saved_map_x = _map_x;
    saved_map_y = _map_y;

    Allocate_Reduced_Map();

    Mark_Block(_screen_seg);

    // SOUNDFX.LBX, 106  "SILENT V"  "sILENCE"
    soundfx_seg = LBX_Reload_Next(soundfx_lbx_file__ovr130, 106, _screen_seg);
    soundfx_seg_size = lbxload_entry_length;
    
    if(player_idx == HUMAN_PLAYER_IDX)
    {

        return_value = Spell_Casting_Screen(stt_Map_Square, &scsv1, &scsv2, &scsv3, &scsv4, &scsv5, aEnchantRoad);

    }
    else
    {
        
        return_value = Get_Map_Square_Target_For_Spell(stt_Map_Square, &scsv1, &scsv2, &scsv3, spl_Enchant_Road, player_idx);

    }

    if(return_value == ST_TRUE)
    {

        if(
            (player_idx == HUMAN_PLAYER_IDX)
            ||
            (
                (magic_set.enemy_spells == ST_TRUE)
                &&
                (SQUARE_EXPLORED(scsv1, scsv2, scsv3) != ST_FALSE)
            )
        )
        {

            Center_Map(&_map_x, &_map_y, scsv1, scsv2, scsv3);

            _map_plane = scsv3;

            Mark_Time();

            Release_Time(3);

            Spell_Animation_Load_Sound_Effect__WIP(spl_Enchant_Road);

            Update_Remap_Color_Range(1, 4);

            // SPECFX.LBX, 046  "FULLBLUE"  "Enchant road"
            Open_File_Animation(specfx_lbx_file__ovr130, 46);

            for(itr = 0; itr < 22; itr++)
            {

                Mark_Time();

                Set_Page_Off();

                Fill(0, 20, 240, 199, ST_TRANSPARENT);

                Reset_Map_Draw();

                Main_Screen_Draw();

                Draw_File_Animation();

                PageFlip_FX();

                Release_Time(2);

            }


            // Play_Sound(SND_SpellCast, SND_SpellCast_size);
            Play_Sound(soundfx_seg, soundfx_seg_size);  /* CLAUDE */  /* REBUG */  /* asm: push [bp+soundfx_seg] / call Play_Sound (SOUNDFX.LBX 106, loaded above) */

        }

        /* CLAUDE */  /* REBUG */  /* asm: the visibility test's skip target is loc_B13FC, the road loop - the roads are enchanted on every successful cast, seen or not; ReMoM had the loops inside the visibility block */
        for(itr_wx = (scsv1 - 2); (scsv1 + 2) >= itr_wx; itr_wx++)
        {

            if(itr_wx < 0)
            {

                curr_wx = (itr_wx + WORLD_WIDTH);

            }
            /* OGBUG  off-by-one at the map edge: itr_wx == WORLD_WIDTH is kept as 60, which is x 0 of the next row */
            else if(itr_wx > WORLD_WIDTH)
            {

                curr_wx = (itr_wx - WORLD_WIDTH);

            }
            else
            {

                curr_wx = itr_wx;

            }

            for(itr_wy = (scsv2 - 2); (scsv2 + 2) >= itr_wy; itr_wy++)
            {

                /* OGBUG  off-by-one at the map edge: itr_wy == WORLD_HEIGHT (row 40) is accepted, which is past the plane */
                if(
                    (itr_wy >= 0)
                    &&
                    (itr_wy <= WORLD_HEIGHT)
                )
                {

                    Make_Road_Enchanted(curr_wx, itr_wy, scsv3);

                }

            }

        }

    }








    
    Dissolve_Main_Screen();

    Release_Block(_screen_seg);

    // _map_x = saved_map_x;
    // _map_y = saved_map_y;
    // _map_plane = saved_map_plane;
    /* CLAUDE */  /* REBUG */  /* asm: _map_plane, then _map_x, then _map_y */
    _map_plane = saved_map_plane;
    _map_x = saved_map_x;
    _map_y = saved_map_y;

    return return_value;

}


// WZD o130p14
int16_t Cast_Spell_Ward(int16_t player_idx)
{
    int8_t * city_enchantments = 0;
    int8_t ward_pos[NUM_MAGIC_TYPES] = { 0, 0, 0, 0, 0 };  // 1-byte, signed
    /* HACK */  char * ward_list[(NUM_MAGIC_TYPES + 1)] = { 0, 0, 0, 0, 0, 0 };
    char * strings[NUM_MAGIC_TYPES] = { 0, 0, 0, 0, 0 };
    int16_t magic_type_ench_idx = 0;
    int16_t status = 0;
    int16_t return_value = 0;
    int16_t scsv5 = 0;
    int16_t scsv4 = 0;
    int16_t scsv3 = 0;
    int16_t scsv2 = 0;
    int16_t city_idx = 0;  // scsv1
    int16_t itr = 0;
    int16_t ctr = 0;  // _DI_

    strings[0] = cnst_NatureWard_3;
    strings[1] = cnst_SorceryWard_3;
    strings[2] = cnst_ChaosWard_3;
    strings[3] = cnst_LifeWard_3;
    strings[4] = cnst_DeathWard_3;

    Allocate_Reduced_Map();

    Mark_Block(_screen_seg);

    if(player_idx != HUMAN_PLAYER_IDX)
    {

        return_value = AITP_Spell_Ward_Wrapper(&city_idx, &magic_type_ench_idx, player_idx);
        
        magic_type_ench_idx += 9;

    }
    else
    {

        status = ST_FALSE;

        return_value = ST_TRUE;

        while((status == ST_FALSE) && (return_value == ST_TRUE))
        {

            return_value = Spell_Casting_Screen(stt_Friendly_City, &city_idx, &scsv2, &scsv3, &scsv4, &scsv5, aSpellWard);

            if(return_value == ST_TRUE)
            {

                if(
                    ((_CITIES[city_idx].enchantments[NATURE_WARD]) > ST_FALSE)
                    &&
                    ((_CITIES[city_idx].enchantments[SORCERY_WARD]) > ST_FALSE)
                    &&
                    ((_CITIES[city_idx].enchantments[CHAOS_WARD]) > ST_FALSE)
                    &&
                    ((_CITIES[city_idx].enchantments[LIFE_WARD]) > ST_FALSE)
                    &&
                    ((_CITIES[city_idx].enchantments[DEATH_WARD]) > ST_FALSE)
                )
                {

                    LBX_Load_Data_Static(message_lbx_file__ovr130, 0, (SAMB_ptr)&GUI_NearMsgString[0], 44, 1, 150);

                    Warn0(GUI_NearMsgString);

                }
                else
                {

                    for(itr = 0; itr < NUM_MAGIC_TYPES; itr++)
                    {

                        ward_list[itr] = str_empty_string__ovr130;

                        ward_pos[itr] = ST_UNDEFINED;

                    }

                    ctr = 0;

                    if((_CITIES[city_idx].enchantments[NATURE_WARD]) == ST_FALSE)
                    {

                        ward_list[ctr] = strings[0];

                        ward_pos[ctr] = 0;

                        ctr++;

                    }

                    if((_CITIES[city_idx].enchantments[SORCERY_WARD]) == ST_FALSE)
                    {

                        ward_list[ctr] = strings[1];

                        ward_pos[ctr] = 1;

                        ctr++;

                    }

                    if((_CITIES[city_idx].enchantments[CHAOS_WARD]) == ST_FALSE)
                    {

                        ward_list[ctr] = strings[2];

                        ward_pos[ctr] = 2;

                        ctr++;

                    }

                    if((_CITIES[city_idx].enchantments[LIFE_WARD]) == ST_FALSE)
                    {

                        ward_list[ctr] = strings[3];

                        ward_pos[ctr] = 3;

                        ctr++;

                    }

                    if((_CITIES[city_idx].enchantments[DEATH_WARD]) == ST_FALSE)
                    {

                        ward_list[ctr] = strings[4];

                        ward_pos[ctr] = 4;

                        ctr++;

                    }

                    /* HACK */  ward_list[ctr] = str_empty_string__ovr130;

                    magic_type_ench_idx = Selection_Box(ctr, &ward_list[0], ST_FALSE, aSelectASpellWa);  // "Select a Spell Ward To Cast"

                    if(magic_type_ench_idx != ST_UNDEFINED)
                    {

                        status = ST_TRUE;

                        magic_type_ench_idx = (9 + ward_pos[magic_type_ench_idx]);

                    }

                }

            }

        }

    }

    if(return_value == ST_TRUE)
    {

        if(
            (player_idx == HUMAN_PLAYER_IDX)
            ||
            (_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX)
            ||
            (
                (magic_set.enemy_spells == ST_TRUE)
                &&
                (SQUARE_EXPLORED(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp) != ST_FALSE)
                &&
                (_players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] != ST_FALSE)
            )
        )
        {

            City_Enchantment_Anim_1__WIP(city_idx, spl_Spell_Ward, player_idx);

        }

        city_enchantments = &_CITIES[city_idx].enchantments[0];

        city_enchantments[magic_type_ench_idx] = (player_idx + 1);

        if(
            (player_idx == HUMAN_PLAYER_IDX)
            ||
            (_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX)
            ||
            (
                (magic_set.enemy_spells == ST_TRUE)
                &&
                (SQUARE_EXPLORED(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp) != ST_FALSE)
                &&
                (_players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] != ST_FALSE)
            )
        )
        {

            City_Enchantment_Anim_2__WIP(city_idx, spl_Earthquake, player_idx);

            Release_Block(_screen_seg);

        }

    }

    return return_value;

}


// WZD o130p15
// drake178: sub_B1843()
/*
    turns off bonus from Holy Weapon / High Prayer
*/
void Cast_Holy_Arms(int16_t player_idx)
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
/*
Consecration:
Life. City Enchantment. Casting Cost: 400 mana;
Upkeep: 8 mana/turn. Very Rare.
Dispels all negative enchantments on a city and protects the city
from further chaos and death enchantments (whether city-specific
or global).
*/
void Apply_Consecration(int16_t city_idx)
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
    // _CITIES[city_idx].enchantments[CHAOS_RIFT] = 0;
    /* CLAUDE */  /* REBUG */  /* asm: five stores only (CHAOS_RIFT, EVIL_PRESENCE, CURSED_LANDS, FAMINE, PESTILENCE; asm 15-44) - no second CHAOS_RIFT clear */

    city_wp = _CITIES[city_idx].wp;

    /* OGBUG  full 5x5 square, with no corner skip: cleanses the 4 corner squares outside the city's catchment area */
    // for(itr_wx = (_CITIES[city_idx].wx - 2); itr_wx < (_CITIES[city_idx].wx + 2); itr_wx++)
    /* CLAUDE */  /* REBUG */  /* asm: wx + 2 / cmp ax, di / jl @@Done (asm 125-135) */
    for(itr_wx = (_CITIES[city_idx].wx - 2); (_CITIES[city_idx].wx + 2) >= itr_wx; itr_wx++)
    {

        if(itr_wx < 0)
        {
            cur_wx = (itr_wx + WORLD_WIDTH);
        }
        /* OGBUG  off-by-one at the map edge: itr_wx == WORLD_WIDTH is kept as 60, which is x 0 of the next row */
        else if(itr_wx > WORLD_WIDTH)
        {
            cur_wx = (itr_wx - WORLD_WIDTH);
        }
        else
        {
            cur_wx = itr_wx;
        }

        // for(itr_wy = (_CITIES[city_idx].wy - 2); itr_wy < (_CITIES[city_idx].wy + 2); itr_wy++)
        /* CLAUDE */  /* REBUG */  /* asm: wy + 2 / cmp ax, [bp+itr_wy] / jge loc_B1987 (asm 113-122) */
        for(itr_wy = (_CITIES[city_idx].wy - 2); (_CITIES[city_idx].wy + 2) >= itr_wy; itr_wy++)
        {

            /* OGBUG  off-by-one at the map edge: itr_wy == WORLD_HEIGHT (row 40) is accepted, which is past the plane */
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

    _page_flip_effect = pfe_Dissolve;

}


// WZD o130p17
void Apply_Subversion(int16_t player_idx)
{
    int16_t itr = 0;
    for(itr = 0; itr < _num_players; itr++)
    {
        if(itr == 0)
        {
            continue;
        }
        /* OGBUG  skips only the human and the target, so an AI caster also lowers its own relation with the target */
        if(itr != player_idx)
        {
            if(_players[itr].Dipl.Contacted[player_idx] == ST_TRUE)
            {
                _players[itr].Dipl.Visible_Rel[player_idx] -= 25;
                if(_players[itr].Dipl.Visible_Rel[player_idx] < -100)
                {
                    _players[itr].Dipl.Visible_Rel[player_idx] = -100;
                }
                _players[player_idx].Dipl.Visible_Rel[itr] = _players[itr].Dipl.Visible_Rel[player_idx];
                _players[player_idx].peace_duration[itr] = 0;
                _players[itr].peace_duration[player_idx] = 0;
            }
        }
    }
}


// WZD o130p18
// drake178: sub_B1ABE()
void Cast_Natures_Awareness(int16_t player_idx)
{
    if(player_idx == HUMAN_PLAYER_IDX)
    {
        Allocate_Reduced_Map();
        Cheat_Reveal();
        All_AI_Players_Contacted();
        Release_Block(_screen_seg);
    }
}
