





MoO2

Set_Legal_Moves_

Move_Cost_

Move_Ship_
Ship_Center_Offsets_
Ship_On_Screen_
Ship_Completely_On_Screen_

Range_To_XY_()
    |-> Range()

Do_Auto_Ship_Turn_
Does_Combat_Ship_Have_Special_
Used_Teleporter_


Module: PLNTSUM

    code (0 bytes) Nearest_Ship_Id_
    Address: 01:0009B287
        Locals:

    code (0 bytes) Send_Nearest_Ship_
    Address: 01:0009B2C7
        Locals:






# Stack Movement


Main Screen
Directional Keys
Left-Click Movement Map



¿ directional keys would assume only moving one map square ?
¿ left click would need to handle the map square clicked relative to the active stack's location ?

¿ any difference in the state for an active stack that has no movement points left ?




## Directional Keys:

calls IDK_UnitMoves_and_PlanarTravel(), with *direction*
    {1,2,3,4,6,7,8}
    {LeftDown,Down,RightDown,Left,Right,LeftUp,Up,RightUp}
    in order of numeric keypad keys, skips 5
calcs x, calcs y, wraps x, limits y, checks planar travel vs planes


// WZD o61p08
void IDK_StackPassable_s53D3F(int16_t * unit_count, int16_t * unit_array)
    populates an array of unit_idx and the count of elements in that array
    presumably, this is units that have the movement points and movement mode to allow them to make the move
    this would also accomodate transports - ships and floating island
        also, wind walker?
    ...sailing...
    ...flying...



## Left-Click Movement Map

if((input_field_idx == _main_map_grid_field) && (_unit_stack_count != 0) )





¿ OVL_GetStackHMoves() vs. STK_GetMovesLeft() ?
¿ ... vs. IDK_GetStackHMoves_s56B56() ?

ovr063
o64p12
OVL_GetStackHMoves()
    sets and returns STK_HMoves_Left

ovr063
o64p13
IDK_GetStackHMoves_s56B56()
    sets and returns STK_HMoves_Left

ovr097
o97p04
STK_GetMovesLeft()
    sets and returns STK_HMoves_Left



itrate on _unit_stack_count
index _unit_stack[]
test _unit_stack[].active
...if no units are active, gets set to zero...
...short-circuits on *transport*, takes its HMoves...
...tests 'Has Wind Walking', sets flag and unit_idx, takes its HMoves, but keeps looping...
otherwise, just keeps updating the minimum of _UNITS[unit_idx].HMoves and tmp_movement_points
Outcomes?
no active units, movement points is 0
if any 'Is Transport', movement points is that of the highest priority transport  (¿ and, transports always have highest priority, of 50 ?)
if any 'Has Wind Walking', non-sensical? only if last active unit in stack? or all units after happen to have higher movement points?
minimum movement points of active units


### Movement Direction Keys
calc x, calc y, wrap x, limit y
planar travel
call do move function


