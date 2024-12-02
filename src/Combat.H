/*
    Combat

    WIZARDS.EXE
        ovr090
        ovr091
        ovr096
        ovr098
        ovr099  ¿ MoO2  Module: CMBTDRW1 ?
        ovr103
        ovr105
        ovr110
        ovr112
        ovr114
        ovr116
        ovr122
        ovr123
        ovr124
        ovr153
        ovr154
        ovr155
        ovr163

*/

#ifndef COMBAT_H
#define COMBAT_H

#include "MoM.H"

#ifdef __cplusplus
extern "C" {
#endif



#define TRUE_LIGHT_ATTKR 0
#define TRUE_LIGHT_DFNDR 1
#define DARKNESS_ATTKR 2
#define DARKNESS_DFNDR 3
#define WARP_REALITY_ATTKR 4
#define WARP_REALITY_DFNDR 5
#define BLACK_PRAYER_ATTKR 6
#define BLACK_PRAYER_DFNDR 7
#define WRACK_ATTKR 8
#define WRACK_DFNDR 9
#define METAL_FIRES_ATTKR 10
#define METAL_FIRES_DFNDR 11
#define PRAYER_ATTKR 12
#define PRAYER_DFNDR 13
#define HIGH_PRAYER_ATTKR 14
#define HIGH_PRAYER_DFNDR 15
#define TERROR_ATTKR 16
#define TERROR_DFNDR 17
#define CALL_LIGHTNING_ATTKR 18
#define CALL_LIGHTNING_DFNDR 19
#define COUNTER_MAGIC_ATTKR 20
#define COUNTER_MAGIC_DFNDR 21
#define MASS_INVISIBILITY_ATTKR 22
#define MASS_INVISIBILITY_DFNDR 23
#define ENTANGLE_ATTKR 24
#define ENTANGLE_DFNDR 25
#define MANA_LEAK_ATTKR 26
#define MANA_LEAK_DFNDR 27
#define BLUR_ATTKR 28
#define BLUR_DFNDR 29



//  ; (sizeof=0x4, standard type)
struct s_CE_Display
{
    /* 00 */  int16_t icon_idx;
    /* 02 */  int16_t help_idx;
    /* 04 */
};

//  ; (sizeof=0x4, standard type)
struct s_Active_CE
{
    /* 00 */ int16_t icon_idx;
    /* 02 */ int16_t help_idx;
    /* 04 */
};


extern SAMB_ptr DBG_EmmHndl_FIGUREX;
extern SAMB_ptr DBG_figure_pict_base_seg;





#define COMBAT_GRID_X1(_x_,_y_)     ( CMB_GetTileX( ( (_x_) + 4), ((_y_) + 4) ) )
#define COMBAT_GRID_Y1(_x_,_y_)     ( CMB_GetTileY( ( (_x_) + 4), ((_y_) + 4) ) )

#define CALC_BASE_CGC2  IDK_base_cgc2 = (itr_y / 2)
#define CALC_BASE_CGC1  IDK_base_cgc1 = (((itr_y + 1) / 2) + 9)

#define CALC_CGC2       cgc2 = (IDK_base_cgc2 + itr_x)
#define CALC_CGC1       cgc1 = (IDK_base_cgc1 - itr_x)

#define CALC_SCREEN_X   screen_x = ((itr_x * 32) - ((itr_y & 0x1) * 16))
#define CALC_SCREEN_Y   screen_y = ((itr_y * 8) - 8)


/*

Combat Battle Unit Figure Picture Cache

EMM_FIGUREX_Init__HACK()
USELESS_Combat_Figure_Load_Compose()
Combat_Figure_Compose_USEFULL()
Combat_Figure_Load()


EMM_FIGUREX_Init__HACK()
    logical_page = ((battle_unit_figure_idx * 3) / 2);
    if((battle_unit_figure_idx & 0x1) == 0)
    {
        offset = 0;
    }
    else
    {
        offset = (512 * SZ_PARAGRAPH_B);  // 512 PR, 8192 B; 512 / 56 = 9.14 ... ¿ 2 sets of 9 figure pictures ?
    }
    offset += (battle_unit_figure_idx * (56 * SZ_PARAGRAPH_B));  // 28 * 30 = 840 / 16 = 52.5 PR per figure picture
    EMM_PageFrame = (EmmHndl_FIGUREX + (logical_page * SZ_EMM_LOGICAL_PAGE));

USELESS_Combat_Figure_Load_Compose()
    if((battle_unit_figure_idx & 0x1) == 0)
    {
        offset = 0;
    }
    else
    {
        offset = (512 * SZ_PARAGRAPH_B);  // 512 PR, 8192 B; 512 / 56 = 9.14 ... ¿ 2 sets of 9 figure pictures ?
    }
    offset += (battle_unit_figure_idx * (56 * SZ_PARAGRAPH_B));
    EMM_FIGUREX_Init__HACK(battle_unit_figure_idx);
    figure_pointer_seg = Allocate_First_Block((EMM_PageFrame + offset), 33);
    ptr_figure_pointer_seg = (SAMB_ptr *)figure_pointer_seg;

Combat_Figure_Compose_USEFULL()
        EMM_FIGUREX_Init__HACK(battle_unit_figure_idx);
        if((battle_unit_figure_idx & 1) == 0)
        {
            offset = 0;
        }
        else
        {
            offset = (512 * SZ_PARAGRAPH_B);
        }
        offset += (battle_unit_figure_idx * (56 * SZ_PARAGRAPH_B));
        figure_pointer_seg = Allocate_First_Block((EMM_PageFrame + offset), 33);
        ptr_figure_pointer_seg = (SAMB_ptr *)figure_pointer_seg;

Combat_Figure_Load()
    EMM_FIGUREX_Init__HACK(battle_unit_figure_idx);
    if((battle_unit_figure_idx & 0x1) == 0)
    {
        offset = 0;
    }
    else
    {
        offset = (512 * SZ_PARAGRAPH_B);  // 512 PR, 8192 B; 512 / 56 = 9.14 ... ¿ 2 sets of 9 figure pictures ?
    }
    offset += (battle_unit_figure_idx * (56 * SZ_PARAGRAPH_B));
    figure_pointer_seg = Allocate_First_Block((EMM_PageFrame + offset), 33);  // 16-byte header + 8 8-byte pointers = (16 + (8 * 8)) = (16 + (64)) / 16 = 5
    ptr_figure_pointer_seg = (SAMB_ptr *)figure_pointer_seg;

Pieces & Parts

    1)  EMM_FIGUREX_Init__HACK(battle_unit_figure_idx);

    2)  if((battle_unit_figure_idx & 0x1) == 0)
        {
            offset = 0;
        }
        else
        {
            offset = (512 * SZ_PARAGRAPH_B);  // 512 PR, 8192 B; 512 / 56 = 9.14 ... ¿ 2 sets of 9 figure pictures ?
        }
        offset += (battle_unit_figure_idx * (56 * SZ_PARAGRAPH_B));

    3)  figure_pointer_seg = Allocate_First_Block((EMM_PageFrame + offset), 33);
        ptr_figure_pointer_seg = (SAMB_ptr *)figure_pointer_seg;

EMM_FIGUREX_Init__HACK()
    2
USELESS_Combat_Figure_Load_Compose()
    2, 1, 3
Combat_Figure_Compose_USEFULL()
    1, 2, 3
Combat_Figure_Load()
    1, 2, 3

*/
// 28 * 30 = 840 * 9 = 7560 / 16 = 472.5
// offsets 56 paragraphs for each figure picture; 56 * = 896 - 840 = 56 / 3.5
// 
#define FIGUREX_MAP         EMM_FIGUREX_Init__HACK(battle_unit_figure_idx);
#define FIGUREX_OFFSET      if((battle_unit_figure_idx & 0x1) == 0) { offset = 0; } else { offset = (512 * SZ_PARAGRAPH_B); } offset += (battle_unit_figure_idx * (56 * SZ_PARAGRAPH_B));
#define FIGUREX_POINTER     figure_pointer_seg = Allocate_First_Block((EMM_PageFrame + offset), 33); ptr_figure_pointer_seg = (SAMB_ptr *)figure_pointer_seg;
/*
    Battle Unit Macros

        Unit Special Abilities
        Unit Mutations
        Unit Enchantments
*/

#define BU_LUCKY()      ( ( battle_unit->Attribs_1 & USA_LUCKY     ) != 0 )
#define BU_CASTER_20()  ( ( battle_unit->Attribs_1 & USA_CASTER_20 ) != 0 )
#define BU_CASTER_40()  ( ( battle_unit->Attribs_1 & USA_CASTER_40 ) != 0 )



enum e_COMBAT_LOCATION_TYPE
{
    clt_OpenField       =  0, 
    clt_Outpost         =  1, 
    clt_City            =  2, 
    clt_Fortress        =  3, 
    clt_Tower           =  4, 
    clt_ChaosNode       =  5, 
    clt_NatureNode      =  6, 
    clt_SorceryNode     =  7, 
    clt_Cave            =  8, 
    clt_Dungeon         =  9, 
    clt_AncientTemple   = 10, 
    clt_Keep            = 11, 
    clt_MonsterLair     = 12, 
    clt_Ruins           = 13, 
    clt_FallenTemple    = 14
};


enum Central_Structures
{
    CS_None         =  0, 
    CS_Outpost      =  1, 
    CS_City         =  2, 
    CS_Fortress     =  3, 
    CS_Dungeon      =  4, 
    CS_Tower        =  5, 
    CS_Cave         =  6, 
    CS_Temple       =  7, 
    CS_Fort         =  8, 
    CS_SorceryNode  =  9, 
    CS_ChaosNode    = 10, 
    CS_NatureNode   = 11, 
    CS_Ruins        = 12
};


enum e_COMBAT_TERRAIN_TYPE
{
    ctt_Water      = 0, 
    ctt_Plains     = 1, 
    ctt_Hills      = 2, 
    ctt_Desert     = 3, 
    ctt_Mountains  = 4, 
    ctt_Tundra     = 5, 
    ctt_Forest     = 6
};


enum Combat_TileGroup
{
    CTG_Grass       = 0, 
    CTG_Rough       = 1, 
    CTG_Dirt        = 2, 
    CTG_River       = 3, 
    CTG_DeepWater   = 4, 
    CTG_IDK         = 5
};


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

enum BU_Action
{
    bua_Ready           =   0,
    bua_Finished        =   4,
    bua_Wait            =   5,

    BUA_MeleeAttack     = 100,
    BUA_RangedAttack    = 101,
    BUA_MoveNFire       = 102,
    BUA_MoveNAttack     = 103,

    BUA_DoomBolt        = 104,
    BUA_Fireball        = 105,
    BUA_Healing         = 106,
    BUA_UseItem         = 107,
    BUA_CastSpell       = 108,
    BUA_SummonDemon     = 109,
    BUA_WebSpell        = 110,

    BUA_Flee            = 150,

    BUA_No_Spells       = 333
};


/*

CMB_PrepareTurn__WIP() uses `test .. jz`

*/
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

/*
    MoO2 "board"
*/
// sizeof: 0x1598
struct s_BATTLEFIELD
{
    /* 0x0000 */  int8_t Tile_Terrain[462];  // 21 x 22
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
    /* 0x1536 */  SAMB_ptr House_IMG_Segs[16];  // [4][4]  ¿ assigned pict seg from [3][15] house types ?
    /* 0x1556 */  int16_t Walled_City;
    /* 0x1558 */  int16_t Wall_Sections[16];   // {4,4,4,4}
    /* 0x1578 */  int16_t Wall_of_Fire;
    /* 0x157A */  int16_t Wall_of_Darkness;
    /* 0x157C */  int16_t wp;
    /* 0x157E */  // struct s_CTY_ENCH City_Enchants[26];  // int16_t City_Enchants CTY_ENCH
                  int8_t City_Enchants[26];
    /* 0x1597 */  int8_t Nightshades;
    /* 0x1598 */
};


// ; (sizeof=0x1E)
// sizeof 40, due to union / pointer size ... in-mem, union looks to be 12 bytes
struct s_COMBAT_ENTITY
{
    /* 0x00 */ int16_t draw_x;  /* only used by CMB_DrawEntities__WIP();  e.g., battle unit figure screen x;  screen x for Draw_Picture_Windowed() */
    /* 0x02 */ int16_t draw_y;  /* only used by CMB_DrawEntities__WIP();  e.g., battle unit figure screen y;  screen y for Draw_Picture_Windowed() */
    /* 0x04 */ // int16_t IMG_Seg_or_Id;
    union {
        int64_t seg_or_idx;
        int16_t index;
        SAMB_ptr pict_seg;
    };
    /* 0x06 */ int16_t Draw_Frame;
    /* 0x08 */ int16_t draw_y_shift;  /* only used by CMB_DrawEntities__WIP(), only subtracted, never added;  curiously hard-coded value, per entity type;  screen y offset for Draw_Picture_Windowed() */
    /* 0x0A */ int16_t draw_x_shift;  /* only used by CMB_DrawEntities__WIP(), only subtracted, never added;  curiously hard-coded value, per entity type;  screen x offset for Draw_Picture_Windowed() */
    /* 0x0C */ int16_t entity_type;  /* ¿ entity type ?  1 is battle unit figure */
    /* 0x0E */ int16_t Owner;  /* battle_unit[].controller_idx */
    /* 0x10 */ int16_t UU_Facing;
    /* 0x12 */ int16_t outline_magic_realm;  /* unit enchantment outline magic realm */
    /* 0x14 */ int16_t Unused_14h;
    /* 0x16 */ int16_t Blood_Frame;
    /* 0x18 */ int16_t Blood_Amt;
    /* 0x1A */ uint32_t draw_order_value;
    /* 0x1E */
};


// ; (sizeof=0x10, standard type)
struct RNGD_ANIM
{
    /* 00 */ int16_t Up;
    /* 02 */ int16_t UpRight;
    /* 04 */ int16_t Right;
    /* 06 */ int16_t DnRight;
    /* 08 */ int16_t Down;
    /* 0A */ int16_t DnLeft;
    /* 0C */ int16_t Left;
    /* 0E */ int16_t UpLeft;
    /* 10 */
};


// ; (sizeof=0x12, standard type)
struct ATK_ICONS
{
    /* 00 */ int16_t Melee;
    /* 02 */ int16_t Ranged_Magic;
    /* 04 */ int16_t Mithril;
    /* 06 */ int16_t Magic_Weapon;
    /* 08 */ int16_t Adamantium;
    /* 0A */ int16_t Ranged_Bow;
    /* 0C */ int16_t Ranged_Rock;
    /* 0E */ int16_t Fire_Breath;
    /* 10 */ int16_t Thrown_Attack;
    /* 12 */
};


// ; (sizeof=0x6, standard type)
struct CMOVE_ICONS
{
    /* 00 */ int16_t Ground_Movement;
    /* 02 */ int16_t Flying_Movement;
    /* 04 */ int16_t Water_Movement;
    /* 06 */
};


// ; (sizeof=0x1E, standard type)
struct STAT_ICONS
{
    /* 00 */ int16_t Melee;
    /* 02 */ int16_t Ranged_Magic;
    /* 04 */ int16_t Mithril;
    /* 06 */ int16_t Magic_Weapon;
    /* 08 */ int16_t Adamantium;
    /* 0A */ int16_t Ranged_Bow;
    /* 0C */ int16_t Ranged_Rock;
    /* 0E */ int16_t Fire_Breath;
    /* 10 */ int16_t Thrown_Attack;
    /* 12 */ int16_t Defense;
    /* 14 */ int16_t Hits;
    /* 16 */ int16_t Ground_Movement;
    /* 18 */ int16_t Flying_Movement;
    /* 1A */ int16_t Water_Movement;
    /* 1C */ int16_t Resist;
    /* 1E */
};


// ; (sizeof=0x24, standard type)
struct CSCAN_ICONS
{
    /* 00 */ struct STAT_ICONS Stat;
    /* 1E */ int16_t White_XP;
    /* 20 */ int16_t Gold_XP;
    /* 22 */ int16_t Red_XP;
    /* 24 */
};


// sizeof=0x1E
struct s_CMB_InfoItem
{
    /* 00 */ SAMB_ptr Icon_IMG_Seg;
    /* 02 */ int16_t HLP_Index;  // enum HELP_TXT
    /* 04 */ char Name[26];
    /* 1E */ 
};



// WZD dseg:C47C
extern int16_t _combat_wp;

// WZD dseg:C47E
extern int16_t _combat_wy;

// WZD dseg:C480
extern int16_t _combat_wx;

// WZD dseg:C582
extern int16_t _active_battle_unit;

// WZD dseg:C584
extern int16_t _combat_defender_player;

// WZD dseg:C586
extern int16_t _combat_attacker_player;

// WZD dseg:C588
extern int16_t _combat_total_unit_count;

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

// WZD dseg:CF9A
extern SAMB_ptr EmmHndl_TILEXXX;

// WZD dseg:CF9C
extern SAMB_ptr EmmHndl_FIGUREX;

// WZD dseg:CFBE
extern SAMB_ptr IMG_CMB_Blood[5];

// WZD dseg:CFCA
extern SAMB_ptr IMG_CMB_Curses[8];

// WZD dseg:CFEE
extern SAMB_ptr IMG_CMB_RiverTile[12];

// WZD dseg:D006
extern SAMB_ptr IMG_CMB_RivrNULLs[12];

// WZD dseg:D01E
extern SAMB_ptr IMG_CMB_RoadTiles[28];

// WZD dseg:D056
extern SAMB_ptr IMG_CMB_OceanTile[4];

// WZD dseg:D05E
extern SAMB_ptr IMG_CMB_ChaosOcn[4];

// WZD dseg:D066
extern SAMB_ptr IMG_CMB_Cloud;

// WZD dseg:D06E
extern SAMB_ptr IMG_CMB_FlotIsle;

// WZD dseg:D070
extern SAMB_ptr IMG_CMB_Mud;

// WZD dseg:D072
extern SAMB_ptr IMG_CMB_SorcNode;

// WZD dseg:D074
extern SAMB_ptr IMG_CMB_NatNode;

// WZD dseg:D076
extern SAMB_ptr IMG_CMB_Volcano[8];

// WZD dseg:D088
extern SAMB_ptr IMG_CMB_DarkWall[14];

// WZD dseg:D0A4
extern SAMB_ptr IMG_CMB_FireWall[14];

// WZD dseg:D0C0
extern SAMB_ptr IMG_CMB_StoneWalls[3][12];

// WZD dseg:D108
extern SAMB_ptr IMG_CMB_DirtTile;
// WZD dseg:D10A
extern SAMB_ptr IMG_CMB_RoadGrid;
// WZD dseg:D10C
extern SAMB_ptr IMG_CMB_Fort;
// WZD dseg:D10E
extern SAMB_ptr IMG_CMB_Ruins;
// WZD dseg:D110
extern SAMB_ptr IMG_CMB_Temple;
// WZD dseg:D112
extern SAMB_ptr IMG_CMB_Dungeon;
// WZD dseg:D114
extern SAMB_ptr IMG_CMB_Tower;
// WZD dseg:D116
extern SAMB_ptr IMG_CMB_Cave;
// WZD dseg:D118
extern SAMB_ptr IMG_CMB_Outpost;
// WZD dseg:D11A
extern SAMB_ptr IMG_CMB_Fortress;

// WZD dseg:D11C
extern SAMB_ptr IMG_CMB_Houses[15];

// WZD dseg:D13A
extern SAMB_ptr IMG_GUI_Chasm;

// WZD dseg:D13C
extern SAMB_ptr IMG_GUI_Vortex;

// WZD dseg:D13E
extern SAMB_ptr CMB_Path_Ys;

// WZD dseg:D140
extern SAMB_ptr CMB_Path_Xs;

// WZD dseg:D144
extern SAMB_ptr CMB_NearBuffer_3;

// WZD dseg:D146
extern SAMB_ptr CMB_Path_Costs;

// WZD dseg:D148
extern SAMB_ptr CMB_ActiveMoveMap;

// WZD dseg:D14E
extern SAMB_ptr CMB_Vortex_Array;

// WZD dseg:D154
extern SAMB_ptr CMB_Projectiles;

// WZD dseg:D15A
extern struct s_COMBAT_ENTITY * combat_grid_entities;

// WZD dseg:D188
extern SAMB_ptr CMB_ActiveFrame_GFX;

// WZD dseg:D18A
extern SAMB_ptr CMB_TargetFrame_GFX;

// WZD dseg:D18C
// extern RNGD_ANIM * CMB_RangedAtx_GFX[15];
extern SAMB_ptr CMB_RangedAtx_GFX[15][8];

// WZD dseg:D27C
extern SAMB_ptr CMB_Damage_GFX;



/*
    WIZARDS.EXE  ovr090
*/

// WZD s90p01
// drake178: CMB_TacticalCombat()
int16_t Tactical_Combat__WIP(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[]);



/*
    WIZARDS.EXE  ovr091
*/

// WZD s91p01
// drake178: UU_BU_LoadFigureGFX()

// WZD s91p02
void CMB_PrepareTurn__WIP(void);

// WZD s91p03
// drake178: BU_GetMoveMap()

// WZD s91p04
// drake178: BU_Move()

// WZD s91p05
void Switch_Active_Battle_Unit(int16_t battle_unit_idx);

// WZD s91p06
void BU_CombatAction__WIP(int16_t battle_unit_idx, int16_t x, int16_t y);

// WZD s91p07
void CMB_FillTargetMaps__WIP(void);

// WZD s91p08
// drake178: BU_IsVisible()

// WZD s91p09
// drake178: CMB_UnitCityDamage()

// WZD s91p10
// drake178: BU_GetInstaMoveType()



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
    WIZARDS.EXE  ovr098
*/

// WZD o98p01
// drake178: CMB_CE_Refresh()
void CMB_CE_Refresh__WIP(void);

// WZD o98p02
int16_t BU_HasSpellAbility__WIP(int16_t battle_unit_idx);

// WZD o98p03
// drake178: sub_7EC81()
// sub_7EC81()

// WZD o98p04
void AI_CMB_PlayTurn__WIP(int16_t player_idx);

// WZD o98p05
void CMB_ProgressTurnFlow__WIP(void);

// WZD o98p06
// drake178: WIZ_FleeCombat()
void Retreat_From_Combat(int16_t player_idx);

// WZD o98p07
// drake178: TILE_HasCityOfPlayer()
int16_t Player_City_At_Square(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx);

// WZD o98p08
int16_t Process_Retreating_Units(int16_t wx, int16_t wy, int16_t wp, int16_t player_idx);

// WZD o98p09
int16_t Unit_Try_To_Move(int16_t wx, int16_t wy, int16_t wp, int16_t unit_idx, int16_t troop_count);

// WZD o98p10
void Assign_Mouse_Image(void);

// WZD o98p11
int16_t BU_GetRangedCursor__WIP(int16_t src_battle_unit_idx, int16_t dst_battle_unit_idx);

// WZD o98p12
void CMB_CreateCELabels__WIP(void);

// WZD o98p13
int16_t Battle_Unit_Movement_Icon(int16_t battle_unit_idx);

// WZD o98p14
// drake178: UNIT_SummonToBattle()
// UNIT_SummonToBattle()

// WZD o98p15
// drake178: BU_UnitLoadToBattle()
// BU_UnitLoadToBattle()

// WZD o98p16
int16_t CMB_Units_Init__WIP(int16_t troop_count, int16_t troops[]);

// WZD o98p17
// drake178: BU_CheckFlight()
// BU_CheckFlight()

// WZD o98p18
// drake178: BU_IsFlying()
// BU_IsFlying()

// WZD o98p19
int16_t BU_MeleeFlightCheck__WIP(int16_t src_battle_unit_idx, int16_t dst_battle_unit_idx);

// WZD o98p20
// drake178: BU_PrepMoveSound()
// BU_PrepMoveSound()

// WZD o98p21
void Turn_Off_Auto_Combat(void);

// WZD o98p22
void STK_ComposeFleeLost__STUB(int16_t troop_count, int16_t troop_list[]);



/*
    WIZARDS.EXE  ovr099
*/

// WZD o99p01
// WZD o99p02
// WZD o99p03
// WZD o99p04
// WZD o99p05
// WZD o99p06
// WZD o99p07

// WZD o99p01
void CMB_DrawFullScreen__WIP(void);

// WZD o99p02
void Draw_Active_Unit_Window(void);

// WZD o99p03
void Tactical_Combat_Draw_Buttons(void);

// WZD o99p04
void Draw_Spell_Information_Window(void);

// WZD o99p05
void Draw_Active_Unit_Stats_And_Icons(void);

// WZD o99p06
void CMB_CreateEntities__WIP(void);

// WZD o99p07
void Draw_Active_Unit_Damage_Bar(int16_t battle_unit_idx, int16_t x, int16_t y);



/*
    WIZARDS.EXE  ovr103
*/

// WZD s103p01
void Combat_Information_Window(void);

// WZD s103p02
void Combat_Information_Window_Draw(void);

// WZD s103p03
void Combat_Info_Effects(void);

// WZD s103p04
void Combat_Info_Effects_Base(void);

// WZD s103p05
int16_t Combat_Info_Effects_Count(void);

// WZD s103p06
void Combat_Node_Type(void);

// WZD s103p07
// BU_CreateImage()

// WZD s103p08
void Next_Battle_Unit(int16_t player_idx);

// WZD s103p09
int16_t Next_Battle_Unit_Nearest_Available(int16_t player_idx);

// WZD s103p10
void CMB_SpellcastError__WIP(int16_t type);

// WZD s103p11
void BU_MoveConfused__WIP(int16_t battle_unit_idx);

// WZD s103p12
void CMB_LoadResources__WIP(void);

// WZD s103p13
void CMB_SetNearAllocs__WIP(void);

// WZD s103p14
void CMB_SelectCaster__WIP(int16_t caster_id);



/*
    WIZARDS.EXE  ovr105
*/

// WZD o105p01
int16_t CMB_WinLoseFlee__WIP(void);

// WZD o105p02
void Combat_Figure_Banner_Color(int16_t player_idx);

// WZD o105p03
void Combat_Figure_Active_Red_Outline(int16_t battle_unit_idx);

// WZD o105p04
void Combat_Figure_Effect__WIP(int16_t effect);

// WZD o105p05
void Combat_Unit_Enchantment_Outline_Set(int16_t battle_unit_idx);

// WZD o105p06
void Combat_Unit_Enchantment_Outline_Draw(int16_t enchantment_magic_realm);

// WZD o105p07
void CMB_UpdateTrueSight(void);

// WZD o105p08
 void BU_SetVisibility__WIP(int16_t battle_unit_idx);

// WZD o105p09
// BU_GetCombatEffect()

// WZD o105p10
// BU_SetBaseAnims()

// WZD o105p11
int16_t Battle_Unit_Melee_Attack_Icon(int16_t battle_unit_idx);

// WZD o105p12
int16_t Battle_Unit_Ranged_Attack_Icon(int16_t battle_unit_idx);

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
void Battle_Unit_Heal(int16_t battle_unit_idx, int16_t Healing, int16_t TempHits);

// WZD o110p04
int16_t Get_Effective_Hits(int16_t hits, int16_t defense);

// WZD o110p05
int16_t Get_Effective_Ranged_Strength(int16_t ranged, int16_t figures, int16_t attack_flags);

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
int16_t G_CMB_CastSpell__WIP(int16_t caster_id, int16_t wx, int16_t wy, int16_t wp);

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
void Deploy_Battle_Units(int16_t player_idx);

// WZD o113p10
// BU_SummonDemon()

// WZD o113p11
int16_t BU_MeleeWallCheck__WIP(int16_t src_battle_unit_idx, int16_t dst_battle_unit_idx);

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
    WIZARDS.EXE  ovr116
*/

// WZD o116p01
uint32_t BU_Apply_Item_Powers(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit);

// WZD o116p02
void BU_Apply_Item_Enchantments(int16_t item_idx, struct s_BATTLE_UNIT * battle_unit);

// WZD o116p03
void BU_Apply_Item_Attack_Specials(uint16_t * attack_flags, int16_t item_idx);

// WZD o116p04
int16_t Unit_Hit_Points(int16_t unit_idx);

// WZD o116p05
int16_t Battle_Unit_Hit_Points(struct s_BATTLE_UNIT * battle_unit);

// WZD o116p06
void Load_Battle_Unit(int16_t unit_idx, struct s_BATTLE_UNIT * BattleUnit);

// WZD o116p07
void BU_Init_Battle_Unit(struct s_BATTLE_UNIT * BattleUnit);

// WZD o116p08
/*
s_BATTLE_UNIT.Move_Flags
s_BATTLE_UNIT.Attribs_1
s_BATTLE_UNIT.Abilities

*/
void BU_Apply_Specials(struct s_BATTLE_UNIT * battle_unit, uint32_t battle_unit_enchantments, uint8_t unit_mutations);

// WZD o116p09
void BU_Apply_Level(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit);

// WZD o116p10
void BU_Init_Hero_Unit(int16_t unit_idx, struct s_BATTLE_UNIT * battle_unit);

// WZD o116p11
void BU_Apply_Battlefield_Effects__WIP(struct s_BATTLE_UNIT * battle_unit);

// WZD o116p12
// sub_9A43E()



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
int16_t Undeployable_Battle_Units_On_Water(int16_t player_idx);

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
int16_t BU_GetHalfMoves__WIP(int16_t battle_unit_idx);

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



/*
    WIZARDS.EXE  ovr153
*/

// WZD ovr153p01
// drake178: UU_sub_DC990()

// WZD ovr153p02
void CMB_DrawMap__WIP(void);

// WZD ovr153p03
void CMB_DrawEntities__WIP(void);

// WZD ovr153p04
// drake178: sub_DD536()

// WZD ovr153p05
// drake178: CMB_SpawnTrees()

// WZD ovr153p06
// drake178: CMB_SpawnRocks()

// WZD ovr153p07
// drake178: CMB_SpawnStructures()

// WZD ovr153p08
// drake178: CMB_SpawnStoneWall()

// WZD ovr153p09
// drake178: CMB_SpawnDarkWall()

// WZD ovr153p10
// drake178: CMB_SpawnFireWall()

// WZD ovr153p11
// drake178: CMB_SpawnVortices()

// WZD ovr153p12
void CMB_SpawnFigure__WIP(int64_t seg_or_idx, int16_t PosX, int16_t PosY, int16_t TarX, int16_t TarY, int16_t MStage, int16_t FigId, int16_t MaxFigs, int16_t Owner, int16_t outline_magic_realm, int16_t BldAmt, int16_t UU, int16_t LostFigs, int16_t SrcBld);

// WZD ovr153p13
// drake178: CMB_SpawnUnitCurse()

// WZD ovr153p14
void CMB_EntitiesReset(void);

// WZD ovr153p15
void USELESS_Combat_Figure_Load_Compose(int16_t figure_index, int16_t figure_set_idx, int16_t player_idx, int16_t enchantment_magic_realm, int16_t Frame);

// WZD ovr153p16
// drake178: CMB_SpawnProjectiles()

// WZD ovr153p17
// drake178: CMB_GetFigDrawPos()

// WZD ovr153p18
void CMB_CreateEntity__WIP(int16_t OrigX, int16_t OrigY, int64_t seg_or_idx, int16_t draw_x_shift, int16_t draw_y_shift, int16_t Frame, int16_t entity_type, int16_t Owner, int16_t UU_10h, int16_t outline_magic_realm, int16_t BldAmt, int16_t UU_14h, int16_t BldFrm);

// WZD ovr153p19
void Set_Entity_Draw_Order(void);

// WZD ovr153p20
void EMM_FIGUREX_Init__HACK(int16_t figure_index);

// WZD ovr153p21
void EMM_TILEX_Init__HACK(void);

// WZD ovr153p22
// drake178: UU_NullSub_153_1()

// WZD ovr153p23
void Combat_Figure_Compose_USEFULL(void);

// WZD ovr153p24
// drake178: RP_NullSub_153_2()



/*
    WIZARDS.EXE  ovr154
*/

// WZD ovr154p01
void CMB_Terrain_Init__WIP(int16_t wx, int16_t wy, int16_t wp);

// WZD ovr154p02
void Generate_Combat_Map__WIP(int16_t location_type, int16_t house_type, int16_t roads_array[], int16_t enchanted_roads_flag, int16_t ctt, int16_t rivers_array[], int16_t flying_fortress_flag, int16_t attacker_floating_island_flag, int16_t defender_floating_island_flag, int16_t city_walls, int16_t city_population, int16_t magic_walls);

// WZD ovr154p03
// drake178: CMB_TreeGen()

// WZD ovr154p04
// drake178: CMB_RockGen()

// WZD ovr154p05
// drake178: CMB_RoadGen()

// WZD ovr154p06
// drake178: CMB_RiverGen()

// WZD ovr154p07
// drake178: CMB_MergeDirt()

// WZD ovr154p08
// drake178: CMB_RemoveRough()

// WZD ovr154p09
void CMB_TileGen__WIP(int16_t ctt);

// WZD ovr154p10
void CMB_SetMoveMaps__WIP(int16_t location_type, int16_t city_walls);

// WZD ovr154p11
// drake178: CMB_TerrPatchGen()

// WZD ovr154p12
void Combat_Grid_Screen_Coordinates(int16_t cgc2, int16_t cgc1, int16_t something_x, int16_t something_y, int16_t * screen_x, int16_t * screen_y);

// WZD ovr154p13
// drake178: CMB_EarthToMud()

// WZD ovr154p14
int16_t Get_Combat_Grid_Cell_X(int16_t screen_x, int16_t screen_y);

// WZD ovr154p15
int16_t Get_Combat_Grid_Cell_Y(int16_t screen_x, int16_t screen_y);

// WZD ovr154p16
void Screen_To_Combat_Grid_Cell_X_And_Offset(int16_t screen_x, int16_t screen_y, int16_t * Tile_X, int16_t * InTile_X);

// WZD ovr154p17
void Screen_To_Combat_Grid_Cell_Y_And_Offset(int16_t screen_x, int16_t screen_y, int16_t * Tile_Y, int16_t * InTile_Y);

// WZD ovr154p18
void Combat_Cache_Read(void);

// WZD ovr154p19
void Combat_Cache_Write(void);



/*
    WIZARDS.EXE  ovr155
*/

// WZD ovr155p01
// WZD ovr155p02

// WZD ovr155p01
// drake178: CMB_GetPath()

// WZD ovr155p02
// drake178: CMB_FillReachMap()



/*
    WIZARDS.EXE  ovr163
*/

// WZD ovr163p01
void Load_Combat_Terrain_Pictures(int16_t ctt, int16_t wp);

// WZD ovr163p02
void CMB_ComposeBackgrnd__WIP(void);

// WZD ovr163p03
// CMB_LoadWallRiseGFX()

// WZD ovr163p04
void CMB_BaseAllocs__WIP(void);

// WZD ovr163p05
// CMB_SetProjectiles()

// WZD ovr163p06
int16_t Combat_Figure_Load(int16_t unit_type, int16_t figure_index);



#ifdef __cplusplus
}
#endif

#endif  /* COMBAT_H */
