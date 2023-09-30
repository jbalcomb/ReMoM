/*
    Magic Screen

    WIZARDS.EXE
        ovr073
    
    MoO2:
        Module: ¿  ?
*/

#ifndef MAGICSCR_H
#define MAGICSCR_H

#include "MoX.H"

#ifdef __cplusplus
extern "C" {
#endif




// WZD dseg:358E
// PRIVATE  extern char magic_lbx_file[];

// WZD dseg:3594
// AKA cnst_LILWIZ_File
// PRIVATE  extern char lilwiz_lbx_file[];





// WZD dseg:958E
// AKA profile_gem;
// PRIVATE  extern SAMB_ptr lilwiz_gem_pict_segs[];




// WZD dseg:95B0
// PRIVATE  extern SAMB_ptr magic_screen_ok_button_pict_seg;

// WZD dseg:95B2
// PRIVATE  extern SAMB_ptr magic_screen_alchemy_button_pict_seg;

// WZD dseg:95CE
// PRIVATE  extern SAMB_ptr magic_screen_background;



// WZD dseg:C1EA                                                 ¿ BEGIN: Magic Screen - Uninitialized Data ?

// WZD dseg:C1EA
// PRIVATE  extern SAMB_ptr magic_screen_15_treaties_icons[];

// WZD dseg:C208 00 00                                           GAME_AlchemyDivisor dw 0                ; DATA XREF: GAME_AlchemyWindow+10Ew ...
// WZD dseg:C20A 00 00                                           human_player_summoning_circle_city_idx dw 0
// WZD dseg:C20A                                                                                         ; DATA XREF: Magic_Screen+187w ...

// WZD dseg:C20C
// AKA G_GUI_TgtWiz_Labels
// PRIVATE  extern int16_t gem_player_idx[];

// WZD dseg:C214
// AKA IDK_CnctWzdsCnt_w42CB4
// PRIVATE  extern int16_t gem_count;

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
// WZD dseg:C22C 00 00                                           word_42CCC dw 0                         ; DATA XREF: Magic_Screen:loc_60C28r ...

// WZD dseg:C22E
// PRIVATE  extern SAMB_ptr IDK_magic_screen_KwnWzd_fidx_w42CCE;

// WZD dseg:C230
// PRIVATE  extern SAMB_ptr IDK_MagicScrn_OvlEnchLineFields_w42CD0;

// WZD dseg:C232 00 00                                           magic_screen_overland_enchantments_pgdn_field_idx dw 0
// WZD dseg:C232                                                                                         ; DATA XREF: Magic_Screen+61Ar ...
// WZD dseg:C234 00 00                                           IDK_OvlEnch_DoScroll_w42CD4 dw 0        ; DATA XREF: Magic_Screen+16Aw ...
// WZD dseg:C236 00 00                                           IDK_MgcScr_OvlEnch_1stLn_w42CD6 dw 0    ; DATA XREF: Magic_Screen+15Fw ...
// WZD dseg:C238 00 00                                           _overland_enchantment_count dw 0        ; DATA XREF: Magic_Screen+170r ...

// WZD dseg:C23A
// PRIVATE  extern SAMB_ptr G_SPL_SomeNearArray;

// WZD dseg:C23C
// PRIVATE  extern SAMB_ptr G_SPL_GlobalArray;

// WZD dseg:C23E 00 00                                           magic_screen_alchemy_button_field_idx dw 0
// WZD dseg:C23E                                                                                         ; DATA XREF: Magic_Screen:loc_60C31r ...
// WZD dseg:C240 00 00                                           magic_screen_ok_button_field_idx dw 0   ; DATA XREF: Magic_Screen:loc_60A89r ...
// WZD dseg:C242 00 00                                           IDK_ManaPcnt_w42CE2 dw 0                ; DATA XREF: Magic_Screen+13Ew ...
// WZD dseg:C244 00 00                                           IDK_RsrchPcnt_w42CE4 dw 0               ; DATA XREF: Magic_Screen+138w ...
// WZD dseg:C246 00 00                                           IDK_SkillPcnt_w42CE6 dw 0               ; DATA XREF: Magic_Screen+132w ...
// WZD dseg:C248 00 00                                           magic_screen_mana_staff_lock_field_idx dw 0
// WZD dseg:C248                                                                                         ; DATA XREF: Magic_Screen:loc_60A47r ...
// WZD dseg:C24A 00 00                                           magic_screen_research_staff_lock_field_idx dw 0
// WZD dseg:C24A                                                                                         ; DATA XREF: Magic_Screen:loc_60A73r ...
// WZD dseg:C24C 00 00                                           magic_screen_skill_staff_lock_field_idx dw 0
// WZD dseg:C24C                                                                                         ; DATA XREF: Magic_Screen:loc_60A5Dr ...
// WZD dseg:C24E 00 00                                           IDK_YNM_ManaPcnt_w42CEE dw 0            ; DATA XREF: Magic_Screen+12Cw ...
// WZD dseg:C250 00 00                                           IDK_RsrchPcnt_w42CF0 dw 0               ; DATA XREF: Magic_Screen+11Ew ...
// WZD dseg:C252 00 00                                           IDK_SkillPcnt_w42CF2 dw 0               ; DATA XREF: Magic_Screen+112w ...

// WZD dseg:C252                                                 ¿ END: Magic Screen - Uninitialized Data ?





/*
    WIZARDS.EXE  ovr073
*/

// WZD o73p01
void Magic_Screen(void);

// WZD o73p02
void Magic_Screen_Draw(void);

// WZD o73p03
void Magic_Screen_Add_Fields(void);

// WZD o73p04

// WZD o73p05
void Magic_Screen_Load_Pictures(void);

// WZD o73p06
// WZD o73p07
// WZD o73p08



#ifdef __cplusplus
}
#endif

#endif  /* MAGICSCR_H */
