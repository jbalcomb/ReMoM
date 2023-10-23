
!!!!! renamed !!!!!
Do_Move_Stack_DirKey() |-> Do_Move_Stack() |-> Move_Stack() |-> Move_Units()
Move_Stack_DirKey()    |-> Move_Stack()    |-> Move_Units() |-> Move_Units_Draw()

// WZD o57p10
// AKA Do_Move_Stack_DirKey()
void Move_Stack_DirKey(int16_t movement_direction);

// WZD o61p02
// AKA Do_Move_Stack()
int16_t Move_Stack(int16_t move_x, int16_t move_y, int16_t player_idx, int16_t * map_x, int16_t * map_y, int16_t * map_p);

// WZD o95p01
// AKA Move_Stack()
int16_t Move_Units(int16_t player_idx, int16_t destination_x, int16_t destination_y, int16_t Spec, int16_t * map_x, int16_t * map_y, int16_t map_p, int16_t unit_array_count, int16_t unit_array[])

// WZD o95p02
// AKA OVL_MoveUnitStack()
// AKA Move_Units()
void Move_Units_Draw(int16_t player_idx, int16_t map_p, int16_t Path_Length, int16_t * map_x, int16_t * map_y, int16_t unit_array[], int16_t unit_array_count)





Do_Move_Stack_DirKey()
Do_Move_Stack()
Move_Stack()
Move_Units()


...
Move_Stack()
    STK_GetPath
    STK_EvaluatePath()
    Move_Units()

...
Do_Move_Stack()
    Move_Stack()
    WIZ_NextIdleStack()

...
Move_Stack()
    ¿ Path_Length >= 1 ... Path_Length <= 1 ?
        ~== Path_Length == 1




MovePath_X
vs.
IDK_MovePath_Y


destination_x, destination_y

Move_From_X, Move_From_Y

Next_Tile_X, Next_Tile_Y


destination_x = *(Scd_Dst_X + Path_Length)
destination_y = *( (IDK_MovePath_Y + 1) + Path_Length)
¿ ~== (MovePath_Y - 2) + 1 + path_length ?
¿ ~== MovePath_Y + (path_length - 1) ?

unit_x = _UNITS[unit_idx].world_x
unit_t = _UNITS[unit_idx].world_y
...
Move_From_X = unit_x
Move_From_Y = unit_y


Next_Tile_X = *(MovePath_X + itr_path_length)
Next_Tile_Y = *( (IDK_MovePath_Y + 2) + itr_path_length)
{MovePath_X[0], ..., MovePath_X[path_length-1]}
with a path length of 1, it will only get one value
and, here, next = dst = MovePath[0]


at the end of the path length loop
Move_From_X = [          MovePath_X + _SI_itr__steps__units]
Move_From_Y = [(IDK_MovePath_Y + 2) + _SI_itr__steps__units]
so, here, curr = next

so, ...
    Move_From_X is current source world x
    Next_Tile_X is current destination world x
    they are not used for anything to do with the unit movement
    Move_From_X
        used for unit_pict_sx for the call to Draw_Unit_Picture()
        used for Cur_Tile_Left for the call to OVL_DrawMapSection()
    Next_Tile_X
        set from MovePath_X
        compared to unit_x to determined unit_pict_shift_sx
        used for Next_Tile_Left for the call to OVL_DrawMapSection()


Move_From_X     curr_src_wx
Move_From_Y     curr_src_wy

Next_Tile_X     curr_dst_wx
Next_Tile_Y     curr_dst_wy

Cur_Tile_Left   curr_src_sx
Cur_Tile_Top    curr_src_sy

Next_Tile_Left   curr_dst_sx
Next_Tile_Top    curr_dst_sy











Re-Group
20231019

Do_Move_Stack_DirKey()
    |-> Do_Move_Stack()
        |-> Build_Moveable_Stack()
        |-> Move_Stack()
            |-> Set_Draw_Active_Stack_Always(); Reset_Map_Draw();
            |-> Units_Moves()
            |-> Stack_Movement_Modes()
            |-> STK_GetLandlubbers()
            |-> STK_GetPath()
            |-> STK_EvaluatePath()
            |-> 
            |-> 
        |-> Select_Unit_Stack()
        |-> Stack_Moves()
        |-> WIZ_NextIdleStack()
        |-> Set_Unit_Draw_Priority(); Reset_Stack_Draw_Priority(); Set_Entities_On_Map_Window(); Reset_Draw_Active_Stack();






What does it do?
When, where, why would you call it?


## PoI >>> arrays of x, y, and cost

Move_Units()
    REQUIRES

    Move_Stack()
        |-> STK_GetPath()
            populates the passed in IDK_MovePath_DestinationX, IDK_MovePath_DestinationY, OVL_Path_Costs

~== NEXT is +1, PREVIOUS is -2

Move_Units()

iters 0 to Path_Length
indexes into *(IDK_MovePath_DestinationX + 1) and *(IDK_MovePath_DestinationY + 1)

destination_x, y is IDK_MovePath_DestinationX[0], IDK_MovePath_DestinationY[0]

Next_Tile_X is *(IDK_MovePath_DestinationX + 1 + itr)


push    [bp+map_p]
mov     al, [(IDK_MovePath_DestinationY+1)+_SI_itr__steps__units]
cbw
push    ax                              ; YPos
mov     al, [(IDK_MovePath_DestinationX+1)+_SI_itr__steps__units]
cbw
push    ax                              ; XPos
call    j_Check_Square_Scouted          ; returns 1 if the tile is both explored and visible,
                                     


mov     bx, [bp+Path_Length]
mov     al, [IDK_MovePath_DestinationX+bx]
cbw
mov     [bp+destination_x], ax


mov     bx, [bp+Path_Length]
mov     al, [IDK_MovePath_DestinationY+bx]
cbw
mov     [bp+destination_y], ax


mov     al, [(IDK_MovePath_DestinationX+1)+_SI_itr__steps__units]
cbw
mov     [bp+Next_Tile_X], ax


mov     al, [(IDK_MovePath_DestinationY+1)+_SI_itr__steps__units]
cbw
mov     [bp+Next_Tile_Y], ax


Move_Stack()
    sets OVL_Action_OriginX, OVL_Action_OriginY

Path_Length > 1
    OVL_Action_OriginX = Fst_Dst_X[Path_Length]
    OVL_Action_OriginY = Fst_Dst_Y[Path_Length]





Move_Units(player_idx, map_p, Path_Length, map_x, map_y, unit_array, unit_array_count)
display_moves = (player_idx == _human_player_idx  ||  magic_set.enemy_moves == ST_TRUE)
CLUE:
    if(Check_Square_Scouted() == ST_TRUE)
        x,y here is? src? dst? either?
        pretty sure it's dst
push    [bp+map_p]
mov     al, [(IDK_MovePath_DestinationY+1)+_SI_itr__steps__units]
cbw
push    ax                              ; YPos
mov     al, [(IDK_MovePath_DestinationX+1)+_SI_itr__steps__units]
cbw
push    ax                              ; XPos
    
SelectUnit_Stack()
    // ; sets the global path arrays and variables based on
    UNIT_SetGlobalPath(unit_idx)
Main_Screen()
    /* left click move map && unit_stack_count != 0 && stack moves < 1 */
    Stack_Action(..., GOTO, ...)
    UNIT_SetGlobalPath(_unit_stack[0].unit_idx)

mov     ax, offset OVL_Path_Costs
push    ax                              ; RCs@
mov     ax, (offset IDK_MovePath_DestinationY+1)
push    ax                              ; RYs@
mov     ax, (offset IDK_MovePath_DestinationX+1)
push    ax                              ; RXs@
push    [_map_plane]                    ; Plane
push    [bp+unit_goto_wy]               ; TgtY
push    [bp+unit_goto_wx]               ; TgtX
push    [bp+unit_world_y]               ; SrcY
push    [bp+unit_world_x]               ; SrcX
push    [word ptr bp-18h]
push    [word ptr bp-1Ah]
push    [word ptr bp-1Ch]
push    [word ptr bp-1Eh]
push    [word ptr bp-20h]
push    [word ptr bp-22h]               ; MTypes
call    j_STK_GetPath 



UNIT_SetGlobalPath()
    builds unit_array from _unit_stack
        ¿ why does it take unit_idx as a parameter then ?
    loops over unit_array
        checks if any are US_GoingTo
...
    OVL_StackHasPath = ST_TRUE
; set to 0 after display-sorting the active stack
; set to 1 if road-building, but the unit is not on any
;   of the plotted line tiles (before returning)
; set to 1 if road-building, and tiles left to do
; set to 1 if moving with path left to go
...
    sets 
        OVL_Path_Costs
        IDK_MovePath_DestinationY+1
        IDK_MovePath_DestinationX+1
        OVL_Path_Length
        may set OVL_StackHasPath to ST_TRUE, DOES NOT unset it





Move_Stack()
    |-> STK_GetPath()
        |-> Overland_Pathfinder()
    |-> STK_EvaluatePath()

stack movement path
path length
accumulated cost of path


OVL_Path_Costs

mov     ax, offset OVL_Path_Costs
push    ax                              ; Costs@
push    [bp+map_plane]                  ; Plane
mov     ax, (offset IDK_MovePath_DestinationY+1)
push    ax                              ; Ys@
mov     ax, (offset IDK_MovePath_DestinationX+1)
push    ax                              ; Xs@







### ¿ Stack_Moves_Active() vs. Stack_Moves() vs. Units_Moves() ?

WZD  o63p12  Stack_Moves_Active()
WZD  o63p13  Stack_Moves()
WZD  o97p04  Units_Moves()



WZD  o63p12  Stack_Moves_Active()
WZD  o63p13  Stack_Moves()
WZD  o97p04  Units_Moves()







// WZD o63p12
// Stack_Moves_Active
/*
    movement points to be used for 'move stack'
    ¿ just here to avoid loading the overlay for 'Move Stack' ?
        ...which would mean being in ovr063 is meaingful?
        ...and ovr097 is differently meaningful?
            ...and, cause called by ovr095?
    
*/
int16_t Stack_Moves_Active(void)
{
/*
XREFs
    j_Stack_Moves_Active()
    OVL_DrawStackMoves()

j_Stack_Moves_Active()
    Main_Screen()
    Do_Move_Stack_DirKey
    IDK_ActiveUnitStack_MovesOrPath_s53150


    loops through current stack
        checks for active
        checks if the unit type is/has/does transport/carries units
            breaks and assigns units current HMoves
        checks for wind walking - ability or enchantment
            does not break, but assigns units current HMoves
    Scenarios/Use-Cases:
        stack does not have a 'carries units' and does not have a 'has wind walking'
        stack does have a 'carries units' but does not have a 'has wind walking'
        stack does not have a 'carries units' but does have a 'has wind walking'
        
        if stack does have a 'carries units', that is that and its are the movement points
        ...
        ? nay 'carries units', nay 'has wind walking' ?
            returns lowest HMoves of 'active' units

NOTE: you can have multiple transport units in a stack

almost the same code as in WZD ovr097 Units_Moves()
Units_Moves() breaks after Unit_Has_WindWalking(); does not have stack_has_windwalker or windwalker_unit_idx
OON XREF STK_move() WZD o95p01

*/
    int16_t stack_has_windwalker;
    int16_t windwalker_unit_idx;
    int16_t tmp_half_move_points; // In Dasm, SI
    int16_t tmp_unit_idx;  // In Dasm, DI
    int16_t itr_unit_stack;
    int8_t tmp_unit_type;  // In Dasm, DNE

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Stack_Moves_Active()\n", __FILE__, __LINE__);
#endif

    tmp_half_move_points = 1000;

    stack_has_windwalker = ST_FALSE;

    for(itr_unit_stack = 0; itr_unit_stack < _unit_stack_count; itr_unit_stack++)
    {
        tmp_unit_idx = _unit_stack[itr_unit_stack].unit_idx;
        if(_unit_stack[itr_unit_stack].active == ST_TRUE)
        {
            if(_UNITS[tmp_unit_idx].HMoves < tmp_half_move_points)
            {
                tmp_half_move_points = _UNITS[tmp_unit_idx].HMoves;
            }

            tmp_unit_type = _UNITS[tmp_unit_idx].type;
            if(_unit_type_table[tmp_unit_type].Transport == ST_TRUE)
            {
                tmp_half_move_points = _UNITS[tmp_unit_idx].HMoves;
                break;
            }

            if(Unit_Has_WindWalking(tmp_unit_idx) == ST_TRUE)
            {
                stack_has_windwalker = ST_TRUE;
                windwalker_unit_idx = tmp_unit_idx;
                tmp_half_move_points = _UNITS[tmp_unit_idx].HMoves;
            }
        }
    }

    if(tmp_half_move_points == 1000)
    {
        tmp_half_move_points = 0;
    }

    STK_HMoves_Left = tmp_half_move_points;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Stack_Moves_Active() { STK_HMoves_Left = %d }\n", __FILE__, __LINE__, STK_HMoves_Left);
#endif

    return STK_HMoves_Left;
}















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




## Direction Key:

    three results:
        1: do nothing
        2: display warning
        3: call Do_Move_Stack()
    
    1: do nothing
        _unit_stack_count == 0
        Stack_Moves_Active() == 0
        all_units_moxed == ST_TRUE
    2: display warning
        Stack_Has_Planar_Travel() == ST_FALSE && unit plane != map plane && unit in tower == ST_FALSE
    3: call Do_Move_Stack()
        player idx
        map x,y,p
        stack destination x,y

calls Do_Move_Stack_DirKey(), with *direction*
    {1,2,3,4,6,7,8,9}
    {LeftDown,Down,RightDown,Left,Right,LeftUp,Up,RightUp}
    in order of numeric keypad keys, excluding 5
    calcs x, calcs y, wraps x, limits y, checks planar travel vs planes


Stack_Moves_Active()


Do_Move_Stack(move_x, move_y, _human_player_idx, &_map_x, &_map_y, &_map_plane)


Build_Moveable_Stack()
    builds array of stack units that should / are able to move
        includes logic for transports - loading, units that require transport on ocean terrain, and adding but not counting Hero Units
        nothing with movement points or terrain types

// WZD o95p01
Move_Stack(int16_t player_idx, int16_t destination_x, int16_t destination_y, int16 Spec, int16_t map_x, int16_t map_y, int16_t map_p, int16_t unit_array_count, int16_t unit_array[])

// WZD o95p02
Move_Units()
    takes Path_Length
    expects OVL_Path_Xs[], OVL_Path_Ys[]



the 'move stages' loop is inside the 'path length' loop
so, ...
    ¿ ... ?

after 'move stages'

...
    Move_From_X = unit_x;
    Move_From_Y = unit_y;
...
                Unit_Draw_Left = Move_From_X;
                Unit_Draw_Top = Move_From_Y;
                World_To_Screen(*map_x, *map_y, &Unit_Draw_Left, &Unit_Draw_Top);
...
            Cur_Tile_Left = Move_From_X;
            Cur_Tile_Top = Move_From_Y;
            World_To_Screen(*map_x, *map_y, &Cur_Tile_Left, &Cur_Tile_Top);

            Next_Tile_Left = Next_Tile_X;
            Next_Tile_Top = Next_Tile_Y;
            World_To_Screen(*map_x, *map_y, &Next_Tile_Left, &Next_Tile_Top);
...
                Unit_Draw_Left = Unit_Draw_Left + Shift_By_X;
                Unit_Draw_Top = Unit_Draw_Top + Shift_By_Y;
...











// WZD o61p08
void Build_Moveable_Stack(int16_t * unit_count, int16_t * unit_array)
    populates an array of unit_idx and the count of elements in that array
    presumably, this is units that have the movement points and movement mode to allow them to make the move
    this would also accomodate transports - ships and floating island
        also, wind walker?
    ...sailing...
    ...flying...

    loops over_unit_stack_count
        Yay active, Nay Go-To, Yay Transport
            sets 'stack has transport' flag
            sets 'transport capacity'
    MAJOR BRANCH: Yay || Nay 'stack has transport' flag
        Yay 'stack has transport' flag
            sets IDK_Stack_Array_cnt to 0
            sets IDK_unit_array_cnt to 0
            loops over_unit_stack_count
                Yay active, Nay Go-To, Yay Transport
                    adds _unit_stack[itr].unit_idx to [intout] unit_array@
                    incrs IDK_unit_array_cnt
                adds _unit_stack[itr].unit_idx to Stack_Array@
                incrs IDK_Stack_Array_cnt
            HERE:
                [intout] unit_array@ is populated with all Transports
                IDK_unit_array_cnt is ~unit_array_count / count of Transports
                Stack_Array@ is populated with all units from _unit_stack
                IDK_Stack_Array_cnt is count of all units
            sets IDK_Stack_Array_cnt to IDK_unit_array_cnt
                ¿ proptly overwrites IDK_Stack_Array_cnt with IDK_unit_array_cnt ?
                so, ...
                    now we don't know the count for Stack_Array@, but it happens to be the same as _unit_stack_count
                    and in the later call to STK_GetLandlubbers(), Stack_Array@ happens to the same as _unit_stack
            ...
            ...
            ...
            BRANCH: Yay || Nay Square_Has_City()
                Yay Square_Has_City()
                    goto Done; sets unit_array_count@ to IDK_Stack_Array_cnt
                    HERE:
                        Yay Transport, Yay City
                        so, ...
                            ¿ must be on Ocean ?
                            ¿ only Transports are moveable from here ?
                Nay Square_Has_City()
                    loops over LandLubber_Array@
                        sets all 9 to ST_UNDEFINED
                    calls STK_GetLandlubbers() with _unit_stack_count, Stack_Array@, and LandLubbers_Array@
                    HERE:  Stack_Array@ ~== _unit_stack
                    BRANCH: LandLubber_Count <=/> 0
                        LandLubber_Count <= 0
                            ...
                            BRANCH: IDK_Stack_Array_cnt < _unit_stack_count
                                !(IDK_Stack_Array_cnt < _unit_stack_count)
                                    HERE: ¿ we're on the ocean and all stack units are transports ?
                                    goto Done; sets unit_array_count@ to IDK_Stack_Array_cnt
                                IDK_Stack_Array_cnt < _unit_stack_count
                                    HERE: ¿ we're on the ocean and ... IDGI ... just adds the rest of the units that aren't in Go-To or a Transport ?
                                    loops over _unit_stack_count
                                        _UNITS[itr].Status != US_GoingTo
                                        _unit_type_table[].Transport < 1
                                        unit_array[] = _unit_stack[itr].unit_idx
                                        IDK_Stack_Array_cnt++
                        LandLubber_Count > 0
                            ...
                            sets IDK_Stack_Array_cnt to IDK_unit_array_cnt
                            sets LandLubber_Array_Index to 0
                            sets transport_load to 0
                            loops over _unit_stack_count
                                if(LandLubber_Array[LandLubber_Array_Index] == _unit_stack[itr_stack].unit_idx)
                                    if transport_load < IDK_transport_capacity OR _UNITS[].type <= 34
                                        unit_array[IDK_Stack_Array_cnt] = _unit_stack[itr_stack].unit_idx;
                                        IDK_Stack_Array_cnt++;
                                        LandLubber_Array_Index++;
                                        if( _UNITS[_unit_stack[itr_stack].unit_idx].type > 34)  /* Hero Unit */
                                            transport_load++;
                            again, loops over _unit_stack_count
                                adds units with air or water travel, but not 'Go-To'

        Nay 'stack has transport' flag
            sets IDK_Stack_Array_cnt to 0
            loops over _unit_stack_count
                adds stack units that are active, but not 'Go-To'



o95p05
STK_GetLandlubbers()
    takes in count, in array, inout array; returns count
    builds array of units without winwalking, airtravel, watertravel, airtravel item, watertravel item
    i.e., units that *require* a transport to move on ocean terrain map squares
    ¿ odd that it checks for items, since only heros can have items, and hero don't count towards transport capacity ?
    not "STK", in that it doesn't use the _unit_stack[]
    ...Non Ocean Fairing...
    !~== water travel
    != air travel
    sailing < swimming < flying ~== windwalking



## Left-Click Movement Map

if((input_field_idx == _main_map_grid_field) && (_unit_stack_count != 0) )

BRANCH: Stack_Moves_Active() < 1 vs >= 1

    Stack_Moves_Active() < 1
        Stack_Action(_human_player_idx, &_map_x, &_map_y, &_map_plane, 3, MMX2WMX(), MMY2WMY());  /* Action 3: 'Go-To' */



### Do_Move_Stack()
    called by both direction key move stack and left click move stack
        two different locations for left click move stack





### ¿ OVL_MapVar3 ?

// WZD dseg:C03E                                                 BEGIN: Main_Screen

// WZD dseg:C03E 00 00                                           CRP_OverlandVar_2 dw 0                  ; DATA XREF: Main_Screen:loc_4FBC0w
// WZD dseg:C040 00 00                                           CRP_OverlandVar_4 dw 0                  ; DATA XREF: Main_Screen:loc_4FC07r ...
// WZD dseg:C042 00 00                                           OVL_MapVar3 dw 0                        ; DATA XREF: Main_Screen+6Cw ...
// WZD dseg:C044 00 00                                           CRP_OVL_Obstacle_Var1 dw 0              ; DATA XREF: Main_Screen+66w ...

OVL_MapVar3
XREF
Main_Screen+6C           mov     [OVL_MapVar3], 1        
Do_Move_Stack+36         cmp     [OVL_MapVar3], 1        
Do_Move_Stack+3D         mov     [OVL_MapVar3], 0        
WIZ_NextIdleStack:@@Done mov     [OVL_MapVar3], e_ST_TRUE
Stack_Action+D           mov     [OVL_MapVar3], 1        

AKA  drake178: WIZ_NextIdleStack()
Select_Next_Stack()
    sets OVL_MapVar3 to ST_TRUE, at end, under all conditions
Main_Screen()
    sets OVL_MapVar3 to ST_TRUE, at beginning
    along with setting CRP_OverlandVar_2 and CRP_OVL_Obstacle_Var1 to ST_FALSE
Do_Move_Stack()
    tests OVL_MapVar3 for ST_TRUE
        sets OVL_MapVar3 and CRP_OVL_Obstacle_Var1 to ST_FALSE
        test CRP_OVL_Obstacle_Var1 for ST_FALSE
            calls j_o58p03_Empty_pFxn()
    so, ...
        the logic here in Do_Move_Stack() results in the same state that is set outright in Main_Screen()
        and testing CRP_OVL_Obstacle_Var1 for ST_FALSE to call j_o58p03_Empty_pFxn()
            could mean CRP_OVL_Obstacle_Var1 is whether something has not been set or some process has not been run
        and testing OVL_MapVar3 for ST_TRUE to set CRP_OVL_Obstacle_Var1 to ST_FALSE
            could mean OVL_MapVar3 is whether that state or process should be set/updated the next time around
        ¿ like how it does with the 'do full screen draw' / 'did first draw' and 'screen changed' ?
    ¿ meaningful that it is called immediately preceding STK_Move() ?
Stack_Action()
    sets OVL_MapVar3 to ST_TRUE and CRP_OVL_Obstacle_Var1 to ST_FALSE, at begining
        ~== to Main_Screen()
            assuming CRP_OverlandVar_2 is part of some other logic set


### ¿ CRP_OVL_Obstacle_Var1 ?

CRP_OVL_Obstacle_Var1
XREF
Main_Screen+66          mov     [CRP_OVL_Obstacle_Var1], 0         
sub_518A3+3             mov     [CRP_OVL_Obstacle_Var1], 0         
Do_Move_Stack+43        mov     [CRP_OVL_Obstacle_Var1], e_ST_FALSE
Do_Move_Stack:loc_533B5 cmp     [CRP_OVL_Obstacle_Var1], e_ST_FALSE
Stack_Action+13         mov     [CRP_OVL_Obstacle_Var1], 0         
STK_Move+4FE            mov     [CRP_OVL_Obstacle_Var1], 0         


### ¿ CRP_OverlandVar_2 ?

CRP_OverlandVar_2
XREF


### ¿ CRP_OverlandVar_4 ?

CRP_OverlandVar_4
XREF







¿ Stack_Moves_Active() vs. Units_Moves() ?
¿ ... vs. Stack_Moves() ?

ovr063
o64p12
Stack_Moves_Active()
    sets and returns STK_HMoves_Left

ovr063
o64p13
Stack_Moves()
    sets and returns STK_HMoves_Left

ovr097
o97p04
Units_Moves()
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


