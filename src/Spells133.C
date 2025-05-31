/*
    WIZARDS.EXE
        ovr133
*/

#include "Spells133.H"

#include "MOX/MOX_DEF.H"
#include "MOX/MOX_TYPE.H"
#include "MOX/SOUND.H"

#include "Combat.H"



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
// WZD  dseg:697D 6D 6F 6E 73 74 65 72 00                         monster_lbx_file__ovr133 db 'monster',0 ; DATA XREF: CMB_CallChaos+11Bo ...
// WZD  dseg:6985 73 70 65 63 66 78 00                            specfx_lbx_file__ovr133 db 'specfx',0   ; DATA XREF: CMB_CallChaos+132o ...
// WZD  dseg:698C 63 6D 62 74 66 78 00                            spec_lbx_file__ovr133 db 'cmbtfx',0     ; DATA XREF: CMB_CallChaos+166o ...

// WZD  dseg:6993 00                                              align 2

// WZD  dseg:6993                                                 END:  ovr133 - Initialized Data




/*
    WIZARDS.EXE  ovr133
*/

// WZD o133p01  BU_WarpCreature()
// WZD o133p02  WIZ_Wrack()
// WZD o133p03  WIZ_CallLightning()
// WZD o133p04  BU_LifeDrain()
// WZD o133p05  CMB_WallRise_Anim()

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


// WZD o133p08  TILE_CracksCall()
// WZD o133p09  TILE_BoltFromAbove()
// WZD o133p10  TILE_CombatSpellAnim()
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

    Spell_Animation_Load_Graphics__WIP(spl_Fire_Elemental);

    IMG_GUI_Chasm = spell_animation_seg;

    CMB_Chasm_Anim = 1;

    Mark_Block(World_Data);

    if(magic_set.sound_effects == ST_TRUE)
    {
        // DOMSDOS  Play_Sound__STUB(SND_SpellCast);
        sdl2_Play_Sound__WIP(SND_SpellCast, SND_SpellCast_size);
        SND_SpellCast = LBX_Reload_Next(soundfx_lbx_file__ovr133__2of2, SFX_CombatSummon, World_Data);
        SND_SpellCast_size = lbxload_entry_length;
    }
    else
    {
        SND_SpellCast = ST_UNDEFINED;
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

    if(SND_SpellCast != ST_UNDEFINED)
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

        CMB_SpellcastMessage__WIP(player_idx, spell_idx);

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


// WZD o133p14  TILE_LightningBolt()


// WZD o133p15  WIZ_CreateVortex()


// segrax
// WZD o133p16
// drake178: CMB_SetVortexCursor()
/*
*/
/*

*/
void CMB_SetVortexCursor__SEGRAX(int Vortex_Index)
{
    int Pointer_Offset = 4;
    int Tile_Y, Scrn_Y, Scrn_X;

    _combat_mouse_grid->image_num = crsr_RedCross;

    Scrn_X = Pointer_X() + Pointer_Offset;
    Scrn_Y = Pointer_Y() + Pointer_Offset;

    frame_active_flag = ST_TRUE;

    frame_active_cgx = CMB_Vortex_Array[Vortex_Index].cgx;
    frame_active_cgy = CMB_Vortex_Array[Vortex_Index].cgy;

    frame_anim_cycle = (frame_active_cgy + 1) % 3;

    if((Pointer_Offset + 168) > Scrn_Y)
    {

        frame_scanned_flag = ST_FALSE;

        Tile_Y = Get_Combat_Grid_Cell_Y(Scrn_X, Scrn_Y);
        int Grid_X = Get_Combat_Grid_Cell_X(Scrn_X, Scrn_Y);

        if(
            (CMB_Vortex_Array[Vortex_Index].cgx == Grid_X)
            &&
            (CMB_Vortex_Array[Vortex_Index].cgy == Tile_Y)
        )
        {

            if(battlefield->MoveCost_Teleport[Grid_X] != -1)
            {
                if(abs((int)(Grid_X - CMB_Vortex_Array[Vortex_Index].cgx)) <= 1
                &&
                    abs((int)(Tile_Y - CMB_Vortex_Array[Vortex_Index].cgy)) <= 1
                )
                {
                    frame_scanned_flag = ST_TRUE;
                    frame_scanned_cgx = Grid_X;
                    frame_scanned_cgy = Tile_Y;
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
void CMB_VortexPlayerMove__SEGRAX(int Vortex_Index)
{
    // int16_t Picked_Y, Y_Retn, X_Retn;
    int16_t cgy = 0;
    int64_t Y_Retn = 0;
    int64_t X_Retn = 0;
    int16_t combat_grid_field = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t cgx = 0;  // _DI_

    if(CMB_Vortex_Array[Vortex_Index].owner_idx != HUMAN_PLAYER_IDX)
    {
        return;
    }

    if(_auto_combat_flag != ST_FALSE)
    {
        return;
    }

    Clear_Fields();

    // int16_t Add_Grid_Field(int16_t xmin, int16_t ymin, int16_t box_width, int16_t box_height, int16_t horizontal_count, int16_t vertical_count, int64_t *xpos, int64_t *ypos, int16_t help);
    combat_grid_field = Add_Grid_Field(0, 0, 1, 1, 319, 168, &X_Retn, &Y_Retn, ST_UNDEFINED);

    leave_screen = ST_FALSE;

    _active_battle_unit = ST_UNDEFINED;

    while(leave_screen == ST_FALSE)
    {
        Mark_Time();

        input_field_idx = Get_Input();

        if(input_field_idx == combat_grid_field)
        {

            cgx = Get_Combat_Grid_Cell_X(X_Retn + 4, Y_Retn + 4);
            cgy = Get_Combat_Grid_Cell_Y(X_Retn + 4, Y_Retn + 4);

            if(
                (CMB_Vortex_Array[Vortex_Index].cgx != cgx)
                ||
                (CMB_Vortex_Array[Vortex_Index].cgy != cgy)
            )
            {

                if(battlefield->MoveCost_Teleport[((cgy * COMBAT_GRID_WIDTH) + cgx)] != -1)
                {

                    if(
                        (abs(cgx - CMB_Vortex_Array[Vortex_Index].cgx) <= 1)
                        &&
                        (abs(cgy - CMB_Vortex_Array[Vortex_Index].cgy) <= 1)
                    )
                    {

                        CMB_VortexMovement__SEGRAX(Vortex_Index, cgx, cgy);

                        leave_screen = ST_TRUE;

                    }

                }

            }

        }

        if(leave_screen == ST_FALSE)
        {

            Tactical_Combat_Draw();

            CMB_SetVortexCursor__SEGRAX(Vortex_Index);

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
    int Tries_Counter, Current_Y, Current_X, Next_Y, Loop_Var;
    unsigned int Vortex_Index = 0;

    if(magic_set.sound_effects == ST_TRUE)
    {
        // DOMSDOS  Play_Sound__WIP(SND_CMB_Silence);
        sdl2_Play_Sound__WIP(SND_CMB_Silence, SND_CMB_Silence_size);
        Mark_Block(World_Data);
        SND_SpellCast = LBX_Reload_Next(soundfx_lbx_file__ovr133__1of2, SFX_Flyer_S, World_Data);
        SND_SpellCast_size = lbxload_entry_length;
        Release_Block(World_Data);
    }

    while (Vortex_Index < CMB_Vortex_Count)
    {
        Loop_Var = 0;

        while (Loop_Var < 3) {
            int Next_X, dx;

            // Set Current_X and Current_Y from CMB_Vortex_Array
            Current_X = CMB_Vortex_Array[Vortex_Index].cgx;
            Current_Y = CMB_Vortex_Array[Vortex_Index].cgy;

            Next_X = Current_X;
            Next_Y = Current_Y;
            Tries_Counter = 0;

            do {
                // Generate random next position
                Next_X = Current_X + (Random(3) - 2);
                Next_Y = Current_Y + (Random(3) - 2);

                // Validate position
                if (Next_X != Current_X || Next_Y != Current_Y) {
                    dx = (Next_Y * COMBAT_GRID_WIDTH) + Next_X;

                    if (battlefield->MoveCost_Teleport[dx] != -1 &&
                        Next_X >= 0 && Next_X <= COMBAT_GRID_WIDTH &&
                        Next_Y >= 0 && Next_Y <= COMBAT_GRID_HEIGHT) {

                        // Calculate absolute movement distances
                        int dist_x = abs(Next_X - CMB_Vortex_Array[Vortex_Index].Prev_or_Next_X);
                        int dist_y = abs(Next_Y - CMB_Vortex_Array[Vortex_Index].Prev_or_Next_Y);

                        if (dist_x + dist_y <= 1 && Tries_Counter < 50) {
                            CMB_VortexMovement__SEGRAX(Vortex_Index, Next_X, Next_Y);
                            break;
                        }
                    }
                }

                Tries_Counter++;
            } while (Tries_Counter < 50);

            Loop_Var++;
        }

        CMB_Vortex_Array[Vortex_Index].Prev_or_Next_X = CMB_Vortex_Array[Vortex_Index].cgx;
        CMB_Vortex_Array[Vortex_Index].Prev_or_Next_Y = CMB_Vortex_Array[Vortex_Index].cgy;

        CMB_VortexPlayerMove__SEGRAX(Vortex_Index);
        Vortex_Index++;
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
void CMB_VortexMovement__SEGRAX(int Vortex_Index, int Next_X, int Next_Y)
{
    int Prev_X, Prev_Y;
    int16_t Damage_Array[3] = { 0, 0, 0 };
    int X_Distance, Y_Distance;

    // Retrieve the vortex
    // struct s_CMB_Vortex* vortex = &CMB_Vortex_Array[Vortex_Index];
    struct s_MAGIC_VORTEX * vortex = &CMB_Vortex_Array[Vortex_Index];
    Prev_X = vortex->cgx;
    Prev_Y = vortex->cgy;
    frame_active_flag = 0;

    // Update the vortex with the new position
    vortex->Prev_or_Next_X = Next_X;
    vortex->Prev_or_Next_Y = Next_Y;

    if(magic_set.sound_effects == 1)
    {
        // DOMSDOS  Play_Sound__WIP(SND_SpellCast);
        sdl2_Play_Sound__WIP(SND_SpellCast, SND_SpellCast_size);
    }

    // Animate the vortex movement
    for (int i = 0; i < 8; i++) {
        vortex->Move_Stage = i;
        Set_Page_Off();
        Tactical_Combat_Draw();
        PageFlip_FX();
    }

    // Update vortex position
    vortex->cgx = Next_X;
    vortex->cgy = Next_Y;

    // Check if the new position affects the city damage
    if (Next_X >= 5 && Next_X <= 8 && Next_Y >= 10 && Next_Y <= 13) {
        CMB_CityDamage += 5;
    }

    vortex->Prev_or_Next_X = Prev_X;
    vortex->Prev_or_Next_Y = Prev_Y;
    vortex->Move_Stage = 0;

    for (int i = 0; i < _combat_total_unit_count; i++) {
        struct s_BATTLE_UNIT* battleunit = &battle_units[i];
        
        struct s_UNIT* unit = &_UNITS[battleunit->unit_idx];
        int32_t Enchants = unit->enchantments | battleunit->enchantments | battleunit->item_enchantments;

        if (battleunit->cgx == Next_X && battleunit->cgy == Next_Y) {

            if (battleunit->Attribs_1 & USA_IMMUNITY_MAGIC) {
                continue;
            }

            if (!(Enchants & UE_RIGHTEOUSNESS)) {
                Damage_Array[0] = 5;
                Damage_Array[1] = 0;
                Damage_Array[2] = 0;

                BU_ApplyDamage__WIP(i, Damage_Array);
            }
        }

        X_Distance = abs(battleunit->cgx - Next_X);
        Y_Distance = abs(battleunit->cgy - Next_Y);
        if ((X_Distance <= 1 && Y_Distance <= 1) && (X_Distance + Y_Distance != 0)) {
            if (Random(3) == 1) {
                CMB_ConvSpellAttack(spl_Lightning_Bolt, i, Damage_Array, 0);
                BU_ApplyDamage__WIP(i, Damage_Array);
            }
        }
    }
}


// WZD o133p20  CMB_CallChaos()


// WZD o133p21  WIZ_CallChaos()
