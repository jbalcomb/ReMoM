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
    do_None = 0


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



#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif


#endif  /* DIPLODEF_H */
