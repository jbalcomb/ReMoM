
I must already have something like this somewhere, right?

~ terrain type tests/categorizations?

C:\STU\devel\ReMoM\MoM\src\Terrain.c

Map_Square_Is_Embarkable()
vs. Square_Is_Sailable()?
vs. Square_Is_OceanLike()?
vs. Square_Is_Ocean()?



C:\STU\devel\ReMoM\MoX\src\MOX_DEF.h
/* DELETEME */  #define TERRAIN_TYPE(_wx_, _wy_, _wp_)        ( (int16_t)GET_2B_OFS(_world_maps, (((_wp_) * WORLD_SIZE * 2) + ((_wy_) * WORLD_WIDTH * 2) + ((_wx_) * 2))) % NUM_TERRAIN_TYPES )

C:\STU\devel\ReMoM\MoM\src\MAPGEN.c
terrain_type = p_world_map[wp][itr_wy][itr_wx];

C:\STU\devel\ReMoM\MoX\src\MOM_DAT.c
// WZD dseg:9CDC
uint8_t * _world_maps;
/*
Declare a pointer to a 2D array of the specified dimensions.
The sizes for dim2 and dim3 must be visible to the compiler.
int (*array_ptr)[dim2][dim3]; 
*/
int16_t (*p_world_map)[40][60];

C:\STU\devel\ReMoM\MoM\src\ALLOC.c
Allocate_Data_Space()
    _world_maps = (uint8_t *)Allocate_Next_Block(World_Data, ( ((((NUM_PLANES * WORLD_SIZE) + WORLD_OVERRUN) * sizeof(int16_t)) / SZ_PARAGRAPH_B) + 2) );  // 602 PR, 9632 B  ... still don't know how they got the +2 for the headers. maybe header and subheader
    p_world_map = (int16_t (*)[WORLD_HEIGHT][WORLD_WIDTH])_world_maps;



## Map_Square_Is_Embarkable() vs. Square_Is_Sailable() vs. Square_Is_OceanLike() vs. Square_Is_Ocean()

Quick comparison summary:
- Broadest water-ish checks: Square_Is_Sailable and Square_Is_OceanLike.
- AI embark-path filter: Map_Square_Is_Embarkable.
- Strict/single-type check: Square_Is_Ocean.
- Built-in coordinate safety: Square_Is_OceanLike only (among these four).

Usage/purpose notes from current call sites:
- Map_Square_Is_Embarkable is used by AI movement/transport logic (for embark/disembark decisions), not as a city/building gate.
- Square_Is_Sailable is the broad gameplay "on water" style predicate used in several systems (movement/status/UI/economy style checks).
- Square_Is_OceanLike is used where shoreline/ocean-like context is desired with coordinate handling (e.g., AI shoreline/ocean-adjacent checks and cityscape ocean background selection).
- Square_Is_Ocean is a narrow single-terrain test for deep-ocean animation class only.

About ship-building prerequisites (Ship Wright's Guild / Ship Yard / Maritime Guild):
- In this codebase snapshot, those building IDs are clearly handled in CITYSCAP placement/port visuals and related flow, but not through these four terrain predicates directly.
- There is also a placeholder comment for WZD s161p27 (Building_Terrain_Type_Prerequisite) in Terrain.c, which suggests historical building-requirement terrain logic may be separate/unfinished or represented elsewhere.
- So: it is correct to document purpose/usage here, but avoid claiming that one of these four is currently the direct ship-building gate unless we add a traced requirement function reference.

### Map_Square_Is_Embarkable()

Purpose:
- AI embark/disembark candidate check. This is not a generic "water" test.

Implementation shape:
- Uses direct terrain read from p_world_map[wp][wy][wx].
- No coordinate safety checks in this function.

Current decision bands:
- Immediate false: tt_BugGrass, tt_Lake.
- True: terrain_type < _Shore11101110.
- False: _Shore11101110 <= terrain_type < _Shore10111000.
- True: _Shore10111000 <= terrain_type < tte_Grasslands.
- False: tte_Grasslands <= terrain_type < _Shore00001R10.
- True: _Shore00001R10 <= terrain_type < _River1100_3.
- False: _River1100_3 <= terrain_type < _Shore1100000R.
- True: _Shore1100000R <= terrain_type < _Shore1000111R.
- Final false: terrain_type >= _Shore1000111R.

Takeaway:
- This is a narrow, threshold-driven AI embarkability predicate with included and excluded shoreline bands.

### Square_Is_Sailable()

Purpose:
- Broad "can be treated as water/sailable" predicate used in multiple gameplay contexts.

Implementation shape:
- Uses TERRAIN_TYPE() macro (legacy accessor path).
- No wx/wy bounds guard inside this function.

Behavior notes:
- Returns true for a wide set of ocean/shore/lake-like IDs and shoreline ranges.
- Unlike OceanLike, this path includes the single-square lake case because it does not special-case tt_Lake as false.
- Uses historical threshold/goto flow; behavior is broader than Embarkable.

Takeaway:
- If you need generic "water-ish/sailable" behavior, this is usually closer than Embarkable.

### Square_Is_OceanLike()

Purpose:
- AI-oriented "ocean-like" check; very close to Sailable but not identical.

Implementation shape:
- Uses TERRAIN_TYPE() macro.
- Wraps wx for cylindrical map behavior and validates wy range before terrain lookup.

Behavior notes:
- Explicitly excludes tt_BugGrass and tt_Lake in the low terrain range.
- Tracks most shoreline/ocean ranges similarly to Sailable.
- Has safer coordinate handling than Sailable and Embarkable.

Takeaway:
- Prefer this when caller may hand in edge coordinates and you want "sailable-like" logic with built-in coordinate sanity.

### Square_Is_Ocean()

Purpose:
- Very strict deep-ocean test.

Implementation shape:
- Uses TERRAIN_TYPE() macro.
- Returns true only when terrain_type == tt_OceanAnim.

Behavior notes:
- This is not a shoreline check.
- This is not a generic water check.

Takeaway:
- Use only when you really mean this single terrain class.
