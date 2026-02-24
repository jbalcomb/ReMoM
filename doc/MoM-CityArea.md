
MoM :: City-Area



drake178:
CTY_CatchmentRefresh() => Reset_City_Area()


CTY_CatchmentRefresh()
completely clears and rebuilds city_area_bits and city_area_shared_bits

CTY_CatchmentRefresh()
OON XREF: j_CTY_CatchmentRefresh()
j_CTY_CatchmentRefresh()
XREF:
    CTY_Remove__STUB+82
    CTY_TakeOver+1EE
    EVNT_Progress+41F
    Loaded_Game_Update+10
    Loaded_Game_Update_WZD+16
    TILE_Settle+238


City_Map_Square_Is_Shared()
OON XREF: j_City_Map_Square_Is_Shared()
j_City_Map_Square_Is_Shared()
XREF:
    City_Screen_Draw_Map+82
    Map_Square_Food2+ED
    Map_Square_Gold_Income+70
    Map_Square_Magic_Power+78
    Map_Square_Production_Bonus+B4
    TILE_GetAdamntmPower+51
    TILE_GetCrysxPower+51
    TILE_GetGemGold+65
    TILE_GetGoldOreGold+65
    TILE_GetMithrilPower+51
    TILE_GetQuorkPower+51
    TILE_GetSilverGold+65
    Terrain_Unit_Cost_Reduction+6D
    UU_TILE_BotchedResFn+B9
...
¿ all just for shared resources calculations ?
.. and the drawing of the 1/2 icon



City_Screen_Draw_Map()





##### Naming Things is Hard

terrain types
city map square  [used as in a "city map square" is a "[world] map square" with a city on it]
all the squares
four corner squares
5X5 area
centered around the city
special resources
selected map square
on the same world
yield ... output
Land that is shared
city map window
city screen
shared map squares

Page 62  (PDF Page 67)
The terrain types within two map squares of a city map square
  (specifically, all the squares except for the four corner squares in the 5X5 area centered around the city)
  determine the maximum population for the city and the special resources available to the city;
  a synopsis of a city’s potential on a selected map square is accessible via the surveyor button.
See the Surveyor, Terrain and Terrain Specials sections for relevant information.

Note that you can only build new cities on map squares that are at least three map squares away from all other cities on the same world.
If you start a new city only three map squares away from another city, however,
  the map squares that both cities share (as a result of being built so close together) yield only 50% of their output to each city.
Land that is shared is demarcated in the city map window of the city screen by a red 1/2 in the shared map squares.





## Iterate Over City-Area

city area y from -2 to 3
    if not above the map and not below the map
city area x from -2 to 3

X,Y Coordinates on the World Map Square for the City Area Square
city_wx + city_area_x
city_wy + city_area_y


checks to exclude the four corners of the city area
{ {-2,-2}, {-2,2}, {2,-2}, {2,2} }

if city_area_y == -2
    but city_area_y != -2 or 2
if city_area_y ==  2
    but city_area_y != -2 or 2



## Testing Bits

bitfield index is calculated as per-plane
disassembly shows in-line handling of far pointer

seems unlikely this would have been worked out and typed up for every instance
code snippets?
macro?
    macro specific to each bitfield?
        here, could just be common to city_area_bit_index and city_area_shared_bits?
still too much missing information?

¿ every instance of Test_Bit_Field() / Set_Bit_Field() has bitfield_index precalculated ?

¿ every instance of Test_Bit_Field() / Set_Bit_Field() has city_wp precalculated ?

Map_Square_Is_Shared()
    mov     ax, [bp+wy]
    mov     _DX_bit_field, e_WORLD_WIDTH
    imul    _DX_bit_field
    add     ax, [bp+wx]
    mov     [bp+bit_index], ax

    mov     ax, [bp+wp]
    mov     _DX_bit_field, e_WORLD_SIZE_DW
    imul    _DX_bit_field
    mov     _DX_bit_field, [word ptr city_area_shared_bits]
    add     _DX_bit_field, ax
    push    [word ptr city_area_shared_bits+2]
    push    _DX_bit_field                   ; bit_field
    push    [bp+bit_index]                  ; bit_idx
    call    Test_Bit_Field




city_area_bit_index = ((city_area_square_wy * WORLD_WIDTH) + city_area_square_wx);
if(Test_Bit_Field(city_area_bit_index, &city_area_bits[(city_wp * WORLD_SIZE)]) != ST_FALSE)

Set_Bit_Field(city_area_bit_index, &city_area_shared_bits[(city_wp * WORLD_SIZE)]);

&city_area_bits[        (city_wp * WORLD_SIZE) ]
&city_area_shared_bits[ (city_wp * WORLD_SIZE) ]

#define CITY_AREA         { &city_area_bits[        (city_wp * WORLD_SIZE) ] }
#define CITY_AREA_SHARED  { &city_area_shared_bits[ (city_wp * WORLD_SIZE) ] }

#define CITY_AREA(_wp_)         { &city_area_bits[        ((_wp_) * WORLD_SIZE) ] }
#define CITY_AREA_SHARED(_wp_)  { &city_area_shared_bits[ ((_wp_) * WORLD_SIZE) ] }


//ClaudeOpus46
## Get_Useable_City_Area() vs Get_Useable_City_Area_NewGame()

### Purpose (shared)

Both functions build the coordinate list for a city's usable catchment area.  They iterate
over the standard 21-square diamond footprint — a 5x5 grid centered on the city with the
four corner squares trimmed (rows at y-offset -2 and +2 use x-offsets -1..+1 only).  Rows
that fall outside the vertical map bounds (0..WORLD_HEIGHT-1) are skipped entirely.
Horizontal coordinates that fall off either edge of the map are wrapped by adding or
subtracting WORLD_WIDTH.  For every qualifying square, the world-X and world-Y are written
into the caller-supplied output arrays at the next available index, and the total count of
emitted squares is returned.

### Differences

| Aspect                      | `Get_Useable_City_Area` (CITYCALC.c)             | `Get_Useable_City_Area_NewGame` (MAPGEN.c)        |
|-----------------------------|--------------------------------------------------|---------------------------------------------------|
| **Overlay / object**        | WZD o142p08 (WIZARDS overlay)                    | MGC o51p53 (MAGIC overlay — map generation code)  |
| **drake178 name**           | (not annotated here)                             | TILE_GetCatchment()                               |
| **Corruption filter**       | Yes — reads `_map_square_flags` for the given    | No — the corruption check is commented out as     |
|                             | plane/row, tests each candidate square against   | `// DNE`.  Every in-bounds candidate is emitted   |
|                             | `MSF_CORRUPTION`, and only emits non-corrupted   | unconditionally.  This is correct: during new-    |
|                             | squares.                                         | game map generation no corruption exists yet.     |
| **terrain_flags_table_row** | Declared as a live `uint8_t *` local.  Set once  | Commented out (`// DNE`).  Never referenced.      |
|                             | per row to point into `_map_square_flags` at the |                                                   |
|                             | start of the plane+row, then indexed by          |                                                   |
|                             | `square_x` inside the inner loop.                |                                                   |
| **Register annotations**    | `itr_city_area_squares` = `_DI_`,                | `map_square_count` = `_SI_`,                      |
|                             | `square_x` = `_SI_`                              | `itr_city_area_squares` = `_CX_`,                 |
|                             |                                                  | `itr_world_x` = `_DI_`,                           |
|                             |                                                  | `square_x` = `_DX_`                               |

### Detail: the corruption filter

This is the only behavioral difference.  In CITYCALC.c, immediately before writing a
candidate square into the output arrays, the code does:

```c
terrain_flags_table_row = (uint8_t *)&_map_square_flags[(city_wp * WORLD_SIZE) + (square_y * WORLD_WIDTH)];
...
if((*(terrain_flags_table_row + square_x) & MSF_CORRUPTION) == 0)
{
    wx_array[map_square_count] = square_x;
    wy_array[map_square_count] = square_y;
    map_square_count++;
}
```

The pointer `terrain_flags_table_row` is set once per row, at the top of the outer-loop
body, to the start of the flags row for the current plane and y-coordinate.  Inside the
inner loop it is indexed by `square_x` (already wrapped) and the flag byte is AND-masked
with `MSF_CORRUPTION`.  Only squares whose corruption bit is clear are stored.

In MAPGEN.c the same three lines are present but with the corruption guard removed:

```c
// DNE  terrain_flags_table_row = (uint8_t *)&_map_square_flags[...];
// DNE  if( (*(terrain_flags_table_row + square_x) & MSF_CORRUPTION) == 0)
// DNE  {
    wx_array[map_square_count] = square_x;
    wy_array[map_square_count] = square_y;
    map_square_count++;
// DNE  }
```

The stores and the increment are unconditional.  Because map generation runs before any
spell or event can corrupt a square, the filter would always pass anyway, so SimTex
(correctly) omitted it from the NewGame variant.

### Practical consequence

`Get_Useable_City_Area()` can return fewer than 21 squares if some catchment squares are
corrupted (or fewer if the city is near the top or bottom map edge).
`Get_Useable_City_Area_NewGame()` will always return exactly as many squares as fit within
the vertical map bounds — corruption never reduces the count.

