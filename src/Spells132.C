/*
    WIZARDS.EXE
        ovr132
*/

#include "Explore.H"
#include "MOM_DEF.H"
#include "MOX/Allocate.H"
#include "MOX/MOX_BASE.H"
#include "MOX/MOX_DAT.H"
#include "MOX/MOX_DEF.H"
#include "MOX/MOX_T4.H"
#include "MainScr.H"
#include "MainScr_Maps.H"
#include "SPELLDEF.H"
#include "STU/STU_CHK.H"
#include "Spells130.H"
#include "Spells132.H"

#include "MOX/LBX_Load.H"
#include "MOX/MOM_Data.H"
#include "MOX/MOX_TYPE.H"
#include "MOX/SOUND.H"


#include "NEXTTURN.H"
#include "SBookScr.H"
#include "Spellbook.H"
#include "Spells137.H"
#include "UNITTYPE.H"



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
    |-> WIZ_HireHero()
        |-> Create_Unit__WIP()

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

        Full_Draw_Main_Screen();

        LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 47, 1, 150);

        Warn0(GUI_NearMsgString);

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
                (
                    (player_idx == HUMAN_PLAYER_IDX)
                    &&
                    (_units < 980)
                )
            )
        )
        {

            if(player_idx != HUMAN_PLAYER_IDX)
            {

                if(
                    (magic_set.enemy_spells == ST_TRUE)
                    &&
                    (_players[HUMAN_PLAYER_IDX].Globals[DETECT_MAGIC] != ST_FALSE)
                    &&
                    (_players[HUMAN_PLAYER_IDX].Dipl.Contacted[player_idx] == ST_TRUE)
                )
                {

                    if((_HEROES2[player_idx]->heroes[unit_type].abilities & HSA_FEMALE) != 0)
                    {

                        IDK_SummonAnim(-3, 3, player_idx);

                    }
                    else
                    {
                        
                        IDK_SummonAnim(-2, 3, player_idx);

                    }

                }

                WIZ_HireHero(player_idx, unit_type, hero_slot_idx, 0);

                UNIT_RemoveExcess((_units - 1));

                _HEROES2[player_idx]->heroes[unit_type].Level = -20;  // Why?

            }
            else
            {

                if((_HEROES2[player_idx]->heroes[unit_type].abilities & HSA_FEMALE) != 0)
                {

                    if(type == 1)
                    {

                        IDK_SummonAnim(-3, 3, player_idx);

                    }
                    else  /* type == 2 */
                    {

                        IDK_SummonAnim(-30, 3, player_idx);

                    }

                }
                else
                {

                    if(type == 1)
                    {

                        IDK_SummonAnim(-2, 3, player_idx);

                    }
                    else  /* type == 2 */
                    {

                        IDK_SummonAnim(-20, 3, player_idx);

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

                    // ¿ BUGBUG  no unit_idx param  Select_Stack_At_Unit();  ...as-compiled, would be _map_plane, in AX?
                    Select_Stack_At_Unit((_units - 1));

                }

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
            ||
            (
                (_units < 950)
                &&
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

                        _active_world_x = _players[HUMAN_PLAYER_IDX].summon_wx;
                        _active_world_y = _players[HUMAN_PLAYER_IDX].summon_wy;
                        _map_plane = _players[HUMAN_PLAYER_IDX].summon_wp;

                        // ¿ BUGBUG  no unit_idx param  Select_Stack_At_Unit();  ...as-compiled, would be _map_plane, in AX?
                        Select_Stack_At_Unit((_units - 1));

                    }

                }
                else
                {

                    WIZ_HireHero(player_idx, unit_type, hero_slot_idx, 0);

                    UNIT_RemoveExcess((_units - 1));

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
// CTY_ChaosRift()

// WZD o132p04
// WIZ_MeteorStorm()

// WZD o132p05
// CTY_StreamOfLife()

// WZD o132p06
// ¿ MoO2  Module: OFFICER  Set_Officer_To_Player_()
// Hire_Officer_() |-> Set_Officer_To_Player_()  ...Hire_Officer_() handles the officer cost
/*

    assumes (_units - 1)

unit_type_idx is passed over all the wzay from Hire_Hero_Popup()
so, not quite 'Generate Random Hero'


XREF:
    sub_B4250()
    sub_B4471()
    sub_B4E00()
    j_WIZ_HireHero()
        Hire_Hero_Popup()
        AI_Accept_Hero()
e.g.,
    Hire_Hero_Popup()
        |-> j_WIZ_HireHero(HUMAN_PLAYER_IDX, unit_type_idx, hero_slot_idx, 0)

    void AI_Accept_Hero(int16_t player_idx, int16_t hero_slot_idx, int16_t unit_type_idx)
        Hero_Hired = WIZ_HireHero(player_idx, unit_type_idx, hero_slot_idx, 0);

"Hire" as in "Summon"
*/
int16_t WIZ_HireHero(int16_t player_idx, int16_t unit_type_idx, int16_t hero_slot_idx, int16_t saved_flag)
{
    int16_t itr;

    Create_Unit__WIP(unit_type_idx, player_idx, FORTX(), FORTY(), FORTP(), -1);

    _UNITS[(_units - 1)].Finished = ST_FALSE;

    _UNITS[(_units - 1)].moves2 = _UNITS[(_units - 1)].moves2_max;

    _UNITS[(_units - 1)].Hero_Slot = hero_slot_idx;

    _players[player_idx].Heroes[hero_slot_idx].unit_idx = (_units - 1);

    Hero_Slot_Types(unit_type_idx, _players[player_idx].Heroes[hero_slot_idx].Item_Slots);

    for(itr = 0; itr < NUM_HERO_ITEM_SLOTS; itr++)
    {

        _players[HUMAN_PLAYER_IDX].Heroes[hero_slot_idx].Items[itr] = ST_UNDEFINED;

    }

    if(saved_flag == ST_TRUE)
    {

        if(player_idx == HUMAN_PLAYER_IDX)
        {

            strcpy(_players[player_idx].Heroes[hero_slot_idx].name, hero_names_table[unit_type_idx].name);

            _UNITS[(_units - 1)].XP = hero_names_table[unit_type_idx].experience_points;

            _UNITS[(_units - 1)].Level = Calc_Unit_Level((_units - 1));

        }
        else
        {

            LBX_Load_Data_Static(names_lbx_file__ovr132, 0, (SAMB_ptr)_players[player_idx].Heroes[hero_slot_idx].name, ((player_idx * 35) + unit_type_idx), 1, 13);

            _UNITS[(_units - 1)].Level = abs(_HEROES2[player_idx]->heroes[unit_type_idx].Level);

            _UNITS[(_units - 1)].XP = TBL_Experience[_UNITS[(_units - 1)].Level];

        }

    }
    else
    {

        LBX_Load_Data_Static(names_lbx_file__ovr132, 0, (SAMB_ptr)_players[player_idx].Heroes[hero_slot_idx].name, ((player_idx * 35) + unit_type_idx), 1, 13);

        SETMAX(_HEROES2[player_idx]->heroes[unit_type_idx].Level, HL_GRANDLORD);

    }

    _UNITS[(_units - 1)].Level = _HEROES2[player_idx]->heroes[unit_type_idx].Level;

    _UNITS[(_units - 1)].XP = TBL_Experience[_HEROES2[player_idx]->heroes[unit_type_idx].Level];

    // BUG  Did this used to do something different? What tests it?
    // may be is/was success status as in cast the spell
    // NOTE(JimBalcomb,202409090905): AI_Accept_Hero() tests this
    return ST_TRUE;

}


// WZD o132p07
int16_t Cast_Resurrection(int16_t player_idx)
{

    return ST_FALSE;

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

    _map_x = IDK_map_x;
    _map_y = IDK_map_y;
    _map_plane = IDK_map_p;

    troop_count = 0;

    for(itr = 0; itr < _num_players; itr++)
    {

        if(_players[itr].Globals[PLANAR_SEAL] > ST_FALSE)
        {

            if(itr == HUMAN_PLAYER_IDX)
            {

                LBX_Load_Data_Static(message_lbx_file__ovr132, 0, (SAMB_ptr)&GUI_NearMsgString[0], 54, 1, 150);

                Warn0(GUI_NearMsgString);

            }

            return ST_FALSE;

        }

    }

    if(player_idx == HUMAN_PLAYER_IDX)
    {

        return_value = Spell_Casting_Screen__WIP(stt_Friendly_Group, &scsv1, &scsv2, &scsv3, &scsv4, &scsv5, cnst_PlaneShift_2);

    }
    else
    {

        return_value = Get_Map_Square_Target_For_Spell(stt_Friendly_Group, &scsv1, &scsv2, &scsv3, spl_Plane_Shift, player_idx);

    }

    scsv2 = _UNITS[scsv1].wy;
    scsv3 = _UNITS[scsv1].wp;
    scsv1 = _UNITS[scsv1].wx;

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

            OVL_BringIntoView(&_map_x, &_map_y, scsv1, scsv2, scsv3);

            _map_plane = scsv3;
            scsv4 = scsv1;
            scsv5 = scsv2;

            World_To_Screen(_map_x, _map_y, &scsv4, &scsv5);

            if(SND_SpellCast != (SAMB_ptr)ST_UNDEFINED)
            {
                // DOMSDOS  Play_Sound__STUB(SND_SpellCast);
                sdl2_Play_Sound__WIP(SND_SpellCast, SND_SpellCast_size);
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

                    Vanish_Bitmap__WIP(bitmap, itr);

                    Draw_Picture(scsv4, scsv5, bitmap);

                    Apply_Palette();

                    PageFlip_FX();

                }

            }

        }

        if(plane_shift_status != 0)
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
                    (Check_Square_Scouted(scsv1, scsv2, scsv3) != ST_FALSE)
                )
                {

                    _map_plane = ((_map_plane + 1) % 2);

                    Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

                    Set_Map_Square_Explored_Flags_XYP(scsv1, scsv2, _map_plane);

                    _page_flip_effect = pfe_TogglePagesFadeIn;

                    Set_Page_Off();

                    Main_Screen_Draw();

                    PageFlip_FX();

                    _page_flip_effect = pfe_None;

                    Update_Scouted_And_Contacted__WIP();

                    Cast_Plane_Shift_Draw(scsv1, scsv2, _map_plane);

                    for(itr = 100; itr >= 50; itr -= 10)
                    {

                        Copy_Bitmap_To_Bitmap(bitmap, spell_animation_seg);

                        Set_Page_Off();

                        Reset_Map_Draw();

                        Main_Screen_Draw();

                        Vanish_Bitmap__WIP(bitmap, itr);

                        Draw_Picture(scsv4, scsv5, bitmap);

                        Apply_Palette();

                        PageFlip_FX();

                    }

                    if(player_idx == HUMAN_PLAYER_IDX)
                    {

                        for(itr = 50; itr >= 100; itr += 10)
                        {

                            Copy_Bitmap_To_Bitmap(bitmap, spell_animation_seg);

                            Set_Page_Off();

                            Reset_Map_Draw();
                            
                            Main_Screen_Draw();

                            Vanish_Bitmap__WIP(bitmap, itr);

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
        else
        {

            for(itr = 0; itr < troop_count; itr++)
            {

                _UNITS[troops[itr]].wp = ((_UNITS[troops[itr]].wp + 1) % 2);

            }

            if(
                (player_idx == HUMAN_PLAYER_IDX)
                ||
                (Check_Square_Scouted(scsv1, scsv2, scsv3) != ST_FALSE)
            )
            {

                _map_plane = ((_map_plane + 1) % 2);

                range = Player_Units_Max_Range(player_idx, scsv1, scsv2, _map_plane);

                // TODO  DIPLO  AI_ContactWizards(player_idx, scsv1, scsv2, _map_plane, range);

                Set_Map_Square_Explored_Flags_XYP(scsv1, scsv2, _map_plane);

                Update_Scouted_And_Contacted__WIP();

                Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

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
                (Check_Square_Scouted(scsv1, scsv2, scsv3) != ST_FALSE)
            )
        )
        {

            // DOMSDOS  Play_Sound__STUB(sound_buffer);
            sdl2_Play_Sound__WIP(sound_buffer, sound_buffer_size);

            scsv4 = scsv1;
            scsv5 = scsv2;

            World_To_Screen(_map_x, _map_y, &scsv4, &scsv5);

            _map_plane = _UNITS[troops[0]].wp;

            Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);

            Cast_Plane_Shift_Draw(scsv1, scsv2, _map_plane);

            if(SND_SpellCast != (SAMB_ptr)ST_UNDEFINED)
            {
                // DOMSDOS  Play_Sound__STUB(SND_SpellCast);
                sdl2_Play_Sound__WIP(SND_SpellCast, SND_SpellCast_size);
            }

            for(itr = 100; itr >= 0; itr -= 5)
            {

                Copy_Bitmap_To_Bitmap(bitmap, spell_animation_seg);

                Set_Page_Off();

                Reset_Map_Draw();

                Main_Screen_Draw();

                Vanish_Bitmap__WIP(bitmap, itr);

                Draw_Picture(scsv4, scsv5, bitmap);

                Apply_Palette();

                PageFlip_FX();

            }

            // DOMSDOS  Play_Sound__STUB(sound_buffer);
            sdl2_Play_Sound__WIP(sound_buffer, sound_buffer_size);

        }

    }

    Release_Block(_screen_seg);

    _map_x = IDK_map_x;
    _map_y = IDK_map_y;
    _map_plane = IDK_map_p;

    return return_value;

}


// WZD o132p11
// WIZ_GreatWasting()

// WZD o132p12
// CTY_GaiasBlessing()

// WZD o132p13
// WIZ_Armageddon()

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
            /* SPELLY */  if((valid_cast = Spell_Casting_Screen__WIP(stt_Map_Square, &wx, &wy, &wp, &target_wx, &target_wy, "Floating Island")) == ST_TRUE)
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
        
        AI_Eval_After_Spell = ST_TRUE;

        // DOMSDOS  Allocate_Reduced_Map__1();
        Allocate_Reduced_Map();

        Mark_Block(_screen_seg);

        Spell_Animation_Load_Graphics(spl_Floating_Island);

        Spell_Animation_Load_Sound_Effect__WIP(spl_Floating_Island);

        Spell_Animation_Screen__WIP(wx, wy, wp);

        Full_Draw_Main_Screen();

        Release_Block(_screen_seg);

    }

    Create_Unit__WIP(spell_data_table[spl_Floating_Island].unit_type, player_idx, wx, wy, wp, ST_UNDEFINED);
   
}


// WZD o132p15
// CTY_NightshadeDispel()

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

        return_value = IDK_Pick_Target_For_Unit_Enchantment__STUB(stt_Friendly_Unit, &unit_idx, spl_Lycanthropy, player_idx);

    }
    else
    {

        return_value = ST_TRUE;

        status = ST_FALSE;

        while((return_value == ST_TRUE) && (status == ST_FALSE))
        {
            
            return_value = Spell_Casting_Screen__WIP(stt_Friendly_Unit, &unit_idx, &scsv2, &scsv3, &scsv4, &scsv5, aLycanthropy);

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

        _UNITS[unit_idx].type = spell_data_table[spl_Lycanthropy].unit_type;

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

        return_value = Pick_Target_For_City_Enchantment__WIP(stt_Friendly_City, &city_idx, spl_Wall_Of_Stone, player_idx);

    }
    else
    {

        status = ST_FALSE;

        return_value = ST_TRUE;

        while((status == ST_FALSE) && (return_value == ST_TRUE))
        {

            return_value = Spell_Casting_Screen__WIP(stt_Friendly_City, &city_idx, &wy, &wp, &target_wx, &target_wy, aWallOfStone);

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

            Cast_Spell_City_Enchantment_Animation_1__WIP(city_idx, spl_Wall_Of_Stone, player_idx);

        }

        _CITIES[city_idx].bldg_status[bt_CityWalls] = bs_Built;
Capture_Cities_Data();

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

            Cast_Spell_City_Enchantment_Animation_2__WIP(city_idx, spl_Wall_Of_Stone, player_idx);

        }

    }

    Release_Block(_screen_seg);

    return return_value;

}

// WZD o132p18
int16_t Cast_SpellOfReturn(int16_t player_idx)
{

    return ST_FALSE;

}
