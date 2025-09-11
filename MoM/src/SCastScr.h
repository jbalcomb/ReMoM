/*
    Spell Casting Screen

    WIZARDS.EXE
        ovr070
*/
#ifndef SCASTSCR_H
#define SCASTSCR_H

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



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
// DONT  void UU_IDK_Entirely_On_Map(int16_t * new_mx, int16_t * new_my, int16_t old_mx, int16_t old_my);

// WZD o70p08
void Spell_Casting_Screen_Reset_Map_Draw(void);

// WZD o70p09
void Spell_Casting_Screen_Reset_Map_Draw_With_WX__1(int16_t wx);

// WZD o70p10
void Spell_Casting_Screen_Reset_Map_Draw_With_WX__2(int16_t wx);

// WZD o70p11
void Spell_Casting_Screen_Reset_Map_Draw_With_WX__3(int16_t wx);

// WZD o70p12
void Spell_Casting_Screen_Reset_Map_Draw_With_WX__4(int16_t wx);

// WZD o70p13
int16_t Map_Square_Is_Targetable(int16_t mx, int16_t my);

// WZD o70p14
void Spell_Casting_Screen_Assign_Mouse_Images(void);

// WZD o70p15
void Spell_Casting_Screen_Draw_Panel(void);



#ifdef __cplusplus
}
#endif

#endif  /* SCASTSCR_H */
