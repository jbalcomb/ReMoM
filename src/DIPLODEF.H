#ifndef DIPLODEF_H
#define DIPLODEF_H

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

    Diplomatic Orders

_players[].Dipl.Dipl_Action[]
cleared/reset to 0

*/

enum e_DIPLOMATIC_ORDERS
{
    do_None = 0,

    do_IDK_greeting_positive  = 42,
    do_IDK_greeting_neutral   = 43,
    do_IDK_greeting_negative  = 44, /* set in IDK_DiplSts_s70570(), if 'core recation' <= -100 or 'War' */

    do_IDK_treaty1   = 45,
    do_IDK_treaty2   = 46,
    do_IDK_war       = 47,
    do_IDK_exchange  = 48,
    do_IDK_peace     = 49,

    // 51
    // 52
    // 53
    do_IDK_gift_spell = 54,
    // 55
    // 56

    // 57

    // 58
    // 59
    // 60
    // 61
    // 62
    // 63
    // 64
    // 65
    // 66
    
    do_IDK_exchange_spell_goodbye = 71  /* Find_Worst_Modifier() < -100 */

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



#define DIPLO_MAIN_LIST_COUNT 6
#define DIPLO_PROPOSE_TREATY_LIST_COUNT 6
#define DIPLO_BREAK_TREATY_LIST_COUNT 4
#define DIPLO_TRIBUTE_LIST_COUNT 6
#define DIPLO_TRIBUTE_SPELL_LIST_COUNT 6
#define DIPLO_EXCHANGE_LIST_COUNT 6



#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif


#endif  /* DIPLODEF_H */
