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

#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"
#include "../../STU/src/STU_GD_MoM.h"  /* STU_GD_ONCE(), STU_GD_DO(), STU_GD_Dump_*() */  /* CLAUDE */

#include "../../MoX/src/MOM_DEF.h"

#include "../../MoX/src/Allocate.h"
#include "../../MoX/src/FLIC_Draw.h"
#include "../../MoX/src/Fonts.h"
#include "../../MoX/src/Graphics.h"
#include "../../MoX/src/Timer.h"
#include "../../MoX/src/paragrph.h"
#include "../../MoX/src/random.h"
#include "../../MoX/src/Fields.h"
#include "../../MoX/src/Input.h"
#include "../../MoX/src/LBX_Load.h"
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_BASE.h"
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_SET.h"
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/SOUND.h"
#include "../../MoX/src/special.h"

#include "AIMOVE.h"
#include "City_ovr55.h"
#include "Combat.h"
#include "DIPLODEF.h"
#include "Lair.h"
#include "LOADER.h"
#include "MainScr.h"
#include "MOM_SCR.h"
#include "NEXTTURN.h"
#include "Spellbook.h"
#include "Spells128.h"
#include "UNITTYPE.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "../../ext/stu_compat.h"

#include "DIPLOMAC.h"






/*
    WIZARDS.EXE  ovr084
*/
// WZD o84p01
void Diplomacy_Screen__WIP(void);
// WZD o84p02
static void Diplomacy_Screen_Draw__WIP(void);
// WZD o84p03
int16_t Diplomacy_Add_Incentive_To_Message(int16_t spell_idx, int16_t gold);
// WZD o84p04
// sub_6ED5D()
// WZD o84p05
static void Get_Main_Diplomacy_Choices(void);
// WZD o84p06
static void Diplomacy_Propose_Treaty__WIP(void);
// WZD o84p07
static int16_t Diplomacy_Choose_Other_Wizard(int16_t break_alliance);
// WZD o84p08
static void Diplomacy_Display_Response(int16_t diplomatic_order, int16_t IDK2);
// WZD o84p09
static int16_t Diplomacy_Ask_For_Sweetener(int16_t diplomacy_test_result);
// WZD o84p10
static void Diplomacy_Display_Response_Draw(void);
// WZD o84p11
static void Diplomacy_Break_Treaty(void);
// WZD o84p12
static void Start_Diplomacy_Music(int16_t IDK);
// WZD o84p13
static void Diplomacy_Screen_Draw_Portrait_Mouth_Animation(void);
// WZD o84p14
static void Diplomacy_Screen_Load__WIP(void);
// WZD o84p15
static void DIPL_LoadTalkGFX(void);
// WZD o84p16
static void Diplomacy_Screen_Draw_Gargoyle_Eyes(void);
// WZD o84p17
static void Diplomacy_Screen_Fade_In(void);
// WZD o84p18
// sub_7038D()
// WZD o84p19
static int16_t Invader_Army_Strength_Comparison(int16_t player1, int16_t player2, int32_t * value1, int32_t * value2);
/*
    WIZARDS.EXE  ovr085
*/
// WZD o85p01
static void Diplomacy_Greeting(void);
// WZD o85p02
void Diplomacy_Growth(void);
// WZD o85p03
static int16_t Npc_Proposal__WIP(void);
// WZD o85p04
static int16_t Diplomacy_Test(int16_t type_modifier, int16_t type);
// WZD o85p05
static int16_t Find_Worst_Modifier(void);
// WZD o85p06
static void Limit_Treaty_Modifiers(void);
// WZD o85p07
static void Diplomacy_Player_Gets_Spell(int16_t player_idx, int16_t spell_idx);
// WZD o85p08
void G_DIPL_PickSides(void);
// WZD o85p09
static void Start_Treaty(int16_t player1, int16_t player2, int16_t type);
// WZD o85p10
static void Declare_Peace(int16_t player1, int16_t player2);
// WZD o85p11
static void Adjust_Diplomat_Modifiers(int16_t player1, int16_t player2);
// WZD o85p12
void Change_Relations(int16_t value, int16_t attacker_idx, int16_t defender_idx, int16_t type, int16_t city_idx, int16_t spell_idx);
// WZD o85p13
void Declare_War(int16_t attacker_idx, int16_t defender_idx);
// WZD o85p14
void Break_Treaties(int16_t attacker_idx, int16_t defender_idx);
// WZD o85p15
static int16_t Get_Exchange_Spell_List(int16_t player1, int16_t player2, int16_t min_value);
// WZD o85p16
static void Diplomacy_Offer_Tribute(void);
// WZD o85p17
static void Diplomacy_Exchange_Spell__WIP(void);
// WZD o85p18
static void Cancel_Players_City_Enchantments(int16_t player1, int16_t player2);
/*
    WIZARDS.EXE  ovr086
*/
// WZD o86p01
static void Npc_Diplomacy_Screen(void);
// WZD o86p02
// void NPC_To_NPC_Treaty_Negotiations(void);
int16_t NPC_To_NPC_Treaty_Negotiations(void);
// WZD o86p03
static int16_t DIPL_sub_72DB6__STUB(void);
// DIPL_sub_72DB6()
// WZD o86p04
// DIPL_sub_72DB6_Draw()
// WZD o86p05
static void Get_Diplomacy_Statement(int16_t IDK, int16_t player_idx);
/*
    WIZARDS.EXE  ovr087
*/
// WZD o87p01
void Determine_First_Contacts(void);
// WZD o87p02
void NPC_To_Human_Diplomacy(void);
// WZD o87p03
void G_DIPL_NeedForWar(int16_t Player_1, int16_t Player_2);
// WZD o87p04
void G_DIPL_SuperiorityWar(int16_t Player_1, int16_t Player_2);
// WZD o87p05
void NPC_Required_Alliances(int16_t target_player_idx);
// WZD o87p06
void Resolve_Delayed_Diplomacy_Orders(void);
// WZD o87p07
void DIPL_GetOffMyLawn(int16_t player1, int16_t player2);
// WZD o87p08
void Decrease_Peace_Duration(void);
// WZD o87p09
void End_Of_Turn_Diplomacy_Adjustments(void);
/*
    WIZARDS.EXE  ovr088
*/
// WZD o88p01
void NPC_Diplo_s74420(int16_t player1_idx, int16_t player2_idx);
// WZD o88p02
void DIPL_HumanWarOrPeace(int16_t player_idx);
// WZD o88p03
void Modifier_Diplomacy_Adjustments(void);
// WZD o88p04
int16_t s88p04_empty_function(int16_t player);
// WZD o88p05
int16_t Army_Strength_Random_Difference__NIU(int16_t player1_idx, int16_t player2_idx);
// WZD o88p06
int16_t Population_Random_Difference__NIU(int16_t player1_idx, int16_t player2_idx);





// WZD dseg:4C44                                                 BEGIN: ovr084 - Initialized Data

// WZD dseg:4C44
int16_t AI_Dipl_Unset_0;

// WZD dseg:4C46
/*
count of players for 'Propose Treaty'
player_idx is in G_Some_DIPL_Alloc_4[]
*/
int16_t m_other_wizard_count_declare_war = 0;

// WZD dseg:4C70
// LBuff@ db '[ Agree    ',0
char LBuff[] = "[ Agree    \0[ Forget It\0";  /* dseg:4C70 / 4C7C, 12 bytes each, + the empty entry at dseg:4C88 that ends the list */
char * LBuff_list[1] = { LBuff };  /* Get_List_Field() walks text_string[0] in string_width (12) steps */
// WZD dseg:4C7C
// aForgetIt_2 db '[ Forget It',0
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

// WZD dseg:4CA6
char aProposeTreaty[] = "[ Propose Treaty";
// WZD dseg:4CB6
char str_empty_string__ovr084[] = "";
// WZD dseg:4CB7
char aThreatenBreakT[] = "[ Threaten/Break Treaty";
// WZD dseg:4CCF
char aOfferTribute[] = "[ Offer Tribute";
// WZD dseg:4CDF
char aExchangeSpells[] = "[ Exchange Spells";
// WZD dseg:4CF1
char aGoodBye[] = "[ Good Bye";
// WZD dseg:4CFC
char aWizardPact[] = "[ Wizard Pact";
// WZD dseg:4D0A
char aAlliance[] = "[ Alliance";
// WZD dseg:4D15
char aPeaceTreaty[] = "[ Peace Treaty";
// WZD dseg:4D24
char aDeclarationOfW[] = "[ Declaration of War on Another Wizard";
// WZD dseg:4D4B
char aBreakAllianceW[] = "[ Break Alliance With Another Wizard";
// WZD dseg:4D70
char aForgetIt[] = "[ Forget It";
// WZD dseg:4D7C
char aBreakWizardPac[] = "[ Break Wizard Pact";
// WZD dseg:4D90
char aBreakAlliance[] = "[ Break Alliance";
// WZD dseg:4DA1
char aThreatenToAtta[] = "[ Threaten To Attack";

// NOTE:  OUT OF ORDER!!
// WZD dseg:4C48
char * IDK_Diplo_msg[19] = {
    aProposeTreaty,
    aThreatenBreakT,
    aOfferTribute,
    aExchangeSpells,
    aGoodBye,
    str_empty_string__ovr084,
    str_empty_string__ovr084,
    aWizardPact,
    aAlliance,
    aPeaceTreaty,
    aDeclarationOfW,
    aBreakAllianceW,
    aForgetIt,
    str_empty_string__ovr084,
    aBreakWizardPac,
    aBreakAlliance,
    aThreatenToAtta,
    aForgetIt,
    str_empty_string__ovr084
};
// WZD dseg:4C6E 00                                              db    0
// WZD dseg:4C6F 00                                              db    0

// WZD dseg:4DB6
char aHowMayIServeYo[] = "How may I serve you:";
// WZD dseg:4DCB
char aYouProposeATre[] = "You propose a treaty: ";
// WZD dseg:4DE2
char aYourActions[] = "Your actions: ";
// NOTE:  OUT OF ORDER!!
// WZD dseg:4CA0
// WZD dseg:4CA2
// WZD dseg:4CA4
char * off_3B740 = aHowMayIServeYo;  // "How may I serve you:"
char * off_3B742 = aYouProposeATre;  // "You propose a treaty: "
char * off_3B744 = aYourActions;  // "Your actions: "

// WZD dseg:4DF1 20 73 70 65 6C 6C 00                            aSpell db ' spell',0
char aSpell[] = " spell";
// WZD dseg:4DF8 20 67 6F 6C 64 00                               aGold db ' gold',0
char aGold[] = " gold";
// WZD dseg:4DFE 5B 20 00                                        asc_3B89E db '[ ',0
char asc_3B89E[] = "[ ";
// WZD dseg:4E01 57 68 6F 20 73 68 6F 75 6C 64 20 49 20 64 65 63+aWhoShouldIDecl db 'Who should I declare war on?',0
char aWhoShouldIDecl[] = "Who should I declare war on?";
// WZD dseg:4E1E 57 68 6F 20 73 68 6F 75 6C 64 20 49 20 62 72 65+aWhoShouldIBrea db 'Who should I break my treaty with?',0
char aWhoShouldIBrea[] = "Who should I break my treaty with?";
// WZD dseg:4E41 50 65 72 68 61 70 73 20 69 66 20 79 6F 75 20 77+aPerhapsIfYouWe db 'Perhaps if you were to throw in ',0
char aPerhapsIfYouWe[] = "Perhaps if you were to throw in ";
// WZD dseg:4E62 20 77 65 20 63 6F 75 6C 64 20 64 65 61 6C 2E 00 aWeCouldDeal_ db ' we could deal.',0
char aWeCouldDeal_[] = " we could deal.";
// WZD dseg:4E72 49 66 20 79 6F 75 20 61 6C 73 6F 20 6F 66 66 65+aIfYouAlsoOffer db 'If you also offer me ',0
char aIfYouAlsoOffer[] = "If you also offer me ";
// WZD dseg:4E88 20 49 20 77 6F 75 6C 64 20 61 63 63 65 70 74 2E+aIWouldAccept_ db ' I would accept.',0
char aIWouldAccept_[] = " I would accept.";

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


// NOTE: OUT OF ORDER!!
// WZD dseg:4EC0
char aWhatDoYouOffer[] = "What do you offer as tribute?";
// WZD dseg:4EDE
char aWhatTypeOfSpel[] = "What type of spell interests you?";
// WZD dseg:4F00
char aWhatWillYouTra[] = "What will you trade for it?";
// WZD dseg:4EBA
// WZD dseg:4EBC
// WZD dseg:4EBE
char * off_3B95A = &aWhatDoYouOffer[0];
char * off_3B95C = &aWhatTypeOfSpel[0];
char * off_3B95E = &aWhatWillYouTra[0];

// WZD dseg:4F1C
char lstr_gold[] = "[ ";
// WZD dseg:4F1F
char aGold_0[] = " gold";
// WZD dseg:4F25
char aSpells[] = "[ Spells";
// WZD dseg:4F2E
char aForgetIt_0[] = "[ Forget It";
// WZD dseg:4F3A
char str_empty_string__ovr086[] = "";

// WZD dseg:4F3B
char asc_3B9DB[] = "[ ";
// WZD dseg:4F3E
char aAccept_0[] = "Accept";
// WZD dseg:4F45
char aReject_0[] = "Reject";
// WZD dseg:4F4C
char aAgree[] = "Agree";
// WZD dseg:4F52
char aForgetIt_1[] = "Forget It";
// WZD dseg:4F5C
char aInExchangeYouWillRe[] = "In exchange you will receive:";
// WZD dseg:4F7A
char aWhatIfWeWereToAlsoO[] = "What if we were to also offer ";
// WZD dseg:4F99
char aPerhapsYouWouldReco[] = "Perhaps you would reconsider if we also provided ";
// WZD dseg:4FCB
char aAsAnIncentive[] = " as an incentive";
// WZD dseg:4FDC
char cnst_QuestionMark4[] = "?";
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

// WZD dseg:C308
int16_t m_other_wizard_count_break_alliance;

// WZD dseg:C30A
/*
list of player_idx for 'Propose Treaty' 'Declare Ware'
*/
byte_ptr m_other_wizard_list_break_alliance;
// WZD dseg:C30C
/*
player_idx list of players for 'Propose Treaty'
count is in word_3B6E6
allocated in DIPL_LoadTalkGFX(); 10 2-byte values
*/
byte_ptr m_other_wizard_list_declare_war;
// WZD dseg:C30E
int16_t m_diplomacy_test_value;
// WZD dseg:C310 00                                              db    0
// WZD dseg:C311 00                                              db    0
// WZD dseg:C312
byte_ptr G_Some_DIPL_Allocs_6[6];
// WZD dseg:C31E
byte_ptr G_Some_DIPL_Alloc_3;
// WZD dseg:C320
uint8_t IDK_DIPLO_NIU[100];
// WZD dseg:C384
uint8_t byte_42E24[10];
// WZD dseg:C38E
uint8_t byte_42E2E[10];
// WZD dseg:C398
uint8_t byte_42E38;
// WZD dseg:C399
/*
1-byte, unsigned
*/
uint8_t byte_42E39[10];
// WZD dseg:C3A3
/*
1-byte, unsigned
*/
uint8_t byte_42E43[10];
// WZD dseg:C3AD
uint8_t byte_42E4D;

/*
    MoO2
        Module: DIP-SCRN
            data (0 bytes) _target_tech_list
                Address: 02:001922DC
            data (0 bytes) _exchange_tech_list
                Address: 02:001923B0
*/
// WZD dseg:C3AE
// ; BUG: these don't fit into a byte variable!
uint8_t m_exchange_spell_values[10];
// WZD dseg:C3B8
uint8_t m_exchange_spell_list[10];

// WZD dseg:C3C2
// MoO2  Module: DIP-SCRN  _exchange_tech_count
/*
1-byte, unsigned
*/
uint8_t m_exchange_spell_count;


// WZD dseg:C3C3 00                                              align 2

// WZD dseg:C3C4
/*
player_idx for 'Declaration of War on Another Wizard'
*/
int16_t Target_Player;
// WZD dseg:C3C6
int16_t word_42E66;
// WZD dseg:C3C8
int16_t Spell_Index;

// WZD dseg:C3CA
SAMB_ptr * m_diplomac_right_eyes_segs;
// WZD dseg:C3CC
SAMB_ptr * m_diplomac_left_eyes_segs;

// WZD dseg:C3CE 00 00                                           dw 0
// WZD dseg:C3D0 00 00                                           dw 0
// WZD dseg:C3D2
SAMB_ptr m_diplomac_talk_portrait_seg;
// WZD dseg:C3D4
int16_t m_diplomacy_current_music;

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
int16_t m_diplomacy_message_IDK_group;
// WZD dseg:C3E2
char * G_DIPL_ComposedMessage;
// WZD dseg:C3E4
char * m_diplomacy_message;
// WZD dseg:C3E6
/*
player_idx for 'Break Alliance with Another Wizard'
*/
int16_t m_break_treaty_player3;
// WZD dseg:C3E8
int16_t m_IDK_old_diplomatic_order;
// WZD dseg:C3EA
int16_t m_diplomsg_1_record_sub_number;
// WZD dseg:C3EC
/*

¿ ~ "conversational option" ?


IDK_DiplSts_s70570()
    sets 42,43,44

Diplomacy_Screen_Draw__WIP()
    short-circuits if 54 or 1

*/
int16_t m_IDK_diplomatic_order;
// WZD dseg:C3EE
/*
only two usages
Diplomacy_Screen_Load__WIP()
    if not 1, calls Diplomacy_Screen_Fade_In()
Diplomacy_Screen_Draw__WIP()
    if not 1, calls Diplomacy_Screen_Draw_Portrait_Mouth_Animation()
    if 0,1,2, sets font/colors, gets a message, prints the message
Diplomacy_Screen_Draw__WIP()
    initialized to 6
    depending on m_IDK_diplomatic_order, reset to 0,2

*/
int16_t m_IDK_diplomatic_flag;
// WZD dseg:C3F0
/*
array of 7 memory addresses
100 bytes each
*/
char * G_Some_DIPL_Allocs_7[7];
// WZD dseg:C3FE
int16_t m_diplomac_player_idx = 0;
/*
DIPLOMSG record
*/
// WZD dseg:C400
// WZD dseg:C402
// WZD dseg:C404
// SAMB_ptr G_DiploMsg_E0_Field0;
// int16_t G_DiploMsg_E0_Field2;
// int16_t G_DiploMsg_E0_Field4;
struct s_DIPLOMSG_RECORD m_diplomacy_message_record_data = { 0, 0, 0 };

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

    Limit_Treaty_Modifiers();

    m_diplomacy_current_music = ST_UNDEFINED;

    word_42E7E = ST_UNDEFINED;

    m_diplomacy_message_IDK_group = 0;

    Apply_Palette();

    leave_screen = ST_FALSE;

    Assign_Auto_Function(Diplomacy_Screen_Draw__WIP, 3);

    Set_Input_Delay(1);

    Clear_Fields();

    m_IDK_diplomatic_flag = 6;

    /* OGBUG  the pending AI message (Dipl_Action) is read and never cleared here: while one is pending, every audience the human asks for with that wizard shows only that response (flag 6) and no menu; orders 2 and 45..49 set flag 2, which has no case in the input loop below, so leave_screen is never set */
    m_IDK_diplomatic_order = _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[m_diplomac_player_idx];

    _page_flip_effect = pfe_Default;

    if(
        (m_IDK_diplomatic_order == 54)
        ||
        (m_IDK_diplomatic_order == 1)
    )
    {

        return;

    }

    if(m_IDK_diplomatic_order == 0)
    {

        m_IDK_diplomatic_flag = 0;

        Diplomacy_Greeting();

    }

    Diplomacy_Screen_Load__WIP();

    m_diplomsg_1_record_sub_number = ST_UNDEFINED;


    // i.e., not Greeting?
    if(
        (
            (m_IDK_diplomatic_order >= do_IDK_treaty1)
            &&
            (m_IDK_diplomatic_order <= do_IDK_peace)
        )
        ||
        (m_IDK_diplomatic_order == 2)
        ||
        (m_IDK_diplomatic_order == do_IDK_war)
    )
    {

        m_IDK_old_diplomatic_order = m_IDK_diplomatic_order;

        m_IDK_diplomatic_order = do_IDK_greeting_positive;

        m_IDK_diplomatic_flag = 2;

        m_break_treaty_player3 = _players[HUMAN_PLAYER_IDX].Dipl.break_treaty[m_diplomac_player_idx];

    }

    full_screen_field = INVALID_FIELD;

    while(leave_screen == ST_FALSE)
    {

        Mark_Time();

        input_field_idx = Get_Input();

        if(m_IDK_diplomatic_flag == 0)
        {

            if(input_field_idx == ST_UNDEFINED)
            {

                leave_screen = ST_TRUE;

            }

            if(input_field_idx == full_screen_field)
            {

                Get_Main_Diplomacy_Choices();

                leave_screen = ST_TRUE;

            }

        }
        else if(m_IDK_diplomatic_flag == 1)
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
        else if(m_IDK_diplomatic_flag == 6)
        {

            Diplomacy_Display_Response(m_IDK_diplomatic_order, 3);

            leave_screen = ST_TRUE;

        }

        Clear_Fields();

        full_screen_field = INVALID_FIELD;

        if(
            (m_IDK_diplomatic_flag == 0)
            ||
            (m_IDK_diplomatic_flag == 1)
            ||
            (m_IDK_diplomatic_flag == 2)
        )
        {

            full_screen_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_empty_string__ovr084, ST_UNDEFINED);

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

    Check_Research_Spell_Is_Known(ST_TRUE);

    Cache_Graphics_Overland();

    // DOMSDOS  Play_Background_Music();
    Play_Background_Music();

}


// WZD o84p02
void Diplomacy_Screen_Draw__WIP(void)
{

    Set_Page_Off();

    Copy_Back_To_Off();

    if(m_IDK_diplomatic_flag != 1)  /* refuse audience/negative greeting */
    {
        
        Diplomacy_Screen_Draw_Portrait_Mouth_Animation();

    }

    FLIC_Draw(95, 1, diplomacy_mirror_seg);

    Diplomacy_Screen_Draw_Gargoyle_Eyes();

    if(
        (m_IDK_diplomatic_flag == 0)
        ||
        (m_IDK_diplomatic_flag == 1)
        ||
        (m_IDK_diplomatic_flag == 2)
    )
    {

        Set_Font_Style_Shadow_Down(4, 4, ST_NULL, ST_NULL);

        Set_Outline_Color(ST_TRANSPARENT);

        Set_Alias_Color(187);

        Set_Font_LF(1);

        Get_Diplomacy_Statement(m_IDK_diplomatic_order, m_diplomac_player_idx);

        Print_Paragraph(38, 140, 245, m_diplomacy_message, 0);

    }

}


// WZD o84p03
/*
appends the AI's incentive to m_diplomacy_message: '<spell name> spell' if spell_idx is set, else '<gold> gold'
called by DIPL_sub_72DB6() ('What if we were to also offer ...' / 'Perhaps you would reconsider if we also provided ...') with offer_spell / offer_gold
*/
int16_t Diplomacy_Add_Incentive_To_Message(int16_t spell_idx, int16_t gold)
{
    char string[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    /* OGBUG  gold is tested before spell_idx: a spell-only incentive (offer_gold 0, which the 1:4 spell branch of the offer code leaves) appends nothing, so the message reads 'What if we were to also offer ?' */
    if(gold == 0)
    {

        return 0;

    }

    if(spell_idx != spl_NONE)
    {

        stu_strcpy(string, spell_data_table[spell_idx].name);  /* asm: String_Copy_Far(0000:string, name) - segment 0, which String_Copy_Far takes as DS */

        stu_strcat(m_diplomacy_message, string);

        stu_strcat(m_diplomacy_message, aSpell);

    }
    else
    {

        stu_itoa(gold, string, 10);

        stu_strcat(m_diplomacy_message, string);

        stu_strcat(m_diplomacy_message, aGold);

    }

    /* HACK */  return 0;  /* the OG falls off the end with AX = strcat()'s return; the caller ignores it */

}


// WZD o84p04
// TODO  Message	VCR003	Function '_sub_6ED5D_Draw' can be made static		C:\STU\devel\ReMoM\MoM\src\DIPLOMAC.c	811		
static void _sub_6ED5D_Draw(void)
{
    int16_t player2_idx = 0;  // _SI_

    Set_Page_Off();

    Copy_Back_To_Off();

    Diplomacy_Screen_Draw_Portrait_Mouth_Animation();

    Diplomacy_Screen_Draw_Gargoyle_Eyes();

    FLIC_Draw(95, 1, diplomacy_mirror_seg);

    Set_Font_Style_Shadow_Down(4, 4, ST_NULL, ST_NULL);

    Set_Outline_Color(ST_TRANSPARENT);

    Set_Alias_Color(187);

    Set_Font_LF(1);

    player2_idx = Get_Paragraph_Max_Height(245, m_diplomacy_message);

    if(player2_idx > 39)
    {

        Set_Font_LF(0);

        player2_idx = Get_Paragraph_Max_Height(245, m_diplomacy_message);

    }

    Print_Paragraph(38, 138, 245, m_diplomacy_message, 0);


}


// WZD o84p05
// MoO2  Module: DIP-SCRN  Get_Main_Diplomacy_Choices_();  Diplomacy_Set_Main_Options_();
/*

XREF:
    Diplomacy_Screen__WIP()
    j_IDK_Diplo_Scrn()

*/
static void Get_Main_Diplomacy_Choices(void)
{
    int16_t list_item_active_flags[DIPLO_MAIN_LIST_COUNT] = { 0, 0, 0, 0, 0, 0 };
    int16_t var_14 = 0;
    int16_t var_12 = 0;
    int16_t var_10 = 0;
    int16_t _variable = 0;
    int16_t leave_screen = 0;  // _DI_
    int16_t itr = 0;  // _SI_

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {

        _variable = 0;

        for(itr = 0; itr < DIPLO_MAIN_LIST_COUNT; itr++)
        {

            list_item_active_flags[itr] = ST_TRUE;

        }

        if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[m_diplomac_player_idx] == DIPL_War)
        {

            list_item_active_flags[1] = ST_FALSE;  // deactive "Threaten/Break Treaty"

            list_item_active_flags[3] = ST_FALSE;  // deactive "Exchange Spells"

        }

        Get_Exchange_Spell_List(_current_player_idx, m_diplomac_player_idx, 0);

        if(
            (_players[HUMAN_PLAYER_IDX].gold_reserve < 25)
            &&
            (m_exchange_spell_count == 0)
        )
        {

            list_item_active_flags[2] = ST_FALSE;  // deactive "Offer Tribute"

        }

        /* OGBUG  ends the audience ('tired of talks', 71) on the lowest interest alone (< -100), before the first menu; Diplomacy_Greeting() refuses only when Hidden_Rel + lowest interest + personality mod <= -100, so with a positive hidden relation or a friendly personality the wizard greets normally and then at once says he is out of time */
        if(Find_Worst_Modifier() < -100)
        {

            // MoO2
            // if(_response_message != _old_response_message)
            //     Diplomacy_Display_Response_();
            // _response_message = 130;
            // leave_screen = ST_TRUE

            Diplomacy_Display_Response(do_IDK_exchange_spell_goodbye, 3);  // IDK2 == 3 means some spell specific, 25% chance something

            leave_screen = ST_TRUE;

        }
        else
        {

            Assign_Auto_Function(_sub_6ED5D_Draw, 3);

            Set_Input_Delay(1);

            stu_strcpy(m_diplomacy_message, off_3B740);

            for(itr = 0; itr < 7; itr++)
            {

                stu_strcpy(G_Some_DIPL_Allocs_7[itr], IDK_Diplo_msg[itr]);

            }

            Set_Font_Style(1, 4, 3, ST_NULL);

            Set_Alias_Color(187);

            _variable = Get_List_Field(38, 142, 245, str_empty_string__ovr084, G_Some_DIPL_Allocs_7, 100, &_variable, &list_item_active_flags[0], 15, 11, 0, 0, 0, ST_UNDEFINED);

            switch(_variable)
            {

                case -1:
                case 4:
                {

                    leave_screen = ST_TRUE;

                } break;

                case 0:
                {

                    Diplomacy_Propose_Treaty__WIP();

                } break;
                
                case 1:
                {

                    Diplomacy_Break_Treaty();

                } break;
                
                case 2:
                {

                    Diplomacy_Offer_Tribute();

                    if(m_IDK_diplomatic_order == do_IDK_exchange_spell_goodbye)
                    {

                        leave_screen = ST_TRUE;

                    }

                } break;
                
                case 3:
                {

                    Diplomacy_Exchange_Spell__WIP();

                } break;

            }
                
        }

    }

    Assign_Auto_Function(Diplomacy_Screen_Draw__WIP, 3);

    Set_Input_Delay(1);

}


// WZD o84p06
/*

*/
static void Diplomacy_Propose_Treaty__WIP(void)
{
    int16_t active_flag[6] = {0, 0, 0, 0, 0, 0 };
    int16_t var_E = 0;
    int16_t var_C = 0;
    int16_t var_A = 0;
    int16_t var_8 = 0;
    int16_t var_6 = 0;
    int16_t diplomacy_message_id = 0;
    int16_t _variable = 0;
    int16_t itr = 0;  // _DI_
    int16_t diplomacy_test_result;

    _variable = 0;

    for(itr = 0; itr < 6; itr++)
    {

        active_flag[itr] = ST_TRUE;

    }

    stu_strcpy(m_diplomacy_message, off_3B742);

    if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[m_diplomac_player_idx] != DIPL_NoTreaty)
    {

        active_flag[0] = ST_FALSE;  // "Wizard Pact"

    }

    if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[m_diplomac_player_idx] >= DIPL_Alliance)
    {

        active_flag[1] = ST_FALSE;  // "Alliance"

    }

    if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[m_diplomac_player_idx] < DIPL_War)
    {

        active_flag[2] = ST_FALSE;  // "Peace Treaty"

    }

    if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[m_diplomac_player_idx] >= DIPL_War)
    {

        active_flag[3] = ST_FALSE;  // "Declaration of War on Another Wizard"

    }

    m_other_wizard_count_declare_war = 0;

    for(itr = 1; itr < _num_players; itr++)
    {

        if(
            (_players[m_diplomac_player_idx].Dipl.Contacted[itr] == 1)
            &&
            (_players[HUMAN_PLAYER_IDX].Dipl.Contacted[itr] == 1)
            &&
            (_players[m_diplomac_player_idx].Dipl.Dipl_Status[itr] < DIPL_War)
            &&
            (itr != m_diplomac_player_idx)
        )
        {

            SET_2B_OFS(m_other_wizard_list_declare_war, (m_other_wizard_count_declare_war * 2), itr);

            m_other_wizard_count_declare_war++;

        }

    }

    if(m_other_wizard_count_declare_war == 0)
    {

        active_flag[3] = ST_FALSE;  // "Declaration of War on Another Wizard"

    }

    m_other_wizard_count_break_alliance = 0;

    for(itr = 1; itr < _num_players; itr++)
    {

        /* OGBUG  the Break Alliance list needs only the partner to have met the wizard - unlike the war list above, there is no test that the human has met it */
        if(
            (_players[m_diplomac_player_idx].Dipl.Contacted[itr] == 1)
            &&
            (_players[m_diplomac_player_idx].Dipl.Dipl_Status[itr] == DIPL_Alliance)
            &&
            (itr != m_diplomac_player_idx)
        )
        {

            SET_2B_OFS(m_other_wizard_list_break_alliance, (m_other_wizard_count_break_alliance * 2), itr);

            m_other_wizard_count_break_alliance++;

        }

    }

    if(m_other_wizard_count_break_alliance == 0)
    {

        active_flag[4] = ST_FALSE;  // "Break Alliance With Another Wizard"

    }

    for(itr = 0; itr < 6; itr++)
    {

        stu_strcpy(G_Some_DIPL_Allocs_7[itr], IDK_Diplo_msg[(7 + itr)]);

    }

    Set_Font_Style(1, 4, 3, ST_NULL);

    Set_Alias_Color(187);

    Set_Font_LF(2);

    _variable = Get_List_Field(38, 142, 245, str_empty_string__ovr084, G_Some_DIPL_Allocs_7, 100, &_variable, &active_flag[0], 15, 11, 0, 0, 0, ST_UNDEFINED);

    switch(_variable)
    {

        case 0:  /* Wizard Pact */
        {

            if(_players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] > 10)
            {

                diplomacy_test_result = Diplomacy_Test(75, 0);

            }
            else
            {

                diplomacy_test_result = 0;

            }

            if(
                (diplomacy_test_result == 1)
                ||
                (diplomacy_test_result == 2)
            )
            {

                diplomacy_test_result = Diplomacy_Ask_For_Sweetener(diplomacy_test_result);  /* CLAUDE */  /* asm: push diplomacy_test_result / call IDK_Npc_Counteroffer__STUB / mov diplomacy_test_result, ax - replaces the non-asm Random(2) placeholder */

            }

            if(diplomacy_test_result == 3)
            {

                Start_Treaty(HUMAN_PLAYER_IDX, m_diplomac_player_idx, 1);

            }

            diplomacy_message_id = 51;

        } break;

        case 1:  /* Alliance */
        {

            if(_players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] > 50)
            {

                diplomacy_test_result = Diplomacy_Test(125, 0);

            }
            else
            {

                diplomacy_test_result = 0;

            }

            if(
                (diplomacy_test_result == 1)
                ||
                (diplomacy_test_result == 2)
            )
            {

                diplomacy_test_result = Diplomacy_Ask_For_Sweetener(diplomacy_test_result);  /* CLAUDE */  /* asm: push diplomacy_test_result / call IDK_Npc_Counteroffer__STUB / mov diplomacy_test_result, ax - replaces the non-asm Random(2) placeholder */

            }

            if(diplomacy_test_result == 3)
            {

                Start_Treaty(HUMAN_PLAYER_IDX, m_diplomac_player_idx, 2);

            }

            diplomacy_message_id = 52;

        } break;

        case 2:  /* Peace Treaty */
        {

            /* OGBUG  the human's peace proposal is scored on the TREATY interest: type 0 (treaty_modifier); Diplomacy_Test's peace type (2) is not used */
            diplomacy_test_result = Diplomacy_Test(60, 0);

            if(
                (diplomacy_test_result == 1)
                ||
                (diplomacy_test_result == 2)
            )
            {

                diplomacy_test_result = Diplomacy_Ask_For_Sweetener(diplomacy_test_result);  /* CLAUDE */  /* asm: push diplomacy_test_result / call IDK_Npc_Counteroffer__STUB / mov diplomacy_test_result, ax - replaces the non-asm Random(2) placeholder */

            }

            if(diplomacy_test_result == 3)
            {

                Declare_Peace(HUMAN_PLAYER_IDX, m_diplomac_player_idx);

            }

            Adjust_Diplomat_Modifiers(HUMAN_PLAYER_IDX, m_diplomac_player_idx);

            Adjust_Diplomat_Modifiers(HUMAN_PLAYER_IDX, m_diplomac_player_idx);

            diplomacy_message_id = 53;

        } break;

        case 3:  /* Declaration of War on Another Wizard */
        {

            // Target_Player = IDK_Npc_Target_Player__STUB(HUMAN_PLAYER_IDX, 0);
            /* CLAUDE */  /* REBUG */  /* asm: xor ax, ax / push ax / call IDK_Npc_Target_Player__STUB / pop cx - one argument, the list selector 0 */
            Target_Player = Diplomacy_Choose_Other_Wizard(ST_FALSE);

            diplomacy_test_result = 2;

            diplomacy_message_id = 56;

            if(Target_Player == HUMAN_PLAYER_IDX)
            {

                _variable = ST_UNDEFINED;

            }
            else
            {

                if(
                    (_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[m_diplomac_player_idx] == DIPL_Alliance)
                    &&
                    (_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[Target_Player] == DIPL_War)
                )
                {

                    if(Random(8) < 5)
                    {

                        diplomacy_test_result = 3;

                    }
                    else
                    {

                        diplomacy_test_result = 2;

                    }

                }
                else
                {

                    /* OGBUG  var_6 = partner-to-target treaty status x 30 + 150 is computed and never read; the difficulty passed is 150 + the partner's relation to the target, so a pact or alliance with the target does not make the request harder */
                    var_6 = (150 + (_players[m_diplomac_player_idx].Dipl.Dipl_Status[Target_Player] * 30));

                    diplomacy_test_result = Diplomacy_Test((150 + _players[m_diplomac_player_idx].Dipl.Visible_Rel[Target_Player]), 0);

                }

                if(
                    (diplomacy_test_result == 1)
                    ||
                    (diplomacy_test_result == 2)
                )
                {

                    diplomacy_test_result = Diplomacy_Ask_For_Sweetener(diplomacy_test_result);  /* CLAUDE */  /* asm: push diplomacy_test_result / call IDK_Npc_Counteroffer__STUB / mov diplomacy_test_result, ax - replaces the non-asm Random(2) placeholder */

                }

                if(diplomacy_test_result == 3)
                {

                    Declare_War(m_diplomac_player_idx, Target_Player);

                }

                diplomacy_message_id = 56;

            }

        } break;

        case 4:  /* Break Alliance with Another Wizard */
        {

            // m_break_treaty_player3 = IDK_Npc_Target_Player__STUB(1, 0);
            /* CLAUDE */  /* REBUG */  /* asm: mov ax, 1 / push ax / call IDK_Npc_Target_Player__STUB / pop cx - one argument, the list selector 1 */
            m_break_treaty_player3 = Diplomacy_Choose_Other_Wizard(ST_TRUE);

            if(m_break_treaty_player3 == HUMAN_PLAYER_IDX)
            {

                _variable = ST_UNDEFINED;

            }
            else
            {

                if(_players[m_diplomac_player_idx].Dipl.Visible_Rel[HUMAN_PLAYER_IDX] > 24)
                {

                    diplomacy_test_result = Diplomacy_Test((175 + _players[m_diplomac_player_idx].Dipl.Visible_Rel[m_break_treaty_player3]), 0);

                }
                else
                {

                    diplomacy_test_result = 2;

                }

                if(
                    (diplomacy_test_result == 1)
                    ||
                    (diplomacy_test_result == 2)
                )
                {

                    diplomacy_test_result = Diplomacy_Ask_For_Sweetener(diplomacy_test_result);  /* CLAUDE */  /* asm: push diplomacy_test_result / call IDK_Npc_Counteroffer__STUB / mov diplomacy_test_result, ax - replaces the non-asm Random(2) placeholder */

                }

                if(diplomacy_test_result == 3)
                {

                    Break_Treaties(m_diplomac_player_idx, m_break_treaty_player3);

                }

                diplomacy_message_id = 55;

            }

        } break;

        case 5:  /* Forget It */
        {

            _variable = ST_UNDEFINED;

        } break;

        default:
        {

            // ¿ DNE ?

        } break;
        
    }

    if(
        (_variable != ST_UNDEFINED)
        &&
        (diplomacy_test_result != 1)
    )
    {

        Diplomacy_Display_Response(diplomacy_message_id, diplomacy_test_result);

    }

}


// WZD o84p07
// sub_6F3E4()
/*
the human names the wizard the diplomacy partner should declare war on (break_alliance == ST_FALSE) or break its alliance with (ST_TRUE)
the choices are m_other_wizard_list_declare_war[] / m_other_wizard_list_break_alliance[], built by Diplomacy_Propose_Treaty__WIP(), then '[ Forget It'

returns  the chosen wizard's player_idx;  0 for Forget It or Esc (both lists start at player 1)
*/
static int16_t Diplomacy_Choose_Other_Wizard(int16_t break_alliance)
{
    int16_t list_choice = 0;
    int16_t other_wizard_count = 0;
    int16_t itr = 0;  // _SI_

    if(break_alliance == ST_FALSE)
    {

        other_wizard_count = m_other_wizard_count_declare_war;

    }
    else
    {

        other_wizard_count = m_other_wizard_count_break_alliance;

    }

    for(itr = 0; itr < other_wizard_count; itr++)
    {

        stu_strcpy(G_Some_DIPL_Allocs_7[itr], asc_3B89E);

        if(break_alliance == ST_FALSE)
        {

            stu_strcat(G_Some_DIPL_Allocs_7[itr], _players[GET_2B_OFS(m_other_wizard_list_declare_war, (itr * 2))].name);

        }
        else
        {

            stu_strcat(G_Some_DIPL_Allocs_7[itr], _players[GET_2B_OFS(m_other_wizard_list_break_alliance, (itr * 2))].name);

        }

    }

    stu_strcpy(G_Some_DIPL_Allocs_7[itr], aForgetIt);

    *G_Some_DIPL_Allocs_7[(itr + 1)] = '\0';

    if(break_alliance == ST_FALSE)
    {

        stu_strcpy(m_diplomacy_message, aWhoShouldIDecl);

    }
    else
    {

        stu_strcpy(m_diplomacy_message, aWhoShouldIBrea);

    }

    /* asm pushes only 1, 4, 3: color3 is the caller's stack (BQ #371); ST_NULL marks it missing */
    Set_Font_Style(1, 4, 3, ST_NULL);

    Set_Alias_Color(187);

    /* DNE  list_choice = 0;  - Diplomacy_Ask_For_Sweetener() stores its list_choice before its Get_List_Field(); here the asm passes lea ax, [bp+_variable] with nothing stored, and Get_List_Field() reads *variable */
    list_choice = Get_List_Field(38, 142, 245, str_empty_string__ovr084, G_Some_DIPL_Allocs_7, 100, &list_choice, ST_NULL, 15, 11, 0, 0, 0, ST_UNDEFINED);

    if(list_choice == ST_UNDEFINED)
    {

        return 0;

    }

    if(list_choice == other_wizard_count)  /* '[ Forget It' */
    {

        return 0;

    }

    if(break_alliance == ST_FALSE)
    {

        return GET_2B_OFS(m_other_wizard_list_declare_war, (list_choice * 2));

    }
    else
    {

        return GET_2B_OFS(m_other_wizard_list_break_alliance, (list_choice * 2));

    }

}


// WZD o84p08
// MoO2  Module: DIP-SCRN  Diplomacy_Display_Response_()
/*

IDK_Diplo_Response(71, 3);
    means Find_Worst_Modifier() < -100 ... impatient, talk later
    maybe, didn't even exchange a spell yet?

IDK2 == 3:
    some spell specific business
    no idea what the 25% chance is for
    field_8A gets used for 'Offer Tribute'

Diplomacy_Propose_Treaty()
    Diplomacy_Display_Response(diplomacy_message_id, diplomacy_test_result);
*/
static void Diplomacy_Display_Response(int16_t diplomatic_order, int16_t IDK2)
{
    int16_t full_screen_field = 0;
    int16_t leave_screen = 0;
    int16_t itr_players = 0;  // _DI_

    itr_players = 0;

    if(diplomatic_order == 57)
    {

        itr_players = 1;

    }

    if(
        (diplomatic_order >= 58)
        &&
        (diplomatic_order <= 66)
    )
    {

        itr_players = 1;

    }

    if(
        (diplomatic_order >= 51)
        &&
        (diplomatic_order <= 56)
    )
    {

        itr_players = 1;

    }

    m_IDK_diplomatic_order = diplomatic_order;

    if(itr_players != 1)
    {
        // So, ... NOT {51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66}

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

                    m_IDK_diplomatic_order = _players[HUMAN_PLAYER_IDX].Dipl.field_84[m_diplomac_player_idx];

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

                        m_IDK_diplomatic_order = 58;

                    }
                    else
                    {

                        m_IDK_diplomatic_order = 57;

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

                    /* spell_idx selected for 'Offer Tribute' - 'Spell' */
                    Spell_Index = _players[HUMAN_PLAYER_IDX].Dipl.field_8A[m_diplomac_player_idx];
                    assert(Spell_Index >= 1 && Spell_Index <= 214);

                    _players[HUMAN_PLAYER_IDX].Dipl.field_8A[m_diplomac_player_idx] = 0;

                    m_IDK_diplomatic_order = do_IDK_treaty1;  // 54: 4 messages specfic to gift spell

                }
                else
                {
                    
                    m_IDK_diplomatic_order = diplomatic_order;

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

        m_IDK_diplomatic_order = diplomatic_order;

    }

    Assign_Auto_Function(Diplomacy_Display_Response_Draw, 3);

    Set_Input_Delay(1);

    m_diplomsg_1_record_sub_number = ST_UNDEFINED;

    Get_Diplomacy_Statement(m_IDK_diplomatic_order, m_diplomac_player_idx);

    leave_screen = ST_FALSE;

    Clear_Fields();

    full_screen_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_empty_string__ovr084, ST_UNDEFINED);

    while(leave_screen == ST_FALSE)
    {

        Mark_Time();

        if(Get_Input() != 0)
        {

            leave_screen = ST_TRUE;

        }

        if(leave_screen == ST_FALSE)
        {

            Diplomacy_Display_Response_Draw();

            PageFlip_FX();

            Release_Time(3);

        }

    }

}


// WZD o84p09
// drake178: N/A  sub_6F6BB()
// ¿ MoO2  Module: DIP-SCRN  Diplomacy_Need_Better_Offer_() ?
/*
AI counter-offer: the AI asks the human to add gold ((Random(8) + Random(8)) x _turn, rounded down to 25s, if the human can pay it) or the first of the human's exchange spells
the human picks Agree / Forget It from a list

returns  0 nothing to ask for;  1 refused (or Esc);  3 accepted (the gold or the spell is handed over)
*/
static int16_t Diplomacy_Ask_For_Sweetener(int16_t diplomacy_test_result)
{
    char spell_name[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char sweetener_text[80] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t message_height = 0;
    int16_t sweetener_is_gold = 0;
    int16_t niu_var_6 = 0;
    int16_t list_choice = 0;
    int16_t spell_idx = 0;
    int16_t has_exchange_spell = 0;  // _DI_
    int16_t gold_amount = 0;  // _SI_

    /* OGBUG  has_exchange_spell is register DI, and the only store is the 0 when there is NO exchange spell: with a spell it keeps the caller's DI, which is
       Diplomacy_Propose_Treaty__WIP()'s itr - its last loop (cmp itr, 6 / jl) leaves it at 6 before all five calls - so it reads as TRUE only by that accident */
    /* HACK */  has_exchange_spell = NUM_PLAYERS;  /* stand-in for the caller's DI (6) */

    niu_var_6 = 0;  /* asm: stored, never read */

    gold_amount = ((((Random(8) + Random(8)) * _turn) / 25) * 25);

    if(gold_amount > _players[HUMAN_PLAYER_IDX].gold_reserve)
    {

        gold_amount = 0;

    }

    Get_Exchange_Spell_List(HUMAN_PLAYER_IDX, m_diplomac_player_idx, _turn);

    if(m_exchange_spell_count > 0)
    {

        spell_idx = m_exchange_spell_list[0];

    }
    else
    {

        has_exchange_spell = ST_FALSE;

    }

    if(
        (gold_amount == 0)
        &&
        (has_exchange_spell == ST_FALSE)
    )
    {

        return 0;

    }

    if(
        (
            (has_exchange_spell != ST_FALSE)
            &&
            (diplomacy_test_result == 1)
        )
        ||
        (gold_amount == 0)
    )
    {

        stu_strcpy(spell_name, spell_data_table[spell_idx].name);  /* asm: String_Copy_Far(0000:spell_name, name) - segment 0, which String_Copy_Far takes as DS */

        stu_strcpy(sweetener_text, spell_name);

        stu_strcat(sweetener_text, aSpell);

        sweetener_is_gold = ST_FALSE;

    }
    else
    {

        stu_itoa(gold_amount, sweetener_text, 10);

        stu_strcat(sweetener_text, aGold);

        sweetener_is_gold = ST_TRUE;

    }

    if(Random(2) == 1)
    {

        stu_strcpy(m_diplomacy_message, aPerhapsIfYouWe);

        stu_strcat(m_diplomacy_message, sweetener_text);

        stu_strcat(m_diplomacy_message, aWeCouldDeal_);

    }
    else
    {

        stu_strcpy(m_diplomacy_message, aIfYouAlsoOffer);

        stu_strcat(m_diplomacy_message, sweetener_text);

        stu_strcat(m_diplomacy_message, aIWouldAccept_);

    }

    Assign_Auto_Function(Diplomacy_Display_Response_Draw, 3);

    Set_Input_Delay(1);

    list_choice = 0;

    /* asm pushes only 4, 4: color2 and color3 are the caller's stack (BQ #371); ST_NULL marks them missing */
    Set_Font_Style(4, 4, ST_NULL, ST_NULL);

    Set_Outline_Color(ST_TRANSPARENT);

    Set_Alias_Color(187);

    Set_Font_LF(1);

    message_height = Get_Paragraph_Max_Height(245, m_diplomacy_message);  /* asm pushes a third word (0) that Get_Paragraph_Max_Height() never reads */

    /* asm pushes 1, 4, 3: color3 is the caller's stack (BQ #371); ST_NULL marks it missing */
    Set_Font_Style(1, 4, 3, ST_NULL);

    Set_Alias_Color(187);

    list_choice = Get_List_Field(38, (message_height + 142), 245, str_empty_string__ovr084, &LBuff_list[0], 12, &list_choice, ST_NULL, 15, 11, 0, 0, 0, ST_UNDEFINED);

    if(
        (list_choice == ST_UNDEFINED)
        ||
        (list_choice == 1)
    )
    {

        return 1;

    }

    if(sweetener_is_gold == ST_FALSE)
    {

        Diplomacy_Player_Gets_Spell(m_diplomac_player_idx, spell_idx);

    }
    else
    {

        _players[HUMAN_PLAYER_IDX].gold_reserve -= gold_amount;

        Player_Add_Gold(m_diplomac_player_idx, gold_amount);

    }

    return 3;

}


// WZD o84p10
/*
vs. Diplomacy_Screen_Draw__WIP()
doesn't check flag for call to IDK_DiplAnim_s6FDA1()
doesn't call get message
otherwise, all the same calls with all the same parameters
*/
static void Diplomacy_Display_Response_Draw(void)
{

    Set_Page_Off();

    Copy_Back_To_Off();

    Diplomacy_Screen_Draw_Portrait_Mouth_Animation();

    Diplomacy_Screen_Draw_Gargoyle_Eyes();

    FLIC_Draw(95, 1, diplomacy_mirror_seg);

    Set_Font_Style_Shadow_Down(4, 4, ST_NULL, ST_NULL);

    Set_Outline_Color(ST_TRANSPARENT);

    Set_Alias_Color(187);

    Set_Font_LF(1);

    Print_Paragraph(38, 140, 245, m_diplomacy_message, 0);

}


// WZD o84p11
/*

*/
static void Diplomacy_Break_Treaty(void)
{
    int16_t active_flag[6] = {0, 0, 0, 0, 0, 0 };
    int16_t var_24 = 0;
    int16_t threat_modifier = 0;
    int32_t value1 = 0;
    int32_t value2 = 0;
    int16_t Amount = 0;
    int16_t relations = 0;
    int16_t diplomacy_test_result;
    int16_t _variable = 0;
    int16_t itr = 0;  // _SI_
    int16_t diplomacy_message_id = 0;  // _DI_

    _variable = 0;

    for(itr = 0; itr < 6; itr++)
    {

        active_flag[itr] = ST_TRUE;

    }

    stu_strcpy(m_diplomacy_message, off_3B744);

    if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[m_diplomac_player_idx] != DIPL_WizardPact)
    {

        active_flag[0] = ST_FALSE;  // "Break Wizard Pact"

    }

    if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[m_diplomac_player_idx] != DIPL_Alliance)
    {

        active_flag[1] = ST_FALSE;  // "Break Alliance"

    }

    // DNE  var_24 = 0;  /* asm: [bp+var_24] is active_flag[1] (-24h = -26h + 2), written only by the Alliance test above */

    for(itr = 0; itr < 5; itr++)
    {

        stu_strcpy(G_Some_DIPL_Allocs_7[itr], IDK_Diplo_msg[(14 + itr)]);

    }

    *G_Some_DIPL_Allocs_7[5] = '\0';

    Set_Font_Style(1, 4, 3, ST_NULL);

    Set_Alias_Color(187);

    // DNE Set_Font_LF(2);

    _variable = Get_List_Field(38, 142, 245, str_empty_string__ovr084, G_Some_DIPL_Allocs_7, 100, &_variable, &active_flag[0], 15, 11, 0, 0, 0, ST_UNDEFINED);

    diplomacy_test_result = 3;

    switch(_variable)
    {

        case 0:  /* Break Wizard Pact */
        case 1:  /* Break Alliance */
        {

            Break_Treaties(HUMAN_PLAYER_IDX, m_diplomac_player_idx);

            diplomacy_message_id = 5;

            _variable = 0;

        } break;

        case 2:  /* Threaten To Attack */
        {

            _variable = 0;

            if(Invader_Army_Strength_Comparison(_current_player_idx, m_diplomac_player_idx, &value1, &value2) != 0)
            {

                if(value1 < value2)  /* invader is stronger */
                {

                    if(value1 > 0)
                    {

                        /* OGBUG  a token force on his continent scores -50 - 100 x (his army / yours), far worse than having no units at all (-100); the 32-bit quotient is cut to 16 bits */
                        threat_modifier = (-50 - ((value2 * 100) / value1));

                    }
                    else
                    {

                        threat_modifier = -100;

                    }

                }
                else  /* invader is weaker */
                {

                    if(value2 > 0)
                    {

                        /* OGBUG  only the low 16 bits of 100 x (your army / his) are kept: outnumbering him more than ~328:1 wraps negative, so he declares war instead of giving in */
                        threat_modifier = (-50 + ((value1 * 100) / value2));

                    }
                    else
                    {

                        threat_modifier = 100;
                        
                    }

                }

                relations = (_players[HUMAN_PLAYER_IDX].Dipl.Hidden_Rel[m_diplomac_player_idx] + TBL_AI_PRS_IDK_Mod[_players[m_diplomac_player_idx].Personality] + threat_modifier + Random(100) + Random(100));

                _players[HUMAN_PLAYER_IDX].Dipl.DA_Strength[m_diplomac_player_idx] -= Random(5);

                _players[HUMAN_PLAYER_IDX].Dipl.field_126[m_diplomac_player_idx] += 1;

                _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] -= Random(15);

                _players[HUMAN_PLAYER_IDX].Dipl.treaty_modifier[m_diplomac_player_idx] = -120;

                SETMIN(_players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx], -100);

                _players[m_diplomac_player_idx].Dipl.Visible_Rel[HUMAN_PLAYER_IDX] = _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx];

                if(relations < 90)
                {

                    if(relations < 0)
                    {

                        Declare_War(HUMAN_PLAYER_IDX, m_diplomac_player_idx);

                        diplomacy_message_id = 39;

                    }
                    else
                    {

                        diplomacy_message_id = 67;

                    }

                }
                else
                {

                    diplomacy_message_id = 67;

                    if(relations >= 200)
                    {

                        Get_Exchange_Spell_List(m_diplomac_player_idx, HUMAN_PLAYER_IDX, 0);

                        /* OGBUG  no else: a threat scoring >= 200 when he has no spell to give is "ignored" (message 67) instead of falling through to the gold payment */
                        if(m_exchange_spell_count > 0)
                        {

                            _players[HUMAN_PLAYER_IDX].peace_duration[m_diplomac_player_idx] = (uint8_t)(Random(15) + 5);

                            diplomacy_message_id = 70;

                            Spell_Index = m_exchange_spell_list[0];

                            Diplomacy_Player_Gets_Spell(HUMAN_PLAYER_IDX, Spell_Index);

                        }

                    }
                    else
                    {

                        if(relations >= 90)
                        {

                            Amount = (int16_t)(((((int32_t)(Random(8) + 2) * _turn) / 25) * 25));

                            if(_players[m_diplomac_player_idx].gold_reserve < Amount)
                            {

                                if(_players[m_diplomac_player_idx].gold_reserve > 0)
                                {

                                    Amount = (_players[m_diplomac_player_idx].gold_reserve / 4);
                                    
                                }
                                else
                                {

                                    Amount = 0;

                                }

                            }

                            if(Amount != 0)
                            {

                                _players[HUMAN_PLAYER_IDX].peace_duration[m_diplomac_player_idx] = (uint8_t)(Random(15) + 5);

                                /* OGBUG  the reparations are created from nothing: his gold_reserve is only read to cap Amount, it is never reduced */
                                Player_Add_Gold(_current_player_idx, Amount);

                                diplomacy_message_id = 69;

                                word_42E66  = Amount;

                            }

                        }

                    }

                }

            }

        } break;

        default:
        {

            // ¿ DNE ?

        } break;
        
    }

    /* OGBUG  runs on every exit, including Forget It and Esc: leaving the Threaten / Break submenu without doing anything still costs -100 to the treaty, trade and peace interests */
    for(itr = 0; itr < 10; itr++)
    {

        Adjust_Diplomat_Modifiers(HUMAN_PLAYER_IDX, m_diplomac_player_idx);

    }

    if(
        (_variable != ST_UNDEFINED)
        &&
        (diplomacy_test_result != 1)
        &&
        (_variable != 3)
    )
    {

        Diplomacy_Display_Response(diplomacy_message_id, diplomacy_test_result);

    }

}


// WZD o84p12
// MoO2  Module: DIP-SCRN  Start_Diplomacy_Music_()
/*
Maybe, good,bad,neutral music

*/
static void Start_Diplomacy_Music(int16_t diplomacy_music)
{

    if(diplomacy_music != m_diplomacy_current_music)
    {

        if(magic_set.background_music == ST_TRUE)
        {

            if(diplomacy_music == 0)
            {

                Play_Sound(sound_buffer1, sound_buffer1_size);

            }
            else if(diplomacy_music == 1)
            {

                Play_Sound(sound_buffer1, sound_buffer1_size);

            }
            else if(diplomacy_music == 2)
            {

                Play_Sound(sound_buffer2, sound_buffer2_size);

            }

        }

        m_diplomacy_current_music = diplomacy_music;

    }

}

// WZD o84p13
/*
draws m_diplomac_talk_portrait_seg or m_diplomac_mood_portrait_seg
checks and updates m_diplomacy_message_IDK_group

XREF:
    Diplomacy_Screen_Draw__WIP()
    sub_6ED5D()
    _sub_6F90E_Draw()
    j_IDK_DiplAnim_s6FDA1()
        DIPL_sub_72DB6_Draw()
            DIPL_sub_72DB6()
            j_DIPL_sub_72DB6_Draw()

*/
static void Diplomacy_Screen_Draw_Portrait_Mouth_Animation(void)
{
    int16_t var_4 = 0;
    int16_t current_frame_num = 0;
    int16_t draw_x = 0;
    int16_t draw_y = 0;

    draw_x = 107;
    draw_y = 13;

    if(m_diplomacy_message_IDK_group != word_42E7E)
    {

        switch(m_diplomacy_message_IDK_group)
        {

            case 0:
            case 1:
            case 2:
            case 5:
            case 6:
            {

                Set_Animation_Frame(m_diplomac_talk_portrait_seg, 0);

            } break;

            case 3:
            {

                Set_Animation_Frame(m_diplomac_mood_portrait_seg, 0);

            } break;

            case 4:
            {

                Set_Animation_Frame(m_diplomac_mood_portrait_seg, 1);

            } break;

            case 7:
            {

                Set_Animation_Frame(m_diplomac_mood_portrait_seg, 2);

            } break;

            default:
            {

                STU_DEBUG_BREAK();

            } break;

        }

    }

    current_frame_num = FLIC_Get_CurrentFrame(m_diplomac_talk_portrait_seg);

    Set_Page_Off();

    switch(m_diplomacy_message_IDK_group)
    {

        case 0:
        case 1:
        {
            if(current_frame_num < 24)
            {

                Set_Animation_Frame(m_diplomac_talk_portrait_seg, 0);

                for(var_4 = 0; var_4 <= current_frame_num; var_4++)
                {

                    FLIC_Draw(draw_x, draw_y, m_diplomac_talk_portrait_seg);

                }

            }
            else
            {

                Set_Animation_Frame(m_diplomac_mood_portrait_seg, 0);

                FLIC_Draw(draw_x, draw_y, m_diplomac_mood_portrait_seg);

                m_diplomacy_message_IDK_group = 3;

            }

        } break;

        case 2:
        {

            if(current_frame_num < 24)
            {

                Set_Animation_Frame(m_diplomac_talk_portrait_seg, 0);

                for(var_4 = 0; var_4 <= current_frame_num; var_4++)
                {

                    FLIC_Draw(draw_x, draw_y, m_diplomac_talk_portrait_seg);

                }

            }
            else
            {

                m_diplomacy_message_IDK_group = 4;

                Set_Animation_Frame(m_diplomac_mood_portrait_seg, 0);

                FLIC_Draw(draw_x, draw_y, m_diplomac_mood_portrait_seg);

                FLIC_Draw(draw_x, draw_y, m_diplomac_mood_portrait_seg);

            }

        } break;

        case 3:
        {

            Set_Animation_Frame(m_diplomac_mood_portrait_seg, 0);

            FLIC_Draw(draw_x, draw_y, m_diplomac_mood_portrait_seg);

        } break;

        case 4:
        {

            Set_Animation_Frame(m_diplomac_mood_portrait_seg, 0);

            FLIC_Draw(draw_x, draw_y, m_diplomac_mood_portrait_seg);

            FLIC_Draw(draw_x, draw_y, m_diplomac_mood_portrait_seg);

        } break;

        case 5:
        {

            // Do Nothing
            STU_DEBUG_BREAK();
            
        } break;

        case 6:
        {

            // Do Nothing
            STU_DEBUG_BREAK();
            
        } break;

        case 7:
        {

            Set_Animation_Frame(m_diplomac_mood_portrait_seg, 0);

            FLIC_Draw(draw_x, draw_y, m_diplomac_mood_portrait_seg);

        } break;

        default:
        {

            STU_DEBUG_BREAK();

        } break;

    }

    word_42E7E = m_diplomacy_message_IDK_group;

}


// WZD o84p14
static void Diplomacy_Screen_Load__WIP(void)
{
    int16_t var_2 = 0;
    int16_t itr = 0;  // _SI_

    Stop_All_Sounds__STUB();

    Cache_Graphics_Diplomacy();

    Combat_Cache_Write();

    // DOMSDOS  _CITIES = SA_MK_FP0(Allocate_First_Block(World_Data, 714));
    _CITIES = (struct s_CITY *)Allocate_First_Block(World_Data, 714);

    /* OGBUG  the partner's Good / Bad music is loaded into World_Data right after the 714-paragraph _CITIES block - over the map layers (Combat_Cache_Write() saved them; Combat_Cache_Read() restores them when the audience ends); in between, Invader_Army_Strength_Comparison() reads _landmasses and sees music bytes, so the threat / proposal army terms count units by an arbitrary byte, fixed per portrait */
    sound_buffer1 = LBX_Reload_Next(music_lbx_file__ovr084, (MUSIC_Merlin_Good + (_players[m_diplomac_player_idx].wizard_id * 2)), World_Data);
    sound_buffer1_size = lbxload_entry_length;

    sound_buffer2 = LBX_Reload_Next(music_lbx_file__ovr084, (MUSIC_Merlin_Bad + (_players[m_diplomac_player_idx].wizard_id * 2)), World_Data);
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

    Diplomacy_Screen_Draw_Gargoyle_Eyes();

    Clear_Fields();

    Apply_Palette();

    Toggle_Pages();

    CLROFF();

    FLIC_Draw(0, 0, diplomacy_background_seg);

    Diplomacy_Screen_Draw_Gargoyle_Eyes();

    Copy_On_To_Off_Page();

    Copy_Off_To_Back();

    if(m_IDK_diplomatic_flag != 1)
    {

        Diplomacy_Screen_Fade_In();

    }

    DIPL_LoadTalkGFX();

}


// WZD o84p15
static void DIPL_LoadTalkGFX(void)
{
    int16_t niu_relationship_level = 0;
    int16_t DIPLOMAC_Portrait_Index = 0;
    int16_t itr = 0;  // _SI_

    DIPLOMAC_Portrait_Index = (24 + _players[m_diplomac_player_idx].wizard_id);

    niu_relationship_level = ((100 + _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]) / 20);

    m_diplomacy_message = (char * )Near_Allocate_First(220);

    G_DIPL_ComposedMessage = (char * )Near_Allocate_Next(220);

    for(itr = 0; itr < 7; itr++)
    {

        G_Some_DIPL_Allocs_7[itr] = (char *)Near_Allocate_Next(100);

    }

    for(itr = 0; itr < 6; itr++)
    {

        G_Some_DIPL_Allocs_6[itr] = Near_Allocate_Next(50);

    }

    G_Some_DIPL_Alloc_3 = Near_Allocate_Next(20);

    m_other_wizard_list_declare_war = Near_Allocate_Next(20);

    m_other_wizard_list_break_alliance = Near_Allocate_Next(20);

    m_diplomac_left_eyes_segs = (SAMB_ptr * )Near_Allocate_Next((11 * sizeof(SAMB_ptr)));

    m_diplomac_right_eyes_segs = (SAMB_ptr *)Near_Allocate_Next((11 * sizeof(SAMB_ptr)));

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
    m_diplomac_mood_portrait_seg = LBX_Reload(moodwiz_lbx_file__ovr084, _players[m_diplomac_player_idx].wizard_id, _screen_seg);

    // BACKGRND.LBX, 018  "MIRROR"    "diplomacy mirror"
    diplomacy_mirror_seg = LBX_Reload_Next(backgrnd_lbx_file__ovr084, 18, _screen_seg);

    // DIPLOMAC.LBX, 024    "MERLTALK"  "blab"
    // DIPLOMAC.LBX, 025    "SHAMAN"    "blab"
    // DIPLOMAC.LBX, 026    "PRIESTES"  "blab"
    // DIPLOMAC.LBX, 027    "WUTALK"    "blab"
    // DIPLOMAC.LBX, 028    "ARABTALK"  "blab"
    // DIPLOMAC.LBX, 029    "OBERTALK"  "blab"
    // DIPLOMAC.LBX, 030    "WRAITH"    "blab"
    // DIPLOMAC.LBX, 031    "DRACTALK"  "blab"
    // DIPLOMAC.LBX, 032    "NMOITALK"  "blab"
    // DIPLOMAC.LBX, 033    "FREYTALK"  "blab"
    // DIPLOMAC.LBX, 034    "GALETALK"  "blab"
    // DIPLOMAC.LBX, 035    "ARIELTAL"  "blab"
    // DIPLOMAC.LBX, 036    "AZTETALK"  "blab"
    // DIPLOMAC.LBX, 037    "KARLTALK"  "blab"
    m_diplomac_talk_portrait_seg = LBX_Reload_Next(diplomac_lbx_file__ovr084, DIPLOMAC_Portrait_Index, _screen_seg);

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


// WZD o84p16
/*

11 pictures
...must be -100 + 100 / 20 = 0 === "vivid red"
11 * 20 = 220
...100 + 220 = 320 / 20 = 16
...100 + 120 = 220 / 20 = 11  deep green

Page 69  (PDF Page 74)  
The wizard appears flanked by gargoyles.  
The eye color of the gargoyles is a measure of your opponent’s feelings toward you.  
The eye color ranges from deep shades of green to vivid red, with green representing positive relations, yellow representing neutrality and red representing anger and negative feelings.  

Table 25.1 The Diplomatic Relations Scale
Relationship Level (Name)
Diplomacy Point (DP)

*/
static void Diplomacy_Screen_Draw_Gargoyle_Eyes(void)
{
    int16_t relationship_level = 0;

    relationship_level = ((100 + _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]) / 20);

    FLIC_Draw(64, 58, m_diplomac_left_eyes_segs[relationship_level]);

    FLIC_Draw(233, 58, m_diplomac_right_eyes_segs[relationship_level]);

}


// WZD o84p17
// MoO2  Module: DIP-SCRN  Diplomacy_Fade_In_();  Draw_Diplomacy_Fade_In_Screen_();
/*
OON XREF
*/
static void Diplomacy_Screen_Fade_In(void)
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


// WZD o84p18
// sub_7038D()


// WZD o84p19
// ~ MoO2  Module: DIPLOMAC  Fleet_Strength_Comparison_()
// drake178: DIPL_GetInvasionStr()
/*
; sets into the return locations the value (cost) sum
; of the two players' units on the second player's
; fortress continent
*/
/*
accumulates the unit type cost for both players
if they are on player2's fortress landmass  (AKA home continent)

*/
static int16_t Invader_Army_Strength_Comparison(int16_t player1, int16_t player2, int32_t * value1, int32_t * value2)
{
    int16_t plane = 0;
    int16_t landmass = 0;
    int16_t itr = 0;  // _DI_

    *value1 = 0;
    *value2 = 0;

    if(
        (player1 == NEUTRAL_PLAYER_IDX)
        ||
        (player2 == NEUTRAL_PLAYER_IDX)
    )
    {

        return ST_FALSE;

    }

    /* OGBUG  inside a diplomacy audience _landmasses holds the partner's music (see Diplomacy_Screen_Load__WIP()): the fortress 'landmass' and every unit's are music bytes, so value1 / value2 sum the units whose square holds the same byte, not the units on the partner's continent */
    landmass = GET_LANDMASS(_FORTRESSES[player2].wx, _FORTRESSES[player2].wy, _FORTRESSES[player2].wp);

    plane = _FORTRESSES[player2].wp;

    *value1 = 0;
    *value2 = 0;

    for(itr = 0; itr < _units; itr++)
    {

        if(
            (
                (_UNITS[itr].owner_idx == player1)
                ||
                (_UNITS[itr].owner_idx == player2)
            )
            &&
            (_UNITS[itr].wp == plane)
            &&
            (GET_LANDMASS(_UNITS[itr].wx, _UNITS[itr].wy, _UNITS[itr].wp) == landmass)
        )
        {

            if(_UNITS[itr].owner_idx == player1)
            {

                *value1 += _unit_type_table[_UNITS[itr].type].cost;
            }
            else
            {

                *value2 += _unit_type_table[_UNITS[itr].type].cost;

            }

        }

    }

    return ST_TRUE;

}



/*
    WIZARDS.EXE  ovr085
*/

// WZD o85p01
/*
    sets m_IDK_diplomatic_order and m_IDK_diplomatic_flag

¿ computer player's Core Reaction ?
¿ Diplomatic Relations Scale ?
*/
static void Diplomacy_Greeting(void)
{
    int16_t worst_modifier_value = 0;
    int16_t relationship_level = 0;  // _SI_
    int16_t IDK_nonvisible_relationship_level = 0;  // _DX_

    worst_modifier_value = Find_Worst_Modifier();

    /* added to chance of forming treaties; added to chance of avoiding superiority wars */
    relationship_level = (
        _players[HUMAN_PLAYER_IDX].Dipl.Hidden_Rel[m_diplomac_player_idx]
        + _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]
        + worst_modifier_value
        + TBL_AI_PRS_IDK_Mod[_players[m_diplomac_player_idx].Personality]
    );

    /* OSG  back out the "Diplomatic Relations Scale" "position" portion of the "Core Reaction" */
    IDK_nonvisible_relationship_level = (relationship_level - _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]);

    /* OGBUG  refuses the audience only when relationship_level - Visible_Rel (= Hidden_Rel + lowest interest + personality mod) <= -100, while Get_Main_Diplomacy_Choices() ends the talk on the lowest interest < -100 alone - see there */
    if(IDK_nonvisible_relationship_level <= -100)  /* //OSG 50% chance of War */
    {

        // ¿ BUGBUG  shoulda been <= -100 || >= DIPL_War ?
        // looks similarly odd in MoO2
        if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[m_diplomac_player_idx] >= DIPL_War)
        {

            // jmp     short $+2

        }

        m_IDK_diplomatic_order = do_IDK_greeting_negative;  // gretting?

        m_IDK_diplomatic_flag = 1;  // refuse audience?

    }
    else
    {

        if(relationship_level > -50)
        {

            m_IDK_diplomatic_order = do_IDK_greeting_positive;  // gretting? neutral?

        }
        else  /* relationship_level > -100 <= -50 ... //OSG 25% chance of War */
        {

            m_IDK_diplomatic_order = do_IDK_greeting_neutral;  // gretting? negative?

        }

        m_IDK_diplomatic_flag = 0;  // allow audience?

    }

}


// WZD o85p02
// MoO2  Module: DIPLOMAC  Diplomacy_Growth_()
/**
 * @brief Applies periodic diplomacy drift and relation adjustments between wizards.
 *
 * @details
 * This routine performs several end-of-turn diplomacy maintenance passes:
 *
 * 1. Treaty goodwill growth pass:
 *    - For each player pair, with 50% chance, increment relations when they are
 *      in a pact/alliance (larger random delta for alliances).
 * 2. Human military-threat penalty pass:
 *    - If the human has non-zero army strength, each wizard with weaker army
 *      can apply a probabilistic negative relation hit against the human
 *      (type 7 adjustment).
 * 3. Human expansion pressure pass (every even turn):
 *    - If human city count exceeds a land-size-derived threshold, compute a
 *      negative reaction value and apply it (type 14 adjustment) to contacted,
 *      non-allied relationships.
 * 4. Gravitation toward default relations:
 *    - For non-human pairs without active strength pressure
 *      (@c DA_Strength == 0), visible relations drift toward each pair's
 *      default relation value, with periodic decay in the opposite direction.
 * 5. Symmetry and diagonal normalization:
 *    - Enforce @c Visible_Rel[i][i] = 0 and mirror pairwise relation values.
 *
 * @return This function returns no value.
 *
 * @note Uses randomized adjustments via @c Random(); outcomes vary by turn.
 * @note Relation values are clamped to [-100, 100] during gravitation.
 * @note The city-count expansion pass recomputes human city count per outer
 *       iteration, preserving original behavior.
 * @note Human expansion penalties are not applied uniformly: when @c itr1 is
 *       human, the computed reaction is stronger than for non-human sources.
 * @note Final mirroring pass ensures matrix consistency for visible relations.
 *
 * @see Change_Relations(), End_Of_Turn_Diplomacy_Adjustments(), Random()
 */
void Diplomacy_Growth(void)
{
    int16_t too_strong_treshold = 0;
    int16_t human_army_strength = 0;
    int16_t gravitation = 0;
    int16_t human_player_city_count = 0;
    int16_t default_relations = 0;
    int16_t reaction_value = 0;
    int16_t itr1 = 0;
    int16_t itr2 = 0;

    TRACE_ENTER();

    /* CLAUDE: GD point 628 -- _players BEFORE Diplomacy_Growth (the diplomacy-phase
     * pass that updates treaty/peace/exchange modifiers, downstream of the
     * All_Outpost_Population_Growth RNG desync).  Fire once. */
    STU_GD_ONCE("628_Diplomacy_Growth_Entry_P", STU_GD_Dump_Players("628_Diplomacy_Growth_Entry_P"));  /* CLAUDE */

    for(itr1 = 0; itr1 < _num_players; itr1++)
    {
        for(itr2 = (itr1 + 1); itr2 < _num_players; itr2++)
        {
            if(Random(2) == 1)
            {
                if(_players[itr1].Dipl.Dipl_Status[itr2] == DIPL_WizardPact)
                {
                    Change_Relations(Random(3), itr1, itr2, 0, ST_NULL, ST_NULL);
                }
                if(_players[itr1].Dipl.Dipl_Status[itr2] == DIPL_Alliance)
                {
                    Change_Relations(Random(6), itr1, itr2, 0, ST_NULL, ST_NULL);
                }
            }
        }
    }
    /*
        OSG  Page 374  (PDG Page 375)
            Human Threat: Milityary Build Up
                Every turn each computer player separately rolls a d20.
                If she rolls a 1, she compares the size of her military to yours.
                If your total army strength is double hers, you instantly take a -10 DP hit in your relations with her.
                If your forces are equal, there is a 50 percent chance of suffering this -10 DP penalty.
                When you have half her strength, there is only a 25 percent chance of this same negative reaction, etc.
                Thus, the percentage chance of suffering this penalty with a particular wizard
                    is based on a sliding scale depending on the strength ratio of your two armed forces.
    */
    human_army_strength = _players[HUMAN_PLAYER_IDX].astrologer.army_strength;
    if(human_army_strength > 0)
    {
        for(itr1 = 0; itr1 < _num_players; itr1++)
        {
            if(
                (_players[itr1].astrologer.army_strength < human_army_strength)
                &&
                (_players[itr1].astrologer.army_strength > 0)
            )
            {
                /* OGBUG  only a wizard WEAKER than the human can be upset, and the weaker he is the less likely: t = 50 x human / his, hit when Random(100) >= t and Random(20) == 1 - about 2.5% a turn at near-equal armies, 0.05% at 2:1, never beyond - the reverse of the strategy guide's rule */
                too_strong_treshold = (((int32_t)human_army_strength * 50) / _players[itr1].astrologer.army_strength);
                if(
                    (Random(100) >= too_strong_treshold)
                    &&
                    (Random(20) == 1)  /* 5% chance */
                )
                {
                    Change_Relations(-10, HUMAN_PLAYER_IDX, itr1, 7, ST_NULL, ST_NULL);
                }
            }
        }
    }
    if((_turn % 2) == 0)
    {
        for(itr1 = 0; itr1 < _num_players; itr1++)
        {
            human_player_city_count = 0;
            /* OGBUG  counts the HUMAN's cities inside the per-wizard loop: every wizard itr1 (half the penalty for an AI) loses relation with everyone it has met by the human's city count; an AI's own city count never matters */
            for(itr2 = 0; itr2 < _cities; itr2++)
            {
                if(_CITIES[itr2].owner_idx == HUMAN_PLAYER_IDX)
                {
                    human_player_city_count++;
                }
            }
            if(
                (((_landsize + 1) * 3) < human_player_city_count)
                &&
                (Random(4) == 1)
            )
            {
                reaction_value = (((-(human_player_city_count - ((_landsize + 1) * 3)) / ((_landsize + 6) - _difficulty)) * Random(4)) / 3);
                reaction_value *= 4;
                if(itr1 != HUMAN_PLAYER_IDX)
                {
                    reaction_value /= 2;
                }
                SETMIN(reaction_value, -15);
                for(itr2 = 1; itr2 < _num_players; itr2++)
                {
                    if(
                        (_players[itr1].Dipl.Contacted[itr2] == ST_TRUE)
                        &&
                        (_players[itr1].Dipl.Dipl_Status[itr2] != DIPL_Alliance)
                    )
                    {
                        Change_Relations(reaction_value, itr1, itr2, 14, ST_NULL, ST_NULL);
                    }
                }
            }
        }
    }
    for(itr1 = 1; itr1 < _num_players; itr1++)
    {
        for(itr2 = 0; itr2 < _num_players; itr2++)
        {
            if(Random(105) > abs(_players[itr1].Dipl.Visible_Rel[itr2]))
            {
                gravitation = Random(2);
            }
            else
            {
                gravitation = 0;
            }
            if(
                (itr2 != itr1)
                &&
                (_players[itr1].Dipl.DA_Strength[itr2] == 0)
            )
            {
                default_relations = _players[itr1].Dipl.Default_Rel[itr2];
                if(_players[itr1].Dipl.Visible_Rel[itr2] < default_relations)
                {
                    _players[itr1].Dipl.Visible_Rel[itr2] += gravitation;
                    SETMAX(_players[itr1].Dipl.Visible_Rel[itr2], (int8_t)default_relations);
                }
                else
                {
                    if((_turn % 10) == 0)
                    {
                        _players[itr1].Dipl.Visible_Rel[itr2] -= gravitation;
                        SETMIN(_players[itr1].Dipl.Visible_Rel[itr2], (int8_t)default_relations);
                    }
                }
                SETMIN(_players[itr1].Dipl.Visible_Rel[itr2], -100);
                SETMAX(_players[itr1].Dipl.Visible_Rel[itr2], 100);
            }
        }
    }
    for(itr1 = 0; itr1 < _num_players; itr1++)
    {
        for(itr2 = 0; itr2 < _num_players; itr2++)
        {
            if(itr2 == itr1)
            {
                _players[itr1].Dipl.Visible_Rel[itr2] = 0;
            }
            else
            {
                _players[itr1].Dipl.Visible_Rel[itr2] = _players[itr2].Dipl.Visible_Rel[itr1];
            }
        }
    }

    /* CLAUDE: GD point 629 -- _players AFTER Diplomacy_Growth.  Pairs with 628 to show
     * whether the treaty/peace/exchange modifier divergence is introduced by this pass
     * (drawing from the desynced RNG stream) or already present on entry.  Fire once. */
    STU_GD_ONCE("629_Diplomacy_Growth_Return_P", STU_GD_Dump_Players("629_Diplomacy_Growth_Return_P"));  /* CLAUDE */
}


// WZD o85p03
/*
NPC:    find a player we are at war with and the human is allied with
        request them to break their alliance

MoO2  Break_Treaties_(_PLAYER_NUM, _player[_PLAYER_NUM].diplomacy_proposal_war_player[_current_ambassador])
*/
static int16_t Npc_Proposal__WIP(void)
{
    int16_t var_6 = 0;
    int16_t other_player = 0;
    int16_t return_value = 0;
    int16_t itr_players = 0;
    int16_t itr_other_players = 0;  // _SI_

    for(itr_players = 1; itr_players < _num_players; itr_players++)
    {

        if(itr_players != m_diplomac_player_idx)
        {

            if(
                (_players[m_diplomac_player_idx].Dipl.Dipl_Status[itr_players] >= DIPL_War)
                &&
                (_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[itr_players] == DIPL_Alliance)
            )
            {

                itr_other_players = itr_players;

            }

        }

    }

    other_player = itr_other_players;

    // BUGBUG  probably just 0, not HUMAN_PLAYER_IDX, but should be -1 / ST_UNDEFINED
    if(other_player == HUMAN_PLAYER_IDX)
    {

        return_value = 1;

    }
    else
    {

        m_IDK_diplomatic_order = do_IDK_war;

        /* OGBUG  the ally the human must drop is stored in field_7E, which nothing in the game reads; the message's third wizard comes from war_request (break_treaty, copied to m_IDK_break_alliance_player3 by Npc_Diplomacy_Screen), so it names whichever wizard was last asked to be warred on, or none - agreeing still breaks the treaty with the right ally */
        _players[HUMAN_PLAYER_IDX].Dipl.field_7E[m_diplomac_player_idx] = (int8_t)other_player;

        m_diplomsg_1_record_sub_number = ST_UNDEFINED;

        var_6 = DIPL_sub_72DB6__STUB();

        if(var_6 != 0)
        {

            itr_players = (6 + Random(6));

            _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] -= itr_players;

            SETMIN(_players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx], -100);

            _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] = -100;

            // DEDU  right? ...double-check Dasm, do some test code in BCPP30
            _players[m_diplomac_player_idx].Dipl.Visible_Rel[HUMAN_PLAYER_IDX] = -100;

            return_value = 0;

        }
        else
        {

            // MoO2  Break_Treaties_(_PLAYER_NUM, _player[_PLAYER_NUM].diplomacy_proposal_war_player[_current_ambassador])
            Break_Treaties(HUMAN_PLAYER_IDX, other_player);

            return_value = 1;

        }

    }

    return return_value;

}


// WZD o85p04
// MoO2  Module: DIPLOMAC  Diplomacy_Test_()
// sub_70AFE()
/*
type indicates which temporary modifier to use
type modifier is ?
75 for Wizard Pact

*/
static int16_t Diplomacy_Test(int16_t type_modifier, int16_t type)
{
    int16_t threat_modifier = 0;
    int32_t value2 = 0;
    int32_t value1 = 0;
    int16_t temporary_modifier = 0;
    int16_t relations = 0;
    int16_t niu_var_2 = 0;
    int16_t final_value = 0;  // _SI_
    int16_t return_value = 0;  // DNE in Dasm

    niu_var_2 = 0;

    switch(type)
    {

        case 0:
        {

            temporary_modifier = _players[HUMAN_PLAYER_IDX].Dipl.treaty_modifier[m_diplomac_player_idx];

        } break;

        case 1:
        {

            // DNE  MoO2 - Trade

        } break;

        case 2:
        {

            temporary_modifier = _players[HUMAN_PLAYER_IDX].Dipl.peace_modifier[m_diplomac_player_idx];

        } break;

        case 3:
        {
            
            temporary_modifier = _players[HUMAN_PLAYER_IDX].Dipl.exchange_modifier[m_diplomac_player_idx];

        } break;

        default:
        {

            // DNE

        } break;

    }

    relations = (_players[HUMAN_PLAYER_IDX].Dipl.Hidden_Rel[m_diplomac_player_idx] + _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] + TBL_AI_PRS_IDK_Mod[_players[m_diplomac_player_idx].Personality] + temporary_modifier);

    value1 = 0;
    value2 = 0;

    Invader_Army_Strength_Comparison(_current_player_idx, m_diplomac_player_idx, &value1, &value2);

    if(value2 > value1)
    {

        if(value1 > 0)
        {

            /* OGBUG  same shape as Diplomacy_Break_Treaty(): no units on his continent scores -50, a token force -50 - 100 x (his army / yours), far worse; the 32-bit quotient is cut to 16 bits */
            threat_modifier = (-50 - ((value2 * 100) / value1));

        }
        else
        {

            threat_modifier = -50;

        }

    }
    else
    {

        if(value2 > 0)
        {

            threat_modifier = (-50 + ((value1 * 100) / value2));

        }
        else
        {

            threat_modifier = 50;

        }

    }

    
    final_value = (relations + Random(100) - type_modifier + threat_modifier);

    if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[m_diplomac_player_idx] == DIPL_Alliance)
    {

        final_value += 40;

    }

    Adjust_Diplomat_Modifiers(HUMAN_PLAYER_IDX, m_diplomac_player_idx);

    if(type == 0)
    {

        _players[HUMAN_PLAYER_IDX].Dipl.treaty_modifier[m_diplomac_player_idx] -= (20 + Random(30));

    }

    if(type == 2)
    {

        _players[HUMAN_PLAYER_IDX].Dipl.peace_modifier[m_diplomac_player_idx] -= (50 + Random(50));

    }

    if(type == 3)
    {

        _players[HUMAN_PLAYER_IDX].Dipl.exchange_modifier[m_diplomac_player_idx] -= (20 + Random(50));

    }

    m_diplomacy_test_value = final_value;

    if(final_value < -75)
    {

        return 0;

    }
    else if(final_value < -50)
    {

        return 1;

    }
    else if(final_value < 0)
    {

        return 2;

    }
    else
    {
        
        return 3;

    }

}

// WZD o85p05
// drake178: DIPL_LowestInterest()
// MoO2  Module: DIPLOMAC  Find_Worst_Modifier_()
/*
; returns the lowest out the target's treaty, trade,
; and peace interests towards the human player
; BUG: checks an unset variable to return -1000 instead
; but it can't (intended to be dipl status maybe?)
*/
/*

`if(Find_Worst_Modifier() < -100)` ~== 'Cold Shoulder'

XREF:
    IDK_Diplo_Scrn()
    IDK_Dipl_s7373B()

*/
static int16_t Find_Worst_Modifier(void)
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

        interest = _players[HUMAN_PLAYER_IDX].Dipl.treaty_modifier[m_diplomac_player_idx];

        if(_players[HUMAN_PLAYER_IDX].Dipl.exchange_modifier[m_diplomac_player_idx] < interest)
        {

            interest = _players[HUMAN_PLAYER_IDX].Dipl.exchange_modifier[m_diplomac_player_idx];

        }

        if(_players[HUMAN_PLAYER_IDX].Dipl.peace_modifier[m_diplomac_player_idx] < interest)
        {

            interest = _players[HUMAN_PLAYER_IDX].Dipl.peace_modifier[m_diplomac_player_idx];

        }

        return_value = interest;
    
    }

    return return_value;

}


// WZD o85p06
// MoO2  Module: DIPLOMAC  Limit_Treaty_Modifiers_()
static void Limit_Treaty_Modifiers(void)
{
    int16_t itr2 = 0;
    int16_t itr1 = 0;
    for(itr1 = 0; itr1 < _num_players; itr1++)
    {
        for(itr2 = 0; itr2 < _num_players; itr2++)
        {
            if(itr1 != itr2)
            {
                SETMAX(_players[itr1].Dipl.treaty_modifier[itr2], 120);
                SETMIN(_players[itr1].Dipl.treaty_modifier[itr2], -200);
            }
        }
    }
}


// WZD o85p07
// drake178: WIZ_DIPL_TeachSpell()
/*
; a wrapper for WIZ_LearnSpell that, if called for the
; human player, will also execute DIPL_LoadTalkGFX
; afterward to (re)load the diplomacy talk graphics
*/
/*

*/
static void Diplomacy_Player_Gets_Spell(int16_t player_idx, int16_t spell_idx)
{

    Player_Gets_Spell(player_idx, spell_idx, ST_FALSE);

    if(player_idx == _current_player_idx)
    {

        DIPL_LoadTalkGFX();

    }

}


// WZD o85p08
void G_DIPL_PickSides(void)
{
    int _SI_i;
    int _DI_j;
    int Third_Player_Index;
    /* OGBUG  both inner loops start above _SI_i and the outer one at 1: a wizard allied to two wizards at war with each other is fixed only when it has the lowest index of the three, and the human is never involved */
    for(_SI_i = 1; _SI_i < _num_players; _SI_i++)
    {
        for(_DI_j = _SI_i + 1; _DI_j < _num_players; _DI_j++)
        {
            for(Third_Player_Index = _SI_i + 1; Third_Player_Index < _num_players; Third_Player_Index++) {
                if(_players[_DI_j].Dipl.Dipl_Status[Third_Player_Index] == DIPL_War)
                {
                    if(_players[_SI_i].Dipl.Dipl_Status[_DI_j] == DIPL_Alliance &&
                        _players[_SI_i].Dipl.Dipl_Status[Third_Player_Index] == DIPL_Alliance)
                        {
                        Break_Treaties(_SI_i, _DI_j);
                        Break_Treaties(_SI_i, Third_Player_Index);
                    }
                }
            }
        }
    }
}


// WZD o85p09
// drake178: DIPL_SignTreaty()
// MoO2  Module: DIPLOMAC  Start_Treaty_()
/*
; sets the selected treaty symmetrically between the
; two players, reduces their hostility to zero, and
; clears all non-road-building orders for all units of
; the AI player(s) involved
*/
/*

*/
static void Start_Treaty(int16_t player1, int16_t player2, int16_t type)
{

    _players[player1].Dipl.Dipl_Status[player2] = (int8_t)type;
    _players[player2].Dipl.Dipl_Status[player1] = (int8_t)type;

    _players[player1].Hostility[player2] = 0;
    _players[player2].Hostility[player1] = 0;

    if(
        (player1 == _current_player_idx)
        ||
        (player2 == _current_player_idx)
    )
    {

        Player_Clear_All_Unit_Orders(player1);
                               
        Player_Clear_All_Unit_Orders(player2);

    }

}


// WZD o85p10
// drake178: DIPL_SignPeaceTreaty()
// MoO2  Module: DIPLOMAC  Declare_Peace_()
/*
; creates a peace treaty between the selected players,
; improving relations, dropping city curses, setting
; peace counters, clearing hostility, and resetting
; war re-evaluation counters
; WARNING: the peace counters are not symmetrical
*/
/*

*/
static void Declare_Peace(int16_t player1, int16_t player2)
{

    Start_Treaty(player1, player2, 0);


    /* OGBUG  the +40 is an 8-bit add before the cap: a relation of 88..100 wraps to -128..-116 */
    _players[player1].Dipl.Visible_Rel[player2] += 40;
    // DNE  _players[player1].Dipl.Visible_Rel[player2] += 40;

    SETMAX(_players[player1].Dipl.Visible_Rel[player2], 100);

    _players[player2].Dipl.Visible_Rel[player1] = _players[player1].Dipl.Visible_Rel[player2];

    if(player1 == HUMAN_PLAYER_IDX)
    {

        _players[player1].peace_duration[player2] = (7 + Random(6));

    }

    if(player2 == HUMAN_PLAYER_IDX)
    {
        
        _players[player2].peace_duration[player1] = (7 + Random(6));

    }

    Cancel_Players_City_Enchantments(player1, player2);

    // ; already done when signing a treaty
    _players[player1].Hostility[player2] = 0;
    _players[player2].Hostility[player1] = 0;

    _players[player1].reevaluate_hostility_countdown = (15 + Random(10));
    _players[player2].reevaluate_hostility_countdown = (15 + Random(10));

    if(
        (player1 == _current_player_idx)
        ||
        (player2 == _current_player_idx)
    )
    {

        Player_Clear_All_Unit_Orders(player1);

        Player_Clear_All_Unit_Orders(player2);

    }

}


// WZD o85p11
// MoO2  Module: DIPLOMAC  Adjust_Diplomat_Modifiers_()
/**
 * @brief Applies a small uniform decay to bilateral diplomatic modifiers.
 *
 * @details
 * Decrements the acting player's stored modifier values toward another player
 * by a fixed amount of 10 points for each tracked channel:
 * - @c treaty_modifier
 * - @c exchange_modifier
 * - @c peace_modifier
 *
 * This helper updates only the @p player1 -> @p player2 direction; callers
 * perform additional invocations when symmetric updates are required.
 *
 * @param player1 Source player index whose diplomacy modifier arrays are mutated.
 * @param player2 Target player index selecting the per-opponent slots to adjust.
 *
 * @return This function returns no value.
 *
 * @note No bounds checks or clamping are performed in this helper.
 * @note Intended for use from diplomacy relation/treaty progression paths.
 *
 * @see Change_Relations(), Diplomacy_Growth()
 */
static void Adjust_Diplomat_Modifiers(int16_t player1, int16_t player2)
{
    _players[player1].Dipl.treaty_modifier[player2]   -= 10;
    _players[player1].Dipl.exchange_modifier[player2] -= 10;
    _players[player1].Dipl.peace_modifier[player2]    -= 10;
}


// WZD o85p12
// MoO2  Module: DIPLOMAC  Change_Relations_()  Module: NETMOX  Russ_Change_Relations_()
/**
 * @brief Applies a diplomatic relation delta between two contacted players.
 *
 * @details
 * This routine is the central relation-adjustment path used by diplomacy events.
 * It validates participants and context, transforms the input relation delta
 * using current relation state/personality/charismatic modifiers, then updates
 * visible relations, treaty/exchange/peace modifiers, hostility, and delayed
 * diplomacy-action memory fields.
 *
 * High-level flow:
 * 1. Require attacker/defender contact.
 * 2. In special mode (@c AI_Dipl_Unset_0 == 3), only record a potential human
 *    diplomatic action for certain negative AI->human events.
 * 3. Otherwise, for non-monster participants and valid pair/state:
 *    - Handle sentinel value @c -10000 as a direct diplomatic-action memory case.
 *    - Apply diminishing-returns scaling based on current visible relation sign/magnitude.
 *    - Apply defender personality multipliers (Lawful/Maniacal cases).
 *    - Apply attacker Charismatic trait adjustment.
 *    - If currently at war and event type is in the low action range, route part
 *      of negative pressure into peace modifiers and suppress direct relation delta.
 *    - Apply late-game AI-vs-AI scaling adjustments.
 *    - If resulting delta is non-zero, write relation and modifier updates,
 *      clamp ranges, and store strongest recent human-caused diplomatic action.
 * 4. Enforce post-conditions by treaty state (war/alliance caps), mirror relation
 *    symmetry, update hostility flags, and reset hostility reevaluation timer.
 *
 * @param value Base relation delta to apply (special sentinel: @c -10000).
 * @param attacker_idx Player index initiating the action.
 * @param defender_idx Player index receiving the relation change.
 * @param type Diplomatic action/event type identifier used for branch rules and
 *             diplomatic-action memory logging.
 * @param city_idx Optional city context associated with the action.
 * @param spell_idx Optional spell context associated with the action.
 *
 * @return This function returns no value.
 *
 * @note This function mutates multiple diplomacy tables for both players,
 *       including @c Visible_Rel, @c treaty_modifier, @c exchange_modifier,
 *       @c peace_modifier, @c Dipl_Action, @c DA_Strength, @c DA_Spell,
 *       @c DA_City, @c Hostility, and reevaluation countdowns.
 * @note A special-case branch can trigger @c Declare_War() from a spell-context
 *       check that currently compares against @c spl_Earth_Lore.
 * @note Relation values are clamped to preserve legal range and treaty-specific
 *       floors/caps after adjustment.
 * @note Behavior intentionally preserves existing OGBUG/legacy conditions.
 *
 * @see Declare_War(), Break_Treaties(), Diplomacy_Growth(), Adjust_Diplomat_Modifiers()
 */
void Change_Relations(int16_t value, int16_t attacker_idx, int16_t defender_idx, int16_t type, int16_t city_idx, int16_t spell_idx)
{
    int16_t diminishing_returns_divisor = 0;
    int16_t relation_value_pre_charisma = 0;
    int16_t relation_value = 0;  // DNE in Dasm
    relation_value = value;  // DNE in Dasm
    if(_players[attacker_idx].Dipl.Contacted[defender_idx] != ST_FALSE)
    {
        if(AI_Dipl_Unset_0 == 3)
        {
            if(
                (attacker_idx != HUMAN_PLAYER_IDX)
                &&
                (relation_value < 0)
                &&
                (defender_idx == HUMAN_PLAYER_IDX)
            )
            {
                if(Random(100) <= (abs(relation_value) / 10))
                {

                    _players[HUMAN_PLAYER_IDX].Dipl.DA_Strength[HUMAN_PLAYER_IDX] = 100;
                    _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[HUMAN_PLAYER_IDX] = 39;
                    _players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[HUMAN_PLAYER_IDX] = spell_idx;
                    _players[HUMAN_PLAYER_IDX].Dipl.DA_City[HUMAN_PLAYER_IDX] = (int8_t)city_idx;
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
                    (relation_value != 0)
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
                    if(relation_value == -10000)
                    {
                        if(
                            (attacker_idx == HUMAN_PLAYER_IDX)
                            &&
                            (type != dipact_AuraOfMajesty)
                        )
                        {
                            _players[HUMAN_PLAYER_IDX].Dipl.DA_Strength[defender_idx] = 10000;
                            _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[defender_idx] = (int8_t)type;
                            _players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[defender_idx] = spell_idx;
                            _players[HUMAN_PLAYER_IDX].Dipl.DA_City[defender_idx] = (int8_t)city_idx;
                        }
                        return;
                    }
                    else
                    {
                        if(relation_value > 0)
                        {
                            if(_players[defender_idx].Dipl.Visible_Rel[attacker_idx] < 0)
                            {
                                relation_value = (relation_value * 2);

                                if((_players[defender_idx].Dipl.Visible_Rel[attacker_idx] + relation_value) > 10)
                                {
                                    relation_value = (10 - _players[defender_idx].Dipl.Visible_Rel[attacker_idx]);
                                }
                            }
                            else
                            {
                                diminishing_returns_divisor = ((_players[defender_idx].Dipl.Visible_Rel[attacker_idx] / 25) + 1);

                                if(diminishing_returns_divisor == 0)
                                {
                                    diminishing_returns_divisor = 1;
                                }
                                relation_value = (relation_value / diminishing_returns_divisor);
                            }
                        }
                        else
                        {
                            if(_players[defender_idx].Dipl.Visible_Rel[attacker_idx] > 0)
                            {
                                relation_value = (relation_value * 2);
                            }
                            else
                            {
                                diminishing_returns_divisor = ((_players[defender_idx].Dipl.Visible_Rel[attacker_idx] / -25) + 1);

                                if(diminishing_returns_divisor == 0)
                                {
                                    diminishing_returns_divisor = 1;
                                }
                                relation_value = (relation_value / diminishing_returns_divisor);
                            }
                        }
                    }
                    if(
                        (_players[defender_idx].Personality == PRS_Lawful)
                        &&
                        (relation_value < 0)
                    )
                    {
                        relation_value = (relation_value * 2);
                    }
                    if(
                        (_players[defender_idx].Personality == PRS_Maniacal)
                        &&
                        (relation_value < 0)
                    )
                    {
                        relation_value = ((relation_value * 3) / 2);
                    }
                    if(
                        (_players[defender_idx].Personality == PRS_Maniacal)
                        &&
                        (relation_value > 0)
                    )
                    {
                        relation_value = ((relation_value * 3) / 4);
                    }
                    relation_value_pre_charisma = relation_value;
                    // "... special ability (Charismatic) doubles the effectiveness of any positive diplomatic actions she takes and halves the negative impact ..."
                    // "... Charismatic wizard obtains twice the normal effect from positive diplomatic actions he or she initiates and suffers only half the normal penalty for negative diplomatic actions."
                    if(
                        (_players[attacker_idx].charismatic == ST_TRUE)
                        &&
                        (relation_value > 0)
                    )
                    {
                        relation_value = (relation_value * 2);
                    }
                    if(
                        (_players[attacker_idx].charismatic == ST_TRUE)
                        &&
                        (relation_value < 0)
                    )
                    {
                        relation_value = (relation_value / 2);
                    }
                    if(
                        (_players[defender_idx].Dipl.Dipl_Status[attacker_idx] == DIPL_War)
                        &&
                        (type <= dipact_Overextension)
                        &&
                        (relation_value < 0)
                    )
                    {
                        if(
                            (_players[defender_idx].Dipl.Dipl_Status[attacker_idx] == DIPL_War)
                            &&
                            (attacker_idx == HUMAN_PLAYER_IDX)
                        )
                        {
                            _players[defender_idx].Dipl.peace_modifier[attacker_idx] += (relation_value / 4);
                            _players[attacker_idx].Dipl.peace_modifier[defender_idx] -= (relation_value / 4);
                        }
                        relation_value = 0;
                    }
                    /*
                        BEGIN:  
                    */
                    if(
                        (attacker_idx != HUMAN_PLAYER_IDX)
                        &&
                        (defender_idx != HUMAN_PLAYER_IDX)
                        &&
                        (relation_value > 0)
                        &&
                        (_turn > 200)
                    )
                    {
                        relation_value = (relation_value * 2);
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
                        (relation_value < 0)
                        &&
                        (_turn > 200)
                    )
                    {
                        relation_value = (relation_value / ((_difficulty / 2) + 1));  // {0,1,2,3,4} / 2 = {0,0,1,1,2} + 1 = {1,1,2,2,3} = {100%,100%,50%,50%,33%}
                    }
                    /*
                        END:  
                    */
                    if(relation_value != 0)  /* Block A (war/overextension path) didn't trigger, do Block B (normal apply path) instead */
                    {
                        /* OGBUG  the relation is added as a byte (mov al / add al, [byte ptr value]) before the -100..100 clamp: a total below -128 wraps positive - e.g. a Lawful observer with relation 1..71 sees -50 x 2 x 2 = -200 and ends at relation + 56, then the 65 cap */
                        _players[defender_idx].Dipl.Visible_Rel[attacker_idx] += relation_value;
                        SETMIN(_players[defender_idx].Dipl.Visible_Rel[attacker_idx], -100);
                        SETMAX(_players[defender_idx].Dipl.Visible_Rel[attacker_idx],  100);
                        _players[defender_idx].Dipl.treaty_modifier[attacker_idx] += relation_value;
                        _players[defender_idx].Dipl.exchange_modifier[attacker_idx] += relation_value;
                        /* OGBUG  the observer's treaty and exchange interest are copied onto the actor's row: when the actor is the human this overwrites _players[0].X[observer], the row every human-AI talk reads and spends, so any relation event the human causes restores the interest spent in talks */
                        _players[attacker_idx].Dipl.treaty_modifier[defender_idx] = _players[defender_idx].Dipl.treaty_modifier[attacker_idx];
                        _players[attacker_idx].Dipl.exchange_modifier[defender_idx] = _players[defender_idx].Dipl.exchange_modifier[attacker_idx];
                        if(relation_value_pre_charisma < 0)  /* relation change is negative */
                        {
                            _players[attacker_idx].Dipl.peace_modifier[defender_idx] -= (relation_value_pre_charisma / 4);  // more negative
                            _players[defender_idx].Dipl.peace_modifier[attacker_idx] += (relation_value_pre_charisma / 4);  // less positive
                        }
                        else
                        {
                            _players[defender_idx].Dipl.peace_modifier[attacker_idx] += relation_value_pre_charisma;
                        }
                        /* OGBUG  the 10% war roll tests the last argument against spl_Earth_Lore (10): Change_Relations_For_Enchantments passes type 10 with spell_idx = spl_Spell_Of_Mastery, so it never fires there; it fires instead when the last argument happens to be 10 - Cast_Spell_Overland's hostile unit spells pass the target unit index, so a curse on unit #10 gives its caster a 10% chance to declare war on the owner */
                        if(
                            (spell_idx == spl_Earth_Lore)  /* OGBUG: should be spl_Spell_Of_Mastery, not spl_Earth_Lore */
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
                            if(abs(relation_value) > abs(_players[HUMAN_PLAYER_IDX].Dipl.DA_Strength[defender_idx]))
                            {
                                _players[HUMAN_PLAYER_IDX].Dipl.DA_Strength[defender_idx] = relation_value;
                                _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[defender_idx] = (int8_t)type;
                                _players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[defender_idx] = spell_idx;
                                _players[HUMAN_PLAYER_IDX].Dipl.DA_City[defender_idx] = (int8_t)city_idx;
                            }
                        }
                    }  /* if(relation_value != 0) */
                    if(_players[defender_idx].Dipl.Dipl_Status[attacker_idx] >= DIPL_War)
                    {
                        SETMAX(_players[defender_idx].Dipl.Visible_Rel[attacker_idx], -25);
                    }
                    if(_players[defender_idx].Dipl.Dipl_Status[attacker_idx] != DIPL_Alliance)
                    {
                        SETMAX(_players[defender_idx].Dipl.Visible_Rel[attacker_idx], 65);
                    }
                    _players[attacker_idx].Dipl.Visible_Rel[defender_idx] = _players[defender_idx].Dipl.Visible_Rel[attacker_idx];
                    if(_players[defender_idx].Dipl.Dipl_Status[attacker_idx] >= DIPL_War)
                    {
                        _players[defender_idx].Hostility[attacker_idx] = 3;
                    }
                    if(_players[defender_idx].Dipl.Dipl_Status[attacker_idx] == DIPL_Alliance)
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
/*
*/
void Declare_War(int16_t attacker_idx, int16_t defender_idx)
{

    if(
        (attacker_idx == HUMAN_PLAYER_IDX)
        &&
        (_players[attacker_idx].Dipl.contact_stage[defender_idx] == pcs_Unmet)
    )
    {
        _players[attacker_idx].Dipl.contact_stage[defender_idx] = pcs_Greeted;
    }

    if(
        (defender_idx == HUMAN_PLAYER_IDX)
        &&
        (_players[defender_idx].Dipl.contact_stage[attacker_idx] == pcs_Unmet)
    )
    {
        _players[defender_idx].Dipl.contact_stage[attacker_idx] = pcs_Greeted;
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

    /* OGBUG  the base relation drops by 5 both ways as a byte with no floor: from -100 the sixth war between the same pair wraps it to about +126 */
    _players[attacker_idx].Dipl.Default_Rel[defender_idx] += -5;
    _players[defender_idx].Dipl.Default_Rel[attacker_idx] += -5;

    _players[attacker_idx].Dipl.Visible_Rel[defender_idx] = (-75 - Random(25));
    _players[defender_idx].Dipl.Visible_Rel[attacker_idx] = _players[attacker_idx].Dipl.Visible_Rel[defender_idx];

    _players[attacker_idx].Dipl.Dipl_Status[defender_idx] = DIPL_War;
    _players[defender_idx].Dipl.Dipl_Status[attacker_idx] = DIPL_War;

    /* 1oom: treaty, trade, tech, peace */

    _players[attacker_idx].Dipl.treaty_modifier[defender_idx] = -200;
    _players[defender_idx].Dipl.treaty_modifier[attacker_idx] = -200;

    _players[attacker_idx].Dipl.exchange_modifier[defender_idx] = -200;
    _players[defender_idx].Dipl.exchange_modifier[attacker_idx] = -200;

    _players[attacker_idx].Dipl.peace_modifier[defender_idx] = -130;
    _players[defender_idx].Dipl.peace_modifier[attacker_idx] = -130;


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

    /* OGBUG  the penalty is negative (-10 pact / -20 alliance, doubled when the BREAKER is Lawful) and is SUBTRACTED: the breaker's hidden relation toward the victim goes UP by 10..40 (a byte, no clamp), and for an alliance so does the base relation below (clamped only below -100, so a base above 87 can wrap and land on -100) */
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
        SETMIN(_players[attacker_idx].Dipl.Default_Rel[defender_idx], -100);
        _players[defender_idx].Dipl.Default_Rel[attacker_idx] = _players[attacker_idx].Dipl.Default_Rel[defender_idx];
    }


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

    _players[attacker_idx].Dipl.niu_attack_bounty[defender_idx] = 0;
    _players[defender_idx].Dipl.niu_attack_bounty[attacker_idx] = 0;

    SETMIN(_players[attacker_idx].Dipl.Visible_Rel[defender_idx], -100);
    _players[defender_idx].Dipl.Visible_Rel[attacker_idx] = _players[attacker_idx].Dipl.Visible_Rel[defender_idx];

    _players[attacker_idx].Dipl.treaty_modifier[defender_idx] = -200;
    _players[attacker_idx].Dipl.exchange_modifier[defender_idx] = -200;
    _players[attacker_idx].Dipl.peace_modifier[defender_idx] = -200;

    _players[defender_idx].Dipl.treaty_modifier[attacker_idx] = -200;
    _players[defender_idx].Dipl.exchange_modifier[attacker_idx] = -200;
    _players[defender_idx].Dipl.peace_modifier[attacker_idx] = -200;

    _players[attacker_idx].reevaluate_hostility_countdown = (15 + Random(10));
    _players[defender_idx].reevaluate_hostility_countdown = (15 + Random(10));

}

// WZD o85p15
/*
; fills the corresponding global variables with up to
; 10 spells and their values that the first player
; could tribute to the second
*/
/*

Get_Main_Diplomacy_Choices()
    Get_Exchange_Spell_List(_current_player_idx, m_diplomac_player_idx, 0);
Diplomacy_Offer_Tribute()
    Get_Exchange_Spell_List(HUMAN_PLAYER_IDX, m_diplomac_player_idx, byte_42E39[itr]);

*/
static int16_t Get_Exchange_Spell_List(int16_t player1, int16_t player2, int16_t min_value)
{
    int16_t spell_count = 0;
    int16_t itr = 0;  // _SI_

    m_exchange_spell_count = 0;

    spell_count = 0;

    spell_count = Get_Differential_Spell_List(player2, player1, min_value, (uint8_t *)&m_exchange_spell_list[0]);

    if(spell_count < 1)
    {

        m_exchange_spell_count = 0;
        
    }
    else
    {

        m_exchange_spell_count = spell_count;

        for(itr = 0; m_exchange_spell_count > itr; itr++)
        {

            /* OGBUG  stores only the low byte of the spell value: Exchange Spells and Offer Tribute pass it back to Get_Differential_Spell_List() as min_value, so the threshold is the value mod 256 (compared against full values) */
            m_exchange_spell_values[itr] = (uint8_t)Calc_Spell_Value(player2, m_exchange_spell_list[itr]);

        }

    }

    return m_exchange_spell_count;

}


// WZD o85p16
/*

clear active flags
set count and list for gold
build string list

XREF:
    j_Diplomacy_Offer_Tribute__STUB()
        Get_Main_Diplomacy_Choices_()

*/
static void Diplomacy_Offer_Tribute(void)
{
    char string[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t gold_amounts[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t list_item_active_flags[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int32_t gold = 0;
    int16_t var_8 = 0;
    int16_t var_6 = 0;
    int16_t list_count = 0;
    int16_t _variable = 0;
    int16_t itr = 0;  // _SI_
    int16_t gold_cap = 0;  // _DI_
    
    for(itr = 0; itr < 10; itr++)
    {

        list_item_active_flags[itr] = ST_TRUE;

    }

    /*
        BEGIN: gold
    */
    /* OGBUG  the gold choices are sized by the AI partner's treasury, not the human's: an AI with less than 25 gold gives list_count 0 - no gold option at all */
    gold = _players[m_diplomac_player_idx].gold_reserve;

    SETMAX(gold, MAX_GOLD_RESERVE);

    gold_cap = ((gold / 25) * 25);

    if(gold_cap < 100)
    {

        list_count = (gold_cap / 25);

        gold_amounts[0] = 25;
        gold_amounts[1] = 50;
        gold_amounts[2] = 75;
        gold_amounts[3] = 100;

    }
    else
    {

        list_count = 4;

        gold_amounts[0] = ((( gold_cap / 4)      / 25) * 25);  //  25%
        gold_amounts[1] = ((( gold_cap / 2)      / 25) * 25);  //  50%
        gold_amounts[2] = ((((gold_cap * 3) / 4) / 25) * 25);  //  75%
        gold_amounts[3] = ((  gold_cap           / 25) * 25);  // 100%

    }
    /*
        END: gold
    */

    _variable = 0;

    stu_strcpy(m_diplomacy_message, off_3B95A);  // "What do you offer as tribute?"


    /*
        BEGIN: Build String List
    */

    for(itr = 0; itr < list_count; itr++)
    {

        stu_itoa(gold_amounts[itr], string, 10);

        stu_strcpy(G_Some_DIPL_Allocs_7[itr], lstr_gold);

        stu_strcat(G_Some_DIPL_Allocs_7[itr], string);

        stu_strcat(G_Some_DIPL_Allocs_7[itr], aGold_0);

        if(gold_amounts[itr] > _players[_current_player_idx].gold_reserve)
        {

            list_item_active_flags[itr] = ST_FALSE;

        }

    }

    stu_strcpy(G_Some_DIPL_Allocs_7[itr], aSpells);

    stu_strcpy(G_Some_DIPL_Allocs_7[(1 + itr)], aForgetIt_0);

    *G_Some_DIPL_Allocs_7[(2 + itr)] = '\0';

    /*
        END: Build String List
    */


    Set_Font_Style(1, 4, 3, ST_NULL);

    Set_Alias_Color(187);

    Set_Font_LF(2);

    _variable = Get_List_Field(38, 142, 245, &aWhatDoYouOffer[29], G_Some_DIPL_Allocs_7, 100, &_variable, &list_item_active_flags[0], 15, 11, 0, 0, 0, ST_UNDEFINED);

    if(
        (_variable != ST_UNDEFINED)
        &&
        ((list_count + 1) != _variable)  /* ¿ ? */
    )
    {

        if(_variable < list_count)
        {

            if(_players[m_diplomac_player_idx].astrologer.army_strength != 0)
            {

                var_6 = ((_variable + 1) * 4);

            }
            else
            {

                var_6 = 8;

            }

            itr = Random(3);

            var_8 = (int16_t)(((int32_t)var_6 * ((int32_t)(_variable + 1) * itr)) / 10);

            _players[HUMAN_PLAYER_IDX].gold_reserve -= gold_amounts[_variable];

            Player_Add_Gold(m_diplomac_player_idx, gold_amounts[_variable]);

            SETMAX(var_8, 100);

            // DNE  _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] += var_8;

            if((_players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] + var_8) > 100)
            {

                _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] = 100;

            }
            else
            {

                _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] += var_8;

            }

            SETMAX(_players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx], 100);

            _players[m_diplomac_player_idx].Dipl.Visible_Rel[HUMAN_PLAYER_IDX] = _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx];

            Diplomacy_Display_Response(1, 3);

        }
        else
        {

            /* OGBUG  min_value is byte_42E39[itr] with itr = the number of gold options: a stale byte left by the last Exchange Spells (or AI spell offer); a high leftover value hides tribute spells */
            Get_Exchange_Spell_List(HUMAN_PLAYER_IDX, m_diplomac_player_idx, byte_42E39[itr]);

            if(m_exchange_spell_count > 0)
            {

                _variable = 0;

                for(itr = 0; ((m_exchange_spell_count > itr) && (itr < 4)); itr++)
                {
                    assert(m_exchange_spell_list[itr] != 0);

                    stu_strcpy(G_Some_DIPL_Allocs_7[itr], lstr_gold);

                    String_Copy_Far(string, spell_data_table[m_exchange_spell_list[itr]].name);

                    stu_strcat(G_Some_DIPL_Allocs_7[itr], string);

                }

                stu_strcpy(G_Some_DIPL_Allocs_7[itr], aForgetIt_0);

                *G_Some_DIPL_Allocs_7[(1 + itr)] = '\0';

                stu_strcpy(m_diplomacy_message, off_3B95A);

                Set_Font_Style(1, 4, 3, ST_NULL);

                Set_Alias_Color(187);

                _variable = Get_List_Field(38, 142, 245, &aWhatDoYouOffer[29], G_Some_DIPL_Allocs_7, 100, &_variable, 0, 15, 11, 0, 0, 0, ST_UNDEFINED);

                if(_variable != ST_UNDEFINED)
                {
                            
                    if(
                        (m_exchange_spell_count > _variable)
                        &&
                        (_variable < 4)
                    )
                    {

                        Diplomacy_Player_Gets_Spell(m_diplomac_player_idx, m_exchange_spell_list[_variable]);

                        if(_players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] < 0)
                        {

                            var_6 = 20;

                        }
                        else
                        {
                                    
                            var_6 = ((100 - _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]) / 10);

                        }

                        var_8 = ((var_6 * (Random(8) + Random(8))) / 10);

                        _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] += var_8;

                        SETMAX(_players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx], 100);

                        _players[m_diplomac_player_idx].Dipl.Visible_Rel[HUMAN_PLAYER_IDX] = _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx];

                        _players[HUMAN_PLAYER_IDX].Dipl.peace_modifier[m_diplomac_player_idx] += var_8;

                        SETMAX(_players[HUMAN_PLAYER_IDX].Dipl.peace_modifier[m_diplomac_player_idx], 200);

                        /* OGBUG  the tribute raises _players[0].Hidden_Rel[m], which the diplomacy screen reads (Diplomacy_Greeting, Diplomacy_Test, NPC_To_Human_Diplomacy, DIPL_HumanWarOrPeace), but not _players[m].Hidden_Rel[0], which AI_Evaluate_Hostility() and AI_Select_Spell_Group() read; nothing copies one to the other, so a tribute makes the AI friendlier in talks but does not lower its war hostility toward the human */
                        _players[HUMAN_PLAYER_IDX].Dipl.Hidden_Rel[m_diplomac_player_idx] += (2 + Random(8));

                        SETMAX(_players[HUMAN_PLAYER_IDX].Dipl.Hidden_Rel[m_diplomac_player_idx], 30);

                        assert(m_exchange_spell_list[_variable] >= 1 && m_exchange_spell_list[_variable] <= 214);
                        _players[HUMAN_PLAYER_IDX].Dipl.field_8A[m_diplomac_player_idx] = m_exchange_spell_list[_variable];

                        Diplomacy_Display_Response(1, 3);

                    }

                }

            }
            else  /* (m_exchange_spell_count <= 0) */
            {

                Diplomacy_Display_Response(do_IDK_exchange_spell_goodbye, 3);

            }

        }
        
    }

}


// WZD o85p17
/*

*/
static void Diplomacy_Exchange_Spell__WIP(void)
{
    char string[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t final_value = 0;
    int16_t relations = 0;
    int16_t var_10 = 0;
    int16_t var_E = 0;
    int16_t var_C = 0;
    int16_t Spell_Index = 0;
    int16_t spell_idx = 0;
    int16_t var_6 = 0;
    int16_t var_4 = 0;
    int16_t _variable = 0;
    int16_t temporary_modifier = 0;  // _DI_
    int16_t itr = 0;  // _SI_

    var_10 = 0;

    temporary_modifier = _players[HUMAN_PLAYER_IDX].Dipl.exchange_modifier[m_diplomac_player_idx];

    if(temporary_modifier > 0)
    {

        temporary_modifier /= 5;

    }

    SETMAX(temporary_modifier, 30);

    if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[m_diplomac_player_idx] == DIPL_Alliance)
    {

        temporary_modifier += 25;

    }

    relations = (_players[HUMAN_PLAYER_IDX].Dipl.Hidden_Rel[m_diplomac_player_idx] + (_players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] / 2) + var_10 + (TBL_AI_PRS_IDK_Mod[_players[m_diplomac_player_idx].Personality] * 2) + temporary_modifier - 125);

    /* OGBUG  the AI's acceptance roll is computed and stored but final_value is never read: the AI accepts every exchange it lists */
    final_value = relations + Random(100);

    Get_Exchange_Spell_List(m_diplomac_player_idx, HUMAN_PLAYER_IDX, 0);

    if(m_exchange_spell_count > 0)
    {

        byte_42E4D = (uint8_t)m_exchange_spell_count;

        for(itr = 0; m_exchange_spell_count > itr; itr++)
        {

            byte_42E43[itr] = m_exchange_spell_list[itr];

            /* OGBUG  the spell value was already cut to its low byte by Get_Exchange_Spell_List(); as the min_value of the counter-offer list below, the threshold is the offered spell's value mod 256 (e.g., 300 -> 44) */
            byte_42E39[itr] = m_exchange_spell_values[itr];

        }

        byte_42E38 = 0;

        for(itr = 0; byte_42E4D > itr; itr++)
        {

            Get_Exchange_Spell_List(HUMAN_PLAYER_IDX, m_diplomac_player_idx, byte_42E39[itr]);

            if(m_exchange_spell_count > 0)
            {

                byte_42E24[byte_42E38] = (uint8_t)m_exchange_spell_count;

                for(var_4 = 0; m_exchange_spell_count > var_4; var_4++)
                {

                    IDK_DIPLO_NIU[((byte_42E38 * 10) + var_4)] = m_exchange_spell_list[var_4];

                }

                byte_42E2E[byte_42E38] = byte_42E43[itr];

                byte_42E38++;

            }

        }

        Assign_Auto_Function(_sub_6ED5D_Draw, 3);

        if(byte_42E38 > 0)
        {

            Adjust_Diplomat_Modifiers(HUMAN_PLAYER_IDX, m_diplomac_player_idx);

            _players[HUMAN_PLAYER_IDX].Dipl.exchange_modifier[m_diplomac_player_idx] -= (20 + Random(50));

            _variable = 0;

            for(itr = 0; ((byte_42E38 > itr) && (itr < 5)); itr++)
            {

                stu_strcpy(G_Some_DIPL_Allocs_7[itr], lstr_gold);

                String_Copy_Far(string, spell_data_table[byte_42E2E[itr]].name);

                stu_strcat(G_Some_DIPL_Allocs_7[itr], string);

            }

            *G_Some_DIPL_Allocs_7[itr] = '\0';

            stu_strcpy(m_diplomacy_message, off_3B95C);

            Set_Font_Style(1, 4, 3, ST_NULL);

            Set_Alias_Color(187);

            // DNE Set_Font_LF(2);

            _variable = Get_List_Field(38, 142, 245, &aWhatDoYouOffer[29], G_Some_DIPL_Allocs_7, 100, &_variable, ST_NULL, 15, 11, 0, 0, 0, ST_UNDEFINED);

            if(_variable != ST_UNDEFINED)
            {

                spell_idx = byte_42E2E[_variable];

                var_C = byte_42E24[_variable];

                var_E = _variable;

                for(itr = 0;  ((itr < var_C) && (itr < 4)); itr++)
                {

                    stu_strcpy(G_Some_DIPL_Allocs_7[itr], lstr_gold);

                    String_Copy_Far(string, spell_data_table[IDK_DIPLO_NIU[((var_E * 10) + itr)]].name);

                    stu_strcat(G_Some_DIPL_Allocs_7[itr], string);

                }

                stu_strcpy(G_Some_DIPL_Allocs_7[itr], aForgetIt_0);

                var_6 = itr;

                *G_Some_DIPL_Allocs_7[(1 + var_6)] = '\0';

                _variable = 0;

                stu_strcpy(m_diplomacy_message, off_3B95E);

                Set_Font_Style(1, 4, 3, ST_NULL);

                Set_Alias_Color(187);

                // DNE Set_Font_LF(2);

                _variable = Get_List_Field(38, 142, 245, &aWhatDoYouOffer[29], G_Some_DIPL_Allocs_7, 100, &_variable, ST_NULL, 15, 11, 0, 0, 0, ST_UNDEFINED);

                if(
                    (_variable != ST_UNDEFINED)
                    &&
                    (_variable != var_6)
                )
                {

                    Spell_Index = IDK_DIPLO_NIU[((var_E * 10) + _variable)];

                    Diplomacy_Player_Gets_Spell(_current_player_idx, spell_idx);

                    Diplomacy_Player_Gets_Spell(m_diplomac_player_idx, Spell_Index);

                    if((uint16_t)spell_data_table[spell_idx].research_cost < (uint16_t)spell_data_table[Spell_Index].research_cost)
                    {

                        Adjust_Diplomat_Modifiers(HUMAN_PLAYER_IDX, m_diplomac_player_idx);
                        Adjust_Diplomat_Modifiers(HUMAN_PLAYER_IDX, m_diplomac_player_idx);

                        Change_Relations(5, HUMAN_PLAYER_IDX, m_diplomac_player_idx, 0, ST_NULL, ST_NULL);

                    }

                }

            }

        }
        else
        {

            Diplomacy_Display_Response(72, 3);

        }

    }
    else
    {

        Diplomacy_Display_Response(72, 3);

    }

    Adjust_Diplomat_Modifiers(HUMAN_PLAYER_IDX, m_diplomac_player_idx);
    Adjust_Diplomat_Modifiers(HUMAN_PLAYER_IDX, m_diplomac_player_idx);
    Adjust_Diplomat_Modifiers(HUMAN_PLAYER_IDX, m_diplomac_player_idx);

    SETMAX(_players[HUMAN_PLAYER_IDX].Dipl.exchange_modifier[m_diplomac_player_idx], 50);

}


// WZD o85p18
// drake178: DIPL_DropCityCurses()
// MoO2  DNE
/*
; removes any city spells that the selected players
; have on each other's cities
*/
/*

*/
static void Cancel_Players_City_Enchantments(int16_t player1, int16_t player2)
{
    int8_t * enchantments = 0;
    int16_t city_idx = 0;  // _SI_
    int16_t ench_idx = 0;  // _CX_

    for(city_idx = 0; city_idx < _cities; city_idx++)
    {

        if(
            (_CITIES[city_idx].owner_idx == player1)
            ||
            (_CITIES[city_idx].owner_idx == player2)
        )
        {

            enchantments = &_CITIES[city_idx].enchantments[0];

            /* OGBUG  loops all NUM_CITY_ENCHANTMENTS slots, including enchantments[NIGHTSHADE], which holds a count of Nightshade squares, not an owner + 1: a count equal to the other wizard's index + 1 is cleared to 0 */
            for(ench_idx = 0; ench_idx < NUM_CITY_ENCHANTMENTS; ench_idx++)
            {

                if(enchantments[ench_idx] > 0)
                {

                    if(_CITIES[city_idx].owner_idx == player1)
                    {

                        if((enchantments[ench_idx] - 1) == player2)
                        {

                            enchantments[ench_idx] = 0;

                        }

                    }
                    else
                    {

                        if((enchantments[ench_idx] - 1) == player1)
                        {

                            enchantments[ench_idx] = 0;

                        }

                    }
                    
                }

            }

        }

    }

}



/*
    WIZARDS.EXE  ovr086
*/

// WZD o86p01
static void Npc_Diplomacy_Screen(void)
{
    int16_t full_screen_field = 0;
    int16_t input_field_idx = 0;
    int16_t leave_screen = 0;
    int16_t var_4 = 0;

    Limit_Treaty_Modifiers();

    m_diplomacy_current_music = ST_UNDEFINED;

    word_42E7E = ST_UNDEFINED;

    m_diplomacy_message_IDK_group = 0;

    Apply_Palette();

    leave_screen = ST_FALSE;

    Assign_Auto_Function(Diplomacy_Screen_Draw__WIP, 3);

    Set_Input_Delay(1);

    Clear_Fields();

    m_IDK_diplomatic_flag = 6;

    m_IDK_diplomatic_order = _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[m_diplomac_player_idx];

    _page_flip_effect = pfe_Default;

    if(
        (m_IDK_diplomatic_order == 54)
        ||
        (m_IDK_diplomatic_order == 1)
    )
    {

        return;

    }

    // BUGBUG  impossible condition - this was excluded in the calling code
    if(m_IDK_diplomatic_order == do_None)
    {

        m_IDK_diplomatic_flag = 0;

        Diplomacy_Greeting();  // does this change m_IDK_diplomatic_flag or m_IDK_diplomatic_order?

    }

    Diplomacy_Screen_Load__WIP();

    m_diplomsg_1_record_sub_number = ST_UNDEFINED;

    if(
        (
            (m_IDK_diplomatic_order >= do_IDK_treaty1)
            &&
            /* Diplomacy_Screen__WIP() (m_IDK_diplomatic_order <= do_IDK_peace) */
            (m_IDK_diplomatic_order <= 50)
        )
        ||
        (m_IDK_diplomatic_order == 2)
        ||
        (m_IDK_diplomatic_order == do_IDK_war)
    )
    {

        m_IDK_old_diplomatic_order = m_IDK_diplomatic_order;

        m_IDK_diplomatic_order = do_IDK_greeting_positive;

        m_IDK_diplomatic_flag = 2;

        m_break_treaty_player3 = _players[HUMAN_PLAYER_IDX].Dipl.break_treaty[m_diplomac_player_idx];

    }

    full_screen_field = INVALID_FIELD;

    while(leave_screen == ST_FALSE)
    {

        Mark_Time();

        input_field_idx = Get_Input();

        if(m_IDK_diplomatic_flag == 0)
        {

            if(input_field_idx == ST_UNDEFINED)
            {

                leave_screen = ST_TRUE;

            }

            if(input_field_idx == full_screen_field)
            {

                Get_Main_Diplomacy_Choices();

                leave_screen = ST_TRUE;

            }

        }
        /* Diplomacy_Screen__WIP()  else if(m_IDK_diplomatic_flag == 1) */
        else if(m_IDK_diplomatic_flag == 2)
        {

            if(
                (input_field_idx == ST_UNDEFINED)
                ||
                (input_field_idx == full_screen_field)
            )
            {

                var_4 = Npc_Proposal__WIP();

                if(var_4 != 0)
                {

                    m_IDK_diplomatic_order = m_IDK_old_diplomatic_order;  /* only known usage  ¿ ~== MoO2  _old_response_message ? */

                    m_diplomsg_1_record_sub_number = ST_UNDEFINED;

                    input_field_idx = DIPL_sub_72DB6__STUB();

                    if(input_field_idx != 0)
                    {

                        if(m_IDK_diplomatic_order == do_IDK_war)
                        {

                            Break_Treaties(m_diplomac_player_idx, HUMAN_PLAYER_IDX);

                        }

                    }
                    else
                    {

                        if(m_IDK_diplomatic_order == do_IDK_treaty1)
                        {

                            Start_Treaty(HUMAN_PLAYER_IDX, m_diplomac_player_idx, 1);

                        }

                        if(m_IDK_diplomatic_order == do_IDK_treaty2)
                        {

                            Start_Treaty(HUMAN_PLAYER_IDX, m_diplomac_player_idx, 2);

                        }

                        if(m_IDK_diplomatic_order == do_IDK_peace)
                        {

                            Declare_Peace(HUMAN_PLAYER_IDX, m_diplomac_player_idx);

                            // improve relationship level by one level, if less than 'Friendly'
                            if(_players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] < 80)
                            {

                                _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] += 20;

                            }

                            _players[m_diplomac_player_idx].Dipl.Visible_Rel[HUMAN_PLAYER_IDX] = _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx];

                            Adjust_Diplomat_Modifiers(HUMAN_PLAYER_IDX, m_diplomac_player_idx);

                            Adjust_Diplomat_Modifiers(HUMAN_PLAYER_IDX, m_diplomac_player_idx);

                        }

                        if(m_IDK_diplomatic_order == do_IDK_war)
                        {

                            Declare_War(HUMAN_PLAYER_IDX, m_break_treaty_player3);

                        }

                        /* OGBUG  accepting a pact / alliance / peace proposal always pays the AI's prepared offer_gold and offer_spell to the human, even when the human accepted at once and they were never offered */
                        if(
                            (m_IDK_diplomatic_order == do_IDK_treaty1)
                            ||
                            (m_IDK_diplomatic_order == do_IDK_treaty2)
                            ||
                            (m_IDK_diplomatic_order == do_IDK_peace)
                        )
                        {

                            Player_Add_Gold(_current_player_idx, _players[HUMAN_PLAYER_IDX].Dipl.offer_gold[m_diplomac_player_idx]);

                            if(_players[HUMAN_PLAYER_IDX].Dipl.offer_spell[m_diplomac_player_idx] != spl_NONE)
                            {

                                Diplomacy_Player_Gets_Spell(HUMAN_PLAYER_IDX, _players[HUMAN_PLAYER_IDX].Dipl.offer_spell[m_diplomac_player_idx]);

                            }

                        }

                    }

                    if(
                        (input_field_idx > ST_UNDEFINED)
                        &&
                        (m_IDK_diplomatic_order == do_IDK_exchange)
                    )
                    {

                        /* OGBUG  only the human receives the chosen spell; the AI never receives the spell it asked for */
                        Diplomacy_Player_Gets_Spell(HUMAN_PLAYER_IDX, _players[HUMAN_PLAYER_IDX].Dipl.field_A8[((m_diplomac_player_idx * 6) + input_field_idx)]);

                    }

                }

                Adjust_Diplomat_Modifiers(HUMAN_PLAYER_IDX, m_diplomac_player_idx);

                if(
                    (m_IDK_diplomatic_order == do_IDK_treaty1)
                    ||
                    (m_IDK_diplomatic_order == do_IDK_treaty2)
                )
                {

                    _players[HUMAN_PLAYER_IDX].Dipl.treaty_modifier[m_diplomac_player_idx] -= (20 + Random(30));

                }

                if(m_IDK_diplomatic_order == do_IDK_peace)
                {

                    _players[HUMAN_PLAYER_IDX].Dipl.peace_modifier[m_diplomac_player_idx] -= (50 + Random(50));

                }

                if(m_IDK_diplomatic_order == do_IDK_exchange)
                {

                    _players[HUMAN_PLAYER_IDX].Dipl.exchange_modifier[m_diplomac_player_idx] -= (20 + Random(30));

                }

                leave_screen = ST_TRUE;

            }

        }
        else if(m_IDK_diplomatic_flag == 6)
        {

            Diplomacy_Display_Response(m_IDK_diplomatic_order, 3);

            leave_screen = ST_TRUE;

        }

        Clear_Fields();

        full_screen_field = INVALID_FIELD;

        if(
            (m_IDK_diplomatic_flag == 0)
            ||
            (m_IDK_diplomatic_flag == 1)
            ||
            (m_IDK_diplomatic_flag == 2)
        )
        {

            full_screen_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, str_empty_string__ovr086, ST_UNDEFINED);

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

    // BUGBUG  c&p error?  doesn't make sense here, will get overridden anyway
    current_screen = scr_Magic_Screen;

    Combat_Cache_Read();

    Cache_Graphics_Overland();

    Play_Background_Music();


}


// WZD o86p02
// MoO2  Module: NPCDIPLO  NPC_To_NPC_Treaty_Negotiations_()
/**
 * @brief Runs AI-to-AI diplomacy negotiations for treaties, spell trades, peace, and alliance war propagation.
 *
 * @details
 * This routine performs autonomous diplomacy updates between non-human wizards.
 * It evaluates each ordered AI pair, probabilistically opens a negotiation pass,
 * scores the current relationship, and may escalate the relationship into a
 * treaty, execute a spell exchange, improve relations, or conclude peace.
 *
 * Main negotiation pass:
 * 1. Early-out if the human is casting @c spl_Spell_Of_Return
 *    (preserved OGBUG: behavior, even though the routine is AI-vs-AI).
 * 2. Iterate ordered non-human player pairs with active fortresses and contact.
 * 3. Gate negotiation by a difficulty-scaled random check.
 * 4. Build @c Base_Score from hidden relation, visible relation, and defender
 *    personality bias; then derive @c Total_Score using additional random terms.
 * 5. Add treaty-state bonuses for existing pact/alliance.
 * 6. If the treaty score is high enough:
 *    - Prefer upgrading to @c DIPL_Alliance.
 *    - Otherwise attempt @c DIPL_WizardPact.
 * 7. If exchange interest is high enough, attempt a one-spell-for-one-spell trade
 *    using the first spell from each differential list.
 * 8. After treaty/trade handling, the common follow-up block may:
 *    - Apply a small positive relation gain.
 *    - Convert an active war into peace if the peace modifier threshold is met.
 *    - Decay diplomat modifiers.
 *    - Invoke downstream strategic helpers (@c G_DIPL_PickSides(),
 *      @c G_DIPL_NeedForWar(), @c DIPL_GetOffMyLawn()).
 *
 * Alliance propagation pass:
 * After the main pair loop, allied AIs may automatically join wars against a
 * third active wizard already at war with their ally.
 *
 * @return This function returns no value.
 *
 * @note The first pass iterates ordered pairs, so player A evaluating player B
 *       is distinct from player B evaluating player A.
 * @note Spell trades use the first entry returned by each differential spell
 *       list rather than optimizing by spell value.
 * @note The duplicate treaty-threshold comparison for alliance and wizard pact is
 *       preserved exactly from the live implementation.
 * @note This routine mutates diplomacy state, treaties, spells known, relation
 *       modifiers, and potentially war/peace state between AI players.
 * @note (OGBUG) The early return on the human player's @c spl_Spell_Of_Return
 *       is unrelated to AI-AI diplomacy but preserved as-is.
 *
 * @see Start_Treaty(), Declare_Peace(), Change_Relations(), G_DIPL_PickSides(),
 *      G_DIPL_NeedForWar(), DIPL_GetOffMyLawn(), Get_Differential_Spell_List(),
 *      Diplomacy_Player_Gets_Spell(), Declare_War()
 */
// void NPC_To_NPC_Treaty_Negotiations(void)
/* CLAUDE */  /* REBUG */  /* asm: the early exit is xor ax, ax / jmp @@Done (a return 0 in an int function); the other path falls into @@Done without a return value; no caller reads AX */
int16_t NPC_To_NPC_Treaty_Negotiations(void)
{
    int16_t Total_Score = 0;
    int16_t Other_Players_Count = 0;
    int16_t Trade_Spell_Count = 0;
    int16_t Trade_For_Spell = 0;
    int16_t First_Trade_Spell = 0;
    int16_t Player_Loop_Var = 0;
    int16_t Base_Score = 0;
    int16_t itr_players1 = 0;
    int16_t itr_players2 = 0;

    /* OGBUG  the whole AI-vs-AI pass (treaties, trades, peace, the war checks and the allied-war pass below) is skipped on every turn the human is casting Spell of Return */
    if(_players[_current_player_idx].casting_spell_idx == spl_Spell_Of_Return)

    {
        // return;
        return 0;  /* CLAUDE */  /* REBUG */  /* asm: xor ax, ax / jmp @@Done */
    }

    for(itr_players1 = 1; itr_players1 < _num_players; itr_players1++)
    {
        for(itr_players2 = 1; itr_players2 < _num_players; itr_players2++)
        {
            if(_FORTRESSES[itr_players1].active != ST_TRUE)
            {
                continue;
            }

            if(_FORTRESSES[itr_players2].active != ST_TRUE)
            {
                continue;
            }

            if(itr_players1 == itr_players2)
            {
                continue;
            }

            /* OGBUG  this rare roll (1 in 15 - 2 x difficulty per ordered pair) gates the whole body, so G_DIPL_PickSides(), G_DIPL_NeedForWar() and DIPL_GetOffMyLawn() at its end run only on the same roll as the friendly treaty / trade tests: AIs seldom break treaties with or declare war on each other */
            if(Random(15 - _difficulty * 2) != 1)
            {
                continue;
            }

            if(_players[itr_players1].Dipl.Contacted[itr_players2] != ST_TRUE)

            {
                continue;
            }

            Base_Score = _players[itr_players1].Dipl.Hidden_Rel[itr_players2] +
                         _players[itr_players1].Dipl.Visible_Rel[itr_players2] +
                         TBL_AI_PRS_IDK_Mod[_players[itr_players2].Personality];

            Total_Score = Base_Score + Random(100);
            Total_Score += (Random(5) + 3) * _difficulty;

            if(_players[itr_players1].Dipl.Dipl_Status[itr_players2] == DIPL_WizardPact)
            {
                Total_Score += 10;
            }

            if(_players[itr_players1].Dipl.Dipl_Status[itr_players2] == DIPL_Alliance)
            {
                Total_Score += 20;
            }

            /* OGBUG  alliance straight from no treaty or war (no Declare_Peace, pact skipped); with the identical test below, an existing alliance is turned back into a pact */
            if((Total_Score + _players[itr_players1].Dipl.treaty_modifier[itr_players2]) > 150)
            {
                if(_players[itr_players1].Dipl.Dipl_Status[itr_players2] != DIPL_Alliance)
                {
                    Start_Treaty(itr_players1, itr_players2, DIPL_Alliance);
                    goto loc_72C33;
                }
            }

            if((Total_Score + _players[itr_players1].Dipl.treaty_modifier[itr_players2]) > 150)
            {
                if(_players[itr_players1].Dipl.Dipl_Status[itr_players2] != DIPL_WizardPact)
                {
                    Start_Treaty(itr_players1, itr_players2, DIPL_WizardPact);
                    goto loc_72C33;
                }
            }

            /* OGBUG  inverted: a higher exchange_modifier makes a trade less likely */
            if((_players[itr_players1].Dipl.exchange_modifier[itr_players2] + 80) < Total_Score)
            {
                Trade_Spell_Count = Get_Differential_Spell_List(itr_players1, itr_players2, 1, m_exchange_spell_list);
                First_Trade_Spell = m_exchange_spell_list[0];

                Other_Players_Count = Get_Differential_Spell_List(itr_players2, itr_players1, 1, m_exchange_spell_list);
                Trade_For_Spell = m_exchange_spell_list[0];

                if(Trade_Spell_Count > 0 && Other_Players_Count > 0)
                {
                    /* OGBUG  crossed: each wizard is given a spell it already knows; nothing is traded, but Player_Gets_Spell() still lowers som_research_cost */
                    Diplomacy_Player_Gets_Spell(itr_players2, First_Trade_Spell);
                    Diplomacy_Player_Gets_Spell(itr_players1, Trade_For_Spell);
                }
            }

        loc_72C33:
            if(Random(20) == 1)
            {
                Change_Relations(Random(5) + 5, itr_players1, itr_players2, 1, 0, 0);
            }

            if(_players[itr_players1].Dipl.Dipl_Status[itr_players2] == DIPL_War)
            {
                /* OGBUG  the AI-vs-AI peace test adds m_diplomsg_1_record_sub_number, the diplomacy screen's message-variant index (ST_UNDEFINED or a Random() variant) left from the last diplomacy screen */
                if((m_diplomsg_1_record_sub_number + _players[itr_players1].Dipl.peace_modifier[itr_players2]) > 100)
                {
                    Declare_Peace(itr_players1, itr_players2);
                }
            }

            Adjust_Diplomat_Modifiers(itr_players1, itr_players2);
            Adjust_Diplomat_Modifiers(itr_players1, itr_players2);
            Adjust_Diplomat_Modifiers(itr_players1, itr_players2);

            G_DIPL_PickSides();
            G_DIPL_NeedForWar(itr_players1, itr_players2);

            if(_players[itr_players1].Dipl.Dipl_Action[itr_players2] == 0)
            {
                DIPL_GetOffMyLawn(itr_players2, itr_players1);
            }
        }
    }

    for(itr_players1 = 1; itr_players1 < _num_players; itr_players1++)
    {
        for(itr_players2 = 1; itr_players2 < _num_players; itr_players2++)
        {
            if(_FORTRESSES[itr_players1].active != ST_TRUE)
            {
                continue;
            }

            if(_FORTRESSES[itr_players2].active != ST_TRUE)
            {
                continue;
            }

            if(_players[itr_players1].Dipl.Dipl_Status[itr_players2] == DIPL_Alliance)
            {
                for(Player_Loop_Var = 1; Player_Loop_Var < _num_players; Player_Loop_Var++)
                {
                    if(_FORTRESSES[Player_Loop_Var].active != ST_TRUE)
                    {
                        continue;
                    }

                    if(Player_Loop_Var == itr_players1)
                    {
                        continue;
                    }

                    if(Player_Loop_Var == itr_players2)
                    {
                        continue;
                    }

                    if(_players[Player_Loop_Var].Dipl.Dipl_Status[itr_players1] == DIPL_War)
                    {
                        if(_players[Player_Loop_Var].Dipl.Dipl_Status[itr_players2] != DIPL_War)
                        {
                            Declare_War(itr_players2, Player_Loop_Var);
                        }
                    }
                }
            }
        }
    }

    /* HACK */  /* CLAUDE */  /* no return value in the asm here (AX is whatever the last call left); no caller reads it */
    return 0;
}


// WZD o86p03
/*
handles {45,46,47,48,49}

*/
static int16_t DIPL_sub_72DB6__STUB(void)
{
    int16_t string = 0;
    int16_t var_A = 0;
    int16_t var_8 = 0;
    int16_t full_screen_field = 0;
    int16_t var_4 = 0;
    int16_t _variable = 0;
    int16_t itr = 0;  // _SI_

    Get_Diplomacy_Statement(m_IDK_diplomatic_order, m_diplomac_player_idx);

    var_8 = INVALID_FIELD;

    for(itr = 0; itr < 7; itr++)
    {

        
        stu_strcpy(G_Some_DIPL_Allocs_7[itr], asc_3B9DB);


    }

    switch(m_IDK_diplomatic_order)
    {

        case do_IDK_treaty1:
        case do_IDK_treaty2:
        case do_IDK_peace:
        {

            stu_strcat(G_Some_DIPL_Allocs_7[0], aAccept_0);

            stu_strcat(G_Some_DIPL_Allocs_7[1], aReject_0);

            G_Some_DIPL_Allocs_7[2] = 0;

            var_4 = 1;

        } break;

        case do_IDK_war:
        {

            stu_strcat(G_Some_DIPL_Allocs_7[0], aAgree);

            stu_strcat(G_Some_DIPL_Allocs_7[1], aForgetIt_1);

            G_Some_DIPL_Allocs_7[2] = 0;

            var_4 = 1;

        } break;

        case do_IDK_exchange:
        {

        } break;

    }









    if(_variable == ST_UNDEFINED)
    {

        _variable = var_4;

    }

    if(_variable == var_8)
    {

        _variable = ST_UNDEFINED;

    }

    Assign_Auto_Function(Diplomacy_Screen_Draw__WIP, 3);

    Set_Input_Delay(1);

    return _variable;

}

// WZD o86p04
// DIPL_sub_72DB6_Draw()


// WZD o86p05
// MoO2  Module: DIP-SCRN  Get_Diplomacy_Statement_();  ... Get_Text_Message_()
/*
...loads DIPLOMSG data and message...

*/
void Get_Diplomacy_Statement(int16_t diplomsg_0_record_number, int16_t player_idx)
{
    char string[(LEN_STRING + 1)] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t diplomsg_1_record_number = 0;
    int16_t var_4 = 0;
    int16_t itr = 0;
    int16_t pos = 0;
    int16_t code = 0;  // _BX_

    pos = 0;

    var_4 = 0;

    diplomsg_1_record_number = diplomsg_0_record_number;

    if(m_diplomsg_1_record_sub_number != ST_UNDEFINED)
    {

        stu_strcpy(m_diplomacy_message, G_DIPL_ComposedMessage);

    }
    else
    {

        LBX_Load_Data_Static(
            diplomsg_lbx_file__ovr86,
            0,
            (SAMB_ptr)&m_diplomacy_message_record_data,
            diplomsg_0_record_number,
            1,
            6
        );

        if(_num_players == 2)
        {

            /* OGBUG  two-player game: Random(IDK_count) - 2 gives -1 .. IDK_count - 2, floored to 0 below - the last variant never appears and variant 0 is twice as likely */
            m_diplomsg_1_record_sub_number = (Random(m_diplomacy_message_record_data.IDK_count) - 2);

        }
        else
        {

            m_diplomsg_1_record_sub_number = (Random(m_diplomacy_message_record_data.IDK_count) - 1);

        }

        SETMIN(m_diplomsg_1_record_sub_number, 0);
    
        LBX_Load_Data_Static(
            diplomsg_lbx_file__ovr86,
            1,
            (SAMB_ptr)G_DIPL_ComposedMessage,
            m_diplomsg_1_record_sub_number + (diplomsg_1_record_number * 15),
            1,
            200
        );

        for(itr = 0; ((itr < 200) && (G_DIPL_ComposedMessage[itr] != 0)); itr++)
        {

            m_diplomacy_message[pos] = G_DIPL_ComposedMessage[itr];

            code = ((uint8_t)m_diplomacy_message[pos] - 128);

            if((uint16_t)code > 20)
            {
                pos++;
            }
            else
            {

                switch(code)
                {

                    case 0:
                    {

                        m_diplomacy_message[pos] = 0;

                        stu_strcat(m_diplomacy_message, _players[HUMAN_PLAYER_IDX].name);

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 1:
                    {

                        m_diplomacy_message[pos] = 0;

                        stu_strcat(m_diplomacy_message, _players[player_idx].name);

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 2:
                    {

                        m_diplomacy_message[pos] = 0;

                        stu_strcat(m_diplomacy_message, _players[_players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[player_idx]].name);

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 3:
                    {

                        String_Copy_Far(&m_diplomacy_message[pos], _CITIES[_players[HUMAN_PLAYER_IDX].Dipl.DA_City[player_idx]].name);

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 4:
                    {

                        stu_strcpy(&m_diplomacy_message[pos], _city_size_names[_CITIES[_players[HUMAN_PLAYER_IDX].Dipl.DA_City[player_idx]].size]);

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 5:
                    {

                        m_diplomacy_message[pos] = 0;

                        stu_strcat(m_diplomacy_message, _players[_players[HUMAN_PLAYER_IDX].Dipl.break_treaty[player_idx]].name);

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 6:
                    {

                        pos++;

                    } break;

                    case 7:
                    {

                        m_diplomacy_message[pos] = 0;

                        stu_strcat(m_diplomacy_message, _players[_players[HUMAN_PLAYER_IDX].Dipl.niu_attack_bounty[player_idx]].name);

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 8:
                    {

                        m_diplomacy_message[pos] = 0;

                        stu_itoa((int16_t)GET_2B_OFS(_players[HUMAN_PLAYER_IDX].Dipl.field_114, (player_idx * 2)), string, 10);
                        assert(string[30] == 0);

                        stu_strcat(m_diplomacy_message, string);

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 9:
                    {

                        m_diplomacy_message[pos] = 0;

                        String_Copy_Far(string, spell_data_table[(int16_t)GET_2B_OFS(_players[HUMAN_PLAYER_IDX].Dipl.niu_au_want_tech, (player_idx * 2))].name);
                        assert(string[30] == 0);

                        stu_strcat(m_diplomacy_message, string);

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 10:
                    {

                        m_diplomacy_message[pos] = 0;

                        stu_itoa((int16_t)GET_2B_OFS(_players[HUMAN_PLAYER_IDX].Dipl.field_114, (player_idx * 2)), string, 10);
                        assert(string[30] == 0);

                        stu_strcat(m_diplomacy_message, string);

                        stu_strcat(m_diplomacy_message, aGold_2);

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 11:
                    {

                        m_diplomacy_message[pos] = 0;

                        if(_players[HUMAN_PLAYER_IDX].Dipl.Broken_Treaty[player_idx] == 2)
                        {

                            stu_strcat(m_diplomacy_message, aAlliance_0);

                        }
                        else if(_players[HUMAN_PLAYER_IDX].Dipl.Broken_Treaty[player_idx] == 1)
                        {

                            stu_strcat(m_diplomacy_message, aWizardPact_0);
                            
                        }
                        else
                        {

                            stu_strcat(m_diplomacy_message, aTreaty);

                        }

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 12:
                    {

                        m_diplomacy_message[pos] = 0;

                        String_Copy_Far(string, spell_data_table[_players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[player_idx]].name);
                        assert(string[30] == 0);

                        stu_strcat(m_diplomacy_message, string);

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 13:
                    {

                        m_diplomacy_message[pos] = 0;

                        stu_strcat(m_diplomacy_message, *_unit_type_table[_players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[player_idx]].name);

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 14:
                    {

                        m_diplomacy_message[pos] = 0;

                        stu_strcat(m_diplomacy_message, An(*_unit_type_table[_players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[player_idx]].name));

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 15:
                    {

                        m_diplomacy_message[pos] = 0;

                        if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[player_idx] == 2)
                        {

                            stu_strcat(m_diplomacy_message, aAlliance_0);

                        }
                        else if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[player_idx] == 1)
                        {

                            stu_strcat(m_diplomacy_message, aWizardPact_0);
                            
                        }
                        else
                        {

                            stu_strcat(m_diplomacy_message, aTreaty);

                        }

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 16:
                    {

                        m_diplomacy_message[pos] = 0;

                        stu_itoa((1400 + (_turn / 12)), string, 10);
                        assert(string[30] == 0);

                        stu_strcat(m_diplomacy_message, string);

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 17:
                    {

                        m_diplomacy_message[pos] = 0;

                        String_Copy_Far(string, spell_data_table[Spell_Index].name);
                        assert(string[30] == 0);

                        stu_strcat(m_diplomacy_message, string);

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 18:
                    {

                        m_diplomacy_message[pos] = 0;

                        stu_strcat(m_diplomacy_message, _players[Target_Player].name);

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 19:
                    {

                        m_diplomacy_message[pos] = 0;

                        stu_strcat(m_diplomacy_message, _players[m_break_treaty_player3].name);

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                    case 20:
                    {

                        m_diplomacy_message[pos] = 0;

                        stu_itoa(word_42E66, string, 10);
                        assert(string[30] == 0);

                        stu_strcat(m_diplomacy_message, string);

                        pos = (int16_t)strlen(m_diplomacy_message);

                    } break;

                }

            }

        }

        m_diplomacy_message[pos] = 0;

        /* the copy isn't pointless. stu_strcpy(G_DIPL_ComposedMessage, m_diplomacy_message) saves the finished message, with the names and numbers filled in. The cache path at DIPLOMAC.c:5318 later reads that copy back. The callers at DIPLOMAC.c:643, 1582, 3049, 4703 and 4776 set the index to -1 to force a fresh compose. */
        stu_strcpy(G_DIPL_ComposedMessage, m_diplomacy_message);  /* this copy fills the cache that the != ST_UNDEFINED path reads */ 

        Start_Diplomacy_Music(m_diplomacy_message_record_data.IDK_mood);

        m_diplomacy_message_IDK_group = m_diplomacy_message_record_data.IDK_group;

    }

}



/*
    WIZARDS.EXE  ovr087
*/

// WZD o87p01
// MoO2  Module: DIPLOMAC  Determine_First_Contacts_()
/**
 * @brief Finalizes newly discovered wizard contacts for the human player.
 *
 * Scans every non-human player and looks for opponents that have already been
 * marked as contacted but whose contact sequence has not yet been advanced.
 * For each such player, this function moves the human-side contact progress to
 * the first active stage and initializes the diplomatic status between the two
 * players to @c DIPL_NoTreaty on both sides.
 *
 * This establishes the baseline diplomacy state used by later AI-to-human
 * diplomacy processing after first contact has been detected elsewhere.
 *
 * @note This function has no parameters and no return value; it operates
 *       directly on the global player diplomacy state.
 */
void Determine_First_Contacts(void)
{
    int16_t itr = 0;
    for(itr = 1; itr < _num_players; itr++)
    {
        if(
            (_players[_current_player_idx].Dipl.contact_stage[itr] == pcs_Unmet)
            &&
            (_players[_current_player_idx].Dipl.Contacted[itr] == ST_TRUE)
        )
        {
            _players[_current_player_idx].Dipl.contact_stage[itr] = pcs_Greeted;
            /* OGBUG  contact is made mid-turn (Contacted[] set both ways in Update_Scouted_And_Contacted() / Contact_Other_Player()), but contact_stage stays pcs_Unmet until here: any Dipl_Status set in the turn of first contact (e.g. a treaty) is reset to DIPL_NoTreaty both ways */
            _players[_current_player_idx].Dipl.Dipl_Status[itr] = DIPL_NoTreaty;
            _players[itr].Dipl.Dipl_Status[_current_player_idx] = DIPL_NoTreaty;
        }
    }
}


// WZD o87p02
// MoO2  Module: NPCDIPLO  NPC_To_Human_Diplomacy_()
void NPC_To_Human_Diplomacy(void)
{
    int16_t Total_Score = 0;
    int16_t Lowest_Interest = 0;
    int16_t player_idx = 0;
    int16_t da_strength = 0;
    if(_players[_current_player_idx].casting_spell_idx == spl_Spell_Of_Return)
    {
        return;
    }
    Limit_Treaty_Modifiers();  // MoO2  OON XREF: Determine_Diplomacy_Messages_()
    for(player_idx = 1; player_idx < _num_players; player_idx++)
    {
        if(_FORTRESSES[player_idx].active == ST_FALSE)
        {
            _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] = do_None;
            continue;
        }
        else if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[player_idx] == DIPL_Crusade)
        {
            _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] = do_None;
        }
        else if(_players[HUMAN_PLAYER_IDX].Dipl.contact_stage[player_idx] == pcs_Greeted)
        {
            _players[HUMAN_PLAYER_IDX].Dipl.contact_stage[player_idx] = pcs_Established;
            _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] = (do_Greeting + _players[player_idx].Personality);
        }
        else if(_players[HUMAN_PLAYER_IDX].Dipl.niu_bounty_collect[player_idx] != 0)
        {
            _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] = do_2;
        }
        else
        {
            if(
                (_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] != do_39)
                &&
                (_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] != do_30)
                &&
                (_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] != do_40)
                &&
                (_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] != do_41)
            )
            {
                m_diplomac_player_idx = player_idx;
                Lowest_Interest = Find_Worst_Modifier();
                Total_Score = (
                    _players[HUMAN_PLAYER_IDX].Dipl.Hidden_Rel[m_diplomac_player_idx]
                    + _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]
                    + Lowest_Interest
                    + TBL_AI_PRS_IDK_Mod[_players[m_diplomac_player_idx].Personality]
                );
                /* OGBUG  inverted: the offense / peace / proposal logic below runs only when bias + 2 x relation + lowest interest + personality mod is BELOW -100; for every friendlier AI the pending message is just cleared to do_None */
                if((Total_Score + _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]) >= -100)
                {
                    _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] = do_None;
                }
                else
                {
                    da_strength = (_players[HUMAN_PLAYER_IDX].Dipl.DA_Strength[player_idx] * 2);
                    if(_players[HUMAN_PLAYER_IDX].Dipl.DA_Strength[player_idx] < 0)
                    {
                        if(
                            (_players[HUMAN_PLAYER_IDX].Dipl.field_126[player_idx] > 0)
                            ||
                            (Random(75) < abs(da_strength))
                        )
                        {
                            DIPL_HumanWarOrPeace(player_idx);
                        }
                        else
                        {
                            _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] = do_None;
                        }
                    }
                    else
                    {
                        if(
                            (_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[player_idx] == DIPL_War)
                            &&
                            ((30 + Random(100)) < _players[HUMAN_PLAYER_IDX].Dipl.peace_modifier[player_idx])
                        )
                        {
                            DIPL_HumanWarOrPeace(player_idx);
                        }
                        else
                        {
                            if(_players[HUMAN_PLAYER_IDX].Dipl.field_126[player_idx] == 0)
                            {
                                da_strength += 3;
                            }
                            if(
                                (Random(100) < da_strength)
                                &&
                                (Random(4) == 1)
                            )
                            {
                                if(_players[HUMAN_PLAYER_IDX].Dipl.field_126[player_idx] > 0)
                                {
                                    /* ¿ OGBUG: doesn't set a diplomatic order ? */
                                    _players[HUMAN_PLAYER_IDX].Dipl.field_126[player_idx] = 0;
                                }
                                else
                                {
                                    NPC_Diplo_s74420(HUMAN_PLAYER_IDX, player_idx);
                                }
                            }
                            else
                            {
                                NPC_Required_Alliances(player_idx);
                            }
                        }
                    }
                }
            }
        }
        if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] == do_None)
        {
            G_DIPL_NeedForWar(HUMAN_PLAYER_IDX, player_idx);
        }
        if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] == do_None)
        {
            DIPL_GetOffMyLawn(player_idx, HUMAN_PLAYER_IDX);
        }
    }
}


// WZD o87p03
void G_DIPL_NeedForWar(int16_t Player_1, int16_t Player_2)
{
    int32_t Defense_Strength;
    int32_t Invasion_Strength;
    int16_t Superiority_Modifier;
    int16_t LoopVar_2;
    int16_t LoopVar_1;
    int16_t Human_Wars;
    int16_t Comparison_Score;

    if(AI_Dipl_Unset_0) {
        for(LoopVar_1 = 1; LoopVar_1 < _num_players; LoopVar_1++) {
            for(LoopVar_2 = 1; LoopVar_2 < _num_players; LoopVar_2++) {
                if(LoopVar_1 != LoopVar_2) {
                    _players[LoopVar_1].Dipl.Dipl_Status[LoopVar_2] = DIPL_Alliance;
                }
            }
        }
        return;
    }

    if(Player_1 == _current_player_idx && _turn < 100)
    {
        return;
    }

    if(_players[Player_1].Dipl.Dipl_Status[Player_2] >= DIPL_War || _players[Player_1].Dipl.Contacted[Player_2] != 1)
    {
        return;
    }

    if(_FORTRESSES[Player_1].active == 0)
    {
        return;
    }

    if(_players[Player_2].Personality == PRS_Chaotic)
    {
        if(Random(300) <= _difficulty) {
            if(Player_1 != 0 || _players[Player_1].peace_duration[Player_2] < 1)
            {
                _players[Player_1].Dipl.Dipl_Action[Player_2] = 40;
                _players[Player_1].Dipl.DA_Strength[Player_2] = 2000;
                Declare_War(Player_2, Player_1);
                goto loc_73CC0;
            }
        }
    }

    if(Random(20) == 1)
    {
        if(_players[Player_2].Objective == OBJ_Militarist || _players[Player_2].Objective == OBJ_Expansionist)
        {
            if(_players[Player_2].Personality != PRS_Lawful) {
                if(Player_1 != 0 || _players[Player_1].peace_duration[Player_2] < 1) {
                    Invasion_Strength = 0L;
                    Invader_Army_Strength_Comparison(Player_1, Player_2, &Invasion_Strength, &Defense_Strength);

                    if(Defense_Strength > Invasion_Strength) {
                        if(Invasion_Strength <= 0L) {
                            Superiority_Modifier = -100;
                        } else {
                            Superiority_Modifier = (-(int16_t)((Defense_Strength * 100) / Invasion_Strength) - 50);
                        }
                    } else {
                        if(Defense_Strength <= 0L) {
                            Superiority_Modifier = 100;
                        } else {
                            Superiority_Modifier = ((int16_t)((Invasion_Strength * 100) / Defense_Strength) - 50);
                        }
                    }

                    Comparison_Score = _players[Player_1].Dipl.Visible_Rel[Player_2]
                                     + Superiority_Modifier
                                     + TBL_AI_PRS_IDK_Mod[_players[Player_2].Personality]
                                     + _players[Player_1].Dipl.Hidden_Rel[Player_2];

                    if(Comparison_Score <= -150)
                    {
                        G_DIPL_SuperiorityWar(Player_1, Player_2);
                    }
                }
            }
        }
    }

    if(Random(20) == 1) {
        if(Player_1 != 0 || _players[Player_1].peace_duration[Player_2] < 1) {
            Invasion_Strength = 0L;
            Invader_Army_Strength_Comparison(Player_1, Player_2, &Invasion_Strength, &Defense_Strength);

            if(Defense_Strength > Invasion_Strength) {
                if(Invasion_Strength <= 0L) {
                    Superiority_Modifier = -100;
                } else {
                    Superiority_Modifier = (-(int16_t)((Defense_Strength * 100) / Invasion_Strength) - 50);
                }
            } else {
                if(Defense_Strength <= 0L) {
                    Superiority_Modifier = 100;
                } else {
                    Superiority_Modifier = ((int16_t)((Invasion_Strength * 100) / Defense_Strength) - 50);
                }
            }

            Comparison_Score = _players[Player_1].Dipl.Visible_Rel[Player_2]
                             + Superiority_Modifier
                             + TBL_AI_PRS_IDK_Mod[_players[Player_2].Personality]
                             + _players[Player_1].Dipl.Hidden_Rel[Player_2];

            if(Comparison_Score <= -150) {
                /* CLAUDE */  /* REBUG */  G_DIPL_SuperiorityWar(Player_1, Player_2);  /* asm: push player2_idx / push player1_idx - (Player_1, Player_2); was (Player_2, Player_1) */
            }
        }
    }

loc_73CC0:
    /* OGBUG  tests the HUMAN's war list (_players[0].Dipl_Status[k]), not Player_1's: Player_2 allied to some k the human is at war with moves (1 in 10) to war with Player_1, even when Player_1 is another AI */
    for(LoopVar_1 = 1; LoopVar_1 < _num_players; LoopVar_1++) {
        if(_players[0].Dipl.Dipl_Status[LoopVar_1] == DIPL_War) {
            if(_players[Player_2].Dipl.Dipl_Status[LoopVar_1] == DIPL_Alliance) {
                if(_players[Player_1].peace_duration[Player_2] < 1) {
                    if(Random(10) == 1) {
                        if(_players[Player_2].Dipl.Dipl_Status[Player_1] != DIPL_Alliance) {
                            /* CLAUDE */  /* REBUG */  G_DIPL_SuperiorityWar(Player_1, Player_2);  /* asm: push player2_idx / push player1_idx - (Player_1, Player_2); was (Player_2, Player_1) */
                        }
                    }
                }
            }
        }
    }

    if(_difficulty >= 2) {
        /* OGBUG  counts the HUMAN's wars for every pair: while the human has fewer wars than the difficulty level, any Player_2 with relation < -30 to Player_1 and no peace timer rolls Random(15) <= -relation / 10 to move to war with Player_1, even when both are AIs */
        Human_Wars = 0;
        for(LoopVar_1 = 1; LoopVar_1 < _num_players; LoopVar_1++) {
            if(_players[0].Dipl.Dipl_Status[LoopVar_1] >= DIPL_War) {
                Human_Wars++;
            }
        }

        if(Human_Wars < _difficulty) {
            if(_players[Player_1].Dipl.Visible_Rel[Player_2] < -30) {
                if(_players[Player_1].peace_duration[Player_2] < 1) {
                    Comparison_Score = -(_players[Player_1].Dipl.Visible_Rel[Player_2]) / 10;
                    if(Random(15) <= Comparison_Score) {
                        /* CLAUDE */  /* REBUG */  G_DIPL_SuperiorityWar(Player_1, Player_2);  /* asm: push player2_idx / push player1_idx - (Player_1, Player_2); was (Player_2, Player_1) */
                    }
                }
            }
        }
    }

}


// WZD o87p04
void G_DIPL_SuperiorityWar(int16_t Player_1, int16_t Player_2)
{
    if(_players[Player_1].Dipl.Dipl_Status[Player_2] == DIPL_Alliance ||
        _players[Player_1].Dipl.Dipl_Status[Player_2] == DIPL_WizardPact)
    {
        if(_difficulty < 2)
        {
            return;
        }

        if(_players[Player_1].Dipl.Dipl_Status[Player_2] == DIPL_Alliance)
        {
            if(Random(4) == 1 || (Random(2) == 1 && _players[Player_2].Objective == OBJ_Expansionist))
            {
                Change_Relations(-10000, Player_1, Player_2, 30, 0, 0);
                Break_Treaties(Player_2, Player_1);
                if(_players[Player_1].Dipl.Visible_Rel[Player_2] > 30)
                {
                    _players[Player_1].Dipl.Visible_Rel[Player_2] = 30;
                    _players[Player_2].Dipl.Visible_Rel[Player_1] = 30;
                }
            }
        }
        else if(_players[Player_1].Dipl.Dipl_Status[Player_2] == DIPL_WizardPact)
        {
            if(Random(2) == 1 || _players[Player_2].Objective == OBJ_Expansionist)
            {
                Change_Relations(-10000, Player_1, Player_2, 30, 0, 0);
                Break_Treaties(Player_2, Player_1);
                if(_players[Player_1].Dipl.Visible_Rel[Player_2] > 30)
                {
                    _players[Player_1].Dipl.Visible_Rel[Player_2] = 30;
                    _players[Player_2].Dipl.Visible_Rel[Player_1] = 30;
                }
            }
        }
    }
    else
    {
        if(_players[Player_1].peace_duration[Player_2] < 1)
        {
            if(_players[Player_1].Dipl.Visible_Rel[Player_2] < 0)
            {
                Change_Relations(-10000, Player_1, Player_2, 39, 0, 0);
            }
            else
            {
                Change_Relations(-10000, Player_1, Player_2, 41, 0, 0);
            }
            Declare_War(Player_2, Player_1);
        }
    }
}


// WZD o87p05
// MoO2  Module: NPCDIPLO  NPC_Required_Alliances_()
void NPC_Required_Alliances(int16_t target_player_idx)
{
    int16_t player1_idx = 0;
    int16_t player2_idx = 0;
    int16_t matched_player_idx = 0;
    int16_t itr_players = 0;
    player1_idx = HUMAN_PLAYER_IDX;
    player2_idx = target_player_idx;
    if(_players[player1_idx].Dipl.Dipl_Status[player2_idx] == DIPL_Alliance)
    {
        if(Random(10) == 1)  /* 10% */
        {
            matched_player_idx = ST_UNDEFINED;
            for(itr_players = 0; itr_players < _num_players; itr_players++)
            {
                if(itr_players != player1_idx && itr_players != player2_idx)
                {
                    if(_players[itr_players].Dipl.Dipl_Status[player2_idx] == DIPL_War)
                    {
                        if(_players[player1_idx].Dipl.Contacted[itr_players] == ST_TRUE)
                        {
                            if(_players[player1_idx].Dipl.Dipl_Status[itr_players] != DIPL_War)
                            {
                                matched_player_idx = itr_players;
                            }
                        }
                    }
                }
            }
            _players[player1_idx].Dipl.break_treaty[player2_idx] = matched_player_idx;
        }
    }
    /* OGBUG  the picked war target is stored in war_request (break_treaty), then the pending message is CLEARED instead of set to the 'join my war' audience (0x2F): nothing else queues 0x2F for the human, so that audience is unreachable */
    _players[player1_idx].Dipl.Dipl_Action[player2_idx] = 0;
}


// WZD o87p06
// MoO2  Module: DIP-SCRN  Resolve_Delayed_Diplomacy_Orders_()
void Resolve_Delayed_Diplomacy_Orders(void)
{
    int16_t itr = 0;
    if(_players[_current_player_idx].casting_spell_idx == spl_Spell_Of_Return)
    {
        return;
    }
    for(itr = 1; itr < _num_players; itr++)
    {
        if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[itr] != do_None)
        {
            m_diplomac_player_idx = itr;
            Npc_Diplomacy_Screen();
        }
        _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[itr] = do_None;
    }
}


// WZD o87p07
void DIPL_GetOffMyLawn(int16_t player1, int16_t player2)
{
    int16_t Random_DA_Value;
    int16_t Violated_City_Index;
    int16_t Have_Violation;
    int16_t Unit_City_Distance;
    int16_t City_LoopVar;
    int16_t Unit_LoopVar;

    if(_players[player2].Dipl.Dipl_Status[player1] == DIPL_War) {
        return;
    }

    Violated_City_Index = ST_UNDEFINED;
    Have_Violation = 0;
    Unit_LoopVar = 0;

    for(Unit_LoopVar = 0; Unit_LoopVar < _units; Unit_LoopVar++) {
        if(_UNITS[Unit_LoopVar].owner_idx == player2) {
            if(_UNITS[Unit_LoopVar].owner_idx != ST_UNDEFINED) {
                if(_UNITS[Unit_LoopVar].owner_idx != NEUTRAL_PLAYER_IDX) {
                    if((_unit_type_table[_UNITS[Unit_LoopVar].type].Abilities & UA_CREATEOUTPOST) == 0) {
                        if(_unit_type_table[_UNITS[Unit_LoopVar].type].Construction < 1) {
                            for(City_LoopVar = 0; City_LoopVar < _cities; City_LoopVar++) {
                                if(_CITIES[City_LoopVar].owner_idx == player1) {
                                    if(_UNITS[Unit_LoopVar].wp == _CITIES[City_LoopVar].wp || _UNITS[Unit_LoopVar].in_tower == ST_TRUE) {
                                        Unit_City_Distance = Delta_XY_With_Wrap(
                                            (int16_t)_UNITS[Unit_LoopVar].wx,
                                            (int16_t)_UNITS[Unit_LoopVar].wy,
                                            (int16_t)_CITIES[City_LoopVar].wx,
                                            (int16_t)_CITIES[City_LoopVar].wy,
                                            WORLD_WIDTH
                                        );
                                        if(Unit_City_Distance < 3) {
                                            Violated_City_Index = City_LoopVar;
                                            Have_Violation = ST_TRUE;
                                        }
                                    }
                                }
                                if(Have_Violation != 0) {
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        if(Have_Violation != 0) {
            break;
        }
    }

    if(Violated_City_Index == ST_UNDEFINED) {
        _players[player2].Dipl.G_Warning_Progress[player1] = 0;
        return;
    }

    if(_players[player2].Dipl.Dipl_Status[player1] == DIPL_Alliance ||
        _players[player2].Dipl.Dipl_Status[player1] == DIPL_WizardPact) 
    {
        if(_players[player2].Dipl.G_Warning_Progress[player1] == 0) {
            Random_DA_Value = Random(5);
            Change_Relations(-Random_DA_Value, player2, player1, 73, Violated_City_Index, 0);
            _players[player2].Dipl.G_Warning_Progress[player1] = 1;
        } else if(_players[player2].Dipl.G_Warning_Progress[player1] == 1) {
            _players[player2].Dipl.G_Warning_Progress[player1] = 2;
        } else {
            _players[player2].Dipl.G_Warning_Progress[player1] = 0;
            Change_Relations(-5, player2, player1, 5, Violated_City_Index, 0);
            Break_Treaties(player1, player2);
            Break_Treaties(player2, player1);
        }
    } else {
        _players[player2].Dipl.G_Warning_Progress[player1] = 0;
        if(Random(100) < 6) {
            int relations_type;
            Random_DA_Value = Random(5);
            if(Random(5) == 1) {
                relations_type = 73;
            } else {
                relations_type = 0;
            }
            Change_Relations(-Random_DA_Value, player2, player1, relations_type, Violated_City_Index, 0);
        }
    }

}


// WZD o87p08
/*
¿ "Once a peace treaty is in place, the opposing wizard will not attack you for about the next dozen turns." ?
*/
void Decrease_Peace_Duration(void)
{
    int16_t itr_players1 = 0;
    int16_t itr_players2 = 0;
    for(itr_players1 = 0; itr_players1 < _num_players; itr_players1++)
    {
        for(itr_players2 = 0; itr_players2 < _num_players; itr_players2++)
        {
            if(itr_players1 == itr_players2)
            {
                continue;
            }
            /* OGBUG  AI_Next_Turn() also decrements the AI's own row (_players[player_idx].peace_duration[], when != 0), so an AI's Random(6) + 7 turn no-attack truce runs at double speed; only the human's row ticks once a turn */
            _players[itr_players1].peace_duration[itr_players2] -= 1;
            if(_players[itr_players1].peace_duration[itr_players2] < 0)
            {
                _players[itr_players1].peace_duration[itr_players2] = 0;
            }
        }
    }
}


// WZD o87p09
/**
 * @brief Performs end-of-turn recovery for per-opponent diplomacy modifiers.
 *
 * @details
 * Iterates all ordered player pairs and, for each non-self pairing, increments
 * these directional diplomacy modifiers by a fixed 10 points when below 100:
 * - @c Dipl.treaty_modifier
 * - @c Dipl.exchange_modifier
 * - @c Dipl.peace_modifier
 *
 * This provides gradual turn-based normalization of temporary negative or
 * reduced modifier values accumulated during diplomacy events.
 *
 * @return This function returns no value.
 *
 * @note Updates are directional (@c player1 -> @c player2) and therefore each
 *       pair may be adjusted twice across the full nested-loop pass.
 * @note No explicit upper clamp is needed here because each modifier is only
 *       incremented when strictly less than @c 100.
 *
 * @see Change_Relations(), Modifier_Diplomacy_Adjustments(),
 *      Decrease_Peace_Duration()
 */
void End_Of_Turn_Diplomacy_Adjustments(void)
{
    int16_t itr1 = 0;
    int16_t itr2 = 0;
    for(itr1 = 0; itr1 < _num_players; itr1++)
    {
        for(itr2 = 0; itr2 < _num_players; itr2++)
        {
            if(itr1 != itr2)
            {
                if(_players[itr1].Dipl.treaty_modifier[itr2] < 100)
                {
                    _players[itr1].Dipl.treaty_modifier[itr2] += 10;
                }
                if(_players[itr1].Dipl.exchange_modifier[itr2] < 100)
                {
                    _players[itr1].Dipl.exchange_modifier[itr2] += 10;
                }
                if(_players[itr1].Dipl.peace_modifier[itr2] < 100)
                {
                    _players[itr1].Dipl.peace_modifier[itr2] += 10;
                }
            }
        }
    }
}



/*
    WIZARDS.EXE  ovr088
*/

// WZD o88p01
// IDK_Dipl_Npc_Demand__STUB()
void NPC_Diplo_s74420(int16_t player1_idx, int16_t player2_idx)
{
/*
IDK_modifier= word ptr -14h
var_12= word ptr -12h
min_value= word ptr -10h
var_E= word ptr -0Eh
divisor= dword ptr -0Ch
var_8= word ptr -8
input= word ptr -6
IDK_relations= word ptr -4
IDK_message= word ptr -2
*/
    int16_t IDK_message = 0;
    int16_t IDK_relations = 0;
    int16_t input = 0;
    int16_t var_8 = 0;
    int32_t divisor = 0;
    int16_t var_E = 0;
    int16_t min_value = 0;
    int16_t var_12 = 0;
    int16_t IDK_modifier = 0;
    int16_t itr_players = 0;

    if(_players[_current_player_idx].Dipl.Contacted[player2_idx] == 0 ||
        _players[player2_idx].Dipl.Contacted[_current_player_idx] == 0)
    {
        if(_players[0].Dipl.DA_Strength[0] == 0 ||
            _players[0].Dipl.Dipl_Status[player2_idx] >= DIPL_War ||
            s88p04_empty_function(player2_idx) == ST_TRUE)
        {
            _players[0].Dipl.Dipl_Action[player2_idx] = do_None;
            return;
        }
    }

    /* loc_74476 */
    if(_players[player1_idx].Dipl.Dipl_Action[player2_idx] != do_None)
    {
        if(Random(2) == 1)  /* 50% */
        {
            return;
        }
    }

    /* loc_7449A */
    IDK_message = 0;
    var_12 = 0;

    IDK_relations = _players[player1_idx].Dipl.Hidden_Rel[player2_idx] +
            _players[player1_idx].Dipl.Visible_Rel[player2_idx] +
            var_12 +
            TBL_AI_PRS_IDK_Mod[_players[player2_idx].Personality];

    if(_players[player1_idx].Dipl.Dipl_Status[player2_idx] == DIPL_NoTreaty &&
        _players[player1_idx].Dipl.Visible_Rel[player2_idx] > 15)
    {
        input = IDK_relations + Random(100) + _players[player1_idx].Dipl.treaty_modifier[player2_idx];
        if(input >= 50)
        {
            IDK_message = 1;
        }
    }

    if((_players[player1_idx].Dipl.Dipl_Status[player2_idx] == DIPL_WizardPact || IDK_message == 1) &&
        _players[player1_idx].Dipl.Visible_Rel[player2_idx] > 50)
    {
        input = IDK_relations + Random(100) + _players[player1_idx].Dipl.treaty_modifier[player2_idx];
        if(input >= 100)
        {
            IDK_message = 2;
        }
    }

    if(IDK_message == 0)
    {
        /* loc_745A6 */
        var_8 = 0;
        for(itr_players = 1; itr_players < _num_players; itr_players++)
        {
            if(itr_players == player2_idx)
            {
                continue;
            }
            // if(_players[player2_idx].Dipl.Dipl_Status[itr_players] >= DIPL_War &&
            //     _players[_current_player_idx].Dipl.Contacted[itr_players] != ST_TRUE)
            /* CLAUDE */  /* REBUG */  /* asm: mov ax, [_human_player_idx] */
            if(_players[player2_idx].Dipl.Dipl_Status[itr_players] >= DIPL_War &&
                _players[_current_player_idx].Dipl.Contacted[itr_players] != ST_TRUE)
            {
                var_8 = itr_players;
            }
        }

        if(
            var_8 != 0
            &&
            Random(10) == 1
        )
        {
            if(_players[player1_idx].Dipl.niu_attack_bounty[player2_idx] == HUMAN_PLAYER_IDX)
            {
                Get_Exchange_Spell_List(player2_idx, 0, 0);
                // *( (int *)&divisor + 1 ) = 0;
                /* CLAUDE */  /* REBUG */  /* asm: mov [word ptr bp+divisor+2], 0 - the high WORD of the dword; (int *) + 1 wrote 4 bytes past divisor */
                *( (int16_t *)&divisor + 1 ) = 0;
                if(m_exchange_spell_count == 0)
                {
                    int temp = (Random(3) + 1) * _turn;
                    // *( (int *)&divisor + 1 ) = (temp / 50) * 50;
                    /* CLAUDE */  /* REBUG */
                    *( (int16_t *)&divisor + 1 ) = (int16_t)((temp / 50) * 50);
                }
            }
        }
    }

    if(IDK_message == 0)
    {
        /* loc_74650 */
        input = IDK_relations + Random(100);
        if(input >= 25)
        {
            var_12 = 0;
            IDK_modifier = _players[player1_idx].Dipl.exchange_modifier[player2_idx];
            if(IDK_modifier > 0)
            {
                IDK_modifier = IDK_modifier / 5;
            }
            if(IDK_modifier > 30)
            {
                IDK_modifier = 30;
            }
            if(_players[player2_idx].Dipl.Dipl_Status[player1_idx] == DIPL_Alliance)
            {
                IDK_modifier += 25;
            }

            /* Note: Visble_Rel division by 2 is signed division rounding to 0, matching the cwd/sub ax,dx/sar ax,1 sequence */
            IDK_relations = _players[player1_idx].Dipl.Hidden_Rel[player2_idx] +
                    (_players[player1_idx].Dipl.Visible_Rel[player2_idx] / 2) +
                    var_12 +
                    TBL_AI_PRS_IDK_Mod[_players[player2_idx].Personality] +
                    IDK_modifier - 125;

            input = IDK_relations + Random(100) + IDK_modifier - 125;
            if(input < 0)
            {
                divisor = abs(input) + 100;
            }
            else
            {
                divisor = 20000 / (input + 200);
            }

            if(divisor < 50)
            {
                divisor = 50;
            }

            // Get_Exchange_Spell_List(player2_idx, 0, 0);
            /* CLAUDE */  /* REBUG */  /* asm loc_7474E: push 0 / push player2_idx / push 0 - (0, player2_idx, 0) */
            Get_Exchange_Spell_List(0, player2_idx, 0);
            if(m_exchange_spell_count > 0)
            {
                byte_42E43[0] = m_exchange_spell_list[0];
                byte_42E39[0] = (uint8_t)((100 * (int32_t)m_exchange_spell_values[0]) / (int32_t)divisor);
                min_value = byte_42E39[0];

                Get_Exchange_Spell_List(player2_idx, 0, min_value);
                var_E = ST_FALSE;
                for(itr_players = 0; itr_players < m_exchange_spell_count; itr_players++)
                {
                    if(m_exchange_spell_values[itr_players] <= byte_42E39[0])
                    {
                        var_E = ST_TRUE;
                    }
                }

                if(var_E != ST_FALSE)
                {
                    IDK_message = 5;  /* ¿ MoO2  Get_Main_Diplomacy_Choices_()  case 5: Diplomacy_Exchange_Technology_() */
                }
            }
        }
    }

    /* loc_747D1 */
    if(IDK_message == 0)
    {
        if(_players[0].Dipl.Visible_Rel[player2_idx] > 0 &&
            _players[player2_idx].empire_mini_pops < _players[0].empire_mini_pops &&
            _players[0].Dipl.Dipl_Status[player2_idx] == DIPL_NoTreaty)
        {
            IDK_message = 1;
        }
    }

    if(IDK_message == 1)
    {
        _players[0].Dipl.Dipl_Action[player2_idx] = do_IDK_treaty1;
    }
    else if(IDK_message == 2)
    {
        _players[0].Dipl.Dipl_Action[player2_idx] = do_IDK_treaty2;
    }
    else if(IDK_message == 5)  /* MoO2  Get_Main_Diplomacy_Choices_()  case 5: Diplomacy_Exchange_Technology_() */
    {
        if(m_exchange_spell_count > 0)
        {
            _players[0].Dipl.niu_au_tech_trade_num[player2_idx] = 0;
            for(itr_players = 0; itr_players < m_exchange_spell_count && itr_players < 6; itr_players++)
            {
                if(m_exchange_spell_values[itr_players] <= byte_42E39[0])
                {
                    int field_F0_val = _players[0].Dipl.niu_au_tech_trade_num[player2_idx];
                    int offset_A8 = player2_idx * 6 + field_F0_val;
                    // ((int *)_players[0].Dipl.field_A8)[offset_A8] = m_exchange_spell_list[itr_players];
                    /* CLAUDE */  /* REBUG */  /* asm: mov [word ptr _players.Dipl.field_A8+bx], ax with bx = player2_idx * 12 + num * 2 - a word store; (int *) wrote 4-byte ints at twice the offset */
                    _players[0].Dipl.field_A8[offset_A8] = m_exchange_spell_list[itr_players];
                    _players[0].Dipl.niu_au_tech_trade_num[player2_idx]++;
                }
            }

            if(_players[0].Dipl.niu_au_tech_trade_num[player2_idx] > 0)
            {
                _players[0].Dipl.Dipl_Action[player2_idx] = do_IDK_exchange;
                // _players[0].Dipl.niu_au_want_tech[player2_idx] = byte_42E43[0];
                /* CLAUDE */  /* REBUG */  /* asm: mov [word ptr _players.Dipl.niu_au_want_tech+bx], ax with bx = player2_idx shl 1 */
                SET_2B_OFS(_players[0].Dipl.niu_au_want_tech, (player2_idx * 2), (uint16_t)byte_42E43[0]);
            }

            _players[player2_idx].Dipl.exchange_modifier[0] = 0;
            _players[0].Dipl.exchange_modifier[player2_idx] = 0;
        }
    }

    if(_players[player2_idx].empire_mini_pops != 0)
    {
        if(IDK_message == 1 || IDK_message == 2)
        {
            if(_players[player2_idx].Personality > PRS_Ruthless)
            {
                if(Random(100) <= (_players[0].empire_mini_pops * 30) / _players[player2_idx].empire_mini_pops)
                {
                    if(Random(8) == 1)  /* 12.5% */
                    {
                        // if(Random(4) <= 1)  /* 1:4 */
                        // {
                        //     Get_Exchange_Spell_List(player2_idx, 0, 0);
                        //     if(m_exchange_spell_count > 0)
                        //     {
                        //         _players[0].Dipl.offer_spell[player2_idx] = m_exchange_spell_list[0];
                        //     }
                        // }
                        // else  /* 3:4 */
                        /* CLAUDE */  /* REBUG */  /* asm: cmp ax, 1 / jle loc_7495A - the gold block falls through first, the spell block is the jump target */
                        if(Random(4) > 1)  /* 3:4 */
                        {
                            int rnd_div = Random(3) + 1;
                            /* OGBUG  the incentive gold is sized by the AI's population stat (empire_mini_pops), not its treasury: (pop / (Random(3) + 1) + _turn) / 25 x 25 - about the turn rounded down to 25, and 0 before turn ~25; Npc_Diplomacy_Screen pays it to the human and never takes it from the AI */
                            int temp_gold = (_players[player2_idx].empire_mini_pops / rnd_div) + _turn;
                            _players[0].Dipl.offer_gold[player2_idx] = (temp_gold / 25) * 25;
                        }
                        /* CLAUDE */
                        else  /* 1:4 */
                        {
                            Get_Exchange_Spell_List(player2_idx, 0, 0);
                            if(m_exchange_spell_count > 0)
                            {
                                _players[0].Dipl.offer_spell[player2_idx] = m_exchange_spell_list[0];
                            }
                        }
                    }
                }
            }
        }
    }

}


// WZD o88p02
void DIPL_HumanWarOrPeace(int16_t player_idx)
{
    int16_t base_score = 0;
    int16_t niu_npc_war_bonus = 0;
    int16_t modded_score = 0;
    int16_t limit = 0;
    int16_t divisor = 0;

    niu_npc_war_bonus = 0;  /* ¿ Debug Override ? */

    if(
        _players[_current_player_idx].Dipl.Contacted[player_idx] != ST_TRUE
        || 
        _players[0].Dipl.DA_Strength[player_idx] == 0)
    {
        _players[0].Dipl.Dipl_Action[player_idx] = 0;
        return;
    }

    base_score = (
        _players[0].Dipl.Hidden_Rel[player_idx]
        +
        _players[0].Dipl.Visible_Rel[player_idx]
        +
        niu_npc_war_bonus
        +
        TBL_AI_PRS_IDK_Mod[_players[player_idx].Personality]
    );

    if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[player_idx] < DIPL_War)
    {
        if(_players[0].Dipl.Visible_Rel[player_idx] <= -75)
        {
            Declare_War(player_idx, 0);
            _players[0].Dipl.Dipl_Action[player_idx] = 39;
        }
        else  /* (_players[0].Dipl.Visible_Rel[player_idx] > -75) */
        {
            modded_score = (
                base_score
                -
                Random(100)
                +
                _players[0].Dipl.treaty_modifier[player_idx]
            );
            if(modded_score <= -100)
            {
                // if(_players[0].Dipl.field_126[player_idx] <= 0)
                // {
                //     _players[0].Dipl.field_126[player_idx] = 1;
                // }
                // else
                /* CLAUDE */  /* REBUG */  /* asm: cmp field_126, 0 / jle loc_74A99 - the > 0 block falls through first, = 1 is the jump target */
                if(_players[0].Dipl.field_126[player_idx] > 0)
                {
                    _players[0].Dipl.field_126[player_idx]++;
                    if(
                        _players[0].Dipl.field_126[player_idx] > 2
                        ||
                        _players[0].Dipl.Dipl_Status[player_idx] == DIPL_NoTreaty
                    )
                    {
                        limit = -(_players[0].Dipl.DA_Strength[player_idx] + TBL_AI_PRS_IDK_Mod[_players[player_idx].Personality] / 2);
                        if(Random(100) < limit)
                        {
                            Declare_War(player_idx, 0);
                            _players[0].Dipl.Dipl_Action[player_idx] = 39;
                            _players[0].Dipl.field_126[player_idx] = 1;
                        }
                    }
                    else
                    {
                        Break_Treaties(player_idx, 0);
                    }
                }
                /* CLAUDE */
                else
                {
                    _players[0].Dipl.field_126[player_idx] = 1;
                }
            }
        }
    }
    else  /* (_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[player_idx] >= DIPL_War) */
    {
        modded_score = base_score + Random(100) + _players[0].Dipl.peace_modifier[player_idx];
        if(modded_score >= 50)
        {
            if(Random(8) == 1)  /* 12.5% */
            {
                _players[0].Dipl.Dipl_Action[player_idx] = 49;
                if(Random(100) <= (_players[player_idx].empire_mini_pops * 30) / _players[0].empire_mini_pops)
                {
                    if(Random(4) > 1)  /* 3:4 */
                    {
                        divisor = Random(3) + 1;
                        /* OGBUG  the peace incentive gold is sized by the AI's population stat (empire_mini_pops), not its treasury: (pop / (Random(3) + 1) + _turn) / 25 x 25; Npc_Diplomacy_Screen pays it to the human and never takes it from the AI */
                        _players[0].Dipl.offer_gold[player_idx] = (((_players[player_idx].empire_mini_pops / divisor) + _turn) / 25) * 25;
                    }
                    else  /* 1:4 */
                    {
                        Get_Exchange_Spell_List(player_idx, 0, 0);
                        if(m_exchange_spell_count > 0)
                        {
                            _players[0].Dipl.offer_spell[player_idx] = m_exchange_spell_list[0];
                        }
                    }
                }
            }
        }
    }

}


// WZD o88p03
// MoO2  Module: DIPLOMAC  Modifier_Diplomacy_Adjustments_()
// 1oom  game_turn.c  game_turn_update_mood_blunder()
void Modifier_Diplomacy_Adjustments(void)
{
    int16_t itr3 = 0;
    int16_t niu_dipl_action = 0;
    int16_t itr2 = 0;
    int16_t itr1 = 0;
    for(itr1 = 0; itr1 < _num_players; itr1++)
    {
        for(itr2 = 1; itr2 < _num_players; itr2++)
        {
            if(itr2 != itr1)
            {
                niu_dipl_action = _players[itr1].Dipl.Dipl_Action[itr2];
                if(_players[itr1].Dipl.Dipl_Status[itr2] != DIPL_Crusade)
                {
                    if(_players[itr1].Dipl.treaty_modifier[itr2] < 0)
                    {
                        _players[itr1].Dipl.treaty_modifier[itr2] += Random(5);
                    }
                    if(_players[itr1].Dipl.treaty_modifier[itr2] < 50)
                    {
                        _players[itr1].Dipl.treaty_modifier[itr2] += Random(5);
                    }
                    if(_players[itr1].Dipl.exchange_modifier[itr2] < 0)
                    {
                        _players[itr1].Dipl.exchange_modifier[itr2] += Random(5);
                    }
                    if(_players[itr1].Dipl.exchange_modifier[itr2] < 50)
                    {
                        _players[itr1].Dipl.exchange_modifier[itr2] += Random(5);
                    }
                    if(_players[itr1].Dipl.peace_modifier[itr2] < 0)
                    {
                        _players[itr1].Dipl.peace_modifier[itr2] += Random(5);
                    }
                    if(_players[itr1].Dipl.peace_modifier[itr2] < 50)
                    {
                        _players[itr1].Dipl.peace_modifier[itr2] += Random(5);
                    }
                    SETMIN(_players[itr1].Dipl.peace_modifier[itr2], -200);
                    SETMAX(_players[itr1].Dipl.peace_modifier[itr2], 200);
                }
            }
        }
    }
    // Hereafter, DNE in MoO2
    for(itr3 = 0; itr3 < _num_players; itr3++)
    {
        for(itr2 = 0; itr2 < _num_players; itr2++)
        {
            _players[itr2].Dipl.DA_Strength[itr3] = 0;
            _players[itr2].Dipl.Dipl_Action[itr3] = do_None;
            if(_players[itr2].Dipl.niu_bounty_collect[itr3] != 0)
            {
                _players[itr2].Dipl.niu_attack_bounty[itr3] = 0;
                _players[itr2].Dipl.niu_bounty_collect[itr3] = 0;
            }
            _players[itr2].Dipl.niu_au_tech_trade_num[itr3] = 0;
            _players[itr2].Dipl.offer_spell[itr3] = 0;
            _players[itr2].Dipl.niu_au_want_tech[itr3] = 0;
            _players[itr2].Dipl.offer_gold[itr3] = 0;
        }
    }
}


// WZD o88p04
int16_t s88p04_empty_function(int16_t player_idx)
{
// push    bp
// mov     bp, sp
// xor     ax, ax
// jmp     short $+2
// pop     bp
// retf
return ST_FALSE;
}


// WZD o88p05
// sub_74E38()
/*
NIU  nothing in WIZARDS.EXE calls it (its thunk, sub_34C24, has no callers)
how far player1's army strength (astrologer.army_strength) is above or below player2's, in percent, clamped to -300 .. 300,
returned as a random value of the same sign: Random(percent), -Random(-percent), or 0
*/
int16_t Army_Strength_Random_Difference__NIU(int16_t player1_idx, int16_t player2_idx)
{
    int32_t percent_difference = 0;
    int32_t player2_value = 0;
    int32_t player1_value = 0;
    int16_t random_range = 0;  // _SI_

    player1_value = _players[player1_idx].astrologer.army_strength;

    player2_value = _players[player2_idx].astrologer.army_strength;

    /* NOTE  army_strength is an int16_t, so this cap of 3,250,000 can never be reached */
    if(player1_value > 3250000)
    {

        player1_value = 3250000;

    }

    if(player1_value == 0)
    {

        player1_value = 1;

    }

    if(player2_value > 3250000)
    {

        player2_value = 3250000;

    }

    if(player2_value == 0)
    {

        player2_value = 1;

    }

    percent_difference = (((player1_value - player2_value) * 100) / player2_value);

    if(percent_difference > 300)
    {

        percent_difference = 300;

    }

    if(percent_difference < -300)
    {

        percent_difference = -300;

    }

    random_range = (int16_t)percent_difference;

    if(random_range == 0)
    {

        return 0;

    }
    else if(random_range < 0)
    {

        return -Random(-random_range);

    }
    else
    {

        return Random(random_range);

    }

}


// WZD o88p06
// sub_74F4A()
/*
NIU  nothing in WIZARDS.EXE calls it (its thunk, sub_34C29, has no callers)
the same as Army_Strength_Random_Difference__NIU(), on empire_mini_pops - except that only player2's value is capped and kept from 0
*/
int16_t Population_Random_Difference__NIU(int16_t player1_idx, int16_t player2_idx)
{
    int32_t percent_difference = 0;
    int32_t player2_value = 0;
    int32_t player1_value = 0;
    int16_t random_range = 0;  // _SI_

    player1_value = _players[player1_idx].empire_mini_pops;  /* asm: cwd - read as signed */

    player2_value = _players[player2_idx].empire_mini_pops;  /* asm: cwd - read as signed */

    /* NOTE  empire_mini_pops is a 16-bit word, so this cap of 3,250,000 can never be reached */
    if(player2_value > 3250000)
    {

        player2_value = 3250000;

    }

    if(player2_value == 0)
    {

        player2_value = 1;

    }

    percent_difference = (((player1_value - player2_value) * 100) / player2_value);

    if(percent_difference > 300)
    {

        percent_difference = 300;

    }

    if(percent_difference < -300)
    {

        percent_difference = -300;

    }

    random_range = (int16_t)percent_difference;

    if(random_range == 0)
    {

        return 0;

    }
    else if(random_range < 0)
    {

        return -Random(-random_range);

    }
    else
    {

        return Random(random_range);

    }

}
