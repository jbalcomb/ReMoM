/*
    Spellbook Screen

    WIZARDS.EXE
        ovr134
    
    SPELLSCR.LBX
*/

#include "MOX/FLIC_Draw.H"
#include "MOX/MOX_DAT.H"  /* _screen_seg */
#include "MOX/MOX_SET.H"  /* magic_set */
#include "MOX/sdl2_Audio.H"
#include "MOX/SOUND.H"

#include "MOM.H"

#include "malloc.h"  // ¿ this is included in MoX_Lib.H, but CLang is complaining ?

#include "SBookScr.H"
#include "Spellbook.H"
#include "MainScr.H"
#include "MainScr_Maps.H"



// WZD dseg:6994                                                 BEGIN:  ovr134 - Initialized Data

// WZD dseg:6994
// REDEF  char hlpentry_lbx_file [] = "hlpentry";

// WZD dseg:699C
// char empty_string__ovr134[] = "";
// WZD dseg:699D 46 00                                           hotkey_F__ovr134 db 'F',0             ; DATA XREF: Spellbook_Screen+205o ...
// WZD dseg:699F 42 00                                           hotkey_B__ovr134 db 'B',0             ; DATA XREF: Spellbook_Screen+232o ...

// WZD dseg:69A1
char _msg_abort_1[] = "Do you wish to abort ";
// WZD dseg:69B6
// cnst_CounterMsg5_2 db 'your ',0         ; DATA XREF: OVL_CounterMessage+10F
char _msg_abort_2[] = "your ";
// WZD dseg:69BC
char _msg_abort_3[] = " spell?";

// WZD dseg:69C4 1B 00                                           hotkey_ESC__ovr134 db 1Bh,0           ; DATA XREF: Spellbook_Screen+75Eo

// WZD dseg:69C6
char message_lbx_file__ovr134[] = "message";

// WZD dseg:69CE
char specfx_lbx_file__ovr134__1of3[] = "Specfx";

// WZD dseg:69D5
char music_lbx_file__ovr134[] = "MUSIC";

// WZD dseg:69DB
char cnst_CounterMsg1_2[] = "Your ";
// WZD dseg:69E1
char cnst_Apostrophe_2[] = "\' ";
// WZD dseg:69E4
char cnst_Possessive_2[] = "\'s ";
// WZD dseg:69E8
char cnst_CounterMsg2_2[] = " spell has caused ";
// WZD dseg:69FB
char cnst_CounterMsg6_2[] = " spell to fizzle.";

// WZD dseg:6A0D
char cmbtfx_lbx_file__ovr134__1of2[] = "cmbtfx";
// WZD dseg:6A14
char specfx_lbx_file__ovr134__2of3[] = "specfx";
// WZD dseg:6A1B
char specfx_lbx_file__ovr134__3of3[] = "SPECFX";
// WZD dseg:6A22
char cmbtfx_lbx_file__ovr134__2of2[] = "CMBTFX";

// WZD dseg:6A29
char soundfx_lbx_file__ovr134__1of2[] = "SOUNDFX";
// WZD dseg:6A31
char newfound_lbx_file__ovr134[] = "NEWSOUND";

// WZD dseg:6A3A
char soundfx_lbx_file__ovr134__2of2[] = "soundfx";

// WZD dseg:6A3A                                                 END:  ovr134 - Initialized Data



/*
    WIZARDS.EXE  ovr134
*/

// WZD o134p01
void Spell_Screen_Load(void)
{
    Allocate_Reduced_Map();
    Set_Page_Off();
    Reset_Map_Draw();
    Main_Screen_Draw();
    Gradient_Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ..., remap_block, Slope, Scale, Seed
    FLIC_Draw(16, 10, _spellbook_small_seg);
    Copy_Off_To_Back();
    Set_Page_On();
    LBX_Load_Data_Static(hlpentry_lbx_file, 0, (SAMB_ptr)_help_entries, 0, 15, 10);  // "Spell Book Help"
}


// WZD o134p02
void Spellbook_Screen_Draw(void)
{
    Copy_Back_To_Off();
    SmlBook_Draw__WIP(16, 10);
}


// WZD o134p03

void Spellbook_Screen(void)
{
    char temp_string[LEN_TEMP_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t spellbook_pages[(2 * 6)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t hotkey_B = 0;
    int16_t hotkey_F = 0;
    int16_t spellbook_page_spell_index = 0;
    int16_t spell_idx = 0;
    int16_t Abort_Spell__YN = 0;
    int16_t hotkey_ESC = 0;
    int16_t y_start = 0;
    int16_t did_select_spell = 0;
    int16_t player_is_casting = 0;
    int16_t itr_page_spell_count = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t itr_spellbook_page_count = 0;  // _SI_
    int16_t itr_spellbook_page_fields = 0;  // _SI_
    int16_t x_start = 0;  // _DI_

    did_select_spell = ST_FALSE;

    OVL_DisableIncmBlink();

    Load_Palette_From_Animation(_spellbook_small_seg);
    Set_Palette_Changes(0, 223);
    Clear_Palette_Changes(224, 255);
    Update_Remap_Gray_Palette();
    Reset_Cycle_Palette_Color();

    Spell_Screen_Load();

    SBK_BuildSpellbook__WIP(slt_Overland, 6);  // Book_Type, Page_Size

    SBK_Dogears = 1;

    player_is_casting = ST_FALSE;
    for(itr_spellbook_page_count = 0; ((itr_spellbook_page_count < m_spellbook_page_count) && (player_is_casting == ST_FALSE)); itr_spellbook_page_count++)
    {
        for(itr_page_spell_count = 0; m_spellbook_pages[itr_spellbook_page_count].count > itr_page_spell_count; itr_page_spell_count++)
        {
            if(player_is_casting == ST_FALSE)
            {
                if(m_spellbook_pages[itr_spellbook_page_count].spell[itr_page_spell_count] == _players[HUMAN_PLAYER_IDX].casting_spell_idx)
                {
                    if((itr_spellbook_page_count % 2) != 1)
                    {
                        SBK_OpenPage = itr_spellbook_page_count;
                    }
                    else
                    {
                        SBK_OpenPage = (itr_spellbook_page_count - 1);
                    }
                    player_is_casting = ST_TRUE;
                }
            }
        }
    }

    if(player_is_casting == ST_FALSE)
    {
        SBK_OpenPage = SBK_SomePageSaveVar;
    }

    Assign_Auto_Function(Spellbook_Screen_Draw, 2);

    Set_Outline_Color(0);

    Set_Font_Style_Shadow_Down(0, 3, 0, 0);

    Clear_Fields();

    Set_Help_List((char *)&_help_entries[0], 15);

    x_start = 16;
    y_start = 12;

    for(itr_spellbook_page_fields = 0; itr_spellbook_page_fields < 6; itr_spellbook_page_fields++)
    {
        spellbook_pages[(itr_spellbook_page_fields + 0)] = Add_Hidden_Field((x_start + 16), (y_start + (itr_spellbook_page_fields * 22) + 17), (x_start + 137), (y_start + (itr_spellbook_page_fields * 22) + 34), 0, ST_UNDEFINED);
    }

    for(itr_spellbook_page_fields = 0; itr_spellbook_page_fields < 6; itr_spellbook_page_fields++)
    {
        spellbook_pages[(itr_spellbook_page_fields + 6)] = Add_Hidden_Field((x_start + 148), (y_start + (itr_spellbook_page_fields * 22) + 17), (x_start + 268), (y_start + (itr_spellbook_page_fields * 22) + 34), 0, ST_UNDEFINED);
    }

    hotkey_ESC = Add_Hidden_Field(x_start + 159, y_start + 154, x_start + 177, y_start + 183, ST_UNDEFINED, ST_UNDEFINED);
    hotkey_F   = Add_Hidden_Field(x_start + 259, y_start +   2, x_start + 272, y_start +  15, 'F', ST_UNDEFINED);
    hotkey_B   = Add_Hidden_Field(x_start +  13, y_start +   2, x_start +  26, y_start +  14, 'B', ST_UNDEFINED);

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {

        input_field_idx = Get_Input();

        // TODO  add help fields

        /*
            Hot-Key ESCAPE
        */
        if(input_field_idx == hotkey_ESC)
        {
            Play_Left_Click__DUPE();
            leave_screen = ST_TRUE;
        }

        /*
            Hot-Key Page Forward
        */
        if(input_field_idx == hotkey_F)
        {
            if((m_spellbook_page_count - 2) > SBK_OpenPage)
            {
                Play_Left_Click__DUPE();
                SmlBook_PageTurn__WIP(1, 0, 0);
                SBK_OpenPage += 2;
            }
        }

        /*
            Hot-Key Page Backward
        */
        if(input_field_idx == hotkey_B)
        {
            if(SBK_OpenPage > 1)
            {
                Play_Left_Click__DUPE();
                SmlBook_PageTurn__WIP(0, 0, 0);
                SBK_OpenPage -= 2;
            }
        }

        /*
            BEGIN:  Left-Click Spellbook Page Spell Fields
        */
        for(itr_spellbook_page_fields = 0; itr_spellbook_page_fields < (2 * 6); itr_spellbook_page_fields++)
        {

            if(spellbook_pages[itr_spellbook_page_fields] == input_field_idx)
            {

                if(itr_spellbook_page_fields < 6)
                {

                    if(m_spellbook_pages[SBK_OpenPage].count > itr_spellbook_page_fields)
                    {

                        Play_Left_Click();

                        spellbook_page_spell_index = itr_spellbook_page_fields;

                        spell_idx = m_spellbook_pages[SBK_OpenPage].spell[itr_spellbook_page_fields];
                        
                        if(spell_idx == _players[HUMAN_PLAYER_IDX].casting_spell_idx)
                        {
                            spell_idx = ST_UNDEFINED;

                            strcpy(GUI_NearMsgString, _msg_abort_1);
                            strcpy(temp_string, spell_data_table[_players[HUMAN_PLAYER_IDX].casting_spell_idx].name);
                            strcat(GUI_NearMsgString, temp_string);
                            strcat(GUI_NearMsgString, _msg_abort_3);
                            Abort_Spell__YN = Confirmation_Box(GUI_NearMsgString);

                            if(Abort_Spell__YN == ST_TRUE)
                            {
                                _players[HUMAN_PLAYER_IDX].casting_cost_remaining = 0;
                                _players[HUMAN_PLAYER_IDX].casting_cost_original = 0;
                                _players[HUMAN_PLAYER_IDX].casting_spell_idx = 0;
                                spell_idx = ST_UNDEFINED;
                                leave_screen = ST_TRUE;
                            }
                        }

                    }

                }
                else  /* (itr_spellbook_page_fields >= 6) */
                {

                    if(m_spellbook_pages[(SBK_OpenPage + 1)].count > (itr_spellbook_page_fields - 6))
                    {

                        Play_Left_Click();

                        spellbook_page_spell_index = (itr_spellbook_page_fields - 6);

                        spell_idx = m_spellbook_pages[(SBK_OpenPage + 1)].spell[(itr_spellbook_page_fields - 6)];
                        
                        if(spell_idx == _players[HUMAN_PLAYER_IDX].casting_spell_idx)
                        {
                            spell_idx = ST_UNDEFINED;

                            strcpy(GUI_NearMsgString, _msg_abort_1);
                            strcpy(temp_string, spell_data_table[_players[HUMAN_PLAYER_IDX].casting_spell_idx].name);
                            strcat(GUI_NearMsgString, temp_string);
                            strcat(GUI_NearMsgString, _msg_abort_3);
                            Abort_Spell__YN = Confirmation_Box(GUI_NearMsgString);

                            if(Abort_Spell__YN == ST_TRUE)
                            {
                                _players[HUMAN_PLAYER_IDX].casting_cost_remaining = 0;
                                _players[HUMAN_PLAYER_IDX].casting_cost_original = 0;
                                _players[HUMAN_PLAYER_IDX].casting_spell_idx = spl_NONE;
                                spell_idx = ST_UNDEFINED;
                                leave_screen = ST_TRUE;
                            }
                        }

                    }

                }

                if(spell_idx > ST_UNDEFINED)
                {

                    if(_players[HUMAN_PLAYER_IDX].casting_spell_idx > spl_NONE)
                    {
                
                        strcpy(GUI_NearMsgString, _msg_abort_1);
                        strcpy(temp_string, spell_data_table[_players[HUMAN_PLAYER_IDX].casting_spell_idx].name);
                        strcat(GUI_NearMsgString, temp_string);
                        strcat(GUI_NearMsgString, _msg_abort_3);
                        Abort_Spell__YN = Confirmation_Box(GUI_NearMsgString);

                        if(Abort_Spell__YN == ST_TRUE)
                        {
                            _players[HUMAN_PLAYER_IDX].casting_cost_remaining -= _players[HUMAN_PLAYER_IDX].casting_cost_original;
                            _players[HUMAN_PLAYER_IDX].casting_cost_original = 0;
                            _players[HUMAN_PLAYER_IDX].casting_spell_idx = spl_NONE;
                        }
                        else
                        {
                            spell_idx = ST_UNDEFINED;
                        }

                    }
                    else
                    {
                        Abort_Spell__YN = ST_TRUE;
                    }

                    // TODO  IDK_Check_Spell_Cast(spell_idx, &Abort_Spell__YN);

                    if(Abort_Spell__YN == ST_TRUE)
                    {
                        leave_screen = ST_TRUE;
                        did_select_spell = ST_TRUE;
                        _players[HUMAN_PLAYER_IDX].casting_spell_idx = spell_idx;
                    }

                }

            }

        }

        /*
            END:  Left-Click Spellbook Page Spell Fields
        */

        Assign_Auto_Function(Spellbook_Screen_Draw, 2);

        if(leave_screen == ST_FALSE)
        {
            Clear_Fields();
            x_start = 16;
            y_start = 12;
            for(itr_spellbook_page_fields = 0; itr_spellbook_page_fields < 6; itr_spellbook_page_fields++)
            {
                spellbook_pages[(0 + itr_spellbook_page_fields)] = Add_Hidden_Field((x_start + 16), (y_start + (itr_spellbook_page_fields * 22) + 17), (x_start + 137), (y_start + (itr_spellbook_page_fields * 22) + 34), 0, ST_UNDEFINED);
            }
            for(itr_spellbook_page_fields = 0; itr_spellbook_page_fields < 6; itr_spellbook_page_fields++)
            {
                spellbook_pages[(6 + itr_spellbook_page_fields)] = Add_Hidden_Field((x_start + 148), (y_start + (itr_spellbook_page_fields * 22) + 17), (x_start + 268), (y_start + (itr_spellbook_page_fields * 22) + 34), 0, ST_UNDEFINED);
            }
            hotkey_ESC = Add_Hidden_Field(x_start + 159, y_start + 154, x_start + 177, y_start + 183, '\x1B', ST_UNDEFINED);
            hotkey_F   = Add_Hidden_Field(x_start + 259, y_start +   2, x_start + 272, y_start +  15, 'F',   ST_UNDEFINED);
            hotkey_B   = Add_Hidden_Field(x_start +  13, y_start +   2, x_start +  26, y_start +  14, 'B',   ST_UNDEFINED);

            Set_Page_Off();
            Spellbook_Screen_Draw();
            PageFlip_FX();
        }

    }  /* END:  while(leave_screen == ST_FALSE) */


    Deactivate_Auto_Function();

    current_screen = scr_Main_Screen;

    SBK_SomePageSaveVar = SBK_OpenPage;

    Clear_Fields();

    OVL_EnableIncmBlink();

    Deactivate_Help_List();

    if(did_select_spell == ST_TRUE)
    {
        WIZ_SetOverlandSpell__WIP(HUMAN_PLAYER_IDX, spell_idx, spellbook_page_spell_index);
    }
    else
    {
        OVL_MosaicFlip__STUB();
    }

}


// WZD o134p04
// drake178: WIZ_SetOverlandSpell()
/*
; sets the selected spell as the current overland cast
; for the specified wizard, processes any pre-cast
; functions (slider, crafting, SoM animation) and, if
; the spell can be cast instantly, executes that too
; returns 1 if the spell was instant, or 0 otherwise
; UNEXPLORED SUBFUNCTIONS INSIDE, RE-EXPLORE!
*/
/*
handles spl_Enchant_Item, spl_Create_Artifact, spl_Spell_Of_Mastery

if "Instant", calls Cast_Spell_Overland__WIP()

Spellbook_Screen() has only set _players[].casting_spell_idx...

¿ handles the 'slider' for variable power spells ?

¿ doesn't *know* it's for the human/current player ?

XREF:
    Spellbook_Screen()
    j_WIZ_SetOverlandSpell__WIP()
        AI_Spell_Select__STUB()

*/
int16_t WIZ_SetOverlandSpell__WIP(int16_t player_idx, int16_t spell_idx, int16_t spellbook_page_spell_index)
{
    int16_t var_8 = 0;
    int16_t item_idx = 0;
    int16_t did_cast_spell__iff_human_player = 0;
    int16_t Mana_This_Turn = 0;

    did_cast_spell__iff_human_player = ST_FALSE;

    _players[player_idx].casting_spell_idx = spell_idx;

    if(_players[player_idx].Skill_Left > _players[player_idx].mana_reserve)
    {
        Mana_This_Turn = _players[player_idx].mana_reserve;
    }
    else
    {
        Mana_This_Turn = _players[player_idx].Skill_Left;
    }

    if(
        (spell_data_table[spell_idx].type < scc_Infusable_Spell)
        ||
        (player_idx != HUMAN_PLAYER_IDX)
    )
    {

        if(spell_data_table[spell_idx].type == scc_Crafting_Spell)
        {

            if(spell_idx == spl_Enchant_Item)
            {

                if(player_idx == HUMAN_PLAYER_IDX)
                {
                    // SPELLY  _players[player_idx].casting_cost_remaining = IDK_CreateArtifact__STUB(0, 0);
                    // _players[player_idx].casting_cost_original = _ITEMS[136].cost;
                    // Allocate_Reduced_Map();
                    item_idx = Make_Item(1, &_players[player_idx].spellranks[0], 1000);
                    _players[player_idx].casting_cost_original = _ITEMS[item_idx].cost;
                    _players[player_idx].casting_cost_remaining = ((_players[player_idx].casting_cost_remaining * var_8) / 100);
                    memcpy(&_ITEMS[(136 - player_idx)], &_ITEMS[item_idx], sizeof(struct s_ITEM));
                    Remove_Item(item_idx);
                }
                else
                {

                    item_idx = Make_Item(1, &_players[player_idx].spellranks[0], 1000);

                    _players[player_idx].casting_cost_original = _ITEMS[item_idx].cost;

                    _players[player_idx].casting_cost_remaining = ((_players[player_idx].casting_cost_remaining * var_8) / 100);

                    memcpy(&_ITEMS[(136 - player_idx)], &_ITEMS[item_idx], sizeof(struct s_ITEM));

                    Remove_Item(item_idx);

                }

            }
            else if(spell_idx == spl_Create_Artifact)
            {

                if(player_idx == HUMAN_PLAYER_IDX)
                {
                    // SPELLY  _players[player_idx].casting_cost_remaining = IDK_CreateArtifact__STUB(0, 1);
                    // _players[player_idx].casting_cost_original = _ITEMS[136].cost;
                    // Allocate_Reduced_Map();
                    item_idx = Make_Item(1, &_players[player_idx].spellranks[0], 30000);
                    _players[player_idx].casting_cost_original = _ITEMS[item_idx].cost;
                    _players[player_idx].casting_cost_remaining = ((_players[player_idx].casting_cost_remaining * var_8) / 100);
                    memcpy(&_ITEMS[(136 - player_idx)], &_ITEMS[item_idx], sizeof(struct s_ITEM));
                    Remove_Item(item_idx);
                }
                else
                {

                    item_idx = Make_Item(1, &_players[player_idx].spellranks[0], 30000);

                    _players[player_idx].casting_cost_original = _ITEMS[item_idx].cost;

                    _players[player_idx].casting_cost_remaining = ((_players[player_idx].casting_cost_remaining * var_8) / 100);

                    memcpy(&_ITEMS[(136 - player_idx)], &_ITEMS[item_idx], sizeof(struct s_ITEM));

                    Remove_Item(item_idx);

                }

            }
            // ¿ no else {} ?

            _players[player_idx].casting_cost_remaining = 0;

        }
        else
        {

            if(spell_idx == spl_Spell_Of_Mastery)
            {
                SoM_Started(player_idx);
                Change_Relations_For_Enchantments(player_idx, spl_Spell_Of_Mastery, 1);
            }

            _players[player_idx].casting_cost_original = spell_data_table[spell_idx].casting_cost;

            _players[player_idx].casting_cost_remaining  = Casting_Cost(player_idx, spell_idx, ST_FALSE);

            if(
                (player_idx != HUMAN_PLAYER_IDX)
                &&
                (spell_data_table[spell_idx].type >= scc_Infusable_Spell)
            )
            {

                _players[player_idx].casting_cost_remaining *= 3;

                _players[player_idx].casting_cost_original *= 3;

            }

            if(_players[player_idx].casting_cost_remaining > _players[player_idx].casting_cost_original)
            {
                
                _players[player_idx].casting_cost_original = _players[player_idx].casting_cost_remaining;

            }

        }

    }
    else  /* ((spell_data_table[spell_idx].type >= scc_Infusable_Spell) && (player_idx == HUMAN_PLAYER_IDX)) */
    {

        _players[player_idx].casting_cost_remaining = Casting_Cost(HUMAN_PLAYER_IDX, spell_idx, ST_FALSE);

        // SPELLY  SBK_SpellSlider(spell_idx, spellbook_page_spell_index);

    }

    if(_players[player_idx].casting_spell_idx == spl_NONE)
    {
        _players[player_idx].casting_cost_original = 0;
    }
    else
    {

        if(player_idx == HUMAN_PLAYER_IDX)
        {

            // ((x) < (y) ? (x) : (y))
            // if(_players[player_idx].casting_cost_remaining < Mana_This_Turn)
            // if(Mana_This_Turn >= _players[player_idx].casting_cost_remaining)
            // Mana_This_Turn = ((_players[player_idx].casting_cost_remaining) < (Mana_This_Turn) ? (_players[player_idx].casting_cost_remaining) : (Mana_This_Turn));
            if(_players[player_idx].casting_cost_remaining < Mana_This_Turn)
            {

                Mana_This_Turn = _players[player_idx].casting_cost_remaining;

            }

            SETMIN(Mana_This_Turn, 0);

            if(_players[player_idx].casting_cost_remaining <= Mana_This_Turn)
            {

                _players[player_idx].Skill_Left -= Mana_This_Turn;

                _players[player_idx].mana_reserve -= Mana_This_Turn;

                // ; BUG  already tested, only true in this path
                if(player_idx == HUMAN_PLAYER_IDX)
                {
                    OVL_MosaicFlip__STUB();
                }

                did_cast_spell__iff_human_player = ST_TRUE;

                Cast_Spell_Overland__WIP(player_idx);

            }

        }

    }

    if(spell_idx > spl_NONE)
    {
        // === { scc_Summoning, scc_Unit_Enchantment, City_Enchantment, scc_City_Curse, scc_Fixed_Dmg_Spell, scc_Special_Spell, scc_Target_Wiz_Spell, scc_Global_Enchantment, Battlefield_Spell }
        // DEDU  But, why?
        if(spell_data_table[spell_idx].type < scc_Crafting_Spell)
        {

            if(player_idx == HUMAN_PLAYER_IDX)
            {
                OVL_MosaicFlip__STUB();
            }
            
        }

    }

    return did_cast_spell__iff_human_player;

}

// WZD o134p05
// sub_B9837()

// WZD o134p06
/*
does a full draw of the main screen on the offscreen video page
and flips the current page to the offscreen video page
    PageFlip_FX() |-> Apply_Palette(); Toggle_Pages();

XREF:
    j_Full_Draw_Main_Screen()
        ...(58)
        (23) G_OVL_Cast()
        Cast_BlackWind(), Cast_CallOfTheVoid(), Cast_ChangeTerain(), Cast_ChaosChannels(), Cast_Corruption(), Cast_FloatingIsland(), Cast_Incarnation(), Cast_Lycantrophy(), Cast_NaturesCures(), Cast_RaiseVolcano(), Cast_Resurrection(), Cast_Stasis(), Cast_SummonHero(), Cast_WarpNode(), Cast_WordOfRecall()
        ...
        Chancellor_Screen()
        City_Built_Building_Message()
        Combat__STUB()
        Show_Event_Message()
        Hero_LevelUp_Popup()
        Hire_Hero_Popup()
        IDK_SummonAnim()
        Player_Process_Item_Pool()
        WIZ_Banishment()
        WIZ_ConquestSpells()

*/
void Full_Draw_Main_Screen(void)
{
    Set_Page_Off();
    Reset_Map_Draw();
    Main_Screen_Draw();
    PageFlip_FX();
}


// WZD o134p07
void OVL_MosaicFlip__STUB(void)
{
    Set_Page_Off();
    Reset_Map_Draw();
    Main_Screen_Draw();
    VGA_MosaicFlip__STUB();
}

// WZD o134p08
// drake178: OVL_CounterMessage()
/*
; displays the overland counter message for having
; successfully fizzled a spell as a type 1 red warning
; dialog, or does nothing if the human player is not
; involved in the action
*/
/*

"FIZZLE"
"Spell Failure Theme"

*/
void Fizzle_Notification(int16_t player_idx, int16_t counter_player_idx, int16_t spell_idx, char * counter_spell_name)
{
    char temp_string[LEN_TEMP_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    SAMB_ptr fizzle_seg = 0;
    int16_t Name_Length = 0;

    if(
        (player_idx != HUMAN_PLAYER_IDX)
        &&
        ((counter_player_idx != HUMAN_PLAYER_IDX))
    )
    {
        return;
    }

    Allocate_Reduced_Map();

    Set_Page_Off();

    Main_Screen_Draw();

    PageFlip_FX();

    Copy_On_To_Off_Page();

    AI_Eval_After_Spell = ST_TRUE;

    Stop_All_Sounds__STUB();

    Mark_Block(_screen_seg);

    // SPECFX.LBX, 050  "FIZZLE"    ""
    fizzle_seg = LBX_Reload_Next(specfx_lbx_file__ovr134__1of3, 50, _screen_seg);

    // MUSIC.LBX, 017 "MOM53 XM"    "Spell Failure Theme"
    SND_Spell_Music = LBX_Reload_Next(music_lbx_file__ovr134, MUSIC_Spell_Failure, _screen_seg);
    SND_Spell_Music_size = lbxload_entry_length;  // DNE in Dasm

    if(magic_set.background_music == ST_TRUE)
    {

        // DOMSDOS  Play_Sound__WIP(SND_Spell_Music);
        sdl2_Play_Sound__WIP(SND_Spell_Music, SND_Spell_Music_size);

    }

    if(counter_player_idx == HUMAN_PLAYER_IDX)
    {

        strcpy(GUI_NearMsgString, cnst_CounterMsg1_2);

    }
    else
    {

        strcpy(GUI_NearMsgString, _players[counter_player_idx].name);

        Name_Length = strlen(_players[counter_player_idx].name);

        if(_players[counter_player_idx].name[(Name_Length - 1)] == 's')
        {
            strcat(GUI_NearMsgString, cnst_Apostrophe_2);
        }
        else
        {
            strcat(GUI_NearMsgString, cnst_Possessive_2);
        }

    }

    strcat(GUI_NearMsgString, counter_spell_name);

    strcat(GUI_NearMsgString, cnst_CounterMsg2_2);

    if(player_idx == HUMAN_PLAYER_IDX)
    {

        strcat(GUI_NearMsgString, _msg_abort_2);

    }
    else
    {


        strcpy(GUI_NearMsgString, _players[player_idx].name);

        Name_Length = strlen(_players[player_idx].name);

        if(_players[player_idx].name[(Name_Length - 1)] == 's')
        {
            strcat(GUI_NearMsgString, cnst_Apostrophe_2);
        }
        else
        {
            strcat(GUI_NearMsgString, cnst_Possessive_2);
        }

    }

    _fstrcpy(temp_string, spell_data_table[spell_idx].name);

    strcat(GUI_NearMsgString, temp_string);

    strcat(GUI_NearMsgString, cnst_CounterMsg6_2);

    Copy_On_To_Off_Page();

    PageFlip_FX();

    Notify2(160, 80, 1, GUI_NearMsgString, 0, fizzle_seg, ST_UNDEFINED, 7, 0, 0, 0, 1, 0);

    Allocate_Reduced_Map();

    Set_Page_Off();

    Main_Screen_Draw();

    PageFlip_FX();

    // DOMSDOS  Stop_All_Sounds__STUB();

    // DOMSDOS  Play_Background_Music__STUB();
    sdl2_Play_Background_Music__WIP();

    Release_Block(_screen_seg);

}


// WZD o134p09
// drake178: sub_B9D14()
// IDK_Spell_Cityscape_1__Load_Music
/*
OON XREF  IDK_Spell_Cityscape_1()
*/

// WZD o134p10
// drake178: GAME_LoadSpellAnim()
/*
; loads and appends the animation corresponding to the
; selected spell into the LBX_Sandbox_Segment after
; reallocating the battle figure space, assigning its
; segment pointer to the IMG_GAME_SpellAnim@ global
; returns 1 for Black Wind, Earth to Mud, and spells
; loading the generic realm effects; or 0 otherwise
; (indicates the draw location for the animation)
; CAN'T LOAD MULTIPLE ANIMATIONS ONE AFTER THE OTHER!
*/
int16_t Spell_Animation_Load_Graphics__WIP(int16_t spell_idx)
{
    int16_t l_spell_idx = 0;
    int16_t return_value = 0;  // _DI_

    Allocate_Next_Block(_screen_seg, 1010);  // 1,010 PR  16,160 B

    return_value = ST_FALSE;

    l_spell_idx = spell_idx;

    switch(l_spell_idx)
    {
        case spl_Earth_To_Mud:
        {
            spell_animation_seg = LBX_Reload_Next(cmbtfx_lbx_file__ovr134__1of2, 14, _screen_seg);
            return_value = ST_TRUE;
        } break;
        case spl_Web:
        {
            __debugbreak();
        } break;
        case spl_Cracks_Call:
        {
            __debugbreak();
        } break;
        case spl_Ice_Bolt:
        {
            __debugbreak();
        } break;
        case spl_Change_Terrain:
        {
            spell_animation_seg = LBX_Reload_Next(specfx_lbx_file__ovr134__2of3, 8, _screen_seg);
        } break;
        case spl_Petrify:
        {
            __debugbreak();
        } break;
        case spl_Ice_Storm:
        {
            __debugbreak();
        } break;
        case spl_Call_Lightning:
        {
            spell_animation_seg = LBX_Reload_Next(cmbtfx_lbx_file__ovr134__2of2, 24, _screen_seg);
        } break;
        case spl_Dispel_Magic_True:
        {
            __debugbreak();
        } break;
        case spl_Confusion:
        {
            __debugbreak();
        } break;
        case spl_Psionic_Blast:
        {
            __debugbreak();
        } break;
        case spl_Disenchant_True:
        {
            __debugbreak();
        } break;
        case spl_Vertigo:
        {
            __debugbreak();
        } break;
        case spl_Spell_Blast:
        {
            __debugbreak();
        } break;
        case spl_Disjunction_True:
        {
            __debugbreak();
        } break;
        case spl_Banish:
        {
            __debugbreak();
        } break;
        case spl_Mind_Storm:
        {
            __debugbreak();
        } break;
        case spl_Stasis:
        {
            __debugbreak();
        } break;
        case spl_Spell_Binding:
        {
            __debugbreak();
        } break;
        case spl_Warp_Wood:
        {
            __debugbreak();
        } break;
        case spl_Disrupt:
        {
            __debugbreak();
        } break;
        case spl_Fire_Bolt:
        {
            __debugbreak();
        } break;
        case spl_Corruption:
        {
            __debugbreak();
        } break;
        case spl_Fireball:      /* 0x60  96*/
        {
            spell_animation_seg = LBX_Reload_Next(cmbtfx_lbx_file__ovr134__1of2, 23, _screen_seg);
        } break;
        case spl_Raise_Volcano: /* 0x62  98*/
        {
            spell_animation_seg = LBX_Reload_Next(specfx_lbx_file__ovr134__3of3, 11, _screen_seg);
        } break;
        case spl_Warp_Lightning:
        {
            __debugbreak();
        } break;
        case spl_Doom_Bolt:
        {
            __debugbreak();
        } break;
        case spl_Magic_Vortex:
        {
            __debugbreak();
        } break;
        case spl_Fire_Storm:
        {
            __debugbreak();
        } break;
        case spl_Flame_Strike:
        {
            __debugbreak();
        } break;
        case spl_Disintegrate:
        {
            __debugbreak();
        } break;
        case spl_Call_The_Void:
        {
            __debugbreak();
        } break;
        case spl_Star_Fires:
        {
            __debugbreak();
        } break;
        case spl_Dispel_Evil:
        {
            __debugbreak();
        } break;
        case spl_Life_Drain:
        {
            __debugbreak();
        } break;
        case spl_Drain_Power:
        {
            __debugbreak();
        } break;
        case spl_Possession:
        {
            __debugbreak();
        } break;
        case spl_Subversion:
        {
            __debugbreak();
        } break;
        case spl_Warp_Node:
        {
            __debugbreak();
        } break;
        case spl_Black_Wind:
        {
            __debugbreak();
        } break;
        case spl_Cruel_Unminding:
        {
            __debugbreak();
        } break;
        case spl_Word_Of_Death:
        {
            __debugbreak();
        } break;
        case spl_Death_Spell:
        {
            __debugbreak();
        } break;
        case spl_Dispel_Magic:
        {
            __debugbreak();
        } break;
        case spl_Disenchant_Area:
        {
            __debugbreak();
        } break;
        case spl_Disjunction:
        {
            __debugbreak();
        } break;

        default:
        {
            // __debugbreak();
        } break;
    }

    if(spell_data_table[spell_idx].type == scc_Summoning)
    {
        if (spell_idx == spl_Floating_Island)
        {
            spell_animation_seg = LBX_Reload_Next(specfx_lbx_file__ovr134__2of3, spell_data_table[spell_idx].magic_realm, _screen_seg);
            return_value = ST_TRUE;
        }
        else if(
            (spell_idx == spl_Raise_Dead)
            ||
            (spell_idx == spl_Animate_Dead)
        )
        {
            spell_animation_seg = LBX_Reload_Next(cmbtfx_lbx_file__ovr134__1of2, 22, _screen_seg);
            return_value = ST_FALSE;
        }
        else
        {
            spell_animation_seg = LBX_Reload_Next(specfx_lbx_file__ovr134__2of3, spell_data_table[spell_idx].magic_realm, _screen_seg);
            return_value = ST_TRUE;
        }
    }
    else
    {
        __debugbreak();  // FTLOGAATIH!?!?!
    }

    return return_value;

}

// WZD o134p11
// drake178: SPL_LoadSoundEffect()
/*
; appends the spell's sound effect into the sandbox,
; from SOUNDFX.LBX if the index value is positive, or
; NEWSOUND.LBX if negative; sets the resource pointer
; into SND_SpellCast@ if successful
*/
/*

*/
void Spell_Animation_Load_Sound_Effect__WIP(int16_t spell_idx)
{
    if(magic_set.sound_effects == ST_TRUE)
    {
        if(spell_data_table[spell_idx].Sound > ST_UNDEFINED)
        {
            SND_SpellCast = LBX_Reload_Next(soundfx_lbx_file__ovr134__1of2, spell_data_table[spell_idx].Sound, _screen_seg);
            SND_SpellCast_size = lbxload_entry_length;
        }
        else
        {
            SND_SpellCast = LBX_Reload_Next(newfound_lbx_file__ovr134, abs(spell_data_table[spell_idx].Sound), _screen_seg);
            SND_SpellCast_size = lbxload_entry_length;
        }
    }
    else
    {
        SND_SpellCast = ST_UNDEFINED;
    }
}


// WZD o134p12
// drake178: sub_BA1AF()
/*
*/
/*
DEDU  ¿ something specific to spell animations that have a target map square ?

Where is the load for SND_SpellCast this uses?
    Spells132.C  Spell_Animation_Load_Sound_Effect__WIP
        |-> SBookScr.C  Spell_Animation_Load_Sound_Effect__WIP()
*/
void Spell_Animation_Screen__WIP(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t map_p = 0;
    int16_t map_y = 0;
    int16_t map_x = 0;
    SAMB_ptr sound_buffer = NULL;
    int16_t Y_To_Top = 0;
    int16_t X_To_Left = 0;
    int16_t itr = 0;  // _SI_
    int16_t frame_count = 0;  // _DI_
    int16_t sound_buffer_size = 0;  // DNE in Dasm

    X_To_Left = wx;

    Y_To_Top = wy;

    sound_buffer = LBX_Reload_Next(soundfx_lbx_file__ovr134__2of2, SFX_Silence, _screen_seg);
    sound_buffer_size = lbxload_entry_length;

    frame_count = FLIC_Get_FrameCount(spell_animation_seg);

    if(frame_count < 12)
    {
        frame_count = 12;
    }

    map_x = _map_x;

    map_y = _map_y;

    map_p = _map_plane;

    OVL_BringIntoView(&_map_x, &_map_y, wx, wy, wp);

    _map_plane = wp;

    World_To_Screen(_map_x, _map_y, &X_To_Left, &Y_To_Top);

    if(SND_SpellCast != ST_UNDEFINED)
    {
        // DOMSDOS  Play_Sound__STUB(SND_SpellCast);
        sdl2_Play_Sound__WIP(SND_SpellCast, SND_SpellCast_size);
    }

    for(itr = 0; (frame_count + 3) > itr; itr++)
    {
        Mark_Time();
        Main_Screen_Draw();
        if(itr >= 3)
        {
            Clipped_Draw((X_To_Left - 20), (Y_To_Top - 18), spell_animation_seg);
        }
        PageFlip_FX();
        Release_Time(2);
    }

    Mark_Time();

    Release_Time(5);

    // DOMSDOS  Play_Sound__STUB(sound_buffer);
    sdl2_Play_Sound__WIP(sound_buffer, sound_buffer_size);  // SFX_Silence

    _map_x = map_x;

    _map_y = map_y;

    _map_plane = map_p;

}
