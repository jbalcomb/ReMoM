/*

    Validate Game-Data Data-Points

*/

#include "STU_DBG.h"
#include "STU_TYPE.h"

#include "../MOX/MOM_Data.h"

#include "STU_VLD.h"



enum e_UNITS_DATA
{
    vud_wx,
    vud_wy,
    vud_wp,
    vud_owner_idx,
    vud_moves2_max,
    vud_type,
    vud_Hero_Slot,
    vud_Finished,
    vud_moves2,
    vud_dst_wx,
    vud_dst_wy,
    vud_Status,
    vud_Level,
    vud_pad2B_0Dh,
    vud_XP,
    vud_Move_Failed,
    vud_Damage,
    vud_Draw_Priority,
    vud_pad2B_13h,
    vud_in_tower,
    vud_Sight_Range,
    vud_mutations,
    vud_enchantments,
    vud_Rd_Constr_Left,
    vud_Rd_From_X,
    vud_Rd_From_Y,
    vud_pad2B_1Fh
};
/*
int8_t  wx
int8_t  wy
int8_t  wp
int8_t  owner_idx
int8_t  moves2_max
uint8_t type
int8_t  Hero_Slot
int8_t  Finished
int8_t  moves2
int8_t  dst_wx
int8_t  dst_wy
int8_t  Status
int8_t  Level
uint8_t pad2B_0Dh
int16_t XP
int8_t  Move_Failed
int8_t  Damage
int8_t  Draw_Priority
uint8_t pad2B_13h
int16_t in_tower
int8_t  Sight_Range
int8_t  mutations
uint32_t enchantments
int8_t  Rd_Constr_Left
int8_t  Rd_From_X
int8_t  Rd_From_Y
uint8_t pad2B_1Fh
*/

/*
    us_Ready          =   0
    us_Patrol         =   1
    us_BuildRoad      =   2
    us_GOTO           =   3
    us_ReachedDest    =   4
    us_Wait           =   5
    us_Casting        =   6
    us_Unknown_7      =   7
    us_Purify         =   8
    us_Meld           =   9
    us_Settle         =  10
    us_SeekTransport  =  11
    us_Unknown_12     =  12
    us_Unknown_13     =  13
    us_Unknown_14     =  14
    us_Unknown_15     =  15
    us_Move           =  16
    us_Unknown_100    = 100
    us_PurifyDone     = 111
*/
void Validate_Units_Status(int8_t status)
{

    if(status != us_Wait)
    {
        STU_DEBUG_BREAK();
    }

}