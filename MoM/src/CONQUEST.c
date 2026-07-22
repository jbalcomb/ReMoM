/*
    WIZARDS.EXE
        ovr093

MoO2
Module: ERIC

¿ CONQUEST.LBX ... CONQUEST.c/h. ?
*/

#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/EXIT.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/GENDRAW.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/paragrph.h"
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/Timer.h"
#include "../../MoX/src/Util.h"
#include "../../MoX/src/MOM_DAT.h"  /* _cities[], _UNITS[] */
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOM_DEF.h"  /* NEUTRAL_PLAYER_IDX */
#include "../../MoX/src/MOX_DEF.h"  /* NEUTRAL_PLAYER_IDX */
#include "../../MoX/src/MOX_SET.h"  /* magic_set */

#include "City_ovr55.h"
#include "CITYCALC.h"
#include "Combat.h"
#include "DIPLOMAC.h"
#include "MainScr.h"  /* Allocate_Reduced_Map(); Play_Background_Music() */
#include "NEXTTURN.h"
#include "SBookScr.h"   /* Full_Draw_Main_Screen() */
#include "SCORE.h"
#include "Spellbook.h"
#include "Spells128.h"
#include "UNITTYPE.h"  /* _unit_type_table[] */
#include "WZD_o059.h"

#include "../../ext/stu_compat.h"

#include "CONQUEST.h"



// WZD dseg:511C                                                 BEGIN:  ovr093 - Initialized Data

// sizeof  0x8  8d
#pragma pack(push)
#pragma pack(2)
struct Spiral_Anim_Step
{
    /* 00 */ int16_t x;
    /* 02 */ int16_t y;
    /* 04 */ int16_t a;
    /* 06 */ int16_t s;
};
#pragma pack(pop)

struct Spiral_Anim_Step TBL_DefeatAnimSteps[27] = {
    {   0,  90,   0, 100 },
    {  12,  73,  10, 100 },
    {  25,  57,  20, 100 },
    {  32,  45,  30, 100 },
    {  40,  29,  40, 100 },
    {  42,  15,  50, 100 },
    {  55,   3,  60, 100 },
    {  77,  -4,  70, 100 },
    { 119, -10,  80,  95 },
    { 147,  -8,  90,  90 },
    { 170,  -5, 100,  85 },
    { 190,   7, 110,  80 },
    { 212,  19, 120,  75 },
    { 218,  40, 130,  70 },
    { 227,  59, 140,  65 },
    { 217,  79, 150,  60 },
    { 212,  97, 160,  55 },
    { 196, 106, 170,  50 },
    { 179, 113, 180,  45 },
    { 160, 109, 190,  40 },
    { 145, 105, 200,  35 },
    { 135, 100, 210,  30 },
    { 130,  94, 220,  25 },
    { 135,  87, 230,  20 },
    { 147,  87, 240,  15 },
    { 152,  89, 250,  10 },
    { 159,  98, 260,   5 }
};

// WZD dseg:51F4
char magic_exe_file__ovr093[] = "MAGIC.EXE";
// WZD dseg:51FD
char str_empty_string__ovr093[] = "";
// WZD dseg:51FE
char str_JENNY__ovr093[] = "JENNY";
// WZD dseg:5204
char str_SPACE__ovr093[] = " ";
// WZD dseg:5206
char wizlab_lbx_file__ovr093[] = "WIZLAB";
// WZD dseg:520D
char cnst_Conquest_Msg3[] = "banishes";
// WZD dseg:5216
char cnst_Conquest_Msg4[] = "defeats";
// WZD dseg:521E
char cnst_Raiders_2[] = "Raiders";
// WZD dseg:5226
char cnst_Conquest_Msg1[] = "destroy";
// WZD dseg:522E
char cnst_Conquest_Msg2[] = "fortress";
// WZD dseg:5237
char cnst_Dot7[] = ".";
// WZD dseg:5239
char conquest_lbx_file__ovr093[] = "CONQUEST";
// WZD dseg:5242
char music_lbx_file__ovr093[] = "MUSIC";
// WZD dseg:5248
char str_hotkey_ESC__ovr093[] = "\x1B";
// WZD dseg:524A
char soundfx_lbx_file__ovr093[] = "SOUNDFX";
// WZD dseg:5252
char win_lbx_file__ovr093[] = "WIN";
// WZD dseg:5256
char cnst_WIN_Msg_1 [] = "Having conquered both the";
// WZD dseg:5270
char cnst_WIN_Msg_2 [] = "world of Arcanus and Myrror,";
// WZD dseg:528D
char cnst_WIN_Msg_3 [] = "I and only I, remain the one";
// WZD dseg:52AA
char cnst_WIN_Msg_4 [] = "and true Master of Magic.";
// WZD dseg:52C4
char lose_lbx_file__ovr093[] = "LOSE";
// WZD dseg:52C9
char cnst_SoReturn_Msg1[] = "There are at least ";
// WZD dseg:52DD
char cnst_SoReturn_Msg2[] = " turns remaining until you may return. Do you wish to resign?";
// WZD dseg:531B
char cnst_SoReturn_Msg3[] = " begins casting the Spell Of Return.";
// WZD dseg:5340
char vortex_lbx_file__ovr093[] = "VORTEX";
// WZD dseg:5347
char cmbtfx_lbx_file__ovr093[] = "CMBTFX";

// WZD dseg:5347                                                 END:  ovr093 - Initialized Data



// WZD dseg:C5BA                                                 BEGIN:  ovr093 - Uninitialized Data

// WZD dseg:C5BA
SAMB_ptr IMG_GAME_LimboFall;
// WZD dseg:C5BC
int16_t GAME_LimboFall_Stage;
// WZD dseg:C5BE
SAMB_ptr IMG_GAME_WizHandsUp;
// WZD dseg:C5C0
SAMB_ptr IMG_GAME_RedSparkle;
// WZD dseg:C5C2
int16_t GAME_Conquering_Wiz;
// WZD dseg:C5C4
int16_t GAME_Conquered_Wiz;
// WZD dseg:C5C6
SAMB_ptr SND_GAME_Scream;
uint32_t SND_GAME_Scream_size;  // DNE in Dasm
// WZD dseg:C5C8
int16_t m_conquest_anim_stage;
// WZD dseg:C5CA
SAMB_ptr conquest_effect_seg;
// WZD dseg:C5CC
SAMB_ptr IMG_GAME_GrowZap;
// WZD dseg:C5CE
SAMB_ptr conquest_axeboy_seg;
// WZD dseg:C5D0
SAMB_ptr conquest_conehead_seg;
// WZD dseg:C5D2
SAMB_ptr conquest_wizard_seg;
// WZD dseg:C5D4
SAMB_ptr wizlab_wizard_seg__ovr093;

// WZD dseg:C5D4                                                 END:  ovr093 - Uninitialized Data



// WZD 093p01
/*
OON XREF:  Change_City_Ownership() |-> j_Resolve_Wizard_Conquest() |-> Resolve_Wizard_Conquest()
*/
void Resolve_Wizard_Conquest(int16_t loser_Idx, int16_t winner_idx, int16_t city_idx)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t resign = 0;
    int16_t city_count = 0;
    uint8_t * city_enchantments;
    uint8_t * overland_enchantments;
    int16_t troop_count = 0;
    int16_t itr_cench = 0;
    int16_t itr_troops = 0;
    int16_t itr_cities = 0;
    int16_t itr_nodes = 0;
    int16_t itr_units = 0;
    int16_t itr_oench = 0;
    int16_t itr_players = 0;
    /* EOG_HACK */ if(magic_master_idx != ST_UNDEFINED) { return; }  /* JIC, AI wins, but then Event_Twiddle - Rebellion */
    for(itr_cities = 0, city_count = 0; ((itr_cities < _cities) && (city_count < 2)); itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == loser_Idx)
        {
            city_count++;
        }
    }
    resign = ST_FALSE;
    if(city_count > 1)  /* not about to lose very last city */
    {
        resign = Banish_Wizard(loser_Idx, winner_idx);
    }
    if(
        (city_count >= 2)
        &&
        (resign < ST_TRUE)
    )
    {
        return;
    }
    if(city_idx != ST_UNDEFINED)
    {
        _CITIES[city_idx].owner_idx = (int8_t)winner_idx;
    }
    _FORTRESSES[loser_Idx].active = ST_FALSE;
    _players[winner_idx].fame += 5;
    // ; remove all city enchantments and garrisoned heroes or
    // ; fantastic units owned by the target, turn all of
    // ; their garrisoned normal units neutral, and if they
    // ; are not the human player, turn their actual cities
    // ; neutral too
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {
        city_enchantments = &_CITIES[itr_cities].enchantments[0];
        // ; remove all city enchantments owned by the defeated player
        for(itr_cench = 0; itr_cench < NUM_CITY_ENCHANTMENTS; itr_cench++)
        {
            if(city_enchantments[itr_cench] > 0)
            {
                if((city_enchantments[itr_cench] - 1) == loser_Idx)
                {
                    city_enchantments[itr_cench] = 0;
                }
            }
        }
        if(_CITIES[itr_cities].owner_idx == loser_Idx)
        {
            Army_At_City(itr_cities, &troop_count, &troops[0]);
            // ; dismiss all fantastic units or heroes from the city, but transfer normal units to the neutral player
            for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
            {
                // ; conflicting condition: the called function can only return these, nothing else - will always jump
                if(_UNITS[troops[itr_troops]].owner_idx == loser_Idx)
                {
                    if((_unit_type_table[_UNITS[troops[itr_troops]].type].Abilities & UA_FANTASTIC) != 0)
                    {
                        Kill_Unit(troops[itr_troops], kt_Dismissed);
                    }
                    else if(_UNITS[troops[itr_troops]].Hero_Slot != -1)
                    {
                        Kill_Unit(troops[itr_troops], kt_Dismissed);
                    }
                    else
                    {
                        _UNITS[troops[itr_troops]].owner_idx = NEUTRAL_PLAYER_IDX;
                    }
                }
            }
            if(_CITIES[itr_cities].owner_idx != _human_player_idx)  /* OGBUG  DEDU  Why just human player? */
            {
                _CITIES[itr_cities].owner_idx = NEUTRAL_PLAYER_IDX;
            }
        }
    }
    // ; reset the meld flags and the owners of all nodes controlled by the target
    for(itr_nodes = 0; itr_nodes < NUM_NODES; itr_nodes++)
    {
        if(_NODES[itr_nodes].owner_idx == loser_Idx)
        {
            _NODES[itr_nodes].owner_idx = ST_UNDEFINED;
            _NODES[itr_nodes].flags = NF_NONE;
        }
    }
    // Finally, when enemy cities are conquered, the conquering wizard gets a portion of the previous owner's gold reserve as loot.
    Player_Add_Gold(winner_idx, (_players[loser_Idx].gold_reserve / 2));
    // When a wizard's enchanted fortress is captured by another wizard, the conqueror steals 50% of his opponent’s mana reserves, ...
    Player_Add_Mana(winner_idx, (_players[loser_Idx].mana_reserve / 2));
    // ; dismiss the target's remaining units (i.e. those that were not in cities)
    for(itr_units = 0; itr_units < _units; itr_units++)
    {
        if(_UNITS[itr_units].owner_idx == loser_Idx)
        {
            Kill_Unit(itr_units, kt_Dismissed);
        }
    }
    // ; remove all of the target's overland enchantments
    overland_enchantments = &_players[loser_Idx].Globals[0];
    for(itr_oench = 0; itr_oench < NUM_OVERLAND_ENCHANTMENTS; itr_oench++)
    {
        overland_enchantments[itr_oench] = 0;
    }
    if(resign != ST_TRUE)
    {
        Conquest_Animation(loser_Idx, winner_idx);
    }
    // ; zero the diplomatic status of the target with all remaining wizards, and generate a type 7 (-40) diplomatic action towards the conqueror for them
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        _players[itr_players].Dipl.Dipl_Status[loser_Idx] = 0;
        _players[loser_Idx].Dipl.Dipl_Status[itr_players] = 0;
        _players[itr_players].Dipl.Contacted[loser_Idx] = 0;
        _players[loser_Idx].Dipl.Contacted[itr_players] = 0;
        if(
            (itr_players != loser_Idx)
            &&
            (itr_players != winner_idx)
        )
        {
            // Note, however, that the conquering wizard suffers permanent diplomatic penalties to his or her interactions with other wizards after such an event.
            Change_Relations(-40, winner_idx, itr_players, 7, 0, 0);
        }
    }
    Conquest_Spells(winner_idx, loser_Idx);
    if(loser_Idx == _human_player_idx)
    {
        Lose_Animation(loser_Idx);
        End_Of_Game_Score();
        s01p16_empty_function();
        /* EOG_HACK */  // DONT  Respawn(magic_exe_file__ovr093, str_JENNY__ovr093, str_empty_string__ovr093, str_empty_string__ovr093);
    }
    else
    {
        if(CP_Is_Dead() == ST_TRUE)
        {
            Win_Animation(_human_player_idx);
            End_Of_Game_Score();
            s01p16_empty_function();
            /* EOG_HACK */  // DONT  Respawn(magic_exe_file__ovr093, str_JENNY__ovr093, str_empty_string__ovr093, str_empty_string__ovr093);
        }
    }
}


// WZD 093p02
/*
"banishes", "defeats", "destroy", "fortress"
WIZ_Conquer()
    Conquest_Animation(loser_Idx, winner_idx);
Banish_Wizard()
    Conquest_Animation(loser_idx, winner_idx);
*/
void Conquest_Animation(int16_t loser_Idx, int16_t winner_idx)
{
    char Space_String[4] = { 0, 0, 0, 0 };
    int16_t city_count = 0;
    SAMB_ptr Music_Data_Seg = 0;
    uint32_t Music_Data_Seg_size = 0;  // HACK  DNE in Dasm
    int16_t full_screen_esc = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t itr_cities = 0;
    SAMB_ptr wizlab_seg = 0;
    stu_strcpy(Space_String, str_SPACE__ovr093);  // char str_SPACE__ovr093[] = " ";
    if(
        (winner_idx != NEUTRAL_PLAYER_IDX)
        &&
        (loser_Idx != NEUTRAL_PLAYER_IDX)
    )
    {
        return;
    }
    if(winner_idx == _human_player_idx)
    {
        Set_Bit_Field_Near(loser_Idx, (char *)&_players[_human_player_idx].Defeated_Wizards);
    }
    GAME_Conquered_Wiz = loser_Idx;
    GAME_Conquering_Wiz = winner_idx;
    Stop_All_Sounds__STUB();
    Set_Mouse_List(1, mouse_list_none);
    Clear_Fields();
    Fade_Out();
    Set_Page_Off();
    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);
    Toggle_Pages();
    // ; count the cities of the conquered wizard
    for(itr_cities = 0, city_count = 0; ((itr_cities < _cities) && (city_count < 2)); itr_cities++)
    {
        if(_CITIES[itr_cities].owner_idx == GAME_Conquered_Wiz)
        {
            city_count++;
        }
    }
    Load_Palette(3, ST_UNDEFINED, ST_NULL);  // ; WILZAB - conquest palette
    Apply_Palette();
    Set_Page_Off();
    // WIZLAB.LBX, 019    "WIZLAB"      ""
    wizlab_seg = LBX_Reload(wizlab_lbx_file__ovr093, 19, _screen_seg);
    FLIC_Draw(0, 0, wizlab_seg);
    Copy_Off_To_Back();
    Copy_Off_To_Page4();
    GUI_String_1 = (char *)Near_Allocate_First(100);
    GUI_String_2 = (char *)Near_Allocate_Next(100);
    if(GAME_Conquering_Wiz != NEUTRAL_PLAYER_IDX)
    {
        stu_strcpy(GUI_String_1, _players[winner_idx].name);
        stu_strcat(GUI_String_1, &Space_String[0]);
        if(city_count > 1)
        {
            stu_strcat(GUI_String_1, cnst_Conquest_Msg3);  // "banishes"
        }
        else
        {
            stu_strcat(GUI_String_1, cnst_Conquest_Msg4);  // "defeats"
        }
        stu_strcat(GUI_String_1, Space_String);
        stu_strcat(GUI_String_1, _players[loser_Idx].name);
    }
    else
    {
        stu_strcpy(GUI_String_1, cnst_Raiders_2);
        stu_strcat(GUI_String_1, Space_String);
        stu_strcat(GUI_String_1, cnst_Conquest_Msg1);  // "destroy"
        stu_strcat(GUI_String_1, Space_String);
        stu_strcpy(GUI_String_2, _players[loser_Idx].name);
        if(city_count > 1)
        {
            Possessive(GUI_String_2);
            stu_strcat(GUI_String_1, GUI_String_2);
            stu_strcat(GUI_String_1, Space_String);
            stu_strcat(GUI_String_1, cnst_Conquest_Msg2);  // "fortress"
        }
        else
        {
            stu_strcat(GUI_String_1, GUI_String_2);
        }
        stu_strcat(GUI_String_1, cnst_Dot7);
    }
    // WIZLAB.LBX, 000    "MERLIN"      ""
    // WIZLAB.LBX, 001    "RAVEN"       ""
    // WIZLAB.LBX, 002    "SHAREE"      ""
    // WIZLAB.LBX, 003    "LOPAN"       ""
    // WIZLAB.LBX, 004    "JAFAR"       ""
    // WIZLAB.LBX, 005    "OBERIC"      ""
    // WIZLAB.LBX, 006    "RJAK"        ""
    // WIZLAB.LBX, 007    "SSSRA"       ""
    // WIZLAB.LBX, 008    "TAURON"      ""
    // WIZLAB.LBX, 009    "FREYA"       ""
    // WIZLAB.LBX, 010    "HORUS"       ""
    // WIZLAB.LBX, 011    "ARIEL"       ""
    // WIZLAB.LBX, 012    "TLALOC"      ""
    // WIZLAB.LBX, 013    "KALI"        ""
    wizlab_wizard_seg__ovr093 = LBX_Reload(wizlab_lbx_file__ovr093, _players[loser_Idx].wizard_id, _screen_seg);
    // CONQUEST.LBX, 000    "MERLIN"    ""
    // CONQUEST.LBX, 001    "SHAMAN"    ""
    // CONQUEST.LBX, 002    "SHAREE"    ""
    // CONQUEST.LBX, 003    "LOPAN"     ""
    // CONQUEST.LBX, 004    "ARAB"      ""
    // CONQUEST.LBX, 005    "ALERIC"    ""
    // CONQUEST.LBX, 006    "WRAITH"    ""
    // CONQUEST.LBX, 007    "DRACON"    ""
    // CONQUEST.LBX, 008    "HORUS"     ""
    // CONQUEST.LBX, 009    "FREYA"     ""
    // CONQUEST.LBX, 010    "GALEN"     ""
    // CONQUEST.LBX, 011    "ARIEL"     ""
    // CONQUEST.LBX, 012    "AZTEC"     ""
    // CONQUEST.LBX, 013    "KALI"      ""
    conquest_wizard_seg = LBX_Reload_Next(conquest_lbx_file__ovr093, _players[winner_idx].wizard_id, _screen_seg);
    // CONQUEST.LBX, 014    "CONEHEAD"  ""
    conquest_conehead_seg = LBX_Reload_Next(conquest_lbx_file__ovr093, 14, _screen_seg);
    // CONQUEST.LBX, 015    "AXEBOY"    ""
    conquest_axeboy_seg = LBX_Reload_Next(conquest_lbx_file__ovr093, 15, _screen_seg);
    // CONQUEST.LBX, 017    "ZAP"       ""
    Open_File_Animation__HACK(conquest_lbx_file__ovr093, 17);
    if(magic_set.background_music == ST_TRUE)
    {
        itr_cities = 109;  /* MUSIC_WIN_Military */
        if(loser_Idx == _human_player_idx) 
        {
            itr_cities = MUSIC_LOSE_Military;
        }
        Music_Data_Seg = LBX_Reload(music_lbx_file__ovr093, itr_cities, SND_Music_Segment);
        Music_Data_Seg_size = lbxload_entry_length;
        Play_Sound(Music_Data_Seg, Music_Data_Seg_size);
    }
    Assign_Auto_Function(Conquest_Animation_Draw, 6);
    m_conquest_anim_stage = 0;
    leave_screen = ST_FALSE;
    for(itr_cities = 0; itr_cities < 3; itr_cities++)
    {
        Mark_Time();
        Conquest_Animation_Draw();
        m_conquest_anim_stage = 0;
        PageFlip_FX();
        Release_Time(5);
    }
    m_conquest_anim_stage = 0;
    Set_Input_Delay(3);
    while(leave_screen == ST_FALSE)
    {
        Mark_Time();
        Clear_Fields();
        full_screen_esc = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, (int16_t)str_hotkey_ESC__ovr093[0], ST_UNDEFINED);
        input_field_idx = Get_Input();
        if(input_field_idx == full_screen_esc)
        {
            leave_screen = ST_UNDEFINED;
        }
        if(m_conquest_anim_stage > 30)
        {
            leave_screen = ST_UNDEFINED;
        }
        if(leave_screen == ST_FALSE)
        {
            Conquest_Animation_Draw();
            PageFlip_FX();
            Release_Time(5);
        }
    }
    Stop_All_Sounds__STUB();
    Clear_Fields();
    Reset_Window();
    Clear_Fields();
    Deactivate_Auto_Function();
    Fade_Out();
    Load_Palette(0, ST_UNDEFINED, ST_NULL);  // ; EMPERATO - main game palette
    Reset_Cycle_Palette_Color();
    Clear_Palette_Changes(0, 255);
    Set_Palette_Changes(0, 223);
    Calculate_Remap_Colors();
    Set_Page_Off();
    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);
    Toggle_Pages();
    Set_Mouse_List(1, mouse_list_default);
    Play_Background_Music();
}


// WZD 093p03
void Conquest_Animation_Draw(void)
{
    Set_Page_Off();
    Reset_Window();
    Copy_Back_To_Off();
    if(m_conquest_anim_stage == 0)
    {
        FLIC_Draw(69, 75, wizlab_wizard_seg__ovr093);
    }
    if(m_conquest_anim_stage > 0)
    {
        if(m_conquest_anim_stage < 7)
        {
            Conquest_Animation_Draw_Conquerors_Approach();
        }
        else
        {
            if(m_conquest_anim_stage == 7)
            {
                Conquest_Animation_Draw_Setup_Zap_Scene();
            }
            if(m_conquest_anim_stage < 11)
            {
                Conquest_Animation_Draw_Zapped_Wizard(1);
            }
            else if(m_conquest_anim_stage < 19)
            {
                if(
                    (GAME_Conquering_Wiz != NEUTRAL_PLAYER_IDX)
                    &&
                    (
                        (m_conquest_anim_stage == 11)
                        ||
                        (magic_set.sound_effects == ST_TRUE)
                    )
                )
                {
                    Play_Sound(SND_GAME_Scream, SND_GAME_Scream_size);
                }
                Conquest_Animation_Draw_Zap_Strike();
            }
            else if(m_conquest_anim_stage < 23)
            {
                Conquest_Animation_Draw_Zapped_Wizard(0);
            }
        }
    }
    if(
        (m_conquest_anim_stage > 6)
        &&
        (GAME_Conquering_Wiz == NEUTRAL_PLAYER_IDX)
    )
    {
        FLIC_Draw(69, 75, wizlab_wizard_seg__ovr093);
        Conquest_Animation_Draw_End_Pose();
    }
    Set_Font_Style_Outline_Heavy(5, 5, 0, 0);
    Set_Outline_Color(1);
    Print_Centered(160, 6, GUI_String_1);
    m_conquest_anim_stage++;
}


// WZD 093p04
void Conquest_Animation_Draw_Conquerors_Approach(void)
{
    int si;
    int di;
    FLIC_Draw(0x45, 0x4B, wizlab_wizard_seg__ovr093);
    Set_Window(0, 0, 0x13F, 0xC7);
    si = 0x42 - ((m_conquest_anim_stage - 1) * 0x0B);
    di = -(m_conquest_anim_stage + 5);
    Clipped_Draw(si + 0xFC, di + 0x58, conquest_axeboy_seg);
    if(GAME_Conquering_Wiz != NEUTRAL_PLAYER_IDX)
    {
        si = 0x5A - ((m_conquest_anim_stage - 1) * 0x0F);
        di = 0x4E - ((m_conquest_anim_stage - 1) * 0x0D);
        Clipped_Draw(si + 0xD0, di + 0x3C, conquest_wizard_seg);
    }
    si = 0x1E - ((m_conquest_anim_stage - 1) * 5);
    di = 0x3C - ((m_conquest_anim_stage - 1) * 10);
    Clipped_Draw(si + 0x64, di + 0x78, conquest_conehead_seg);
}


// WZD 093p05
void Conquest_Animation_Draw_Setup_Zap_Scene(void)
{
    int16_t Entry_Index = 0;
    int16_t si = 0;
    int16_t di = 0;
    int16_t wizard_id = 0;
    unsigned int bx = 0;
    if(GAME_Conquering_Wiz == NEUTRAL_PLAYER_IDX)
    {
        return;
    }
    Set_Window(0, 0, 319, 199);
    si = 66 - (m_conquest_anim_stage - 1) * 11;
    di = -(m_conquest_anim_stage + 5);
    Clipped_Draw(si + 252, di + 88, conquest_axeboy_seg);
    si = 90 - (m_conquest_anim_stage - 1) * 15;
    di = 78 - (m_conquest_anim_stage - 1) * 13;
    Clipped_Draw(si + 208, di + 60, conquest_wizard_seg);
    si = 30 - (m_conquest_anim_stage - 1) * 5;
    di = 60 - (m_conquest_anim_stage - 1) * 10;
    Clipped_Draw(si + 100, di + 120, conquest_conehead_seg);
    Copy_Off_To_Back();
    wizlab_wizard_seg__ovr093 = LBX_Reload(wizlab_lbx_file__ovr093, _players[GAME_Conquered_Wiz].wizard_id, _screen_seg);
    conquest_wizard_seg = LBX_Reload_Next(conquest_lbx_file__ovr093, _players[GAME_Conquering_Wiz].wizard_id + 33, _screen_seg);
    conquest_effect_seg = LBX_Reload_Next(conquest_lbx_file__ovr093, _players[GAME_Conquered_Wiz].wizard_id + 18, _screen_seg);
    IMG_GAME_GrowZap = LBX_Reload_Next(conquest_lbx_file__ovr093, 16, _screen_seg);
    conquest_conehead_seg = LBX_Reload_Next(conquest_lbx_file__ovr093, 32, _screen_seg);
    if(magic_set.sound_effects == 1)
    {
        wizard_id = _players[GAME_Conquered_Wiz].wizard_id;
        bx = wizard_id - 2;
        if(bx > 11)
        {
            Entry_Index = SFX_MaleScream;
        }
        else
        {
            switch(bx)
            {
                case 0:  /* Sharee */
                case 6:  /* Kali */
                case 9:  /* Ariel */
                case 10: /* Freya */
                    Entry_Index = SFX_FemaleScream;
                    break;
                default:
                    Entry_Index = SFX_MaleScream;
                    break;
            }
        }
        SND_GAME_Scream = LBX_Reload_Next(soundfx_lbx_file__ovr093, Entry_Index, _screen_seg);
    }
    return;
}


// WZD 093p06
void Conquest_Animation_Draw_Zapped_Wizard(int16_t flag)
{
    int16_t Zap_Frame_Count = 0;
    int16_t frame = 0;
    if(GAME_Conquering_Wiz == NEUTRAL_PLAYER_IDX)
    {
        return;
    }
    Zap_Frame_Count = FLIC_Get_FrameCount(IMG_GAME_GrowZap);
    frame = FLIC_Get_CurrentFrame(IMG_GAME_GrowZap);
    if(flag == ST_TRUE)
    {
        FLIC_Draw(0x45, 0x4B, wizlab_wizard_seg__ovr093);
        FLIC_Draw(0xB1, 0x3E, IMG_GAME_GrowZap);
    }
    else
    {
        FLIC_Draw(0xB1, 0x3E, IMG_GAME_GrowZap);
        Set_Animation_Frame(IMG_GAME_GrowZap, frame - 1);
        frame = FLIC_Get_CurrentFrame(conquest_effect_seg);
        if(frame <= 3)
        {
            switch(frame)
            {
                case 3:
                    Set_Animation_Frame(conquest_effect_seg, 0);
                    FLIC_Draw(0x42, 0x10, conquest_effect_seg);
                    FLIC_Draw(0x42, 0x10, conquest_effect_seg);
                    FLIC_Draw(0x42, 0x10, conquest_effect_seg);
                    FLIC_Draw(0x42, 0x10, conquest_effect_seg);
                    break;
                case 2:
                    Set_Animation_Frame(conquest_effect_seg, 0);
                    FLIC_Draw(0x42, 0x10, conquest_effect_seg);
                    FLIC_Draw(0x42, 0x10, conquest_effect_seg);
                    FLIC_Draw(0x42, 0x10, conquest_effect_seg);
                    break;
                case 1:
                    Set_Animation_Frame(conquest_effect_seg, 0);
                    FLIC_Draw(0x42, 0x10, conquest_effect_seg);
                    FLIC_Draw(0x42, 0x10, conquest_effect_seg);
                    break;
                case 0:
                    FLIC_Draw(0x42, 0x10, conquest_effect_seg);
                    break;
            }
        }
    }
    Clipped_Draw(0xD0, 0x3C, conquest_wizard_seg);
    Clipped_Draw(0x64, 0x78, conquest_conehead_seg);
}


// WZD 093p07
void Conquest_Animation_Draw_Zap_Strike(void)
{
    if(GAME_Conquering_Wiz == NEUTRAL_PLAYER_IDX)
    {
        return;
    }
    FLIC_Draw(0x45, 0x4B, wizlab_wizard_seg__ovr093);
    Draw_File_Animation__HACK();
    Clipped_Draw(0xD0, 0x3C, conquest_wizard_seg);
    Clipped_Draw(0x64, 0x78, conquest_conehead_seg);
}


// WZD 093p08
void Conquest_Animation_Draw_End_Pose(void)
{
    Clipped_Draw(252, 88, conquest_axeboy_seg);
    if(GAME_Conquering_Wiz != NEUTRAL_PLAYER_IDX)
    {
        Clipped_Draw(208, 60, conquest_wizard_seg);
    }
    Clipped_Draw(100, 120, conquest_conehead_seg);
}


// WZD 093p09
/* 
OON XREF:  WIZ_Conquer()
*/
/**
 * @brief Determines whether all non-human opponents have been eliminated.
 *
 * Iterates over player slots 1 through (_num_players - 1), skipping the
 * presumed human slot at index 0. A player is considered still active if
 * they have an active fortress, or if they own at least one city.
 *
 * @return int16_t ST_TRUE if no active non-human opponents remain;
 *         otherwise ST_FALSE.
 */
int16_t CP_Is_Dead(void)
{
    int16_t city_count = 0;
    int16_t itr_cities = 0;
    int16_t itr_players = 0;
    int16_t active_computer_players = 0;
    active_computer_players = 0;
    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {
        if(_FORTRESSES[itr_players].active == ST_TRUE)
        {
            active_computer_players++;
        }
        else
        {
            city_count = 0;
            for(itr_cities = 0; ((itr_cities < _cities) && (city_count < 2)); itr_cities++)
            {
                if(_CITIES[itr_cities].owner_idx == itr_players)
                {
                    city_count++;
                }
            }
            if(city_count > 0)
            {
                active_computer_players++;
            }
        }
    }
    if(active_computer_players == 0)
    {
        return ST_TRUE;
    }
    return ST_FALSE;
}


// WZD 093p10
void Win_Animation(int16_t player_idx)
{
    int16_t leave_screen = 0;
    int16_t input_field_idx = 0;
    int16_t full_screen_esc = 0;
    SAMB_ptr sound_data_seg = ST_NULL;
    uint32_t sound_data_seg_size = 0;  // HACK  DNE in Dasm
    int16_t hands_type = 0;
    SAMB_ptr temp_seg = ST_NULL;
    Combat_Cache_Write();
    // ; OGBUG: overwrites the city data pointer, only to be overwritten itself by an LBXE_LoadReplace...
    _CITIES = (struct s_CITY *)Allocate_First_Block(World_Data, 714);
    Stop_All_Sounds__STUB();
    Set_Mouse_List(1, mouse_list_none);
    Clear_Fields();
    Fade_Out();
    Set_Page_Off();
    Fill(0, 0, 319, 199, 0);
    Toggle_Pages();
    Load_Palette(5, ST_UNDEFINED, ST_NULL);
    Apply_Palette();
    Set_Page_Off();
    // WIN.LBX, 000  "BACKGRND" "'
    temp_seg = LBX_Reload(win_lbx_file__ovr093, 0, _screen_seg);
    FLIC_Draw(0, 0, temp_seg);
    switch(_players[player_idx].wizard_id)
    {
        case 1:  { hands_type = 20; } break;
        case 2:  { hands_type = 19; } break;
        case 3:  { hands_type = 21; } break;
        case 4:  { hands_type = 22; } break;
        case 5:  { hands_type = 18; } break;
        default: { hands_type = 17; } break;
    }
    // WIN.LBX, 017  "MHANDS"   "male hands"
    // WIN.LBX, 018  "FHANDS"   "female hands"
    // WIN.LBX, 019  "PHANDS"   "priestess hands"
    // WIN.LBX, 020  "SHANDS"   "shaman hands"
    // WIN.LBX, 021  "OHANDS"   "oberic hands"
    // WIN.LBX, 022  "DHANDS"   "draconian hands"
    temp_seg = LBX_Reload(win_lbx_file__ovr093, hands_type, _screen_seg);
    FLIC_Draw(22, 143, temp_seg);
    Copy_Off_To_Back();
    Copy_Off_To_Page4();  /* OGBUG  ; this image is never loaded back from here */
    // WIN.LBX, 003  "MERLBLUE" ""
    // WIN.LBX, 004  "SHAMBLUE" ""
    // WIN.LBX, 005  "PRIEBLUE" ""
    // WIN.LBX, 006  "WUBLUE"   ""
    // WIN.LBX, 007  "ARABBLUE" ""
    // WIN.LBX, 008  "OBERBLUE" ""
    // WIN.LBX, 009  "WRAIBLUE" ""
    // WIN.LBX, 010  "DRACBLUE" ""
    // WIN.LBX, 011  "NMOIBLUE" ""
    // WIN.LBX, 012  "FREYBLUE" ""
    // WIN.LBX, 013  "GALEBLUE" ""
    // WIN.LBX, 014  "ARIEBLUE" ""
    // WIN.LBX, 015  "AZTEBLUE" ""
    // WIN.LBX, 016  "KARLBLUE" ""
    IMG_GAME_WizHandsUp = LBX_Reload(win_lbx_file__ovr093, (3 + _players[player_idx].wizard_id), _screen_seg);
    // WIN.LBX, 001  "REDPOINT" "'
    IMG_GAME_RedSparkle = LBX_Reload(win_lbx_file__ovr093, 1, World_Data);
    // WIN.LBX, 002  "PLANETS"  ""
    Open_File_Animation__HACK(win_lbx_file__ovr093, 2);
    if(magic_set.background_music == ST_TRUE)
    {
        sound_data_seg = LBX_Reload(music_lbx_file__ovr093, MUSIC_WIN_Military, SND_Music_Segment);
        sound_data_seg_size = lbxload_entry_length;
        Play_Sound(sound_data_seg, sound_data_seg_size);
    }
    /* OGBUG  Win_Animation_Draw() uses GUI_String_1, but Win_Animation() doesn't allocate it. */
    GUI_String_1 = (char *)Near_Allocate_First(100);   /* <-- here */
    /* Note: Segment stub093 used as base for function pointer */
    Assign_Auto_Function(Win_Animation_Draw, 4);
    leave_screen = 0;
    m_conquest_anim_stage = 0;
    Set_Input_Delay(3);
    while(leave_screen == 0)
    {
        Mark_Time();
        Clear_Fields();
        full_screen_esc = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, (int16_t)str_hotkey_ESC__ovr093[0], ST_UNDEFINED);
        input_field_idx = Get_Input();
        if(input_field_idx == full_screen_esc)
        {
            leave_screen = ST_UNDEFINED;
        }
        if(m_conquest_anim_stage > 90)
        {
            leave_screen = ST_UNDEFINED;
        }
        if(leave_screen == ST_FALSE)
        {
            Win_Animation_Draw();
            PageFlip_FX();
            Release_Time(4);
        }
    }
    Stop_All_Sounds__STUB();
    Clear_Fields();
    Reset_Window();
    Clear_Fields();
    Deactivate_Auto_Function();
    Fade_Out();
    Load_Palette(0, ST_UNDEFINED, ST_NULL);
    Reset_Cycle_Palette_Color();
    Clear_Palette_Changes(0, 255);
    Set_Palette_Changes(0, 223);
    Calculate_Remap_Colors();
    Set_Page_Off();
    Fill(0, 0, 319, 199, 0);
    Toggle_Pages();
    Set_Mouse_List(1, mouse_list_default);
    Play_Background_Music();
    Combat_Cache_Read();
}


// WZD 093p11
void Win_Animation_Draw(void)
{
    char niu_space[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t frame = 0;
    int16_t itr = 0;
    stu_strcpy(niu_space, str_SPACE__ovr093);
    Set_Page_Off();
    Reset_Window();
    Copy_Back_To_Off();
    frame = FLIC_Get_CurrentFrame(IMG_GAME_WizHandsUp);
    Set_Animation_Frame(IMG_GAME_WizHandsUp, 0);
    for(itr = 0; itr <= frame; itr++)
    {
        FLIC_Draw(93, 2, IMG_GAME_WizHandsUp);
    }
    FLIC_Draw(0, 0, IMG_GAME_RedSparkle);
    Draw_File_Animation__HACK();
    Set_Font_Style_Outline_Heavy(5, 5, 0, 0);
    Set_Outline_Color(0);
    if(m_conquest_anim_stage < 15)
    {
        stu_strcpy(GUI_String_1, cnst_WIN_Msg_1);  // "Having conquered both the"
    }
    else if(m_conquest_anim_stage < 30)
    {
        stu_strcpy(GUI_String_1, cnst_WIN_Msg_2);  // "world of Arcanus and Myrror,"
    }
    else if(m_conquest_anim_stage < 45)
    {
        stu_strcpy(GUI_String_1, cnst_WIN_Msg_3);  // "I and only I, remain the one"
    }
    else if(m_conquest_anim_stage < 60)
    {
        stu_strcpy(GUI_String_1, cnst_WIN_Msg_4);  // "and true Master of Magic."
    }
    Print_Centered(160, 180, GUI_String_1);
    m_conquest_anim_stage++;
}


// WZD 093p12
void Lose_Animation(int16_t player_idx)
{
    SAMB_ptr music_data_seg = ST_NULL;
    uint32_t music_data_seg_size = 0;  // HACK  DNE in Dasm
    int16_t full_screen_esc = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    Stop_All_Sounds__STUB();
    Set_Mouse_List(1, mouse_list_none);
    Clear_Fields();
    Fade_Out();
    Set_Page_Off();
    Fill(0, 0, 319, 199, 0);
    Toggle_Pages();
    Load_Palette(6, ST_UNDEFINED, ST_NULL);
    Apply_Palette();
    Set_Page_Off();
    GAME_LimboFall_Stage = 0;
    // LOSE.LBX, 001    "MERLIWIN"  ""
    // LOSE.LBX, 002    "SHAMMWIN"  ""
    // LOSE.LBX, 003    "SHAREWIN"  ""
    // LOSE.LBX, 004    "LOPANWIN"  ""
    // LOSE.LBX, 005    "ARABWIN"   ""
    // LOSE.LBX, 006    "OBERWIN"   ""
    // LOSE.LBX, 007    "RJAKWIN"   ""
    // LOSE.LBX, 008    "DRACOWIN"  ""
    // LOSE.LBX, 009    "TAURNWIN"  ""
    // LOSE.LBX, 010    "FREYAWIN"  ""
    // LOSE.LBX, 011    "HORUSWIN"  ""
    // LOSE.LBX, 012    "ARIELWIN"  ""
    // LOSE.LBX, 013    "AZTECWIN"  ""
    // LOSE.LBX, 014    "KALIWIN"   ""
    IMG_GAME_WizHandsUp = LBX_Reload(lose_lbx_file__ovr093, (1 + _players[player_idx].wizard_id), _screen_seg);
    IMG_GAME_LimboFall = Allocate_Next_Block(_screen_seg, 0x190);
    // LOSE.LBX, 000    "VORTEX"    ""
    Open_File_Animation__HACK(lose_lbx_file__ovr093, 0);
    if(magic_set.background_music == 1)
    {
        music_data_seg = LBX_Reload(music_lbx_file__ovr093, MUSIC_LOSE_Military, SND_Music_Segment);
        music_data_seg_size = lbxload_entry_length;
        Play_Sound(music_data_seg, music_data_seg_size);
    }
    Assign_Auto_Function(Lose_Animation_Draw, 4);
    leave_screen = ST_FALSE;
    m_conquest_anim_stage = 0;
    Set_Input_Delay(3);
    while(leave_screen == 0)
    {
        Mark_Time();
        Clear_Fields();
        full_screen_esc = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, (int16_t)str_empty_string__ovr093[0], ST_UNDEFINED);
        input_field_idx = Get_Input();
        if(input_field_idx == full_screen_esc)
        {
            leave_screen = ST_UNDEFINED;
        }
        if(m_conquest_anim_stage > 45)
        {
            leave_screen = ST_UNDEFINED;
        }
        if(leave_screen == ST_FALSE)
        {
            Lose_Animation_Draw();
            PageFlip_FX();
            Release_Time(4);
        }
    }
    Stop_All_Sounds__STUB();
    Clear_Fields();
    Reset_Window();
    Clear_Fields();
    Deactivate_Auto_Function();
    Fade_Out();
    Load_Palette(0, ST_UNDEFINED, ST_NULL);
    Reset_Cycle_Palette_Color();
    Clear_Palette_Changes(0, 255);
    Set_Palette_Changes(0, 223);
    Calculate_Remap_Colors();
    Set_Page_Off();
    Fill(0, 0, 319, 199, 0);
    Toggle_Pages();
    Set_Mouse_List(1, mouse_list_default);
    Play_Background_Music();
}


// WZD 093p13
void Lose_Animation_Draw(void)
{
    int16_t y = 0;
    int16_t x = 0;
    Set_Page_Off();
    Draw_File_Animation__HACK();
    if(GAME_LimboFall_Stage < 27)
    {
        x = (int16_t)TBL_DefeatAnimSteps[GAME_LimboFall_Stage].x;
        y = (int16_t)TBL_DefeatAnimSteps[GAME_LimboFall_Stage].y;
        Draw_Picture_To_Bitmap(IMG_GAME_WizHandsUp, IMG_GAME_LimboFall);
        Scale_Bitmap(IMG_GAME_LimboFall, (int16_t)TBL_DefeatAnimSteps[GAME_LimboFall_Stage].s, (int16_t)TBL_DefeatAnimSteps[GAME_LimboFall_Stage].s);
        Draw_Bitmap_Rotated(x, y, (int16_t)TBL_DefeatAnimSteps[GAME_LimboFall_Stage].a, IMG_GAME_LimboFall);
    }
    m_conquest_anim_stage++;
    GAME_LimboFall_Stage++;
}


// WZD 093p14
/**
 * @brief Handles wizard banishment, return spell setup, and post-defeat resolution.
 *
 * For non-human losers, checks whether the defeated wizard has enough casting
 * strength and mana to continue; if not, returns 99 immediately. Otherwise,
 * disables the loser’s fortress, grants the winner fame and mana, plays the
 * conquest animation, applies diplomatic penalties, and then resolves the
 * return dialog or return animation depending on whether the loser was human.
 *
 * @param loser_idx Index of the wizard being banished.
 * @param winner_idx Index of the wizard who won the conquest.
 * @return int16_t ST_FALSE when the human player is prompted and declines is
 *         not yet resolved, 99 when an AI wizard cannot continue, or the
 *         resignation result from the human confirmation dialog.
 */
int16_t Banish_Wizard(int16_t loser_idx, int16_t winner_idx)
{
    char buffer[50] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char niu_space[4] = { 0, 0, 0, 0 };
    int16_t turns_to_return = 0;
    int16_t resign = 0;
    int16_t itr_players = 0;
    stu_strcpy(niu_space, str_SPACE__ovr093);
    _players[loser_idx].casting_spell_idx = spl_Spell_Of_Return;
    _players[loser_idx].casting_cost_remaining = Casting_Cost(loser_idx, spl_Spell_Of_Return, 0);
    _players[loser_idx].casting_cost_original = Casting_Cost(loser_idx, spl_Spell_Of_Return, 0);
    turns_to_return = _players[loser_idx].casting_cost_remaining / _players[loser_idx].Nominal_Skill;
    if(loser_idx != _human_player_idx)
    {
        if(
            (Player_Base_Casting_Skill(loser_idx) + Player_Hero_Casting_Skill(loser_idx) < 40)
            ||
            (_players[loser_idx].mana_reserve < 1)
        )
        {
            return 99;
        }
    }
    _FORTRESSES[loser_idx].active = ST_FALSE;
    _players[winner_idx].fame += 5;
    _players[loser_idx].mana_reserve /= 2;
    Player_Add_Mana(winner_idx, _players[loser_idx].mana_reserve);
    Conquest_Animation(loser_idx, winner_idx);
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        if(itr_players != loser_idx && itr_players != winner_idx)
        {
            Change_Relations(-20, winner_idx, itr_players, 7, 0, 0);
        }
    }
    resign = ST_FALSE;
    Allocate_Reduced_Map();
    Full_Draw_Main_Screen();
    Copy_On_To_Off_Page();
    if(loser_idx == _human_player_idx)
    {
        stu_itoa(turns_to_return, buffer, 10);
        stu_strcpy(GUI_NearMsgString, cnst_SoReturn_Msg1);  // "There are at least "
        stu_strcat(GUI_NearMsgString, buffer);
        stu_strcat(GUI_NearMsgString, cnst_SoReturn_Msg2);  // " turns remaining until you may return. Do you wish to resign?"
        resign = Confirmation_Box(GUI_NearMsgString);
    }
    else
    {
        if(loser_idx != NEUTRAL_PLAYER_IDX)
        {
            Copy_Off_To_Back();
            Return_Animation(loser_idx);
            Conquest_Spells(winner_idx, loser_idx);
        }
    }
    return resign;
}


// WZD 093p15
void Return_Animation(int16_t player_idx)
{
    char niu_space[4] = { 0, 0, 0, 0 };
    int16_t full_screen_esc = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    stu_strcpy(niu_space, str_SPACE__ovr093);
    GUI_String_1 = (char *)Near_Allocate_First(100);
    stu_strcpy(GUI_String_1, _players[player_idx].name);
    stu_strcat(GUI_String_1, cnst_SoReturn_Msg3);  // " begins casting the Spell Of Return."
    // VORTEX.LBX, 000   "VORTEX2"  ""
    conquest_effect_seg = LBX_Reload(vortex_lbx_file__ovr093, 0, _screen_seg);
    // CMBTFX.LBX, 027  "UNDEADBK"  ""
    conquest_conehead_seg = LBX_Reload_Next(cmbtfx_lbx_file__ovr093, 27, _screen_seg);
    Assign_Auto_Function(Return_Animation_Draw, 6);
    m_conquest_anim_stage = 0;
    leave_screen = ST_FALSE;
    m_conquest_anim_stage = 0;
    Set_Input_Delay(3);
    while(leave_screen == ST_FALSE)
    {
        Mark_Time();
        Clear_Fields();
        full_screen_esc = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, (int16_t)str_hotkey_ESC__ovr093[0], ST_UNDEFINED);
        input_field_idx = Get_Input();
        if(input_field_idx == full_screen_esc)
        {
            leave_screen = ST_UNDEFINED;
        }
        if(m_conquest_anim_stage > 40)
        {
            leave_screen = ST_UNDEFINED;
        }
        if(leave_screen == ST_FALSE)
        {
            Return_Animation_Draw();
            PageFlip_FX();
            Release_Time(5);
        }
    }
    Clear_Fields();
    Reset_Window();
    Clear_Fields();
    Deactivate_Auto_Function();
}


// WZD 093p16
void Return_Animation_Draw(void)
{
    int16_t x = 0;
    int16_t y = 0;
    x = 51;
    y = 28;
    Set_Page_Off();
    Copy_Back_To_Off();
    FLIC_Draw(x, y, conquest_conehead_seg);
    FLIC_Draw(x + 8, y + 8, conquest_effect_seg);
    Set_Outline_Color(16);
    Set_Font_Style_Shadow_Down(4, 4, 0, 0);
    Set_Alias_Color(190);
    Print_Paragraph(x + 7, y + 114, 165, GUI_String_1, 2);
    m_conquest_anim_stage++;
}
