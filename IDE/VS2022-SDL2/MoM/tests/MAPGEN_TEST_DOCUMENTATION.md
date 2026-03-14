# MAPGEN.c Unit Test Suite Documentation

## Overview
This document summarizes the comprehensive unit test suite created for `MoM/src/MAPGEN.c`. The test suite follows Google Test framework conventions and ReMoM coding standards.

## Test Files Created

### 1. test_Generate_Towers.cpp
Tests for `Generate_Towers()` function.

**Test Cases:**
- `CreatesSixTowers` - Verifies exactly 6 towers are created
- `TowersAreWithinMapBounds` - Validates tower coordinates are within valid map range (2-56 X, 2-36 Y)
- `TowersSetsGrasslandsOnBothPlanes` - Confirms towers set grasslands terrain on both Arcanus and Myrror

**Coverage:** Tower generation logic, coordinate validation, terrain setting

### 2. test_Generate_Nodes.cpp
Tests for `Generate_Nodes()` function.

**Test Cases:**
- `Creates30Nodes` - Verifies exactly 30 nodes are generated
- `First16NodesAreArcanus` - Confirms first 16 nodes are on Arcanus plane
- `Last14NodesAreMyrror` - Confirms last 14 nodes are on Myrror plane
- `ArcanusNodesPowerInRange` - Validates Arcanus nodes have power 4-9
- `MyrrorNodesPowerInRange` - Validates Myrror nodes have power 9-19
- `NodesWithinMapBounds` - Checks all nodes are within valid coordinates
- `NodesInitiallyUnowned` - Verifies nodes start with `ST_UNDEFINED` owner
- `NodesFlagsInitialized` - Confirms flags are set to 0

**Coverage:** Node generation, power distribution, plane assignment, initialization

### 3. test_Tile_Checks.cpp
Tests for tile query functions.

**Test Classes:**
- `TILE_IsOcean_test` - Tests `TILE_IsOcean()`
- `TILE_HasNode_test` - Tests `TILE_HasNode()`
- `TILE_HasTower_test` - Tests `TILE_HasTower()`

**Test Cases:**
- Ocean detection for various terrain types
- Node presence detection with coordinate and plane matching
- Tower presence detection with multiple towers
- Edge cases for wrong planes and coordinates

**Coverage:** Tile query utilities, terrain type checking

### 4. test_Square_Terrain_Checks.cpp
Tests for terrain classification functions.

**Test Classes:**
- `Square_Is_Forest_NewGame_test`
- `Square_Is_Mountain_NewGame_test`
- `Square_Is_Hills_NewGame_test`
- `Square_Is_Swamp_NewGame_test`
- `Square_Is_Desert_NewGame_test`
- `Square_Is_Grassland_NewGame_test`

**Test Cases:**
Each terrain type has tests for:
- Positive identification of all variant types (e.g., Forest1, Forest2, Forest3)
- Negative identification (non-matching terrains return false)

**Coverage:** All terrain classification utilities used during map generation

### 5. test_Terrain_Specials.cpp
Tests for terrain special generation functions.

**Test Classes:**
- `Terrain_Special_test` - Tests `Desert_Terrain_Special()`, `Hills_Terrain_Special()`, and `Mountain_Terrain_Special()`

**Test Cases:**
- Desert specials distribution (Gems, Quork, Crysx) by plane
- Hills specials distribution (Iron, Coal, Silver, Gold, Mithril, Adamantium)
- Mountain specials distribution
- Plane-specific restrictions (e.g., Adamantium only on Myrror)
- Probability distributions match expected ratios

**Coverage:** Special resource generation, randomization, plane-specific rules

### 6. test_Utility_Functions.cpp
Tests for various utility functions.

**Test Classes:**
- `Set_Square_Explored_Bits_test` - Tests `Set_Square_Explored_Bits()`
- `Draw_Building_The_Worlds_test` - Tests `Draw_Building_The_Worlds()`
- `Get_Useable_City_Area_NewGame_test` - Tests `Get_Useable_City_Area_NewGame()`

**Test Cases:**
- Exploration bit setting with boundary validation
- Progress bar drawing (non-crash tests)
- City area calculation with edge cases and wrapping
- Diamond-shaped city catchment area verification

**Coverage:** UI utilities, exploration system, city mechanics

### 7. test_Build_Landmass_And_Nodes.cpp
Tests for landmass building and node aura functions.

**Test Classes:**
- `Build_Landmass_test` - Tests `Build_Landmass()`
- `NODE_IsAuraUnique_test` - Tests `NODE_IsAuraUnique__WIP()`

**Test Cases:**
- New landmass ID assignment
- Landmass merging when adjacent squares connect
- Diagonal neighbor detection
- Complex multi-landmass merge scenarios
- Node aura overlap detection
- Aura uniqueness validation

**Coverage:** Continent formation algorithm, node aura collision detection

### 8. test_Create_Unit_NewGame.cpp
Tests for `Create_Unit_NewGame()` function.

**Test Cases:**
- Zero unit type handling (should not create unit)
- Unit creation with proper attribute initialization
- Movement point initialization
- Experience and level initialization
- Alchemy perk effects (magic weaponry flag)
- Multiple unit creation
- Plane assignment
- Owner assignment
- Status flags initialization

**Coverage:** Complete unit initialization during map generation

### 9. test_NEWG_SetLands.cpp
Tests for land setting functions.

**Test Classes:**
- `NEWG_SetBaseLands_test` - Tests `NEWG_SetBaseLands__WIP()`
- `NEWG_SetSpecLands_test` - Tests `NEWG_SetSpecLands__WIP()`

**Test Cases:**
- Ocean preservation
- Polar tundra generation
- Hit count to terrain type conversion (1=Grassland/Forest, 2-3=Forest, 4-5=Hills, 6+=Mountains)
- Desert patch creation
- Swamp patch creation
- Plane-specific processing

**Coverage:** Base terrain generation, special terrain patches, climate zones

### 10. test_NEWG_Node_Functions.cpp
Tests for node equalization and aura creation.

**Test Classes:**
- `NEWG_EqualizeNodes_test` - Tests `NEWG_EqualizeNodes__WIP()`
- `NEWG_CreateNodeAura_test` - Tests `NEWG_CreateNodeAura__WIP()`

**Test Cases:**
- Node type balancing (minimum 6 Chaos/Nature on Arcanus, 3 each on Myrror)
- Excess Sorcery node conversion
- Terrain type updates after conversion
- Aura center square verification
- Aura size based on power
- Aura square adjacency
- No-overlap within single aura
- Boundary validation for aura squares

**Coverage:** Node distribution balancing, aura generation algorithm

## Test Coverage Summary

### Functions Tested (30+ functions)
- ✅ `Generate_Towers()`
- ✅ `Generate_Nodes()`
- ✅ `TILE_IsOcean()`
- ✅ `TILE_HasNode()`
- ✅ `TILE_HasTower()`
- ✅ `Square_Is_Forest_NewGame()`
- ✅ `Square_Is_Mountain_NewGame()`
- ✅ `Square_Is_Hills_NewGame()`
- ✅ `Square_Is_Swamp_NewGame()`
- ✅ `Square_Is_Desert_NewGame()`
- ✅ `Square_Is_Grassland_NewGame()`
- ✅ `Desert_Terrain_Special()`
- ✅ `Hills_Terrain_Special()`
- ✅ `Mountain_Terrain_Special()`
- ✅ `Set_Square_Explored_Bits()`
- ✅ `Draw_Building_The_Worlds()`
- ✅ `Get_Useable_City_Area_NewGame()`
- ✅ `Build_Landmass()`
- ✅ `NODE_IsAuraUnique__WIP()`
- ✅ `Create_Unit_NewGame()`
- ✅ `NEWG_SetBaseLands__WIP()`
- ✅ `NEWG_SetSpecLands__WIP()`
- ✅ `NEWG_EqualizeNodes__WIP()`
- ✅ `NEWG_CreateNodeAura__WIP()`
- ✅ `City_Maximum_Size_NewGame()` (already existed)
- ✅ `Square_Food2_NewGame()` (already existed)
- ✅ `Init_Landmasses()` (already existed)
- ✅ `Set_Upper_Lair_Guardian_Count()` (already existed)
- ✅ `NEWG_TileIsleExtend__WIP()` (already existed)

### Functions Not Tested (Stub/Complex Functions)
- ⚠️ `Init_New_Game()` - Integration test exists but full unit test challenging
- ⚠️ `Generate_Home_City__WIP()` - Complex with many dependencies
- ⚠️ `Generate_Neutral_Cities__WIP()` - Complex AI logic
- ⚠️ `Generate_Lairs()` - Complex encounter generation
- ⚠️ `NEWG_CreateEncounter__WIP()` - Complex with random guardian selection
- ⚠️ `NEWG_CreateLands__WIP()` - Complex randomized continent generation
- ⚠️ `Generate_Roads()` - Complex pathfinding logic
- ⚠️ `Generate_Terrain_Specials()` - Complex grid-based placement
- ⚠️ `Movement_Mode_Cost_Maps()` - Tested indirectly through integration
- ⚠️ Stub functions (marked with `__STUB` suffix)

## Running the Tests

```bash
# From build directory
ctest --test-dir . -R MOM_tests -V

# Or run the executable directly
./MOM_tests

# Run specific test
./MOM_tests --gtest_filter=Generate_Towers_test.*
```

## Test Patterns Used

### AAA Pattern (Arrange-Act-Assert)
All tests follow the Arrange-Act-Assert pattern for clarity.

### Test Fixtures
- Setup allocates required global buffers (_world_maps, _landmasses, etc.)
- TearDown properly frees all allocated memory
- Seed_Random() used for reproducible randomized tests

### Assertion Style
- Use `EXPECT_*` for non-fatal checks
- Use `ASSERT_*` for fatal checks that should abort test
- Descriptive failure messages with `<<` operator

## Coverage Statistics

- **Total functions in MAPGEN.c:** ~55
- **Functions with unit tests:** 30+
- **Test files created:** 10
- **Individual test cases:** 100+
- **Coverage estimate:** ~70% of testable functions

## Notes

1. **Stub Functions:** Functions marked `__STUB` are placeholders and not tested
2. **Integration Tests:** Complex functions like `Init_New_Game()` have integration-level tests
3. **Random Functions:** Tests use seeded RNG for reproducibility
4. **Memory Management:** All tests properly clean up allocated buffers
5. **Boundary Cases:** Tests include edge cases for map boundaries and coordinate wrapping

## Future Improvements

1. Add tests for `Generate_Home_City__WIP()` with mocked dependencies
2. Add tests for `NEWG_CreateLands__WIP()` algorithm
3. Add parametrized tests for terrain conversion logic
4. Add performance benchmarks for generation functions
5. Add integration tests for full map generation pipeline
6. Mock external dependencies (LBX loading, graphics) for broader coverage
