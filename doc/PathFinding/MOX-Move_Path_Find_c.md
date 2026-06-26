MOX-Move_Path_Find_c.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr147\Move_Path_Find.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr147\Move_Path_Find.c

WZD ovr147  Move_Path_Find()    OON
WZD ovr148

// WZD o147p01
void Move_Path_Find(int16_t wx, int16_t wy, struct s_MOVE_PATH * movepath_cost_map);



Progression?
Move_Path_Find__MEH() was the original?
...then, I carved out the macros? - CHECK_COST, DO_COSTS_FIRST, DO_COSTS, DO_COSTS_LAST
But, VS/VSC suck-hard at expanding/parsing/debuging macros...
So, I created Move_Path_Find, with helper functions instead - Check_Cost(); Do_Costs_Lst(); Do_Costs_Mid(); Do_Costs_Fst();
...and moved the Code Segment variables into the TU's global space



```c

```


```c
#define CHECK_COST(_adj_) {                                             \
    adjacent_reach_cost = *(movepath_reach_cost + _adj_);               \
    if((adjacent_reach_cost + tmp_move_cost) >= 0)                      \
    {                                                                   \
        adjacent_reach_cost += tmp_move_cost;                           \
        if(adjacent_reach_cost < *movepath_reach_cost)                  \
        {                                                               \
            *movepath_reach_cost = new_cost_to_reach;                      \
            *movepath_reach_from = (ofst_movepath_cost + (_adj_ - 1));  \
            a_cost_was_updated = ST_TRUE;                              \
        }                                                               \
    }                                                                   \
}


#define DO_COSTS_FIRST() {          \
    move_cost = *movepath_cost++;   \
    ofst_movepath_cost++;           \
    if(move_cost != -1)             \
    {                               \
        incr_flag++;                \
        tmp_move_cost = move_cost;  \
        /* North-West */            \
        CHECK_COST(-1);             \
        /* North      */            \
        CHECK_COST(-60);            \
        /* North-East */            \
        CHECK_COST(-59);            \
        /* West       */            \
        CHECK_COST(59);             \
        /* East       */            \
        CHECK_COST(1);              \
        /* South-West */            \
        CHECK_COST(119);            \
        /* South      */            \
        CHECK_COST(60);             \
        /* South-East */            \
        CHECK_COST(61);             \
    }                               \
    movepath_reach_cost++;          \
    movepath_reach_from++;          \
}

#define DO_COSTS() {                \
    move_cost = *movepath_cost++;   \
    ofst_movepath_cost++;           \
    if(move_cost != -1)             \
    {                               \
        incr_flag++;                \
        tmp_move_cost = move_cost;  \
        /* North-West */            \
        CHECK_COST(-61);            \
        /* North      */            \
        CHECK_COST(-60);            \
        /* North-East */            \
        CHECK_COST(-59);            \
        /* West       */            \
        CHECK_COST(-1);             \
        /* East       */            \
        CHECK_COST(1);              \
        /* South-West */            \
        CHECK_COST(59);             \
        /* South      */            \
        CHECK_COST(60);             \
        /* South-East */            \
        CHECK_COST(61);             \
    }                               \
    movepath_reach_cost++;          \
    movepath_reach_from++;          \
}

#define DO_COSTS_LAST() {           \
    move_cost = *movepath_cost++;   \
    ofst_movepath_cost++;           \
    if(move_cost != -1)             \
    {                               \
        incr_flag++;                \
        tmp_move_cost = move_cost;  \
        /* North-West */            \
        CHECK_COST(-61);            \
        /* North      */            \
        CHECK_COST(-60);            \
        /* North-East */            \
        CHECK_COST(-119);           \
        /* West       */            \
        CHECK_COST(-1);             \
        /* East       */            \
        CHECK_COST(-59);            \
        /* South-West */            \
        CHECK_COST(59);             \
        /* South      */            \
        CHECK_COST(60);             \
        /* South-East */            \
        CHECK_COST(1);              \
    }                               \
    movepath_reach_cost++;          \
    movepath_reach_from++;          \
}



// WZD o147p1  [1of1]  (OON)
// drake178: Overland_Pathfinder()
/*
¿¿¿
loop over each row (world map height, as 1-D array index)
start at each x = 0
handle special case
    at x = 0, 'North-West' is -1, rather than -61, etc.
loop over balance of row

???
*/
static void Move_Path_Find__MEH(int16_t wx, int16_t wy, struct s_MOVE_PATH * movepath_cost_map)
{
    int8_t  a_cost_was_updated = ST_TRUE;  // WZD ovr147:0000  Code-Segment Variable
    int16_t origin_row = 0;                 // WZD ovr147:0002  Code-Segment Variable
    int16_t itr = 0;
    int8_t * movepath_cost = 0;
    uint8_t * movepath_reach_cost = 0;
    int16_t * movepath_reach_from = 0;
    int16_t ofst_movepath_cost = 0;
    int8_t move_cost = 0;
    int8_t itr_row = 0;  // _CL_
    int8_t incr_flag = 0;  // _CH_
    int8_t tmp_move_cost = 0;
    uint8_t adjacent_reach_cost = 0;
    uint8_t new_cost_to_reach = 0;
    uint8_t current_reach_cost = 0;
    int8_t reach_cost = 0;
    int16_t adj_pos = 0;
    int8_t dbg_wms0_reach_cost = 0;
    int8_t dbg_wms1_reach_cost = 0;
    int8_t dbg_wms2_reach_cost = 0;
    int8_t dbg_wms3_reach_cost = 0;
    int16_t dbg_wms0_reach_from = 0;
    int16_t dbg_wms1_reach_from = 0;
    int16_t dbg_wms2_reach_from = 0;
    int16_t dbg_wms3_reach_from = 0;

#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: BEGIN: Move_Path_Find(wx = %d, wy = %d, movepath_cost_map = %p)", __FILE__, __LINE__, wx, wy, movepath_cost_map);
#endif

    // start of first map grid row
    origin_row = (wy * WORLD_WIDTH);
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: origin_row: %d", __FILE__, __LINE__, origin_row);
#endif

    for(itr = 0; itr < WORLD_SIZE; itr++)
    {
        movepath_cost_map->Reach_Costs[itr] = -1;
    }

    for(itr = 0; itr < WORLD_SIZE; itr++)
    {
        movepath_cost_map->Reach_From[itr] = itr;
    }

    // set the src wms to 0 - no cost will ever be less
    movepath_cost_map->Reach_Costs[((wy * WORLD_WIDTH) + wx)] = 0;
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: movepath_cost_map->Reach_Costs[((wy * WORLD_WIDTH) + wx)]: %d", __FILE__, __LINE__, movepath_cost_map->Reach_Costs[((wy * WORLD_WIDTH) + wx)]);
#endif

    dbg_wms0_reach_cost = movepath_cost_map->Reach_Costs[(((wy - 0) * WORLD_WIDTH) + (wx + 0))];
    dbg_wms1_reach_cost = movepath_cost_map->Reach_Costs[(((wy - 1) * WORLD_WIDTH) + (wx + 1))];
    dbg_wms2_reach_cost = movepath_cost_map->Reach_Costs[(((wy - 2) * WORLD_WIDTH) + (wx + 2))];
    dbg_wms3_reach_cost = movepath_cost_map->Reach_Costs[(((wy - 3) * WORLD_WIDTH) + (wx + 3))];

    dbg_wms0_reach_from = movepath_cost_map->Reach_From[(((wy - 0) * WORLD_WIDTH) + (wx + 0))];
    dbg_wms1_reach_from = movepath_cost_map->Reach_From[(((wy - 1) * WORLD_WIDTH) + (wx + 1))];
    dbg_wms2_reach_from = movepath_cost_map->Reach_From[(((wy - 2) * WORLD_WIDTH) + (wx + 2))];
    dbg_wms3_reach_from = movepath_cost_map->Reach_From[(((wy - 3) * WORLD_WIDTH) + (wx + 3))];



/*
    BEGIN:  @@MajorBlock_1_Outer
*/

    // ¿ do{ ...} while(a_cost_was_updated == ST_TRUE) ?

MajorBlock_1_Outer:

    a_cost_was_updated = ST_FALSE;
    ofst_movepath_cost = origin_row;
    movepath_cost = &movepath_cost_map->moves2[origin_row];
    movepath_reach_cost = &movepath_cost_map->Reach_Costs[origin_row];
    movepath_reach_from = &movepath_cost_map->Reach_From[origin_row];
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: ofst_movepath_cost: %d", __FILE__, __LINE__, ofst_movepath_cost);
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: &movepath_cost_map->moves2[origin_row]: %p", __FILE__, __LINE__, movepath_cost);
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: &movepath_cost_map->Reach_Costs[origin_row]: %p", __FILE__, __LINE__, movepath_reach_cost);
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: &movepath_cost_map->Reach_From[origin_row]: %p", __FILE__, __LINE__, movepath_reach_from);
#endif

/*
    BEGIN:  @@MajorBlock_1_Inner
*/

MajorBlock_1_Impassible:

    incr_flag = 0;

    // Handle Special Case:  (x = 0)  wrap around backward
    // DO_COSTS_FIRST();
    move_cost = *movepath_cost++;
    ofst_movepath_cost++;
    if(move_cost != -1)
    {
        incr_flag++;
        tmp_move_cost = move_cost;
        // North-West
        adj_pos = -1;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)  // ~ non-negative
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // North
        adj_pos = -60;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // North-East
        adj_pos = -59;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // West
        adj_pos = 59;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // East
        adj_pos = 1;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // South-West
        adj_pos = 119;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // South
        adj_pos = 60;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // South-East
        adj_pos = 61;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
    }
    movepath_reach_cost++;
    movepath_reach_from++;

    // Handle Normal Cases:  x = {1, ..., 59}
    itr_row = 29;  // ((60 - 1 - 1) / 2)  ((WORLD_WIDTH - Fst - Lst) / 2)
    while(itr_row != 0)
    {
        
        // DO_COSTS();
        move_cost = *movepath_cost++;
        ofst_movepath_cost++;
        if(move_cost != -1)
        {
            incr_flag++;
            tmp_move_cost = move_cost;
            // North-West
            adj_pos = -61;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // North
            adj_pos = -60;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // North-East
            adj_pos = -59;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // West
            adj_pos = -1;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // East
            adj_pos = 1;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // South-West
            adj_pos = 59;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // South
            adj_pos = 60;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // South-East
            adj_pos = 61;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
        }
        movepath_reach_cost++;
        movepath_reach_from++;

        
        // DO_COSTS();
        move_cost = *movepath_cost++;
        ofst_movepath_cost++;
        if(move_cost != -1)
        {
            incr_flag++;
            tmp_move_cost = move_cost;
            // North-West
            adj_pos = -61;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // North
            adj_pos = -60;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // North-East
            adj_pos = -59;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // West
            adj_pos = -1;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // East
            adj_pos = 1;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // South-West
            adj_pos = 59;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // South
            adj_pos = 60;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // South-East
            adj_pos = 61;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
        }
        movepath_reach_cost++;
        movepath_reach_from++;

        itr_row--;
    }

    // Handle Special Case:  (x = 59)  wrap around forward

    // DO_COSTS_LAST();
    move_cost = *movepath_cost++;
    ofst_movepath_cost++;
    if(move_cost != -1)
    {
        incr_flag++;
        tmp_move_cost = move_cost;
        // North-West
        adj_pos = -61;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // North
        adj_pos = -60;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // North-East
        adj_pos = -119;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // West
        adj_pos = -1;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // East
        adj_pos = -59;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // South-West
        adj_pos = 59;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // South
        adj_pos = 60;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // South-East
        adj_pos = 1;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
    }
    movepath_reach_cost++;
    movepath_reach_from++;

    if(
        (incr_flag != 0) &&  /* found any that were not *impassible* */
        (ofst_movepath_cost < 2320)  /* ¿ (WORLD_SIZE - (2 * WORLD_HEIGHT)) ? */
    )
    {
        goto MajorBlock_1_Impassible;
    }

/*
    END:  @@MajorBlock_1_Inner
*/



/*
    BEGIN:  @@MajorBlock_2
*/

    ofst_movepath_cost = origin_row;
    movepath_cost = &movepath_cost_map->moves2[origin_row];
    movepath_reach_cost = &movepath_cost_map->Reach_Costs[origin_row];
    movepath_reach_from = &movepath_cost_map->Reach_From[origin_row];
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: ofst_movepath_cost: %d", __FILE__, __LINE__, ofst_movepath_cost);
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: &movepath_cost_map->moves2[origin_row]: %p", __FILE__, __LINE__, movepath_cost);
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: &movepath_cost_map->Reach_Costs[origin_row]: %p", __FILE__, __LINE__, movepath_reach_cost);
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: &movepath_cost_map->Reach_From[origin_row]: %p", __FILE__, __LINE__, movepath_reach_from);
#endif


MajorBlock_2_Impassible:

    incr_flag = 0;

    // Handle Special Case:  (x = 0)  wrap around backward
    
    // DO_COSTS_FIRST();
    move_cost = *movepath_cost++;
    ofst_movepath_cost++;
    if(move_cost != -1)
    {
        incr_flag++;
        tmp_move_cost = move_cost;
        // North-West
        adj_pos = -1;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // North
        adj_pos = -60;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // North-East
        adj_pos = -59;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // West
        adj_pos = 59;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // East
        adj_pos = 1;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // South-West
        adj_pos = 119;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // South
        adj_pos = 60;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // South-East
        adj_pos = 61;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
    }
    movepath_reach_cost++;
    movepath_reach_from++;

    // Handle Normal Cases:  x = {1, ..., 59}
    itr_row = 29;  // ((60 - 1 - 1) / 2)  ((WORLD_WIDTH - Fst - Lst) / 2)
    while(itr_row != 0)
    {
        
        // DO_COSTS();
        move_cost = *movepath_cost++;
        ofst_movepath_cost++;
        if(move_cost != -1)
        {
            incr_flag++;
            tmp_move_cost = move_cost;
            // North-West
            adj_pos = -61;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // North
            adj_pos = -60;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // North-East
            adj_pos = -59;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // West
            adj_pos = -1;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // East
            adj_pos = 1;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // South-West
            adj_pos = 59;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // South
            adj_pos = 60;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // South-East
            adj_pos = 61;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
        }
        movepath_reach_cost++;
        movepath_reach_from++;

        
        // DO_COSTS();
        move_cost = *movepath_cost++;
        ofst_movepath_cost++;
        if(move_cost != -1)
        {
            incr_flag++;
            tmp_move_cost = move_cost;
            // North-West
            adj_pos = -61;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // North
            adj_pos = -60;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // North-East
            adj_pos = -59;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // West
            adj_pos = -1;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // East
            adj_pos = 1;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // South-West
            adj_pos = 59;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // South
            adj_pos = 60;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
            // South-East
            adj_pos = 61;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
                if(new_cost_to_reach >= 0)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_cost_to_reach;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        a_cost_was_updated = ST_TRUE;
                    }
                }
            }
        }
        movepath_reach_cost++;
        movepath_reach_from++;

        itr_row--;
    }

    // Handle Special Case:  (x = 59)  wrap around forward

    // DO_COSTS_LAST();
    move_cost = *movepath_cost++;
    ofst_movepath_cost++;
    if(move_cost != -1)
    {
        incr_flag++;
        tmp_move_cost = move_cost;
        // North-West
        adj_pos = -61;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // North
        adj_pos = -60;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // North-East
        adj_pos = -119;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // West
        adj_pos = -1;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // East
        adj_pos = -59;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // South-West
        adj_pos = 59;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // South
        adj_pos = 60;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
        // South-East
        adj_pos = 1;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
            if(adjacent_reach_cost == INF) { adjacent_reach_cost = 1; }
            new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
            if(new_cost_to_reach >= 0)
            {
                current_reach_cost = *movepath_reach_cost;
                // if(current_reach_cost == 0) { MOX_DBG_BREAK; }
                if(new_cost_to_reach < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_cost_to_reach;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    a_cost_was_updated = ST_TRUE;
                }
            }
        }
    }
    movepath_reach_cost++;
    movepath_reach_from++;


    movepath_cost -= 120;
    ofst_movepath_cost  -= 120;
    movepath_reach_cost -= 120;
    movepath_reach_from -= 120;


    if(
        (incr_flag != 0) &&
        (ofst_movepath_cost >= 60)
    )
    {
        goto MajorBlock_2_Impassible;
    }

/*
    END:  @@MajorBlock_2
*/

    if(a_cost_was_updated == ST_TRUE)
    {
        goto MajorBlock_1_Outer;
    }

/*
    END:  @@MajorBlock_1_Outer
*/

#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_GENERAL, "DEBUG: [%s, %d]: END: Move_Path_Find(wx = %d, wy = %d, movepath_cost_map = %p)", __FILE__, __LINE__, wx, wy, movepath_cost_map);
#endif

}
```
