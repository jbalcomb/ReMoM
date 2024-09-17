/*
    WIZARDS.EXE
        ovr117
        ovr128
        ovr130
        ovr132
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
extern SAMB_ptr _spellbook_small_symbols[NUM_MAGIC_REALMS];

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

// WZD dseg:C8FC
extern int16_t SBK_NewSpellAnim_Stg;

// WZD dseg:C8FE
// drake178: IMG_SBK_BookRealms@ SPBK_ICONS
// PRIVATE  extern SAMB_ptr spellbook_symbols_bitm[6];

// WZD dseg:C90A
// drake178:  the predicted amount of mana that the wizard will
// drake178:  be able to channel into a spell each turn?
// drake178:  (lower of mana+manaperturn and skill+heroskill)
// drake178:  in combat, the highest castable cost
// updated by 'Magic Screen'
extern int16_t SBK_BookManaLimit;

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
extern int16_t GUI_Multipurpose_Int;

// WZD dseg:C928
extern char * SBK_Descriptions[];

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
void SBK_AddHumanPlayer(int16_t Book_Type, SAMB_ptr SpList_Ptr);

// WZD o117p02
// SBK_AddRealm()

// WZD o117p03
void SBK_CountSpellGroups(void);

// WZD o117p04
void SBK_BuildSpellbook__WIP(int16_t Book_Type, int16_t Page_Size);

// WZD o117p05
// sub_9A9E8()

// WZD o117p06
// CMB_BuildSpellbook()

// WZD o117p07
void OVL_ComposeBookText(struct s_SPELL_BOOK_PAGE Page, SAMB_ptr spellbook_bitmap);

// WZD o117p08
// CMB_ComposeBookText()

// WZD o117p09
void OVL_DrawSpellbook(int16_t x, int16_t y);

// WZD o117p10
// CMB_DrawSpellbook()

// WZD o117p11
void SBK_PageTurn_Small(int turn_type, int combat_flag, int player_idx);

// WZD o117p12
void SBK_AddPages(int16_t Spell_Count, int16_t Grp_Index, char * Title, int16_t Page_Size);

// WZD o117p13
void SBK_Fill(int16_t Page_Size);




/*
    WIZARDS.EXE  ovr118
*/

// WZD o118p01
void SBK_PageFlip_Big__WIP(int16_t direction);

// WZD o118p02
void SBK_ApprenticeBook__WIP(void);

// WZD o118p03
// GAME_NewSpellAnim()

// WZD o118p04
// VGA_DrawBlackScreen()

// WZD o118p05
// GAME_LoadNewSpell()

// WZD o118p06
// GAME_LearnSpellAnim()

// WZD o118p07
// SBK_DrawSpellDecode()

// WZD o118p08
void SBK_LoadSpellDescs__WIP(int16_t page);

// WZD o118p09
void Apprentice_Screen__WIP(void);

// WZD o118p10
void SBK_ComposeApprPage__WIP(int16_t page, SAMB_ptr seg, int16_t flag);

// WZD o118p11
// SBK_Research_Dialog()

// WZD o118p12
// GAME_CheckResearch()



/*
    WIZARDS.EXE  ovr128
*/

// WZD o128p01
int16_t WIZ_RefreshResearch__STUB(int16_t player_idx);

// WZD o128p02
void WIZ_GetResearchList__STUB(int16_t player_idx, int16_t research_list[]);

// WZD o128p03
int16_t WIZ_RollSpellReward(int16_t player_idx, int16_t rarity);

// WZD o128p04
// WIZ_GetSpellValue()

// WZD o128p05
// WIZ_SpellTradeList()

// WZD o128p06
// sub_AC19E()

// WZD o128p07
// WIZ_GE_Diplomacy()

// WZD o128p08
void WIZ_ResearchSort__STUB(int16_t player_idx, int16_t count);

// WZD o128p09
void WIZ_AddSpellbook__WIP(int16_t player_idx, int16_t magic_realm);

// WZD o128p10
// sub_ACACC()

// WZD o128p11
// WIZ_ConquestSpells()




/*
    WIZARDS.EXE  ovr130
*/

// WZD o130p01
void Hero_Slot_Types(int16_t unit_type_idx, int16_t item_slots[]);

// WZD o130p02
// sub_AF331()

// WZD o130p03
// sub_AF56D()

// WZD o130p04
// sub_AF7C1()

// WZD o130p05
// sub_AF9AA()

// WZD o130p06
// sAFA06_Anim_EarthLore()

// WZD o130p07
// sub_AFB7F()

// WZD o130p08
// sub_AFCA8()

// WZD o130p09
// sub_B01F7()

// WZD o130p10
// sub_B0692()

// WZD o130p11
// sub_B0C07()

// WZD o130p12
// sub_B1108()

// WZD o130p13
// sB1280_Anim_EnchantRoad()

// WZD o130p14
// sub_B148C()

// WZD o130p15
// sub_B1843()

// WZD o130p16
// CTY_Consecration()

// WZD o130p17
// sub_B1A01()

// WZD o130p18
// sub_B1ABE()



/*
    WIZARDS.EXE  ovr132
*/

// WZD o132p01
// sub_B4250()

// WZD o132p02
// sub_B4471()

// WZD o132p03
// CTY_ChaosRift()

// WZD o132p04
// WIZ_MeteorStorm()

// WZD o132p05
// CTY_StreamOfLife()

// WZD o132p06
int16_t WIZ_HireHero(int16_t player_idx, int16_t Type, int16_t Hero_Slot, int16_t Saved);

// WZD o132p07
// sub_B4E00()

// WZD o132p08
// sub_B50AE()

// WZD o132p09
// sub_B517B()

// WZD o132p10
// IDK_SplCst_sB529D()

// WZD o132p11
// WIZ_GreatWasting()

// WZD o132p12
// CTY_GaiasBlessing()

// WZD o132p13
// WIZ_Armageddon()

// WZD o132p14
// sub_B5D8E()

// WZD o132p15
// CTY_NightshadeDispel()

// WZD o132p16
// sub_B609C()

// WZD o132p17
// sub_B62F7()

// WZD o132p18
// sub_B6505()



#ifdef __cplusplus
}
#endif


#endif  /* SPELLBOOK_H */
