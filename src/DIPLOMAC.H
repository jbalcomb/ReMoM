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


/*
    Diplomatic Relations
    Treaty Status

_players[].Dipl.Dipl_Status[]
_players[].Dipl.Visible_Rel[]
_players[].Hostility[]

_players[].Hostility[]
    OSG
        four levels of aggression
        None, Annoyed, Warlike, Jihad

*/
enum DIPL_Status
{
    DIPL_NoTreaty    = 0,
    DIPL_WizardPact  = 1,
    DIPL_Alliance    = 2,
    DIPL_War         = 3,
    DIPL_Crusade     = 4
};


/*
    Diplomatic Action Type

; 00 - aura of majesty, pact/alliance gravitation
; 01 - AI to AI friendly gravitation
02 ?
03 ?
04 ?
; 05 - unit near city, breaking treaty
06 ?  MoO2  Apply_Bio_Weapon_Penalty_()
; 07 - army too strong
; 08 - defensive battle losses
; 09 - city attacked
; 0A - spell of mastery
0B ?
; 0C - global enchantment
0D ?
; 0E - overextension
NOTE: some code checks for `<= 14` ¿ meaningful, how ?

; 0F - greeting, maniacal
; 10 - greeting, ruthless
; 11 - greeting, aggressive
; 12 - greeting, chaotic
; 13 - greeting, lawful
; 14 - greeting, peaceful

; 1E - superiority war, in pact/alliance

; 27 - superiority war, bad relations

; 29 - superiority war, good relations

; 31 - peace offer?

; 49 - unit near city, in pact/alliance
*/
enum e_DIPLOMATIC_ACTION
{
    dipact_AuraOfMajesty      =  0,  /* aura of majesty, pact/alliance gravitation  */
    /* AI to AI friendly gravitation               */

    /* unit near city, breaking treaty             */

    /* army too strong                             */
    /* defensive battle losses                     */
    /* city attacked                               */
    /* spell of mastery                            */

    /* global enchantment                          */

    dipact_Overextension      = 14,  /* overextension                               */

    dipact_Greeting_Maniacal    = 15,  /* greeting, maniacal                          */
    dipact_Greeting_Ruthless    = 16,  /* greeting, ruthless                          */
    dipact_Greeting_Aggressive  = 17,  /* greeting, aggressive                        */
    dipact_Greeting_Chaotic     = 18,  /* greeting, chaotic                           */
    dipact_Greeting_Lawful      = 19,  /* greeting, lawful                            */
    dipact_Greeting_Peaceful    = 20,  /* greeting, peaceful                          */

    /* superiority war, in pact/alliance           */
    /* superiority war, bad relations              */
    /* superiority war, good relations             */
    /* peace offer?                                */
    /* unit near city, in pact/alliance            */
};






// WZD dseg:C308                                                 BEGIN:  ovr084 - Uninitialized Data

// WZD dseg:C308 00 00                                           word_42DA8 dw 0                         ; DATA XREF: sub_6EFA5:loc_6F07Aw ...
// WZD dseg:C30A 00 00                                           G_Some_DIPL_Alloc_5 dw 0                ; DATA XREF: sub_6EFA5+111r ...
// WZD dseg:C30A                                                                                         ; 20 LBX_NearAlloc_Next bytes
// WZD dseg:C30C 00 00                                           G_Some_DIPL_Alloc_4 dw 0                ; DATA XREF: sub_6EFA5+B6r ...
// WZD dseg:C30C                                                                                         ; 20 LBX_NearAlloc_Next bytes
// WZD dseg:C30E 00 00                                           word_42DAE dw 0                         ; DATA XREF: sub_70AFE:loc_70CAFw
// WZD dseg:C310 00                                              db    0
// WZD dseg:C311 00                                              db    0
// WZD dseg:C312 00 00 00 00 00 00 00 00 00 00 00 00             G_Some_DIPL_Allocs_6 dw 6 dup(0)        ; DATA XREF: DIPL_LoadTalkGFX+79w
// WZD dseg:C312                                                                                         ; 50 LBX_NearAlloc_Next bytes each
// WZD dseg:C31E 00 00                                           G_Some_DIPL_Alloc_3 dw 0                ; DATA XREF: DIPL_LoadTalkGFX+8Dw
// WZD dseg:C31E                                                                                         ; 20 LBX_NearAlloc_Next bytes
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
// WZD dseg:C3C4 00 00                                           Target_Player dw 0                      ; DATA XREF: sub_6EFA5+2D2w ...
// WZD dseg:C3C6 00 00                                           word_42E66 dw 0                         ; DATA XREF: sub_6F982+39Bw ...
// WZD dseg:C3C8 00 00                                           Spell_Index dw 0                        ; DATA XREF: sub_6F51A+D7w ...
// WZD dseg:C3CA 00 00                                           IMG_DIPL_RightEyes@ dw 0                ; DATA XREF: IDK_Diplo_LoadWorldData_s6FF63+AFw ...
// WZD dseg:C3CA                                                                                         ; 22 LBX_NearAlloc_Next bytes holding IMG_Seg addresses
// WZD dseg:C3CC 00 00                                           IMG_DIPL_LeftEyes@ dw 0                 ; DATA XREF: IDK_Diplo_LoadWorldData_s6FF63+A2w ...
// WZD dseg:C3CC                                                                                         ; 22 LBX_NearAlloc_Next bytes holding IMG_Seg addresses
// WZD dseg:C3CE 00 00                                           dw 0
// WZD dseg:C3D0 00 00                                           dw 0
// WZD dseg:C3D2 00 00                                           IMG_DIPL_TalkAnim@ dw 0                 ; DATA XREF: IDK_DiplAnim_s6FDA1+2Ar ...
// WZD dseg:C3D4 00 00                                           word_42E74 dw 0                         ; DATA XREF: Diplomacy_Screen+Dw ...
// WZD dseg:C3D6 00 00                                           word_42E76 dw 0                         ; DATA XREF: sub_6FD5C:loc_6FD94r ...
// WZD dseg:C3D8 00 00                                           word_42E78 dw 0                         ; DATA XREF: sub_6FD5C:loc_6FD86r ...
// WZD dseg:C3DA 00 00                                           dw 0
// WZD dseg:C3DC 00 00                                           word_42E7C dw 0                         ; DATA XREF: IDK_Diplo_LoadWorldData_s6FF63+C5w ...
// WZD dseg:C3DE 00 00                                           word_42E7E dw 0                         ; DATA XREF: Diplomacy_Screen+13w ...
// WZD dseg:C3E0 00 00                                           word_42E80 dw 0                         ; DATA XREF: Diplomacy_Screen+19w ...
// WZD dseg:C3E2 00 00                                           G_DIPL_ComposedMessage dw 0             ; DATA XREF: DIPL_LoadTalkGFX+48w ...
// WZD dseg:C3E2                                                                                         ; 220 LBX_NearAlloc_Next bytes
// WZD dseg:C3E4 00 00                                           G_DIPL_TempMessage dw 0                 ; DATA XREF: Diplomacy_Screen_Draw+83r ...
// WZD dseg:C3E4                                                                                         ; 220 LBX_NearAlloc_First bytes
// WZD dseg:C3E6 00 00                                           word_42E86 dw 0                         ; DATA XREF: Diplomacy_Screen+C8w ...
// WZD dseg:C3E8 00 00                                           word_42E88 dw 0                         ; DATA XREF: Diplomacy_Screen+B0w ...
// WZD dseg:C3EA 00 00                                           G_DIPL_RndMsgIndex dw 0                 ; DATA XREF: Diplomacy_Screen+8Bw ...
// WZD dseg:C3EC 00 00                                           word_42E8C dw 0                         ; DATA XREF: Diplomacy_Screen+58w ...
// WZD dseg:C3EE 00 00                                           word_42E8E dw 0                         ; DATA XREF: Diplomacy_Screen+49w ...
// WZD dseg:C3F0 00 00 00 00 00 00 00 00 00 00 00 00 00 00       G_Some_DIPL_Allocs_7 dw 0, 0, 0, 0, 0, 0, 0
// WZD dseg:C3F0                                                                                         ; DATA XREF: IDK_Diplo_Scrn+C0r ...
// WZD dseg:C3F0                                                                                         ; 100 LBX_NearAlloc_Next bytes each
// WZD dseg:C3FE
extern int16_t G_DIPL_TargetWizard;
// WZD dseg:C400 00 00                                           G_DiploMsg_E0_Field0 dw 0               ; DATA XREF: sub_732D9+2Do ...
// WZD dseg:C402 00 00                                           G_DiploMsg_E0_Field2 dw 0               ; DATA XREF: sub_732D9+3A4r
// WZD dseg:C404 00 00                                           G_DiploMsg_E0_Field4 dw 0               ; DATA XREF: sub_732D9+47r ...

// WZD dseg:C404                                                 END:  ovr086 - Uninitialized Data





#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr084
*/

// WZD o84p01
void Diplomacy_Screen__STUB(void);

// Diplomacy_Screen_Draw                            ovr084
// sub_6ECD9                                      ovr084
// sub_6ED5D                                      ovr084
// sub_6EE03                                      ovr084
// sub_6EFA5                                      ovr084
// sub_6F3E4                                      ovr084
// sub_6F51A                                      ovr084
// sub_6F6BB                                      ovr084
// sub_6F90E                                      ovr084
// sub_6F982                                      ovr084
// sub_6FD5C                                      ovr084
// IDK_DiplAnim_s6FDA1                            ovr084
// IDK_Diplo_LoadWorldData_s6FF63                 ovr084
// DIPL_LoadTalkGFX                               ovr084
// IDK_DiplEyes_s7028D                            ovr084
// IDK_Dipl_Draw_s702E6                           ovr084
// sub_7038D                                      ovr084
// DIPL_GetInvasionStr                            ovr084



/*
    WIZARDS.EXE  ovr085
*/

// WZD o85p01
// IDK_DiplSts_s70570()

// WZD o85p02
// DIPL_Gravitation()

// WZD o85p03
// sub_70A1A()

// WZD o85p04
// sub_70AFE()

// WZD o85p05
// DIPL_LowestInterest()

// WZD o85p06
// DIPL_TreatyIntCaps()

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

// WZD o86p0

// IDK_Dipl_s72690()
// DIPL_AI_To_AI()
// sub_72DB6()
// sub_7323F()
// sub_732D9()



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




#ifdef __cplusplus
}
#endif


#endif  /* DIPLOMAC_H */
