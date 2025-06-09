/*
    WIZARDS.EXE
        ovr136
        ovr138

MoO2
    ¿ DNE // N/A ?
    ~ Science Room?
*/

#include "MOX/LBX_Load.H"
#include "MOX/MOM_Data.H"
#include "MOX/MOX_DAT.H"  /* _screen_seg */
#include "MOX/MOX_DEF.H"
#include "MOX/MOX_SET.H"  /* magic_set */
#include "MOX/MOX_TYPE.H"
#include "MOX/sdl2_Audio.H"
#include "MOX/SOUND.H"

#include "Combat.H"
#include "MainScr.H"



// Spellbook.C
// WZD dseg:C968
extern SAMB_ptr wizlab_wizard_seg;
// WZD dseg:C96A
extern SAMB_ptr wizlab_familiar_seg;
// WZD dseg:C96C
extern SAMB_ptr wizlab_blue_column_seg;

// Spells137.C
// WZD dseg:CA54
extern SAMB_ptr IDK_wizard_id_thing_seg;



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
int16_t _osc_anim_ctr;

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
// WZD dseg:CA2C 00 00                                           _temp_sint_4 dw 0        ; DATA XREF: IDK_Spell_DisjunctOrBind_Draw+44r ...
// WZD dseg:CA2E 00 00                                           IDK_SUMMONBK_pict_seg dw 0              ; DATA XREF: IDK_Spell_DisjunctOrBind_Load+17Dw ...
// WZD dseg:CA30 00 00                                           IMG_SBK_SliderOK@ dw 0                  ; DATA XREF: SBK_LoadSpellSlider+7Dw ...
// WZD dseg:CA32 00 00                                           IMG_OVL_TargetWizBG@ dw 0               ; DATA XREF: IDK_SplScr_sBFAA5+39w ...
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

*/
int16_t CMB_SpellSlider__STUB(int16_t spell_idx, int16_t Selected_Spell, int16_t caster_idx)
{
    if(caster_idx >= CASTER_IDX_BASE)
    {
        return Casting_Cost(HUMAN_PLAYER_IDX, spell_idx, 1);
    }
    else
    {
        return spell_data_table[spell_idx].casting_cost;
    }
}


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
    Print_Centered(160, 5, GUI_NearMsgString, 2);

    Print_Centered(160, 25, strSpellOfMastery, ST_NULL);

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

    _temp_sint_4 = 67;

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

    }

    // DOMSDOS  Stop_All_Sounds__STUB();

    // DOMSDOS  Play_Background_Music__STUB();
    sdl2_Play_Background_Music__WIP();

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
