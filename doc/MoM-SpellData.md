


spell_data_table

struct s_SPELL_DATA * spell_data_table;


/* 20 */  int8_t Param0;                // 2-byte, signed;  ; unit type, base damage, UE flag, or CE index;  for sdt_Global_Enchantment, used to index _players[].Globals[]



Magic Screen
Overland Enchantments
(only) iterates over first 215 spell data records
matches Param0 to index of player's overland enchantments array
and type of 9: Global_Enchantment

"overland enchantment window"

¿ "a list of overland enchantments" ?

Page 32
OVERLAND ENCHANTMENTS
This section of the magic summary screen shows any global enchantments currently in effect.  
The enchantments are shown in the banner-color of the casting wizard,  
  except for enchantments cast by your wizard, which are listed in dark print.  
To remove one of your wizard’s global enchantments (in dark letters),  
  click on the spell in the overland enchantment window.  
This also ends any maintenance you were paying for that spell.  
If you wish to dispel global enchantments that were cast by other wizards,  
  you must cast (by clicking on the spells button in the main movement screen and then clicking on the appropriate spell in your spell book; see Spell Casting)  
  disjunction, disjunction true or spell binding.  
After casting one of these spells, you can choose which global enchantment you wish to eliminate.  
Right-clicking on a global enchantment calls up its description.  


should all be just like the lists codes?

ovl_ench_list_spells
indexed by ovl_ench_cnt
ovl_ench_list_spells[ovl_ench_cnt] = itr2;
array of spell indicies
used to index spell_data_table to get .name for "Overland Enchantments"


ovl_ench_list_players
indexed by ovl_ench_cnt
ovl_ench_list_players[ovl_ench_cnt] = player_idx;
checked against _human_player_idx for "cancel"


Magic_Screen_Load_Pictures()
ovl_ench_list_spells = Near_Allocate_Next(192)
ovl_ench_list_players = Near_Allocate_Next(96)





// sizeof:  24h  36d
#pragma pack(push)
#pragma pack(2)
struct s_SPELL_DATA
{
    /* 00 */ char name[19];
    /* 13 */  int8_t AI_Group;              //  ; enum AI_Spell_Groups
    /* 14 */  int8_t AI_Value;
    /* 15 */  int8_t type;                  //  ; enum SPELLTYPES   "Type of spell effect"
    /* 16 */  int8_t Book_Section;
    /* 17 */  int8_t Realm;
    /* 18 */  int8_t Eligibility;
    /* 19 */  /* int8_t Unknown_19h; */  /* ¿ 2-byte alignment padding ? */
    /* 1A */  int16_t Casting_Cost;
    /* 1C */  int16_t research_cost;
    /* 1E */  int8_t Sound;
    /* 1F */  /* int8_t Unknown_1Fh; */  /* ¿ 2-byte alignment padding ? */
    /* 20 */  int8_t Param0;                // 2-byte, signed;  ; unit type, base damage, UE flag, or CE index;  for sdt_Global_Enchantment, used to index _players[].Globals[]
    /* 21 */  int8_t Param1;
    /* 22 */  int16_t Params2_3;            //  ; ATK_Flags
    /* 24 */
};
#pragma pack(pop)



enum_SPELL_DATA_TYPE
Summoning_Spell         =  0
Unit_Enchantment        =  1
City_Enchantment        =  2
City_Curse              =  3
Fixed_Dmg_Spell         =  4
Special_Spell           =  5
Target_Wiz_Spell        =  6
sdt_Global_Enchantment  =  9
Battlefield_Spell       = 10
Crafting_Spell          = 11
Destruction_Spell       = 12
Resistable_Spell        = 13
No_Resist_Spell         = 14
Mundane_Enchantment     = 15
Mundane_Curse           = 16
Infusable_Spell         = 17
Dispel_Spell            = 18
Disenchant_Spell        = 19
Disjunction_Spell       = 20
Counter_Spell           = 21
Var_Dmg_Spell           = 22
Banish_Spell            = 23
