


TILE_ExploreRadius__WIP()   ==>  Set_Square_Explored_Flags_XYP_Range()
TILE_Explore()              ==>  Set_Square_Explored_Flags_XYP()
TILE_ExploreCorners__WIP()  ==>  Set_Square_Explored_Flags_Bottom_Right_Corner()
RP_TILE_ExploreMatch__WIP()  ==>  Set_Square_Explored_Flags_Fix()



Set_Map_Square_Explored_Flags_XYP_Range()
    |-> Set_Map_Square_Explored_Flags_XYP()
        |-> Set_Square_Explored_Flags_Bottom_Right_Corner()
        |-> Set_Square_Explored_Flags_Fix()

Set_Map_Square_Explored_Flags_XYP()
    Units_In_Tower()
    Set_Map_Square_Explored_Flags_XYP_Range()
    sAFA06_Anim_EarthLore()
    Cast_PlaneShift()

Set_Square_Explored_Flags_Bottom_Right_Corner()
    NX_j_Set_Square_Explored_Flags_Bottom_Right_Corner()
    Set_Map_Square_Explored_Flags_XYP()

Set_Square_Explored_Flags_Fix()
    NX_j_Set_Square_Explored_Flags_Fix()
    Set_Map_Square_Explored_Flags_XYP()




Explored vs. Scouted

## _square_explored
src/MOX/MOM_Data.C/.H
// WZD dseg:9CB0
// drake178: TBL_Scouting
// MoO2  Module: PLNTSUM  data (0 bytes) _planet_explored  Address: 02:001945B2
// 1oom: ¿ ?
/*
    302 Paragraphs, 4832 Bytes
    4800 bytes used - 1-byte values, 2400 world squares, 2 planes
    {F,T}

*/
uint8_t * _square_explored;                     // alloc in ALLOC.C; load in Load_SAVE_GAM()


ovr059 has TILE_ExploreRadius__WIP(), just calls TILE_Explore_XYP()
ovr068 has one map, mostly reduced map, and sets `_square_explored`
ovr092 is scouted, but has one explored


ovr068
TILE_Explore_XYP(x, y, p)
TILE_ExploreCorners(x, y, p)
RP_TILE_ExploreMatch(x, y, p)

EXPLORE.C
// WZD o92p08
int16_t Check_Square_Explored(int16_t wx, int16_t wy, int16_t wp)
...directly accesses `_square_explored`



; MoO2  Set_Planet_Explored_Flags_
; drake178: TILE_Explore()
;
; explores a tile, and the appropriate sides or corners
; of the neighboring tiles around it (equivalent to a
; scouting radius of 1)


// WZD o68p06
// drake178: TILE_Explore()
// MoO2  Module: PLNTSUM  code (0 bytes) Set_Planet_Explored_Flags_()  Address: 01:0009BB40
/*
    called from Units_In_Tower()
        only for human_player_idx
    called from TILE_ExploreRadius__WIP()

*/
NOTE: Set_Planet_Explored_Flags_() passes _planet_explored to Set_Bit_Field_(), based on Ok_To_View_System_()  ¿ feels more like scouted ?
...Clear_Planet_Explored_Flags_()...Filter_Explored_Planets_()...


_square_explored
XREF:
    Save_SAVE_GAM()
    Load_SAVE_GAM()
    Cheat_Reveal()
    Set_Entities_On_Map_Window()
    IDK_Nodes_Entities_s5A53C()
    Redraw_Map_Unexplored_Area()
    DEPR_MainScr_Draw_Map_Cities()
    Road_Build_Path()
    sub_5D15B()
    sub_5D20A()
    Cartographer_Screen_Draw__WIP()
    Cartograph_Screen_Draw_Map__WIP()
    Check_Square_Explored()
    Surveyor_Window_Display()
    Cast_CallOfTheVoid()
    Cast_WarpNode()
    Cast_Move_Fortress()
    Cast_Earthquake()
    Cast_ChangeTerain()
    Cast_RaiseVolcano()
    Cast_Transmute()
    Cast_Corruption()
    Cast_EnchantRoad()
    Cast_SpellWard()
    Cast_WallOfStone()
    Cast_Spell_Overland__WIP()
    Compute_Base_Map_Square_Values()
    Draw_Map_Terrain()
    Draw_Map_Unexplored_Area()
    Draw_Map_Cities()
    Draw_Map_Towers()
    Draw_Map_Lairs()
    Draw_Map_Nodes()
    Draw_Map_Biota()
    Draw_Map_Minerals()
    Draw_Map_Roads()
    Create_Reduced_Map_Picture()
    Cartograph_Screen_Draw_Map_Terrain__NOWORKIE()
    Allocate_Data_Space()
    Square_Is_Forest()
    Set_Map_Square_Explored_Flags_XYP()
    Set_Square_Explored_Flags_Bottom_Right_Corner()
    Set_Square_Explored_Flags_Fix()



Draw_Map_Unexplored_Area()

Draw_Maps()
    |-> Redraw_Map_Unexplored_Area(screen_x, screen_y, map_width, map_height, l_map_x, l_map_y, map_plane);



Terrain_Init()
// Loop MAPBACK 0 to 13:
MAPBACK.LBX
{0, ..., 13}
14 records
{"MASK", ..., "MASK"}
Why 14?
bit-field of 0 just does not draw any "xploid"
bit-field of 15 just fills the map square with ST_BLACK
So, 14, in-between...


## unexplored_mask_seg[]
14 pictures




## ¿ starts from Set_Map_Square_Explored_Flags_XYP_Range() ?
XREF:
    Do_Plane_Button__WIP()
    j_Set_Map_Square_Explored_Flags_XYP_Range()
        Cast_Awareness()
        Cast_Spell_Overland__WIP()
        Change_City_Ownership()
        City_Apply_Production()
        Create_Outpost()
        Create_Outpost()
        EarthGateTeleport__WIP()
        Event_Twiddle()
        Move_Units_Draw()
        Move_Units_Draw()

IDK.IDGI.
But, ...
Set_Map_Square_Explored_Flags_XYP_Range()
Set_Map_Square_Explored_Flags_XYP()




...uses masks

## Set_Map_Square_Explored_Flags_XYP_Range()
calls Set_Map_Square_Explored_Flags_XYP()
just once, if scout_range is 1
else
((scout_range - 1) * 2) + 1
e.g.,
    with a scout range of 2: ((2-1)*2)+1=3
    with a scout range of 3: ((3-1)*2)+1=5
then
    for (y_start + scout_range) > itr_world_y
    for (x_start + scout_range) > itr_world_x
where
    y_start = wy - (scout_range - 1)
    x_start = wx - (scout_range - 1)

## Set_Map_Square_Explored_Flags_XYP()

if(wy == 0)
    SET_SQUARE_EXPLORED(wx, wy, wp, (0x01 | 0x02 | 0x04 | 0x08));
#define SET_SQUARE_EXPLORED(_wx_, _wy_, _wp_, _value_)  ( _square_explored[ ( ( (_wp_) * WORLD_SIZE ) + ( (_wy_) * WORLD_WIDTH ) + (_wx_) ) ] = (_value_)  )
...top row of the world map
`mov     [byte ptr es:bx], SCT_BottomLeft or SCT_TopLeft or SCT_TopRight or SCT_BottomRight`
00000001 ; enum SCOUT_BITS (bitfield)
00000001 SCT_BottomLeft  = 1
00000002 SCT_TopLeft  = 2
00000004 SCT_TopRight  = 4
00000008 SCT_BottomRight  = 8
// ... sets all four corners ...


## TILE_ExploreCorners__WIP()

XREF:
    NX_j_TILE_ExploreCorners__WIP
    Set_Map_Square_Explored_Flags_XYP+256
    Set_Map_Square_Explored_Flags_XYP+272
    Set_Map_Square_Explored_Flags_XYP+281
    Set_Map_Square_Explored_Flags_XYP+29A
    Set_Map_Square_Explored_Flags_XYP+2B6
    Set_Map_Square_Explored_Flags_XYP+2C5

loops over six patterns
if the existing _square_explored[] value matches a pattern, increments a counter
early exit on 0 matches
same loop, checks the map square to the right, increments the same counter
early exit on 1 matches
same loop, checks the map square to the bottom, increments the same counter
early exit on 2 matches
same loop, checks the map square to the bottom-right, increments the same counter


So, ...
gets passed wx, wy, wp
checks the map squares at wx,wy, wx+1,wy, wx,wy+1, wx+1,wy+1
```
[-] [-] [-]
[-] [X] [+]
[-] [+] [+]
```
adds SCT_BottomRight to wx,wy
adds SCT_BottomLeft to wx+1,wy
adds SCT_TopRight to wx,wy+1
adds SCT_TopLeft to wx+1,wy+1

¿ sets the 4 complimentary corners for the bottom-right of the target map square ?




SCT_BottomLeft   0x01  00000001
SCT_TopLeft      0x02  00000010
SCT_TopRight     0x04  00000100
SCT_BottomRight  0x08  00001000

```
- - x
- - -
x - -

x - x
- - -
x - -

x - -
- - -
- - x

x - -
- - -
x - x

- - x
- - -
x - x

x - x
- - -
- - x
```


## Set_Square_Explored_Flags_Fix()
