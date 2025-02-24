/*
    ¿ Overland Spell Casting ?

    WIZARDS.EXE
        ovr135
*/

#include "MoX/MOX_DAT.H"  /* _screen_seg */
#include "MoX/MOX_SET.H"  /* magic_set */

#include "MoM.H"



// WZD dseg:6A42                                                 BEGIN:  ovr135 - Initialized Data

// WZD dseg:6A42 53 70 45 63 46 78 00                            cnst_SPECFX_FileA db 'SpEcFx',0         ; DATA XREF: IDK_Cast_Disenchant_True+15o ...
// WZD dseg:6A42                                                                                         ; should use dseg:5e17
// WZD dseg:6A49 20 68 61 73 20 62 65 65 6E 20 64 69 73 70 65 6C+cnst_Dispel_Msg_2 db ' has been dispelled.',0 ; should use dseg:684b

// WZD dseg:6A5E
char message_lbx_file__ovr135[] = "message";

// WZD dseg:6A66
char str_SuppressMagic[] = "Suppress Magic";
// WZD dseg:6A75
char str_Tranquility[] = "Tranquility";
// WZD dseg:6A81
char str_LifeForce[] = "Life Force";

// WZD dseg:6A8C 54 68 61 74 20 75 6E 69 74 20 61 6C 72 65 61 64+cnst_SpellError_1_2 db 'That unit already has ',0
// WZD dseg:6A8C 79 20 68 61 73 20 00                                                                    ; DATA XREF: Cast_Spell_Overland__WIP+6F8o
// WZD dseg:6A8C                                                                                         ; should use dseg:5b48
// WZD dseg:6AA3 20 63 61 73 74 20 6F 6E 20 69 74 00             cnst_SpellError_2_2 db ' cast on it',0  ; DATA XREF: Cast_Spell_Overland__WIP+716o ...
// WZD dseg:6AA3                                                                                         ; should use dseg:5b5f
// WZD dseg:6AAF 4F 6E 6C 79 20 6E 6F 72 6D 61 6C 20 75 6E 69 74+aOnlyNormalUnit db 'Only normal units may have ',0
// WZD dseg:6AAF 73 20 6D 61 79 20 68 61 76 65 20 00                                                     ; DATA XREF: Cast_Spell_Overland__WIP+87Ao
// WZD dseg:6ACB 20 63 61 73 74 20 6F 6E 20 74 68 65 6D 00       aCastOnThem db ' cast on them',0        ; DATA XREF: Cast_Spell_Overland__WIP+898o ...
// WZD dseg:6AD9 4F 6E 6C 79 20 6E 6F 72 6D 61 6C 20 75 6E 69 74+aOnlyNormalUn_0 db 'Only normal units and created undead may have ',0
// WZD dseg:6AD9 73 20 61 6E 64 20 63 72 65 61 74 65 64 20 75 6E+                                        ; DATA XREF: Cast_Spell_Overland__WIP+C40o
// WZD dseg:6B08 54 68 61 74 20 63 69 74 79 20 61 6C 72 65 61 64+aThatCityAlread db 'That city already has ',0
// WZD dseg:6B08 79 20 68 61 73 20 00                                                                    ; DATA XREF: Cast_Spell_Overland__WIP+EB0o ...
// WZD dseg:6B1F 20 73 70 65 6C 6C 20 68 61 73 20 66 61 69 6C 65+cnst_SpellError_4 db ' spell has failed.',0
// WZD dseg:6B1F 64 2E 00                                                                                ; DATA XREF: Cast_Spell_Overland__WIP+1825o
// WZD dseg:6B32 53 70 65 6C 6C 20 42 6C 61 73 74 00             aSpellBlast db 'Spell Blast',0
// WZD dseg:6B3E 44 69 73 6A 75 6E 63 74 69 6F 6E 00             aDisjunction db 'Disjunction',0         ; DATA XREF: Cast_Spell_Overland__WIP+1E1Fo
// WZD dseg:6B4A 20 74 68 65 20                                  aThe_0 db ' the '

// WZD dseg:6B4A                                                 END:  ovr135 - Initialized Data



/*
    WIZARDS.EXE  ovr135
*/

// WZD o135p01
// SPL_Get_CityE_Index()

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
// sub_BA3A4()

// WZD o135p04
// sub_BAB5D()


// WZD o135p05
// drake178: ¿ G_OVL_Cast() ?
/*
*/
/*

*/
void Cast_Spell_Overland__WIP(int16_t player_idx)
{
    char spell_name[LEN_SPELL_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t show_message_flag = 0;
    int16_t var_1A = 0;
    int16_t item_list = 0;
    int16_t MultiPurpose_Local_Var = 0;
    int16_t Cast_Successful = 0;
    int16_t var_12 = 0;
    int16_t var_10 = 0;
    int16_t RetP = 0;
    int16_t RetY = 0;
    int16_t G_Have_Targets = 0;
    int16_t itr_players = 0;  // itr
    int16_t itr_cities = 0;  // itr
    int16_t itr_units = 0;  // itr
    int16_t Dispel_Chance = 0;
    int16_t players_globals_idx = 0;  // DNE in Dasm, uses Dispel_Chance
    uint8_t * ptr_players_globals = 0;
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


    String_Copy_Far(spell_name, spell_data_table[spell_idx].name);


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

                Dispel_Chance = (500 + Calculate_Dispel_Difficulty(_players[player_idx].casting_cost_original, player_idx, spell_data_table[spell_idx].magic_realm));

                Dispel_Chance = (125000 / Dispel_Chance);

                if(Random(250) <= Dispel_Chance)
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

                    Dispel_Chance = (500 + Calculate_Dispel_Difficulty(_players[player_idx].casting_cost_original, player_idx, spell_data_table[spell_idx].magic_realm));

                    Dispel_Chance = (125000 / Dispel_Chance);

                    if(Random(250) <= Dispel_Chance)
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

                    Dispel_Chance = (500 + Calculate_Dispel_Difficulty(_players[player_idx].casting_cost_original, player_idx, spell_data_table[spell_idx].magic_realm));

                    Dispel_Chance = (125000 / Dispel_Chance);

                    if(Random(250) <= Dispel_Chance)
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

            // TODO  Cast_Spell_Of_Mastery(player_idx);

            AI_Eval_After_Spell = ST_TRUE;
            
        }
        else
        {

// enum enum_SPELL_DATA_TYPE
// sdt_Summoning         =  0,
// Unit_Enchantment        =  1,
// City_Enchantment        =  2,
// City_Curse              =  3,
// Fixed_Dmg_Spell         =  4,
// Special_Spell           =  5,
// Target_Wiz_Spell        =  6,
// 7 DNE
// 8 DNE
// sdt_Global_Enchantment  =  9,
// Battlefield_Spell       = 10,
// sdt_Crafting_Spell      = 11,
// Destruction_Spell       = 12,
// Resistable_Spell        = 13,
// No_Resist_Spell         = 14,
// Mundane_Enchantment     = 15,
// Mundane_Curse           = 16,
// sdt_Infusable_Spell     = 17,
// Dispel_Spell            = 18,
// Disenchant_Spell        = 19,
// Disjunction_Spell       = 20,
// Counter_Spell           = 21,
// Var_Dmg_Spell           = 22,
// Banish_Spell            = 23

// ovr135:28B4 95 0C 31 10 59 17 77 1B 3D 0E 55 1F 3A 21 FA 27+jt_SpellDataType dw offset sdt_Summon   ; DATA XREF: Cast_Spell_Overland__WIP+2CCr
// ovr135:28B4 FA 27 77 1E FA 27 0F 1B FA 27 FA 27 FA 27 31 10+dw offset Unit                          ; jump table for switch statement
// ovr135:28B4 FA 27 3A 21 FA 27 3E 23 AB 26                   dw offset City
// ovr135:28B4                                                 dw offset CityCurse
// ovr135:28B4                                                 dw offset FixDmg
// ovr135:28B4                                                 dw offset Special
// ovr135:28B4                                                 dw offset sdt_Target_Wizard
// ovr135:28B4                                                 dw offset Nothing
// ovr135:28B4                                                 dw offset Nothing
// ovr135:28B4                                                 dw offset Global
// ovr135:28B4                                                 dw offset Nothing
// ovr135:28B4                                                 dw offset Crafting
// ovr135:28B4                                                 dw offset Nothing
// ovr135:28B4                                                 dw offset Nothing
// ovr135:28B4                                                 dw offset Nothing
// ovr135:28B4                                                 dw offset Unit
// ovr135:28B4                                                 dw offset Nothing
// ovr135:28B4                                                 dw offset sdt_Target_Wizard
// ovr135:28B4                                                 dw offset Nothing
// ovr135:28B4                                                 dw offset DE
// ovr135:28B4                                                 dw offset Disj

// ovr135:28B4 95 0C 31 10 59 17 77 1B 3D 0E 55 1F 3A 21 FA 27+jt_SpellDataType dw offset jt_sdt_00    ; DATA XREF: Cast_Spell_Overland__WIP+2CCr
// ovr135:28B4 FA 27 77 1E FA 27 0F 1B FA 27 FA 27 FA 27 31 10+dw offset jt_sdt_01_15                  ; jump table for switch statement
// ovr135:28B4 FA 27 3A 21 FA 27 3E 23 AB 26                   dw offset jt_sdt_02
// ovr135:28B4                                                 dw offset jt_sdt_03
// ovr135:28B4                                                 dw offset jt_sdt_04
// ovr135:28B4                                                 dw offset jt_sdt_05
// ovr135:28B4                                                 dw offset jt_sdt_06_17
// ovr135:28B4                                                 dw offset jt_sdt_07_08_10_12_13_14_16_18
// ovr135:28B4                                                 dw offset jt_sdt_07_08_10_12_13_14_16_18
// ovr135:28B4                                                 dw offset jt_sdt_09
// ovr135:28B4                                                 dw offset jt_sdt_07_08_10_12_13_14_16_18
// ovr135:28B4                                                 dw offset jt_sdt_11
// ovr135:28B4                                                 dw offset jt_sdt_07_08_10_12_13_14_16_18
// ovr135:28B4                                                 dw offset jt_sdt_07_08_10_12_13_14_16_18
// ovr135:28B4                                                 dw offset jt_sdt_07_08_10_12_13_14_16_18
// ovr135:28B4                                                 dw offset jt_sdt_01_15
// ovr135:28B4                                                 dw offset jt_sdt_07_08_10_12_13_14_16_18
// ovr135:28B4                                                 dw offset jt_sdt_06_17
// ovr135:28B4                                                 dw offset jt_sdt_07_08_10_12_13_14_16_18
// ovr135:28B4                                                 dw offset jt_sdt_19
// ovr135:28B4                                                 dw offset jt_sdt_20

// ptr_players_globals = &_players[player_idx].Globals[0];

/*
    IDA colors
        sdt_Summoning          ( 0)  #24 reddish-brown

        sdt_Global_Enchantment ( 9)  #13 ~ blue, greyish/greenish

        sdt_Crafting_Spell     (11)  #17 brown

*/

            switch(spell_data_table[spell_idx].type)
            {
                case sdt_Summoning:
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

                            // TODO  Cast_FloatingIsland(player_idx);

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
                                    (magic_set.Enemy_Spells == ST_TRUE)
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

                            LBX_Load_Data_Static(message_lbx_file__ovr135, 0, (SAMB_ptr)GUI_NearMsgString, 21, 1, 150);  // "Maximum unit count exceeded.  Summons fails."

                            Warn0(GUI_NearMsgString);

                        }

                        Cast_Successful = ST_FALSE;

                    }

                } break;

                case sdt_Global_Enchantment:
                {

                    AI_Eval_After_Spell = ST_TRUE;

                    WIZ_GlobalSpellAnim(player_idx, spell_idx);

                    Cast_Successful = ST_TRUE;

                    ptr_players_globals = &_players[player_idx].Globals[0];

                    players_globals_idx = spell_data_table[spell_idx].Param0;

                    ptr_players_globals[players_globals_idx] = (player_idx + 1);

                    if(spell_idx == spl_Time_Stop)
                    {

                        g_TimeStop_PlayerNum = (player_idx + 1);

                    }

                    if(spell_idx == spl_Natures_Awareness)
                    {

                        // TODO  Cast_NaturesAwareness(player_idx);

                    }

                    if(spell_idx == spl_Holy_Arms)
                    {

                        // TODO  Cast_HolyArms(player_idx);

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

                        // TODO  Cast_Awareness(player_idx);

                    }

                    Change_Relations_For_Enchantments(player_idx, spell_idx, 1);

                } break;

                case sdt_Crafting_Spell:
                {

                } break;

            }

        }

    }


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
