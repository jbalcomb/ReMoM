/*
    WIZARDS.EXE
        ovr147
            Move_Path_Find()
*/

#include "../../STU/src/STU_DBG.h"

#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOX_BASE.h"
#include "../../MoX/src/MOX_TYPE.h"

#include "MovePath.h"
#include "../../STU/src/STU_LOG.h"



// // Debugger Variables
// int8_t  dbg_wms0_reach_cost;
// int8_t  dbg_wms1_reach_cost;
// int8_t  dbg_wms2_reach_cost;
// int8_t  dbg_wms3_reach_cost;
// int16_t dbg_wms0_reach_from;
// int16_t dbg_wms1_reach_from;
// int16_t dbg_wms2_reach_from;
// int16_t dbg_wms3_reach_from;
// void Initialize_Debugger_Variable(void)
// {
//     dbg_wms0_reach_cost = movepath_cost_map->Reach_Costs[(((wy - 0) * WORLD_WIDTH) + (wx + 0))];
//     dbg_wms1_reach_cost = movepath_cost_map->Reach_Costs[(((wy - 1) * WORLD_WIDTH) + (wx + 1))];
//     dbg_wms2_reach_cost = movepath_cost_map->Reach_Costs[(((wy - 2) * WORLD_WIDTH) + (wx + 2))];
//     dbg_wms3_reach_cost = movepath_cost_map->Reach_Costs[(((wy - 3) * WORLD_WIDTH) + (wx + 3))];
//     dbg_wms0_reach_from = movepath_cost_map->Reach_From[(((wy - 0) * WORLD_WIDTH) + (wx + 0))];
//     dbg_wms1_reach_from = movepath_cost_map->Reach_From[(((wy - 1) * WORLD_WIDTH) + (wx + 1))];
//     dbg_wms2_reach_from = movepath_cost_map->Reach_From[(((wy - 2) * WORLD_WIDTH) + (wx + 2))];
//     dbg_wms3_reach_from = movepath_cost_map->Reach_From[(((wy - 3) * WORLD_WIDTH) + (wx + 3))];
// }



/*
    WIZARDS.EXE  ovr147
*/

// WZD o147p01

int16_t wx;
int16_t wy;
// MoM_Data  struct s_MOVE_PATH * movepath_cost_map;

int8_t    a_cost_was_updated = ST_TRUE; /* relaxation "changed" flag */  // WZD ovr147:0000  Code-Segment Variable
int16_t   origin_row = 0;               // WZD ovr147:0002  Code-Segment Variable
/* origin_row is the linear (1-D) array index of the first cell of the source tile's row — the anchor offset where each pathfinding sweep begins. */
int16_t   itr;                  /* array-init / generic loop var */
int8_t  * movepath_cost;
uint8_t * movepath_reach_cost;
int16_t * movepath_reach_from;
int16_t   ofst_movepath_cost;   /* current tile 1-D index */
int8_t    move_cost;            /* current tile entry cost */
int8_t    itr_row;    // _CL_
int8_t    incr_flag;  // _CH_
int8_t    tmp_move_cost;
uint8_t   adjacent_reach_cost;
uint8_t   new_cost_to_reach;       /* candidate new cost */
uint8_t   current_reach_cost;
int8_t    reach_cost;
int16_t   adj_pos;
int16_t   curr_wx;
int16_t   curr_wy;


static void Check_Cost(void)
{
    adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
    // if(adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
    if(!IS_INF(adjacent_reach_cost))
    {
        new_cost_to_reach = adjacent_reach_cost + tmp_move_cost;
        current_reach_cost = *movepath_reach_cost;
        if(new_cost_to_reach < current_reach_cost)
        {
            *movepath_reach_cost = new_cost_to_reach;
            *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
            a_cost_was_updated = ST_TRUE;
        }
    }
}
// Handle Special Case:  (x = 0)  wrap around backward
static void Do_Costs_Fst(void)
{
    // if(ofst_movepath_cost == (((wy - 1) * WORLD_WIDTH) + (wx + 1))) { MOX_DBG_BREAK; }
    move_cost = *movepath_cost++;
    ofst_movepath_cost++;
    if(move_cost != (int8_t)INF)
    {
        incr_flag++;
        tmp_move_cost = move_cost;
        // North-West
        adj_pos = -1;
        Check_Cost();
        // North
        adj_pos = -60;
        Check_Cost();
        // North-East
        adj_pos = -59;
        Check_Cost();
        // West
        adj_pos = 59;
        Check_Cost();
        // East
        adj_pos = 1;
        Check_Cost();
        // South-West
        adj_pos = 119;
        Check_Cost();
        // South
        adj_pos = 60;
        Check_Cost();
        // South-East
        adj_pos = 61;
        Check_Cost();
    }
    movepath_reach_cost++;
    movepath_reach_from++;
}
// Handle Normal Cases:  x = {1, ..., 59}
static void Do_Costs_Mid(void)
{
    // if(ofst_movepath_cost == (((wy - 1) * WORLD_WIDTH) + (wx + 1))) { MOX_DBG_BREAK; }
    move_cost = *movepath_cost++;
    ofst_movepath_cost++;
    if(move_cost != (int8_t)INF)
    {
        incr_flag++;
        tmp_move_cost = move_cost;
        // North-West
        adj_pos = -61;
        Check_Cost();
        // North
        adj_pos = -60;
        Check_Cost();
        // North-East
        adj_pos = -59;
        Check_Cost();
        // West
        adj_pos = -1;
        Check_Cost();
        // East
        adj_pos = 1;
        Check_Cost();
        // South-West
        adj_pos = 59;
        Check_Cost();
        // South
        adj_pos = 60;
        Check_Cost();
        // South-East
        adj_pos = 61;
        Check_Cost();
    }
    movepath_reach_cost++;
    movepath_reach_from++;
}
// Handle Special Case:  (x = 59)  wrap around forward
static void Do_Costs_Lst(void)
{
    // if(ofst_movepath_cost == (((wy - 1) * WORLD_WIDTH) + (wx + 1))) { MOX_DBG_BREAK; }
    move_cost = *movepath_cost++;
    ofst_movepath_cost++;
    if(move_cost != (int8_t)INF)
    {
        incr_flag++;
        tmp_move_cost = move_cost;
        // North-West
        adj_pos = -61;
        Check_Cost();
        // North
        adj_pos = -60;
        Check_Cost();
        // North-East
        adj_pos = -119;
        Check_Cost();
        // West
        adj_pos = -1;
        Check_Cost();
        // East
        adj_pos = -59;
        Check_Cost();
        // South-West
        adj_pos = 59;
        Check_Cost();
        // South
        adj_pos = 60;
        Check_Cost();
        // South-East
        adj_pos = 1;
        Check_Cost();
    }
    movepath_reach_cost++;
    movepath_reach_from++;
}

/*
    Structure map — see doc/PathFinding/MoM-MovePath-Compare.md

    Overland shortest-path solver (live). Of the shared 5-step skeleton (that doc, "The shared skeleton"),
    this function implements only steps 1 and 3; the destination-impassable bail (step 2), the back-trace
    (step 4), and the reverse + (x,y) conversion (step 5) live in the caller Make_Move_Path (UnitMove.c).
    The inline [Skeleton step N] labels below mark each section:

        [Skeleton 1]  init parallel arrays: Reach_Costs[] = INF, Reach_From[] = self, source = 0
        [Skeleton 3]  relaxation sweep to fixed point (while a_cost_was_updated) — see the doc's
                      "Optimization" note: each outer pass runs a downward row pass (Iter_Rows_Down)
                      then an upward row pass (Iter_Rows_Up), the alternating-raster (Gauss-Seidel)
                      acceleration that propagates costs both with and against the scan.
*/
void Move_Path_Find(int16_t arg_wx, int16_t arg_wy, struct s_MOVE_PATH * arg_movepath_cost_map)
{
    wx = arg_wx;
    wy = arg_wy;
    movepath_cost_map = arg_movepath_cost_map;

    /* [Skeleton step 1]  Two parallel arrays: Reach_Costs[] init INF, Reach_From[] init to self, source = 0 (MoM-MovePath-Compare.md, "The shared skeleton"). origin_row anchors each sweep at the source's row. */
    origin_row = (wy * WORLD_WIDTH);
    for(itr = 0; itr < WORLD_SIZE; itr++)
        movepath_cost_map->Reach_Costs[itr] = INF;
    for(itr = 0; itr < WORLD_SIZE; itr++)
        movepath_cost_map->Reach_From[itr] = itr;
    movepath_cost_map->Reach_Costs[((wy * WORLD_WIDTH) + wx)] = 0;

    /* [Skeleton step 3]  Relaxation sweep to fixed point — alternating down/up raster passes (Gauss-Seidel), repeated while a_cost_was_updated (MoM-MovePath-Compare.md, "Move_Path_Find — overland"). */
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

/*
    BEGIN:  @@Iter_Rows_Down
*/
    /* [Skeleton step 3 — sweep 1 of 2]  Downward row pass (top -> bottom raster). */
Iter_Rows_Down:

    incr_flag = 0;
    Do_Costs_Fst();
    itr_row = 29;
    while(itr_row != 0)
    {
        Do_Costs_Mid();
        Do_Costs_Mid();
        itr_row--;
    }
    Do_Costs_Lst();

    if((incr_flag != 0) && (ofst_movepath_cost < 2320))
    {
        goto Iter_Rows_Down;
    }
/*
    END:  @@Iter_Rows_Down
*/



/*
    BEGIN:  @@Iter_Rows_Up
*/

    /* [Skeleton step 3 — sweep 2 of 2]  Upward row pass (bottom -> top raster; alternating direction is the Gauss-Seidel acceleration). */
    ofst_movepath_cost = origin_row;
    movepath_cost = &movepath_cost_map->moves2[origin_row];
    movepath_reach_cost = &movepath_cost_map->Reach_Costs[origin_row];
    movepath_reach_from = &movepath_cost_map->Reach_From[origin_row];


Iter_Rows_Up:
    incr_flag = 0;
    Do_Costs_Fst();
    itr_row = 29;
    while(itr_row != 0)
    {
        Do_Costs_Mid();
        Do_Costs_Mid();
        itr_row--;
    }
    Do_Costs_Lst();

    movepath_cost       -= 120;
    ofst_movepath_cost  -= 120;
    movepath_reach_cost -= 120;
    movepath_reach_from -= 120;

    if((incr_flag != 0) && (ofst_movepath_cost >= 60))
    {
        goto Iter_Rows_Up;
    }

/*
    END:  @@Iter_Rows_Up
*/

    /* [Skeleton step 3 — fixed-point gate]  Repeat the down+up macro-pass until a full sweep changes nothing. */
    if(a_cost_was_updated == ST_TRUE)
    {
        goto MajorBlock_1_Outer;
    }

/*
    END:  @@MajorBlock_1_Outer
*/

    /* Steps 2 (destination-impassable bail), 4 (back-trace Reach_From[] -> source), and 5 (reverse + (x,y)) are done by the caller Make_Move_Path (UnitMove.c). */
}
