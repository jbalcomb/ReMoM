/*
    WIZARDS.EXE
        ovr136
        ovr138

MoO2
    ¿ DNE // N/A ?
    ~ Science Room?
*/

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/GENDRAW.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/MOX_BASE.h"
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOX_DAT.h"  /* _screen_seg */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_SET.h"  /* magic_set */
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/Timer.h"
#include "../../MoX/src/random.h"

#include "Combat.h"
#include "MagicScr.h"
#include "MainScr.h"
#include "NEXTTURN.h"
#include "OverSpel.h"
#include "SBookScr.h"
#include "Spellbook.h"
#include "UNITTYPE.h"

#include <stdlib.h>
#include <string.h>

#include <SDL_stdinc.h>

#include "SPLMASTR.h"



// GENDRAW.C
// WZD dseg:CB12
extern SAMB_ptr selectbk_bottom_arrow_seg;
// WZD dseg:CB14
extern SAMB_ptr selectbk_top_arrow_seg;

// MagicScr.C
// WZD dseg:958E
extern SAMB_ptr lilwiz_gem_segs[5];
// WZD dseg:95C0
extern SAMB_ptr grey_gem_seg;
// WZD dseg:95C2
extern SAMB_ptr broken_grey_gem_seg;
// WZD dseg:C238
extern int16_t ovl_ench_cnt; // overland_enchantment_count
// WZD dseg:C23A
extern int8_t * ovl_ench_list_players;
// WZD dseg:C23C
// SAMB_ptr ovl_ench_list_spells;
extern int16_t * ovl_ench_list_spells;



// Spellbook.C
// WZD dseg:C968
extern SAMB_ptr wizlab_wizard_seg;
// WZD dseg:C96A
extern SAMB_ptr wizlab_familiar_seg;
// WZD dseg:C96C
extern SAMB_ptr wizlab_blue_column_seg;

// Spells137.C
// WZD dseg:CA50
extern int16_t _temp_sint_1;
// WZD dseg:CA54
extern SAMB_ptr IDK_wizard_id_thing_seg;





// WZD dseg:6B4F                                                 BEGIN:  ovr136 - Initialized Data

// WZD dseg:6B4A
char aThe_0[] = " the ";

// WZD dseg:6B4F
char str_empty_string__ovr136[] = "";

// WZD dseg:6B50
char aSelectAHeroToR[] = "Select a hero to Ressurect";

// WZD dseg:6B6B
char spellscr_lbx_file__ovr136[] = "SPELLSCR";

// WZD dseg:6B74
char str_PLUS_SIGN__ovr136[] = "+";

// WZD dseg:6B76
char str_AdditionalPower__ovr136[] = "Additional Power:";

// WZD dseg:6B88
char str_hotkey_O__ovr136[] = "O";

// WZD dseg:6B8A
char lilwiz_lbx_file__ovr136[] = "Lilwiz";
// WZD dseg:6B91
char magic_lbx_file__ovr136[] = "MAGIC";
// WZD dseg:6B97
char resource_lbx_file__ovr136[] = "RESOURCE";
// WZD dseg:6BA0
char aSelectASpellTo[] = "Select a spell to ";
// WZD dseg:6BB3
char aDisjunct_[] = "disjunct.";
// WZD dseg:6BBD
char aBind_[] = "bind.";
// WZD dseg:6BC3
char aSpellSuccesful[] = "Spell succesfully dispelled.";
// WZD dseg:6BE0
char aSpellSuccesf_0[] = "Spell succesfully bound.";
// WZD dseg:6BF9
char aDisjunctionUns[] = "Disjunction unsuccesful";
// WZD dseg:6C11
char aSpellBindingUn[] = "Spell Binding unsuccesful";
// WZD dseg:6C2B
char aThereAreNoGlob[] = "There are no global spells to ";

// WZD dseg:6C2B                                                 END:  ovr136 - Initialized Data



// WZD dseg:6DCA                                                 BEGIN:  ovr138 - Initialized Data

// WZD dseg:6DCA
char spellose_lbx_file__ovr138[] = "spellose";
// WZD dseg:6DD3
char strHasCastThe[] = " has cast the";
// WZD dseg:6DE1
char strSpellOfMastery[] = "Spell of Mastery";
// WZD dseg:6DF2
char music_lbx_file__ovr138[] = "music";
// WZD dseg:6DF8
char cnst_MAGIC_EXE_File2[] = "MAGIC.EXE";
// WZD dseg:6E02
char cnst_MAGICEXE_arg0_2[] = "JENNY";
// WZD dseg:6E08
char strHasStartedCastingThe[] = " has started casting the";
// WZD dseg:6E21
char spellscr_lbx_file__ovr138[] = "spellscr";
// WZD dseg:6E2A
char str_hotkey_ESC__ovr138[] = "\x1B";
// WZD dseg:6E2C
char wizlab_lbx_file__ovr138[] = "wizlab";
// WZD dseg:6E33
char splmastr_lbx_file__ovr138[] = "splmastr";
// WZD dseg:6E3C
char cmbtfx_lbx_file__ovr138[] = "cmbtfx";
// WZD dseg:6E43
char monster_lbx_file__ovr138[] = "Monster";

// WZD dseg:6E4B 00                                              align 2

// WZD dseg:6E4B                                                 END:  ovr138 - Initialized Data



// WZD dseg:C9EE                                                 ¿ END:  ovr127 - Uninitialized Data ?
// WZD dseg:C9EE
// WZD dseg:C9F0 00 00                                           AI_Eval_After_Spell dw 0                ; DATA XREF: UNIT_ConvSpellATK+Bw ...
// WZD dseg:C9F2 00                                              db    0
// WZD dseg:C9F3 00                                              db    0

// WZD dseg:C9F4
SAMB_ptr word_43494;

// WZD dseg:C9F6
int16_t cast_spell_of_mastery_player_idx;

// WZD dseg:C9F8 00                                              db    0
// WZD dseg:C9F9 00                                              db    0

// WZD dseg:C9FA                                                 BEGIN: ovr136 - Uninitialized Data



// WZD dseg:C9FA                                                 BEGIN: ovr136 - Uninitialized Data

// WZD dseg:C9FA
/*
count of elements in the player's subset of the ovl_ench_list_spells array
multiple definition - MagicScr.c and SPLMASTR.c
*/
int16_t ovl_ench_list_cnt2[NUM_PLAYERS] = { 0, 0, 0, 0, 0, 0 };

// WZD dseg:CA06
// int16_t IDK_list_count_end[(NUM_PLAYERS - 1)];
/*
pointer to first element of the player's subset of the ovl_ench_list_spells array
*/
int16_t * ovl_ench_list_ptr[(NUM_PLAYERS - 1)] = { 0, 0, 0, 0, 0 };

// WZD dseg:CA10
// drake178: is this really necessary?
// MOM_Data  int16_t CMB_SliderLimit;

// WZD dseg:CA12
/*
; holds the spell strength during sliders
; holds the anim stage during global cast anims
*/
// MOM_Data  int16_t GAME_MP_SpellVar_1;

// WZD dseg:CA14
/*
; steps 0 to 7 for sliders
*/
// MOM_Data  int16_t _osc_anim_ctr;

// WZD dseg:CA16
// MOM_Data  int16_t SBK_Spell_Index;

// WZD dseg:CA18
// MOM_Data  int16_t SBK_SliderState;

// WZD dseg:CA1A
/*
index of first/current element in the player's subset of the ovl_ench_list_spells array
*/
int16_t ovl_ench_list_fst[(NUM_PLAYERS - 1)] = { 0, 0, 0, 0, 0};

// WZD dseg:CA24
// MOM_Data  SAMB_ptr word_434C4;

// WZD dseg:CA26 00 00                                           IMG_SBK_SliderBG@ dw 0                  ; DATA XREF: SBK_LoadSpellSlider+38w ...
// WZD dseg:CA28 00 00                                           GAME_MP_SpellVar_2 dw 0                 ; DATA XREF: CMB_SliderRedraw+3Fr ...
// WZD dseg:CA28                                                                                         ; holds the caster ID during combat sliders
// WZD dseg:CA28                                                                                         ; holds the mirror reveal mask during global cast anims

// WZD dseg:CA2A
SAMB_ptr IMG_OVL_TrgtWizCncl;

// WZD dseg:CA2C 00 00                                           _temp_sint_4 dw 0        ; DATA XREF: Cast_Spell_Disjuncts_Draw__WIP+44r ...

// WZD dseg:CA2E
/*
only used by 'Cast Spell Disjunct'
*/
SAMB_ptr spellscr_oversbut_seg;


// WZD dseg:CA30 00 00                                           IMG_SBK_SliderOK@ dw 0                  ; DATA XREF: SBK_LoadSpellSlider+7Dw ...

// WZD dseg:CA32
SAMB_ptr IMG_OVL_TargetWizBG;

// WZD dseg:CA34 00 00                                           IMG_SBK_Anims@ dw 0                     ; DATA XREF: CMB_ShowSpellbook+30w ...
// WZD dseg:CA36 00 00                                           IMG_SBK_PageText@ dw 0                  ; DATA XREF: Learn_Spell_Animation+34Bw ...

// WZD dseg:CA36                                                 END: ovr136 - Uninitialized Data



// WZD dseg:CA56                                                 BEGIN: ovr138 - Uninitialized Data

// WZD dseg:CA56
// drake178: GAME_SoM_Cast_By
/*
; HoF will skip turn count points if this is not -1 or
; the index of the human player, and SoM if it is not
; the human player
*/
int16_t GAME_SoM_Cast_By;

// WZD dseg:CA58
SAMB_ptr spellose_wizard_sphere_seg;
// WZD dseg:CA5A
SAMB_ptr spellose_sphere_seg;

// WZD dseg:CA5A                                                 END: ovr138 - Uninitialized Data



/*
    WIZARDS.EXE  ovr136
*/

// WZD o136p01
// Select_Hero_To_Ressurect()
int16_t Select_Hero_To_Ressurect(int16_t hero_count, int16_t hero_list[])
{
    char * selection_list[31] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t multi_page = 0;
    int16_t selection_count = 0;
    int16_t selection_idx = 0;
    int16_t itr = 0;  // _SI_
    char * strings_seg = 0;

    strings_seg = (char *)Near_Allocate_First(1240);  // 1240 / 40 = 31

    for(itr = 0; itr < hero_count; itr++)
    {

        _fstrcpy(&strings_seg[(itr * 40)], hero_names_table[hero_list[itr]].name);

        strcat(&strings_seg[(itr * 40)], aThe_0);

        strcat(&strings_seg[itr * 40], *_unit_type_table[hero_list[itr]].name);

        selection_list[itr] = &strings_seg[(itr * 40)];

    }

    strcpy(&strings_seg[(itr * 40)], str_empty_string__ovr136);

    selection_list[itr] = &strings_seg[(itr * 40)];

    if(hero_count > NUM_HERO_SLOTS)
    {

        selection_count = NUM_HERO_SLOTS;

        multi_page = ST_TRUE;

    }
    else
    {

        selection_count = hero_count;

        multi_page = ST_FALSE;

    }

    selection_idx = Selection_Box(selection_count, &selection_list[0], multi_page, aSelectAHeroToR);  // "Select a hero to Ressurect"

    Clear_Fields();

    Allocate_Reduced_Map();

    Set_Page_Off();

    Main_Screen_Draw();

    PageFlip_FX();

    Assign_Auto_Function(Main_Screen_Draw, 2);

    return hero_list[selection_idx];

}


// WZD o136p02
/*
; reallocates the minimap, marks the sandbox, then
; loads the spell slider images into it, finally
; resetting the blink color state
*/
/*

*/
void Spellbook_Mana_Adder_Load(void)
{

    Allocate_Reduced_Map();

    Mark_Block(_screen_seg);

    IMG_SBK_PageText = Allocate_Next_Block(_screen_seg, 40);

    // SPELLSCR.LBX, 005  "XTRAMANA"    "background"
    IMG_SBK_SliderBG = LBX_Reload_Next(spellscr_lbx_file__ovr136, 5, _screen_seg);

    // SPELLSCR.LBX, 003  "XTRAMANA"    "star"
    IMG_SBK_SliderDot = LBX_Reload_Next(spellscr_lbx_file__ovr136, 3, _screen_seg);

    // SPELLSCR.LBX, 004  "XTRAMANA"    "arrow bar"
    IMG_SBK_SliderBar = LBX_Reload_Next(spellscr_lbx_file__ovr136, 4, _screen_seg);

    // SPELLSCR.LBX, 042  "XTRABUTT"    "var ok button"
    xtramana_ok_button_seg = LBX_Reload_Next(spellscr_lbx_file__ovr136, 42, _screen_seg);

    Reset_Cycle_Palette_Color();

}


// WZD o136p03
/*
; redraws the screen from the 3rd VGA frame, then the
; overland spellbook pages over it, and finally the
; active infusion slider control, while also converting
; the the slider value into total spell cost and saving
; it to the wizard record
; BUG: ignores Evil Omens when calculating the slider
; values
*/
/*

*/
void Spellbook_Mana_Adder_Draw(void)
{
    char buffer[LEN_TEMP_BUFFER] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char string[LEN_TEMP_BUFFER] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t x_start = 0;  // _SI_
    int16_t y_start = 0;  // _DI_

    if(_temp_sint_1 > 5)
    {
        x_start = 5;
    }
    else
    {
        x_start = 165;
    }

    y_start = (21 + ((_temp_sint_1 % 6) * 22));  // center the popup box on the selected spell, vertically

    _osc_anim_ctr = ((_osc_anim_ctr + 1) % 8);

    Copy_Back_To_Off();

    SmlBook_Draw__WIP(16, 10);

    FLIC_Draw((x_start - 4), (y_start + 1), IMG_SBK_SliderBG);

    GAME_MP_SpellVar_1 = (((_xtra_mana_pos - 3) * spell_data_table[SBK_Spell_Index].casting_cost) / 100);  // extra mana amount

    strcpy(string, str_PLUS_SIGN__ovr136);

    SDL_itoa(GAME_MP_SpellVar_1, buffer, 10);  // extra mana amount

    strcat(string, buffer);  // extra mana amount

    Set_Outline_Color(246);
    Set_Font_Style_Shadow_Up(4, 4, 4, 4);
    Set_Alias_Color(6);
    Print((x_start + 4), (y_start + 8), str_AdditionalPower__ovr136);  // "Additional Power:"
    Print_Right((x_start + 144), (y_start + 8), string);  // extra mana amount

    Set_Outline_Color(251);
    Set_Font_Style_Shadow_Down(4, 4, 4, 4);
    Set_Alias_Color(6);
    Print((x_start + 4), (y_start + 8), str_AdditionalPower__ovr136);  // "Additional Power:"
    Print_Right((x_start + 144), (y_start + 8), string);  // extra mana amount
    
    GAME_MP_SpellVar_1 += spell_data_table[SBK_Spell_Index].casting_cost;
    _players[HUMAN_PLAYER_IDX].casting_cost_original = GAME_MP_SpellVar_1;
    _players[HUMAN_PLAYER_IDX].casting_cost_remaining = (GAME_MP_SpellVar_1 - (GAME_MP_SpellVar_1 * Casting_Cost_Reduction(HUMAN_PLAYER_IDX, SBK_Spell_Index)) / 100);

    Draw_Picture_To_Bitmap(IMG_SBK_SliderBar, IMG_SBK_PageText);

    Set_Window((x_start + 8), 0, (x_start + _xtra_mana_pos + 8), 199);

    Draw_Picture_Windowed((x_start + _osc_anim_ctr + 8), (y_start + 23), IMG_SBK_PageText);

    Draw_Picture_Windowed((x_start + _osc_anim_ctr - 40), (y_start + 23), IMG_SBK_PageText);

    FLIC_Draw((x_start + _xtra_mana_pos + 5),(y_start + 22), IMG_SBK_SliderDot);

    Reset_Window();

}


// WZD o136p04
/*
; displays the spell infusion dialog, allowing the
; player to specify the additional power they wish to
; channel into an infusable spell, setting both the
; initial and final cost into the wizard record
; BUG: ignores Evil Omens almost entirely
*/
/*

*/
void Spellbook_Mana_Adder_Screen(int16_t spell_idx, int16_t spellbook_field_idx)
{
    int16_t y_start = 0;
    int16_t ok_button_field = 0;
    int16_t input_field_idx = 0;
    int16_t x_start = 0;  // _DI_

    _temp_sint_1 = spellbook_field_idx;

    if(spellbook_field_idx > 5)
    {

        x_start = 5;

    }
    else
    {
        
        x_start = 165;

    }

    y_start = (21 + ((spellbook_field_idx % 6) * 22));  // center the popup box on the selected spell, vertically
    
    Spellbook_Mana_Adder_Load();

    _osc_anim_ctr = 0;

    GAME_MP_SpellVar_1 = spell_data_table[spell_idx].casting_cost;

    _xtra_mana_pos = 3;

    SBK_Spell_Index = spell_idx;

    Clear_Fields();

    Add_Scroll_Field((x_start + 12), (y_start + 22), 0, 106, 3, 103, 106, 7, &_xtra_mana_pos, (int16_t)str_empty_string__ovr136[0], ST_UNDEFINED);

    ok_button_field = Add_Button_Field((x_start + 123), (y_start + 18), &str_empty_string__ovr136[0], xtramana_ok_button_seg, (int16_t)str_hotkey_O__ovr136[0], ST_UNDEFINED);

    Assign_Auto_Function(Spellbook_Mana_Adder_Draw, 2);

    _osc_leave_screen = ST_FALSE;

    // ; completely redundant, the result of all this is zero
    GAME_MP_SpellVar_1 = (((_xtra_mana_pos - 3) * spell_data_table[SBK_Spell_Index].casting_cost) / 100);
    GAME_MP_SpellVar_1 += spell_data_table[SBK_Spell_Index].casting_cost;
    _players[HUMAN_PLAYER_IDX].casting_cost_original = GAME_MP_SpellVar_1;
    _players[HUMAN_PLAYER_IDX].casting_cost_remaining = (GAME_MP_SpellVar_1 - (GAME_MP_SpellVar_1 * Casting_Cost_Reduction(HUMAN_PLAYER_IDX, SBK_Spell_Index)) / 100);

    while(_osc_leave_screen == ST_FALSE)
    {

        Mark_Time();

        input_field_idx = Get_Input();

        if(input_field_idx == ok_button_field)
        {

            _osc_leave_screen = ST_TRUE;

        }

        if(_osc_leave_screen == ST_FALSE)
        {

            Set_Page_Off();

            Spellbook_Mana_Adder_Draw();
                                   
            PageFlip_FX();

        }

        Release_Time(2);

    }

    Clear_Fields();

    Deactivate_Auto_Function();

    // BUGBUG  Release_Block();
    Release_Block(_screen_seg);

}


// WZD o136p05
/*
; marks the sandbox, loads the spell slider images into
; it, then redoes the book page work area and realm
; icon allocations, the latter of which it rebuilds too
*/
/*

*/
void Combat_Spellbook_Mana_Adder_Load(void)
{
    int16_t itr = 0;  // _SI_

    Mark_Block(_screen_seg);

    IMG_SBK_PageText = Allocate_Next_Block(_screen_seg, 40);

    // SPELLSCR.LBX, 005  "XTRAMANA"    "background"
    IMG_SBK_SliderBG = LBX_Reload_Next(spellscr_lbx_file__ovr136, 5, _screen_seg);

    // SPELLSCR.LBX, 003  "XTRAMANA"    "star"
    IMG_SBK_SliderDot = LBX_Reload_Next(spellscr_lbx_file__ovr136, 3, _screen_seg);

    // SPELLSCR.LBX, 004  "XTRAMANA"    "arrow bar"
    IMG_SBK_SliderBar = LBX_Reload_Next(spellscr_lbx_file__ovr136, 4, _screen_seg);

    // SPELLSCR.LBX, 042  "XTRABUTT"    "var ok button"
    xtramana_ok_button_seg = LBX_Reload_Next(spellscr_lbx_file__ovr136, 42, _screen_seg);

    IMG_SBK_Anims = Allocate_Next_Block(_screen_seg, 1090);

    for(itr = 0; itr < 6; itr++)
    {

        spellbook_symbols_bitm[itr] = Allocate_Next_Block(_screen_seg, 4);

        Draw_Picture_To_Bitmap(_spellbook_small_symbols[itr], spellbook_symbols_bitm[itr]);

    }

    Reset_Cycle_Palette_Color();

}


// WZD o136p06
void Combat_Spellbook_Mana_Adder_Draw(void)
{
    char buffer[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
    char string[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
    int16_t x_start = 0;  // _SI_
    int16_t y_start = 0;  // _DI_

    if(_temp_sint_1 > 5)
    {
        x_start = 5;
    }
    else
    {
        x_start = 165;
    }

    y_start = (21 + ((_temp_sint_1 % 6) * 22));  // center the popup box on the selected spell, vertically

    _osc_anim_ctr = ((_osc_anim_ctr + 1) % 8);

    Copy_Back_To_Off();

    // CmbBook_Draw__WIP(16, 10, (int16_t)GAME_MP_SpellVar_2);
    CmbBook_Draw__WIP(16, 10, _mana_adder_caster_idx);

    FLIC_Draw((x_start - 4), (y_start + 1), IMG_SBK_SliderBG);

    GAME_MP_SpellVar_1 = (((_xtra_mana_pos - 3) * CMB_SliderLimit) / 100);  // extra mana amount

    strcpy(string, str_PLUS_SIGN__ovr136);

    SDL_itoa(GAME_MP_SpellVar_1, buffer, 10);  // extra mana amount

    strcat(string, buffer);  // extra mana amount

    Set_Outline_Color(246);

    Set_Font_Style_Shadow_Up(4, 4, 4, 4);

    Set_Alias_Color(6);

    Print((x_start + 4), (y_start + 8), str_AdditionalPower__ovr136);

    Print_Right((x_start + 144), (y_start + 8), string);  // extra mana amount

    Set_Outline_Color(251);

    Set_Font_Style_Shadow_Down(4, 4, 4, 4);

    Set_Alias_Color(6);

    Print((x_start + 4), (y_start + 8), str_AdditionalPower__ovr136);  // "Additional Power:"

    Print_Right((x_start + 144), (y_start + 8), string);

    GAME_MP_SpellVar_1 += spell_data_table[SBK_Spell_Index].casting_cost;  // extra mana amount

    // if((int16_t)GAME_MP_SpellVar_2 > CASTER_IDX_BASE)
    if(_mana_adder_caster_idx > CASTER_IDX_BASE)
    {

        // ; BUG: ignores Evil Omens
        _players[HUMAN_PLAYER_IDX].casting_cost_remaining = (GAME_MP_SpellVar_1 - ((GAME_MP_SpellVar_1 * Casting_Cost_Reduction(HUMAN_PLAYER_IDX, SBK_Spell_Index)) / 100));

    }
    else
    {

        // ; BUG: ignores Evil Omens
        _players[HUMAN_PLAYER_IDX].casting_cost_remaining = GAME_MP_SpellVar_1;

    }

    Draw_Picture_To_Bitmap(IMG_SBK_SliderBar, IMG_SBK_PageText);

    Set_Window((x_start + 8), 0, (x_start + _xtra_mana_pos + 8), 199);

    Draw_Picture_Windowed((x_start + _osc_anim_ctr + 8), (y_start + 23), IMG_SBK_PageText);

    Draw_Picture_Windowed((x_start + _osc_anim_ctr - 40), (y_start + 23), IMG_SBK_PageText);

    FLIC_Draw((x_start + _xtra_mana_pos + 5),(y_start + 22), IMG_SBK_SliderDot);

    Reset_Window();

}


// WZD o136p07
// drake178: CMB_SpellSlider()
/*
; displays the spell infusion dialog, allowing the
; player to specify the additional power they wish to
; channel into an infusable spell
; returns the total cost of the spell
; BUG: will not allow infusion with >16383 mana while
; fighting at the fortress, ignores Evil Omens almost
; entirely, and has several rounding bugs
; WARNING: obscures the actual extra power value
*/
/*
    OON XREF:

    IDK_mana = CMB_SpellSlider__STUB(spell_idx, Selected_Spell, caster_idx);

HERE:
    From Combat_Spellbook_Screen()
        spellbook_field_idx the passed in Selected_Spell@, the index on page of selected spell
        CMB_SpellBookPage is the associated page number

XREF:

*/
int16_t Combat_Spellbook_Mana_Adder_Screen(int16_t spell_idx, int16_t spellbook_field_idx, int16_t caster_idx)
{
    int16_t y_start = 0;
    int16_t ok_button_field = 0;
    int16_t input_field_idx = 0;
    int16_t cost_reduction = 0;  // _SI_
    int16_t x_start = 0;  // DNE in DAsm, reuses cost_reduction

    _temp_sint_1 = spellbook_field_idx;

    // GAME_MP_SpellVar_2 = (SAMB_ptr)caster_idx;
    _mana_adder_caster_idx = caster_idx;

    SBK_Spell_Index = spell_idx;

    Copy_On_To_Off_Page();

    Copy_Off_To_Back();

    if(caster_idx < CASTER_IDX_BASE)
    {

        // ; BUG: ignores Evil Omens
        // ; is this really necessary?
        CMB_SliderLimit = (battle_units[caster_idx].mana - spell_data_table[SBK_Spell_Index].casting_cost);

        if((spell_data_table[SBK_Spell_Index].casting_cost * 4) < CMB_SliderLimit)
        {

            CMB_SliderLimit = (spell_data_table[SBK_Spell_Index].casting_cost * 4);

        }

    }
    else
    {

        cost_reduction = Casting_Cost_Reduction(HUMAN_PLAYER_IDX, SBK_Spell_Index);

        CMB_SliderLimit = (spell_data_table[SBK_Spell_Index].casting_cost * 4);

        if(((_players[HUMAN_PLAYER_IDX].Cmbt_Skill_Left * (cost_reduction + 100)) / 100) < CMB_SliderLimit)
        {

            CMB_SliderLimit = ((_players[HUMAN_PLAYER_IDX].Cmbt_Skill_Left * (cost_reduction + 100)) / 100);

        }

        while((((CMB_SliderLimit + spell_data_table[SBK_Spell_Index].casting_cost) * cost_reduction) /100) > _players[HUMAN_PLAYER_IDX].Cmbt_Skill_Left)
        {

            CMB_SliderLimit--;

        }

        cost_reduction = Combat_Casting_Cost_Multiplier(HUMAN_PLAYER_IDX);

        // ; BUG: rounds the wrong way, one is a reverse-adjusted
        // ; value, the other is not
        // ; BUG: x0.5 multiplier results in a negative signed
        // ; value if the player has over 16,383 mana
        if((((_players[HUMAN_PLAYER_IDX].mana_reserve * 10) / cost_reduction) - Casting_Cost(HUMAN_PLAYER_IDX, spell_idx, 1)) < CMB_SliderLimit)
        {
            CMB_SliderLimit = (((_players[HUMAN_PLAYER_IDX].mana_reserve * 10) / cost_reduction) - Casting_Cost(HUMAN_PLAYER_IDX, spell_idx, 1));

        }

    }

    if(spellbook_field_idx > 5)
    {

        x_start = 5;

    }
    else
    {
        
        x_start = 165;

    }

    y_start = (21 + ((spellbook_field_idx % 6) * 22));  // center the popup box on the selected spell, vertically

    Combat_Spellbook_Mana_Adder_Load();

    _osc_anim_ctr = 0;

    GAME_MP_SpellVar_1 = spell_data_table[spell_idx].casting_cost;

    _xtra_mana_pos = 3;

    Clear_Fields();

    Add_Scroll_Field((x_start + 12), (y_start + 22), 0, 106, 3, 103, 106, 7, &_xtra_mana_pos, (int16_t)str_empty_string__ovr136[0], ST_UNDEFINED);

    ok_button_field = Add_Button_Field((x_start + 123), (y_start + 18), &str_empty_string__ovr136[0], xtramana_ok_button_seg, (int16_t)str_hotkey_O__ovr136[0], ST_UNDEFINED);

    Assign_Auto_Function(Combat_Spellbook_Mana_Adder_Draw, 2);

    _osc_leave_screen = ST_FALSE;

    while(_osc_leave_screen == ST_FALSE)
    {

        Mark_Time();

        input_field_idx = Get_Input();

        if(input_field_idx == ok_button_field)
        {

            _osc_leave_screen = ST_TRUE;

        }

        if(_osc_leave_screen == ST_FALSE)
        {

            Set_Page_Off();

            Combat_Spellbook_Mana_Adder_Draw();
                                   
            PageFlip_FX();

        }

    }

    Clear_Fields();

    Deactivate_Auto_Function();

    Release_Block(_screen_seg);

    CMB_ComposeBackgrnd__WIP();

    return GAME_MP_SpellVar_1;

}


// WZD o136p08
/*
screen for targeting other players' global enchantments

"Tries to remove one global enemy enchantment."
"...the purposes of dispelling the target global enchantment."
*/
void Spell_Target_Global_Enchantment_Screen_Load(int16_t spell_idx)
{
    int16_t x_start = 0;
    int16_t itr1 = 0;
    int16_t itr2 = 0;  // _SI_
    int16_t y_start = 0;  // _DI_

    Near_Allocate_Reset();

    Allocate_Reduced_Map();

    Near_Allocate_Mark();

    ovl_ench_list_spells = (int16_t *)Near_Allocate_Next(288);  // 144 2-byte values as int16_t

    ovl_ench_list_players = (int8_t *)Near_Allocate_Next(144);  // 144 int8_t

    Mark_Block(_screen_seg);

    xtramana_ok_button_seg = LBX_Reload_Next(spellscr_lbx_file__ovr136, 1, _screen_seg);

    x_start = 50;

    y_start = ((5 - _num_players) * 23);

    Set_Page_Off();

    Main_Screen_Draw();

    Set_Window(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, (y_start + 56 + ((_num_players - 2) * 46)));

    Clipped_Draw(x_start, y_start, xtramana_ok_button_seg);

    Set_Window(SCREEN_XMIN, (y_start + 56 + ((_num_players - 2) * 46)), SCREEN_XMAX, SCREEN_YMAX);

    Clipped_Draw(x_start, (y_start + (((_num_players - 2) * 46) - 138)), xtramana_ok_button_seg);

    if(y_start > 0)
    {

        Line(x_start, (y_start - 1), (x_start + 208), (y_start - 1), ST_BLACK);
        
        Line(x_start, (y_start + 62 + ((_num_players - 2) * 46)), (x_start + 208), (y_start + 62 + ((_num_players - 2) * 46)), ST_BLACK);

    }

    Copy_Off_To_Back();

    Set_Page_On();

    Release_Block(_screen_seg);

    Mark_Block(_screen_seg);

    // SPELLSCR.LBX, 007  "OVERSBUT"    ""
    spellscr_oversbut_seg = LBX_Reload_Next(spellscr_lbx_file__ovr136, 7, _screen_seg);

    Spell_Animation_Load_Graphics(spell_idx);

    for(itr1 = 1; itr1 < _num_players; itr1++)
    {
        lilwiz_gem_segs[itr1] = LBX_Reload_Next(lilwiz_lbx_file__ovr136, ((_players[itr1].wizard_id * 5) + _players[itr1].banner_id), _screen_seg);
    }

    // MAGIC.LBX, 006  "GEMS"      "grey gem"
    grey_gem_seg              = LBX_Reload_Next(magic_lbx_file__ovr136,  6, _screen_seg);
    // MAGIC.LBX, 051  "GEMS"      "broken grey gem"
    broken_grey_gem_seg       = LBX_Reload_Next(magic_lbx_file__ovr136, 51, _screen_seg);

    IMG_SBK_Anims = Allocate_Next_Block(_screen_seg, 100);

    IMG_SBK_PageText = Allocate_Next_Block(_screen_seg, 100);

    // RESOURCE.LBX, 032  "SELECTBK"  "top arrow"
    // RESOURCE.LBX, 033  "SELECTBK"  "bottom arrow"
    selectbk_top_arrow_seg = LBX_Reload_Next(resource_lbx_file__ovr136, 32, _screen_seg);
    selectbk_bottom_arrow_seg = LBX_Reload_Next(resource_lbx_file__ovr136, 33, _screen_seg);



    for(itr2 = 0; itr2 < _num_players; itr2++)
    {
        ovl_ench_list_cnt2[itr2] = 0;
    }

    for(itr2 = 0; itr2 < 144; itr2++)
    {
        ovl_ench_list_spells[itr2] = ST_UNDEFINED;
    }

    Build_Overland_Enchantment_List();

    for(itr2 = 0; itr2 < ovl_ench_cnt; itr2++)
    {
        ovl_ench_list_cnt2[ovl_ench_list_players[itr2]] += 1;
    }

    ovl_ench_list_ptr[0] = (ovl_ench_list_spells + (ovl_ench_list_cnt2[0]));
    ovl_ench_list_ptr[1] = (ovl_ench_list_spells + (ovl_ench_list_cnt2[0] + ovl_ench_list_cnt2[1]));
    ovl_ench_list_ptr[2] = (ovl_ench_list_spells + (ovl_ench_list_cnt2[0] + ovl_ench_list_cnt2[1] + ovl_ench_list_cnt2[2]));
    ovl_ench_list_ptr[3] = (ovl_ench_list_spells + (ovl_ench_list_cnt2[0] + ovl_ench_list_cnt2[1] + ovl_ench_list_cnt2[2] + ovl_ench_list_cnt2[3]));
    
    for(itr2 = 0; (_num_players - 1) > itr2; itr2++)
    {
        ovl_ench_list_fst[itr2] = 0;
    }

}


// WZD o136p09
/*
screen for targeting other players' global enchantments
*/
void Spell_Target_Global_Enchantment_Screen_Draw(void)
{
    char string[LEN_SPELL_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t colors[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t y = 0;
    int16_t x = 0;
    int16_t itr1 = 0;
    int16_t y_start = 0;
    int16_t x_start = 0;
    int16_t itr2 = 0;  // _DI_
    int16_t DBG_spell_idx = 0;  // DNE in Dasm
    int16_t DBG_string_width = 0;  // DNE in Dasm
    int16_t DBG_ovl_ench_list_idx = 0;  // DNE in Dasm

    x_start = 50;

    y_start = ((5 - _num_players) * 23);

    colors[0] = 246;

    for(itr1 = 1; itr1 < 6; itr1++)
    {
        colors[itr1] = (177 + itr1);
    }

    Copy_Back_To_Off();

    if(_osc_scanned_field > 0)
    {

        if(((_num_players - 1) * 3) >= _osc_scanned_field)
        {

            FLIC_Draw((x_start + 73), (y_start + 16 + ((_osc_scanned_field - 1) * 13) + (((_osc_scanned_field - 1) / 3) * 7)), spellscr_oversbut_seg);
            
        }

    }

    for(itr2 = 1; itr2 < _num_players; itr2++)
    {
        
        x = (x_start + 8);
        
        y = (y_start + 14 + ((itr2 - 1) * 46));

        if(
            (
                (_FORTRESSES[itr2].active == ST_TRUE)
                &&
                (_players[_human_player_idx].Dipl.Contacted[itr2] == ST_TRUE)
            )
            ||
            (ovl_ench_list_cnt2[itr2] > 0)
        )
        {

            FLIC_Draw(x, y, lilwiz_gem_segs[itr2]);

        }
        else
        {

            if(_FORTRESSES[itr2].active == ST_TRUE)
            {

                FLIC_Draw(x, y, grey_gem_seg);

            }
            else
            {

                FLIC_Draw(x, y, broken_grey_gem_seg);

            }

        }

        if(ovl_ench_list_cnt2[itr2] > 0)
        {

            x = (x_start + 76);

            y = (y_start + 18 + ((itr2 - 1) * 46));

            for(itr1 = 0; itr1 < 3; itr1++)
            {

                if(ovl_ench_list_cnt2[itr2] > (ovl_ench_list_fst[(itr2 - 1)] + itr1))
                {

                    Set_Font_Style(2, 1, 0, 0);

                    Set_Alias_Color(186);

                    _fstrcpy((char *)&string[0], spell_data_table[ovl_ench_list_ptr[(itr2 - 1)][(ovl_ench_list_fst[(itr2 - 1)] + itr1)]].name);

                    Print(x, (y + (itr1 * 13)), (char *)&string[0]);

                }

            }

        }

    }

    Set_Font_Colors_15(4, &colors[0]);

    if(_osc_need_target_flag == ST_TRUE)
    {

        strcpy(GUI_NearMsgString, aSelectASpellTo);  // "Select a spell to "

        if(SBK_Spell_Index != spl_Spell_Binding)
        {

            strcat(GUI_NearMsgString, aDisjunct_);  // "disjunct."

        }
        else
        {

            strcat(GUI_NearMsgString, aBind_);  // "bind."
        }

    }
    else
    {

        // _temp_sint_1 is {F,T} - disjunct/bind succeeded  (same as _ce_bldg_idx)
        if(_temp_sint_1 == ST_TRUE)
        {
            
            if(SBK_Spell_Index != spl_Spell_Binding)
            {

                strcpy(GUI_NearMsgString, aSpellSuccesful);  // "Spell succesfully dispelled."

            }
            else
            {

                strcpy(GUI_NearMsgString, aSpellSuccesf_0);  // "Spell succesfully bound."

            }

            // _ce_bldg_idx is {F,T} - disjunct/bind succeeded  (same as _temp_sint_1)
            if(_ce_bldg_idx == ST_TRUE)
            {

                FLIC_Draw((x_start + 73), (y_start + 16 + ((_osc_scanned_field - 1) * 13) + (((_osc_scanned_field - 1) % 3) * 7)), spellscr_oversbut_seg);

            }

        }
        else
        {

            if(SBK_Spell_Index != spl_Spell_Binding)
            {

                strcpy(GUI_NearMsgString, aDisjunctionUns);  // "Disjunction unsuccesful"

            }
            else
            {

                strcpy(GUI_NearMsgString, aSpellBindingUn);  // "Spell Binding unsuccesful"

            }

        }

    }

    Print_Centered((x_start + 105), (y_start + 3), GUI_NearMsgString);

}


// WZD o136p10
/*
screen for targeting other players' global enchantments
*/
int16_t Spell_Target_Global_Enchantment_Screen(int16_t spell_idx, int16_t player_idx)
{
    int16_t fields[(3 * (5 - 1))] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t bot_arrow_fields[4] = { 0, 0, 0, 0 };
    int16_t top_arrow_fields[4] = { 0, 0, 0, 0 };
    int16_t y = 0;
    int16_t x = 0;
    int16_t var_C = 0;
    int16_t var_A = 0;
    int16_t y_start = 0;
    int16_t x_start = 0;
    int16_t var_4 = 0;
    int16_t input_field_idx = 0;
    int16_t itr = 0;

/*
#define UIOBJ_CLEAR_LOCAL() \
    do { \
        STARMAP_UIOBJ_CLEAR_COMMON(); \
        oi_accept = UIOBJI_INVALID; \
        oi_cancel = UIOBJI_INVALID; \
    } while (0)

    UIOBJ_CLEAR_LOCAL();
*/
#define ADD_SPELL_FIELDS_LOCAL()  \
    do {  \
        for(itr = 0; ((_num_players - 1) * 3) > itr; itr++)  \
        {  \
            x = (x_start + 74);  \
            y = (y_start + 16 + ((itr / 3) * 46) + ((itr % 3) * 13));  \
            fields[itr] = Add_Hidden_Field(x, y, (x + 120), (y + 13), (int16_t)str_empty_string__ovr136[0], ST_UNDEFINED);  \
        }  \
    } while(0)
#define ADD_ARROW_FIELDS_LOCAL()  \
    do {  \
        for(itr = 0; (_num_players - 1) > itr; itr++)  \
        {  \
            if(ovl_ench_list_fst[itr] > 0)  \
            {  \
                top_arrow_fields[itr] = Add_Button_Field((x_start + 61), (y_start + 19 + (itr * 46)), str_empty_string__ovr136, selectbk_top_arrow_seg, (int16_t)str_empty_string__ovr136[0], ST_UNDEFINED);  \
            }  \
            else  \
            {  \
                top_arrow_fields[itr] = INVALID_FIELD;  \
            }  \
            if((ovl_ench_list_fst[itr] + 3) < ovl_ench_list_cnt2[(itr + 1)])  \
            {  \
                bot_arrow_fields[itr] = Add_Button_Field((x_start + 61), (y_start + 43 + (itr * 46)), str_empty_string__ovr136, selectbk_bottom_arrow_seg, (int16_t)str_empty_string__ovr136[0], ST_UNDEFINED);  \
            }  \
            else  \
            {  \
                bot_arrow_fields[itr] = INVALID_FIELD;  \
            }  \
        }  \
    } while(0)
#define ADD_FIELDS_LOCAL()  \
    do {  \
        Clear_Fields();  \
        ADD_SPELL_FIELDS_LOCAL();  \
        ADD_ARROW_FIELDS_LOCAL();  \
    } while(0)

    x_start = 50;

    y_start = ((5 - _num_players) * 23);  // { 0, 23, 46, 69, 92 }

    _osc_need_target_flag = ST_TRUE;

    _ce_bldg_idx = ST_FALSE;  // {F,T} - disjunct/bind succeeded

    SBK_Spell_Index = spell_idx;

    Spell_Target_Global_Enchantment_Screen_Load(spell_idx);

    // total count - human player count ... you might, but they don't
    if((ovl_ench_cnt - ovl_ench_list_cnt2[0]) <= 0)
    {
        strcpy(GUI_NearMsgString, aThereAreNoGlob);  // "There are no global spells to "
        if(spell_idx != spl_Spell_Binding)
        {
            strcat(GUI_NearMsgString, aDisjunct_);  // "disjunct."
            Warn0(GUI_NearMsgString);
            var_A = ST_UNDEFINED;
            var_C = ST_UNDEFINED;
        }
        else
        {
            strcat(GUI_NearMsgString, aBind_);  // "bind."
            Warn0(GUI_NearMsgString);
            var_A = ST_UNDEFINED;
            var_C = ST_UNDEFINED;
        }
    }
    else
    {

        Assign_Auto_Function(Spell_Target_Global_Enchantment_Screen_Draw, 2);

        ADD_FIELDS_LOCAL();

        _osc_leave_screen = ST_FALSE;

        while(_osc_leave_screen == ST_FALSE)
        {

            input_field_idx = Get_Input();

            _osc_scanned_field = Scan_Input();

            if(((_osc_scanned_field - 1) % 3) >= ovl_ench_list_cnt2[(1 + ((_osc_scanned_field - 1) / 3))])
            {
                
                _osc_scanned_field = 0;

            }


            for(itr = 0; ((_num_players - 1) * 3) > itr; itr++)
            {
                if(fields[itr] == input_field_idx)
                {
                    if(_osc_scanned_field != 0)
                    {
                        Play_Left_Click();

                        var_A = ovl_ench_list_ptr[(itr / 3)][(itr % 3) + ovl_ench_list_fst[(itr / 3)]];

                        var_C = (1 + (itr / 3));

                        var_4 = itr;

                        _osc_leave_screen = ST_TRUE;
                    }
                }
            }


            /*
                BEGIN:  Left-Click Arrows
            */
            for(itr = 0; (_num_players - 1) > itr; itr++)
            {
                if(top_arrow_fields[itr] == input_field_idx)
                {
                    if(ovl_ench_list_fst[itr] > 0)
                    {
                        Play_Left_Click__DUPE();
                        ovl_ench_list_fst[itr] -= 1;
                    }
                }
                if(bot_arrow_fields[itr] == input_field_idx)
                {
                    if((ovl_ench_list_fst[itr] + 3) < ovl_ench_list_cnt2[(1 + itr)])
                    {
                        Play_Left_Click__DUPE();
                        ovl_ench_list_fst[itr] += 1;
                    }
                }
            }
            /*
                END:  Left-Click Arrows
            */

            ADD_FIELDS_LOCAL();

            if(_osc_leave_screen == ST_FALSE)
            {

                Set_Page_Off();

                Spell_Target_Global_Enchantment_Screen_Draw();

                PageFlip_FX();

            }

        }

    }

    Clear_Fields();

    fields[0] = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, (int16_t)str_empty_string__ovr136[0], ST_UNDEFINED);

    // we did it
    if(var_A > ST_UNDEFINED)
    {

        // which one did we do?
        if(spell_idx != spl_Spell_Binding)
        {

            // apply the effects of spl_Disjunction or spl_Disjunction_True

            // ; is this part of a redraw function? (seravy's bug)
            Spell_Target_Global_Enchantment_Disjunct__WIP(var_4, player_idx, spell_idx, var_C);

        }
        else
        {

            // apply the effects ofspl_Spell_Binding

            // ; is this part of a redraw function? (seravy's bug)
            Spell_Target_Global_Enchantment_Bind__WIP(var_4, player_idx, spell_idx, var_C);

        }

    }

    /* SPELLY */  OVL_MosaicFlip__STUB();

    Release_Block(_screen_seg);

    Near_Allocate_Undo();


    // I don't see anything anywhere that returns a value
    return ST_TRUE;

}


// WZD o136p11
/*
applies the effects of spl_Spell_Binding
*/
void Spell_Target_Global_Enchantment_Bind__WIP(int16_t field_idx, int16_t player_idx, int16_t target_spell_idx, int16_t target_player_idx)
{
    char string[LEN_SPELL_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
    int16_t spell_idx = 0;
    int16_t input_field_idx = 0;
    int16_t x = 0;
    int16_t y = 0;
    int16_t y_start = 0;
    int16_t IDK_x = 0;
    int16_t x_start = 0;
    uint8_t * ptr_enchantments = 0;
    int16_t succeeded = 0;
    int16_t itr = 0;  // _SI_
    // _DI_IDK_cast_or_dispel_cost__strlen = di
    int16_t casting_cost = 0;
    int16_t threshold = 0;
    int16_t character_count = 0;
    int16_t half_width = 0;

    x_start = 50;

    y_start = ((5 - _num_players) * 23);

    Update_Remap_Color_Range(9, 10);

    // spell_idx = ovl_ench_list_ptr[(itr2 - 1)][(ovl_ench_list_fst[itr2] + itr1)]
    spell_idx = ovl_ench_list_ptr[(target_player_idx - 1)][((field_idx % 3) + ovl_ench_list_fst[(target_player_idx - 1)])];

    _players[player_idx].casting_cost_original *= 20;

    succeeded = ST_FALSE;

    casting_cost = (spell_data_table[spell_idx].casting_cost + _players[player_idx].casting_cost_original);

    casting_cost = ((_players[player_idx].casting_cost_original * 250) / casting_cost);

    threshold = Calculate_Dispel_Difficulty(casting_cost, target_player_idx, spell_data_table[target_spell_idx].magic_realm);

    if(Random(250) <= threshold)
    {

        succeeded = ST_TRUE;

        ptr_enchantments = &_players[target_player_idx].Globals[0];

        ptr_enchantments[spell_data_table[spell_idx].ge_idx] = 0;

        ptr_enchantments = &_players[player_idx].Globals[0];

        ptr_enchantments[spell_data_table[spell_idx].ge_idx] = (player_idx + 1);  // player num

    }

    _temp_sint_1 = succeeded;

    x = (x_start + 73);

    y = (y_start + 16 + (field_idx * 13) + ((field_idx / 3) * 7));
// FLIC_Draw((x_start + 73), (y_start + 16 + ((_osc_scanned_field - 1) * 13) + (((_osc_scanned_field - 1) / 3) * 7)), spellscr_oversbut_seg);

    _fstrcpy(string, spell_data_table[spell_idx].name);

    character_count = (int16_t)strlen(string);

    half_width = ((character_count * 5) / 2);

    IDK_x = (x + half_width - 18);

    input_field_idx = 0;

    for(itr = 0; ((itr < 54) && (input_field_idx == 0)); itr++)
    {

        input_field_idx = Get_Input();

        Mark_Time();

        Set_Page_Off();

        Spell_Target_Global_Enchantment_Screen_Draw();

        if(itr > 23)
        {

            _osc_need_target_flag = ST_FALSE;

        }

        if(succeeded == ST_TRUE)
        {

            Create_Picture(126, 12, IMG_SBK_PageText);

            if(
                (itr > 23)
                &&
                (itr < 36)
            )
            {

                _ce_bldg_idx = ST_TRUE;  // ¿ redundant ?

                Set_Font_Style(2, 1, 0, 0);

                Print_To_Bitmap(3, 2, string, IMG_SBK_PageText);

                Scale_Bitmap(IMG_SBK_PageText, (100 - ((itr - 23) * 8)), 100);

                Draw_Picture((x + ((((itr - 23) * half_width) * 8) / 100)), y, IMG_SBK_PageText);

            }

        }

        Reset_Window();

        if(itr < 36)
        {

            FLIC_Draw(IDK_x, (y - 1), spell_animation_seg);

        }
        else
        {

            Clipped_Draw((IDK_x - ((IDK_x / 12) * (itr - 35))), y, spell_animation_seg);

        }

        PageFlip_FX();

        Release_Time(2);

    }

    if(succeeded == ST_TRUE)
    {

        _ce_bldg_idx = ST_TRUE;

    }

}


// WZD o136p12
/*
applies the effects of spl_Disjunction or spl_Disjunction_True
*/
void Spell_Target_Global_Enchantment_Disjunct__WIP(int16_t field_idx, int16_t player_idx, int16_t target_spell_idx, int16_t target_player_idx)
{
    int16_t spell_idx = 0;
    int16_t input_field_idx = 0;
    int16_t x = 0;
    int16_t y = 0;
    int16_t y_start = 0;
    int16_t IDK_x = 0;
    int16_t x_start = 0;
    uint8_t * ptr_enchantments = 0;
    int16_t succeeded = 0;
    int16_t itr = 0;  // _SI_
    // _DI_IDK_cast_or_dispel_cost__strlen = di
    int16_t casting_cost = 0;
    int16_t threshold = 0;
    int16_t character_count = 0;
    int16_t half_width = 0;

    x_start = 50;

    y_start = ((5 - _num_players) * 23);

    spell_idx = ovl_ench_list_ptr[(target_player_idx - 1)][(ovl_ench_list_fst[target_player_idx] + (field_idx % 3))];

    if(target_spell_idx == spl_Disjunction_True)
    {

        _players[player_idx].casting_cost_original *= 3;

    }

    succeeded = ST_FALSE;

    casting_cost = (spell_data_table[spell_idx].casting_cost + _players[player_idx].casting_cost_original);

    casting_cost = ((_players[player_idx].casting_cost_original * 250) / casting_cost);


    threshold = Calculate_Dispel_Difficulty(casting_cost, target_player_idx, spell_data_table[target_spell_idx].magic_realm);

    if(Random(250) <= threshold)
    {

        succeeded = ST_TRUE;

        ptr_enchantments = &_players[target_player_idx].Globals[0];

        ptr_enchantments[spell_data_table[spell_idx].ge_idx] = 0;

    }

    _temp_sint_1 = succeeded;

    x = (x_start + 73);

    y = (y_start + 16 + (field_idx * 13) + ((field_idx / 3) * 7));

    input_field_idx = 0;

    for(itr = 0; ((itr < 24) && (input_field_idx == 0)); itr++)
    {

        input_field_idx = Get_Input();

        Mark_Time();

        Set_Page_Off();

        Spell_Target_Global_Enchantment_Screen_Draw();

        if(itr > 10)
        {

            _osc_need_target_flag = ST_FALSE;

        }

        if(
            (succeeded == ST_TRUE)
            &&
            (itr > 10)
        )
        {

            Draw_Picture_To_Bitmap(spellscr_oversbut_seg, IMG_SBK_Anims);

            if(((itr - 11) * 10) < 100)
            {

                Vanish_Bitmap__WIP(IMG_SBK_Anims, ((itr - 11) * 10));

            }

            Draw_Picture(x, y, IMG_SBK_Anims);

        }

        FLIC_Draw((x - 4), (y - 12), spell_animation_seg);

        PageFlip_FX();

        Release_Time(2);
        
    }

    if(succeeded == ST_TRUE)
    {

        _ce_bldg_idx = ST_TRUE;

    }

}



/*
    WIZARDS.EXE  ovr138
*/

// WZD o138p01
// IDK_SomScr_Lose_Load()
void Spell_Of_Mastery_Lose_Load(int16_t wizard_id)
{

    IMG_SBK_SliderBG = LBX_Reload(spellose_lbx_file__ovr138, 28, _screen_seg);

    wizlab_familiar_seg = LBX_Reload_Next(spellose_lbx_file__ovr138, (30 + _players[HUMAN_PLAYER_IDX].Prim_Realm), _screen_seg);

    Load_Palette_From_Animation(IMG_SBK_SliderBG);

    Apply_Palette();

    Set_Page_Off();

    FLIC_Draw(0, 0, IMG_SBK_SliderBG);

    Copy_Off_To_Back();

    strcpy(GUI_NearMsgString, _players[GAME_SoM_Cast_By].name);

    strcat(GUI_NearMsgString, strHasCastThe);  // " has cast the"

    // SPELLOSE.LBX, 029  "SPHERE"    ""
    spellose_sphere_seg = LBX_Reload(spellose_lbx_file__ovr138, 29, _screen_seg);

    // SPELLOSE.LBX, 000  "MERLISPH"  ""
    // SPELLOSE.LBX, 001  "SHAMMSPH"  ""
    // SPELLOSE.LBX, 002  "PRIESSPH"  ""
    // SPELLOSE.LBX, 003  "WUSPH"     ""
    // SPELLOSE.LBX, 004  "ARABSPH"   ""
    // SPELLOSE.LBX, 005  "OBERSPH"   ""
    // SPELLOSE.LBX, 006  "WRAITSPH"  ""
    // SPELLOSE.LBX, 007  "DRACOSPH"  ""
    // SPELLOSE.LBX, 008  "NMOISPH"   ""
    // SPELLOSE.LBX, 009  "FREYASPH"  ""
    // SPELLOSE.LBX, 010  "GALESPH"   ""
    // SPELLOSE.LBX, 011  "ARIELSPH"  ""
    // SPELLOSE.LBX, 012  "AZTECSPH"  ""
    // SPELLOSE.LBX, 013  "KARLASPH"  ""
    spellose_wizard_sphere_seg = LBX_Reload_Next(spellose_lbx_file__ovr138, wizard_id, _screen_seg);

    // SPELLOSE.LBX, 014  "MERLIWIN"  ""
    // SPELLOSE.LBX, 015  "SHAMMWIN"  ""
    // SPELLOSE.LBX, 016  "PRIESWIN"  ""
    // SPELLOSE.LBX, 017  "WUWIN"     ""
    // SPELLOSE.LBX, 018  "ARABWIN"   ""
    // SPELLOSE.LBX, 019  "OBERWIN"   ""
    // SPELLOSE.LBX, 020  "WRAITWIN"  ""
    // SPELLOSE.LBX, 021  "DRACOWIN"  ""
    // SPELLOSE.LBX, 022  "NMOIWIN"   ""
    // SPELLOSE.LBX, 023  "FREYAWIN"  ""
    // SPELLOSE.LBX, 024  "GALEWIN"   ""
    // SPELLOSE.LBX, 025  "ARIELWIN"  ""
    // SPELLOSE.LBX, 026  "AZTECWIN"  ""
    // SPELLOSE.LBX, 027  "KARLAWIN"  ""
    wizlab_wizard_seg = LBX_Reload_Next(spellose_lbx_file__ovr138, (14 + wizard_id), _screen_seg);

    IMG_SBK_Anims = Allocate_Next_Block(_screen_seg, 900);  // 900 PR  14400 B

}


// WZD o138p02
// IDK_SomScr_Lose_Draw()
void Spell_Of_Mastery_Lose_Draw(void)
{
    uint8_t colors[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t start_x = 0;
    int16_t x = 0;
    int16_t height = 0;
    int16_t width = 0;
    int16_t y1 = 0;
    int16_t x1 = 0;
    int16_t itr = 0;  // _DI_
    int16_t IDK = 0;  // _SI_

    for(itr = 0; itr < 16; itr++)
    {
        colors[itr] = 5;
    }

    Copy_Back_To_Off();

    Reset_Animation_Frame(wizlab_wizard_seg);

    Draw_Picture_To_Bitmap(wizlab_wizard_seg, IMG_SBK_Anims);

    Get_Bitmap_Actual_Size(IMG_SBK_Anims, &x1, &y1, &width, &height);

    start_x = 35 + x1 + (width / 2);

    Reset_Animation_Frame(wizlab_wizard_seg);

    Set_Font_Colors_15(5, &colors[0]);

    Set_Outline_Color(0);

    Set_Font_Style_Outline(5, 15, 0, 0);

    // SPELLY  Print_Centered(160, 5, GUI_NearMsgString, 2);
    Print_Centered(160, 5, GUI_NearMsgString);

    // SPELLY  Print_Centered(160, 25, strSpellOfMastery, ST_NULL);
    Print_Centered(160, 25, strSpellOfMastery);

    if(_osc_anim_ctr < 12)
    {

        FLIC_Draw(61, 68, wizlab_wizard_seg);

        Set_Animation_Frame(spellose_sphere_seg, (_osc_anim_ctr / 2));

        FLIC_Draw(start_x, 34, spellose_sphere_seg);

    }
    else
    {

        if(_osc_anim_ctr < 24)
        {

            Set_Animation_Frame(spellose_sphere_seg, 5);

            FLIC_Draw(start_x, 34, spellose_sphere_seg);

            x = 61;  // BUGBUG  never used

            IDK = -y1;

            if(((_osc_anim_ctr - 12) / 2) < 2)
            {

                // SPELLY  LBX_IMG_VShiftRect(x1, 0, (x1 + (width / 4)), (IDK / 6), IMG_SBK_Anims);

                // SPELLY  LBX_IMG_VShiftRect((x1 + (width / 4) + 1), (IDK / 6), (x1 + (width / 2)), (IDK / 3), IMG_SBK_Anims);

                // SPELLY  LBX_IMG_VShiftRect((x1 + (width / 2) + 1), (IDK / 3), (x1 + ((width * 3) / 4)), (IDK / 6), IMG_SBK_Anims);

                // SPELLY  LBX_IMG_VShiftRect((x1 + ((width * 3) / 4) + 1), (IDK / 6), (x1 + width), 0, IMG_SBK_Anims);

            }
            else
            {

                if(((_osc_anim_ctr - 12) / 2) < 4)
                {

                    // SPELLY  LBX_IMG_VShiftRect(x1, 0, (x1 + (width / 4)), (IDK / 4), IMG_SBK_Anims);

                    // SPELLY  LBX_IMG_VShiftRect((x1 + (width / 4) + 1), (IDK / 4), (x1 + (width / 2)), (IDK / 2), IMG_SBK_Anims);

                    // SPELLY  LBX_IMG_VShiftRect((x1 + (width / 2) + 1), (IDK / 2), (x1 + ((width * 3) / 4)), (IDK / 4), IMG_SBK_Anims);

                    // SPELLY  LBX_IMG_VShiftRect((x1 + ((width * 3) / 4) + 1), (IDK / 4), (x1 + width), 0, IMG_SBK_Anims);

                }
                else
                {

                    // SPELLY  LBX_IMG_VShiftRect(x1, 0, (x1 + (width / 4)), (IDK / 2), IMG_SBK_Anims);

                    // SPELLY  LBX_IMG_VShiftRect((x1 + (width / 2) + 1), (IDK / 2), (x1 + (width / 2)), IDK, IMG_SBK_Anims);

                    // SPELLY  LBX_IMG_VShiftRect((x1 + (width / 2) + 1), IDK, (x1 + ((width * 3) / 4)), (IDK / 2), IMG_SBK_Anims);

                    // SPELLY  LBX_IMG_VShiftRect((x1 + ((width * 3) / 4) + 1), (IDK / 2), (x1 + width), 0, IMG_SBK_Anims);

                }

            }

            Scale_Bitmap(IMG_SBK_Anims, (100 - ((_osc_anim_ctr - 12) * 6)), 100);

            x = (61 + (((_osc_anim_ctr - 12) * 714) / 200));

            Draw_Picture(x, 68, IMG_SBK_Anims);

            Reset_Animation_Frame(spellose_wizard_sphere_seg);

            Draw_Picture_To_Bitmap(spellose_wizard_sphere_seg, IMG_SBK_Anims);

            // SPELLY  Vanish_Bitmap(IMG_SBK_Anims, ((_osc_anim_ctr - 12) * 8));

            Draw_Picture(start_x, 34, IMG_SBK_Anims);

        }
        else
        {

            if(_osc_anim_ctr < 36)
            {

                Set_Animation_Frame(spellose_wizard_sphere_seg, ((_osc_anim_ctr - 24) / 2));

                FLIC_Draw(start_x, 34, spellose_wizard_sphere_seg);

            }

        }

    }

    FLIC_Draw(190, 160, wizlab_familiar_seg);

}


// WZD o138p03
// IDK_SomScr_Lose()
void Spell_Of_Mastery_Lose(void)
{

    Clear_Fields();

    Stop_All_Sounds__STUB();

    SND_Spell_Music = LBX_Reload(music_lbx_file__ovr138, MUSIC_LOSE_By_Magic, SND_Music_Segment);
    SND_Spell_Music_size = lbxload_entry_length;


    if(magic_set.background_music == ST_TRUE)
    {
        // DOMSDOS  Play_Sound__STUB(SND_Spell_Music);
        sdl2_Play_Sound__WIP(SND_Spell_Music, SND_Spell_Music_size);
    }

    Set_Mouse_List(1, mouse_list_none);

    CLROFF();

    Toggle_Pages();

    Copy_On_To_Off_Page();

    Spell_Of_Mastery_Lose_Load(_players[HUMAN_PLAYER_IDX].wizard_id);

    Assign_Auto_Function(Spell_Of_Mastery_Lose_Draw, 2);
    
    _osc_anim_ctr = 0;

    _page_flip_effect = 3;

    Set_Page_Off();

    Spell_Of_Mastery_Lose_Draw();

    PageFlip_FX();

    for(_osc_anim_ctr = 0; _osc_anim_ctr < 43; _osc_anim_ctr++)
    {

        Mark_Time();

        Set_Page_Off();

        Spell_Of_Mastery_Lose_Draw();

        PageFlip_FX();

        Release_Time(2);

        _osc_anim_ctr++;
 
    }

    Combat_Cache_Read();

    // SPELLY  GAME_LimboFallAnim(HUMAN_PLAYER_IDX);

    // SPELLY  GAME_OVER();

    s01p16_empty_function();

    // SPELLY  GAME_EXE_Swap(cnst_MAGIC_EXE_File2, cnst_MAGICEXE_arg0_2, &spellose_lbx_file__ovr138[8], &spellose_lbx_file__ovr138[8]);

}


// WZD o138p04
void SoM_Started_Draw__STUB(void)
{
    int16_t itr_frames = 0;  // _SI_
    int16_t frame_count = 0;  // _DI_

    frame_count = (Get_File_Animation_Frame() + 1);

    Set_File_Animation_Frame(0);

    Set_Page_Off();

    for(itr_frames = 0; itr_frames < frame_count; itr_frames++)
    {

    }

    Set_Outline_Color(0);

    Set_Font_Style_Outline(5, 5, 0, 0);

    // SPELLY  Print_Centered(160, 160, GUI_NearMsgString, 2);
    Print_Centered(160, 160, GUI_NearMsgString);

    Print_Centered(160, 180, strSpellOfMastery);

    _combat_wx++;

}

// WZD o138p05
void SoM_Started__STUB(int16_t player_idx)
{
    int16_t full_screen_ESC_field = 0;

    Deactivate_Auto_Function();

    Stop_All_Sounds__STUB();

    strcpy(GUI_NearMsgString, _players[player_idx].name);

    strcat(GUI_NearMsgString, strHasStartedCastingThe);  // " has started casting the"

    SND_Spell_Music = LBX_Reload(music_lbx_file__ovr138, MUSIC_SoM_Started, SND_Music_Segment);
    SND_Spell_Music_size = lbxload_entry_length;

    // SPELLSCR.LBX, 067  "VORTEX3" ""
    Open_File_Animation__HACK(spellscr_lbx_file__ovr138, 67);

    _temp_sint_4 = 67;  // ¿ DEDU ?

    Set_Mouse_List(1, mouse_list_none);

    Fade_Out();

    Set_Page_On();

    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_BLACK);

    Copy_On_To_Off_Page();

    Toggle_Pages();

    // TODO  Load_Palette(4, -1);  // ; TEST - spell mastery palette
    Load_Palette(4, 0, 255);

    Set_Page_Off();

    SoM_Started_Draw__STUB();

    Toggle_Pages();

    Fade_In();

    Assign_Auto_Function(SoM_Started_Draw__STUB, 2);

    if(magic_set.background_music == ST_TRUE)
    {
        // DOMSDOS  Play_Sound__WIP(SND_Spell_Music);
        sdl2_Play_Sound__WIP(SND_Spell_Music, SND_Spell_Music_size);
    }

    Clear_Fields();

    full_screen_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr138[0], ST_UNDEFINED);

    _combat_wx = 0;
    /* incremented in SoM_Started_Draw__STUB() */
    while((_combat_wx < 200) && (Get_Input() != full_screen_ESC_field))
    {

        if(_combat_wx == 17)
        {

            Open_File_Animation__HACK(spellscr_lbx_file__ovr138, 68);

        }
        else if(_combat_wx == 33)
        {

            Open_File_Animation__HACK(spellscr_lbx_file__ovr138, 69);

        }
        else if(_combat_wx == 49)
        {

            Open_File_Animation__HACK(spellscr_lbx_file__ovr138, 70);

        }

    }

    // DOMSDOS  Stop_All_Sounds__STUB();

    // DOMSDOS  Play_Background_Music__STUB();
    sdl2_Play_Background_Music__WIP();

    Fade_Out();

    // SPELLY  Load_Palette(0, -1);  // ; EMPERATO - main game palette
    Load_Palette(0, ST_UNDEFINED, ST_NULL);

    Allocate_Reduced_Map();

    Assign_Auto_Function(Main_Screen_Draw, 2);

    if(player_idx == HUMAN_PLAYER_IDX)
    {
        Set_Mouse_List(1, mouse_list_default);
    }
    else
    {
        Set_Mouse_List(1, mouse_list_hourglass);
    }

    Set_Page_Off();

    Main_Screen_Draw();

    Toggle_Pages();

    Copy_On_To_Off_Page();

    Fade_In();

}

// WZD o138p06
// IDK_SomScr_Load()
/*

*/
void Spell_Of_Mastery_Load(void)
{

    Set_Page_Off();

    word_43494 = LBX_Reload(wizlab_lbx_file__ovr138, 19, _screen_seg);

    FLIC_Draw(0, 0, word_43494);

    Copy_Off_To_Back();

    Set_Page_On();

    wizlab_blue_column_seg = LBX_Reload(splmastr_lbx_file__ovr138, 29, _screen_seg);

    IDK_wizard_id_thing_seg = LBX_Reload(splmastr_lbx_file__ovr138, (14 + _players[cast_spell_of_mastery_player_idx].wizard_id), World_Data);

    word_434C4 = LBX_Reload_Next(splmastr_lbx_file__ovr138, 28, World_Data);

}


// WZD o138p07
// IDK_SomScr_Draw()
/*

*/
void Spell_Of_Mastery_Draw(void)
{

    Copy_Back_To_Off();

    FLIC_Draw(90, 0, wizlab_blue_column_seg);

    if(_osc_anim_ctr <= 6)
    {

        Reset_Animation_Frame(IDK_wizard_id_thing_seg);

        FLIC_Draw(69, 75, IDK_wizard_id_thing_seg);

    }
    else
    {

        if(((_osc_anim_ctr - 6) % 60) < 10)
        {

            Set_Animation_Frame(IDK_wizard_id_thing_seg, (((_osc_anim_ctr - 6) % 60) / 2));

        }
        else
        {

            if(((_osc_anim_ctr - 6) % 60) > 50)
            {

                Set_Animation_Frame(IDK_wizard_id_thing_seg, ((60 - ((_osc_anim_ctr - 6) % 60)) / 2));

            }
            else
            {

                Set_Animation_Frame(IDK_wizard_id_thing_seg, 4);

            }

        }

        FLIC_Draw(69, 75, IDK_wizard_id_thing_seg);

        if((6 + ((_num_players - 1) * 60)) > _osc_anim_ctr)
        {

            Draw_File_Animation__HACK();

            if(((_osc_anim_ctr - 6) % 60) < 9)
            {

                FLIC_Draw(95, 55, word_434C4);

            }

        }

    }

}


// WZD o138p08
// IDK_SomScr()
/*

*/
void Spell_Of_Mastery(int16_t player_idx)
{
    int16_t var_4 = 0;
    int16_t hotkey_ESC = 0;
    int16_t itr_players = 0;  // _DI_
    int16_t IDK = 0;  // _SI_

    GAME_SoM_Cast_By = player_idx;

    Combat_Cache_Write();

    Deactivate_Auto_Function();

    if(player_idx != HUMAN_PLAYER_IDX)
    {
        Spell_Of_Mastery_Lose();
    }
    
    _osc_anim_ctr = 0;

    cast_spell_of_mastery_player_idx = player_idx;

    // DOMSDOS  Stop_All_Sounds__STUB();

    SND_Spell_Music = LBX_Reload(music_lbx_file__ovr138, MUSIC_SoM_Started, SND_Music_Segment);  // MUSIC.LBX, 012 "MOM62 XM"    "Spell Mastery Started"
    SND_Spell_Music_size = lbxload_entry_length;

    Set_Mouse_List(1, mouse_list_none);

    Fade_Out();

    Spell_Of_Mastery_Load();

    CLROFF();

    Toggle_Pages();

    Copy_On_To_Off_Page();

    Set_Page_Off();

    Spell_Of_Mastery_Draw();

    Toggle_Pages();

    Fade_In();

    if(magic_set.background_music == ST_TRUE)
    {
        // DOMSDOS  Play_Sound__STUB(SND_Spell_Music);
        sdl2_Play_Sound__WIP(SND_Spell_Music, SND_Spell_Music_size);
    }

    Clear_Fields();

    hotkey_ESC = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, (int16_t)str_hotkey_ESC__ovr138[0], ST_UNDEFINED);

    var_4 = 12 + ((_num_players - 1) * 60);

    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {

        if(_players[itr_players].casting_spell_idx == spl_Spell_Of_Return)
        {
            var_4 -= 60;
        }

    }

    IDK = 0;

    _osc_anim_ctr = 0;

    while((_osc_anim_ctr < var_4) && (Get_Input() != hotkey_ESC))
    {

        // IDGI
        if(((_osc_anim_ctr + 65530) % 60) == 0)
        {
            while(_players[++IDK].casting_spell_idx == spl_Spell_Of_Return) {}

            Open_File_Animation__HACK(splmastr_lbx_file__ovr138, _players[IDK].wizard_id);

            Reset_Animation_Frame(word_434C4);
        }

        if((var_4 - 6) == _osc_anim_ctr)
        {

            Release_Block(_screen_seg);

            Mark_Block(_screen_seg);

            wizlab_blue_column_seg = LBX_Reload(splmastr_lbx_file__ovr138, 31, _screen_seg);

        }

        if(_osc_anim_ctr == 7)
        {

            wizlab_blue_column_seg = LBX_Reload(splmastr_lbx_file__ovr138, 30, _screen_seg);

        }

        Mark_Time();

        Set_Page_Off();

        Spell_Of_Mastery_Draw();

        Toggle_Pages();

        Release_Time(3);

        _osc_anim_ctr++;

    }

    // DOMSDOS  Stop_All_Sounds__STUB();

    // DOMSDOS Play_Background_Music__STUB();
    sdl2_Play_Background_Music__WIP();

    Fade_Out();

    Load_Palette(0, -1, ST_NULL);

    CLROFF();

    Toggle_Pages();

    Copy_On_To_Off_Page();

    Fade_In();

    Combat_Cache_Read();

    // SPELLY  GAME_PlayVictoryAnim(HUMAN_PLAYER_IDX);

    // SPELLY  GAME_OVER();

    s01p16_empty_function();

    // SPELLY  GAME_EXE_Swap(cnst_MAGIC_EXE_File2, cnst_MAGICEXE_arg0_2, &spellose_lbx_file__ovr138[0], &spellose_lbx_file__ovr138[0]);

}


// WZD o138p09
// CMB_LoadUndeadAnim()

// WZD o138p10
// CMB_DrawUndeadAnim()

// WZD o138p11
// CMB_CreateUndeadAnim()
