/*

    Validate Game-Data Data-Points

*/

#include "STU_DBG.h"
#include "STU_TYPE.h"

#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"      /* GET_TERRAIN_TYPE() */
#include "../../MoM/src/TerrType.h"

#include "STU_VLD.h"

#include <stdio.h>



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

/*
should be no terrain type 1 - Landmass/Land?   AKA tt_BugGrass
*/
void Validate_Terrain_Types(void)
{
    int16_t wx = 0;
    int16_t wy = 0;
    int16_t wp = 0;
    int16_t terrain_type = 0;
    for(wp = 0; wp < NUM_PLANES; wp++)
    {
        for(wy = 0; wy < WORLD_HEIGHT; wy++)
        {
            for(wx = 0; wx < WORLD_WIDTH; wx++)
            {
                terrain_type = p_world_map[wp][wy][wx];
                if(terrain_type == tt_BugGrass)
                {
                    STU_DEBUG_BREAK();
                }
            }
        }
    }
}

static int16_t Validate_Lair_Fail_Range(int16_t lair_idx, const char * field_name, int32_t value, int32_t min_value, int32_t max_value)
{
    fprintf(stderr, "ERROR: Validate_Lair_Record(): lair_idx=%d has invalid %s=%ld (expected [%ld, %ld])\n",
        lair_idx,
        field_name,
        (long)value,
        (long)min_value,
        (long)max_value);

    return ST_FALSE;
}

static int16_t Validate_Lair_Fail_Message(int16_t lair_idx, const char * message)
{
    fprintf(stderr, "ERROR: Validate_Lair_Record(): lair_idx=%d %s\n", lair_idx, message);

    return ST_FALSE;
}

int16_t Validate_Lair_Record(int16_t lair_idx)
{
    const struct s_LAIR * lair = NULL;
    int16_t item_idx = 0;

    if(_LAIRS == NULL)
    {
        fprintf(stderr, "ERROR: Validate_Lair_Record(): _LAIRS is NULL\n");
        return ST_FALSE;
    }

    if((lair_idx < 0) || (lair_idx >= NUM_LAIRS))
    {
        fprintf(stderr, "ERROR: Validate_Lair_Record(): lair_idx=%d out of range [0, %d)\n", lair_idx, NUM_LAIRS);
        return ST_FALSE;
    }

    lair = &_LAIRS[lair_idx];

    if(lair->intact != ST_TRUE)
    {
        return ST_TRUE;
    }

    if((lair->wx < 0) || (lair->wx >= WORLD_WIDTH))
    {
        return Validate_Lair_Fail_Range(lair_idx, "wx", lair->wx, 0, (WORLD_WIDTH - 1));
    }

    if((lair->wy < 0) || (lair->wy >= WORLD_HEIGHT))
    {
        return Validate_Lair_Fail_Range(lair_idx, "wy", lair->wy, 0, (WORLD_HEIGHT - 1));
    }

    if((lair->wp < 0) || (lair->wp >= NUM_PLANES))
    {
        return Validate_Lair_Fail_Range(lair_idx, "wp", lair->wp, 0, (NUM_PLANES - 1));
    }

    if((lair->type < lt_NONE) || (lair->type > lt_Fallen_Temple))
    {
        return Validate_Lair_Fail_Range(lair_idx, "type", lair->type, lt_NONE, lt_Fallen_Temple);
    }

    if(lair->guard1_count > 9)
    {
        return Validate_Lair_Fail_Range(lair_idx, "guard1_count", lair->guard1_count, 0, 9);
    }

    if(lair->guard2_count > 9)
    {
        return Validate_Lair_Fail_Range(lair_idx, "guard2_count", lair->guard2_count, 0, 9);
    }

    if((lair->guard1_count + lair->guard2_count) > 9)
    {
        fprintf(stderr,
            "ERROR: Validate_Lair_Record(): lair_idx=%d guard count overflow (%u + %u > 9)\n",
            lair_idx,
            (unsigned int)lair->guard1_count,
            (unsigned int)lair->guard2_count);
        return ST_FALSE;
    }

    if((lair->guard1_unit_type == 0) && (lair->guard1_count != 0))
    {
        return Validate_Lair_Fail_Message(lair_idx, "guard1_count must be 0 when guard1_unit_type is 0");
    }

    if((lair->guard2_unit_type == 0) && (lair->guard2_count != 0))
    {
        return Validate_Lair_Fail_Message(lair_idx, "guard2_count must be 0 when guard2_unit_type is 0");
    }

    if(lair->Loot_Gold < 0)
    {
        return Validate_Lair_Fail_Range(lair_idx, "Loot_Gold", lair->Loot_Gold, 0, 32767);
    }

    if(lair->Loot_Mana < 0)
    {
        return Validate_Lair_Fail_Range(lair_idx, "Loot_Mana", lair->Loot_Mana, 0, 32767);
    }

    if((lair->Spell_n_Special < 0) || (lair->Spell_n_Special > 6))
    {
        return Validate_Lair_Fail_Range(lair_idx, "Spell_n_Special", lair->Spell_n_Special, 0, 6);
    }

    if((lair->Item_Count < 0) || (lair->Item_Count > 3))
    {
        return Validate_Lair_Fail_Range(lair_idx, "Item_Count", lair->Item_Count, 0, 3);
    }

    for(item_idx = 0; item_idx < lair->Item_Count; item_idx++)
    {
        if(lair->Item_Values[item_idx] <= 0)
        {
            fprintf(stderr,
                "ERROR: Validate_Lair_Record(): lair_idx=%d has non-positive Item_Values[%d]=%d while Item_Count=%d\n",
                lair_idx,
                item_idx,
                lair->Item_Values[item_idx],
                lair->Item_Count);
            return ST_FALSE;
        }
    }

    return ST_TRUE;
}

int16_t Validate_All_Lairs(void)
{
    int16_t lair_idx = 0;

    if(_LAIRS == NULL)
    {
        fprintf(stderr, "ERROR: Validate_All_Lairs(): _LAIRS is NULL\n");
        return ST_FALSE;
    }

    for(lair_idx = 0; lair_idx < NUM_LAIRS; lair_idx++)
    {
        if(Validate_Lair_Record(lair_idx) != ST_TRUE)
        {
            fprintf(stderr, "ERROR: Validate_All_Lairs(): failed at lair_idx=%d\n", lair_idx);
            return ST_FALSE;
        }
    }

    return ST_TRUE;
}
