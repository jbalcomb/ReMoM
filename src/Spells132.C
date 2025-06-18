/*
    WIZARDS.EXE
        ovr132
*/

#include "MOX/MOX_DEF.H"
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
int16_t Cast_SummonHero(int16_t player_idx, int16_t type)
{

    return ST_FALSE;

}

// WZD o132p02
int16_t Cast_Incarnation(int16_t player_idx)
{

    return ST_FALSE;

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

    _UNITS[(_units - 1)].Finished = 0;

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
// sub_B50AE()

// WZD o132p09
// sub_B517B()

// WZD o132p10
int16_t Cast_PlaneShift(int16_t player_idx)
{

    return ST_FALSE;

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

    return ST_FALSE;

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

            return_value = Spell_Casting_Screen__WIP(stt_Friendly_City, &city_idx, &wy, &wp, &target_wx, &target_wy, aWallOfStone);  // "Wall of Stone"

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
