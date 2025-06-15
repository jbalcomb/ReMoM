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


// WZD o133p02  WIZ_Wrack()
// WZD o133p03  WIZ_CallLightning()

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

        Tactical_Combat_Draw();

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

        Tactical_Combat_Draw();  // incrs frame, 0-7; sets frame

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

    Tactical_Combat_Draw();

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

*/
void TILE_CombatSpellAnim__WIP(int16_t cgx, int16_t cgy, int16_t Anim_Size, int16_t caster_idx, int16_t spell_idx)
{



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

        Tactical_Combat_Draw();

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

    Tactical_Combat_Draw();

    PageFlip_FX();

    CMB_Chasm_Anim = 0;

    Release_Block(_screen_seg);

    Release_Block(World_Data);

}


// WZD o133p14
// drake178: TILE_LightningBolt()
void TILE_LightningBolt__WIP(int16_t cgx, int16_t cgy, int16_t caster_idx)
{



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

    CMB_Vortex_Array[_vortex_count].cgx = cgx;
    CMB_Vortex_Array[_vortex_count].cgy = cgy;

    CMB_Vortex_Array[_vortex_count].move_cgx = cgx;
    CMB_Vortex_Array[_vortex_count].move_cgy = cgy;

    CMB_Vortex_Array[_vortex_count].stage = 0;

    CMB_Vortex_Array[_vortex_count].owner_idx = player_idx;

    _vortex_count++;

}


// segrax
// WZD o133p16
// drake178: CMB_SetVortexCursor()
/*
*/
/*

*/
void CMB_SetVortexCursor__SEGRAX(int vortex_idx)
{
    int16_t pointer_offset = 0;
    int16_t cgy = 0;
    int16_t screen_y = 0; 
    int16_t screen_x = 0;
    int16_t cgx = 0;
    pointer_offset = 4;

    _combat_mouse_grid->image_num = crsr_RedCross;

    screen_x = Pointer_X() + pointer_offset;
    screen_y = Pointer_Y() + pointer_offset;

    frame_active_flag = ST_TRUE;

    frame_active_cgx = CMB_Vortex_Array[vortex_idx].cgx;
    frame_active_cgy = CMB_Vortex_Array[vortex_idx].cgy;

    frame_anim_cycle = ((frame_active_cgy + 1) % 3);

    if((168 + pointer_offset) > screen_y)
    {

        frame_scanned_flag = ST_FALSE;

        cgy = Get_Combat_Grid_Cell_Y(screen_x, screen_y);
        cgx = Get_Combat_Grid_Cell_X(screen_x, screen_y);

        if(
            (CMB_Vortex_Array[vortex_idx].cgx == cgx)
            &&
            (CMB_Vortex_Array[vortex_idx].cgy == cgy)
        )
        {

            if(battlefield->MoveCost_Teleport[cgx] != -1)
            {

                if(
                    (abs((cgx - CMB_Vortex_Array[vortex_idx].cgx)) <= 1)
                    &&
                    (abs((cgy - CMB_Vortex_Array[vortex_idx].cgy)) <= 1)
                )
                {

                    frame_scanned_flag = ST_TRUE;
                    frame_scanned_cgx = cgx;
                    frame_scanned_cgy = cgy;
                    _combat_mouse_grid->image_num = crsr_WingedBoot;

                }

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
void CMB_VortexPlayerMove__SEGRAX(int vortex_idx)
{
    int16_t target_cgy = 0;
    int64_t cgy = 0;
    int64_t cgx = 0;
    int16_t combat_grid_field = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t target_cgx = 0;  // _DI_

    if(CMB_Vortex_Array[vortex_idx].owner_idx != HUMAN_PLAYER_IDX)
    {
        return;
    }

    if(_auto_combat_flag != ST_FALSE)
    {
        return;
    }

    Clear_Fields();

    combat_grid_field = Add_Grid_Field(0, 0, 1, 1, 319, 168, &cgx, &cgy, ST_UNDEFINED);

    leave_screen = ST_FALSE;

    _active_battle_unit = ST_UNDEFINED;

    while(leave_screen == ST_FALSE)
    {

        Mark_Time();

        input_field_idx = Get_Input();

        if(input_field_idx == combat_grid_field)
        {

            target_cgx = Get_Combat_Grid_Cell_X(cgx + 4, cgy + 4);
            target_cgy = Get_Combat_Grid_Cell_Y(cgx + 4, cgy + 4);

            if(
                (CMB_Vortex_Array[vortex_idx].cgx != target_cgx)
                ||
                (CMB_Vortex_Array[vortex_idx].cgy != target_cgy)
            )
            {

                if(battlefield->MoveCost_Teleport[((target_cgy * COMBAT_GRID_WIDTH) + target_cgx)] != -1)
                {

                    if(
                        (abs(target_cgx - CMB_Vortex_Array[vortex_idx].cgx) <= 1)
                        &&
                        (abs(target_cgy - CMB_Vortex_Array[vortex_idx].cgy) <= 1)
                    )
                    {

                        CMB_VortexMovement__SEGRAX(vortex_idx, target_cgx, target_cgy);

                        leave_screen = ST_TRUE;

                    }

                }

            }

        }

        if(leave_screen == ST_FALSE)
        {

            Tactical_Combat_Draw();

            CMB_SetVortexCursor__SEGRAX(vortex_idx);

            PageFlip_FX();

            Release_Time(2);

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
void CMB_ProcessVortices__SEGRAX(void)
{
    int16_t tries = 0;
    int16_t Current_Y = 0;
    int16_t Current_X = 0;
    int16_t Next_Y = 0;
    int16_t itr = 0;
    int16_t vortex_idx = 0;
    int16_t Next_X = 0;
    int16_t dx = 0;

    if(magic_set.sound_effects == ST_TRUE)
    {
        // DOMSDOS  Play_Sound__WIP(SND_CMB_Silence);
        sdl2_Play_Sound__WIP(SND_CMB_Silence, SND_CMB_Silence_size);
        Mark_Block(World_Data);
        SND_SpellCast = LBX_Reload_Next(soundfx_lbx_file__ovr133__1of2, SFX_Flyer_S, World_Data);
        SND_SpellCast_size = lbxload_entry_length;
        Release_Block(World_Data);
    }

    while(vortex_idx < _vortex_count)
    {

        itr = 0;

        while(itr < 3)
        {

            // Set Current_X and Current_Y from CMB_Vortex_Array
            Current_X = CMB_Vortex_Array[vortex_idx].cgx;
            Current_Y = CMB_Vortex_Array[vortex_idx].cgy;

            Next_X = Current_X;
            Next_Y = Current_Y;

            tries = 0;

            do {

                // Generate random next position
                Next_X = Current_X + (Random(3) - 2);
                Next_Y = Current_Y + (Random(3) - 2);

                // Validate position
                if(Next_X != Current_X || Next_Y != Current_Y)
                {

                    dx = (Next_Y * COMBAT_GRID_WIDTH) + Next_X;

                    if(
                        (battlefield->MoveCost_Teleport[dx] != -1)
                        &&
                        (Next_X >= 0)
                        &&
                        (Next_X <= COMBAT_GRID_WIDTH)
                        &&
                        (Next_Y >= 0)
                        &&
                        (Next_Y <= COMBAT_GRID_HEIGHT)
                    )
                    {

                        // Calculate absolute movement distances
                        int dist_x = abs(Next_X - CMB_Vortex_Array[vortex_idx].move_cgx);
                        int dist_y = abs(Next_Y - CMB_Vortex_Array[vortex_idx].move_cgy);

                        if(
                            (dist_x + dist_y <= 1)
                            &&
                            (tries < 50)
                        )
                        {

                            CMB_VortexMovement__SEGRAX(vortex_idx, Next_X, Next_Y);

                            break;

                        }

                    }

                }

                tries++;

            } while (tries < 50);

            itr++;

        }

        CMB_Vortex_Array[vortex_idx].move_cgx = CMB_Vortex_Array[vortex_idx].cgx;
        CMB_Vortex_Array[vortex_idx].move_cgy = CMB_Vortex_Array[vortex_idx].cgy;

        CMB_VortexPlayerMove__SEGRAX(vortex_idx);

        vortex_idx++;

    }

}


// segrax
// WZD o133p19
// drake178: CMB_VortexMovement()
/*
 * moves	a Magic	Vortex from its	current	location to the
 * target tile using an 8-step animation, processing its
 * damage after reaching	the destination
 *
 * prev coordinates are set to /	used as	next ones for
 * the duration of the movement
 */
void CMB_VortexMovement__SEGRAX(int vortex_idx, int Next_X, int Next_Y)
{
    int Prev_X, Prev_Y;
    int16_t Damage_Array[3] = { 0, 0, 0 };
    int X_Distance, Y_Distance;

    // Retrieve the vortex
    // struct s_CMB_Vortex* vortex = &CMB_Vortex_Array[vortex_idx];
    struct s_MAGIC_VORTEX * vortex = &CMB_Vortex_Array[vortex_idx];
    Prev_X = vortex->cgx;
    Prev_Y = vortex->cgy;
    frame_active_flag = 0;

    // Update the vortex with the new position
    vortex->move_cgx = Next_X;
    vortex->move_cgy = Next_Y;

    if(magic_set.sound_effects == 1)
    {
        // DOMSDOS  Play_Sound__WIP(SND_SpellCast);
        sdl2_Play_Sound__WIP(SND_SpellCast, SND_SpellCast_size);
    }

    // Animate the vortex movement
    for(int i = 0; i < 8; i++)
    {
        vortex->stage = i;
        Set_Page_Off();
        Tactical_Combat_Draw();
        PageFlip_FX();
    }

    // Update vortex position
    vortex->cgx = Next_X;
    vortex->cgy = Next_Y;

    // Check if the new position affects the city damage
    if(Next_X >= 5 && Next_X <= 8 && Next_Y >= 10 && Next_Y <= 13)
    {
        CMB_CityDamage += 5;
    }

    vortex->move_cgx = Prev_X;
    vortex->move_cgy = Prev_Y;
    vortex->stage = 0;

    for(int i = 0; i < _combat_total_unit_count; i++)
    {
        struct s_BATTLE_UNIT* battleunit = &battle_units[i];
        
        struct s_UNIT* unit = &_UNITS[battleunit->unit_idx];

        int32_t Enchants = unit->enchantments | battleunit->enchantments | battleunit->item_enchantments;

        if(battleunit->cgx == Next_X && battleunit->cgy == Next_Y)
        {

            if(battleunit->Attribs_1 & USA_IMMUNITY_MAGIC)
            {

                continue;

            }

            if(!(Enchants & UE_RIGHTEOUSNESS))
            {
                Damage_Array[0] = 5;
                Damage_Array[1] = 0;
                Damage_Array[2] = 0;

                BU_ApplyDamage__WIP(i, Damage_Array);

            }
        }

        X_Distance = abs(battleunit->cgx - Next_X);
        Y_Distance = abs(battleunit->cgy - Next_Y);

        if((X_Distance <= 1 && Y_Distance <= 1) && (X_Distance + Y_Distance != 0))
        {
            if(Random(3) == 1)
            {

                CMB_ConvSpellAttack__WIP(spl_Lightning_Bolt, i, Damage_Array, 0);

                BU_ApplyDamage__WIP(i, Damage_Array);

            }

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
        Tactical_Combat_Draw();
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
                Tactical_Combat_Draw();
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
