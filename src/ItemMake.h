/*
    ENCHANT
    Make Item
    ...itemmake (IM)
    
    WIZARDS.EXE
        ovr115
*/

#ifndef ITEMMAKE_H
#define ITEMMAKE_H

#include "MOX/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr115
*/

// WZD o115p01
// static void Item_Make_Screen_Load(void);

// WZD o115p02
// static void Item_Make_Screen_Draw(void);

// WZD o115p03
char * Get_Item_Type_Name(int16_t item_idx);

// WZD o115p04
// static const char * Get_Item_Name(int16_t item_idx);

// WZD o115p05
// static int16_t Get_Item_Cost(int16_t item_idx);

// WZD o115p06
// static void Item_Make_Screen_Build_Spell_List(void);

// WZD o115p07
// static void Item_Make_Screen_Add_Fields(void);

// WZD o115p08
int16_t Item_Make_Screen(int16_t player_idx, int16_t type);

// WZD o115p09
// static void Create_Item_Record(int16_t item_idx);

// WZD o115p11
// static int16_t Itam_Make_Screen_Build_Weapon_Powers_List(int16_t player_idx, int16_t max_cost);

// WZD o115p12
// static void Item_Make_Screen_Spellbook_Popup_Draw(void);

// WZD o115p13
// static int16_t Item_Make_Screen_Spellbook_Popup(void);

// WZD o115p14
// static int16_t Add_Item(int16_t itemdata_idx);

// WZD o115p15
int16_t Activate_Item(int16_t old_item_idx);

// WZD o115p16
// static void Create_Random_Item(int16_t Power, int16_t Value);

// WZD o115p17
int16_t Make_Item(int16_t Power, int16_t spellranks[], int16_t Value);



#ifdef __cplusplus
}
#endif

#endif  /* ITEMMAKE_H */
