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
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOX_BASE.h"
#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_SET.h"
#include "../../MoX/src/MOX_T4.h"
#include "../../MoX/src/SOUND.h"


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

#include <SDL_stdinc.h>

#include "DIPLOMAC.h"



/*
    WIZARDS.EXE  ovr084
*/
// WZD o84p01
void Diplomacy_Screen__WIP(void);
// WZD o84p02
static void Diplomacy_Screen_Draw__WIP(void);
// WZD o84p01
// IDK_Diplo_Add_Spell_Or_Gold_To_Message()
// WZD o84p02
// sub_6ED5D()
// WZD o84p03
static void Get_Main_Diplomacy_Choices(void);
// WZD o84p04
static void Diplomacy_Propose_Treaty__WIP(void);
// WZD o84p05
static int16_t IDK_Npc_Target_Player__STUB(int16_t player_idx, int16_t type);
// WZD o84p06
static void Diplomacy_Display_Response(int16_t diplomatic_order, int16_t IDK2);
// WZD o84p07
static int16_t IDK_Npc_Counteroffer__STUB(int16_t arg_0);
// WZD o84p08
static void Diplomacy_Display_Response_Draw(void);
// WZD o84p09
static void Diplomacy_Break_Treaty(void);
// WZD o84p10
static void Start_Diplomacy_Music(int16_t IDK);
// WZD o84p11
static void Diplomacy_Screen_Draw_Portrait_Mouth_Animation(void);
// WZD o84p12
static void Diplomacy_Screen_Load__WIP(void);
// WZD o84p13
static void DIPL_LoadTalkGFX(void);
// WZD o84p14
static void Diplomacy_Screen_Draw_Gargoyle_Eyes(void);
// WZD o84p15
static void Diplomacy_Screen_Fade_In(void);
// WZD o84p16
// sub_7038D()
// WZD o84p17
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
static void Limit_Temporary_Peace_Modifier(void);
// WZD o85p07
static void Diplomacy_Player_Gets_Spell(int16_t player_idx, int16_t spell_idx);
// WZD o85p08
// G_DIPL_PickSides()
// WZD o85p09
static void Start_Treaty(int16_t player1, int16_t player2, int16_t type);
// WZD o85p10
static void Declare_Peace(int16_t player1, int16_t player2);
// WZD o85p11
static void Adjust_Diplomat_Modifiers(int16_t player1, int16_t player2);
// WZD o85p12
void Change_Relations__WIP(int16_t value, int16_t attacker_idx, int16_t defender_idx, int16_t type, int16_t city_idx, int16_t spell_idx);
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
void NPC_To_NPC_Treaty_Negotiations__STUB(void);
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
void NPC_To_Human_Diplomacy__WIP(void);
// WZD o87p03
// G_DIPL_NeedForWar()
// WZD o87p04
// G_DIPL_SuperiorityWar()
// WZD o87p05
// IDK_Dipl_s73F1C()
// WZD o87p06
void Resolve_Delayed_Diplomacy_Orders(void);
// WZD o87p07
// DIPL_GetOffMyLawn()
// WZD o87p08
void Decrease_Peace_Duration(void);
// WZD o87p09
void End_Of_Turn_Diplomacy_Adjustments(void);
/*
    WIZARDS.EXE  ovr088
*/
// WZD o88p01
// sub_74420()
// WZD o88p02
// DIPL_HumanWarOrPeace()
// WZD o88p03
void Modifier_Diplomacy_Adjustments(void);
// WZD o88p04
// sub_74E2F()
// WZD o88p05
// sub_74E38()
// WZD o88p06
// sub_74F4A()





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
uint8_t byte_42E39[10];
// WZD dseg:C3A3
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
int16_t m_exchange_spell_count;


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
int16_t m_IDK_break_alliance_player3;
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

    Limit_Temporary_Peace_Modifier();

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

    _page_flip_effect = pfe_None;

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

        m_IDK_break_alliance_player3 = _players[HUMAN_PLAYER_IDX].Dipl.field_102[m_diplomac_player_idx];

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

    Check_Research_Spell_Is_Known(ST_TRUE);

    Cache_Graphics_Overland();

    // DOMSDOS  Play_Background_Music__STUB();
    sdl2_Play_Background_Music__WIP();

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


// WZD o84p01
// IDK_Diplo_Add_Spell_Or_Gold_To_Message()

// WZD o84p02
// TODO  Message	VCR003	Function '_sub_6ED5D_Draw' can be made static		C:\STU\devel\ReMoM\MoM\src\DIPLOMAC.c	811		
static void _sub_6ED5D_Draw(void)
{
    int16_t si = 0;  // _SI_

    Set_Page_Off();

    Copy_Back_To_Off();

    Diplomacy_Screen_Draw_Portrait_Mouth_Animation();

    Diplomacy_Screen_Draw_Gargoyle_Eyes();

    FLIC_Draw(95, 1, diplomacy_mirror_seg);

    Set_Font_Style_Shadow_Down(4, 4, ST_NULL, ST_NULL);

    Set_Outline_Color(ST_TRANSPARENT);

    Set_Alias_Color(187);

    Set_Font_LF(1);

    si = Get_Paragraph_Max_Height(245, m_diplomacy_message);

    if(si > 39)
    {

        Set_Font_LF(0);

        si = Get_Paragraph_Max_Height(245, m_diplomacy_message);

    }

    Print_Paragraph(38, 138, 245, m_diplomacy_message, 0);


}


// WZD o84p03
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

        Get_Exchange_Spell_List(_human_player_idx, m_diplomac_player_idx, 0);

        if(
            (_players[HUMAN_PLAYER_IDX].gold_reserve <= 25)
            &&
            (list_item_active_flags[2] == 0)
        )
        {

            list_item_active_flags[2] = ST_FALSE;  // deactive "Offer Tribute"

        }

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

            strcpy(m_diplomacy_message, off_3B740);

            for(itr = 0; itr < 7; itr++)
            {

                strcpy(G_Some_DIPL_Allocs_7[itr], IDK_Diplo_msg[itr]);

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


// WZD o84p04
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

    strcpy(m_diplomacy_message, off_3B742);

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

    if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[m_diplomac_player_idx] >= DIPL_NoTreaty)
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
            (_players[m_diplomac_player_idx].Dipl.Dipl_Status[itr] != DIPL_War)
            &&
            (itr != m_diplomac_player_idx)
        )
        {

            m_other_wizard_list_declare_war[m_other_wizard_count_declare_war] = (unsigned char)itr;

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

        if(
            (_players[m_diplomac_player_idx].Dipl.Contacted[itr] == 1)
            &&
            (_players[HUMAN_PLAYER_IDX].Dipl.Contacted[itr] == 1)
            &&
            (_players[m_diplomac_player_idx].Dipl.Dipl_Status[itr] != DIPL_Alliance)
            &&
            (itr != m_diplomac_player_idx)
        )
        {

            m_other_wizard_list_break_alliance[m_other_wizard_count_break_alliance] = (unsigned char)itr;

            m_other_wizard_count_break_alliance++;

        }

    }

    if(m_other_wizard_count_break_alliance == 0)
    {

        active_flag[4] = ST_FALSE;  // "Break Alliance With Another Wizard"

    }

    for(itr = 0; itr < 6; itr++)
    {

        strcpy(G_Some_DIPL_Allocs_7[itr], IDK_Diplo_msg[(7 + itr)]);

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

                // TODO  DIPLO ROUND 2  diplomacy_test_result = IDK_Npc_Counteroffer__STUB(diplomacy_test_result);
                if(Random(2) == 1)
                {
                    diplomacy_test_result = 3;
                }

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

                // TODO  DIPLO ROUND 2  diplomacy_test_result = IDK_Npc_Counteroffer__STUB(diplomacy_test_result);
                if(Random(2) == 1)
                {
                    diplomacy_test_result = 3;
                }

            }

            if(diplomacy_test_result == 3)
            {

                Start_Treaty(HUMAN_PLAYER_IDX, m_diplomac_player_idx, 2);

            }

            diplomacy_message_id = 52;

        } break;

        case 2:  /* Peace Treaty */
        {

            diplomacy_test_result = Diplomacy_Test(60, 0);

            if(
                (diplomacy_test_result == 1)
                ||
                (diplomacy_test_result == 2)
            )
            {

                // TODO  DIPLO ROUND 2  diplomacy_test_result = IDK_Npc_Counteroffer__STUB(diplomacy_test_result);
                if(Random(2) == 1)
                {
                    diplomacy_test_result = 3;
                }

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

            Target_Player = IDK_Npc_Target_Player__STUB(HUMAN_PLAYER_IDX, 0);

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
                    (_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[Target_Player] == DIPL_Alliance)
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

                    var_6 = (150 + (_players[m_diplomac_player_idx].Dipl.Dipl_Status[Target_Player] * 30));

                    diplomacy_test_result = Diplomacy_Test((150 + _players[m_diplomac_player_idx].Dipl.Visible_Rel[Target_Player]), 0);

                }

                if(
                    (diplomacy_test_result == 1)
                    ||
                    (diplomacy_test_result == 2)
                )
                {

                    // TODO  DIPLO ROUND 2  diplomacy_test_result = IDK_Npc_Counteroffer__STUB(diplomacy_test_result);
                    if(Random(2) == 1)
                    {
                        diplomacy_test_result = 3;
                    }

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

            m_IDK_break_alliance_player3 = IDK_Npc_Target_Player__STUB(HUMAN_PLAYER_IDX, 0);

            if(m_IDK_break_alliance_player3 == HUMAN_PLAYER_IDX)
            {

                _variable = ST_UNDEFINED;

            }
            else
            {

                if(_players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] > 24)
                {

                    diplomacy_test_result = Diplomacy_Test((175 + _players[m_diplomac_player_idx].Dipl.Visible_Rel[m_IDK_break_alliance_player3]), 0);

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

                    // TODO  DIPLO ROUND 2  diplomacy_test_result = IDK_Npc_Counteroffer__STUB(diplomacy_test_result);
                    if(Random(2) == 1)
                    {
                        diplomacy_test_result = 3;
                    }

                }

                if(diplomacy_test_result == 3)
                {

                    Break_Treaties(m_diplomac_player_idx, m_IDK_break_alliance_player3);

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


// WZD o84p05
// sub_6F3E4()
static int16_t IDK_Npc_Target_Player__STUB(int16_t player_idx, int16_t type)
{


    return Random(_num_players);


}


// WZD o84p06
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
    int16_t di = 0;  // _DI_

    di = 0;

    if(diplomatic_order == 57)
    {

        di = 1;

    }

    if(
        (diplomatic_order >= 58)
        &&
        (diplomatic_order <= 66)
    )
    {

        di = 1;

    }

    if(
        (diplomatic_order >= 51)
        &&
        (diplomatic_order <= 56)
    )
    {

        di = 1;

    }

    m_IDK_diplomatic_order = diplomatic_order;

    if(di != 1)
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

            Diplomacy_Display_Response_Draw();

            PageFlip_FX();

            Release_Time(3);

        }

    }

}


// WZD o84p07
// sub_6F6BB()
// ¿ MoO2  Module: DIP-SCRN  Diplomacy_Need_Better_Offer_() ?
static int16_t IDK_Npc_Counteroffer__STUB(int16_t arg_0)
{
    char string[LEN_STRING] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    char strP[80] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int16_t var_A = 0;
    int16_t var_8 = 0;
    int16_t var_6 = 0;
    int16_t _variable = 0;
    int16_t spell_idx = 0;
    int16_t si = 0;

    var_6 = 0;

    si = ((((Random(8) + Random(8)) * _turn) / 25) * 25);

    if(si > _players[HUMAN_PLAYER_IDX].gold_reserve)
    {

        si = 0;

    }

    Get_Exchange_Spell_List(HUMAN_PLAYER_IDX, m_diplomac_player_idx, _turn);

    if(m_exchange_spell_count > 0)
    {

    }

    return 0;

}


// WZD o84p08
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


// WZD o84p09
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

    strcpy(m_diplomacy_message, off_3B744);

    if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[m_diplomac_player_idx] != DIPL_WizardPact)
    {

        active_flag[0] = ST_FALSE;  // "Break Wizard Pact"

    }

    if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[m_diplomac_player_idx] != DIPL_Alliance)
    {

        active_flag[1] = ST_FALSE;  // "Break Alliance"

    }

    var_24 = 0;

    for(itr = 0; itr < 5; itr++)
    {

        strcpy(G_Some_DIPL_Allocs_7[itr], IDK_Diplo_msg[(14 + itr)]);

    }

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

            if(Invader_Army_Strength_Comparison(_human_player_idx, m_diplomac_player_idx, &value1, &value2) != 0)
            {

                if(value1 < value2)  /* invader is stronger */
                {

                    if(value1 > 0)
                    {

                        threat_modifier = (-50 + ((value2 * 100) / value1));

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

                        threat_modifier = (-50 + ((value1 * 100) / value2));

                    }
                    else
                    {

                        threat_modifier = 100;
                        
                    }

                }

                relations = (_players[HUMAN_PLAYER_IDX].Dipl.Hidden_Rel[m_diplomac_player_idx]
                    + _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]
                    + TBL_AI_PRS_IDK_Mod[_players[m_diplomac_player_idx].Personality]
                    + threat_modifier
                    + Random(100)
                    + Random(100));

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

                        if(m_exchange_spell_count > 0)
                        {

                            _players[HUMAN_PLAYER_IDX].peace_duration[m_diplomac_player_idx] = (uint8_t)Random(15);

                            diplomacy_message_id = 70;

                            Spell_Index = m_exchange_spell_list[0];

                            Diplomacy_Player_Gets_Spell(HUMAN_PLAYER_IDX, Spell_Index);

                        }

                    }
                    else
                    {

                        if(relations >= 90)
                        {

                            Amount = ((((2 + Random(8)) * 25) / 25) * 25);

                            if(_players[m_diplomac_player_idx].gold_reserve < Amount)
                            {

                                if(_players[m_diplomac_player_idx].gold_reserve > 0)
                                {

                                    Amount = _players[m_diplomac_player_idx].gold_reserve;
                                    
                                }
                                else
                                {

                                    Amount = 0;

                                }

                            }

                            if(Amount != 0)
                            {

                                _players[HUMAN_PLAYER_IDX].peace_duration[m_diplomac_player_idx] = (uint8_t)Random(15);

                                Player_Add_Gold(_human_player_idx, Amount);

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


// WZD o84p10
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

                // DOMSDOS  Play_Sound__STUB(sound_buffer1);
                sdl2_Play_Sound__WIP(sound_buffer1, sound_buffer1_size);

            }
            else if(diplomacy_music == 1)
            {

                // DOMSDOS  Play_Sound__STUB(sound_buffer1);
                sdl2_Play_Sound__WIP(sound_buffer1, sound_buffer1_size);

            }
            else if(diplomacy_music == 2)
            {

                // DOMSDOS  Play_Sound__STUB(sound_buffer2);
                sdl2_Play_Sound__WIP(sound_buffer2, sound_buffer2_size);

            }

        }

        m_diplomacy_current_music = diplomacy_music;

    }

}

// WZD o84p11
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


// WZD o84p13
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


// WZD o84p14
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


// WZD o84p15
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


// WZD o84p16
// sub_7038D()


// WZD o84p17
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

// // ; added to chance of forming treaties
// // ; added to chance of avoiding superiority wars
// int16_t TBL_AI_PRS_IDK_Mod[6] = { 0, 10, 20, 30, 40, 50 };
    relationship_level = (worst_modifier_value
        + _players[HUMAN_PLAYER_IDX].Dipl.Hidden_Rel[m_diplomac_player_idx]
        + _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]
        + TBL_AI_PRS_IDK_Mod[_players[m_diplomac_player_idx].Personality]);

    //OSG  back out the "Diplomatic Relations Scale" "position" portion of the "Core Reaction"
    IDK_nonvisible_relationship_level = (relationship_level - _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]);

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
// drake178: DIPL_Gravitation()
// MoO2  Module: DIPLOMAC  Diplomacy_Growth_()
/*
; process wizard pact and alliance relation
; gravitation, military and city overextension, and
; gravitation towards default relation
; RE-EXPLORE!
*/
/*

*/
void Diplomacy_Growth(void)
{
    int16_t Too_Strong_Treshold = 0;
    int16_t human_army_strength = 0;
    int16_t Gravitation = 0;
    int16_t human_player_city_count = 0;
    int16_t Dipl_182h_Field = 0;
    int16_t Reaction_Value = 0;
    int16_t itr1 = 0;  // _SI_
    int16_t itr2 = 0;  // _DI_

    // ; for each wizard, generate a +1-3 diplomatic reaction
    // ; towards pacted wizards, or a +1-6 one towards allied
    // ; wizards with a 50% chance
    for(itr1 = 0; itr1 < _num_players; itr1++)
    {

        for(itr2 = (itr1 + 1); itr2 < _num_players; itr2++)
        {

            if(Random(2) == 1)
            {

                if(_players[itr1].Dipl.Dipl_Status[itr2] == DIPL_WizardPact)
                {

                    Change_Relations__WIP(Random(3), itr1, itr2, 0, ST_NULL, ST_NULL);

                }

                if(_players[itr1].Dipl.Dipl_Status[itr2] == DIPL_Alliance)
                {

                    Change_Relations__WIP(Random(6), itr1, itr2, 0, ST_NULL, ST_NULL);

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
    human_army_strength = _players[HUMAN_PLAYER_IDX].Astr.Army_Strength;

    if(human_army_strength > 0)
    {

        for(itr1 = 0; itr1 < _num_players; itr1++)
        {

            if(
                (_players[itr1].Astr.Army_Strength < human_army_strength)
                &&
                (_players[itr1].Astr.Army_Strength > 0)
            )
            {

                Too_Strong_Treshold = ((human_army_strength * 50) / _players[itr1].Astr.Army_Strength);

                if(
                    (Random(100) >= Too_Strong_Treshold)
                    &&
                    (Random(20) == 1)  /* 5% chance */
                )
                {

                    Change_Relations__WIP(-10, HUMAN_PLAYER_IDX, itr1, 7, ST_NULL, ST_NULL);

                }

            }

        }

    }

    // ; trigger overextension reactions towards all other
    // ; wizards based on the human player's city count?
    if((_turn % 2) == 0)
    {

        for(itr1 = 0; itr1 < _num_players; itr1++)
        {

            human_player_city_count = 0;

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

                Reaction_Value = (((-(human_player_city_count - ((_landsize + 1) * 3)) / ((_landsize + 6) - _difficulty)) * Random(4)) / 3);

                Reaction_Value *= 2;

                if(itr1 != HUMAN_PLAYER_IDX)
                {

                    Reaction_Value /= 2;

                }

                SETMIN(Reaction_Value, -15);

                for(itr2 = 1; itr2 < _num_players; itr2++)
                {

                    if(
                        (_players[itr1].Dipl.Contacted[itr2] == ST_TRUE)
                        &&
                        (_players[itr1].Dipl.Dipl_Status[itr2] != DIPL_Alliance)
                    )
                    {

                        Change_Relations__WIP(Reaction_Value, itr1, itr2, 14, ST_NULL, ST_NULL);

                    }

                }

            }

        }

    }

    // ; gravitate visible relations toward default relations
    for(itr1 = 1; itr1 < _num_players; itr1++)
    {

        for(itr2 = 1; itr2 < _num_players; itr2++)
        {

            if(Random(105) > abs(_players[itr1].Dipl.Visible_Rel[itr2]))
            {

                Gravitation = Random(2);

            }
            else
            {

                Gravitation = 0;

            }

            if(
                (itr2 != itr1)
                &&
                (_players[itr1].Dipl.DA_Strength[itr2] == 0)
            )
            {

                Dipl_182h_Field = _players[itr1].Dipl.Default_Rel[itr2];

                if(_players[itr1].Dipl.Visible_Rel[itr2] < Dipl_182h_Field)
                {
                    
                    _players[itr1].Dipl.Visible_Rel[itr2] += Gravitation;

                    SETMAX(_players[itr1].Dipl.Visible_Rel[itr2], (int8_t)Dipl_182h_Field);

                }
                else
                {

                    if((_turn % 10) == 0)
                    {
                    
                        _players[itr1].Dipl.Visible_Rel[itr2] -= Gravitation;

                        SETMIN(_players[itr1].Dipl.Visible_Rel[itr2], (int8_t)Dipl_182h_Field);

                    }

                }

                SETMIN(_players[itr1].Dipl.Visible_Rel[itr2], -100);

                SETMAX(_players[itr1].Dipl.Visible_Rel[itr2], 100);

            }

        }

    }

    // ; make visible relations symmetrical, and self fields
    // ; zero
    for(itr1 = 1; itr1 < _num_players; itr1++)
    {

        for(itr2 = 1; itr2 < _num_players; itr2++)
        {

            if(itr2 == itr1)
            {

                _players[itr1].Dipl.Visible_Rel[itr2] = 0;

            }
            else
            {

                _players[itr2].Dipl.Visible_Rel[itr1] = _players[itr1].Dipl.Visible_Rel[itr2];

            }

        }
        
    }


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
    int16_t itr_players = 0;
    int16_t return_value = 0;
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

        _players[HUMAN_PLAYER_IDX].Dipl.field_7E[m_diplomac_player_idx] = (int8_t)other_player;

        m_diplomsg_1_record_sub_number = ST_UNDEFINED;

        var_6 = DIPL_sub_72DB6__STUB();

        if(var_6 != 0)
        {

            itr_players = (6 + Random(6));

            _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] -= itr_players;

            SETMIN(_players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx], -100);

            _players[m_diplomac_player_idx].Dipl.Visible_Rel[m_diplomac_player_idx] = -100;

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

    relations = (temporary_modifier
        + _players[HUMAN_PLAYER_IDX].Dipl.Hidden_Rel[m_diplomac_player_idx]
        + _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]
        + TBL_AI_PRS_IDK_Mod[_players[m_diplomac_player_idx].Personality]);

    value1 = 0;
    value2 = 0;

    Invader_Army_Strength_Comparison(_human_player_idx, m_diplomac_player_idx, &value1, &value2);

    if(value2 < value1)  /* invader is stronger */
    {

        if(value2 > 0)
        {

            threat_modifier = (-50 + (value1 * 100 / value2));

        }
        else
        {

            threat_modifier = 50;

        }

    }
    else  /* invader is weaker */
    {

        if(value1 > 0)
        {

            threat_modifier = (-50 + (value2 * 100 / value1));

        }
        else
        {

            threat_modifier = -50;

        }

    }

    
    final_value = (relations + Random(100) - type_modifier + threat_modifier);

    if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[m_diplomac_player_idx] == DIPL_Alliance)
    {

        final_value += 40;

    }

    Adjust_Diplomat_Modifiers(HUMAN_PLAYER_IDX, m_diplomac_player_idx);

    switch(type)
    {

        case 0:
        {

            _players[HUMAN_PLAYER_IDX].Dipl.treaty_modifier[m_diplomac_player_idx] -= (20 + Random(30));

        } break;

        case 1:
        {

            // DNE - 'trade_modifier'

        } break;

        case 2:
        {

            _players[HUMAN_PLAYER_IDX].Dipl.peace_modifier[m_diplomac_player_idx] -= (50 + Random(50));

        } break;

        case 3:
        {

            _players[HUMAN_PLAYER_IDX].Dipl.exchange_modifier[m_diplomac_player_idx] -= (20 + Random(50));

        } break;

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

        if(_players[HUMAN_PLAYER_IDX].Dipl.treaty_modifier[m_diplomac_player_idx] < _players[HUMAN_PLAYER_IDX].Dipl.exchange_modifier[m_diplomac_player_idx])
        {

            interest = _players[HUMAN_PLAYER_IDX].Dipl.treaty_modifier[m_diplomac_player_idx];

        }

        if(interest < _players[HUMAN_PLAYER_IDX].Dipl.peace_modifier[m_diplomac_player_idx])
        {

            interest = _players[HUMAN_PLAYER_IDX].Dipl.peace_modifier[m_diplomac_player_idx];

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

    if(player_idx == _human_player_idx)
    {

        DIPL_LoadTalkGFX();

    }

}


// WZD o85p08
// G_DIPL_PickSides()


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
        (player1 == _human_player_idx)
        ||
        (player2 == _human_player_idx)
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


    _players[player1].Dipl.Visible_Rel[player2] += 40;
    _players[player1].Dipl.Visible_Rel[player2] += 40;

    SETMAX(_players[player1].Dipl.Visible_Rel[player2], 100);

    _players[player1].Dipl.Visible_Rel[player2] = _players[player2].Dipl.Visible_Rel[player1];

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
        (player1 == _human_player_idx)
        ||
        (player2 == _human_player_idx)
    )
    {

        Player_Clear_All_Unit_Orders(player1);

        Player_Clear_All_Unit_Orders(player2);

    }

}


// WZD o85p11
// MoO2  Module: DIPLOMAC  Adjust_Diplomat_Modifiers_()
static void Adjust_Diplomat_Modifiers(int16_t player1, int16_t player2)
{

    _players[player1].Dipl.treaty_modifier[player2] -= 10;

    _players[player1].Dipl.exchange_modifier[player2] -= 10;

    _players[player1].Dipl.peace_modifier[player2] -= 10;

}


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
                            _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[defender_idx] = (int8_t)type;
                            _players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[defender_idx] = spell_idx;
                            _players[HUMAN_PLAYER_IDX].Dipl.DA_City[defender_idx] = (int8_t)city_idx;
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
                            _players[defender_idx].Dipl.peace_modifier[attacker_idx] += (value / 4);
                            // drake178: ; the result is still negative ; increasing the aggressor's toward the target
                            _players[attacker_idx].Dipl.peace_modifier[defender_idx] -= (value / 4);
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

                        _players[defender_idx].Dipl.treaty_modifier[attacker_idx] += value;
                        _players[defender_idx].Dipl.exchange_modifier[attacker_idx] += value;

                        _players[attacker_idx].Dipl.treaty_modifier[defender_idx] = _players[defender_idx].Dipl.treaty_modifier[attacker_idx];
                        _players[attacker_idx].Dipl.exchange_modifier[defender_idx] = _players[defender_idx].Dipl.exchange_modifier[attacker_idx];

                        // HERE:  ¿ NoCharisma_RelValue is value, without the Charismatic modifier, but with ?
                        if(NoCharisma_RelValue < 0)  /* relation change is negative */
                        {
                            // drake178: ; double shift if not at war?
                            _players[attacker_idx].Dipl.peace_modifier[defender_idx] -= (NoCharisma_RelValue / 4);  // more negative
                            _players[defender_idx].Dipl.peace_modifier[attacker_idx] += (NoCharisma_RelValue / 4);  // less positive
                        }
                        else
                        {
                            _players[defender_idx].Dipl.peace_modifier[attacker_idx] += NoCharisma_RelValue;
                        }

                        
                        if(
                            (spell_idx == spl_Earth_Lore)
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
                                _players[defender_idx].Dipl.Dipl_Action[HUMAN_PLAYER_IDX] = (int8_t)type;
                                _players[defender_idx].Dipl.DA_Spell[HUMAN_PLAYER_IDX] = spell_idx;
                                _players[defender_idx].Dipl.DA_City[HUMAN_PLAYER_IDX] = (int8_t)city_idx;
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
/*
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
    _players[attacker_idx].Dipl.field_6C[defender_idx] = 0;
    _players[defender_idx].Dipl.field_6C[attacker_idx] = 0;

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
; BUG: only stores the low order byte of the spell
; values, invalidating all comparisons
*/
/*

Get_Main_Diplomacy_Choices()
    Get_Exchange_Spell_List(_human_player_idx, m_diplomac_player_idx, 0);
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

            // ; BUG: value doesn't fit into a byte
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

    strcpy(m_diplomacy_message, off_3B95A);  // "What do you offer as tribute?"


    /*
        BEGIN: Build String List
    */

    for(itr = 0; itr < list_count; itr++)
    {

        SDL_itoa(gold_amounts[itr], string, 10);

        strcpy(G_Some_DIPL_Allocs_7[itr], lstr_gold);

        strcat(G_Some_DIPL_Allocs_7[itr], string);

        strcat(G_Some_DIPL_Allocs_7[itr], aGold_0);

        if(gold_amounts[itr] < _players[_human_player_idx].gold_reserve)
        {

            list_item_active_flags[itr] = ST_FALSE;

        }

    }

    strcpy(G_Some_DIPL_Allocs_7[itr], aSpells);

    strcpy(G_Some_DIPL_Allocs_7[(1 + itr)], aForgetIt_0);

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

        if(_variable >= list_count)
        {

            Get_Exchange_Spell_List(HUMAN_PLAYER_IDX, m_diplomac_player_idx, byte_42E39[itr]);

            if(m_exchange_spell_count > 0)
            {

                _variable = 0;

                for(itr = 0; ((m_exchange_spell_count > itr) && (itr < 4)); itr++)
                {
                    assert(m_exchange_spell_list[itr] != 0);

                    strcpy(G_Some_DIPL_Allocs_7[itr], lstr_gold);

                    _fstrcpy(string, spell_data_table[m_exchange_spell_list[itr]].name);

                    strcat(G_Some_DIPL_Allocs_7[itr], string);

                }

                strcpy(G_Some_DIPL_Allocs_7[itr], aForgetIt_0);

                *G_Some_DIPL_Allocs_7[1] = '\0';

                strcpy(m_diplomacy_message, off_3B95A);

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

                        // BUGBUG  improvement should be added to CP, not HP
                        _players[HUMAN_PLAYER_IDX].Dipl.peace_modifier[m_diplomac_player_idx] += var_8;

                        SETMAX(_players[HUMAN_PLAYER_IDX].Dipl.peace_modifier[m_diplomac_player_idx], 100);

                        // BUGBUG  improvement should be added to CP, not HP
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
        else  /* (_variable < var_4) */
        {

            if(_players[m_diplomac_player_idx].Astr.Army_Strength != 0)
            {

                var_6 = ((_variable + 1) * 4);

            }
            else
            {

                var_6 = 8;

            }

            itr = Random(3);

            var_8 = ((var_6 * itr) / 10);

            _players[HUMAN_PLAYER_IDX].gold_reserve -= gold_amounts[_variable];

            Player_Add_Gold(m_diplomac_player_idx, gold_amounts[_variable]);

            SETMAX(var_8, 100);

            _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] += var_8;

            if(_players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] > 100)
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

        temporary_modifier += 35;

    }


    relations = (-125
        + _players[HUMAN_PLAYER_IDX].Dipl.Hidden_Rel[m_diplomac_player_idx]
        + (_players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] / 2)
        + var_10
        + TBL_AI_PRS_IDK_Mod[_players[m_diplomac_player_idx].Personality]);

    final_value = relations + Random(100);

    Get_Exchange_Spell_List(m_diplomac_player_idx, HUMAN_PLAYER_IDX, 0);

    if(m_exchange_spell_count > 0)
    {

        byte_42E4D = (uint8_t)m_exchange_spell_count;

        for(itr = 0; m_exchange_spell_count > itr; itr++)
        {

            byte_42E43[itr] = m_exchange_spell_list[itr];

            // ; BUG: these don't fit into a byte variable!

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

                strcpy(G_Some_DIPL_Allocs_7[itr], lstr_gold);

                _fstrcpy(string, spell_data_table[byte_42E2E[itr]].name);

                strcat(G_Some_DIPL_Allocs_7[itr], string);

            }

            *G_Some_DIPL_Allocs_7[itr] = '\0';

            strcpy(m_diplomacy_message, off_3B95C);

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

                    strcpy(G_Some_DIPL_Allocs_7[itr], lstr_gold);

                    _fstrcpy(string, spell_data_table[IDK_DIPLO_NIU[((var_E * 10) + itr)]].name);

                    strcat(G_Some_DIPL_Allocs_7[itr], string);

                }

                strcat(G_Some_DIPL_Allocs_7[itr], aForgetIt_0);

                var_6 = itr;

                *G_Some_DIPL_Allocs_7[(1 + var_6)] = '\0';

                _variable = 0;

                strcpy(m_diplomacy_message, off_3B95E);

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

                    Diplomacy_Player_Gets_Spell(_human_player_idx, spell_idx);

                    Diplomacy_Player_Gets_Spell(m_diplomac_player_idx, Spell_Index);

                    if(spell_data_table[spell_idx].research_cost < spell_data_table[Spell_Index].research_cost)
                    {

                        Adjust_Diplomat_Modifiers(HUMAN_PLAYER_IDX, m_diplomac_player_idx);
                        Adjust_Diplomat_Modifiers(HUMAN_PLAYER_IDX, m_diplomac_player_idx);

                        Change_Relations__WIP(5, HUMAN_PLAYER_IDX, m_diplomac_player_idx, 0, ST_NULL, ST_NULL);

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
    uint8_t * enchantments = 0;
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
/*

XREF:
    j_IDK_Dipl_s72690()
        Resolve_Delayed_Diplomacy_Orders
            Next_Turn_Calc()

*/
static void Npc_Diplomacy_Screen(void)
{
    int16_t full_screen_field = 0;
    int16_t input_field_idx = 0;  // _DI_
    int16_t leave_screen = 0;  // _SI_
    int16_t var_4 = 0;

    Limit_Temporary_Peace_Modifier();

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

    _page_flip_effect = pfe_None;

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

        m_IDK_break_alliance_player3 = _players[HUMAN_PLAYER_IDX].Dipl.field_102[m_diplomac_player_idx];

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

                            // ¿ BUGBUG  should be setting them equivalent, not to a player's relations with itself ?
                            _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx] = _players[m_diplomac_player_idx].Dipl.Visible_Rel[m_diplomac_player_idx];

                            Adjust_Diplomat_Modifiers(HUMAN_PLAYER_IDX, m_diplomac_player_idx);

                            Adjust_Diplomat_Modifiers(m_diplomac_player_idx, HUMAN_PLAYER_IDX);

                        }

                        if(m_IDK_diplomatic_order == do_IDK_war)
                        {

                            Declare_War(HUMAN_PLAYER_IDX, m_IDK_break_alliance_player3);

                        }

                        if(
                            (m_IDK_diplomatic_order == do_IDK_treaty1)
                            ||
                            (m_IDK_diplomatic_order == do_IDK_treaty2)
                            ||
                            (m_IDK_diplomatic_order == do_IDK_peace)
                        )
                        {

                            Player_Add_Gold(_human_player_idx, _players[HUMAN_PLAYER_IDX].Dipl.Tribute_Gold[m_diplomac_player_idx]);

                            if(_players[HUMAN_PLAYER_IDX].Dipl.Tribute_Spell[m_diplomac_player_idx] != spl_NONE)
                            {

                                Diplomacy_Player_Gets_Spell(HUMAN_PLAYER_IDX, _players[HUMAN_PLAYER_IDX].Dipl.Tribute_Spell[m_diplomac_player_idx]);

                            }

                        }

                    }

                    if(
                        (input_field_idx > ST_UNDEFINED)
                        &&
                        (m_IDK_diplomatic_order == do_IDK_exchange)
                    )
                    {

                        Diplomacy_Player_Gets_Spell(HUMAN_PLAYER_IDX, _players[HUMAN_PLAYER_IDX].Dipl.field_A8[((m_diplomac_player_idx * 12) + input_field_idx)]);

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

            full_screen_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, (int16_t)str_empty_string__ovr086[0], ST_UNDEFINED);

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

    // DOMSDOS  Play_Background_Music__STUB();
    sdl2_Play_Background_Music__WIP();


}


// WZD o86p02
// drake178: DIPL_AI_To_AI()
// MoO2  Module: NPCDIPLO  NPC_To_NPC_Treaty_Negotiations_()
/*
; many BUGs and INCONSISTENCIES inside
; RE-EXPLORE in more context!
*/
/*

*/
void NPC_To_NPC_Treaty_Negotiations__STUB(void)
{

    // ; BUG: what does this have to do with AI-AI diplomacy?
    if(_players[_human_player_idx].casting_spell_idx == spl_Spell_Of_Return)
    {

        return;

    }

    

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

        
        strcpy(G_Some_DIPL_Allocs_7[itr], asc_3B9DB);


    }

    switch(m_IDK_diplomatic_order)
    {

        case do_IDK_treaty1:
        case do_IDK_treaty2:
        case do_IDK_peace:
        {

            strcat(G_Some_DIPL_Allocs_7[0], aAccept_0);

            strcat(G_Some_DIPL_Allocs_7[1], aReject_0);

            G_Some_DIPL_Allocs_7[2] = 0;

            var_4 = 1;

        } break;

        case do_IDK_war:
        {

            strcat(G_Some_DIPL_Allocs_7[0], aAgree);

            strcat(G_Some_DIPL_Allocs_7[1], aForgetIt_1);

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

        strcpy(m_diplomacy_message, G_DIPL_ComposedMessage);

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

        for(itr = 0; itr < 200; itr++)
        {

            if(G_DIPL_ComposedMessage[itr] != 0)
            {

                m_diplomacy_message[pos] = G_DIPL_ComposedMessage[itr];

                // // code = (m_diplomacy_message[pos] - 128);
                // code = m_diplomacy_message[pos];
                // code -= 128;
                // ...
                // 0x7F  127 - 128 = -1
                // 0x80  128 - 128 =  0
                // ...
                // 0x94  148 - 128 = 20
                // ...
                // 0xFF  255 - 128 = 127
                code = m_diplomacy_message[pos];
                code &= 0x7F;


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

                            m_diplomacy_message[pos] = 0;

                            strcat(m_diplomacy_message, _players[HUMAN_PLAYER_IDX].name);

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 1:
                        {

                            m_diplomacy_message[pos] = 0;

                            strcat(m_diplomacy_message, _players[player_idx].name);

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 2:
                        {

                            m_diplomacy_message[pos] = 0;

                            strcat(m_diplomacy_message, _players[_players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[player_idx]].name);

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 3:
                        {

                            m_diplomacy_message[pos] = 0;

                            _fstrcpy(&m_diplomacy_message[pos], _CITIES[_players[HUMAN_PLAYER_IDX].Dipl.DA_City[player_idx]].name);

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 4:
                        {

                            m_diplomacy_message[pos] = 0;

                            strcpy(&m_diplomacy_message[pos], _city_size_names[_CITIES[_players[HUMAN_PLAYER_IDX].Dipl.DA_City[player_idx]].size]);

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 5:
                        {

                            m_diplomacy_message[pos] = 0;

                            strcat(m_diplomacy_message, _players[_players[HUMAN_PLAYER_IDX].Dipl.field_102[player_idx]].name);

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 6:
                        {

                            // DNE or default or increment position?
                            STU_DEBUG_BREAK();

                        } break;

                        case 7:
                        {

                            m_diplomacy_message[pos] = 0;

                            strcat(m_diplomacy_message, _players[_players[HUMAN_PLAYER_IDX].Dipl.field_6C[player_idx]].name);

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 8:
                        {

                            m_diplomacy_message[pos] = 0;

                            SDL_itoa(_players[HUMAN_PLAYER_IDX].Dipl.field_114[player_idx], string, 10);
                            assert(string[30] == 0);

                            strcat(m_diplomacy_message, string);

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 9:
                        {

                            m_diplomacy_message[pos] = 0;

                            _fstrcpy(string, spell_data_table[_players[HUMAN_PLAYER_IDX].Dipl.field_9C[player_idx]].name);
                            assert(string[30] == 0);

                            strcat(m_diplomacy_message, string);

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 10:
                        {

                            m_diplomacy_message[pos] = 0;

                            SDL_itoa(_players[HUMAN_PLAYER_IDX].Dipl.field_114[player_idx], string, 10);
                            assert(string[30] == 0);

                            strcat(m_diplomacy_message, string);

                            strcat(m_diplomacy_message, aGold_2);

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 11:
                        {

                            m_diplomacy_message[pos] = 0;

                            if(_players[HUMAN_PLAYER_IDX].Dipl.Broken_Treaty[player_idx] == 2)
                            {

                                strcat(m_diplomacy_message, aAlliance_0);

                            }
                            else if(_players[HUMAN_PLAYER_IDX].Dipl.Broken_Treaty[player_idx] == 1)
                            {

                                strcat(m_diplomacy_message, aWizardPact_0);
                                
                            }
                            else
                            {

                                strcat(m_diplomacy_message, aTreaty);

                            }

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 12:
                        {

                            m_diplomacy_message[pos] = 0;

                            _fstrcpy(string, spell_data_table[_players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[player_idx]].name);
                            assert(string[30] == 0);

                            strcat(m_diplomacy_message, string);

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 13:
                        {

                            m_diplomacy_message[pos] = 0;

                            strcat(m_diplomacy_message, *_unit_type_table[_players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[player_idx]].name);

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 14:
                        {

                            m_diplomacy_message[pos] = 0;
    
                            strcat(m_diplomacy_message, An(*_unit_type_table[_players[HUMAN_PLAYER_IDX].Dipl.DA_Spell[player_idx]].name));

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 15:
                        {

                            m_diplomacy_message[pos] = 0;

                            if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[player_idx] == 2)
                            {

                                strcat(m_diplomacy_message, aAlliance_0);

                            }
                            else if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[player_idx] == 1)
                            {

                                strcat(m_diplomacy_message, aWizardPact_0);
                                
                            }
                            else
                            {

                                strcat(m_diplomacy_message, aTreaty);

                            }

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 16:
                        {

                            m_diplomacy_message[pos] = 0;

                            SDL_itoa((1400 + (_turn / 12)), string, 10);
                            assert(string[30] == 0);

                            strcat(m_diplomacy_message, string);

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 17:
                        {

                            m_diplomacy_message[pos] = 0;

                            _fstrcpy(string, spell_data_table[Spell_Index].name);
                            assert(string[30] == 0);

                            strcat(m_diplomacy_message, string);

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 18:
                        {

                            m_diplomacy_message[pos] = 0;

                            strcat(m_diplomacy_message, _players[Target_Player].name);

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 19:
                        {

                            m_diplomacy_message[pos] = 0;

                            strcat(m_diplomacy_message, _players[m_IDK_break_alliance_player3].name);

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        case 20:
                        {

                            m_diplomacy_message[pos] = 0;

                            SDL_itoa(word_42E66, string, 10);
                            assert(string[30] == 0);

                            strcat(m_diplomacy_message, string);

                            pos = (int16_t)strlen(m_diplomacy_message);

                        } break;

                        default:
                        {

                            STU_DEBUG_BREAK();

                        } break;

                    }

                }

            }

        }

        m_diplomacy_message[pos] = 0;

    }

    strcpy(G_DIPL_ComposedMessage, m_diplomacy_message);  // pointless?

    Start_Diplomacy_Music(m_diplomacy_message_record_data.IDK_mood);

    m_diplomacy_message_IDK_group = m_diplomacy_message_record_data.IDK_group;

}



/*
    WIZARDS.EXE  ovr087
*/

// WZD o87p01
// drake178: DIPL_ContactProgress()
// MoO2  Module: DIPLOMAC  Determine_First_Contacts_()
/*
progress the human player's contact with other
wizards - if progress is 0, but contacted is 1,
set progress to 1 and relations to no treaty
*/
/*

*/
void Determine_First_Contacts(void)
{
    int16_t itr = 0;  // _CX_

    for(itr = 1; itr < _num_players; itr++)
    {

        if(
            (_players[_human_player_idx].Dipl.Contact_Progress[itr] == 0)
            ||
            (_players[_human_player_idx].Dipl.Contact_Progress[itr] == 1)
        )
        {

            _players[_human_player_idx].Dipl.Contact_Progress[itr] = 1;

            _players[_human_player_idx].Dipl.Dipl_Status[itr] = DIPL_NoTreaty;

            _players[itr].Dipl.Dipl_Status[_human_player_idx] = DIPL_NoTreaty;

        }

    }

}


// WZD o87p02
// MoO2  Module: NPCDIPLO  NPC_To_Human_Diplomacy_()
/*

*/
void NPC_To_Human_Diplomacy__WIP(void)
{
    int16_t Total_Score = 0;
    int16_t Lowest_Interest = 0;
    int16_t player_idx = 0;  // _SI_
    int16_t di = 0;  // _DI_

    if(_players[_human_player_idx].casting_spell_idx == spl_Spell_Of_Return)\
    {

        return;

    }

    Limit_Temporary_Peace_Modifier();

    for(player_idx = 1; player_idx < _num_players; player_idx++)
    {

        if(_FORTRESSES[player_idx].active == ST_FALSE)
        {

            _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] = do_None;

        }
        else if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Status[player_idx] == DIPL_Crusade)
        {

            _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] = do_None;

        }
        else if(_players[HUMAN_PLAYER_IDX].Dipl.Contact_Progress[player_idx] == 1)  /* ¿ New Contact ? */
        {

            _players[HUMAN_PLAYER_IDX].Dipl.Contact_Progress[player_idx] = 2;

            _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] = (15 + _players[player_idx].Personality);

        }
        else if(_players[HUMAN_PLAYER_IDX].Dipl.field_120[player_idx] != 0)
        {

            _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] = 2;

        }
        else
        {

            if(
                (_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] != 39)
                &&
                (_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] != 30)
                &&
                (_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] != 40)
                &&
                (_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] != 41)
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

                if((Total_Score + _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]) >= -100)
                {

                    _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] = do_None;

                }
                else
                {

                    di = _players[HUMAN_PLAYER_IDX].Dipl.DA_Strength[player_idx];

                    if(_players[HUMAN_PLAYER_IDX].Dipl.DA_Strength[player_idx] < 0)
                    {

                        if(
                            (_players[HUMAN_PLAYER_IDX].Dipl.field_126[player_idx] > 0)
                            ||
                            (Random(75) < abs(di))
                        )
                        {

                            // TODO  DIPL_HumanWarOrPeace(player_idx);

                        }
                        else
                        {

                            _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] = do_None;

                        }

                    }
                    else
                    {

                        if(
                            (_players[_human_player_idx].Dipl.Dipl_Status[player_idx] == DIPL_War)
                            &&
                            ((30 + Random(100)) < _players[HUMAN_PLAYER_IDX].Dipl.peace_modifier[player_idx])
                        )
                        {

                            // TODO  DIPL_HumanWarOrPeace(player_idx);

                        }
                        else
                        {

                            if(_players[HUMAN_PLAYER_IDX].Dipl.field_126[player_idx] == 0)
                            {

                                di += 3;

                            }

                            if(
                                (Random(100) < di)
                                &&
                                (Random(4) == 1)
                            )
                            {

                                if(_players[HUMAN_PLAYER_IDX].Dipl.field_126[player_idx] > 0)
                                {

                                    // ¿ BUGBUG  doesn't set a diplomatic order ?
                                    _players[HUMAN_PLAYER_IDX].Dipl.field_126[player_idx] = 0;

                                }
                                else
                                {

                                    // TODO  sub_34C10(player_idx);

                                }

                            }
                            else
                            {

                                // TODO  IDK_Dipl_s73F1C(player_idx);

                            }

                        }

                    }

                }

            }

        }

        if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] == do_None)
        {

            // TODO  G_DIPL_NeedForWar(HUMAN_PLAYER_IDX, player_idx);

        }

        if(_players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[player_idx] == do_None)
        {

            // TODO  DIPL_GetOffMyLawn(player_idx, HUMAN_PLAYER_IDX);

        }

    }

}


// WZD o87p03
// G_DIPL_NeedForWar()

// WZD o87p04
// G_DIPL_SuperiorityWar()

// WZD o87p05
// IDK_Dipl_s73F1C()

// WZD o87p06
// MoO2  Module: DIP-SCRN  Resolve_Delayed_Diplomacy_Orders_()
/*
OON XREF:  Next_Turn_Calc()
*/
void Resolve_Delayed_Diplomacy_Orders(void)
{
    int16_t itr = 0;  // _SI_
    
    if(_players[_human_player_idx].casting_spell_idx != spl_Spell_Of_Return)
    {

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

}


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
void End_Of_Turn_Diplomacy_Adjustments(void)
{
    int16_t itr1 = 0;  // _CX_
    int16_t itr2 = 0;  // _SI_

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
// sub_74420()
// IDK_Dipl_Npc_Demand__STUB()

// WZD o88p02
// DIPL_HumanWarOrPeace()


// WZD o88p03
// MoO2  Module: DIPLOMAC  Modifier_Diplomacy_Adjustments_()
// 1oom  game_turn.c  game_turn_update_mood_blunder()
/*

*/
void Modifier_Diplomacy_Adjustments(void)
{
    int16_t itr3 = 0;
    int16_t IDK_Dipl_Action = 0;
    int16_t itr2 = 0;  // _DI_
    int16_t itr1 = 0;  // _SI_

    for(itr1 = 0; itr1 < _num_players; itr1++)
    {

        for(itr2 = 1; itr2 < _num_players; itr2++)
        {

            if(itr2 != itr1)
            {

                IDK_Dipl_Action = _players[itr1].Dipl.Dipl_Action[itr2];

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

            if(_players[itr2].Dipl.field_120[itr3] != 0) // ¿ e_1oom_PLAYER_NONE ?
            {

                _players[itr2].Dipl.field_6C[itr3] = 0;

                _players[itr2].Dipl.field_120[itr3] = 0;

            }

            _players[itr2].Dipl.field_F0[itr3] = 0;

            _players[itr2].Dipl.Tribute_Spell[itr3] = 0;

            _players[itr2].Dipl.field_9C[itr3] = 0;

            _players[itr2].Dipl.Tribute_Gold[itr3] = 0;

        }

    }

}


// WZD o88p04
// sub_74E2F()
// WZD o88p05
// sub_74E38()
// WZD o88p06
// sub_74F4A()
