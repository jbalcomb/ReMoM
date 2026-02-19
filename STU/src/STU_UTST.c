
#include "STU_UTST.h"

#include "../../MoX/src/MOX_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_BITS.h"  /* SET_2B_OFS() */
#include "../../MoX/src/MOX_DEF.h"  /* SET_TERRAIN_TYPE() */
#include "../../MoX/src/MOX_TYPE.h"

/**
 * @brief Set the terrain type at a specific world map tile.
 *
 * Writes terrain_type into _world_maps at the tile addressed by
 * world coordinates (wx, wy) on world plane wp.
 *
 * @param wx World X coordinate.
 * @param wy World Y coordinate.
 * @param wp World plane index.
 * @param terrain_type Terrain type value to assign.
 */
void Set_Terrain(int16_t wx, int16_t wy, int16_t wp, int16_t terrain_type)
{
    // _world_maps[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] = (uint8_t)terrain_type;
    // MOX_DEF.h #define SET_TERRAIN_TYPE(_wx_, _wy_, _wp_, _terrain_type_)  ( SET_2B_OFS(_world_maps, (((_wp_) * WORLD_SIZE * 2) + ((_wy_) * WORLD_WIDTH * 2) + ((_wx_) * 2)), (_terrain_type_)) )
    SET_TERRAIN_TYPE(wx, wy, wp, terrain_type);
}

/**
 * @brief Set every tile on a world plane to the same terrain type.
 *
 * Iterates over all map coordinates on the selected plane and writes
 * terrain_type into _world_maps via Set_Terrain().
 *
 * @param wp World plane index to modify.
 * @param terrain_type Terrain type value assigned to all tiles.
 */
void Set_Terrain_All(int16_t wp, int16_t terrain_type)
{
    for (int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for (int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            Set_Terrain(wx, wy, wp, terrain_type);
        }
    }
}
