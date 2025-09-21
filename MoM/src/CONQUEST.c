/*
    WIZARDS.EXE
        ovr093

MoO2
Module: ERIC

¿ CONQUEST.LBX ... CONQUEST.c/h. ?
*/

#include "../../MoX/src/MOX_TYPE.h"

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/GENDRAW.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/Timer.h"
#include "../../MoX/src/Util.h"

#include "../../MoX/src/MOM_Data.h"  /* _cities[] */
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOM_DEF.h"  /* NEUTRAL_PLAYER_IDX */
#include "../../MoX/src/MOX_DEF.h"  /* NEUTRAL_PLAYER_IDX */
#include "../../MoX/src/MOX_SET.h"  /* magic_set */
#include "UNITTYPE.h"  /* _unit_type_table[] */

#include "City_ovr55.h"
#include "CITYCALC.h"
#include "DIPLOMAC.h"
#include "MainScr.h"  /* sdl2_Play_Background_Music__WIP() */
#include "Spells128.h"
#include "WZD_o059.h"

#include "CONQUEST.h"



// WZD dseg:511C                                                 BEGIN:  ovr093 - Initialized Data

// WZD dseg:511C 00 00 5A 00 00 00 64 00 0C 00 49 00 0A 00 64 00+TBL_DefeatAnimSteps Spiral_Anim_Step <0, 90, 0, 100, 0>
// WZD dseg:511C 19 00 39 00 14 00 64 00 20 00 2D 00 1E 00 64 00+                                        ; DATA XREF: GAME_DrawLimboFall+1Fr ...
// WZD dseg:511C 28 00 1D 00 28 00 64 00 2A 00 0F 00 32 00 64 00+Spiral_Anim_Step <12, 73, 10, 100, 0>
// WZD dseg:511C 37 00 03 00 3C 00 64 00 4D 00 FC FF 46 00 64 00+Spiral_Anim_Step <25, 57, 20, 100, 0>
// WZD dseg:511C 77 00 F6 FF 50 00 5F 00 93 00 F8 FF 5A 00 5A 00+Spiral_Anim_Step <32, 45, 30, 100, 0>
// WZD dseg:511C AA 00 FB FF 64 00 55 00 BE 00 07 00 6E 00 50 00+Spiral_Anim_Step <40, 29, 40, 100, 0>
// WZD dseg:511C D4 00 13 00 78 00 4B 00 DA 00 28 00 82 00 46 00+Spiral_Anim_Step <42, 15, 50, 100, 0>
// WZD dseg:511C E3 00 3B 00 8C 00 41 00 D9 00 4F 00 96 00 3C 00+Spiral_Anim_Step <55, 3, 60, 100, 0>
// WZD dseg:511C D4 00 61 00 A0 00 37 00 C4 00 6A 00 AA 00 32 00+Spiral_Anim_Step <77, 65532, 70, 100, 0>
// WZD dseg:511C B3 00 71 00 B4 00 2D 00 A0 00 6D 00 BE 00 28 00+Spiral_Anim_Step <119, 65526, 80, 95, 0>
// WZD dseg:511C 91 00 69 00 C8 00 23 00 87 00 64 00 D2 00 1E 00+Spiral_Anim_Step <147, 65528, 90, 90, 0>
// WZD dseg:511C 82 00 5E 00 DC 00 19 00 87 00 57 00 E6 00 14 00+Spiral_Anim_Step <170, 65531, 100, 85, 0>
// WZD dseg:511C 93 00 57 00 F0 00 0F 00 98 00 59 00 FA 00 0A 00+Spiral_Anim_Step <190, 7, 110, 80, 0>
// WZD dseg:511C 9F 00 62 00 04 01 05 00                         Spiral_Anim_Step <212, 19, 120, 75, 0>
// WZD dseg:511C                                                 Spiral_Anim_Step <218, 40, 130, 70, 0>
// WZD dseg:511C                                                 Spiral_Anim_Step <227, 59, 140, 65, 0>
// WZD dseg:511C                                                 Spiral_Anim_Step <217, 79, 150, 60, 0>
// WZD dseg:511C                                                 Spiral_Anim_Step <212, 97, 160, 55, 0>
// WZD dseg:511C                                                 Spiral_Anim_Step <196, 106, 170, 50, 0>
// WZD dseg:511C                                                 Spiral_Anim_Step <179, 113, 180, 45, 0>
// WZD dseg:511C                                                 Spiral_Anim_Step <160, 109, 190, 40, 0>
// WZD dseg:511C                                                 Spiral_Anim_Step <145, 105, 200, 35, 0>
// WZD dseg:511C                                                 Spiral_Anim_Step <135, 100, 210, 30, 0>
// WZD dseg:511C                                                 Spiral_Anim_Step <130, 94, 220, 25, 0>
// WZD dseg:511C                                                 Spiral_Anim_Step <135, 87, 230, 20, 0>
// WZD dseg:511C                                                 Spiral_Anim_Step <147, 87, 240, 15, 0>
// WZD dseg:511C                                                 Spiral_Anim_Step <152, 89, 250, 10, 0>
// WZD dseg:511C                                                 Spiral_Anim_Step <159, 98, 260, 5, 0>

// WZD dseg:51F4
char magic_exe_file__ovr093[] = "MAGIC.EXE";

// WZD dseg:51FD
char str_empty_string__ovr093[] = "";

// WZD dseg:51FE
char str_JENNY__ovr093[] = "JENNY";

// WZD dseg:5204
char cnst_Conquest_Msg0__ovr093[] = " ";

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
// WZD dseg:5248 1B 00                                           
char cnst_HOTKEY_Esc10[] = "\x1B";

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
SAMB_ptr GAME_LimboFall_Stage;
// WZD dseg:C5BE
SAMB_ptr IMG_GAME_WizHandsUp;
// WZD dseg:C5C0
SAMB_ptr IMG_GAME_RedSparkle;
// WZD dseg:C5C2
SAMB_ptr GAME_Conquering_Wiz;
// WZD dseg:C5C4
SAMB_ptr GAME_Conquered_Wiz;
// WZD dseg:C5C6
SAMB_ptr SND_GAME_Scream;
uint32_t SND_GAME_Scream_size;  // DNE in Dasm
// WZD dseg:C5C8
SAMB_ptr m_conquest_anim_stage;
// WZD dseg:C5CA
SAMB_ptr IMG_GAME_ZappedWiz;
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
// drake178: WIZ_Conquer()
/*
; handles all actions associated with one wizard
; conquering another, including the banishment/defeat
; decision, animations, and displaying scoring if the
; human or last AI player is defeated, in which case
; the function will never return either, but will
; load MAGIC.EXE instead
*/
/*

Change_City_Ownership()
    WIZ_Conquer(city_owner_idx, player_idx, city_idx);

WIZ_Conquer(city_owner_idx, player_idx, city_idx);
// |-> WIZ_Banishment()
// MoO2 Module: COLCALC Eliminate_Player_

*/
void WIZ_Conquer__WIP(int16_t city_owner_idx, int16_t player_idx, int16_t city_idx)
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t Forfeited = 0;
    int16_t City_Count = 0;
    uint32_t * city_enchantments = 0;
    uint32_t * overland_enchantments = 0;
    int16_t troop_count = 0;
    int16_t itr_cench = 0;
    int16_t itr_troops = 0;  // uses itr_cench
    int16_t Target = 0;
    int16_t Conqueror = 0;
    int16_t City = 0;
    int16_t itr_cities = 0;  // _SI_
    int16_t itr_nodes = 0;  // _SI_
    int16_t itr_units = 0;  // _SI_
    int16_t itr_oench = 0;  // _SI_
    int16_t itr_players = 0;  // _SI_

    for(itr_cities = 0, City_Count = 0; ((itr_cities < _cities) && (City_Count < 2)); itr_cities++)
    {

        if(_CITIES[itr_cities].owner_idx == city_owner_idx)
        {

            City_Count++;

        }

    }

    Forfeited = ST_FALSE;

    if(City_Count > 1)  /* not about to lose very last city */
    {

        Forfeited = WIZ_Banishment__STUB(city_owner_idx, player_idx);

    }

    if(
        (City_Count >= 2)
        &&
        (Forfeited < 1)
    )
    {
        return;
    }

    if(city_idx != ST_UNDEFINED)
    {

        _CITIES[city_idx].owner_idx = player_idx;

    }

    _FORTRESSES[city_owner_idx].active = ST_FALSE;

    _players[player_idx].fame += 5;

    // ; remove all city enchantments and garrisoned heroes or
    // ; fantastic units owned by the target, turn all of
    // ; their garrisoned normal units neutral, and if they
    // ; are not the human player, turn their actual cities
    // ; neutral too
    for(itr_cities = 0; itr_cities < _cities; itr_cities++)
    {

        city_enchantments = &_CITIES[itr_cities].enchantments[0];

        // ; remove all city enchantments owned by the defeated
        // ; player
        for(itr_cench = 0; itr_cench < NUM_CITY_ENCHANTMENTS; itr_cench++)
        {
        
            if(city_enchantments[itr_cench] > 0)
            {

                if(city_enchantments[itr_cench] == city_owner_idx)
                {

                    city_enchantments[itr_cench] = 0;

                }

            }

        }

        if(_CITIES[itr_cities].owner_idx == city_owner_idx)
        {

            Army_At_City(itr_cities, &troop_count, &troops[0]);

            // ; dismiss all fantastic units or heroes from the city,
            // ; but transfer normal units to the neutral player
            for(itr_troops = 0; itr_troops < troop_count; itr_troops++)
            {

                // ; conflicting condition: the called function can only
                // ; return these, nothing else - will always jump
                if(_UNITS[troops[itr_troops]].owner_idx == city_owner_idx)
                {

                    if((_unit_type_table[_UNITS[troops[itr_troops]].type].Abilities & UA_FANTASTIC) != 0)
                    {

                        Kill_Unit(troops[itr_troops], 1);

                    }
                    else if(_UNITS[troops[itr_troops]].Hero_Slot != -1)
                    {

                        Kill_Unit(troops[itr_troops], 1);

                    }
                    else
                    {

                        _UNITS[troops[itr_troops]].owner_idx = NEUTRAL_PLAYER_IDX;

                    }

                }

            }

            if(_CITIES[itr_cities].owner_idx == _human_player_idx)  /* BUGBUG  DEDU  Why just human player? */
            {

                _CITIES[itr_cities].owner_idx = NEUTRAL_PLAYER_IDX;

            }

        }
        
    }

    // ; reset the meld flags and the owners of all nodes
    // ; controlled by the target
    for(itr_nodes = 0; itr_nodes < NUM_NODES; itr_nodes++)
    {

        if(_NODES[itr_nodes].owner_idx == city_owner_idx)
        {

            _NODES[itr_nodes].owner_idx = ST_UNDEFINED;

            _NODES[itr_nodes].flags = NF_NONE;

        }

    }

    // Finally, when enemy cities are conquered, the conquering wizard gets a portion of the previous owner's gold reserve as loot.
    Player_Add_Gold(player_idx, (_players[city_owner_idx].gold_reserve / 2));

    // When a wizard's enchanted fortress is captured by another wizard, the conqueror steals 50% of his opponent’s mana reserves, ...
    Player_Add_Mana(player_idx, (_players[city_owner_idx].mana_reserve / 2));

    // ; dismiss the target's remaining units (i.e. those that
    // ; were not in cities)
    for(itr_units = 0; itr_units < _units; itr_units++)
    
    {
        if(_UNITS[itr_units].owner_idx == city_owner_idx)
        {

            Kill_Unit(itr_units, 1);

        }

    }

    // ; remove all of the target's global enchantments
    overland_enchantments = &_players[city_owner_idx].Globals;

    for(itr_oench = 0; itr_oench < NUM_OVERLAND_ENCHANTMENTS; itr_oench++)
    {

        overland_enchantments[itr_oench] = 0;

    }


    WIZ_Conquest__WIP(city_owner_idx, player_idx);


    // ; zero the diplomatic status of the target with all
    // ; remaining wizards, and generate a type 7 (-40)
    // ; diplomatic action towards the conqueror for them
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        _players[itr_players].Dipl.Dipl_Status[city_owner_idx] = 0;
        _players[city_owner_idx].Dipl.Dipl_Status[itr_players] = 0;

        _players[itr_players].Dipl.Contacted[city_owner_idx] = 0;
        _players[city_owner_idx].Dipl.Contacted[itr_players] = 0;

        if(itr_players != city_owner_idx)
        {

            // Note, however, that the conquering wizard suffers permanent diplomatic penalties to his or her interactions with other wizards after such an event.
            Change_Relations__WIP(-40, player_idx, itr_players, 7, 0, 0);

        }

    }

    WIZ_ConquestSpells__STUB(player_idx, city_owner_idx);

    if(city_owner_idx == _human_player_idx)
    {

        GAME_LimboFallAnim__STUB(city_owner_idx);

        // GAME_OVER();

        // s01p16_empty_function();

        // GAME_EXE_Swap(cnst_MAGIC_EXE_File, cnst_MAGICEXE_arg0, cnst_ZeroString_7, cnst_ZeroString_7);

    }
    else
    {

        if(GAME_IsWon__STUB() == ST_TRUE)
        {

            GAME_PlayVictoryAnim__STUB(_human_player_idx);

            // GAME_OVER();

            // s01p16_empty_function();

            // GAME_EXE_Swap(cnst_MAGIC_EXE_File, cnst_MAGICEXE_arg0, cnst_ZeroString_7, cnst_ZeroString_7);

        }

    }

}


// WZD 093p02
// drake178: WIZ_Conquest()
/*
; plays the conquest animation (banishment or defeat)
; and, if the conqueror is the human player, sets the
; bit in their Defeated_Wizard bitfield
*/
/*

WIZ_Conquer__WIP()
    WIZ_Conquest__WIP(city_owner_idx, player_idx);

*/
void WIZ_Conquest__WIP(int16_t city_owner_idx, int16_t player_idx)
{
    int16_t Space_String[4] = { 0, 0, 0, 0 };
    int16_t City_Count = 0;
    int16_t Music_Data_Seg = 0;
    uint32_t Music_Data_Seg_size = 0;  // HACK  DNE in Dasm
    int16_t Esc_Hotkey_Index = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t itr_cities = 0;  // _SI_

    strcpy(Space_String, cnst_Conquest_Msg0__ovr093);

    if(
        (player_idx != NEUTRAL_PLAYER_IDX)
        &&
        (city_owner_idx != NEUTRAL_PLAYER_IDX)
    )
    {
        return;
    }

    if(player_idx == _human_player_idx)
    {

        Set_Bit_Field_Near(city_owner_idx, &_players[_human_player_idx].Defeated_Wizards);

    }

    GAME_Conquered_Wiz = city_owner_idx;

    GAME_Conquering_Wiz = player_idx;

    Stop_All_Sounds__STUB();

    Set_Mouse_List(1, mouse_list_none);
    Clear_Fields();
    Fade_Out();
    Set_Page_Off();
    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);
    Toggle_Pages();

    // ; count the cities of the conquered wizard
    for(itr_cities = 0, City_Count = 0; ((itr_cities < _cities) && (City_Count < 2)); itr_cities++)
    {

        if(_CITIES[itr_cities].owner_idx == GAME_Conquered_Wiz)
        {

            City_Count++;

        }

    }

    Load_Palette(3, ST_UNDEFINED, ST_NULL);  // ; WILZAB - conquest palette

    Apply_Palette();

    Set_Page_Off();

    // WIZLAB.LBX, 019    "WIZLAB"      ""
    itr_cities = LBX_Reload(wizlab_lbx_file__ovr093, 19, _screen_seg);

    FLIC_Draw(0, 0, itr_cities);

    Copy_Off_To_Back();

    Copy_Off_To_Page4();

    GUI_String_1 = Near_Allocate_First(100);

    GUI_String_2 = Near_Allocate_First(100);

    if(GAME_Conquering_Wiz != NEUTRAL_PLAYER_IDX)
    {
        strcpy(GUI_String_1, _players[player_idx].name);
        strcat(GUI_String_1, Space_String);
        if(City_Count > 1)
        {
            strcat(GUI_String_1, cnst_Conquest_Msg3);  // "banishes"
        }
        else
        {
            strcat(GUI_String_1, cnst_Conquest_Msg4);  // "defeats"
        }
        strcat(GUI_String_1, Space_String);
        strcat(GUI_String_1, _players[city_owner_idx].name);
    }
    else
    {
        strcpy(GUI_String_1, cnst_Raiders_2);
        strcat(GUI_String_1, Space_String);
        strcat(GUI_String_1, cnst_Conquest_Msg1);  // "destroy"
        strcat(GUI_String_1, Space_String);
        strcpy(GUI_String_2, _players[city_owner_idx].name);
        if(City_Count > 1)
        {
            Possessive(GUI_String_2);
            strcat(GUI_String_1, GUI_String_2);
            strcat(GUI_String_1, Space_String);
            strcat(GUI_String_1, cnst_Conquest_Msg2);  // "fortress"
        }
        else
        {
            strcat(GUI_String_1, GUI_String_2);
        }
        strcat(GUI_String_1, cnst_Dot7);
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
    wizlab_wizard_seg__ovr093 = LBX_Reload(wizlab_lbx_file__ovr093, _players[city_owner_idx].wizard_id, _screen_seg);

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
    conquest_wizard_seg = LBX_Reload_Next(conquest_lbx_file__ovr093, _players[player_idx].wizard_id, _screen_seg);

    // CONQUEST.LBX, 014    "CONEHEAD"  ""
    conquest_conehead_seg = LBX_Reload_Next(conquest_lbx_file__ovr093, 14, _screen_seg);

    // CONQUEST.LBX, 015    "AXEBOY"    ""
    conquest_axeboy_seg = LBX_Reload_Next(conquest_lbx_file__ovr093, 15, _screen_seg);

    // CONQUEST.LBX, 017    "ZAP"       ""
    Open_File_Animation__HACK(conquest_lbx_file__ovr093, 17);

    if(magic_set.background_music == ST_TRUE)
    {
        itr_cities = 109;  /* MUSIC_WIN_Military */
        if(city_owner_idx == _human_player_idx) 
        {
            itr_cities = 110;  /* MUSIC_LOSE_Military */
        }

        Music_Data_Seg = LBX_Reload(music_lbx_file__ovr093, itr_cities, SND_Music_Segment);
        Music_Data_Seg_size = lbxload_entry_length;

        sdl2_Play_Sound__WIP(Music_Data_Seg, Music_Data_Seg_size);

    }

    Assign_Auto_Function(Conquest_Draw__WIP, 6);

    m_conquest_anim_stage = 0;

    leave_screen = ST_FALSE;

    for(itr_cities = 0; itr_cities < 3; itr_cities++)
    {

        Mark_Time();

        Conquest_Draw__WIP();

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

        Esc_Hotkey_Index = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, cnst_HOTKEY_Esc10, ST_UNDEFINED);

        input_field_idx = Get_Input();

        if(input_field_idx == Esc_Hotkey_Index)
        {

            leave_screen = ST_UNDEFINED;

        }

        if(m_conquest_anim_stage > 30)
        {

            leave_screen = ST_UNDEFINED;

        }

        if(leave_screen == ST_FALSE)
        {

            Conquest_Draw__WIP();

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

    // DOMSDOS  Play_Background_Music__STUB();
    sdl2_Play_Background_Music__WIP();

}


// WZD 093p03
// drake178: GAME_DrawConqstAnim()
/*
; plays a frame of the conquest animation based on the
; global GAME_ConqstAnimStage variable (which it
; increments in the end), changing memory allocation
; halfway through to load and play the appropriate
; scream sound
*/
/*

*/
void Conquest_Draw__WIP(void)
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

            GAME_DrawConquerors__STUB();

        }
        else if(m_conquest_anim_stage < 7)
        {

            GAME_DrawConquerors__STUB();

        }
        else if(m_conquest_anim_stage == 7)
        {

            GAME_Conqest_Scene2__STUB();

        }
        else if(m_conquest_anim_stage < 11)
        {

            GAME_DrawZappedWiz__STUB(1);

        }
        else if(m_conquest_anim_stage < 19)
        {

            if(
                (GAME_Conquering_Wiz != NEUTRAL_PLAYER_IDX)
                &&
                (m_conquest_anim_stage == 11)
                ||
                (magic_set.sound_effects == ST_TRUE)
            )
            {

                sdl2_Play_Sound__WIP(SND_GAME_Scream, SND_GAME_Scream_size);

            }

            GAME_DrawZapping__STUB();

        }
        else if(m_conquest_anim_stage < 23)
        {

            GAME_DrawZappedWiz__STUB(0);

        }

    }

    if(
        (m_conquest_anim_stage > 6)
        &&
        (GAME_Conquering_Wiz == NEUTRAL_PLAYER_IDX)
    )
    {

        FLIC_Draw(69, 75, wizlab_wizard_seg__ovr093);

        sub_79907__WIP();

    }

    Set_Font_Style_Outline_Heavy(5, 5, 0, 0);

    Set_Outline_Color(1);

    Print_Centered(160, 6, GUI_String_1);

    m_conquest_anim_stage++;

}


// WZD 093p04
// drake178: GAME_DrawConquerors()
/*
*/
/*

*/
void GAME_DrawConquerors__STUB(void)
{



}


// WZD 093p05
// drake178: GAME_Conqest_Scene2()
/*
; re-saves draw frame 3 with conehead, axeboy, and the
; conquering wizard already present, loads new
; animations, and loads the appropriate scream sound
*/
/*

*/
void GAME_Conqest_Scene2__STUB(void)
{

    if(GAME_Conquering_Wiz == NEUTRAL_PLAYER_IDX)
    {
        return;
    }



}


// WZD 093p06
// drake178: GAME_DrawZappedWiz()
/*
*/
/*

*/
void GAME_DrawZappedWiz__STUB(int16_t flag)
{



}


// WZD 093p07
// drake178: GAME_DrawZapping()
/*
*/
/*

*/
void GAME_DrawZapping__STUB(void)
{


    
}


// WZD 093p08
// drake178: sub_79907()
/*
    Draws Axeboy and Conehead, and maybe Conquering Wizard.

If the conquerer is the neutral player, then do not draw a wizard.
But, only called is GAME_Conquering_Wiz == NEUTRAL_PLAYER_IDX?!?

*/
void sub_79907__WIP(void)
{

    Clipped_Draw(252, 88, conquest_axeboy_seg);

    if(GAME_Conquering_Wiz != NEUTRAL_PLAYER_IDX)
    {

        Clipped_Draw(208, 60, conquest_wizard_seg);

    }

    Clipped_Draw(100, 120, conquest_conehead_seg);

}


// WZD 093p09
// drake178: GAME_IsWon()
/*
*/
/*

*/
int16_t GAME_IsWon__STUB(void)
{



}


// WZD 093p10
// drake178: GAME_PlayVictoryAnim()
/*
*/
/*

*/
void GAME_PlayVictoryAnim__STUB(int16_t player_idx)
{



}


// WZD 093p11
// drake178: GAME_Draw_WIN_Anim()
// GAME_Draw_WIN_Anim()

// WZD 093p12
// drake178: GAME_LimboFallAnim()
/*
*/
/*

*/
void GAME_LimboFallAnim__STUB(int16_t player_idx)
{



}


// WZD 093p13
// drake178: GAME_DrawLimboFall()
// GAME_DrawLimboFall()

// WZD 093p14
// drake178: WIZ_Banishment()
/*
*/
/*

*/
int16_t WIZ_Banishment__STUB(int16_t loser_idx, int16_t winner_idx)
{



}


// WZD 093p15
// drake178: GAME_ReturnDialog()
// GAME_ReturnDialog()

// WZD 093p16
// drake178: GAME_DrawReturnAnim()
// GAME_DrawReturnAnim()

