/*
    Unit View

    WIZARDS.EXE
        ovr072
        ovr078
        ovr089
*/

#ifndef UNITVIEW_H
#define UNITVIEW_H

#include "MOX/MOM_Data.H"
#include "MOX/MOX_TYPE.H"

#include "Help.H"



// ¿ "UNICON" ?
#define SPECIAL_ICON_TELEPORTING          0
#define SPECIAL_ICON_FORESTER             1
#define SPECIAL_ICON_MOUNTAINEER          2
#define SPECIAL_ICON_PATHFINDING         20
#define SPECIAL_ICON_CONSTRUCTION        36
#define SPECIAL_ICON_SCOUTING            37
#define SPECIAL_ICON_CASTER              55

#define SPECIAL_ICON_WARPED_DEFENSE      62
#define SPECIAL_ICON_WARPED_FLIGHT       63
#define SPECIAL_ICON_WARPED_BREATH       64

#define SPECIAL_ICON_STASIS             115
#define SPECIAL_ICON_UNDEAD             120
#define SPECIAL_ICON_QUIVER             136
#define SPECIAL_ICON_SUMMON_DEMONS      139

#define SPECIAL_ICON_LEVEL_MEDALS       102

#define SPECIAL_ICON_THROWN             130
#define SPECIAL_ICON_FIRE_BREATH        138
#define SPECIAL_ICON_LIGHTNING_BREATH   137
#define SPECIAL_ICON_STONING_GAZE        26
#define SPECIAL_ICON_DOOM_GAZE          134
#define SPECIAL_ICON_DEATH_GAZE         135



enum e_VIEW_TYPE
{
    uvt_Prod,
    uvt_Stat,
    uvt_Cmbt
};


// drake178: USW_HeroAbl
// sizeof:  08h  8d
struct USW_HeroAbl
{
    /* 00 */  uint32_t flags;
    /* 04 */  uint8_t icon_idx;
    /* 05 */  // 2-byte alignment padding
    /* 06 */  int16_t help_idx;
    /* 08 */
};



#ifdef __cplusplus
extern "C" {
#endif



// WZD dseg:3A50
extern struct USW_HeroAbl USW_HeroAbilities[23];

// WZD dseg:3F52
extern char * USW_HeroLevels[9];




/*
    WIZARDS.EXE  ovr072
*/



/*
    WIZARDS.EXE  ovr078
*/

// WZD o78p01
// USW_Build_Effect_List
void USW_Build_Effect_List(int16_t unit_idx, struct s_UV_List * specials_list, int16_t * specials_count);

// WZD o78p02
/*
    only called from 'Production Screen'

*/
void Prod_Build_Specials_List(int16_t unit_type, struct s_UV_List * list_buffer, int16_t * list_count);

// WZD o78p03
void ITEM_GetPowerNames(int16_t item_idx, char power_names[5][20], char power_descriptions[5][40], int16_t * power_count);



/*
    WIZARDS.EXE  ovr089
*/

// WZD o89p01
void Prod_Load_Battle_Unit(int16_t unit_type, struct s_BATTLE_UNIT * battle_unit);

// WZD o89p02
void Unit_View_Allocate(void);

// WZD o89p03
void Load_Unit_Figure(int16_t type_or_unit, int16_t use_type);

// WZD o89p04
void Get_Unit_Enchantment_Names(int16_t unit_idx, char ** unit_enchantment_names, int16_t * unit_enchantment_count);

// WZD o89p05
void Draw_Unit_Figure(int16_t x_start, int16_t y_start, int16_t unit_idx, int16_t VType);

// WZD o89p06
void Unit_Figure_Position(int16_t figure_count, int16_t current_figure, int16_t * figure_x, int16_t * figure_y);

// WZD o89p07
int16_t UV_Remove_Unit_Enchantment(int16_t unit_odx, uint32_t enchantmnt, char * message);

// WZD o89p08
void Unit_Remove_Enchantment(int16_t unit_idx, uint32_t enchantment);

// WZD o89p09
// IDK_Drown_Units()

// WZD o89p10
void UNIT_GetDependants__WIP(int16_t unit_idx, int16_t * troop_count, int16_t troops[]);



#ifdef __cplusplus
}
#endif


#endif  /* UNITVIEW_H */
