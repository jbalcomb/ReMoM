


Move_Units()
    Army_Movement_Modes()
    Army_Boat_Riders()
    STK_GetPath()
    STK_EvaluatePath()

## Movement Mode Flags
    6 movement mode boolean values
        { CAVALRY, FORESTER, MOUNTAINEER, SWIMMING, SAILING, FLYING }
    F/T for each mode
    meaning which modes the whole army can utilize
    forester, mountaineer, pathfinding, and sailing apply "for all stacked units"


int16_t movement_mode_flags[9];  
movement_mode_flags[0], movement_mode_flags[0], movement_mode_flags[1], movement_mode_flags[2], movement_mode_flags[3], movement_mode_flags[4], movement_mode_flags[5]  
MvMd_0, MvMd_1, MvMd_2, MvMd_3, MvMd_4, MvMd_5  
MvMd_0_Cvlry
MvMd_1_Frstr
MvMd_2_Mntnr
MvMd_3_Swim
MvMd_4_Sail
MvMd_5_Fly

// struct s_Movement_Modes movement_modes_array = 
// {
//     Cavalry = 0x0001
//     Forester = 0x0020,
//     Mountaineer = 0x0040,
//     Swimming = 0x0004,
//     Sailing = 0x0002,
//     Flying = 0x0008
// };
// struct s_Movement_Modes movement_modes_array = { 0x0001, 0x0020, 0x0040, 0x0004, 0x0002, 0x0008 };
struct s_Movement_Modes movement_modes_array = { CAVALRY, FORESTER, MOUNTAINEER, SWIMMING, SAILING, FLYING };



movement_mode_flags[9]
troops[9]
troop_count
boatriders[9]
boatrider_count

movepath_length



## STK_SetOvlMoveMap()
    sets 2400 values in movepath_cost_map[]
    the movement cost for each world map square
    given the stacks available movement modes

Sailing:
    memcpy(movepath_cost_map, &movement_mode_cost_maps->Sailing, 2400);
    copies movement_mode_cost_maps[5] to movepath_cost_map
    should be 2 for every map squarew that is ocean

Flying:  
    Set_Memory(movepath_cost_map, 10080, 2);  
    iter, if road then 0
    so, ...
        sets every world map sqaure to a cost of 2 movement points (1 moves2) (AKA "1MP")
        which also counts as setting ever world map square to passible
        then, sets all world map squares with any road to 0 movement points

Branches:
    if MvMd_5 then Flying
    if MvMd_4 then Sailing

feels like a bottom-up approach
what does the map state need to end up as
what needs to be done to set that state
what values should trigger that state setting
...
if pathfinding and swimming then all 1
if pathfinding and not swimming then walking, with all land as 1
if swimming and not pathfinding then walking, with all water as 1
else
walking [+forester][+mountaineer][+swimming]

something like
if not Frstr or not Mntr then not PthFnd?
then, if MvMd_0?
...

if Frstr
    if Swim
if Mntnr



if ((nay MvMd_1_Frstr or nay MvMd_2_Mntnr) and nay MvMd_0_Cvlry)
    then, if MvMd_1_Frstr overlays movement_mode_cost_maps[2] onto movepath_cost_map
    then, if MvMd_3_Swim overlays movement_mode_cost_maps[4] onto movepath_cost_map
    then, if MvMd_2_Mntnr overlays movement_mode_cost_maps[3] onto movepath_cost_map



if ((nay MvMd_1_Frstr or nay MvMd_2_Mntnr) but yay MvMd_0_Cvlry)
    copies movement_mode_cost_maps[0] to movepath_cost_map
    then, if MvMd_3_Swim overlays movement_mode_cost_maps[4] onto movepath_cost_map
    then, if MvMd_1_Frstr overlays movement_mode_cost_maps[2] onto movepath_cost_map
    then, if MvMd_2_Mntnr overlays movement_mode_cost_maps[3] onto movepath_cost_map


## movement_mode_cost_maps
6 * 60 * 40
movement point cost for each movement mode for the world map
¿ per terrain type ?
~ movement_mode_cost_maps[6][2400]
    Unused_MoveType, Walking, Forester, Mountaineer, Swimming, Sailing


## movepath_cost_map

¿ OON *created* by STK_SetOvlMoveMap() ?


WZD dseg:9CA4
movepath_cost_map

EMMDATAH


Allocate_Data_Space()
movepath_cost_map = SA_MK_FP0(EMM_EMMDATAH_AllocFirst(632))  //  632 PR  10,112B
movement_mode_cost_maps = SA_MK_FP0(EMM_EMMDATAH_AllocNext(1802))     // 1802 PR  28,832B

10,112 + 28,832 = 38,944B  2434PR  38.03125KB
10,112 - 10,080 = 32
10,080 / 2 = 5040
5040 / 2 = 2520
¿ 2400 + 120 ?

28,832 / 2 = 14,416
14,416 / 2,400 = 6.00666
2400 * 6 = 14,400
14,400 / 2400 = 6
6 movement modes, 2400 map squares
+ 1 PR for header/memblk


movepath_cost_map is what gets passed to Overland_Pathfinder()





dseg:9C90 00 00 00 00                                     TBL_OvlMovePathsEMS@ dd 0               ; DATA XREF: Loaded_Game_Update_WZD+8Br ...


dseg:9CAC 00 00 00 00                                     movement_mode_cost_maps dd 0                   ; DATA XREF: Save_SAVE_GAM+322r ...
dseg:9CAC                                                                                         ; 70Ah EMMData paragraphs
dseg:9CAC                                                                                         ; contains 2 HMP_MAPS structures, one per Plane






Special Unit Abilities
    Forester              In Forests, gain one movement point per square for all stacked units.  
    Great Wind Walking    Allows the unit’s entire stack to fly overland.  
    Mountaineer           In mountains and hills, only pay one movement point per square for all stacked units.  
    Pathfinding           For any overland map square, get one movement point per square for all stacked units.  
    Teleporting           Unit can move to any free square on the combat map for a cost of two movement points.  

BASIC MOVEMENT TYPES
MOVEMENT MODE       ALLOWED MOVEMENT
Flying              1/2 MP/square for all terrain  
Forester            All stacked units get 1 MP/square in forest  
Mountaineer         All stacked units get 1 MP/square in hills and mountains
Pathfinding         All stacked units get 1 MP/square for all land-based terrain
Sailing             Can only move over water 1 MP/ocean square All stacked units move at Sailing speed  
Swimming            Can move over oceans and rivers for 1 MP/square  
Wind Walking        All stacked units get 6 MP/turn and can fly  

