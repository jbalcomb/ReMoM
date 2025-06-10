/*
    WIZARDS.EXE
        ovr131
*/

#include "MOX/Fonts.H"
#include "MOX/MOX_DAT.H"
#include "MOX/MOM_Data.H"
#include "MOX/Video.H"
#include "Spellbook.H"
#include "Spells131.H"


#include "Combat.H"
#include "SBookScr.H"
#include "Spells133.H"
#include "UNITTYPE.H"



// WZD dseg:6844                                                 BEGIN:  ovr131 - Initialized Data

/* HACK */  char str_empty_string__ovr131[] = "";

// WZD dseg:6844 53 50 45 43 46 58 00                            specfx_lbx_file__ovr131__1of2 db 'SPECFX',0         ; DATA XREF: CMB_Disenchant+1C8o
// WZD dseg:684B 20 68 61 73 20 62 65 65 6E 20 64 69 73 70 65 6C+cnst_Dispel_Msg db ' has been dispelled.',0
// WZD dseg:6860 6D 65 73 73 61 67 65 00                         message_lbx_file__ovr131 db 'message',0       ; DATA XREF: CMB_Disenchant+22Co ...
// WZD dseg:6868 73 70 65 63 66 78 00                            specfx_lbx_file__ovr131__2of2 db 'specfx',0         ; DATA XREF: CMB_Disenchant+50Fo ...
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
// CMB_Disenchant()

// WZD o131p02
// drake178: TILE_DispelMagic()
// TILE_DispelMagic()

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

*/
void CMB_BattlefieldSpell__WIP(int16_t player_idx, int16_t spell_idx, int16_t Anims, int16_t caster_idx)
{



}


// WZD o131p05
// drake178: WIZ_FlameStrike()
// WIZ_FlameStrike()

// WZD o131p06
// drake178: WIZ_HolyWord()
// WIZ_HolyWord()

// WZD o131p07
// drake178: WIZ_DeathSpell()
// WIZ_DeathSpell()

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
    OON XREF:  G_CMB_SpellEffect__WIP()

XREF:
    j_CMB_PlaySpellAnim__WIP(*)
        G_CMB_SpellEffect__WIP()

*/
void Combat_Spell_Animation__WIP(int16_t cgx, int16_t cgy, int16_t spell_idx, int16_t player_idx, int16_t Anims, int16_t caster_idx)
{
    int16_t blue = 0;
    int16_t green = 0;
    int16_t red = 0;
    int16_t Anim_Size = 0;
    int16_t itr = 0;  // _DI_

    if(
        (spell_data_table[spell_idx].type == scc_Battlefield_Spell)
        ||
        (spell_data_table[spell_idx].type == scc_Counter_Spell)
        ||
        (spell_data_table[spell_idx].type == scc_Disenchant_Spell)
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

            /* SPELLY */  CMB_BattlefieldSpell__WIP(player_idx, spell_idx, Anims, caster_idx);

        }
        else
        {

// shade the screen to 40% of the realm's color and
// then back to the original, while displaying the
// combat spell cast message (but no animation)
            if(Anims != ST_FALSE)
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

                CMB_SpellcastMessage__WIP(caster_idx, spell_idx);

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

        if(Anims != ST_FALSE)
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
