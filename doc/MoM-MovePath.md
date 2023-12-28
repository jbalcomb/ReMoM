
 ∴ 


Move Path
Movement Path
Path Finding Algorithm

not sure what is what

¿ something like find a path from src to dst ?

ovr095
ovr096
ovr097



Move_Units()
    |-> STK_GetPath()
    |-> STK_EvaluatePath()

STK_GetPath() returns path length
EZ_Resolve() is called inside of Move_Units()





calls Units_Moves() to get available moes2
bails if its < 1

gets movement modes
gets boat riders

Spec/Special Move
RdBd_UNIT_MoveStack_WIP() sets it to 0, just like Move_Units(), 
but, changes it to 2 if the unit count == road builder count
Move_Units() checks 0 then 2 then 3
0 and 3 are the same - to call STK_GetPath()
2 is ALL road building
1 and everything else just skip the call to STK_GetPath()

checks path length
bails if its < 1


STK_EvaluatePath()


Total_Move_Cost += OVL_Path_Costs[itr]  itr < Path_Length

checks updated path length - now just 'moves length'

sets OVL_Action_OriginX and OVL_Action_OriginY
    if <= 1 unit_wx,wy
    if > 1 Fst_Dst_X[Path_Length], IDK_MovePath_Y[Path_Length]

checks updated path length - still just 'moves length'
if != 0 Move_Units_Draw()

checks OVL_SWardTriggered  OVL_SpellWardError()

checks Combat_Move - set by STK_EvaluatePath()
    goes in combat handlers for Lair, Enemy Stack/City
    handles cancel/win/lose
    updates unit wx,y,p
else
    goes to @@EndOfMoving

so, ...
    it's spell ward or combat - can't be both

NOTE:
    Total_Move_Cost is in moves2 units
    it's what gets deducted from the units moves2 (AKA HMoves)





## STK_EvaluatePath()

// ; evaluates the stack's set path, and sets the return
// ; values accordingly:
// ;   Cmplt@ - 1 if running out of moves (no obstacles)
// ;   Path_Length@ - amount of moves that can be executed
// ;   Combat@ - the move enters a hostile tile (the human
// ;     player's move is stopped short before this)
// ;   Obst@ - a player or unit index identifying the
// ;     hostile entity (BUG? should be player index?)
// ; returns 1 if the stack can use up all of its
// ; movement, or 0 otherwise (moving gets interrupted)
// ;
// ; contains multiple transport-related BUGs












dseg:C666 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IDK_MovePath_Y db 78h dup(0)            ; DATA XREF: Move_Units+495r ...

dseg:C6E0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+MovePath_X db 76h dup(0)                ; DATA XREF: RdBd_UNIT_MoveStack_WIP+26Fo ...



                    // TODO(JimBalcomb,20231016):  ¿ why the `[bx-2]` ? indexing lo-bye of word, but array is DB? so, sizeof() pointer data-type?
                    // mov     bx, [bp+Path_Length];  mov     al, OVL_Path_Xs[bx-2];  cbw;  mov     [OVL_Action_OriginX], ax
                    // ... mov     al, [bx-3922h] ...
                    
                    // mov     bx, [bp+Path_Length];  mov     al, OVL_Path_Ys[bx-2];  cbw;  mov     [OVL_Action_OriginY], ax
                    // ... mov     al, [bx-399Ah] ...
                    
                    // mov     bx, [bp+Path_Length];  mov     al, [Scd_Dst_Y+bx];     cbw;  mov     [bp+destination_y], ax
                    // ... mov     al, [bx-3999h] ...
                    // mov     al, [(IDK_MovePath_Y+1)+bx]

                    // // // OVL_Action_OriginX = Fst_Dst_X[Path_Length];
                    // // // OVL_Action_OriginY = Fst_Dst_Y[Path_Length];
                    // // OVL_Action_OriginX = *(Fst_Dst_X + Path_Length);
                    // // OVL_Action_OriginY = *(Fst_Dst_Y + Path_Length);
                    // OVL_Action_OriginX = *(((uint8_t *)(&Fst_Dst_X)) + 1 + Path_Length);
                    // OVL_Action_OriginY = *(((uint8_t *)(&Fst_Dst_Y)) + 1 + Path_Length);
                    OVL_Action_OriginX = MovePath_X[(Path_Length - 1)];
                    OVL_Action_OriginY = MovePath_Y[(Path_Length - 1)];













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





UNIT_SetGlobalPath()
    sets OVL_Path_Length from STK_GetPath()





Allocate_Data_Space()

TBL_TempMoveMap_EMS[]
632 PR, 10112 B
2400 + 2400 + 2400 + 256 = 7456

    STK_SetOvlMoveMap()
        memset() of 10,080 to 2
            10112 - 10080 = 32 ... 2 16 byte headers?

TBL_MoveMaps_EMS[]
1802 PR, 28832 B

6 arrays of 2400, for each plane
6 * 24000 * 2 = 28800 B
cost of each world map square, given the terrain type, for each movement mode


00000000 struc OVL_Path_Results ; (sizeof=0x2780)
00000000 Tile_Costs db 2400 dup(?)
00000960 Reach_Costs db 2400 dup(?)
000012C0 Reach_From dw 2400 dup(?)
00002580 Reverse_Path dw 256 dup(?)
00002780 ends OVL_Path_Results


00000000 struc HMP_MAPS ; (sizeof=0x3840, standard type)
00000000 Unused_MoveType HMP_COSTS ?
00000960 Walking HMP_COSTS ?
000012C0 Forester HMP_COSTS ?
00001C20 Mountaineer HMP_COSTS ?
00002580 Swimming HMP_COSTS ?
00002EE0 Sailing HMP_COSTS ?
00003840 ends HMP_MAPS

00000000 struc HMP_COSTS ; (sizeof=0x960, standard type)
00000000 HalfMPCost db 2400 dup(?)
00000960 ends HMP_COSTS

¿ something somewhere incorrectly clears / copies 14400 bytes ?
so, maybe I just assuming one of the two world map movement cost arrays
STK_SetOvlMoveMap()
    accesses TBL_MoveMaps_EMS[] with a sizeof() 14400  (0x3840)
    So, ...
        either a 2D array or a struct?

TBL_TempMoveMap_EMS[]
OVL_Path_Results.Reverse_Path[itr_build_path_length] is set with dst_world_map_idx, as it iterates
starts from dst x,y



TBL_TempMoveMap_EMS[]
    Overland_Pathfinder()
    updated by STK_OvlObstacles()  ¿ identifies and marks *impassible* ?

STK_SetOvlMoveMap()
    copies from TBL_MoveMaps_EMS[] to TBL_TempMoveMap_EMS



settings the definitions as seen here makes all of the disassembly look like it makes sense for the addresses/offsets
but, results in it looking like nothing is setting Fst_Dst_Y, Scd_Dst_Y, Fst_Dst_X, Scd_Dst_X

dseg:C5F0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+OVL_Path_Costs db 76h dup(0)            ; DATA XREF: UNIT_MoveStack+267o ...
dseg:C5F0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; Move_Stack() uses `mov al; cbw` so, definitely DB
dseg:C666 00                                              Fst_Dst_Y db 0                          ; DATA XREF: Move_Stack+495r
dseg:C667 00                                              Scd_Dst_Y db 0                          ; DATA XREF: Move_Units+2C9r
dseg:C668 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+MovePath_Y db 76h dup(0)                ; DATA XREF: UNIT_MoveStack+26Bo ...
dseg:C668 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; Move_Stack() uses `mov al; cbw` so, definitely DB
dseg:C6DE 00                                              Fst_Dst_X db    0                       ; DATA XREF: Move_Stack+48Ar
dseg:C6DF 00                                              Scd_Dst_X db 0                          ; DATA XREF: Move_Units+2BEr
dseg:C6E0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+MovePath_X db 76h dup(0)                ; DATA XREF: UNIT_MoveStack+26Fo ...
dseg:C6E0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; Move_Stack() uses `mov al; cbw` so, definitely DB
dseg:C756 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IDK_MovePath db 3Eh dup(0)





Elsewhere, WTF is OVL_Action_OriginX, Y?
    different that *active x,y* for the *active* stack, first, highest priority unit `_unit`








Do_Move_Stack_DirKey()
Do_Move_Stack()
Move_Stack()
...
...
...

WZD o148p03

STK_GetPath()

WZD o97p01
STK_EvaluatePath()



WZD  ovr147  [1of1]
Overland_Pathfinder()
¿ sets || returns ?
    Overland_Pathfinder(src_x, src_y, TBL_TempMoveMap_EMS)
00000000 struc OVL_Path_Results ; (sizeof=0x2780)
00000000 Tile_Costs db 2400 dup(?)
00000960 Reach_Costs db 2400 dup(?)
000012C0 Reach_From dw 2400 dup(?)
00002580 Reverse_Path dw 256 dup(?)
00002780 ends OVL_Path_Results



Allocate_Data_Space()



OVL_Path_Costs[]
OVL_Path_Xs[]
OVL_Path_Ys[]

OVL_Action_OriginX
OVL_Action_OriginY



// WZD o95p01
Move_Stack(int16_t player_idx, int16_t destination_x, int16_t destination_y, int16 Spec, int16_t map_x, int16_t map_y, int16_t map_p, int16_t unit_array_count, int16_t unit_array[])

(only) bails on 0 movement points or 0 path length

    Set_Draw_Active_Stack_Always()
    Reset_Map_Draw()
    STK_GetMovesLeft()
    Stack_Movement_Modes()
    STK_GetLandlubbers()

// WZD o95p02
OVL_MoveUnitStack()




sizeof MovePath structure is 118 (0x76) bytes
TBL_OvlMovePathsEMS is 1033 (0x409) paragraphs  1033 * 16 = 16528 bytes
118 * 140 = 16520

00000000 struc MovePath ; (sizeof=0x76, standard type)
00000000 Start_X dw ?
00000002 Start_Y dw ?
00000004 Target_X dw ?
00000006 Target_Y dw ?
00000008 Plane dw ?
0000000A Length dw ?
0000000C _Xs db 35 dup(?)
0000002F _Ys db 35 dup(?)
00000052 Costs db 35 dup(?)
00000075 Unused_75h db ?
00000076 ends MovePath

; fills the return arrays with a path from the source
; to the destination tile, either by reading it out
; from the stored paths or by generating a new one from
; scratch
; returns the path length

; int __cdecl __far 
STK_GetPath(
    MoveType_Matrix MTypes, 
    int SrcX, int SrcY, 
    int TgtX, int TgtY, 
    int Plane, 
    char *RXs@, char *RYs@, char *RCs@, 
    int UU_bFlag_1, int UU_vFlag_2, 
    int LLCnt, 
    int UCnt, 
    int player_idx)
## STK_GetPath()

immediately branches on player_idx ==/!= HUMAN_PLAYER_IDX


sets CRP_UNIT_OverlandPath to ST_UNDEFINED
loops 140
...
    if...
        TBL_OvlMovePathsEMS[itr].Start_X == SrcX
        TBL_OvlMovePathsEMS[itr].Start_Y == SrcY
        TBL_OvlMovePathsEMS[itr].Target_X == TgtX
        TBL_OvlMovePathsEMS[itr].Target_Y == TgtY
        TBL_OvlMovePathsEMS[itr].Plane == Plane
    sets CRP_UNIT_OverlandPath to _SI_itr_Path_Length_Max ; an index into OvlMovePaths_EMS@
then,
    if CRP_UNIT_OverlandPath != ST_UNDEFINED
        fmemcpy()
            from
                TBL_OvlMovePathsEMS@  MovePath._Xs
                TBL_OvlMovePathsEMS@  MovePath._Ys
                TBL_OvlMovePathsEMS@  MovePath.Costs
            to
                RXs@
                RYs@
                RCs@
            count of
                TBL_OvlMovePathsEMS@  MovePath.Length
            where
                RXs@ is &IDK_MovePath_DestinationX[0]
                RXs@ is &IDK_MovePath_DestinationY[0]
                RXs@ is &OVL_Path_Costs[0]
                    passed in from Move_Stack()
also,
    UU_PathingVar1:  debug counter for movement path cache hits
    UU_PathingVar2:  debug counter for movement path cache misses
    UU_DBG_MovePatchCache_Hits
    UU_DBG_MovePatchCache_Misses


...
...
...
push    [word ptr TBL_TempMoveMap_EMS+2]
push    [word ptr TBL_TempMoveMap_EMS]  ; MoveMap@
push    [bp+src_y]                      ; Unit_Y
push    [bp+src_x]                      ; Unit_X
call    j_Overland_Pathfinder           ; calculates the shortest path to every tile from the
...
...
...





dseg:C5F0 00                                              OVL_Path_Costs db    0                  ; DATA XREF: UNIT_MoveStack+267o ...

### OVL_Path_Costs
DSEG shows space for 119 bytes, from here to the next known XREF
elsewhere, something had an itr < 140
XREF:
UNIT_MoveStack+267       mov     ax, offset OVL_Path_Costs                         
UNIT_SetGlobalPath+112   mov     ax, offset OVL_Path_Costs                         
Move_Stack+D9            mov     ax, offset OVL_Path_Costs                         
Move_Stack+272           mov     [byte ptr OVL_Path_Costs+_SI_itr__units__path], al
Move_Stack+27C           mov     al, [byte ptr OVL_Path_Costs+_SI_itr__units__path]
Move_Stack+285           mov     [byte ptr OVL_Path_Costs+_SI_itr__units__path], al
Move_Stack:loc_7BB09     cmp     [byte ptr OVL_Path_Costs+_SI_itr__units__path], 0 
Move_Stack+290           mov     [byte ptr OVL_Path_Costs+_SI_itr__units__path], 1 
Move_Stack+3F0           mov     al, [byte ptr OVL_Path_Costs]                     
Move_Stack+44B           mov     ax, offset OVL_Path_Costs                         
Move_Stack:loc_7BCEE     mov     al, [byte ptr OVL_Path_Costs+_SI_itr__units__path]
AI_AssignStackTarget+146 mov     ax, offset OVL_Path_Costs                         

Move_Stack()
    passes the offset of OVL_Path_Costs to STK_GetPath()
    passes the offset of OVL_Path_Costs to STK_EvaluatePath()

UNIT_SetGlobalPath
    passes the offset of OVL_Path_Costs to STK_GetPath()

UNIT_MoveStack
    passes the offset of OVL_Path_Costs to OVL_StoreLongPath()

Move_Stack()
    has two blocks assigning values to OVL_Path_Costs
        both are iterationg and setting its value to TILE_GetRoadBldTime()
    ¿ these are in a separate major branch from passing the offset to STK_GetPath() ? ¿ and STK_EvaluatePath() ?


¿ OVL_Path_Costs is 119 and  IDK_MovePath_DestinationX,Y are 1 + 119 ?

STK_GetPath()
    passed offset of OVL_Path_Costs RCs
        memcpy() from TBL_OvlMovePathsEMS@  Move_Path.Costs
        ¿ OR ?
        RCs[itr_path_length] = TBL_TempMoveMap_EMS[((y * WORLD_WIDTH) + x)]

TBL_TempMoveMap_EMS[] is created by Overland_Pathfinder()

TBL_TempMoveMap_EMS[] is updated by STK_OvlObstacles()  ¿ identifies and marks *impassible* ?

STK_SetOvlMoveMap()
    copies from TBL_MoveMaps_EMS[] to TBL_TempMoveMap_EMS






STK_GetPath()
    XREF: ***OON*** j_STK_GetPath()
j_STK_GetPath()
    XREF:  UNIT_SetGlobalPath();  Move_Stack();  AI_AssignStackTarget();
STK_EvaluatePath()
    XREF: ***OON*** j_STK_EvaluatePath()
j_STK_EvaluatePath
    XREF: ***OON*** Move_Stack()





### STK_GetPath()
XREF:
j_STK_GetPath()



### STK_EvaluatePath()
XREF:
j_STK_EvaluatePath()



### j_STK_GetPath()
XREF:
UNIT_SetGlobalPath()
Move_Stack()
AI_AssignStackTarget()



### j_STK_EvaluatePath()
XREF:
Move_Stack()
