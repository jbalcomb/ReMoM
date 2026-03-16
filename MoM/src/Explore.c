/*
    WIZARDS.EXE
    ovr092
*/

#include "Explore.h"

/* Copilot TEMP DEBUG: targeted scout/reveal tracing */
#include "../../STU/src/STU_DBG.h"

#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/Util.h"

#ifdef STU_DEBUG
/* Copilot TEMP DEBUG: set DBG_TRACE_SCOUT_WX/WY/WP to filter a single tile; leave ST_UNDEFINED for all. */
static int16_t DBG_TRACE_SCOUT_WX = ST_UNDEFINED;
static int16_t DBG_TRACE_SCOUT_WY = ST_UNDEFINED;
static int16_t DBG_TRACE_SCOUT_WP = ST_UNDEFINED;
static int16_t DBG_TRACE_SCOUT_BUDGET = 160;

static int16_t DBG_Trace_Scout_Match(int16_t wx, int16_t wy, int16_t wp)
{
    if((DBG_TRACE_SCOUT_WP != ST_UNDEFINED) && (DBG_TRACE_SCOUT_WP != wp))
    {
        return ST_FALSE;
    }

    if((DBG_TRACE_SCOUT_WX != ST_UNDEFINED) && (DBG_TRACE_SCOUT_WX != wx))
    {
        return ST_FALSE;
    }

    if((DBG_TRACE_SCOUT_WY != ST_UNDEFINED) && (DBG_TRACE_SCOUT_WY != wy))
    {
        return ST_FALSE;
    }

    return ST_TRUE;
}
#endif



// WZD o92p01
// drake178: TILE_VisibilityClear
void Clear_Square_Scouted_Flags(int16_t world_p)
{
    int16_t bit_field_size;
    int16_t itr_bit_field;

    bit_field_size = WORLD_SIZE / 8;  // bits per byte / bytes per data type size

    if(world_p == 0)
    {
        for(itr_bit_field = 0; itr_bit_field < bit_field_size; itr_bit_field++)
        {
            square_scouted_p0[itr_bit_field] = 0;
        }
    }
    else
    {
        for(itr_bit_field = 0; itr_bit_field < bit_field_size; itr_bit_field++)
        {
            square_scouted_p1[itr_bit_field] = 0;
        }
    }
}

// WZD o92p02
/* COPILOT */
/**
 * @brief Returns whether a world square is currently scouted on the specified plane.
 *
 * @details
 * A square is considered "scouted" only when both of the following are true:
 * - its transient visibility bit is set in the plane-specific `square_scouted_p0` or
 *   `square_scouted_p1` bitfield, and
 * - the square is also marked explored via `Check_Square_Explored()`.
 *
 * This means the function does not test raw current-visibility state alone; it enforces
 * the game rule that an unexplored square cannot be treated as actively scouted.
 *
 * Implementation notes:
 * - Computes a linear tile index with `world_y * WORLD_WIDTH + world_x`.
 * - Selects the scouting bitfield based on `world_p`.
 * - Combines the bitfield result with explored-state validation before returning.
 *
 * Related functions:
 * - `Set_Square_Scouted()` sets the current-plane scouting bit.
 * - `Clear_Square_Scouted()` clears the current-plane scouting bit.
 * - `Check_Square_Explored()` validates persistent exploration state.
 *
 * @param world_x World-space X coordinate of the square to test.
 * @param world_y World-space Y coordinate of the square to test.
 * @param world_p World plane index: `0` selects `square_scouted_p0`, any other value
 *                selects `square_scouted_p1`.
 *
 * @return int16_t
 * Returns `ST_TRUE` if the square is both currently scouted and explored; otherwise
 * returns `ST_FALSE`.
 *
 * @note This function assumes the caller passes valid world coordinates and plane values;
 *       it performs no bounds checking.
 */
int16_t Check_Square_Scouted(int16_t world_x, int16_t world_y, int16_t world_p)
{
    int16_t world_square_idx = 0;
    int16_t square_scouted_flag = 0;
    int16_t square_explored_flag = 0;
    int16_t square_is_scouted = 0;
    square_is_scouted = ST_FALSE;
    world_square_idx = world_y * WORLD_WIDTH + world_x;
    if(world_p == 0)
    {
        square_scouted_flag = Test_Bit_Field(world_square_idx, square_scouted_p0);
    }
    else
    {
        square_scouted_flag = Test_Bit_Field(world_square_idx, square_scouted_p1);
    }
    square_explored_flag = Check_Square_Explored(world_x, world_y, world_p);
    if(square_scouted_flag == ST_TRUE && square_explored_flag == ST_TRUE)
    {
        square_is_scouted = ST_TRUE;
    }
    return square_is_scouted;
}

// WZD o92p03
/* COPILOT */
/**
 * @brief Marks a world square as currently scouted in the plane-specific visibility bitfield.
 *
 * @details
 * This function sets the transient "currently visible/scouted" bit for a single square
 * on the requested world plane. It does not mark the tile as explored; persistent map
 * discovery is handled separately by the explored-state system.
 *
 * Implementation behavior:
 * - Computes the linear square index as `(world_y * WORLD_WIDTH) + world_x`.
 * - Derives byte/bit addressing information for the corresponding bitfield entry.
 * - Selects the destination bitfield by plane:
 *   - `world_p == 0` uses `square_scouted_p0`
 *   - any other value uses `square_scouted_p1`
 * - Calls `Set_Bit_Field()` to perform the actual write.
 *
 * The local variables that derive `byte_idx`, `byte_bit_idx`, and the merged byte value
 * mirror the original logic and are useful for debugging/verification, even though the
 * final write is delegated to `Set_Bit_Field()`.
 *
 * Related functions:
 * - `Check_Square_Scouted()` tests whether a square is both scouted and explored.
 * - `UU_Clear_Square_Scouted()` removes the transient scouted bit.
 * - `Set_Square_Scouted_Flags()` applies this function across a sight-radius area.
 *
 * @param world_x World-space X coordinate of the square to mark.
 * @param world_y World-space Y coordinate of the square to mark.
 * @param world_p World plane index selecting the destination scouting bitfield.
 *
 * @return void
 * No return value. Updates global scouting state in place.
 *
 * @note This function assumes valid coordinates and plane values; it performs no bounds
 *       validation before computing the linear bit index.
 */
void Set_Square_Scouted(int16_t world_x, int16_t world_y, int16_t world_p)
{
    int16_t world_square_idx = 0;
    int16_t bit_idx = 0;
    uint8_t * bit_field = 0;
    int16_t byte_idx = 0;
    uint16_t bit_field_byte = 0;
    int16_t byte_bit_idx = 0;
    uint16_t bit_field_test_bit = 0;
    uint16_t new_bit_field_byte = 0;
    world_square_idx = (world_y * WORLD_WIDTH) + world_x;
    bit_idx = world_square_idx;
    bit_field = square_scouted_p0;
    byte_idx = (bit_idx >> 3);
    bit_field_byte = (uint16_t)bit_field[byte_idx];
    byte_bit_idx = (bit_idx & 0x07);
    // bit_field[byte_idx] = (bit_field_byte | bit_field_test_bits[byte_bit_idx]);
    bit_field_test_bit = bit_field_test_bits[byte_bit_idx];
    new_bit_field_byte = (bit_field_byte | bit_field_test_bits[byte_bit_idx]);
    if(world_p == 0)
    {
        Set_Bit_Field(world_square_idx, square_scouted_p0);
    }
    else
    {
        Set_Bit_Field(world_square_idx, square_scouted_p1);
    }
}

// WZD o92p04
/* COPILOT */
/**
 * @brief Clears the transient scouted/visible flag for a single world square.
 *
 * @details
 * This function removes the current-turn visibility bit for one square from the
 * plane-specific scouting bitfield. It affects only transient scouting state and does
 * not modify persistent exploration data.
 *
 * Implementation behavior:
 * - Computes the linear world-square index as `(world_y * WORLD_WIDTH) + world_x`.
 * - Selects the target bitfield by plane:
 *   - `world_p == 0` clears from `square_scouted_p0`
 *   - any other value clears from `square_scouted_p1`
 * - Delegates the actual bit removal to `Clear_Bit_Field()`.
 *
 * This routine is the inverse of `Set_Square_Scouted()` and is typically used as part
 * of visibility refresh logic that rebuilds current scouting each update cycle.
 *
 * Related functions:
 * - `Set_Square_Scouted()` sets the transient scouting bit.
 * - `Clear_Square_Scouted_Flags()` clears an entire plane's scouting bitfield.
 * - `Check_Square_Scouted()` tests the final scouted state together with explored state.
 *
 * @param world_x World-space X coordinate of the square to clear.
 * @param world_y World-space Y coordinate of the square to clear.
 * @param world_p World plane index selecting which scouting bitfield to update.
 *
 * @return void
 * No return value. Updates global scouting visibility state in place.
 *
 * @note This function assumes valid coordinates and plane values; it performs no
 *       bounds checking before computing the bit index.
 */
void Clear_Square_Scouted(int16_t world_x, int16_t world_y, int16_t world_p)
{
    int16_t world_square_idx;
    world_square_idx = (world_y * WORLD_WIDTH) + world_x;
    if(world_p == 0)
    {
        Clear_Bit_Field(world_square_idx, square_scouted_p0);
    }
    else
    {
        Clear_Bit_Field(world_square_idx, square_scouted_p1);
    }
}

// WZD o92p05
// drake178: TILE_VisibilityReset
/*
*/
/*

*/
void Update_Scouted_And_Contacted__WIP(void)
{
    int16_t itr_planes;
    int16_t itr_units;
    int16_t itr_cities;
    int16_t curr_world_p;
    int16_t scout_level;

    for(itr_planes = 0; itr_planes < NUM_PLANES; itr_planes++)
    {

        curr_world_p = itr_planes;

        Clear_Square_Scouted_Flags(itr_planes);  // ¿ MoO2  Clear_Planet_Explored_Flags_() ?

        if(_players[_human_player_idx].Globals[NATURES_AWARENESS] == ST_FALSE)
        {
            for(itr_units = 0; itr_units < _units; itr_units++)
            {

                if( (_UNITS[itr_units].owner_idx == _human_player_idx) && ((_UNITS[itr_units].wp == curr_world_p) || (_UNITS[itr_units].in_tower == ST_TRUE)) )
                {
                    scout_level = _UNITS[itr_units].Sight_Range;
                    // TODO  UNIT_IsFlying
                    // TODO  UNIT_HasWindWalking
                    // TODO  UNIT_ReturnZero
                    // TODO  UNIT_HasItemFlight
                    // if scout_level < 2 scout_level = 2
                    if(_UNITS[itr_units].Status == us_Patrol)
                    {
                        scout_level++;
                    }

                    Set_Square_Scouted_Flags(_UNITS[itr_units].wx, _UNITS[itr_units].wy, curr_world_p, scout_level);

                }
                
            }

            for(itr_cities = 0; itr_cities < _cities; itr_cities++)
            {
                if( (_CITIES[itr_cities].owner_idx == _human_player_idx) && ( (_CITIES[itr_cities].wp == curr_world_p) || (_CITIES[itr_cities].wp == 2) ) )
                {
                    scout_level = 2;

                    if(_CITIES[itr_cities].bldg_status[CITY_WALLS] != bs_NotBuilt)
                    {
                        scout_level = 3;
                    }
                    if(_CITIES[itr_cities].enchantments[NATURES_EYE] != ST_FALSE)
                    {
                        scout_level = 5;
                    }
                    // BUG: Overrides Nature's Eye
                    if(_CITIES[itr_cities].bldg_status[ORACLE] != bs_NotBuilt)
                    {
                        scout_level = 4;
                    }

                    Set_Square_Scouted_Flags(_CITIES[itr_cities].wx, _CITIES[itr_cities].wy, curr_world_p, scout_level);
                }
            }

            for(itr_cities = 0; itr_cities < _cities; itr_cities++)
            {

                if( (_CITIES[itr_cities].owner_idx != _human_player_idx) && (_CITIES[itr_cities].owner_idx != NEUTRAL_PLAYER_IDX) && ( (_CITIES[itr_cities].wp == curr_world_p) || (_CITIES[itr_cities].wp == 2) ) )
                {

                    if(
                        (Check_Square_Scouted(_CITIES[itr_cities].wx, _CITIES[itr_cities].wy, curr_world_p))
                        &&
                        (_CITIES[itr_cities].owner_idx < NEUTRAL_PLAYER_IDX)
                    )
                    {

                        _players[_human_player_idx].Dipl.Contacted[_CITIES[itr_cities].owner_idx] = ST_TRUE;

                        _players[_CITIES[itr_cities].owner_idx].Dipl.Contacted[_human_player_idx] = ST_TRUE;

                    }

                }

            }

            for(itr_units = 0; itr_units < _units; itr_units++)
            {
                // TODO  UNIT_ContactHuman(itr_units, curr_world_p);
            }

        }
        else
        {
            // SPELLY  if(_players[_human_player_idx].Globals.Nature_Awareness != ST_FALSE)
        }


    }

}

// WZD o92p06
// drake178: TILE_ApplySightRange
/*
    within the scout range, sets all explored squares as scouted squares
*/
void Set_Square_Scouted_Flags(int16_t world_x, int16_t world_y, int16_t world_p, int16_t scout_range)
{
    int16_t start_world_x;
    int16_t start_world_y;
    int16_t itr_world_x;
    int16_t itr_world_y;
    int16_t curr_world_x;
    int16_t square_is_explored;
#ifdef STU_DEBUG
    int16_t dbg_do_trace = ST_FALSE;
    int16_t dbg_tiles_marked = 0;
    int16_t dbg_y_end = 0;
    int16_t dbg_x_end = 0;
#endif

    if(scout_range == 0)
    {
        return;
    }

#ifdef STU_DEBUG
        dbg_do_trace = DBG_Trace_Scout_Match(world_x, world_y, world_p);
        dbg_y_end = (world_y + scout_range);
        dbg_x_end = (world_x + scout_range);
        if((dbg_do_trace == ST_TRUE) && (DBG_TRACE_SCOUT_BUDGET > 0))
        {
            DBG_TRACE_SCOUT_BUDGET--;
            dbg_prn("DEBUG: [%s, %d]: BEGIN Set_Square_Scouted_Flags(world_x=%d, world_y=%d, world_p=%d, scout_range=%d, nominal_end_x=%d, nominal_end_y=%d)", __FILE__, __LINE__, world_x, world_y, world_p, scout_range, dbg_x_end, dbg_y_end);
        }
#endif

        start_world_y = world_y - scout_range;

        if(start_world_y < 0)
        {
            start_world_y = 0;
        }

        start_world_x = world_x - scout_range;

        if(start_world_x < 0)
        {
            start_world_x = start_world_x + WORLD_WIDTH;
        }

        scout_range = (scout_range * 2) + 1;
        for (itr_world_y = start_world_y; itr_world_y < start_world_y + scout_range; itr_world_y++)
        {
            if (itr_world_y >= WORLD_HEIGHT)
            {
                continue;
            }
            for (itr_world_x = start_world_x; itr_world_x < start_world_x + scout_range; itr_world_x++)
            {
                if(itr_world_x < WORLD_WIDTH)
                {
                    curr_world_x = itr_world_x;
                }
                else
                {
                    curr_world_x = itr_world_x - WORLD_WIDTH;
                }
                square_is_explored = Check_Square_Explored(curr_world_x, itr_world_y, world_p);

                if(square_is_explored != ST_FALSE)
                {
                    Set_Square_Scouted(curr_world_x, itr_world_y, world_p);

#ifdef STU_DEBUG
                    dbg_tiles_marked++;
                    if((dbg_do_trace == ST_TRUE) && (DBG_TRACE_SCOUT_BUDGET > 0))
                    {
                        DBG_TRACE_SCOUT_BUDGET--;
                        dbg_prn("DEBUG: [%s, %d]: SCOUT set (%d,%d,p=%d) explored=%d", __FILE__, __LINE__, curr_world_x, itr_world_y, world_p, square_is_explored);
                    }
#endif

                }
            }
        }
        
#ifdef STU_DEBUG
        if((dbg_do_trace == ST_TRUE) && (DBG_TRACE_SCOUT_BUDGET > 0))
        {
            DBG_TRACE_SCOUT_BUDGET--;
            dbg_prn("DEBUG: [%s, %d]: END Set_Square_Scouted_Flags(world_x=%d, world_y=%d, world_p=%d, marked=%d)", __FILE__, __LINE__, world_x, world_y, world_p, dbg_tiles_marked);
        }
#endif
    
}

// WZD o92p07
// drake178: UNIT_ContactHuman

// WZD o92p08
int16_t Check_Square_Explored(int16_t wx, int16_t wy, int16_t wp)
{
    uint8_t square_explored_flag;
    int16_t square_is_explored;

    square_is_explored = ST_FALSE;

    square_explored_flag = GET_SQUARE_EXPLORED(wx, wy, wp);

    if(square_explored_flag != UNEXPLORED)  /* if any bits are set */
    {
        square_is_explored = ST_TRUE;
    }

    return square_is_explored;
}
