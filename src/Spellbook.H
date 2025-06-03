/*
    WIZARDS.EXE
        ovr117
        ovr128
        ovr130
        ovr132
*/

#ifndef SPELLBOOK_H
#define SPELLBOOK_H

#include "MOM.H"

#ifdef __cplusplus
extern "C" {
#endif



enum e_SPELLS
{
    spl_UNDEFINED           =  -1,  /* Eh? */
    spl_NONE                =   0,
    spl_Earth_To_Mud        =   1,  /* sbr_Nature */
    spl_Resist_Elements     =   2,
    spl_Wall_Of_Stone       =   3,
    spl_Giant_Strength      =   4,
    spl_Web                 =   5,
    spl_War_Bears           =   6,
    spl_Stone_Skin          =   7,
    spl_Water_Walking       =   8,
    spl_Sprites             =   9,
    spl_Earth_Lore          =  10,
    spl_Cracks_Call         =  11,
    spl_Natures_Eye         =  12,
    spl_Ice_Bolt            =  13,
    spl_Giant_Spiders       =  14,
    spl_Change_Terrain      =  15,
    spl_Path_Finding        =  16,
    spl_Cockatrices         =  17,
    spl_Transmute           =  18,
    spl_Natures_Cures       =  19,
    spl_Basilisk            =  20,
    spl_Elemental_Armor     =  21,
    spl_Petrify             =  22,
    spl_Stone_Giant         =  23,
    spl_Iron_Skin           =  24,
    spl_Ice_Storm           =  25,
    spl_Earthquake          =  26,
    spl_Gorgons             =  27,
    spl_Move_Fortress       =  28,
    spl_Gaias_Blessing      =  29,
    spl_Earth_Elemental     =  30,
    spl_Regeneration        =  31,
    spl_Behemoth            =  32,
    spl_Entangle            =  33,
    spl_Natures_Awareness   =  34,
    spl_Call_Lightning      =  35,
    spl_Colossus            =  36,
    spl_Earth_Gate          =  37,
    spl_Herb_Mastery        =  38,
    spl_Great_Wyrm          =  39,
    spl_Natures_Wrath       =  40,
    spl_Resist_Magic        =  41,  /* sbr_Sorcery */
    spl_Dispel_Magic_True   =  42,
    spl_Floating_Island     =  43,
    spl_Guardian_Wind       =  44,
    spl_Phantom_Warriors    =  45,
    spl_Confusion           =  46,
    spl_Word_Of_Recall      =  47,
    spl_Counter_Magic       =  48,
    spl_Nagas               =  49,
    spl_Psionic_Blast       =  50,
    spl_Blur                =  51,
    spl_Disenchant_True     =  52,
    spl_Vertigo             =  53,
    spl_Spell_Lock          =  54,
    spl_Enchant_Road        =  55,
    spl_Flight              =  56,
    spl_Wind_Mastery        =  57,
    spl_Spell_Blast         =  58,
    spl_Aura_Of_Majesty     =  59,
    spl_Phantom_Beast       =  60,
    spl_Disjunction_True    =  61,
    spl_Invisibility        =  62,
    spl_Wind_Walking        =  63,
    spl_Banish              =  64,
    spl_Storm_Giant         =  65,
    spl_Air_Elemental       =  66,
    spl_Mind_Storm          =  67,
    spl_Stasis              =  68,
    spl_Magic_Immunity      =  69,
    spl_Haste               =  70,
    spl_Djinn               =  71,
    spl_Spell_Ward          =  72,
    spl_Creature_Binding    =  73,
    spl_Mass_Invisibility   =  74,
    spl_Great_Unsummoning   =  75,
    spl_Spell_Binding       =  76,
    spl_Flying_Fortress     =  77,
    spl_Sky_Drake           =  78,
    spl_Suppress_Magic      =  79,
    spl_Time_Stop           =  80,
    spl_Warp_Wood           =  81,  /* sbr_Chaos */
    spl_Disrupt             =  82,
    spl_Fire_Bolt           =  83,
    spl_Hell_Hounds         =  84,
    spl_Corruption          =  85,
    spl_Eldritch_Weapon     =  86,
    spl_Wall_Of_Fire        =  87,
    spl_Shatter             =  88,
    spl_Warp_Creature       =  89,
    spl_Fire_Elemental      =  90,
    spl_Lightning_Bolt      =  91,
    spl_Fire_Giant          =  92,
    spl_Chaos_Channels      =  93,
    spl_Flame_Blade         =  94,
    spl_Gargoyles           =  95,
    spl_Fireball            =  96,
    spl_Doombat             =  97,
    spl_Raise_Volcano       =  98,
    spl_Immolation          =  99,
    spl_Chimeras            = 100,
    spl_Warp_Lightning      = 101,
    spl_Metal_Fires         = 102,
    spl_Chaos_Spawn         = 103,
    spl_Doom_Bolt           = 104,
    spl_Magic_Vortex        = 105,
    spl_Efreet              = 106,
    spl_Fire_Storm          = 107,
    spl_Warp_Reality        = 108,
    spl_Flame_Strike        = 109,
    spl_Chaos_Rift          = 110,
    spl_Hydra               = 111,
    spl_Disintegrate        = 112,
    spl_MeteorStorms        = 113,
    spl_Great_Wasting       = 114,
    spl_Call_Chaos          = 115,
    spl_Chaos_Surge         = 116,
    spl_Doom_Mastery        = 117,
    spl_Great_Drake         = 118,
    spl_Call_The_Void       = 119,
    spl_Armageddon          = 120,  /* sbr_Life */
    spl_Bless               = 121,
    spl_Star_Fires          = 122,
    spl_Endurance           = 123,
    spl_Holy_Weapon         = 124,
    spl_Healing             = 125,
    spl_Holy_Armor          = 126,
    spl_Just_Cause          = 127,
    spl_True_Light          = 128,
    spl_Guardian_Spirit     = 129,
    spl_Heroism             = 130,
    spl_True_Sight          = 131,
    spl_Plane_Shift         = 132,
    spl_Resurrection        = 133,
    spl_Dispel_Evil         = 134,
    spl_Planar_Seal         = 135,
    spl_Unicorns            = 136,
    spl_Raise_Dead          = 137,
    spl_Planar_Travel       = 138,
    spl_Heavenly_Light      = 139,
    spl_Prayer              = 140,
    spl_Lionheart           = 141,
    spl_Incarnation         = 142,
    spl_Invulnerability     = 143,
    spl_Righteousness       = 144,
    spl_Prosperity          = 145,
    spl_Altar_Of_Battle     = 146,
    spl_Angel               = 147,
    spl_Stream_Of_Life      = 148,
    spl_Mass_Healing        = 149,
    spl_Holy_Word           = 150,
    spl_High_Prayer         = 151,
    spl_Inspirations        = 152,
    spl_Astral_Gate         = 153,
    spl_Holy_Arms           = 154,
    spl_Consecration        = 155,
    spl_Life_Force          = 156,
    spl_Tranquility         = 157,
    spl_Crusade             = 158,
    spl_Arch_Angel          = 159,
    spl_Charm_Of_Life       = 160,
    spl_Skeletons           = 161,  /* sbr_Death */
    spl_Weakness            = 162,
    spl_Dark_Rituals        = 163,
    spl_Cloak_Of_Fear       = 164,
    spl_Black_Sleep         = 165,
    spl_Ghouls              = 166,
    spl_Life_Drain          = 167,
    spl_Terror              = 168,
    spl_Darkness            = 169,
    spl_Mana_Leak           = 170,
    spl_Drain_Power         = 171,
    spl_Possession          = 172,
    spl_Lycantrophy         = 173,
    spl_Black_Prayer        = 174,
    spl_Black_Channels      = 175,
    spl_Night_Stalker       = 176,
    spl_Subversion          = 177,
    spl_Wall_Of_Darkness    = 178,
    spl_Berserk             = 179,
    spl_Shadow_Demons       = 180,
    spl_Wraith_Form         = 181,
    spl_Wrack               = 182,
    spl_Evil_Presence       = 183,
    spl_Wraiths             = 184,
    spl_Cloud_Of_Shadow     = 185,
    spl_Warp_Node           = 186,
    spl_Black_Wind          = 187,
    spl_Zombie_Mastery      = 188,
    spl_Famine              = 189,
    spl_Cursed_Lands        = 190,
    spl_Cruel_Unminding     = 191,
    spl_Word_Of_Death       = 192,
    spl_Death_Knights       = 193,
    spl_Death_Spell         = 194,
    spl_Animate_Dead        = 195,
    spl_Pestilence          = 196,
    spl_Eternal_Night       = 197,
    spl_Evil_Omens          = 198,
    spl_Death_Wish          = 199,
    spl_Demon_Lord          = 200,
    spl_Magic_Spirit        = 201,  /* sbr_Arcane */
    spl_Dispel_Magic        = 202,
    spl_Summoning_Circle    = 203,
    spl_Disenchant_Area     = 204,
    spl_Recall_Hero         = 205,
    spl_Detect_Magic        = 206,
    spl_Enchant_Item        = 207,
    spl_Summon_Hero         = 208,
    spl_Awareness           = 209,
    spl_Disjunction         = 210,
    spl_Create_Artifact     = 211,
    spl_Summon_Champion     = 212,
    spl_Spell_Of_Mastery    = 213,
    spl_Spell_Of_Return     = 214
};

enum e_SPELL_LIBRARY_STATUS
{
    sls_Unknown         = 0,
    sls_Knowable        = 1,
    sls_Known           = 2,
    sls_Researchable    = 3
};


// drake178: BOOK_PAGE
// sizeof:  1Eh  30d
#pragma pack(push)
#pragma pack(2)
struct s_SPELL_BOOK_PAGE
{
    /* 00 */   char title[16];
    /* 10 */   int16_t spell[6];
    /* 1C */   int8_t count;  // 1-byte, signed
    /* 1D */   /* 2-byte alignment padding */
    /* 1E */
};
#pragma pack(pop)

// drake178: N/A
// sizeof:  0Ch  12d
#pragma pack(push)
#pragma pack(2)
struct s_SPELL_DECODE
{
    /* 00 */    int16_t field_0;
    /* 02 */    int16_t field_2;
    /* 04 */    int16_t field_4;
    /* 06 */    int16_t field_6;
    /* 08 */    int16_t field_8;
    /* 0A */    int16_t field_A;
    /* 0C */
};
#pragma pack(pop)



// WZD dseg:9448                                                 ¿ BEGIN: Spellbook (!Screen) - Uninitialized Data ?
// WZD dseg:9448                                                 BEGIN:  ovr118

// WZD dseg:9448
extern int16_t SBK_Candidate_Page;

// WZD dseg:944A
extern int16_t SBK_SomePageSaveVar;

// WZD dseg:944C
extern int16_t CMB_SpellBookPage;

// WZD dseg:944E 00 00                                           dw 0
// WZD dseg:9450 00 00                                           dw 0

// WZD dseg:9452
extern SAMB_ptr _spellbook_small_text;

// WZD dseg:9454
// _spellbook_small_symbols SPBK_ICONS <0>
extern SAMB_ptr _spellbook_small_symbols[NUM_MAGIC_REALMS];

// WZD dseg:9460 00 00                                           _spellbook_big_seg@ dw 0                   ; DATA XREF: GAME_LearnSpellAnim+16Dw ...
// WZD dseg:9460                                                                                         ; LBXE_LoadReplace into the LBX_Sandbox_Segment
// WZD dseg:9462 00 00                                           dw 0
// WZD dseg:9464 00 00                                           _spellbook_big_right_corner_seg@ dw 0                ; DATA XREF: SBK_PageFlip_Big+79w ...
// WZD dseg:9466 00 00                                           _spellbook_big_left_corner_seg@ dw 0                ; DATA XREF: SBK_PageFlip_Big+62w ...

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
extern struct s_SPELL_BOOK_PAGE * m_spellbook_pages;

// WZD dseg:C8FC
extern int16_t SBK_NewSpellAnim_Stg;

// WZD dseg:C8FE
// drake178: IMG_SBK_BookRealms@ SPBK_ICONS
extern SAMB_ptr spellbook_symbols_bitm[NUM_MAGIC_REALMS];

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
// PRIVATE  extern int16_t * m_spellbook_spell_list;

// WZD dseg:C914
extern int16_t m_spellbook_page_count;

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
extern int16_t spell_list_count;  // DNE in Dasm;  uses GUI_Multipurpose_Int

// WZD dseg:C928
extern char * SBK_Descriptions[];

// WZD dseg:C968 00 00                                           wizlab_wizard_seg@ dw 0               ; DATA XREF: GAME_NewSpellAnim+1Cr ...
// WZD dseg:C96A 00 00                                           wizlab_familiar_seg@ dw 0               ; DATA XREF: GAME_NewSpellAnim+3Ar ...
// WZD dseg:C96C 00 00                                           wizlab_blue_column_seg@ dw 0               ; DATA XREF: GAME_NewSpellAnim+4Er ...
// WZD dseg:C96E 00 00                                           wizlab_podium_seg@ dw 0               ; DATA XREF: GAME_NewSpellAnim+8r ...
// WZD dseg:C970 00                                              db    0
// WZD dseg:C971 00                                              db    0

// WZD dseg:C971                                                 END:  ovr118



/*
    WIZARDS.EXE  ovr117
*/

// WZD o117p01
void Build_Spell_List(int16_t type, int16_t spell_list[]);

// WZD o117p02
void Combat_Spellbook_Add_Spells_From_Magic_Realm(int16_t * spell_list, int16_t magic_realm);

// WZD o117p03
void Spellbook_Group_Counts(void);

// WZD o117p04
void SBK_BuildSpellbook__WIP(int16_t spell_list_type, int16_t page_spell_count);

// WZD o117p05
int16_t UU_IDK_turn_to_cast__STUB(int16_t player_idx);

// WZD o117p06
int16_t Combat_Spellbook_Build__WIP(int16_t caster_idx);

// WZD o117p07
void SmlBook_Compose__WIP(struct s_SPELL_BOOK_PAGE spell_book_page, SAMB_ptr spellbook_bitmap);

// WZD o117p08
void CmbBook_Compose__WIP(struct s_SPELL_BOOK_PAGE spell_book_page, SAMB_ptr spellbook_bitmap, int16_t caster_idx);

// WZD o117p09
void SmlBook_Draw__WIP(int16_t x, int16_t y);

// WZD o117p10
void CmbBook_Draw__WIP(int16_t x, int16_t y, int16_t caster_idx);

// WZD o117p11
void SmlBook_PageTurn__WIP(int turn_type, int combat_flag, int player_idx);

// WZD o117p12
void Spellbook_Add_Page(int16_t Spell_Count, int16_t Grp_Index, char * Title, int16_t Page_Size);

// WZD o117p13
void Spellbook_Add_Group_Pages(int16_t Page_Size);




/*
    WIZARDS.EXE  ovr118
*/

// WZD o118p01
void BigBook__PageTurn__WIP(int16_t direction);

// WZD o118p02
void BigBook_Draw(void);

// WZD o118p03
void Learn_Spell_Animation_Draw(void);

// WZD o118p04
void Fill_Black_Draw(void);

// WZD o118p05
void Learn_Spell_Animation_Load(void);

// WZD o118p06
void Learn_Spell_Animation(int16_t spell_idx, int16_t research_flag);

// WZD o118p07
void SBK_DrawSpellDecode__STUB(void);

// WZD o118p08
void SBK_LoadSpellDescs__WIP(int16_t page);

// WZD o118p09
void Apprentice_Screen__WIP(void);

// WZD o118p10
void BigBook_Compose__WIP(int16_t pict_seg, SAMB_ptr seg, int16_t flag);

// WZD o118p11
void Spell_Research_Select(void);

// WZD o118p12
// GAME_CheckResearch()



/*
    WIZARDS.EXE  ovr128
*/

// WZD o128p01
int16_t Player_Research_Spells(int16_t player_idx);

// WZD o128p02
void Build_Research_List(int16_t player_idx, int16_t research_list[]);

// WZD o128p03
int16_t WIZ_RollSpellReward(int16_t player_idx, int16_t rarity);

// WZD o128p04
// WIZ_GetSpellValue()

// WZD o128p05
// WIZ_SpellTradeList()

// WZD o128p06
// sub_AC19E()

// WZD o128p07
void Change_Relations_For_Enchantments(int16_t player_idx, int16_t spell_idx, int16_t divisor);

// WZD o128p08
void Sort_Research_List(int16_t player_idx, int16_t count);

// WZD o128p09
void WIZ_AddSpellRank__WIP(int16_t player_idx, int16_t magic_realm);

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
//Cast_DeathWish()

// WZD o130p03
// drake178: sub_AF56D()

// WZD o130p04
//Cast_BlackWind()

// WZD o130p05
//IDK_Random_Spell_Cost()

// WZD o130p06
//sAFA06_Anim_EarthLore()

// WZD o130p07
//Cast_SpellBinding()

// WZD o130p08
//Cast_ChangeTerain()

// WZD o130p09
//Cast_RaiseVolcano()

// WZD o130p10
//Cast_Transmute()

// WZD o130p11
//Cast_Corruption()

// WZD o130p12
//Cast_WordOfRecall()

// WZD o130p13
//Cast_EnchantRoad()

// WZD o130p14
//Cast_SpellWard()

// WZD o130p15
void Cast_HolyArms(int16_t player_idx);

// WZD o130p16
//CTY_Consecration()

// WZD o130p17
//sub_B1A01()

// WZD o130p18
void Cast_NaturesAwareness(int16_t player_idx);



#ifdef __cplusplus
}
#endif


#endif  /* SPELLBOOK_H */
