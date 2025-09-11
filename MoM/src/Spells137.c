/*
    WIZARDS.EXE
        ovr137  ¿ ~ Spell Animation(s) ?
*/

#include "../../STU/src/STU_DBG.h"

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/GENDRAW.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/Timer.h"
#include "../../MoX/src/Video.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_SET.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/paragrph.h"

#include "../../MoX/src/Help.h"
#include "../../MoX/src/MOM_DEF.h"
#include "NEXTTURN.h"
#include "SCastScr.h"
#include "Spells129.h"
#include "Spells130.h"
#include "CITYSCAP.h"
#include "MainScr.h"
#include "RACETYPE.h"
#include "SBookScr.h"
#include "Spellbook.h"
#include "UNITTYPE.h"
#include "WIZVIEW.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <SDL_stdinc.h>

#include "Spells137.h"



// MagicScr.C
// WZD dseg:35A4
extern char diplo_state[11][9];
// WZD dseg:958E
extern SAMB_ptr lilwiz_gem_segs[5];
// WZD dseg:95C0
extern SAMB_ptr grey_gem_seg;
// WZD dseg:95C2
extern SAMB_ptr broken_grey_gem_seg;
// WZD dseg:C20C
extern int16_t gem_player_nums[4];

// SPLMASTR.C
// WZD dseg:CA14
extern int16_t _osc_anim_ctr;
// WZD dseg:CA2A
extern SAMB_ptr IMG_OVL_TrgtWizCncl;
// WZD dseg:CA32
extern SAMB_ptr IMG_OVL_TargetWizBG;



// WZD dseg:6C4A                                                 BEGIN:  ovr137 - Initialized Data

// WZD dseg:6C4A
char spellscr_lbx_file__ovr137__1[] = "Spellscr";

// WZD dseg:6C52
char empty_string__ovr137[] = "";

// WZD dseg:6C53
char monster_lbx_file__ovr137[] = "Monster";
// WZD dseg:6C5B
char aSummoned[] = " Summoned";
// WZD dseg:6C65
char aItemEnchanted[] = "Item Enchanted";
// WZD dseg:6C74
char aHeroSummoned_0 [] = "Hero Summoned";
// WZD dseg:6C82
char aChampionSumm_0 [] = "Champion Summoned";

// WZD dseg:6C94
char music_lbx_file__ovr137[] = "Music";

// WZD dseg:6C9A
char spellscr_lbx_file__ovr137__2[] = "spellscr";

// WZD dseg:6CA3
char cnst_Sp_Of_Sp_5[] = " of ";

// WZD dseg:6CA8
char aYouCast[] = "You cast ";

// WZD dseg:6CB2
char aCasts[] = " casts ";

// WZD dseg:6CBA
char aDestroyed[] = "Destroyed";

// WZD dseg:6CC4
char moodwiz_lbx_file__ovr137[] = "moodwiz";

// WZD dseg:6CCC
char backgrnd_lbx_file__ovr137[] = "BACKGRND";

// WZD dseg:6CD5
char specfx_lbx_file__ovr137[] = "Specfx";

// WZD dseg:6CDC
char diplomac_lbx_file__ovr137[] = "Diplomac";

// WZD dseg:6CE5
char cnst_Spellcast_Msg_1[] = "You have completed casting...";

// WZD dseg:6D03
char cnst_Spellcast_Msg_2[] = " has cast...";

// WZD dseg:6D10
char spellscr_lbx_file__ovr137__3[] = "SPELLSCR";

// WZD dseg:6D19
char magic_lbx_file__ovr137[] = "MAGIC";

// WZD dseg:6D1F
char lilwiz_lbx_file__ovr137[] = "Lilwiz";

// WZD dseg:6D26
char hlpentry_lbx_file__ovr137[] = "hlpentry";

// WZD dseg:6D2F
char cnst_Space_MP_3[] = " MP";

// WZD dseg:6D33
char aChooseTargetFo[] = "Choose target for a ";
// WZD dseg:6D48
char aSpell_0[] = " spell";
// WZD dseg:6D4F
char aHasBeenSpellBl[] = " has been spell blasted";
// WZD dseg:6D67
char aLoses[] = " loses ";
// WZD dseg:6D6F
char aPointsOfCastin[] = " points of casting ability";
// WZD dseg:6D8A
char aPointsOfMana[] = " points of mana";
// WZD dseg:6D9A
char cnst_Apostrophe_3[] = "' ";
// WZD dseg:6D9D
char cnst_Possessive_3[] = "'s ";
// WZD dseg:6DA1
char aRelationsHaveB[] = " relations have been subverted";
// WZD dseg:6DC0
char str_hotkey_ESC__ovr137[] = "\1B";
// WZD dseg:6DC2
char message_lbx_file__ovr137[] = "message";

// WZD dseg:6E4B                                                 END:  ovr137 - Initialized Data



// WZD dseg:CA38                                                 BEGIN: ovr137 - Uninitialized Data

// WZD dseg:CA38
SAMB_ptr IDK_MONSTER_seg;

// WZD dseg:CA3A
SAMB_ptr SPELLSCR_ENCHANT_seg;

// WZD dseg:CA3C
SAMB_ptr SPELLSCR_FLAMEFR1_seg;

// WZD dseg:CA3E
SAMB_ptr SPELLSCR_GLOBALMK_seg;

// WZD dseg:CA40 00 00                                           IMG_SBK_SliderBar@ dw 0                 ; DATA XREF: ITEM_LoadCraftingRes+188w ...
// WZD dseg:CA40                                                                                         ; also the item crafting right arrow (2 state image)
// WZD dseg:CA42 00 00                                           IMG_SBK_SliderDot@ dw 0                 ; DATA XREF: ITEM_LoadCraftingRes+171w ...
// WZD dseg:CA42                                                                                         ; also the item crafting left arrow (2 state image)

// WZD dseg:CA44
/*
¿ was array of 4 ?
*/
SAMB_ptr ge_anim_moodwiz_seg;

// WZD dseg:CA46 00                                              db    0
// WZD dseg:CA47 00                                              db    0
// WZD dseg:CA48 00                                              db    0
// WZD dseg:CA49 00                                              db    0
// WZD dseg:CA4A 00                                              db    0
// WZD dseg:CA4B 00                                              db    0

// WZD dseg:CA4C 00 00                                           _osc_leave_screen dw 0               ; DATA XREF: SBK_SpellSlider+D5w ...
// WZD dseg:CA4E 00 00                                           _osc_need_target_flag dw 0          ; DATA XREF: IDK_Spell_DisjunctOrBind_Draw+20Ar ...
// WZD dseg:CA50 00 00                                           _osc_player_idx dw 0                 ; DATA XREF: SBK_SliderRedraw+8r ...
// WZD dseg:CA50                                                                                         ; clicked spell label index during combat sliders
// WZD dseg:CA50                                                                                         ; player_idx during global cast anims
// MOX/MOM_Data.c  // WZD dseg:CA50
// MOX/MOM_Data.c  
int16_t _temp_sint_1;
#define _osc_player_idx _temp_sint_1
#define _osc_spell_idx _temp_sint_1
#define _osc_city_idx _temp_sint_1
#define _osc_bldg_idx _temp_sint_1

// WZD dseg:CA50                                                                                         ; city_idx during ¿ ?
// WZD dseg:CA52 00 00                                           word_434F2 dw 0                         ; DATA XREF: IDK_Spell_DisjunctOrBind_Draw+25Fr ...
// WZD dseg:CA52
// WZD dseg:CA52                                                 END: ovr136 - Uninitialized Data



// WZD dseg:CA54                                                 BEGIN: ovr137 - Uninitialized Data

// WZD dseg:CA54
/*

IDK_SummonAnimLoad()
    IDK_wizard_id_thing_seg = LBX_Reload_Next(spellscr_lbx_file__ovr137__1, (46 + _players[player_idx].wizard_id), _screen_seg);

Cast_Spell_Of_Mastery_Load()
    IDK_wizard_id_thing_seg = LBX_Reload(splmastr_lbx_file__ovr138, (14 + _players[cast_spell_of_mastery_player_idx].wizard_id), World_Data);

*/
SAMB_ptr IDK_wizard_id_thing_seg;

// WZD dseg:CA54                                                 END: ovr137 - Uninitialized Data



/*
    WIZARDS.EXE  ovr137
*/

// WZD o137p01
// drake178: sub_BE860()
/*

¿ parameter is spell index or unit type / index into MONSTER.LBX ?

Cast_Spell_Overland__WIP()
    IDK_SummonAnim(spell_data_table[spell_idx].Param0, MultiPurpose_Local_Var, player_idx);
e.g.,
    'Magic Spirit'  (201)
    spell_data_table[spl_Magic_Spirit].Param0 == 154  AKA ut_Magic_Spirit
    // MONSTER.LBX, 000  "MONSTER1" "Magic Spirit"
    200 - 154 = 46
    46 - 46 = 0
    'Demon Lord'    (200)
    spell_data_table[spl_Demon_Lord].Param0 == 173  AKA ut_Demon_Lord
    // MONSTER.LBX, 019  "MONSTER2" "Demon Lord"
    200 - 173 = 27
    46 - 27 = 19
*/
void IDK_SummonAnim_Load(int16_t type_or_other, int16_t player_idx)
{
    SAMB_ptr IDK_flic_for_palette_seg = 0;
    int16_t unit_race = 0;  // _DI_
    int16_t DBG_entry_num = 0;

    if(type_or_other < 0)
    {
        unit_race = rt_Arcane;
    }
    else
    {
        unit_race = _unit_type_table[type_or_other].Race;
    }

    unit_race += 46;  // ¿ WTF ?  {0,...,20} + 46 = {46,...,66}

    Mark_Block(_screen_seg);

    // SPELLSCR.LBX, 009  "FLAMEFR1"    ""
    SPELLSCR_FLAMEFR1_seg = LBX_Reload_Next(spellscr_lbx_file__ovr137__1, 9, _screen_seg);

    // SPELLSCR.LBX, 046  "SUMMON02"    ""
    // SPELLSCR.LBX, 047  "SUMMON03"    ""
    // SPELLSCR.LBX, 048  "SUMMON04"    ""
    // SPELLSCR.LBX, 049  "SUMMON05"    ""
    // SPELLSCR.LBX, 050  "SUMMON06"    ""
    // SPELLSCR.LBX, 051  "SUMMON07"    ""
    // SPELLSCR.LBX, 052  "SUMMON08"    ""
    // SPELLSCR.LBX, 053  "SUMMON09"    ""
    // SPELLSCR.LBX, 054  "SUMMON10"    "horus"
    // SPELLSCR.LBX, 055  "SUMMON11"    "ariel"
    // SPELLSCR.LBX, 056  "SUMMON12"    "tlaloc"
    // SPELLSCR.LBX, 057  "SUMMON13"    "kali"
    // SPELLSCR.LBX, 058  "SMLRESCH"    ""
    // SPELLSCR.LBX, 059  "SUMMGREY"    ""
    // SPELLSCR.LBX, 060  "SUMGREEN"    ""
    // SPELLSCR.LBX, 061  "SUMMBLUE"    ""
    // SPELLSCR.LBX, 062  "SUMMRED"     ""
    // SPELLSCR.LBX, 063  "SUMWHITE"    ""
    // SPELLSCR.LBX, 064  "SUMMPURP"    ""
    // SPELLSCR.LBX, 065  "VORTEX1"     ""
    // SPELLSCR.LBX, 066  "VORTEX2"     ""
    IDK_flic_for_palette_seg = LBX_Reload_Next(spellscr_lbx_file__ovr137__1, unit_race, _screen_seg);

    Load_Palette_From_Animation(IDK_flic_for_palette_seg);

    Set_Page_Off();

    Main_Screen_Draw();

    FLIC_Draw(30, 42, SPELLSCR_FLAMEFR1_seg);

    Copy_Off_To_Back();

    Release_Block(_screen_seg);

    Mark_Block(_screen_seg);

    // SPELLSCR.LBX, 010  "GLOBALMK"    ""
    SPELLSCR_GLOBALMK_seg = LBX_Reload_Next(spellscr_lbx_file__ovr137__1, 10, _screen_seg);

    // SPELLSCR.LBX, 011  "ENCHANT"     ""
    SPELLSCR_ENCHANT_seg = LBX_Reload_Next(spellscr_lbx_file__ovr137__1, 11, _screen_seg);

    if(type_or_other > 0)
    {

        // MONSTER.LBX, 000  "MONSTER1" "Magic Spirit"
        // ...
        // MONSTER.LBX, 043  "MONSTER4" "Nagas"
        // IDK_MONSTER_seg = LBX_Reload_Next(monster_lbx_file__ovr137, (46 - (200 - spell_data_table[type_or_other].Param0)), _screen_seg);
        // DBG_entry_num = (46 - (200 - spell_data_table[type_or_other].Param0));
        // DBG_entry_num = (46 - (200 - spell_data_table[type_or_other].unit_type));
        DBG_entry_num = spell_data_table[type_or_other].Param0;
        DBG_entry_num = spell_data_table[type_or_other].unit_type;
        DBG_entry_num = (200 - DBG_entry_num);
        DBG_entry_num = (46 - DBG_entry_num);
        DBG_entry_num = (46 - (200 - type_or_other));
        IDK_MONSTER_seg = LBX_Reload_Next(monster_lbx_file__ovr137, DBG_entry_num, _screen_seg);

    }
    else
    {

        if(type_or_other == -1)
        {

            // MONSTER.LBX, 046  "MONSTER2" "Sword"
            IDK_MONSTER_seg = LBX_Reload_Next(monster_lbx_file__ovr137, 46, _screen_seg);

        }
        else if(
            (type_or_other == -2)
            ||
            (type_or_other == -20)
        )
        {

            // MONSTER.LBX, 045  "MONSTER4" "Male Hero"
            IDK_MONSTER_seg = LBX_Reload_Next(monster_lbx_file__ovr137, 45, _screen_seg);

        }
        else if(
            (type_or_other == -3)
            ||
            (type_or_other == -30)
        )
        {

            // MONSTER.LBX, 044  "MONSTER4" "Female Hero"
            IDK_MONSTER_seg = LBX_Reload_Next(monster_lbx_file__ovr137, 44, _screen_seg);

        }

    }

    IDK_wizard_id_thing_seg = LBX_Reload_Next(spellscr_lbx_file__ovr137__1, (46 + _players[player_idx].wizard_id), _screen_seg);

    IDK_flic_for_palette_seg = LBX_Reload_Next(spellscr_lbx_file__ovr137__1, unit_race, _screen_seg);

    Load_Palette_From_Animation(IDK_flic_for_palette_seg);

    Apply_Palette();

}


// WZD o137p02
// drake178: sub_BE9FF()
void IDK_SummonAnim_Draw(void)
{
    int16_t start_y = 0;  // _SI_
    int16_t start_x = 0;  // _DI_

    start_x = 30;
    start_y = 42;

    Copy_Back_To_Off();

    Set_Window((start_x + 7), (start_y + 8), SCREEN_XMAX, SCREEN_YMAX);

    Clipped_Draw((start_x + 8), (start_y + 3), IDK_wizard_id_thing_seg);

    FLIC_Draw((start_x + 55), (start_y + 54), SPELLSCR_GLOBALMK_seg);

    Set_Window(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, (start_y + 105));

    if(
        (_osc_anim_ctr < 30)
        &&
        (magic_set.spell_animations == ST_TRUE)
        &&
        (_osc_player_idx == HUMAN_PLAYER_IDX)
    )
    {

        Clipped_Draw((start_x + 76), (start_y + 110 - (_osc_anim_ctr * 3)), IDK_MONSTER_seg);

    }
    else
    {

        Clipped_Draw((start_x + 76), (start_y + 18), IDK_MONSTER_seg);

    }

    FLIC_Draw((start_x + 64), (start_y + 80), SPELLSCR_ENCHANT_seg);

    Set_Outline_Color(16);

    Set_Font_Style_Shadow_Down(4, 4, 0, 0);

    Set_Alias_Color(190);

    if(_osc_summon_unit_type > 0)
    {

        strcpy(GUI_NearMsgString, *_unit_type_table[spell_data_table[_players[_osc_player_idx].casting_spell_idx].unit_type].name);

        strcat(GUI_NearMsgString, aSummoned);

    }
    else
    {
        if(_osc_summon_unit_type == -1)
        {

            strcpy(GUI_NearMsgString, aItemEnchanted);

        }
        else
        {

            if(_osc_summon_unit_type > -10)
            {

                strcpy(GUI_NearMsgString, aHeroSummoned_0);

            }
            else
            {

                strcpy(GUI_NearMsgString, aChampionSumm_0);

            }

        }

    }

    Print_Centered((start_x + 90), (start_y + 117), GUI_NearMsgString);

}


// WZD o137p03
// drake178: sub_BEB92()
/*

¿ magic_realm_spell_idx is Spell_Rarity ?
    MultiPurpose_Local_Var = ((spell_idx - 1) % NUM_SPELLS_PER_MAGIC_REALM);
    MultiPurpose_Local_Var = (MultiPurpose_Local_Var / 10);

*/
void IDK_SummonAnim(int16_t unit_type, int16_t magic_realm_spell_idx, int16_t player_idx)
{
    int16_t anim_stg_cnt;
    int16_t input_field_idx;
    int16_t full_screen_field;

    // DOMSDOS  Stop_All_Sounds__STUB();

    SND_Spell_Music = LBX_Reload(music_lbx_file__ovr137, (MUSIC_Weak_Summon + magic_realm_spell_idx), SND_Music_Segment);
    SND_Spell_Music_size = lbxload_entry_length;

    if(magic_set.background_music == ST_TRUE)
    {

        // DOMSDOS  Play_Sound__WIP(SND_Spell_Music);
        sdl2_Play_Sound__WIP(SND_Spell_Music, SND_Spell_Music_size);

    }

    _osc_anim_ctr = 0;

    _osc_player_idx = player_idx;

    _osc_summon_unit_type = unit_type;

    Allocate_Reduced_Map();

    Full_Draw_Main_Screen();

    IDK_SummonAnim_Load(unit_type, player_idx);

    Assign_Auto_Function(IDK_SummonAnim_Draw, 2);

    _page_flip_effect = pfe_Dissolve;

    Clear_Fields();

    full_screen_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, empty_string__ovr137[0], ST_UNDEFINED);

    if(magic_set.spell_animations == ST_TRUE)
    {
        anim_stg_cnt = 130;
    }
    else
    {
        anim_stg_cnt = 40;
    }

    _osc_leave_screen = ST_FALSE;

    for(_osc_anim_ctr = 0; ((_osc_anim_ctr < anim_stg_cnt) && (_osc_leave_screen == ST_FALSE)); _osc_anim_ctr++)
    {

        Mark_Time();

        input_field_idx = Get_Input();

        if(input_field_idx == full_screen_field)
        {
            _osc_leave_screen = ST_TRUE;
        }

        Set_Page_Off();

        IDK_SummonAnim_Draw();

        PageFlip_FX();

        Release_Time(2);

        _page_flip_effect = pfe_None;

    }

    Deactivate_Auto_Function();

    OVL_MosaicFlip__STUB();

    Stop_All_Sounds__STUB();

    Release_Block(_screen_seg);

    // DOMSDOS  Play_Background_Music__STUB();
    sdl2_Play_Background_Music__WIP();

    if(player_idx == HUMAN_PLAYER_IDX)
    {
        Set_Mouse_List(1, mouse_list_default);
    }
    else
    {
        Set_Mouse_List(1, mouse_list_hourglass);
    }

}

// WZD o137p04
// drake178: sub_BECD9()
// AKA IDK_CastCityEnchMsg()
void Cast_Spell_City_Enchantment_Animation_Load(int16_t spell_idx,int16_t  player_idx)
{
    char string[26] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    Copy_On_To_Off_Page();

    // SPELLSCR.LBX, 073  "CITYSPEL"    ""
    cityspel_seg = LBX_Reload(spellscr_lbx_file__ovr137__2, 73, _screen_seg);

    Set_Page_Off();

    FLIC_Draw(53, 38, cityspel_seg);

    Set_Outline_Color(17);

    Set_Font_Style_Shadow_Down(5, 5, 0, 0);

    strcpy(GUI_NearMsgString, _city_size_names[_CITIES[_osc_city_idx].size]);  // { "Outpost", "Hamlet", "Village", "Town", "City", "Capital" }

    strcat(GUI_NearMsgString, cnst_Sp_Of_Sp_5);  // " of "

    _fstrcpy(string, _CITIES[_osc_city_idx].name);  // [YOUR CITY NAME HERE]

    strcat(GUI_NearMsgString, string);

    Print_Centered(160, 44, GUI_NearMsgString);

    Set_Outline_Color(17);

    Set_Font_Style_Shadow_Down(4, 4, 0, 0);

    Set_Alias_Color(24);

    if(player_idx == HUMAN_PLAYER_IDX)
    {

        strcpy(GUI_NearMsgString, aYouCast);  // "You cast "

    }
    else
    {

        strcpy(GUI_NearMsgString, _players[player_idx].name);

        strcat(GUI_NearMsgString, aCasts);  // " casts "

    }

    _fstrcpy(string, spell_data_table[spell_idx].name);

    strcat(GUI_NearMsgString, string);

    Print_Centered(160, 164, GUI_NearMsgString);

    Copy_Off_To_Back();

    Near_Allocate_Mark();

    city_cityscape_fields = Near_Allocate_First(144);

}

// WZD o137p05
// drake178: sub_BEE75()
// AKA IDK_City_sBEE75()
// AKA IDK_Spell_Cityscape_Draw()
/*

~ City_ovr55.C  // WZD o55p04  Cityscape_Draw_Scanned_Building_Name()
*/
void Cast_Spell_City_Enchantment_Animation_Draw(void)
{
    int16_t y_start = 0;
    int16_t x_start = 0;
    int16_t itr = 0;  // _SI_
    struct s_BLDG * cityscape_bldg_field = 0;

    x_start = 53;
    y_start = 38;

    Copy_Back_To_Off();

    Cityscape_Window__WIP(_osc_city_idx, (x_start + 5),(y_start + 27), _ce_bldg_idx, _ce_bldg_idx);

    for(itr = 0; itr < city_cityscape_field_count; itr++)
    {
        assert(cityscape_bldg_fields[itr].field_bldg_idx >= 0);
        assert(cityscape_bldg_fields[itr].field_bldg_idx <= 35);

        if(city_cityscape_fields[itr] == _osc_scanned_field)
        {

            cityscape_bldg_field = &cityscape_bldg_fields[itr];
            // cityscape_bldg_fields[itr].field_bldg_idx
            Set_Font_Style_Outline(1, 0, 0, 0);
            Set_Alias_Color(8);
            if(cityscape_bldg_fields[itr].field_bldg_idx >= bt_NUM_BUILDINGS)
            {
                Print_Centered(cityscape_bldg_fields[itr].field_print_sx, cityscape_bldg_fields[itr].field_print_sy, _magic_building_names[(cityscape_bldg_fields[itr].field_bldg_idx - bt_NUM_BUILDINGS)]);
            }
            else
            {
// Severity	Code	Description	Project	File	Line	Suppression State	Details
// Warning	C6385	Reading invalid data from '_CITIES[_temp_sint_1].bldg_status'.	sdl2_ReMoM	C:\STU\devel\ReMoM\src\Spells137.C	661		
                assert(cityscape_bldg_fields[itr].field_bldg_idx >= 0);
                assert(cityscape_bldg_fields[itr].field_bldg_idx <= 35);
                if(cityscape_bldg_fields[itr].field_bldg_idx < 0)
                {
                    STU_DEBUG_BREAK();
                }
                if(cityscape_bldg_fields[itr].field_bldg_idx > 35)
                {
                    STU_DEBUG_BREAK();
                }
                if(_CITIES[_osc_city_idx].bldg_status[cityscape_bldg_fields[itr].field_bldg_idx] != bs_Removed)
                {
                    Print_Centered_Far(cityscape_bldg_fields[itr].field_print_sx, cityscape_bldg_fields[itr].field_print_sy, bldg_data_table[cityscape_bldg_fields[itr].field_bldg_idx].name);
                }
                else
                {
                    Set_Font_Style_Outline(1, 2, 0, 0);
                    Set_Alias_Color(166);
                    Print_Centered_Far(cityscape_bldg_fields[itr].field_print_sx, (cityscape_bldg_fields[itr].field_print_sy + 6), aDestroyed);
                    Print_Centered_Far(cityscape_bldg_fields[itr].field_print_sx, cityscape_bldg_fields[itr].field_print_sy, bldg_data_table[cityscape_bldg_fields[itr].field_bldg_idx].name);
                }
            }
        }
    }

}


// WZD o137p06
// drake178: sub_BF048()
void Cast_Spell_City_Enchantment_Animation_1__WIP(int16_t city_idx, int16_t spell_idx, int16_t player_idx)
{
    int16_t sw_spell_idx = 0;
    int16_t good_bad = 0;
    int16_t OT = 0;
    int16_t OL = 0;
    int16_t itr = 0;  // _SI_

    Stop_All_Sounds__STUB();

    if(spell_data_table[spell_idx].type == scc_City_Enchantment_Negative)
    {
        good_bad = 6;
    }
    else  /* (spell_data_table[spell_idx].type == scc_City_Enchantment_Positive) */
    {
        good_bad = 0;
    }

    Cast_Spell_City_Enchantment_Animation_Load_Music(spell_idx, good_bad);

    if(magic_set.background_music == ST_TRUE)
    {
        // DOMSDOS  Play_Sound__STUB(SND_Spell_Music);
        sdl2_Play_Sound__WIP(SND_Spell_Music, SND_Spell_Music_size);

    }

    _osc_need_target_flag = ST_TRUE;

    _osc_city_idx = city_idx;

    _osc_scanned_field = INVALID_FIELD;

    sw_spell_idx = spell_idx;

    switch(sw_spell_idx)
    {
        case spl_Move_Fortress:    { _ce_bldg_idx = bt_Fortress; } break;
        case spl_Earth_Gate:       { _ce_bldg_idx = bt_Earth_Gate; } break;
        case spl_Stream_Of_Life:   { _ce_bldg_idx = bt_Stream_Of_Life; } break;
        case spl_Astral_Gate:      { _ce_bldg_idx = bt_Astral_Gate; } break;
        case spl_Summoning_Circle: { _ce_bldg_idx = bt_Summoning_Circle; } break;
        case spl_Dark_Rituals:     { _ce_bldg_idx = bt_Dark_Rituals; } break;
        case spl_Altar_Of_Battle:  { _ce_bldg_idx = bt_Altar_Of_Battle; } break;
        default: { _ce_bldg_idx = bt_NONE; } break;
    }

    Cast_Spell_City_Enchantment_Animation_Load(spell_idx, player_idx);

    Reset_First_Block(_screen_seg);

    Assign_Auto_Function(Cast_Spell_City_Enchantment_Animation_Draw, 2);

    _osc_need_target_flag = 0;

    Set_Page_Off();

    Cast_Spell_City_Enchantment_Animation_Draw();

    Copy_Off_To_Page4();

    _osc_need_target_flag = 1;

    Set_Page_Off();

    Cast_Spell_City_Enchantment_Animation_Draw();

    _osc_need_target_flag = 0;

    if(magic_set.spell_animations != ST_FALSE)
    {

        OL = _CITIES[city_idx].wx;
        OT = _CITIES[city_idx].wy;

        World_To_Screen(_map_x, _map_y, &OL, &OT);

        // TODO  HLP_NoResizeExpand__TODO(53, 38, 268, 162, OL, OT, (OL + 20), (OT + 18), 0);

        Set_Page_Off();

        Copy_Page4_To_Off();

        Copy_Off_To_Back();

        PageFlip_FX();

        if(spell_idx == spl_Earthquake)
        {

            Spell_Animation_Load_Sound_Effect__WIP(spell_idx);

            Set_Page_Off();

            Cast_Spell_City_Enchantment_Animation_Draw();

            Copy_Off_To_Back();

            if(SND_SpellCast != (SAMB_ptr)ST_UNDEFINED)
            {
                // DOMSDOS  Play_Sound__STUB(SND_SpellCast)
                sdl2_Play_Sound__WIP(SND_SpellCast, SND_SpellCast_size);
            }

            for(itr = 35; itr > 5; itr--)
            {
            
                // TODO  SPELLY  GRRRFUCKS  VGA_PartCopyFromF3((Random(itr) / 5), (Random(itr) / 5));
            
                PageFlip_FX();
            
            }

            Set_Page_Off();

            Cast_Spell_City_Enchantment_Animation_Draw();

            PageFlip_FX();

            SND_SpellCast = (SAMB_ptr)ST_UNDEFINED;

        }
        else
        {
            
            for(itr = 0; itr < 8; itr++)
            {

                Set_Page_Off();

                Cast_Spell_City_Enchantment_Animation_Draw();

                PageFlip_FX();

            }

        }

        Near_Allocate_Undo();

        Clear_Fields();

        Deactivate_Auto_Function();

        Cityscape_Build_Anim_Reset();

    }

}


// WZD o137p07
// drake178: sub_BF2AF()
void Cast_Spell_City_Enchantment_Animation_2__WIP(int16_t city_idx, int16_t spell_idx, int16_t player_idx)
{
    int16_t y2 = 0;
    int16_t y1 = 0;
    int16_t x2 = 0;
    int16_t x1 = 0;
    int16_t fullscreen_field = 0;
    int16_t itr = 0;  // _SI_

    _osc_city_idx = city_idx;

    if(spell_idx != spl_Earthquake)
    {

        Reset_First_Block(_screen_seg);

        Spell_Animation_Load_Sound_Effect__WIP(spell_idx);

    }

    _ce_bldg_idx = bt_NONE;

    _page_flip_effect = pfe_Dissolve;

    Set_Page_Off();

    Cast_Spell_City_Enchantment_Animation_Draw();

    PageFlip_FX();


    if(
        (spell_idx != spl_Earthquake)
        &&
        (SND_SpellCast != (SAMB_ptr)ST_UNDEFINED)
    )
    {

        // DOMSDOS  Play_Sound__STUB(SND_SpellCast);
        sdl2_Play_Sound__WIP(SND_SpellCast, SND_SpellCast_size);

    }

    Assign_Auto_Function(Cast_Spell_City_Enchantment_Animation_Draw, 2);

    Clear_Fields();

    for(itr = 0; itr < cityscape_bldg_count; itr++)
    {

        x1 = cityscape_bldg_fields[itr].field_x1;
        y1 = cityscape_bldg_fields[itr].field_y1;
        x2 = cityscape_bldg_fields[itr].field_x2;
        y2 = cityscape_bldg_fields[itr].field_y2;

        city_cityscape_fields[city_cityscape_field_count] = Add_Hidden_Field(x1, y1, x2, y2, (int16_t)empty_string__ovr137[0], ST_UNDEFINED);

        city_cityscape_field_count++;

    }

    fullscreen_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, (int16_t)empty_string__ovr137[0], ST_UNDEFINED);

    for(itr = 0; itr < 250; itr++)
    {

        if(Get_Input() != ST_FALSE)
        {
            break;
        }

        _osc_scanned_field = Scan_Input();

        Mark_Time();

        Set_Page_Off();

        Cast_Spell_City_Enchantment_Animation_Draw();

        PageFlip_FX();

        Release_Time(2);

    }

    Near_Allocate_Undo();

    Release_Block(_screen_seg);

    Clear_Fields();

    // DOMSDOS  Stop_All_Sounds__STUB();

    // DOMSDOS  Play_Background_Music__STUB();
    sdl2_Play_Background_Music__WIP();

    Deactivate_Auto_Function();

    Allocate_Reduced_Map();

    OVL_MosaicFlip__STUB();

}


// WZD o137p08
// drake178: OVL_LoadGlobalAnim()
/*
; appends into the sandbox the player's moodwiz
; portrait, the diplomacy mirror, two 12320 byte
; working buffers, and two more images based on the
; current animation frame:
;   0-9:   mirror pane + first mask sequence
;   10-19: mirror pane + second mask sequence
;   20:    global image + first mask sequence
;   30:    global image + second mask sequence
*/
/*

*/
void OVL_LoadGlobalAnim(int16_t spell_idx, int16_t player_idx)
{
    int16_t specfx_entry_num = 0;  // _SI_

    // MOODWIZ.LBX, 000  "MERLFACE"  "g,b,n merlin"
    // MOODWIZ.LBX, 001  "SHAMFACE"  "g,b,n raven"
    // MOODWIZ.LBX, 002  "PRIEFACE"  "g,b,n sharee"
    // MOODWIZ.LBX, 003  "WUFACE"    "g,b,n lopan"
    // MOODWIZ.LBX, 004  "ARABFACE"  "g,b,n enki"
    // MOODWIZ.LBX, 005  "OBERFACE"  "g,b,n alric"
    // MOODWIZ.LBX, 006  "WRATFACE"  "g,b,n druga"
    // MOODWIZ.LBX, 007  "DRACFACE"  "g,b,n sss'ra"
    // MOODWIZ.LBX, 008  "NMOIFACE"  "g,b,n horus"
    // MOODWIZ.LBX, 009  "FREYFACE"  "g,b,n freya"
    // MOODWIZ.LBX, 010  "GALEFACE"  "g,b,n ahto?"
    // MOODWIZ.LBX, 011  "ARIELFAC"  "g,b,n ariel"
    // MOODWIZ.LBX, 012  "AZTEFACE"  "g,b,n tlaloc"
    // MOODWIZ.LBX, 013  "KARLFACE"  "g,b,n kali        "
    ge_anim_moodwiz_seg = LBX_Reload_Next(moodwiz_lbx_file__ovr137, _players[player_idx].wizard_id, _screen_seg);

    // BACKGRND.LBX, 018    "MIRROR"      "diplomacy mirror"
    diplomacy_mirror_seg = LBX_Reload_Next(backgrnd_lbx_file__ovr137, 18, _screen_seg);

    if(spell_idx == spl_Death_Wish)
    {
        specfx_entry_num = 23;
    }
    else if(spell_idx == spl_Great_Unsummoning)
    {
        specfx_entry_num = 24;
    }
    else if(spell_idx == spl_Awareness)
    {
        specfx_entry_num = 41;
    }
    else
    {
        specfx_entry_num = spell_data_table[spell_idx].Param0;
    }

    IMG_SBK_Anims = Allocate_Next_Block(_screen_seg, 770);  // 770 PR, 12320 B

    IMG_SBK_PageText = Allocate_Next_Block(_screen_seg, 770);  // 770 PR, 12320 B

    if(
        (GAME_MP_SpellVar_1 == 20)
        ||
        (magic_set.spell_animations == ST_FALSE)
    )
    {

        spell_animation_seg = LBX_Reload_Next(specfx_lbx_file__ovr137, (15 + specfx_entry_num), _screen_seg);

        // SPECFX.LBX, 054  "MASK1"     ""
        GAME_MP_SpellVar_2 = LBX_Reload_Next(specfx_lbx_file__ovr137, 54, _screen_seg);

        Reset_Animation_Frame(GAME_MP_SpellVar_2);

    }
    else if(GAME_MP_SpellVar_1 == 30)
    {

        spell_animation_seg = LBX_Reload_Next(specfx_lbx_file__ovr137, (15 + specfx_entry_num), _screen_seg);

        // SPECFX.LBX, 055  "MASK2"     ""
        GAME_MP_SpellVar_2 = LBX_Reload_Next(specfx_lbx_file__ovr137, 55, _screen_seg);

        Reset_Animation_Frame(GAME_MP_SpellVar_2);

    }
    else if(GAME_MP_SpellVar_1 < 10)
    {

        // DIPLOMAC.LBX, 001    "PANE"      "mirror pane"
        IMG_SBK_SliderBG = LBX_Reload_Next(diplomac_lbx_file__ovr137, 1, _screen_seg);

        // SPECFX.LBX, 054  "MASK1"     ""
        GAME_MP_SpellVar_2 = LBX_Reload_Next(specfx_lbx_file__ovr137, 54, _screen_seg);

    }
    else if(GAME_MP_SpellVar_1 < 20)
    {

        // DIPLOMAC.LBX, 001    "PANE"      "mirror pane"
        IMG_SBK_SliderBG = LBX_Reload_Next(diplomac_lbx_file__ovr137, 1, _screen_seg);

        // SPECFX.LBX, 055  "MASK2"     ""
        GAME_MP_SpellVar_2 = LBX_Reload_Next(specfx_lbx_file__ovr137, 55, _screen_seg);

        Reset_Animation_Frame(GAME_MP_SpellVar_2);
        
    }

}


// WZD o137p09
// drake178: OVL_DrawGlobalAnim()
/*
; redraws the main overland view into the current draw
; segment, then composes the global spell animation
; over it
*/
/*

XREF:
    WIZ_GlobalSpellAnim()
    j_OVL_DrawGlobalAnim()
        WIZ_GlobalSpellAnim()

*/
void OVL_DrawGlobalAnim(void)
{
    int16_t start_x = 0;  // _SI_
    int16_t start_y = 0;  // _DI_

    start_x = 58;
    start_y = 28;

    if(_osc_leave_screen == ST_TRUE)
    {
        return;
    }

    Main_Screen_Draw();

    Set_Outline_Color(0);

    Set_Font_Style_Outline(5, 0, 0, 0);  // fancy script

    if(magic_set.spell_animations != ST_TRUE)
    {

        if(_osc_player_idx == HUMAN_PLAYER_IDX)
        {

            strcpy(GUI_NearMsgString, cnst_Spellcast_Msg_1);  // "You have completed casting..."

        }
        else
        {

            strcpy(GUI_NearMsgString, _players[_osc_player_idx].name);

            strcat(GUI_NearMsgString, cnst_Spellcast_Msg_2);  // " has cast..."

        }

        Print_Centered((start_x + 62), (start_y + 133), GUI_NearMsgString);

        FLIC_Draw(start_x, start_y, diplomacy_mirror_seg);

        FLIC_Draw((start_x + 10), (start_y + 8), spell_animation_seg);

        Print_Centered_Far((start_x + 62), (start_y + 153), spell_data_table[SBK_Spell_Index].name);

    }
    else  /* (magic_set.spell_animations != ST_TRUE) */
    {

        if(GAME_MP_SpellVar_1 < 34)
        {

            if(_osc_player_idx == 0)
            {

                strcpy(GUI_NearMsgString, cnst_Spellcast_Msg_1);

            }
            else
            {

                strcpy(GUI_NearMsgString, _players[_osc_player_idx].name);

                strcat(GUI_NearMsgString, cnst_Spellcast_Msg_2);

            }

            Print_Centered((start_x + 62), (start_y + 133), GUI_NearMsgString);

        }

        if(GAME_MP_SpellVar_1 < 14)
        {

            Reset_Animation_Frame(ge_anim_moodwiz_seg);

            FLIC_Draw((start_x + 12), (start_y + 12), ge_anim_moodwiz_seg);

            Draw_Picture_To_Bitmap(GAME_MP_SpellVar_2, IMG_SBK_Anims);

            Draw_Picture_To_Bitmap(IMG_SBK_SliderBG, IMG_SBK_PageText);

            Clipped_Copy_Mask(0, 0, IMG_SBK_PageText, IMG_SBK_Anims);

            Draw_Picture((start_x + 12), (start_y + 12), IMG_SBK_PageText);

            FLIC_Draw(start_x, start_y, diplomacy_mirror_seg);

        }
        else if(GAME_MP_SpellVar_1 < 20)
        {

            FLIC_Draw((start_x + 12), (start_y + 12), IMG_SBK_SliderBG);

            Reset_Animation_Frame(ge_anim_moodwiz_seg);

            FLIC_Draw((start_x + 12), (start_y + 12), ge_anim_moodwiz_seg);

            FLIC_Draw(start_x, start_y, diplomacy_mirror_seg);

        }
        else if(GAME_MP_SpellVar_1 < 34)
        {

            FLIC_Draw((start_x + 10), (start_y + 8), spell_animation_seg);

            Draw_Picture_To_Bitmap(GAME_MP_SpellVar_2, IMG_SBK_Anims);

            Reset_Animation_Frame(ge_anim_moodwiz_seg);

            Draw_Picture_To_Bitmap(ge_anim_moodwiz_seg, IMG_SBK_PageText);

            Clipped_Copy_Mask(0, 0, IMG_SBK_PageText, IMG_SBK_Anims);

            Draw_Picture((start_x + 12), (start_y + 12), IMG_SBK_PageText);

            FLIC_Draw(start_x, start_y, diplomacy_mirror_seg);

        }
        else
        {

            FLIC_Draw(start_x, start_y, diplomacy_mirror_seg);

            FLIC_Draw((start_x + 10), (start_y + 8), spell_animation_seg);

            Print_Centered_Far((start_x + 62), (start_y + 133), spell_data_table[SBK_Spell_Index].name);

        }


    }


}


// WZD o137p10
// drake178: WIZ_GlobalSpellAnim()
/*
; plays the global spell cast animation for the
; selected wizard and spell, which can be clicked away
; but will also end and return automatically
*/
/*
Spell Cast Animation - Global Enchantment
Global Enchantment Animation



*/
void WIZ_GlobalSpellAnim(int16_t player_idx, int16_t spell_idx)
{
    int16_t music_entry_number = 0;
    int16_t fullscreen_ESC_field = 0;
    int16_t input_field_idx = 0;

    // DOMSDOS  Stop_All_Sounds__STUB();

    if(spell_idx == spl_Death_Wish)
    {
        music_entry_number = MUSIC_Wind_Mastery;
    }
    else if(spell_idx == spl_Great_Unsummoning)
    {
        music_entry_number = MUSIC_Suppress_Magic;
    }
    else
    {
        music_entry_number = spell_data_table[spell_idx].Param0;
    }

    if(music_entry_number > 21)
    {
        // MUSIC.LBX, 040 "MOM51 XM"    "New Spell music"
        SND_Spell_Music = LBX_Reload(music_lbx_file__ovr137, MUSIC_New_Spell, SND_Music_Segment);
        SND_Spell_Music_size = lbxload_entry_length;
    }
    else
    {
        // MUSIC.LBX, 019 "MOM35 XM"    "Eternal Night  DEATH"
        // MUSIC.LBX, 020 "MOM36 XM"    "Evil Omens"
        // MUSIC.LBX, 021 "MOM37 XM"    "Zombie Mastery"
        // MUSIC.LBX, 022 "MOM31 XM"    "Aura of Majesty  SORCE"
        // MUSIC.LBX, 023 "MOM46 XM"    "Wind Mastery "
        // MUSIC.LBX, 024 "MOM47 XM"    "Supress Magic"
        // MUSIC.LBX, 025 "MOM48 XM"    "Time Stop"
        // MUSIC.LBX, 026 "MOM43 XM"    "Nature's Awareness NAT"
        // MUSIC.LBX, 027 "MOM45 XM"    "Force of Nature"
        // MUSIC.LBX, 028 "MOM44 XM"    "Herb Mastery"
        // MUSIC.LBX, 029 "MOM30 XM"    "Chaos Surge  CHAOS"
        // MUSIC.LBX, 030 "MOM32 XM"    "Doom Mastery"
        // MUSIC.LBX, 031 "MOM29 XM"    "Great Wasting"
        // MUSIC.LBX, 032 "MOM33 XM"    "Meteor Storm"
        // MUSIC.LBX, 033 "MOM34 XM"    "Armagedon"
        // MUSIC.LBX, 034 "MOM41 XM"    "Tranquility    LIFE"
        // MUSIC.LBX, 035 "MOM40 XM"    "Life Force"
        // MUSIC.LBX, 036 "MOM42 XM"    "Crusade"
        // MUSIC.LBX, 037 "MOM38 XM"    "Just Cause"
        // MUSIC.LBX, 038 "MOM39 XM"    "Holy Arms"
        // MUSIC.LBX, 039 "TEMP XMI"    "Wrath of God"
        SND_Spell_Music = LBX_Reload(music_lbx_file__ovr137, (19 + music_entry_number), SND_Music_Segment);
        SND_Spell_Music_size = lbxload_entry_length;
    }

    if(magic_set.background_music == ST_TRUE)
    {

        // DOMSDOS  Play_Sound__WIP(SND_Spell_Music);
        sdl2_Play_Sound__WIP(SND_Spell_Music, SND_Spell_Music_size);

    }

    GAME_MP_SpellVar_1 = 0;

    _osc_player_idx = player_idx;

    SBK_Spell_Index = spell_idx;

    Reset_First_Block(_screen_seg);

    Allocate_Reduced_Map();

    Mark_Block(_screen_seg);

    OVL_LoadGlobalAnim(spell_idx, player_idx);

    Clear_Fields();

    fullscreen_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, empty_string__ovr137[0], ST_UNDEFINED);

    Assign_Auto_Function(OVL_DrawGlobalAnim, 2);

    _osc_leave_screen = ST_FALSE;

    for(GAME_MP_SpellVar_1 = 0; ((GAME_MP_SpellVar_1 < 120) && (_osc_leave_screen == 0)); GAME_MP_SpellVar_1++)
    {

        input_field_idx = abs(Get_Input());

        if(input_field_idx == fullscreen_ESC_field)
        {

            _osc_leave_screen = ST_TRUE;

        }

        if(_osc_leave_screen != ST_TRUE)
        {

            if(GAME_MP_SpellVar_1 == 10)
            {

                Release_Block(_screen_seg);

                Mark_Block(_screen_seg);

                OVL_LoadGlobalAnim(spell_idx, player_idx);

            }
            else if(GAME_MP_SpellVar_1 == 20)
            {

                Release_Block(_screen_seg);

                Mark_Block(_screen_seg);

                OVL_LoadGlobalAnim(spell_idx, player_idx);

            }
            else if(GAME_MP_SpellVar_1 == 30)
            {

                Release_Block(_screen_seg);

                Mark_Block(_screen_seg);

                OVL_LoadGlobalAnim(spell_idx, player_idx);

            }

            if(GAME_MP_SpellVar_1 == 34)
            {
                _page_flip_effect = 3;
            }

            Mark_Time();  // DNE in Dasm

            Set_Page_Off();

            OVL_DrawGlobalAnim();

            PageFlip_FX();

            Release_Time(2);

        }

    }

    Release_Block(_screen_seg);

    Stop_All_Sounds__STUB();

    // DOMSDOS  Play_Background_Music__STUB();
    sdl2_Play_Background_Music__WIP();

    OVL_MosaicFlip__STUB();

}


// WZD o137p11
static void Target_Wizard_Screen_Load(int16_t spell_idx)
{
    int16_t y = 0;
    int16_t x = 0;
    int16_t y_start = 0;
    int16_t x_start = 0;
    int16_t itr2 = 0;  // _SI_
    int16_t itr1 = 0;  // _DI_

    x_start = 50;
    y_start = 20;

    Allocate_Reduced_Map();

    if(spell_idx != spl_Spell_Blast)
    {
        // SPELLSCR.LBX, 000  "WIZBACK"     ""
        IMG_OVL_TargetWizBG = LBX_Reload_Next(spellscr_lbx_file__ovr137__3, 0, _screen_seg);
    }
    else
    {
        // SPELLSCR.LBX, 072  "WIZBLAST"    ""
        IMG_OVL_TargetWizBG = LBX_Reload_Next(spellscr_lbx_file__ovr137__3, 72, _screen_seg);
    }

    // SPELLSCR.LBX, 071  "WIZBUTTN"    ""
    IMG_OVL_TrgtWizCncl = LBX_Reload_Next(spellscr_lbx_file__ovr137__1, 71, _screen_seg);

    // MAGIC.LBX, 006  "GEMS"      "grey gem"
    grey_gem_seg = LBX_Reload_Next(magic_lbx_file__ovr137, 6, _screen_seg);

    // MAGIC.LBX, 051  "GEMS"      "broken grey gem"
    broken_grey_gem_seg = LBX_Reload_Next(magic_lbx_file__ovr137, 51, _screen_seg);

    for(itr1 = 1; itr1 < _num_players; itr1++)
    {
        lilwiz_gem_segs[itr1] = LBX_Reload_Next(lilwiz_lbx_file__ovr137, ((_players[itr1].wizard_id * 5) + _players[itr1].banner_id), _screen_seg);
    }

    Set_Page_Off();

    Main_Screen_Draw();

    FLIC_Draw((x_start - 5), (y_start - 10), IMG_OVL_TargetWizBG);

    Set_Font_Style(0, 3, 0, 0);

    Set_Alias_Color(249);

    for(itr2 = 0; itr2 < 4; itr2++)
    {
        x = (x_start + 19 + (itr2 % 2) * 77);
        y = (y_start + 27 + (itr2 / 2) * 61);
        if(
            (_players[HUMAN_PLAYER_IDX].Dipl.Contacted[(1 + itr2)] == ST_TRUE)
            &&
            (_FORTRESSES[(1 + itr2)].active == ST_TRUE)
        )
        {
            FLIC_Draw(x, y, lilwiz_gem_segs[(1 + itr2)]);
        }
        else
        {

            if(_FORTRESSES[(1 + itr2)].active == ST_TRUE)
            {
                FLIC_Draw(x, y, grey_gem_seg);
            }
            else
            {
                FLIC_Draw(x, y, broken_grey_gem_seg);
            }
        }
    }

    Copy_Off_To_Back();

    Set_Page_On();

    // HLPENTRY.LBX, 037  ""  "Target Wizard Help"
    LBX_Load_Data_Static(hlpentry_lbx_file__ovr137, 37, (SAMB_ptr)&_help_entries[0], 0, 7, 10);

}


// WZD o137p12
/*
"Choose target for a " ... " spell"
*/
static void Target_Wizard_Screen_Draw(void)
{
    char string[LEN_SPELL_NAME] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t var_14 = 0;
    char buffer[6] = { 0, 0, 0, 0, 0, 0 };
    uint8_t colors[6] = { 0, 0, 0, 0, 0, 0 };
    int16_t y = 0;
    int16_t x = 0;
    int16_t itr = 0;
    int16_t x_start = 0;  // _SI_
    int16_t y_start = 0;  // _DI_

    x_start = 50;
    y_start = 20;

    colors[0] = 249;

    for(itr = 1; itr < 6; itr++)
    {
        colors[itr] = (177 + itr);
    }

    Copy_Back_To_Off();

    Set_Font_Style(0, 3, 0, 0);

    Set_Alias_Color(249);

    for(itr = 0; itr < NUM_GEMS; itr++)
    {

        x = (x_start + 19 + ((itr % 2) * 77));

        y = (y_start + 27 + ((itr / 2) * 61));

        if(
            (_players[itr].Dipl.Contacted[1] == ST_TRUE)
            &&
            (_FORTRESSES[itr].active == ST_TRUE)
        )
        {
            
            if(SBK_Spell_Index != spl_Spell_Blast)
            {

                var_14 = ((_players[itr].Dipl.Visible_Rel[(1 + _human_player_idx)] + 100) / 20);

                Print_Centered((x + 21), (y + 49), diplo_state[var_14]);

            }
            else
            {

                if(gem_player_nums[itr] == _temp_sint_4)
                {
                    Set_Outline_Color(254);
                    Set_Font_Colors_15(4, &colors[0]);
                    SDL_itoa((_players[(1 + itr)].casting_cost_original - _players[(1 + itr)].casting_cost_remaining), string, 10);
                    strcat(string, cnst_Space_MP_3);
                    Print_Centered((x_start + 39), (y_start + 149), string);
                }
                else
                {
                    Set_Font_Style(0, 3, 0, 0);
                    Set_Alias_Color(249);
                    Print_Centered_Far((x + 21), (y + 49), spell_data_table[_players[(1 + itr)].casting_spell_idx].name);
                }

            }

        }

    }

    Set_Outline_Color(151);

    Set_Font_Colors_15(4, &colors[0]);

    Set_Font_Style_Shadow_Down(4, 15, 0, 0);

    Set_Font_LF(1);

    if(_osc_need_target_flag == ST_TRUE)
    {

        strcpy(GUI_NearMsgString, aChooseTargetFo);  // "Choose target for a "

        _fstrcpy(string, spell_data_table[SBK_Spell_Index].name);

        strcat(GUI_NearMsgString, string);

        strcat(GUI_NearMsgString, aSpell_0);  // " spell"

    }
    else
    {

        strcpy(GUI_NearMsgString, _players[_temp_sint_1].name);

        switch(spell_data_table[SBK_Spell_Index].Param0)
        {
            case 0:
            {
                strcat(GUI_NearMsgString, aHasBeenSpellBl);  // " has been spell blasted"
            } break;
            case 1:
            {
                strcat(GUI_NearMsgString, aLoses);  // " loses "
                SDL_itoa(GAME_MP_SpellVar_1, buffer, 10);
                strcat(GUI_NearMsgString, buffer);
                strcat(GUI_NearMsgString, aPointsOfCastin);  // " points of casting ability"
            } break;
            case 2:
            {
                strcat(GUI_NearMsgString, aLoses);  // " loses "
                SDL_itoa(GAME_MP_SpellVar_1, buffer, 10);
                strcat(GUI_NearMsgString, buffer);
                strcat(GUI_NearMsgString, aPointsOfMana);  // " points of mana"
            } break;
            case 3:
            {
                if(GUI_NearMsgString[(strlen(GUI_NearMsgString) - 1)] == 's')
                {
                    strcat(GUI_NearMsgString, cnst_Apostrophe_3);  // "' "
                }
                else
                {
                    strcat(GUI_NearMsgString, cnst_Possessive_3);  // "'s "
                }
                strcat(GUI_NearMsgString, aRelationsHaveB);  // " relations have been subverted"
            } break;
            default:
            {
                STU_DEBUG_BREAK();
            } break;
        }
    }

    Print_Paragraph((x_start + 5), (y_start - 1), 144, GUI_NearMsgString , 2);

}

// WZD o137p13
/*

"Choose target for a " ... " spell"

*/
int16_t Target_Wizard_Screen(int16_t spell_idx)
{
    int16_t var_10 = 0;
    int16_t var_E = 0;
    int16_t G_Cancel_Button_Index = 0;
    int16_t y = 0;
    int16_t x = 0;
    int16_t input_field_idx = 0;
    int16_t y_start = 0;
    int16_t x_start = 0;
    int16_t player_idx = 0;  // _DI_
    int16_t itr = 0;  // _SI_

#define ADD_FIELDS_LOCAL()  \
    do {  \
        Clear_Fields();  \
        for(itr = 0; itr < 4; itr++)  \
        {  \
            gem_player_nums[itr] = Add_Hidden_Field((x_start + 17 + ((itr % 2) * 77)), (y_start + 27 + ((itr / 2) * 61)), (x_start + 62 + ((itr % 2) * 77)), (y_start + 72 + ((itr / 2) * 61)), (int16_t)empty_string__ovr137[0], ST_UNDEFINED);  \
        }  \
        if(spell_idx != spl_Spell_Blast)  \
        {  \
            G_Cancel_Button_Index = Add_Button_Field((x_start + 40), (y_start + 145), empty_string__ovr137, IMG_OVL_TrgtWizCncl, (int16_t)str_hotkey_ESC__ovr137[0], ST_UNDEFINED);  \
            _help_entries[1].help_idx = ST_UNDEFINED;  \
            _help_entries[2].help_idx = ST_UNDEFINED;  \
            for(itr = 0; itr < 4; itr++)  \
            {  \
                _help_entries[(3 + itr)].help_idx = HLP_RELATIONS;  \
            }  \
        }  \
        else  \
        {  \
            G_Cancel_Button_Index = Add_Button_Field((x_start + 78), (y_start + 145), empty_string__ovr137, IMG_OVL_TrgtWizCncl, (int16_t)str_hotkey_ESC__ovr137[0], ST_UNDEFINED);  \
            _help_entries[0].help_idx = ST_UNDEFINED;  \
            for(itr = 0; itr < 4; itr++)  \
            {  \
                if(_players[(1 + itr)].casting_spell_idx == spl_NONE)  \
                {  \
                    _help_entries[(3 + itr)].help_idx = HLP_NONE;  \
                }  \
                else  \
                {  \
                    _help_entries[(3 + itr)].help_idx = _players[(1 + itr)].casting_spell_idx;  \
                }  \
            }  \
        }  \
        Set_Help_List(&_help_entries[0], 7);  \
    } while(0)

    var_10 = ST_FALSE;

    x_start = 50;
    y_start = 20;

    SBK_Spell_Index = spell_idx;

    _osc_need_target_flag = ST_TRUE;

    Target_Wizard_Screen_Load(spell_idx);

    Assign_Auto_Function(Target_Wizard_Screen_Draw, 2);

    ADD_FIELDS_LOCAL();

    _osc_leave_screen = ST_FALSE;

    while(_osc_leave_screen == ST_FALSE)
    {

        input_field_idx = Get_Input();

        _osc_scanned_field = Scan_Input();

        if(input_field_idx == G_Cancel_Button_Index)
        {
            _osc_leave_screen = ST_TRUE;
            var_10 = ST_TRUE;
        }

        for(itr = 0; itr < 4; itr++)
        {

            if(
                (gem_player_nums[itr] == input_field_idx)
                &&
                (_players[HUMAN_PLAYER_IDX].Dipl.Contacted[(1 + itr)] == ST_TRUE)
                &&
                (_FORTRESSES[(1 + itr)].active == ST_TRUE)
            )
            {
                Play_Left_Click();
                player_idx = (itr + 1);

                if(spell_idx == spl_Spell_Blast)
                {
                    var_E = (_players[player_idx].casting_cost_original - _players[player_idx].casting_cost_remaining);
                    if(var_E > _players[HUMAN_PLAYER_IDX].mana_reserve)
                    {

                    LBX_Load_Data_Static(message_lbx_file__ovr137, 0, (SAMB_ptr)&GUI_NearMsgString[0], 68, 1, 150);

                    Warn1(GUI_NearMsgString);

                    }
                    else
                    {
                        _osc_leave_screen = ST_TRUE;
                    }

                }
                else
                {
                    _osc_leave_screen = ST_TRUE;
                }

            }

            if(
                (-(gem_player_nums[itr]) == input_field_idx)
                &&
                (_players[HUMAN_PLAYER_IDX].Dipl.Contacted[(1 + itr)] == ST_TRUE)
                &&
                (_FORTRESSES[(1 + itr)].active == ST_TRUE)
            )
            {
                Play_Left_Click();
                player_idx = (itr + 1);
                Mirror_Screen(player_idx, (x_start + 17 + ((itr % 2) * 77)), (y_start + 27 + ((itr / 2) * 61)), (x_start + 62 + ((itr % 2) * 77)), (y_start + 72 + ((itr / 2) * 61)));
                Target_Wizard_Screen_Load(spell_idx);
                Assign_Auto_Function(Target_Wizard_Screen_Draw, 2);
                ADD_FIELDS_LOCAL();
            }

        }

        if(_osc_leave_screen == ST_FALSE)
        {
            Set_Page_Off();
            Target_Wizard_Screen_Draw();
            PageFlip_FX();
        }

    }

    if(var_10 == ST_FALSE)
    {

        _temp_sint_1 = player_idx;

        x = (x_start + 10 + (((player_idx - 1) % 2) * 77));

        y = (y_start + 21 + (((player_idx - 1) / 2) * 61));

        Reset_First_Block(_screen_seg);

        Spell_Animation_Load_Graphics(spell_idx);

        Spell_Animation_Load_Sound_Effect__WIP(spell_idx);

        if(SND_SpellCast != (SAMB_ptr)ST_UNDEFINED)
        {
            // DOMSDOS  Play_Sound__STUB(SND_SpellCast);
            sdl2_Play_Sound__WIP(SND_SpellCast, SND_SpellCast_size);
        }

        Clear_Fields();

        G_Cancel_Button_Index = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, (int16_t)empty_string__ovr137[0], ST_UNDEFINED);

        switch(spell_data_table[spell_idx].Param0)
        {
            case 0:
            {
                var_E = (_players[player_idx].casting_cost_original - _players[player_idx].casting_cost_remaining);
                if(var_E <= _players[HUMAN_PLAYER_IDX].mana_reserve)
                {
                    _players[HUMAN_PLAYER_IDX].mana_reserve -= var_E;
                    if(_players[player_idx].casting_spell_idx != spl_Spell_Of_Return)
                    {
                        _players[player_idx].casting_spell_idx = 0;
                        _players[player_idx].casting_cost_remaining = 0;
                        _players[player_idx].casting_cost_original = 0;
                    }
                    else
                    {
                        _players[player_idx].casting_cost_remaining = Casting_Cost(player_idx, spl_Spell_Of_Return, ST_FALSE);
                    }
                }
            } break;
            case 1:
            {
                GAME_MP_SpellVar_1 = Apply_Cruel_Unminding(player_idx);
            } break;
            case 2:
            {
                GAME_MP_SpellVar_1 = Apply_Drain_Power(player_idx);
            } break;
            case 3:
            {
                Apply_Subversion(player_idx);
            } break;
            default:
            {
                STU_DEBUG_BREAK();
            } break;
        }

        for(itr = 0; itr < 45; itr++)
        {

            if(Get_Input() == G_Cancel_Button_Index)
            {
                break;
            }

            Mark_Time();

            Set_Page_Off();

            if(itr == 12)
            {
                _osc_need_target_flag = ST_FALSE;
            }

            Target_Wizard_Screen_Draw();

            if(itr < 36)
            {
                if((itr % 6) == 0)
                {
                    Reset_Animation_Frame(spell_animation_seg);
                }
                FLIC_Draw(x, y, spell_animation_seg);
            }

            PageFlip_FX();

            Release_Time(2);

        }

    }

    /* SPELLY */  OVL_MosaicFlip__STUB();

    // BUG  Release_Block();
    Release_Block(_screen_seg);

    Deactivate_Auto_Function();

    Deactivate_Help_List();

    return player_idx;
    
}
