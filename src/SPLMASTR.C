/*
    WIZARDS.EXE
        ovr136
        ovr137  ¿ ~ Spell Animation(s) ?
        ovr138

MoO2
    ¿ DNE // N/A ?

*/

#include "MOX/MOX_DAT.H"  /* _screen_seg */
#include "MOX/MOX_SET.H"  /* magic_set */
#include "MOX/SOUND.H"

#include "MOM.H"



// Spellbook.C
// WZD dseg:C968
extern SAMB_ptr wizlab_wizard_seg;
// WZD dseg:C96A
extern SAMB_ptr wizlab_familiar_seg;
// WZD dseg:C96C
extern SAMB_ptr wizlab_blue_column_seg;



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

// WZD dseg:6D2F 20 4D 50 00                                     cnst_Space_MP_3 db ' MP',0              ; DATA XREF: IDK_WizTgtSpl_sBFC85+152o
// WZD dseg:6D2F                                                                                         ; should use dseg:3515
// WZD dseg:6D33 43 68 6F 6F 73 65 20 74 61 72 67 65 74 20 66 6F+aChooseTargetFo db 'Choose target for a ',0
// WZD dseg:6D33 72 20 61 20 00                                                                          ; DATA XREF: IDK_WizTgtSpl_sBFC85+218o
// WZD dseg:6D48 20 73 70 65 6C 6C 00                            aSpell_0 db ' spell',0                  ; DATA XREF: IDK_WizTgtSpl_sBFC85+258o
// WZD dseg:6D4F 20 68 61 73 20 62 65 65 6E 20 73 70 65 6C 6C 20+aHasBeenSpellBl db ' has been spell blasted',0
// WZD dseg:6D4F 62 6C 61 73 74 65 64 00                                                                 ; DATA XREF: IDK_WizTgtSpl_sBFC85:@@Cast_SpellBlasto
// WZD dseg:6D67 20 6C 6F 73 65 73 20 00                         aLoses db ' loses ',0                   ; DATA XREF: IDK_WizTgtSpl_sBFC85:@@Cast_CruelUnmindingo ...
// WZD dseg:6D6F 20 70 6F 69 6E 74 73 20 6F 66 20 63 61 73 74 69+aPointsOfCastin db ' points of casting ability',0
// WZD dseg:6D6F 6E 67 20 61 62 69 6C 69 74 79 00                                                        ; DATA XREF: IDK_WizTgtSpl_sBFC85+2E6o
// WZD dseg:6D8A 20 70 6F 69 6E 74 73 20 6F 66 20 6D 61 6E 61 00 aPointsOfMana db ' points of mana',0    ; DATA XREF: IDK_WizTgtSpl_sBFC85+31Do
// WZD dseg:6D9A 27 20 00                                        cnst_Apostrophe_3 db 27h,' ',0          ; DATA XREF: IDK_WizTgtSpl_sBFC85+335o
// WZD dseg:6D9A                                                                                         ; should use dseg:6902
// WZD dseg:6D9D 27 73 20 00                                     cnst_Possessive_3 db 27h,'s ',0         ; DATA XREF: IDK_WizTgtSpl_sBFC85:loc_BFFBFo
// WZD dseg:6D9D                                                                                         ; should use dseg:6905
// WZD dseg:6DA1 20 72 65 6C 61 74 69 6F 6E 73 20 68 61 76 65 20+aRelationsHaveB db ' relations have been subverted',0
// WZD dseg:6DA1 62 65 65 6E 20 73 75 62 76 65 72 74 65 64 00                                            ; DATA XREF: IDK_WizTgtSpl_sBFC85+349o
// WZD dseg:6DC0 1B 00                                           str_hotkey_ESC__ovr137 db 1Bh,0         ; DATA XREF: IDK_DiploScrn+CAo ...
// WZD dseg:6DC0                                                                                         ; should use dseg:2c56
// WZD dseg:6DC2 6D 65 73 73 61 67 65 00                         message_lbx_file__ovr137 db 'message',0 ; DATA XREF: IDK_DiploScrn+245o

// WZD dseg:6E4B                                                 END:  ovr137 - Initialized Data




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
int16_t word_43494;
// WZD dseg:C9F6
int16_t cast_spell_of_mastery_player_idx;
// WZD dseg:C9F8 00                                              db    0
// WZD dseg:C9F9 00                                              db    0
// WZD dseg:C9FA
// WZD dseg:C9FA                                                 BEGIN: ovr136 - Uninitialized Data



// WZD dseg:C9FA                                                 BEGIN: ovr136 - Uninitialized Data

// WZD dseg:C9FA 00 00                                           word_4349A dw 0                         ; DATA XREF: IDK_Spell_DisjunctOrBind_Load+265w ...
// WZD dseg:C9FC 00 00                                           word_4349C dw 0                         ; DATA XREF: IDK_Spell_DisjunctOrBind_Load+2BEr ...
// WZD dseg:C9FE 00 00                                           word_4349E dw 0                         ; DATA XREF: IDK_Spell_DisjunctOrBind_Load+2D5r ...
// WZD dseg:CA00 00 00                                           word_434A0 dw 0                         ; DATA XREF: IDK_Spell_DisjunctOrBind_Load+2F0r
// WZD dseg:CA02 00                                              db    0
// WZD dseg:CA03 00                                              db    0
// WZD dseg:CA04 00 00                                           word_434A4 dw 0                         ; DATA XREF: IDK_Spell_DisjunctOrBind_Draw+194r ...
// WZD dseg:CA06 00 00                                           word_434A6 dw 0                         ; DATA XREF: IDK_Spell_DisjunctOrBind_Load+2B7w ...
// WZD dseg:CA08 00 00                                           word_434A8 dw 0                         ; DATA XREF: IDK_Spell_DisjunctOrBind_Load+2CAw
// WZD dseg:CA0A 00 00                                           word_434AA dw 0                         ; DATA XREF: IDK_Spell_DisjunctOrBind_Load+2E1w
// WZD dseg:CA0C 00 00                                           word_434AC dw 0                         ; DATA XREF: IDK_Spell_DisjunctOrBind_Load+2FCw
// WZD dseg:CA0E 00                                              db    0
// WZD dseg:CA0F 00                                              db    0
// WZD dseg:CA10 00 00                                           CMB_SliderLimit dw 0                    ; DATA XREF: CMB_SliderRedraw+7Br ...
// WZD dseg:CA10                                                                                         ; is this really necessary?
// WZD dseg:CA12 00 00                                           GAME_MP_SpellVar_1 dw 0                 ; DATA XREF: SBK_SliderRedraw+9Fw ...
// WZD dseg:CA12                                                                                         ; holds the spell strength during sliders
// WZD dseg:CA12                                                                                         ; holds the anim stage during global cast anims

// WZD dseg:CA14
/*
; steps 0 to 7 for sliders
*/
int16_t SBK_SliderAnimStage;

// WZD dseg:CA16 00 00                                           SBK_Spell_Index dw 0                    ; DATA XREF: Learn_Spell_Animation+Cw ...
// WZD dseg:CA18 00 00                                           SBK_SliderState dw 0                    ; DATA XREF: SBK_SliderRedraw+6Br ...
// WZD dseg:CA1A 00 00 00 00 00 00 00 00 00 00                   word_434BA dw 5 dup(0)                  ; DATA XREF: IDK_Spell_DisjunctOrBind_Load+308w ...
// WZD dseg:CA24
SAMB_ptr word_434C4;
// WZD dseg:CA26 00 00                                           IMG_SBK_SliderBG@ dw 0                  ; DATA XREF: SBK_LoadSpellSlider+38w ...
// WZD dseg:CA28 00 00                                           GAME_MP_SpellVar_2 dw 0                 ; DATA XREF: CMB_SliderRedraw+3Fr ...
// WZD dseg:CA28                                                                                         ; holds the caster ID during combat sliders
// WZD dseg:CA28                                                                                         ; holds the mirror reveal mask during global cast anims
// WZD dseg:CA2A 00 00                                           IMG_OVL_TrgtWizCncl@ dw 0               ; DATA XREF: IDK_SplScr_sBFAA5+50w ...
// WZD dseg:CA2C 00 00                                           IDK_DiploScrn_scanned_field dw 0        ; DATA XREF: IDK_Spell_DisjunctOrBind_Draw+44r ...
// WZD dseg:CA2E 00 00                                           IDK_SUMMONBK_pict_seg dw 0              ; DATA XREF: IDK_Spell_DisjunctOrBind_Load+17Dw ...
// WZD dseg:CA30 00 00                                           IMG_SBK_SliderOK@ dw 0                  ; DATA XREF: SBK_LoadSpellSlider+7Dw ...
// WZD dseg:CA32 00 00                                           IMG_OVL_TargetWizBG@ dw 0               ; DATA XREF: IDK_SplScr_sBFAA5+39w ...
// WZD dseg:CA34 00 00                                           IMG_SBK_Anims@ dw 0                     ; DATA XREF: CMB_ShowSpellbook+30w ...
// WZD dseg:CA36 00 00                                           IMG_SBK_PageText@ dw 0                  ; DATA XREF: Learn_Spell_Animation+34Bw ...

// WZD dseg:CA36                                                 END: ovr136 - Uninitialized Data



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

// WZD dseg:CA4C 00 00                                           GUI_Interaction_Done dw 0               ; DATA XREF: SBK_SpellSlider+D5w ...
// WZD dseg:CA4E 00 00                                           IDK_WizTgt_SplCmpl_w434EE dw 0          ; DATA XREF: IDK_Spell_DisjunctOrBind_Draw+20Ar ...
// WZD dseg:CA50 00 00                                           GAME_MP_SpellVar_3 dw 0                 ; DATA XREF: SBK_SliderRedraw+8r ...
// WZD dseg:CA50                                                                                         ; clicked spell label index during combat sliders
// WZD dseg:CA50                                                                                         ; player_idx during global cast anims
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
SAMB_ptr word_434F8;
// WZD dseg:CA5A
SAMB_ptr word_434FA;

// WZD dseg:CA5A                                                 END: ovr138 - Uninitialized Data



/*
    WIZARDS.EXE  ovr136
*/

// WZD o136p01
// Select_Hero_To_Ressurect()

// WZD o136p02
// SBK_LoadSpellSlider()

// WZD o136p03
// SBK_SliderRedraw()

// WZD o136p04
// SBK_SpellSlider()

// WZD o136p05
// CMB_LoadSpellSlider()

// WZD o136p06
// CMB_SliderRedraw()

// WZD o136p07
// CMB_SpellSlider()

// WZD o136p08
// sub_BD8A8()

// WZD o136p09
// sub_BDBC5()

// WZD o136p10
// IDK_SplBk_OvlEnch_sBDEA0()

// WZD o136p11
// sub_BE356()

// WZD o136p12
// sub_BE656()



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
    int16_t DBG_unit_type;

    start_x = 30;
    start_y = 42;

    Copy_Back_To_Off();

    Set_Window((start_x + 7), (start_y + 8), SCREEN_XMAX, SCREEN_YMAX);

    Clipped_Draw((start_x + 8), (start_y + 3), IDK_wizard_id_thing_seg);

    FLIC_Draw((start_x + 55), (start_y + 54), SPELLSCR_GLOBALMK_seg);

    Set_Window(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, (start_y + 105));

    if(
        (SBK_SliderAnimStage < 30)
        &&
        (magic_set.spell_animations == ST_TRUE)
        &&
        (GAME_MP_SpellVar_3 == HUMAN_PLAYER_IDX)
    )
    {

        Clipped_Draw((start_x + 76), (start_y + 110 - (SBK_SliderAnimStage * 3)), IDK_MONSTER_seg);

    }
    else
    {

        Clipped_Draw((start_x + 76), (start_y + 18), IDK_MONSTER_seg);

    }

    FLIC_Draw((start_x + 64), (start_y + 80), SPELLSCR_ENCHANT_seg);

    Set_Outline_Color(16);

    Set_Font_Style_Shadow_Down(4, 4, 0, 0);

    Set_Alias_Color(190);

    if(IDK_DiploScrn_scanned_field > 0)
    {

        // strcpy(GUI_NearMsgString, *_unit_type_table[spell_data_table[_players[GAME_MP_SpellVar_3].casting_spell_idx].Param0].name);
        DBG_unit_type = spell_data_table[_players[GAME_MP_SpellVar_3].casting_spell_idx].Param0;
        DBG_unit_type = spell_data_table[_players[GAME_MP_SpellVar_3].casting_spell_idx].unit_type;
        strcpy(GUI_NearMsgString, *_unit_type_table[DBG_unit_type].name);

        strcat(GUI_NearMsgString, aSummoned);

    }
    else
    {
        if(IDK_DiploScrn_scanned_field == -1)
        {

            strcpy(GUI_NearMsgString, aItemEnchanted);

        }
        else
        {

            if(IDK_DiploScrn_scanned_field > -10)
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
    int16_t var_6;
    int16_t input_field_idx;
    int16_t full_screen_field;

    Stop_All_Sounds__STUB();

    SND_Spell_Music = LBX_Reload(music_lbx_file__ovr137, (MUSIC_Weak_Summon + magic_realm_spell_idx), SND_Music_Segment);

    if(magic_set.background_music == ST_TRUE)
    {

        Play_Sound__WIP(SND_Spell_Music);

    }

    SBK_SliderAnimStage = 0;

    GAME_MP_SpellVar_3 = player_idx;

    IDK_DiploScrn_scanned_field = unit_type;

    Allocate_Reduced_Map();

    Full_Draw_Main_Screen();

    IDK_SummonAnim_Load(unit_type, player_idx);

    Assign_Auto_Function(IDK_SummonAnim_Draw, 2);

    PageFlipEffect = 3;

    Clear_Fields();

    full_screen_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, empty_string__ovr137[0], ST_UNDEFINED);

    if(magic_set.spell_animations == ST_TRUE)
    {
        var_6 = 130;
    }
    else
    {
        var_6 = 40;
    }

    GUI_Interaction_Done = ST_FALSE;

    for(SBK_SliderAnimStage = 0; ((SBK_SliderAnimStage < var_6) && (GUI_Interaction_Done == ST_FALSE)); SBK_SliderAnimStage++)
    {

        Mark_Time();

        input_field_idx = Get_Input();

        if(input_field_idx == full_screen_field)
        {
            GUI_Interaction_Done = ST_TRUE;
        }

        Set_Page_Off();

        IDK_SummonAnim_Draw();

        PageFlip_FX();

        Release_Time(2);

        PageFlipEffect =  0;

    }

    Deactivate_Auto_Function();

    OVL_MosaicFlip__STUB();

    Stop_All_Sounds__STUB();

    Release_Block(_screen_seg);

    Play_Background_Music__STUB();

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
// sub_BECD9()

// WZD o137p05
// drake178: sub_BEE75()
// IDK_City_sBEE75()

// WZD o137p06
// drake178: sub_BF048()
// sub_BF048()

// WZD o137p07
// drake178: sub_BF2AF()
// sub_BF2AF()

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

        // SPECFX.LBX, 015  "ETRNIGHT"  ""
        // ...
        // SPECFX.LBX, 038  "DETHWISH"  ""
        // SPECFX.LBX, 039  "UNSUMMON"  ""
        // SPECFX.LBX, 056  "AWARENES"  ""
        spell_animation_seg = LBX_Reload_Next(specfx_lbx_file__ovr137, (15 + specfx_entry_num), _screen_seg);

        // SPECFX.LBX, 054  "MASK1"     ""
        GAME_MP_SpellVar_2 = LBX_Reload_Next(specfx_lbx_file__ovr137, 54, _screen_seg);

        FLIC_Reset_CurrentFrame(GAME_MP_SpellVar_2);

    }
    else if(GAME_MP_SpellVar_1 == 30)
    {

        // SPECFX.LBX, 015  "ETRNIGHT"  ""
        spell_animation_seg = LBX_Reload_Next(specfx_lbx_file__ovr137, 15, _screen_seg);

        // SPECFX.LBX, 055  "MASK2"     ""
        GAME_MP_SpellVar_2 = LBX_Reload_Next(specfx_lbx_file__ovr137, 55, _screen_seg);

        FLIC_Reset_CurrentFrame(GAME_MP_SpellVar_2);

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

        FLIC_Reset_CurrentFrame(GAME_MP_SpellVar_2);
        
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

*/
void OVL_DrawGlobalAnim(void)
{
    int16_t start_x = 0;  // _SI_
    int16_t start_y = 0;  // _DI_

    start_x = 58;
    start_y = 28;

    if(GUI_Interaction_Done == ST_TRUE)
    {
        return;
    }

    Main_Screen_Draw();

    Set_Outline_Color(0);

    Set_Font_Style_Outline(5, 0, 0, 0);

    if(magic_set.spell_animations != ST_TRUE)
    {

        if(GAME_MP_SpellVar_3 == 0)
        {

            strcpy(GUI_NearMsgString, cnst_Spellcast_Msg_1);

        }
        else
        {

            strcpy(GUI_NearMsgString, _players[GAME_MP_SpellVar_3].name);

            strcat(GUI_NearMsgString, cnst_Spellcast_Msg_2);

        }

        Print_Centered((62 + start_x), (133 + start_y), GUI_NearMsgString);

        FLIC_Draw(start_x, start_y, diplomacy_mirror_seg);

        FLIC_Draw((10 + start_x), (8 + start_y), spell_animation_seg);

        Print_Centered_Far((62 + start_x), (153 + start_y), spell_data_table[SBK_Spell_Index].name);

    }
    else
    {

        if(GAME_MP_SpellVar_1 < 34)
        {

            if(GAME_MP_SpellVar_3 == 0)
            {

                strcpy(GUI_NearMsgString, cnst_Spellcast_Msg_1);

            }
            else
            {

                strcpy(GUI_NearMsgString, _players[GAME_MP_SpellVar_3].name);

                strcat(GUI_NearMsgString, cnst_Spellcast_Msg_2);

            }

            Print_Centered((62 + start_x), (133 + start_y), GUI_NearMsgString);

        }

        if(GAME_MP_SpellVar_1 < 14)
        {

            FLIC_Reset_CurrentFrame(ge_anim_moodwiz_seg);

            FLIC_Draw((start_x + 12), (start_y + 12), ge_anim_moodwiz_seg);

            Draw_Picture_To_Bitmap(GAME_MP_SpellVar_2, IMG_SBK_Anims);

            Draw_Picture_To_Bitmap(IMG_SBK_SliderBG, IMG_SBK_PageText);

            LBX_IMG_Overlay(0, 0, IMG_SBK_PageText, IMG_SBK_Anims);

            Draw_Picture((start_x + 12), (start_y + 12), IMG_SBK_PageText);

            FLIC_Draw(start_x, start_y, diplomacy_mirror_seg);

        }
        else if(GAME_MP_SpellVar_1 < 20)
        {

            FLIC_Draw((start_x + 12), (start_y + 12), IMG_SBK_SliderBG);

            FLIC_Reset_CurrentFrame(ge_anim_moodwiz_seg);

            FLIC_Draw((start_x + 12), (start_y + 12), ge_anim_moodwiz_seg);

            FLIC_Draw(start_x, start_y, diplomacy_mirror_seg);

        }
        else if(GAME_MP_SpellVar_1 < 34)
        {

            FLIC_Draw((start_x + 10), (start_y + 8), spell_animation_seg);

            Draw_Picture_To_Bitmap(GAME_MP_SpellVar_2, IMG_SBK_Anims);

            FLIC_Reset_CurrentFrame(ge_anim_moodwiz_seg);

            Draw_Picture_To_Bitmap(ge_anim_moodwiz_seg, IMG_SBK_PageText);

            LBX_IMG_Overlay(0, 0, IMG_SBK_PageText, IMG_SBK_Anims);

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

    // SPELLY  Stop_All_Sounds__STUB();

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
        // ...
        // MUSIC.LBX, 039 "TEMP XMI"    "Wrath of God"
        SND_Spell_Music = LBX_Reload(music_lbx_file__ovr137, (music_entry_number + 19), SND_Music_Segment);
        SND_Spell_Music_size = lbxload_entry_length;
    }

    if(magic_set.background_music == ST_TRUE)
    {

        // DOMSDOS  Play_Sound__WIP(SND_Spell_Music);
        sdl2_Play_Sound(SND_Spell_Music, SND_Spell_Music_size);

    }

    GAME_MP_SpellVar_1 = 0;

    GAME_MP_SpellVar_3 = player_idx;

    SBK_Spell_Index = spell_idx;

    Reset_First_Block(_screen_seg);

    Allocate_Reduced_Map();

    Mark_Block(_screen_seg);

    OVL_LoadGlobalAnim(spell_idx, player_idx);

    Clear_Fields();

    fullscreen_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, empty_string__ovr137[0], ST_UNDEFINED);

    Assign_Auto_Function(OVL_DrawGlobalAnim, 2);

    GUI_Interaction_Done = 0;

    for(GAME_MP_SpellVar_1 = 0; ((GAME_MP_SpellVar_1 < 120) && (GUI_Interaction_Done == 0)); GAME_MP_SpellVar_1++)
    {

        input_field_idx = abs(Get_Input());

        if(input_field_idx == fullscreen_ESC_field)
        {

            GUI_Interaction_Done = ST_TRUE;

        }

        if(GUI_Interaction_Done != ST_TRUE)
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
                PageFlipEffect = 3;
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

    Play_Background_Music__STUB();

    OVL_MosaicFlip__STUB();

}


// WZD o137p11
// drake178: sub_BFAA5()
// IDK_SplScr_sBFAA5()

// WZD o137p12
// drake178: sub_BFC85()
// IDK_WizTgtSpl_sBFC85()

// WZD o137p13
// drake178: ¿ ?
// IDK_DiploScrn()



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

    word_434FA = LBX_Reload(spellose_lbx_file__ovr138, 29, _screen_seg);

    word_434F8 = LBX_Reload_Next(spellose_lbx_file__ovr138, wizard_id, _screen_seg);

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

    FLIC_Reset_CurrentFrame(wizlab_wizard_seg);

    Draw_Picture_To_Bitmap(wizlab_wizard_seg, IMG_SBK_Anims);

    Get_Bitmap_Actual_Size(IMG_SBK_Anims, &x1, &y1, &width, &height);

    start_x = 35 + x1 + (width / 2);

    FLIC_Reset_CurrentFrame(wizlab_wizard_seg);

    Set_Font_Colors_15(5, &colors[0]);

    Set_Outline_Color(0);

    Set_Font_Style_Outline(5, 15, 0, 0);

    // SPELLY  Print_Centered(160, 5, GUI_NearMsgString, 2);
    Print_Centered(160, 5, GUI_NearMsgString, 2);

    Print_Centered(160, 25, strSpellOfMastery, ST_NULL);

    if(SBK_SliderAnimStage < 12)
    {

        FLIC_Draw(61, 68, wizlab_wizard_seg);

        FLIC_Set_CurrentFrame(word_434FA, (SBK_SliderAnimStage / 2));

        FLIC_Draw(start_x, 34, word_434FA);

    }
    else
    {

        if(SBK_SliderAnimStage < 24)
        {

            FLIC_Set_CurrentFrame(word_434FA, 5);

            FLIC_Draw(start_x, 34, word_434FA);

            x = 61;  // BUGBUG  never used

            IDK = -y1;

            if(((SBK_SliderAnimStage - 12) / 2) < 2)
            {

                // SPELLY  LBX_IMG_VShiftRect(x1, 0, (x1 + (width / 4)), (IDK / 6), IMG_SBK_Anims);

                // SPELLY  LBX_IMG_VShiftRect((x1 + (width / 4) + 1), (IDK / 6), (x1 + (width / 2)), (IDK / 3), IMG_SBK_Anims);

                // SPELLY  LBX_IMG_VShiftRect((x1 + (width / 2) + 1), (IDK / 3), (x1 + ((width * 3) / 4)), (IDK / 6), IMG_SBK_Anims);

                // SPELLY  LBX_IMG_VShiftRect((x1 + ((width * 3) / 4) + 1), (IDK / 6), (x1 + width), 0, IMG_SBK_Anims);

            }
            else
            {

                if(((SBK_SliderAnimStage - 12) / 2) < 4)
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

            Scale_Bitmap(IMG_SBK_Anims, (100 - ((SBK_SliderAnimStage - 12) * 6)), 100);

            x = (61 + (((SBK_SliderAnimStage - 12) * 714) / 200));

            Draw_Picture(x, 68, IMG_SBK_Anims);

            FLIC_Reset_CurrentFrame(word_434F8);

            Draw_Picture_To_Bitmap(word_434F8, IMG_SBK_Anims);

            // SPELLY  Vanish_Bitmap(IMG_SBK_Anims, ((SBK_SliderAnimStage - 12) * 8));

            Draw_Picture(start_x, 34, IMG_SBK_Anims);

        }
        else
        {

            if(SBK_SliderAnimStage < 36)
            {

                FLIC_Set_CurrentFrame(word_434F8, ((SBK_SliderAnimStage - 24) / 2));

                FLIC_Draw(start_x, 34, word_434F8);

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
        sdl2_Play_Sound(SND_Spell_Music, SND_Spell_Music_size);
    }

    Set_Mouse_List(1, mouse_list_none);

    CLROFF();

    Toggle_Pages();

    Copy_On_To_Off_Page();

    Spell_Of_Mastery_Lose_Load(_players[HUMAN_PLAYER_IDX].wizard_id);

    Assign_Auto_Function(Spell_Of_Mastery_Lose_Draw, 2);
    
    SBK_SliderAnimStage = 0;

    PageFlipEffect = 3;

    Set_Page_Off();

    Spell_Of_Mastery_Lose_Draw();

    PageFlip_FX();

    for(SBK_SliderAnimStage = 0; SBK_SliderAnimStage < 43; SBK_SliderAnimStage++)
    {

        Mark_Time();

        Set_Page_Off();

        Spell_Of_Mastery_Lose_Draw();

        PageFlip_FX();

        Release_Time(2);

        SBK_SliderAnimStage++;
 
    }

    Combat_Cache_Read();

    // SPELLY  GAME_LimboFallAnim(HUMAN_PLAYER_IDX);

    // SPELLY  GAME_OVER();

    s01p16_empty_function();

    // SPELLY  GAME_EXE_Swap(cnst_MAGIC_EXE_File2, cnst_MAGICEXE_arg0_2, &spellose_lbx_file__ovr138[8], &spellose_lbx_file__ovr138[8]);

}


// WZD o138p04
void SoM_Started_Draw(void)
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
void SoM_Started(int16_t player_idx)
{
    int16_t full_screen_ESC_field = 0;

    Deactivate_Auto_Function();

    Stop_All_Sounds__STUB();

    strcpy(GUI_NearMsgString, _players[player_idx].name);

    strcat(GUI_NearMsgString, strHasStartedCastingThe);

    SND_Spell_Music = LBX_Reload(music_lbx_file__ovr138, MUSIC_SoM_Started, SND_Music_Segment);
    SND_Spell_Music_size = lbxload_entry_length;

    // SPELLSCR.LBX, 067  "VORTEX3" ""
    Open_File_Animation__HACK(spellscr_lbx_file__ovr138, 67);

    IDK_DiploScrn_scanned_field = 67;

    Set_Mouse_List(1, mouse_list_none);

    Fade_Out();

    Set_Page_On();

    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_BLACK);

    Copy_On_To_Off_Page();

    Toggle_Pages();

    // TODO  Load_Palette(4, -1);  // ; TEST - spell mastery palette
    Load_Palette(4, 0, 255);

    Set_Page_Off();

    SoM_Started_Draw();

    Toggle_Pages();

    Fade_In();

    Assign_Auto_Function(SoM_Started_Draw, 2);

    if(magic_set.background_music == ST_TRUE)
    {
        // DOMSDOS  Play_Sound__WIP(SND_Spell_Music);
        sdl2_Play_Sound(SND_Spell_Music, SND_Spell_Music_size);
    }

    Clear_Fields();

    full_screen_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr138[0], ST_UNDEFINED);

    
    _combat_wx = 0;
    /* incremented in SoM_Started_Draw() */
    while((_combat_wx < 200) && (Get_Input() != full_screen_ESC_field))
    {

    }

    Stop_All_Sounds__STUB();

    Play_Background_Music__STUB();

    Fade_Out();

    // SPELLY  Load_Palette(0, -1);  // ; EMPERATO - main game palette
    Load_Palette(0, 0, 255);

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

    if(SBK_SliderAnimStage <= 6)
    {

        FLIC_Reset_CurrentFrame(IDK_wizard_id_thing_seg);

        FLIC_Draw(69, 75, IDK_wizard_id_thing_seg);

    }
    else
    {

        if(((SBK_SliderAnimStage - 6) % 60) < 10)
        {

            FLIC_Set_CurrentFrame(IDK_wizard_id_thing_seg, (((SBK_SliderAnimStage - 6) % 60) / 2));

        }
        else
        {

            if(((SBK_SliderAnimStage - 6) % 60) > 50)
            {

                FLIC_Set_CurrentFrame(IDK_wizard_id_thing_seg, ((60 - ((SBK_SliderAnimStage - 6) % 60)) / 2));

            }
            else
            {

                FLIC_Set_CurrentFrame(IDK_wizard_id_thing_seg, 4);

            }

        }

        FLIC_Draw(69, 75, IDK_wizard_id_thing_seg);

        if((6 + ((_num_players - 1) * 60)) > SBK_SliderAnimStage)
        {

            Draw_File_Animation__HACK();

            if(((SBK_SliderAnimStage - 6) % 60) < 9)
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
    
    SBK_SliderAnimStage = 0;

    cast_spell_of_mastery_player_idx = player_idx;

    Stop_All_Sounds__STUB();

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
        // DOMSDOS Play_Sound__STUB(SND_Spell_Music);
        sdl2_Play_Sound(SND_Spell_Music, SND_Spell_Music_size);
    }

    Clear_Fields();

    hotkey_ESC = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, &str_hotkey_ESC__ovr138[0], ST_UNDEFINED);

    var_4 = 12 + ((_num_players - 1) * 60);

    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {

        if(_players[itr_players].casting_spell_idx == spl_Spell_Of_Return)
        {
            var_4 -= 60;
        }

    }

    IDK = 0;

    SBK_SliderAnimStage = 0;

    while((SBK_SliderAnimStage < var_4) && (Get_Input() != hotkey_ESC))
    {

        // IDGI
        if(((SBK_SliderAnimStage + 65530) % 60) == 0)
        {
            while(_players[++IDK].casting_spell_idx == spl_Spell_Of_Return) {}

            Open_File_Animation__HACK(splmastr_lbx_file__ovr138, _players[IDK].wizard_id);

            FLIC_Reset_CurrentFrame(word_434C4);
        }

        if((var_4 - 6) == SBK_SliderAnimStage)
        {

            Release_Block(_screen_seg);

            Mark_Block(_screen_seg);

            wizlab_blue_column_seg = LBX_Reload(splmastr_lbx_file__ovr138, 31, _screen_seg);

        }

        if(SBK_SliderAnimStage == 7)
        {

            wizlab_blue_column_seg = LBX_Reload(splmastr_lbx_file__ovr138, 30, _screen_seg);

        }

        Mark_Time();

        Set_Page_Off();

        Spell_Of_Mastery_Draw();

        Toggle_Pages();

        Release_Time(3);

        SBK_SliderAnimStage++;

    }

    // SPELLY  Stop_All_Sounds__STUB();

    // SPELLY  Play_Background_Music__STUB();

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
