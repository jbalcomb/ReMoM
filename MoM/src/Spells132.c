/*
    WIZARDS.EXE
        ovr132
*/

#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_GARB.h"

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/capture.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/GENDRAW.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_BASE.h"
#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_SET.h"
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/special.h"
#include "../../MoX/src/Util.h"  /* Copy_Memory_Near() */

#include "AISPELL.h"
#include "City_ovr55.h"
#include "CITYCALC.h"
#include "HIRE.h"
#include "Items.h"
#include "Explore.h"
#include "MainScr.h"
#include "MainScr_Maps.h"
#include "SCastScr.h"
#include "SPELLDEF.h"
#include "Spells130.h"
#include "SPLMASTR.h"
#include "NEXTTURN.h"
#include "SBookScr.h"
#include "Spellbook.h"
#include "Spells129.h"
#include "Spells137.h"
#include "Terrain.h"
#include "TerrType.h"
#include "UNITTYPE.h"
#include "UnitMove.h"
#include "OverSpel.h"  /* Get_Spell_For_City_Enchantment(); Calculate_Dispel_Difficulty() */
#include "RACETYPE.h"  /* rt_Death */
#include "WZD_o059.h"
#include "WZD_o143.h"

#include "../../ext/stu_compat.h"

#include <stdlib.h>

#include "Spells132.h"



// WZD dseg:68A0                                                 ¿ BEGIN:  ovr132 - Strings ?

// WZD dseg:68A0
char message_lbx_file__ovr132[] = "message";
// WZD dseg:68A8
char names_lbx_file__ovr132[] = "NAMES";
// WZD dseg:68AE
char cnst_PlaneShift_2[] = "Plane Shift";
// WZD dseg:68BA
char soundfx_lbx_file__ovr132[] = "soundfx";
// wzd dseg:68C2
char str_Floating_Island__ovr132[] = "Floating Island";
// wzd dseg:68D2
char aLycanthropy[] = "Lycanthropy";
// wzd dseg:68DE
char aWallOfStone[] = "Wall of Stone";
// wzd dseg:68EC
char aSpellOfReturn[] = "Spell of Return";

// WZD dseg:68BA                                                 ¿ END:  ovr132 - Strings ?



/*
    WIZARDS.EXE  ovr132
*/

// WZD o132p01
/*

type
    1: Hero
    2: Champion

Hire_Hero_Popup()
    |-> Hire_Hero()
        |-> Create_Unit()

*/
// int16_t Cast_Summon_Hero(int16_t player_idx, int16_t type)
void Cast_Summon_Hero(int16_t player_idx, int16_t type)
{
    int16_t hired = 0;
    int16_t hero_slot_idx = 0;
    int16_t unit_type = 0;  // _DI_

    hero_slot_idx = Hero_Slot_Open(player_idx);

    if(hero_slot_idx == ST_UNDEFINED)
    {

        if(player_idx == HUMAN_PLAYER_IDX)
        {

            Full_Draw_Main_Screen();

            LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 47, 1, 150);

            Warn0(GUI_NearMsgString);

        }

    }
    else
    {

        unit_type = Pick_Random_Hero(player_idx, 1, type);

        if(
            (unit_type != ST_UNDEFINED)
            &&
            (
                (_units < 950)
                ||
                (player_idx == HUMAN_PLAYER_IDX)
            )
            &&
            (_units < 980)
        )
        {

            if(player_idx == HUMAN_PLAYER_IDX)
            {

                if((_HEROES2[player_idx]->heroes[unit_type].abilities & HSA_FEMALE) != 0)
                {

                    if(type == 1)
                    {

                        Summon_Animation(-3, 3, player_idx);

                    }
                    else  /* type == 2 */
                    {

                        Summon_Animation(-30, 3, player_idx);

                    }

                }
                else
                {

                    if(type == 1)
                    {

                        Summon_Animation(-2, 3, player_idx);

                    }
                    else  /* type == 2 */
                    {

                        Summon_Animation(-20, 3, player_idx);

                    }

                }

                Allocate_Reduced_Map();

                if(type == 1)
                {

                    hired = Hire_Hero_Popup(hero_slot_idx, unit_type, 1);

                }
                else  /* type == 2 */
                {

                    hired = Hire_Hero_Popup(hero_slot_idx, unit_type, 3);

                }

                if(hired == ST_TRUE)
                {

                    _active_world_x = _UNITS[(_units - 1)].wx;
                    _active_world_y = _UNITS[(_units - 1)].wy;
                    _map_plane = _UNITS[(_units - 1)].wp;

                    /* OGBUG  Select_Stack_At_Unit() is called with nothing pushed: its unit_idx is the stale word on top of the stack (the caller's saved register), not the new unit (_units - 1), so it selects and centres on another unit's stack, or reads past the live units */
                    // Select_Stack_At_Unit((_units - 1));
                    /* HACK */  Select_Stack_At_Unit(Stack_Garbage_Range(0, (_units - 1)));  /* CLAUDE */  /* OG: no argument pushed (the callee reads [bp+6] = the caller's saved DI); the stand-in stays inside the live units */

                }

            }
            else
            {

                if(
                    (magic_set.enemy_spells == ST_TRUE)
                    &&
                    (_players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] > 0)
                    &&
                    (_players[HUMAN_PLAYER_IDX].Dipl.Contacted[player_idx] == ST_TRUE)
                )
                {

                    if((_HEROES2[player_idx]->heroes[unit_type].abilities & HSA_FEMALE) != 0)
                    {

                        Summon_Animation(-3, 3, player_idx);

                    }
                    else
                    {

                        Summon_Animation(-2, 3, player_idx);

                    }

                }

                Hire_Hero(player_idx, unit_type, hero_slot_idx, ST_FALSE);

                Evict_Weakest_Unit((_units - 1));

                _HEROES2[player_idx]->heroes[unit_type].Level = -20;  // Why?

            }

        }
        else
        {

            if(player_idx == HUMAN_PLAYER_IDX)
            {

                Full_Draw_Main_Screen();

                LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 48, 1, 150);

                Warn0(GUI_NearMsgString);

            }

        }

    }

}

// WZD o132p02
// int16_t Cast_Incarnation(int16_t player_idx)
void Cast_Incarnation(int16_t player_idx)
{
    int16_t hired = 0;
    int16_t hero_slot_idx = 0;
    int16_t unit_type = 0;  // _DI_

    hero_slot_idx = Hero_Slot_Open(player_idx);

    if(hero_slot_idx == ST_UNDEFINED)
    {

        if(player_idx == HUMAN_PLAYER_IDX)
        {

            Full_Draw_Main_Screen();

            LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 49, 1, 150);

            Warn0(GUI_NearMsgString);

        }

    }
    else
    {

        if(_HEROES2[player_idx]->heroes[ut_Chosen].Level != -20)
        {

            unit_type = ut_Chosen;

        }
        else
        {

            unit_type = ST_UNDEFINED;

        }

        if(
            (_units < 980)
            &&
            (
                (_units < 950)
                ||
                (player_idx == HUMAN_PLAYER_IDX)
            )
        )
        {

            if(unit_type == ST_UNDEFINED)
            {

                if(player_idx == HUMAN_PLAYER_IDX)
                {

                    Full_Draw_Main_Screen();

                    LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 51, 1, 150);

                    Warn0(GUI_NearMsgString);

                }

            }
            else
            {

                if(player_idx == HUMAN_PLAYER_IDX)
                {

                    Allocate_Reduced_Map();

                    hired = Hire_Hero_Popup(hero_slot_idx, unit_type, 1);

                    if(hired == ST_TRUE)
                    {

                        _active_world_x = _players[player_idx].summon_wx;
                        _active_world_y = _players[player_idx].summon_wy;
                        _map_plane = _players[player_idx].summon_wp;

                        /* OGBUG  Select_Stack_At_Unit() is called with nothing pushed: its unit_idx is the stale word on top of the stack (the caller's saved register), not the new unit (_units - 1), so it selects and centres on another unit's stack, or reads past the live units */
                        /* HACK */  Select_Stack_At_Unit(Stack_Garbage_Range(0, (_units - 1)));

                    }

                }
                else
                {

                    Hire_Hero(player_idx, unit_type, hero_slot_idx, ST_FALSE);

                    Evict_Weakest_Unit((_units - 1));

                    _HEROES2[player_idx]->heroes[unit_type].Level = -20;  // Why?

                }

            }

        }
        else
        {

            if(player_idx == HUMAN_PLAYER_IDX)
            {

                Full_Draw_Main_Screen();

                LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 50, 1, 150);

                Warn0(GUI_NearMsgString);

            }

        }

    }

}


// WZD o132p03
// drake178: CTY_ChaosRift()
/*
Chaos Rift, each turn (Apply_City_Changes()): up to five strength-8 Lightning Bolts at random units in the city, then a 5% chance to destroy each building
items from killed heroes go to the city owner's item pool; the human gets 'unit killed' and 'building lost' notices
*/
void Apply_Chaos_Rift(int16_t city_idx)
{
    int16_t unit_list_idx = 0;
    int16_t unit_count = 0;
    int16_t original_unit_count = 0;
    int16_t surviving_list_idx = 0;
    int16_t item_count = 0;
    int16_t unit_list[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t original_unit_list[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t item_list[(NUM_HEROES * NUM_HERO_ITEMS)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t building_list[NUM_BUILDINGS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t itr_buildings = 0;  // _SI_
    int16_t itr = 0;  // _DI_

    Army_At_City(city_idx, &unit_count, &unit_list[0]);

    original_unit_count = unit_count;

    Copy_Memory_Near((uint8_t *)&original_unit_list[0], (uint8_t *)&unit_list[0], sizeof(unit_list));

    item_count = 0;

    for(itr = 0; ((itr < 5) && (unit_count > 0)); itr++)
    {

        unit_list_idx = (Random(unit_count) - 1);

        if(unit_list[unit_list_idx] > ST_UNDEFINED)
        {

            Cast_Attack_Spell_On_Enemy_Unit(unit_list[unit_list_idx], spl_Lightning_Bolt, &item_count, &item_list[0], 8);

        }

        Army_At_City(city_idx, &unit_count, &unit_list[0]);

    }

    item_pool_in_process = ST_TRUE;

    m_item_wx = _CITIES[city_idx].wx;

    m_item_wy = _CITIES[city_idx].wy;

    m_item_wp = _CITIES[city_idx].wp;

    Player_Process_Item_Pool(_CITIES[city_idx].owner_idx, item_count, &item_list[0]);

    if(
        (unit_count != original_unit_count)
        &&
        (_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX)
    )
    {

        surviving_list_idx = 0;

        /* OGBUG  Army_At_City() rewrites only the first unit_count entries, so unit_list[] past the survivors still holds the earlier lists: a killed unit that was last in the city's list matches its own stale entry and is never reported (a lone garrison wiped out included) */
        for(itr = 0; itr < original_unit_count; itr++)
        {

            if(unit_list[surviving_list_idx] != original_unit_list[itr])
            {

                if(MSG_UnitKilled_Count < 20)
                {

                    MSG_UnitKilled_Array[MSG_UnitKilled_Count].Unit_Type = _UNITS[original_unit_list[itr]].type;

                    MSG_UnitKilled_Array[MSG_UnitKilled_Count].City = (int8_t)city_idx;

                    MSG_UnitKilled_Array[MSG_UnitKilled_Count].Spell = spl_Chaos_Rift;

                    MSG_UnitKilled_Count++;

                }

            }
            else
            {

                surviving_list_idx++;

            }

        }

    }

    for(itr_buildings = 0; itr_buildings < NUM_BUILDINGS; itr_buildings++)
    {

        building_list[itr_buildings] = 0;

    }

    Apply_Damage_To_City(city_idx, 0, 5, &building_list[0]);

    if(_CITIES[city_idx].owner_idx == HUMAN_PLAYER_IDX)
    {

        for(itr_buildings = 0; itr_buildings < NUM_BUILDINGS; itr_buildings++)
        {

            if(
                (building_list[itr_buildings] > 0)
                &&
                (MSG_BldLost_Count < 20)
            )
            {

                MSG_BldLost_Array[MSG_BldLost_Count].city_idx = (int8_t)city_idx;

                MSG_BldLost_Array[MSG_BldLost_Count].bldg_type_idx = building_list[itr_buildings];

                MSG_BldLost_Count++;

            }

        }

    }

}


// WZD o132p04
// drake178: WIZ_MeteorStorm()
/*
Meteor Storm, each turn (Next_Turn_Proc()):
    every city not owned by the caster takes Apply_Damage_To_City(city_idx, 0, 1, ...), unless countered
    every unit that is NOT in a city (of anyone's) takes the spl_Call_Chaos conventional spell attack
NOTE:  the asm passes spl_Call_Chaos, not spl_Meteor_Storms, to Cast_Attack_Spell_On_Enemy_Unit(); the killed-unit message still says spl_Meteor_Storms
*/
void Apply_Meteor_Storm(int16_t player_idx)
{
    int16_t building_list[NUM_BUILDINGS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t unit_list[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t item_list[3] = { 0, 0, 0 };
    int16_t item_count = 0;
    int16_t unit_owner_idx = 0;
    int16_t unit_count = 0;
    int16_t itr_units = 0;  // also the city iterator
    uint8_t * unit_in_city = NULL;  // _DI_
    int16_t itr = 0;  // _SI_

    unit_in_city = Near_Allocate_First(_units);

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        unit_in_city[itr_units] = ST_FALSE;
    }

    for(itr_units = 0; itr_units < _cities; itr_units++)
    {
        Army_At_City(itr_units, &unit_count, &unit_list[0]);
        for(itr = 0; itr < unit_count; itr++)
        {
            unit_in_city[unit_list[itr]] = ST_TRUE;
        }
        if(Apply_Automatic_Spell_Counters(spl_Meteor_Storms, itr_units, 0, 0) != 0)
        {
            if(_CITIES[itr_units].owner_idx != player_idx)
            {
                for(itr = 0; itr < NUM_BUILDINGS; itr++)
                {
                    building_list[itr] = 0;
                }
                Apply_Damage_To_City(itr_units, 0, 1, &building_list[0]);
                if(_CITIES[itr_units].owner_idx == HUMAN_PLAYER_IDX)
                {
                    for(itr = 0; itr < NUM_BUILDINGS; itr++)
                    {
                        if(building_list[itr] > 0 && MSG_BldLost_Count < 20)
                        {
                            MSG_BldLost_Array[MSG_BldLost_Count].city_idx = itr_units;
                            MSG_BldLost_Array[MSG_BldLost_Count].bldg_type_idx = building_list[itr];
                            MSG_BldLost_Count++;
                        }
                    }
                }
            }
        }
    }

    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(unit_in_city[itr_units] == ST_FALSE)
        {
            unit_owner_idx = _UNITS[itr_units].owner_idx;
            item_count = 0;
            item_pool_in_process = ST_TRUE;
            m_item_wx = _UNITS[itr_units].wx;
            m_item_wy = _UNITS[itr_units].wy;
            m_item_wp = _UNITS[itr_units].wp;
            Cast_Attack_Spell_On_Enemy_Unit(itr_units, spl_Call_Chaos, &item_count, &item_list[0], 0);
            Player_Process_Item_Pool(unit_owner_idx, item_count, &item_list[0]);
            if(unit_owner_idx == HUMAN_PLAYER_IDX)
            {
                if(_UNITS[itr_units].owner_idx == ST_UNDEFINED)
                {
                    if(MSG_UnitKilled_Count < 20)
                    {
                        MSG_UnitKilled_Array[MSG_UnitKilled_Count].Unit_Type = _UNITS[itr_units].type;
                        MSG_UnitKilled_Array[MSG_UnitKilled_Count].City = ST_UNDEFINED;
                        MSG_UnitKilled_Array[MSG_UnitKilled_Count].Spell = spl_Meteor_Storms;
                        MSG_UnitKilled_Count++;
                    }
                }
            }
        }
    }

}


// WZD o132p05
// drake178: CTY_StreamOfLife()
/*
Stream of Life, each turn (Apply_City_Changes()): every unit in the city that is neither Death-realm nor undead is fully healed (Damage = 0)
*/
void Apply_Stream_Of_Life(int16_t city_idx)
{
    int16_t unit_count = 0;
    int16_t unit_list[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t itr = 0;  // _SI_

    Army_At_City(city_idx, &unit_count, &unit_list[0]);

    for(itr = 0; itr < unit_count; itr++)
    {

        if(
            (_unit_type_table[_UNITS[unit_list[itr]].type].race_type != rt_Death)
            &&
            ((_UNITS[unit_list[itr]].mutations & UM_UNDEAD) == 0)
        )
        {

            _UNITS[unit_list[itr]].Damage = 0;

        }

    }

}


// WZD o132p06
// drake178: WIZ_HireHero()
// ¿ MoO2  Module: OFFICER  Set_Officer_To_Player_()
// Hire_Officer_() |-> Set_Officer_To_Player_()  ...Hire_Officer_() handles the officer cost
/*

    assumes (_units - 1)

unit_type_idx is passed over all the way from Hire_Hero_Popup()
so, not quite 'Generate Random Hero'


XREF:
    Cast_Summon_Hero()   (o132p01)  saved_flag = ST_FALSE
    Cast_Incarnation()   (o132p02)  saved_flag = ST_FALSE
    Cast_Resurrection()  (o132p07)  saved_flag = ST_TRUE  (x2)
    j_Hire_Hero()
        Hire_Hero_Popup()  (o127p06)  saved_flag = ST_FALSE
        AI_Accept_Hero()   (o127p09)  saved_flag = ST_FALSE
e.g.,
    Hire_Hero_Popup()
        |-> j_Hire_Hero(HUMAN_PLAYER_IDX, unit_type_idx, hero_slot_idx, ST_FALSE)

    void AI_Accept_Hero(int16_t player_idx, int16_t hero_slot_idx, int16_t unit_type_idx)
        Hero_Hired = Hire_Hero(player_idx, unit_type_idx, hero_slot_idx, ST_FALSE);

"Hire" as in "Summon"
...except "Summon" uses 'Summoning Circle', rather than 'Fortress City'
*/
int16_t Hire_Hero(int16_t player_idx, int16_t unit_type_idx, int16_t hero_slot_idx, int16_t saved_flag)
{
    int16_t itr = 0;

    Create_Unit(unit_type_idx, player_idx, FORTX(), FORTY(), FORTP(), ST_UNDEFINED);

    _UNITS[(_units - 1)].Finished = ST_FALSE;  // default is ST_TRUE
    _UNITS[(_units - 1)].moves2 = _UNITS[(_units - 1)].moves2_max;  // default is 0
    _UNITS[(_units - 1)].Hero_Slot = hero_slot_idx;  // default is ST_UNDEFINED

    _players[player_idx].Heroes[hero_slot_idx].unit_idx = (_units - 1);

    Hero_Slot_Types(unit_type_idx, _players[player_idx].Heroes[hero_slot_idx].Item_Slots);

    for(itr = 0; itr < NUM_HERO_ITEM_SLOTS; itr++)
    {

        _players[player_idx].Heroes[hero_slot_idx].Items[itr] = ST_UNDEFINED;
    }

    if(saved_flag == ST_TRUE)
    {

        if(player_idx == HUMAN_PLAYER_IDX)
        {

            stu_strcpy(_players[HUMAN_PLAYER_IDX].Heroes[hero_slot_idx].name, hero_names_table[unit_type_idx].name);

            _UNITS[(_units - 1)].XP = hero_names_table[unit_type_idx].experience_points;

            _UNITS[(_units - 1)].Level = Calc_Unit_Level((_units - 1));

        }
        else
        {

            LBX_Load_Data_Static(names_lbx_file__ovr132, 0, (SAMB_ptr)_players[player_idx].Heroes[hero_slot_idx].name, ((player_idx * NUM_HERO_TYPES) + unit_type_idx), 1, 13);

            _UNITS[(_units - 1)].Level = (abs(_HEROES2[player_idx]->heroes[unit_type_idx].Level) - 1);

            _UNITS[(_units - 1)].XP = TBL_Experience[_UNITS[(_units - 1)].Level];

        }

    }
    else
    {

        LBX_Load_Data_Static(names_lbx_file__ovr132, 0, (SAMB_ptr)_players[player_idx].Heroes[hero_slot_idx].name, ((player_idx * NUM_HERO_TYPES) + unit_type_idx), 1, 13);

        SETMAX(_HEROES2[player_idx]->heroes[unit_type_idx].Level, HL_GRANDLORD);

        _UNITS[(_units - 1)].Level = _HEROES2[player_idx]->heroes[unit_type_idx].Level;

        _UNITS[(_units - 1)].XP = TBL_Experience[_HEROES2[player_idx]->heroes[unit_type_idx].Level];

    }

    // BUG  Did this used to do something different? What tests it?
    // may be is/was success status as in cast the spell
    // NOTE(JimBalcomb,202409090905): AI_Accept_Hero() tests this
    /* CLAUDE */  /* NOTE  asm: mov ax, e_ST_TRUE  ...the OG always returns ST_TRUE; there is no failure path */
    return ST_TRUE;

}


// WZD o132p07
// int16_t Cast_Resurrection(int16_t player_idx)
void Cast_Resurrection(int16_t player_idx)
{
    int16_t hero_list[NUM_HERO_TYPES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t hero_slot_idx = 0;
    int16_t unit_type = 0;
    int16_t itr = 0;
    int16_t hero_ctr = 0;  // _DI_

    hero_ctr = 0;

    if(
        (_units < 980)
        ||
        (
            (_units < 950)
            &&
            (player_idx == HUMAN_PLAYER_IDX)
        )
    )
    {

        for(itr = 0; itr < (NUM_HERO_TYPES - 1); itr++)
        {

            if(
                (_HEROES2[player_idx]->heroes[itr].Level < 0)
                &&
                (_HEROES2[player_idx]->heroes[itr].Level != -20)
            )
            {

                hero_list[(1 + hero_ctr)] = itr;

                hero_ctr++;

            }

        }

        hero_slot_idx = Hero_Slot_Open(player_idx);

        if(player_idx != HUMAN_PLAYER_IDX)
        {

            if(
                (hero_ctr != 0)
                &&
                (hero_slot_idx != ST_UNDEFINED)
            )
            {

                unit_type = hero_list[hero_ctr];

                Hire_Hero(player_idx, unit_type, hero_slot_idx, ST_TRUE);

                Evict_Weakest_Unit((_units - 1));

            }

        }
        else
        {

            if(
                (hero_ctr != 0)
                &&
                (hero_slot_idx != ST_UNDEFINED)
            )
            {

                Allocate_Reduced_Map();

                Set_Page_Off();

                Main_Screen_Draw();

                PageFlip_FX();


                if(hero_ctr == 1)
                {

                    unit_type = hero_list[1];

                }
                else
                {

                    unit_type = Select_Hero_To_Ressurect(hero_ctr, &hero_list[1]);

                }

                Hire_Hero(player_idx, unit_type, hero_slot_idx, ST_TRUE);

                _UNITS[(_units - 1)].wx = (int8_t)_players[player_idx].summon_wx;
                _UNITS[(_units - 1)].wy = (int8_t)_players[player_idx].summon_wy;
                _UNITS[(_units - 1)].wp = (int8_t)_players[player_idx].summon_wp;

                Evict_Weakest_Unit((_units - 1));

                Spell_Animation_Load_Graphics(spl_Resurrection);

                Spell_Animation_Load_Sound_Effect__WIP(spl_Resurrection);

                Spell_Animation_Screen__WIP(_players[player_idx].summon_wx, _players[player_idx].summon_wy, _players[player_idx].summon_wp);

                _UNITS[(_units - 1)].Finished = ST_FALSE;

                _UNITS[(_units - 1)].moves2 = _UNITS[(_units - 1)].moves2_max;

                if(player_idx == HUMAN_PLAYER_IDX)
                {

                    _active_world_x = _UNITS[(_units - 1)].wx;
                    _active_world_y = _UNITS[(_units - 1)].wy;
                    _map_plane = _UNITS[(_units - 1)].wp;

                    /* OGBUG  Select_Stack_At_Unit() is called with nothing pushed: its unit_idx is the stale word on top of the stack (the caller's saved register), not the new unit (_units - 1), so it selects and centres on another unit's stack, or reads past the live units */
                    // Select_Stack_At_Unit((_units - 1));
                    /* HACK */  Select_Stack_At_Unit(Stack_Garbage_Range(0, (_units - 1)));  /* CLAUDE */  /* OG: no argument pushed (the callee reads [bp+6] = the caller's saved DI); the stand-in stays inside the live units */

                }

            }

        }

        _HEROES2[player_idx]->heroes[unit_type].Level = -20;

    }
    else
    {

        Full_Draw_Main_Screen();

        LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 50, 1, 150);

        Warn0(GUI_NearMsgString);

    }

}

// WZD o132p08
/*
    returns {0,1,2,3}
    {0: Success, 1: Failure - Water, 2: Failure - Enemy Stack, 3: Falure - Too Many Units}

¿ vs. WZD_o059.C  Check_Stack_Plane_Shift() ?
        stack_plane_shift = Check_Stack_Plane_Shift(_unit_stack[0].unit_idx, *map_plane);

*/
int16_t Cast_Plane_Shift_Check(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t move_modes[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int16_t troops2[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int16_t wp2 = 0;
    int16_t troop_count = 0;
    int16_t troops2_count = 0;
    int16_t return_value = 0;  // _SI_
    
    return_value = 0;

    wp2 = (1 - wp);

    Army_At_Square_1(wx, wy, wp, &troops2_count, &troops2[0]);

    Army_Movement_Modes(&move_modes[0], &troops2[0], troops2_count);

    if(Square_Is_Sailable(wx, wy, wp2) != ST_FALSE)
    {

        return_value = 1;

        if(
            (move_modes[MM_SWIMMING] > 0)
            ||
            (move_modes[MM_SAILING] > 0)
            ||
            (move_modes[MM_FLYING] > 0)
        )
        {

            return_value = 0;

        }

    }

    troop_count = 0;

    Army_At_Square_1(wx, wy, wp2, &troop_count, &troops[0]);

    if(troop_count > 0)
    {

        if(
            (_UNITS[troops[0]].owner_idx == _UNITS[troops2[0]].owner_idx)
            &&
            ((troops2_count + troop_count) > MAX_STACK)
        )
        {

            return_value = 3;

        }
        else
        {

            return_value = 2;

        }

    }

    return return_value;

}


// WZD o132p09
void Cast_Plane_Shift_Draw(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int16_t y1 = 0;
    int16_t x1 = 0;
    int16_t troop_count = 0;
    int16_t itr = 0;  // _SI_

    x1 = wx;
    y1 = wy;

    World_To_Screen(_map_x, _map_y, &x1, &y1);

    Army_At_Square_1(wx, wy, wp, &troop_count, &troops[0]);

    for(itr = 0; itr < troop_count; itr++)
    {

        _UNITS[troops[itr]].wp = (1 - _UNITS[troops[itr]].wp);

    }

    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

    Set_Page_Off();

    Main_Screen_Draw();

    Screen_Picture_Capture(x1, y1, (x1 + SQUARE_WIDTH), (y1 + SQUARE_HEIGHT), spell_animation_seg);

    for(itr = 0; itr < troop_count; itr++)
    {

        _UNITS[troops[itr]].wp = ((_UNITS[troops[itr]].wp + 1) % 2);

    }

    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

    Set_Page_On();

}


// WZD o132p10
/*

¿ vs. WZD_o059.C  Do_Plane_Button__WIP() ?

*/
int16_t Cast_Plane_Shift(int16_t player_idx)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t IDK_map_p = 0;
    int16_t IDK_map_y = 0;
    int16_t IDK_map_x = 0;
    SAMB_ptr sound_buffer = 0;
    /* HACK */  uint32_t sound_buffer_size = 0;  // DNE in Dasm
    int16_t plane = 0;
    int16_t plane_shift_status = 0;
    SAMB_ptr bitmap = 0;
    int16_t range = 0;
    int16_t troop_count = 0;
    int16_t return_value = 0;
    int16_t scsv5 = 0;
    int16_t scsv4 = 0;
    int16_t scsv3 = 0;
    int16_t scsv2 = 0;
    int16_t scsv1 = 0;
    int16_t itr = 0;  // _SI_

    IDK_map_x = _map_x;
    IDK_map_y = _map_y;
    IDK_map_p = _map_plane;

    troop_count = 0;

    for(itr = 0; itr < _num_players; itr++)
    {

        if(_players[itr].Globals[PLANAR_SEAL] > ST_FALSE)
        {

            if(player_idx == HUMAN_PLAYER_IDX)
            {

                LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 54, 1, 150);

                Warn0(GUI_NearMsgString);

            }

            return ST_FALSE;

        }

    }

    if(player_idx == HUMAN_PLAYER_IDX)
    {

        return_value = Spell_Casting_Screen(stt_Friendly_Group, &scsv1, &scsv2, &scsv3, &scsv4, &scsv5, cnst_PlaneShift_2);

        scsv2 = _UNITS[scsv1].wy;
        scsv3 = _UNITS[scsv1].wp;
        scsv1 = _UNITS[scsv1].wx;

    }
    else
    {

        return_value = Get_Map_Square_Target_For_Spell(stt_Friendly_Group, &scsv1, &scsv2, &scsv3, spl_Plane_Shift, player_idx);

    }

    plane = ((scsv3 + 1) % NUM_PLANES);

    if(return_value == ST_TRUE)
    {

        Army_At_Square_1(scsv1, scsv2, scsv3, &troop_count, &troops[0]);

        plane_shift_status = Cast_Plane_Shift_Check(scsv1, scsv2, scsv3);

        if(
            (player_idx == HUMAN_PLAYER_IDX)
            ||
            (
                (plane_shift_status == 0)
                &&
                (Check_Square_Scouted(scsv1, scsv2, scsv3) != ST_FALSE)
            )
        )
        {

            Allocate_Reduced_Map();

            Mark_Block(_screen_seg);

            sound_buffer = LBX_Reload_Next(soundfx_lbx_file__ovr132, 106, _screen_seg);
            sound_buffer_size = lbxload_entry_length;

            Spell_Animation_Load_Sound_Effect__WIP(spl_Plane_Shift);

            spell_animation_seg = Allocate_Next_Block(_screen_seg, 30);

            bitmap = Allocate_Next_Block(_screen_seg, 30);

            Bring_Square_Into_View(&_map_x, &_map_y, scsv1, scsv2, scsv3);

            _map_plane = scsv3;
            scsv4 = scsv1;
            scsv5 = scsv2;

            World_To_Screen(_map_x, _map_y, &scsv4, &scsv5);

            if(SND_SpellCast != (SAMB_ptr)ST_UNDEFINED)
            {
                Play_Sound(SND_SpellCast, SND_SpellCast_size);
            }

            Cast_Plane_Shift_Draw(scsv1, scsv2, scsv3);

            if(
                (player_idx == HUMAN_PLAYER_IDX)
                ||
                (Check_Square_Scouted(scsv1, scsv2, scsv3) != ST_FALSE)
            )
            {

                for(itr = 0; itr < 101; itr += 5)
                {

                    Copy_Bitmap_To_Bitmap(bitmap, spell_animation_seg);

                    Set_Page_Off();

                    Main_Screen_Draw();

                    Vanish_Bitmap(bitmap, itr);

                    Draw_Picture(scsv4, scsv5, bitmap);

                    Apply_Palette();

                    PageFlip_FX();

                }

            }

        }

        if(plane_shift_status == 0)
        {

            for(itr = 0; itr < troop_count; itr++)
            {

                _UNITS[troops[itr]].wp = ((_UNITS[troops[itr]].wp + 1) % 2);

            }

            if(player_idx == HUMAN_PLAYER_IDX)
            {

                _map_plane = ((_map_plane + 1) % 2);

                range = Player_Units_Max_Range(player_idx, scsv1, scsv2, _map_plane);

                // TODO  DIPLO  AI_ContactWizards(player_idx, scsv1, scsv2, _map_plane, range);
                Contact_Players_In_Scouting_Range(player_idx, scsv1, scsv2, _map_plane, range);  /* CLAUDE */  /* asm 332: call j_AI_ContactWizards; the return value is dropped */

                Set_Square_Explored_Flags(scsv1, scsv2, _map_plane);

                Update_Scouted_And_Contacted();

                Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

            }
            else
            {

                if(Check_Square_Scouted(scsv1, scsv2, plane) != ST_FALSE)  /* CLAUDE */  /* REBUG */  /* asm: loc_B55D1: push [bp+plane] / push scsv2 / push scsv1 / call j_Check_Square_Scouted / or ax, ax / jz loc_B55F4 */
                {

                    _map_plane = ((_map_plane + 1) % 2);

                }

            }

        }
        else
        {

            if(player_idx == HUMAN_PLAYER_IDX)
            {

                for(itr = 0; itr < troop_count; itr++)
                {

                    _UNITS[troops[itr]].wp = ((_UNITS[troops[itr]].wp + 1) % 2);

                }

                if(
                    (player_idx == HUMAN_PLAYER_IDX)
                    ||
                    // (Check_Square_Scouted(scsv1, scsv2, scsv3) != ST_FALSE)
                    (Check_Square_Scouted(scsv1, scsv2, plane) != ST_FALSE)  /* CLAUDE */  /* REBUG */  /* asm: push [bp+plane] / push [bp+scsv2] / push [bp+scsv1] / call j_Check_Square_Scouted */
                )
                {

                    _map_plane = ((_map_plane + 1) % 2);

                    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

                    Set_Square_Explored_Flags(scsv1, scsv2, _map_plane);

                    // _page_flip_effect = pfe_TogglePagesFadeIn;
                    _page_flip_effect = pfe_Dissolve;  /* CLAUDE */  /* REBUG */  /* asm: mov [_page_flip_effect], 3 */

                    Set_Page_Off();

                    Main_Screen_Draw();

                    PageFlip_FX();

                    _page_flip_effect = pfe_Default;

                    Update_Scouted_And_Contacted();

                    Cast_Plane_Shift_Draw(scsv1, scsv2, _map_plane);

                    for(itr = 100; itr >= 50; itr -= 10)
                    {

                        Copy_Bitmap_To_Bitmap(bitmap, spell_animation_seg);

                        Set_Page_Off();

                        Reset_Map_Draw();

                        Main_Screen_Draw();

                        Vanish_Bitmap(bitmap, itr);

                        Draw_Picture(scsv4, scsv5, bitmap);

                        Apply_Palette();

                        PageFlip_FX();

                    }

                    if(player_idx == HUMAN_PLAYER_IDX)
                    {

                        for(itr = 50; itr <= 100; itr += 10)
                        {

                            Copy_Bitmap_To_Bitmap(bitmap, spell_animation_seg);

                            Set_Page_Off();

                            Reset_Map_Draw();
                            
                            Main_Screen_Draw();

                            Vanish_Bitmap(bitmap, itr);

                            Draw_Picture(scsv4, scsv5, bitmap);

                            Apply_Palette();

                            PageFlip_FX();

                        }

                    }

                    for(itr = 0; itr < troop_count; itr++)
                    {

                        _UNITS[troops[itr]].wp = (1 - _UNITS[troops[itr]].wp);

                    }

                    if(player_idx == HUMAN_PLAYER_IDX)
                    {

                        switch(plane_shift_status)
                        {

                            case 1:
                            {

                                LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 55, 1, 150);

                                Warn0(GUI_NearMsgString);

                            } break;

                            case 2:
                            {

                                LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 56, 1, 150);

                                Warn0(GUI_NearMsgString);

                            } break;

                            case 3:
                            {
                                
                                LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 57, 1, 150);

                                Warn0(GUI_NearMsgString);

                            } break;

                            default:
                            {

                                STU_DEBUG_BREAK();

                            } break;
                            
                        }

                    }

                }

            }

        }

        /*
            AFTER:
                succes OR failure
        */
        if(
            (player_idx == HUMAN_PLAYER_IDX)
            ||
            (
                (plane_shift_status == 0)
                &&
                // (Check_Square_Scouted(scsv1, scsv2, scsv3) != ST_FALSE)
                (Check_Square_Scouted(scsv1, scsv2, _map_plane) != ST_FALSE)  /* CLAUDE */  /* REBUG */  /* asm: loc_B580F: push [_map_plane] / push [bp+scsv2] / push [bp+scsv1] / call j_Check_Square_Scouted */
            )
        )
        {

            Play_Sound(sound_buffer, sound_buffer_size);

            scsv4 = scsv1;
            scsv5 = scsv2;

            World_To_Screen(_map_x, _map_y, &scsv4, &scsv5);

            _map_plane = _UNITS[troops[0]].wp;

            Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

            Cast_Plane_Shift_Draw(scsv1, scsv2, _map_plane);

            if(SND_SpellCast != (SAMB_ptr)ST_UNDEFINED)
            {
                Play_Sound(SND_SpellCast, SND_SpellCast_size);
            }

            for(itr = 100; itr >= 0; itr -= 5)
            {

                Copy_Bitmap_To_Bitmap(bitmap, spell_animation_seg);

                Set_Page_Off();

                Reset_Map_Draw();

                Main_Screen_Draw();

                Vanish_Bitmap(bitmap, itr);

                Draw_Picture(scsv4, scsv5, bitmap);

                Apply_Palette();

                PageFlip_FX();

            }

            Play_Sound(sound_buffer, sound_buffer_size);

        }

    }

    /* OGBUG  Mark_Block(_screen_seg) happens only in the setup branch (the human, or an AI cast with Cast_Plane_Shift_Check() == 0 on a square the human has scouted); an AI cast elsewhere, or a cast with no target (return_value != ST_TRUE), reaches this release with no mark, rolling _screen_seg back to whatever older mark it holds */
    Release_Block(_screen_seg);

    _map_x = IDK_map_x;
    _map_y = IDK_map_y;
    _map_plane = IDK_map_p;

    return return_value;

}


// WZD o132p11
// drake178: WIZ_GreatWasting()
/*
Great Wasting, each turn (Next_Turn_Proc()): 3..5 attempts, each trying up to 50 random squares, to corrupt one land square
that is not within 2 squares of one of the caster's own cities and is not already corrupted
NOTE:  an attempt counts even when all 50 tries fail
*/
void Apply_Great_Wasting(int16_t player_idx)
{
    int16_t squares_corrupted = 0;
    int16_t squares_to_corrupt = 0;
    int16_t tries = 0;
    int16_t wx = 0;
    int16_t wy = 0;
    int16_t wp = 0;
    uint8_t * niu_array_1 = NULL;  /* OGBUG */  /* allocated and never used */
    uint8_t * niu_array_2 = NULL;  /* OGBUG */  /* allocated and never used */
    int16_t itr_cities = 0;  // _SI_
    int16_t square_ok = 0;  // _DI_
    niu_array_1 = Near_Allocate_First(50);
    niu_array_2 = Near_Allocate_Next(50);
    squares_to_corrupt = Random(3) + 3;
    squares_corrupted = 0;
    while(squares_corrupted < squares_to_corrupt)
    {
        tries = 0;
        square_ok = ST_FALSE;
        while(square_ok == ST_FALSE && tries < 50)
        {
            tries++;
            square_ok = ST_TRUE;
            wx = Random(WORLD_WIDTH) - 1;
            wy = Random(WORLD_HEIGHT) - 1;
            wp = Random(NUM_PLANES) - 1;
            for(itr_cities = 0; itr_cities < _cities; itr_cities++)
            {
                if(_CITIES[itr_cities].wp == wp && _CITIES[itr_cities].owner_idx == player_idx)
                {
                    if(Delta_XY_With_Wrap(_CITIES[itr_cities].wx, _CITIES[itr_cities].wy, wx, wy, WORLD_WIDTH) <= 2)
                    {
                        square_ok = ST_FALSE;
                    }
                }
            }
            if(Square_Is_Sailable(wx, wy, wp) != ST_FALSE)
            {
                square_ok = ST_FALSE;
            }
            if(Square_Has_Corruption(wx, wy, wp) != ST_FALSE)
            {
                square_ok = ST_FALSE;
            }
            if(square_ok == ST_TRUE)
            {
                _map_square_flags[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] |= MSF_CORRUPTION;
            }
        }
        squares_corrupted++;
    }
}


// WZD o132p12
// drake178: CTY_GaiasBlessing()
/*
Gaia's Blessing, each turn (Apply_City_Changes()): over the 5x5 squares around the city,
    a desert becomes grasslands (10%), a volcano becomes hills (10%), corruption is cleared (20%)
*/
void Apply_Gaias_Blessing(int16_t city_idx)
{
    int16_t wp = 0;
    int16_t square_wx = 0;
    int16_t itr_wy = 0;  // _SI_
    int16_t itr_wx = 0;  // _DI_

    wp = _CITIES[city_idx].wp;

    for(itr_wx = (_CITIES[city_idx].wx - 2); itr_wx <= (_CITIES[city_idx].wx + 2); itr_wx++)
    {

        if(itr_wx < 0)
        {

            square_wx = (itr_wx + WORLD_WIDTH);

        }
        /* OGBUG  > where >= was meant (asm: cmp di, 60 / jle): for a city at wx 58 or 59, the column 60 is not wrapped, so square (60, wy) - in the map arrays, (0, wy + 1) - is changed instead of (0, wy) */
        else if(itr_wx > WORLD_WIDTH)
        {

            square_wx = (itr_wx - WORLD_WIDTH);

        }
        else
        {

            square_wx = itr_wx;

        }

        for(itr_wy = (_CITIES[city_idx].wy - 2); itr_wy <= (_CITIES[city_idx].wy + 2); itr_wy++)
        {

            if(
                (itr_wy < 0)
                ||
                (itr_wy >= WORLD_HEIGHT)
            )
            {

                continue;

            }

            if(
                (Square_Is_Desert(square_wx, itr_wy, wp) != ST_FALSE)
                &&
                (Random(100) <= 10)
            )
            {

                Set_Terrain_Type_Grasslands(square_wx, itr_wy, wp);

            }

            /* OGBUG  Square_Is_Volcano() is also true for tt_ChaosNode (asm: cmp cx, tt_ChaosNode / jz), so a Chaos Node square in the city area can become ordinary hills; the node record is not touched */
            if(
                (Square_Is_Volcano(square_wx, itr_wy, wp) != ST_FALSE)
                &&
                (Random(100) <= 10)
            )
            {

                Set_Terrain_Type_Hills(square_wx, itr_wy, wp);

            }

            if(
                (Square_Has_Corruption(square_wx, itr_wy, wp) != ST_FALSE)
                &&
                (Random(100) <= 20)
            )
            {

                Square_Clear_Corruption(square_wx, itr_wy, wp);

            }

        }

    }

}


// WZD o132p13
// drake178: WIZ_Armageddon()
/*
Armageddon, each turn (Next_Turn_Proc()): 3..5 attempts, each trying up to 50 random squares, to raise a volcano (owned by the caster)
on a land square that is not a volcano, river or magic node, and is not within 2 squares of one of the caster's own cities
NOTE:  an attempt counts even when all 50 tries fail
*/
void Apply_Armageddon(int16_t player_idx)
{
    int16_t volcanoes_created = 0;
    int16_t volcanoes_to_create = 0;
    int16_t tries = 0;
    int16_t wx = 0;
    int16_t wy = 0;
    int16_t wp = 0;
    int16_t terrain_type = 0;
    int16_t square_ok = 0;  // _SI_
    int16_t itr_cities = 0;  // _DI_
    volcanoes_to_create = Random(3) + 3;
    volcanoes_created = 0;
    while(volcanoes_created < volcanoes_to_create)
    {
        tries = 0;
        square_ok = ST_FALSE;
        while(square_ok == ST_FALSE && tries < 50)
        {
            tries++;
            square_ok = ST_TRUE;
            wx = Random(WORLD_WIDTH) - 1;
            wy = Random(WORLD_HEIGHT) - 1;
            wp = Random(NUM_PLANES) - 1;
            if(Square_Is_Sailable(wx, wy, wp) != ST_FALSE)
            {
                square_ok = ST_FALSE;
            }
            if(Square_Is_Volcano(wx, wy, wp) != ST_FALSE)
            {
                square_ok = ST_FALSE;
            }
            if(Square_Is_River(wx, wy, wp) != ST_FALSE)
            {
                square_ok = ST_FALSE;
            }
            terrain_type = (p_world_map[wp][wy][wx] % NUM_TERRAIN_TYPES);
            if(terrain_type == tt_ChaosNode || terrain_type == tt_SorceryNode || terrain_type == tt_NatureNode)
            {
                square_ok = ST_FALSE;
            }
            for(itr_cities = 0; itr_cities < _cities; itr_cities++)
            {
                if(_CITIES[itr_cities].wp == wp && _CITIES[itr_cities].owner_idx == player_idx)
                {
                    if(Delta_XY_With_Wrap(_CITIES[itr_cities].wx, _CITIES[itr_cities].wy, wx, wy, WORLD_WIDTH) <= 2)
                    {
                        square_ok = ST_FALSE;
                    }
                }
            }
            if(square_ok == ST_TRUE)
            {
                Set_Terrain_Type_Volcano_With_Owner(wx, wy, wp, player_idx);
            }
        }
        volcanoes_created++;
    }
    Volcano_Counts();
}


// WZD o132p14
// drake178: sub_B5D8E()
/*

*/
void Cast_Floating_Island(int16_t player_idx)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t troop_count = 0;
    int16_t valid_target = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t wp = 0;
    int16_t wy = 0;
    int16_t wx = 0;
    int16_t valid_cast = 0;  // _DI_

    if(player_idx != HUMAN_PLAYER_IDX)
    {

        valid_cast = Get_Map_Square_Target_For_Spell(stt_Map_Square, &wx, &wy, &wp, spl_Floating_Island, player_idx);  // returns {F,T} got target?

    }
    else
    {

        valid_target = ST_FALSE;

        valid_cast = ST_TRUE;

        while((valid_target == ST_FALSE) && (valid_cast == ST_TRUE))
        {

            // if(valid_cast = IDK_Spell_Casting_Screen_s5C500(4, &wx, &wy, &wp, &var_8, &var_A, aFloatingIsla_0) == ST_TRUE)
            /* SPELLY */  if((valid_cast = Spell_Casting_Screen(stt_Map_Square, &wx, &wy, &wp, &target_wx, &target_wy, "Floating Island")) == ST_TRUE)
            {

                valid_target = ST_TRUE;

                if(Square_Is_Sailable(wx, wy, wp) == ST_FALSE)
                {

                    valid_target = ST_FALSE;

                    LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 58, 1, 150);

                    Warn0(GUI_NearMsgString);

                }

                Army_At_Square_1(wx, wy, wp, &troop_count, &troops[0]);

                if(troop_count > 0)
                {

                    if(_UNITS[troops[0]].owner_idx != player_idx)
                    {

                        valid_target = ST_FALSE;

                        LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 59, 1, 150);

                        Warn0(GUI_NearMsgString);

                    }
                    else
                    {

                        if(troop_count == MAX_STACK)
                        {

                            valid_target = ST_FALSE;

                            LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 60, 1, 150);

                            Warn0(GUI_NearMsgString);

                        }

                    }

                }

            }

        }

    }

    if(valid_cast != ST_TRUE)
    {
        return;
    }

    if(
        (
            (player_idx == HUMAN_PLAYER_IDX)
            ||
            (Check_Square_Scouted(wx, wy, wp) != ST_FALSE)
        )
        &&
        (magic_set.spell_animations == ST_TRUE)
    )
    {
        
        g_ai_recompute_needed = ST_TRUE;

        // DOMSDOS  Allocate_Reduced_Map__1();
        Allocate_Reduced_Map();

        Mark_Block(_screen_seg);

        Spell_Animation_Load_Graphics(spl_Floating_Island);

        Spell_Animation_Load_Sound_Effect__WIP(spl_Floating_Island);

        Spell_Animation_Screen__WIP(wx, wy, wp);

        Full_Draw_Main_Screen();

        Release_Block(_screen_seg);

    }

    Create_Unit(spell_data_table[spl_Floating_Island].unit_type, player_idx, wx, wy, wp, ST_UNDEFINED);
   
}


// WZD o132p15
// drake178: CTY_NightshadeDispel()
/*
Nightshade, each turn (Apply_City_Changes()): every enchantment on the city not cast by its owner may be dispelled
    chance in 250:  (nightshade_count x 25000) / (Calculate_Dispel_Difficulty() + nightshade_count x 100)
*/
void Apply_Nightshade_Dispel(int16_t city_idx)
{
    int16_t dispel_chance = 0;
    int16_t caster_idx = 0;
    int8_t * city_enchantments = NULL;
    int16_t spell_idx = 0;
    int16_t itr = 0;  // _SI_

    city_enchantments = &_CITIES[city_idx].enchantments[0];

    /* OGBUG  loops all NUM_CITY_ENCHANTMENTS slots, including enchantments[NIGHTSHADE], which holds a count of Nightshade squares, not an owner + 1: unless the count equals owner_idx + 1, it is treated as an enemy enchantment of player count - 1, Get_Spell_For_City_Enchantment() returns spl_NONE, and the difficulty is read from spell_data_table[0] - a dispelled count is put back by All_City_Nightshade_Count() later in Next_Turn_Calc() */
    for(itr = 0; itr < NUM_CITY_ENCHANTMENTS; itr++)
    {

        if(
            (city_enchantments[itr] > 0)
            &&
            (city_enchantments[itr] != (_CITIES[city_idx].owner_idx + 1))
        )
        {

            spell_idx = Get_Spell_For_City_Enchantment(itr);

            caster_idx = (city_enchantments[itr] - 1);

            dispel_chance = (Calculate_Dispel_Difficulty(spell_data_table[spell_idx].casting_cost, caster_idx, spell_data_table[spell_idx].magic_realm) + (_CITIES[city_idx].enchantments[NIGHTSHADE] * 100));

            dispel_chance = ((_CITIES[city_idx].enchantments[NIGHTSHADE] * 25000) / dispel_chance);

            if(Random(250) <= dispel_chance)
            {

                city_enchantments[itr] = 0;

            }

        }

    }

}


// WZD o132p16
int16_t Cast_Lycantrophy(int16_t player_idx)
{
    int16_t status = 0;
    int16_t scsv5 = 0;
    int16_t scsv4 = 0;
    int16_t scsv3 = 0;
    int16_t scsv2 = 0;
    int16_t unit_idx = 0;
    int16_t return_value = 0;  // _SI_

    Allocate_Reduced_Map();

    Mark_Block(_screen_seg);

    if(player_idx != HUMAN_PLAYER_IDX)
    {

        return_value = AITP_Unit_Enchantment(stt_Friendly_Unit, &unit_idx, spl_Lycanthropy, player_idx);

    }
    else
    {

        return_value = ST_TRUE;

        status = ST_FALSE;

        while((return_value == ST_TRUE) && (status == ST_FALSE))
        {
            
            return_value = Spell_Casting_Screen(stt_Friendly_Unit, &unit_idx, &scsv2, &scsv3, &scsv4, &scsv5, aLycanthropy);

            if(return_value == ST_TRUE)
            {

                if(_UNITS[unit_idx].type < ut_Trireme)
                {

                    Full_Draw_Main_Screen();

                    LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 62, 1, 150);

                    Warn0(GUI_NearMsgString);

                }
                else if((_unit_type_table[_UNITS[unit_idx].type].Abilities & UA_FANTASTIC) != 0)
                {
                    
                    Full_Draw_Main_Screen();

                    LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 63, 1, 150);

                    Warn0(GUI_NearMsgString);

                }
                else if((_UNITS[unit_idx].mutations & (CC_ARMOR | CC_ARMOR | CC_BREATH | UM_UNDEAD)) != 0)
                {

                    Full_Draw_Main_Screen();

                    LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 64, 1, 150);

                    Warn0(GUI_NearMsgString);

                }
                else
                {

                    status = ST_TRUE;

                }

            }

        }

        if(return_value == ST_TRUE)
        {

            if(
                (player_idx == HUMAN_PLAYER_IDX)
                ||
                (Check_Square_Scouted(_UNITS[unit_idx].wx, _UNITS[unit_idx].wy, _UNITS[unit_idx].wp) != ST_FALSE)
            )
            {

                Spell_Animation_Load_Sound_Effect__WIP(spl_Lycanthropy);

                Spell_Animation_Load_Graphics(spl_Lycanthropy);

                Spell_Animation_Screen__WIP(_UNITS[unit_idx].wx, _UNITS[unit_idx].wy, _UNITS[unit_idx].wp);

                Full_Draw_Main_Screen();

            }

        }

        _UNITS[unit_idx].type = (uint8_t)spell_data_table[spl_Lycanthropy].unit_type;

        _UNITS[unit_idx].XP = 0;

        _UNITS[unit_idx].Level = 0;

    }

    Release_Block(_screen_seg);

    return return_value;

}

// WZD o132p17
int16_t Cast_Wall_Of_Stone(int16_t player_idx)
{
    int16_t status = 0;
    int16_t return_value = 0;
    int16_t target_wy = 0;
    int16_t target_wx = 0;
    int16_t wp = 0;
    int16_t wy = 0;
    int16_t city_idx = 0;

    Allocate_Reduced_Map();

    Mark_Block(_screen_seg);

    if(player_idx != HUMAN_PLAYER_IDX)
    {

        return_value = AITP_City_Enchantment(stt_Friendly_City, &city_idx, spl_Wall_Of_Stone, player_idx);

    }
    else
    {

        status = ST_FALSE;

        return_value = ST_TRUE;

        while((status == ST_FALSE) && (return_value == ST_TRUE))
        {

            return_value = Spell_Casting_Screen(stt_Friendly_City, &city_idx, &wy, &wp, &target_wx, &target_wy, aWallOfStone);

            if(return_value == ST_TRUE)
            {

                if(_CITIES[city_idx].bldg_status[bt_CityWalls] > bs_Replaced)
                {

                    LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 65, 1, 150);

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

        if(
            (player_idx == HUMAN_PLAYER_IDX)
            ||
            (
                (magic_set.enemy_spells == ST_TRUE)
                &&
                (SQUARE_EXPLORED(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp) != ST_FALSE)
                &&
                (_players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] > ST_FALSE)
            )
        )
        {

            City_Enchantment_Anim_1__WIP(city_idx, spl_Wall_Of_Stone, player_idx);

        }

        _CITIES[city_idx].bldg_status[bt_CityWalls] = bs_Built;

        if(
            (player_idx == HUMAN_PLAYER_IDX)
            ||
            (
                (magic_set.enemy_spells == ST_TRUE)
                &&
                (SQUARE_EXPLORED(_CITIES[city_idx].wx, _CITIES[city_idx].wy, _CITIES[city_idx].wp) != ST_FALSE)
                &&
                (_players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] > ST_FALSE)
            )
        )
        {

            City_Enchantment_Anim_2__WIP(city_idx, spl_Wall_Of_Stone, player_idx);

        }

    }

    Release_Block(_screen_seg);

    return return_value;

}

// WZD o132p18
int16_t Cast_Spell_Of_Return(int16_t player_idx)
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

        do {
            return_value = Spell_Casting_Screen(stt_Friendly_City, &scsv1, &scsv2, &scsv3, &scsv4, &scsv5, aSpellOfReturn);  // "Spell of Return"
        } while(return_value == ST_FALSE);

    }
    else
    {

        return_value = AITP_City_Enchantment(stt_Friendly_City, &scsv1, spl_Move_Fortress, player_idx);

    }

    if(return_value == ST_TRUE)
    {
        City_Enchantment_Anim_1__WIP(scsv1, spl_Spell_Of_Return, player_idx);
        _players[player_idx].summon_wx = _CITIES[scsv1].wx;
        _players[player_idx].summon_wy = _CITIES[scsv1].wy;
        _players[player_idx].summon_wp = _CITIES[scsv1].wp;
        _FORTRESSES[player_idx].wx = _CITIES[scsv1].wx;
        _FORTRESSES[player_idx].wy = _CITIES[scsv1].wy;
        _FORTRESSES[player_idx].wp = _CITIES[scsv1].wp;
        _FORTRESSES[player_idx].active = ST_TRUE;
        _players[player_idx].capital_race = _CITIES[scsv1].race;
        City_Enchantment_Anim_2__WIP(scsv1, spl_Spell_Of_Return, player_idx);
    }

    Release_Block(_screen_seg);

    return return_value;

}
