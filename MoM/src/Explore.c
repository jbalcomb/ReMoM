/*
    WIZARDS.EXE
    ovr092
*/

#include "../../STU/src/STU_DBG.h"

#include "../../MoX/src/MOX_DAT.h"  /* _players[] */
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../../MoX/src/Util.h"

#include "UnitMove.h"

#include "Explore.h"



// WZD o92p01
/**
 * @brief Clears all transient scouted flags for the specified world plane.
 *
 * @details
 * This function resets the plane-specific scouting bitfield used to track which
 * world squares are currently visible/scouted. It does not affect persistent
 * explored-state data; it only clears the transient per-plane visibility flags.
 *
 * Implementation behavior:
 * - Computes the bitfield length as `WORLD_SIZE / 8` bytes.
 * - When `wp == 0`, clears `square_scouted_p0`.
 * - For any other `wp` value, clears `square_scouted_p1`.
 * - Iterates through the entire selected bitfield and writes zero to each byte.
 *
 * This routine is typically used before rebuilding current scouting visibility
 * for a plane during map-visibility refresh.
 *
 * Related functions:
 * - `Set_Square_Scouted()` marks a single square as currently scouted.
 * - `Clear_Square_Scouted()` clears a single square's transient scouted flag.
 * - `Set_Square_Scouted_Flags()` reapplies scouting over an area.
 *
 * @param wp World plane index. A value of `0` selects plane 0; any other value
 *           selects plane 1.
 *
 * @return void
 * No return value. The selected global scouting bitfield is cleared in place.
 *
 * @note This function assumes the plane selection convention used elsewhere in
 *       the codebase, where only plane 0 and plane 1 scouting buffers exist.
 */
void Clear_Square_Scouted_Flags(int16_t wp)
{
    int16_t bit_field_size = 0;
    int16_t itr_bit_field = 0;

    bit_field_size = (WORLD_SIZE / 8);  // bits per byte / bytes per data type size

    if(wp == 0)
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
 * - Computes a linear tile index with `wy * WORLD_WIDTH + wx`.
 * - Selects the scouting bitfield based on `wp`.
 * - Combines the bitfield result with explored-state validation before returning.
 *
 * Related functions:
 * - `Set_Square_Scouted()` sets the current-plane scouting bit.
 * - `Clear_Square_Scouted()` clears the current-plane scouting bit.
 * - `Check_Square_Explored()` validates persistent exploration state.
 *
 * @param wx World-space X coordinate of the square to test.
 * @param wy World-space Y coordinate of the square to test.
 * @param wp World plane index: `0` selects `square_scouted_p0`, any other value
 *                selects `square_scouted_p1`.
 *
 * @return int16_t
 * Returns `ST_TRUE` if the square is both currently scouted and explored; otherwise
 * returns `ST_FALSE`.
 *
 * @note This function assumes the caller passes valid world coordinates and plane values;
 *       it performs no bounds checking.
 */
int16_t Check_Square_Scouted(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t world_square_idx = 0;
    int16_t square_scouted_flag = 0;
    int16_t square_explored_flag = 0;
    int16_t square_is_scouted = 0;
    square_is_scouted = ST_FALSE;
    world_square_idx = ((wy * WORLD_WIDTH) + wx);
    if(wp == 0)
    {
        square_scouted_flag = Test_Bit_Field(world_square_idx, square_scouted_p0);
    }
    else
    {
        square_scouted_flag = Test_Bit_Field(world_square_idx, square_scouted_p1);
    }
    square_explored_flag = Check_Square_Explored(wx, wy, wp);
    if(square_scouted_flag == ST_TRUE && square_explored_flag == ST_TRUE)
    {
        square_is_scouted = ST_TRUE;
    }
    return square_is_scouted;
}

// WZD o92p03
/**
 * @brief Marks a world square as currently scouted in the plane-specific visibility bitfield.
 *
 * @details
 * This function sets the transient "currently visible/scouted" bit for a single square
 * on the requested world plane. It does not mark the tile as explored; persistent map
 * discovery is handled separately by the explored-state system.
 *
 * Implementation behavior:
 * - Computes the linear square index as `(wy * WORLD_WIDTH) + wx`.
 * - Derives byte/bit addressing information for the corresponding bitfield entry.
 * - Selects the destination bitfield by plane:
 *   - `wp == 0` uses `square_scouted_p0`
 *   - any other value uses `square_scouted_p1`
 * - Calls `Set_Bit_Field()` to perform the actual write.
 *
 * The local variables that derive `byte_idx`, `byte_bit_idx`, and the merged byte value
 * mirror the original logic and are useful for debugging/verification, even though the
 * final write is delegated to `Set_Bit_Field()`.
 *
 * Related functions:
 * - `Check_Square_Scouted()` tests whether a square is both scouted and explored.
 * - `Clear_Square_Scouted()` removes the transient scouted bit.
 * - `Set_Square_Scouted_Flags()` applies this function across a sight-radius area.
 *
 * @param wx World-space X coordinate of the square to mark.
 * @param wy World-space Y coordinate of the square to mark.
 * @param wp World plane index selecting the destination scouting bitfield.
 *
 * @return void
 * No return value. Updates global scouting state in place.
 *
 * @note This function assumes valid coordinates and plane values; it performs no bounds
 *       validation before computing the linear bit index.
 */
void Set_Square_Scouted(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t world_square_idx = 0;
    int16_t bit_idx = 0;
    uint8_t * bit_field = 0;
    int16_t byte_idx = 0;
    uint16_t bit_field_byte = 0;
    int16_t byte_bit_idx = 0;
    uint16_t bit_field_test_bit = 0;
    uint16_t new_bit_field_byte = 0;
    world_square_idx = (wy * WORLD_WIDTH) + wx;
    bit_idx = world_square_idx;
    bit_field = square_scouted_p0;
    byte_idx = (bit_idx >> 3);
    bit_field_byte = (uint16_t)bit_field[byte_idx];
    byte_bit_idx = (bit_idx & 0x07);
    // bit_field[byte_idx] = (bit_field_byte | bit_field_test_bits[byte_bit_idx]);
    bit_field_test_bit = bit_field_test_bits[byte_bit_idx];
    new_bit_field_byte = (bit_field_byte | bit_field_test_bits[byte_bit_idx]);
    if(wp == 0)
    {
        Set_Bit_Field(world_square_idx, square_scouted_p0);
    }
    else
    {
        Set_Bit_Field(world_square_idx, square_scouted_p1);
    }
}

// WZD o92p04
/**
 * @brief Clears the transient scouted/visible flag for a single world square.
 *
 * @details
 * This function removes the current-turn visibility bit for one square from the
 * plane-specific scouting bitfield. It affects only transient scouting state and does
 * not modify persistent exploration data.
 *
 * Implementation behavior:
 * - Computes the linear world-square index as `(wy * WORLD_WIDTH) + wx`.
 * - Selects the target bitfield by plane:
 *   - `wp == 0` clears from `square_scouted_p0`
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
 * @param wx World-space X coordinate of the square to clear.
 * @param wy World-space Y coordinate of the square to clear.
 * @param wp World plane index selecting which scouting bitfield to update.
 *
 * @return void
 * No return value. Updates global scouting visibility state in place.
 *
 * @note This function assumes valid coordinates and plane values; it performs no
 *       bounds checking before computing the bit index.
 */
void Clear_Square_Scouted(int16_t wx, int16_t wy, int16_t wp)
{
    int16_t world_square_idx = 0;
    world_square_idx = ((wy * WORLD_WIDTH) + wx);
    if(wp == 0)
    {
        Clear_Bit_Field(world_square_idx, square_scouted_p0);
    }
    else
    {
        Clear_Bit_Field(world_square_idx, square_scouted_p1);
    }
}

// WZD o92p05
/**
 * @brief Rebuilds current scouting visibility and updates diplomatic contact state.
 *
 * @details
 * This routine refreshes the human player's transient scouting map on every
 * world plane and then uses the resulting visibility data to determine whether
 * contact with non-neutral enemy city owners should be established.
 *
 * Processing overview:
 * - Iterates over all world planes.
 * - Clears the plane's transient scouted bitfield with
 *   `Clear_Square_Scouted_Flags()`.
 * - If the human player does not have `NATURES_AWARENESS`, rebuilds scouting
 *   from visible human-owned units and cities.
 * - For human-owned units on the current plane, or units in towers, derives a
 *   scouting radius from `Sight_Range` and increases it by one when the unit is
 *   patrolling.
 * - For human-owned cities on the current plane, or cities with `wp == 2`,
 *   derives scouting radius from city state:
 *   - default radius `2`
 *   - `CITY_WALLS` raises it to `3`
 *   - `NATURES_EYE` raises it to `5`
 *   - `ORACLE` sets it to `4`
 * - Applies the computed radius through `Set_Square_Scouted_Flags()`.
 * - Checks non-human, non-neutral cities on the same plane and marks both sides
 *   as contacted when the city square is currently scouted.
 * - Leaves unit-contact handling as a future `TODO`.
 *
 * This function updates transient visibility only; persistent exploration is
 * still governed by `Check_Square_Explored()` and related map-discovery logic.
 *
 * Related functions:
 * - `Clear_Square_Scouted_Flags()` resets a plane before visibility rebuild.
 * - `Set_Square_Scouted_Flags()` applies scouting over a square area.
 * - `Check_Square_Scouted()` determines whether a target city square is both
 *   explored and currently visible.
 *
 * @return void
 * No return value. Updates global scouting state and diplomatic contact flags
 * in place.
 *
 * @note The branch for `NATURES_AWARENESS` is currently unimplemented, and the
 *       comment in the city logic indicates an existing bug where `ORACLE`
 *       overrides the larger `NATURES_EYE` scouting radius.
 */
void Update_Scouted_And_Contacted(void)
{
    int16_t scout_level = 0;
    int16_t itr_wp = 0;
    int16_t itr_wy = 0;
    int16_t itr_wx = 0;
    int16_t itr_players = 0;
    int16_t itr_units = 0;
    int16_t itr_cities = 0;
    int16_t curr_wp = 0;
    
    for(itr_wp = 0; itr_wp < NUM_PLANES; itr_wp++)
    {
        curr_wp = itr_wp;

        Clear_Square_Scouted_Flags(itr_wp);  // ¿ MoO2  Clear_Planet_Explored_Flags_() ?

        if(_players[_human_player_idx].Globals[NATURES_AWARENESS] != ST_FALSE)
        {

            for(itr_wy = 0; itr_wy < WORLD_HEIGHT; itr_wy++)
            {
                for(itr_wx = 0; itr_wx < WORLD_WIDTH; itr_wx++)
                {
                    if(Check_Square_Explored(itr_wx, itr_wy, curr_wp) == ST_TRUE)
                    {
                        Set_Square_Scouted(itr_wx, itr_wy, curr_wp);
                    }
                }
            }
            // ; mark all players as contacted
            // NOTE(drake178): INCONSISTENT with at least one parent function, which still checks for the visibility of their units (ie. if they are all invisible, contact is not made!)
            for(itr_players = 0; itr_players < _num_players; itr_players++)
            {
                _players[_human_player_idx].Dipl.Contacted[itr_players] = ST_TRUE;
                _players[itr_players].Dipl.Contacted[_human_player_idx] = ST_TRUE;
            }

        }
        else
        {

            for(itr_units = 0; itr_units < _units; itr_units++)
            {
                if(
                    (_UNITS[itr_units].owner_idx == _human_player_idx)
                    &&
                    (
                        (_UNITS[itr_units].wp == curr_wp)
                        ||
                        (_UNITS[itr_units].in_tower == ST_TRUE)
                    )
                )
                {
                    scout_level = _UNITS[itr_units].Sight_Range;
                    if( 
                        (Unit_Has_AirTravel(itr_units) == ST_TRUE) ||
                        (Unit_Has_WindWalking(itr_units) == ST_TRUE) ||
                        /* DONT  (o71p08_Empty_pFxn(itr_units) == ST_TRUE) || */
                        (Unit_Has_AirTravel_Item(itr_units) == ST_TRUE)
                    )
                    {
                        if(scout_level < 2)
                        {
                            scout_level = 2;
                        }
                    }
                    else
                    {
                        if(_UNITS[itr_units].Status == us_Patrol)
                        {
                            scout_level++;
                        }
                    }
                    Set_Square_Scouted_Flags(_UNITS[itr_units].wx, _UNITS[itr_units].wy, curr_wp, scout_level);
                }
            }
            for(itr_cities = 0; itr_cities < _cities; itr_cities++)
            {
                if(
                    (_CITIES[itr_cities].owner_idx == _human_player_idx)
                    &&
                    (
                        (_CITIES[itr_cities].wp == curr_wp)
                        ||
                        (_CITIES[itr_cities].wp == NUM_PLANES)  // BUGBUG  weird and impossible
                    )
                )
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
                    Set_Square_Scouted_Flags(_CITIES[itr_cities].wx, _CITIES[itr_cities].wy, curr_wp, scout_level);
                }
            }
            for(itr_cities = 0; itr_cities < _cities; itr_cities++)
            {
                if(
                    (_CITIES[itr_cities].owner_idx != _human_player_idx)
                    &&
                    (_CITIES[itr_cities].owner_idx != NEUTRAL_PLAYER_IDX)
                    &&
                    (
                        (_CITIES[itr_cities].wp == curr_wp)
                        ||
                        (_CITIES[itr_cities].wp == NUM_PLANES)  // BUGBUG  weird and impossible
                    )
                )
                {
                    if(
                        (Check_Square_Scouted(_CITIES[itr_cities].wx, _CITIES[itr_cities].wy, curr_wp) == ST_TRUE)
                        &&
                        (_CITIES[itr_cities].owner_idx < NEUTRAL_PLAYER_IDX)  // BUGBUG  ¿ leftover from MoO1, where this means *real players*, not monsters ?
                    )
                    {
                        _players[_human_player_idx].Dipl.Contacted[_CITIES[itr_cities].owner_idx] = ST_TRUE;
                        _players[_CITIES[itr_cities].owner_idx].Dipl.Contacted[_human_player_idx] = ST_TRUE;
                    }
                }
            }
            for(itr_units = 0; itr_units < _units; itr_units++)
            {
                Contact_Other_Player(itr_units, curr_wp);
            }

        }

        curr_wp = 1;

    }

}

// WZD o92p06
/**
 * @brief Marks explored squares within a sight radius as currently scouted.
 *
 * @details
 * This function applies transient scouting visibility around a center world
 * square using the supplied scout radius. Only squares that are already marked
 * as explored are eligible to receive the scouted flag.
 *
 * Implementation behavior:
 * - Returns immediately when `scout_range == 0`.
 * - Computes the top-left corner of the scan region from the center square and
 *   clamps the starting Y coordinate to the world top edge.
 * - Wraps the starting X coordinate across the world width when it goes below
 *   zero, preserving horizontal world wrapping behavior.
 * - Expands `scout_range` into a square side length of `(scout_range * 2) + 1`.
 * - Iterates over every square in that square area.
 * - Skips rows that extend beyond `WORLD_HEIGHT`.
 * - Wraps X coordinates that extend beyond `WORLD_WIDTH` back into range.
 * - Calls `Check_Square_Explored()` for each candidate square.
 * - Calls `Set_Square_Scouted()` only for squares that are already explored.
 *
 * This routine updates only transient scouting state. It does not reveal new
 * territory by itself and does not modify persistent exploration markers.
 *
 * Related functions:
 * - `Set_Square_Scouted()` sets the transient flag for one square.
 * - `Check_Square_Explored()` filters out unexplored squares.
 * - `Clear_Square_Scouted_Flags()` resets plane scouting before this function
 *   is typically used to rebuild visibility.
 *
 * @param wx World-space X coordinate of the scouting center.
 * @param wy World-space Y coordinate of the scouting center.
 * @param wp World plane index whose scouting bitfield will be updated.
 * @param scout_range Radius, in squares, of the scouting area around the center.
 *
 * @return void
 * No return value. Updates the selected plane's transient scouting bitfield in
 * place for eligible squares within the computed area.
 *
 * @note The affected area is a square, not a circle. Horizontal coordinates wrap
 *       across the map width, while vertical coordinates are clamped to valid
 *       world rows.
 */
void Set_Square_Scouted_Flags(int16_t wx, int16_t wy, int16_t wp, int16_t scout_range)
{
    int16_t start_world_x = 0;
    int16_t start_world_y = 0;
    int16_t itr_world_x = 0;
    int16_t itr_world_y = 0;
    int16_t curr_world_x = 0;
    int16_t square_is_explored = 0;

    if(scout_range == 0)
    {
        return;
    }

    start_world_y = (wy - scout_range);

    if(start_world_y < 0)
    {
        start_world_y = 0;
    }

    start_world_x = (wx - scout_range);

    if(start_world_x < 0)
    {
        start_world_x = (start_world_x + WORLD_WIDTH);
    }

    scout_range = ((scout_range * 2) + 1);

    for (itr_world_y = start_world_y; itr_world_y < (start_world_y + scout_range); itr_world_y++)
    {
        if (itr_world_y >= WORLD_HEIGHT)
        {
            continue;
        }
        for (itr_world_x = start_world_x; itr_world_x < (start_world_x + scout_range); itr_world_x++)
        {
            if(itr_world_x < WORLD_WIDTH)
            {
                curr_world_x = itr_world_x;
            }
            else
            {
                curr_world_x = (itr_world_x - WORLD_WIDTH);
            }
            square_is_explored = Check_Square_Explored(curr_world_x, itr_world_y, wp);

            if(square_is_explored != ST_FALSE)
            {
                Set_Square_Scouted(curr_world_x, itr_world_y, wp);
            }
        }
    }
    
}

// WZD o92p07
/* COPILOT */
/**
 * @brief Establishes diplomatic contact when a visible foreign unit is encountered.
 *
 * @details
 * This function checks whether the specified unit belongs to another real player
 * and is currently visible to the human player on the requested world plane. If
 * all conditions are satisfied, it marks diplomatic contact for both players.
 *
 * Implementation behavior:
 * - Retrieves the unit record from `_UNITS[unit_idx]`.
 * - Ignores units owned by the human player.
 * - Ignores units with `owner_idx == ST_UNDEFINED` or `owner_idx == NEUTRAL_PLAYER_IDX`.
 * - Requires the unit to be either on the specified plane or currently in a tower.
 * - Ignores units with the `UE_INVISIBILITY` enchantment flag set.
 * - Reads the unit's world coordinates and checks current visibility through
 *   `Check_Square_Scouted()`.
 * - When the square is currently scouted, sets mutual `Dipl.Contacted` flags for
 *   the human player and the unit owner.
 *
 * The function only establishes contact; it does not reveal terrain, move units,
 * or alter any scouting data.
 *
 * Related functions:
 * - `Update_Scouted_And_Contacted()` calls this function while rebuilding
 *   diplomatic contact state.
 * - `Check_Square_Scouted()` determines whether the unit's square is visible to
 *   the human player.
 *
 * @param unit_idx Index into `_UNITS[]` for the unit being evaluated.
 * @param wp World plane currently being processed for visibility/contact checks.
 *
 * @return void
 * No return value. Updates the human and foreign player's diplomatic contact
 * flags in place when contact conditions are met.
 *
 * @note As indicated by the in-code comment, this logic only checks the unit's
 *       active invisibility enchantment and does not account for natural
 *       invisibility or invisibility granted by items.
 */
void Contact_Other_Player(int16_t unit_idx, int16_t wp)
{
    int owner_idx;
    int wx, wy;
    struct s_UNIT * unit_ptr;
    unit_ptr = &_UNITS[unit_idx];
    owner_idx = (int)unit_ptr->owner_idx;
    if(owner_idx == _human_player_idx)
    {
        return;
    }
    if(owner_idx == ST_UNDEFINED || owner_idx == NEUTRAL_PLAYER_IDX)
    {
        return;
    }
    if((unit_ptr->wp != wp) && (unit_ptr->in_tower != 1))
    {
        return;
    }
    // OGBUG  only checks the unit's active enchantments, t fails to check for  natural invisibility abilities (e.g. Nightstalker) or  invisibility granted by magical items.
    if ((unit_ptr->enchantments & UE_INVISIBILITY) != 0)
    {
        return;
    }
    wx = unit_ptr->wx;
    wy = unit_ptr->wy;
    if(Check_Square_Scouted(wx, wy, wp) != ST_TRUE)
    {
        return;
    }
    _players[_human_player_idx].Dipl.Contacted[owner_idx] = ST_TRUE;
    _players[owner_idx].Dipl.Contacted[_human_player_idx] = ST_TRUE;
}


// WZD o92p08
/**
 * @brief Returns whether a world square has been persistently explored.
 *
 * @details
 * This function checks the saved exploration state for a single world square on
 * the specified plane. Unlike scouting checks, it does not consider transient
 * current-visibility flags; it only reads the persistent explored marker stored
 * by the map exploration system.
 *
 * Implementation behavior:
 * - Reads the square's exploration byte via `GET_SQUARE_EXPLORED(wx, wy, wp)`.
 * - Treats any value other than `UNEXPLORED` as explored.
 * - Returns `ST_TRUE` when any exploration bits are present; otherwise returns
 *   `ST_FALSE`.
 *
 * Related functions:
 * - `Check_Square_Scouted()` requires a square to be explored before it can be
 *   considered currently scouted.
 * - `Set_Square_Scouted_Flags()` uses this function to avoid scouting tiles that
 *   have not yet been discovered.
 *
 * @param wx World-space X coordinate of the square to test.
 * @param wy World-space Y coordinate of the square to test.
 * @param wp World plane index of the square to test.
 *
 * @return int16_t
 * Returns `ST_TRUE` if the square has any explored-state bits set; otherwise
 * returns `ST_FALSE`.
 *
 * @note This function assumes valid world coordinates and plane values. It does
 *       not perform bounds checking before reading exploration state.
 */
int16_t Check_Square_Explored(int16_t wx, int16_t wy, int16_t wp)
{
    uint8_t square_explored_flag = 0;
    int16_t square_is_explored = 0;
    square_is_explored = ST_FALSE;
    square_explored_flag = GET_SQUARE_EXPLORED(wx, wy, wp);
    if(square_explored_flag != UNEXPLORED)
    {
        square_is_explored = ST_TRUE;
    }
    return square_is_explored;
}
