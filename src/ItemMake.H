/*
    ENCHANT
    Make Item
    ...itemmake (IM)
    
    WIZARDS.EXE
        ovr115
*/

#ifndef ITEMMAKE_H
#define ITEMMAKE_H

#include "MoM.H"

#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr115
*/

// WZD o115p01
// ITEM_LoadCraftingRes()

// WZD o115p02
// sub_94FDD()

// WZD o115p03
char * Get_Item_Type_Name(int16_t item_idx);

// WZD o115p04
// PRIVATE  char * Get_Item_Name(int16_t item_idx);

// WZD o115p05
// PRIVATE  int16_t Get_Item_Cost(int16_t item_idx);

// WZD o115p06
// sub_95B62()

// WZD o115p07
// ITEM_CreateControls()

// WZD o115p08
// sub_95E65()

// WZD o115p09
// PRIVATE  void Create_Item_Record(int16_t item_idx);

// WZD o115p11
// ITEM_Get_IC_Powers()

// WZD o115p12
// sub_96AF2()

// WZD o115p13
// sub_96D84()

// WZD o115p14
// PRIVATE  int16_t Add_Item(int16_t itemdata_idx);

// WZD o115p15
static int16_t Activate_Item(int16_t old_item_idx);

// WZD o115p16
// PRIVATE  int16_t Create_Random_Item(int16_t Power, int16_t Value);

// WZD o115p17
int16_t Make_Item(int16_t Power, int16_t spellranks[], int16_t Value);



#ifdef __cplusplus
}
#endif

#endif  /* ITEMMAKE_H */
