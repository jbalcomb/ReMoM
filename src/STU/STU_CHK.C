#define _CRT_SECURE_NO_WARNINGS

#include "STU_CHK.H"

#include "STU.H"
#include "STU_TYPE.H"
#include "STU_DEF.H"
#include "STU_DBG.H"
#include "STU_UTIL.H"   /* get_datetime() */
#include "STU_CHK.H"

#include "../MOX/MOM_Data.H"

#include <string.h>     /* memcpy() memset(), strcat(), strcpy(); */



int16_t game_data_captured = STU_FALSE;
int16_t cities_data_captured = STU_FALSE;
int16_t units_data_captured = STU_FALSE;



/*
// WZD dseg:9CDC
uint8_t * _world_maps;

Allocate_Data_Space()
    _world_maps = (uint8_t *)Allocate_Next_Block(World_Data, 602);  // 602 PR, 9632 B

GET_TERRAIN_TYPE()
SET_TERRAIN_TYPE()

*/

/*
// WZD dseg:9CBC  
struct s_CITY * _CITIES;

Allocate_Data_Space()
    _CITIES = (struct s_CITY*)Allocate_First_Block(World_Data, 714);  // 714 PR, 11424 B

*/

uint8_t STU_CHK__world_maps[(NUM_PLANES * WORLD_WIDTH * WORLD_HEIGHT)];

struct s_CITY STU_CHK__CITIES[NUM_CITIES];

struct s_UNIT STU_CHK__UNITS[UNIT_COUNT_MAX];



void Copy_City_Struct(struct s_CITY * dst, struct s_CITY * src)
{
    int16_t itr;
    strcpy(dst->name, src->name);
    dst->race = src->race;
    dst->wx = src->wx;
    dst->wy = src->wy;
    dst->wp = src->wp;
    dst->owner_idx = src->owner_idx;
    dst->size = src->size;
    dst->population = src->population;
    dst->farmer_count = src->farmer_count;
    dst->sold_building = src->sold_building;
    dst->pad2B_17h = src->pad2B_17h;
    dst->Pop_10s = src->Pop_10s;
    dst->contacts = src->contacts;
    dst->pad2B_1Bh = src->pad2B_1Bh;
    dst->construction = src->construction;
    dst->bldg_cnt = src->bldg_cnt;
    for(itr = 0; itr < NUM_BUILDINGS; itr++)
        dst->bldg_status[itr] = src->bldg_status[itr];
    for(itr = 0; itr < NUM_CITY_ENCHANTMENTS; itr++)
        dst->enchantments[itr] = src->enchantments[itr];
    dst->production_units = src->production_units;
    dst->Prod_Accu = src->Prod_Accu;
    dst->gold_units = src->gold_units;
    dst->building_maintenance = src->building_maintenance;
    dst->mana_units = src->mana_units;
    dst->research_units = src->research_units;
    dst->food_units = src->food_units;
    for(itr = 0; itr < 13; itr++)
        dst->road_connections[itr] = src->road_connections[itr];
}

int16_t Check_City_Struct(struct s_CITY * dst, struct s_CITY * src)
{
    int16_t itr = 0xBB;
    if(strcmp(dst->name, src->name) != 0)
        STU_DEBUG_BREAK();
    if(dst->race != src->race)
        STU_DEBUG_BREAK();
    if(dst->wx != src->wx)
        STU_DEBUG_BREAK();
    if(dst->wy != src->wy)
        STU_DEBUG_BREAK();
    if(dst->wp != src->wp)
        STU_DEBUG_BREAK();
    if(dst->owner_idx != src->owner_idx)
        STU_DEBUG_BREAK();
    if(dst->size != src->size)
        STU_DEBUG_BREAK();
    if(dst->population != src->population)
        STU_DEBUG_BREAK();
    if(dst->farmer_count != src->farmer_count)
        STU_DEBUG_BREAK();
    if(dst->sold_building != src->sold_building)
        STU_DEBUG_BREAK();
    if(dst->pad2B_17h != src->pad2B_17h)
        STU_DEBUG_BREAK();
    if(dst->Pop_10s != src->Pop_10s)
        STU_DEBUG_BREAK();
    if(dst->contacts != src->contacts)
        STU_DEBUG_BREAK();
    if(dst->pad2B_1Bh != src->pad2B_1Bh)
        STU_DEBUG_BREAK();
    if(dst->construction != src->construction)
        STU_DEBUG_BREAK();
    if(dst->bldg_cnt != src->bldg_cnt)
        STU_DEBUG_BREAK();
    for(itr = 0; itr < NUM_BUILDINGS; itr++)
        if(dst->bldg_status[itr] != src->bldg_status[itr])
            STU_DEBUG_BREAK();
    for(itr = 0; itr < NUM_CITY_ENCHANTMENTS; itr++)
        if(dst->enchantments[itr] != src->enchantments[itr])
            STU_DEBUG_BREAK();
    if(dst->production_units != src->production_units)
        STU_DEBUG_BREAK();
    if(dst->Prod_Accu != src->Prod_Accu)
        STU_DEBUG_BREAK();
    if(dst->gold_units != src->gold_units)
        STU_DEBUG_BREAK();
    if(dst->building_maintenance != src->building_maintenance)
        STU_DEBUG_BREAK();
    if(dst->mana_units != src->mana_units)
        STU_DEBUG_BREAK();
    if(dst->research_units != src->research_units)
        STU_DEBUG_BREAK();
    if(dst->food_units != src->food_units)
        STU_DEBUG_BREAK();
    for(itr = 0; itr < 13; itr++)
        if(dst->road_connections[itr] != src->road_connections[itr])
            STU_DEBUG_BREAK();
    
    return STU_TRUE;

}

void Capture_Cities_Data(void)
{
    int16_t city_idx = 0;
    for(city_idx = 0; city_idx < NUM_CITIES; city_idx++)
        Copy_City_Struct(&STU_CHK__CITIES[city_idx], &_CITIES[city_idx]);
    cities_data_captured = STU_TRUE;
}

void Check_Cities_Data(void)
{
    int16_t city_idx = 0;
    if(!cities_data_captured)
        return;
    for(city_idx = 0; city_idx < NUM_CITIES; city_idx++)
        if(!Check_City_Struct(&STU_CHK__CITIES[city_idx], &_CITIES[city_idx]))
            STU_DEBUG_BREAK();
}

void Release_Cities_Data(void)
{
    cities_data_captured = STU_FALSE;
}

void Copy_Unit_Struct(struct s_UNIT * dst, struct s_UNIT * src)
{
    int16_t itr;
    dst->wx = src->wx;
    dst->wy = src->wy;
    dst->wp = src->wp;
    dst->owner_idx = src->owner_idx;
    dst->moves2_max = src->moves2_max;
    dst->type = src->type;
    dst->Hero_Slot = src->Hero_Slot;
    dst->Finished = src->Finished;
    dst->moves2 = src->moves2;
    dst->dst_wx = src->dst_wx;
    dst->dst_wy = src->dst_wy;
    dst->Status = src->Status;
    dst->Level = src->Level;
    /* 0D */  /* int8_t  Unused_0Dh; */  /* ¿ 2-byte alignment padding ? */ /* CLUE: here is odd and next one is 2-byte value */
    dst->XP = src->XP;
    dst->Move_Failed = src->Move_Failed;
    dst->Damage = src->Damage;
    dst->Draw_Priority = src->Draw_Priority;
    /* 13 */  /* int8_t  Unknown_13h; */  /* ¿ 2-byte alignment padding ? */ /* CLUE: here is odd and next one is 2-byte value */
    dst->in_tower = src->in_tower;
    dst->Sight_Range = src->Sight_Range;
    dst->mutations = src->mutations;
    dst->enchantments = src->enchantments;
    dst->Rd_Constr_Left = src->Rd_Constr_Left;  // 1-byte, signed;  turns to build  ~ movement points;  Move_Units() sets movepath_cost_array[0]
    dst->Rd_From_X = src->Rd_From_X;  // wx?
    dst->Rd_From_Y = src->Rd_From_Y;  // wy?
    /* 1F */  /* int8_t  Unknown_1Fh; */  /* ¿ 2-byte alignment padding ? */
}

int16_t Check_Unit_Struct(struct s_UNIT * dst, struct s_UNIT * src)
{
    int16_t itr;
    if(dst->wx != src->wx)
        STU_DEBUG_BREAK();
    if(dst->wy != src->wy)
        STU_DEBUG_BREAK();
    if(dst->wp != src->wp)
        STU_DEBUG_BREAK();
    if(dst->owner_idx != src->owner_idx)
        STU_DEBUG_BREAK();
    if(dst->moves2_max != src->moves2_max)
        STU_DEBUG_BREAK();
    if(dst->type != src->type)
        STU_DEBUG_BREAK();
    if(dst->Hero_Slot != src->Hero_Slot)
        STU_DEBUG_BREAK();
    if(dst->Finished != src->Finished)
        STU_DEBUG_BREAK();
    if(dst->moves2 != src->moves2)
        STU_DEBUG_BREAK();
    if(dst->dst_wx != src->dst_wx)
        STU_DEBUG_BREAK();
    if(dst->dst_wy != src->dst_wy)
        STU_DEBUG_BREAK();
    if(dst->Status != src->Status)
        STU_DEBUG_BREAK();
    if(dst->Level != src->Level)
        STU_DEBUG_BREAK();
    /* 0D */  /* int8_t  Unused_0Dh; */  /* ¿ 2-byte alignment padding ? */ /* CLUE: here is odd and next one is 2-byte value */
    if(dst->XP != src->XP)
        STU_DEBUG_BREAK();
    if(dst->Move_Failed != src->Move_Failed)
        STU_DEBUG_BREAK();
    if(dst->Damage != src->Damage)
        STU_DEBUG_BREAK();
    if(dst->Draw_Priority != src->Draw_Priority)
        STU_DEBUG_BREAK();
    /* 13 */  /* int8_t  Unknown_13h; */  /* ¿ 2-byte alignment padding ? */ /* CLUE: here is odd and next one is 2-byte value */
    if(dst->in_tower != src->in_tower)
        STU_DEBUG_BREAK();
    if(dst->Sight_Range != src->Sight_Range)
        STU_DEBUG_BREAK();
    if(dst->mutations != src->mutations)
        STU_DEBUG_BREAK();
    if(dst->enchantments != src->enchantments)
        STU_DEBUG_BREAK();
    if(dst->Rd_Constr_Left != src->Rd_Constr_Left)
        STU_DEBUG_BREAK();
    if(dst->Rd_From_X != src->Rd_From_X)
        STU_DEBUG_BREAK();
    if(dst->Rd_From_Y != src->Rd_From_Y)
        STU_DEBUG_BREAK();
    /* 1F */  /* int8_t  Unknown_1Fh; */  /* ¿ 2-byte alignment padding ? */
    return STU_TRUE;
}

void Capture_Units_Data(void)
{
    int16_t unit_idx = 0;
    for(unit_idx = 0; unit_idx < _units; unit_idx++)
        Copy_Unit_Struct(&STU_CHK__UNITS[unit_idx], &_UNITS[unit_idx]);
    units_data_captured = STU_TRUE;
}

void Check_Units_Data(void)
{
    int16_t unit_idx = 0;
    if(!units_data_captured)
        return;
    for(unit_idx = 0; unit_idx < _units; unit_idx++)
        if(!Check_Unit_Struct(&STU_CHK__UNITS[unit_idx], &_UNITS[unit_idx]))
            STU_DEBUG_BREAK();
}

void Release_Units_Data(void)
{
    units_data_captured = STU_FALSE;
}

void Capture_Game_Data(void)
{
    int16_t x = 0;
    int16_t y = 0;
    int16_t p = 0;

    for(p = 0; p < NUM_PLANES; p++)
    {
        for(y = 0; y < WORLD_HEIGHT; y++)
        {
            for(x = 0; x < WORLD_WIDTH; x++)
            {
                STU_CHK__world_maps[((p * WORLD_SIZE) + (y * WORLD_HEIGHT) + x)] = _world_maps[((p * WORLD_SIZE) + (y * WORLD_HEIGHT) + x)];
            }
        }
    }

    game_data_captured = STU_TRUE;

}

void Check_Game_Data(void)
{
    int16_t x = 0;
    int16_t y = 0;
    int16_t p = 0;
    if(!game_data_captured)
        return;
    for(p = 0; p < NUM_PLANES; p++) {
        for(y = 0; y < WORLD_HEIGHT; y++) {
            for(x = 0; x < WORLD_WIDTH; x++) {
                if(_world_maps[((p * WORLD_SIZE) + (y * WORLD_HEIGHT) + x)] != STU_CHK__world_maps[((p * WORLD_SIZE) + (y * WORLD_HEIGHT) + x)]) {
                    STU_DEBUG_BREAK();
                }
            }
        }
    }
}

void Release_Game_Data(void)
{
    game_data_captured = STU_FALSE;
}

// void Sanity_Check_Game_Data(void)
// {
//     int16_t itr_units;
// 
//     for(itr_units = 0; itr_units < _units; itr_units++)
//     {
//         if(
//             (_UNITS[itr_units].wp != 0)
//             &&
//             (_UNITS[itr_units].wp != 1)
//         )
//         {
// 
//             STU_DEBUG_BREAK();
// 
//         }
// 
//     }
// 
// }
