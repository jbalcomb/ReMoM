/*
    WIZARDS.EXE
        ovr130
*/

#include "MOM_DEF.H"
#include "MOX/Allocate.H"
#include "MOX/MOX_BASE.H"
#include "MOX/MOX_DAT.H"
#include "MOX/MOX_T4.H"
#include "MOX/GENDRAW.H"
#include "MOX/random.H"
#include "MOX/MOM_Data.H"
#include "MOX/MOX_DEF.H"
#include "MOX/MOX_TYPE.H"

#include "AISPELL.H"
#include "MainScr.H"
#include "OverSpel.H"
#include "STU/STU_CHK.H"
#include "Spells129.H"
#include "Spells130.H"
#include "SPELLDEF.H"
#include "NEXTTURN.H"
#include "Spellbook.H"
#include "SPLMASTR.H"
#include "Spells137.H"
#include "Terrain.H"
#include "UNITTYPE.H"
#include "WZD_o059.H"



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
int16_t Cast_DeathWish(int16_t player_idx)
{

    return ST_FALSE;

}


// WZD o130p03
int16_t Cast_GreatUnsummoning(int16_t player_idx)
{

    return ST_FALSE;

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

    Allocate_Reduced_Map();

    Mark_Block(_screen_seg);

    if(player_idx == HUMAN_PLAYER_IDX)
    {

        return_value = Spell_Casting_Screen__WIP(stt_Enemy_Group, &scsv1, &scsv2, &scsv3, &scsv4, &scsv5, aBlackWind);

        scsv2 = _UNITS[scsv1].wy;
        scsv3 = _UNITS[scsv1].wp;
        scsv1 = _UNITS[scsv1].wx;

    }
    else
    {

        return_value = IDK_Pick_Target_For_Unit_Enchantment__STUB(stt_Enemy_Group, &scsv1, spl_Black_Wind, player_idx);

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

        if(
            (player_idx == HUMAN_PLAYER_IDX)
            ||
            (_UNITS[scsv1].owner_idx == HUMAN_PLAYER_IDX)  // BUGBUG  scsv1 is wx here, not unit_idx ...c&p error
            ||
            (Check_Square_Scouted(scsv1, scsv2, scsv3) != ST_FALSE)
        )
        {
            
            AI_Eval_After_Spell = ST_TRUE;

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
            Change_Relations__WIP(-3, player_idx, _UNITS[scsv1].owner_idx, 8, 0, scsv1);

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
int16_t Cast_Earthlore(int16_t player_idx)
{

    return ST_FALSE;

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

        Spell_Target_Global_Enchantment_Screen(spl_Spell_Binding, player_idx);

    }
    else
    {

        var_A = IDK_AITP_Disjunction__STUB(&target_player_idx, &spell_idx, spl_Spell_Binding, player_idx);

        if(var_A == 1)
        {

            threshold = (_players[player_idx].casting_cost_original + Calculate_Dispel_Difficulty(spell_data_table[spell_idx].casting_cost, target_player_idx, spell_data_table[spell_idx].magic_realm));

            threshold = ((_players[player_idx].casting_cost_original * 250) / threshold);

            if(Random(250) <= threshold)
            {

                ptr_enchantments = &_players[target_player_idx].Globals[0];

                ptr_enchantments[spell_data_table[spell_idx].ge_idx] = 0;

                ptr_enchantments = &_players[player_idx].Globals[0];

                ptr_enchantments[spell_data_table[spell_idx].ge_idx] = (player_idx + 1);  // player_num

            }

        }

    }

}


// WZD o130p08
int16_t Cast_ChangeTerain(int16_t player_idx)
{

    return ST_FALSE;

}


// WZD o130p09
int16_t Cast_RaiseVolcano(int16_t player_idx)
{

    return ST_FALSE;

}


// WZD o130p10
int16_t Cast_Transmute(int16_t player_idx)
{

    return ST_FALSE;

}


// WZD o130p11
int16_t Cast_Corruption(int16_t player_idx)
{

    return ST_FALSE;

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

        return_value = Spell_Casting_Screen__WIP(stt_Friendly_Unit, &scsv1, &scsv2, &scsv3, &scsv4, &scsv5, aWordOfRecall);

    }
    else
    {

        return_value = IDK_Pick_Target_For_Unit_Enchantment__STUB(stt_Friendly_Unit, &scsv1, spl_Word_Of_Recall, player_idx);

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

        _UNITS[scsv1].wx = _players[player_idx].summon_wx;
        _UNITS[scsv1].wy = _players[player_idx].summon_wy;
        _UNITS[scsv1].wp = _players[player_idx].summon_wp;

        UNIT_RemoveExcess(scsv1);

    }

    Full_Draw_Main_Screen();

    Release_Block(_screen_seg);

    return return_value;

}


// WZD o130p13
int16_t Cast_EnchantRoad(int16_t player_idx)
{

    return ST_FALSE;

}


// WZD o130p14
int16_t Cast_Spell_Ward(int16_t player_idx)
{
    uint8_t * ptr_enchantments = 0;
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

        return_value = AITP_SpellWard_Wrapper__STUB(&city_idx, &magic_type_ench_idx, player_idx);
        
        magic_type_ench_idx += 9;

    }
    else
    {

        status = ST_FALSE;

        return_value = ST_TRUE;

        while((status == ST_FALSE) && (return_value == ST_TRUE))
        {

            return_value = Spell_Casting_Screen__WIP(stt_Friendly_City, &city_idx, &scsv2, &scsv3, &scsv4, &scsv5, aSpellWard);

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

            Cast_Spell_City_Enchantment_Animation_1__WIP(city_idx, spl_Spell_Ward, player_idx);

        }

        ptr_enchantments = &_CITIES[city_idx].enchantments[0];

        ptr_enchantments[magic_type_ench_idx] = (player_idx + 1);
Capture_Cities_Data();

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

            Cast_Spell_City_Enchantment_Animation_2__WIP(city_idx, spl_Earthquake, player_idx);

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

    _page_flip_effect = pfe_Dissolve;

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
