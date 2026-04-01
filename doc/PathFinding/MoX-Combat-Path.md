
SEEALSO:  MoX-Combat-PathFindAlgo.md  C:\STU\devel\ReMoM\doc\PathFinding\

Two Use-Cases
1. Path and Cost
2. Valid Destination



Explain the use of the offset array and what the values do.
// WZD dseg:70C8                                                 BEGIN:  ovr155 - Initialized Data
//                                    NE     SW     SE     NW  North  South   West   East
int16_t CMB_AdjacentOffsets[8] = {   -20,    20,    22,   -22,   -(COMBAT_GRID_WIDTH),    (COMBAT_GRID_WIDTH),    -1,     1 };
int16_t CMB_AdjctOfs_NoWest[8] = {   -20, -1000,    22, -1000,   -(COMBAT_GRID_WIDTH),    (COMBAT_GRID_WIDTH), -1000,     1 };
int16_t CMB_AdjctOfs_NoEast[8] = { -1000,    20, -1000,   -22,   -(COMBAT_GRID_WIDTH),    (COMBAT_GRID_WIDTH),    -1, -1000 };
int16_t adjacent_offsets[24] =
{
      -20,    20,    22,   -22,   -(COMBAT_GRID_WIDTH),    (COMBAT_GRID_WIDTH),    -1,     1,
      -20, -1000,    22, -1000,   -(COMBAT_GRID_WIDTH),    (COMBAT_GRID_WIDTH), -1000,     1,
    -1000,    20, -1000,   -22,   -(COMBAT_GRID_WIDTH),    (COMBAT_GRID_WIDTH),    -1, -1000
};
This code snippet is the definitive key to how 1990s game developers optimized grid-based pathfinding. It reveals how they completely eliminated expensive 2D coordinate math (X and Y) from the innermost, most performance-heavy loops of the engine.
They achieved this by flattening the 2D combat map into a single 1D array and using these "Offset Arrays" to navigate it.
Here is the breakdown of exactly how this math works and why the NoWest and NoEast arrays exist.
1. The Core Concept: The 1D Grid
In previous context, we established that the combat grid is 21 tiles wide.
When you store a 2D grid in a computer's memory, it doesn't actually sit in a square; it sits in a straight line (a 1D array).
Tile X:0, Y:0 is index 0.
Tile X:20, Y:0 (the far right edge) is index 20.
Tile X:0, Y:1 (the left edge of the next row down) is index 21.
Because the grid is exactly 21 tiles wide, moving strictly "South" (down one row) means you simply add 21 to your current 1D index. Moving "North" means you subtract 21.
2. Decoding the CMB_AdjacentOffsets Math
If we look at the main CMB_AdjacentOffsets array, the values perfectly map the 8 directions of movement to flat array arithmetic based on COMBAT_GRID_WIDTH (21):
East: +1 (Move one index forward)
West: -1 (Move one index backward)
South: +21 (Move down one full row)
North: -21 (Move up one full row)
South-East: +22 (South +21 combined with East +1)
North-West: -22 (North -21 combined with West -1)
South-West: +20 (South +21 combined with West -1)
North-East: -20 (North -21 combined with East +1)
Instead of doing (Y * 21) + X to check neighbors during the flood-fill pathfinding algorithm, the engine just takes the current tile's 1D index and adds these 8 pre-calculated numbers.
3. The "Pac-Man" Wrapping Problem
Using a flat 1D array introduces a fatal visual bug: screen wrapping.
Imagine a unit is standing on the far-left edge of the map at X:0, Y:1 (which is 1D index 21). If the algorithm tries to check the tile to the West, it adds -1.
The resulting index is 20. But index 20 is the far-right edge of the row above them! The algorithm would essentially teleport the unit across the map like Pac-Man.
Normally, modern programmers fix this with if statements inside the loop:
if (current_x > 0) { /* Safe to check West */ }
But if statements inside a tight loop cause pipeline stalls on older CPUs, drastically slowing down the game.
4. The -1000 Bounds Check Hack
To solve the wrapping problem without using expensive if (x > 0) logic, the developers created the CMB_AdjctOfs_NoWest and CMB_AdjctOfs_NoEast arrays.
When the pathfinder processes a tile on the left edge of the map, it swaps its pointer to use the NoWest array. Notice what happens to the westward offsets in that array:
SW: -1000 (instead of 20)
NW: -1000 (instead of -22)
West: -1000 (instead of -1)
Why exactly -1000?
The maximum size of the combat grid is 462 tiles (indices 0 to 461). If you add -1000 to any valid tile index on the board, the result is guaranteed to be a massive negative number.
In the game's assembly code, immediately after applying an offset, there is an instruction that checks for a negative sign flag (like jl - Jump if Less than zero). The CPU's hardware instantly catches the negative number and skips the tile, effectively executing a map boundary check without doing any actual X/Y boundary math!






Tactical Combat

Move-Map
Move-Path


Combat vs. Overland
CMB_Path_Costs[]    ~==  mvpth_c[]
CMB_Path_Xs[]       ~==  mvpth_x[]
CMB_Path_Ys[]       ~==  mvpth_y[]
_cmbt_movepath_cost_map   ~==  movepath_cost_map->moves2[]

CMB_Path_Xs  ==>  _cmbt_mvpth_x
CMB_Path_Ys  ==>  _cmbt_mvpth_y
CMB_Path_Costs  ==>  _cmbt_mvpth_c
_cmbt_movepath_cost_map   ~==  _cmbt_movepath_cost_map



How to debug?

cgx,cgy
ctr

for(Y_Loop_Var = 0; Y_Loop_Var < Max_Y_Index; Y_Loop_Var++)

...at Y_Loop_Var == 12

Tile_Move_Cost = _cmbt_movepath_cost_map[ctr];
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

        _cmbt_path_data[itr] = itr;  /* ¿ reach from ? */





CMB_GetPath__WIP()
Combat_Move_Path_Valid()

What's the difference(s)?

Combat_Move_Path_Valid() does not set CMB_Path_Xs and CMB_Path_Ys
Combat_Move_Path_Valid() zeroes CMB_Path_Length, but does mpt set it back up



CMB_Path_Length

_cmbt_path_data[]



CMB_GetPath__WIP()
    sets CMB_Path_Length to zero
    does nothing else with it
    sets CMB_Path_Length to zero, again
    sets CMB_Path_Length based on _cmbt_path_data[]



Elsewhere, ...

CMB_TargetRows[]
value is used to choose the mouse image
indexing uses Get_Combat_Grid_Cell_X(), Get_Combat_Grid_Cell_Y(), with (Pointer_X() + 4), (Pointer_Y() + 4)
'Right-Click Combat Grid' uses it to get battle_unit_idx
