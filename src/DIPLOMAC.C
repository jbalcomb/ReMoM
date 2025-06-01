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

#include "MOX/MOX_DAT.H"  /* _players[] */
#include "MOX/SOUND.H"

#include "DIPLOMAC.H"
#include "LOADER.H"
#include "MainScr.H"
#include "MOM_SCR.H"



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
// WZD dseg:4CB6 00                                              cnst_ZeroString_4 db 0                  ; DATA XREF: dseg:off_3B6E8o ...
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
// WZD dseg:4E99 4D 55 53 49 43 00                               cnst_MUSIC_File6 db 'MUSIC',0           ; DATA XREF: IDK_Diplo_LoadWorldData_s6FF63+4Co ...
// WZD dseg:4E99                                                                                         ; should use dseg:2c81
// WZD dseg:4E9F 44 49 50 4C 4F 4D 41 43 00                      cnst_DIPLOMAC_File db 'DIPLOMAC',0      ; DATA XREF: IDK_Diplo_LoadWorldData_s6FF63+B9o ...
// WZD dseg:4EA8 4D 4F 4F 44 57 49 5A 00                         cnst_MOODWIZ_File db 'MOODWIZ',0        ; DATA XREF: DIPL_LoadTalkGFX+D8o
// WZD dseg:4EB0 42 41 43 4B 47 52 4E 44 00                      cnst_BACKGRND_File2 db 'BACKGRND',0     ; DATA XREF: DIPL_LoadTalkGFX+EFo ...

// WZD dseg:4EB9 00                                              align 2

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
// WZD dseg:4FDE 44 49 50 4C 4F 4D 53 47 00                      aDiplomsg db 'DIPLOMSG',0
// WZD dseg:4FE7 20 67 6F 6C 64 00                               aGold_2 db ' gold',0
// WZD dseg:4FED 41 6C 6C 69 61 6E 63 65 00                      aAlliance_0 db 'Alliance',0
// WZD dseg:4FF6 57 69 7A 61 72 64 20 50 61 63 74 00             aWizardPact_0 db 'Wizard Pact',0
// WZD dseg:5002 74 72 65 61 74 79 00                            aTreaty db 'treaty',0
// WZD dseg:5009 00                                              db    0

// WZD dseg:5009                                                 ¿ END:  ?





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
int16_t G_DIPL_TargetWizard = 0;
// WZD dseg:C400 00 00                                           G_DiploMsg_E0_Field0 dw 0               ; DATA XREF: sub_732D9+2Do ...
// WZD dseg:C402 00 00                                           G_DiploMsg_E0_Field2 dw 0               ; DATA XREF: sub_732D9+3A4r
// WZD dseg:C404 00 00                                           G_DiploMsg_E0_Field4 dw 0               ; DATA XREF: sub_732D9+47r ...

// WZD dseg:C404                                                 END:  ovr086 - Uninitialized Data






/*
    WIZARDS.EXE  ovr084
*/


// WZD o84p01
void Diplomacy_Screen__STUB(void)
{



    // @@leave_screen
    // DOMSDOS  Stop_All_Sounds__STUB();
    Deactivate_Auto_Function();
    Clear_Fields();
    current_screen = scr_Magic_Screen;
    Fade_Out();
    PageFlipEffect = 2;
    // TODO  GAME_ReloadWorldData();
    // TODO  GAME_CheckResearch(1);
    Cache_Graphics_Overland();
    // DOSMSODS  Play_Background_Music__STUB();
    sdl2_Play_Background_Music__WIP();

    // @@done

}

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
// drake178: G_DIPL_Action()
// MoO2  Module: DIPLOMAC  Change_Relations_()  Module: NETMOX  Russ_Change_Relations_()
/*
    Applies a change in diplomatic relations

; multiple BUGs, unfinished subfunctions, and unknown variables; needs further study!

called from End_Of_Combat()
    |-> G_DIPL_Action(-Diplomatic_Value, _combat_attacker_player, _combat_defender_player, 8, ST_NULL, ST_NULL);

Spellbook_Screen()
    if(did_select_spell == ST_TRUE)
        WIZ_SetOverlandSpell__WIP(HUMAN_PLAYER_IDX, spell_idx, spellbook_page_spell_index);
            Change_Relations_For_Enchantments(player_idx, spl_Spell_Of_Mastery, 1)
                if(spell_idx == spl_Spell_Of_Mastery)
                    Change_Relations((-50 / divisor), player_idx, itr_players, 10, 0, spell_idx);

*/
void Change_Relations(int16_t value, int16_t attacker_idx, int16_t defender_idx, int16_t type, int16_t city_idx, int16_t spell_idx)
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
