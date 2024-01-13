/*
    WIZARDS.EXE
        ovr117
*/

#ifndef SPELLBOOK_H
#define SPELLBOOK_H

#include "MoM.H"

#ifdef __cplusplus
extern "C" {
#endif



// drake178: BOOK_PAGE
// sizeof:  1Eh  30d
#pragma pack(push)
#pragma pack(2)
struct s_SPELL_BOOK_PAGE
{
    /* 00 */   char Title[16];
    /* 10 */   int16_t Spells[6];
    /* 1C */   int8_t Spell_Count;
    /* 1D */   // 2-byte alignment padding  Unused_1Dh db ?
    /* 1E */
};
#pragma pack(pop)



// WZD dseg:9448                                                 ¿ BEGIN: Spellbook (!Screen) - Uninitialized Data ?
// WZD dseg:9448                                                 BEGIN:  ovr118

// WZD dseg:9448
// PRIVATE  extern int16_t SBK_Candidate_Page;

// WZD dseg:944A
extern int16_t SBK_SomePageSaveVar;

// WZD dseg:944C 00 00                                           CMB_SpellBookPage dw 0                  ; DATA XREF: G_WLD_StaticAssetRfrsh+61w ...
// WZD dseg:944E 00 00                                           dw 0
// WZD dseg:9450 00 00                                           dw 0

// WZD dseg:9452
extern SAMB_ptr _spellbook_small_text;

// WZD dseg:9454
// _spellbook_small_symbols SPBK_ICONS <0>
extern SAMB_ptr _spellbook_small_symbols[5];

// WZD dseg:9460 00 00                                           IMG_SPL_BigBook@ dw 0                   ; DATA XREF: GAME_LearnSpellAnim+16Dw ...
// WZD dseg:9460                                                                                         ; LBXE_LoadReplace into the LBX_Sandbox_Segment
// WZD dseg:9462 00 00                                           dw 0
// WZD dseg:9464 00 00                                           IMG_SPL_BigRDogear@ dw 0                ; DATA XREF: SBK_PageFlip_Big+79w ...
// WZD dseg:9466 00 00                                           IMG_SPL_BigLDogear@ dw 0                ; DATA XREF: SBK_PageFlip_Big+62w ...

// WZD dseg:9468
extern SAMB_ptr _spellbook_small_right_corner_seg;

// WZD dseg:946A
extern SAMB_ptr _spellbook_small_left_corner_seg;

// WZD dseg:946C 00 00                                           word_3FF0C dw 0                         ; DATA XREF: sub_BECD9+23w ...

// WZD dseg:946E
extern SAMB_ptr _spellbook_small_seg;

// WZD dseg:946E                                                 END:  ovr118



// WZD dseg:C8FA                                                 ¿ BEGIN: Spellbook (!Screen) - Uninitialized Data ?
// WZD dseg:C8FA                                                 BEGIN:  ovr117

// WZD dseg:C8FA
extern struct s_SPELL_BOOK_PAGE * SBK_BookPages;

// WZD dseg:C8FC 00 00                                           SBK_NewSpellAnim_Stg dw 0               ; DATA XREF: CMB_SetTargetCursor+24Dr ...
// WZD dseg:C8FC                                                                                         ; (0-7), also combat casting cursor anim stage (0-4)

// WZD dseg:C8FE
// drake178: IMG_SBK_BookRealms@ SPBK_ICONS
// PRIVATE  extern SAMB_ptr spellbook_symbols_bitm[6];

// WZD dseg:C90A
// drake178:  the predicted amount of mana that the wizard will
// drake178:  be able to channel into a spell each turn?
// drake178:  (lower of mana+manaperturn and skill+heroskill)
// drake178:  in combat, the highest castable cost
// PRIVATE  extern int16_t SBK_BookManaLimit;

// WZD dseg:C90C
// drake178: NearAlloc_First!
// PRIVATE  extern SAMB_ptr SBK_Research_Incomes;

// WZD dseg:C90E
extern int16_t WIZ_ManaPerTurn;

// WZD dseg:C910
// drake178: 0Dh LBX_Alloc_Space paragraphs (first or next); NOT BIG ENOUGH TO HOLD MORE THAN 104 SPELLS!
// PRIVATE  extern SAMB_ptr SBK_SpellList;

// WZD dseg:C914
extern int16_t SBK_BookPageCount;

// WZD dseg:C916
// drake178: Combat Spells or Sorcery
// PRIVATE  extern int16_t SBK_Group_6_Count;

// WZD dseg:C918
// drake178: Enchantments or Life
// PRIVATE  extern int16_t SBK_Group_5_Count;

// WZD dseg:C91A
// drake178: Unit Spells or Nature
// PRIVATE  extern int16_t SBK_Group_4_Count;

// WZD dseg:C91C
// drake178: City Spells or Death
// PRIVATE  extern int16_t SBK_Group_3_Count;

// WZD dseg:C91E
// drake178: Special Spells or Chaos
// PRIVATE  extern int16_t SBK_Group_2_Count;

// WZD dseg:C920
// drake178: Summoning Spells or Arcane
// PRIVATE  extern int16_t SBK_Group_1_Count;

// WZD dseg:C922
// drake178: apprentice: 0 - choose research; 2 - apprentice book
// drake178: new spell anim: 0 - spell on left; 1 - spell on right
extern int16_t SBK_Dogears;

// WZD dseg:C924
// drake178: ; set to the index + 1 (into the page) of the spell
// drake178: ; being cast while setting up the spellbook display
// drake178: ; in the apprentice, it is then reset to the page count
// drake178: ; without the research candidates
extern int16_t SBK_OpenPage;

// WZD dseg:C926
// drake178:  Spell List Count, New Spell Index, ...
// PRIVATE  extern int16_t GUI_Multipurpose_Int;

// WZD dseg:C928 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+SBK_Descriptions@ dd 0                                    ; 0
// WZD dseg:C928 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; DATA XREF: GAME_LearnSpellAnim+31Ew ...
// WZD dseg:C928 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+dd 0                                    ; 1 ; 9 or 12 LBX_Alloc_Next paragraphs in the
// WZD dseg:C928 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 dd 0                                    ; 2 ; LBX_Sandbox_Segment (after a flush)
// WZD dseg:C928                                                 dd 0                                    ; 3
// WZD dseg:C928                                                 dd 0                                    ; 4
// WZD dseg:C928                                                 dd 0                                    ; 5
// WZD dseg:C928                                                 dd 0                                    ; 6
// WZD dseg:C928                                                 dd 0                                    ; 7
// WZD dseg:C928                                                 dd 0                                    ; 8
// WZD dseg:C928                                                 dd 0                                    ; 9
// WZD dseg:C928                                                 dd 0                                    ; 10
// WZD dseg:C928                                                 dd 0                                    ; 11
// WZD dseg:C928                                                 dd 0                                    ; 12
// WZD dseg:C928                                                 dd 0                                    ; 13
// WZD dseg:C928                                                 dd 0                                    ; 14
// WZD dseg:C928                                                 dd 0                                    ; 15
// WZD dseg:C968 00 00                                           IMG_GAME_Lab_Wizard@ dw 0               ; DATA XREF: GAME_NewSpellAnim+1Cr ...
// WZD dseg:C96A 00 00                                           IMG_GAME_LabFmiliar@ dw 0               ; DATA XREF: GAME_NewSpellAnim+3Ar ...
// WZD dseg:C96C 00 00                                           IMG_GAME_MagicColmn@ dw 0               ; DATA XREF: GAME_NewSpellAnim+4Er ...
// WZD dseg:C96E 00 00                                           IMG_GAME_Lab_Podium@ dw 0               ; DATA XREF: GAME_NewSpellAnim+8r ...
// WZD dseg:C970 00                                              db    0
// WZD dseg:C971 00                                              db    0

// WZD dseg:C971                                                 END:  ovr118



/*
    WIZARDS.EXE  ovr117
*/

// WZD o117p01
// drake178: SBK_AddHumanPlayer()
// PRIVATE  void SBK_AddHumanPlayer(int16_t Book_Type, SAMB_ptr SpList_Ptr);

// WZD o117p02
// drake178: SBK_AddRealm()
// SBK_AddRealm()

// WZD o117p03
// drake178: SBK_CountSpellGroups()
// PRIVATE  void SBK_CountSpellGroups(void);

// WZD o117p04
// drake178: SBK_BuildSpellbook()
void SBK_BuildSpellbook(int16_t Book_Type, int16_t Page_Size);

// WZD o117p05
// drake178: sub_9A9E8()
// sub_9A9E8()

// WZD o117p06
// drake178: CMB_BuildSpellbook()
// CMB_BuildSpellbook()

// WZD o117p07
// drake178: OVL_ComposeBookText()
// PRIVATE  void OVL_ComposeBookText(struct s_SPELL_BOOK_PAGE Page, SAMB_ptr Work_IMG);

// WZD o117p08
// drake178: CMB_ComposeBookText()
// CMB_ComposeBookText()

// WZD o117p09
// drake178: OVL_DrawSpellbook()
void OVL_DrawSpellbook(int16_t x, int16_t y);

// WZD o117p10
// drake178: CMB_DrawSpellbook()
// CMB_DrawSpellbook()

// WZD o117p11
// drake178: SBK_PageFlip_Small()
// BOOK.LBX  "PTURNSML"
void SBK_PageTurn_Small(int turn_type, int combat_flag, int player_idx);

// WZD o117p12
// drake178: SBK_AddPages()
// PRIVATE  void SBK_AddPages(int16_t Spell_Count, int16_t Grp_Index, char * Title, int16_t Page_Size);

// WZD o117p13
// drake178: SBK_Fill()
// PRIVATE  void SBK_Fill(int16_t Page_Size);




/*
    WIZARDS.EXE  ovr118
*/

// WZD o118p01
// drake178: SBK_PageFlip_Big()
// SBK_PageFlip_Big()

// WZD o118p02
// drake178: SBK_ApprenticeBook()
// SBK_ApprenticeBook()

// WZD o118p03
// drake178: GAME_NewSpellAnim()
// GAME_NewSpellAnim()

// WZD o118p04
// drake178: VGA_DrawBlackScreen()
// VGA_DrawBlackScreen()

// WZD o118p05
// drake178: GAME_LoadNewSpell()
// GAME_LoadNewSpell()

// WZD o118p06
// drake178: GAME_LearnSpellAnim()
// GAME_LearnSpellAnim()

// WZD o118p07
// drake178: SBK_DrawSpellDecode()
// SBK_DrawSpellDecode()

// WZD o118p08
// drake178: SBK_LoadSpellDescs()
// SBK_LoadSpellDescs()

// WZD o118p09
// drake178: sub_9DDBC()
// sub_9DDBC()

// WZD o118p10
// drake178: SBK_ComposeApprPage()
// SBK_ComposeApprPage()

// WZD o118p11
// drake178: SBK_Research_Dialog()
// SBK_Research_Dialog()

// WZD o118p12
// drake178: GAME_CheckResearch()
// GAME_CheckResearch()



#ifdef __cplusplus
}
#endif


#endif  /* SPELLBOOK_H */
