/*
    AI - Move

    WIZARDS.EXE
        ovr158
        ovr162
*/

#ifndef AIMOVE_H
#define AIMOVE_H

#include "../../MoX/src/MOX_TYPE.h"



#ifdef __cplusplus
extern "C" {
#endif



/*
    WIZARDS.EXE  ovr158
*/

// WZD o158p01
void AI_Set_Unit_Orders(int16_t player_idx);

// WZD o158p02
// static void AI_Stacks_Stage_Expedition_Forces(int16_t landmass_idx, int16_t wp, int16_t player_idx);

// WZD o158p03
void AI_Stacks_Garrison_Sites(int16_t player_idx, int16_t wp, int16_t landmass_idx);

// WZD o158p04
void AI_ProcessOcean__WIP(int16_t player_idx, int16_t wp);

// WZD o158p05
void G_AI_ProcessTransports__WIP(int16_t player_idx, int16_t wp);

// WZD o158p06
void AI_Stacks_Roamers_Target_Or_Deploy(int16_t landmass_idx, int16_t wp, int16_t player_idx);

// WZD o158p07
void AI_Stacks_Relocate_Roamers(int16_t landmass_idx, int16_t wp, int16_t player_idx);

// WZD o158p08
void AI_Stacks_Order_To_War_Landmass(int16_t player_idx, int16_t wp);

// WZD o158p09
void AI_Stacks_Setup_Ferry(int16_t stack_idx, int16_t landmass_idx, int16_t wp, int16_t stage_wx, int16_t stage_wy, int16_t stage_count_slot_count, int16_t player_idx);

// WZD o158p10
void AI_Build_Target_List(int16_t player_idx, int16_t landmass_idx, int16_t wp);

// WZD o158p11
void AI_Add_Target(int16_t wx, int16_t wy, int16_t strength, int16_t value);

// WZD o158p12
int16_t AI_Stacks_Assign_Target(int16_t stack_wx, int16_t stack_wy, int16_t * target_wx, int16_t * target_wy, int16_t unit_list_idx, int16_t niu_list_unit_idx);

// WZD o158p13
void AI_Stacks_Init_Build_Target_Order(int16_t player_idx, int16_t landmass_idx, int16_t wp);

// WZD o158p14
int16_t AI_Stacks_Target_Nearest_Hostile_Stack(int16_t stack_idx, int16_t landmass_idx, int16_t * target_wx, int16_t * target_wy, int16_t wp);

// WZD o158p15
void AI_Find_Opportunity_City_Target(int16_t wp, int16_t player_idx);

// WZD o158p16
void AI_Disband_To_Balance_Budget(int16_t player_idx);

// WZD o158p17
void AI_Shift_Off_Home_Plane(int16_t player_idx);

// WZD o158p18
void AI_Move_Out_Boats(void);

// WZD o158p19
void AI_Stacks_Move_Out_NonMilitary_Garrisoned(int16_t wp);

// WZD o158p20
// static void AI_Stacks_Survey_Expedition_Forces(void);

// WZD o158p21
// static void AI_Stacks_Survey_Expedition_Forces_Stack(int16_t stack_idx, int16_t unit_count, int16_t excess_count);

// WZD o158p22
void AI_Do_Meld(int16_t player_idx);

// WZD o158p23
void AI_Do_Settle(int16_t player_idx, int16_t landmass_idx);

// WZD o158p24
void AI_Do_Purify(int16_t landmass_idx, int16_t wp);

// WZD o158p25
void AI_Do_RoadBuild(int16_t landmass_idx);

// WZD o158p26
void AI_Stacks_Order_Attack_Target_Or_Goto_Destination(int16_t unit_idx, int16_t target_wx, int16_t target_wy, int16_t itr_stacks1, int16_t itr_stacks2);

// WZD o158p27
void AI_Order_Settle(int16_t unit_idx, int16_t unit_list_idx, int16_t list_unit_idx);

// WZD o158p28
void AI_Order_RoadBuild(int16_t unit_idx, int16_t wx, int16_t wy, int16_t unit_list_idx, int16_t list_unit_idx);

// WZD o158p29
void AI_Stacks_Order_Ferry(int16_t unit_idx, int16_t unit_list_idx, int16_t list_unit_idx);

// WZD o158p30
void AI_Order_Meld(int16_t unit_idx, int16_t unit_list_idx, int16_t list_unit_idx);

// WZD o158p31
void AI_Order_Purify(int16_t unit_idx, int16_t unit_list_idx, int16_t list_unit_idx);

// WZD o158p32
int16_t Map_Square_Area_Has_Opponent(int16_t wx, int16_t wy, int16_t wp, int16_t radius, int16_t player_idx);

// WZD o158p33
int16_t AI_Enemy_Unit_In_Range(int16_t wx, int16_t wy, int16_t wp, int16_t radius, int16_t player_idx, int16_t landmass_idx);

// WZD o158p34
// AI_CanSettleOffPlane()
int16_t AI_CanSettleOffPlane__STUB(int16_t player_idx, int16_t unit_idx, int16_t * Tower_X, int16_t * Tower_Y, int16_t unit_wp);

// WZD o158p35
void AI_SendToColonize__WIP(int16_t unit_idx, int16_t wx, int16_t wy, int16_t wp, int16_t player_idx, int16_t unit_list_idx, int16_t list_unit_idx);

// WZD o158p36
void AI_Stacks_Ferry_Add_Location(int16_t wx, int16_t wy, int16_t wp);

// WZD o158p37
int16_t AI_Tower_Target_Worthwhile(int16_t player_idx, int16_t wx, int16_t wy, int16_t wp);

// WZD o158p38
void AI_Reevaluate_Continent(int16_t player_idx, int16_t landmass_idx, int16_t wp);



/*
    WIZARDS.EXE  ovr162
*/

// WZD o162p01
// sub_F48F0()

// WZD o162p02
// sub_F49E3()

// WZD o162p03
// sub_F4A9E()

// WZD o162p04
// sub_F4B96()

// WZD o162p05
// sub_F4C89()

// WZD o162p06
// sub_F4D32()

// WZD o162p07
// sub_F4E28()

// WZD o162p08
// sub_F5025()

// WZD o162p09
// sub_F514D()

// WZD o162p10
// sub_F528F()

// WZD o162p11
// sub_F5432()

// WZD o162p12
void AI_Stack_Set_Destination(int16_t stack_idx, int16_t wx, int16_t wy, int16_t player_idx);

// WZD o162p13
// sub_F57AF()

// WZD o162p14
// sub_F58D2()

// WZD o162p15
// sub_F59DA()

// WZD o162p16
// sub_F5B56()

// WZD o162p17
// sub_F5D73()

// WZD o162p18
// sub_F5EBF()

// WZD o162p19
void AI_Stack_Set_Boats_Goto(int16_t ai_stack_idx, int16_t wx, int16_t wy);

// WZD o162p20
// sub_F60F4()

// WZD o162p21
void TILE_AI_FindLoadTile__WIP(int16_t wx, int16_t wy, int16_t wp, int16_t * RetX, int16_t * RetY);

// WZD o162p22
// sub_F6316()

// WZD o162p23
int16_t TILE_AI_FindEmptyLnd__WIP(int16_t wx, int16_t wy, int16_t wp, int16_t * RetX, int16_t * RetY);

// WZD o162p24
// sub_F64C2()

// WZD o162p25
// sub_F671D()

// WZD o162p26
// sub_F689A()

// WZD o162p27
// sub_F68C9()

// WZD o162p28
// sub_F69A6()

// WZD o162p29
// sub_F6CAB()

// WZD o162p30
// sub_F6E1A()

// WZD o162p31
// sub_F6EBF()

// WZD o162p32
int16_t Adjacent_Free_Square(int16_t wx, int16_t wy, int16_t wp, int16_t * RetX, int16_t * RetY);

// WZD o162p33
int16_t Make_Monster_List(int16_t budget, int16_t lair_race, int16_t * unit_list);

// WZD o162p34
int16_t AI_Stack_Can_Mobilize(int16_t stack_idx, int16_t wx, int16_t wy, int16_t wp);

// WZD o162p35
void AI_Evaluate_Continents(int16_t player_idx);

// WZD o162p36
void AI_Evaluation_Map(int16_t player_idx);

// WZD o162p37
void AI_Choose_War_Landmass(int16_t player_idx);

// WZD o162p38
void Player_Clear_All_Unit_Orders(int16_t player_idx);



#ifdef __cplusplus
}
#endif

#endif  /* AIMOVE_H */
