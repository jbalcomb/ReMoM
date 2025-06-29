/*
    Diplomacy

    WIZARDS.EXE
        ovr084          Diplomacy_Screen();
        ovr085
        ovr086
        ovr087
        ovr088

MoO2
    Module: DIPLODEF
    Module: DIP-SCRN    Diplomacy_Screen_(); Draw_Diplomacy_Screen_();
    Module: NPCDIPLO
    Module: DIPLOMAC
*/

#include "Combat.H"
#include "DIPLOMAC.H"

#include "MOX/Fields.H"
#include "MOX/Input.H"
#include "MOX/LBX_Load.H"
#include "MOX/MOX_DAT.H"  /* _players[] */
#include "MOX/MOX_DEF.H"
#include "MOX/MOX_T4.H"
#include "MOX/SOUND.H"

#include "Lair.H"
#include "LOADER.H"
#include "MainScr.H"
#include "MOM_SCR.H"
#include "UNITTYPE.H"

/*
    WIZARDS.EXE  ovr084
*/
// WZD o84p01
void Diplomacy_Screen__WIP(void);
// WZD o84p02
static void Diplomacy_Screen_Draw__WIP(void);
// WZD o84p01
// sub_6ECD9()
// WZD o84p02
// sub_6ED5D()
// WZD o84p03
// sub_6EE03()
// WZD o84p04
// sub_6EFA5()
// WZD o84p05
// sub_6F3E4()
static void sub_6F51A(int16_t IDK1, int16_t IDK2);
// sub_6F51A()
// WZD o84p07
// sub_6F6BB()
// WZD o84p08
static void _sub_6F90E_Draw(void);
// WZD o84p09
// sub_6F982()
// WZD o84p10
static void IDK_Diplomacy_Background_Music__STUB(int16_t IDK);
// WZD o84p11
// IDK_DiplAnim_s6FDA1()
// WZD o84p12
static void Diplomacy_Screen_Load__WIP(void);
// WZD o84p13
static void DIPL_LoadTalkGFX(void);
// WZD o84p14
static void IDK_DiplEyes_s7028D(void);
// WZD o84p15
static void IDK_Dipl_Draw_s702E6(void);
// WZD o84p16
// sub_7038D()
// WZD o84p17
// DIPL_GetInvasionStr()
/*
    WIZARDS.EXE  ovr085
*/
// WZD o85p01
static void IDK_DiplSts_s70570(void);
// WZD o85p02
// DIPL_Gravitation()
// WZD o85p03
// sub_70A1A()
// WZD o85p04
// sub_70AFE()
// WZD o85p05
static int16_t DIPL_LowestInterest(void);
// WZD o85p06
static void Limit_Temporary_Peace_Modifier(void);
// WZD o85p07
// WIZ_DIPL_TeachSpell()
// WZD o85p08
// G_DIPL_PickSides()
// WZD o85p09
// DIPL_SignTreaty()
// WZD o85p10
// DIPL_SignPeaceTreaty()
// WZD o85p11
// DIPL_LeaveMeAlone()
// WZD o85p12
void Change_Relations__WIP(int16_t value, int16_t attacker_idx, int16_t defender_idx, int16_t type, int16_t city_idx, int16_t spell_idx);
// WZD o85p13
void Declare_War(int16_t attacker_idx, int16_t defender_idx);
// WZD o85p14
void Break_Treaties(int16_t attacker_idx, int16_t defender_idx);
// WZD o85p15
// DIPL_TribSpellList()
// WZD o85p16
// sub_71B90()
// WZD o85p17
// sub_72131()
// WZD o85p18
// DIPL_DropCityCurses()
/*
    WIZARDS.EXE  ovr086
*/
// WZD o86p01
// IDK_Dipl_s72690()
// WZD o86p02
// DIPL_AI_To_AI()
// WZD o86p03
// DIPL_sub_72DB6()
// WZD o86p04
// DIPL_sub_72DB6_Draw()
// WZD o86p05
static void IDK_DIPLOMSG_s732D9(int16_t IDK, int16_t player_idx);
/*
    WIZARDS.EXE  ovr087
*/
// WZD o87p01
// DIPL_ContactProgress()
// WZD o87p02
// IDK_Dipl_s7373B()
// WZD o87p03
// G_DIPL_NeedForWar()
// WZD o87p04
// G_DIPL_SuperiorityWar()
// WZD o87p05
// IDK_Dipl_s73F1C()
// WZD o87p06
// IDK_Dipl_s73FBF()
// WZD o87p07
// DIPL_GetOffMyLawn()
// WZD o87p08
void Decrease_Peace_Duration(void);
// WZD o87p09
// IDK_Dipl_s7436F()
/*
    WIZARDS.EXE  ovr088
*/
// WZD o88p0
// sub_74420()
// DIPL_HumanWarOrPeace()
// IDK_Dipl_s74B68()
// sub_74E2F()
// sub_74E38()
// sub_74F4A()





// WZD dseg:4C43                                                 ¿ END: ovr083 - Strings ?

// WZD dseg:4C44
int16_t AI_Dipl_Unset_0;

// WZD dseg:4C46 00 00                                           word_3B6E6 dw 0                         ; DATA XREF: sub_6EFA5:loc_6F018w ...
// WZD dseg:4C48 A6 4C B7 4C CF 4C DF 4C F1 4C B6 4C B6 4C FC 4C+off_3B6E8 dw offset aProposeTreaty, offset aThreatenBreakT, offset aOfferTribute, offset aExchangeSpells, offset aGoodBye, offset cnst_ZeroString_4, offset cnst_ZeroString_4, offset aWizardPact, offset aAlliance, offset aPeaceTreaty, offset aDeclarationOfW, offset aBreakAllianceW ; "[ Propose Treaty"
// WZD dseg:4C48 0A 4D 15 4D 24 4D 4B 4D 70 4D B6 4C 7C 4D 90 4D+dw offset aForgetIt, offset cnst_ZeroString_4, offset aBreakWizardPac, offset aBreakAlliance, offset aThreatenToAtta, offset aForgetIt, offset cnst_ZeroString_4
// WZD dseg:4C6E 00                                              db    0
// WZD dseg:4C6F 00                                              db    0
// WZD dseg:4C70 5B 20 41 67 72 65 65 20 20 20 20 00             LBuff@ db '[ Agree    ',0               ; DATA XREF: sub_6F6BB+1EFo
// WZD dseg:4C7C 5B 20 46 6F 72 67 65 74 20 49 74 00             aForgetIt_2 db '[ Forget It',0
// WZD dseg:4C88 00                                              db    0
// WZD dseg:4C89 00                                              db    0
// WZD dseg:4C8A 00                                              db    0
// WZD dseg:4C8B 00                                              db    0
// WZD dseg:4C8C 00                                              db    0
// WZD dseg:4C8D 00                                              db    0
// WZD dseg:4C8E 00                                              db    0
// WZD dseg:4C8F 00                                              db    0
// WZD dseg:4C90 00                                              db    0
// WZD dseg:4C91 00                                              db    0
// WZD dseg:4C92 00                                              db    0
// WZD dseg:4C93 00                                              db    0
// WZD dseg:4C94 00                                              db    0
// WZD dseg:4C95 00                                              db    0
// WZD dseg:4C96 00                                              db    0
// WZD dseg:4C97 00                                              db    0
// WZD dseg:4C98 00                                              db    0
// WZD dseg:4C99 00                                              db    0
// WZD dseg:4C9A 00                                              db    0
// WZD dseg:4C9B 00                                              db    0
// WZD dseg:4C9C 00                                              db    0
// WZD dseg:4C9D 00                                              db    0
// WZD dseg:4C9E 00                                              db    0
// WZD dseg:4C9F 00                                              db    0
// WZD dseg:4CA0 B6 4D                                           off_3B740 dw offset aHowMayIServeYo     ; DATA XREF: sub_6EE03+A1r
// WZD dseg:4CA0                                                                                         ; "How may I serve you:"
// WZD dseg:4CA2 CB 4D                                           off_3B742 dw offset aYouProposeATre     ; DATA XREF: sub_6EFA5+24r
// WZD dseg:4CA2                                                                                         ; "You propose a treaty: "
// WZD dseg:4CA4 E2 4D                                           off_3B744 dw offset aYourActions        ; DATA XREF: sub_6F982+24r
// WZD dseg:4CA4                                                                                         ; "Your actions: "
// WZD dseg:4CA6 5B 20 50 72 6F 70 6F 73 65 20 54 72 65 61 74 79 aProposeTreaty db '[ Propose Treaty'    ; DATA XREF: dseg:off_3B6E8o

// WZD dseg:4CB6
char str_empty_string__ovr084[] = "";

// WZD dseg:4CB7 5B 20 54 68 72 65 61 74 65 6E 2F 42 72 65 61 6B+aThreatenBreakT db '[ Threaten/Break Treaty',0
// WZD dseg:4CB7 20 54 72 65 61 74 79 00                                                                 ; DATA XREF: dseg:off_3B6E8o
// WZD dseg:4CCF 5B 20 4F 66 66 65 72 20 54 72 69 62 75 74 65 00 aOfferTribute db '[ Offer Tribute',0    ; DATA XREF: dseg:off_3B6E8o
// WZD dseg:4CDF 5B 20 45 78 63 68 61 6E 67 65 20 53 70 65 6C 6C+aExchangeSpells db '[ Exchange Spells',0
// WZD dseg:4CDF 73 00                                                                                   ; DATA XREF: dseg:off_3B6E8o
// WZD dseg:4CF1 5B 20 47 6F 6F 64 20 42 79 65 00                aGoodBye db '[ Good Bye',0              ; DATA XREF: dseg:off_3B6E8o
// WZD dseg:4CFC 5B 20 57 69 7A 61 72 64 20 50 61 63 74 00       aWizardPact db '[ Wizard Pact',0        ; DATA XREF: dseg:off_3B6E8o
// WZD dseg:4D0A 5B 20 41 6C 6C 69 61 6E 63 65 00                aAlliance db '[ Alliance',0             ; DATA XREF: dseg:off_3B6E8o
// WZD dseg:4D15 5B 20 50 65 61 63 65 20 54 72 65 61 74 79 00    aPeaceTreaty db '[ Peace Treaty',0      ; DATA XREF: dseg:off_3B6E8o
// WZD dseg:4D24 5B 20 44 65 63 6C 61 72 61 74 69 6F 6E 20 6F 66+aDeclarationOfW db '[ Declaration of War on Another Wizard',0
// WZD dseg:4D24 20 57 61 72 20 6F 6E 20 41 6E 6F 74 68 65 72 20+                                        ; DATA XREF: dseg:off_3B6E8o
// WZD dseg:4D4B 5B 20 42 72 65 61 6B 20 41 6C 6C 69 61 6E 63 65+aBreakAllianceW db '[ Break Alliance With Another Wizard',0
// WZD dseg:4D4B 20 57 69 74 68 20 41 6E 6F 74 68 65 72 20 57 69+                                        ; DATA XREF: dseg:off_3B6E8o
// WZD dseg:4D70 5B 20 46 6F 72 67 65 74 20 49 74 00             aForgetIt db '[ Forget It',0            ; DATA XREF: dseg:off_3B6E8o
// WZD dseg:4D7C 5B 20 42 72 65 61 6B 20 57 69 7A 61 72 64 20 50+aBreakWizardPac db '[ Break Wizard Pact',0
// WZD dseg:4D7C 61 63 74 00                                                                             ; DATA XREF: dseg:off_3B6E8o
// WZD dseg:4D90 5B 20 42 72 65 61 6B 20 41 6C 6C 69 61 6E 63 65+aBreakAlliance db '[ Break Alliance',0  ; DATA XREF: dseg:off_3B6E8o
// WZD dseg:4DA1 5B 20 54 68 72 65 61 74 65 6E 20 54 6F 20 41 74+aThreatenToAtta db '[ Threaten To Attack',0
// WZD dseg:4DA1 74 61 63 6B 00                                                                          ; DATA XREF: dseg:off_3B6E8o
// WZD dseg:4DB6 48 6F 77 20 6D 61 79 20 49 20 73 65 72 76 65 20+aHowMayIServeYo db 'How may I serve you:',0
// WZD dseg:4DB6 79 6F 75 3A 00                                                                          ; DATA XREF: dseg:off_3B740o
// WZD dseg:4DCB 59 6F 75 20 70 72 6F 70 6F 73 65 20 61 20 74 72+aYouProposeATre db 'You propose a treaty: ',0
// WZD dseg:4DCB 65 61 74 79 3A 20 00                                                                    ; DATA XREF: dseg:off_3B742o
// WZD dseg:4DE2 59 6F 75 72 20 61 63 74 69 6F 6E 73 3A 20 00    aYourActions db 'Your actions: ',0      ; DATA XREF: dseg:off_3B744o
// WZD dseg:4DF1 20 73 70 65 6C 6C 00                            aSpell db ' spell',0
// WZD dseg:4DF8 20 67 6F 6C 64 00                               aGold db ' gold',0
// WZD dseg:4DFE 5B 20 00                                        asc_3B89E db '[ ',0
// WZD dseg:4E01 57 68 6F 20 73 68 6F 75 6C 64 20 49 20 64 65 63+aWhoShouldIDecl db 'Who should I declare war on?',0
// WZD dseg:4E1E 57 68 6F 20 73 68 6F 75 6C 64 20 49 20 62 72 65+aWhoShouldIBrea db 'Who should I break my treaty with?',0
// WZD dseg:4E41 50 65 72 68 61 70 73 20 69 66 20 79 6F 75 20 77+aPerhapsIfYouWe db 'Perhaps if you were to throw in ',0
// WZD dseg:4E62 20 77 65 20 63 6F 75 6C 64 20 64 65 61 6C 2E 00 aWeCouldDeal_ db ' we could deal.',0
// WZD dseg:4E72 49 66 20 79 6F 75 20 61 6C 73 6F 20 6F 66 66 65+aIfYouAlsoOffer db 'If you also offer me ',0
// WZD dseg:4E88 20 49 20 77 6F 75 6C 64 20 61 63 63 65 70 74 2E+aIWouldAccept_ db ' I would accept.',0

// WZD dseg:4E99
char music_lbx_file__ovr084[] = "MUSIC";
// WZD dseg:4E9F
char diplomac_lbx_file__ovr084[] = "DIPLOMAC";
// WZD dseg:4EA8
char moodwiz_lbx_file__ovr084[] = "MOODWIZ";
// WZD dseg:4EB0
char backgrnd_lbx_file__ovr084[] = "BACKGRND";

// WZD dseg:4EB9 00                                              align 2

// WZD dseg:4EB9                                                 END:  ovr084 - Initialized Data



// WZD dseg:4EBA C0 4E                                           off_3B95A dw offset aWhatDoYouOffer     ; DATA XREF: sub_71B90+F5r ...
// WZD dseg:4EBA                                                                                         ; "What do you offer as tribute?"
// WZD dseg:4EBC DE 4E                                           off_3B95C dw offset aWhatTypeOfSpel     ; DATA XREF: sub_72131+1FDr
// WZD dseg:4EBC                                                                                         ; "What type of spell interests you?"
// WZD dseg:4EBE 00 4F                                           off_3B95E dw offset aWhatWillYouTra     ; DATA XREF: sub_72131+31Dr
// WZD dseg:4EBE                                                                                         ; "What will you trade for it?"
// WZD dseg:4EC0 57 68 61 74 20 64 6F 20 79 6F 75 20 6F 66 66 65+aWhatDoYouOffer db 'What do you offer as tribute?',0
// WZD dseg:4EC0 72 20 61 73 20 74 72 69 62 75 74 65 3F 00                                               ; DATA XREF: dseg:off_3B95Ao
// WZD dseg:4EDE 57 68 61 74 20 74 79 70 65 20 6F 66 20 73 70 65+aWhatTypeOfSpel db 'What type of spell interests you?',0
// WZD dseg:4EDE 6C 6C 20 69 6E 74 65 72 65 73 74 73 20 79 6F 75+                                        ; DATA XREF: dseg:off_3B95Co
// WZD dseg:4F00 57 68 61 74 20 77 69 6C 6C 20 79 6F 75 20 74 72+aWhatWillYouTra db 'What will you trade for it?',0
// WZD dseg:4F00 61 64 65 20 66 6F 72 20 69 74 3F 00                                                     ; DATA XREF: dseg:off_3B95Eo
// WZD dseg:4F1C 5B 20 00                                        db '[ ',0
// WZD dseg:4F1F 20 67 6F 6C 64 00                               aGold_0 db ' gold',0
// WZD dseg:4F25 5B 20 53 70 65 6C 6C 73 00                      aSpells db '[ Spells',0
// WZD dseg:4F2E 5B 20 46 6F 72 67 65 74 20 49 74 00             aForgetIt_0 db '[ Forget It',0
// WZD dseg:4F3A 00                                              db    0
// WZD dseg:4F3B 5B 20 00                                        asc_3B9DB db '[ ',0                     ; DATA XREF: sub_72DB6:loc_72DD6o
// WZD dseg:4F3E 41 63 63 65 70 74 00                            aAccept_0 db 'Accept',0                 ; DATA XREF: sub_72DB6:loc_72E05o
// WZD dseg:4F45 52 65 6A 65 63 74 00                            aReject_0 db 'Reject',0
// WZD dseg:4F4C 41 67 72 65 65 00                               aAgree db 'Agree',0                     ; DATA XREF: sub_72DB6:loc_72E32o
// WZD dseg:4F52 46 6F 72 67 65 74 20 49 74 00                   aForgetIt_1 db 'Forget It',0
// WZD dseg:4F5C 49 6E 20 65 78 63 68 61 6E 67 65 20 79 6F 75 20+aInExchangeYouWillRe db 'In exchange you will receive:',0
// WZD dseg:4F7A 57 68 61 74 20 69 66 20 77 65 20 77 65 72 65 20+aWhatIfWeWereToAlsoO db 'What if we were to also offer ',0
// WZD dseg:4F99 50 65 72 68 61 70 73 20 79 6F 75 20 77 6F 75 6C+aPerhapsYouWouldReco db 'Perhaps you would reconsider if we also provided ',0
// WZD dseg:4FCB 20 61 73 20 61 6E 20 69 6E 63 65 6E 74 69 76 65+aAsAnIncentive db ' as an incentive',0
// WZD dseg:4FDC 3F 00                                           cnst_QuestionMark4 db '?',0             ; should use dseg:2adb
// WZD dseg:4FDE
char diplomsg_lbx_file__ovr86[] = "DIPLOMSG";
// WZD dseg:4FE7
char aGold_2[] = " gold";
// WZD dseg:4FED
char aAlliance_0[] = "Alliance";
// WZD dseg:4FF6
char aWizardPact_0[] = "Wizard Pact";
// WZD dseg:5002
char aTreaty[] = "treaty";

// WZD dseg:5009 00                                              align 2

// WZD dseg:5009                                                 END:  ovr086 - Initialized Data





// WZD dseg:C308                                                 BEGIN:  ovr084 - Uninitialized Data

// WZD dseg:C308 00 00                                           word_42DA8 dw 0                         ; DATA XREF: sub_6EFA5:loc_6F07Aw ...
// WZD dseg:C30A
byte_ptr G_Some_DIPL_Alloc_5;
// WZD dseg:C30C
byte_ptr G_Some_DIPL_Alloc_4;
// WZD dseg:C30E 00 00                                           word_42DAE dw 0                         ; DATA XREF: sub_70AFE:loc_70CAFw
// WZD dseg:C310 00                                              db    0
// WZD dseg:C311 00                                              db    0
// WZD dseg:C312
byte_ptr * G_Some_DIPL_Allocs_6;
// WZD dseg:C31E
byte_ptr G_Some_DIPL_Alloc_3;
// WZD dseg:C320 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IDK_DIPLO_NIU db 78h dup(0)
// WZD dseg:C398 00                                              byte_42E38 db 0                         ; DATA XREF: sub_72131+CAw ...
// WZD dseg:C399 00 00 00 00 00 00 00 00 00 00                   byte_42E39 db 0Ah dup(0)                ; DATA XREF: sub_74420+36Aw ...
// WZD dseg:C3A3 00 00 00 00 00 00 00 00 00 00                   byte_42E43 db 0Ah dup(0)                ; DATA XREF: sub_74420+347w ...
// WZD dseg:C3AD 00                                              byte_42E4D db 0                         ; DATA XREF: sub_72131+A9w ...
// WZD dseg:C3AE 00 00 00 00 00 00 00 00 00 00                   DIPL_SpellValues db 0Ah dup(0)          ; DATA XREF: DIPL_TribSpellList+53w ...
// WZD dseg:C3AE                                                                                         ; BUG: these don't fit into a byte variable!
// WZD dseg:C3B8 00 00 00 00 00 00 00 00 00 00                   DIPL_TradeSpells db 0Ah dup(0)          ; DATA XREF: sub_6F6BB+5Ar ...
// WZD dseg:C3C2 00                                              DIPL_TradeSpellCount db 0               ; DATA XREF: IDK_Diplo_Scrn+58r ...
// WZD dseg:C3C2                                                                                         ; 1-byte, unsigned

// WZD dseg:C3C3 00                                              align 2

// WZD dseg:C3C4
int16_t Target_Player;
// WZD dseg:C3C6 00 00                                           word_42E66 dw 0                         ; DATA XREF: sub_6F982+39Bw ...
// WZD dseg:C3C8
int16_t Spell_Index;

// WZD dseg:C3CA
SAMB_ptr * m_diplomac_right_eyes_segs;
// WZD dseg:C3CC
SAMB_ptr * m_diplomac_left_eyes_segs;

// WZD dseg:C3CE 00 00                                           dw 0
// WZD dseg:C3D0 00 00                                           dw 0
// WZD dseg:C3D2
SAMB_ptr IMG_DIPL_TalkAnim;
// WZD dseg:C3D4
int16_t word_42E74;

// WZD dseg:C3D6
SAMB_ptr sound_buffer2;
/* HACK */  uint32_t sound_buffer2_size;
// WZD dseg:C3D8
SAMB_ptr sound_buffer1;
/* HACK */  uint32_t sound_buffer1_size;

// WZD dseg:C3DA 00 00                                           dw 0

// WZD dseg:C3DC
SAMB_ptr diplomacy_background_seg;
// WZD dseg:C3DE
int16_t word_42E7E;
// WZD dseg:C3E0
int16_t word_42E80;
// WZD dseg:C3E2
char * G_DIPL_ComposedMessage;
// WZD dseg:C3E4
char * G_DIPL_TempMessage;
// WZD dseg:C3E6
int16_t word_42E86;
// WZD dseg:C3E8
int16_t word_42E88;
// WZD dseg:C3EA
int16_t G_DIPL_RndMsgIndex;
// WZD dseg:C3EC
int16_t word_42E8C;
// WZD dseg:C3EE
int16_t word_42E8E;
// WZD dseg:C3F0
byte_ptr * G_Some_DIPL_Allocs_7;
// WZD dseg:C3FE
int16_t m_diplomac_player_idx = 0;
// WZD dseg:C400
SAMB_ptr G_DiploMsg_E0_Field0;
// WZD dseg:C402
int16_t G_DiploMsg_E0_Field2;
// WZD dseg:C404
int16_t G_DiploMsg_E0_Field4;

// WZD dseg:C404                                                 END:  ovr086 - Uninitialized Data





/*
    WIZARDS.EXE  ovr084
*/



// WZD o84p01
/*
*/
void Diplomacy_Screen__WIP(void)
{
    int16_t full_screen_field = 0;
    int16_t input_field_idx = 0;  // _DI_
    int16_t leave_screen = 0;  // _SI_

    Limit_Temporary_Peace_Modifier();

    word_42E74 = ST_UNDEFINED;

    word_42E7E = ST_UNDEFINED;

    word_42E80 = 0;

    Apply_Palette();

    leave_screen = ST_FALSE;

    Assign_Auto_Function(Diplomacy_Screen_Draw__WIP, 3);

    Set_Input_Delay(1);

    Clear_Fields();

    word_42E8E = 6;

    word_42E8C = _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[m_diplomac_player_idx];

    _page_flip_effect = pfe_None;

    if(
        (word_42E8C == 54)
        ||
        (word_42E8C == 1)
    )
    {

        return;

    }

    if(word_42E8C == 0)
    {

        word_42E8E = 0;

        IDK_DiplSts_s70570();

    }

    Diplomacy_Screen_Load__WIP();

    G_DIPL_RndMsgIndex = ST_UNDEFINED;


    if(
        (
            (word_42E8C >= 45)
            &&
            (word_42E8C <= 49)
        )
        ||
        (word_42E8C == 2)
        ||
        (word_42E8C == 47)
    )
    {

        word_42E88 = word_42E8C;

        word_42E8C = 42;

        word_42E8E = 2;

        word_42E86 = _players[HUMAN_PLAYER_IDX].Dipl.Unknown_22Ah[m_diplomac_player_idx];

    }

    full_screen_field = INVALID_FIELD;

    while(leave_screen == ST_FALSE)
    {

        Mark_Time();

        input_field_idx = Get_Input();

        if(word_42E8E == 0)
        {

            if(input_field_idx == ST_UNDEFINED)
            {

                leave_screen = ST_TRUE;

            }

            if(input_field_idx == full_screen_field)
            {

                IDK_Diplo_Scrn();

                leave_screen = ST_TRUE;

            }

        }
        else if(word_42E8E == 1)
        {

            if(
                (input_field_idx == ST_UNDEFINED)
                ||
                (input_field_idx == full_screen_field)
            )
            {

                leave_screen = ST_TRUE;

            }

        }
        else if(word_42E8E == 6)
        {

            sub_6F51A(word_42E8C, 3);

            leave_screen = ST_TRUE;

        }

        Clear_Fields();

        full_screen_field = INVALID_FIELD;

        if(
            (word_42E8E == 0)
            ||
            (word_42E8E == 1)
            ||
            (word_42E8E == 2)
        )
        {

            full_screen_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, (int16_t)str_empty_string__ovr084[0], ST_UNDEFINED);

        }

        if(leave_screen == ST_FALSE)
        {

            Diplomacy_Screen_Draw__WIP();

            PageFlip_FX();

            Release_Time(3);

        }

    }

    Stop_All_Sounds__STUB();

    Deactivate_Auto_Function();

    Clear_Fields();

    current_screen = scr_Magic_Screen;

    Fade_Out();

    _page_flip_effect = pfe_TogglePagesFadeIn;

    Combat_Cache_Read();

    GAME_CheckResearch(1);

    Cache_Graphics_Overland();

    Play_Background_Music__STUB();

}


// WZD o84p02
void Diplomacy_Screen_Draw__WIP(void)
{



}


// WZD o84p01
// sub_6ECD9()

// WZD o84p02
// sub_6ED5D()

// WZD o84p03
// sub_6EE03()

// WZD o84p04
// sub_6EFA5()

// WZD o84p05
// sub_6F3E4()

// WZD o84p06
static void sub_6F51A(int16_t IDK1, int16_t IDK2)
{
    int16_t full_screen_field = 0;
    int16_t leave_screen = 0;
    int16_t si = 0;  // _SI_
    int16_t di = 0;  // _DI_

    si = IDK1;

    di = 0;

    if(si == 57)
    {

        di = 1;

    }

    if(
        (si >= 58)
        &&
        (si <= 66)
    )
    {

        di = 1;

    }

    if(
        (si >= 51)
        &&
        (si <= 56)
    )
    {

        di = 1;

    }

    word_42E8C = si;

    if(di != 1)
    {

        switch(IDK2)
        {

            case 0:
            case 1:
            case 2:
            {
                // mov     bx, [m_diplomac_player_idx]
                // cmp     [(_players.Dipl.Unknown_1A6h+6)+bx], 0
                if(
                    (_players[HUMAN_PLAYER_IDX].Dipl.field_84[m_diplomac_player_idx] != 0)
                    &&
                    (Random(2)  == 1)
                )
                {

                    word_42E8C = _players[HUMAN_PLAYER_IDX].Dipl.field_84[m_diplomac_player_idx];

                    _players[HUMAN_PLAYER_IDX].Dipl.field_84[m_diplomac_player_idx] = 0;

                }
                else
                {

                    if(
                        (_players[HUMAN_PLAYER_IDX].Dipl.Broken_Treaty[m_diplomac_player_idx] != 0)
                        &&
                        (Random(4) == 1)
                    )
                    {

                        _players[HUMAN_PLAYER_IDX].Dipl.Broken_Treaty[m_diplomac_player_idx] = 0;

                        word_42E8C = 58;

                    }
                    else
                    {

                        word_42E8C = 57;

                    }

                }

            } break;

            case 3:
            {

                if(
                    (Random(4) == 1)
                    &&
                    (_players[HUMAN_PLAYER_IDX].Dipl.field_8A[m_diplomac_player_idx] != 0)
                )
                {

                    Spell_Index = _players[HUMAN_PLAYER_IDX].Dipl.field_8A[m_diplomac_player_idx];

                    _players[HUMAN_PLAYER_IDX].Dipl.field_8A[m_diplomac_player_idx] = 0;

                    word_42E8C = 54;

                }
                else
                {
                    
                    word_42E8C = si;

                }

            } break;

            default:
            {

                STU_DEBUG_BREAK();

            } break;


        }

    }
    else
    {

        word_42E8C = si;

    }

    Assign_Auto_Function(_sub_6F90E_Draw, 3);

    Set_Input_Delay(1);

    G_DIPL_RndMsgIndex = ST_UNDEFINED;

    IDK_DIPLOMSG_s732D9(word_42E8C, m_diplomac_player_idx);

    leave_screen = ST_FALSE;

    Clear_Fields();

    full_screen_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, (int16_t)str_empty_string__ovr084[0], ST_UNDEFINED);

    while(leave_screen == ST_FALSE)
    {

        Mark_Time();

        if(Get_Input() != 0)
        {

            leave_screen = ST_TRUE;

        }

        if(leave_screen == ST_FALSE)
        {

            _sub_6F90E_Draw();

            PageFlip_FX();

            Release_Time(3);

        }

    }

}


// WZD o84p07
// sub_6F6BB()

// WZD o84p08
static void _sub_6F90E_Draw(void)
{



}


// WZD o84p09
// sub_6F982()

// WZD o84p10
static void IDK_Diplomacy_Background_Music__STUB(int16_t IDK)
{

    if(IDK != word_42E74)
    {

        if(magic_set.background_music == ST_TRUE)
        {

            if(IDK == 0)
            {

                // DOMSDOS  Play_Sound__STUB(sound_buffer1);
                sdl2_Play_Sound__WIP(sound_buffer1, sound_buffer1_size);

            }
            else if(IDK == 1)
            {

                // DOMSDOS  Play_Sound__STUB(sound_buffer1);
                sdl2_Play_Sound__WIP(sound_buffer1, sound_buffer1_size);

            }
            else if(IDK == 2)
            {

                // DOMSDOS  Play_Sound__STUB(sound_buffer2);
                sdl2_Play_Sound__WIP(sound_buffer2, sound_buffer2_size);

            }

        }

        word_42E74 = IDK;

    }

}

// WZD o84p11
// IDK_DiplAnim_s6FDA1()

// WZD o84p12
static void Diplomacy_Screen_Load__WIP(void)
{
    int16_t var_2 = 0;
    int16_t itr = 0;  // _SI_

    Stop_All_Sounds__STUB();

    Cache_Graphics_Diplomacy();

    Combat_Cache_Write();

    // DOMSDOS  _CITIES = SA_MK_FP0(Allocate_First_Block(World_Data, 714));
    _CITIES = (struct s_CITY *)Allocate_First_Block(World_Data, 714);

    sound_buffer1 = LBX_Reload_Next(music_lbx_file__ovr084, (MUSIC_Merlin_Good + _players[m_diplomac_player_idx].wizard_id), World_Data);
    sound_buffer1_size = lbxload_entry_length;

    sound_buffer2 = LBX_Reload_Next(music_lbx_file__ovr084, (MUSIC_Merlin_Bad + _players[m_diplomac_player_idx].wizard_id), World_Data);
    sound_buffer2_size = lbxload_entry_length;

    var_2 = ((100 + _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]) / 20);

    m_diplomac_left_eyes_segs = (SAMB_ptr *)Near_Allocate_First((11 * sizeof(SAMB_ptr)));  // was 11 * 2, now 11 * 8

    m_diplomac_right_eyes_segs = (SAMB_ptr *)Near_Allocate_Next((11 * sizeof(SAMB_ptr)));  // was 11 * 2, now 11 * 8

    // DIPLOMAC.LBX, 000    "DIPLGAR2"  "diplomacy back"
    diplomacy_background_seg = LBX_Reload(diplomac_lbx_file__ovr084, 0, _screen_seg);

    for(itr = 0; itr < 11; itr++)
    {

        // DIPLOMAC.LBX, 002    "EYES1"     "left eyes"
        // DIPLOMAC.LBX, 003    "EYES2"     "left eyes"
        // DIPLOMAC.LBX, 004    "EYES3"     "left eyes"
        // DIPLOMAC.LBX, 005    "EYES4"     "left eyes"
        // DIPLOMAC.LBX, 006    "EYES5"     "left eyes"
        // DIPLOMAC.LBX, 007    "EYES6"     "left eyes"
        // DIPLOMAC.LBX, 008    "EYES7"     "left eyes"
        // DIPLOMAC.LBX, 009    "EYES8"     "left eyes"
        // DIPLOMAC.LBX, 010    "EYES9"     "left eyes"
        // DIPLOMAC.LBX, 011    "EYES10"    "left eyes"
        // DIPLOMAC.LBX, 012    "EYES11"    "left eyes"
        m_diplomac_left_eyes_segs[itr] = LBX_Reload_Next(diplomac_lbx_file__ovr084, (2 + itr), _screen_seg);

    }

    for(itr = 0; itr < 11; itr++)
    {

        // DIPLOMAC.LBX, 013    "EYES1"     "right eyes"
        // DIPLOMAC.LBX, 014    "EYES2"     "right eyes"
        // DIPLOMAC.LBX, 015    "EYES3"     "right eyes"
        // DIPLOMAC.LBX, 016    "EYES4"     "right eyes"
        // DIPLOMAC.LBX, 017    "EYES5"     "right eyes"
        // DIPLOMAC.LBX, 018    "EYES6"     "right eyes"
        // DIPLOMAC.LBX, 019    "EYES7"     "right eyes"
        // DIPLOMAC.LBX, 020    "EYES8"     "right eyes"
        // DIPLOMAC.LBX, 021    "EYES9"     "right eyes"
        // DIPLOMAC.LBX, 022    "EYES10"    "right eyes"
        // DIPLOMAC.LBX, 023    "EYES11"    "right eyes"
        m_diplomac_right_eyes_segs[itr] = LBX_Reload_Next(diplomac_lbx_file__ovr084, (13 + itr), _screen_seg);

    }

    CLROFF();

    FLIC_Draw(0, 0, diplomacy_background_seg);

    IDK_DiplEyes_s7028D();

    Clear_Fields();

    Apply_Palette();

    Toggle_Pages();

    CLROFF();

    FLIC_Draw(0, 0, diplomacy_background_seg);

    IDK_DiplEyes_s7028D();

    Copy_On_To_Off_Page();

    Copy_Off_To_Back();

    if(word_42E8E != 1)
    {

        IDK_Dipl_Draw_s702E6();

    }

    DIPL_LoadTalkGFX();

}


// WZD o84p13
static void DIPL_LoadTalkGFX(void)
{
    int16_t UU_Relation_Index = 0;
    int16_t DIPLOMAC_Portrait_Index = 0;
    int16_t itr = 0;  // _SI_

    DIPLOMAC_Portrait_Index = (24 + _players[m_diplomac_player_idx].wizard_id);

    UU_Relation_Index = ((100 + _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]) / 20);

    G_DIPL_TempMessage = (char * )Near_Allocate_First(220);

    G_DIPL_ComposedMessage = (char * )Near_Allocate_First(220);

    for(itr = 0; itr < 7; itr++)
    {

        G_Some_DIPL_Allocs_7[itr] = Near_Allocate_Next(100);

    }

    for(itr = 0; itr < 6; itr++)
    {

        G_Some_DIPL_Allocs_6[itr] = Near_Allocate_Next(50);

    }

    G_Some_DIPL_Alloc_3 = Near_Allocate_Next(20);

    G_Some_DIPL_Alloc_4 = Near_Allocate_Next(20);

    G_Some_DIPL_Alloc_5 = Near_Allocate_Next(20);

    m_diplomac_left_eyes_segs = (SAMB_ptr * )Near_Allocate_Next((11 * sizeof(SAMB_ptr)));

    m_diplomac_right_eyes_segs = (SAMB_ptr *)Near_Allocate_Next((11 * sizeof(SAMB_ptr)));

    IMG_MOODWIZPortrait = LBX_Reload(moodwiz_lbx_file__ovr084, _players[m_diplomac_player_idx].wizard_id, _screen_seg);

    diplomacy_mirror_seg = LBX_Reload_Next(backgrnd_lbx_file__ovr084, 18, _screen_seg);

    IMG_DIPL_TalkAnim = LBX_Reload_Next(diplomac_lbx_file__ovr084, DIPLOMAC_Portrait_Index, _screen_seg);

    for(itr = 0; itr < 11; itr++)
    {

        // DIPLOMAC.LBX, 002    "EYES1"     "left eyes"
        // DIPLOMAC.LBX, 003    "EYES2"     "left eyes"
        // DIPLOMAC.LBX, 004    "EYES3"     "left eyes"
        // DIPLOMAC.LBX, 005    "EYES4"     "left eyes"
        // DIPLOMAC.LBX, 006    "EYES5"     "left eyes"
        // DIPLOMAC.LBX, 007    "EYES6"     "left eyes"
        // DIPLOMAC.LBX, 008    "EYES7"     "left eyes"
        // DIPLOMAC.LBX, 009    "EYES8"     "left eyes"
        // DIPLOMAC.LBX, 010    "EYES9"     "left eyes"
        // DIPLOMAC.LBX, 011    "EYES10"    "left eyes"
        // DIPLOMAC.LBX, 012    "EYES11"    "left eyes"
        m_diplomac_left_eyes_segs[itr] = LBX_Reload_Next(diplomac_lbx_file__ovr084, (2 + itr), _screen_seg);
        
    }

    for(itr = 0; itr < 11; itr++)
    {

        // DIPLOMAC.LBX, 013    "EYES1"     "right eyes"
        // DIPLOMAC.LBX, 014    "EYES2"     "right eyes"
        // DIPLOMAC.LBX, 015    "EYES3"     "right eyes"
        // DIPLOMAC.LBX, 016    "EYES4"     "right eyes"
        // DIPLOMAC.LBX, 017    "EYES5"     "right eyes"
        // DIPLOMAC.LBX, 018    "EYES6"     "right eyes"
        // DIPLOMAC.LBX, 019    "EYES7"     "right eyes"
        // DIPLOMAC.LBX, 020    "EYES8"     "right eyes"
        // DIPLOMAC.LBX, 021    "EYES9"     "right eyes"
        // DIPLOMAC.LBX, 022    "EYES10"    "right eyes"
        // DIPLOMAC.LBX, 023    "EYES11"    "right eyes"
        m_diplomac_right_eyes_segs[itr] = LBX_Reload_Next(diplomac_lbx_file__ovr084, (13 + itr), _screen_seg);
        
    }

}


// WZD o84p14
static void IDK_DiplEyes_s7028D(void)
{
    int16_t si = 0;

    si = ((100 + _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]) / 20);

    FLIC_Draw(64, 58, m_diplomac_left_eyes_segs[si]);

    FLIC_Draw(233, 58, m_diplomac_right_eyes_segs[si]);

}


// WZD o84p15
static void IDK_Dipl_Draw_s702E6(void)
{
    SAMB_ptr picture = 0;
    int16_t entry_num = 0;
    int16_t itr = 0;

    entry_num = (38 + _players[m_diplomac_player_idx].wizard_id);

    // DIPLOMAC.LBX, 038    "MERLFADE"  "fade"
    // DIPLOMAC.LBX, 039    "SHAMFADE"  "fade"
    // DIPLOMAC.LBX, 040    "PRIEFADE"  "fade"
    // DIPLOMAC.LBX, 041    "WUFADE"    "fade"
    // DIPLOMAC.LBX, 042    "ARABFADE"  "fade"
    // DIPLOMAC.LBX, 043    "OBERFADE"  "fade"
    // DIPLOMAC.LBX, 044    "WRAIFADE"  "fade"
    // DIPLOMAC.LBX, 045    "DRACFADE"  "fade"
    // DIPLOMAC.LBX, 046    "NMOIFADE"  "fade"
    // DIPLOMAC.LBX, 047    "FREYFADE"  "fade"
    // DIPLOMAC.LBX, 048    "GALEFADE"  "fade"
    // DIPLOMAC.LBX, 049    "ARIEFADE"  "fade"
    // DIPLOMAC.LBX, 050    "AZTEFADE"  "fade"
    // DIPLOMAC.LBX, 051    "KARLFADE"  "fade"
    picture = LBX_Reload(diplomac_lbx_file__ovr084, entry_num, _screen_seg);

    diplomacy_mirror_seg = LBX_Reload_Next(backgrnd_lbx_file__ovr084, 18, _screen_seg);

    Mark_Time();

    Release_Time(6);

    for(itr = 0; itr < 5; itr++)
    {

        Mark_Time();

        Set_Page_Off();

        FLIC_Draw(107, 13, picture);

        FLIC_Draw(95, 1, diplomacy_mirror_seg);

        PageFlip_FX();

        Release_Time(3);

    }

}


// WZD o84p16
// sub_7038D()

// WZD o84p17
// DIPL_GetInvasionStr()



/*
    WIZARDS.EXE  ovr085
*/

// WZD o85p01
static void IDK_DiplSts_s70570(void)
{
    int16_t var_2 = 0;
    int16_t si = 0;

    var_2 = DIPL_LowestInterest();

// // ; added to chance of forming treaties
// // ; added to chance of avoiding superiority wars
// int16_t TBL_AI_PRS_IDK_Mod[6] = { 0, 10, 20, 30, 40, 50 };
    si = (var_2
        + _players[HUMAN_PLAYER_IDX].Dipl.Hidden_Rel[m_diplomac_player_idx]
        + _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]
        + TBL_AI_PRS_IDK_Mod[_players[m_diplomac_player_idx].Personality]);

    if((si - _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]) <= -100)
    {

        if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[m_diplomac_player_idx] >= 3)
        {

            // jmp     short $+2

        }

        word_42E8C = 44;

        word_42E8E = 1;

    }
    else
    {

        if(si > -50)
        {

            word_42E8C = 42;

        }
        else
        {

            word_42E8C = 43;

        }

        word_42E8E = 0;

    }

}


// WZD o85p02
// DIPL_Gravitation()

// WZD o85p03
// sub_70A1A()

// WZD o85p04
// sub_70AFE()

// WZD o85p05
/*
; returns the lowest out the target's treaty, trade,
; and peace interests towards the human player
; BUG: checks an unset variable to return -1000 instead
; but it can't (intended to be dipl status maybe?)
*/
/*
*/
static int16_t DIPL_LowestInterest(void)
{
    int16_t interest = 0;  // _DX_
    int16_t return_value = 0;  // _AX_

    if(AI_Dipl_Unset_0 == 3)
    {

        interest = -1000;

        return_value = 0;

    }
    else
    {

        if(_players[HUMAN_PLAYER_IDX].Dipl.Treaty_Interest[m_diplomac_player_idx] < _players[HUMAN_PLAYER_IDX].Dipl.Trade_Interest[m_diplomac_player_idx])
        {

            interest = _players[HUMAN_PLAYER_IDX].Dipl.Treaty_Interest[m_diplomac_player_idx];

        }

        if(interest < _players[HUMAN_PLAYER_IDX].Dipl.Peace_Interest[m_diplomac_player_idx])
        {

            interest = _players[HUMAN_PLAYER_IDX].Dipl.Peace_Interest[m_diplomac_player_idx];

        }

        return_value = interest;
    
    }

    return return_value;

}


// WZD o85p06
static void Limit_Temporary_Peace_Modifier(void)
{
    int16_t itr2 = 0;  // _SI_
    int16_t itr1 = 0;  // CX_

    for(itr1 = 0; itr1 < _num_players; itr1++)
    {

        for(itr2 = 0; itr2 < _num_players; itr2++)
        {

            if(itr1 != itr2)
            {

                SETMAX(_players[itr1].Dipl.Treaty_Interest[itr2], 120);

                SETMIN(_players[itr1].Dipl.Treaty_Interest[itr2], -200);

            }

        }

    }

}

// WZD o85p07
// WIZ_DIPL_TeachSpell()

// WZD o85p08
// G_DIPL_PickSides()

// WZD o85p09
// DIPL_SignTreaty()

// WZD o85p10
// DIPL_SignPeaceTreaty()

// WZD o85p11
// DIPL_LeaveMeAlone()

// WZD o85p12
// drake178: G_DIPL_Action()
// MoO2  Module: DIPLOMAC  Change_Relations_()  Module: NETMOX  Russ_Change_Relations_()
/*
    Applies a change in diplomatic relations

; multiple BUGs, unfinished subfunctions, and unknown variables; needs further study!

called from End_Of_Combat()
    |-> G_DIPL_Action(-Diplomatic_Value, _combat_attacker_player, _combat_defender_player, 8, ST_NULL, ST_NULL);

Spellbook_Screen()
    if(did_select_spell == ST_TRUE)
        Cast_Spell_Overland_Do(HUMAN_PLAYER_IDX, spell_idx, spellbook_page_spell_index);
            Change_Relations_For_Enchantments(player_idx, spl_Spell_Of_Mastery, 1)
                if(spell_idx == spl_Spell_Of_Mastery)
                    Change_Relations((-50 / divisor), player_idx, itr_players, 10, 0, spell_idx);

*/
void Change_Relations__WIP(int16_t value, int16_t attacker_idx, int16_t defender_idx, int16_t type, int16_t city_idx, int16_t spell_idx)
{
    int16_t Rel_Divisor;
    int16_t NoCharisma_RelValue;

    if(_players[attacker_idx].Dipl.Contacted[defender_idx] != ST_FALSE)
    {
        if(AI_Dipl_Unset_0 == 3)
        {
            if(
                (attacker_idx != HUMAN_PLAYER_IDX)
                &&
                (value < 0)
                &&
                (defender_idx == HUMAN_PLAYER_IDX)
            )
            {
                if(Random(100) <= (abs(value) / 10))
                {

                    _players[HUMAN_PLAYER_IDX].Dipl.DA_Strength[HUMAN_PLAYER_IDX] = 100;
                    _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[HUMAN_PLAYER_IDX] = 39;
                    _players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[HUMAN_PLAYER_IDX] = spell_idx;
                    _players[HUMAN_PLAYER_IDX].Dipl.DA_City[HUMAN_PLAYER_IDX] = city_idx;
                }
            }
        }
        else
        {
            if(
                (attacker_idx < MOO_MONSTER_PLAYER_IDX)
                &&
                (defender_idx < MOO_MONSTER_PLAYER_IDX)
            )
            {
                if(
                    (attacker_idx != defender_idx)
                    &&
                    (value != 0)
                    &&
                    (defender_idx != HUMAN_PLAYER_IDX)
                    &&
                    (_players[attacker_idx].Dipl.Dipl_Status[defender_idx] != DIPL_Crusade)
                    &&
                    (defender_idx <= _num_players)
                    &&
                    (attacker_idx <= _num_players)
                )
                {
                    if(value == -10000)
                    {
                        if(
                            (attacker_idx == HUMAN_PLAYER_IDX)
                            &&
                            (type != dipact_AuraOfMajesty)
                        )
                        {
                            _players[HUMAN_PLAYER_IDX].Dipl.DA_Strength[defender_idx] = 10000;
                            _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[defender_idx] = type;
                            _players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[defender_idx] = spell_idx;
                            _players[HUMAN_PLAYER_IDX].Dipl.DA_City[defender_idx] = city_idx;
                        }
                    }
                    else
                    {
                        if(value > 0)
                        {
                            if(_players[defender_idx].Dipl.Visible_Rel[attacker_idx] < 0)
                            {
                                value = (value * 2);

                                if((_players[defender_idx].Dipl.Visible_Rel[attacker_idx] + value) > 10)
                                {
                                    value = (10 - _players[defender_idx].Dipl.Visible_Rel[attacker_idx]);
                                }
                            }
                            else
                            {
                                Rel_Divisor = ((_players[defender_idx].Dipl.Visible_Rel[attacker_idx] / 25) + 1);

                                if(Rel_Divisor == 0)
                                {
                                    Rel_Divisor = 1;
                                }

                                value = (value / Rel_Divisor);
                            }
                        }
                        else
                        {
                            if(_players[defender_idx].Dipl.Visible_Rel[attacker_idx] > 0)
                            {
                                value = (value * 2);
                            }
                            else
                            {
                                Rel_Divisor = ((_players[defender_idx].Dipl.Visible_Rel[attacker_idx] / -25) + 1);

                                if(Rel_Divisor == 0)
                                {
                                    Rel_Divisor = 1;
                                }

                                value = (value / Rel_Divisor);
                            }
                        }
                    }

                    if(
                        (_players[defender_idx].Personality == PRS_Lawful)
                        &&
                        (value < 0)
                    )
                    {
                        value = (value * 2);
                    }
                    
                    if(
                        (_players[defender_idx].Personality == PRS_Maniacal)
                        &&
                        (value < 0)
                    )
                    {
                        value = ((value * 3) / 2);
                    }
                    
                    if(
                        (_players[defender_idx].Personality == PRS_Maniacal)
                        &&
                        (value > 0)
                    )
                    {
                        value = ((value * 3) / 4);
                    }
                    
                    NoCharisma_RelValue = value;

                    // "... special ability (Charismatic) doubles the effectiveness of any positive diplomatic actions she takes and halves the negative impact ..."
                    // "... Charismatic wizard obtains twice the normal effect from positive diplomatic actions he or she initiates and suffers only half the normal penalty for negative diplomatic actions."
                    if(
                        (_players[attacker_idx].charismatic == ST_TRUE)
                        &&
                        (value > 0)
                    )
                    {
                        value = (value * 2);
                    }

                    if(
                        (_players[attacker_idx].charismatic == ST_TRUE)
                        &&
                        (value < 0)
                    )
                    {
                        value = (value / 2);
                    }


                    if(
                        (_players[defender_idx].Dipl.Dipl_Status[attacker_idx] == DIPL_War)
                        &&
                        (type <= dipact_Overextension)
                        &&
                        (value < 0)
                    )
                    {
                        if(
                            (_players[defender_idx].Dipl.Dipl_Status[attacker_idx] == DIPL_War)
                            &&
                            (attacker_idx == HUMAN_PLAYER_IDX)
                        )
                        {
                            // drake178: ; the result is still negative ; decreasing the target's toward the aggressor
                            _players[defender_idx].Dipl.Peace_Interest[attacker_idx] += (value / 4);
                            // drake178: ; the result is still negative ; increasing the aggressor's toward the target
                            _players[attacker_idx].Dipl.Peace_Interest[defender_idx] -= (value / 4);
                        }

                        value = 0;
                    }

                    /*
                        BEGIN:  
                    */
                    if(
                        (attacker_idx != HUMAN_PLAYER_IDX)
                        &&
                        (defender_idx != HUMAN_PLAYER_IDX)
                        &&
                        (value > 0)
                        &&
                        (_turn > 200)
                    )
                    {
                        value = (value * 2);
                    }
                    /*
                        END:  
                    */
                    /*
                        BEGIN:  
                    */
                    if(
                        (attacker_idx != HUMAN_PLAYER_IDX)
                        &&
                        (defender_idx != HUMAN_PLAYER_IDX)
                        &&
                        (value < 0)
                        &&
                        (_turn > 200)
                    )
                    {
                        value = (value / ((_difficulty / 2) + 1));  // {0,1,2,3,4} / 2 = {0,0,1,1,2} + 1 = {1,1,2,2,3} = {100%,100%,50%,50%,33%}
                    }
                    /*
                        END:  
                    */

                    if(value != 0)
                    {
                        _players[defender_idx].Dipl.Visible_Rel[attacker_idx] += value;

                        SETMIN(_players[defender_idx].Dipl.Visible_Rel[attacker_idx], -100);
                        SETMAX(_players[defender_idx].Dipl.Visible_Rel[attacker_idx],  100);

                        _players[defender_idx].Dipl.Treaty_Interest[attacker_idx] += value;
                        _players[defender_idx].Dipl.Trade_Interest[attacker_idx] += value;

                        _players[attacker_idx].Dipl.Treaty_Interest[defender_idx] = _players[defender_idx].Dipl.Treaty_Interest[attacker_idx];
                        _players[attacker_idx].Dipl.Trade_Interest[defender_idx] = _players[defender_idx].Dipl.Trade_Interest[attacker_idx];

                        // HERE:  ¿ NoCharisma_RelValue is value, without the Charismatic modifier, but with ?
                        if(NoCharisma_RelValue < 0)  /* relation change is negative */
                        {
                            // drake178: ; double shift if not at war?
                            _players[attacker_idx].Dipl.Peace_Interest[defender_idx] -= (NoCharisma_RelValue / 4);  // more negative
                            _players[defender_idx].Dipl.Peace_Interest[attacker_idx] += (NoCharisma_RelValue / 4);  // less positive
                        }
                        else
                        {
                            _players[defender_idx].Dipl.Peace_Interest[attacker_idx] += NoCharisma_RelValue;
                        }

                        
                        if(
                            (spell_idx == 10)  /* Earth_Lore */
                            &&
                            (Random(100) < 11)
                        )
                        {
                            Declare_War(attacker_idx, defender_idx);
                        }

                        
                        if(
                            (attacker_idx == HUMAN_PLAYER_IDX)
                            &&
                            (type != dipact_AuraOfMajesty)
                        )
                        {
                            if(abs(value) > abs(_players[defender_idx].Dipl.DA_Strength[HUMAN_PLAYER_IDX]))
                            {
                                _players[defender_idx].Dipl.DA_Strength[HUMAN_PLAYER_IDX] = value;
                                _players[defender_idx].Dipl.Dipl_Action[HUMAN_PLAYER_IDX] = type;
                                _players[defender_idx].Dipl.DA_Spell[HUMAN_PLAYER_IDX] = spell_idx;
                                _players[defender_idx].Dipl.DA_City[HUMAN_PLAYER_IDX] = city_idx;
                            }
                        }

                    }  /* if(value != 0) */


                    if(_players[defender_idx].Dipl.Dipl_Status[attacker_idx] >= 3)  /* DIPL_War */
                    {
                        SETMAX(_players[defender_idx].Dipl.Visible_Rel[attacker_idx], -25);
                    }

                    if(_players[defender_idx].Dipl.Dipl_Status[attacker_idx] != 2)  /* DIPL_Alliance */
                    {
                        SETMAX(_players[defender_idx].Dipl.Visible_Rel[attacker_idx], 65);
                    }


                    _players[attacker_idx].Dipl.Dipl_Status[defender_idx] = _players[defender_idx].Dipl.Dipl_Status[attacker_idx];


                    if(_players[defender_idx].Dipl.Dipl_Status[attacker_idx] >= 3)  /* DIPL_War */
                    {
                        _players[defender_idx].Hostility[attacker_idx] = 3;
                    }

                    if(_players[defender_idx].Dipl.Dipl_Status[attacker_idx] == 2)  /* DIPL_Alliance */
                    {
                        _players[defender_idx].Hostility[attacker_idx] = 0;
                    }


                    _players[defender_idx].reevaluate_hostility_countdown = (15 + Random(10));


                }  /* ¿ Normal / Everything ? */
            }

        }

    }

}

// WZD o85p13
// MoO2  Module: DIPLOMAC  Declare_War_()
// 1oom  game\game_diplo.c  game_diplo_start_war()
/*

; this starts the actual war, after calling the war
; prepare function that does things like handle broken treaties
; uses at least one unknown variable, and has a
; possible parameter passing BUG, needs further study!

*/
void Declare_War(int16_t attacker_idx, int16_t defender_idx)
{

    if(
        (attacker_idx == HUMAN_PLAYER_IDX)
        &&
        (_players[attacker_idx].Dipl.Contact_Progress[defender_idx] == 0)
    )
    {
        _players[attacker_idx].Dipl.Contact_Progress[defender_idx] = 1;
    }

    if(
        (defender_idx == HUMAN_PLAYER_IDX)
        &&
        (_players[defender_idx].Dipl.Contact_Progress[attacker_idx] == 0)
    )
    {
        _players[defender_idx].Dipl.Contact_Progress[attacker_idx] = 1;
    }

    if(
        (attacker_idx > NEUTRAL_PLAYER_IDX)
        ||
        (defender_idx > NEUTRAL_PLAYER_IDX)
        ||
        (_players[attacker_idx].Dipl.Dipl_Status[defender_idx] >= DIPL_War)
    )
    {
        return;
    }


    Break_Treaties(attacker_idx, defender_idx);

    _players[attacker_idx].Dipl.Default_Rel[defender_idx] += -5;
    _players[defender_idx].Dipl.Default_Rel[attacker_idx] += -5;

    _players[attacker_idx].Dipl.Visible_Rel[defender_idx] = (-75 - Random(25));
    _players[defender_idx].Dipl.Visible_Rel[attacker_idx] = _players[attacker_idx].Dipl.Visible_Rel[defender_idx];

    _players[attacker_idx].Dipl.Dipl_Status[defender_idx] = DIPL_War;
    _players[defender_idx].Dipl.Visible_Rel[attacker_idx] = DIPL_War;


    // 1oom: treaty, trade, tech, peace

    _players[attacker_idx].Dipl.Treaty_Interest[defender_idx] = -200;
    _players[defender_idx].Dipl.Treaty_Interest[attacker_idx] = -200;

    _players[attacker_idx].Dipl.Trade_Interest[defender_idx] = -200;
    _players[defender_idx].Dipl.Trade_Interest[attacker_idx] = -200;

    _players[attacker_idx].Dipl.Peace_Interest[defender_idx] = -130;
    _players[defender_idx].Dipl.Peace_Interest[attacker_idx] = -130;


    _players[defender_idx].Hostility[attacker_idx] = 3;
    _players[attacker_idx].Hostility[defender_idx] = 3;

    _players[defender_idx].reevaluate_hostility_countdown = (15 + Random(10));
    _players[attacker_idx].reevaluate_hostility_countdown = (15 + Random(10));

}


// WZD o85p14
// MoO2  Module: DIPLOMAC  Break_Treaties_()
// 1oom  game\game_diplo.c  game_diplo_break_treaty()
/*

Oath Breaker
-5 permanent, cumulative modifier to victim's Core Reaction

; multiple BUGs and unknown variables, needs further study
; returns 0 if either player is out of bounds or the players are already at war
*/
void Break_Treaties(int16_t attacker_idx, int16_t defender_idx)
{
    int16_t itr_players;
    int16_t Rel_Modifier;

    if(
        (attacker_idx > NEUTRAL_PLAYER_IDX)
        ||
        (defender_idx > NEUTRAL_PLAYER_IDX)
        ||
        (_players[attacker_idx].Dipl.Dipl_Status[defender_idx] >= DIPL_War)
    )
    {
        return;
    }


    /*
        OSG: "Oath Breaker"
            Break 'Wizard Pact' or 'Alliance'  (not 'Peace Treaty')
        
        MoM: "break treaty" ... "permanent negative diplomatic penalty"

        applied to Hidden_Rel[]
        applied to Default_Rel[], if Treaty was Alliance
    */
    Rel_Modifier = 0;

    if(_players[attacker_idx].Dipl.Dipl_Status[defender_idx] == DIPL_WizardPact)
    {
        Rel_Modifier = -10;
    }

    if(_players[attacker_idx].Dipl.Dipl_Status[defender_idx] == DIPL_Alliance)
    {
        Rel_Modifier = -20;
    }

    if(_players[attacker_idx].Personality == PRS_Lawful)
    {
        Rel_Modifier = (Rel_Modifier * 2);  // could be 0 * 2 = 0
    }

    // ; BUG: subtracting a negative number
    // ; BUG: no overflow check
    _players[attacker_idx].Dipl.Hidden_Rel[defender_idx] -= Rel_Modifier;


    // decrease the hidden relation of all other wizards towards the target by 5
    // BUG: the target?
    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        if(
            (itr_players != attacker_idx)
            &&
            (itr_players != defender_idx)
        )
        {
            _players[itr_players].Dipl.Hidden_Rel[defender_idx] += -5;  // ¿ -5 permanent, cumulative modifier to victim's Core Reaction ?
        }
    }


    // 1oom:  relation2 === Default_Rel
    if(_players[attacker_idx].Dipl.Dipl_Status[defender_idx] == DIPL_Alliance)
    {
        _players[attacker_idx].Dipl.Default_Rel[defender_idx] -= Rel_Modifier;
    }
    SETMIN(_players[attacker_idx].Dipl.Default_Rel[defender_idx], -100);
    _players[defender_idx].Dipl.Default_Rel[attacker_idx] = _players[attacker_idx].Dipl.Default_Rel[defender_idx];


    // What does it mean for Rel_Modifier to not be zero?
    // ...a treaty was violated...
    if(Rel_Modifier != 0)
    {
        _players[attacker_idx].Dipl.Broken_Treaty[defender_idx] = _players[attacker_idx].Dipl.Dipl_Status[defender_idx];
        /* WASBUG MOO1 blames both parties for breaking treaties */
        _players[defender_idx].Dipl.Broken_Treaty[attacker_idx] = _players[attacker_idx].Dipl.Dipl_Status[defender_idx];
        _players[attacker_idx].Dipl.Visible_Rel[defender_idx] -= Random(20);
    }


    _players[attacker_idx].Dipl.Dipl_Status[defender_idx] = DIPL_NoTreaty;
    _players[defender_idx].Dipl.Dipl_Status[attacker_idx] = DIPL_NoTreaty;

    // 1oom:  attack_bounty[] = PLAYER_NONE
    // MoO2:  reward_attack_player
    _players[attacker_idx].Dipl.Unknown_194h[defender_idx] = 0;
    _players[defender_idx].Dipl.Unknown_194h[attacker_idx] = 0;

    SETMIN(_players[attacker_idx].Dipl.Visible_Rel[defender_idx], -100);
    _players[defender_idx].Dipl.Visible_Rel[attacker_idx] = _players[attacker_idx].Dipl.Visible_Rel[defender_idx];

    _players[attacker_idx].Dipl.Treaty_Interest[defender_idx] = -200;
    _players[attacker_idx].Dipl.Trade_Interest[defender_idx] = -200;
    _players[attacker_idx].Dipl.Peace_Interest[defender_idx] = -200;

    _players[defender_idx].Dipl.Treaty_Interest[attacker_idx] = -200;
    _players[defender_idx].Dipl.Trade_Interest[attacker_idx] = -200;
    _players[defender_idx].Dipl.Peace_Interest[attacker_idx] = -200;

    _players[attacker_idx].reevaluate_hostility_countdown = (15 + Random(10));
    _players[defender_idx].reevaluate_hostility_countdown = (15 + Random(10));

}

// WZD o85p15
// DIPL_TribSpellList()

// WZD o85p16
// sub_71B90()

// WZD o85p17
// sub_72131()

// WZD o85p18
// DIPL_DropCityCurses()




/*
    WIZARDS.EXE  ovr086
*/

// WZD o86p01
// IDK_Dipl_s72690()
// WZD o86p02
// DIPL_AI_To_AI()
// WZD o86p03
// DIPL_sub_72DB6()
// WZD o86p04
// DIPL_sub_72DB6_Draw()

// WZD o86p05
void IDK_DIPLOMSG_s732D9(int16_t G_Msg_Type, int16_t player_idx)
{
    char string[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t CRP_G_Msg_Type_Repeat = 0;
    int16_t var_4 = 0;
    int16_t itr = 0;
    int16_t pos = 0;
    uint8_t code = 0;  // _BX_

    pos = 0;

    var_4 = 0;

    CRP_G_Msg_Type_Repeat = G_Msg_Type;

    if(G_DIPL_RndMsgIndex != ST_UNDEFINED)
    {

        strcpy(G_DIPL_TempMessage, G_DIPL_ComposedMessage);

    }
    else
    {

        LBX_Load_Data_Static(diplomsg_lbx_file__ovr86, 0, G_DiploMsg_E0_Field0, G_Msg_Type, 1, 6);

        if(_num_players == 2)
        {

            G_DIPL_RndMsgIndex = (Random(G_DiploMsg_E0_Field4) - 2);

        }
        else
        {

            G_DIPL_RndMsgIndex = (Random(G_DiploMsg_E0_Field4) - 1);

        }

        SETMIN(G_DIPL_RndMsgIndex, 0);
    
        LBX_Load_Data_Static(diplomsg_lbx_file__ovr86, 1, (SAMB_ptr)G_DIPL_ComposedMessage, G_DIPL_RndMsgIndex + (CRP_G_Msg_Type_Repeat * 15), 1, 200);

        for(itr = 0; itr < 200; itr++)
        {

            if(G_DIPL_ComposedMessage[itr] != 0)
            {

                G_DIPL_TempMessage[pos] = G_DIPL_ComposedMessage[itr];

                code = (G_DIPL_TempMessage[pos] - 128);

                if(code > 20)
                {
                    pos++;
                }
                else
                {

                    switch(code)
                    {

                        case 0:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            strcat(G_DIPL_TempMessage, _players[HUMAN_PLAYER_IDX].name);

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 1:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            strcat(G_DIPL_TempMessage, _players[player_idx].name);

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 2:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            strcat(G_DIPL_TempMessage, _players[_players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[player_idx]].name);

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 3:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            _fstrcpy(&G_DIPL_TempMessage[pos], _CITIES[_players[HUMAN_PLAYER_IDX].Dipl.DA_City[player_idx]].name);

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 4:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            strcpy(&G_DIPL_TempMessage[pos], _city_size_names[_CITIES[_players[HUMAN_PLAYER_IDX].Dipl.DA_City[player_idx]].size]);

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 5:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            strcat(G_DIPL_TempMessage, _players[_players[HUMAN_PLAYER_IDX].Dipl.field_102[player_idx]].name);

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 6:
                        {

                            // DNE or default or increment position?
                            STU_DEBUG_BREAK();

                        } break;

                        case 7:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            strcat(G_DIPL_TempMessage, _players[_players[HUMAN_PLAYER_IDX].Dipl.IDK_MoO1_attack_bounty[player_idx]].name);

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 8:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            itoa(_players[HUMAN_PLAYER_IDX].Dipl.field_114[player_idx], string, 10);

                            strcat(G_DIPL_TempMessage, string);

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 9:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            _fstrcpy(string, spell_data_table[_players[HUMAN_PLAYER_IDX].Dipl.UU_MoO1_field_9C[player_idx]].name);

                            strcat(G_DIPL_TempMessage, string);

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 10:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            itoa(_players[HUMAN_PLAYER_IDX].Dipl.field_114[player_idx], string, 10);

                            strcat(G_DIPL_TempMessage, string);

                            strcat(G_DIPL_TempMessage, aGold_2);

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 11:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            if(_players[HUMAN_PLAYER_IDX].Dipl.Broken_Treaty[player_idx] == 2)
                            {

                                strcat(G_DIPL_TempMessage, aAlliance_0);

                            }
                            else if(_players[HUMAN_PLAYER_IDX].Dipl.Broken_Treaty[player_idx] == 1)
                            {

                                strcat(G_DIPL_TempMessage, aWizardPact_0);
                                
                            }
                            else
                            {

                                strcat(G_DIPL_TempMessage, aTreaty);

                            }

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 12:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            _fstrcpy(string, spell_data_table[_players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[player_idx]].name);

                            strcat(G_DIPL_TempMessage, string);

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 13:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            strcat(G_DIPL_TempMessage, _unit_type_table[_players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[player_idx]].name);

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 14:
                        {

                            G_DIPL_TempMessage[pos] = 0;
    
                            strcat(G_DIPL_TempMessage, An(_unit_type_table[_players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[player_idx]].name));

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 15:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[player_idx] == 2)
                            {

                                strcat(G_DIPL_TempMessage, aAlliance_0);

                            }
                            else if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[player_idx] == 1)
                            {

                                strcat(G_DIPL_TempMessage, aWizardPact_0);
                                
                            }
                            else
                            {

                                strcat(G_DIPL_TempMessage, aTreaty);

                            }

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 16:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            itoa((1400 + (_turn / 12)), string, 10);

                            strcat(G_DIPL_TempMessage, string);

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 17:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            _fstrcpy(string, spell_data_table[Spell_Index].name);

                            strcat(G_DIPL_TempMessage, string);

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 18:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            strcat(G_DIPL_TempMessage, _players[Target_Player].name);

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 19:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            strcat(G_DIPL_TempMessage, _players[word_42E86].name);

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        case 20:
                        {

                            G_DIPL_TempMessage[pos] = 0;

                            itoa(word_42E66, string, 10);

                            strcat(G_DIPL_TempMessage, string);

                            pos = strlen(G_DIPL_TempMessage);

                        } break;

                        default:
                        {

                            STU_DEBUG_BREAK();

                        } break;

                    }

                }

            }

        }

    }

    G_DIPL_TempMessage[pos] = 0;

    strcpy(G_DIPL_ComposedMessage, G_DIPL_TempMessage);

    sub_6FD5C(G_DiploMsg_E0_Field0);

    word_42E80 = G_DiploMsg_E0_Field2;

}



/*
    WIZARDS.EXE  ovr087
*/

// WZD o87p01
// DIPL_ContactProgress()

// WZD o87p02
// IDK_Dipl_s7373B()

// WZD o87p03
// G_DIPL_NeedForWar()

// WZD o87p04
// G_DIPL_SuperiorityWar()

// WZD o87p05
// IDK_Dipl_s73F1C()

// WZD o87p06
// IDK_Dipl_s73FBF()

// WZD o87p07
// DIPL_GetOffMyLawn()

// WZD o87p08
// drake178: WIZ_DecreasePeaceCs()
/*
; decreases the peace counters for all wizards towards
; all others
*/
/*

¿ "Once a peace treaty is in place, the opposing wizard will not attack you for about the next dozen turns." ?

*/
void Decrease_Peace_Duration(void)
{
    int16_t itr_players1;  // _CX_
    int16_t itr_players2;  // _SI_

    for(itr_players1 = 0; itr_players1 < _num_players; itr_players1++)
    {

        for(itr_players2 = 0; itr_players2 < _num_players; itr_players2++)
        {

            _players[itr_players1].peace_duration[itr_players2] -= 1;

            if(_players[itr_players1].peace_duration[itr_players2] != 0)
            {

                _players[itr_players1].peace_duration[itr_players2] = 0;

            }

        }

    }

}


// WZD o87p09
// IDK_Dipl_s7436F()




/*
    WIZARDS.EXE  ovr088
*/

// WZD o88p0

// sub_74420()
// DIPL_HumanWarOrPeace()
// IDK_Dipl_s74B68()
// sub_74E2F()
// sub_74E38()
// sub_74F4A()
