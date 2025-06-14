/*
    WIZARDS.EXE
        ovr131
*/

#include "MOX/GENDRAW.H"
#include "MOM_DEF.H"
#include "MOX/Fonts.H"
#include "MOX/MOM_Data.H"
#include "MOX/MOX_DAT.H"
#include "MOX/MOM_Data.H"
#include "MOX/SOUND.H"
#include "MOX/Video.H"
#include "RACETYPE.H"
#include "Spellbook.H"
#include "Spells131.H"


#include "Combat.H"
#include "SBookScr.H"
#include "Spells133.H"
#include "UNITTYPE.H"



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
// drake178: CMB_Disenchant()
void Combat_Cast_Disenchant(int16_t caster_idx, int16_t strength)
{
    int16_t spells[15] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t enemy_player_idx = 0;
    int16_t player_idx = 0;
    SAMB_ptr dispell2_notify_background_seg = 0;
    int16_t notify_count = 0;
    int8_t * ptr_enchantments = 0;
    int16_t spell_idx = 0;
    int16_t itr2 = 0;
    int16_t Enchant_Offset = 0;
    int16_t itr1 = 0;  // _SI_
    int16_t threshold = 0;  // _DI_

    spells[0] = spl_True_Light;
    spells[1] = spl_Darkness;
    spells[2] = spl_Warp_Reality;
    spells[3] = spl_Black_Prayer;
    spells[4] = spl_Wrack;
    spells[5] = spl_Metal_Fires;
    spells[6] = spl_Prayer;
    spells[7] = spl_High_Prayer;
    spells[8] = spl_Terror;
    spells[9] = spl_Call_Lightning;
    spells[10] = spl_Counter_Magic;
    spells[11] = spl_Mass_Invisibility;
    spells[12] = spl_Entangle;
    spells[13] = spl_Mana_Leak;
    spells[14] = spl_Blur;

    if(caster_idx >= CASTER_IDX_BASE)
    {
        player_idx = (caster_idx - CASTER_IDX_BASE);
    }
    else
    {
        // ; BUG: this is NOT the caster ID
        player_idx = battle_units[itr1].controller_idx;
    }

    notify_count = 0;

    Copy_On_To_Off_Page();

    if(player_idx == _combat_attacker_player)
    {
        // WTF  mov     [bp+Enchant_Offset], s_COMBAT_ENCHANTMENT_STATUS.Dfndr
        // Enchant_Offset = struct s_COMBAT_ENCHANTMENT_STATUS.Dfndr;
        Enchant_Offset = 1;
        enemy_player_idx = _combat_defender_player;
    }
    else
    {
        Enchant_Offset = 0;
        enemy_player_idx = _combat_attacker_player;
    }

    // ; BUG: neutral units can cast Combat Enchantments too
    if(enemy_player_idx != NEUTRAL_PLAYER_IDX)
    {
        // ; process any Combat Enchantments that the opponent
        // ; has active
        // ; BUG: dispelling Mass Invisibility will remove all
        // ; combat-cast Invisibility effects indiscriminately

        for(itr1 = 0; itr1 < 30; itr1++)
        {

            // if(*(combat_enchantments + itr1 + Enchant_Offset) > 0)
            if(combat_enchantments[(itr1 + Enchant_Offset)] > 0)
            {

                spell_idx = spells[itr1];

                threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[spell_idx].casting_cost, enemy_player_idx, spell_data_table[spell_idx].magic_realm));

                threshold = ((strength * 250) / threshold);

                if(Random(250) <= (threshold + 500))
                {

                    if(itr1 == 22)
                    {
                        // ; remove combat-cast Invisibility from all opposing
                        // ; units
                        // ; BUG: will also remove those that were cast
                        // ; individually instead of placed by Mass Invisibility

                    }

                    combat_enchantments[(itr1 + Enchant_Offset)] = 0;

                    Mark_Block(_screen_seg);

                    // SPECFX.LBX, 052  "DISPELL2"  ""
                    dispell2_notify_background_seg = LBX_Reload_Next(specfx_lbx_file__ovr131__2of2, 51, _screen_seg);

                    if(notify_count < 5)
                    {

                        if(notify_count < 4)
                        {

                            _fstrcpy(GUI_NearMsgString, spell_data_table[spell_idx].name);

                            strcat(GUI_NearMsgString, cnst_Dispel_Msg);  // " has been dispelled."

                        }
                        else
                        {

                            LBX_Load_Data_Static(message_lbx_file__ovr131, 0, (SAMB_ptr)&GUI_NearMsgString[0], 45, 1, 150);

                        }

                        Notify2((160 + (notify_count * 10)), (40 + (notify_count * 25)), 3, GUI_NearMsgString, 0, dispell2_notify_background_seg, 0, 8, 0, 0, 0, 1, 0);

                        notify_count += 1;

                    }

                    Release_Block(_screen_seg);

                }

            }

        }

    }

    // ; check if the battle is taking place in a warped node
    // ; owned by the caster, and if so, attempt to dispel
    // ; the Warp Node effect
    // ; WARNING: the effect has no owner, and thus its dispel
    // ;  resistance can't be improved
    // ; INCONSISTENT: there is no dispel message
    for(itr1 = 0; itr1 < NUM_NODES; itr1++)
    {

        // ; should really use jnz
        if(
            ((_NODES[itr1].flags & NF_WARPED) > 0)
            &&
            (_NODES[itr1].owner_idx == player_idx)
            &&
            (_NODES[itr1].wx == _combat_wx)
            &&
            (_NODES[itr1].wy == _combat_wy)
            &&
            (_NODES[itr1].wp == _combat_wp)
        )
        {

            spell_idx = spl_Warp_Node;

            threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[spell_idx].casting_cost, enemy_player_idx, spell_data_table[spell_idx].magic_realm));

            threshold = ((strength * 250) / threshold);

            if(Random(250) <= (threshold + 500))
            {

                _NODES[itr1].flags ^= NF_WARPED;

            }
            
        }

    }

    // ; attempt to dispel every Magic Vortex that does not
    // ; belong to the caster
    // ; BUG: this will cause all Vortices that are on the
    // ;  same tile as a unit to be targeted twice
    // ; INCONSISTENT: Dispel Magic does not check owners
    // ; INCONSISTENT: there is no dispel message
    for(itr1 = 0; itr1 < CMB_Vortex_Count; itr1++)
    {

        if(CMB_Vortex_Array[itr1].owner_idx == player_idx)
        {

            spell_idx = spl_Magic_Vortex;

            threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[spell_idx].casting_cost, enemy_player_idx, spell_data_table[spell_idx].magic_realm));

            threshold = ((strength * 250) / threshold);

            if(Random(250) <= (threshold + 500))
            {

                Delete_Structure(itr1, (uint8_t *)&CMB_Vortex_Array[0], sizeof(struct s_MAGIC_VORTEX), CMB_Vortex_Count);

                CMB_Vortex_Count--;

            }
            
        }

    }

    // ; attempt to dispel every existing city enchantment
    // ; that was not placed by the caster
    // ; BUG: does not recognize combat-cast Wall spells
    // ; BUG: uses the wrong costs for overland-cast Wall of
    // ;  Fire and Wall of Darkness
    // ; BUG: the loop includes Nightshades, which return no
    // ;  spell index to work with
    ptr_enchantments = (int8_t *)&battlefield->city_enchantments;

    // ; BUG: also includes Nightshades - not an enchantment

    for(Enchant_Offset = 0; Enchant_Offset < NUM_CITY_ENCHANTMENTS; Enchant_Offset++)
    {

        if(
            (ptr_enchantments[Enchant_Offset] > 0)
            &&
            (ptr_enchantments[Enchant_Offset] == (player_idx + 1))
        )
        {

            spell_idx = Get_Spell_For_City_Enchantment(Enchant_Offset);

            threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[spell_idx].casting_cost, enemy_player_idx, spell_data_table[spell_idx].magic_realm));

            threshold = ((strength * 250) / threshold);

            if(Random(250) <= (threshold + 500))
            {

                ptr_enchantments[Enchant_Offset] = 0;

                Mark_Block(_screen_seg);

                // SPECFX.LBX, 052  "DISPELL2"  ""
                dispell2_notify_background_seg = LBX_Reload_Next(specfx_lbx_file__ovr131__2of2, 51, _screen_seg);

                if(spell_idx == spl_Wall_Of_Fire)
                {

                    battlefield->Wall_Of_Fire = 0;

                }

                if(spell_idx == spl_Wall_Of_Darkness)
                {
                    
                    battlefield->Wall_Of_Darkness = 0;

                }

                if(notify_count < 5)
                {

                    if(notify_count < 4)
                    {

                        _fstrcpy(GUI_NearMsgString, spell_data_table[spell_idx].name);

                        strcat(GUI_NearMsgString, cnst_Dispel_Msg);  // " has been dispelled."

                    }
                    else
                    {

                        LBX_Load_Data_Static(message_lbx_file__ovr131, 0, (SAMB_ptr)&GUI_NearMsgString[0], 46, 1, 150);

                    }

                    Notify2((160 + (notify_count * 10)), (40 + (notify_count * 25)), 3, GUI_NearMsgString, 0, dispell2_notify_background_seg, 0, 8, 0, 0, 0, 1, 0);

                    notify_count += 1;

                }

                Release_Block(_screen_seg);

            }

        }

    }

    // ; call TILE_DispelMagic for every tile with an active
    // ; unit on it
    // ; BUG: can attempt to dispel Magic Vortices twice if
    // ;  they are on the same tile as an active unit
    // ; BUGs: iherits everything from TILE_DispelMagic

    notify_count = 0;
    for(itr1 = 0; itr1 < _combat_total_unit_count; itr1++)
    {

        if(battle_units[itr1].status == bus_Active)
        {

            Combat_Cast_Dispel(battle_units[itr1].cgx, battle_units[itr1].cgy, caster_idx, strength, &notify_count);

        }

    }

}


// WZD o131p02
// drake178: TILE_DispelMagic()
/*
; attempts to dispel, from a combat tile, persistent
; spells cast by the opponent, and Magic Votrices cast
; by anyone, producing click-away GUI messages for each
; successful dispel, except for those of Vortices
; contains an extreme amount of BUGs - surprisingly,
;  none of them can corrupt memory or cause crashes
*/
/*
    'Dispel Magic' & 'Dispel Magic True'

    in-outs a count of magics dispelled

scc_Dispels        = 18,   / * COMBAT:  Dispel Magic, Dispel Magic True * /

¿ code-blocks ?

battle_units[battle_unit_idx].controller_idx == player_idx
0-14
battle_units[battle_unit_idx].Combat_Effects

(battle_units[battle_unit_idx].controller_idx != player_idx)
'Spell Lock'
OR
1-31
if((battle_units[battle_unit_idx].enchantments & test) > 0)

Haste

Confusion (state 2)

*/
void Combat_Cast_Dispel(int16_t cgx, int16_t cgy, int16_t caster_idx, int16_t strength, int16_t * notify_count)
{
    SAMB_ptr dispell1_notify_background_seg = 0;
    int16_t enemy_player_idx = 0;
    int16_t threshold = 0;
    int16_t Test_Flag = 0;
    uint32_t test = 0;
    int16_t Flag_Loop_Var = 0;
    int16_t itr = 0;
    int16_t player_idx = 0;
    int16_t battle_unit_idx = 0;  // _DI_

    Near_Allocate_Mark();

    CMB_NearDispel_UEs = (int16_t *)Near_Allocate_Next(64);

    CMB_NearDispel_UCs = (int16_t *)Near_Allocate_Next(30);

    CMB_NearDispel_UEs[ 0] = spl_Immolation;
    CMB_NearDispel_UCs[ 0] = spl_Vertigo;
    CMB_NearDispel_UEs[ 1] = spl_Guardian_Wind;
    CMB_NearDispel_UCs[ 1] = spl_Confusion;
    CMB_NearDispel_UEs[ 2] = spl_Berserk;
    CMB_NearDispel_UCs[ 2] = spl_NONE;
    CMB_NearDispel_UEs[ 3] = spl_Cloak_Of_Fear;
    CMB_NearDispel_UCs[ 3] = spl_Mind_Storm;
    CMB_NearDispel_UEs[ 4] = spl_Black_Channels;
    CMB_NearDispel_UCs[ 4] = spl_Shatter;
    CMB_NearDispel_UEs[ 5] = spl_Wraith_Form;
    CMB_NearDispel_UCs[ 5] = spl_Weakness;
    CMB_NearDispel_UEs[ 6] = spl_Regeneration;
    CMB_NearDispel_UCs[ 6] = spl_Black_Sleep;
    CMB_NearDispel_UEs[ 7] = spl_Path_Finding;
    CMB_NearDispel_UCs[ 7] = spl_Warp_Creature;
    CMB_NearDispel_UEs[ 8] = spl_Water_Walking;
    CMB_NearDispel_UCs[ 8] = spl_Warp_Creature;
    CMB_NearDispel_UEs[ 9] = spl_Resist_Elements;
    CMB_NearDispel_UCs[ 9] = spl_Warp_Creature;
    CMB_NearDispel_UEs[10] = spl_Elemental_Armor;
    CMB_NearDispel_UCs[10] = spl_NONE;
    CMB_NearDispel_UEs[11] = spl_Stone_Skin;
    CMB_NearDispel_UCs[11] = spl_Haste;
    CMB_NearDispel_UEs[12] = spl_Iron_Skin;
    CMB_NearDispel_UCs[12] = spl_Web;
    CMB_NearDispel_UEs[13] = spl_Endurance;
    CMB_NearDispel_UCs[13] = spl_Creature_Binding;

    CMB_NearDispel_UEs[14] = spl_Spell_Lock;
    CMB_NearDispel_UEs[15] = spl_Invisibility;
    CMB_NearDispel_UEs[16] = spl_Wind_Walking;
    CMB_NearDispel_UEs[17] = spl_Flight;
    CMB_NearDispel_UEs[18] = spl_Resist_Magic;
    CMB_NearDispel_UEs[19] = spl_Magic_Immunity;
    CMB_NearDispel_UEs[20] = spl_Flame_Blade;
    CMB_NearDispel_UEs[21] = spl_Eldritch_Weapon;
    CMB_NearDispel_UEs[22] = spl_True_Sight;
    CMB_NearDispel_UEs[23] = spl_Holy_Weapon;
    CMB_NearDispel_UEs[24] = spl_Heroism;
    CMB_NearDispel_UEs[25] = spl_Bless;
    CMB_NearDispel_UEs[26] = spl_Lionheart;
    CMB_NearDispel_UEs[27] = spl_Giant_Strength;
    CMB_NearDispel_UEs[28] = spl_Planar_Travel;
    CMB_NearDispel_UEs[29] = spl_Holy_Armor;
    CMB_NearDispel_UEs[30] = spl_Righteousness;
    CMB_NearDispel_UEs[31] = spl_Invulnerability;

    Copy_On_To_Off_Page();

    if(caster_idx >= CASTER_IDX_BASE)
    {
        player_idx -= CASTER_IDX_BASE;
    }
    else
    {

        player_idx = battle_units[battle_unit_idx].controller_idx;

    }

    if(player_idx == _combat_attacker_player)
    {
        enemy_player_idx = _combat_defender_player;
    }
    else
    {
        enemy_player_idx = _combat_attacker_player;
    }

    // ; attempt to dispel all Magic Vortices from the tile
    // ; WARNING: will also affect the caster's own!
    for(itr = 0; itr < CMB_Vortex_Count; itr++)
    {
        /* SPELLY  - dispell magic vortex */
    }

    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {

        if(
            (battle_units[battle_unit_idx].cgx == cgx)
            &&
            (battle_units[battle_unit_idx].cgy == cgy)
        )
        {

            // ; BUG: non-active units are not excluded!
            if(battle_units[battle_unit_idx].controller_idx == player_idx)
            {
                // attempt to remove all curses from the unit
                // BUG: includes Possession and Creature Binding
                // BUG: overrides the result to always succeed
                // BUG: can still dispel other curses from confused
                //  state 2 units
                // WARNING: dispelling Web does not restore any lost
                //  movement types

                // ; BUG: also includes Creature Binding
                for(Flag_Loop_Var = 0; Flag_Loop_Var < 14; Flag_Loop_Var++)
                {

                    Test_Flag = (1 << Flag_Loop_Var);

                    if((battle_units[battle_unit_idx].Combat_Effects & Test_Flag) > 0)
                    {

                        if(
                            (Test_Flag != bue_Haste)
                            &&
                            (
                                (Test_Flag != bue_Haste)
                                ||
                                (battle_units[battle_unit_idx].Confusion_State != 2)
                            )
                        )
                        {

                            threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[CMB_NearDispel_UCs[Flag_Loop_Var]].casting_cost, enemy_player_idx, spell_data_table[CMB_NearDispel_UCs[Flag_Loop_Var]].magic_realm));

                            threshold = ((strength * 250) / threshold);

                            // ; BUG: will always yield a success, making the whole
                            // ; check redundant
                            if(Random(250) <= (threshold + 500))
                            {

                                if(Test_Flag == bue_Web)
                                {

                                    battle_units[battle_unit_idx].Web_HP = 0;

                                }

                                Mark_Block(_screen_seg);

                                // SPECFX.LBX, 051  "DISPELL1"  ""
                                dispell1_notify_background_seg = LBX_Reload_Next(specfx_lbx_file__ovr131__2of2, 51, _screen_seg);

                                battle_units[battle_unit_idx].Combat_Effects ^= Test_Flag;

                                if(*notify_count < 5)
                                {

                                    if(*notify_count < 4)
                                    {

                                        _fstrcpy(GUI_NearMsgString, spell_data_table[CMB_NearDispel_UCs[Flag_Loop_Var]].name);

                                        strcat(GUI_NearMsgString, cnst_Dispel_Msg);  // " has been dispelled."

                                    }
                                    else
                                    {

                                        LBX_Load_Data_Static(message_lbx_file__ovr131, 0, (SAMB_ptr)&GUI_NearMsgString[0], 19, 1, 150);

                                    }

                                    threshold = (160 + (*notify_count * 10));

                                    Notify2(threshold, (20 + (*notify_count * 25)), 3, GUI_NearMsgString, 0, dispell1_notify_background_seg, 0, 8, 0, 0, 0, 1, 0);

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
            
                // ; BUG: ignores combat-cast Spell Lock
                if((_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments & UE_SPELLLOCK) != 0)
                {

                    threshold = (strength + Calculate_Dispel_Difficulty(150, battle_units[battle_unit_idx].controller_idx, spell_data_table[spl_Spell_Lock].magic_realm));

                    threshold = ((strength * 250) / threshold);

                    // ; BUG: Haste is not protected by Spell Lock
                    if(Random(250) <= threshold)
                    {
                        
                        _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments ^= Test_Flag;

                    }

                }
                else
                {

                    // ; attempt to remove all overland-cast unit enchantments
                    // ; from the unit
                    // ; BUG: uses signed comparisons - Invulnerability can
                    // ;  not be dispelled
                    // ; BUG: uses the wrong costs for overland-only spells

                    // ; BUG: will ignore the highest order bit
                    for(Flag_Loop_Var = 0; Flag_Loop_Var < 31; Flag_Loop_Var++)
                    {

                        test = (1 << Flag_Loop_Var);

                        // ; BUG: signed comparison on a bitfield!
                        if((_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments & test) > 0)
                        {

                            // ; BUG: this is not the right value if the spell can
                            // ; not be cast in combat
                            threshold = (strength + Calculate_Dispel_Difficulty((spell_data_table[CMB_NearDispel_UCs[Flag_Loop_Var]].casting_cost * 5), battle_units[battle_unit_idx].controller_idx, spell_data_table[CMB_NearDispel_UEs[Flag_Loop_Var]].magic_realm));

                            threshold = ((strength * 250) / threshold);

                            if(Random(250) <= threshold)
                            {
                                
                                _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments ^= Test_Flag;

                                // ; BUG: this should be done elsewhere - if the unit has
                                // ; natural flight too, that will also be removed here
                                if(Flag_Loop_Var == 24)
                                {

                                    _UNITS[battle_units[battle_unit_idx].unit_idx].Level = Calc_Unit_Level(battle_units[battle_unit_idx].unit_idx);

                                }
                                
                                // ; BUG: this should be done elsewhere - if the unit has
                                // ; natural flight too, that will also be removed here
                                if(Flag_Loop_Var == 17)
                                {

                                    battle_units[battle_unit_idx].Move_Flags ^= MV_FLYING;

                                }

                                if(*notify_count < 5)
                                {

                                    Mark_Block(_screen_seg);

                                    // SPECFX.LBX, 051  "DISPELL1"  ""
                                    dispell1_notify_background_seg = LBX_Reload_Next(specfx_lbx_file__ovr131__2of2, 51, _screen_seg);

                                    if(*notify_count < 4)
                                    {

                                        _fstrcpy(GUI_NearMsgString, spell_data_table[CMB_NearDispel_UCs[Flag_Loop_Var]].name);

                                        strcat(GUI_NearMsgString, cnst_Dispel_Msg);  // " has been dispelled."

                                    }
                                    else
                                    {

                                        LBX_Load_Data_Static(message_lbx_file__ovr131, 0, (SAMB_ptr)&GUI_NearMsgString[0], 19, 1, 150);

                                    }

                                    Notify2((160 + (*notify_count * 10)), (20 + (*notify_count * 25)), 3, GUI_NearMsgString, 0, dispell1_notify_background_seg, 0, 8, 0, 0, 0, 1, 0);

                                    *notify_count += 1;
                                    
                                    Release_Block(_screen_seg);
                                }

                            }

                        }

                    }
                                        
                    // ; attempt to remove all combat-cast unit enchantments
                    // ; from the unit
                    // ; BUG: uses signed comparisons - Invulnerability grants
                    // ;  immunity to the entire effect
                    // ; BUG: ignores combat-cast Invisibility if Mass
                    // ;  Invisibility is also in effect

                    // ; BUG: will ignore the highest order bit
                    for(Flag_Loop_Var = 0; Flag_Loop_Var < 31; Flag_Loop_Var++)
                    {

                        test = (1 << Flag_Loop_Var);

                        if(enemy_player_idx == _combat_attacker_player)
                        {

                            // ; BUG: will ignore an actual combat-cast invisibility
                            if(
                                (combat_enchantments[MASS_INVISIBILITY_ATTKR] > 0)
                                &&
                                (test == UE_INVISIBILITY)
                            )
                            {

                                continue;

                            }

                            if(
                                (test == UE_HOLYWEAPON)
                                &&
                                (_players[player_idx].Globals[HOLY_ARMS] > 0)
                            )
                            {

                                continue;

                            }

                            // ; BUG: signed comparison on a bitfield!
                            if((battle_units[battle_unit_idx].enchantments & test) > 0)
                            {

                                threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[CMB_NearDispel_UCs[Flag_Loop_Var]].casting_cost, battle_units[battle_unit_idx].controller_idx, spell_data_table[CMB_NearDispel_UEs[Flag_Loop_Var]].magic_realm));

                                threshold = ((strength * 250) / threshold);

                                if(Random(250) <= threshold)
                                {

                                    Mark_Block(_screen_seg);

                                    // SPECFX.LBX, 051  "DISPELL1"  ""
                                    dispell1_notify_background_seg = LBX_Reload_Next(specfx_lbx_file__ovr131__2of2, 51, _screen_seg);

                                    _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments ^= Test_Flag;  // turn off the enchantment

                                    // ; BUG: this should be done elsewhere - if the unit has
                                    // ; natural flight too, that will also be removed here
                                    if(Flag_Loop_Var == 24)  // CMB_NearDispel_UEs[24] = spl_Heroism;
                                    {

                                        _UNITS[battle_units[battle_unit_idx].unit_idx].Level = Calc_Unit_Level(battle_units[battle_unit_idx].unit_idx);

                                    }
                                    
                                    // ; BUG: this should be done elsewhere - if the unit has
                                    // ; natural flight too, that will also be removed here
                                    if(Flag_Loop_Var == 17)  // CMB_NearDispel_UEs[17] = spl_Flight;
                                    {

                                        battle_units[battle_unit_idx].Move_Flags ^= MV_FLYING;

                                    }

                                    if(*notify_count < 5)
                                    {

                                        if(*notify_count < 4)
                                        {

                                            _fstrcpy(GUI_NearMsgString, spell_data_table[CMB_NearDispel_UCs[Flag_Loop_Var]].name);

                                            strcat(GUI_NearMsgString, cnst_Dispel_Msg);  // " has been dispelled."

                                        }
                                        else
                                        {

                                            LBX_Load_Data_Static(message_lbx_file__ovr131, 0, (SAMB_ptr)&GUI_NearMsgString[0], 19, 1, 150);

                                        }

                                        Notify2((160 + (*notify_count * 10)), (20 + (*notify_count * 25)), 3, GUI_NearMsgString, 0, dispell1_notify_background_seg, 0, 8, 0, 0, 0, 1, 0);

                                        *notify_count += 1;

                                        Release_Block(_screen_seg);
                                    }

                                }

                            }

                        }

                    }

                }

                /*
                    BEGIN:  Haste
                */

                // ; attempt to remove Haste if present
                // ; BUG: not protected by Spell Lock

                if(battle_units[battle_unit_idx].Combat_Effects == bue_Haste)
                {

                    threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[spl_Haste].casting_cost, battle_units[battle_unit_idx].controller_idx, spell_data_table[spl_Haste].magic_realm));

                    threshold = ((strength * 250) / threshold);

                    if(Random(250) <= threshold)
                    {

                        Mark_Block(_screen_seg);

                        // SPECFX.LBX, 051  "DISPELL1"  ""
                        dispell1_notify_background_seg = LBX_Reload_Next(specfx_lbx_file__ovr131__2of2, 51, _screen_seg);

                        battle_units[battle_unit_idx].Combat_Effects ^= bue_Haste;  // turn off the enchantment

                        if(*notify_count < 5)
                        {

                            if(*notify_count < 4)
                            {

                                _fstrcpy(GUI_NearMsgString, spell_data_table[spl_Haste].name);

                                strcat(GUI_NearMsgString, cnst_Dispel_Msg);  // " has been dispelled."

                            }
                            else
                            {

                                LBX_Load_Data_Static(message_lbx_file__ovr131, 0, (SAMB_ptr)&GUI_NearMsgString[0], 19, 1, 150);

                            }

                            Notify2((160 + (*notify_count * 10)), (20 + (*notify_count * 25)), 3, GUI_NearMsgString, 0, dispell1_notify_background_seg, 0, 8, 0, 0, 0, 1, 0);

                            *notify_count += 1;

                            Release_Block(_screen_seg);

                        }

                    }

                }

                /*
                    END:  Haste
                */

                /*
                    BEGIN:  Confusion
                */

                // ; attempt to remove state 2 Confusion if present
                // ; BUG: fails to clear the confusion state

                if(
                    (battle_units[battle_unit_idx].Combat_Effects == bue_Confusion)
                    &&
                    (battle_units[battle_unit_idx].Confusion_State == 2)
                )
                {

                    threshold = (strength + Calculate_Dispel_Difficulty(spell_data_table[spl_Confusion].casting_cost, battle_units[battle_unit_idx].controller_idx, spell_data_table[spl_Confusion].magic_realm));

                    threshold = ((strength * 250) / threshold);

                    if(Random(250) <= threshold)
                    {

                        Mark_Block(_screen_seg);

                        if(battle_units[battle_unit_idx].controller_idx == _combat_attacker_player)
                        {

                            battle_units[battle_unit_idx].controller_idx = _combat_defender_player;

                        }
                        else
                        {

                            battle_units[battle_unit_idx].controller_idx = _combat_attacker_player;

                        }

                        // SPECFX.LBX, 051  "DISPELL1"  ""
                        dispell1_notify_background_seg = LBX_Reload_Next(specfx_lbx_file__ovr131__2of2, 51, _screen_seg);

                        // ; BUG: should also clear the confusion state
                        battle_units[battle_unit_idx].Combat_Effects ^= bue_Confusion;  // turn off the enchantment

                        if(*notify_count < 5)
                        {

                            if(*notify_count < 4)
                            {

                                _fstrcpy(GUI_NearMsgString, spell_data_table[spl_Confusion].name);

                                strcat(GUI_NearMsgString, cnst_Dispel_Msg);  // " has been dispelled."

                            }
                            else
                            {

                                LBX_Load_Data_Static(message_lbx_file__ovr131, 0, (SAMB_ptr)&GUI_NearMsgString[0], 19, 1, 150);

                            }

                            Notify2((160 + (*notify_count * 10)), (20 + (*notify_count * 25)), 3, GUI_NearMsgString, 0, dispell1_notify_background_seg, 0, 8, 0, 0, 0, 1, 0);

                            *notify_count += 1;

                            Release_Block(_screen_seg);

                        }

                    }

                }

                /*
                    END:  Confusion
                */

            }  /* (battle_units[battle_unit_idx].controller_idx != player_idx) */

        }

    }

    Near_Allocate_Undo();

    CMB_ComposeBackgrnd__WIP();

}


// WZD o131p03
// drake178: CMB_ApplyCracksCall()
// CMB_ApplyCracksCall()

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
    NX_j_CMB_BattlefieldSpell()

*/
void CMB_BattlefieldSpell__WIP(int16_t player_idx, int16_t spell_idx, int16_t anims_on, int16_t caster_idx)
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
                // DOMSDOS  Play_Sound__STUB(SND_SpellCast)
                sdl2_Play_Sound__WIP(SND_SpellCast, SND_SpellCast_size);
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
                Tactical_Combat_Draw();
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
            Tactical_Combat_Draw();
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
// drake178: WIZ_FlameStrike()
/*
; processes and applies the effect of Flame Strike cast
; by the speficied wizard during battle
;
; BUG: Wraith Form incorrectly grants immunity from
;  this effect
*/
/*

*/
void Apply_Flame_Strike(int16_t player_idx)
{
    uint32_t enchantments = 0;
    int16_t damage_types[3] = { 0, 0, 0 };
    int16_t figure_count = 0;
    int16_t battle_unit_idx = 0;  // _SI_
    int16_t itr = 0;  // _DI_

    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {

        enchantments = ( _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments | battle_units[battle_unit_idx].enchantments | battle_units[battle_unit_idx].item_enchantments);

        if(
            (battle_units[battle_unit_idx].controller_idx != player_idx)
            &&
            (battle_units[battle_unit_idx].status == bus_Active)
        )
        {

            // ; BUG: this enchantment should not grant immunity here
            if(
                ((enchantments & UE_WRAITHFORM) == 0)
                &&
                ((enchantments & UE_RIGHTEOUSNESS) == 0)
            )
            {

                CMB_ConvSpellAttack__WIP(spl_Flame_Strike, battle_unit_idx, &damage_types[0], 0);

                BU_ApplyDamage__WIP(battle_unit_idx, &damage_types[0]);    

            }

        }

    }

}


// WZD o131p06
// drake178: WIZ_HolyWord()
/*
; processes and applies the effect of Holy Word cast
; by the speficied wizard during battle
; BUG: Wraith Form incorrectly grants immunity from
;  this effect
*/
/*

*/
void Apply_Holy_Word(int16_t player_idx)
{
    int16_t damage_types[3] = { 0, 0, 0 };
    int16_t resistance_modifier = 0;
    uint32_t enchantments = 0;
    int16_t figure_count = 0;
    int16_t battle_unit_idx = 0;  // _SI_
    int16_t itr = 0;  // _DI_

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

            // ; BUG: this enchantment should not grant immunity here
            if(
                ((enchantments & UE_WRAITHFORM) == 0)
                &&
                ((enchantments & UE_SPELLLOCK) == 0)
                &&
                (battle_units[battle_unit_idx].race >= rt_Arcane)
            )
            {

                resistance_modifier = -2;

                if((_UNITS[battle_units[battle_unit_idx].unit_idx].mutations & UM_UNDEAD) == 0)
                {

                    resistance_modifier += -5;

                }

                figure_count = battle_units[battle_unit_idx].Cur_Figures;

                for(itr = 0; itr < figure_count; itr++)
                {
 
                    damage_types[2] = 0;

                    if(Combat_Resistance_Check(battle_units[battle_unit_idx], -2, sbr_Death) > 0)
                    {

                        damage_types[2] = battle_units[battle_unit_idx].hits;

                        BU_ApplyDamage__WIP(battle_unit_idx, &damage_types[0]);    

                    }

                }

            }

        }

    }

}


// WZD o131p07
// drake178: WIZ_DeathSpell()
/*
; processes and applies the effect of Death Spell cast
; by the speficied wizard during battle
;
; BUG: Wraith Form incorrectly grants immunity from
;  this effect
*/
/*

*/
void Apply_Death_Spell(int16_t player_idx)
{
    int16_t damage_types[3] = { 0, 0, 0 };
    uint32_t enchantments = 0;
    int16_t figure_count = 0;
    int16_t battle_unit_idx = 0;  // _SI_
    int16_t itr = 0;  // _DI_

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

            // ; BUG: this enchantment should not grant immunity here
            if(
                ((enchantments & UE_WRAITHFORM) == 0)
                &&
                ((battle_units[battle_unit_idx].Attribs_1 & USA_IMMUNITY_DEATH) == 0)
            )
            {

                figure_count = battle_units[battle_unit_idx].Cur_Figures;

                for(itr = 0; itr < figure_count; itr++)
                {
 
                    damage_types[0] = 0;

                    if(Combat_Resistance_Check(battle_units[battle_unit_idx], -2, sbr_Death) > 0)
                    {

                        damage_types[0] = battle_units[battle_unit_idx].hits;

                        BU_ApplyDamage__WIP(battle_unit_idx, &damage_types[0]);    

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
    OON XREF:  Cast_Spell_On_Battle_Unit()

XREF:
    j_CMB_PlaySpellAnim__WIP(*)
        Cast_Spell_On_Battle_Unit()

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

            CMB_BattlefieldSpell__WIP(player_idx, spell_idx, anims_on, caster_idx);

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

                Tactical_Combat_Draw();

                Combat_Cast_Spell_Message(caster_idx, spell_idx);

                PageFlip_FX();

                for(itr = 0; itr < 20; itr++)
                {

                    Set_Palette_Changes(0, -1);

                    /* SPELLY */  VGA_ShadeScreen__STUB((itr / 2), red, green, blue);

                }

                for(itr = 20; itr > -1; itr--)
                {

                    Set_Palette_Changes(0, -1);

                    /* SPELLY */  VGA_ShadeScreen__STUB((itr / 2), red, green, blue);

                }

            }

        }

    }
    else
    {

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

                /* SPELLY */  TILE_LightningBolt__WIP(cgx, cgy, caster_idx);

            }
            else
            {

                Mark_Block(_screen_seg);

                Anim_Size = Spell_Animation_Load_Graphics(spell_idx);

                Release_Block(_screen_seg);

                if(spell_idx == spl_Cracks_Call)
                {

                    /* SPELLY */  TILE_CracksCall__WIP(cgx, cgy, caster_idx);

                }
                else
                {

                    /* SPELLY */  TILE_CombatSpellAnim__WIP(cgx, cgy, Anim_Size, caster_idx, spell_idx);

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

    CMB_ComposeBackgrnd__WIP();
    Set_Page_Off();
    Tactical_Combat_Draw();
    PageFlip_FX();
    Copy_On_To_Off_Page();
    Copy_Off_To_Back();

    // ; create a list of target BU indexes, and another of
    // ; the pointers to their names
    // ; BUG: considers non-involved, recalled, and fleeing
    // ;  units as valid targets
    // ; BUG: the arrays are only 8 elements long, need 9+
    // ; BUG? considers the Chosen as an invalid target
    // ; BUG: writes to the name pointers instead of assigning
    // ;  to them
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
            // // BUGBUG  strcpy(Target_Name_Ptr_List[Target_Count], _unit_type_table[_UNITS[battle_units[itr].unit_idx].type].name);
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
    Tactical_Combat_Draw();
    PageFlip_FX();

    if(Picked_Target != ST_UNDEFINED)
    {
        if(player_idx != HUMAN_PLAYER_IDX)
        {
            // ; BUG: will enter an infinite loop if all 9 tiles in
            // ; the selected area are occupied or invalid
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
            } while (CMB_TargetRows[cgy][cgx] >= 0);
        }

        battle_unit_idx = Target_BU_List[Picked_Target];

        if(battle_units[battle_unit_idx].Max_Figures > 1)
        {
            battle_units[battle_unit_idx].Cur_Figures = (battle_units[battle_unit_idx].Max_Figures / 2);
            battle_units[battle_unit_idx].TopFig_Dmg = 0;
        }
        else
        {
            battle_units[battle_unit_idx].Cur_Figures = battle_units[battle_unit_idx].Max_Figures;
            battle_units[battle_unit_idx].TopFig_Dmg = (battle_units[battle_unit_idx].hits / 2);
        }
        battle_units[battle_unit_idx].Combat_Effects = 0;
        battle_units[battle_unit_idx].enchantments = 0;
        _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments = 0;
        battle_units[battle_unit_idx].cgx = cgx;
        battle_units[battle_unit_idx].cgy = cgy;
        battle_units[battle_unit_idx].target_cgx = cgx;
        battle_units[battle_unit_idx].target_cgy = cgy;
        battle_units[battle_unit_idx].MoveStage = 0;
        battle_units[battle_unit_idx].outline_magic_realm = 0;
        battle_units[battle_unit_idx].Atk_FigLoss = 0;
        battle_units[battle_unit_idx].Moving = 0;
        battle_units[battle_unit_idx].action = bua_Ready;
        battle_units[battle_unit_idx].bufpi = Combat_Figure_Load(_UNITS[battle_units[battle_unit_idx].unit_idx].type, Battle_Unit_Pict_Open());
        battle_units[battle_unit_idx].status = bus_Active;
        BU_Init_Battle_Unit(&battle_units[battle_unit_idx]);
        BU_Apply_Battlefield_Effects__WIP(&battle_units[battle_unit_idx]);
        BU_CombatSummon__SEGRAX(battle_unit_idx, cgx, cgy, spl_Raise_Dead, caster_idx);
    }

    Set_Page_Off();
    Tactical_Combat_Draw();
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

    CMB_ComposeBackgrnd__WIP();
    Set_Page_Off();
    Tactical_Combat_Draw();
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
            // BUGBUG  strcpy(Target_Name_Ptr_List[Target_Count], _unit_type_table[_UNITS[battle_units[itr].unit_idx].type].name);
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
        while(CMB_TargetRows[cgy][cgx] >= 0);
    }

// Severity Code Description Project File Line Suppression State Details
//     Warning C6385 Reading invalid data from 'Target_BU_List'.sdl2_ReMoM C :\STU\devel\ReMoM\src\Spells131.C 344		
    if(Picked_Target > ST_UNDEFINED)
        battle_unit_idx = Target_BU_List[Picked_Target];

    battle_units[battle_unit_idx].controller_idx = player_idx;  // ; BUG: fails to set the overland owner of the unit
    battle_units[battle_unit_idx].Cur_Figures = battle_units[battle_unit_idx].Max_Figures;
    battle_units[battle_unit_idx].TopFig_Dmg = 0;
    battle_units[battle_unit_idx].Combat_Effects = 0;
    battle_units[battle_unit_idx].enchantments = 0;
    battle_units[battle_unit_idx].MoveStage = 0;
    battle_units[battle_unit_idx].outline_magic_realm = 0;
    battle_units[battle_unit_idx].Atk_FigLoss = 0;
    battle_units[battle_unit_idx].Moving = 0;
    battle_units[battle_unit_idx].action = bua_Ready;
    _UNITS[battle_units[battle_unit_idx].unit_idx].enchantments = 0;
    _UNITS[battle_units[battle_unit_idx].unit_idx].mutations |= UM_UNDEAD;
    battle_units[battle_unit_idx].cgx = cgx;
    battle_units[battle_unit_idx].cgy = cgy;
    // ¿ BUGBUG  doesn't set target_cgx,cgy, like 'Raise Dead' ?
    battle_units[battle_unit_idx].bufpi = Combat_Figure_Load(_UNITS[battle_units[battle_unit_idx].unit_idx].type, Battle_Unit_Pict_Open());
    battle_units[battle_unit_idx].status = bus_Active;
    BU_Init_Battle_Unit(&battle_units[battle_unit_idx]);
    BU_Apply_Battlefield_Effects__WIP(&battle_units[battle_unit_idx]);
    BU_CombatSummon__SEGRAX(battle_unit_idx, cgx, cgy, spl_Animate_Dead, caster_idx);

}
