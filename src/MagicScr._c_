/*
    Magic Screen
    Mirrow Screen

    WIZARDS.EXE
        ovr073
        ovr074
    
    MoO2:
        Module: ¿  ?
*/

#include "MOX/MOX_DAT.H"  /* _screen_seg */

#include "City_ovr55.H"
#include "CITYCALC.H"
#include "DIPLOMAC.H"
#include "Explore.H"
#include "Help.H"
#include "MagicScr.H"
#include "MOM_SCR.H"
#include "NEXTTURN.H"
#include "Spellbook.H"
#include "WIZVIEW.H"
#include "WZD_o059.H"



enum e_Alchemy_Conversion_Direction
{
    e_PowerToGold,
    e_GoldToPower
};





// WZD dseg:3617                                                 BEGIN:  _treaty_type_names
// WZD dseg:3617
extern char aNoTreaty[];
// WZD dseg:3621
extern char aWizardPact_1[];
// WZD dseg:362D
extern char aAlliance_1[];
// WZD dseg:3636
extern char aWar[];
// WZD dseg:363A
extern char aFinalWar[];
// WZD dseg:363A                                                 END:  _treaty_type_names



// WZD dseg:347E                                                 BEGIN: ovr073 - Strings

// WZD dseg:347E
int16_t m_alchemy_conversion_direction = e_GoldToPower;

// WZD dseg:3480
char aRvl[] = "RVL";

// WZD dseg:3483
// borrowed null-terminator from aRvl
char str_empty_string__ovr073[]  = "";

// WZD dseg:3484
char aPwr[] = "PWR";

// ¿ Add_Scroll_Field() for research_stave_pct_pos borrows 'R' as offset [aPwr+2] ?

// WZD dseg:3488
char aDestin[] = "DESTIN";

// WZD dseg:348F
char cnst_HOTKEY_X_5 = 'X';

// WZD dseg:3491
char cnst_HOTKEY_Esc7 = '\x1B';

// WZD dseg:3493
char aWriteItUpGiveMeASav[] = "Write it up, give me a save game.";

// WZD dseg:3493
char aDoYouWishToCancelYour[] = "Do you wish to cancel your \x02";
// WZD dseg:34D2
char aSpell_1[] = "\x01 spell.";

// WZD dseg:34DB
char aYouMayNotContactOth[] = "You may not contact other wizards while you are banished.";

// WZD dseg:3515
char cnst_Space_MP[] = " MP";
// WZD dseg:3519
char cnst_NoSpell[] = "No Spell";
// WZD dseg:3522
char aRp_0[] = " RP";
// WZD dseg:3526
char aSp[] = " SP";

// WZD dseg:352A
char aCastingSkill[] = "Casting Skill:";

// WZD dseg:3539
char cnst_OpeningBrace_2[] = "(";

// WZD dseg:353B
char cnst_ClosingBrace_2[] = ")";

// WZD dseg:353D
char aMagicReserve[] = "Magic Reserve:";
// WZD dseg:354C
char aPowerBase[] = "Power Base:";
// WZD dseg:3558
char aCasting_0[] = "Casting:";

// WZD dseg:3561
char aResearching[] = "Researching:";

// WZD dseg:356E
char aSummonTo[] = "Summon To:";

// WZD dseg:3579
char aNone[] = "None";

// WZD dseg:357E
char aMore___[] = "More...";

// WZD dseg:3586 4D 00                                           cnst_HOTKEY_M_3 db 'M',0                ; DATA XREF: Magic_Screen_Add_Fields+Co
// WZD dseg:3588 53 00                                           cnst_HOTKEY_S_2 db 'S',0                ; DATA XREF: Magic_Screen_Add_Fields+70o

// WZD dseg:358A
char cnst_HOTKEY_O_7 = 'O';

// WZD dseg:358C
char cnst_HOTKEY_A_2[] = "A";

// WZD dseg:358E
char magic_lbx_file[] = "MAGIC";

// WZD dseg:3592
char cnst_HOTKEY_C_5 = 'C';

// WZD dseg:3594
char lilwiz_lbx_file[] = "LILWIZ";

// WZD dseg:359B
char cnst_GP_3[] = "GP";
// WZD dseg:359E
char cnst_PP[] = "PP";
// WZD dseg:35A1
char cnst_HOTKEY_SPACE[] = " ";

// WZD dseg:35A3 00                                              align 2

// WZD dseg:35A3                                                 END: ovr073 - Strings

// WZD dseg:35A4                                                 BEGIN: ovr074 - Strings

// WZD dseg:35A4
char diplo_state[11][9] =
{
    "Hate",
    "Troubled",
    "Tense",
    "Restless",
    "Unease",
    "Neutral",
    "Relaxed",
    "Calm",
    "Peaceful",
    "Friendly",
    "Harmony"
};

// WZD dseg:35BA
char * _treaty_type_names[5] =
{
    aNoTreaty,
    aWizardPact_1,
    aAlliance_1,
    aWar,
    aFinalWar
};

// WZD dseg:35C4
char aHate[] = "Hate";                         
// WZD dseg:35C8 00                                              mirror_screen_strings_null_terminator db 0

// WZD dseg:35C9 54 72 6F 75 62 6C 65 64 00                      aTroubled db 'Troubled',0
// WZD dseg:35D2 54 65 6E 73 65 00                               aTense db 'Tense',0
// WZD dseg:35D8 52 65 73 74 6C 65 73 73 00                      aRestless db 'Restless',0
// WZD dseg:35E1 55 6E 65 61 73 65 00                            aUnease db 'Unease',0
// WZD dseg:35E8 4E 65 75 74 72 61 6C 00                         aNeutral db 'Neutral',0
// WZD dseg:35F0 52 65 6C 61 78 65 64 00                         aRelaxed db 'Relaxed',0
// WZD dseg:35F8 43 61 6C 6D 00                                  aCalm db 'Calm',0
// WZD dseg:35FD 50 65 61 63 65 66 75 6C 00                      aPeaceful_0 db 'Peaceful',0
// WZD dseg:3606 46 72 69 65 6E 64 6C 79 00                      aFriendly db 'Friendly',0
// WZD dseg:360F 48 61 72 6D 6F 6E 79 00                         aHarmony db 'Harmony',0

// WZD dseg:3617                                                 BEGIN:  _treaty_type_names
// WZD dseg:3617
char aNoTreaty[] = "No Treaty";
// WZD dseg:3621
char aWizardPact_1[] = "Wizard Pact";
// WZD dseg:362D
char aAlliance_1[] = "Alliance";
// WZD dseg:3636
char aWar[] = "War";
// WZD dseg:363A
char aFinalWar[] = "Final War";
// WZD dseg:363A                                                 END:  _treaty_type_names

// WZD dseg:3644 1B 00                                           cnst_HOTKEY_Esc8 db 1Bh,0               ; should use dseg:2c56
// WZD dseg:3646 20 00                                           cnst_HOTKEY_SPACE_2 db ' ',0            ; DATA XREF: Mirror_Screen_Draw+8o
// WZD dseg:3648 52 65 6C 61 74 69 6F 6E 73 3A 00                aRelations db 'Relations:',0            ; DATA XREF: Mirror_Screen_Draw+17Fo
// WZD dseg:3653 54 72 65 61 74 69 65 73 3A 00                   aTreaties db 'Treaties:',0              ; DATA XREF: Mirror_Screen_Draw+1F5o
// WZD dseg:365D 50 65 72 73 6F 6E 61 6C 69 74 79 3A 00          aPersonality db 'Personality:',0        ; DATA XREF: Mirror_Screen_Draw+2B3o
// WZD dseg:366A 4F 62 6A 65 63 74 69 76 65 3A 00                aObjective db 'Objective:',0            ; DATA XREF: Mirror_Screen_Draw+342o
// WZD dseg:3675 47 50 00                                        cnst_GP_4 db 'GP',0                     ; DATA XREF: Mirror_Screen_Draw+444o
// WZD dseg:3678 4D 50 00                                        cnst_MP_3 db 'MP',0                     ; DATA XREF: Mirror_Screen_Draw+45Eo
// WZD dseg:367B 46 61 6D 65 00                                  cnst_Fame db 'Fame',0                   ; DATA XREF: Mirror_Screen_Draw+520o
// WZD dseg:3680 61 6E 64 00                                     mirror_screen_abilities_list_and db 'and',0
// WZD dseg:3684 2C 00                                           mirror_screen_abilities_list_comma db ',',0
// WZD dseg:3686 2E 00                                           mirror_screen_abilities_list_period db '.',0
// WZD dseg:3688 48 65 72 6F 65 73 00                            aHeroes db 'Heroes',0                   ; DATA XREF: Mirror_Screen_Draw+759o
// WZD dseg:368F 54 68 65 00                                     cnst_The_2 db 'The',0                   ; DATA XREF: Mirror_Screen_Draw+801o
// WZD dseg:3693 4C 49 4C 57 49 5A 00                            mirror_lilwiz_lbx_file db 'LILWIZ',0    ; DATA XREF: Mirror_Screen_Load+5Bo
// WZD dseg:369A 4D 41 47 49 43 00                               mirror_magic_lbx_file db 'MAGIC',0      ; DATA XREF: Mirror_Screen_Load+78o ...

// WZD dseg:369A                                                 END: ovr074 - Strings





// dseg:958E                                                 BEGIN:  ovr073 - Uninitialized Data

// WZD dseg:958E
/*
    ¿ 28 DB, 14 DW, 7 DD ? ¿ gem wiz picts - player 0 always human, always index 0 ?
    ~ Wizard Picture
    
*/
SAMB_ptr lilwiz_gem_segs[5];

// WZD dseg:959A 00                                              db    0
// WZD dseg:959B 00                                              db    0
// WZD dseg:959C 00                                              db    0
// WZD dseg:959D 00                                              db    0
// WZD dseg:959E 00                                              db    0
// WZD dseg:959F 00                                              db    0
// WZD dseg:95A0 00                                              db    0
// WZD dseg:95A1 00                                              db    0
// WZD dseg:95A2 00                                              db    0
// WZD dseg:95A3 00                                              db    0
// WZD dseg:95A4 00                                              db    0
// WZD dseg:95A5 00                                              db    0
// WZD dseg:95A6 00                                              db    0
// WZD dseg:95A7 00                                              db    0
// WZD dseg:95A8 00                                              db    0
// WZD dseg:95A9 00                                              db    0

// WZD dseg:95AA
SAMB_ptr skill_staff_locked_seg;

// WZD dseg:95AC
SAMB_ptr research_staff_locked_seg;

// WZD dseg:95AE
SAMB_ptr mana_staff_locked_seg;

// WZD dseg:95B0
SAMB_ptr magic_ok_button_seg;

// WZD dseg:95B2
SAMB_ptr magic_alchemy_button_seg;

// WZD dseg:95B4
SAMB_ptr skill_staff_full_seg;

// WZD dseg:95B6
SAMB_ptr skill_staff_empty_seg;

// WZD dseg:95B8
SAMB_ptr research_staff_full_seg;

// WZD dseg:95BA
SAMB_ptr research_staff_empty_seg;

// WZD dseg:95BC
SAMB_ptr mana_staff_full_seg;

// WZD dseg:95BE
SAMB_ptr mana_staff_empty_seg;

// WZD dseg:95C0
SAMB_ptr grey_gem_seg;

// WZD dseg:95C2
SAMB_ptr broken_grey_gem_seg;

// WZD dseg:95C4 00 00                                           dw 0
// WZD dseg:95C6 00 00                                           dw 0
// WZD dseg:95C8 00 00                                           dw 0
// WZD dseg:95CA 00 00                                           dw 0
// WZD dseg:95CC 00 00                                           dw 0

// WZD dseg:95CE
SAMB_ptr magic_background_seg;





// dseg:C1EA                                                 ¿ BEGIN: ovr073 - Uninitialized Data ?

// WZD dseg:C1EA
SAMB_ptr magic_dipl_icon_segs[15];

// WZD dseg:C208
int16_t m_alchemy_divisor;

// WZD dseg:C20A
int16_t human_player_summoning_circle_city_idx;

// WZD dseg:C20C
int16_t gem_player_nums[4];

// WZD dseg:C214
int16_t gem_count;

// WZD dseg:C216
int16_t m_alchemy_popup_exit_field;
// WZD dseg:C218
int16_t m_alchemy_popup_window_field;
// WZD dseg:C21A
int16_t m_alchemy_arrow_button_field;
// WZD dseg:C21C
int16_t m_alchemy_ok_button_field;
// WZD dseg:C21E
int16_t m_alchemy_cancel_button_field;

// WZD dseg:C220
int16_t m_alchemy_arrowbar_cycle;
// WZD dseg:C222
int16_t m_alchemy_arrowbar_pos;
// WZD dseg:C224
int16_t m_alchemy_amount;
// WZD dseg:C226
int16_t m_alchemy_popup_start_y;
// WZD dseg:C228
int16_t m_alchemy_popup_start_x;

// WZD dseg:C22A
SAMB_ptr m_alchemy_bar_bitm_seg;

// WZD dseg:C22C
int16_t ovl_ench_list_cnt;

// WZD dseg:C22E
SAMB_ptr magic_gem_fields;

// WZD dseg:C230
SAMB_ptr magic_ovl_ench_flds;

// WZD dseg:C232
int16_t magic_ovl_ench_list_down;

// WZD dseg:C234
/*
    flag - controls creation of field for ovl ench list down
*/
int16_t magic_ovl_ench_list_scroll_flag;

// WZD dseg:C236
int16_t magic_ovl_ench_list_first_item;

// WZD dseg:C238
int16_t ovl_ench_cnt; // overland_enchantment_count

// WZD dseg:C23A
int8_t * ovl_ench_list_players;

// WZD dseg:C23C
/*
spell_idx
*/
// SAMB_ptr ovl_ench_list_spells;
int16_t * ovl_ench_list_spells;

// WZD dseg:C23E
int16_t button_magic_alchemy;

// WZD dseg:C240
int16_t button_magic_ok;

// WZD dseg:C242
int16_t mana_stave_pct;

// WZD dseg:C244
int16_t research_stave_pct;

// WZD dseg:C246
int16_t skill_stave_pct;

// WZD dseg:C248
int16_t  magic_mana_staff_locked;

// WZD dseg:C24A
int16_t  magic_research_staff_locked;

// WZD dseg:C24C
int16_t  magic_skill_staff_locked;

// WZD dseg:C24E
int16_t mana_stave_pct_pos;

// WZD dseg:C250
int16_t research_stave_pct_pos;

// WZD dseg:C252
int16_t skill_stave_pct_pos;

// WZD dseg:C252                                                 ¿ END: ovr073 - Uninitialized Data ?



// WZD o73p01
void Magic_Screen(void)
{
    int16_t multihotkey_PWR;
    int16_t multihotkey_RVL;
    int16_t multihotkey_DESTIN;
    int16_t hotkey_ESC;
    int16_t gem_y2;
    int16_t gem_x2;
    int16_t gem_y1;
    int16_t gem_x1;
    int16_t hotkey_X;
    int16_t screen_changed;
    uint8_t * players_globals;
    int16_t leave_screen_flag;

    int16_t itr_help;
    int16_t itr_players;
    int16_t input_field_idx;

    int16_t itr;  // _SI_


    Deactivate_Help_List();
    Set_Magic_Screen_Help_List();

    screen_changed = ST_FALSE;

    for(itr_help = 0; itr_help < NUM_GEMS; itr_help++)
    {
        // SEE: MoX-Help.md  ¿ * 10 because record_size is 10 ?
        // TODO  *(_help_entries + (itr_help * 10)) = HLP_GRAY_GEM;  // HLP_GRAY_GEM = 262
        // TODO  *(_help_entries + 0x64 + (itr_help * 10)) = ST_UNDEFINED;
    }

    gem_count = 0;

    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {
        gem_player_nums[gem_count] = -2;
        // TODO  _help_entries[(IDK_CnctWzdsCnt_w42CB4 * 10)] = HLP_SHATTERED_GEM;  // HLP_SHATTERED_GEM = 263
        // TODO  _help_entries[(100 + (IDK_CnctWzdsCnt_w42CB4 * 10))] = ST_UNDEFINED;

        if(_FORTRESSES[itr_players].active == ST_TRUE)
        {
            gem_player_nums[gem_count] = -1;
            // TODO  _help_entries[(gem_count * 10)] = HLP_GRAY_GEM;  // HLP_GRAY_GEM = 262
        }

        if(_FORTRESSES[itr_players].active == ST_TRUE)
        {
            if(_players[_human_player_idx].Dipl.Contacted[itr_players] == ST_TRUE)
            {
                gem_player_nums[gem_count] = itr_players;
                // *(_help_entries + (itr_help * 10)) = ST_UNDEFINED;
                // TODO  _help_entries[(100 + (IDK_CnctWzdsCnt_w42CB4 * 10))] = HLP_RELATIONS;  // HLP_RELATIONS = 271

            }
        }
        gem_count++;
    }

    Magic_Screen_Load();


    /*
        BEGIN:  rebase MP, RP, SP
    */
    skill_stave_pct_pos = (_players[HUMAN_PLAYER_IDX].skill_ratio / 2);
    research_stave_pct_pos = (_players[HUMAN_PLAYER_IDX].research_ratio / 2);
    mana_stave_pct_pos = (50 - skill_stave_pct_pos - research_stave_pct_pos);
    skill_stave_pct = skill_stave_pct_pos;
    research_stave_pct = research_stave_pct_pos;
    mana_stave_pct = mana_stave_pct_pos;
    Players_Update_Magic_Power();
    /*
        END:  rebase MP, RP, SP
    */

    Deactivate_Auto_Function();
    Assign_Auto_Function(Magic_Screen_Draw, 1);

    magic_ovl_ench_list_first_item = 0;
    Build_Overland_Enchantment_List();
    magic_ovl_ench_list_scroll_flag = ST_FALSE;
    if(ovl_ench_cnt > 18)
    {
        magic_ovl_ench_list_scroll_flag = ST_TRUE;
    }

    human_player_summoning_circle_city_idx = Player_Summon_City(_human_player_idx);

    Set_Input_Delay(1);

    leave_screen_flag = ST_FALSE;

    multihotkey_RVL = Add_Multi_Hot_Key_Field(aRvl);
    multihotkey_PWR = Add_Multi_Hot_Key_Field(aPwr);
    multihotkey_DESTIN = Add_Multi_Hot_Key_Field(aDestin);


    while(leave_screen_flag == ST_FALSE)
    {

        Mark_Time();

        Clear_Fields_Above(multihotkey_DESTIN);

        Magic_Screen_Add_Fields();

        hotkey_X = Add_Hot_Key('X');  // cnst_HOTKEY_X_5

        hotkey_ESC = Add_Hot_Key('\x1B');  // cnst_HOTKEY_Esc7

        input_field_idx = Get_Input();


        if(input_field_idx == multihotkey_RVL)
        {
            Cheat_Reveal();
            screen_changed = ST_TRUE;
        }

        if(input_field_idx == multihotkey_PWR)
        {
            Cheat_Power();
            screen_changed = ST_TRUE;
        }

        if(input_field_idx == multihotkey_DESTIN)
        {
            Warn1(aWriteItUpGiveMeASav);  // "Write it up, give me a save game."
            Assign_Auto_Function(Magic_Screen_Draw, 1);
            Deactivate_Help_List();
            Set_Magic_Screen_Help_List();
            screen_changed = ST_TRUE;
        }
        
        if(input_field_idx == magic_mana_staff_locked)
        {
            Play_Left_Click();
            // mov  ax, [mana_staff_locked];  neg  ax;  sbb  ax, ax;  inc  ax;  mov  [mana_staff_locked], ax;
            mana_staff_locked = (1 - mana_staff_locked);
        }

        if(input_field_idx == magic_skill_staff_locked)
        {
            Play_Left_Click();
            // mov  ax, [skill_staff_locked];  neg  ax;  sbb  ax, ax;  inc  ax;  mov  [skill_staff_locked], ax;
            skill_staff_locked = (1 - skill_staff_locked);
        }
        
        if(input_field_idx == magic_research_staff_locked)
        {
            Play_Left_Click();
            // mov  ax, [research_staff_locked];  neg  ax;  sbb  ax, ax;  inc  ax;  mov  [research_staff_locked], ax;
            research_staff_locked = (1 - research_staff_locked);
        }
        
        /*
            OK button || ESC hotkey
        */
       if((input_field_idx == button_magic_ok) || (input_field_idx == hotkey_ESC))
       {
            Play_Left_Click();
            leave_screen_flag = ST_UNDEFINED;
       }

        for(itr = 0; itr < ovl_ench_list_cnt; itr++)
        {
            if((magic_ovl_ench_flds[itr] == input_field_idx) && (ovl_ench_list_players[(magic_ovl_ench_list_first_item + itr)] == _human_player_idx))
            {
                Play_Left_Click();
                strcpy(GUI_String_1, aDoYouWishToCancelYour);  // "Do you wish to cancel your \x02"
                strcpy(GUI_String_2, spell_data_table[ovl_ench_list_spells[(magic_ovl_ench_list_first_item + itr)]].name);
                strcat(GUI_String_1, GUI_String_2);
                strcat(GUI_String_1, aSpell_1);  // "\x01 spell."  // BUGBUG  should be "...spell?"
                if(Confirmation_Box(GUI_String_1) == ST_TRUE)
                {
                    players_globals = &_players[HUMAN_PLAYER_IDX].Globals[0];
                    players_globals[spell_data_table[ovl_ench_list_spells[(magic_ovl_ench_list_first_item + itr)]].Param0] = 0;
                    if(spell_data_table[ovl_ench_list_spells[(magic_ovl_ench_list_first_item + itr)]].Param0 == NATURES_AWARENESS)
                    {
                        Update_Scouted_And_Contacted__WIP();
                    }
                    if(spell_data_table[ovl_ench_list_spells[(magic_ovl_ench_list_first_item + itr)]].Param0 == TIME_STOP)
                    {
                        g_timestop_player_num = 0;  // NONE
                    }
                    magic_ovl_ench_list_first_item = 0;
                    Build_Overland_Enchantment_List();
                    magic_ovl_ench_list_scroll_flag = ST_FALSE;
                    if(ovl_ench_cnt > 18)
                    {
                        magic_ovl_ench_list_scroll_flag = ST_TRUE;
                    }
                }
                Assign_Auto_Function(Magic_Screen_Draw, 1);
                Deactivate_Help_List();
                Set_Magic_Screen_Help_List();
                screen_changed = ST_TRUE;
                Clear_Fields();
                multihotkey_RVL = Add_Multi_Hot_Key_Field(aRvl);
                multihotkey_PWR = Add_Multi_Hot_Key_Field(aPwr);
                multihotkey_DESTIN = Add_Multi_Hot_Key_Field(aDestin);
            }
        }
        
        /*
            BEGIN:  Alchemy Button
        */
        if(input_field_idx == button_magic_alchemy)
        {
            Play_Left_Click();
            Alchemy_Popup(80, 60);

            Assign_Auto_Function(Magic_Screen_Draw, 1);
            Magic_Screen_Load();
            Build_Overland_Enchantment_List();
            screen_changed = ST_TRUE;
            if((WIZ_ManaPerTurn + _players[HUMAN_PLAYER_IDX].mana_reserve) > _players[HUMAN_PLAYER_IDX].Nominal_Skill)
            {
                SBK_BookManaLimit = _players[HUMAN_PLAYER_IDX].Nominal_Skill;
            }
            else if(WIZ_ManaPerTurn > _players[HUMAN_PLAYER_IDX].mana_reserve)
            {
                SBK_BookManaLimit = (WIZ_ManaPerTurn + _players[HUMAN_PLAYER_IDX].mana_reserve);
            }
            else
            {
                SBK_BookManaLimit = 0;
            }
            Deactivate_Help_List();
            Set_Magic_Screen_Help_List();
            _page_flip_effect = 3;
            Clear_Fields();
            multihotkey_RVL = Add_Multi_Hot_Key_Field(aRvl);
            multihotkey_PWR = Add_Multi_Hot_Key_Field(aPwr);
            multihotkey_DESTIN = Add_Multi_Hot_Key_Field(aDestin);
            screen_changed = ST_TRUE;
        }
        /*
            END:  Alchemy Button
        */


        /*
            BEGIN:  Right-Click Gem
        */
        for(itr = 0; itr < gem_count; itr++)
        {
            if((-(magic_gem_fields[itr]) == input_field_idx) && (gem_player_nums[itr] > 0))
            {
                Play_Left_Click();
                gem_x1 = (29 + (77 * itr));
                gem_y1 = 10;
                gem_x2 = (gem_x1 + 30);
                gem_y2 = (gem_y1 + 30);
                Mirror_Screen(gem_player_nums[itr], gem_x1, gem_y1, gem_x2, gem_y2);
                Magic_Screen_Load();
                Build_Overland_Enchantment_List();
                Assign_Auto_Function(Magic_Screen_Draw, 1);
                Deactivate_Help_List();
                Set_Magic_Screen_Help_List();
                Clear_Fields();
                multihotkey_RVL = Add_Multi_Hot_Key_Field(aRvl);
                multihotkey_PWR = Add_Multi_Hot_Key_Field(aPwr);
                multihotkey_DESTIN = Add_Multi_Hot_Key_Field(aDestin);
                screen_changed = ST_TRUE;
            }
        }
        /*
            END:  Right-Click Gem
        */


        /*
            BEGIN:  Left-Click Gem
        */
        for(itr = 0; itr < gem_count; itr++)
        {
            if((magic_gem_fields[itr] == input_field_idx) && (gem_player_nums[itr] > 0))
            {
                if(_players[_human_player_idx].casting_spell_idx == spl_Spell_Of_Return)
                {
                    Warn1(aYouMayNotContactOth);  // "You may not contact other wizards while you are banished."
                }
                else
                {
                    Play_Left_Click();
                    m_diplomac_player_idx = gem_player_nums[itr];
                    current_screen = scr_Diplomacy_Screen;
                    leave_screen_flag = 2;
                }
            }
        }
        /*
            END:  Left-Click Gem
        */


        if(magic_ovl_ench_list_down == abs(input_field_idx))
        {
            Play_Left_Click();
            magic_ovl_ench_list_first_item += 18;
            if(magic_ovl_ench_list_first_item >= ovl_ench_cnt)
            {
                magic_ovl_ench_list_first_item = 0;
            }
            screen_changed = ST_TRUE;
        }


        if(leave_screen_flag == ST_FALSE && screen_changed == ST_FALSE)
        {
            Magic_Screen_Draw();
            PageFlip_FX();
            Release_Time(1);
        }

        screen_changed = ST_FALSE;

    }

    // @@LeaveScreen:
    Clear_Fields();
    Deactivate_Auto_Function();
    Deactivate_Help_List();
    current_screen = scr_Main_Screen;
    if(leave_screen_flag == 2)
    {
        current_screen = scr_Diplomacy_Screen;
    }

}


/*

if( (mana_stave_pct_pos     + IDK  ) < 0) { skill_stave_pct_pos    = (skill_stave_pct        + mana_stave_pct_pos    ); mana_stave_pct_pos     = 0; } else { mana_stave_pct_pos     += IDK;   }
if( (research_stave_pct_pos + IDK  ) < 0) { skill_stave_pct_pos    = (skill_stave_pct        + research_stave_pct_pos); research_stave_pct_pos = 0; } else { research_stave_pct_pos += IDK;   }
if( (research_stave_pct_pos + var_2) < 0) { IDK                    = (research_stave_pct_pos + var_2                 ); research_stave_pct_pos = 0; } else { research_stave_pct_pos += var_2; }
if( (mana_stave_pct_pos     + IDK  ) < 0) { research_stave_pct_pos = (mana_stave_pct_pos     + IDK                   ); mana_stave_pct_pos     = 0; } else { mana_stave_pct_pos     += IDK;   }

*/

// #define ADJUST_STAVES(_staff1_,_staff2_,_staff3_) { \
//     if(skill_stave_pct != skill_stave_pct_pos) { \
//         IDK = skill_stave_pct - skill_stave_pct_pos; \
//         if(research_staff_locked + mana_staff_locked == 2) { \
//                 skill_stave_pct_pos = skill_stave_pct; \
//         } else { \
//             if((research_staff_locked != ST_FALSE) && (mana_staff_locked == ST_FALSE)) { \
//                 if((mana_stave_pct_pos + IDK) < 0) { \
//                     skill_stave_pct_pos = (skill_stave_pct + mana_stave_pct_pos); \
//                     mana_stave_pct_pos = 0; \
//                 } else { \
//                     mana_stave_pct_pos += IDK; \
//                 } \
//             } else if( (mana_staff_locked != ST_FALSE) && (research_staff_locked == ST_FALSE)) { \
//                 if((research_stave_pct_pos + IDK) < 0) { \
//                     skill_stave_pct_pos = (skill_stave_pct + research_stave_pct_pos); \
//                     research_stave_pct_pos = 0; \
//                 } else { \
//                     research_stave_pct_pos += IDK; \
//                 } \
//             } else { /* skill changed, neither mana nor research are locked */ \
//                 skill_stave_pct_pos += (IDK % 2); \
//                 var_2 = (IDK / 2); \
//                 IDK = var_2; \
//                 if((research_stave_pct_pos + var_2) < 0) { \
//                     IDK = (research_stave_pct_pos + var_2); \
//                     research_stave_pct_pos = 0; \
//                 } else { \
//                     research_stave_pct_pos += var_2; \
//                 } \
//                 if((mana_stave_pct_pos + IDK) < 0) \
//                 { \
//                     research_stave_pct_pos = (mana_stave_pct_pos + IDK); \
//                     mana_stave_pct_pos = 0; \
//                 } else { \
//                     mana_stave_pct_pos += IDK; \
//                 } \
//             } \
//         } \
//     } \
// }

// WZD o73p02
/*

NOTE(JimBalcomb,20240622):  should be all done now, except for HELP and constants
*/
void Magic_Screen_Draw(void)
{
    int16_t y_start;
    int16_t x_start;
    int16_t diplomatic_treaties;
    int16_t itr_players_help;
    uint8_t colors1[NUM_PLAYERS];
    uint8_t colors2[NUM_PLAYERS];
    int16_t diplomatic_relations_idx;
    int16_t itr_treaties;
    int16_t research;
    int16_t skill;
    int16_t mana;
    int16_t var_2;
    int16_t itr;  // _SI_
    int16_t IDK;  // _DI_
    int16_t itr_help_entries;  // _SI_
    int16_t itr_players;  // _SI_
    int16_t itr_gems;  // _SI_
    int16_t itr_ovl_enchs;  // _SI_

    // TODO  ¿ player banner colors ?  add manifest constants
    colors1[0] = BANNER_COLOR_BLUE;
    colors1[1] = BANNER_COLOR_GREEN;
    colors1[2] = BANNER_COLOR_PURPLE_123;
    colors1[3] = BANNER_COLOR_RED;
    colors1[4] = BANNER_COLOR_YELLOW;

    for(itr_help_entries = 0; itr_help_entries < 4; itr_help_entries++)
    {
        // TODO  *(_help_entries +   0 + (itr_help_entries * 10)) = HLP_GRAY_GEM;
        // TODO  *(_help_entries + 100 + (itr_help_entries * 10)) = ST_UNDEFINED;
        // TODO  *(_help_entries + 430 + (itr_help_entries * 10)) = ST_UNDEFINED;
    }

    for(itr_players_help = 0, itr_players = 1; itr_players < _num_players; itr_players_help++, itr_players++)
    {
        // TODO  *(_help_entries +   0 + (itr_players_help * 10)) = HLP_SHATTERED_GEM;
        // TODO  *(_help_entries + 100 + (itr_players_help * 10)) = ST_UNDEFINED;
        if(_FORTRESSES[itr_players].active == ST_TRUE)
        {
            // TODO  *(_help_entries +   0 + (itr_players_help * 10)) = HLP_GRAY_GEM;
        }
        if((_FORTRESSES[itr_players].active == ST_TRUE) && (_players[_human_player_idx].Dipl.Contacted[itr_players] == ST_TRUE))
        {
            // TODO  *(_help_entries +   0 + (itr_players_help * 10)) = ST_UNDEFINED;
            // TODO  *(_help_entries + 100 + (itr_players_help * 10)) = HLP_RELATIONS;
        }
    }

    Set_Page_Off();

    FLIC_Draw(0, 0, magic_background_seg);

    Set_Animation_Frame(magic_ok_button_seg, 0);
    FLIC_Draw(291, 181, magic_ok_button_seg);

    Set_Animation_Frame(magic_alchemy_button_seg, 0);
    FLIC_Draw(232, 181, magic_alchemy_button_seg);


    /*
        BEGIN: Draw Staves
    */
    if((skill_stave_pct != skill_stave_pct_pos) && (skill_staff_locked == ST_TRUE))
    {
        skill_stave_pct_pos = skill_stave_pct;
    }
    if((research_stave_pct != research_stave_pct_pos) && (research_staff_locked == ST_TRUE))
    {
        research_stave_pct_pos = research_stave_pct;
    }
    if((mana_stave_pct != mana_stave_pct_pos) && (mana_staff_locked == ST_TRUE))
    {
        mana_stave_pct_pos = mana_stave_pct;
    }

    /* skill changed; adjust mana, research, or both */
    if(skill_stave_pct != skill_stave_pct_pos)
    {
        IDK = skill_stave_pct - skill_stave_pct_pos;
        if(research_staff_locked + mana_staff_locked == 2)
        {
            skill_stave_pct_pos = skill_stave_pct;
        }
        else
        {
            if((research_staff_locked == ST_TRUE) && (mana_staff_locked == ST_FALSE))
            {
                if((mana_stave_pct_pos + IDK) < 0)
                {
                    skill_stave_pct_pos = (skill_stave_pct + mana_stave_pct_pos);
                    mana_stave_pct_pos = 0;
                }
                else
                {
                    mana_stave_pct_pos += IDK;
                }
            }
            else if((mana_staff_locked == ST_TRUE) && (research_staff_locked == ST_FALSE))
            {
                if((research_stave_pct_pos + IDK) < 0)
                {
                    skill_stave_pct_pos = (skill_stave_pct + research_stave_pct_pos);
                    research_stave_pct_pos = 0;
                }
                else
                {
                    research_stave_pct_pos += IDK;
                }
            }
            else  /* skill changed, neither mana nor research are locked */
            {
                skill_stave_pct_pos += (IDK % 2);
                var_2 = (IDK / 2);
                IDK = var_2;
                if((research_stave_pct_pos + var_2) < 0)
                {
                    IDK += (research_stave_pct_pos + var_2);
                    research_stave_pct_pos = 0;
                }
                else
                {
                    research_stave_pct_pos += var_2;
                }
                if((mana_stave_pct_pos + IDK) < 0)
                {
                    research_stave_pct_pos = (mana_stave_pct_pos + IDK);
                    mana_stave_pct_pos = 0;
                }
                else
                {
                    mana_stave_pct_pos += IDK;
                }
            }
        }

    }
    /* research changed; adjust mana, skill, or both */
    else if(research_stave_pct != research_stave_pct_pos)
    {
        IDK = research_stave_pct - research_stave_pct_pos;
        if(skill_staff_locked + mana_staff_locked == 2)
        {
            research_stave_pct_pos = research_stave_pct;
        }
        else
        {
            if((skill_staff_locked == ST_TRUE) && (mana_staff_locked == ST_FALSE))
            {
                if((mana_stave_pct_pos + IDK) < 0)
                {
                    research_stave_pct_pos = (research_stave_pct + mana_stave_pct_pos);
                    mana_stave_pct_pos = 0;
                }
                else
                {
                    mana_stave_pct_pos += IDK;
                }
            }
            else if((mana_staff_locked == ST_TRUE) && (skill_staff_locked == ST_FALSE))
            {
                if((skill_stave_pct_pos + IDK) < 0)
                {
                    research_stave_pct_pos = (research_stave_pct + skill_stave_pct_pos);
                    skill_stave_pct_pos = 0;
                }
                else
                {
                    skill_stave_pct_pos += IDK;
                }
            }
            else  /* research changed, neither skill nor mana are locked */
            {
                research_stave_pct_pos += (IDK % 2);
                var_2 = (IDK / 2);
                IDK = var_2;
                if((skill_stave_pct_pos + var_2) < 0)
                {
                    IDK += (skill_stave_pct_pos + var_2);
                    skill_stave_pct_pos = 0;
                }
                else
                {
                    skill_stave_pct_pos += var_2;
                }
                if((mana_stave_pct_pos + IDK) < 0)
                {
                    skill_stave_pct_pos = (mana_stave_pct_pos + IDK);
                    mana_stave_pct_pos = 0;
                }
                else
                {
                    mana_stave_pct_pos += IDK;
                }
            }
        }

    }
    /* mana changed; adjust mana, research, or both */
    else if(mana_stave_pct != mana_stave_pct_pos)
    {
        IDK = mana_stave_pct - mana_stave_pct_pos;
        if(research_staff_locked + skill_staff_locked == 2)
        {
            mana_stave_pct_pos = mana_stave_pct;
        }
        else
        {
            if((research_staff_locked == ST_TRUE) && (skill_staff_locked == ST_FALSE))
            {
                if((skill_stave_pct_pos + IDK) < 0)
                {
                    mana_stave_pct_pos = (mana_stave_pct + skill_stave_pct_pos);
                    skill_stave_pct_pos = 0;
                }
                else
                {
                    skill_stave_pct_pos += IDK;
                }
            }
            else if((skill_staff_locked == ST_TRUE) && (research_staff_locked == ST_FALSE))
            {
                if((research_stave_pct_pos + IDK) < 0)
                {
                    mana_stave_pct_pos = (mana_stave_pct + research_stave_pct_pos);
                    research_stave_pct_pos = 0;
                }
                else
                {
                    research_stave_pct_pos += IDK;
                }
            }
            else  /* mana changed, neither research nor skill are locked */
            {
                mana_stave_pct_pos += (IDK % 2);
                var_2 = (IDK / 2);
                IDK = var_2;
                if((research_stave_pct_pos + var_2) < 0)
                {
                    IDK += (research_stave_pct_pos + var_2);
                    research_stave_pct_pos = 0;
                }
                else
                {
                    research_stave_pct_pos += var_2;
                }
                if((skill_stave_pct_pos + IDK) < 0)
                {
                    research_stave_pct_pos = (skill_stave_pct_pos + IDK);
                    skill_stave_pct_pos = 0;
                }
                else
                {
                    skill_stave_pct_pos += IDK;
                }
            }
        }
    }

    mana_stave_pct = mana_stave_pct_pos;
    research_stave_pct = research_stave_pct_pos;
    skill_stave_pct = skill_stave_pct_pos;

    SETMIN(mana_stave_pct, 0);
    SETMIN(research_stave_pct, 0);
    SETMIN(skill_stave_pct, 0);

    Set_Window(0, 79, SCREEN_XMAX, (152 - mana_stave_pct_pos));
    Clipped_Draw(29, 83, mana_staff_empty_seg);
    Set_Window(0, (152 - mana_stave_pct_pos), SCREEN_XMAX, 166);
    if(mana_staff_locked != ST_FALSE)
    {
        FLIC_Draw(27, 81, mana_staff_locked_seg);
    }
    Clipped_Draw(32, 102, mana_staff_full_seg);

    Set_Window(0, 79, SCREEN_XMAX, (152 - research_stave_pct_pos));
    Clipped_Draw(75, 85, research_staff_empty_seg);
    Set_Window(0, (152 - research_stave_pct_pos), SCREEN_XMAX, 166);
    if(research_staff_locked != ST_FALSE)
    {
        FLIC_Draw(74, 81, research_staff_locked_seg);
    }
    Clipped_Draw(79, 102, research_staff_full_seg);
    
    Set_Window(0, 79, SCREEN_XMAX, (152 - skill_stave_pct_pos));
    Clipped_Draw(122, 83, skill_staff_empty_seg);
    Set_Window(0, (152 - skill_stave_pct_pos), SCREEN_XMAX, 166);
    if(skill_staff_locked != ST_FALSE)
    {
        FLIC_Draw(121, 81, skill_staff_locked_seg);
    }
    Clipped_Draw(126, 102, skill_staff_full_seg);

    Reset_Window();

    _players[HUMAN_PLAYER_IDX].mana_ratio     = (mana_stave_pct_pos     * 2);
    _players[HUMAN_PLAYER_IDX].research_ratio = (research_stave_pct_pos * 2);
    _players[HUMAN_PLAYER_IDX].skill_ratio    = (skill_stave_pct_pos    * 2);

    /*
        END: Draw Staves
    */

    
    Player_Magic_Power_Income_Total(&mana, &research, &skill, 0);

    Set_Font_Style_Shadow_Down(2, 3, 0, 0);
    Set_Outline_Color(19);
    Set_Alias_Color(185);

    itoa(mana, GUI_String_1, 10);
    strcat(GUI_String_1, cnst_Space_MP);
    Print_Right(54, 160, GUI_String_1);

    if(_players[HUMAN_PLAYER_IDX].researching_spell_idx == 0)
    {
        Print(67, 100, cnst_NoSpell);
    }
    else
    {
        itoa(research, GUI_String_1, 10);
        strcat(GUI_String_1, aRp_0);
        Print_Right(101, 160, GUI_String_1);
    }

    itoa(skill, GUI_String_1, 10);
    strcat(GUI_String_1, aSp);
    Print_Right(148, 160, GUI_String_1);


    Set_Font_Style_Shadow_Down(1, 3, 0, 0);
    Set_Outline_Color(240);
    Set_Alias_Color(228);
    Set_Font_Spacing_Width(1);

    /*
        "Casting Skill:"
    */
    Print(5, 177, aCastingSkill);
    itoa((Player_Base_Casting_Skill(_human_player_idx) + Player_Hero_Casting_Skill(_human_player_idx)), GUI_String_1, 10);
    itoa(Player_Base_Casting_Skill(_human_player_idx), GUI_String_2, 10);
    strcat(GUI_String_1, cnst_OpeningBrace_2);
    strcat(GUI_String_1, GUI_String_2);
    strcat(GUI_String_1, cnst_ClosingBrace_2);
    Print_Right(90, 177, GUI_String_1);

    /*
        "Magic Reserve:"
    */
    Print(5, 185, aMagicReserve);
    if(_players[HUMAN_PLAYER_IDX].mana_reserve > 19999)
    {
        Set_Font_Style_Shadow_Down(0, 3, 0, 0);
        Set_Outline_Color(240);
        Set_Alias_Color(228);
        Set_Font_Spacing_Width(1);
    }
    Print_Integer_Right(90, 185, _players[HUMAN_PLAYER_IDX].mana_reserve);

    Set_Font_Style_Shadow_Down(1, 3, 0, 0);
    Set_Outline_Color(240);
    Set_Alias_Color(228);
    Set_Font_Spacing_Width(1);

    /*
        "Power Base:"
    */
    Print(5, 193, aPowerBase);
    Print_Integer_Right(90, 193, _players[HUMAN_PLAYER_IDX].Power_Base);

    /*
        "Casting:"
    */
    Print(100, 177, aCasting_0);
    // TODO  Print_Far()
    Print(156, 177, spell_data_table[_players[HUMAN_PLAYER_IDX].casting_spell_idx].name);

    /*
        "Researching:"
    */
    Print(100, 185, aResearching);
    // TODO  Print_Far()
    Print(156, 185, spell_data_table[_players[HUMAN_PLAYER_IDX].researching_spell_idx].name);

    /*
        "Summon To:"
    */
    Print(100, 193, aSummonTo);
    if(human_player_summoning_circle_city_idx == ST_UNDEFINED)
    {
        Print(156, 193, aNone);
    }
    else
    {
        // TODO  Print_Far()
        Print(156, 193, _CITIES[human_player_summoning_circle_city_idx].name);
    }


    /*
        BEGIN:
            Gem Portrait Pictures  (LILWIZ)
            Relations
            Treaties  (DIPLICON)
            'Detect Magic' - Casting
    */
    Set_Outline_Color(0);
    for(itr_gems = 0; itr_gems < 4; itr_gems++)
    {
        FLIC_Draw((24 + (77 * itr_gems)), 4, grey_gem_seg);
    }
    x_start = 68;  // ¿ only used for treaty icons ?
    for(itr_gems = 0; itr_gems < gem_count; itr_gems++)
    {
        y_start = 2;  // ¿ only used for treaty icons ?

        Set_Font_Style(0, 3, 0, 0);
        Set_Alias_Color(228);

        if(gem_player_nums[itr_gems] == -2)
        {
            FLIC_Draw((23 + (77 * itr_gems)), 3, broken_grey_gem_seg);
        }
        else if(gem_player_nums[itr_gems] == -1)
        {
            FLIC_Draw((24 + (77 * itr_gems)), 4, grey_gem_seg);
        }
        else
        {
            FLIC_Draw((24 + (77 * itr_gems)), 4, lilwiz_gem_segs[itr_gems]);

            diplomatic_relations_idx = ((_players[gem_player_nums[itr_gems]].Dipl.Visible_Rel[_human_player_idx] + 100) / 20);
            Print_Centered(45 + (77 * itr_gems), 53, diplo_state[diplomatic_relations_idx]);

            // wizard pact, alliance, war  ...  icons  ...  scroll, peace symbol, crossed swords
            diplomatic_treaties = (_players[gem_player_nums[itr_gems]].Dipl.Dipl_Status[_human_player_idx]);
            if(diplomatic_treaties > 0)
            {
                if(diplomatic_treaties > 2) { diplomatic_treaties = 3; }
                diplomatic_treaties--;
                FLIC_Draw(x_start, y_start, magic_dipl_icon_segs[diplomatic_treaties + (_players[_human_player_idx].banner_id * 3)]);  // status + color to index icons
                // TODO  _help_entries[43 + itr_gems] = HLP_TREATIES;
                y_start += 12;
            }

            for(itr_treaties = 0; itr_treaties < gem_count; itr_treaties++)
            {
                if((itr_treaties != itr_gems) && (gem_player_nums[itr_treaties] > ST_UNDEFINED))
                {
                    diplomatic_treaties = _players[gem_player_nums[itr_gems]].Dipl.Dipl_Status[gem_player_nums[itr_treaties]];
                    if(diplomatic_treaties > 0)
                    {
                        if(diplomatic_treaties > 2) { diplomatic_treaties = 3; }
                        diplomatic_treaties--;
                        FLIC_Draw(x_start, y_start, magic_dipl_icon_segs[diplomatic_treaties + (_players[gem_player_nums[itr_treaties]].banner_id * 3)]);  // status + color to index icons
                        // TODO  *(_help_entries + 430 + (itr_gems * 10)) = HLP_TREATIES;
                        y_start += 12;
                    }
                }
            }

            if(_players[_human_player_idx].Globals[DETECT_MAGIC] != ST_FALSE)
            {
                colors2[0] = 182;
                colors2[1] = 177;
                Set_Font_Colors_15(0, &colors2[0]);
                Set_Font_Style_Outline(0, 15, 0, 0);
                Set_Alias_Color(182);
                Set_Outline_Color(1);
                Print_Centered((45 + (77 * itr_gems)), 4, spell_data_table[_players[gem_player_nums[itr_gems]].casting_spell_idx].name);
            }

            x_start += 77;
        }
    }

    /*
        END:
            Gem Portrait Pictures  (LILWIZ)
            Relations
            Treaties  (DIPLICON)
            'Detect Magic' - Casting
    */



    /*
        BEGIN: Overland Enchantments
    */

    // TODO  _help_entries.help_09.entry_idx = HLP_ENCHANTMENTS_Global

    if(ovl_ench_cnt == 0)
    {
        // TODO  _help_entries.help_09.entry_idx = ST_UNDEFINED
    }

    if(ovl_ench_cnt < 10)
    {
        for(itr_ovl_enchs = 0; itr_ovl_enchs < ovl_ench_cnt; itr_ovl_enchs++)
        {
            // TODO  _help_entries[(16 + itr_ovl_enchs)].entry_idx = ST_UNDEFINED;
            colors2[0] = colors1[_players[ovl_ench_list_players[(magic_ovl_ench_list_first_item + itr_ovl_enchs)]].banner_id];
            colors2[1] = colors1[_players[ovl_ench_list_players[(magic_ovl_ench_list_first_item + itr_ovl_enchs)]].banner_id];
            colors2[2] = colors1[_players[ovl_ench_list_players[(magic_ovl_ench_list_first_item + itr_ovl_enchs)]].banner_id];
            colors2[3] = colors1[_players[ovl_ench_list_players[(magic_ovl_ench_list_first_item + itr_ovl_enchs)]].banner_id];
            colors2[4] = colors1[_players[ovl_ench_list_players[(magic_ovl_ench_list_first_item + itr_ovl_enchs)]].banner_id];
            colors2[5] = colors1[_players[ovl_ench_list_players[(magic_ovl_ench_list_first_item + itr_ovl_enchs)]].banner_id];
            Set_Font_Colors_15(2, &colors2[0]);
            Set_Font_Style_Shadow_Down(2, 15, 0, 0);
            Set_Outline_Color(19);
            Set_Alias_Color(185);
            Print(171, (80 + (10 * itr_ovl_enchs)), spell_data_table[ovl_ench_list_spells[(magic_ovl_ench_list_first_item + itr_ovl_enchs)]].name);
            // TODO  _help_entries[(16 + itr_ovl_enchs)].entry_idx = ovl_ench_list_spells[(magic_ovl_ench_list_first_item + itr_ovl_enchs)];
        }
    }
    else
    {
        for(itr = 0; itr < 18; itr++)
        {
            // TODO  _help_entries[(25 + itr)].entry_idx = ST_UNDEFINED;
        }
        for(itr_ovl_enchs = 0; itr_ovl_enchs < ovl_ench_list_cnt; itr_ovl_enchs++)
        {
            if(ovl_ench_list_spells[(magic_ovl_ench_list_first_item + itr_ovl_enchs)] > 0)
            {
                colors2[0] = colors1[_players[ovl_ench_list_players[(magic_ovl_ench_list_first_item + itr_ovl_enchs)]].banner_id];
                colors2[1] = colors1[_players[ovl_ench_list_players[(magic_ovl_ench_list_first_item + itr_ovl_enchs)]].banner_id];
                colors2[2] = colors1[_players[ovl_ench_list_players[(magic_ovl_ench_list_first_item + itr_ovl_enchs)]].banner_id];
                colors2[3] = colors1[_players[ovl_ench_list_players[(magic_ovl_ench_list_first_item + itr_ovl_enchs)]].banner_id];
                colors2[4] = colors1[_players[ovl_ench_list_players[(magic_ovl_ench_list_first_item + itr_ovl_enchs)]].banner_id];
                colors2[5] = colors1[_players[ovl_ench_list_players[(magic_ovl_ench_list_first_item + itr_ovl_enchs)]].banner_id];
                Set_Font_Colors_15(0, &colors2[0]);
                Set_Font_Style_Shadow_Down(0, 15, 0, 0);
                Set_Outline_Color(19);
                Set_Alias_Color(185);
                Print((171 + (69 * (itr_ovl_enchs % 2))), (81 + (9 * (itr_ovl_enchs / 2))), spell_data_table[ovl_ench_list_spells[(magic_ovl_ench_list_first_item + itr_ovl_enchs)]].name);
                // TODO  _help_entries[(25 + itr_ovl_enchs)].entry_idx = ovl_ench_list_spells[(magic_ovl_ench_list_first_item + itr_ovl_enchs)];
            }
        }
    }

    Set_Font_Style(0, 13, 0, 0);
    Set_Alias_Color(185);
    if(magic_ovl_ench_list_scroll_flag == ST_TRUE)
    {
        Print(286, 163, aMore___);  // "More..."
    }

    /*
        END: Overland Enchantments
    */

}

// WZD o73p03
void Magic_Screen_Add_Fields(void)
{
    int16_t itr_gem_count;
    int16_t x1;
    int16_t y1;
    int16_t itr_ovl_ench_cnt;
    int16_t itr_ovl_ench_list_cnt;

    //                         min_value, max_value, min_valid, max_valid, width, height
    Add_Scroll_Field( 32, 102, 0, 50, 0, 50, 5, 50, &mana_stave_pct_pos,     'M', ST_UNDEFINED);  // cnst_HOTKEY_M_3
    Add_Scroll_Field( 79, 102, 0, 50, 0, 50, 5, 50, &research_stave_pct_pos, 'R', ST_UNDEFINED);  // (offset aPwr+2)
    Add_Scroll_Field(126, 102, 0, 50, 0, 50, 5, 50, &skill_stave_pct_pos,    'S', ST_UNDEFINED);  // cnst_HOTKEY_S_2

    button_magic_ok       = Add_Button_Field(291, 181, str_empty_string__ovr073, magic_ok_button_seg,      'O', ST_UNDEFINED);  // cnst_HOTKEY_O_7
    button_magic_alchemy  = Add_Button_Field(232, 181, str_empty_string__ovr073, magic_alchemy_button_seg, 'A', ST_UNDEFINED);  // cnst_HOTKEY_A_2

    magic_mana_staff_locked      = Add_Hidden_Field( 28, 82,  41, 101, 0, ST_UNDEFINED);
    magic_research_staff_locked  = Add_Hidden_Field( 75, 82,  88, 101, 0, ST_UNDEFINED);
    magic_skill_staff_locked     = Add_Hidden_Field(122, 82, 135, 101, 0, ST_UNDEFINED);

    for(itr_gem_count = 0; itr_gem_count < gem_count; itr_gem_count++)
    {
        x1 = (29 + (77 * itr_gem_count));
        y1 = 10;
        magic_gem_fields[itr_gem_count] = Add_Hidden_Field(x1, y1, (x1 + 30), (y1 + 32), str_empty_string__ovr073[0], ST_UNDEFINED);
    }

    ovl_ench_list_cnt = 0;
    x1 = 169;
    y1 = 80;
    if(ovl_ench_cnt < 10)
    {
        ovl_ench_list_cnt = ovl_ench_cnt;
        for(itr_ovl_ench_cnt = 0; itr_ovl_ench_cnt < ovl_ench_cnt; itr_ovl_ench_cnt++)
        {
            magic_ovl_ench_flds[itr_ovl_ench_cnt] = Add_Hidden_Field(x1, (y1 + (10 * itr_ovl_ench_cnt)), (x1 + 120), (y1 + 9 + (10 * itr_ovl_ench_cnt)), str_empty_string__ovr073[0], ST_UNDEFINED);
        }
    }
    else
    {
        ovl_ench_list_cnt = ovl_ench_cnt - magic_ovl_ench_list_first_item;
        SETMAX(ovl_ench_list_cnt, 18);
        for(itr_ovl_ench_list_cnt = 0; itr_ovl_ench_list_cnt < ovl_ench_list_cnt; itr_ovl_ench_list_cnt++)
        {
            magic_ovl_ench_flds[itr_ovl_ench_list_cnt] = Add_Hidden_Field((x1 + (70 * (itr_ovl_ench_list_cnt % 2))), (y1 + (9 * (itr_ovl_ench_list_cnt / 2))), (63 + (x1 + (70 * (itr_ovl_ench_list_cnt % 2)))), (6 + (y1 + (9 * (itr_ovl_ench_list_cnt / 2)))), str_empty_string__ovr073[0], ST_UNDEFINED);
        }
    }

    magic_ovl_ench_list_down = INVALID_FIELD;
    if(magic_ovl_ench_list_scroll_flag == ST_TRUE)
    {
        magic_ovl_ench_list_down = Add_Hidden_Field(286, 163, 307, 169, str_empty_string__ovr073[0], ST_UNDEFINED);
    }

}


// WZD o73p04
/*

updates ovl_ench_cnt

*/
void Build_Overland_Enchantment_List(void)
{
    uint8_t * ptr_enchantments;
    int16_t player_idx;
    int16_t itr;  // _SI_
    int16_t itr2;  // _CX_

    ovl_ench_cnt = 0;

    for(player_idx = 0; player_idx < NUM_PLAYERS; player_idx++)
    {

        ptr_enchantments = &_players[player_idx].Globals[0];

        for(itr = 0; itr < NUM_OVERLAND_ENCHANTMENTS; itr++)
        {

            if(ptr_enchantments[itr] > 0)
            {

                for(itr2 = spl_NONE; itr2 < spl_MAX_SPELL_COUNT; itr2++)
                {

                    if(
                        (spell_data_table[itr2].ge_idx == itr)
                        &&
                        (spell_data_table[itr2].type == scc_Global_Enchantment)
                    )
                    {

                        ovl_ench_list_spells[ovl_ench_cnt] = (int16_t)itr2;

                    }
                }

                ovl_ench_list_players[ovl_ench_cnt] = (int8_t)player_idx;  // "enchantments are shown in the banner-color of the casting wizard"

                ovl_ench_cnt++;

            }

        }

    }

}


// WZD o73p05
void Magic_Screen_Load(void)
{
    int16_t itr;
    int16_t itr_gem_count;
    int16_t itr_diplicon;

    int16_t gem_player_idx;
    uint8_t wizard_id;
    uint8_t banner_id;
    int16_t lilwiz_entry_num;

    GUI_String_1 = (char *)Near_Allocate_First(100);
    GUI_String_2 = (char *)Near_Allocate_Next(30);
    ovl_ench_list_spells = (int16_t *)Near_Allocate_Next(192);  // 192 bytes ... ¿ 96 of ?
    ovl_ench_list_players = (int8_t *)Near_Allocate_Next(96);  // 96 bytes ... ¿ 96 of player_idx ?
    magic_ovl_ench_flds = Near_Allocate_Next(40);  // 40 bytes ... ¿ 40 or 20 of ... ?
    magic_gem_fields = Near_Allocate_Next(12);  // 12 bytes ... ¿ 12 or 6 of ... ?

    for(itr = 0; itr < 96; itr++)
    {
        ovl_ench_list_spells[itr] = 0;
        ovl_ench_list_players[itr] = ST_UNDEFINED;
    }

    magic_background_seg = LBX_Reload(magic_lbx_file, 0, _screen_seg);
    magic_alchemy_button_seg = LBX_Reload_Next(magic_lbx_file, 13, _screen_seg);
    magic_ok_button_seg = LBX_Reload_Next(magic_lbx_file, 14, _screen_seg);
    // SAMB_ptr armylist_ok_button_seg;
    for(itr_gem_count = 0; itr_gem_count < gem_count; itr_gem_count++)
    {
        lilwiz_gem_segs[itr_gem_count] = ST_NULL;
        if(gem_player_nums[itr_gem_count] > -1)
        {
            /*
                14 wizards
                6 colors
            */
            // lilwiz_gem_segs[itr_gem_count] = LBX_Reload_Next(lilwiz_lbx_file, (_players[gem_player_idx[itr_gem_count]].wizard_id *_players[gem_player_idx[itr_gem_count]].banner_id), _screen_seg);
            gem_player_idx = gem_player_nums[itr_gem_count];
            wizard_id = _players[gem_player_nums[itr_gem_count]].wizard_id;
            banner_id = _players[gem_player_nums[itr_gem_count]].banner_id;
            lilwiz_entry_num = ((wizard_id * 5) + banner_id);
            lilwiz_gem_segs[itr_gem_count] = LBX_Reload_Next(lilwiz_lbx_file, lilwiz_entry_num, _screen_seg);
        }
    }

    // MAGIC.LBX
    // GEMS     grey gem
    // GEMS     broken grey gem

    grey_gem_seg              = LBX_Reload_Next(magic_lbx_file,  6, _screen_seg);
    broken_grey_gem_seg       = LBX_Reload_Next(magic_lbx_file, 51, _screen_seg);

    // MAGIC.LBX
    // STAVES   mana staff empty
    // STAVES   mana staff full
    // STAVES   researchstaff empt
    // STAVES   researchstaff full
    // STAVES   skill staff empty
    // STAVES   skill staff full
    // STAVES   mana staff locked
    // STAVES   researstaff locked
    // STAVES   skill staff locked

    mana_staff_empty_seg      = LBX_Reload_Next(magic_lbx_file,  7, _screen_seg);
    mana_staff_full_seg       = LBX_Reload_Next(magic_lbx_file,  8, _screen_seg);
    research_staff_empty_seg  = LBX_Reload_Next(magic_lbx_file,  9, _screen_seg);
    research_staff_full_seg   = LBX_Reload_Next(magic_lbx_file, 10, _screen_seg);
    skill_staff_empty_seg     = LBX_Reload_Next(magic_lbx_file, 11, _screen_seg);
    skill_staff_full_seg      = LBX_Reload_Next(magic_lbx_file, 12, _screen_seg);
    mana_staff_locked_seg     = LBX_Reload_Next(magic_lbx_file, 15, _screen_seg);
    research_staff_locked_seg = LBX_Reload_Next(magic_lbx_file, 16, _screen_seg);
    skill_staff_locked_seg    = LBX_Reload_Next(magic_lbx_file, 17, _screen_seg);

    /*
        DIPLICON
        {blue, green, purple, red, yellow}
        {wizpak, alliance, war}
        15 = (number of wizard colors) * (number of diplomatic relation states)
    */
    for(itr_diplicon = 0; itr_diplicon < 15; itr_diplicon++)
    {
        magic_dipl_icon_segs[itr_diplicon] = LBX_Reload_Next(magic_lbx_file, (60 + itr_diplicon), _screen_seg);
    }

}


// WZD o73p06
/*
; displays and processes the alchemy conversion window,
; overlaid into the current screen as background (any
; sandbox allocations need to be redone afterwards)

Item_Screen()
    if(input_field_idx == m_item_screen_alchemy_button_field)
        Alchemy_Popup((_item_window_start_x + 63), (_item_window_start_y + 58));

*/
void Alchemy_Popup(int16_t start_x, int16_t y_start)
{
    int16_t hotkey_ESC;
    int16_t input_field_idx;  // _SI_
    int16_t leave_screen;  // _DI_

    m_alchemy_popup_start_x = start_x;
    m_alchemy_popup_start_y = y_start;

    m_alchemy_arrowbar_cycle = 0;
    m_alchemy_amount = 0;
    m_alchemy_arrowbar_pos = 3;

    Deactivate_Help_List();

    Assign_Auto_Function(Alchemy_Popup_Draw, 1);

    Set_Alchemy_Screen_Help_List();

    m_alchemy_bar_bitm_seg = Allocate_First_Block(_screen_seg, 40);  // 40 PR, 640 B

    // MAGIC.LBX, 052  ALCHEMY    alchemy background
    // MAGIC.LBX, 053  ALCHBUTN   alchemy cancel
    // MAGIC.LBX, 054  ALCHBUTN   alchemy ok
    // MAGIC.LBX, 055  ALCHBUTN   alchemy right arro
    // MAGIC.LBX, 056  ALCHBUTN   alchemy left arrow
    // MAGIC.LBX, 057  ARROWBAR   alchemy lines >>>
    // MAGIC.LBX, 058  ALCHSTAR   alchemy star
    // MAGIC.LBX, 059  ALCHEMY    alchemy pow2gold
    m_alchemy_background_seg  = LBX_Reload_Next(magic_lbx_file, 52, _screen_seg);
    m_alchemy_cancel_button_seg = LBX_Reload_Next(magic_lbx_file, 53, _screen_seg);
    m_alchemy_ok_button_seg   = LBX_Reload_Next(magic_lbx_file, 54, _screen_seg);
    m_alchemy_left_arrow_button_seg = LBX_Reload_Next(magic_lbx_file, 56, _screen_seg);
    m_alchemy_right_arrow_button_seg = LBX_Reload_Next(magic_lbx_file, 55, _screen_seg);
    m_alchemy_bar_pict_seg  = LBX_Reload_Next(magic_lbx_file, 57, _screen_seg);
    m_alchemy_star_seg = LBX_Reload_Next(magic_lbx_file, 58, _screen_seg);
    m_alchemy_pow2gold_seg = LBX_Reload_Next(magic_lbx_file, 59, _screen_seg);

    m_alchemy_divisor = 2;  // 50%

    if(_players[_human_player_idx].alchemy == ST_TRUE)
    {
        m_alchemy_divisor = 1;  // 100%
    }

    Clear_Fields();
    Set_Page_Off();
    Copy_On_To_Off_Page();
    Copy_Off_To_Back();
    PageFlip_FX();
    Set_Input_Delay(1);
    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {
        Mark_Time();
        Clear_Fields();
        Alchemy_Popup_Add_Fields();
        hotkey_ESC = Add_Hot_Key(cnst_HOTKEY_Esc7);
        input_field_idx = Get_Input();

        if(
            (input_field_idx == m_alchemy_cancel_button_field)
            ||
            (input_field_idx == m_alchemy_popup_exit_field)
            ||
            (input_field_idx == hotkey_ESC)
        )
        {
            Play_Left_Click();
            leave_screen = ST_TRUE;
        }

        if(input_field_idx == m_alchemy_ok_button_field)
        {
            Play_Left_Click();
            leave_screen = ST_TRUE;

            if(m_alchemy_conversion_direction == 1)
            {
                _players[HUMAN_PLAYER_IDX].gold_reserve -= m_alchemy_amount;
                Player_Add_Mana(_human_player_idx, (m_alchemy_amount / m_alchemy_divisor));
            }
            else
            {
                Player_Add_Gold(_human_player_idx, (m_alchemy_amount / m_alchemy_divisor));
                _players[HUMAN_PLAYER_IDX].mana_reserve -= m_alchemy_amount;
            }
        }

        if(input_field_idx == m_alchemy_arrow_button_field)
        {
            Play_Left_Click();
            /*
                IDGI: two's compliment trickery?
                    mov     ax, [m_alchemy_conversion_direction]
                    neg     ax
                    sbb     ax, ax
                    inc     ax
                    mov     [m_alchemy_conversion_direction], ax
            */
            m_alchemy_conversion_direction = (1 - m_alchemy_conversion_direction);
        }


        if(leave_screen == ST_FALSE)
        {
            Copy_Back_To_Off();
            Alchemy_Popup_Draw();
            PageFlip_FX();
            Release_Time(2);
        }

    }

    // @@Leave_Screen:
    Clear_Fields();
    Deactivate_Auto_Function();
    Deactivate_Help_List();
}

// WZD o73p07
/*
; draws the alchemy window into the current draw
; segment, including the animated slider bar
*/
void Alchemy_Popup_Draw(void)
{
    int16_t conversion_rate;
    uint8_t colors[6];

    colors[0] = 192;
    colors[1] = 179;

    Set_Page_Off();

    if(m_alchemy_conversion_direction == e_GoldToPower)
    {
        m_alchemy_arrowbar_cycle = ((m_alchemy_arrowbar_cycle + 1) % 8);
    }
    else
    {
        m_alchemy_arrowbar_cycle = ((m_alchemy_arrowbar_cycle + 7) % 8);
    }

    FLIC_Draw((m_alchemy_popup_start_x - 5), m_alchemy_popup_start_y, m_alchemy_background_seg);

    Set_Font_Colors_15(1, &colors[0]);

    Set_Outline_Color(0);

    Set_Font_Style_Shadow_Down(3, 15, 0, 0);

    if(m_alchemy_conversion_direction == e_GoldToPower)
    {
        if(_players[HUMAN_PLAYER_IDX].gold_reserve <= 1)
        {
            Set_Font_Style_Shadow_Down(4, 2, 0, 0);
            m_alchemy_arrowbar_pos = 3;
        }
    }
    else
    {
        FLIC_Draw((m_alchemy_popup_start_x + 7), (m_alchemy_popup_start_y + 10), m_alchemy_pow2gold_seg);

        if(_players[HUMAN_PLAYER_IDX].mana_reserve <= 1)
        {
            Set_Font_Style_Shadow_Down(4, 2, 0, 0);
            m_alchemy_arrowbar_pos = 3;
        }
    }

    Set_Font_Style(1, 15, 0, 0);

    if(m_alchemy_conversion_direction == e_GoldToPower)
    {
        conversion_rate = 50;
        if(((50 * _players[HUMAN_PLAYER_IDX].gold_reserve) / conversion_rate) > 999)
        {
            conversion_rate = ((50 * _players[HUMAN_PLAYER_IDX].gold_reserve) / 999);
        }
        m_alchemy_amount = ((_players[HUMAN_PLAYER_IDX].gold_reserve * (m_alchemy_arrowbar_pos - 3)) / conversion_rate);
        SETMAX(m_alchemy_amount,999);
        Print_Integer_Right((m_alchemy_popup_start_x + 28), (m_alchemy_popup_start_y + 26), m_alchemy_amount);
        Print_Right((m_alchemy_popup_start_x + 40), (m_alchemy_popup_start_y + 26), cnst_GP_3);
        Print_Integer_Right((m_alchemy_popup_start_x + 130), (m_alchemy_popup_start_y + 26), (m_alchemy_amount / m_alchemy_divisor));
    }
    else
    {
        conversion_rate = 50;
        if(((50 * _players[HUMAN_PLAYER_IDX].mana_reserve) / conversion_rate) > 999)
        {
            conversion_rate = ((50 * _players[HUMAN_PLAYER_IDX].mana_reserve) / 999);
        }
        m_alchemy_amount = ((_players[HUMAN_PLAYER_IDX].mana_reserve * (m_alchemy_arrowbar_pos - 3)) / conversion_rate);
        SETMAX(m_alchemy_amount,999);
        Print_Integer_Right((m_alchemy_popup_start_x + 28), (m_alchemy_popup_start_y + 26), (m_alchemy_amount / m_alchemy_divisor));
        Print_Right((m_alchemy_popup_start_x + 40), (m_alchemy_popup_start_y + 26), cnst_GP_3);
        Print_Integer_Right((m_alchemy_popup_start_x + 130), (m_alchemy_popup_start_y + 26), m_alchemy_amount);
    }

        // ; BUG: should be MP
        Print_Right((m_alchemy_popup_start_x + 142), (m_alchemy_popup_start_y + 26), cnst_PP);

        Draw_Picture_To_Bitmap(m_alchemy_bar_pict_seg, m_alchemy_bar_bitm_seg);

    if(m_alchemy_conversion_direction == e_PowerToGold)
    {
        Flip_Bitmap(m_alchemy_bar_bitm_seg);
    }

    Set_Window((m_alchemy_popup_start_x + 50), 0, (m_alchemy_popup_start_x + 50 + m_alchemy_arrowbar_pos), SCREEN_YMAX);

    Draw_Picture_Windowed((m_alchemy_popup_start_x + 50 + m_alchemy_arrowbar_cycle), (m_alchemy_popup_start_y + 25), m_alchemy_bar_bitm_seg);

    Draw_Picture_Windowed((m_alchemy_popup_start_x + 2 + m_alchemy_arrowbar_cycle), (m_alchemy_popup_start_y + 25), m_alchemy_bar_bitm_seg);

    Reset_Window();

    FLIC_Draw((m_alchemy_popup_start_x + 47 + m_alchemy_arrowbar_pos), (m_alchemy_popup_start_y + 25), m_alchemy_star_seg);

}

// WZD o73p08
/*
; creates the GUI controls for the alchemy window,
; including the slider
*/
void Alchemy_Popup_Add_Fields(void)
{

    Add_Scroll_Field((m_alchemy_popup_start_x + 50), (m_alchemy_popup_start_y + 25), 0, 55, 3, 53, 55, 7, &m_alchemy_arrowbar_pos, cnst_HOTKEY_A_2[0], ST_UNDEFINED);

    m_alchemy_cancel_button_field = Add_Button_Field((m_alchemy_popup_start_x + 13), (m_alchemy_popup_start_y + 39), str_empty_string__ovr073, m_alchemy_cancel_button_seg, cnst_HOTKEY_C_5, ST_UNDEFINED);

    m_alchemy_ok_button_field = Add_Button_Field((m_alchemy_popup_start_x + 96), (m_alchemy_popup_start_y + 39), str_empty_string__ovr073, m_alchemy_ok_button_seg, cnst_HOTKEY_O_7, ST_UNDEFINED);

    if(m_alchemy_conversion_direction == e_GoldToPower)
    {
        m_alchemy_arrow_button_field = Add_Button_Field((m_alchemy_popup_start_x + 66), (m_alchemy_popup_start_y + 39), str_empty_string__ovr073, m_alchemy_left_arrow_button_seg, cnst_HOTKEY_SPACE[0], ST_UNDEFINED);
    }
    else
    {
        m_alchemy_arrow_button_field = Add_Button_Field((m_alchemy_popup_start_x + 66), (m_alchemy_popup_start_y + 39), str_empty_string__ovr073, m_alchemy_right_arrow_button_seg, cnst_HOTKEY_SPACE[0], ST_UNDEFINED);
    }

    // ¿ BUG: never checked for input match ?
    m_alchemy_popup_window_field = Add_Hidden_Field(m_alchemy_popup_start_x, m_alchemy_popup_start_y, (m_alchemy_popup_start_x + 157), (m_alchemy_popup_start_y + 65), str_empty_string__ovr073[0], ST_UNDEFINED);

    // ¿ ~== _global_exit_field ?
    m_alchemy_popup_exit_field = Add_Hidden_Field(0, 0, SCREEN_XMAX, SCREEN_YMAX, str_empty_string__ovr073[0], ST_UNDEFINED);

}




/*
    WIZARDS.EXE  ovr074
*/

// WZD o74p01
// Mirror_Screen()

// WZD o74p02
// Mirror_Screen_Draw()

// WZD o74p03
// Mirror_Screen_Load()
