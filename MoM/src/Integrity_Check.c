/*
    Integrity_Check - runtime integrity checkpoints.  See Integrity_Check.h.

    NOT original game code.
*/

#include "../../MoX/src/Allocate_Pool.h"
#include "../../MoX/src/MOM_DAT.h"   /* _map_plane, _active_world_x/_y, _units, _world_maps */
#include "../../MoX/src/MOM_DEF.h"   /* NUM_PLANES, WORLD_WIDTH, WORLD_HEIGHT, WORLD_SIZE */
#include "../../MoX/src/MOX_DEF.h"   /* MAX_UNIT_COUNT */
#include "../../MoX/src/MOX_BITS.h"  /* GET_2B_OFS */
#include "../../STU/src/STU_LOG.h"

#include "Integrity_Check.h"
#include "TerrType.h"                 /* TerType_Count */

#include <stddef.h>



#define INTEGRITY_MAX_GUARD_REPORTS 16

/* Per-check state: non-zero while that check is failing, so a failure is logged when it appears and when it clears. */
static int16_t integrity_map_plane_bad = 0;
static int16_t integrity_active_square_bad = 0;
static int16_t integrity_units_bad = 0;
static int16_t integrity_world_maps_bad = 0;



/* Returns non-zero when the check changed state since the last checkpoint, i.e. when it should be logged. */
static int16_t Integrity_State_Changed(int16_t * failing_flag, int16_t failing_now)
{
    int16_t changed = 0;
    changed = (((failing_now != 0) ? 1 : 0) != ((*failing_flag != 0) ? 1 : 0)) ? 1 : 0;
    *failing_flag = failing_now;
    return changed;
}


static int16_t Integrity_Check_Pool_Guards(const char * where)
{
    struct s_POOL_GUARD_DAMAGE damage[INTEGRITY_MAX_GUARD_REPORTS];
    int damage_count = 0;
    int itr = 0;
    int reported = 0;

    damage_count = Pool_Check_Guards(damage, INTEGRITY_MAX_GUARD_REPORTS);
    if(damage_count == 0)
    {
        return 0;
    }

    reported = (damage_count < INTEGRITY_MAX_GUARD_REPORTS) ? damage_count : INTEGRITY_MAX_GUARD_REPORTS;
    for(itr = 0; itr < reported; itr++)
    {
        if(damage[itr].arena_idx == POOL_GUARD_REGION_LEADING)
        {
            LOG_ERROR(LOG_CAT_INTEGRITY, "[INTEGRITY] %s: pool leading guard written (underrun off the first arena): first changed byte at guard +%u, %u bytes changed", where, damage[itr].first_bad_offset, damage[itr].bad_byte_count);
        }
        else if(damage[itr].arena_idx == POOL_GUARD_REGION_TRAILING)
        {
            LOG_ERROR(LOG_CAT_INTEGRITY, "[INTEGRITY] %s: pool trailing margin written (overrun off the last arena): first changed byte at margin +%u, %u bytes changed", where, damage[itr].first_bad_offset, damage[itr].bad_byte_count);
        }
        else
        {
            LOG_ERROR(LOG_CAT_INTEGRITY, "[INTEGRITY] %s: pool arena #%d overran its end -- allocated at %s:%d, %u bytes at %p; first changed byte %u past the end, %u guard bytes changed", where, damage[itr].arena_idx, (damage[itr].site_file != NULL) ? damage[itr].site_file : "(unknown)", damage[itr].site_line, damage[itr].arena_bytes, (void *)damage[itr].arena_base, damage[itr].first_bad_offset, damage[itr].bad_byte_count);
            if(damage[itr].label[0] != '\0')
            {
                LOG_ERROR(LOG_CAT_INTEGRITY, "[INTEGRITY] %s: pool arena #%d holds LBX \"%s\" entry %d", where, damage[itr].arena_idx, damage[itr].label, damage[itr].label_num);
            }
        }
    }
    if(damage_count > reported)
    {
        LOG_ERROR(LOG_CAT_INTEGRITY, "[INTEGRITY] %s: %d more damaged pool guards not listed", where, (damage_count - reported));
    }

    /* Each overrun is reported once; later checkpoints report only new damage. */
    Pool_Reset_Guards();

    return (int16_t)damage_count;
}


static int16_t Integrity_Count_Bad_World_Map_Squares(int16_t * first_bad_wx, int16_t * first_bad_wy, int16_t * first_bad_wp, int16_t * first_bad_value)
{
    int16_t bad_count = 0;
    int16_t itr_wp = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;
    int16_t terrain_type = 0;

    if(_world_maps == NULL)
    {
        return 0;
    }

    for(itr_wp = 0; itr_wp < NUM_PLANES; itr_wp++)
    {
        for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy++)
        {
            for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
            {
                terrain_type = (int16_t)GET_2B_OFS(_world_maps, ((itr_wp * WORLD_SIZE * 2) + (itr_wy * WORLD_WIDTH * 2) + (itr_wx * 2)));
                if((terrain_type < 0) || (terrain_type > TerType_Count))
                {
                    if(bad_count == 0)
                    {
                        *first_bad_wx = itr_wx;
                        *first_bad_wy = itr_wy;
                        *first_bad_wp = itr_wp;
                        *first_bad_value = terrain_type;
                    }
                    bad_count++;
                }
            }
        }
    }

    return bad_count;
}


int16_t Integrity_Checkpoint(const char * where)
{
    int16_t failing = 0;
    int16_t failing_now = 0;
    int16_t bad_squares = 0;
    int16_t first_bad_wx = 0;
    int16_t first_bad_wy = 0;
    int16_t first_bad_wp = 0;
    int16_t first_bad_value = 0;

    if(where == NULL)
    {
        where = "(unnamed checkpoint)";
    }

    failing_now = ((_map_plane < 0) || (_map_plane >= NUM_PLANES)) ? 1 : 0;
    if(Integrity_State_Changed(&integrity_map_plane_bad, failing_now) != 0)
    {
        LOG_ERROR(LOG_CAT_INTEGRITY, "[INTEGRITY] %s: %s_map_plane = %d, expected 0..%d", where, (failing_now != 0) ? "" : "cleared -- ", _map_plane, (NUM_PLANES - 1));
    }
    failing += failing_now;

    failing_now = ((_active_world_x < 0) || (_active_world_x >= WORLD_WIDTH) || (_active_world_y < 0) || (_active_world_y >= WORLD_HEIGHT)) ? 1 : 0;
    if(Integrity_State_Changed(&integrity_active_square_bad, failing_now) != 0)
    {
        LOG_ERROR(LOG_CAT_INTEGRITY, "[INTEGRITY] %s: %s_active_world = (%d,%d), expected (0..%d, 0..%d)", where, (failing_now != 0) ? "" : "cleared -- ", _active_world_x, _active_world_y, (WORLD_WIDTH - 1), (WORLD_HEIGHT - 1));
    }
    failing += failing_now;

    failing_now = ((_units < 0) || (_units > MAX_UNIT_COUNT)) ? 1 : 0;
    if(Integrity_State_Changed(&integrity_units_bad, failing_now) != 0)
    {
        LOG_ERROR(LOG_CAT_INTEGRITY, "[INTEGRITY] %s: %s_units = %d, expected 0..%d", where, (failing_now != 0) ? "" : "cleared -- ", _units, MAX_UNIT_COUNT);
    }
    failing += failing_now;

    bad_squares = Integrity_Count_Bad_World_Map_Squares(&first_bad_wx, &first_bad_wy, &first_bad_wp, &first_bad_value);
    failing_now = (bad_squares > 0) ? 1 : 0;
    if(Integrity_State_Changed(&integrity_world_maps_bad, failing_now) != 0)
    {
        LOG_ERROR(LOG_CAT_INTEGRITY, "[INTEGRITY] %s: %s_world_maps has %d squares with a terrain index outside 0..%d; first at (%d,%d,%d) = %d", where, (failing_now != 0) ? "" : "cleared -- ", bad_squares, TerType_Count, first_bad_wx, first_bad_wy, first_bad_wp, first_bad_value);
    }
    failing += failing_now;

    failing += Integrity_Check_Pool_Guards(where);

    return failing;
}


int16_t Integrity_Terrain_Square_Is_Valid(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t terrain_type = 0;

    if((wp < 0) || (wp >= NUM_PLANES) || (wx < 0) || (wx >= WORLD_WIDTH) || (wy < 0) || (wy >= WORLD_HEIGHT) || (_world_maps == NULL))
    {
        return ST_FALSE;
    }

    terrain_type = (int16_t)GET_2B_OFS(_world_maps, ((wp * WORLD_SIZE * 2) + (wy * WORLD_WIDTH * 2) + (wx * 2)));

    return ((terrain_type >= 0) && (terrain_type <= TerType_Count)) ? ST_TRUE : ST_FALSE;
}


void Integrity_Report_Refused_Squares(const char * where, int16_t * refusing_state, int16_t refused_count, int16_t first_wx, int16_t first_wy, int16_t wp)
{
    int16_t refusing_now = 0;

    refusing_now = (refused_count > 0) ? 1 : 0;
    if(Integrity_State_Changed(refusing_state, refusing_now) != 0)
    {
        if(refusing_now != 0)
        {
            LOG_ERROR(LOG_CAT_INTEGRITY, "[INTEGRITY] %s: refused to draw %d squares with an invalid plane/square/terrain index (wp = %d; first at (%d,%d)) -- drawn as unexplored instead of reading outside the map data", where, refused_count, wp, first_wx, first_wy);
        }
        else
        {
            LOG_ERROR(LOG_CAT_INTEGRITY, "[INTEGRITY] %s: cleared -- drawing every square again (wp = %d)", where, wp);
        }
    }
}
