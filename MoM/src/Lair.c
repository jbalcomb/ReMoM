/*
    Lair

    WIZARDS.EXE
        ovr083

BACKGRND.LBX
    LCONFIRM    lair confirm borde
    LCONFIRM    lair bottom w/butt
    LCONFIRM    lair bottom wo/but

NOTE: IDGI, but the FanDom MoM Wiki uses the term "Encounter Zone"
*/

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/GENDRAW.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/MOX_DAT.h"  /* _screen_seg */
#include "../../MoX/src/MOX_SET.h"  /* magic_set */
#include "../../MoX/src/Util.h"
#include "../../MoX/src/paragrph.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/SOUND.h"

#include "CITYCALC.h"
#include "City_ovr55.h"
#include "Combat.h"
#include "HIRE.h"
#include "ItemMake.h"
#include "ItemScrn.h"
#include "MainScr.h"
#include "MainScr_Maps.h"
#include "NEXTTURN.h"
#include "Spellbook.h"
#include "Spells128.h"
#include "UNITTYPE.h"
#include "WZD_o059.h"

#include <stdlib.h>

#include <SDL_stdinc.h>

#include "Lair.h"



// WZD dseg:4AB4                                                 ¿ BEGIN: ovr083 - Strings ?

// WZD dseg:4AE5
char cnst_ZeroString_0[] = "";

// WZD dseg:4B51
char cnst_Reward_Msg_9[] = "s";

// WZD dseg:4AE0
char cnst_Tower[] = "tower";
// WZD dseg:4AE6
char cnst_ChaosNode[] = "chaos node";
// WZD dseg:4AF1
char cnst_NatureNode[] = "nature node";
// WZD dseg:4AFD
char cnst_SorceryNode[] = "sorcery node";
// WZD dseg:4B0A
char cnst_Cave[] = "mysterious cave";
// WZD dseg:4B1A
char cnst_Dungeon[] = "dungeon";
// WZD dseg:4B22
char cnst_AncientTmpl[] = "ancient temple";
// WZD dseg:4B31
char cnst_Keep[] = "abandoned keep";
// WZD dseg:4B40
char cnst_Lair[] = "monster lair";
// WZD dseg:4B4D
char cnst_Ruins[] = "ruins";
// WZD dseg:4B53
char cnst_FallenTmpl[] = "fallen temple";



// WZD dseg:4B61
char cnst_EZ_Indef1[] = "a";
// WZD dseg:4B63
char cnst_EZ_Indef2[] = "an";
// WZD dseg:4B66
char cnst_EZ_Indef3[] = "some";


// WZD dseg:4AB4
char * TBL_EZ_Names[] =
{
    cnst_Tower,
    cnst_ChaosNode,
    cnst_NatureNode,
    cnst_SorceryNode,
    cnst_Cave,
    cnst_Dungeon,
    cnst_AncientTmpl,
    cnst_Keep,
    cnst_Lair,
    cnst_Ruins,
    cnst_FallenTmpl
};

// WZD dseg:4ACA
char * TBL_EZ_Indefinites[] = 
{
    cnst_EZ_Indef1,
    cnst_EZ_Indef1,
    cnst_EZ_Indef1,
    cnst_EZ_Indef1,
    cnst_EZ_Indef1,
    cnst_EZ_Indef1,
    cnst_EZ_Indef2,
    cnst_EZ_Indef2,
    cnst_EZ_Indef1,
    cnst_EZ_Indef3,
    cnst_EZ_Indef1
};


// WZD dseg:4B6B
char music_lbx_file__ovr083[] = "MUSIC";

// WZD dseg:4B71
char cnst_EZ_Msg_1[] = "You have found ";
// WZD dseg:4B7F
char cnst_EZ_Msg_2[] = " ";
// WZD dseg:4B81
char cnst_Dot6[] = ".";

// WZD dseg:4B83
char cnst_EZ_Msg_3[] = "  Scouts have spotted ";
// WZD dseg:4B9A
char cnst_EZ_Msg_4[] = " within the ";
// WZD dseg:4BA7
char cnst_EZ_Msg_5[] = ".  Do you wish to enter?";

// WZD dseg:4BC0
char reload_lbx_file__ovr083[] = "RELOAD";
// WZD dseg:4BC7
char cnst_RESOURCE_File[] = "RESOURCE";

// WZD dseg:4BD0
char cnst_Reward_Msg_1[] = "Inside you find ";
// WZD dseg:4BE1
char cnst_Reward_Msg_2[] = "absolutely nothing";
// WZD dseg:4BF4
char cnst_Reward_Msg_3[] = " gold";
// WZD dseg:4BFA
char cnst_Reward_Msg_4[] = " mana crystals";
// WZD dseg:4C09
char cnst_Reward_Msg_5[] = "a prisoner";
// WZD dseg:4C14
char cnst_Reward_Msg_6[] = " spell";
// WZD dseg:4C1B
char cnst_Reward_Msg_7[] = " spell book";
// WZD dseg:4C27
char cnst_Reward_Msg_8[] = "a retort of ";
// WZD dseg:4C34
char cnst_And[] = " and ";
// WZD dseg:4C3A
char cnst_Comma[] = ", ";

// WZD dseg:4C3D
char cnst_HOTKEY_Y_2[] = "Y";
// WZD dseg:4C3F
char cnst_HOTKEY_N_3[] = "N";
// WZD dseg:4C41
char cnst_HOTKEY_EscE[] = "\x1B";

// WZD dseg:4C43 00                                              db    0

// WZD dseg:4C43                                                 ¿ END: ovr083 - Strings ?



// WZD dseg:C2FC                                                 ¿ BEGIN" ovr083 - Uninitialized Data ?

// WZD dseg:C2FC
char * lair_message_box_text;
// WZD dseg:C2FE
int16_t lair_confirm_buttons_flag;

// WZD dseg:C2FE                                                 ¿ END" ovr083 - Uninitialized Data ?




// WZD o83p01
// drake178: TILE_HasEncounter()
// DEDU  ¿ ~Lair_At_Square() ? ¿ ever tested for anything but !-1 ?
int16_t Square_Has_Lair(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t square_has_lair;
    int16_t itr_lairs;

    square_has_lair = ST_UNDEFINED;

    for(itr_lairs = 0; ((itr_lairs < NUM_LAIRS) && (square_has_lair == ST_UNDEFINED)); itr_lairs++)
    {
        if(
            (_LAIRS[itr_lairs].wp == wp)
            &&
            (_LAIRS[itr_lairs].intact == ST_TRUE)
            &&
            (_LAIRS[itr_lairs].wy == wy)
            &&
            (_LAIRS[itr_lairs].wx == wx)
        )
        {
            square_has_lair = itr_lairs;
        }
    }

    return square_has_lair;
}

// drake178: EZ_Clear();
void Lair_Clear(int16_t lair_idx)
{
    int16_t status;  // _DI_
    int16_t itr_lairs;  // _CX_

    _LAIRS[lair_idx].intact = ST_FALSE;

    // For Towers, clear the other side, as well.
    if(_LAIRS[lair_idx].type == lt_Tower)
    {
        status = ST_UNDEFINED;
        for(itr_lairs = 0; ((itr_lairs < NUM_LAIRS) && (status == ST_UNDEFINED)); itr_lairs++)
        {
            if(_LAIRS[itr_lairs].intact == ST_TRUE)
            {
                if(
                    (_LAIRS[itr_lairs].wy == _LAIRS[lair_idx].wy)
                    &&
                    (_LAIRS[itr_lairs].wx == _LAIRS[lair_idx].wx)
                )
                {
                    _LAIRS[itr_lairs].intact = ST_FALSE;
                    status = ST_FALSE;
                }
            }
        }
    }

}


// WZD o83p03
// drake178: sub_6D490();
int16_t UU_Player_Maybe_Lair_Combat(int16_t player_idx, int16_t wx, int16_t wy, int16_t wp)
{
    int16_t lair_idx;  // _SI_
    int16_t status;  // _DI_

    lair_idx = Square_Has_Lair(wx, wy, wp);

    status = ST_TRUE;

    if(lair_idx != -1)
    {
        status = Lair_Combat__WIP(lair_idx, player_idx);
    }

    return status;
}

// WZD o83p04
/*
returns 1 if cleared, 0 if not, 99 if cancelled
*/
int16_t Lair_Combat__WIP(int16_t lair_idx, int16_t player_idx)
{
    int16_t Reward_Specials[2] = { 0, 0 };
    int16_t Reward_Special_Count = 0;
    SAMB_ptr sound_seg = 0;
    int16_t Previous_Game_State = 0;
    int16_t Total_Rewards = 0;
    int16_t Reward_Hero = 0;
    int16_t Hero_Slot = 0;
    int16_t Reward_Spell = 0;
    int16_t Reward_Items[3] = { 0, 0, 0 };
    int16_t treasure_window_y = 0;
    int16_t Encounter_Outcome = 0;
    uint32_t sound_seg_size = 0;  // DNE in Dasm

    Clear_Fields();

    notify_draw_offscr = ST_FALSE;  // ¿ ALWAYS 0 ?

    Encounter_Outcome = ST_TRUE;


    if (player_idx == _human_player_idx)
    {
        if (magic_set.event_music == ST_TRUE)
        {
            sound_seg = LBX_Reload(music_lbx_file__ovr083, MUSIC_Site_Discovery, SND_Music_Segment);
            sound_seg_size = lbxload_entry_length;
            // DOMSDOS  Play_Sound__WIP(sound_seg);
            sdl2_Play_Sound__WIP(sound_seg, sound_seg_size);
        }

        Encounter_Outcome = Do_Lair_Confirm(lair_idx);

        Set_Bit_Field(1, &_LAIRS[lair_idx].Misc_Flags);

        // DOMSDOS  Play_Background_Music__STUB();
        sdl2_Play_Background_Music__WIP();
    }

    if(Encounter_Outcome == ST_FALSE)
    {
        goto Done_Cancel;
    }

    treasure_window_y = 100;  // unused and changed to 60 below; only used for Lair_Treasure_Popup()

    notify_draw_offscr = ST_FALSE;  // ¿ ALWAYS 0 ?

    // DEDU: Why _combat_environ_idx here instead of passed in lair_idx? Macro? API Boundary?
    _combat_wx = _LAIRS[_combat_environ_idx].wx;
    _combat_wy = _LAIRS[_combat_environ_idx].wy;
    _combat_wp = _LAIRS[_combat_environ_idx].wp;

    if(
        (Encounter_Outcome == ST_TRUE)  // drake178: ; conflicting condition, will never jump
        &&
        ((_LAIRS[lair_idx].guard1_count & 0x0F) > 0)
    )
    {
        notify_draw_offscr = ST_FALSE;  // drake178: ; already 0



        Encounter_Outcome = Lair_Combat_Do(lair_idx, player_idx);



        if(player_idx == _human_player_idx)
        {
            Set_Bit_Field(2, &_LAIRS[lair_idx].Misc_Flags);  // DEDU: ¿ bit 2 is ~ did_attack ? ¿ ever tested ? ¿ how diff from explored/visited ?
        }

    }

    if(
        (Encounter_Outcome == ST_TRUE)
        &&
        (player_idx == _human_player_idx)
    )
    {
        treasure_window_y = 60;
        Clear_Fields();
        Allocate_Reduced_Map();
        Clear_Palette_Changes(0, 255);
        Set_Palette_Changes(0, 223);
        Update_Remap_Gray_Palette();
        Set_Page_Off();
        Main_Screen_Draw();
        PageFlip_FX();
        Copy_On_To_Off_Page();
    }

    /*
        BEGIN: Fought & Won
    */
    if (Encounter_Outcome == ST_TRUE)
    {
        Reward_Special_Count = 0;

        Lair_Generate_Treasure(player_idx, lair_idx, &Reward_Items[0], &Reward_Spell, &Reward_Hero, &Hero_Slot, &Total_Rewards, &Reward_Special_Count, &Reward_Specials[0]);

        if(player_idx == _human_player_idx)
        {
            Lair_Treasure_Popup(lair_idx, treasure_window_y, &Reward_Items[0], Reward_Spell, Hero_Slot, Total_Rewards, Reward_Special_Count, &Reward_Specials[0]);
        }

        Lair_Loot_Gold_And_Mana(lair_idx, player_idx);

        notify_draw_offscr = ST_FALSE;  // ¿ ALWAYS 0 ?

        /*
            BEGIN:  Hero Treasure (Prisoner)
        */
        if (
            (Hero_Slot != ST_UNDEFINED)
            &&
            (player_idx == _human_player_idx)
        )
        {
            Set_Draw_Active_Stack_Always();
            // drake178: ; BUG: can push a unit out of the capital when a prisoner is found also has some inherited BUGs, and handles the USW not completely through its own functions
            // int16_t Hire_Hero_Popup(int16_t hero_slot_idx, int16_t unit_type_idx, int16_t hire_type)
            if(Hire_Hero_Popup(Hero_Slot, Reward_Hero, 2) == ST_TRUE)
            {
                // drake178: ; BUG: this needs to be done when hiring the hero
                _UNITS[(_units - 1)].wx = (int8_t)_combat_wx;
                _UNITS[(_units - 1)].wy = (int8_t)_combat_wy;
                _UNITS[(_units - 1)].wp = (int8_t)_combat_wp;
            }
            Reset_Draw_Active_Stack();
        }
        /*
            END:  Hero Treasure (Prisoner)
        */


        /*
            BEGIN:  Item Treasure
        */
        if (
            (_LAIRS[lair_idx].Item_Count > 0)
            &&
            (player_idx == _human_player_idx)
        )
        {
            Previous_Game_State = current_screen;
            Clear_Fields();
            Allocate_Reduced_Map();
            Clear_Palette_Changes(0, 255);
            Set_Palette_Changes(0, 223);
            Update_Remap_Gray_Palette();
            Set_Page_Off();
            Main_Screen_Draw();
            PageFlip_FX();
            Copy_On_To_Off_Page();

            item_pool_in_process = ST_TRUE;
            m_item_wx = _LAIRS[lair_idx].wx;
            m_item_wy = _LAIRS[lair_idx].wy;
            m_item_wp = _LAIRS[lair_idx].wp;
            Process_Item_Pool(_LAIRS[lair_idx].Item_Count, &Reward_Items[0]);
            item_pool_in_process = ST_FALSE;

            current_screen = Previous_Game_State;
        }
        /*
            END:  Item Treasure
        */


        /*
            BEGIN:  Spell Treasure
        */

        if(Reward_Spell > -1)
        {
            Player_Gets_Spell(player_idx, Reward_Spell, 1);
        }
        /*
            END:  Spell Treasure
        */


        Lair_Clear(lair_idx);

    }
    /*
        END: Fought & Won
    */

    // DEDU: ¿ ~generic* reset message box ? ¿ Macro ?
    confirmation_box_x_offset = 0;
    confirmation_box_y_offset = 0;
    notify_draw_offscr = ST_FALSE;  // ¿ ALWAYS 0 ?

    if(player_idx == _human_player_idx)
    {
        Check_Research_Spell_Is_Known(0);
    }

    goto Done;

Done_Cancel:
    Encounter_Outcome = 99;  // ~ LAIR_CONFIRM_CANCEL

Done:
    return Encounter_Outcome;
}


// WZD o83p05
// drake178: OVL_EZ_Confirm_Entry()
int16_t Do_Lair_Confirm(int16_t lair_idx)
{
    int16_t choice;
    int16_t lair_is_node;
    int16_t lair_has_guardians;
    char * UU_temp_string;
    SAMB_ptr lair_confirm_pict;
    int16_t lair_type;  // _SI_

    Set_Mouse_List(1, mouse_list_default);

    lair_type = _LAIRS[lair_idx].type;

    lair_message_box_text = (char *)Near_Allocate_First(500);

    UU_temp_string = (char *)Near_Allocate_Next(100);

    strcpy(lair_message_box_text, cnst_EZ_Msg_1);  // "You have found"

    strcat(lair_message_box_text, TBL_EZ_Indefinites[lair_type]);

    strcat(lair_message_box_text, cnst_EZ_Msg_2);  // " "

    strcat(lair_message_box_text, TBL_EZ_Names[lair_type]);

    strcat(lair_message_box_text, cnst_Dot6);  // "."


    lair_has_guardians = ST_FALSE;

    if( (_LAIRS[lair_idx].guard1_count & 0x0F) != 0)
    {
        lair_has_guardians = ST_TRUE;

        strcat(lair_message_box_text, cnst_EZ_Msg_3);  // "  Scouts have spotted "

        if(_unit_type_table[_LAIRS[lair_idx].guard1_unit_type].Figures == 1)
        {
            strcat(lair_message_box_text, An(*_unit_type_table[_LAIRS[lair_idx].guard1_unit_type].name));
            strcat(lair_message_box_text, cnst_EZ_Msg_2);
        }

        strcat(lair_message_box_text, *_unit_type_table[_LAIRS[lair_idx].guard1_unit_type].name);

        strcat(lair_message_box_text, cnst_EZ_Msg_4);  // " within the "

        strcat(lair_message_box_text, TBL_EZ_Names[lair_type]);

        strcat(lair_message_box_text, cnst_EZ_Msg_5);  // ".  Do you wish to enter?"

    }


    // {chaos node, nature node, sorcery node}
    lair_is_node = ST_FALSE;
    if(
        (lair_type > lt_Tower)
        &&
        (lair_type < lt_Cave)
    )
    {
        lair_is_node = ST_TRUE;
    }


    // TODO  EMM_Sandbox2VGAFILEH();

    lair_confirm_pict = LBX_Reload(reload_lbx_file__ovr083, (9 + lair_type), _screen_seg);    // "RELOAD" ... {LAIR, CHAOS, NATURE, WATER, LAIR, LAIR, LAIR, LAIR, LAIR, LAIR, LAIR}

    confirmation_button_yes_seg = LBX_Reload_Next(cnst_RESOURCE_File, 3, _screen_seg);  // "RESOURCE" ... CONFMBUT yes

    confirmation_button_no_seg = LBX_Reload_Next(cnst_RESOURCE_File, 4, _screen_seg);   // "RESOURCE" ... CONFMBUT no

    choice = Lair_Confirm(160, 60, lair_message_box_text, lair_confirm_pict, 0, 0, lair_has_guardians, lair_is_node);

    // TODO  EMM_VGAFILEH2Sandbox();


    if(lair_has_guardians == ST_FALSE)
    {
        choice = ST_TRUE;
    }

    return choice;
}

// WZD o83p06
/*

*/
void Lair_Treasure_Popup(int16_t lair_idx, int16_t window_y, int16_t item_list[], int16_t spell, int16_t hero_slot, int16_t Total_Rewards, int16_t Reward_Special_Count, int16_t Reward_Specials[])
{

    char Spell_Name[30] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t List_Count = 0;
    int16_t lair_item_count = 0;
    SAMB_ptr lair_treasure_notify_pict_seg = 0;
    int16_t amount[3] = { 0, 0, 0 };
    char * Item_Type_Names[3] = { 0, 0, 0 };
    char * temp_string = 0;
    int16_t itr = 0;  // _SI_

    lair_message_box_text = (char *)Near_Allocate_First(500);  // ¿ same size asMoO2  Rescued_Loknar_Popup_() ?

    temp_string = (char *)Near_Allocate_Next(100);

    for(itr = 0; itr < 3; itr++)
    {
        Item_Type_Names[itr] = (char *)Near_Allocate_Next(20);
    }

    // RELOAD.LBX, 20  020  TREASUR2������������������������
    lair_treasure_notify_pict_seg = LBX_Reload(reload_lbx_file__ovr083, 20, _screen_seg);

    strcpy(lair_message_box_text, cnst_Reward_Msg_1);  // "Inside you find "

    // BUG:  ¿ should be if nothing else something ?
    if(Total_Rewards < 1)
    {
        strcat(lair_message_box_text, cnst_Reward_Msg_2);  // "absolutely nothing"
    }

    List_Count = 0;

    // ¿ ; BUG: ignores 1 ?
    if(_LAIRS[lair_idx].Loot_Gold > 1)
    {
        Add_Comma_Or_And(&List_Count, Total_Rewards, lair_message_box_text);
        SDL_itoa(_LAIRS[lair_idx].Loot_Gold, temp_string, 10);
        strcat(lair_message_box_text, temp_string);
        strcat(lair_message_box_text, cnst_Reward_Msg_3);  // " gold"
    }

    // ¿ ; BUG: ignores 1 ?
    if(_LAIRS[lair_idx].Loot_Mana > 1)
    {
        Add_Comma_Or_And(&List_Count, Total_Rewards, lair_message_box_text);
        SDL_itoa(_LAIRS[lair_idx].Loot_Mana, temp_string, 10);
        strcat(lair_message_box_text, temp_string);
        strcat(lair_message_box_text, cnst_Reward_Msg_4);  // " mana crystals"
    }

    if(hero_slot != -1)
    {
        Add_Comma_Or_And(&List_Count, Total_Rewards, lair_message_box_text);
        strcat(lair_message_box_text, cnst_Reward_Msg_5);  // "a prisoner"
    }

    if(spell > -1)
    {
        Add_Comma_Or_And(&List_Count, Total_Rewards, lair_message_box_text);
        strcpy(Spell_Name, spell_data_table[spell].name);
        strcat(lair_message_box_text, An(Spell_Name));
        strcat(lair_message_box_text, cnst_EZ_Msg_2);  // " "
        strcat(lair_message_box_text, Spell_Name);
        strcat(lair_message_box_text, cnst_Reward_Msg_6);  // " spell"
    }

    if(Reward_Special_Count > 0)
    {
        amount[0] = 1;
        amount[1] = 1;
        if(Reward_Special_Count > 1)
        {
            if(Reward_Specials[0] == Reward_Specials[1])
            {
                amount[0] = 2;
                amount[1] = -1;
                Reward_Special_Count--;
                Total_Rewards--;
            }
        }

        for(itr = 0; itr < Reward_Special_Count; itr++)
        {
            Add_Comma_Or_And(&List_Count, Total_Rewards, lair_message_box_text);
            if(Reward_Specials[itr] < 100)
            {
                SDL_itoa(amount[itr], temp_string, 10);
                strcat(lair_message_box_text, temp_string);
                strcat(lair_message_box_text, cnst_EZ_Msg_2);
                /*
                            Severity	Code	Description	Project	File	Line	Suppression State
                    TODO    Warning	C6385	Reading invalid data from 'rtn16':  the readable size is '40' bytes, but 'Reward_Specials[itr]' bytes may be read.	ReMoM	C:\STU\devel\ReMoM\src\Lair.C	634	
                */
                strcat(lair_message_box_text, rtn16[Reward_Specials[itr]]);
                strcat(lair_message_box_text, cnst_Reward_Msg_7);  // " spell book"
                if(amount[itr] > 1)
                {
                    strcat(lair_message_box_text, cnst_Reward_Msg_9);
                }
            }
            else
            {
                strcat(lair_message_box_text, cnst_Reward_Msg_8);  // "a retort of "
                // push    _wizard_abilities_names[bx-(2*64h)]
                /*
                            Severity	Code	Description	Project	File	Line	Suppression State
                    TODO    Warning	C6385	Reading invalid data from '_wizard_abilities_names':  the readable size is '144' bytes, but 'Reward_Specials[itr]' bytes may be read.	ReMoM	C:\STU\devel\ReMoM\src\Lair.C	645
                */
                strcat(lair_message_box_text, _wizard_abilities_names[Reward_Specials[itr]]);
            }
        }
    }


    lair_item_count = _LAIRS[lair_idx].Item_Count;

    if(lair_item_count > 0)
    {
        amount[0] = -1;
        amount[1] = -1;
        amount[2] = -1;

        for(itr = 0; itr < lair_item_count; itr++)
        {
            amount[itr] = 1;
            strcpy(Item_Type_Names[itr], Get_Item_Type_Name(item_list[itr]));
        }

        for(itr = 0; itr < lair_item_count; itr++)
        {
            if(amount[itr] != -1)
            {
                Add_Comma_Or_And(&List_Count, Total_Rewards, lair_message_box_text);
                if(amount[itr] > 1)
                {
                    SDL_itoa(amount[itr], temp_string, 10);
                    strcat(lair_message_box_text, temp_string);
                    strcat(lair_message_box_text, cnst_EZ_Msg_2);  // " "
                    strcat(lair_message_box_text, Item_Type_Names[itr]);
                    strcat(lair_message_box_text, cnst_Reward_Msg_9);
                }
                else
                {
                    strcpy(temp_string, Item_Type_Names[itr]);
                    strcat(lair_message_box_text, An(temp_string));
                    strcat(lair_message_box_text, cnst_EZ_Msg_2);  // " "
                    strcat(lair_message_box_text, temp_string);
                }
            }
        }
    }


    strcat(lair_message_box_text, cnst_Dot6);

    Notify1(190, window_y, 3, lair_message_box_text, 0, lair_treasure_notify_pict_seg, 2, 8, 0, 0, 0, 2, 0); 

}


// WZD o83p07
void Lair_Loot_Gold_And_Mana(int16_t lair_idx, int16_t player_idx)
{
    Player_Add_Gold(player_idx, _LAIRS[lair_idx].Loot_Gold);
    Player_Add_Mana(player_idx, _LAIRS[lair_idx].Loot_Mana);
}


// WZD o83p08
// drake178: STR_ListSeparator()
void Add_Comma_Or_And(int16_t * size, int16_t count, char * list)
{
    *size += 1;

    if(*size == count)
    {
        if(*size > 1)
        {
            strcat(list, " and ");
        }
    }
    else
    {
        if(*size > 1)
        {
            strcat(list, ", ");
        }
    }

}


// WZD o83p09
// MoO2  Module: DIP-SCRN  An_()  Address: 01:0001DE17
/*
    https://owl.purdue.edu/owl/general_writing/grammar/articles_a_versus_an.html

...a and an are different forms of the same word, the indefinite article that often precedes a noun.
     a is used before a noun that starts with a consonant sound.
    an is used before a noun that starts with a vowel sound.

Maybe, probably, the string optimizer made this look crazy.
In MoO2, it's just "an" and "a" strings, with a check for language set to English.
*/
char * An(char * word)
{
    char * an;
    char * a;
    char * indefinite_article_string;  // DNE in Dasm

    an = cnst_EZ_Indef2;      // "an"
    a = cnst_EZ_Indef1;  // "a"

    switch(word[0])
    {
        case 'A':
        case 'a':
        case 'E':
        case 'e':
        case 'I':
        case 'i':
        case 'O':
        case 'o':
        case 'U':
        case 'u':
        {
            indefinite_article_string = an;
        } break;
        default:
        {
            indefinite_article_string = a;
        } break;
    }

    return indefinite_article_string;
    
}


// WZD o83p10
/*

*/
void Lair_Generate_Treasure(int16_t player_idx, int16_t lair_idx, int16_t items[], int16_t * spell, int16_t * hero, int16_t * hero_slot, int16_t * total_rewards, int16_t * reward_special_count, int16_t reward_specials[])
{
    int16_t troops[MAX_STACK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t troop_count = 0;
    int16_t lair_item_count = 0;
    int16_t Prisoner_Reward = 0;
    int16_t Spell_Or_Special = 0;
    int16_t Mana_Reward = 0;
    int16_t Gold_Reward = 0;
    int16_t itr = 0;


    Gold_Reward = _LAIRS[lair_idx].Loot_Gold;

    Mana_Reward = _LAIRS[lair_idx].Loot_Mana;

    Spell_Or_Special = _LAIRS[lair_idx].Spell_n_Special;

    Prisoner_Reward = Test_Bit_Field(0, &_LAIRS[lair_idx].Misc_Flags);

    lair_item_count = _LAIRS[lair_idx].Item_Count;  // 1-byte, signed


    for(itr = 0; itr < lair_item_count; itr++)
    {
        // ¿ drake178:  ; BUG: parameter mismatch ?
        items[itr] = Make_Item(_LAIRS[lair_idx].Item_Values[itr], _players[player_idx].spellranks, 0);
    }

    *reward_special_count = 0;
    *spell = -1;
    if(Spell_Or_Special > 0)
    {
        switch(Spell_Or_Special)
        {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            {
                *spell = WIZ_RollSpellReward(player_idx, Spell_Or_Special);
                if(*spell < 1)  /* First Spell Index (Earth_to_Mud) */
                {
                    Spell_Or_Special = 0;
                    *spell = -1;
                }
            } break;
            case 5:
            {
                EZ_SpecialTreasure(player_idx, lair_idx, 1, reward_special_count, reward_specials);
                if(*reward_special_count < 1)
                {
                    if(_LAIRS[lair_idx].Item_Count < 3)
                    {
                        _LAIRS[lair_idx].Item_Count += 1;
                        lair_item_count = _LAIRS[lair_idx].Item_Count;
                        items[lair_item_count] = Make_Item(0, &_players[player_idx].spellranks[0], 1200);
                    }
                    else
                    {
                        _LAIRS[lair_idx].Loot_Mana = 1000;
                    }
                }
            } break;
            case 6:
            {
                EZ_SpecialTreasure(player_idx, lair_idx, 2, reward_special_count, reward_specials);
                if(*reward_special_count < 1)
                {
                    if(_LAIRS[lair_idx].Item_Count < 3)
                    {
                        _LAIRS[lair_idx].Item_Count += 1;
                        lair_item_count = _LAIRS[lair_idx].Item_Count;
                        items[lair_item_count] = Make_Item(0, &_players[player_idx].spellranks[0], 1200);
                    }
                    else
                    {
                        _LAIRS[lair_idx].Loot_Mana = 1000;
                    }
                }
            } break;
        }
    }



    *hero = -1;
    *hero_slot = -1;

    if(Prisoner_Reward > 0)
    {
        Player_Army_At_Square(_combat_wx, _combat_wy, _combat_wp, player_idx, &troop_count, &troops[0]);
        if(troop_count >= MAX_STACK)
        {
            Prisoner_Reward = 0;
            *hero_slot = -1;
        }
        else
        {
            *hero = Pick_Random_Hero(player_idx, 1, Prisoner_Reward);
            if(*hero > -1)
            {
                // ; returns the index of the first empty hero slot that the player has, or -1 if they have 6 heroes
                *hero_slot = Hero_Slot_Open(player_idx);
                if(*hero_slot == -1)
                {
                    Prisoner_Reward = 0;
                }
            }
            else
            {
                Prisoner_Reward = 0;
            }
        }
    }


    *total_rewards = 0;
    if(Gold_Reward > 0)  // DEDU  ¿ this is SimTex for (!INVALID && !FALSE) ?  ...positive,non-zero
    {
        *total_rewards += 1;
    }
    if(Mana_Reward > 0)
    {
        *total_rewards += 1;
    }
    if(Spell_Or_Special > 0)
    {
        *total_rewards += 1;
    }
    if(Prisoner_Reward > 0)
    {
        *total_rewards += 1;
    }
    *total_rewards += lair_item_count;
    *total_rewards += *reward_special_count;

}


// WZD o83p11
// drake178: OVL_EZ_Confirm_Dlg();
// OON XREF: Lair_Confirm()
/*

sets notify_pict1_x,y but then sets and uses message_box_x,y
*/
int16_t Lair_Confirm(int16_t UU_lair_confirm_start_x, int16_t UU_lair_confirm_start_y, char * lair_message_box_text, SAMB_ptr lair_confirm_pict, int16_t UU_lair_confirm_pict_x, int16_t UU_lair_confirm_pict_y, int16_t lair_has_guardians, int16_t lair_is_node)
{
    int16_t choice;
    int16_t Label_Ctrl_Index;  // TODO  rename  ¿ other generic mouse click cancel fields ?
    int16_t hotkey_ESC;
    int16_t no_button;
    int16_t yes_button;
    int16_t leave_screen;
    int16_t Box_Height;  // TODO  rename
    int16_t max_para_height;
    int16_t tmp_text_y2;  // _SI_
    int16_t input_field_idx;  // _DI_

    Save_Alias_Colors();

    Set_Font_Colors_15(0, COL_Dialog_Text);

    Copy_On_To_Off_Page();

    Load_Palette_From_Animation(lair_confirm_pict);

    Apply_Palette();

    message_box_text = lair_message_box_text;

    lair_confirm_buttons_flag = lair_has_guardians;

    notify_pict1 = lair_confirm_pict;

    notify_pict1_x = UU_lair_confirm_pict_x;

    notify_pict1_y = UU_lair_confirm_pict_y;

    notify_color_slide = lair_is_node;

    Set_Font_Style(4, 4, 15, ST_NULL);

    Set_Paragraph_Exclusion_Area(0, 0, 57, 45);

    max_para_height = Get_Paragraph_Max_Height(166, message_box_text);

    Box_Height = (34 + max_para_height);

    message_box_x = 68;

    message_box_y = ((SCREEN_HEIGHT - Box_Height) / 2);

    tmp_text_y2 = max_para_height;
    if(tmp_text_y2 < 70)
    {
        tmp_text_y2 = 75;
    }

    // tmp_text_y2 = tmp_text_y2 + (message_box_y + 6);
    // HACK: tmp_text_y2 is coming up short; not sure where; ¿ miscalc in Get_Paragraph_Max_Height() ?
    tmp_text_y2 = tmp_text_y2 + (message_box_y + 6) + 6;


    Clear_Fields();

    yes_button = -1000;
    no_button = -1000;
    Label_Ctrl_Index = -1000;

    if(lair_confirm_buttons_flag == ST_TRUE)
    {

        // yes_button = Add_Button_Field((message_box_x + 100), (tmp_text_y2 + 5), cnst_ZeroString_0, confirmation_button_yes_seg, cnst_HOTKEY_Y_2, -1);
        yes_button = Add_Button_Field((message_box_x + 100), (tmp_text_y2 + 5), cnst_ZeroString_0, confirmation_button_yes_seg, 'Y', -1);

        // no_buttom = Add_Button_Field((message_box_x + 17), (tmp_text_y2 + 5), cnst_ZeroString_0, confirmation_button_no_seg, cnst_HOTKEY_N_3, -1);
        no_button = Add_Button_Field((message_box_x + 17), (tmp_text_y2 + 5), cnst_ZeroString_0, confirmation_button_no_seg, 'N', -1);

        // Label_Ctrl_Index = Add_Hidden_Field(message_box_x, message_box_y, (message_box_x + 184), (tmp_text_y2 + 28), cnst_ZeroString_0, -1);
        Label_Ctrl_Index = Add_Hidden_Field(message_box_x, message_box_y, (message_box_x + 184), (tmp_text_y2 + 28), 0, -1);

    }

    // hotkey_ESC = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, cnst_HOTKEY_EscE, -1);
    hotkey_ESC = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, '\x1B', -1);

    Assign_Auto_Function(Lair_Confirm_Draw, 1);


    choice = ST_TRUE;

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {

        input_field_idx = abs(Get_Input());

        if(
            (input_field_idx == -1) ||
            (input_field_idx == no_button) ||
            (input_field_idx == hotkey_ESC)
        )
        {
            leave_screen = ST_TRUE;
            choice = ST_FALSE;
        }

        if(input_field_idx == yes_button)
        {
            leave_screen = ST_TRUE;
            choice = ST_TRUE;
        }

        Lair_Confirm_Draw();
        PageFlip_FX();
    }

    Deactivate_Auto_Function();
    Clear_Fields();
    Restore_Alias_Colors();
    Reset_Window();
    Reset_Paragraph_Exclusions();

    return choice;
}

// WZD o83p12
// drake178: OVL_DrawEZConfirm();
// OON XREF: Lair_Confirm()
void Lair_Confirm_Draw(void)
{
    int16_t max_para_height;
    int16_t tmp_text_y2; // _SI_

    Set_Font_Style(4, 4, 15, ST_NULL);

    Set_Paragraph_Exclusion_Area(0, 0, 57, 45);

    max_para_height = Get_Paragraph_Max_Height(166, message_box_text);

    Reset_Paragraph_Exclusions();

    Set_Page_Off();

    tmp_text_y2 = max_para_height;

    if(tmp_text_y2 < 70)
    {
        tmp_text_y2 = 75;
    }

    // tmp_text_y2 = tmp_text_y2 + (message_box_y + 6);
    // HACK: tmp_text_y2 is coming up short; not sure where; ¿ miscalc in Get_Paragraph_Max_Height() ?
    tmp_text_y2 = tmp_text_y2 + (message_box_y + 6) + 6;

    Set_Window(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, tmp_text_y2);

    Clipped_Draw(message_box_x, message_box_y, lair_confirm_border_seg);

    Reset_Window();

    FLIC_Draw((message_box_x + 7), (message_box_y + 7), notify_pict1);

    if(lair_confirm_buttons_flag != ST_FALSE)
    {
        FLIC_Draw(message_box_x, tmp_text_y2, lair_bottom_with_button_seg);
    }
    else
    {
        FLIC_Draw(message_box_x, tmp_text_y2, lair_bottom_without_button_seg);
    }

    Set_Font_Colors_15(4, COL_ConfirmShadows);

    Set_Font_Style(4, 15, 15, ST_NULL);

    Set_Font_LF(2);

    Set_Paragraph_Exclusion_Area(message_box_x, message_box_y, (message_box_x + 58), (message_box_y + 55));

    Print_Paragraph((message_box_x + 10), (message_box_y + 11), 166, message_box_text, 0);

    Set_Paragraph_Exclusion_Area(message_box_x, message_box_y, (message_box_x + 57), (message_box_y + 55));

    Print_Paragraph((message_box_x + 9), (message_box_y + 11), 166, message_box_text, 0);

    Set_Alias_Color(18);

    Set_Font_Colors_15(4, COL_Dialog_Text);

    Set_Font_Style(4, 4, 15, ST_NULL);

    Set_Font_LF(2);

    Set_Paragraph_Exclusion_Area(message_box_x, message_box_y, (message_box_x + 57), (message_box_y + 55));

    Print_Paragraph((message_box_x + 9), (message_box_y + 10), 166, message_box_text, 0);

    if(notify_color_slide == ST_TRUE)
    {
        // TODO  VGA_SlideColors(247, 8, notify_color_slide_cycle);
        VGA_SlideColors__STUB(247, 8, notify_color_slide_cycle);
        notify_color_slide_cycle = ((notify_color_slide_cycle + 1) % 8);
    }

}


// WZD o83p13
/*
~ Spell-Picks Treasure

; generates and awards the specified amount of points'
; worth of profile traits for the selected wizard,
; logging them into the return pointers
; contains a number of BUGs for both books and retorts

XREF:
    Lair_Generate_Treasure()
    NX_j_EZ_SpecialTreasure()

e.g.,
    EZ_SpecialTreasure(player_idx, lair_idx, 1, reward_special_count, reward_specials)
    EZ_SpecialTreasure(player_idx, lair_idx, 2, reward_special_count, reward_specials)

Wizard Special Abilities

*/
void EZ_SpecialTreasure(int16_t player_idx, int16_t lair_idx, int16_t points, int16_t * reward_special_count, int16_t reward_specials[])
{
    int16_t EZ_Book_Realm = 0;
    int16_t Tries = 0;
    int16_t Valid_Reward = 0;
    int16_t Finished = 0;
    int16_t DoublePick_Retorts[5] = { 0, 0, 0, 0, 0 };
    int16_t Book_Count = 0;
    int16_t Random_Result = 0;
    int16_t Retort_Count = 0;
    int8_t * wsa_ptr = NULL;
    int16_t Points_Spent = 0;
    int16_t retort_idx = 0;
    int16_t itr = 0;  // _DI_

    // all 5 retorts that cost 2 spell-picks
    DoublePick_Retorts[0] = rtt_Warlord;
    DoublePick_Retorts[1] = rtt_Channeller;
    DoublePick_Retorts[2] = rtt_Divine_Power;
    DoublePick_Retorts[3] = rtt_Famous;
    DoublePick_Retorts[4] = rtt_Infernal_Power;


    *reward_special_count = 0;

    Finished = ST_FALSE;

    Points_Spent = 0;
    while((Points_Spent < points) && (Finished == ST_FALSE))
    {
        Retort_Count = 0;
        wsa_ptr = &_players[player_idx].alchemy;  // DEDU  ¿ taking a pointer *means* it is not array in the struct ?
        for(itr = 0; itr < NUM_WIZARD_SPECIAL_ABILITIES; itr++)
        {
            if(wsa_ptr[itr] == 1)
            {
                Retort_Count++;
            }
        }


        Random_Result = Random(100);


        Book_Count = 0;
        for(itr = 0; itr < 5; itr++)
        {
            Book_Count += _players[player_idx].spellranks[itr];
        }

        if(
            (Retort_Count > 5)
            ||
            (Book_Count > 12)
        )
        {
            Finished = ST_UNDEFINED;
        }


        if(Finished == ST_FALSE)
        {
            if(Retort_Count > 5)
            {
                Random_Result = 0;
            }
            if(Book_Count > 12)
            {
                Random_Result = 100;
            }

            if(Random_Result < 75)
            {
                EZ_Book_Realm = Lair_Magic_Realm(lair_idx);
                Valid_Reward = ST_TRUE;
                if((EZ_Book_Realm == sbr_Life) && (_players[player_idx].spellranks[sbr_Death] > 0))
                {
                    Valid_Reward = ST_FALSE;
                }
                if((EZ_Book_Realm == sbr_Death) && (_players[player_idx].spellranks[sbr_Life] > 0))
                {
                    Valid_Reward = ST_FALSE;
                }
                if(Valid_Reward == ST_TRUE)
                {
                    WIZ_AddSpellRank__WIP(player_idx, EZ_Book_Realm);
                    reward_specials[*reward_special_count] = EZ_Book_Realm;
                    *reward_special_count += 1;
                }
            }
            else
            {
                for(Tries = 0; Tries < 100; Tries++)
                {
                    retort_idx = (Random(NUM_WIZARD_SPECIAL_ABILITIES) - 1);
                    if(wsa_ptr[retort_idx] == 0)
                    {
                        Valid_Reward = ST_FALSE;
                        for(itr = 0; ((itr < 5) && (Valid_Reward == ST_FALSE)); itr++)
                        {
                            if(DoublePick_Retorts[itr]  == retort_idx)
                            {
                                Valid_Reward = ST_TRUE;
                            }
                        }
                        if(retort_idx == rtt_Myrran)
                        {
                            Valid_Reward = ST_FALSE;
                        }
                        if(Valid_Reward == ST_TRUE)
                        {
                            if(
                                (Points_Spent == 0)
                                &&
                                (points == 2)
                            )
                            {
                                wsa_ptr[retort_idx] = 1;
                                Tries = 1000;
                                reward_specials[*reward_special_count] = (100 + retort_idx);
                                *reward_special_count = 1;
                                Finished = -1;
                            }
                        }
                        else
                        {
                            // BUG  ¿ test is wrong or this is macro ?
                            Valid_Reward = ST_TRUE;
                            if(retort_idx == rtt_Myrran)
                            {
                                Valid_Reward = ST_FALSE;
                            }
                            // ¿ drake178: ; BUG: not a single pick reward ?
                            if(
                                (retort_idx == rtt_Infernal_Power)
                                &&
                                (_players[player_idx].spellranks[sbr_Life] > 0)
                            )
                            {
                                Valid_Reward = ST_FALSE;
                            }
                            // ¿ drake178: ; BUG: not a single pick reward ?
                            if(
                                (retort_idx == rtt_Divine_Power)
                                &&
                                (_players[player_idx].spellranks[sbr_Death] > 0)
                            )
                            {
                                Valid_Reward = ST_FALSE;
                            }
                            if(Valid_Reward == ST_TRUE)
                            {
                                wsa_ptr[retort_idx] = 1;
                                reward_specials[*reward_special_count] = (100 + retort_idx);
                                *reward_special_count += 1;
                                // ¿ drake178: ; BUG: not a single pick reward ?
                                if(retort_idx == rtt_Famous)
                                {
                                    _players[player_idx].fame += 10;
                                }
                                Tries = 1000;
                            }
                        }  /* if(Valid_Reward == ST_TRUE) {} else {} */
                    }  /* if(wsa_ptr[retort_idx] == 0) */
                }  /* for(Tries = 0; Tries < 100; Tries++) */
            }  /* if(Random_Result < 75) {} else {} */
        }  /* if(Finished == ST_FALSE) */

    }  /* while((Points_Spent < points) && (Finished == ST_FALSE)) */

}

// WZD o83p14
int16_t Lair_Magic_Realm(int16_t lair_idx)
{
    int16_t magic_realm = 0;  // _SI_

    switch(_LAIRS[lair_idx].type)
    {
        case lt_Tower:
        {
            // DNE
        } break;
        case lt_Chaos_Node:
        {
            magic_realm = sbr_Chaos;
        } break;
        case lt_Nature_Node:
        {
            magic_realm = sbr_Nature;
        } break;
        case lt_Sorcery_Node:
        {
            magic_realm = sbr_Sorcery;
        } break;
        case lt_Cave:
        case lt_Monster_Lair:
        {
            magic_realm = Random(5);
        } break;
        case lt_Dungeon:
        case lt_Keep:
        case lt_Ruins:
        {
            magic_realm = sbr_Death;
        } break;
        case lt_Ancient_Temple:
        case lt_Fallen_Temple:
        {
            magic_realm = sbr_Life;
        } break;

    }

    return magic_realm;
}

