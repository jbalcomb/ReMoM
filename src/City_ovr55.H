/*
    ¿ ~== City (Screen) Util / Misc ?

    WIZARDS.EXE
        ovr055

*/

#ifndef CITYOVR55_H
#define CITYOVR55_H

#include "MoM.H"


#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr055
*/



// WZD o55p01
// drake178: ¿ ?
// Enemy_City_Screen()

// WZD o55p02
// drake178: ¿ ?
// Enemy_City_Screen_Draw()

// WZD o55p03
// drake178: ¿ ?
// Enemy_City_Screen_Load()

// WZD o55p04
// drake178: ¿ ?
// IDK_City_Cityscape_Draw_MouseOver()

// WZD o55p05
// drake178: CTY_CreateEmpty()

// WZD o55p06
// drake178: CTY_TakeOver()
void Change_City_Ownership(int16_t city_idx, int16_t player_idx);

// WZD o55p07
/*
    called from End_Of_Combat()
        |-> CTY_ApplyDamage(OVL_Action_Structure, _DI_IDK_population_lost, Destruction_Chance, &Buildings_Lost[0])
processes direct population loss,
rolls for and applies building destruction (marking them into the return array),
recalculates production validity,
and destroys the city if it has no population left
; INCONSISTENT with tile requirements and destroyed buildings
*/
void CTY_ApplyDamage(int16_t city_idx, int16_t PopLoss, int16_t DChance, int16_t BList[]);

// WZD o55p08
// drake178: CTY_Remove()
void Destroy_City(int16_t city_idx);

// WZD o55p09
void Print_City_Enchantment_List(int16_t start_x, int16_t start_y, int16_t * city_enchantment_list, int16_t * city_enchantment_owner_list, int16_t city_enchantment_list_count);

// WZD o55p10
// drake178: CTY_ClearEnchant()

// WZD o55p11
// drake178: N/A
// sub_4B9BF()

// WZD o55p12
// drake178: CTY_DestroyBuilding()
// MoO2  Module: ERICNET  Remove_Building_()
int16_t City_Remove_Building(int16_t city_idx, int16_t bldg_idx);

// WZD o55p13
// drake178: N/A
int16_t City_Sell_Building(int16_t city_idx, int16_t bldg_idx, int16_t gold_amount);

// WZD o55p14
// drake178: ¿ ?
int16_t City_Building_Has_Requirement(int16_t bldg_idx, int16_t city_idx, int16_t * reqd_bldg_idx);

// WZD o55p15
// drake178: N/A
// sub_4BBAF()
int16_t City_Building_Is_Currently_Required(int16_t city_idx, int16_t bldg_idx);

// WZD o55p16
void City_Check_Production(int16_t city_idx);

// WZD o55p17
void City_Cancel_Production(int16_t city_idx);

// WZD o55p18
// drake178: N/A
// sub_4BEB3()

// WZD o55p19
// drake178: N/A
// sub_4BEF0()

// WZD o55p20
// drake178: CTY_BuyProduction()
void Player_City_Buy_Production(int16_t player_idx, int16_t city_idx);

// WZD o55p21
// drake178: CTY_GetProdBuyCost()
// MoO2  Module: COLCALC  Colony_Cost_To_Buy_Product_()
int16_t City_Cost_To_Buy_Product(int16_t city_idx);

// WZD o55p22
// drake178: ¿ ?
void City_Screen_Add_Fields_Resource_Window(int16_t city_idx, int16_t xstart, int16_t ystart);

// WZD o55p23
// drake178: ¿ ?
void City_Screen_Draw_Resource_Icons(int16_t city_idx, int16_t xstart, int16_t ystart);

// WZD o55p24
// drake178: CTY_DrawResourceCnt()
void Draw_Resource_Icons(int16_t * xstart, int16_t ystart, int16_t total_amount, int16_t group_amount, SAMB_ptr big_icon_pict, SAMB_ptr lil_icon_pict);

// WZD o55p25
// drake178: ¿ ?
void City_Screen_Draw_Production_Coins(int16_t city_idx);

// WZD o55p26
// drake178: ¿ ?
void Build_City_Enchantment_List(int16_t city_idx, int16_t * EList, int16_t * EOwners, int16_t * ECount);

// WZD o55p27
// drake178: ¿ ?
void Do_Build_City_Enchantment_List(void);

// WZD o55p28
// drake178: ¿ ?
void City_Screen_Draw_City_Enchantments__WIP(int16_t xstart, int16_t ystart);

// WZD o55p29
// drake178: CTY_CreateEnchCtrls()
void City_Add_Fields_City_Enchantments(int16_t xstart, int16_t ystart);

// WZD o55p30
// drake178: ¿ ?
void City_Screen_Draw_Population_Row(int16_t city_idx, int16_t xstart, int16_t ystart);

// WZD o55p31
// drake178: ¿ ?
void City_Screen_Add_Fields_Population_Row(int16_t city_idx, int16_t xstart, int16_t ystart);

// WZD o55p32
// drake178: ¿ ?
void City_Screen_Garrison_Window_Picture_Coords(int16_t stack_idx, int16_t * x1, int16_t * y1, int16_t * x2, int16_t * y2);

// WZD o55p33
// drake178: N/A
// IDK_CtyBld_s4D357()

// WZD o55p34
// drake178: WIZ_AddGold()
void Player_Add_Gold(int16_t player_idx, int16_t gold_amount);

// WZD o55p35
// drake178: WIZ_AddMana()
void Player_Add_Mana(int16_t player_idx, int16_t mana);

// WZD o55p36
// drake178: ¿ ?
// City_Enchantment_HelpIdx()

// WZD o55p37
// drake178: STR_GenitiveCase()

// WZD o55p38
// drake178: N/A
// sub_4D5EA()

// WZD o55p39
// drake178: N/A
// sub_4DA19()

// WZD o55p40
// drake178: N/A
// sub_4E507()



#ifdef __cplusplus
}
#endif



#endif /* CITYOVR55_H */
