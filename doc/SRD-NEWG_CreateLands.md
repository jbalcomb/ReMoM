# SRD: NEWG_CreateLands — Continent Generation

| Field         | Value                                         |
|---------------|-----------------------------------------------|
| Module        | MAPGEN (`MoM/src/MAPGEN.c`)                   |
| Function      | `NEWG_CreateLands__WIP(int16_t wp)`           |
| Origin        | `MAGIC.EXE` overlay 051, procedure 10 (o51p10) |
| Drake178 Name | `NEWG_CreateLands()`                          |
| Status        | WIP (decompilation in progress)               |
| Date          | 2026-02-20                                    |

SEEALSO: [MoX-Landmasses.md](MoX-Landmasses.md)
SEEALSO: [MoX-MapGen.md](MoX-MapGen.md)
SEEALSO: [MoX-WorldMap.md](MoX-WorldMap.md)
SEEALSO: [MoX-NewGame-MapGen-Landmass.md](MoX-NewGame-MapGen-Landmass.md)



## 1. Purpose

`NEWG_CreateLands__WIP()` generates the base continent shapes for a single world plane (Arcanus or Myrror). It is the primary land-distribution step during new-game world creation. The function fills the 60x40 tile plane with ocean, then grows organic continent blobs using a Drunkard's Walk (random walk) algorithm. Each tile records how many times a walker visited it; this hit-count is later interpreted as terrain type by downstream functions.



## 2. Position in the Map Generation Pipeline

`NEWG_CreateLands__WIP()` is called from `Init_New_Game()` as part of the "Building The Worlds..." sequence. The full pipeline is:

| Step | % Bar | Function                           | What It Does                                    |
|------|-------|------------------------------------|-------------------------------------------------|
| 1    | 5-10  | `Init_Landmasses(wp)`              | Zeros the landmass ID array; resets counter to 1 |
| 2    | 15-20 | **`NEWG_CreateLands__WIP(wp)`**    | Fills plane with ocean, walks continents, records hit-counts, assigns landmass IDs |
| 3    | 25-30 | `NEWG_SetBaseLands__WIP(wp)`       | Converts hit-counts to terrain types (Grassland/Forest/Hills/Mountain) + tundra at poles |
| 4    | 35-40 | `NEWG_SetSpecLands__WIP(wp)`       | Adds tundra gradient, desert patches, swamp patches |
| 5    | 45    | `Generate_Nodes()`                 | Places 30 magic nodes across both planes         |
| 6    | 50-55 | `NEWG_EqualizeNodes__WIP(wp)`      | Rebalances node types (Chaos/Nature/Sorcery)     |
| 7    | 60    | `Generate_Towers()`                | Places Towers of Wizardry                        |
| 8    | 60    | `NEWG_TileIsleExtend__WIP(wp)`     | Grows land around isolated node/tower islands    |
| 9    | 65+   | Lairs, cities, specials, roads, shores, rivers... | Remaining world features |

Each plane (Arcanus, Myrror) is processed separately in sequence. Steps 1-4 each run twice (once per plane) before proceeding to step 5.



## 3. Signature and Parameters

```c
void NEWG_CreateLands__WIP(int16_t wp);
```

| Parameter | Type      | Description                                           |
|-----------|-----------|-------------------------------------------------------|
| `wp`      | `int16_t` | World plane index: `ARCANUS_PLANE` (0) or `MYRROR_PLANE` (1) |



## 4. Preconditions

| Condition                        | Ensured By                   | Notes |
|----------------------------------|------------------------------|-------|
| `_world_maps` buffer allocated   | `Allocate_Data_Space()`      | 602 paragraphs (9632 bytes) for both planes |
| `_landmasses` buffer allocated   | `Allocate_Data_Space()`      | 302 paragraphs (4832 bytes) for both planes |
| Landmass array zeroed, counter=1 | `Init_Landmasses(wp)`        | Must be called before `NEWG_CreateLands__WIP()` for the same plane |
| `_landsize` set                  | New Game UI                  | One of `gol_Small`, `gol_Medium`, `gol_Large` |
| RNG seeded                       | `Randomize()`                | Called during game initialization |



## 5. Postconditions

After `NEWG_CreateLands__WIP()` returns for a given plane:

1. **All tiles initialized.** Every tile on the plane has been written at least once (ocean baseline).
2. **Land tiles contain hit-counts.** Non-ocean tiles hold a small positive integer (1-6+) representing how many times a walker visited them. These are *not* final terrain type values yet.
3. **Landmass IDs assigned.** Every land tile has a landmass ID via `Build_Landmass()`. Adjacent land tiles share the same ID. The global `m_landmasses_ctr` reflects the next available ID.
4. **Land quantity meets target.** At least `n_needed` tiles have been converted from ocean to land.
5. **Border margins preserved.** No land is placed within 2 tiles of the X edges or 4 tiles of the Y edges.



## 6. Algorithm — Step by Step

### 6.1 Initialize Plane to Ocean

```
for every tile (wx, wy) on the plane:
    SET_TERRAIN_TYPE(wx, wy, wp, tt_Ocean1)    // tt_Ocean1 = 0x0
```

All 2400 tiles (60 x 40) are set to ocean.


### 6.2 Clear the Section Tracking Grid

A local 5x5 array `map_sections[5][5]` is zeroed. This grid prevents all continent origins from clustering in the same area.

> **Known inconsistency:** The array is 5x5 but only a 4x3 portion is reachable given the coordinate ranges and divisors.


### 6.3 Set Parameters

| Variable         | Value | Purpose |
|------------------|-------|---------|
| `n_sections`     | 8     | Number of map sections that must host at least one origin |
| `section_width`  | 16    | Width of each section in tiles |
| `section_height` | 11    | Height of each section in tiles |

> **Known inconsistency:** 60/16 = 3.75 and 40/11 = 3.6 do not divide evenly. These values may be artifacts of an earlier map size.


### 6.4 Determine Land Target from `_landsize`

| `_landsize`  | `n_needed` | % of 2400 tiles |
|--------------|-----------|-----------------|
| `gol_Small`  | 360       | 15%             |
| `gol_Medium` | 480       | 20%             |
| `gol_Large`  | 720       | 30%             |


### 6.5 Main Loop: Generate Continent Blobs

The outer loop `while(n_generated <= n_needed)` repeats until enough land exists. Each iteration creates one continent blob.

#### 6.5.1 Pick an Origin with Section Spreading

```
base_wx = 6 + Random(46)     // range: {7, ..., 52}
base_wy = 6 + Random(26)     // range: {7, ..., 32}
```

The section grid is consulted:
- If `n_sections > 0` and the section containing `(base_wx, base_wy)` is unused, mark it used and decrement `n_sections`.
- If the section is already used, re-roll (infinite loop until an unused section is found).
- If `n_sections <= 0` (all 8 sections claimed), accept any origin immediately.

This guarantees the first 8 continent origins are spread across different regions.


#### 6.5.2 Determine Arm Count

```
direction_change_count = 2 + Random(3)     // range: {3, 4, 5}
```

Each continent has 3-5 "arms" radiating from the origin point, creating multi-lobed shapes.


#### 6.5.3 For Each Arm: Determine Walk Length

| `_landsize`  | `Steps_To_Take`    |
|--------------|-------------------|
| `gol_Small`  | 5 + Random(10) = {6, ..., 15}   |
| `gol_Medium` | 10 + Random(10) = {11, ..., 20} |
| `gol_Large`  | 20 + Random(10) = {21, ..., 30} |


#### 6.5.4 Set Arm Starting Position

```
curr_wx = base_wx + TILE_Cardinal_XMod2[arm_index]
curr_wy = base_wy + TILE_Cardinal_YMod2[arm_index]
```

The cardinal direction arrays are `{S, W, N, E, none}` = `{(0,1), (-1,0), (0,-1), (1,0), (0,0)}`, so each arm index starts one step in a different cardinal direction from the origin. When there are 5 arms, the last arm starts at the origin itself.


#### 6.5.5 Walk the Arm (Inner Step Loop)

For each step, while `Steps_Taken < Steps_To_Take` and `n_generated <= n_needed`:

**a) Count new land:**
If the current tile is still `tt_Ocean1`, increment `n_generated`.

**b) Record hit-count:**
The terrain type value at the current tile is incremented by 1. This builds a density map where repeated visits produce higher values.

**c) Assign landmass ID:**
`Build_Landmass(wp, curr_wx, curr_wy)` is called. This function:
- Checks the center tile plus all 8 neighbors for existing landmass IDs.
- If no neighbors have a landmass, assigns a new unique ID from `m_landmasses_ctr++`.
- If neighbors have different landmass IDs, merges them all to the first found ID (full plane scan to reassign).

**d) Choose next direction (anti-straight-line bias):**
A random cardinal direction (0-3 mapping to S/W/N/E) is chosen. The algorithm biases against continuing in the same direction:

```
if Direction == Last_Direction:
    accept only with probability 1 / (Same_Dir_Steps * 2)
    if accepted: Same_Dir_Steps++
else:
    Same_Dir_Steps = 1
```

This prevents long straight coastlines and creates organic, blobby shapes. The longer the walker has gone straight, the less likely it is to continue straight.

**e) Boundary check:**
The candidate position is tested against map borders with margins:

```
reject if:
    next_wx < WORLD_XMIN + 2     (< 2)
    next_wy < WORLD_YMIN + 4     (< 4)
    next_wx >= WORLD_XMAX - 2    (>= 57)
    next_wy >= WORLD_YMAX - 4    (>= 35)
```

If out of bounds, `Last_Direction` is set to `ST_UNDEFINED` and the direction loop retries. This keeps land away from map edges.

**f) Advance:**
`curr_wx = next_wx; curr_wy = next_wy;`


### 6.6 Termination

The function returns when `n_generated > n_needed`. The plane now has the target amount of land distributed across several irregular continents.



## 7. Key Data Structures

### 7.1 Global Buffers

| Buffer          | Type        | Size per Plane | Access Macros |
|-----------------|-------------|----------------|---------------|
| `_world_maps`   | `uint8_t *` | 4800 bytes (2400 tiles x 2 bytes) | `GET_TERRAIN_TYPE(wx,wy,wp)` / `SET_TERRAIN_TYPE(wx,wy,wp,val)` |
| `_landmasses`   | `uint8_t *` | 2400 bytes (2400 tiles x 1 byte)  | `GET_LANDMASS(wx,wy,wp)` / `SET_LANDMASS(wx,wy,wp,val)` |

### 7.2 Global Scalars

| Variable             | Type      | Description |
|----------------------|-----------|-------------|
| `m_landmasses_ctr`   | `int16_t` | Next available landmass ID (starts at 1) |
| `_landsize`          | `int16_t` | Game setting: `gol_Small`/`gol_Medium`/`gol_Large` |

### 7.3 Direction Arrays

```c
// South, West, North, East, None
int16_t TILE_Cardinal_XMod[5]  = {  0, -1,  0,  1,  0 };
int16_t TILE_Cardinal_YMod[5]  = {  1,  0, -1,  0,  0 };
int16_t TILE_Cardinal_XMod2[5] = {  0, -1,  0,  1,  0 };
int16_t TILE_Cardinal_YMod2[5] = {  1,  0, -1,  0,  0 };
```

Both arrays are identical. `TILE_Cardinal_XMod`/`YMod` are used for the random walk step; `XMod2`/`YMod2` are used for the arm starting offset.

### 7.4 Local State: `map_sections[5][5]`

Tracks which coarse grid sections have already received a continent origin. Each cell is either 0 (unused) or 1 (used). Indexed by `[base_wy / section_height][base_wx / section_width]`.

### 7.5 RNG: `Random(n)`

Returns a value in `{1, ..., n}` inclusive. Uses an xorshift PRNG seeded at game start.



## 8. Constants

| Constant        | Value | Source |
|-----------------|-------|--------|
| `WORLD_WIDTH`   | 60    | `MOM_DEF.h` |
| `WORLD_HEIGHT`  | 40    | `MOM_DEF.h` |
| `WORLD_SIZE`    | 2400  | `MOM_DEF.h` |
| `WORLD_XMIN`    | 0     | `MOM_DEF.h` |
| `WORLD_XMAX`    | 59    | `MOM_DEF.h` |
| `WORLD_YMIN`    | 0     | `MOM_DEF.h` |
| `WORLD_YMAX`    | 39    | `MOM_DEF.h` |
| `NUM_PLANES`    | 2     | `MOM_DEF.h` |
| `ARCANUS_PLANE` | 0     | `MOM_DEF.h` |
| `MYRROR_PLANE`  | 1     | `MOM_DEF.h` |
| `tt_Ocean1`     | 0x0   | `TerrType.h` |
| `ST_UNDEFINED`  | -1    | convention  |



## 9. Downstream Consumers

### 9.1 `NEWG_SetBaseLands__WIP(wp)` — Hit-Count to Terrain Conversion

This is the immediate consumer of the hit-count values written by `NEWG_CreateLands__WIP()`. It interprets the raw integer stored in each non-ocean tile:

| Hit-Count | Terrain Type         | Notes |
|-----------|----------------------|-------|
| 0         | Ocean (unchanged)    | Never visited by a walker |
| 1         | Grasslands (75%) or Forest (25%) | Single pass; coastal fringe |
| 2-3       | Forest               | Light overlap |
| 4-5       | Hills                | Moderate overlap |
| 6+        | Mountain             | Heavy overlap; continent interior |

After terrain assignment, it also applies tundra strips to the top and bottom rows with random extensions into adjacent rows.

### 9.2 `NEWG_SetSpecLands__WIP(wp)` — Special Terrain Patches

Adds climate zones on top of the base terrain:
- **Tundra gradient:** Converts Grassland/Forest in rows 2-7 and 32-37 to Tundra with distance-decay probability.
- **Desert patches:** Up to 8 random-walk desert regions on Forest tiles.
- **Swamp patches:** Up to 8 random-walk swamp regions on Forest tiles (avoiding mid-map Y range).

### 9.3 `Build_Landmass(wp, wx, wy)` — Landmass ID Tracking

Called once per walker step. Maintains a union-find-like structure of landmass IDs across the plane. When walker paths from different blobs touch, their landmass IDs are merged so the entire connected land area shares one ID. Used later by city placement, AI evaluation, and other systems that need to know which continent a tile belongs to.



## 10. Known Issues and Quirks

### 10.1 Section Grid Sizing Inconsistency

The `map_sections` array is 5x5 but with `section_width=16` and `section_height=11`, the reachable indices are `[0..2][0..3]` (only 12 cells). The remaining 13 cells are allocated but never used. The 8-section cap means only 8 of those 12 are ever marked.

### 10.2 Section Dimensions Don't Divide the World

`60 / 16 = 3.75` and `40 / 11 = 3.6`. These magic numbers may be artifacts from an earlier map size (e.g., 80x55 would give clean 5x5 divisions). The practical effect is that section boundaries don't align evenly with the world edges, and the last column/row of sections extends beyond the map.

### 10.3 Infinite Loop Risk in Origin Selection

When `n_sections > 0`, the origin selection loop has no escape condition if all reachable sections are already marked but `n_sections` hasn't reached 0. In practice this doesn't hang because 8 < 12 reachable sections, but it's fragile.

### 10.4 Direction Bias: `Random(4) - 1`

`Random(4)` returns `{1,2,3,4}`, so `Random(4) - 1` produces `{0,1,2,3}`. This maps to `{S,W,N,E}` uniformly. However, the same-direction rejection logic means the walker is biased toward direction *changes*, producing more compact blobs.

### 10.5 Border Margins Are Asymmetric

X-margin is 2 tiles; Y-margin is 4 tiles. This keeps land further from the poles (where tundra will later be placed) than from the east/west edges.

### 10.6 Duplicate Direction Arrays

`TILE_Cardinal_XMod` and `TILE_Cardinal_XMod2` (and their Y counterparts) are byte-identical. Both are used in this function for different purposes (walk steps vs. arm offsets). The duplication may be a decompilation artifact or may have served different values at some point in development.



## 11. Documentation

### 11.1 Source Code

The function's doxygen comment block is at `MAPGEN.c:2046-2069`. It should be kept in sync with this document. Key fields:

- `@brief` — one-line summary
- `@details` — algorithm overview
- `@param wp` — plane index
- `@see` — cross-references to `Init_Landmasses`, `NEWG_SetBaseLands__WIP`, `Build_Landmass`

### 11.2 Related Documentation Files

| File | Contents |
|------|----------|
| `doc/MoX-Landmasses.md`               | Landmass data structures and `Build_Landmass` details |
| `doc/MoX-MapGen.md`                   | Map generation module overview |
| `doc/MoX-WorldMap.md`                 | World map data layout |
| `doc/MoX-NewGame-MapGen-Landmass.md`  | Working notes on this function |
| `doc/MoX-MapGen-Debug.md`             | Debug tracing notes and terrain type reference |

### 11.3 Inline Comments

The function contains extensive inline comments explaining decompilation uncertainties. Comments prefixed with:
- `// DEDU` — deduction or hypothesis about original intent
- `// ;` — transcribed from disassembly annotations (drake178 / Implode community)
- `// WTF` — unclear or suspicious logic



## 12. Test Plan

Tests use Google Test (gtest) and follow the existing patterns in `MoM/tests/`. Test files are added to `MoM/tests/CMakeLists.txt` under the `MOM_tests` executable.

### 12.1 Test Fixture Setup

```cpp
class NEWG_CreateLands__WIP_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _world_maps = static_cast<uint8_t *>(calloc(WORLD_SIZE * NUM_PLANES, 2));
        _landmasses = static_cast<uint8_t *>(calloc(WORLD_SIZE * NUM_PLANES, 1));
        m_landmasses_ctr = 1;
    }

    void TearDown() override
    {
        free(_world_maps);  _world_maps = nullptr;
        free(_landmasses);  _landmasses = nullptr;
    }
};
```

### 12.2 Test Cases

#### TC-01: Ocean Baseline — All Tiles Start as Ocean

**Given:** `_landsize = gol_Small`, plane = `ARCANUS_PLANE`
**When:** `NEWG_CreateLands__WIP(ARCANUS_PLANE)` is called
**Verify:** Before any walking begins (or by checking the function's behavior), every tile was initialized to `tt_Ocean1`. This is implicitly tested by TC-02.

#### TC-02: Land Count Meets Target — Small

**Given:** `_landsize = gol_Small`
**When:** `NEWG_CreateLands__WIP(ARCANUS_PLANE)` completes
**Then:** Count tiles where `GET_TERRAIN_TYPE(wx, wy, ARCANUS_PLANE) != tt_Ocean1`. The count must be >= 361 (n_needed=360, loop exits when n_generated > n_needed).

#### TC-03: Land Count Meets Target — Medium

Same as TC-02 but with `_landsize = gol_Medium`. Expected land count >= 481.

#### TC-04: Land Count Meets Target — Large

Same as TC-02 but with `_landsize = gol_Large`. Expected land count >= 721.

#### TC-05: Land Count Scales with Land Size

**Given:** Run all three land sizes with the same random seed.
**Then:** `count_Large > count_Medium > count_Small`.

#### TC-06: Border Margin — No Land in Restricted Zones

**Given:** Any `_landsize`
**When:** `NEWG_CreateLands__WIP(ARCANUS_PLANE)` completes
**Then:** For all tiles:
- `wx < 2` or `wx >= 57` => `tt_Ocean1`
- `wy < 4` or `wy >= 35` => `tt_Ocean1`

> Note: The walker's boundary check prevents stepping into these margins, but the arm starting offsets begin from origins that are already inside the safe zone. This test confirms no land leaks into the margins.

#### TC-07: Other Plane Untouched

**Given:** `_landsize = gol_Medium`
**When:** `NEWG_CreateLands__WIP(ARCANUS_PLANE)` completes
**Then:** Every tile on `MYRROR_PLANE` is still 0 (the calloc initial value). Confirms no cross-plane writes.

#### TC-08: Landmass IDs Are Non-Zero on Land Tiles

**Given:** Any `_landsize`
**When:** `NEWG_CreateLands__WIP(ARCANUS_PLANE)` completes
**Then:** For every tile where `GET_TERRAIN_TYPE(wx,wy,ARCANUS_PLANE) != tt_Ocean1`, `GET_LANDMASS(wx,wy,ARCANUS_PLANE) != 0`.

#### TC-09: Landmass IDs Are Zero on Ocean Tiles

**Given:** Any `_landsize`
**When:** `NEWG_CreateLands__WIP(ARCANUS_PLANE)` completes
**Then:** For every tile where `GET_TERRAIN_TYPE(wx,wy,ARCANUS_PLANE) == tt_Ocean1`, `GET_LANDMASS(wx,wy,ARCANUS_PLANE) == 0`.

#### TC-10: Hit-Count Values Are Small Positive Integers

**Given:** `_landsize = gol_Large` (highest density)
**When:** `NEWG_CreateLands__WIP(ARCANUS_PLANE)` completes
**Then:** Every non-ocean tile has a terrain value in range `[1, ~20]` (a reasonable upper bound for walker revisits). No tile should have the actual terrain enum values like 0xA2 (162) — those are assigned by `NEWG_SetBaseLands__WIP`, not this function.

#### TC-11: Deterministic with Fixed Seed

**Given:** `Set_Random_Seed(12345)` before each call
**When:** `NEWG_CreateLands__WIP(ARCANUS_PLANE)` is called twice
**Then:** The resulting `_world_maps` buffers are byte-identical.

#### TC-12: Multiple Landmass IDs Exist

**Given:** `_landsize = gol_Small` (most likely to produce separated continents)
**When:** `NEWG_CreateLands__WIP(ARCANUS_PLANE)` completes
**Then:** There are at least 2 distinct non-zero landmass IDs across the plane. (The algorithm creates up to 8+ origin points, and on small maps many will remain separate.)

### 12.3 Test Infrastructure Notes

- The `Random()` function is the real xorshift implementation. Use `Set_Random_Seed(N)` to get deterministic results for reproducible tests.
- Buffer allocation in tests must match the layout expected by `GET_TERRAIN_TYPE` / `SET_TERRAIN_TYPE` macros: `_world_maps` needs `WORLD_SIZE * NUM_PLANES * 2` bytes; `_landmasses` needs `WORLD_SIZE * NUM_PLANES` bytes.
- Follow the existing `extern "C"` wrapper pattern for C headers included in C++ test files.



## 13. Glossary

| Term | Definition |
|------|-----------|
| **Drunkard's Walk** | Random walk algorithm where each step moves one tile in a random cardinal direction. Used to generate organic, irregular shapes. |
| **Hit-count** | The number of times a walker visits a tile during continent generation. Stored temporarily in the terrain type field and later converted to actual terrain. |
| **Arm** | One of 3-5 directional walks radiating from a continent's origin point. Each arm extends the continent in a different cardinal direction. |
| **Section** | A coarse grid region of the map (16x11 tiles) used to ensure continent origins are spatially distributed. |
| **Landmass ID** | A unique per-continent identifier assigned to each land tile. Adjacent tiles that touch are merged to share the same ID. |
| **Plane** | One of two parallel worlds: Arcanus (0) or Myrror (1). Each is a separate 60x40 tile grid. |
| **Origin** | The randomly selected starting coordinate for a continent blob. |
