/*
    Combat

    WIZARDS.EXE
        ovr090
        ovr096
        ovr103
        ovr105
        ovr110
        ovr112
        ovr122
        ovr123
        ovr124

*/

#ifndef COMBAT_H
#define COMBAT_H

#include "MoX.H"

#ifdef __cplusplus
extern "C" {
#endif





/*
    ¿ Meaning / Usage ?

*/
enum e_BATTLE_UNIT_STATUS
{
    bus_Active      = 0,  /* ~== Alive */
    bus_Recalled    = 1,  /* ¿ ? */
    bus_Fleeing     = 2,  /* ¿ ? */
    bus_Uninvolved  = 3,  /* ¿ ? */
    bus_Dead        = 4,  /* ¿ ? */
    bus_Drained     = 5,  /* ¿ ? */
    bus_Gone        = 6   /* ¿ ? ¿ ~== _UNITS[].wp == 9 ? */
};


enum e_BATTLE_UNIT_EFFECT
{
    bue_Vertigo           = 0x1,
    bue_Confusion         = 0x2,
    bue_Whirlwind         = 0x4,
    bue_Mind_Storm        = 0x8,
    bue_Shatter           = 0x10,
    bue_Weakness          = 0x20,
    bue_Black_Sleep       = 0x40,
    bue_Warped_Attack     = 0x80,
    bue_Warped_Defense    = 0x100,
    bue_Warped_Resist     = 0x200,
    bue_Mind_Twist        = 0x400,
    bue_Haste             = 0x800,
    bue_Web               = 0x1000,
    bue_Creature_Binding  = 0x2000,
    bue_Possession        = 0x4000,
    bue_NoEffect          = 0x8000
};


enum e_CMB_ScrollMsg_Type
{
    cmbmsg_AttVictory  =  1,  // ; combat victory
    cmbmsg_AttDefeat   =  2,  // ; combat defeat
    cmbmsg_AttFled     =  3,  // ; combat fled
    cmbmsg_Draw        =  4,  // ; combat draw
    cmbmsg_DefFled     =  5,  // ; combat enemy fled
    cmbmsg_DefVictory  =  6,  // ; combat victory
    cmbmsg_DefDefeat   =  7,  // ; combat defeat
    cmbmsg_IDK_08      =  8,  // ; 8-11 - ??? (no extra text)
    cmbmsg_IDK_09      =  9,
    cmbmsg_IDK_10      = 10,
    cmbmsg_IDK_11      = 11,
    cmbmsg_CityLost    = 12   // ; city lost
};


enum e_COMBAT_STRUCTURE
{
    cs_NONE              = 0,
    cs_City              = 1,
    cs_SorceryNode       = 2,
    cs_NatureNode        = 3,
    cs_ChaosNode         = 4,
    cs_UNKOWN            = 5,
    cs_OceanTerrainType  = 6
};


struct s_COMBAT_ENCHANTMENT_STATUS
{
    /* 00 */ int8_t Attkr;
    /* 01 */ int8_t Dfndr;
    /* 02 */
};

// sizeof:  1Eh  30d  (15*2)
struct s_COMBAT_ENCHANTMENTS
{
    /* 00 */  struct s_COMBAT_ENCHANTMENT_STATUS True_Light;
    /* 02 */  struct s_COMBAT_ENCHANTMENT_STATUS Darkness;
    /* 04 */  struct s_COMBAT_ENCHANTMENT_STATUS Warp_Reality;
    /* 06 */  struct s_COMBAT_ENCHANTMENT_STATUS Black_Prayer;
    /* 08 */  struct s_COMBAT_ENCHANTMENT_STATUS Wrack;
    /* 0A */  struct s_COMBAT_ENCHANTMENT_STATUS Metal_Fires;
    /* 0C */  struct s_COMBAT_ENCHANTMENT_STATUS Prayer;
    /* 0E */  struct s_COMBAT_ENCHANTMENT_STATUS High_Prayer;
    /* 10 */  struct s_COMBAT_ENCHANTMENT_STATUS Terror;
    /* 12 */  struct s_COMBAT_ENCHANTMENT_STATUS Call_Lightning;
    /* 14 */  struct s_COMBAT_ENCHANTMENT_STATUS Counter_Magic;
    /* 16 */  struct s_COMBAT_ENCHANTMENT_STATUS Mass_Invisibility;
    /* 18 */  struct s_COMBAT_ENCHANTMENT_STATUS Entangle;
    /* 1A */  struct s_COMBAT_ENCHANTMENT_STATUS Mana_Leak;
    /* 1C */  struct s_COMBAT_ENCHANTMENT_STATUS Blur;
    /* 1E */
};


//  sizeof: 19h  25d
struct s_CTY_ENCH
{
    /* 0x00 */ int8_t Wall_of_Fire;
    /* 0x01 */ int8_t Chaos_Rift;     // ¿ why'd drake178 start the name with an underscore ?
    /* 0x02 */ int8_t Dark_Rituals;
    /* 0x03 */ int8_t Evil_Presence;  // ¿ why'd drake178 start the name with an underscore ?
    /* 0x04 */ int8_t Cursed_Lands;   // ¿ why'd drake178 start the name with an underscore ?
    /* 0x05 */ int8_t Pestilence;     // ¿ why'd drake178 start the name with an underscore ?
    /* 0x06 */ int8_t Cloud_of_Shadow;
    /* 0x07 */ int8_t Famine;
    /* 0x08 */ int8_t Flying_Fortress;
    /* 0x09 */ int8_t Nature_Ward;
    /* 0x0A */ int8_t Sorcery_Ward;
    /* 0x0B */ int8_t Chaos_Ward;
    /* 0x0C */ int8_t Life_Ward;
    /* 0x0D */ int8_t Death_Ward;
    /* 0x0E */ int8_t Natures_Eye;
    /* 0x0F */ int8_t Earth_Gate;
    /* 0x10 */ int8_t Stream_of_Life;
    /* 0x11 */ int8_t Gaias_Blessing;
    /* 0x12 */ int8_t Inspirations;
    /* 0x13 */ int8_t Prosperity;
    /* 0x14 */ int8_t Astral_Gate;
    /* 0x15 */ int8_t Heavenly_Light;
    /* 0x16 */ int8_t Consecration;
    /* 0x17 */ int8_t Wall_of_Darkness;
    /* 0x18 */ int8_t Altar_of_Battle;
    /* 0x19 */
};

// sizeof: 0x1598
struct s_BATTLEFIELD
{
    /* 0x0000 */  int8_t Tile_Terrain[462];
    /* 0x039C */  int8_t Tile_TerrGroup[462];
    /* 0x056A */  int8_t Tile_Road[462];
    /* 0x0738 */  int8_t MoveCost_Ground[462];
    /* 0x0906 */  int8_t MoveCost_Teleport[462];
    /* 0x0AD4 */  int8_t MoveCost_Ground2[462];
    /* 0x0CA2 */  int8_t MoveCost_Sailing[462];
    /* 0x0E70 */  int16_t Tree_Count;
    /* 0x0E72 */  int16_t Tree_DrawXs[100];
    /* 0x0F3A */  int16_t Tree_DrawYs[100];
    /* 0x1002 */  int16_t Tree_Indices[100];
    /* 0x10CA */  int16_t Rock_Count;
    /* 0x10CC */  int16_t Rock_DrawXs[100];
    /* 0x1194 */  int16_t Rock_DrawYs[100];
    /* 0x125C */  int16_t Rock_IMG_Segs[100];
    /* 0x1324 */  int8_t Tile_Mud[462];
    /* 0x14F2 */  int16_t Central_Structure;   // enum Central_Structures
    /* 0x14F4 */  int16_t House_Count;
    /* 0x14F6 */  int16_t House_TileXs[16];    // [4][4]
    /* 0x1516 */  int16_t House_TileYs[16];    // [4][4]
    /* 0x1536 */  int16_t House_IMG_Segs[16];  // [4][4]
    /* 0x1556 */  int16_t Walled_City;
    /* 0x1558 */  int16_t Wall_Sections[16];   // {4,4,4,4}
    /* 0x1578 */  int16_t Wall_of_Fire;
    /* 0x157A */  int16_t Wall_of_Darkness;
    /* 0x157C */  int16_t Plane;
    /* 0x157E */  struct s_CTY_ENCH City_Enchants[26];  // int16_t City_Enchants CTY_ENCH
    /* 0x1597 */  int8_t Nightshades;
    /* 0x1598 */
};





// WZD dseg:C406                                                 ¿ BEGIN:  ?



// WZD dseg:C47C
extern int16_t OVL_Action_Plane;
// WZD dseg:C47E
extern int16_t OVL_Action_YPos;
// WZD dseg:C480
extern int16_t OVL_Action_XPos;



// WZD dseg:C582
extern int16_t CMB_SelectedUnit;
// WZD dseg:C584
extern int16_t _combat_defender_player;
// WZD dseg:C586
extern int16_t _combat_attacker_player;
// WZD dseg:C588
extern int16_t _combat_total_unit_count;



// WZD dseg:C5D4                                                 ¿ END:  ?





// WZD dseg:C972                                                 ¿ BEGIN: ovr123 - Uninitialized Data ?

// WZD dseg:C972
extern int16_t CMB_Winner;
// WZD dseg:C974
extern int16_t GAME_RazeCity;
// WZD dseg:C976
extern int16_t CMB_Gold_Reward;
// WZD dseg:C978
extern int16_t CMB_Buildings_Lost;
// WZD dseg:C97A
extern int16_t * CMB_LostBuildings;
// WZD dseg:C97C
extern int16_t CMB_Population_Lost;
// WZD dseg:C97E
extern int16_t CMB_ScrollMsg_Type;

// WZD dseg:C97E                                                 ¿ END: ovr123 - Uninitialized Data ?    






// WZD dseg:D13E
extern SAMB_ptr CMB_Path_Ys;
// WZD dseg:D140
extern SAMB_ptr CMB_Path_Xs;
// WZD dseg:D142 00 00                                           CMB_Path_Length dw 0                    ; DATA XREF: BU_Move:loc_77989w ...
// WZD dseg:D144
extern SAMB_ptr CMB_NearBuffer_3;
// WZD dseg:D146
extern SAMB_ptr CMB_Path_Costs;
// WZD dseg:D148
extern SAMB_ptr CMB_ActiveMoveMap;
// WZD dseg:D14A 00 00                                           CMB_Chasm_Anim_Y dw 0                   ; DATA XREF: TILE_CracksCall+1Ew ...
// WZD dseg:D14C 00 00                                           CMB_Chasm_Anim_X dw 0                   ; DATA XREF: TILE_CracksCall+18w ...
// WZD dseg:D14E
extern SAMB_ptr CMB_Vortex_Array;
// WZD dseg:D152 00 00                                           CMB_Vortex_Count dw 0                   ; DATA XREF: BU_Move:loc_77934r ...
// WZD dseg:D154
extern SAMB_ptr CMB_Projectiles;
// WZD dseg:D158 00 00                                           CMB_ProjectileCount dw 0                ; DATA XREF: CMB_RangedAnim+464r ...
// WZD dseg:D15A
extern SAMB_ptr CMB_EntitySprites;
// WZD dseg:D15E 00 00                                           CMB_SpriteCount dw 0                    ; DATA XREF: CMB_DrawEntities:loc_DD3A3r ...





/*
    WIZARDS.EXE  ovr090
*/

// WZD s90p01
// drake178: CMB_TacticalCombat()
int16_t Tactical_Combat__WIP(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[]);



/*
    WIZARDS.EXE  ovr096
*/

// WZD s96p01
int16_t Combat__WIP(int16_t attacker_player_idx, int16_t defender_player_idx, int16_t troop_count, int16_t troops[]);

// WZD s96p02
// drake178: EZ_CreateGuardStack()
void Lair_Make_Guardians(int16_t lair_idx);

// WZD s96p03
// drake178: EZ_ResolveEntry()
int16_t Lair_Combat_Do(int16_t lair_idx, int16_t player_idx);

// WZD s96p04
// drake178: sub_7DE08()
// UU_IDK_CMB_s7DE08()

// WZD s96p05
// drake178: WIZ_SetHostile()
void Update_Defender_Hostility(int attacker_player_idx, int defender_player_idx);

// WZD s96p06
// UU_IDK_Main_Screen_Draw()



/*
    WIZARDS.EXE  ovr103
*/

// WZD s103p01
// CMB_ShowInfoWindow()

// WZD s103p02
// CMB_DrawInfoWindow()

// WZD s103p03
// CMB_FillInfoArrays()

// WZD s103p04
// CMB_SetCommonXFX()

// WZD s103p05
// CMB_CountExternalFX()

// WZD s103p06
void CMB_Set_Node_Type(void);

// WZD s103p07
// BU_CreateImage()

// WZD s103p08
// WIZ_BU_SelectNext()

// WZD s103p09
// WIZ_BU_SelectClosest()

// WZD s103p10
// CMB_SpellcastError()

// WZD s103p11
// BU_MoveConfused()

// WZD s103p12
// CMB_LoadResources()

// WZD s103p13
// CMB_SetNearAllocs()

// WZD s103p14
// CMB_SelectCaster()



/*
    WIZARDS.EXE  ovr105
*/

// WZD o105p01
// CMB_WinLoseFlee()

// WZD o105p02
// LBX_IMG_BannerPaint()

// WZD o105p03
// LBX_IMG_Highlight()

// WZD o105p04
// LBX_IMG_FX()

// WZD o105p05
// BU_SetUEColor()

// WZD o105p06
// LBX_IMG_UE_Outline()

// WZD o105p07
void CMB_UpdateTrueSight(void);

// WZD o105p08
// BU_SetVisibility()

// WZD o105p09
// BU_GetCombatEffect()

// WZD o105p10
// BU_SetBaseAnims()

// WZD o105p11
// BU_GetWpnIconIndex()

// WZD o105p12
// BU_GetRngIconIndex()

// WZD o105p13
// CMB_DrawUnitDisplay()



/*
    WIZARDS.EXE  ovr110
*/

// WZD o110p01
void Strategic_Combat_Allocate(void);

// WZD o110p02
int16_t Strategic_Combat__WIP(int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[]);

// WZD o110p03
// drake178: BU_Heal()

// WZD o110p04
int16_t Get_Effective_Hits(int16_t hits, int16_t defense);

// WZD o110p05
int16_t Get_Effective_Ranged_Strength();

// WZD o110p06
int16_t Get_Effective_Melee_Strength(int16_t melee, int16_t thrown, int16_t figures, int16_t attack_flags, int16_t ranged_type);



/*
    WIZARDS.EXE  ovr112
*/

// WZD o112p01
// CMB_ComposeBookBG()

// WZD o112p02
// CMB_RedrawSpellbook()

// WZD o112p03
// G_CMB_CastSpell()

// WZD o112p04
// CMB_ShowSpellbook()

// WZD o112p05
// CMB_CheckSpellErrors()

// WZD o112p06
// SPL_GetResistMod()

// WZD o112p07
int16_t Combat_Casting_Cost_Multiplier(int16_t player_idx);

// WZD o112p08
// AITP_EarthToMud()

// WZD o112p09
// AITP_Disrupt()

// WZD o112p10
// AITP_CracksCall()

// WZD o112p11
// UU_AITP_WordofRecall()

// WZD o112p12
// AITP_RecallHero()



/*
    WIZARDS.EXE  ovr113
*/

// WZD o113p01
// CMB_SetTargetCursor()

// WZD o113p02
// CMB_DrawTargetScreen()

// WZD o113p03
// CMB_TargetSpell()

// WZD o113p04
// CMB_RangedAnim()

// WZD o113p05
// CMB_MeleeAnim()

// WZD o113p06
// CMB_ConvSpellAttack()

// WZD o113p07
void BU_ApplyDamage(int16_t battle_unit_idx, int16_t Dmg_Array[]);

// WZD o113p08
// BU_RangedValidate()

// WZD o113p09
// CMB_PlaceArmy()

// WZD o113p10
// BU_SummonDemon()

// WZD o113p11
// BU_MeleeWallCheck()

// WZD o113p12
// CMB_ManaLeak()

// WZD o113p13
// GUI_DrawNearMessage()

// WZD o113p14
// CMB_SpellcastMessage()

// WZD o113p15
// CMB_FindEmptyFigSlot()

// WZD o113p16
int16_t Check_For_Winner(void);



/*
    WIZARDS.EXE  ovr122
*/

// WZD o122p01
// CMB_AttackRoll()

// WZD o122p02
// CMB_DefenseRoll()

// WZD o122p03
int16_t BU_ResistRoll__STUB(struct s_BATTLE_UNIT battle_unit, int16_t Save_Mod, int16_t magic_realm);

// WZD o122p04
// BU_GetEffectiveRES()

// WZD o122p05
// BU_GetATKImmFlags()

// WZD o122p06
// BU_AttackTarget()

// WZD o122p07
// BU_ProcessAttack()

// WZD o122p08
// BU_GetEffectiveDEF()

// WZD o122p09
// BU_GetATKRealm()

// WZD o122p10
void Init_Battlefield_Effects(int16_t combat_structure);

// WZD o122p11
// BU_Attack()

// WZD o122p12
// BU_GetDistanceFrom()

// WZD o122p13
// AI_BU_GetAttackValue()

// WZD o122p14
int16_t Battle_Unit_Has_Ranged_Attack(int16_t battle_unit_idx);

// WZD o122p15
// BU_CauseFear()



/*
    WIZARDS.EXE  ovr123
*/

// WZD o123p01
// drake178: CMB_Victory()
void End_Of_Combat__WIP(int16_t player_idx, int16_t * item_count, int16_t item_list[], int16_t MsgType);

// WZD o123p02
// CMB_GetUndeadCreator()

// WZD o123p03
void Combat_Results_Scroll(void);

// WZD o123p04
// ¿ MoO2  Module: COMBFIND  Strategic_Results_Popup_() ?
void Combat_Results_Scroll_Draw(void);

// WZD o123p05
int16_t Combat_Results_Scroll_Text(void);

// WZD o123p06
int16_t CTY_GetConquerGold(int16_t city_idx);

// WZD o123p07
int16_t CTY_RampageVictory(void);



/*
    WIZARDS.EXE  ovr124
*/

// WZD o124p01
// WIZ_GetLastRangedStr()

// WZD o124p02
// BU_SetCityMovement()

// WZD o124p03
// AI_RestrictToCity()

// WZD o124p04
int16_t Combat_Structure(int16_t wx, int16_t wy, int16_t wp, int16_t set_city_flag);

// WZD o124p05
int16_t CMB_MarkLandLubbers(int16_t player_idx);

// WZD o124p06
// AI_CompareArmies()

// WZD o124p07
// BU_LoadMoveSound()

// WZD o124p08
// BU_LoadMeleeSound()

// WZD o124p09
// GAME_LoadSpellSound()

// WZD o124p10
// BU_WallofFire()

// WZD o124p11
// BU_SetBloodAnim()

// WZD o124p12
// BU_ClearBlood()

// WZD o124p13
// BU_IsInCityProper()

// WZD o124p14
// TILE_HasIntactWall()

// WZD o124p15
int16_t BU_IsCombatSummon(int16_t battle_unit_idx);

// WZD o124p16
// BU_GetHalfMoves()

// WZD o124p17
void STK_CaptureCity__WIP(int16_t troop_count, int16_t troops[]);

// WZD o124p18
// AI_FightorFlight()

// WZD o124p19
// AI_Raze_Decision()

// WZD o124p20
int16_t Raze_City_Prompt(char * message);

// WZD o124p21
void Raze_City_Prompt_Draw(void);



#ifdef __cplusplus
}
#endif

#endif  /* COMBAT_H */
