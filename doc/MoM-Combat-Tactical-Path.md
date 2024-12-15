

Tactical Combat

Move-Map
Move-Path





How to debug?

cgx,cgy
ctr

for(Y_Loop_Var = 0; Y_Loop_Var < Max_Y_Index; Y_Loop_Var++)

...at Y_Loop_Var == 12

Tile_Move_Cost = CMB_ActiveMoveMap[ctr];
...what is ctr?


logic for iteration over CMB_Path_Costs[]
is first column, middle columns, last column?

break on ctr = ((cgy * 21) + cgx)
on current cell
and NW target cell
12 * 21 = 252 + 14 = 266
11 * 21 = 231 + 13 = 244

DBG_move_cost_1 == 255
DBG_move_cost_2 ==   1

DBG_move_cost_1 == 255
DBG_move_cost_2 ==   1
DBG_path_cost_1 ==   0
DBG_path_cost_2 == 255



Do_Costs_Fst()
    move_cost = *movepath_cost++;
    if(move_cost != -1)
    {
        incr_flag++;
        tmp_move_cost = move_cost;
        // North-West
        adj_pos = -1;
        Check_Cost();

void Check_Cost(void)
{
    adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
    // if (adjacent_reach_cost == 0) { MOX_DBG_BREAK; }
    if(adjacent_reach_cost != 255)
    {
        new_reach_cost = adjacent_reach_cost + tmp_move_cost;
        current_reach_cost = *movepath_reach_cost;
        if(new_reach_cost < current_reach_cost)
        {
            *movepath_reach_cost = new_reach_cost;
            *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
            reach_costs_changed = ST_TRUE;
        }
    }
}


    for(itr = 0; itr < WORLD_SIZE; itr++)
        movepath_cost_map->Reach_From[itr] = itr;
    for(itr = 0; itr < 462; itr++)
    {

        CMB_NearBuffer_3[itr] = itr;  /* ¿ reach from ? */





CMB_GetPath__WIP()
Combat_Move_Path_Valid()

What's the difference(s)?

Combat_Move_Path_Valid() does not set CMB_Path_Xs and CMB_Path_Ys
Combat_Move_Path_Valid() zeroes CMB_Path_Length, but does mpt set it back up



CMB_Path_Length

CMB_NearBuffer_3[]



CMB_GetPath__WIP()
    sets CMB_Path_Length to zero
    does nothing else with it
    sets CMB_Path_Length to zero, again
    sets CMB_Path_Length based on CMB_NearBuffer_3[]



Elsewhere, ...

CMB_TargetRows[]
value is used to choose the mouse image
indexing uses Get_Combat_Grid_Cell_X(), Get_Combat_Grid_Cell_Y(), with (Pointer_X() + 4), (Pointer_Y() + 4)
'Right-Click Combat Grid' uses it to get battle_unit_idx
