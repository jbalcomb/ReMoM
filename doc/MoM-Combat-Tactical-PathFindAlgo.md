
SEEALSO:  dijkstra.c





CMB_GetPath() ==> CMB_GetPath__WIP()
CMB_GetPath__WIP() ==> Combat_Move_Path_Find()
CMB_FillReachMap() ==> CMB_FillReachMap__WIP()
CMB_FillReachMap__WIP() ==> Combat_Move_Path_Valid()
Adj_Tile_Cost ==> adjacent_path_cost
Cost_As_Origin ==> potential_path_cost

Current_Origin ==> old_next_cell_index

existing_path_cost ==> DNE in Dasm
if(CMB_Path_Costs[ctr] > potential_path_cost)
existing_path_cost = CMB_Path_Costs[ctr];
if(existing_path_cost > potential_path_cost)

new_next_cell_index ===> DNE in Dasm


CMB_Path_Costs@
Tile_GetTo_Cost


movepath_cost = &movepath_cost_map->moves2[CS_Row_Start];
move_cost = *movepath_cost++;
vs.
move_cost = CMB_ActiveMoveMap[ctr];



Eh. `Combat_Move_Path_Find()` and `Combat_Move_Path_Valid()`?
... `Combat_Move_Path()` and `Combat_Move_Valid()`?



Combat.C    CMB_GetPath__WIP()
MovePath.C  Move_Path_Find()

loop until there are no changes to ...?

    movepath_cost = &movepath_cost_map->moves2[CS_Row_Start];
    movepath_reach_cost = &movepath_cost_map->Reach_Costs[CS_Row_Start];
    movepath_reach_from = &movepath_cost_map->Reach_From[CS_Row_Start];

        new_reach_cost = adjacent_reach_cost + tmp_move_cost;
        current_reach_cost = *movepath_reach_cost;
        if(new_reach_cost < current_reach_cost)
        {
            *movepath_reach_cost = new_reach_cost;
            *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
            reach_costs_changed = ST_TRUE;
        }

Are we checking to see which adjacent cell has the lowest cost?





What would you imagine it might get up to for accomplishing the task?

I am at 'A' and I want to get to 'Z'.
There are 8 adjacent cells.
Which of those has the lowest cost?
...


How do I know which way to go?
No such thing?
The whole algo starts at 0 {0,0} and just runs until there's no change.
So, it's just the minimum length from every cell to every other cell?

CMB_NearBuffer_3[] starts with every cell is best reached from itself
CMB_Path_Costs[] starts with every cell has infinite reach cost, except the source cell, which is zero

gets the moves2 cost from CMB_ActiveMoveMap[], given the terrain (group?) and the battle unit's movement mode.
starting from ctr = 0, which would be {0,0}
checks if INF/*impassible*


So, at `adjacent_path_cost = CMB_Path_Costs[adjacent_idx];`
first-run: CMB_Path_Costs[] is all 255/INF, except the source
Then?
It iterates over the adjacent cells?
    NE, SW, SE, NW
    North, South, West, East

So, the first update would take place when it hits the cell where the source cell is an adjacent cell.
Given the linear array accessing, that would be the cell at {-1,-1} from the source cell, at SE in the adjacent cell iteration.
Every cell adjacent to the source cell is going to get it's path cost updated to 0 plus its own movement cost.
Spreading out from that epicenter, every cell will get its 'next cell' updated to the adjacent cell with the lowest cumulative cost, from within the portion of the wave within its purview.




same for each section
                    if(GTELT(adjacent_idx, 0, COMBAT_GRID_CELL_COUNT))
                    {
                        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                        if(!IS_INF(adjacent_path_cost))
                        {
                            potential_path_cost = adjacent_path_cost + move_cost;
                            existing_path_cost = CMB_Path_Costs[ctr];
                            if(existing_path_cost > potential_path_cost)
                            {
                                CMB_NearBuffer_3[ctr] = adjacent_idx;
                                CMB_Path_Costs[ctr] = potential_path_cost;
                                new_next_cell_index = CMB_NearBuffer_3[ctr];
                                if(new_next_cell_index != old_next_cell_index)
                                {
                                    unstable = ST_TRUE;
                                }
                            }
                        }
                    }

first difference between first,middle,last
adjacent_idx = (ctr + CMB_AdjctOfs_NoEast[itr_adjacent]);
...could just pass a parameter of {0,1,2} for the adjcent offsets array indexing

difference between Valid and Find
    for(itr_adjacent = 0; itr_adjacent < 8; itr_adjacent++)
Valid does 0 to 8
Find does 0 to 4 and 4 to 8
...could just pass a parameter for start and finish







                    if(adjacent_idx == ((DBG_cgy * 21) + DBG_cgx))
                    {
                        __debugbreak();
                    }
                    // if((adjacent_idx >= 0) && (adjacent_idx < 462))
                    if(GTELT(adjacent_idx, 0, 462))
                    {
                        // adjacent_reach_cost = *(movepath_reach_cost + (adj_pos));
                        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                        // if(adjacent_reach_cost != 255)
                        // if(adjacent_path_cost != 255)
                        if(!IS_INF(adjacent_path_cost))
                        {
                            // new_reach_cost = adjacent_reach_cost + tmp_move_cost;
                            // current_reach_cost = *movepath_reach_cost;
                            potential_path_cost = adjacent_path_cost + move_cost;
                            // if(new_reach_cost < current_reach_cost)
                            if(CMB_Path_Costs[ctr] > potential_path_cost)
                            {
                                // *movepath_reach_from = (ofst_movepath_cost + ((adj_pos) - 1));
                                CMB_NearBuffer_3[ctr] = adjacent_idx;
                                // *movepath_reach_cost = new_reach_cost;
                                CMB_Path_Costs[ctr] = potential_path_cost;
                                if(CMB_NearBuffer_3[ctr] != old_next_cell_index)
                                {
                                    // reach_costs_changed = ST_TRUE;
                                    unstable = ST_TRUE;
                                }
                            }
                        }
                    }

                    if((adjacent_idx >= 0) && (adjacent_idx < 462))
                    {
                        adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
                        if(adjacent_path_cost != 255)
                        {
                            potential_path_cost = adjacent_path_cost + move_cost;
                            if(CMB_Path_Costs[ctr] > potential_path_cost)
                            {
                                CMB_NearBuffer_3[ctr] = adjacent_idx;
                                CMB_Path_Costs[ctr] = potential_path_cost;
                                if(CMB_NearBuffer_3[ctr] != old_next_cell_index)
                                {
                                    unstable = ST_TRUE;
                                }
                            }
                        }
                    }

// adjacent_idx = CMB_AdjctOfs_NoWest[itr_adjacent];
// adjacent_idx = CMB_AdjctOfs_NoWest[itr_adjacent];
// adjacent_idx = CMB_AdjacentOffsets[itr_adjacent];
// adjacent_idx = CMB_AdjacentOffsets[itr_adjacent];
// adjacent_idx = CMB_AdjctOfs_NoEast[itr_adjacent];
// adjacent_idx = CMB_AdjctOfs_NoEast[itr_adjacent];

if((adjacent_idx >= 0) && (adjacent_idx < 462))
{
    adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
    if(adjacent_path_cost != 255)
    {
        potential_path_cost = adjacent_path_cost + move_cost;
        if(CMB_Path_Costs[ctr] > potential_path_cost)
        {
            CMB_NearBuffer_3[ctr] = adjacent_idx;
            CMB_Path_Costs[ctr] = potential_path_cost;
            if(CMB_NearBuffer_3[ctr] != old_next_cell_index)
            {
                unstable = ST_TRUE;
            }
        }
    }
}





for the source cell, on the first pass, at {-1,-1}

move_cost = CMB_ActiveMoveMap[ctr];
this is 2

old_next_cell_index = CMB_NearBuffer_3[ctr];
by default, this is the array index of itself / the current cell

adjacent_idx = CMB_AdjacentOffsets[itr_adjacent];
this is the array index of the source cell

adjacent_path_cost = CMB_Path_Costs[adjacent_idx];
this is 0

potential_path_cost = adjacent_path_cost + move_cost;
this is (0 + 2)

if(CMB_Path_Costs[ctr] > potential_path_cost)
this is (INF > 2)
...the current cells path cost was set to INF beforehand

CMB_NearBuffer_3[ctr] = adjacent_idx;
this sets the current cells 'reach from' index to that of the source cell

CMB_Path_Costs[ctr] = potential_path_cost;
this changes the current cells path cost from INF to (0 + 2)

if(CMB_NearBuffer_3[ctr] != old_next_cell_index)
    unstable = ST_TRUE;

@`if(CMB_Path_Costs[ctr] > potential_path_cost)`
potential_path_cost is new/next/adjacent path cost  ... adjacent cells path cost plus current cells movement cost


if (length[u] + weight[u][i] < length[i])
In my cool new 1994 ref-mat, that is seemingly, somehow the equivalent of testing the current cells path cost against the adjacent cells path cost plus the current cells movement cost.



So, just at `Combat_Move_Path_Find() |-> NEW_PATH_COST_ALL() |> NEW_PATH_COST_ANY() ... adjacent_idx`
...where adjacent_idx == ((DBG_cgy * 21) + DBG_cgx)

What do I want to see?
What do I get out of it?

`if((itr_i == (DBG_cgy - 1)) && (itr_j == (DBG_cgx -1))) { __debugbreak(); }`
...outside the macros
...in Combat_Move_Path_Valid()



move_cost is 1

old_next_cell_index is 245
245 / 21 = 11.66666
11 * 21 = 231
245 - 231 = 14
{14,11}

old_next_cell_index is 286
286 / 21 = 13.61905
13 * 21 = 273
286 - 273 = 13
{13,13}

adjacent_idx is 266
266 / 21 = 12.66666
12 * 21 = 252
266 - 252 = 14
{14,12}

adjacent_path_cost is 0

potential_path_cost is 1


source cell is {14,12}
current cell is {13,11}

So, ...
adjacent_idx is source cell
move_cost is 1
adjacent_path_cost is 1

potential_path_cost = adjacent_path_cost + move_cost;

...we can test on the cell index of the iterator, the cell index of the adjacent cell, the adjacent path cost of 0, and/or the potential path cost of 1...

set the debug test flag when
    DBG_PFA_1311_set = ST_TRUE;
    DBG_PFA_1411_set = ST_TRUE;
    DBG_PFA_1511_set = ST_TRUE;
ctr == {13,11}
ctr == {14,11}
ctr == {15,11}
WhereWhen?


if(existing_path_cost > potential_path_cost)
vs.
if(new_next_cell_index != old_next_cell_index)


existing_path_cost = CMB_Path_Costs[ctr];
...on the first pass, this should be INF
CMB_Path_Costs[ctr] = potential_path_cost;
...on the first pass, that should set it to 1

first break should be at
ctr == 244
adjacent_idx == 266



offsets array indexing
first section sequence
{   0,  21,  42,  63,  84, 105, 126, 147, 168, 189, 210, 231, 252, 273, 294, 315, 336, 357, 378, 399 }
{}
{  20,  41,  62,  83, 104, 125, 146, 167, 188, 209, 230, 251, 272, 293, 314, 335, 356, 377, 398, 419 }

WTF!!!!!
potential_path_cost is -47
existing_path_cost is 66
move_cost is -51
adjacent_idx is 350
ctr is 370
adjacent_path_cost is 4


CMB_Terrain_Init__WIP() |-> Generate_Combat_Map__WIP() |-> Set_Movement_Cost_Maps()







How do I test for when it should be stable?
Maybe just test for cells being updated after they have the value I know they should end up with?
e.g., set a flag when {14,11} gets set to 1, then break if it branches into updating the value for that cell again. 
Pretty sure that logic holds, as no other cell should have a cost equal to or less than the source cell's cost of 0.
So, all eight cells adjacent to the source cell should get one update - from INF to 1 - and never change again.

Where? When? How?

...current cell gets checked/updated, based on adjacent cell...
...adjacent cell is source cell...
ctr = (((DBG_cgy + -1) * COMBAT_GRID_WIDTH) + (DBG_cgx + -1)))
ctr = (((DBG_cgy + -1) * COMBAT_GRID_WIDTH) + (DBG_cgx +  0)))
ctr = (((DBG_cgy + -1) * COMBAT_GRID_WIDTH) + (DBG_cgx +  1)))
if(adjacent_idx == ((DBG_cgy * COMBAT_GRID_WIDTH) + DBG_cgx))


movement cost of the current cell
index of the adjacent cell
path cost of the adjacent cell
potential path cost of the current cell
existing path cost of the current cell

potential_path_cost
existing_path_cost





Keeps looping while 'changed'...
...but, what really/actually changed?
    if(CMB_NearBuffer_3[ctr] != old_next_cell_index)
    ...
    CMB_NearBuffer_3[ctr] = adjacent_idx;
    potential_path_cost = adjacent_path_cost + move_cost;
    ...
        where, ...
            CMB_NearBuffer_3[ctr] = adjacent_idx;
            ...means 


old_next_cell_index = CMB_NearBuffer_3[ctr];
potential_path_cost = adjacent_path_cost + move_cost;
if(CMB_Path_Costs[ctr] > potential_path_cost)
    CMB_NearBuffer_3[ctr] = adjacent_idx;
    CMB_Path_Costs[ctr] = potential_path_cost;
    if(CMB_NearBuffer_3[ctr] != old_next_cell_index)











path length vs. path cost

"length" is the sum of the *cost* of the edges




shortest path
based on movement cost

What's with the 'Reach From' array?
sets every cell to be reachable from itself?


According to a document from 1996, part of my problem is that I am using the wrong definition of "length".
Shortest path is: 0 2 5 4
Length of shortest path is 13.000000
length is the sum of the weights of the edges

'number of vertices' is the number of cells in the grid
'terminal vertex' is the id of the destination cell








z,                /* terminal vertex */
would be the array index for the destination cell?



u,                /* minimal vertex */
DEF: "minimal vertex"?



/* get input */
n = get_number_of_vertices();
z = get_terminal_vertex(n);

n = number_of_vertices = (22 * 21) = 462
z = terminal_vertex = ((target_cgy * 21) + target_cgx)



-1
255
0xFF
INF
infinity




adj_pos = -60;
