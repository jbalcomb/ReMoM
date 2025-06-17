/*
    WIZARDS.EXE
        ovr133
*/


#include "MOM_DEF.H"
#include "MOX/FLIC_Draw.H"
#include "MOX/MOM_Data.H"
#include "MOX/LBX_Load.H"
#include "MOX/MOX_DAT.H"
#include "MOX/MOX_DEF.H"
#include "MOX/MOX_SET.H"
#include "MOX/MOX_TYPE.H"
#include "MOX/SOUND.H"

#include "CMBTDEF.H"
#include "Combat.H"
#include "NEXTTURN.H"
#include "SBookScr.H"
#include "Spellbook.H"
#include "Spells129.H"
#include "Spells133.H"

#include "STU/STU_DBG.H"



// WZD  dseg:68FC                                                 BEGIN:  ovr133 - Initialized Data

// WZD  dseg:68FC 59 6F 75 72 20 00                               cnst_CounterMsg1 db 'Your ',0           ; DATA XREF: CMB_CounterMessage__STUB+17o
// WZD  dseg:6902 27 20 00                                        cnst_Apostrophe db 27h,' ',0            ; DATA XREF: CMB_CounterMessage__STUB+66o ...
// WZD  dseg:6905 27 73 20 00                                     cnst_Possessive db 27h,'s ',0           ; DATA XREF: CMB_CounterMessage__STUB:loc_B6E25o ...
// WZD  dseg:6909 20 73 70 65 6C 6C 20 68 61 73 20 63 61 75 73 65+cnst_CounterMsg2 db ' spell has caused ',0
// WZD  dseg:691C 54 68 65 20 70 6F 77 65 72 20 6F 66 20          cnst_CounterMsg3 db 'The power of '     ; DATA XREF: CMB_CounterMessage__STUB:loc_B6E47o
// WZD  dseg:6929 74 68 65 20 00                                  cnst_CounterMsg7 db 'the ',0            ; DATA XREF: CMB_CounterMessage__STUB:loc_B6F4Do
// WZD  dseg:692E 53 6F 72 63 65 72 79 00                         cnst_Rlm1_Sorcery_2 db 'Sorcery',0      ; DATA XREF: CMB_CounterMessage__STUB:loc_B6E70o
// WZD  dseg:6936 43 68 61 6F 73 00                               cnst_Rlm2_Chaos_2 db 'Chaos',0          ; DATA XREF: CMB_CounterMessage__STUB:loc_B6E81o
// WZD  dseg:693C 4E 61 74 75 72 65 00                            cnst_Rlm0_Nature_2 db 'Nature',0        ; DATA XREF: CMB_CounterMessage__STUB:loc_B6E86o
// WZD  dseg:6943 20 4E 6F 64 65 20 68 61 73 20 63 61 75 73 65 64+cnst_CounterMsg4 db ' Node has caused ',0
// WZD  dseg:6955 79 6F 75 72 20 00                               cnst_CounterMsg5 db 'your ',0           ; DATA XREF: CMB_CounterMessage__STUB+E5o
// WZD  dseg:695B 20 73 70 65 6C 6C 20 74 6F 20 66 69 7A 7A 6C 65+cnst_CounterMsg6 db ' spell to fizzle.',0
// WZD  dseg:696D
char soundfx_lbx_file__ovr133__1of2[] = "soundfx";
// WZD  dseg:6975
char soundfx_lbx_file__ovr133__2of2[] = "SOUNDFX";
// WZD  dseg:697D
char monster_lbx_file__ovr133[] = "monster";
// WZD  dseg:6985
char specfx_lbx_file__ovr133[] = "specfx";
// WZD  dseg:698C
char cmbtfx_lbx_file__ovr133[] = "cmbtfx";

// WZD  dseg:6993 00                                              align 2

// WZD  dseg:6993                                                 END:  ovr133 - Initialized Data




/*
    WIZARDS.EXE  ovr133
*/

// WZD o133p01
/*
; applies the Warp Creature effect to a battle unit,
; choosing a random one of the three possible effects,
; or less if some are already active
; WARNING: will enter an infinite loop if all 3 effects
; are already present
*/
/*

*/
void Apply_Warp_Creature(int16_t battle_unit_idx)
{
    int16_t warp = 0;  // _DI_

    warp = -2;

    while(warp < 0)
    {

        warp = Random(3);

        switch(warp)
        {

            case 1:
            {

                if((battle_units[battle_unit_idx].Combat_Effects & bue_Warped_Attack) == 0)
                {

                    battle_units[battle_unit_idx].Combat_Effects |= bue_Warped_Attack;

                }
                else
                {

                    warp = -1;

                }

            } break;

            case 2:
            {

                if((battle_units[battle_unit_idx].Combat_Effects & bue_Warped_Defense) == 0)
                {

                    battle_units[battle_unit_idx].Combat_Effects |= bue_Warped_Defense;

                }
                else
                {

                    warp = -2;

                }

            } break;

            case 3:
            {

                if((battle_units[battle_unit_idx].Combat_Effects & bue_Warped_Resist) == 0)
                {

                    battle_units[battle_unit_idx].Combat_Effects |= bue_Warped_Resist;

                }
                else
                {

                    warp = -3;

                }

            } break;

        }

    }

}


// WZD o133p02
/*
; processes the Wrack effect of the selected player,
; including loading and playing its animation
; BUGs: can only affect up to 40 figures at a time, can
; corrupt memory in unlucky scenarios, its effect is
; prevented by Wraith Form, and it does squared the
; intended damage to units
*/
/*

Wrack:
Death. Combat Enchantment. Casting Cost: 40 mana. Rare.
Wracks all non-death creatures with the agonies of darkness
every combat turn. All enemy figures that cannot resist this pain
lose one hit point.
*/
void Apply_Wrack(int16_t player_idx)
{
    // count of damages per battle unit
    int8_t Damage_Per_Unit_Array[MAX_BATTLE_UNIT_SLOT_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;
    // populated with battle_unit_idx
    int8_t Damaged_Unit_Array[MAX_BATTLE_UNIT_SLOT_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;
    int16_t screen_y = 0 ;
    int16_t screen_x = 0 ;
    int16_t figure_ctr = 0 ;  /// used to index Damaged_Unit_Array[]
    int16_t damage_types[3] = { 0, 0, 0 } ;
    uint32_t enchantments = 0 ;
    int16_t anim_size = 0 ;
    int16_t itr1 = 0 ;
    int16_t itr2 = 0;  // _SI_
    int16_t itr3 = 0;  // _DI_

    damage_types[0] = 1;
    damage_types[2] = 0;
    damage_types[1] = 0;

    figure_ctr = 0;

    for(itr1 = 0; itr1 < MAX_BATTLE_UNIT_SLOT_COUNT; itr1++)
    {

        Damage_Per_Unit_Array[itr1] = 0;

    }

    Mark_Block(_screen_seg);

    anim_size = Spell_Animation_Load_Graphics(spl_Wrack);

    // preform a resistance check for each figure of every
    // enemy unit, storing the results into the local arrays
    // BUGs: Wraith Form grants immunity, and the damaged
    // unit array is filled out fundamentally wrong

    for(itr1 = 0; itr1 < _combat_total_unit_count; itr1++)
    {

        enchantments = (_UNITS[battle_units[itr1].unit_idx].enchantments | battle_units[itr1].enchantments | battle_units[itr1].item_enchantments);

        if(
            (battle_units[itr1].controller_idx != player_idx)
            &&
            (battle_units[itr1].status == bus_Active)
            &&
            ((battle_units[itr1].enchantments & UE_WRAITHFORM) == 0)  /* ; BUG: this enchantment should not help here */
            &&
            ((battle_units[itr1].Attribs_1 & USA_IMMUNITY_DEATH) == 0)
        )
        {

            for(itr2 = 0; battle_units[itr1].Cur_Figures > itr2; itr2++)
            {
                
                if(Combat_Resistance_Check(battle_units[itr1], 1, sbr_Death) > 0)
                {

                    Damaged_Unit_Array[figure_ctr] = itr1;

                    Damage_Per_Unit_Array[itr1] += 1;

                    figure_ctr++;

                }

            }

        }

    }

    for(itr1 = 0; (FLIC_Get_FrameCount(spell_animation_seg) + figure_ctr) > itr1; itr1++)
    {

        if(((FLIC_Get_FrameCount(spell_animation_seg) + figure_ctr) / 2) == itr1)
        {

            // apply damage to the affected units
            // BUG: overwriting the damage array causes the actual
            // damage done to be the square of what was intended
            for(itr2 = 0; itr2 < figure_ctr; itr2++)
            {

                if(Damage_Per_Unit_Array[Damaged_Unit_Array[itr2]] > 0)
                {

                    damage_types[0] = Damage_Per_Unit_Array[Damaged_Unit_Array[itr2]];

                    BU_ApplyDamage__WIP(Damaged_Unit_Array[itr2], &damage_types[0]);

                }

            }
            
            Set_Page_Off();

            Combat_Screen_Draw();

            for(itr3 = 0; itr3 < figure_ctr; itr3++)
            {

                Combat_Grid_Screen_Coordinates(battle_units[Damaged_Unit_Array[itr3]].cgx, battle_units[Damaged_Unit_Array[itr3]].cgy, 4, 4, &screen_x, &screen_y);

                if(anim_size == 0)
                {

                    screen_x -= 14;
                    screen_y -= 21;

                }
                else
                {

                    screen_x -= 28;
                    screen_y -= 30;

                }

                Set_Animation_Frame(spell_animation_seg, (itr1 - itr3));

                Clipped_Draw(screen_x, screen_y, spell_animation_seg);

            }

            PageFlip_FX();

        }

    }

    Release_Block(_screen_seg);

}


// WZD o133p03
/*
; processes the Call Lightning effect of the selected
; player, including loading and playing both sound and
; animation
; BUGs: will sometimes fire less bolts than intended,
; ignores units with Wraith Form, stores its spell data
; parameters in Wall of Stone's even though it doesn't
; actually need to, and undoes an alloc it didn't mark
*/
/*

*/
void Apply_Call_Lightning(int16_t player_idx)
{
    int16_t damage_types[3] = { 0, 0, 0 };
    int16_t bolts = 0;
    uint32_t enchantments = 0;
    int16_t tries = 0;  // _DI_
    int16_t itr = 0;  // _SI_

    for(itr = 0; itr < 3; itr++)
    {

        damage_types[itr] = 0;

    }

    Mark_Block(_screen_seg);

    Spell_Animation_Load_Graphics(spl_Call_Lightning);

    Combat_Load_Spell_Sound_Effect(spl_Lightning_Bolt);

    bolts = (Random(3) + 2);

    tries = 0;
    while((bolts > 0) && (tries < 30))
    {
        
        tries++;

        itr = (Random(_combat_total_unit_count) -1);

        enchantments = (_UNITS[battle_units[itr].unit_idx].enchantments | battle_units[itr].enchantments | battle_units[itr].item_enchantments);

        if(
            (battle_units[itr].controller_idx != player_idx)
            &&
            (battle_units[itr].status == bus_Active)
            &&
            ((battle_units[itr].enchantments & UE_WRAITHFORM) == 0)
            &&
            (Random(2) == 1)
        )
        {

            Mark_Time();

            CMB_ConvSpellAttack__WIP(spl_Wall_Of_Stone, itr, &damage_types[0], 0);

            Animate_Lightning_Bolt(battle_units[itr].cgx, battle_units[itr].cgy, ST_UNDEFINED);

            BU_ApplyDamage__WIP(itr, &damage_types[0]);

            Release_Time((Random(10) + 3));

            bolts--;

            tries = 0;

        }

    }

    Mark_Time();

    Release_Time(12);

    Release_Block(_screen_seg);

    Release_Block(World_Data);

}


// WZD o133p04
// drake178: BU_LifeDrain()
/*
Which damage type is 'Life Drain'?

*/
void BU_LifeDrain__WIP(int16_t target_idx, int16_t damage_types[], int16_t caster_idx)
{

    damage_types[0] = 1;
    damage_types[1] = 1;
    damage_types[2] = 1;

}


// WZD o133p05
void Wall_Rise(int16_t spell_idx, int16_t caster_idx)
{
    int16_t frame_count = 0;
    int16_t itr_cgx = 0;
    int16_t itr_cgy = 0;  // _DI_
    int16_t wall_type = 0;  // _SI_

    frame_count = 10;

    if(spell_idx == spl_Wall_Of_Stone)
    {

        wall_type = 0;

    }
    else if(spell_idx == spl_Wall_Of_Fire)
    {

        wall_type = 1;

    }
    else
    {

        wall_type = 2;

    }

    if(wall_type == 1)
    {

        frame_count = 2;

    }

    Wall_Rise_Load(wall_type);

    _wall_rise_on = ST_TRUE;

    if(wall_type == 0)
    {

        // mark the city as Walled, and mark all wall sections
        // as intact
        // BUG: would also mark the center sections although
        // there is no wall there (but the spell can't be cast
        // in combat in v1.31 anyway)

        battlefield->walled = ST_TRUE;

        for(itr_cgy = 0; itr_cgy < 4; itr_cgy++)
        {

            for(itr_cgx = 0; itr_cgx < 4; itr_cgx++)
            {
                
                battlefield->walls[((itr_cgy * 4) + itr_cgx)] = 1;

            }

        }

    }
    else if (wall_type == 1)
    {

        battlefield->wall_of_fire = ST_TRUE;

    }
    else if(wall_type == 2)
    {

        battlefield->wall_of_darkness = ST_TRUE;

    }

    for(_wall_rise_frame = 0; _wall_rise_frame < frame_count; _wall_rise_frame++)
    {

        Set_Page_Off();

        Combat_Screen_Draw();

        Combat_Cast_Spell_Message(caster_idx, spell_idx);

        PageFlip_FX();

    }

    _wall_rise_on = ST_FALSE;
    
}


// WZD o133p06
// drake178: CMB_CounterMessage()
/*
; displays the combat counter message for having
; successfully fizzled a spell, either as a type 1 red
; warning dialog, or an onscreen message for 30 frames
; if Auto combat is enabled
*/
/*

*/
void CMB_CounterMessage__STUB(int16_t caster_idx, int16_t type, int16_t spell_idx, char * title)
{



}


// WZD o133p07
// drake178: WIZ_DispelAttempt()
/*
; makes a dispel attempt against a spell of the target
; wizard, using the passed parameters - considers all
; dispel resistance retorts, but dispel enhancing ones
; need to be applied beforehand
; returns 1 if the dispel is successful, or 0 if not
*/
/*

*/
int16_t WIZ_DispelAttempt__STUB(int16_t dispel_strength, int16_t spell_cast, int16_t player_idx, int16_t magic_realm)
{

    return ST_FALSE;

}


// WZD o133p08
/*
; plays the Cracks Call animation using the chasm
; functionality built into GUI_DrawCombatScreen
*/
/*

*/
void Animate_Cracks_Call(int16_t cgx, int16_t cgy, int16_t caster_idx)
{
    int16_t frame = 0;  // _SI_
    int16_t frame_count = 0;  // DNE in Dasm

    if(SND_SpellCast != (SAMB_ptr)ST_UNDEFINED)
    {
        // DOMSDOS  Play_Sound__STUB(SND_SpellCast);
        sdl2_Play_Sound__WIP(SND_SpellCast, SND_SpellCast_size);
    }

    CMB_Chasm_Anim_X = cgx;

    CMB_Chasm_Anim_Y = cgy;

    IMG_GUI_Chasm = spell_animation_seg;

    CMB_Chasm_Anim = ST_TRUE;

    frame_count = FLIC_Get_FrameCount(spell_animation_seg);

    for(frame = 0; (frame_count * 7) > frame; frame++)
    {

        CMB_ChasmAnimStage = (frame / 6);

        Set_Page_Off();

        Combat_Screen_Draw();  // incrs frame, 0-7; sets frame

        Combat_Cast_Spell_Message(caster_idx, spl_Cracks_Call);

        PageFlip_FX();

    }

    CMB_Chasm_Anim = ST_FALSE;

}


// WZD o133p09
// drake178: TILE_BoltFromAbove()
/*
; plays the bolt from the sky animations of Ice Bolt,
; Fire Bolt, Fireball, or Doom Bolt, using a single
; draw of the combat screen, which is reloaded between
; each frame instead of being redrawn
*/
/*
    spl_Ice_Bolt, spl_Fire_Bolt, spl_Fireball, spl_Doom_Bolt

*/
void TILE_BoltFromAbove__WIP(int16_t cgx, int16_t cgy, int16_t spell_idx, int16_t caster_idx)
{
    int16_t sw_spell_idx = 0;
    int16_t cgy_add = 0;
    int16_t cgx_add = 0;
    int16_t frame_num = 0;
    int16_t screen_y = 0;
    int16_t screen_x = 0;
    int16_t frame_count = 0;

    Combat_Grid_Screen_Coordinates(cgx, cgy, 4, 4, &screen_x, &screen_y);

    sw_spell_idx = spell_idx;

    switch(sw_spell_idx)
    {
        case spl_Ice_Bolt:
        {
            screen_x -= 17;
            screen_y -= 20;
            cgx = (screen_x + 110);
            cgy = (screen_y - 100);
            cgx_add = -10;
            cgy_add = 10;
            frame_count = 11;
        } break;

        case spl_Fire_Bolt:
        {
            screen_x -= 16;
            screen_y -= 20;
            cgx = (screen_x + 110);
            cgy = (screen_y - 66);
            cgx_add = -10;
            cgy_add = 6;
            frame_count = 11;
        } break;

        case spl_Fireball:
        {
            screen_x -= 14;
            screen_y -= 21;
            cgx = (screen_x + 110);
            cgy = (screen_y - 66);
            cgx_add = -10;
            cgy_add = 6;
            frame_count = 16;
        } break;

        case spl_Doom_Bolt:
        {
            screen_x -= 14;
            screen_y -= 25;
            cgx = screen_x;
            cgy = (screen_y - 72);
            cgx_add = 0;
            cgy_add = 6;
            frame_count = 13;
        } break;

        default:
        {
            STU_DEBUG_BREAK();
        } break;

    }

    Set_Page_Off();

    Combat_Screen_Draw();

    Combat_Cast_Spell_Message(caster_idx, spell_idx);

    Copy_Off_To_Back();

    Mark_Block(_screen_seg);

    Spell_Animation_Load_Graphics(spell_idx);

    if(SND_SpellCast != (SAMB_ptr)ST_UNDEFINED)
    {

        // DOMSDOS  Play_Sound__STUB(SND_SpellCast);
        sdl2_Play_Sound__WIP(SND_SpellCast, SND_SpellCast_size);

    }

    for(frame_num = 0; frame_num < frame_count; frame_num++)
    {

        Mark_Time();

        if(spell_idx == spl_Fireball)
        {

            Set_Animation_Frame(spell_animation_seg, frame_num);

        }
        else if(spell_idx == spl_Doom_Bolt)
        {
            Set_Animation_Frame(spell_animation_seg, (frame_num / 4));
        }
        else
        {

            if((frame_count - 1) == frame_num)
            {

                Set_Animation_Frame(spell_animation_seg, 3);

            }
            else
            {

                Set_Animation_Frame(spell_animation_seg, (frame_num % 3));
            
            }

        }

        Set_Page_Off();

        Copy_Back_To_Off();

        if(frame_num > 10)
        {

            Clipped_Draw(screen_x, screen_y, spell_animation_seg);

            PageFlip_FX();

            Release_Time(2);

        }
        else
        {

            Clipped_Draw(cgx, cgy, spell_animation_seg);

            cgx += cgx_add;
            cgy += cgy_add;

            PageFlip_FX();

            Release_Time(1);

        }

    }

}


// WZD o133p10
// drake178: TILE_CombatSpellAnim()
/*
; plays a generic combat spellcast animation along with
; sound effect if loaded, redrawing the combat screen
; between each frame with a standard spellcast message
*/
/*

Usage:
'Magic Vortex'

XREF:
    j_TILE_CombatSpellAnim()
        Combat_Spell_Animation__WIP()

(spell_data_table[spell_idx].type != scc_Battlefield_Spell)
(spell_data_table[spell_idx].type != scc_Combat_Counter_Magic)
(spell_data_table[spell_idx].type != scc_Disenchants)
(spell_idx != spl_Raise_Dead)
(spell_idx != spl_Animate_Dead)
∴
Fireball, Fire Bolt, Ice Bolt, Doom Bolt;  Lightning Bolt;  Cracks Call
...
    spell_idx != spl_Fireball, spl_Fire_Bolt, spl_Ice_Bolt, spl_Doom_Bolt
    spell_idx != spl_Lightning_Bolt
    spell_idx != spl_Cracks_Call
    ...
        ¿ ..., Magic Vortex, ... ?

*/
void Combat_Spell_Animation_Generic__WIP(int16_t cgx, int16_t cgy, int16_t anim_size, int16_t caster_idx, int16_t spell_idx)
{
    int16_t screen_y = 0;
    int16_t screen_x = 0;
    int16_t frame_count = 0;
    int16_t frame = 0;  // _SI_

    Combat_Grid_Screen_Coordinates(cgx, cgy, 4, 4, &screen_x, &screen_y);

    if(anim_size == 0)
    {
        screen_x = 14;
        screen_y = 25;
    }
    else
    {
        screen_x = 28;
        screen_y = 30;
    }

    if(SND_SpellCast != (SAMB_ptr)ST_UNDEFINED)
    {
        // DOMSDOS  Play_Sound__STUB(SND_SpellCast);
        sdl2_Play_Sound__WIP(SND_SpellCast, SND_SpellCast_size);
    }

    frame_count = FLIC_Get_FrameCount(spell_animation_seg);

    Reset_Animation_Frame(spell_animation_seg);

    for(frame = 0; frame < frame_count; frame++)
    {

        Mark_Time();

        Combat_Screen_Draw();

        Combat_Cast_Spell_Message(caster_idx, spell_idx);

        Clipped_Draw(screen_x, screen_y, spell_animation_seg);

        PageFlip_FX();

        Release_Time(2);

    }

}

// WZD o133p11  BU_Teleport()

// WZD o133p12  BU_TunnelTo()


// segrax
// WZD ovr133p13
// drake178: BU_CombatSummon()
/*
*/
/*
*/
void BU_CombatSummon__SEGRAX(int16_t battle_unit_idx, int16_t cgx, int16_t cgy, int16_t spell_idx, int16_t player_idx)
{
    int16_t uu_screen_x = 0;
    int16_t uu_screen_y = 0;
    int16_t screen_x = 0;
    int16_t screen_y = 0;
    int16_t anim_ctr = 0;
    
    struct s_BATTLE_UNIT* battle_unit = &battle_units[battle_unit_idx];

    CMB_Chasm_Anim_X = battle_unit->cgx;
    CMB_Chasm_Anim_Y = battle_unit->cgy;

    Combat_Grid_Screen_Coordinates(cgx, cgy, 4, 4, &screen_x, &screen_y);

    uu_screen_x = screen_x - 14;
    uu_screen_y = screen_y - 25;

    screen_x -= 13;
    screen_y -= 27;

    Mark_Block(_screen_seg);

    Spell_Animation_Load_Graphics(spl_Fire_Elemental);

    IMG_GUI_Chasm = spell_animation_seg;

    CMB_Chasm_Anim = 1;

    Mark_Block(World_Data);

    if(magic_set.sound_effects == ST_TRUE)
    {
        // DOMSDOS  Play_Sound__STUB(SND_CMB_Silence);
        sdl2_Play_Sound__WIP(SND_CMB_Silence, SND_CMB_Silence_size);
        SND_SpellCast = LBX_Reload_Next(soundfx_lbx_file__ovr133__2of2, SFX_CombatSummon, World_Data);
        SND_SpellCast_size = lbxload_entry_length;
    }
    else
    {
        SND_SpellCast = (SAMB_ptr)ST_UNDEFINED;
    }

    if (battle_unit->controller_idx == _combat_attacker_player)
    {
        battle_unit->target_cgx = 8;
    }
    else
    {
        battle_unit->target_cgx = 14;
    }

    battle_unit->target_cgy = 12;

    BU_CreateImage__SEGRAX(battle_unit_idx);

    battle_unit->status = bus_Dead;

    if(SND_SpellCast != (SAMB_ptr)ST_UNDEFINED)
    {
        // DOMSDOS  Play_Sound__STUB(SND_SpellCast);
        sdl2_Play_Sound__WIP(SND_SpellCast, SND_SpellCast_size);
    }

    
    for(anim_ctr = 0; anim_ctr < 16; anim_ctr++)
    {

        CMB_ChasmAnimStage = anim_ctr;

        Mark_Time();

        if(anim_ctr == 14)
        {
            battle_unit->status = bus_Active;
        }

        Set_Page_Off();

        Combat_Screen_Draw();

        Combat_Cast_Spell_Message(player_idx, spell_idx);

        Create_Picture(45, 42, GfxBuf_2400B);

        Copy_Bitmap_To_Bitmap(GfxBuf_2400B, IMG_CMB_FX_Figure);

        if(
            (anim_ctr > 6)
            &&
            (anim_ctr < 14)
        )
        {

            Vanish_Bitmap__WIP(GfxBuf_2400B, ((anim_ctr - 6) * 14));

            Set_Window(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, (screen_y + 30));

            FLIC_Set_LoopFrame_1(GfxBuf_2400B);

            Draw_Picture_Windowed(screen_x, ((screen_y + 21) - ((anim_ctr - 6) * 3)), GfxBuf_2400B);

            Reset_Window();

        }

        PageFlip_FX();

        Release_Time(2);

    }

    Set_Page_Off();

    Combat_Screen_Draw();

    PageFlip_FX();

    CMB_Chasm_Anim = 0;

    Release_Block(_screen_seg);

    Release_Block(World_Data);

}


// WZD o133p14
// drake178: TILE_LightningBolt()
/*
; plays the Lightning Bolt spellcast animation, with
; or without a corresponding cast message (none if
; Caster_ID is -1, used by Call Lightning)
; GUI_SpellAnimGFX@ and GUI_Spell_SFX@ need to be
; loaded and assigned to the correct effects
*/
/*

if caster_idx is ST_UNDEFINED, will not display the message "..has cast Lightning Bolt"

*/
void Animate_Lightning_Bolt(int16_t cgx, int16_t cgy, int16_t caster_idx)
{
    int16_t screen_y = 0;
    int16_t screen_x = 0;
    int16_t itr = 0;  // _SI_

    for(itr = 0; itr < 5; itr++)
    {

        Combat_Screen_Draw();


        if(caster_idx > ST_UNDEFINED)
        {

            Combat_Cast_Spell_Message(caster_idx, spl_Lightning_Bolt);

        }

        PageFlip_FX();

    }


// ; repeast three redraws of the combat screen, with a
// ; Lightning Bolt spellcast message if Caster_ID is
// ; greater than -1, drawing a random frame of the loaded
// ; GUI_SpellAnimGFX@ each time, and initiating
// ; GUI_Spell_SFX@ playback on the first frame

    for(itr = 0; itr < 2; itr++)
    {

        Set_Page_Off();

        Combat_Screen_Draw();

        if(caster_idx > ST_UNDEFINED)
        {

            Combat_Cast_Spell_Message(caster_idx, spl_Lightning_Bolt);

        }

        Combat_Grid_Screen_Coordinates(cgx, cgy, 4, 4, &screen_x, &screen_y);

        if(itr == 0)
        {
            if(magic_set.sound_effects == ST_TRUE)
            {
                // DOMSDOS  Play_Sound__STUB(SND_SpellCast);
                sdl2_Play_Sound__WIP(SND_SpellCast, SND_SpellCast_size);
            }
        }

        Set_Animation_Frame(spell_animation_seg, (Random(4) - 1));

        Clipped_Draw(screen_x, (screen_y - 199), spell_animation_seg);  // DEDU  `- 199`?

        PageFlip_FX();

    }

    for(itr = 0; itr < 5; itr++)
    {

        Combat_Screen_Draw();


        if(caster_idx > ST_UNDEFINED)
        {

            Combat_Cast_Spell_Message(caster_idx, spl_Lightning_Bolt);

        }

        PageFlip_FX();

    }

    Set_Page_Off();

    Combat_Screen_Draw();

    PageFlip_FX();

}


// WZD o133p15
/*
; adds a magic vortex to the vortex array using the
; specified attributes
*/
/*

*/
void Magic_Vortex_Create(int16_t player_idx, int16_t cgx, int16_t cgy)
{

    _vortexes[_vortex_count].cgx = cgx;
    _vortexes[_vortex_count].cgy = cgy;

    _vortexes[_vortex_count].move_cgx = cgx;
    _vortexes[_vortex_count].move_cgy = cgy;

    _vortexes[_vortex_count].stage = 0;

    _vortexes[_vortex_count].owner_idx = player_idx;

    _vortex_count++;

}


// segrax
// WZD o133p16
// drake178: CMB_SetVortexCursor()
/*
*/
/*

*/
void Vortex_Move_Screen_Assign_Mouse_Images(int vortex_idx)
{
    int16_t pointer_offset = 0;
    int16_t cgy = 0;
    int16_t screen_y = 0; 
    int16_t screen_x = 0;
    int16_t cgx = 0;  // _DI_
    int16_t movement_cost = 0;  // DNE in DAsm
    int16_t dist_x = 0;        // DNE in DAsm
    int16_t dist_y = 0;         // DNE in DAsm

    pointer_offset = 4;

    _combat_mouse_grid->image_num = crsr_RedCross;

    screen_x = Pointer_X() + pointer_offset;
    screen_y = Pointer_Y() + pointer_offset;

    frame_active_flag = ST_TRUE;

    frame_active_cgx = _vortexes[vortex_idx].cgx;
    frame_active_cgy = _vortexes[vortex_idx].cgy;

    frame_anim_cycle = ((frame_anim_cycle + 1) % 3);

    if(screen_y <= (168 + pointer_offset))
    {

        frame_scanned_flag = ST_FALSE;

        cgx = Get_Combat_Grid_Cell_X(screen_x, screen_y);
        cgy = Get_Combat_Grid_Cell_Y(screen_x, screen_y);

        if(
            (_vortexes[vortex_idx].cgx != cgx)
            ||
            (_vortexes[vortex_idx].cgy != cgy)
        )
        {

            movement_cost = battlefield->MoveCost_Teleport[((cgy * COMBAT_GRID_WIDTH) + cgx)];
            dist_x = (cgx - _vortexes[vortex_idx].cgx);
            dist_y = (cgy - _vortexes[vortex_idx].cgy);

//             if (
//                 (battlefield->MoveCost_Teleport[cgx] != -1) // INF
//                 &&
//                 (abs((cgx - _vortexes[vortex_idx].cgx)) <= 1) &&
//                 (abs((cgy - _vortexes[vortex_idx].cgy)) <= 1)
//             )

            if(
                (movement_cost != -1) // INF
                &&
                (abs(dist_x) <= 1)
                &&
                (abs(dist_y) <= 1)
            )
            {

                frame_scanned_flag = ST_TRUE;
                frame_scanned_cgx = cgx;
                frame_scanned_cgy = cgy;

                _combat_mouse_grid->image_num = crsr_WingedBoot;

            }

        }

    }

    _combat_mouse_grid->center_offset = 2;
    _combat_mouse_grid->x1 = SCREEN_XMIN;
    _combat_mouse_grid->y1 = SCREEN_YMIN;
    _combat_mouse_grid->x2 = SCREEN_XMAX;
    _combat_mouse_grid->y2 = SCREEN_YMAX;

    Set_Mouse_List(1, _combat_mouse_grid);

}


// segrax
// WZD o133p17
// drake178: CMB_VortexPlayerMove()
void Vortex_Move_Screen(int vortex_idx)
{
    int16_t target_cgy = 0;
    int64_t grid_sx = 0;
    int64_t grid_sy = 0;
    int16_t combat_grid_field = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t target_cgx = 0;  // _DI_
    
    if(
        (_vortexes[vortex_idx].owner_idx == HUMAN_PLAYER_IDX)
        &&
        (_auto_combat_flag == ST_FALSE)
    )
    {

        Clear_Fields();

        combat_grid_field = Add_Grid_Field(0, 0, 1, 1, 319, 168, &grid_sx, &grid_sy, ST_UNDEFINED);

        leave_screen = ST_FALSE;

        _active_battle_unit = ST_UNDEFINED;

        while(leave_screen == ST_FALSE)
        {

            Mark_Time();

            input_field_idx = Get_Input();

            if(input_field_idx == combat_grid_field)
            {

                target_cgx = Get_Combat_Grid_Cell_X(grid_sx + 4, grid_sy + 4);
                target_cgy = Get_Combat_Grid_Cell_Y(grid_sx + 4, grid_sy + 4);

                if(
                    (_vortexes[vortex_idx].cgx != target_cgx)
                    ||
                    (_vortexes[vortex_idx].cgy != target_cgy)
                )
                {

                    if(battlefield->MoveCost_Teleport[((target_cgy * COMBAT_GRID_WIDTH) + target_cgx)] != -1)  // INF
                    {

                        if(
                            (abs(target_cgx - _vortexes[vortex_idx].cgx) <= 1)
                            &&
                            (abs(target_cgy - _vortexes[vortex_idx].cgy) <= 1)
                        )
                        {

                            Vortex_Move_And_Attack(vortex_idx, target_cgx, target_cgy);

                            leave_screen = ST_TRUE;

                        }

                    }

                }

            }

            if(leave_screen == ST_FALSE)
            {

                Combat_Screen_Draw();

                Vortex_Move_Screen_Assign_Mouse_Images(vortex_idx);

                PageFlip_FX();

                Release_Time(2);

            }

        }

    }

}


// segrax
// WZD o133p18
// drake178: CMB_ProcessVortices()
/*
; processes the movement of magic votrices - 3 random
; moves, then one player selected one
;
; BUGs: the player can't move their vortex if actions
; are locked out, and the AI never gets its 4th move
*/
void Vortex_Combat_Round(void)
{
    int16_t tries = 0;
    int16_t curr_cgy = 0;
    int16_t curr_cgx = 0;
    int16_t next_cgy = 0;
    int16_t itr = 0;
    int16_t next_cgx = 0;
    int16_t dx = 0;
    int16_t vortex_idx = 0;  // _SI_

    if(magic_set.sound_effects == ST_TRUE)
    {
        // DOMSDOS  Play_Sound__WIP(SND_CMB_Silence);
        sdl2_Play_Sound__WIP(SND_CMB_Silence, SND_CMB_Silence_size);

        Mark_Block(World_Data);

        // SOUNDFX.LBX, 007  "FLYERS2 "  "Flyers, small"
        SND_SpellCast = LBX_Reload_Next(soundfx_lbx_file__ovr133__1of2, SFX_Flyer_S, World_Data);
        SND_SpellCast_size = lbxload_entry_length;

        Release_Block(World_Data);
    }

    for(vortex_idx = 0; vortex_idx < _vortex_count; vortex_idx++)
    {

        for(itr = 0; itr < 3; itr++)
        {

            curr_cgx = _vortexes[vortex_idx].cgx;
            curr_cgy = _vortexes[vortex_idx].cgy;

            next_cgx = curr_cgx;
            next_cgy = curr_cgy;

            tries = 0;
            while(tries < 50)
            {

                if(
                    (next_cgx != curr_cgx)
                    ||
                    (next_cgy != curr_cgy)
                )
                {

                    if(
                        (battlefield->MoveCost_Teleport[((next_cgy * COMBAT_GRID_WIDTH) + next_cgx)] != ST_UNDEFINED)
                        &&
                        (next_cgx >= 0)
                        &&
                        (next_cgx <= COMBAT_GRID_WIDTH)
                        &&
                        (next_cgy >= 0)
                        &&
                        (next_cgy <= COMBAT_GRID_HEIGHT)
                    )
                    {

                        // ¿ BUGBUG  typo, should be 2 ...same or 1 adjacent in all 8 directions ...adjacent unit check does x<=1 and y<=1 separently ?
                        if(DIST(next_cgx, _vortexes[vortex_idx].move_cgx, next_cgy, _vortexes[vortex_idx].move_cgy) > 1)
                        {

                            break;

                        }

                    }

                }

                tries++;

                // Generate random next position
                // ... {1,2,3} - 2 = {-1,0,+1}
                // 3^2  {{-1,-1},{0,-1},{+1,-1}
                //      {{-1, 0},{0, 0},{+1, 0}
                //      {{-1,+1},{0,+1},{+1,+1}
                next_cgx = (curr_cgx + (Random(3) - 2));
                next_cgy = (curr_cgy + (Random(3) - 2));

            }

            assert( (next_cgx >= (curr_cgx - 1)) && (next_cgx <= (curr_cgx + 1)) );
            assert( (next_cgy >= (curr_cgy - 1)) && (next_cgy <= (curr_cgy + 1)) );

            // ¿ Do it, even if we never got valid values ?
            Vortex_Move_And_Attack(vortex_idx, next_cgx, next_cgy);

        }

        _vortexes[vortex_idx].move_cgx = _vortexes[vortex_idx].cgx;
        _vortexes[vortex_idx].move_cgy = _vortexes[vortex_idx].cgy;

        Vortex_Move_Screen(vortex_idx);

    }

}


// segrax
// WZD o133p19
// drake178: CMB_VortexMovement()
/*
; moves a Magic Vortex from its current location to the
; target tile using an 8-step animation, processing its
; damage after reaching the destination
; prev coordinates are set to / used as next ones for
; the duration of the movement
*/
/*
Dasm shows 1:3 chance of lightning bolt, not 1:4 as stated in the manual and OSG.

*/
void Vortex_Move_And_Attack(int vortex_idx, int next_cgx, int next_cgy)
{
    int16_t prev_cgx = 0;
    int16_t prev_cgy = 0;
    uint32_t enchantments = 0;
    int16_t damage_types[3] = { 0, 0, 0 };
    int16_t y_dist = 0;
    int16_t x_dist = 0;
    int16_t itr = 0;  // _SI_
    int16_t battle_unit_idx = 0;  // _SI_
    struct s_MAGIC_VORTEX * vortex = 0;  // DNE in Dasm


    vortex = &_vortexes[vortex_idx];


    prev_cgx = vortex->cgx;
    prev_cgy = vortex->cgy;

    frame_active_flag = ST_FALSE;

    vortex->move_cgx = next_cgx;
    vortex->move_cgy = next_cgy;

    if(magic_set.sound_effects == ST_TRUE)
    {
        // DOMSDOS  Play_Sound__WIP(SND_SpellCast);
        sdl2_Play_Sound__WIP(SND_SpellCast, SND_SpellCast_size);
    }

    for(itr = 0; itr < 8; itr++)
    {
        vortex->stage = itr;
        Set_Page_Off();
        Combat_Screen_Draw();
        PageFlip_FX();
    }

    vortex->cgx = next_cgx;
    vortex->cgy = next_cgy;

    if(
        (next_cgx >= MIN_CGX_CITY)
        &&
        (next_cgx <= MAX_CGX_CITY)
        &&
        (next_cgy >= MIN_CGY_CITY)
        &&
        (next_cgy <= MAX_CGY_CITY)
    )
    {

        _combat_city_damage += 5;

    }

    vortex->move_cgx = prev_cgx;
    vortex->move_cgy = prev_cgy;

    vortex->stage = 0;

    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {
        
        enchantments = (_UNITS[battle_units[battle_unit_idx].unit_idx].enchantments | battle_units[battle_unit_idx].enchantments | battle_units[battle_unit_idx].item_enchantments);

        if(
            (battle_units[battle_unit_idx].cgx == next_cgx)
            &&
            (battle_units[battle_unit_idx].cgy == next_cgy)
            &&
            ((battle_units[battle_unit_idx].Attribs_1 & USA_IMMUNITY_MAGIC) == 0)
            &&
            ((enchantments & UE_RIGHTEOUSNESS) == 0)
        )
        {

            damage_types[0] = 5;
            damage_types[1] = 0;
            damage_types[2] = 0;

            BU_ApplyDamage__WIP(battle_unit_idx, &damage_types[0]);

        }

        x_dist = abs(battle_units[battle_unit_idx].cgx - next_cgx);
        y_dist = abs(battle_units[battle_unit_idx].cgy - next_cgy);

        if(
            (x_dist <= 1)
            &&
            (y_dist <= 1)
            &&
            (x_dist + y_dist != 0)
            &&
            (Random(3) == 1)
        )
        {

            CMB_ConvSpellAttack__WIP(spl_Lightning_Bolt, battle_unit_idx, &damage_types[0], 0);

            BU_ApplyDamage__WIP(battle_unit_idx, &damage_types[0]);

        }

    }

}


// WZD o133p20
// drake178: CMB_CallChaos()
/*
; processes the Call Chaos effect cast by the selected
; entity, including loading and playing both sound and
; animation
;
; BUG: the healing animation is misaligned
; BUG: dumps the sandbox even though the animations
;  would all fit, causing a crash if the GUI is not
;  cleared before casting the spell
; BUG: can damage units with disintegrate despite them
;  not being affected by it
; BUG: glitches the graphic during effect application
;  (drawing on the wrong background)
*/
/*

*/
void Cast_Call_Chaos__WIP(int16_t caster_idx, int16_t anims_on)
{
    int16_t Y_Pos_Array[MAX_BATTLE_UNIT_SLOT_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t X_Pos_Array[MAX_BATTLE_UNIT_SLOT_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Anim_Delay_Array[MAX_BATTLE_UNIT_SLOT_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    SAMB_ptr Blank_Effect = 0;
    SAMB_ptr Healing_Anim = 0;
    SAMB_ptr Generic_Chaos_Anim = 0;
    SAMB_ptr Generic_Chaos_Anim2 = 0;
    SAMB_ptr FlameStrike_Anim = 0;
    SAMB_ptr WarpLightning_Anim = 0;
    SAMB_ptr Generic_Chaos_Anim3 = 0;
    SAMB_ptr Disintegrate_Anim = 0;
    int16_t effects[MAX_BATTLE_UNIT_SLOT_COUNT] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t screen_y = 0;
    int16_t screen_x = 0;
    int16_t player_idx = 0;
    int16_t Effect_Frame_Count = 0;
    int16_t Highest_Frame_Count = 0;
    int16_t itr2 = 0;
    int16_t itr1 = 0;  // _DI_

    // MONSTER.LBX, 047  "BLANK"    ""
    // SPECFX.LBX, 003  "UNCHANT2"  "Life"
    // SPECFX.LBX, 002  "CHAOS"     "Chaos"
    // CMBTFX.LBX, 033  "FIREPILL"  "Fire Storm"
    // CMBTFX.LBX, 003  "WARPLITE"  "Warp Lightning"
    // CMBTFX.LBX, 004  "DISINTEG"  "disintegrate"
#define LOAD_ANIMS_LOCAL()  \
do {  \
        Blank_Effect = LBX_Reload(monster_lbx_file__ovr133, 47, _screen_seg);  \
        Healing_Anim = LBX_Reload_Next(specfx_lbx_file__ovr133, 3, _screen_seg);  \
        Generic_Chaos_Anim = LBX_Reload_Next(specfx_lbx_file__ovr133, 2, _screen_seg);  \
        Generic_Chaos_Anim2 = Generic_Chaos_Anim;  \
        FlameStrike_Anim = LBX_Reload_Next(cmbtfx_lbx_file__ovr133, 33, _screen_seg);  \
        WarpLightning_Anim = LBX_Reload_Next(cmbtfx_lbx_file__ovr133, 3, _screen_seg);  \
        Generic_Chaos_Anim3 = Generic_Chaos_Anim;  \
        Disintegrate_Anim = LBX_Reload_Next(cmbtfx_lbx_file__ovr133, 4, _screen_seg);  \
} while(0)

    if(caster_idx >= CASTER_IDX_BASE)
    {
        player_idx = (caster_idx - CASTER_IDX_BASE);
    }
    else
    {
        player_idx = battle_units[caster_idx].controller_idx;
    }

    Highest_Frame_Count = 0;

    if(anims_on != ST_FALSE)
    {

        Copy_On_To_Off_Page();
        Copy_Off_To_Back();

        for(itr1 = 0; itr1 < _combat_total_unit_count; itr1++)
        {

            if(
                (battle_units[itr1].controller_idx != player_idx)
                &&
                (battle_units[itr1].status == bus_Active)
            )
            {
                Anim_Delay_Array[itr1] = (Random(5) - 1);
                X_Pos_Array[itr1] = battle_units[itr1].cgx;
                Y_Pos_Array[itr1] = battle_units[itr1].cgy;
            }
            else
            {
                Anim_Delay_Array[itr1] = ST_UNDEFINED;
            }



        }

        CMB_ComposeBackgrnd__WIP();
        Set_Page_Off();
        Combat_Screen_Draw();
        PageFlip_FX();
        Copy_On_To_Off_Page();
        Copy_Off_To_Back();
        Save_ScreenSeg();

        LOAD_ANIMS_LOCAL();

        for(itr1 = 0; itr1 < _combat_total_unit_count; itr1++)
        {

            effects[itr1] = (Random(8) - 1);

            Effect_Frame_Count = FLIC_Get_FrameCount(&Blank_Effect[effects[itr1]]);

            if(Effect_Frame_Count > Highest_Frame_Count)
            {
                Highest_Frame_Count = Effect_Frame_Count;
            }

        }

        if(magic_set.sound_effects == ST_TRUE)
        {
            Combat_Load_Spell_Sound_Effect(spl_Call_Chaos);
            if(SND_SpellCast != (SAMB_ptr)ST_UNDEFINED)
            {
                // DOMSDOS  Play_Sound__STUB(SND_SpellCast);
                sdl2_Play_Sound__WIP(SND_SpellCast, SND_SpellCast_size);
            }
        }

        for(itr2 = 0; (Highest_Frame_Count + 4) > itr2; itr2++)
        {

            Mark_Time();

            Set_Page_Off();

            if(((Highest_Frame_Count + 4) / 2) == 0)
            {
                
                Restore_ScreenSeg();
                Apply_Call_Chaos__WIP(player_idx, &effects[0]);
                // ; BUG: the background is not redrawn
                Set_Page_Off();
                Combat_Screen_Draw();
                Combat_Cast_Spell_Message(caster_idx, spl_Call_Chaos);
                Copy_Off_To_Back();
                Save_ScreenSeg();

                LOAD_ANIMS_LOCAL();

            }

            Copy_Back_To_Off();

            for(itr1 = 0; itr1 < _combat_total_unit_count; itr1++)
            {

                if(Anim_Delay_Array[itr1] > ST_UNDEFINED)
                {

                    Combat_Grid_Screen_Coordinates(X_Pos_Array[itr1], Y_Pos_Array[itr1], 4, 4, &screen_x, &screen_y);

                    // ; BUG: the Healing animation is also this size
                    if(
                        (effects[itr1] == 2)
                        ||
                        (effects[itr1] == 3)
                        ||
                        (effects[itr1] == 6)
                    )
                    {
                        screen_x -= 28;
                        screen_y -= 30;
                    }
                    else
                    {
                        screen_x -= 14;
                        screen_y -= 27;
                    }

                    if(
                        ((itr2 - Anim_Delay_Array[itr1]) < Highest_Frame_Count)
                        &&
                        ((itr2 - Anim_Delay_Array[itr1]) >= 0)
                    )
                    {

                        if(FLIC_Get_FrameCount(&Blank_Effect[effects[itr1]]) > (itr2 - Anim_Delay_Array[itr1]))
                        {

                            Set_Animation_Frame(&Blank_Effect[effects[itr1]], (itr2 - Anim_Delay_Array[itr1]));

                            Clipped_Draw(screen_x, screen_y, &Blank_Effect[effects[itr1]]);

                        }

                    }

                }

            }

            PageFlip_FX();

            Release_Time(2);

        }

        Restore_ScreenSeg();

        CMB_ComposeBackgrnd__WIP();

    }

    // ; BUG: this can't be done here, the effect array is not
    // ; yet filled out
    Apply_Call_Chaos__WIP(player_idx, &effects[0]);

}

// WZD o133p21
/*
; applies the effects of Call Chaos cast by the
; specified player, based on the passed effect array:
;   0 - no effect
;   1 - healing
;   2 - chaos channels
;   3 - warp creature
;   4 - fire bolt (strength 15)
;   5 - warp lightning
;   6 - doom bolt
;   7 - disintegrate (BUG: can cause damage anyway)
*/
/*

...all enemy units.
Individual units may be randomly subjected to one of the following effects:
nothing, healing of five hits of damage, chaos channels, warp creature, fire bolt of strength 15, warp lightning, doom bolt or disintegrate.
*/
void Apply_Call_Chaos__WIP(int16_t player_idx, int16_t effects[])
{
    int16_t resist_fails = 0;
    int16_t damage_types[3] = { 0, 0, 0 };
    int16_t battle_unit_idx = 0;

    for(battle_unit_idx = 0; battle_unit_idx < _combat_total_unit_count; battle_unit_idx++)
    {

        if(
            (battle_units[battle_unit_idx].controller_idx != player_idx)
            &&
            (battle_units[battle_unit_idx].status == bus_Active)
        )
        {

            switch(effects[battle_unit_idx])
            {

                case 0:
                {

                    // "nothing"

                } break;

                case 1:
                {

                    if(battle_units[battle_unit_idx].race != rt_Death)
                    {
                        Battle_Unit_Heal(battle_unit_idx, 5, 0);
                    }

                } break;

                case 2:
                {

                    if(Unit_Is_Normal(battle_units[battle_unit_idx].unit_idx) != ST_FALSE)
                    {

                        /* SPELLY */  UNIT_ChaosChannel__STUB(battle_units[battle_unit_idx].unit_idx);

                    }


                } break;

                case 3:
                {

                    resist_fails = Combat_Resistance_Check(battle_units[battle_unit_idx], -1, sbr_Chaos);

                    if(resist_fails > 0)
                    {

                        Apply_Warp_Creature(battle_unit_idx);

                    }

                } break;

                case 4:
                {

                    CMB_ConvSpellAttack__WIP(spl_Fire_Bolt, battle_unit_idx, &damage_types[0], 15);

                } break;

                case 5:
                {

                    CMB_ConvSpellAttack__WIP(spl_Warp_Lightning, battle_unit_idx, &damage_types[0], 0);

                } break;

                case 6:
                {

                    CMB_ConvSpellAttack__WIP(spl_Doom_Bolt, battle_unit_idx, &damage_types[0], 0);

                } break;

                case 7:
                {

                    if(Combat_Effective_Resistance(battle_units[battle_unit_idx], sbr_Chaos) < 10)
                    {

                        damage_types[0] = 0;
                        damage_types[1] = 0;
                        damage_types[2] = 200;

                    }

                } break;

                default:
                {

                    STU_DEBUG_BREAK();

                }

            }

        }

    }

}
