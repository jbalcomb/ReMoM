/*
    ¿ ~== City (Screen) Util / Misc ?

    WIZARDS.EXE
        ovr055

*/

#ifndef CITYOVR55_H
#define CITYOVR55_H

#include "MOX/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr055
*/



// WZD o55p01
void Enemy_City_Screen(void);

// WZD o55p02
void Enemy_City_Screen_Draw(void);

// WZD o55p03
void Enemy_City_Screen_Load(void);

// WZD o55p04
void Cityscape_Draw_Scanned_Building_Name(int16_t scanned_field, int16_t x_start, int16_t owner_idx);

// WZD o55p05
int16_t Init_Outpost(void);

// WZD o55p06
void Change_City_Ownership(int16_t city_idx, int16_t player_idx);

// WZD o55p07
void Apply_Damage_To_City(int16_t city_idx, int16_t population_lost, int16_t bldg_chance, int16_t bldg_list[]);

// WZD o55p08
void Destroy_City(int16_t city_idx);

// WZD o55p09
void Print_City_Enchantment_List(int16_t start_x, int16_t start_y, int16_t * city_enchantment_list, int16_t * city_enchantment_owner_list, int16_t city_enchantment_list_count);

// WZD o55p10
void Turn_Off_City_Enchantment(int16_t city_idx, int16_t ench_idx);

// WZD o55p11
// sub_4B9BF()

// WZD o55p12
int16_t City_Remove_Building(int16_t city_idx, int16_t bldg_idx);

// WZD o55p13
int16_t City_Sell_Building(int16_t city_idx, int16_t bldg_idx, int16_t gold_amount);

// WZD o55p14
int16_t City_Building_Has_Requirement(int16_t bldg_idx, int16_t city_idx, int16_t * reqd_bldg_idx);

// WZD o55p15
int16_t City_Building_Is_Currently_Required(int16_t city_idx, int16_t bldg_idx);

// WZD o55p16
void City_Check_Production(int16_t city_idx);

// WZD o55p17
void City_Cancel_Production(int16_t city_idx);

// WZD o55p18
int16_t City_Sell_Building_Value(int16_t bldg_idx);

// WZD o55p19
// sub_4BEF0()

// WZD o55p20
void Player_City_Buy_Production(int16_t player_idx, int16_t city_idx);

// WZD o55p21
int16_t City_Cost_To_Buy_Product(int16_t city_idx);

// WZD o55p22
void City_Screen_Add_Fields_Resource_Window(int16_t city_idx, int16_t xstart, int16_t ystart);

// WZD o55p23
void City_Screen_Draw_Resource_Icons(int16_t city_idx, int16_t xstart, int16_t ystart);

// WZD o55p24
void Draw_Resource_Icons(int16_t * xstart, int16_t ystart, int16_t total_amount, int16_t group_amount, SAMB_ptr big_icon_pict, SAMB_ptr lil_icon_pict);

// WZD o55p25
void City_Screen_Draw_Production_Coins(int16_t city_idx);

// WZD o55p26
void Build_City_Enchantment_List(int16_t city_idx, int16_t city_enchantment_list[], int16_t city_enchantment_owner_list[], int16_t * city_enchantment_list_count);

// WZD o55p27
void Do_Build_City_Enchantment_List(void);

// WZD o55p28
void City_Screen_Draw_City_Enchantments(int16_t xstart, int16_t ystart);

// WZD o55p29
void City_Add_Fields_City_Enchantments(int16_t xstart, int16_t ystart);

// WZD o55p30
void City_Screen_Draw_Population_Row(int16_t city_idx, int16_t xstart, int16_t ystart);

// WZD o55p31
void City_Screen_Add_Fields_Population_Row(int16_t city_idx, int16_t xstart, int16_t ystart);

// WZD o55p32
void City_Screen_Garrison_Window_Picture_Coords(int16_t stack_idx, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2);

// WZD o55p33
void All_City_Removed_Buildings(void);

// WZD o55p34
void Player_Add_Gold(int16_t player_idx, int16_t gold_amount);

// WZD o55p35
void Player_Add_Mana(int16_t player_idx, int16_t mana);

// WZD o55p36
int16_t City_Enchantment_HelpIdx(int16_t city_enchantment_idx);

// WZD o55p37
void Possessive(char * string);

// WZD o55p38
void Resource_Breakdown__STUB(int16_t IDK);

// WZD o55p39
void Resource_Breakdown_Draw__STUB(void);

// WZD o55p40
// sub_4E507()



#ifdef __cplusplus
}
#endif



#endif /* CITYOVR55_H */
