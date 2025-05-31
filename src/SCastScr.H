/*
    Spell Casting Screen

    WIZARDS.EXE
        ovr070
*/
#ifndef SCASTSCR_H
#define SCASTSCR_H

#include "MOX/MOX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif



enum e_OSC_TARGET_TYPE
{
    osc_Friendly_Unit   = 0,
    osc_Friendly_Group  = 1,
    osc_Enemy_Unit      = 2,
    osc_Enemy_Group     = 3,
    osc_Map_Square      = 4,
    osc_Friendly_City   = 5,
    osc_Enemy_City      = 6,
    osc_Magic_Node      = 7
};



/*
    WIZARDS.EXE  ovr070

*/

// WZD o70p01
int16_t Spell_Casting_Screen__WIP(int16_t spell_target_type, int16_t * wx, int16_t * wy, int16_t * wp, int16_t * target_wx, int16_t * target_wy, char * spell_name);

// WZD o70p02
void Spell_Casting_Screen_Add_Fields(void);

// WZD o70p03
void Spell_Casting_Screen_Draw(void);

// WZD o70p04
int16_t World_To_Screen(int16_t map_wx, int16_t map_wy, int16_t * unit_wx, int16_t * unit_wy);

// WZD o70p05
void Spell_Casting_Screen_Allocate(void);

// WZD o70p06
void Build_Select_Target_String(int16_t spell_target_type, char * spell_name);

// WZD o70p07

// WZD o70p08

// WZD o70p09

// WZD o70p10

// WZD o70p11

// WZD o70p12

// WZD o70p13
int16_t IDK_Map_Square_Is_Targetable(int16_t mx, int16_t my);

// WZD o70p14
void Set_Mouse_List_Image_Nums(void);

// WZD o70p15
void Spell_Casting_Screen_Draw_Panel(void);


#ifdef __cplusplus
}
#endif

#endif  /* SCASTSCR_H */
