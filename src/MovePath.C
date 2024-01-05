/*
    WIZARDS.EXE
        ovr147
*/

#include "MoX.H"



#define CHECK_COST(_adj_) {                                             \
    adjacent_reach_cost = *(movepath_reach_cost + _adj_);               \
    if((adjacent_reach_cost + tmp_move_cost) >= 0)                       \
    {                                                                   \
        adjacent_reach_cost += tmp_move_cost;                           \
        if(adjacent_reach_cost < *movepath_reach_cost)                  \
        {                                                               \
            *movepath_reach_cost = new_reach_cost;                 \
            *movepath_reach_from = (ofst_movepath_cost + (_adj_ - 1));  \
            CS_Value_Change = ST_TRUE;                                  \
        }                                                               \
    }                                                                   \
}


#define DO_COSTS_FIRST() {          \
    move_cost = *movepath_cost++; \
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
    move_cost = *movepath_cost++; \
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
    move_cost = *movepath_cost++; \
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
void Move_Path_Find(int16_t wx, int16_t wy, struct s_MOVE_PATH * movepath_cost_map)
{
    int8_t  CS_Value_Change = ST_TRUE;  // WZD ovr147:0000  Code-Segment Variable
    int16_t CS_Row_Start = 0;           // WZD ovr147:0002  Code-Segment Variable
    int16_t itr;
    int8_t * movepath_cost;
    int8_t * movepath_reach_cost;
    int16_t * movepath_reach_from;
    int16_t ofst_movepath_cost;
    int8_t move_cost;
    int8_t itr_row;  // _CL_
    int8_t incr_flag;  // _CH_
    int8_t tmp_move_cost;
    int8_t adjacent_reach_cost;
    int8_t reach_cost;
    int16_t adj_pos;
    int8_t new_reach_cost;
    int8_t current_reach_cost;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Move_Path_Find(wx = %d, wy = %d, movepath_cost_map = %p)\n", __FILE__, __LINE__, wx, wy, movepath_cost_map);
#endif

    // start of first map grid row
    CS_Row_Start = (wy * WORLD_WIDTH);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: CS_Row_Start: %d\n", __FILE__, __LINE__, CS_Row_Start);
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
    dbg_prn("DEBUG: [%s, %d]: movepath_cost_map->Reach_Costs[((wy * WORLD_WIDTH) + wx)]: %d\n", __FILE__, __LINE__, movepath_cost_map->Reach_Costs[((wy * WORLD_WIDTH) + wx)]);
#endif


/*
    BEGIN:  @@MajorBlock_1_Outer
*/

    // ¿ do{ ...} while(CS_Value_Change == ST_TRUE) ?

MajorBlock_1_Outer:

    CS_Value_Change = ST_FALSE;
    ofst_movepath_cost = CS_Row_Start;
    movepath_cost = &movepath_cost_map->moves2[CS_Row_Start];
    movepath_reach_cost = &movepath_cost_map->Reach_Costs[CS_Row_Start];
    movepath_reach_from = &movepath_cost_map->Reach_From[CS_Row_Start];
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: ofst_movepath_cost: %d\n", __FILE__, __LINE__, ofst_movepath_cost);
    dbg_prn("DEBUG: [%s, %d]: &movepath_cost_map->moves2[CS_Row_Start]: %p\n", __FILE__, __LINE__, movepath_cost);
    dbg_prn("DEBUG: [%s, %d]: &movepath_cost_map->Reach_Costs[CS_Row_Start]: %p\n", __FILE__, __LINE__, movepath_reach_cost);
    dbg_prn("DEBUG: [%s, %d]: &movepath_cost_map->Reach_From[CS_Row_Start]: %p\n", __FILE__, __LINE__, movepath_reach_from);
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
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)  // ~ non-negative
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // North
        adj_pos = -60;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // North-East
        adj_pos = -59;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // West
        adj_pos = 59;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // East
        adj_pos = 1;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // South-West
        adj_pos = 119;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // South
        adj_pos = 60;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // South-East
        adj_pos = 61;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
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
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // North
            adj_pos = -60;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // North-East
            adj_pos = -59;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // West
            adj_pos = -1;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // East
            adj_pos = 1;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // South-West
            adj_pos = 59;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // South
            adj_pos = 60;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // South-East
            adj_pos = 61;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
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
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // North
            adj_pos = -60;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // North-East
            adj_pos = -59;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // West
            adj_pos = -1;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // East
            adj_pos = 1;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // South-West
            adj_pos = 59;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // South
            adj_pos = 60;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // South-East
            adj_pos = 61;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
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
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // North
        adj_pos = -60;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // North-East
        adj_pos = -119;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // West
        adj_pos = -1;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // East
        adj_pos = -59;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // South-West
        adj_pos = 59;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // South
        adj_pos = 60;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // South-East
        adj_pos = 1;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
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

    ofst_movepath_cost = CS_Row_Start;
    movepath_cost = &movepath_cost_map->moves2[CS_Row_Start];
    movepath_reach_cost = &movepath_cost_map->Reach_Costs[CS_Row_Start];
    movepath_reach_from = &movepath_cost_map->Reach_From[CS_Row_Start];
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: ofst_movepath_cost: %d\n", __FILE__, __LINE__, ofst_movepath_cost);
    dbg_prn("DEBUG: [%s, %d]: &movepath_cost_map->moves2[CS_Row_Start]: %p\n", __FILE__, __LINE__, movepath_cost);
    dbg_prn("DEBUG: [%s, %d]: &movepath_cost_map->Reach_Costs[CS_Row_Start]: %p\n", __FILE__, __LINE__, movepath_reach_cost);
    dbg_prn("DEBUG: [%s, %d]: &movepath_cost_map->Reach_From[CS_Row_Start]: %p\n", __FILE__, __LINE__, movepath_reach_from);
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
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // North
        adj_pos = -60;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // North-East
        adj_pos = -59;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // West
        adj_pos = 59;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // East
        adj_pos = 1;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // South-West
        adj_pos = 119;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // South
        adj_pos = 60;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // South-East
        adj_pos = 61;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
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
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // North
            adj_pos = -60;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // North-East
            adj_pos = -59;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // West
            adj_pos = -1;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // East
            adj_pos = 1;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // South-West
            adj_pos = 59;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // South
            adj_pos = 60;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // South-East
            adj_pos = 61;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
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
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // North
            adj_pos = -60;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // North-East
            adj_pos = -59;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // West
            adj_pos = -1;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // East
            adj_pos = 1;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // South-West
            adj_pos = 59;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // South
            adj_pos = 60;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
                    }
                }
            }
            // South-East
            adj_pos = 61;
            {
                adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
                if(new_reach_cost >= 1)
                {
                        current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                    {
                        *movepath_reach_cost = new_reach_cost;
                        *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                        CS_Value_Change = ST_TRUE;
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
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // North
        adj_pos = -60;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // North-East
        adj_pos = -119;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // West
        adj_pos = -1;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // East
        adj_pos = -59;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // South-West
        adj_pos = 59;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // South
        adj_pos = 60;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
                }
            }
        }
        // South-East
        adj_pos = 1;
        {
            adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
            // if (adjacent_reach_cost == 0) { __debugbreak(); }
            if(adjacent_reach_cost == -1) { adjacent_reach_cost = 1; }
            new_reach_cost = adjacent_reach_cost + tmp_move_cost + 1;
            if(new_reach_cost >= 1)
            {
                current_reach_cost = *movepath_reach_cost;
                // if (current_reach_cost == 0) { __debugbreak(); }
                if(new_reach_cost < (uint8_t)current_reach_cost)
                {
                    *movepath_reach_cost = new_reach_cost;
                    *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                    CS_Value_Change = ST_TRUE;
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

    if(CS_Value_Change == ST_TRUE)
    {
        goto MajorBlock_1_Outer;
    }

/*
    END:  @@MajorBlock_1_Outer
*/

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Move_Path_Find(wx = %d, wy = %d, movepath_cost_map = %p)\n", __FILE__, __LINE__, wx, wy, movepath_cost_map);
#endif

}
