/*
    ¿ Overland Spell Casting ?

    WIZARDS.EXE
        ovr135
*/

#include "Explore.H"
#include "MOM_DEF.H"
#include "MOX/MOM_Data.H"
#include "MOX/MOX_DAT.H"  /* _screen_seg */
#include "MOX/MOX_DEF.H"
#include "MOX/MOX_SET.H"  /* magic_set */
#include "MOX/MOX_TYPE.H"

#include "SPELLDEF.H"

#include "DIPLOMAC.H"
#include "NEXTTURN.H"
#include "Outpost.H"
#include "Spellbook.H"
#include "Spells128.H"
#include "Spells129.H"
#include "Spells130.H"
#include "Spells132.H"
#include "Spells137.H"
#include "SPLMASTR.H"



// WZD dseg:6A42                                                 BEGIN:  ovr135 - Initialized Data

// WZD dseg:6A42
char specfx_lbx_file__ovr135[] = "SpEcFx";

// WZD dseg:6A49
char cnst_Dispel_Msg_2[] = " has been dispelled.";

// WZD dseg:6A5E
char message_lbx_file__ovr135[] = "message";

// WZD dseg:6A66
char str_SuppressMagic[] = "Suppress Magic";
// WZD dseg:6A75
char str_Tranquility[] = "Tranquility";
// WZD dseg:6A81
char str_LifeForce[] = "Life Force";

// WZD dseg:6A8C
char cnst_SpellError_1_2[] = "That unit already has ";
// WZD dseg:6AA3
char cnst_SpellError_2_2[] = " cast on it";
// WZD dseg:6AAF
char aOnlyNormalUnit[] = "Only normal units may have ";
// WZD dseg:6ACB
char aCastOnThem[] = " cast on them";
// WZD dseg:6AD9
char aOnlyNormalUn_0[] = "Only normal units and created undead may have ";
// WZD dseg:6B08
char aThatCityAlread[] = "That city already has ";

// WZD dseg:6B1F 20 73 70 65 6C 6C 20 68 61 73 20 66 61 69 6C 65+cnst_SpellError_4 db ' spell has failed.',0

// WZD dseg:6B32 53 70 65 6C 6C 20 42 6C 61 73 74 00             aSpellBlast db 'Spell Blast',0

// WZD dseg:6B3E
char aDisjunction[] = "Disjunction";

// WZD dseg:6B4A 20 74 68 65 20                                  aThe_0 db ' the '

// WZD dseg:6B4A                                                 END:  ovr135 - Initialized Data



/*
    WIZARDS.EXE  ovr135
*/

// WZD o135p01
// drake178: SPL_Get_CityE_Index()
/*
; returns the spell index corresponding to a specific
; offset in the city enchantments array, based on the
; parameters stored in the spell records
*/
/*
    gives spell_dx
    takes city_enchantment_idx
*/
int16_t Get_Spell_For_City_Enchandment(int16_t city_enchantment_idx)
{
    int16_t spell_idx = 0;  // _SI_
    int16_t itr = 0;  // _CX_

    spell_idx = spl_NONE;

    if(
        (city_enchantment_idx == NATURE_WARD)
        ||
        (city_enchantment_idx == DEATH_WARD)
    )
    {

        spell_idx = spl_Spell_Ward;

    }

    for(itr = spl_Earth_To_Mud; ((itr < spl_MAX_SPELL_COUNT) && (spell_idx == spl_NONE)); itr++)
    {

        if(
            (spell_data_table[itr].type == scc_City_Enchantment_Positive)
            ||
            (spell_data_table[itr].type == scc_City_Enchantment_Positive)
        )
        {

            if(spell_data_table[itr].ce_idx == city_enchantment_idx)
            {

                spell_idx = itr;

            }

        }

    }

    return spell_idx;
    
}


// WZD o135p02
// drake178: WIZ_GetSpellStrength()
/*
; applies dispel resistance retorts to the passed
; spell cost based on its realm, returning the
; effective cost as a 32bit value in dx:ax
*/
/*

archmage
It is also twice as difficult for an enemy wizard to dispel an Archmage’s spells.
Chaos Mastery
In addition, chaos spells cast by a wizard with this special ability are twice as difficult to dispel.
Nature Mastery
It also makes nature spells cast by a wizard with this special ability twice as difficult to dispel.
Sorcery Mastery
In addition, Sorcery Mastery makes sorcery spells cast by a wizard twice as difficult to dispel as normal.

*/
int16_t Calculate_Dispel_Difficulty(int16_t casting_cost, int16_t player_idx, int16_t magic_realm)
{
    int16_t multiplyer = 0;  // _CX_

    multiplyer = 1;

    if(player_idx != ST_UNDEFINED)
    {

        switch(magic_realm)
        {
            case sbr_Nature:
            {
                if(_players[player_idx].nature_mastery > 0)
                {
                    multiplyer += 1;
                }
            } break;

            case sbr_Sorcery:
            {
                if(_players[player_idx].sorcery_mastery > 0)
                {
                    multiplyer += 1;
                }
            } break;

            case sbr_Chaos:
            {
                if(_players[player_idx].chaos_mastery > 0)
                {
                    multiplyer += 1;
                }
            } break;
        }

        if(_players[player_idx].archmage > 0)
        {

            multiplyer += 1;
            
        }

    }

    return (casting_cost * multiplyer);

}


// WZD o135p03
// drake178: sub_BA3A4()
void Cast_Disenchant(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx, int16_t strength)
{
    int16_t spells[32] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint32_t test_bit = 0;
    SAMB_ptr dispell2_notify_background_seg = 0 ;
    SAMB_ptr dispell1_notify_background_seg = 0 ;
    int16_t notify_count = 0;
    int16_t threshold = 0 ;
    uint8_t * ptr_enchantments = 0 ;
    int16_t spell_idx = 0;
    int16_t city_enchantment_idx = 0 ;
    int16_t itr = 0;  // _DI_

    notify_count = 0;

    // SPECFX.LBX, 051  "DISPELL2"  ""
    dispell1_notify_background_seg = LBX_Reload_Next(specfx_lbx_file__ovr135, 51, _screen_seg);

    // SPECFX.LBX, 052  "DISPELL2"  ""
    dispell2_notify_background_seg = LBX_Reload_Next(specfx_lbx_file__ovr135, 52, _screen_seg);

    spells[ 0] = spl_Immolation;
    spells[ 1] = spl_Guardian_Wind;
    spells[ 2] = spl_Berserk;
    spells[ 3] = spl_Cloak_Of_Fear;
    spells[ 4] = spl_Black_Channels;
    spells[ 5] = spl_Wraith_Form;
    spells[ 6] = spl_Regeneration;
    spells[ 7] = spl_Path_Finding;
    spells[ 8] = spl_Water_Walking;
    spells[ 9] = spl_Resist_Elements;
    spells[10] = spl_Elemental_Armor;
    spells[11] = spl_Stone_Skin;
    spells[12] = spl_Iron_Skin;
    spells[13] = spl_Endurance;
    spells[14] = spl_Spell_Lock;
    spells[15] = spl_Invisibility;
    spells[16] = spl_Wind_Walking;
    spells[17] = spl_Flight;
    spells[18] = spl_Resist_Magic;
    spells[19] = spl_Magic_Immunity;
    spells[20] = spl_Flame_Blade;
    spells[21] = spl_Eldritch_Weapon;
    spells[22] = spl_True_Sight;
    spells[23] = spl_Holy_Weapon;
    spells[24] = spl_Heroism;
    spells[25] = spl_Bless;
    spells[26] = spl_Lionheart;
    spells[27] = spl_Giant_Strength;
    spells[28] = spl_Planar_Travel;
    spells[29] = spl_Holy_Armor;
    spells[30] = spl_Righteousness;
    spells[31] = spl_Invulnerability;

    for(itr = 0; itr < NUM_NODES; itr++)
    {

        if(
            ((_NODES[itr].flags & NF_WARPED) > 0)
            &&
            (_NODES[itr].wx == wx)
            &&
            (_NODES[itr].wy == wy)
            &&
            (_NODES[itr].wp == wp)
        )
        {

            threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[spl_Warp_Node].casting_cost, ST_UNDEFINED, sbr_Chaos));

            threshold = ((strength * 250) / threshold);

            if(Random(250) <= threshold)
            {

                _NODES[itr].flags ^= NF_WARPED;

            }
        }

    }

    for(itr = 0; itr < _cities; itr++)
    {

        if(
            (_CITIES[itr].wx == wx)
            &&
            (_CITIES[itr].wy == wy)
            &&
            (_CITIES[itr].wp == wp)
        )
        {

            ptr_enchantments = (uint8_t *)&_CITIES[itr].enchantments[0];

            for(city_enchantment_idx = 0; city_enchantment_idx < 26; city_enchantment_idx++)
            {

                if(
                    (ptr_enchantments[city_enchantment_idx] > 0)
                    &&
                    (ptr_enchantments[city_enchantment_idx] == (player_idx + 1))
                )
                {

                    spell_idx = Get_Spell_For_City_Enchandment(city_enchantment_idx);

                    threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[spell_idx].casting_cost, (ptr_enchantments[city_enchantment_idx] - 1), spell_data_table[spell_idx].magic_realm));

                    threshold = ((strength * 250) / threshold);

                    if(Random(250) <= (threshold + 500))
                    {

                        ptr_enchantments[city_enchantment_idx] = 0;

                        if(player_idx == HUMAN_PLAYER_IDX)
                        {

                            if(notify_count < 5)
                            {

                                if(notify_count < 4)
                                {

                                    _fstrcpy(GUI_NearMsgString, spell_data_table[spell_idx].name);

                                    strcat(GUI_NearMsgString, cnst_Dispel_Msg_2);  // " has been dispelled."

                                }
                                else
                                {

                                    LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 18, 1, 150);

                                }

                                Notify2(160, (40 + (notify_count * 25)), 2, GUI_NearMsgString, 0, dispell2_notify_background_seg, 0, 8, 0, 0, 0, 1, 0);

                                notify_count += 1;

                            }

                        }

                    }

                }

            }

        }

    }

    for(itr = 0; itr > _units; itr++)
    {

        if(
            (_UNITS[itr].wx == wx)
            &&
            (_UNITS[itr].wy == wy)
            &&
            (_UNITS[itr].wp == wp)
        )
        {

            if(_UNITS[itr].owner_idx == player_idx)
            {

                if((_UNITS[itr].mutations & (C_STASISINIT | C_STASISLINGER)) != 0)
                {

                    spell_idx = spl_Stasis;

                    threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[spell_idx].casting_cost, _UNITS[itr].owner_idx, spell_data_table[spell_idx].magic_realm));

                    threshold = ((strength * 250) / threshold);

                    if(Random(250) <= (threshold + 500))
                    {

                        if((_UNITS[itr].mutations & C_STASISINIT) != 0)
                        {

                            _UNITS[itr].mutations ^= C_STASISINIT;

                        }
                        else
                        {

                            _UNITS[itr].mutations ^= C_STASISLINGER;

                        }

                        if(player_idx == HUMAN_PLAYER_IDX)
                        {

                            LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 20, 1, 150);

                            Notify2(160, (40 + (notify_count * 25)), 2, GUI_NearMsgString, 0, dispell2_notify_background_seg, 0, 8, 0, 0, 0, 1, 0);

                        }

                    }

                }

            }
            else  /* (_UNITS[itr].owner_idx == player_idx) */
            {

                if((_UNITS[itr].enchantments & UE_SPELLLOCK) != 0)
                {

                    threshold = (strength + Calculate_Dispel_Difficulty(150, _UNITS[itr].owner_idx, sbr_Sorcery));

                    threshold = ((strength * 250) / threshold);

                    if(Random(250) <= (threshold + 500))
                    {

                        _UNITS[itr].enchantments ^= UE_SPELLLOCK;

                    }

                }  /* ((_UNITS[itr].enchantments & UE_SPELLLOCK) == 0) */
                else
                {

                    for(city_enchantment_idx = 0; city_enchantment_idx < 31; city_enchantment_idx++)
                    {

                        test_bit = (1 << city_enchantment_idx);

                        if((_UNITS[itr].enchantments & test_bit) >= 0)
                        {

                            threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[spells[city_enchantment_idx]].casting_cost, _UNITS[itr].owner_idx, spell_data_table[spells[city_enchantment_idx]].magic_realm));

                            threshold = ((strength * 250) / threshold);

                            if(Random(250) <= (threshold + 500))
                            {

                                _UNITS[itr].enchantments ^= test_bit;

                                if(player_idx == HUMAN_PLAYER_IDX)
                                {

                                    if(notify_count < 5)
                                    {

                                        if(notify_count < 4)
                                        {

                                            _fstrcpy(GUI_NearMsgString, spell_data_table[spells[city_enchantment_idx]].name);

                                            strcat(GUI_NearMsgString, cnst_Dispel_Msg_2);  // " has been dispelled."

                                        }
                                        else
                                        {

                                            LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 19, 1, 150);

                                        }

                                        Notify2(160, (40 + (notify_count * 25)), 2, GUI_NearMsgString, 0, dispell1_notify_background_seg, 4, 8, 0, 0, 0, 1, 0);

                                        notify_count += 1;

                                    }

                                }

                            }


                        }

                    }

                }

            }

        }

    }

}


// WZD o135p04
// drake178: sub_BAB5D()
/*
*/
/*
    returns ST_TRUE if the map square has a viable target
    checks Nodes for Warped
*/
int16_t Square_Has_Disenchant_Target(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t itr = 0;  // _SI_
    int16_t found_target_flag = 0;  // _DI_

    found_target_flag = ST_FALSE;

    for(itr = 0; itr < _cities; itr++)
    {

        if(
            (_CITIES[itr].wx == wx)
            &&
            (_CITIES[itr].wy == wy)
            &&
            (_CITIES[itr].wp == wp)
        )
        {

            found_target_flag = ST_TRUE;

        }
        
    }

    if(found_target_flag == ST_FALSE)
    {

        for(itr = 0; itr < NUM_NODES; itr++)
        {

            if(
                ((_NODES[itr].flags & NF_WARPED) > 0)
                &&
                (_NODES[itr].wx == wx)
                &&
                (_NODES[itr].wy == wy)
                &&
                (_NODES[itr].wp == wp)
            )
            {

                found_target_flag = ST_TRUE;

            }

        }

    }


    if(found_target_flag == ST_FALSE)
    {

        for(itr = 0; itr < NUM_NODES; itr++)
        {

            if(
                (_UNITS[itr].wx == wx)
                &&
                (_UNITS[itr].wy == wy)
                &&
                (_UNITS[itr].wp == wp)
            )
            {

                found_target_flag = ST_TRUE;

            }

        }

    }

    return found_target_flag;

}


// WZD o135p05
// drake178: ¿ G_OVL_Cast() ?
// MoO2  N/A
// 1oom  N/A
/*
*/
/*

IDA Group Colors
    scc_Summoning                  ( 0)  #24 reddish-brown
    scc_Unit_Enchantment           ( 1)  #43 pea green
    scc_City_Enchantment_Positive  ( 2)  #14 blueish lighter
    scc_City_Enchantment_Negative  ( 3)  #14 blueish lighter
    scc_Direct_Damage_Fixed        ( 4)  #32 purple
    scc_Special_Spell              ( 5)  
    scc_Global_Enchantment         ( 9)  #13 ~ blue, greyish/greenish
    scc_Crafting_Spell             (11)  #17 mauve

    scc_Direct_Damage_Variable     (22)  #31 redish purple

*/
void Cast_Spell_Overland__WIP(int16_t player_idx)
{
    char spell_name[LEN_SPELL_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t show_message_flag = 0;
    int16_t var_1A = 0;
    int16_t item_list = 0;
    int16_t MultiPurpose_Local_Var = 0;
    int16_t Cast_Successful = 0;
    int16_t target_wy = 0;
    int16_t target_wp = 0;
    int16_t wp = 0;
    int16_t wy = 0;
    int16_t spell_target_idx = 0; /* also gets used as &wx */
    int16_t itr_players = 0;  // DNE in Dasm, reuses itr
    int16_t itr_cities = 0;  // DNE in Dasm, reuses itr
    int16_t itr_units = 0;  // DNE in Dasm, reuses itr
    int16_t threshold = 0;
    int16_t enchantments_idx = 0;  // DNE in Dasm, uses Dispel_Chance
    uint8_t * ptr_enchantments = 0;  // (uint8_t *)&_CITIES[].enchantments[0]
    int16_t spell_idx = 0;  // _DI_
    struct s_SPELL_DATA * DBG_spell_data_table;
    // int8_t * DBG_spell_data_table__Param0;
    int16_t DBG_spell_data_table__Param0;

    if(player_idx == HUMAN_PLAYER_IDX)
    {
        show_message_flag = ST_TRUE;
    }
    else
    {
        show_message_flag = ST_FALSE;
    }

    Cast_Successful = ST_TRUE;

    AI_Eval_After_Spell = ST_FALSE;

    spell_idx = _players[player_idx].casting_spell_idx;

    if(spell_idx == spl_NONE)
    {
        return;
    }


    strcpy(spell_name, spell_data_table[spell_idx].name);


    /*
        BEGIN:  Suppress Magic
    */
    {
        // "Tries to prevent, with a strength 500 dispelling force, all enemy overland spells and enchantments."

        // To calculate Suppress Magic's dispelling potency against the target spell, the game uses the following formula:
        // Potency = (500 / (500 + TSCC)) * 100
        // Where TSCC is the total Casting Cost of the targeted enemy spell.

        for(itr_players = 0; ((itr_players < _num_players) && (Cast_Successful == ST_TRUE)); itr_players++)
        {

            if(
                (_players[itr_players].Globals[SUPPRESS_MAGIC] > 0)
                &&
                (itr_players != player_idx)
            )
            {

                threshold = (500 + Calculate_Dispel_Difficulty(_players[player_idx].casting_cost_original, player_idx, spell_data_table[spell_idx].magic_realm));

                threshold = (125000 / threshold);  // 250 * 500 = 125000

                if(Random(250) <= threshold)
                {

                    Fizzle_Notification(player_idx, itr_players, spell_idx, str_SuppressMagic);

                    Cast_Successful = ST_FALSE;

                }

            }

        }
    }
    /*
        END:  Suppress Magic
    */


    /*
        BEGIN:  Tranquility  (~== Suppress Chaos Magic)
    */
    {
        if(
            (spell_data_table[spell_idx].magic_realm == sbr_Chaos)
            &&
            (Cast_Successful == ST_TRUE)
        )
        {

            for(itr_players = 0; ((itr_players < _num_players) && (Cast_Successful == ST_TRUE)); itr_players++)
            {

                if(
                    (_players[itr_players].Globals[TRANQUILITY] > 0)
                    &&
                    (itr_players != player_idx)
                )
                {

                    threshold = (500 + Calculate_Dispel_Difficulty(_players[player_idx].casting_cost_original, player_idx, spell_data_table[spell_idx].magic_realm));

                    threshold = (125000 / threshold);

                    if(Random(250) <= threshold)
                    {

                        Fizzle_Notification(player_idx, itr_players, spell_idx, str_Tranquility);

                        Cast_Successful = ST_FALSE;

                    }

                }

            }

        }
    }
    /*
        END:  Tranquility  (~== Suppress Chaos Magic)
    */



    /*
        BEGIN:  Life Force  (~== Suppress Death Magic)
    */
    {
        if(
            (spell_data_table[spell_idx].magic_realm == sbr_Death)
            &&
            (Cast_Successful == ST_TRUE)
        )
        {

            for(itr_players = 0; ((itr_players < _num_players) && (Cast_Successful == ST_TRUE)); itr_players++)
            {

                if(
                    (_players[itr_players].Globals[LIFE_FORCE] > 0)
                    &&
                    (itr_players != player_idx)
                )
                {

                    threshold = (500 + Calculate_Dispel_Difficulty(_players[player_idx].casting_cost_original, player_idx, spell_data_table[spell_idx].magic_realm));

                    threshold = (125000 / threshold);

                    if(Random(250) <= threshold)
                    {

                        Fizzle_Notification(player_idx, itr_players, spell_idx, str_LifeForce);

                        Cast_Successful = ST_FALSE;

                    }

                }

            }

        }
    }
    /*
        END:  Life Force  (~== Suppress Death Magic)
    */


    if(Cast_Successful == ST_TRUE)
    {

        SBK_Spell_Index = spell_idx;

        if(spell_idx == spl_Spell_Of_Mastery)
        {

            Spell_Of_Mastery(player_idx);

            AI_Eval_After_Spell = ST_TRUE;
            
        }
        else
        {

            switch(spell_data_table[spell_idx].type)
            {

                // Air Elemental, Angel, Arch Angel, Basilisk, Behemoth, Chaos Spawn, Chimeras, Cockatrices, Colossus, Death Knights, Demon Lord, Djinn, Doom Bat, Earth Elemental, Efreet, Fire Elemental, Fire Giant, Floating Island, Gargoyles, Ghouls, Giant Spiders, Gorgons, Great Drake, Great Wyrm, Guardian Spirit, Hell Hounds, Hydra, Magic Spirit, Nagas, Night Stalker, Phantom Beast, Phantom Warriors, Shadow Demons, Skeletons, Sky Drake, Sprites, Stone Giant, Storm Giant, Unicorns, War Bears, Wraiths
                case scc_Summoning:
                {

                    // ¿ BUG should be ((player_idx == HUMAN_PLAYER_IDX) && (_units < 950)) ?
                    if(
                        (_units < 980)
                        ||
                        (player_idx == HUMAN_PLAYER_IDX)
                        ||
                        (
                            (player_idx != HUMAN_PLAYER_IDX)
                            &&
                            (_units < 950)
                        )
                    )
                    {

                        if(spell_idx == spl_Floating_Island)
                        {

                            AI_Eval_After_Spell = ST_TRUE;

                            Cast_Floating_Island(player_idx);

                        }
                        else
                        {

                            Create_Unit__WIP(spell_data_table[spell_idx].Param0, player_idx, _players[player_idx].summon_wx, _players[player_idx].summon_wy, _players[player_idx].summon_wp, ST_UNDEFINED);

                            UNIT_RemoveExcess((_units - 1));

                        }

                        MultiPurpose_Local_Var = ((spell_idx - 1) % NUM_SPELLS_PER_MAGIC_REALM);

                        MultiPurpose_Local_Var = (MultiPurpose_Local_Var / 10);

                        if(spell_idx != spl_Floating_Island)
                        {

                            if(
                                (player_idx == HUMAN_PLAYER_IDX)
                                ||
                                (
                                    (magic_set.enemy_spells == ST_TRUE)
                                    &&
                                    (_players[player_idx].Dipl.Contacted[HUMAN_PLAYER_IDX] == ST_TRUE)
                                    &&
                                    (_players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] > 0)
                                )
                            )
                            {

                                DBG_spell_data_table = &spell_data_table[spell_idx];

                                // DBG_spell_data_table__Param0 = spell_data_table[spell_idx].Param0;
                                DBG_spell_data_table__Param0 = GET_2B_OFS(DBG_spell_data_table, 0x20);

                                // DBG_spell_data_table__Param0 = GET_1B_OFS(DBG_spell_data_table, (0x20 - 1));

                                // IDK_SummonAnim(spell_data_table[spell_idx].Param0, MultiPurpose_Local_Var, player_idx);
                                IDK_SummonAnim(DBG_spell_data_table__Param0, MultiPurpose_Local_Var, player_idx);

                                AI_Eval_After_Spell = ST_TRUE;

                            }

                        }

                        _UNITS[(_units - 1)].Finished = ST_FALSE;
                        _UNITS[(_units - 1)].moves2 = _UNITS[(_units - 1)].moves2_max;

                        if(player_idx == HUMAN_PLAYER_IDX)
                        {

                            _active_world_x = _UNITS[_units].wx;
                            _active_world_y = _UNITS[_units].wy;
                            _map_plane = _UNITS[_units].wp;

                            // ; BUG: this function has a parameter!
                            /* WASBUG */ IDK_HumanPlayer_SelectStack_UnitLocation((_units - 1));  // how to reproduce calling this without the unit_idx parameter?

                        }

                        Cast_Successful = ST_TRUE;

                    }
                    else
                    {
                        
                        if(player_idx == HUMAN_PLAYER_IDX)
                        {

                            LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 21, 1, 150);  // "Maximum unit count exceeded.  Summons fails."

                            Warn0(GUI_NearMsgString);

                        }

                        Cast_Successful = ST_FALSE;

                    }

                } break;  /* case scc_Summoning: */
                
                // Berserk, Black Channels, Bless, Cloak of Fear, Elemental Armor, Endurance, Flight, Giant Strength, Guardian Wind, Haste, Immolation, Invisibility, Invulnerability, Iron Skin, Lionheart, Magic Immunity, Path Finding, Planar Travel, Regeneration, Resist Elements, Resist Magic, Righteousness, Spell Lock, Stone Skin, True Sight, Water Walking, Wind Walking, Wraith Form
                case scc_Unit_Enchantment:
                // Eldritch Weapon, Flame Blade, Heroism, Holy Armor, Holy Weapon
                case scc_Unit_Enchantment_Normal_Only:
                {

                    if(player_idx != HUMAN_PLAYER_IDX)
                    {

                        /* SPELLY */  Cast_Successful = IDK_Pick_Target_For_Unit_Enchantment__STUB(stt_Friendly_Unit, &spell_target_idx, spell_idx, player_idx);

                    }
                    else
                    {

                        MultiPurpose_Local_Var = ST_FALSE;
                        Cast_Successful = ST_TRUE;

                        while((MultiPurpose_Local_Var == ST_FALSE) && (Cast_Successful == ST_TRUE))
                        {

                            MultiPurpose_Local_Var = ST_TRUE;

                            Cast_Successful = Spell_Casting_Screen__WIP(stt_Friendly_Unit, &spell_target_idx, &wy, &wp, &target_wy, &target_wp, (char *)&spell_name[0]);

                            if(Cast_Successful == ST_TRUE)
                            {

                                if((_UNITS[spell_target_idx].enchantments & spell_data_table[spell_idx].enchantments) != 0)
                                {
                                    MultiPurpose_Local_Var = ST_FALSE;
                                    Allocate_Reduced_Map();
                                    Full_Draw_Main_Screen();
                                    strcpy(GUI_NearMsgString, cnst_SpellError_1_2);
                                    strcat(GUI_NearMsgString, spell_name);
                                    strcat(GUI_NearMsgString, cnst_SpellError_2_2);
                                    Warn0(GUI_NearMsgString);
                                }

                                if(
                                    (spell_idx == spl_Stone_Skin)
                                    &&
                                    ((_UNITS[spell_target_idx].enchantments & UE_IRONSKIN) != 0)
                                )
                                {
                                    MultiPurpose_Local_Var = ST_FALSE;
                                    Allocate_Reduced_Map();
                                    Full_Draw_Main_Screen();
                                    LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 22, 1, 150);  // "Stone Skin is not cumulative with Iron Skin."
                                    Warn0(GUI_NearMsgString);
                                }

                                if(
                                    (spell_idx == spl_Resist_Elements)
                                    &&
                                    ((_UNITS[spell_target_idx].enchantments & UE_ELEMENTALARMOR) != 0)
                                )
                                {
                                    MultiPurpose_Local_Var = ST_FALSE;
                                    Allocate_Reduced_Map();
                                    Full_Draw_Main_Screen();
                                    LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 23, 1, 150);
                                    Warn0(GUI_NearMsgString);
                                }

                                if(
                                    (spell_idx == spl_Resist_Elements)
                                    &&
                                    ((_UNITS[spell_target_idx].enchantments & UE_WRAITHFORM) != 0)
                                )
                                {
                                    MultiPurpose_Local_Var = ST_FALSE;
                                    Allocate_Reduced_Map();
                                    Full_Draw_Main_Screen();
                                    LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 24, 1, 150);
                                    Warn0(GUI_NearMsgString);
                                }

                                if(
                                    (spell_data_table[spell_idx].type == scc_Unit_Enchantment_Normal_Only)
                                    ||
                                    (spell_idx == spl_Chaos_Channels)
                                )
                                {
                                    // BUGBUG:  bad definition of "Normal Unit"
                                    if(Unit_Is_Normal(spell_target_idx) == ST_FALSE)
                                    {
                                        MultiPurpose_Local_Var = ST_FALSE;
                                        Allocate_Reduced_Map();
                                        Full_Draw_Main_Screen();
                                        strcpy(GUI_NearMsgString, aOnlyNormalUnit);
                                        strcat(GUI_NearMsgString, spell_name);
                                        strcat(GUI_NearMsgString, aCastOnThem);
                                        Warn0(GUI_NearMsgString);
                                    }
                                }

                                if(
                                    (spell_idx == spl_Immolation)
                                    &&
                                    ((_unit_type_table[_UNITS[spell_target_idx].type].Attribs_2 & USA_IMMOLATION) != 0)
                                )
                                {
                                    MultiPurpose_Local_Var = ST_FALSE;
                                    Allocate_Reduced_Map();
                                    Full_Draw_Main_Screen();
                                    LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 87, 1, 150);
                                    Warn0(GUI_NearMsgString);
                                }

                                if(
                                    (spell_idx == spl_Cloak_Of_Fear)
                                    &&
                                    ((_unit_type_table[_UNITS[spell_target_idx].type].Attribs_2 & USA_CAUSEFEAR) != 0)
                                )
                                {
                                    MultiPurpose_Local_Var = ST_FALSE;
                                    Allocate_Reduced_Map();
                                    Full_Draw_Main_Screen();
                                    LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 87, 1, 150);
                                    Warn0(GUI_NearMsgString);
                                }

                                if(
                                    (spell_idx == spl_True_Sight)
                                    &&
                                    ((_unit_type_table[_UNITS[spell_target_idx].type].Attribs_1 & USA_IMMUNITY_ILLUSION) != 0)
                                )
                                {
                                    MultiPurpose_Local_Var = ST_FALSE;
                                    Allocate_Reduced_Map();
                                    Full_Draw_Main_Screen();
                                    LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 87, 1, 150);
                                    Warn0(GUI_NearMsgString);
                                }

                                if(
                                    (spell_idx == spl_Magic_Immunity)
                                    &&
                                    ((_unit_type_table[_UNITS[spell_target_idx].type].Attribs_1 & USA_IMMUNITY_MAGIC) != 0)
                                )
                                {
                                    MultiPurpose_Local_Var = ST_FALSE;
                                    Allocate_Reduced_Map();
                                    Full_Draw_Main_Screen();
                                    LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 87, 1, 150);
                                    Warn0(GUI_NearMsgString);
                                }

                                if(
                                    (spell_idx == spl_Planar_Travel)
                                    &&
                                    ((_unit_type_table[_UNITS[spell_target_idx].type].Abilities & UA_PLANARTRAVEL) != 0)
                                )
                                {
                                    MultiPurpose_Local_Var = ST_FALSE;
                                    Allocate_Reduced_Map();
                                    Full_Draw_Main_Screen();
                                    LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 87, 1, 150);
                                    Warn0(GUI_NearMsgString);
                                }

                                if(
                                    (spell_idx == spl_Invisibility)
                                    &&
                                    ((_unit_type_table[_UNITS[spell_target_idx].type].Abilities & UA_INVISIBILITY) != 0)
                                )
                                {
                                    MultiPurpose_Local_Var = ST_FALSE;
                                    Allocate_Reduced_Map();
                                    Full_Draw_Main_Screen();
                                    LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 87, 1, 150);
                                    Warn0(GUI_NearMsgString);
                                }

                                if(
                                    (spell_idx == spl_Wraith_Form)
                                    &&
                                    ((_unit_type_table[_UNITS[spell_target_idx].type].Abilities & UA_NONCORPOREAL) != 0)
                                )
                                {
                                    MultiPurpose_Local_Var = ST_FALSE;
                                    Allocate_Reduced_Map();
                                    Full_Draw_Main_Screen();
                                    LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 87, 1, 150);
                                    Warn0(GUI_NearMsgString);
                                }

                                if(
                                    (spell_idx == spl_Wind_Walking)
                                    &&
                                    ((_unit_type_table[_UNITS[spell_target_idx].type].Abilities & UA_WINDWALKING) != 0)
                                )
                                {
                                    MultiPurpose_Local_Var = ST_FALSE;
                                    Allocate_Reduced_Map();
                                    Full_Draw_Main_Screen();
                                    LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 87, 1, 150);
                                    Warn0(GUI_NearMsgString);
                                }

                                if(
                                    (spell_idx == spl_Regeneration)
                                    &&
                                    ((_unit_type_table[_UNITS[spell_target_idx].type].Abilities & UA_REGENERATION) != 0)
                                )
                                {
                                    MultiPurpose_Local_Var = ST_FALSE;
                                    Allocate_Reduced_Map();
                                    Full_Draw_Main_Screen();
                                    LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 87, 1, 150);
                                    Warn0(GUI_NearMsgString);
                                }

                                // ¿ BUGBUG  should be NOT normal AND undead ?
                                if(
                                    (spell_idx == spl_Black_Channels)
                                    &&
                                    (Unit_Is_Normal(spell_target_idx) == ST_FALSE)
                                    &&
                                    ((_UNITS[spell_target_idx].mutations & UM_UNDEAD) != 0)
                                )
                                {
                                    MultiPurpose_Local_Var = ST_FALSE;
                                    Allocate_Reduced_Map();
                                    Full_Draw_Main_Screen();
                                    strcpy(GUI_NearMsgString, aOnlyNormalUn_0);
                                    strcat(GUI_NearMsgString, spell_name);
                                    strcat(GUI_NearMsgString, aCastOnThem);
                                    Warn0(GUI_NearMsgString);
                                }

                            }  /* if(Cast_Successful == ST_TRUE)  */

                        }  /* while((MultiPurpose_Local_Var == ST_FALSE) && (Cast_Successful == ST_TRUE)) */

                    }

                    // @@After_CastUnitEnch

                    if(player_idx == HUMAN_PLAYER_IDX)
                    {
                        
                        Allocate_Reduced_Map();

                        Spell_Animation_Load_Sound_Effect__WIP(spell_idx);

                        Spell_Animation_Load_Graphics(spell_idx);

                        Spell_Animation_Screen__WIP(_UNITS[spell_target_idx].wx, _UNITS[spell_target_idx].wy, _UNITS[spell_target_idx].wp);

                        Full_Draw_Main_Screen();

                    }

                    _UNITS[spell_target_idx].enchantments |= spell_data_table[spell_idx].enchantments;

                    if(spell_idx == spl_Heroism)
                    {

                        _UNITS[spell_target_idx].Level = Calc_Unit_Level(spell_target_idx);

                    }

                    if(spell_idx == spl_Black_Channels)
                    {

                        _UNITS[spell_target_idx].mutations |= UM_UNDEAD;

                    }

                } break;  /* case scc_Unit_Enchantment, scc_Unit_Enchantment_Normal_Only:*/

                // Altar of Battle, Astral Gate, Cloud of Shadow, Consecration, Dark Rituals, Earth Gate, Flying Fortress, Gaias Blessing, Heavenly Light, Inspirations, Natures Eye, Prosperity, Stream of Life, Summoning Circle, Wall of Darkness, Wall of Fire
                case scc_City_Enchantment_Positive:  //  2
                {

                    if(player_idx != HUMAN_PLAYER_IDX)
                    {

                        /* SPELLY */  Cast_Successful = Pick_Target_For_City_Enchantment__WIP(stt_Friendly_City, &spell_target_idx, spell_idx, player_idx);

                    }
                    else
                    {

                        MultiPurpose_Local_Var = ST_FALSE;

                        Cast_Successful = ST_TRUE;

                        while((MultiPurpose_Local_Var == ST_FALSE) && (Cast_Successful == ST_TRUE))
                        {

                            MultiPurpose_Local_Var = ST_TRUE;

                            Cast_Successful = Spell_Casting_Screen__WIP(stt_Friendly_City, &spell_target_idx, &wy, &wp, &target_wy, &target_wp, (char *)&spell_name[0]);

                            if(Cast_Successful == ST_TRUE)
                            {

                                if(
                                    (spell_idx == spl_Summoning_Circle)
                                    &&
                                    (_CITIES[spell_target_idx].wx == _players[HUMAN_PLAYER_IDX].summon_wx)
                                    &&
                                    (_CITIES[spell_target_idx].wy == _players[HUMAN_PLAYER_IDX].summon_wy)
                                    &&
                                    (_CITIES[spell_target_idx].wp == _players[HUMAN_PLAYER_IDX].summon_wp)
                                )
                                {

                                    MultiPurpose_Local_Var = ST_FALSE;

                                    Full_Draw_Main_Screen();

                                    LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 25, 1, 150);

                                    Warn0(GUI_NearMsgString);

                                }

                                ptr_enchantments = (uint8_t *)&_CITIES[spell_target_idx].enchantments[0];

                                if(ptr_enchantments[spell_data_table[spell_idx].ce_idx] > 0)
                                {

                                    MultiPurpose_Local_Var = ST_FALSE;

                                    Full_Draw_Main_Screen();

                                    strcpy(GUI_NearMsgString, aThatCityAlread);  // "That city already has "

                                    strcat(GUI_NearMsgString, spell_name);

                                    strcat(GUI_NearMsgString, cnst_SpellError_2_2);  // " cast on it"

                                    Warn0(GUI_NearMsgString);

                                }

                            }

                        }

                    }

                    // ovr135:18D3  @@CE2AfterWhileCityEnchantmentAndPlayerIdx:
                    if(Cast_Successful == ST_TRUE)  // ...else @@END_CityEnchantment
                    {

                        if(
                            (player_idx == HUMAN_PLAYER_IDX)
                            ||
                            (_CITIES[spell_target_idx].owner_idx == HUMAN_PLAYER_IDX)
                            ||
                            (
                                (magic_set.enemy_spells == ST_TRUE)
                                &&
                                (SQUARE_EXPLORED(_CITIES[spell_target_idx].wx, _CITIES[spell_target_idx].wy, _CITIES[spell_target_idx].wp))
                                &&
                                (_players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] > 0)
                            )
                        )
                        {

                            AI_Eval_After_Spell = ST_TRUE;

                            Cast_Spell_City_Enchantment_Animation_1__WIP(spell_target_idx, spell_idx, player_idx);

                        }

                        if(spell_idx == spl_Summoning_Circle)
                        {

                            _players[player_idx].summon_wx = _CITIES[spell_target_idx].wx;

                            _players[player_idx].summon_wy = _CITIES[spell_target_idx].wy;

                            _players[player_idx].summon_wp = _CITIES[spell_target_idx].wp;

                        }
                        else
                        {

                            ptr_enchantments = (uint8_t *)&_CITIES[spell_target_idx].enchantments[0];

                            enchantments_idx = spell_data_table[spell_idx].ce_idx;

                            ptr_enchantments[enchantments_idx] = (player_idx + 1);
Capture_Cities_Data();
                        }

                        if(
                            (player_idx == HUMAN_PLAYER_IDX)
                            ||
                            (
                                (magic_set.enemy_spells == ST_TRUE)
                                &&
                                (SQUARE_EXPLORED(_CITIES[spell_target_idx].wx, _CITIES[spell_target_idx].wy, _CITIES[spell_target_idx].wp))
                                &&
                                (_players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] > 0)
                            )
                        )
                        {

                            Cast_Spell_City_Enchantment_Animation_2__WIP(spell_target_idx, spell_idx, player_idx);

                        }

                        if(
                            (player_idx == HUMAN_PLAYER_IDX)
                            &&
                            (spell_idx == spl_Natures_Eye)
                        )
                        {
                            // Nature's Eye:
                            // Nature. City Enchantment. Casting Cost: 75 mana;
                            // Upkeep: 1 mana/turn. Uncommon.
                            // Extends the scouting range of a friendly target city to five squares
                            // in any direction, revealing all lands and all non-invisible enemy
                            // troops within that radius.
                            Set_Map_Square_Explored_Flags_XYP_Range(_CITIES[spell_target_idx].wx, _CITIES[spell_target_idx].wy, _CITIES[spell_target_idx].wp, 5);
                        }

                        if(spell_idx == spl_Consecration)
                        {

                            Apply_Spell_Consecration(spell_target_idx);

                        }

                    }

                } break;  /* case scc_City_Enchantment_Positive: */

                // Chaos Rift, Cursed Lands, Evil Presence, Famine, Pestilence
                case scc_City_Enchantment_Negative:  //  3
                {
                    if(player_idx != HUMAN_PLAYER_IDX)
                    {
                        /* SPELLY */  Cast_Successful = Pick_Target_For_City_Enchantment__WIP(stt_Friendly_City, &spell_target_idx, spell_idx, player_idx);
                    }
                    else
                    {

                        MultiPurpose_Local_Var = ST_FALSE;

                        Cast_Successful = ST_TRUE;

                        while((MultiPurpose_Local_Var == ST_FALSE) && (Cast_Successful == ST_TRUE))
                        {
                            MultiPurpose_Local_Var = ST_TRUE;
                            Cast_Successful = Spell_Casting_Screen__WIP(stt_Enemy_City, &spell_target_idx, &wy, &wp, &target_wy, &target_wp, (char *)&spell_name[0]);
                            if(Cast_Successful == ST_TRUE)
                            {
                                ptr_enchantments = (uint8_t *)&_CITIES[spell_target_idx].enchantments[0];
                                if(ptr_enchantments[spell_data_table[spell_idx].ce_idx] > 0)
                                {
                                    Full_Draw_Main_Screen();
                                    strcpy(GUI_NearMsgString, aThatCityAlread);  // "That city already has "
                                    strcat(GUI_NearMsgString, spell_name);
                                    strcat(GUI_NearMsgString, cnst_SpellError_2_2);  // " cast on it"
                                    Warn0(GUI_NearMsgString);
                                }
                                else
                                {
                                    if(
                                        (spell_idx == spl_Evil_Presence)
                                        &&
                                        (_CITIES[spell_target_idx].owner_idx < _num_players)
                                        &&
                                        (_players[_CITIES[spell_target_idx].owner_idx].spellranks[sbr_Death] > 0)
                                    )
                                    {
                                        Full_Draw_Main_Screen();
                                        LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 26, 1, 150);  // "Evil Presence has no effect on cities whose controller can cast Death magic."
                                        Warn0(GUI_NearMsgString);
                                    }
                                    else
                                    {
                                        MultiPurpose_Local_Var = ST_TRUE;
                                    }
                                }
                            }
                        }
                    }
                    if(Cast_Successful == ST_TRUE)
                    {
                        Cast_Successful = Apply_Automatic_Spell_Counters(spell_idx, spell_target_idx, player_idx, show_message_flag);
                    }
                    if(Cast_Successful == ST_TRUE)
                    {
                        if(
                            (player_idx == HUMAN_PLAYER_IDX)
                            ||
                            (_CITIES[spell_target_idx].owner_idx == HUMAN_PLAYER_IDX)
                            ||
                            (
                                (magic_set.enemy_spells == ST_TRUE)
                                &&
                                (SQUARE_EXPLORED(_CITIES[spell_target_idx].wx, _CITIES[spell_target_idx].wy, _CITIES[spell_target_idx].wp))
                                &&
                                (_players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] > 0)
                            )
                        )
                        {
                            AI_Eval_After_Spell = ST_TRUE;
                            Allocate_Reduced_Map();
                            Mark_Block(_screen_seg);
                            Cast_Spell_City_Enchantment_Animation_1__WIP(spell_target_idx, spell_idx, player_idx);
                        }
                        ptr_enchantments = (uint8_t *)&_CITIES[spell_target_idx].enchantments[0];
                        enchantments_idx = spell_data_table[spell_idx].ce_idx;
                        ptr_enchantments[enchantments_idx] = (player_idx + 1);  // PLAYER_NUM();
Capture_Cities_Data();
                        if(
                            (player_idx == HUMAN_PLAYER_IDX)
                            ||
                            (
                                (magic_set.enemy_spells == ST_TRUE)
                                &&
                                (SQUARE_EXPLORED(_CITIES[spell_target_idx].wx, _CITIES[spell_target_idx].wy, _CITIES[spell_target_idx].wp))
                                &&
                                (_players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] > 0)
                            )
                        )
                        {
                            Cast_Spell_City_Enchantment_Animation_2__WIP(spell_target_idx, spell_idx, player_idx);
                        }
                        Change_Relations_For_Bad_City_Spell(player_idx, spell_idx, spell_target_idx);
                    }
                } break;  /* case scc_City_Enchantment_Negative: */

                // BOTH:  Doom Bolt, Fire Storm, Ice Storm, Star Fires, Warp Lightning
                case scc_Direct_Damage_Fixed:  //  4
                {
                    
                    if(player_idx == HUMAN_PLAYER_IDX)
                    {

                        Cast_Successful = Spell_Casting_Screen__WIP(stt_Enemy_Group, &spell_target_idx, &wy, &wp, &target_wy, &target_wp, (char *)&spell_name[0]);

                    }
                    else
                    {

                        Cast_Successful = Get_Map_Square_Target_For_Spell(stt_Enemy_Group, &spell_target_idx, &wy, &wp, spell_idx, player_idx);

                    }

                    if(Cast_Successful == ST_TRUE)
                    {

                        for(itr_cities = 0; itr_cities < _cities; itr_cities++)
                        {

                            if(
                                (_CITIES[itr_cities].wx == _UNITS[spell_target_idx].wx)
                                &&
                                (_CITIES[itr_cities].wy == _UNITS[spell_target_idx].wy)
                                &&
                                (_CITIES[itr_cities].wp == _UNITS[spell_target_idx].wp)
                            )
                            {

                                Cast_Successful = Apply_Automatic_Spell_Counters(spell_idx, itr_cities, player_idx, show_message_flag);

                            }

                        }

                    }

                    if(Cast_Successful == ST_TRUE)
                    {

                        if(
                            (player_idx == HUMAN_PLAYER_IDX)
                            ||
                            (_UNITS[spell_target_idx].owner_idx == HUMAN_PLAYER_IDX)
                            ||
                            (
                                (Check_Square_Scouted(_UNITS[spell_target_idx].wx, _UNITS[spell_target_idx].wy, _UNITS[spell_target_idx].wp) != ST_FALSE)
                                &&
                                (magic_set.enemy_spells == ST_TRUE)
                            )
                        )
                        {

                            AI_Eval_After_Spell = ST_TRUE;
                            
                            Allocate_Reduced_Map();

                            Spell_Animation_Load_Sound_Effect__WIP(spell_idx);

                            Spell_Animation_Load_Graphics(spell_idx);

                            Spell_Animation_Screen__WIP(_UNITS[spell_target_idx].wx, _UNITS[spell_target_idx].wy, _UNITS[spell_target_idx].wp);

                            Full_Draw_Main_Screen();

                        }

                        /* SPELLY */  Change_Relations__WIP(-3, player_idx, _UNITS[spell_target_idx].owner_idx, 8, 0, spell_target_idx);

                        /* SPELLY */  OVL_ConvSpellAttack__WIP(spell_target_idx, spell_idx);

                    }

                } break;  /* case scc_Fixed_Dmg_Spell: */

                // Animate Dead, Black Wind, Call The Void, Change Terrain, Chaos Channels, Corruption, Cracks Call, Death Wish, Disrupt, Earth Lore, Earth to Mud, Earthquake, Enchant Road, Great Unsummoning, Healing, Incarnation, Lycanthropy, Magic Vortex, Move Fortress, Natures Cures, Plane Shift, Raise Dead, Raise Volcano, Recall Hero, Resurrection, Spell Binding, Spell Of Mastery, Spell Of Return, Spell Ward, Stasis, Summon Champion, Summon Hero, Transmute, Wall of Stone, Warp Creature, Warp Node, Warp Wood, Word of Recall
                case scc_Special_Spell:  //  5
                {

                } break;

                // WIZARD:  Cruel Unminding, Drain Power, Spell Blast, Subversion
                case scc_Target_Wiz_Spell:  //  6
                {

                } break;

                // Armageddon, Aura of Majesty, Awareness, Chaos Surge, Charm of Life, Crusade, Detect Magic, Doom Mastery, Eternal Night, Evil Omens, Great Wasting, Herb Mastery, Holy Arms, Just Cause, Life Force, Meteor Storms, Natures Awareness, Natures Wrath, Planar Seal, Suppress Magic, Time Stop, Tranquility, Wind Mastery, Zombie Mastery
                case scc_Global_Enchantment:  // 9
                {

                    AI_Eval_After_Spell = ST_TRUE;

                    WIZ_GlobalSpellAnim(player_idx, spell_idx);

                    Cast_Successful = ST_TRUE;

                    ptr_enchantments = &_players[player_idx].Globals[0];

                    enchantments_idx = spell_data_table[spell_idx].ge_idx;

                    ptr_enchantments[enchantments_idx] = (player_idx + 1);

                    if(spell_idx == spl_Time_Stop)
                    {

                        g_TimeStop_PlayerNum = (player_idx + 1);

                    }

                    if(spell_idx == spl_Natures_Awareness)
                    {

                        Cast_NaturesAwareness(player_idx);

                    }

                    if(spell_idx == spl_Holy_Arms)
                    {

                        Cast_HolyArms(player_idx);

                    }

                    if(spell_idx == spl_Crusade)
                    {

                        for(itr_units = 0; itr_units < _units; itr_units++)
                        {

                            if(_UNITS[itr_units].owner_idx == player_idx)
                            {

                                _UNITS[itr_units].Level = Calc_Unit_Level(itr_units);

                            }

                        }

                    }

                    if(spell_idx == spl_Awareness)
                    {

                        Cast_Awareness(player_idx);

                    }

                    Change_Relations_For_Enchantments(player_idx, spell_idx, 1);

                } break;

                case scc_Crafting_Spell:  // 11
                {

                } break;

                // COMBAT:  Possession, Shatter
                case scc_Mundane_Curse:  // 16
                {

                } break;

                // ¿ BOTH ?  Disenchant Area, Disenchant True
                case scc_Disenchants:  // 19
                {

                    if(player_idx != HUMAN_PLAYER_IDX)
                    {

                        Cast_Successful = Get_Map_Square_Target_For_Spell(stt_Map_Square, &spell_target_idx, &wy, &wp, spell_idx, player_idx);

                    }
                    else
                    {

                        MultiPurpose_Local_Var = ST_FALSE;

                        Cast_Successful = ST_TRUE;

                        Allocate_Reduced_Map();

                        Mark_Block(_screen_seg);

                        while((MultiPurpose_Local_Var == ST_FALSE) && (Cast_Successful == ST_TRUE))
                        {

                            Cast_Successful = Spell_Casting_Screen__WIP(stt_Map_Square, &spell_target_idx, &wy, &wp, &target_wy, &target_wp, (char *)&spell_name);

                            if(Cast_Successful == ST_TRUE)
                            {

                                MultiPurpose_Local_Var = Square_Has_Disenchant_Target(spell_target_idx, wy, wp);

                                if(MultiPurpose_Local_Var == ST_FALSE)
                                {

                                    Full_Draw_Main_Screen();

                                    LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)&GUI_NearMsgString[0], 28, 1, 150);  // "There is nothing to dispel in that area."

                                    Warn0(GUI_NearMsgString);
                                }

                            }

                        }

                    }

                    if(Cast_Successful == ST_TRUE)
                    {

                        MultiPurpose_Local_Var = ST_FALSE;


                        if(player_idx == HUMAN_PLAYER_IDX)
                        {

                            MultiPurpose_Local_Var = ST_TRUE;

                        }
                        else
                        {

                            for(itr_cities = 0; ((itr_cities < _cities) && (MultiPurpose_Local_Var == ST_FALSE)); itr_cities++)
                            {


                                if(
                                    (_CITIES[itr_cities].wx == spell_target_idx)
                                    &&
                                    (_CITIES[itr_cities].wy == wy)
                                    &&
                                    (_CITIES[itr_cities].wp == wp)
                                    
                                )
                                {

                                    if(
                                        (_CITIES[itr_units].owner_idx != HUMAN_PLAYER_IDX)
                                        ||
                                        (
                                            (magic_set.enemy_spells == ST_TRUE)
                                            &&
                                            (SQUARE_EXPLORED(_CITIES[itr_units].wx, _CITIES[itr_units].wy, _CITIES[itr_units].wp) != ST_FALSE)
                                            &&
                                            (_players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] > 0)
                                        )
                                    )
                                    {

                                        AI_Eval_After_Spell = ST_TRUE;

                                        MultiPurpose_Local_Var = ST_TRUE;

                                        Allocate_Reduced_Map();

                                        Mark_Block(_screen_seg);

                                    }

                                }

                            }

                        }

                        for(itr_units = 0; ((itr_units < _units) && (MultiPurpose_Local_Var == ST_FALSE)); itr_units++)
                        {

                            if(
                                (_UNITS[itr_units].wx == spell_target_idx)
                                &&
                                (_UNITS[itr_units].wy == wy)
                                &&
                                (_UNITS[itr_units].wp == wp)
                                
                            )
                            {

                                if(
                                    (_UNITS[itr_units].owner_idx != HUMAN_PLAYER_IDX)
                                    ||
                                    (
                                        (Check_Square_Scouted(_UNITS[itr_units].wx, _UNITS[itr_units].wy, _UNITS[itr_units].wp) != ST_FALSE)
                                        &&
                                        (magic_set.enemy_spells == ST_TRUE)
                                        &&
                                        (_players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] > 0)
                                    )
                                )
                                {

                                    MultiPurpose_Local_Var = ST_TRUE;

                                    AI_Eval_After_Spell = ST_TRUE;

                                    Allocate_Reduced_Map();

                                    Mark_Block(_screen_seg);

                                }

                            }

                        }

                    }


                    if(MultiPurpose_Local_Var == ST_TRUE)
                    {
                        AI_Eval_After_Spell = ST_TRUE;
                        Spell_Animation_Load_Sound_Effect__WIP(spell_idx);
                        Spell_Animation_Load_Graphics(spell_idx);
                        Spell_Animation_Screen__WIP(spell_target_idx, wy, wp);
                        Full_Draw_Main_Screen();
                        Release_Block(_screen_seg);
                    }

                    if(_players[player_idx].runemaster > 0)
                    {

                        _players[player_idx].casting_cost_original *= 2;

                    }

                    if(spell_idx == spl_Disenchant_True)
                    {

                        _players[player_idx].casting_cost_original *= 3;

                    }

                    Cast_Disenchant(spell_target_idx, wy, wp, player_idx, _players[player_idx].casting_cost_original);

                } break;

                // OVERLAND:  Disjunction, Disjunction True
                case scc_Disjunctions:  // 20
                {

                    if(player_idx == HUMAN_PLAYER_IDX)
                    {

                        Cast_Successful = Spell_Target_Global_Enchantment_Screen__WIP(spell_idx, player_idx);

                    }
                    else
                    {

                        /* SPELLY */  Cast_Successful = IDK_AITP_Disjunction__STUB(&spell_target_idx, &wy, spell_idx, player_idx);

                        if(Cast_Successful == ST_TRUE)
                        {

                            if(_players[player_idx].runemaster > 0)
                            {

                                _players[player_idx].casting_cost_original *= 2;

                            }

                            if(spell_idx == spl_Disjunction_True)
                            {

                                _players[player_idx].casting_cost_original *= 3;

                            }
/* SPELLY */  enchantments_idx = IDK_Get_Global_Enchant_Index__STUB(wy);
item_list = (_players[player_idx].casting_cost_original + Calculate_Dispel_Difficulty(spell_data_table[enchantments_idx].casting_cost, spell_target_idx, spell_data_table[enchantments_idx].magic_realm));
item_list = ((_players[player_idx].casting_cost_original * 250) / item_list);
if(Random(250) <= item_list)
{
    ptr_enchantments = &_players[spell_target_idx].Globals[0];
    ptr_enchantments[spell_data_table[enchantments_idx].Param0] = 0;
    Fizzle_Notification(spell_target_idx, player_idx, enchantments_idx, aDisjunction);  // "Disjunction"
}

                        }

                    }

                } break;

                default:
                {
                    STU_DEBUG_BREAK();
                } break;

            }

        }

    }



// ¿¿¿
// After Did/Didn't Cast
// ...
// Nature's Wrath
// ..
// re-init
// ???



    if(Cast_Successful == ST_TRUE)
    {
        MultiPurpose_Local_Var = ST_UNDEFINED;

        for(itr_players = 0; itr_players < _num_players; itr_players++)
        {

            if(
                (_players[itr_players].Globals[NATURES_WRATH] > 0)
                &&
                (itr_players != player_idx)
            )
            {

                MultiPurpose_Local_Var = itr_players;

            }

        }

    }


    /*
        BEGIN:  Nature's Wrath
    */
    {
        if(
            (MultiPurpose_Local_Var >= 0)
            &&
            (
                (spell_data_table[spell_idx].magic_realm == sbr_Chaos)
                ||
                (spell_data_table[spell_idx].magic_realm == sbr_Death)
            )
        )
        {

            AI_Eval_After_Spell = ST_TRUE;

            // TODO  Call_Forth_The_Force_Of_Nature

        }
    }
    /*
        END:  Nature's Wrath
    */


    _players[player_idx].casting_cost_remaining = 0;

    _players[player_idx].casting_cost_original = 0;

    _players[player_idx].casting_spell_idx = 0;

}


// WZD o135p06
// sub_BCB9E()
int16_t IDK_Get_Global_Enchant_Index__STUB(int16_t value)
{



}