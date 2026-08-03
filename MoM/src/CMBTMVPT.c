/*
    Combat

    WIZARDS.EXE
        ovr155  ¿ MoO2  Module: CMBTMOV1 ?  (would have been CMBTMOVE?)

...a Grid-based Bellman-Ford Iterative Relaxation algorithm...  //Sidav
*/

#include "../../MoX/src/MOX_TYPE.h"

#include "Combat.h"

#include "CMBTMVPT.h"

#include <assert.h>     /* assert() */



// WZD dseg:D13C                                                 ovr153
// WZD dseg:D142
extern int16_t movement_path_grid_cell_count;



// WZD dseg:70C8                                                 BEGIN:  ovr155 - Initialized Data

/*
1D verions of the offsets for { NE, SW, SE, NW, N, S, W, E } one the combat map grid
-1000, so can use the instruction that checks for a negative sign flag (like jl - Jump if Less than zero)
cardinal
diagonal
On the edges, the "illegal" directions are also set to -1000 to prevent wraparound pathfinding bugs
First-Column: No NW, W, SW
Last-Column:  No NE, E, SE
*/
//                                    NE     SW     SE     NW  North  South   West   East
int16_t adjacent_offsets[3][8] =
{
    /*                      NE                       SW                       SE                        NW                     N                    S      W      E */
    { -(COMBAT_GRID_WIDTH - 1), (COMBAT_GRID_WIDTH - 1), (COMBAT_GRID_WIDTH + 1), -(COMBAT_GRID_WIDTH + 1), -(COMBAT_GRID_WIDTH), (COMBAT_GRID_WIDTH),    -1,     1 },
    { -(COMBAT_GRID_WIDTH - 1),                   -1000, (COMBAT_GRID_WIDTH + 1),                    -1000, -(COMBAT_GRID_WIDTH), (COMBAT_GRID_WIDTH), -1000,     1 },
    {                    -1000, (COMBAT_GRID_WIDTH - 1),                   -1000, -(COMBAT_GRID_WIDTH + 1), -(COMBAT_GRID_WIDTH), (COMBAT_GRID_WIDTH),    -1, -1000 }
};



// WZD dseg:D384                                                 BEGIN:  ovr155 - Uninitialized Data

// WZD dseg:D384
// FTW? Just a 2-byte pointer over a 1-byte array?
// Yup, yup.
/*
m_movement_path_grid_cell_index points to the same allocation as _cmbt_movepath_cost_map
after the code is done making the cost map, it reuses that same memory for the path reconstruction indices
*/
int16_t * m_movement_path_grid_cell_index;

// WZD dseg:D384                                                 END:  ovr155 - Uninitialized Data



/*
    WIZARDS.EXE  ovr155
*/

#define PREP \
{ \
    /* predecessor array */ \
    for(itr = 0; itr < COMBAT_GRID_CELL_COUNT; itr++) { _cmbt_path_data[itr] = itr; } \
    /* distance array */ \
    for(itr = 0; itr < COMBAT_GRID_CELL_COUNT; itr++) { _cmbt_mvpth_c[itr] = (uint8_t)INF; } \
    /* Set the starting square cost to 0, which "ignites" the flood fill */ \
    _cmbt_mvpth_c[((source_cgy * COMBAT_GRID_WIDTH) + source_cgx)] = (uint8_t)0; \
}

/* * MACRO: RELAX_ADJACENT_CELLS
 * Inlines the Bellman-Ford edge relaxation math. 
 * Relies on standard local variables: itr_adjacent, ctr, adjacent_idx, adjacent_path_cost, 
 * move_cost, new_cost_to_reach, current_origin, and a_cost_was_updated.
 */
#define RELAX_ADJACENT_CELLS(OFFSET_ARRAY, START_IDX, END_IDX, EXTRA_COST) \
    for(itr_adjacent = (START_IDX); itr_adjacent < (END_IDX); itr_adjacent++) { \
        adjacent_idx = ctr + OFFSET_ARRAY[itr_adjacent]; \
        if(adjacent_idx >= 0 && adjacent_idx < COMBAT_GRID_CELL_COUNT) { \
            adjacent_path_cost = _cmbt_mvpth_c[adjacent_idx]; \
            if(adjacent_path_cost != INF) { \
                new_cost_to_reach = adjacent_path_cost + move_cost + (EXTRA_COST); \
                if(new_cost_to_reach < _cmbt_mvpth_c[ctr]) { \
                    _cmbt_path_data[ctr] = adjacent_idx; \
                    _cmbt_mvpth_c[ctr] = (uint8_t)new_cost_to_reach; \
                    if(_cmbt_path_data[ctr] != current_origin) { \
                        a_cost_was_updated = ST_TRUE; \
                    } \
                } \
            } \
        } \
    }

// WZD ovr155p01
void Combat_Move_Path_Find(int16_t source_cgx, int16_t source_cgy, int16_t destination_cgx, int16_t destination_cgy)
{
    uint8_t move_cost = 0;              /* current tile entry cost */  /* 1-byte, unsigned */
    uint8_t new_cost_to_reach = 0;      /* candidate new cost */
    int16_t max_y = 0;
    int16_t adjacent_path_cost = 0;
    int16_t max_x = 0;
    int16_t itr_x = 0;
    int16_t itr_y = 0;
    int16_t current_origin = 0;
    int16_t a_cost_was_updated = 0;     /* relaxation "changed" flag */
    int16_t itr = 0;
    int16_t ctr = 0;                    /* current tile 1-D index */
    int16_t itr_adjacent = 0;
    int16_t adjacent_idx = 0;
    int16_t path_cgx = 0;               // DNE in Dasm
    int16_t path_cgy = 0;               // DNE in Dasm

    movement_path_grid_cell_count = 0;

    /* [Skeleton step 2]  1. Bail out early if the destination is an illegal/impassable square */
    if(_cmbt_movepath_cost_map[((destination_cgy * COMBAT_GRID_WIDTH) + destination_cgx)] == INF)
    {
        return; 
    }

    /* [Skeleton step 1]  2. Initialize the Pathing — predecessor _cmbt_path_data[] = self, distance _cmbt_mvpth_c[] = INF, source = 0 (PREP macro) */
    PREP

    /* --- [Skeleton step 3]  3. THE BELLMAN-FORD RELAXATION SWEEP (single-direction raster, to fixed point) --- */
    a_cost_was_updated = ST_TRUE;
    while(a_cost_was_updated == ST_TRUE)
    {
        a_cost_was_updated = ST_FALSE;
        max_x = COMBAT_GRID_WIDTH - 2;
        max_y = COMBAT_GRID_HEIGHT - 2;
        ctr = 0; /* 1D Array Index Counter */

        for(itr_y = 0; itr_y < max_y; itr_y++)
        {

            /* -- PHASE 1: Left Edge (X = 0) -- */
            move_cost = _cmbt_movepath_cost_map[ctr];
            if(move_cost != INF)
            {
                current_origin = _cmbt_path_data[ctr];
                RELAX_ADJACENT_CELLS(adjacent_offsets[1], 0, 4, 1); /* Diagonals (+1 Cost) */
                RELAX_ADJACENT_CELLS(adjacent_offsets[1], 4, 8, 0); /* Orthogonals */
            }
            ctr++;

            /* -- PHASE 2: Middle Squares (X = 1 to 19) -- */
            for(itr_x = 0; itr_x < max_x; itr_x++)
            {
                move_cost = _cmbt_movepath_cost_map[ctr];
                if(move_cost != INF)
                {
                    current_origin = _cmbt_path_data[ctr];
                    RELAX_ADJACENT_CELLS(adjacent_offsets[0], 0, 4, 1);
                    RELAX_ADJACENT_CELLS(adjacent_offsets[0], 4, 8, 0);
                }
                ctr++;
            }

            /* -- PHASE 3: Right Edge (X = 20) -- */
            move_cost = _cmbt_movepath_cost_map[ctr];
            if(move_cost != INF)
            {
                current_origin = _cmbt_path_data[ctr];
                RELAX_ADJACENT_CELLS(adjacent_offsets[2], 0, 4, 1);
                RELAX_ADJACENT_CELLS(adjacent_offsets[2], 4, 8, 0);
            }
            ctr++;
        }
    }

    /* --- [Skeleton step 4]  4. TRACE THE PATH (back-trace dst -> _cmbt_path_data[] until a self-link) --- */

    /* Temp array to hold the raw index path */
    /* THE HIJACK: Aim the global pointer at the terrain cost map */
    m_movement_path_grid_cell_index = (int16_t *)_cmbt_movepath_cost_map;

    movement_path_grid_cell_count = 0;

    /* Start at destination, follow the breadcrumbs backwards to the source */
    ctr = (destination_cgy * COMBAT_GRID_WIDTH) + destination_cgx;

    /* The Trace Loop */
    while(_cmbt_path_data[ctr] != ctr)
    {
        m_movement_path_grid_cell_index[movement_path_grid_cell_count] = ctr;
        ctr = _cmbt_path_data[ctr];
        movement_path_grid_cell_count++;
    }

    /* --- [Skeleton step 5]  5. REVERSE AND CONVERT TO 2D --- */
    /* Reverse the path and convert the 1D indices back into 2D (X,Y) coordinates */
    for(itr = 0; itr < movement_path_grid_cell_count; itr++)
    {
        path_cgx = (m_movement_path_grid_cell_index[((movement_path_grid_cell_count - 1) - itr)] % COMBAT_GRID_WIDTH);
        _cmbt_mvpth_x[itr] = path_cgx;
        path_cgy = (m_movement_path_grid_cell_index[((movement_path_grid_cell_count - 1) - itr)] / COMBAT_GRID_WIDTH);
        _cmbt_mvpth_y[itr] = path_cgy;
    }

}


// WZD ovr155p02
// ¿ MoO2 Set_Legal_Moves_() ?
void Combat_Move_Path_Valid(int16_t source_cgx, int16_t source_cgy, int16_t moves2)
{
    uint8_t move_cost = 0;  /* 1-byte, unsigned */
    uint8_t new_cost_to_reach = 0;
    int16_t get_to_cost = 0;
    int16_t max_y = 0;
    int16_t adjacent_path_cost = 0;
    int16_t max_x = 0;
    int16_t itr_x = 0;
    int16_t itr_y = 0;
    int16_t current_origin = 0;
    int16_t a_cost_was_updated = 0;
    int16_t itr = 0;
    int16_t itr2_y = 0;
    int16_t itr2_x = 0;
    int16_t ctr = 0;
    int16_t itr_adjacent = 0;
    int16_t adjacent_idx = 0;

    movement_path_grid_cell_count = 0;

    /* Initialize the Pathing */
    PREP

    /* --- THE RELAXATION SWEEP --- */
    a_cost_was_updated = ST_TRUE;
    while(a_cost_was_updated == ST_TRUE)
    {
        a_cost_was_updated = ST_FALSE;
        
        max_x = COMBAT_GRID_WIDTH - 2;  /* 19 */
        max_y = COMBAT_GRID_HEIGHT - 2; /* 20 */
        
        ctr = 0; /* Current 1D grid index */

        /* Loop through the rows */
        for(itr_y = 0; itr_y < max_y; itr_y++) {
            
            /* -- PHASE 1: Left Edge (X = 0) -- */
            move_cost = _cmbt_movepath_cost_map[ctr];
            if(move_cost != INF) {
                current_origin = _cmbt_path_data[ctr];
                RELAX_ADJACENT_CELLS(adjacent_offsets[1], 0, 8, 0);
            }
            ctr++;

            /* -- PHASE 2: Middle Squares (X = 1 to 19) -- */
            for(itr_x = 0; itr_x < max_x; itr_x++) {
                move_cost = _cmbt_movepath_cost_map[ctr];
                if(move_cost != INF) {
                    current_origin = _cmbt_path_data[ctr];
                    RELAX_ADJACENT_CELLS(adjacent_offsets[0], 0, 8, 0);
                }
                ctr++;
            }

            /* -- PHASE 3: Right Edge (X = 20) -- */
            move_cost = _cmbt_movepath_cost_map[ctr];
            if(move_cost != INF) {
                current_origin = _cmbt_path_data[ctr];
                RELAX_ADJACENT_CELLS(adjacent_offsets[2], 0, 8, 0);
            }
            ctr++;
        }
    }

    /* --- BUILD THE HIGHLIGHT OVERLAY --- */
    /* Iterate over the entire 21x22 grid one last time */
    for(itr2_y = 0; itr2_y < COMBAT_GRID_HEIGHT; itr2_y++)
    {
        for(itr2_x = 0; itr2_x < COMBAT_GRID_WIDTH; itr2_x++)
        {
            ctr = (itr2_y * COMBAT_GRID_WIDTH) + itr2_x;
            
            /* Default this square to "Unreachable" */
            _cmbt_path_data[ctr] = ST_FALSE;

            /* If the flood fill ever reached this square... */
            if(_cmbt_mvpth_c[ctr] != INF)
            {
                
                move_cost = _cmbt_movepath_cost_map[ctr];
                
                /* Subtract the final step's terrain cost to find out how many 
                   movement points it took to reach the edge of this square */
                get_to_cost = _cmbt_mvpth_c[ctr] - move_cost;

                /* If our remaining movement points are strictly GREATER than the cost 
                   to get there, we can step into it! */
                // ...player-friendly mechanic...
                if(moves2 > get_to_cost)
                {
                    /* Mark the square as "Reachable" (Draw the blue highlight here) */
                    _cmbt_path_data[ctr] = ST_TRUE;
                }
            }
        }
    }

}
