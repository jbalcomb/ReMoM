/*
    WIZARDS.EXE
        ovr136
        ovr137  ¿ ~ Spell Animation(s) ?
        ovr138

MoO2
    ¿ DNE // N/A ?

*/

#include "MoM.H"



// WZD dseg:6C4A                                                 BEGIN:  ovr137 - Initialized Data

// WZD dseg:6C4A 53 70 65 6C 6C 73 63 72                         spellscr_lbx_file__ovr137__1 db 'Spellscr'
// WZD dseg:6C52 00                                              
char empty_string__ovr137[] = "";
// WZD dseg:6C53 4D 6F 6E 73 74 65 72 00                         monster_lbx_file__ovr137 db 'Monster',0 ; DATA XREF: IDK_SummonAnim_Load+E2o ...
// WZD dseg:6C5B 20 53 75 6D 6D 6F 6E 65 64 00                   aSummoned db ' Summoned',0
// WZD dseg:6C65 49 74 65 6D 20 45 6E 63 68 61 6E 74 65 64 00    aItemEnchanted db 'Item Enchanted',0
// WZD dseg:6C74 48 65 72 6F 20 53 75 6D 6D 6F 6E 65 64 00       aHeroSummoned_0 db 'Hero Summoned',0
// WZD dseg:6C82 43 68 61 6D 70 69 6F 6E 20 53 75 6D 6D 6F 6E 65+aChampionSumm_0 db 'Champion Summoned',0

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
// WZD dseg:6DF8 4D 41 47 49 43 2E 45 58 45 00                   cnst_MAGIC_EXE_File2 db 'MAGIC.EXE',0   ; DATA XREF: IDK_SomScr_Lose+FDo ...
// WZD dseg:6E02 4A 45 4E 4E 59 00                               cnst_MAGICEXE_arg0_2 db 'JENNY',0       ; DATA XREF: IDK_SomScr_Lose+F9o ...
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
// WZD dseg:CA14 00 00                                           SBK_SliderAnimStage dw 0                ; DATA XREF: SBK_SliderRedraw+2Cr ...
// WZD dseg:CA14                                                                                         ; steps 0 to 7 for sliders
// WZD dseg:CA16 00 00                                           SBK_Spell_Index dw 0                    ; DATA XREF: Learn_Spell_Animation+Cw ...
// WZD dseg:CA18 00 00                                           SBK_SliderState dw 0                    ; DATA XREF: SBK_SliderRedraw+6Br ...
// WZD dseg:CA1A 00 00 00 00 00 00 00 00 00 00                   word_434BA dw 5 dup(0)                  ; DATA XREF: IDK_Spell_DisjunctOrBind_Load+308w ...
// WZD dseg:CA24 00 00                                           word_434C4 dw 0                         ; DATA XREF: IDK_SomScr_Load+8Cw ...
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

// WZD dseg:CA38 00 00                                           IDK_MONSTER_seg dw 0                    ; DATA XREF: IDK_SummonAnim_Load+EEw ...
// WZD dseg:CA3A 00 00                                           word_434DA dw 0                         ; DATA XREF: IDK_SummonAnim_Load+CBw ...
// WZD dseg:CA3C 00 00                                           IDK_SPELLSCR_seg__1 dw 0                ; DATA XREF: IDK_SummonAnim_Load+49w ...
// WZD dseg:CA3E 00 00                                           word_434DE dw 0                         ; DATA XREF: IDK_SummonAnim_Load+B4w ...
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
// WZD dseg:CA52
// WZD dseg:CA54
// WZD dseg:CA54                                                 BEGIN: ovr137 - Uninitialized Data
// WZD dseg:CA54
// WZD dseg:CA54 00 00                                           IDK_SPELLSCR_seg__2 dw 0                ; DATA XREF: IDK_SummonAnim_Load+174w ...

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

// WZD dseg:CA58 00 00                                           word_434F8 dw 0                         ; DATA XREF: IDK_SomScr_Lose_Load+B1w ...
// WZD dseg:CA5A 00 00                                           word_434FA dw 0                         ; DATA XREF: IDK_SomScr_Lose_Load+9Dw ...

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
// sub_BE860()

// WZD o137p02
// drake178: sub_BE9FF()
// sub_BE9FF()

// WZD o137p03
// drake178: sub_BEB92()
// sub_BEB92()

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

    Stop_All_Sounds__STUB();

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
    }
    else
    {
        // MUSIC.LBX, 019 "MOM35 XM"    "Eternal Night  DEATH"
        // ...
        // MUSIC.LBX, 039 "TEMP XMI"    "Wrath of God"
        SND_Spell_Music = LBX_Reload(music_lbx_file__ovr137, (music_entry_number + 19), SND_Music_Segment);
    }

    if(magic_set.background_music == ST_TRUE)
    {

        Play_Sound__STUB(SND_Spell_Music);

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

// WZD o138p02
// IDK_SomScr_Lose_Draw()

// WZD o138p03
// IDK_SomScr_Lose()

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

    // TODO  Print_Centered(160, 160, GUI_NearMsgString, 2);
    Print_Centered(160, 160, GUI_NearMsgString);

    Print_Centered(160, 180, strSpellOfMastery);

    OVL_Action_XPos++;

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
        Play_Sound__STUB(SND_Spell_Music);
    }

    Clear_Fields();

    full_screen_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_hotkey_ESC__ovr138[0], ST_UNDEFINED);

    
    OVL_Action_XPos = 0;
    /* incremented in SoM_Started_Draw() */
    while((OVL_Action_XPos < 200) && (Get_Input() != full_screen_ESC_field))
    {

    }

    Stop_All_Sounds__STUB();

    Play_Background_Music__STUB();

    Fade_Out();

    // TODO  Load_Palette(0, -1);  // ; EMPERATO - main game palette
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

// WZD o138p07
// IDK_SomScr_Draw()

// WZD o138p08
// IDK_SomScr()

// WZD o138p09
// CMB_LoadUndeadAnim()

// WZD o138p10
// CMB_DrawUndeadAnim()

// WZD o138p11
// CMB_CreateUndeadAnim()
