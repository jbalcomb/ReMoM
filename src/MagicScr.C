
/*
    Magic Screen

    WIZARDS.EXE
        ovr073
    
    MoO2:
        Module: ¿  ?
*/

#include "MoX.H"
#include "MagicScr.H"





// WZD dseg:347E                            ¿ BEGIN: DIPLOMAC || Magic Screen - Initialized Data ?

// WZD dseg:347E
// WZD dseg:347E 01 00                                           GAME_AlchemyDir dw 1                    ; DATA XREF: GAME_AlchemyWindow+19Dr ...

// WZD dseg:3480 52 56 4C                                        
char aRvl[] = "RVL";

// WZD dseg:3483
// borrowed null-terminator from aRvl
char cnst_ZeroString_23  = '0';

// WZD dseg:3484
char aPwr[] = "PWR";

// ¿ Add_Scroll_Field() for research_stave_pct_pos borrows 'R' as offset [aPwr+2] ?

// WZD dseg:3488
char aDestin[] = "DESTIN";

// WZD dseg:348F
char cnst_HOTKEY_X_5 = 'X';

// WZD dseg:3491 1B 00                                           
char cnst_HOTKEY_Esc7 = '\x1B';


// WZD dseg:3493 57 72 69 74 65 20 69 74 20 75 70 2C 20 67 69 76+aWriteItUpGiveMeASav db 'Write it up, give me a save game.',0
// WZD dseg:3493 65 20 6D 65 20 61 20 73 61 76 65 20 67 61 6D 65+                                        ; DATA XREF: Magic_Screen+21Ao
// WZD dseg:34B5 44 6F 20 79 6F 75 20 77 69 73 68 20 74 6F 20 63+aDoYouWishToCancelYo db 'Do you wish to cancel your '
// WZD dseg:34B5 61 6E 63 65 6C 20 79 6F 75 72 20                                                        ; DATA XREF: Magic_Screen+2CFo
// WZD dseg:34D0 02                                              db    2
// WZD dseg:34D1 00                                              db    0
// WZD dseg:34D2 01                                              unk_39F72 db    1                       ; DATA XREF: Magic_Screen+31Co
// WZD dseg:34D3 20 73 70 65 6C 6C 2E 00                         cnst_SpaceSpellDot_2 db ' spell.',0     ; should use dseg:33a8
// WZD dseg:34DB 59 6F 75 20 6D 61 79 20 6E 6F 74 20 63 6F 6E 74+aYouMayNotContactOth db 'You may not contact other wizards while you are banished.',0

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
char cnst_HOTKEY_A_2 = 'A';

// WZD dseg:358E
char magic_lbx_file[] = "MAGIC";

// WZD dseg:3592
char cnst_HOTKEY_C_5 = 'C';

// WZD dseg:3594
char lilwiz_lbx_file[] = "LILWIZ";

// WZD dseg:359B 47 50 00                                        cnst_GP_3 db 'GP',0                     ; DATA XREF: GAME_DrawAlchWindow+1A2o ...
// WZD dseg:359B                                                                                         ; should use dseg:31a2
// WZD dseg:359E 50 50 00                                        cnst_PP db 'PP',0                       ; DATA XREF: GAME_DrawAlchWindow+2A5o
// WZD dseg:359E                                                                                         ; BUG: should be MP
// WZD dseg:35A1 20 00                                           cnst_HOTKEY_SPACE db ' ',0              ; DATA XREF: GAME_AlchemyControls+99o ...
// WZD dseg:35A1                                                                                         ; (could use dseg:2ad9)
// WZD dseg:35A3 00                                              db    0
// WZD dseg:35A4 C4 35                                           diplo_state@ dw offset aHate            ; DATA XREF: IDK_WizTgtSpl_sBFC85+DEr
// WZD dseg:35A4                                                                                         ; "Hate"
// WZD dseg:35A6 C9 35                                           dw offset aTroubled                     ; "Troubled"
// WZD dseg:35A8 D2 35                                           dw offset aTense                        ; "Tense"
// WZD dseg:35AA D8 35                                           dw offset aRestless                     ; "Restless"
// WZD dseg:35AC E1 35                                           dw offset aUnease                       ; "Unease"
// WZD dseg:35AE E8 35                                           dw offset aNeutral                      ; "Neutral"
// WZD dseg:35B0 F0 35                                           dw offset aRelaxed                      ; "Relaxed"
// WZD dseg:35B2 F8 35                                           dw offset aCalm                         ; "Calm"
// WZD dseg:35B4 FD 35                                           dw offset aPeaceful_0                   ; "Peaceful"
// WZD dseg:35B6 06 36                                           off_3A056 dw offset aFriendly           ; "Friendly"
// WZD dseg:35B8 0F 36                                           dw offset aHarmony                      ; "Harmony"
// WZD dseg:35BA 17 36                                           off_3A05A dw offset aNoTreaty           ; "No Treaty"
// WZD dseg:35BC 21 36                                           dw offset aWizardPact_1                 ; "Wizard Pact"
// WZD dseg:35BE 2D 36                                           dw offset aAlliance_1                   ; "Alliance"
// WZD dseg:35C0 36 36                                           off_3A060 dw offset aWar                ; "War"
// WZD dseg:35C2 3A 36                                           dw offset aFinalWar                     ; "Final War"
// WZD dseg:35C4 48 61 74 65                                     aHate db 'Hate'                         ; DATA XREF: dseg:diplo_state@o
// WZD dseg:35C8 00                                              mirror_screen_strings_null_terminator db 0
// WZD dseg:35C8                                                                                         ; DATA XREF: Mirror_Screen_Draw+55Eo ...
// WZD dseg:35C9 54 72 6F 75 62 6C 65 64 00                      aTroubled db 'Troubled',0               ; DATA XREF: dseg:35A6o
// WZD dseg:35D2 54 65 6E 73 65 00                               aTense db 'Tense',0                     ; DATA XREF: dseg:35A8o
// WZD dseg:35D8 52 65 73 74 6C 65 73 73 00                      aRestless db 'Restless',0               ; DATA XREF: dseg:35AAo
// WZD dseg:35E1 55 6E 65 61 73 65 00                            aUnease db 'Unease',0                   ; DATA XREF: dseg:35ACo
// WZD dseg:35E8 4E 65 75 74 72 61 6C 00                         aNeutral db 'Neutral',0                 ; DATA XREF: dseg:35AEo
// WZD dseg:35F0 52 65 6C 61 78 65 64 00                         aRelaxed db 'Relaxed',0                 ; DATA XREF: dseg:35B0o
// WZD dseg:35F8 43 61 6C 6D 00                                  aCalm db 'Calm',0                       ; DATA XREF: dseg:35B2o
// WZD dseg:35FD 50 65 61 63 65 66 75 6C 00                      aPeaceful_0 db 'Peaceful',0             ; DATA XREF: dseg:35B4o
// WZD dseg:3606 46 72 69 65 6E 64 6C 79 00                      aFriendly db 'Friendly',0               ; DATA XREF: dseg:off_3A056o
// WZD dseg:360F 48 61 72 6D 6F 6E 79 00                         aHarmony db 'Harmony',0                 ; DATA XREF: dseg:35B8o
// WZD dseg:3617 4E 6F 20 54 72 65 61 74 79 00                   aNoTreaty db 'No Treaty',0              ; DATA XREF: dseg:off_3A05Ao
// WZD dseg:3621 57 69 7A 61 72 64 20 50 61 63 74 00             aWizardPact_1 db 'Wizard Pact',0        ; DATA XREF: dseg:35BCo
// WZD dseg:362D 41 6C 6C 69 61 6E 63 65 00                      aAlliance_1 db 'Alliance',0             ; DATA XREF: dseg:35BEo
// WZD dseg:3636 57 61 72 00                                     aWar db 'War',0                         ; DATA XREF: dseg:off_3A060o
// WZD dseg:363A 46 69 6E 61 6C 20 57 61 72 00                   aFinalWar db 'Final War',0              ; DATA XREF: dseg:35C2o
// WZD dseg:3644 1B 00                                           cnst_HOTKEY_Esc8 db 1Bh,0               ; should use dseg:2c56
// WZD dseg:3646 20 00                                           cnst_HOTKEY_SPACE_2 db ' ',0            ; DATA XREF: Mirror_Screen_Draw+8o
// WZD dseg:3646                                                                                         ; should use dseg:35a1
// WZD dseg:3648 52 65 6C 61 74 69 6F 6E 73 3A 00                aRelations db 'Relations:',0            ; DATA XREF: Mirror_Screen_Draw+17Fo
// WZD dseg:3653 54 72 65 61 74 69 65 73 3A 00                   aTreaties db 'Treaties:',0              ; DATA XREF: Mirror_Screen_Draw+1F5o
// WZD dseg:365D 50 65 72 73 6F 6E 61 6C 69 74 79 3A 00          aPersonality db 'Personality:',0        ; DATA XREF: Mirror_Screen_Draw+2B3o
// WZD dseg:366A 4F 62 6A 65 63 74 69 76 65 3A 00                aObjective db 'Objective:',0
// WZD dseg:3675 47 50 00                                        cnst_GP_4 db 'GP',0                     ; DATA XREF: Mirror_Screen_Draw+444o
// WZD dseg:3675                                                                                         ; should use dseg:31a2
// WZD dseg:3678                                                 ; char cnst_MP_3[]
// WZD dseg:3678 4D 50 00                                        cnst_MP_3 db 'MP',0                     ; DATA XREF: Mirror_Screen_Draw+45Eo
// WZD dseg:3678                                                                                         ; should use dseg:31a5
// WZD dseg:367B                                                 ; char cnst_Fame[]
// WZD dseg:367B 46 61 6D 65 00                                  cnst_Fame db 'Fame',0                   ; DATA XREF: Mirror_Screen_Draw+520o
// WZD dseg:3680 61 6E 64 00                                     mirror_screen_abilities_list_and db 'and',0
// WZD dseg:3680                                                                                         ; DATA XREF: Mirror_Screen_Draw+64Eo
// WZD dseg:3684 2C 00                                           mirror_screen_abilities_list_comma db ',',0
// WZD dseg:3684                                                                                         ; DATA XREF: Mirror_Screen_Draw:loc_6320Fo
// WZD dseg:3686                                                 ; char mirror_screen_abilities_list_period[]
// WZD dseg:3686 2E 00                                           mirror_screen_abilities_list_period db '.',0
// WZD dseg:3686                                                                                         ; DATA XREF: Mirror_Screen_Draw:loc_632EFo
// WZD dseg:3686                                                                                         ; should use dseg:2b31
// WZD dseg:3688 48 65 72 6F 65 73 00                            aHeroes db 'Heroes',0                   ; DATA XREF: Mirror_Screen_Draw+759o
// WZD dseg:368F 54 68 65 00                                     cnst_The_2 db 'The',0                   ; DATA XREF: Mirror_Screen_Draw+801o
// WZD dseg:368F                                                                                         ; should use dseg:342a
// WZD dseg:3693 4C 49 4C 57 49 5A 00                            mirror_lilwiz_lbx_file db 'LILWIZ',0    ; DATA XREF: IDK_MirrorScreen_s6343B+5Bo
// WZD dseg:3693                                                                                         ; should use dseg:3594
// WZD dseg:369A 4D 41 47 49 43 00                               mirror_magic_lbx_file db 'MAGIC',0      ; DATA XREF: IDK_MirrorScreen_s6343B+78o ...
// WZD dseg:369A                                                                                         ; should use dseg:358e

// WZD dseg:369A                            ¿ END: DIPLOMAC || Magic Screen - Initialized Data ?





// WZD dseg:958E
// AKA profile_gem;
SAMB_ptr lilwiz_gem_segs[5];

/*
    ¿ 28 DB, 14 DW, 7 DD ? ¿ gem wiz picts - player 0 always human, always index 0 ?
    ~ Wizard Picture
    
*/
// WZD dseg:958E 00                                              profile_gem_00 db    0                  ; DATA XREF: Magic_Screen_Load_Pictures+BFw ...
// WZD dseg:958F 00                                              db    0
// WZD dseg:9590 00                                              db    0
// WZD dseg:9591 00                                              db    0
// WZD dseg:9592 00                                              db    0
// WZD dseg:9593 00                                              db    0
// WZD dseg:9594 00                                              db    0
// WZD dseg:9595 00                                              db    0
// WZD dseg:9596 00                                              db    0
// WZD dseg:9597 00                                              db    0
// WZD dseg:9598 00                                              db    0
// WZD dseg:9599 00                                              db    0
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





// WZD dseg:C1EA                                                 ¿ BEGIN: DIPLOMAC || Magic Screen - Uninitialized Data ?

// WZD dseg:C1EA
SAMB_ptr magic_dipl_icon_segs[15];

// WZD dseg:C208 00 00                                           GAME_AlchemyDivisor dw 0                ; DATA XREF: GAME_AlchemyWindow+10Ew ...

// WZD dseg:C20A 00 00                                           human_player_summoning_circle_city_idx dw 0

// WZD dseg:C20C
int16_t gem_player_nums[4];

// WZD dseg:C214
// AKA IDK_CnctWzdsCnt_w42CB4
int16_t gem_count;

// WZD dseg:C214                                                 ¿ END: Magic Screen - ?

// WZD dseg:C216                                                 ¿ BEGIN: Alchemy Screen - ?

// WZD dseg:C216 00 00                                           GAME_AlchFullScrLbl dw 0                ; DATA XREF: GAME_AlchemyWindow+17Cr ...
// WZD dseg:C218 00 00                                           GAME_AlchWndLabel dw 0                  ; DATA XREF: GAME_AlchemyControls+F2w
// WZD dseg:C21A 00 00                                           GAME_AlchSwitchLbl dw 0                 ; DATA XREF: GAME_AlchemyWindow:loc_625B9r ...
// WZD dseg:C21C 00 00                                           GAME_AlchemyOK dw 0                     ; DATA XREF: GAME_AlchemyWindow:loc_62566r ...
// WZD dseg:C21E 00 00                                           GAME_AlchemyCancel dw 0                 ; DATA XREF: GAME_AlchemyWindow+176r ...
// WZD dseg:C220 00 00                                           GAME_AlchAnimState dw 0                 ; DATA XREF: GAME_AlchemyWindow+14w ...
// WZD dseg:C220                                                                                         ; now 0
// WZD dseg:C222 00 00                                           GAME_AlchSliderState dw 0               ; DATA XREF: GAME_AlchemyWindow+20w ...
// WZD dseg:C222                                                                                         ; now 3
// WZD dseg:C224 00 00                                           GAME_AlchConvValue dw 0                 ; DATA XREF: GAME_AlchemyWindow+1Aw ...
// WZD dseg:C224                                                                                         ; now 0
// WZD dseg:C226 00 00                                           GAME_AlchemyWndTop dw 0                 ; DATA XREF: GAME_AlchemyWindow+11w ...
// WZD dseg:C228 00 00                                           GAME_AlchemyWndLeft dw 0                ; DATA XREF: GAME_AlchemyWindow+Bw ...
// WZD dseg:C22A 00 00                                           IMG_Alchemy_Bar@ dw 0                   ; DATA XREF: GAME_AlchemyWindow+53w ...

// WZD dseg:C22A                                                 ¿ END: Alchemy Screen - ?

// WZD dseg:C22C                                                 ¿ BEGIN: Magic Screen - ?

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
SAMB_ptr G_SPL_SomeNearArray;

// WZD dseg:C23C
SAMB_ptr G_SPL_GlobalArray;

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

// WZD dseg:C252                                                 ¿ END: Magic Screen - Uninitialized Data ?

// WZD dseg:C254 00 00                                           mirror_screen_player_idx dw 0           ; DATA XREF: Mirror_Screen:loc_62A9Aw ...
// WZD dseg:C256 00 00                                           word_42CF6 dw 0                         ; DATA XREF: Mirror_Screen_Draw+4A2r ...
// WZD dseg:C258 00 00                                           mirror_start_y dw 0                     ; DATA XREF: Mirror_Screen:loc_62AE9w ...
// WZD dseg:C25A 00 00                                           mirror_start_x dw 0                     ; DATA XREF: Mirror_Screen:loc_62AE3w ...
// WZD dseg:C25C 00 00                                           word_42CFC dw 0                         ; DATA XREF: Mirror_Screen_Draw+478r ...

// WZD dseg:C25C                                                 ¿ END: DIPLOMAC || Magic Screen - Uninitialized Data ?





// WZD o73p01
void Magic_Screen(void)
{
    int16_t multihotkey_PWR;
    int16_t multihotkey_RVL;
    int16_t multihotkey_DESTIN;
    int16_t hotkey_ESC;
// var_10= word ptr -10h
// var_E= word ptr -0Eh
// var_C= word ptr -0Ch
// var_A= word ptr -0Ah
    int16_t hotkey_X;
    int16_t screen_changed;
// var_4= word ptr -4
    int16_t leave_screen_flag;

    int16_t itr_help;
    int16_t itr_players;
    int16_t input_field_idx;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Magic_Screen()\n", __FILE__, __LINE__);
#endif

    Deactivate_Help_List();
    Set_Magic_Screen_Help_List();

    screen_changed = ST_FALSE;

    for(itr_help = 0; itr_help < 4; itr_help++)
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

        if(_FORTRESSES[itr_players].Active == ST_TRUE)
        {
            gem_player_nums[gem_count] = -1;
            // TODO  _help_entries[(gem_count * 10)] = HLP_GRAY_GEM;  // HLP_GRAY_GEM = 262
        }

        if(_FORTRESSES[itr_players].Active == ST_TRUE)
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

    Magic_Screen_Load_Pictures();


    // ¿ cbw; cwd; sub ax,dx; sar ax,1; ?
    // Convert Byte to Word; Convert Word to Doubleword; AX = AX - DX; AX = AX / 2;
    // sign-extending and subtracted the high-byte from the low-byte would amount to subtracting the sign-bit ?
    skill_stave_pct_pos = _players[0].Skill_Pcnt / 2;  
    research_stave_pct_pos = _players[0].Research_Pcnt / 2;
    mana_stave_pct_pos = 50 - skill_stave_pct_pos - research_stave_pct_pos;
    skill_stave_pct = skill_stave_pct_pos;
    research_stave_pct = research_stave_pct_pos;
    mana_stave_pct = mana_stave_pct_pos;

    // TODO WIZ_SetPowerBases();

    // TODO  Deactivate_Auto_Function();
    // TODO  Assign_Auto_Function(Magic_Screen_Draw(), 1);

    magic_ovl_ench_list_first_item = 0;
    // TODO  IDK_MagicScrn_OvlEnch_s620F5();
    magic_ovl_ench_list_scroll_flag = 0;
    if(ovl_ench_cnt > 18)
    {
        magic_ovl_ench_list_scroll_flag = ST_TRUE;
    }

    // TODO  human_player_summoning_circle_city_idx = WIZ_GetSummonTo(_human_player_idx);

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

        // if(input_field_idx == field_idx_RVL)
        // PWR
        // DESTIN
        // mana lock
        // skill lock
        // research lock
        
        /*
            OK button || ESC hotkey
        */
       if( (input_field_idx == button_magic_ok) || (input_field_idx == hotkey_ESC) )
       {
            // TODO  SND_LeftClickSound();
            leave_screen_flag = ST_UNDEFINED;
       }

        // ...
        // ...
        // ...



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
    // TODO  Deactivate_Auto_Function();
    Deactivate_Help_List();
    current_screen = scr_Main_Screen;
    if(leave_screen_flag == 2)
    {
        current_screen = scr_Diplomacy_Screen;
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Magic_Screen()\n", __FILE__, __LINE__);
#endif

}


// WZD o73p02
void Magic_Screen_Draw(void)
{
    int16_t y_start;
    int16_t x_start;
    int16_t diplomatic_treaties;
    int16_t itr_players_help;
    uint8_t colors1[6];
    uint8_t colors2[2];
// var_10= byte ptr -10h
// var_F= byte ptr -0Fh
// var_E= byte ptr -0Eh
// var_D= byte ptr -0Dh
    int16_t diplomatic_relations_idx;
    int16_t itr_treaties;
    int16_t research;
    int16_t skill;
    int16_t mana;
// var_2= word ptr -2

    int16_t itr_help_entries;
    int16_t itr_num_players;
    int16_t itr_gems;
    int16_t itr_ovl_enchs;

    int16_t gem_player_idx;
    int8_t Dipl__Dipl_Status;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Magic_Screen_Draw()\n", __FILE__, __LINE__);
#endif

    colors1[0] = 172;
    colors1[1] = 216;
    colors1[2] = 123;
    colors1[3] = 201;
    colors1[4] = 211;

    for(itr_help_entries = 0; itr_help_entries < 4; itr_help_entries++)
    {
        *(_help_entries +   0 + (itr_help_entries * 10)) = HLP_GRAY_GEM;
        *(_help_entries + 100 + (itr_help_entries * 10)) = ST_UNDEFINED;
        *(_help_entries + 430 + (itr_help_entries * 10)) = ST_UNDEFINED;
    }

    for(itr_players_help = 0, itr_num_players = 1; itr_num_players < _num_players; itr_players_help++, itr_num_players++)
    {
        *(_help_entries +   0 + (itr_players_help * 10)) = HLP_SHATTERED_GEM;
        *(_help_entries + 100 + (itr_players_help * 10)) = ST_UNDEFINED;
        if(_FORTRESSES[itr_num_players].Active == ST_TRUE)
        {
            *(_help_entries +   0 + (itr_players_help * 10)) = HLP_GRAY_GEM;
        }
        if( (_FORTRESSES[itr_num_players].Active == ST_TRUE) && (_players[_human_player_idx].Dipl.Contacted[itr_num_players] == ST_TRUE) )
        {
            *(_help_entries +   0 + (itr_players_help * 10)) = ST_UNDEFINED;
            *(_help_entries + 100 + (itr_players_help * 10)) = HLP_RELATIONS;
        }
    }

    Set_Page_Off();
    FLIC_Draw(0, 0, magic_background_seg);
    FLIC_Set_CurrentFrame(magic_ok_button_seg, 0);
    FLIC_Draw(291, 181, magic_ok_button_seg);
    FLIC_Set_CurrentFrame(magic_alchemy_button_seg, 0);
    FLIC_Draw(232, 181, magic_alchemy_button_seg);


    /*
        BEGIN: Draw Staves
    */
    // if( (skill_stave_pct != skill_stave_pct_pos) && (skill_staff_locked != ST_FALSE) )
    // {
    //     skill_stave_pct_pos = skill_stave_pct;
    // }

    // ...
    // ...
    // ...


    FLIC_Draw(27, 81, mana_staff_empty_seg);            // DEMO
    // FLIC_Draw(27, 81, mana_staff_full_seg);          // DEMO
    // FLIC_Draw(27, 81, mana_staff_locked_seg);        // DEMO
    FLIC_Draw(74, 81, research_staff_empty_seg);        // DEMO
    // FLIC_Draw(74, 81, research_staff_full_seg);      // DEMO
    // FLIC_Draw(74, 81, research_staff_locked_seg);    // DEMO
    FLIC_Draw(121, 81, skill_staff_empty_seg);          // DEMO
    // FLIC_Draw(121, 81, skill_staff_full_seg);        // DEMO
    // FLIC_Draw(121, 81, skill_staff_locked_seg);      // DEMO

    /*
        END: Draw Staves
    */



    Reset_Window();


    // Ummm... Why is the screen draw function update the player data?
    _players[0].Mana_Pnct = mana_stave_pct_pos;
    _players[0].Research_Pcnt = research_stave_pct_pos;
    _players[0].Skill_Pcnt = skill_stave_pct_pos;
    
    Player_Magic_Power_Income_Total(&mana, &research, &skill, 0);


    Set_Font_Style1(2, 3, 0, 0);
    Set_Outline_Color(19);
    Set_Alias_Color(185);

    itoa(mana, GUI_String_1, 10);
    strcat(GUI_String_1, cnst_Space_MP);
    Print_Right(54, 160, GUI_String_1);

    if(_players[0].Researching == 0)
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


    Set_Font_Style1(1, 3, 0, 0);
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
    if(_players[0].mana_reserve > 19999)
    {
        Set_Font_Style1(0, 3, 0, 0);
        Set_Outline_Color(240);
        Set_Alias_Color(228);
        Set_Font_Spacing_Width(1);
    }
    Print_Integer_Right(90, 185, _players[0].mana_reserve);


    Set_Font_Style1(1, 3, 0, 0);
    Set_Outline_Color(240);
    Set_Alias_Color(228);
    Set_Font_Spacing_Width(1);


    /*
        "Power Base:"
    */
    Print(5, 193, aPowerBase);
    Print_Integer_Right(90, 193, _players[0].Power_Base);


    /*
        "Casting:"
    */
    Print(100, 177, aCasting_0);
    // Print_Far(156, 177, spell_data_table[_players[_human_player_idx].Spell_Cast]);
    Print(156, 177, "None");  // DEMO


    /*
        "Researching:"
    */
    Print(100, 185, aResearching);
    // Print_Far(156, 185, spell_data_table[_players[_human_player_idx].Researching]);
    Print(156, 185, "Stasis");  // DEMO


    /*
        "Summon To:"
    */
    Print(100, 193, aSummonTo);
    // TODO  if(human_player_summoning_circle_city_idx == ST_UNDEFINED)
    // TODO  {
    // TODO      Print(156, 193, aNone);
    // TODO  }
    // TODO  else
    // TODO  {
    // TODO      Print_Far(156, 193, _CITIES[human_player_summoning_circle_city_idx].name);
    // TODO  }
    Print(156, 193, "Banbury");  // DEMO


    /*
        BEGIN:
            Gem Portrait Pictures  (LILWIZ)
            Relations
            Treaties  (DIPLICON)
            'Detect Magic' - Casting
    */
    Set_Outline_Color(0);
    x_start = 68;
    for(itr_gems = 0; itr_gems < gem_count; itr_gems++)
    {
        y_start = 2;

        Set_Font(0, 3, 0, 0);
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

            // TODO  ¿ mod or div ? ¿ also, why? ? ¿ 20 is 10 * sizeof() ?
            // TODO  diplomatic_relations_idx = (((_human_player_idx + _players[gem_player_idx[itr_gems]].Dipl.Visible_Rel[itr_gems]) + 100) / 20)
            // TODO  diplomatic_relations_idx = (((_human_player_idx + _players[gem_player_idx[itr_gems]].Dipl.Visible_Rel[itr_gems]) + 100) % 20)
            // TODO  Print_Centered(45 + (77 * itr_gems), 53, diplo_state[diplomatic_relations_idx]);
            Print_Centered(45 + (77 * itr_gems), 53, "Harmony");

            // wizard pact, alliance, war  ...  icons  ...  scroll, peace symbol, crossed swords
            diplomatic_treaties = (_human_player_idx + _players[gem_player_nums[itr_gems]].Dipl.Dipl_Status[itr_gems]);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: diplomatic_treaties: %d\n", __FILE__, __LINE__, diplomatic_treaties);
#endif

            gem_player_idx = gem_player_nums[itr_gems];
            Dipl__Dipl_Status = _players[gem_player_idx].Dipl.Dipl_Status[gem_player_idx];
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: gem_player_idx: %d\n", __FILE__, __LINE__, gem_player_idx);
    dbg_prn("DEBUG: [%s, %d]: Dipl__Dipl_Status: %d\n", __FILE__, __LINE__, Dipl__Dipl_Status);
    dbg_prn("DEBUG: [%s, %d]: Dipl__Dipl_Status: %02X\n", __FILE__, __LINE__, Dipl__Dipl_Status);
#endif
            if(diplomatic_treaties > 0)
            {
                if(diplomatic_treaties > 2)
                {
                    diplomatic_treaties = 3;
                }
                // status + color to index icons
                diplomatic_treaties--;
                FLIC_Draw(x_start, y_start, magic_dipl_icon_segs[diplomatic_treaties + (_players[_human_player_idx].banner_id * 3)]);
                // Meh. FLIC_Draw(x_start, y_start, magic_dipl_icon_segs[((diplomatic_treaties * 3) + _players[_human_player_idx].banner_id)]);
                *(_help_entries + 430 + (itr_gems * 10)) = HLP_TREATIES;
                y_start += 12;

            }

            for(itr_treaties = 0; itr_treaties < itr_gems; itr_treaties++)
            {
                // TOOD  
            }
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

// mov     [word ptr _help_entries+5Ah], HLP_ENCHANTMENTS_Global
// cmp     [ovl_ench_cnt], 0
// _help_entries+5Ah], e_ST_UNDEFINED

    // itr_ovl_enchs;



// TODO     Set_Font(0, 13, 0, 0);
// TODO     Set_Alias_Color(185);
// TODO     if(magic_ovl_ench_list_scroll_flag == ST_TRUE)
// TODO     {
// TODO         Print(286, 163, aMore___);
// TODO     }

    /*
        END: Overland Enchantments
    */



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Magic_Screen_Draw()\n", __FILE__, __LINE__);
#endif
}

// WZD o73p03
void Magic_Screen_Add_Fields(void)
{
    int16_t itr_gem_count;
    int16_t x1;
    int16_t y1;
    int16_t itr_ovl_ench_cnt;
    int16_t itr_ovl_ench_list_cnt;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Magic_Screen_Add_Fields()\n", __FILE__, __LINE__);
#endif

/*
drake178:
; a wrapper for GUI_CreateSlideBar to create a slide
; bar control with a near state pointer
; returns the index of the created control
;
;   Min_S  - minimum slider position
;   Max_S  - maximum slider position
;   Min_V  - minimum represented value
;   Max_V  - maximum represented value
;   Pos@   - near pointer to the position state
; the orientation (horizontal/vertical) is defined by
; the larger of Width/Height, slider bars need to be
; drawn using a graphic redraw function, the GUI only
; controls their input state
*/

    // TODO  Add_Scroll_Field( 32, 102, 0, 50, 0, 50, 5, 50, mana_stave_pct_pos,     cnst_HOTKEY_M_3, ST_UNDEFINED);
    // TODO  Add_Scroll_Field( 79, 102, 0, 50, 0, 50, 5, 50, research_stave_pct_pos, (offset aPwr+2), ST_UNDEFINED);
    // TODO  Add_Scroll_Field(126, 102, 0, 50, 0, 50, 5, 50, skill_stave_pct_pos,    cnst_HOTKEY_S_2, ST_UNDEFINED);

    // Meh? e.g., int16_t button_armylist_down_left;
    button_magic_ok       = Add_Button_Field(291, 181, "", magic_ok_button_seg, cnst_HOTKEY_O_7, ST_UNDEFINED);
    button_magic_alchemy  = Add_Button_Field(232, 181, "", magic_alchemy_button_seg, cnst_HOTKEY_A_2, ST_UNDEFINED);

    magic_mana_staff_locked      = Add_Hidden_Field( 28, 82,  41, 101, 0, ST_UNDEFINED);
    magic_research_staff_locked  = Add_Hidden_Field( 75, 82,  88, 101, 0, ST_UNDEFINED);
    magic_skill_staff_locked     = Add_Hidden_Field(122, 82, 135, 101, 0, ST_UNDEFINED);


    for(itr_gem_count = 0; itr_gem_count < gem_count; itr_gem_count++)
    {
        x1 = (29 + (77 * itr_gem_count));
        y1 = 10;
        magic_gem_fields[itr_gem_count] = Add_Hidden_Field(x1, y1, (x1 + 30), (y1 + 32), cnst_ZeroString_23, ST_UNDEFINED);
    }


    ovl_ench_list_cnt = 0;
    x1 = 169;
    y1 = 80;
    if(ovl_ench_cnt < 10)
    {
        ovl_ench_list_cnt = ovl_ench_cnt;
        for(itr_ovl_ench_cnt = 0; itr_ovl_ench_cnt < ovl_ench_cnt; itr_ovl_ench_cnt++)
        {
            magic_ovl_ench_flds[itr_ovl_ench_cnt] = Add_Hidden_Field(x1, (y1 + (10 * itr_ovl_ench_cnt)), (x1 + 120), (y1 + 9 + (10 * itr_ovl_ench_cnt)), cnst_ZeroString_23, ST_UNDEFINED);
        }
    }
    else
    {
        ovl_ench_list_cnt = ovl_ench_cnt - magic_ovl_ench_list_first_item;
        if(ovl_ench_list_cnt > 18)
        {
            ovl_ench_list_cnt = 18;
        }
        for(itr_ovl_ench_list_cnt = 0; itr_ovl_ench_list_cnt < ovl_ench_list_cnt; itr_ovl_ench_list_cnt++)
        {
            magic_ovl_ench_flds[itr_ovl_ench_list_cnt] = Add_Hidden_Field((x1 + (70 * itr_ovl_ench_list_cnt)), (y1 + (9 * (itr_ovl_ench_list_cnt / 2))), (63 + (x1 + (70 * itr_ovl_ench_list_cnt))), (6 + (y1 + (9 * (itr_ovl_ench_list_cnt / 2)))), cnst_ZeroString_23, ST_UNDEFINED);
        }
    }

    magic_ovl_ench_list_down = INVALID_FIELD;
    if(magic_ovl_ench_list_scroll_flag == ST_TRUE)
    {
        magic_ovl_ench_list_down = Add_Hidden_Field(286, 163, 307, 169, cnst_ZeroString_23, ST_UNDEFINED);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Magic_Screen_Add_Fields()\n", __FILE__, __LINE__);
#endif
}

// WZD o73p04

// WZD o73p05
void Magic_Screen_Load_Pictures(void)
{
    int16_t itr;
    int16_t itr_gem_count;
    int16_t itr_diplicon;

    int16_t gem_player_idx;
    uint8_t wizard_id;
    uint8_t banner_id;
    int16_t lilwiz_entry_num;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Magic_Screen_Load_Pictures()\n", __FILE__, __LINE__);
#endif

    GUI_String_1 = (char *)Near_Allocate_First(100);
    GUI_String_2 = (char *)Near_Allocate_Next(30);
    G_SPL_GlobalArray = Near_Allocate_Next(192);  // 192 bytes ... ¿ 96 of ?
    G_SPL_SomeNearArray = Near_Allocate_Next(96);  // 96 bytes ... ¿ 96 of player_idx ?
    magic_ovl_ench_flds = Near_Allocate_Next(40);  // 40 bytes ... ¿ 40 or 20 of ... ?
    magic_gem_fields = Near_Allocate_Next(12);  // 12 bytes ... ¿ 12 or 6 of ... ?

    for(itr = 0; itr < 96; itr++)
    {
        G_SPL_GlobalArray[itr] = 0;
        G_SPL_SomeNearArray[itr] = ST_UNDEFINED;
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
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: gem_player_idx: %d\n", __FILE__, __LINE__, gem_player_idx);
    dbg_prn("DEBUG: [%s, %d]: wizard_id: %d\n", __FILE__, __LINE__, wizard_id);
    dbg_prn("DEBUG: [%s, %d]: banner_id: %d\n", __FILE__, __LINE__, banner_id);
    dbg_prn("DEBUG: [%s, %d]: lilwiz_entry_num: %d\n", __FILE__, __LINE__, lilwiz_entry_num);
#endif

            lilwiz_gem_segs[itr_gem_count] = LBX_Reload_Next(lilwiz_lbx_file, lilwiz_entry_num, _screen_seg);
        }
    }

    // MAGIC.LBX
    // GEMS     grey gem
    // GEMS     broken grey gem
    // STAVES   mana staff empty
    // STAVES   mana staff full
    // STAVES   researchstaff empt
    // STAVES   researchstaff full
    // STAVES   skill staff empty
    // STAVES   skill staff full
    // STAVES   mana staff locked
    // STAVES   researstaff locked
    // STAVES   skill staff locked

    grey_gem_seg = LBX_Reload_Next(magic_lbx_file, 6, _screen_seg);
    broken_grey_gem_seg = LBX_Reload_Next(magic_lbx_file, 51, _screen_seg);
    mana_staff_empty_seg = LBX_Reload_Next(magic_lbx_file, 7, _screen_seg);
    mana_staff_full_seg = LBX_Reload_Next(magic_lbx_file, 8, _screen_seg);
    research_staff_empty_seg = LBX_Reload_Next(magic_lbx_file, 9, _screen_seg);
    research_staff_full_seg = LBX_Reload_Next(magic_lbx_file, 10, _screen_seg);
    skill_staff_empty_seg = LBX_Reload_Next(magic_lbx_file, 11, _screen_seg);
    skill_staff_full_seg = LBX_Reload_Next(magic_lbx_file, 12, _screen_seg);
    mana_staff_locked_seg = LBX_Reload_Next(magic_lbx_file, 15, _screen_seg);
    research_staff_locked_seg = LBX_Reload_Next(magic_lbx_file, 16, _screen_seg);
    skill_staff_locked_seg = LBX_Reload_Next(magic_lbx_file, 17, _screen_seg);

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

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Magic_Screen_Load_Pictures()\n", __FILE__, __LINE__);
#endif
}

// WZD o73p06
// WZD o73p07
// WZD o73p08
