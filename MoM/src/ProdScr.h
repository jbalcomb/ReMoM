/*
    Production Screen

    WIZARDS.EXE
        ovr056

*/

#ifndef PRODSCR_H
#define PRODSCR_H

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr056
*/



// WZD o56p01
void Production_Screen(void);

// WZD o56p02
void Production_Screen_Load(void);

// WZD o56p03
void Production_Screen_Allocate_First_Block(void);

// WZD o56p04
void Production_Screen_Add_Fields(void);

// WZD o56p05
void Production_Screen_Draw(void);

// WZD o56p06
void Production_Screen_Draw_(void);

// WZD o56p07
void Building_Allows_List(int16_t bldg_idx, int16_t * allows_list_count, int16_t allows_list[]);

// WZD o56p08
void Calculate_Product_Array(int16_t city_idx, int16_t * total_count, int16_t * depr_count, int16_t * unit_count, int16_t product_array[]);

// WZD o56p09
int16_t City_Check_Terrain_Requisite(int16_t city_idx, int16_t btp_idx);

// WZD o56p10
int16_t City_Production_Cost(int16_t production_idx, int16_t city_idx);

// WZD o56p11
int16_t City_N_Turns_To_Produce(int16_t production_cost, int16_t city_idx);

// WZD o56p12
int16_t City_Race_Can_Unlock_Building(int16_t uu_allowed_by_bldg_idx, int16_t building);



#ifdef __cplusplus
}
#endif



#endif /* PRODSCR_H */
