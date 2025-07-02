/*
    Diplomacy

    WIZARDS.EXE
        ovr084
        ovr085
        ovr086
        ovr087
        ovr088

MoO2
    Module: DIPLODEF
    Module: DIP-SCRN
    Module: DIPLOMAC
*/

#ifndef DIPLOMAC_H
#define DIPLOMAC_H

#include "MOX/MOX_TYPE.H"






// WZD dseg:C308                                                 BEGIN:  ovr084 - Uninitialized Data

// WZD dseg:C3FE
extern int16_t m_diplomac_player_idx;

// WZD dseg:C404                                                 END:  ovr086 - Uninitialized Data



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr084
*/

// WZD o84p01
void Diplomacy_Screen__WIP(void);

// WZD o84p02
// static void Diplomacy_Screen_Draw__WIP(void);

// WZD o84p01
// IDK_Diplo_Add_Spell_Or_Gold_To_Message()

// WZD o84p02
// sub_6ED5D()

// WZD o84p03
// static void IDK_Diplo_Scrn(void);

// WZD o84p04
// static void Diplomacy_Propose_Treaty__STUB(void);

// WZD o84p05
// sub_6F3E4()

// WZD o84p06
// static void IDK_Diplo_Response(int16_t diplomatic_order, int16_t IDK2);

// WZD o84p07
// sub_6F6BB()

// WZD o84p08
// static void _sub_6F90E_Draw(void);

// WZD o84p09
// static void Diplomacy_Break_Treaty__STUB(void);

// WZD o84p10
// static void IDK_Diplomacy_Background_Music__STUB(int16_t IDK);

// WZD o84p11
// static void IDK_DiplAnim_s6FDA1(void);

// WZD o84p12
// static void Diplomacy_Screen_Load__WIP(void);

// WZD o84p13
// static void DIPL_LoadTalkGFX(void);

// WZD o84p14
// static void IDK_DiplEyes_s7028D(void);

// WZD o84p15
// static void IDK_Dipl_Draw_s702E6(void);

// WZD o84p16
// sub_7038D()

// WZD o84p17
// DIPL_GetInvasionStr()



/*
    WIZARDS.EXE  ovr085
*/

// WZD o85p01
static void Diplomacy_Greeting(void);

// WZD o85p02
// DIPL_Gravitation()

// WZD o85p03
// int16_t IDK_Dipl_PorposeBreakTreaty(void)

// WZD o85p04
// sub_70AFE()

// WZD o85p05
// static int16_t DIPL_LowestInterest(void);

// WZD o85p06
// static void Limit_Temporary_Peace_Modifier(void);

// WZD o85p07
// static void WIZ_DIPL_TeachSpell(int16_t player_idx, int16_t spell_idx);

// WZD o85p08
// G_DIPL_PickSides()

// WZD o85p09
// static void DIPL_SignTreaty__STUB(int16_t player1, int16_t player2, int16_t type);

// WZD o85p10
// static void DIPL_SignPeaceTreaty__STUB(int16_t player1, int16_t player2);

// WZD o85p11
// static void Adjust_Diplomat_Modifiers(int16_t player1, int16_t player2);

// WZD o85p12
void Change_Relations__WIP(int16_t value, int16_t attacker_idx, int16_t defender_idx, int16_t type, int16_t city_idx, int16_t spell_idx);

// WZD o85p13
void Declare_War(int16_t attacker_idx, int16_t defender_idx);

// WZD o85p14
void Break_Treaties(int16_t attacker_idx, int16_t defender_idx);

// WZD o85p15
// static int16_t Get_Exchange_Spell_List(int16_t player1, int16_t player2, int16_t min_value);

// WZD o85p16
// static void Diplomacy_Offer_Tribute__STUB(void)

// WZD o85p17
// static void Diplomacy_Exchange_Spell__STUB(void);

// WZD o85p18
// DIPL_DropCityCurses()



/*
    WIZARDS.EXE  ovr086
*/

// WZD o86p01
// static void Npc_Diplomacy_Screen(void);

// WZD o86p02
// DIPL_AI_To_AI()

// WZD o86p03
// static int16_t DIPL_sub_72DB6(void);

// WZD o86p04
// DIPL_sub_72DB6_Draw()

// WZD o86p05
// static void IDK_DIPLOMSG_s732D9(int16_t IDK, int16_t player_idx);



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
void Resolve_Delayed_Diplomacy_Orders(void);

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




#ifdef __cplusplus
}
#endif


#endif  /* DIPLOMAC_H */
