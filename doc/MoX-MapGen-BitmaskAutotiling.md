


// MGC o51p20
void Simex_Autotiling(void)
...does Ocean, Mountain, Tundra, Hills

Bitmask Autotiling
A popular method that assigns binary values to neighboring positions:

Each neighbor position gets a bit value (1, 2, 4, 8, etc.)
The sum creates a unique index for tile selection
Supports up to 256 different tile configurations (8-bit)



//Claude
Why It Looks "Inverted"
Standard bitmask autotiling asks: "Which of my neighbors are the same terrain as me?"

This code does the opposite: the current square is Ocean, and it checks which neighbors are land. The resulting mask selects a shoreline variant from TERRTYPE.LBX. It's autotiling the coastline from the ocean's perspective — "where is the land around me?" — which is why the lookup table entries (_Shore00000001 through _Shore10101111 at TerrType.h:390-452) are indexed by land-neighbor bitmask rather than same-neighbor bitmask.

Both approaches produce equivalent results — one mask is just the bitwise complement of the other — so the lookup table is ordered accordingly.



Simplifying the Branching
The 8 direction blocks are ~140 lines of nearly identical code. You can collapse them with a direction table and a helper:


// Direction offsets: NW, N, NE, E, SE, S, SW, W
static const int autotile_dx[8] = { -1,  0,  1,  1,  1,  0, -1, -1 };
static const int autotile_dy[8] = { -1, -1, -1,  0,  1,  1,  1,  0 };
static const int autotile_bit[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };
Then a helper that wraps X and bounds-checks Y:


static int Is_Land_Neighbor(int wp, int wx, int wy)
{
    if (wy < 0 || wy >= WORLD_HEIGHT)
    {
        return 0;
    }
    if (wx < 0) { wx += WORLD_WIDTH; }
    else if (wx >= WORLD_WIDTH) { wx -= WORLD_WIDTH; }

    uint16_t terrain = GET_2B_OFS(_world_maps, (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx);
    return (terrain >= tte_Grasslands) && (terrain >= tte_Hills);  // BUGBUG: original condition preserved
}
And the entire 140-line ocean block becomes:

if (p_world_map[wp][wy][wx] == tte_Ocean)
{
    mask = 0;
    for (int d = 0; d < 8; d++)
    {
        if (Is_Land_Neighbor(wp, wx + autotile_dx[d], wy + autotile_dy[d]))
        {
            mask |= autotile_bit[d];
        }
    }

    if (mask > 0)
    {
        terrain_type_index = terrtype[mask];
        p_world_map[wp][wy][wx] = terrain_type_index;
    }
}

The same pattern applies to the Mountain, Tundra, and Hills blocks — each just needs a different Is_*_Neighbor predicate and a different terrtype offset. You could even unify all four with a function pointer or a generic callback for the neighbor test.

Note on the Y-bounds checks: The original code has wy < WORLD_HEIGHT for south-facing neighbors (SE, S, SW) which should be (wy + 1) < WORLD_HEIGHT — the existing BUGBUG comments in the Mountain/Tundra/Hills blocks flag this same issue. The helper above fixes it correctly with the wy >= WORLD_HEIGHT guard on the already-offset coordinate.





## TERRTYPE.LBX
05 00   5 records
00 02   512 record size
Entries:    1
Type:       5  Data



if tt_Ocean
convert to ¿ ?
// NOTE(drake178): set the tile border flags based on the types of the adjacent tiles in every direction:
// NOTE(drake178):   1   2   4
// NOTE(drake178): 128       8
// NOTE(drake178):  64  32  16

Grasslands, Forest, Mountain, Desert, Swamp, Tundra, SorceryNodeLake, NatureNodeForest, ChaosNodeVolcano, Hills
162, ..., 171

    tte_Grasslands1    = 0xA2,  /* tt_Grasslands1 */
    _Forest1        = 0xA3,
    tte_1Mountain1  = 0xA4,  /* type used by Set_Terrain_Type_Mountain() */
    _AllDesert1     = 0xA5,
    _Swamp1         = 0xA6,
    _AllTundra1     = 0xA7,
    _SorceryLake    = 0xA8,
    _NatureForest   = 0xA9,
    _ChaosVolcano   = 0xAA,
    tte_1Hills1     = 0xAB,

So, target square is Ocean
Then, set a bit for each postion that the adjacent map square is a base terrain type
0 + 1
...
0 + 128
...
0 + 1 + 2 + 4 + 8 + 16 + 32 + 64 + 128 = 255 0xFF
255  0xFF  0b11111111 is Ocean surrounded by basic land types?
Finally, use that value to index terrtype[] to get the terrain type index
e.g., 1 maps to `1F 00`  0x001F  _Shore10000000

similar for Mountain

similar for Tundra - update the target square's terrain index if the adjacent squares are not Tundra
...uses the same terrtype map as Ocean, mapped at 600 rather than 0
...first mapping value is 1, so range is {1,...,255} + 600 = {601,...,855}

                SET_2B_OFS(
                    _world_maps,
                    ((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx),
                    (600 + terrtype[Tile_Border_Flags])
                );

similar for hills
does not use Adjacent_X variable
uses second block of terrtype, same as mountains
BUGBUG  turns single map square hills into grasslands, because (16 + tte_Mountain) is tte_Grasslands4 rather than tte_Hills
    mov     ax, [bp+Tile_Border_Flags]
    shl     ax, 1
    mov     bx, [bp+terrtype]
    add     bx, ax
    mov     ax, [bx+200h]
    add     ax, _Hills_0010-_Mount0010
    push    ax

kinda looks like the terrtype data for Hills gotch smashed in the LBX create tool

    SET_2B_OFS(
        _world_maps,
        ((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx),
        (16 + terrtype[(512 + Tile_Border_Flags)])
    );




To smooth terrain using bitmasking, calculate a 4-bit or 8-bit value for each cell by checking adjacent terrain (NW, N, NE, E, SE, S, SW, W) in clockwise order. If a neighbor matches the current type, set its corresponding bit, then map this 
 value to a specific terrain sprite for seamless blending. 
Hacking with Swift
Hacking with Swift
 +1
Terrain Smoothing Algorithm (Bitmasking)
Define Terrain Types: Determine which tiles constitute "land" (e.g., grass) and which are "water".
Accumulate Bits (Clockwise): For each tile, check its 4 or 8 neighbors to see if they are the same type.
4-bit (Cardinal): North, East, South, West (
).
8-bit (Clockwise): NW, N, NE, E, SE, S, SW, W (
).
Example: If a tile has land to the North, East, and South, but not West, the mask might be 
 (14).
Map to Sprites: Use the resulting 4 or 8-bit integer (
) as an index or lookup key for a sprite sheet containing all edge, corner, and transition combinations.
Automapping: Implement an automapping system that checks neighbors and selects the appropriate tile, such as using SKTileMapNode to handle blending water and grass edge tiles automatically. 
Hacking with Swift
This technique ensures that as the map is generated, tiles are chosen to match the surrounding terrain, creating smooth, non-gridlike transitions. 

Example Code Logic (Pseudocode)
python
val = 0
if neighbor_nw == type: val += 1
if neighbor_n  == type: val += 2
if neighbor_ne == type: val += 4
if neighbor_e  == type: val += 8
if neighbor_se == type: val += 16
if neighbor_s  == type: val += 32
if neighbor_sw == type: val += 64
if neighbor_w  == type: val += 128

sprite_to_draw = terrain_atlas[val]



https://www.redblobgames.com/articles/autotile/index-claude-me.html
Bitmask Autotiling
A popular method that assigns binary values to neighboring positions:
* Each neighbor position gets a bit value (1, 2, 4, 8, etc.)
* The sum creates a unique index for tile selection
* Supports up to 256 different tile configurations (8-bit)






As an oldschool C coder, code it up so we have ocean and land terrain types and we want to remap a target ocean square to pictures that have shores to blend with the land squares

Bitmask Autotiling A popular method that assigns binary values to neighboring positions: Each neighbor position gets a bit value (1, 2, 4, 8, etc.) The sum creates a unique index for tile selection Supports up to 256 different tile configurations (8-bit)
It sounds like you're diving into one of the most satisfying parts of 2D game development! Bitmask Autotiling is the industry standard for making level design feel like "painting" rather than manual puzzle-solving.
By checking the neighbors of a tile and assigning a unique value to each direction, you can instantly swap a generic square for a corner, an edge, or an intersection.
How the Math Works
The system typically uses a Powers of 2 logic. Each neighbor is assigned a specific bit value. If a neighbor of the same type exists in that direction, you add that value to a running total (the index). 
For a standard 4-neighbor (North, West, East, South) setup, the math looks like this:
Neighbor Direction 	Bit Value	Binary
North	1	
West	2	
East	4	
South	8	
The Result: A sum between 0 (no neighbors) and 15 (surrounded on all 4 sides). This maps perfectly to a 16-tile spritesheet. 
Expanding to 8-Bit (256 Tiles)
When you include diagonals (Northwest, Northeast, Southwest, Southeast), you reach the 256-tile configuration you mentioned. This allows for much smoother transitions, though it is significantly more complex to draw. 
Most developers use a Blob Tileset (47 tiles). Even though there are 256 mathematical combinations, many of them look identical visually. For example, if you don't have neighbors to the North or East, the "Northeast" corner doesn't actually change the tile's shape. 
Common Implementation Logic
In code, the check usually looks something like this:
python
# Pseudo-code example
mask = 0
if get_neighbor(x, y-1): mask += 1  # North
if get_neighbor(x-1, y): mask += 2  # West
if get_neighbor(x+1, y): mask += 4  # East
if get_neighbor(x, y+1): mask += 8  # South

tile_to_draw = spritesheet[mask]
Use code with caution.

Why this is a Game Changer:
Speed: You can prototype levels in seconds.
Consistency: Eliminates "seam" errors where tiles don't align.
Dynamic: If a player destroys a block (like in Terraria or Minecraft), the surrounding blocks automatically "heal" and update their shapes. 
Would you like me to provide a specific 47-tile mapping layout or a code snippet in a specific language (like C#, Python, or GDScript) to implement this?







15 sites
Memory – Part 6: Optimizing the FIFO and Stack allocators · Intersec TechTalk
Sep 28, 2014 — The key for efficient computation is to use the fact that alignment is always a power of two. The choice in implementation resides...

techtalk.intersec.com

How to Use Tile Bitmasking to Auto-Tile Your Level Layouts | Envato Tuts+
Feb 3, 2016 — Overview Tile bitmasking is all about calculating a numerical value and assigning a specific sprite based on that value. Each tile...

Envato Tuts+

I've been working on a 3D Tilemap tool for my game, here's a demo of it's features and how it looks in my game. : r/Unity3D
Mar 31, 2021 — Basically each neighbour direction is assigned bit value, you add up every neighbour with the same layer (in this case dirt on gra...

Reddit

Show all
As an oldschool C code, code it up so we have ocean and land terrain types and we want to remap a target ocean square to pictures that have shores to blend with the land squares

To implement this in "Oldschool C," we will focus on a 4-neighbor bitmask (North, West, East, South). This creates 16 unique tile configurations, which is the perfect balance for a classic RPG or strategy game aesthetic.

In this scenario, we check the neighbors of an Ocean tile. If the neighbor is Land, we set the bit. This tells us exactly which "Shore" sprites to draw to blend the ocean into the land.

The Bitmask Logic
We define our directions using bit-shifting (
):
c
#define SHORE_N (1 << 0) // 1
#define SHORE_W (1 << 1) // 2
#define SHORE_E (1 << 2) // 4
#define SHORE_S (1 << 3) // 8

typedef enum { LAND, OCEAN } Terrain;

/* A simple map structure */
#define MAP_W 20
#define MAP_H 15
Terrain world_map[MAP_H][MAP_W];

The Implementation

This function calculates the mask for a specific coordinate. It treats the edges of the map as "Land" to ensure the ocean has a border.

#include <stdio.h>

/**
 * Returns the tile index (0-15) for an ocean square 
 * based on adjacent land tiles.
 */
int get_ocean_tile_index(int x, int y) {
    int mask = 0;

    // Check North
    if (y > 0 && world_map[y-1][x] == LAND) mask |= SHORE_N;
    // Check West
    if (x > 0 && world_map[y][x-1] == LAND) mask |= SHORE_W;
    // Check East
    if (x < MAP_W - 1 && world_map[y][x+1] == LAND) mask |= SHORE_E;
    // Check South
    if (y < MAP_H - 1 && world_map[y+1][x] == LAND) mask |= SHORE_S;

    return mask;
}

/**
 * Visualizing the draw loop
 */
void draw_map() {
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            if (world_map[y][x] == LAND) {
                printf(" L "); // Draw grass/dirt
            } else {
                int index = get_ocean_tile_index(x, y);
                // In a real game, you'd use: draw_sprite(tile_atlas[index], x, y);
                printf("%02d ", index); 
            }
        }
        printf("\n");
    }
}

How to Map the Sprites

The index returned (0–15) corresponds to a specific visual layout. You would arrange your spritesheet to match these indices:

Index	Visual Description	Usage
    0	Pure Water	        No land nearby.
    1	North Shore	        Land is directly above.
    3	NW Corner	        Land is North and West (Top-Left corner).
    15	Pond / Hole     	Surrounded by land on all 4 sides.

Handling the "Oldschool" Edge Cases

Map Boundaries: In the code above, if an ocean tile is at x=0, it simply doesn't check for a neighbor. If you want the ocean to "tuck under" the edge of the screen, you can default those boundary checks to LAND.
Lookup Table: If you have more than 16 tiles (e.g., you want variations for "Pure Water"), you can use a lookup table:
int actual_sprite = shore_lookup_table[mask];
Would you like me to show you how to extend this to include diagonal checks (the 47-tile "Blob" method) to handle inner corners?

