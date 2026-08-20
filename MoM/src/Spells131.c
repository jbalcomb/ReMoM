/*
    WIZARDS.EXE
        ovr131
*/

#include "../../STU/src/STU_DBG.h"

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/GENDRAW.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/Timer.h"
#include "../../MoX/src/Util.h"
#include "../../MoX/src/Video.h"
#include "../../MoX/src/random.h"

#include "CITYCALC.h"
#include "CMBTDEF.h"
#include "Combat.h"
#include "COMBINIT.h"
#include "OverSpel.h"
#include "SBookScr.h"
#include "Spells133.h"
#include "UNITTYPE.h"
#include "RACETYPE.h"
#include "Spellbook.h"

#include "../../ext/stu_compat.h"

#include "Spells131.h"



// WZD dseg:6844                                                 BEGIN:  ovr131 - Initialized Data

/* HACK */  char str_empty_string__ovr131[] = "";

// WZD dseg:6844
char specfx_lbx_file__ovr131__1of2[] = "SPECFX";
// WZD dseg:684B
char cnst_Dispel_Msg[] = " has been dispelled.";
// WZD dseg:6860
char message_lbx_file__ovr131[] = "message";
// WZD dseg:6868
char specfx_lbx_file__ovr131__2of2[] = "specfx";
// WZD dseg:686F
char cnst_RaiseDead_Msg[] = "Select a unit to Raise";
// WZD dseg:6886
char cnst_AnimDead_Msg[] = "Select a unit to Animate";

// WZD dseg:689F 00                                              align 2

// WZD dseg:6886                                                 END:  ovr131 - Initialized Data




/*
    WIZARDS.EXE  ovr131
*/

// WZD o131p01
void Combat_Cast_Disenchant(int16_t caster_idx, int16_t strength)
{
    int16_t disench_spell_by_slot[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t enemy_player_idx = 0;
    int16_t player_idx = 0;
    SAMB_ptr notify_background_seg = 0;
    int16_t notify_count = 0;
    int8_t * ptr_enchantments = 0;
    int16_t spell_idx = 0;
    int16_t itr_battle_units = 0;
    int16_t side = 0;
    int16_t city_enchantment_idx = 0;
    int16_t battle_unit_idx = 0;
    int16_t itr_nodes = 0;
    int16_t itr_vortexes = 0;
    int16_t enchantment_slot = 0; 
    int16_t threshold = 0;
    disench_spell_by_slot[0] = spl_True_Light;
    disench_spell_by_slot[1] = spl_Darkness;
    disench_spell_by_slot[2] = spl_Warp_Reality;
    disench_spell_by_slot[3] = spl_Black_Prayer;
    disench_spell_by_slot[4] = spl_Wrack;
    disench_spell_by_slot[5] = spl_Metal_Fires;
    disench_spell_by_slot[6] = spl_Prayer;
    disench_spell_by_slot[7] = spl_High_Prayer;
    disench_spell_by_slot[8] = spl_Terror;
    disench_spell_by_slot[9] = spl_Call_Lightning;
    disench_spell_by_slot[10] = spl_Counter_Magic;
    disench_spell_by_slot[11] = spl_Mass_Invisibility;
    disench_spell_by_slot[12] = spl_Entangle;
    disench_spell_by_slot[13] = spl_Mana_Leak;
    disench_spell_by_slot[14] = spl_Blur;
    if(caster_idx >= CASTER_IDX_BASE)
    {
        player_idx = (caster_idx - CASTER_IDX_BASE);
    }
    else
    {
        player_idx = battle_units[battle_unit_idx].controller_idx;  /* OGBUG: not a player_idx, battle_unit_idx is not even initialized here; same as in Combat_Cast_Dispel() */
    }
    notify_count = 0;
    Copy_On_To_Off_Page();
    if(player_idx == _combat_attacker_player)
    {
        side = 1;
        enemy_player_idx = _combat_defender_player;
    }
    else
    {
        side = 0;
        enemy_player_idx = _combat_attacker_player;
    }
    /* OGBUG: neutral units can cast Combat Enchantments too */
    if(enemy_player_idx != NEUTRAL_PLAYER_IDX)
    {
        /* 1 pair per iteration */
        for(enchantment_slot = 0; enchantment_slot < (15 * 2); enchantment_slot += 2)
        {
            if(combat_enchantments[(enchantment_slot + side)] > 0)
            {
                spell_idx = disench_spell_by_slot[(enchantment_slot / 2)];  // map 30 to 15
                threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[spell_idx].casting_cost, enemy_player_idx, spell_data_table[spell_idx].magic_realm));
                threshold = (int16_t)(((int32_t)250 * strength) / threshold);
                if(Random(250) <= threshold)
                {
                    if(enchantment_slot == MASS_INVISIBILITY_ATTKR)
                    {
                        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
                        {
                            if(battle_units[itr_battle_units].controller_idx == enemy_player_idx)
                            {
                                battle_units[itr_battle_units].enchantments ^= UE_INVISIBILITY;
                            }
                        }
                    }
                    combat_enchantments[(enchantment_slot + side)] = 0;
                    Mark_Block(_screen_seg);
                    // SPECFX.LBX, 052  "DISPELL2"  ""
                    notify_background_seg = LBX_Reload_Next(specfx_lbx_file__ovr131__1of2, 52, _screen_seg);
                    if(notify_count < 5)
                    {
                        if(notify_count < 4)
                        {
                            _fstrcpy(GUI_NearMsgString, spell_data_table[spell_idx].name);
                            stu_strcat(GUI_NearMsgString, cnst_Dispel_Msg);  // " has been dispelled."
                        }
                        else
                        {
                            LBX_Load_Data_Static(message_lbx_file__ovr131, 0, (SAMB_ptr)&GUI_NearMsgString[0], 45, 1, 150);
                        }
                        Notify2(160, (40 + (notify_count * 25)), 3, GUI_NearMsgString, 0, notify_background_seg, 0, 8, 0, 0, 0, 1, 0);
                        notify_count += 1;
                    }
                    Release_Block(_screen_seg);
                }
            }
        }
    }
    /* OGBUG: dispels the node silently, should call Notify2 */
    for(itr_nodes = 0; itr_nodes < NUM_NODES; itr_nodes++)
    {
        if(
            ((_NODES[itr_nodes].flags & NF_WARPED) > 0)
            &&
            (_NODES[itr_nodes].owner_idx == player_idx)
            &&
            (_NODES[itr_nodes].wx == _combat_wx)
            &&
            (_NODES[itr_nodes].wy == _combat_wy)
            &&
            (_NODES[itr_nodes].wp == _combat_wp)
        )
        {
            spell_idx = spl_Warp_Node;
            threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[spell_idx].casting_cost, ST_UNDEFINED, spell_data_table[spell_idx].magic_realm));
            threshold = (int16_t)(((int32_t)250 * strength) / threshold);
            if(Random(250) <= threshold)
            {
                _NODES[itr_nodes].flags ^= NF_WARPED;
            }
        }
    }
    /* OGBUG: a vortex sharing a square with an active unit is rolled against twice — once here, once via the Combat_Cast_Dispel pass at the end */
    /* OGBUG: dispels the vortex silently, should call Notify2 */
    for(itr_vortexes = 0; itr_vortexes < _vortex_count; itr_vortexes++)
    {
        if(_vortexes[itr_vortexes].owner_idx != player_idx)
        {
            spell_idx = spl_Magic_Vortex;
            threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[spell_idx].casting_cost, _vortexes[itr_vortexes].owner_idx, spell_data_table[spell_idx].magic_realm));
            threshold = (int16_t)(((int32_t)250 * strength) / threshold);
            if(Random(250) <= threshold)
            {
                Delete_Structure(itr_vortexes, (uint8_t *)&_vortexes[0], sizeof(struct s_MAGIC_VORTEX), _vortex_count);
                itr_vortexes--;
                _vortex_count--;
            }
        }
    }
    /* OGBUG: reads only battlefield->city_enchantments, so combat-cast Wall spells are ignored */
    /* OGBUG: prices Wall of Fire and Wall of Darkness at their city-enchantment cost, not the overland cost that placed them */
    ptr_enchantments = (int8_t *)&battlefield->city_enchantments;
    /* OGBUG: NUM_CITY_ENCHANTMENTS is 26 and slot 25 is Nightshade, which is not an enchantment — Get_Spell_For_City_Enchantment returns spl_NONE for it and the difficulty is then read from spell_data_table[0] */
    for(city_enchantment_idx = 0; city_enchantment_idx < NUM_CITY_ENCHANTMENTS; city_enchantment_idx++)
    {
        if(
            (ptr_enchantments[city_enchantment_idx] > 0)
            &&
            (ptr_enchantments[city_enchantment_idx] != (player_idx + 1))
        )
        {
            spell_idx = Get_Spell_For_City_Enchantment(city_enchantment_idx);
            threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[spell_idx].casting_cost, (ptr_enchantments[city_enchantment_idx] - 1), spell_data_table[spell_idx].magic_realm));
            threshold = (int16_t)(((int32_t)250 * strength) / threshold);
            if(Random(250) <= threshold)
            {
                ptr_enchantments[city_enchantment_idx] = 0;
                Mark_Block(_screen_seg);
                // SPECFX.LBX, 052  "DISPELL2"  ""
                notify_background_seg = LBX_Reload_Next(specfx_lbx_file__ovr131__2of2, 52, _screen_seg);
                if(spell_idx == spl_Wall_Of_Fire)
                {
                    battlefield->wall_of_fire = 0;
                }
                if(spell_idx == spl_Wall_Of_Darkness)
                {
                    battlefield->wall_of_darkness = 0;
                }
                if(notify_count < 5)
                {
                    if(notify_count < 4)
                    {
                        _fstrcpy(GUI_NearMsgString, spell_data_table[spell_idx].name);
                        stu_strcat(GUI_NearMsgString, cnst_Dispel_Msg);  // " has been dispelled."
                    }
                    else
                    {
                        LBX_Load_Data_Static(message_lbx_file__ovr131, 0, (SAMB_ptr)&GUI_NearMsgString[0], 46, 1, 150);
                    }
                    Notify2(160, (40 + (notify_count * 25)), 3, GUI_NearMsgString, 0, notify_background_seg, 0, 8, 0, 0, 0, 1, 0);
                    notify_count += 1;
                }
                Release_Block(_screen_seg);
            }
        }
    }
    notify_count = 0;
    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        if(battle_units[battle_unit_idx].status == bus_Active)
        {
            Combat_Cast_Dispel(battle_units[battle_unit_idx].cgx, battle_units[battle_unit_idx].cgy, caster_idx, strength, &notify_count);
        }
    }
}


// WZD o131p02
/*
    'Dispel Magic' & 'Dispel Magic True'
    in-outs a count of magics dispelled
*/
void Combat_Cast_Dispel(int16_t cgx, int16_t cgy, int16_t caster_idx, int16_t strength, int16_t * notify_count)
{
    SAMB_ptr notify_background_seg = 0;
    int16_t enemy_player_idx = 0;
    int16_t threshold = 0;
    int16_t combat_effect_bit = 0;
    int32_t enchantment_bit = 0;
    int16_t bit_idx = 0;
    int16_t itr = 0;
    int16_t player_idx = 0;
    int16_t battle_unit_idx = 0;
    Near_Allocate_Mark();
    dispel_enchantment_bits = (int16_t *)Near_Allocate_Next(64);
    dispel_combat_effect_bits = (int16_t *)Near_Allocate_Next(30);
    dispel_enchantment_bits[0] = spl_Immolation;
    dispel_combat_effect_bits[0] = spl_Vertigo;
    dispel_enchantment_bits[1] = spl_Guardian_Wind;
    dispel_combat_effect_bits[1] = spl_Confusion;
    dispel_enchantment_bits[2] = spl_Berserk;
    dispel_combat_effect_bits[2] = spl_NONE;
    dispel_enchantment_bits[3] = spl_Cloak_Of_Fear;
    dispel_combat_effect_bits[3] = spl_Mind_Storm;
    dispel_enchantment_bits[4] = spl_Black_Channels;
    dispel_combat_effect_bits[4] = spl_Shatter;
    dispel_enchantment_bits[5] = spl_Wraith_Form;
    dispel_combat_effect_bits[5] = spl_Weakness;
    dispel_enchantment_bits[6] = spl_Regeneration;
    dispel_combat_effect_bits[6] = spl_Black_Sleep;
    dispel_enchantment_bits[7] = spl_Path_Finding;
    dispel_combat_effect_bits[7] = spl_Warp_Creature;
    dispel_enchantment_bits[8] = spl_Water_Walking;
    dispel_combat_effect_bits[8] = spl_Warp_Creature;
    dispel_enchantment_bits[9] = spl_Resist_Elements;
    dispel_combat_effect_bits[9] = spl_Warp_Creature;
    dispel_enchantment_bits[10] = spl_Elemental_Armor;
    dispel_combat_effect_bits[10] = spl_NONE;
    dispel_enchantment_bits[11] = spl_Stone_Skin;
    dispel_combat_effect_bits[11] = spl_Haste;
    dispel_enchantment_bits[12] = spl_Iron_Skin;
    dispel_combat_effect_bits[12] = spl_Web;
    dispel_enchantment_bits[13] = spl_Endurance;
    dispel_combat_effect_bits[13] = spl_Creature_Binding;
    dispel_enchantment_bits[14] = spl_Spell_Lock;
    dispel_enchantment_bits[15] = spl_Invisibility;
    dispel_enchantment_bits[16] = spl_Wind_Walking;
    dispel_enchantment_bits[17] = spl_Flight;
    dispel_enchantment_bits[18] = spl_Resist_Magic;
    dispel_enchantment_bits[19] = spl_Magic_Immunity;
    dispel_enchantment_bits[20] = spl_Flame_Blade;
    dispel_enchantment_bits[21] = spl_Eldritch_Weapon;
    dispel_enchantment_bits[22] = spl_True_Sight;
    dispel_enchantment_bits[23] = spl_Holy_Weapon;
    dispel_enchantment_bits[24] = spl_Heroism;
    dispel_enchantment_bits[25] = spl_Bless;
    dispel_enchantment_bits[26] = spl_Lionheart;
    dispel_enchantment_bits[27] = spl_Giant_Strength;
    dispel_enchantment_bits[28] = spl_Planar_Travel;
    dispel_enchantment_bits[29] = spl_Holy_Armor;
    dispel_enchantment_bits[30] = spl_Righteousness;
    dispel_enchantment_bits[31] = spl_Invulnerability;
    Copy_On_To_Off_Page();
    if(caster_idx >= CASTER_IDX_BASE)
    {
        player_idx = (caster_idx - CASTER_IDX_BASE);
    }
    else
    {
        player_idx = battle_units[battle_unit_idx].controller_idx;  /* OGBUG: not a player_idx, battle_unit_idx is not even initialized here; same as in Combat_Cast_Disenchant() */
    }
    if(player_idx == _combat_attacker_player)
    {
        enemy_player_idx = _combat_defender_player;
    }
    else
    {
        enemy_player_idx = _combat_attacker_player;
    }
    /* Dispel Magic Vortexes from the Combat Grid Cell */
    for(itr = 0; itr < _vortex_count; itr++)
    {
        if(_vortexes[itr].cgx == cgx && _vortexes[itr].cgy == cgy)
        {
            threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[spl_Magic_Vortex].casting_cost, _vortexes[itr].owner_idx, spell_data_table[spl_Magic_Vortex].magic_realm));
            threshold = (int16_t)(((int32_t)250 * strength) / threshold);
            if(Random(250) <= threshold)
            {
                Delete_Structure(itr, (uint8_t *)&_vortexes[0], sizeof(struct s_MAGIC_VORTEX), _vortex_count);
                itr--;
                _vortex_count--;
            }
        }
    }
    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        if(
            (battle_units[battle_unit_idx].cgx == cgx)
            &&
            (battle_units[battle_unit_idx].cgy == cgy)
        )
        {
            /* OGBUG: matches on square and owner only, so dead, fled, recalled, and uninvolved units are dispelled too */
            if(battle_units[battle_unit_idx].controller_idx == player_idx)
            {
                /* OGBUG: walks all 14 combat-effect bits, so it dispels Possession and Creature Binding as well as curses */
                for(bit_idx = 0; bit_idx < 14; bit_idx++)
                {
                    combat_effect_bit = (1 << bit_idx);
                    if((battle_units[battle_unit_idx].combat_effects & combat_effect_bit) > 0)  /* OGBUG: should be `!=`, not `>` */
                    {
                        if(
                            (combat_effect_bit != bue_Haste)
                            &&
                            (
                                (combat_effect_bit != bue_Confusion)
                                ||
                                (battle_units[battle_unit_idx].confusion_state != 2)
                            )
                        )
                        {
                            threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[dispel_combat_effect_bits[bit_idx]].casting_cost, enemy_player_idx, spell_data_table[dispel_combat_effect_bits[bit_idx]].magic_realm));
                            threshold = (int16_t)(((int32_t)250 * strength) / threshold);
                            /* OGBUG: should be Random(250) <= threshold, not threshold + 500 — Random returns 1..250, so this always succeeds */
                            if(Random(250) <= (threshold + 500))
                            {
                                if(combat_effect_bit == bue_Web)
                                {
                                    /* OGBUG: clears Web_HP but never restores the movement types Web removed */
                                    battle_units[battle_unit_idx].Web_HP = 0;
                                }
                                Mark_Block(_screen_seg);
                                // SPECFX.LBX, 051  "DISPELL1"  ""
                                notify_background_seg = LBX_Reload_Next(specfx_lbx_file__ovr131__2of2, 51, _screen_seg);
                                battle_units[battle_unit_idx].combat_effects ^= combat_effect_bit;
                                if(*notify_count < 5)
                                {
                                    if(*notify_count < 4)
                                    {
                                        _fstrcpy(GUI_NearMsgString, spell_data_table[dispel_combat_effect_bits[bit_idx]].name);
                                        stu_strcat(GUI_NearMsgString, cnst_Dispel_Msg);  // " has been dispelled."
                                    }
                                    else
                                    {
                                        LBX_Load_Data_Static(message_lbx_file__ovr131, 0, (SAMB_ptr)&GUI_NearMsgString[0], 19, 1, 150);
                                    }
                                    threshold = (160 + (*notify_count * 10));
                                    Notify2(threshold, (20 + (*notify_count * 25)), 3, GUI_NearMsgString, 0, notify_background_seg, 0, 8, 0, 0, 0, 1, 0);
                                    *notify_count += 1;
                                }
                                Release_Block(_screen_seg);
                            }
                        }
                    }
                }
            }
            else  /* (battle_units[battle_unit_idx].controller_idx != player_idx) */
            {
                /* OGBUG: should also test battle_units[].enchantments — only the overland set is checked, so combat-cast Spell Lock does not protect */
                if((_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments & UE_SPELL_LOCK) != 0)
                {
                    threshold = (strength + Calculate_Dispel_Difficulty(150, battle_units[battle_unit_idx].controller_idx, spell_data_table[spl_Spell_Lock].magic_realm));
                    threshold = (int16_t)(((int32_t)250 * strength) / threshold);
                    // ; BUG: Haste is not protected by Spell Lock
                    if(Random(250) <= threshold)
                    {
                        _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments ^= UE_SPELL_LOCK;
                    }
                }
                else
                {
                    /* OGBUG: bound should be 32, not 31 — bit 31 is never tested, so Invulnerability (UEs[31]) can never be dispelled */
                    for(bit_idx = 0; bit_idx < 31; bit_idx++)
                    {
                        enchantment_bit = (1 << bit_idx);
                        if((_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments & enchantment_bit) > 0)  /* OGBUG: should be `!=`, not `>` */
                        {
                            /* OGBUG: uses casting_cost * 5 as a stand-in for spells with no combat cost, which overprices some and underprices others */
                            threshold = (strength + Calculate_Dispel_Difficulty((spell_data_table[dispel_enchantment_bits[bit_idx]].casting_cost * 5), battle_units[battle_unit_idx].controller_idx, spell_data_table[dispel_enchantment_bits[bit_idx]].magic_realm));
                            threshold = (int16_t)(((int32_t)250 * strength) / threshold);
                            if(Random(250) <= threshold)
                            {
                                _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments ^= enchantment_bit;
                                if(bit_idx == BIT_POS(UE_HEROISM))
                                {
                                    _UNITS[battle_units[battle_unit_idx].unit_idx].Level = (int8_t)Calc_Unit_Level(battle_units[battle_unit_idx].unit_idx);
                                }
                                /* OGBUG: strips Flight (UEs[17]) unconditionally, removing natural flight from units that have it */
                                if(bit_idx == BIT_POS(UE_FLIGHT))
                                {
                                    battle_units[battle_unit_idx].Move_Flags ^= MV_FLYING;
                                }
                                if(*notify_count < 5)
                                {
                                    Mark_Block(_screen_seg);
                                    // SPECFX.LBX, 051  "DISPELL1"  ""
                                    notify_background_seg = LBX_Reload_Next(specfx_lbx_file__ovr131__2of2, 51, _screen_seg);
                                    if(*notify_count < 4)
                                    {
                                        _fstrcpy(GUI_NearMsgString, spell_data_table[dispel_enchantment_bits[bit_idx]].name);
                                        stu_strcat(GUI_NearMsgString, cnst_Dispel_Msg);  // " has been dispelled."
                                    }
                                    else
                                    {
                                        LBX_Load_Data_Static(message_lbx_file__ovr131, 0, (SAMB_ptr)&GUI_NearMsgString[0], 19, 1, 150);
                                    }
                                    Notify2((160 + (*notify_count * 10)), (20 + (*notify_count * 25)), 3, GUI_NearMsgString, 0, notify_background_seg, 0, 8, 0, 0, 0, 1, 0);
                                    *notify_count += 1;
                                    Release_Block(_screen_seg);
                                }
                            }
                        }
                    }
                    /* OGBUG: bound should be 32, not 31 — bit 31 is never tested, so Invulnerability (UEs[31]) can never be dispelled */
                    for(bit_idx = 0; bit_idx < 31; bit_idx++)
                    {
                        enchantment_bit = (1 << bit_idx);
                        if(enemy_player_idx == _combat_attacker_player)
                        {
                            /* OGBUG: checks the Mass Invisibility battlefield enchantment first, so a unit's own combat-cast Invisibility is never reached */
                            if(
                                (combat_enchantments[MASS_INVISIBILITY_ATTKR] > 0)
                                &&
                                (enchantment_bit == UE_INVISIBILITY)
                            )
                            {
                                continue;
                            }
                        }
                        else
                        {
                            if(
                                (combat_enchantments[MASS_INVISIBILITY_DFNDR] > 0)
                                &&
                                (enchantment_bit == UE_INVISIBILITY)
                            )
                            {
                                continue;
                            }
                        }
                        if(
                            (enchantment_bit == UE_HOLY_WEAPON)
                            &&
                            (_players[player_idx].Globals[HOLY_ARMS] > 0)
                        )
                        {
                            continue;
                        }
                        if((battle_units[battle_unit_idx].enchantments & enchantment_bit) > 0)  /* OGBUG: should `!=`, not `>` */
                        {
                            threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[dispel_enchantment_bits[bit_idx]].casting_cost, battle_units[battle_unit_idx].controller_idx, spell_data_table[dispel_enchantment_bits[bit_idx]].magic_realm));
                            threshold = (int16_t)(((int32_t)250 * strength) / threshold);
                            if(Random(250) <= threshold)
                            {
                                Mark_Block(_screen_seg);
                                // SPECFX.LBX, 051  "DISPELL1"  ""
                                notify_background_seg = LBX_Reload_Next(specfx_lbx_file__ovr131__2of2, 51, _screen_seg);
                                _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments ^= enchantment_bit;  // turn off the enchantment
                                if(bit_idx == BIT_POS(UE_HEROISM))
                                {
                                    _UNITS[battle_units[battle_unit_idx].unit_idx].Level = (int8_t)Calc_Unit_Level(battle_units[battle_unit_idx].unit_idx);
                                }
                                /* OGBUG: strips Flight (UEs[17]) unconditionally, removing natural flight from units that have it */
                                if(bit_idx == BIT_POS(UE_FLIGHT))
                                {
                                    battle_units[battle_unit_idx].Move_Flags ^= MV_FLYING;
                                }
                                if(*notify_count < 5)
                                {
                                    if(*notify_count < 4)
                                    {
                                        _fstrcpy(GUI_NearMsgString, spell_data_table[dispel_enchantment_bits[bit_idx]].name);
                                        stu_strcat(GUI_NearMsgString, cnst_Dispel_Msg);  // " has been dispelled."
                                    }
                                    else
                                    {
                                        LBX_Load_Data_Static(message_lbx_file__ovr131, 0, (SAMB_ptr)&GUI_NearMsgString[0], 19, 1, 150);
                                    }
                                    Notify2((160 + (*notify_count * 10)), (20 + (*notify_count * 25)), 3, GUI_NearMsgString, 0, notify_background_seg, 0, 8, 0, 0, 0, 1, 0);
                                    *notify_count += 1;
                                }
                                Release_Block(_screen_seg);
                            }
                        }
                    }
                }
                /*
                    BEGIN:  Haste
                */
                /* OGBUG: Haste is dispelled without a Spell Lock check, unlike every other enchantment above */
                if((battle_units[battle_unit_idx].combat_effects & bue_Haste) != 0)
                {
                    threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[spl_Haste].casting_cost, battle_units[battle_unit_idx].controller_idx, spell_data_table[spl_Haste].magic_realm));
                    threshold = (int16_t)(((int32_t)250 * strength) / threshold);
                    if(Random(250) <= threshold)
                    {
                        Mark_Block(_screen_seg);
                        // SPECFX.LBX, 051  "DISPELL1"  ""
                        notify_background_seg = LBX_Reload_Next(specfx_lbx_file__ovr131__2of2, 51, _screen_seg);
                        battle_units[battle_unit_idx].combat_effects ^= bue_Haste;
                        if(*notify_count < 5)
                        {
                            if(*notify_count < 4)
                            {
                                _fstrcpy(GUI_NearMsgString, spell_data_table[spl_Haste].name);
                                stu_strcat(GUI_NearMsgString, cnst_Dispel_Msg);  // " has been dispelled."
                            }
                            else
                            {
                                LBX_Load_Data_Static(message_lbx_file__ovr131, 0, (SAMB_ptr)&GUI_NearMsgString[0], 19, 1, 150);
                            }
                            Notify2((160 + (*notify_count * 10)), (20 + (*notify_count * 25)), 3, GUI_NearMsgString, 0, notify_background_seg, 0, 8, 0, 0, 0, 1, 0);
                            *notify_count += 1;
                        }
                        Release_Block(_screen_seg);
                    }
                }
                /*
                    END:  Haste
                */
                /*
                    BEGIN:  Confusion
                */
                if(
                    ((battle_units[battle_unit_idx].combat_effects & bue_Confusion) != 0)
                    &&
                    (battle_units[battle_unit_idx].confusion_state == 2)
                )
                {
                    threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[spl_Confusion].casting_cost, battle_units[battle_unit_idx].controller_idx, spell_data_table[spl_Confusion].magic_realm));
                    threshold = (int16_t)(((int32_t)250 * strength) / threshold);
                    if(Random(250) <= threshold)
                    {
                        Mark_Block(_screen_seg);
                        if(battle_units[battle_unit_idx].controller_idx == _combat_attacker_player)
                        {
                            battle_units[battle_unit_idx].controller_idx = (int8_t)_combat_defender_player;
                        }
                        else
                        {
                            battle_units[battle_unit_idx].controller_idx = (int8_t)_combat_attacker_player;
                        }
                        // SPECFX.LBX, 051  "DISPELL1"  ""
                        notify_background_seg = LBX_Reload_Next(specfx_lbx_file__ovr131__2of2, 51, _screen_seg);
                        /* OGBUG: clears bue_Confusion but leaves confusion_state at 2 */
                        battle_units[battle_unit_idx].combat_effects ^= bue_Confusion;
                        if(*notify_count < 5)
                        {
                            if(*notify_count < 4)
                            {
                                _fstrcpy(GUI_NearMsgString, spell_data_table[spl_Confusion].name);
                                stu_strcat(GUI_NearMsgString, cnst_Dispel_Msg);  // " has been dispelled."
                            }
                            else
                            {
                                LBX_Load_Data_Static(message_lbx_file__ovr131, 0, (SAMB_ptr)&GUI_NearMsgString[0], 19, 1, 150);
                            }
                            Notify2((160 + (*notify_count * 10)), (20 + (*notify_count * 25)), 3, GUI_NearMsgString, 0, notify_background_seg, 0, 8, 0, 0, 0, 1, 0);
                            *notify_count += 1;
                        }
                        Release_Block(_screen_seg);
                    }
                }
                /*
                    END:  Confusion
                */
            }  /* (battle_units[battle_unit_idx].controller_idx != player_idx) */
        }
    }
    Near_Allocate_Undo();
    Combat_Compose_Background();
}


// WZD o131p03
void Apply_Cracks_Call(int16_t cgx, int16_t cgy)
{
    int16_t wall_cgy = 0;
    int16_t wall_cgx = 0;
    int16_t damage_types[3] = { 0, 0, 0, };
    int16_t battle_unit_idx = 0;
    int16_t itr = 0;
    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        for(itr = 0; itr < NUM_DAMAGE_TYPES; itr++)
        {
            damage_types[itr] = 0;
        }
        if(
            (battle_units[battle_unit_idx].cgx == cgx)
            &&
            (battle_units[battle_unit_idx].cgy == cgy)
            &&
            ((battle_units[battle_unit_idx].Move_Flags & MV_FLYING) == 0)
            &&
            ((battle_units[battle_unit_idx].Abilities & UA_NONCORPOREAL) == 0)
        )
        {
            itr = Random(4);
            if(itr == 1)
            {
                damage_types[2] = 200;
                Battle_Unit_Commit_Damage(battle_unit_idx, &damage_types[0]);
            }
        }
    }
    if(Combat_Grid_Cell_Has_City_Wall(cgx, cgy) == ST_TRUE)
    {
        wall_cgx = (cgx - 5);
        wall_cgy = (cgy - 10);
        battlefield->walls[wall_cgy][wall_cgx] = 2;
    }
}


// WZD o131p04
// drake178: CMB_BattlefieldSpell()
/*
; processes the effects and animations for battlefield
; spells that affect all of a player's units at the
; same time
; BUG: if spell anims are disabled, the effects nearly
;  always won't be applied either
; BUG: GUI_CallChaos has multiple other issues too
*/
/*
OON XREF:  Combat_Spell_Animation__WIP()

Boundaries?
    each does all _combat_total_unit_count?
    only own units?
    only active units?

All Units, but not Enchantment?

calls out for Call Chaos, Death Spell, Holy Word, Flame Strike
handles in-line Mass Healing

XREF:
    Combat_Spell_Animation__WIP()
    NX_j_Combat_Battlefield_Instant()

*/
void Combat_Battlefield_Instant(int16_t player_idx, int16_t spell_idx, int16_t anims_on, int16_t caster_idx)
{
    int16_t Anim_Delay_Array[MAX_BATTLE_UNIT_SLOT_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Anim_Size = 0;
    int16_t screen_y = 0;
    int16_t screen_x = 0;
    int16_t frame_count = 0;
    int16_t itr2 = 0;
    int16_t itr1 = 0;  // _SI_

    if(spell_idx == spl_Call_Chaos)
    {

        Cast_Call_Chaos__WIP(player_idx, anims_on);

    }
    else
    {

        if(anims_on != ST_FALSE)
        {
            Mark_Block(_screen_seg);
            Spell_Animation_Load_Graphics(spell_idx);
            Combat_Load_Spell_Sound_Effect(spell_idx);
            frame_count = FLIC_Get_FrameCount(spell_animation_seg);
            if(SND_SpellCast != (SAMB_ptr)ST_UNDEFINED)
            {
                Play_Sound(SND_SpellCast, SND_SpellCast_size);
            }
            // ; randomize the animation delay array (0-4), or set it
            // ; to -1 for units that belong to the wrong player to
            // ; be affected by the spell
            for(itr1 = 0; itr1 < _combat_total_unit_count; itr1++)
            {
                if(
                    (
                        (
                            (battle_units[itr1].controller_idx == player_idx)
                            &&
                            (spell_idx == spl_Mass_Healing)
                        )
                        ||
                        (
                            (battle_units[itr1].controller_idx != player_idx)
                            &&
                            (spell_idx != spl_Mass_Healing)
                        )
                    )
                    &&
                    (battle_units[itr1].status == bus_Active)
                )
                {
                    Anim_Delay_Array[itr1] = (Random(5) - 1);
                }
                else
                {
                    Anim_Delay_Array[itr1] = ST_UNDEFINED;
                }
            }
            for(itr2 = 0; (frame_count + 4) > itr2; itr2++)
            {
                Mark_Time();
                if(((frame_count + 4) / 2) == itr2)
                {
                    if(spell_idx == spl_Death_Spell)
                    {
                        Apply_Death_Spell(player_idx);
                    }
                    if(spell_idx == spl_Holy_Word)
                    {
                        Apply_Holy_Word(player_idx);
                    }
                    if(spell_idx == spl_Flame_Strike)
                    {
                        Apply_Flame_Strike(player_idx);
                    }
                    if(spell_idx == spl_Mass_Healing)
                    {
                        if(
                        (battle_units[itr1].controller_idx == player_idx)
                        &&
                        (battle_units[itr1].status == bus_Active)
                        &&
                        (battle_units[itr1].race != rt_Death)
                        )
                        {
                            Battle_Unit_Heal(itr1, 5, 0);
                        }
                    }
                }
                Combat_Screen_Draw();
                Combat_Cast_Spell_Message(caster_idx, spell_idx);
                for(itr1 = 0; itr1 < _combat_total_unit_count; itr1++)
                {
                    if(Anim_Delay_Array[itr1] > ST_UNDEFINED)
                    {
                        Combat_Grid_Screen_Coordinates(battle_units[itr1].cgx, battle_units[itr1].cgy, 4, 4, &screen_x, &screen_y);
                        if(Anim_Size == 0)  /* Flame Strike */
                        {
                            screen_x -= 14;
                            screen_y -= 21;
                        }
                        else
                        {
                            screen_x -= 28;
                            screen_y -= 30;
                        }
                        if(
                            ((itr2 - Anim_Delay_Array[itr1]) < frame_count)
                            &&
                            ((Anim_Delay_Array[itr1] - itr2) >= 0)
                        )
                        {
                            Set_Animation_Frame(spell_animation_seg, (itr2 - Anim_Delay_Array[itr1]));
                            Clipped_Draw(screen_x, screen_y, spell_animation_seg);
                        }
                    }
                }
                PageFlip_FX();
                Release_Time(2);
            }  /* for(itr2 = 0; (frame_count + 4) > itr2; itr2++) */
            Release_Block(_screen_seg);
            Set_Page_Off();
            Combat_Screen_Draw();
            PageFlip_FX();
        }
        else  /* (anims_on == ST_FALSE) */
        {

            STU_DEBUG_BREAK();

            // ; process Death Spell, Holy Word, Flame Strike, or Mass
            // ; Healing without any animations
            // ; BUG: the application frame condition block is
            // ; repeated here, making this code branch fail nearly
            // ; every time
            if(spell_idx == spl_Death_Spell)
            {
                // ; BUG: this block should not be here
                if(((frame_count + 4) / 2) == itr2)
                {
                    Apply_Death_Spell(player_idx);
                }
            }
            if(spell_idx == spl_Holy_Word)
            {
                // ; BUG: this block should not be here
                if(((frame_count + 4) / 2) == itr2)
                {
                    Apply_Holy_Word(player_idx);
                }
            }
            if(spell_idx == spl_Flame_Strike)
            {
                // ; BUG: this block should not be here
                if(((frame_count + 4) / 2) == itr2)
                {
                    Apply_Flame_Strike(player_idx);
                }
            }
            if(spell_idx == spl_Mass_Healing)
            {
                // ; BUG: this block should not be here
                if(((frame_count + 4) / 2) == itr2)
                {
                    if(spell_idx == spl_Mass_Healing)
                    {
                        if(
                            (battle_units[itr1].controller_idx == player_idx)
                            &&
                            (battle_units[itr1].status == bus_Active)
                            &&
                            (battle_units[itr1].race != rt_Death)
                        )
                        {
                            Battle_Unit_Heal(itr1, 5, 0);
                        }
                    }
                }
            }

        }

    }

}


// WZD o131p05
void Apply_Flame_Strike(int16_t player_idx)
{
    uint32_t enchantments = 0;
    int16_t damage_types[NUM_DAMAGE_TYPES] = { 0, 0, 0 };
    int16_t figure_count = 0;
    int16_t battle_unit_idx = 0;
    int16_t itr = 0;
    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        enchantments = ( _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments | battle_units[battle_unit_idx].enchantments | battle_units[battle_unit_idx].item_enchantments);
        if(
            (battle_units[battle_unit_idx].controller_idx != player_idx)
            &&
            (battle_units[battle_unit_idx].status == bus_Active)
        )
        {
            if(
                ((enchantments & UE_WRAITH_FORM) == 0)  /* OGBUG: 'Wraith Form' should not grant immunity here */
                &&
                ((enchantments & UE_RIGHTEOUSNESS) == 0)
            )
            {
                Compute_Battle_Unit_Damage_From_Spell(spl_Flame_Strike, battle_unit_idx, &damage_types[0], ST_FALSE);
                Battle_Unit_Commit_Damage(battle_unit_idx, &damage_types[0]);    
            }
        }
    }
}


// WZD o131p06
void Apply_Holy_Word(int16_t player_idx)
{
    int16_t damage_types[NUM_DAMAGE_TYPES] = { 0, 0, 0 };
    int16_t resistance_modifier = 0;
    uint32_t enchantments = 0;
    int16_t figure_count = 0;
    int16_t battle_unit_idx = 0;
    int16_t itr = 0;
    damage_types[0] = 0;
    damage_types[1] = 0;
    damage_types[2] = 0;
    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        enchantments = ( _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments | battle_units[battle_unit_idx].enchantments | battle_units[battle_unit_idx].item_enchantments);
        if(
            (battle_units[battle_unit_idx].controller_idx != player_idx)
            &&
            (battle_units[battle_unit_idx].status == bus_Active)
        )
        {
            if(
                ((enchantments & UE_WRAITH_FORM) == 0)  /* OGBUG: 'Wraith Form' should not grant immunity here */
                &&
                ((enchantments & UE_SPELL_LOCK) == 0)
                &&
                (battle_units[battle_unit_idx].race >= rt_Arcane)
            )
            {
                resistance_modifier = -2;
                if((_UNITS[battle_units[battle_unit_idx].unit_idx].mutations & UM_UNDEAD) == 0)
                {
                    resistance_modifier += -5;
                }
                figure_count = battle_units[battle_unit_idx].figure_cnt;
                for(itr = 0; itr < figure_count; itr++)
                {
                    damage_types[dt_Doom] = 0;
                    if(Combat_Resistance_Check(battle_units[battle_unit_idx], -2, sbr_Death) > 0)
                    {
                        damage_types[dt_Doom] = battle_units[battle_unit_idx].hits;
                        Battle_Unit_Commit_Damage(battle_unit_idx, &damage_types[0]);
                    }
                }
            }
        }
    }
}


// WZD o131p07
void Apply_Death_Spell(int16_t player_idx)
{
    int16_t damage_types[NUM_DAMAGE_TYPES] = { 0, 0, 0 };
    uint32_t enchantments = 0;
    int16_t figure_count = 0;
    int16_t battle_unit_idx = 0;
    int16_t itr = 0;
    damage_types[dt_Normal] = 0;
    damage_types[dt_Drain] = 0;
    damage_types[dt_Doom] = 0;
    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        enchantments = ( _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments | battle_units[battle_unit_idx].enchantments | battle_units[battle_unit_idx].item_enchantments);
        if(
            (battle_units[battle_unit_idx].controller_idx != player_idx)
            &&
            (battle_units[battle_unit_idx].status == bus_Active)
        )
        {
            if(
                ((enchantments & UE_WRAITH_FORM) == 0)  /* OGBUG: 'Wraith Form' should not grant immunity here */
                &&
                ((battle_units[battle_unit_idx].Attribs_1 & USA_IMMUNITY_DEATH) == 0)
            )
            {
                figure_count = battle_units[battle_unit_idx].figure_cnt;
                for(itr = 0; itr < figure_count; itr++)
                {
                    damage_types[dt_Normal] = 0;
                    if(Combat_Resistance_Check(battle_units[battle_unit_idx], -2, sbr_Death) > 0)
                    {
                        damage_types[dt_Normal] = battle_units[battle_unit_idx].hits;
                        Battle_Unit_Commit_Damage(battle_unit_idx, &damage_types[0]);
                    }
                }
            }
        }
    }
}


// WZD o131p08
// drake178: CMB_PlaySpellAnim()
/*
; plays the animation for and, in the case of spells
; affecting all units of a player, also processes the
; effects of the selected spell
; inherits BUGs from GUI_CallChaos, and will nearly
; always fail fail to apply "all units" spell effects
; if combat spell anims (deprecated) are disabled
*/
/*
    OON XREF:  Combat_Cast_Apply_Spell_Effect()

XREF:
    j_CMB_PlaySpellAnim__WIP(*)
        Combat_Cast_Apply_Spell_Effect()
            Combat_Spell_Animation__WIP()
*/
void Combat_Spell_Animation__WIP(int16_t cgx, int16_t cgy, int16_t spell_idx, int16_t player_idx, int16_t anims_on, int16_t caster_idx)
{
    int16_t blue = 0;
    int16_t green = 0;
    int16_t red = 0;
    int16_t Anim_Size = 0;
    int16_t itr = 0;  // _DI_

    if(
        (spell_data_table[spell_idx].type == scc_Battlefield_Spell)
        ||
        (spell_data_table[spell_idx].type == scc_Combat_Counter_Magic)
        ||
        (spell_data_table[spell_idx].type == scc_Disenchants)
        ||
        (spell_idx == spl_Raise_Dead)
        ||
        (spell_idx == spl_Animate_Dead)
    )
    {

        if(
            (spell_idx == spl_Flame_Strike)
            ||
            (spell_idx == spl_Holy_Word)
            ||
            (spell_idx == spl_Death_Spell)
            ||
            (spell_idx == spl_Call_Chaos)
            ||
            (spell_idx == spl_Mass_Healing)
        )
        {

            Combat_Battlefield_Instant(player_idx, spell_idx, anims_on, caster_idx);

        }
        else
        {

            // shade the screen to 40% of the realm's color and
            // then back to the original, while displaying the
            // combat spell cast message (but no animation)

            if(anims_on != ST_FALSE)
            {

                switch(spell_data_table[spell_idx].magic_realm)
                {

                    case sbr_Nature:   // 0
                    {
                        red = 0;
                        green = 63;
                        blue = 0;
                    } break;
                    case sbr_Sorcery:  // 1
                    {
                        red = 0;
                        green = 0;
                        blue = 63;
                    } break;
                    case sbr_Chaos:    // 2
                    {
                        red = 63;
                        green = 0;
                        blue = 0;
                    } break;
                    case sbr_Life:     // 3
                    {
                        red = 63;
                        green = 63;
                        blue = 63;
                    } break;
                    case sbr_Death:    // 4
                    {
                        red = 0;
                        green = 0;
                        blue = 0;
                    } break;
                    case sbr_Arcane:   // 5
                    {
                        red = 63;
                        green = 63;
                        blue = 63;
                    } break;
                    default:
                    {
                        STU_DEBUG_BREAK();
                    } break;

                }

                Set_Page_Off();

                Combat_Screen_Draw();

                Combat_Cast_Spell_Message(caster_idx, spell_idx);

                PageFlip_FX();

                for(itr = 0; itr < 20; itr++)
                {

                    Set_Palette_Changes(0, 255);

                    Tint_Palette((itr * 2), red, green, blue);

                }

                for(itr = 20; itr > -1; itr--)
                {

                    Set_Palette_Changes(0, 255);

                    Tint_Palette((itr * 2), red, green, blue);

                }

            }

        }

    }
    else
    {
        // (spell_data_table[spell_idx].type != scc_Battlefield_Spell)
        // (spell_data_table[spell_idx].type != scc_Combat_Counter_Magic)
        // (spell_data_table[spell_idx].type != scc_Disenchants)
        // (spell_idx != spl_Raise_Dead)
        // (spell_idx != spl_Animate_Dead)
        // ∴
        // Fireball, Fire Bolt, Ice Bolt, Doom Bolt
        // Lightning Bolt
        // Cracks Call
        // ..., Magic Vortex, ...

        if(anims_on != ST_FALSE)
        {

            Combat_Load_Spell_Sound_Effect(spell_idx);

            if(
                (spell_idx == spl_Fireball)
                ||
                (spell_idx == spl_Fire_Bolt)
                ||
                (spell_idx == spl_Ice_Bolt)
                ||
                (spell_idx == spl_Doom_Bolt)
            )
            {

                /* SPELLY */  TILE_BoltFromAbove__WIP(cgx, cgy, spell_idx, caster_idx);

            }
            else if(spell_idx == spl_Lightning_Bolt)
            {

                Spell_Animation_Load_Graphics(spl_Call_Lightning);

                Animate_Lightning_Bolt(cgx, cgy, caster_idx);

            }
            else
            {
                // (spell_idx != spl_Fireball)
                // (spell_idx != spl_Fire_Bolt)
                // (spell_idx != spl_Ice_Bolt)
                // (spell_idx != spl_Doom_Bolt)
                // (spell_idx != spl_Lightning_Bolt)

                Mark_Block(_screen_seg);

                Anim_Size = Spell_Animation_Load_Graphics(spell_idx);

                Release_Block(_screen_seg);

                if(spell_idx == spl_Cracks_Call)
                {

                    Animate_Cracks_Call(cgx, cgy, caster_idx);

                }
                else
                {
                    // (spell_idx != spl_Fireball)
                    // (spell_idx != spl_Fire_Bolt)
                    // (spell_idx != spl_Ice_Bolt)
                    // (spell_idx != spl_Doom_Bolt)
                    // (spell_idx != spl_Lightning_Bolt)
                    // (spell_idx != spl_Cracks_Call)

                    Combat_Spell_Animation_Generic__WIP(cgx, cgy, Anim_Size, caster_idx, spell_idx);

                }

            }

            Release_Block(_screen_seg);

        }

    }

}


// WZD o131p09
// drake178: CMB_RaiseDead()
/*
; processes the casting of the Raise Dead spell, at the
; specified coordinates for the player or random ones
; for the AI; including presenting the list selection
; dialog for choosing the unit to raise, playing the
; animation, and setting the stats of the unit
; contains numerous BUGs, including several that will
; crash the game or corrupt memory
*/
/*

*/
void Cast_Raise_Dead(int16_t player_idx, int16_t caster_idx, int16_t cgx, int16_t cgy)
{
    char * Target_Name_Ptr_List[(MAX_STACK - 1)] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Target_BU_List[(MAX_STACK - 1)] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Picked_Target = 0;
    int16_t Target_Count = 0;
    int16_t itr = 0;  // _DI_
    int16_t battle_unit_idx = 0;  // _SI_

    Combat_Compose_Background();
    Set_Page_Off();
    Combat_Screen_Draw();
    PageFlip_FX();
    Copy_On_To_Off_Page();
    Copy_Off_To_Back();

    // ; create a list of target BU indexes, and another of the pointers to their names
    // ; BUG: considers non-involved, recalled, and fleeing  units as valid targets
    // ; BUG: the arrays are only 8 elements long, need 9+
    // ; BUG? considers the Chosen as an invalid target
    // ; BUG: writes to the name pointers instead of assigning  to them
    Target_Count = 0;
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        // ; BUG: allows "raising" uninvolved, recalled, or even
        // ; fleeing units (gone is checked later)
        if(
            (battle_units[itr].status > bus_Active)
            &&
            (battle_units[itr].controller_idx == player_idx)
            &&
            (battle_units[itr].status != bus_Gone)
            &&
            (_UNITS[battle_units[itr].unit_idx].wp != 9)
        )
        {
            // ; BUG: this list can only hold 8 units - it needs at
            // ; least 9 even if combat summons can't be raised
            Target_BU_List[Target_Count] = itr;
            // ; BUG: this list can only hold 8 units - it needs at
            // ; least 9 even if combat summons can't be raised
            // ; BUG: this will copy the string to a random address
            // ; instead of saving its pointer here
            // // BUGBUG  stu_strcpy(Target_Name_Ptr_List[Target_Count], _unit_type_table[_UNITS[battle_units[itr].unit_idx].type].name);
            // Target_Name_Ptr_List[Target_Count] = _unit_type_table[_UNITS[battle_units[itr].unit_idx].type].name;
            Target_Name_Ptr_List[Target_Count] = (char *)*_unit_type_table[_UNITS[battle_units[itr].unit_idx].type].name;
            Target_Count++;
            /* HACK */  Target_Name_Ptr_List[Target_Count] = (char *)str_empty_string__ovr131;
        }

    }

    Picked_Target = ST_UNDEFINED;

    if(Target_Count > 0)
    {
        if(player_idx != HUMAN_PLAYER_IDX)
        {
            // ; BUG: the AI always picks the first unit on the list
            Picked_Target = 0;
        }
        else
        {
            if(Target_Count == 1)
            {
                Picked_Target = 0;
            }
            else
            {
                if(Target_Count < 7)
                {
                    // Picked_Target = Selection_Box(Target_Count, &Target_Name_Ptr_List[0], 0, cnst_RaiseDead_Msg);  // "Select a unit to Raise"
                    // Picked_Target = Selection_Box(Target_Count, Target_Name_Ptr_List, 0, cnst_RaiseDead_Msg);  // "Select a unit to Raise"
                    Picked_Target = Selection_Box(Target_Count, &Target_Name_Ptr_List[0], 0, cnst_RaiseDead_Msg);  // "Select a unit to Raise"
                }
                else
                {
                    // Picked_Target = Selection_Box(6, &Target_Name_Ptr_List[0], 1, cnst_RaiseDead_Msg);  // "Select a unit to Raise"
                    // Picked_Target = Selection_Box(6, Target_Name_Ptr_List, 1, cnst_RaiseDead_Msg);  // "Select a unit to Raise"
                    Picked_Target = Selection_Box(6, &Target_Name_Ptr_List[0], 1, cnst_RaiseDead_Msg);  // "Select a unit to Raise"
                }
            }

        }
    }

    Set_Page_Off();
    Combat_Screen_Draw();
    PageFlip_FX();

    if(Picked_Target != ST_UNDEFINED)
    {
        if(player_idx != HUMAN_PLAYER_IDX)
        {
            // ; BUG: will enter an infinite loop if all 9 squares in the selected area are occupied or invalid
            do
            {
                if(player_idx == _combat_attacker_player)
                {
                    cgx = (14 - Random(3));
                }
                else
                {
                    cgx = (Random(3) + 7);
                }
                cgy = (8 + Random(3));
            } while(g_combat_grid_action_map[cgy][cgx] >= 0);
        }

        battle_unit_idx = Target_BU_List[Picked_Target];

        // ~ Prepare_Battle_Unit()
        if(battle_units[battle_unit_idx].figure_max > 1)
        {
            battle_units[battle_unit_idx].front_figure_damage = 0;
            battle_units[battle_unit_idx].figure_cnt = (battle_units[battle_unit_idx].figure_max / 2);
        }
        else
        {
            battle_units[battle_unit_idx].figure_cnt = battle_units[battle_unit_idx].figure_max;
            battle_units[battle_unit_idx].front_figure_damage = (battle_units[battle_unit_idx].hits / 2);
        }
        battle_units[battle_unit_idx].combat_effects = 0;
        battle_units[battle_unit_idx].enchantments = 0;
        _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments = 0;
        battle_units[battle_unit_idx].cgx = cgx;
        battle_units[battle_unit_idx].cgy = cgy;
        battle_units[battle_unit_idx].target_cgx = cgx;
        battle_units[battle_unit_idx].target_cgy = cgy;
        battle_units[battle_unit_idx].move_anim_ctr = 0;
        battle_units[battle_unit_idx].outline_magic_realm = 0;
        battle_units[battle_unit_idx].Atk_FigLoss = 0;
        battle_units[battle_unit_idx].mid_move = ST_FALSE;
        battle_units[battle_unit_idx].action = bua_Ready;
        battle_units[battle_unit_idx].bufpi = Combat_Figure_Load(_UNITS[battle_units[battle_unit_idx].unit_idx].type, Battle_Unit_Pict_Open());
        battle_units[battle_unit_idx].status = bus_Active;
        Battle_Unit_Regular_Stats(&battle_units[battle_unit_idx]);
        Battle_Unit_Special_Stats(&battle_units[battle_unit_idx]);
        Battle_Unit_Summon_Animation(battle_unit_idx, cgx, cgy, spl_Raise_Dead, caster_idx);
    }

    Set_Page_Off();
    Combat_Screen_Draw();
    PageFlip_FX();

}


// WZD o131p10
// drake178: CMB_AnimateDead()
/*
; processes the casting of the Animate Dead spell,
; including presenting the list selection dialog for
; choosing the unit to raise, playing the animation,
; and setting the stats of the unit
; contains numerous BUGs, including several that will
; crash the game or corrupt memory
*/
/*

¿ 'Raise Dead' calls these two other times - Set_Page_Off(); Tactical_Combat_Draw(); PageFlip_FX(); ?

*/
void Cast_Animate_Dead(int16_t player_idx, int16_t caster_idx)
{
    char * Target_Name_Ptr_List[(MAX_BATTLE_UNIT_COUNT - 1)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Target_BU_List[(MAX_BATTLE_UNIT_COUNT - 1)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t cgx = 0;
    int16_t cgy = 0;
    int16_t Picked_Target = 0;
    int16_t Target_Count = 0;
    int16_t itr = 0;  // _DI_
    int16_t battle_unit_idx = 0;  // _SI_

    Target_Count = 0;

    Combat_Compose_Background();
    Set_Page_Off();
    Combat_Screen_Draw();
    PageFlip_FX();
    Copy_On_To_Off_Page();
    Copy_Off_To_Back();

    // ; create a list of target BU indexes, and another of
    // ; the pointers to their names
    // ; BUG: considers non-involved, recalled, and fleeing
    // ;  units as valid targets
    // ; BUG: the arrays are only 17 elements long, need 35
    // ; BUG: writes to the name pointers instead of assigning
    // ;  to them
    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        // ; BUG: allows "animating" uninvolved, recalled, or even
        // ; fleeing units
        if(
            (battle_units[itr].status > bus_Active)
            &&
            (battle_units[itr].status != bus_Gone)
            &&
            (battle_units[itr].controller_idx == player_idx)
            &&
            (_UNITS[battle_units[itr].unit_idx].Hero_Slot == -1)
            &&
            (_UNITS[battle_units[itr].unit_idx].wp != 9)
        )
        {
            // ; BUG: this list can only hold 17 units - it needs the
            // ; 35 to account for every scenario
            Target_BU_List[Target_Count] = itr;
            // ; BUG: this list can only hold 17 units - it needs the
            // ; 35 to account for every scenario
            // ; BUG: this will copy the string to a random address
            // ; instead of saving its pointer here
            // ; dest
            // BUGBUG  stu_strcpy(Target_Name_Ptr_List[Target_Count], _unit_type_table[_UNITS[battle_units[itr].unit_idx].type].name);
            // Target_Name_Ptr_List[Target_Count] = _unit_type_table[_UNITS[battle_units[itr].unit_idx].type].name;
            Target_Name_Ptr_List[Target_Count] = (char *)*_unit_type_table[_UNITS[battle_units[itr].unit_idx].type].name;
            Target_Count++;
            /* HACK */  Target_Name_Ptr_List[Target_Count] = (char *)str_empty_string__ovr131;
        }
    }

    if(Target_Count < 7)
    {
        // Picked_Target = Selection_Box(Target_Count, &Target_Name_Ptr_List[0], 0, cnst_RaiseDead_Msg);  // "Select a unit to Raise"
        // Picked_Target = Selection_Box(Target_Count, Target_Name_Ptr_List, 0, cnst_RaiseDead_Msg);  // "Select a unit to Raise"
        Picked_Target = Selection_Box(Target_Count, &Target_Name_Ptr_List[0], 0, cnst_RaiseDead_Msg);  // "Select a unit to Raise"
    }
    else
    {
        // Picked_Target = Selection_Box(6, &Target_Name_Ptr_List[0], 1, cnst_RaiseDead_Msg);  // "Select a unit to Raise"
        // Picked_Target = Selection_Box(6, Target_Name_Ptr_List, 1, cnst_RaiseDead_Msg);  // "Select a unit to Raise"
        Picked_Target = Selection_Box(6, &Target_Name_Ptr_List[0], 1, cnst_RaiseDead_Msg);  // "Select a unit to Raise"
    }

    if(Picked_Target != ST_UNDEFINED)
    {
        do
        {
            if(player_idx == _combat_attacker_player)
            {
                cgx = (14 - Random(3));
            }
            else
            {
                cgx = (Random(3) + 7);
            }
            cgy = (8 + Random(3));
        }
        while(g_combat_grid_action_map[cgy][cgx] >= 0);
    }

// Severity Code Description Project File Line Suppression State Details
//     Warning C6385 Reading invalid data from 'Target_BU_List'.sdl2_ReMoM C :\STU\devel\ReMoM\src\Spells131.C 344		
    if(Picked_Target > ST_UNDEFINED)
        battle_unit_idx = Target_BU_List[Picked_Target];

    // ~ Prepare_Battle_Unit()
    battle_units[battle_unit_idx].controller_idx = (int8_t)player_idx;  // ; BUG: fails to set the overland owner of the unit
    battle_units[battle_unit_idx].figure_cnt = battle_units[battle_unit_idx].figure_max;
    battle_units[battle_unit_idx].front_figure_damage = 0;
    battle_units[battle_unit_idx].combat_effects = 0;
    battle_units[battle_unit_idx].enchantments = 0;
    battle_units[battle_unit_idx].move_anim_ctr = 0;
    battle_units[battle_unit_idx].outline_magic_realm = 0;
    battle_units[battle_unit_idx].Atk_FigLoss = 0;
    battle_units[battle_unit_idx].mid_move = ST_FALSE;
    battle_units[battle_unit_idx].action = bua_Ready;
    _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments = 0;
    _UNITS[battle_units[battle_unit_idx].unit_idx].mutations |= UM_UNDEAD;
    battle_units[battle_unit_idx].cgx = cgx;
    battle_units[battle_unit_idx].cgy = cgy;
    // ¿ BUGBUG  doesn't set target_cgx,cgy, like 'Raise Dead' ?
    battle_units[battle_unit_idx].bufpi = Combat_Figure_Load(_UNITS[battle_units[battle_unit_idx].unit_idx].type, Battle_Unit_Pict_Open());
    battle_units[battle_unit_idx].status = bus_Active;
    Battle_Unit_Regular_Stats(&battle_units[battle_unit_idx]);
    Battle_Unit_Special_Stats(&battle_units[battle_unit_idx]);
    Battle_Unit_Summon_Animation(battle_unit_idx, cgx, cgy, spl_Animate_Dead, caster_idx);

}
