/*
    Items

    WIZARDS.EXE
        ovr109
*/

#ifndef ITEMS_H
#define ITEMS_H

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr109
*/

// WZD o109p01
void Player_Process_Item_Pool(int16_t player_idx, int16_t item_count, int16_t item_list[]);

// WZD o109p02
void Remove_Item(int16_t item_idx);

// WZD o109p03
void AI_Process_Item_Pool(int16_t player_idx, int16_t item_count, int16_t item_list[]);

// WZD o109p04
// drake178: AI_EquipHeroFirst()
void AI_Equip_Hero_Best_Items(int16_t player_idx, int16_t hero_slot_idx, int16_t item_list[], int16_t * item_count);

// WZD o109p05
// drake178: AI_EquipRemainder()
void AI_Equip_Hero_Empty_Slots(int16_t player_idx, int16_t hero_slot_idx, int16_t item_list[], int16_t * item_count);



#ifdef __cplusplus
}
#endif

#endif  /* ITEMS_H */
