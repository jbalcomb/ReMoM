/*
    STU_DBG_Combat_Move_Path -- the combat movement-path learning / debugging code, boxed up in one place.

    Scratch from working out the combat move-path algorithm (Combat_Move_Path_Find / Combat_Move_Path_Valid in MoM/src/CMBTMVPT.c).  No asm
    counterpart, and nothing uses it: kept for reference.  Moved from MoM/src/Combat.h (the NEW_PATH_COST* macros and their design notes),
    MoM/src/Combat.c (DBG_cgx / DBG_cgy) and STU/src/STU_DBG_MoM (the other coordinate knobs).

    The macros expand inside a path-solver loop and use that function's locals (ctr, itr_adjacent, adjacent_idx, adjacent_path_cost,
    current_origin, a_cost_was_updated, ...) and the combat path globals (_cmbt_mvpth_c, _cmbt_path_data, adjacent_offsets[]), so they compile
    only where those are in scope.  NEW_PATH_COST_ANY compares each adjacent cell against the DBG_cgx / DBG_cgy break-point square and calls
    STU_DEBUG_BREAK().  Only NEW_PATH_COST_ALL uses NEW_PATH_COST_ANY; nothing uses NEW_PATH_COST_ALL or the others.
*/

#ifndef STU_DBG_COMBAT_MOVE_PATH_H
#define STU_DBG_COMBAT_MOVE_PATH_H

#ifdef STU_DEBUG

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* break-point square for NEW_PATH_COST_ANY (combat grid x, y) */
extern int16_t DBG_cgx;
extern int16_t DBG_cgy;
/* other path-finding knobs, never read; the comments in STU_DBG_MoM.c name grid squares (13,11), (14,11), (15,11) */
extern int16_t DBG_target_cgx;
extern int16_t DBG_target_cgy;
extern int16_t DBG_PFA_1311_set;
extern int16_t DBG_PFA_1411_set;
extern int16_t DBG_PFA_1511_set;
extern int16_t DBG_player_idx;

#ifdef __cplusplus
}
#endif

/*
    reduce the loops over the adjacent cells to a single macro
    iter 0-8 or iter 0-4 & 4-8

NEW_PATH_COST()
...the the portion inside each iteration, because that just needs itr_adjacent

NEW_PATH_COST_0()
NEW_PATH_COST_1()
NEW_PATH_COST_2()
...treats the three adjacent cell offsets array as one array - adjacent_offsets[(({0,1,2} * 8) + itr_adjacent)]

NEW_PATH_COST_ANY()
...takes a parameter, which just reduces NEW_PATH_COST_0(), NEW_PATH_COST_1(), and NEW_PATH_COST_2()

what I want is all of this...
            move_cost = _cmbt_movepath_cost_map[ctr];  // moves2 cost of cell, given terrain and movement mode
            if(!IS_INF(move_cost))
            {
                current_origin = _cmbt_path_data[ctr];
                for(itr_adjacent = 0; itr_adjacent < 4; itr_adjacent++)
                {
                    adjacent_idx = adjacent_offsets[1][itr_adjacent];
                    NEW_PATH_COST()
                }
                for(itr_adjacent = 4; itr_adjacent < 8; itr_adjacent++)
                {
                    adjacent_idx = adjacent_offsets[1][itr_adjacent];
                    NEW_PATH_COST()
                }
            }
            ctr++;

NEW_PATH_COST_ALL()
...uses NEW_PATH_COST_ANY()


*/
#define NEW_PATH_COST() {                                               \
    if((adjacent_idx >= 0) && (adjacent_idx < COMBAT_GRID_CELL_COUNT))  \
    {                                                                   \
        adjacent_path_cost = _cmbt_mvpth_c[adjacent_idx];               \
        if(adjacent_path_cost != 255)                                   \
        {                                                               \
            potential_path_cost = adjacent_path_cost + move_cost;       \
            if(_cmbt_mvpth_c[ctr] > potential_path_cost)                \
            {                                                           \
                _cmbt_path_data[ctr] = adjacent_idx;                    \
                _cmbt_mvpth_c[ctr] = (uint8_t)potential_path_cost;      \
                if(_cmbt_path_data[ctr] != current_origin)              \
                {                                                       \
                    a_cost_was_updated = ST_TRUE;                       \
                }                                                       \
            }                                                           \
        }                                                               \
    }                                                                   \
}


#define NEW_PATH_COST_0() {                                             \
    adjacent_idx = adjacent_offsets[((0 * 8) + itr_adjacent)];          \
    if((adjacent_idx >= 0) && (adjacent_idx < COMBAT_GRID_CELL_COUNT))  \
    {                                                                   \
        adjacent_path_cost = _cmbt_mvpth_c[adjacent_idx];               \
        if(adjacent_path_cost != 255)                                   \
        {                                                               \
            potential_path_cost = adjacent_path_cost + move_cost;       \
            if(_cmbt_mvpth_c[ctr] > potential_path_cost)                \
            {                                                           \
                _cmbt_path_data[ctr] = adjacent_idx;                    \
                _cmbt_mvpth_c[ctr] = (uint8_t)potential_path_cost;      \
                if(_cmbt_path_data[ctr] != current_origin)              \
                {                                                       \
                    a_cost_was_updated = ST_TRUE;                       \
                }                                                       \
            }                                                           \
        }                                                               \
    }                                                                   \
}

#define NEW_PATH_COST_1() {                                             \
    adjacent_idx = adjacent_offsets[((1 * 8) + itr_adjacent)];          \
    if((adjacent_idx >= 0) && (adjacent_idx < COMBAT_GRID_CELL_COUNT))  \
    {                                                                   \
        adjacent_path_cost = _cmbt_mvpth_c[adjacent_idx];               \
        if(adjacent_path_cost != 255)                                   \
        {                                                               \
            potential_path_cost = adjacent_path_cost + move_cost;       \
            if(_cmbt_mvpth_c[ctr] > potential_path_cost)                \
            {                                                           \
                _cmbt_path_data[ctr] = adjacent_idx;                    \
                _cmbt_mvpth_c[ctr] = (uint8_t)potential_path_cost;      \
                if(_cmbt_path_data[ctr] != current_origin)              \
                {                                                       \
                    a_cost_was_updated = ST_TRUE;                       \
                }                                                       \
            }                                                           \
        }                                                               \
    }                                                                   \
}

#define NEW_PATH_COST_2() {                                             \
    adjacent_idx = adjacent_offsets[((2 * 8) + itr_adjacent)];          \
    if((adjacent_idx >= 0) && (adjacent_idx < COMBAT_GRID_CELL_COUNT))  \
    {                                                                   \
        adjacent_path_cost = _cmbt_mvpth_c[adjacent_idx];               \
        if(adjacent_path_cost != 255)                                   \
        {                                                               \
            potential_path_cost = adjacent_path_cost + move_cost;       \
            if(_cmbt_mvpth_c[ctr] > potential_path_cost)                \
            {                                                           \
                _cmbt_path_data[ctr] = adjacent_idx;                    \
                _cmbt_mvpth_c[ctr] = (uint8_t)potential_path_cost;      \
                if(_cmbt_path_data[ctr] != current_origin)              \
                {                                                       \
                    a_cost_was_updated = ST_TRUE;                       \
                }                                                       \
            }                                                           \
        }                                                               \
    }                                                                   \
}

#define NEW_PATH_COST_ANY(_value_) {                                            \
    adjacent_idx = (ctr + adjacent_offsets[(((_value_) * 8) + itr_adjacent)]);  \
    if(adjacent_idx == ((DBG_cgy * COMBAT_GRID_WIDTH) + DBG_cgx))               \
    {                                                                           \
        STU_DEBUG_BREAK();                                                      \
    }                                                                           \
    if(GTELT(adjacent_idx, 0, COMBAT_GRID_CELL_COUNT))                          \
    {                                                                           \
        adjacent_path_cost = _cmbt_mvpth_c[adjacent_idx];                       \
        if(!IS_INF(adjacent_path_cost))                                         \
        {                                                                       \
            potential_path_cost = adjacent_path_cost + move_cost;               \
            existing_path_cost = _cmbt_mvpth_c[ctr];                            \
            if(existing_path_cost > potential_path_cost)                        \
            {                                                                   \
                _cmbt_path_data[ctr] = adjacent_idx;                            \
                _cmbt_mvpth_c[ctr] = (uint8_t)potential_path_cost;              \
                new_next_cell_index = _cmbt_path_data[ctr];                     \
                if(new_next_cell_index != current_origin)                       \
                {                                                               \
                    unstable = ST_TRUE;                                         \
                }                                                               \
            }                                                                   \
        }                                                                       \
    }                                                                           \
}

#define NEW_PATH_COST_ALL(_value_) {                            \
    move_cost = _cmbt_movepath_cost_map[ctr];                   \
    if(!IS_INF(move_cost))                                      \
    {                                                           \
        current_origin = _cmbt_path_data[ctr];                  \
        for(itr_adjacent = 0; itr_adjacent < 4; itr_adjacent++) \
        {                                                       \
            NEW_PATH_COST_ANY(itr_adjacent)                     \
        }                                                       \
        for(itr_adjacent = 4; itr_adjacent < 8; itr_adjacent++) \
        {                                                       \
            NEW_PATH_COST_ANY(itr_adjacent)                     \
        }                                                       \
    }                                                           \
    ctr++;                                                      \
}

#endif  /* STU_DEBUG */

#endif  /* STU_DBG_COMBAT_MOVE_PATH_H */
