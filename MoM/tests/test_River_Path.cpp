/*
====================================================================================================================================
    test_River_Path.cpp -- Unit tests for River_Path() (MGC o51p22)
====================================================================================================================================

    WHAT THIS FILE TESTS
    --------------------
    River_Path() is a single-river placement function in the Master of Magic map generation pipeline.  It is called from
    Init_New_Game() (MGC o51p01) in a tight retry loop:

        for(rivers = 0; rivers < NUM_RIVERS; rivers++)              NUM_RIVERS is 10.
        {
            for(tries = 0; ((tries < 2000) && (River_Path(ARCANUS_PLANE) != 0)); tries++) { }
            for(tries = 0; ((tries < 2000) && (River_Path(MYRROR_PLANE)  != 0)); tries++) { }
        }

    Each call either places exactly one river (returns ST_TRUE == 1) or fails and leaves the map untouched (returns ST_FALSE == 0).
    After all rivers are placed, River_Terrain() (MGC o51p23) converts the temporary placeholder values into real river terrain
    types with proper cardinal connectivity.

    HOW THE ALGORITHM WORKS  (River_Path, MAPGEN.c:4318)
    ----------------------------------------------------
    Step 1 -- Pick a random starting square.
        base_wx = 4 + Random(WORLD_WIDTH  - 8)     gives range [5 .. 56]  (at least 4 squares from any edge)
        base_wy = 4 + Random(WORLD_HEIGHT - 8)     gives range [5 .. 35]

    Step 2 -- Validate the starting square.  The function rejects (returns ST_FALSE) if any of these are true:
        - The square has a terrain special  (_map_square_terrain_specials != 0)
        - Any of the 9 squares in the 3x3 neighborhood (including center) is Ocean
        - The square is Mountain, Hills, a Magic Node, or already a River

    Step 3 -- Choose a random downstream direction.
        downstream = Random(4) - 1      gives 0..3, indexing the cardinal direction tables:
            dir_chg_tbl_wx[4] = { 0, -1,  0, 1 }       Index 0 = South  (+Y)
            dir_chg_tbl_wy[4] = { 1,  0, -1, 0 }       Index 1 = West   (-X)
            upstream[4]       = { 2,  3,  0, 1 }        Index 2 = North  (-Y)
                                                         Index 3 = East   (+X)
        The upstream[] table maps each direction to its opposite:
            downstream 0 (South) -> upstream 2 (North)
            downstream 1 (West)  -> upstream 3 (East)
            downstream 2 (North) -> upstream 0 (South)
            downstream 3 (East)  -> upstream 1 (West)

    Step 4 -- Walk up to 30 attempts to build the path.  On each attempt:
        a) Choose direction for this step:
            - 50% chance (Random(2) > 1) or first step (same_dir == ST_UNDEFINED):  use the downstream direction.
            - Otherwise:  pick a random cardinal direction, rerolling if it equals the upstream opposite of downstream.
              This prevents the river from doubling back on itself.
        b) Compute the next square:  next_wx/wy = current + dir_chg_tbl[direction]
        c) Reject this step (continue to next attempt) if the next square has:
            - A terrain special
            - Mountain, Hills, or a Magic Node
            - Desert terrain
        d) Accept this step:  record the square in wx_array/wy_array, increment length.
        e) Check for outflow:  if any of the 5 cardinal neighbors (N, W, center, E, S) of the new square is Ocean,
           or the new square itself is already a River, set Have_Outflow = ST_TRUE and stop the loop.
        f) If length exceeds 28, bail out (return ST_FALSE) -- river is too long.

    Step 5 -- Final validation.
        - If the path has outflow but length < 4, reject.  Rivers must be at least 4 squares long.
        - Check the 5-square cardinal cross around the endpoint for invalid outflow shore types via TILE_InvalidOutflow().

    Step 6 -- Commit the path.
        For each recorded square, write the placeholder value TT_RIVER_PLACEHOLDER (1000) into p_world_map[wp][wy][wx].
        Return ST_TRUE.

    WORLD MAP LAYOUT
    ----------------
    The world is a 60x40 grid of squares, with two parallel planes (Arcanus and Myrror).

        WORLD_WIDTH   = 60          WORLD_XMIN = 0      WORLD_XMAX = 59
        WORLD_HEIGHT  = 40          WORLD_YMIN = 0      WORLD_YMAX = 39
        WORLD_SIZE    = 2400        (60 * 40)
        NUM_PLANES    = 2           ARCANUS_PLANE = 0    MYRROR_PLANE = 1

    The terrain map is a 3D array accessed as p_world_map[plane][wy][wx], backed by a flat byte buffer _world_maps.
    Terrain specials are a parallel flat array indexed as: _map_square_terrain_specials[(wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx].

    COORDINATE CONVENTION
    ---------------------
    wx = world X coordinate (column), range [0..59].
    wy = world Y coordinate (row),    range [0..39].
    wp = world plane index,            0 = Arcanus, 1 = Myrror.

    The direction tables use +Y = South (screen down), -Y = North (screen up):
        Index 0: South  (wx += 0, wy += 1)
        Index 1: West   (wx -= 1, wy += 0)
        Index 2: North  (wx += 0, wy -= 1)
        Index 3: East   (wx += 1, wy += 0)

    THE PLACEHOLDER VALUE
    ---------------------
    TT_RIVER_PLACEHOLDER = 1000 (0x3E8), defined in TerrType.h.  This is intentionally outside the normal e_TERRAIN_TYPES range.
    River_Path() writes 1000 into the terrain map as a temporary marker.  River_Terrain() (MGC o51p23) later resolves these into
    real river terrain enums (tt_RiverM_1st..tt_RiverM_end, tt_Rivers_1st..tt_Rivers_end) based on cardinal adjacency.

    RANDOM NUMBER GENERATOR
    -----------------------
    The game uses an XOR-shift PRNG (random.c):
        state ^= (state << 13);
        state ^= (state >> 17);
        state ^= (state << 5);
        return ((state >> 16) % max) + 1;       result is in range [1..max]
    The global seed is set via Set_Random_Seed() and advanced by each call to Random().
    The test helpers replicate this algorithm locally in Predict_Random() so we can pre-compute what River_Path() will do for
    a given seed without calling Random() and mutating the global state.

    RETURN VALUES / STATUS CONSTANTS
    --------------------------------
    ST_FALSE     =  0       River placement failed; map is unchanged.
    ST_TRUE      =  1       River placement succeeded; 4+ squares written with placeholder 1000.
    ST_UNDEFINED = -1       Used as a sentinel (e.g., "no direction chosen yet").

    WHAT THE TESTS COVER
    --------------------
    Test 1: TerrainSpecialAtStart_RejectsPathAndLeavesMapUnchanged
        Verifies the early-rejection path: when every square has a terrain special (value 1), River_Path() must return ST_FALSE
        and must not write any placeholder values to the map.  This is the simplest failure mode.

    Test 2: ManufacturedCandidate_SucceedsAndWritesExpectedPath
        Verifies the happy path: constructs a carefully controlled world state where the river will succeed, seeds the RNG to
        a known value, calls River_Path(), and then confirms that exactly 4 squares were stamped with placeholder 1000 at the
        predicted locations.  This test exercises the full algorithm including RNG consumption, direction selection, terrain
        validation, ocean outflow detection, and map writing.

====================================================================================================================================
*/

#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_Data.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/random.h"
#include "../src/MAPGEN.h"
#include "../src/TerrType.h"
#ifdef __cplusplus
}
#endif

#include <cstdlib>
#include <cstring>



/*
------------------------------------------------------------------------------------------------------------------------------------
    s_river_seed_plan -- Test helper structure
------------------------------------------------------------------------------------------------------------------------------------
    Captures the four values that fully determine a straight-line river path from a given RNG seed:

        seed        The value passed to Set_Random_Seed() before calling River_Path().  River_Path() will consume this seed
                    through Random() calls to derive base_wx, base_wy, and downstream.

        base_wx     The X coordinate of the river's starting square.  Computed as 4 + Random(WORLD_WIDTH - 8).
                    The +4 offset and the (WIDTH-8) range guarantee a margin of at least 4 squares from the map edges.

        base_wy     The Y coordinate of the river's starting square.  Computed as 4 + Random(WORLD_HEIGHT - 8).
                    Same edge-margin guarantee as base_wx.

        downstream  The primary flow direction, an index into the cardinal direction tables (0=South, 1=West, 2=North, 3=East).
                    Computed as Random(4) - 1.  The river is biased to flow in this direction on each step.

    Find_Usable_Seed() populates this structure by brute-forcing seeds until it finds one where River_Path() would produce
    a clean 3-step straight-line path that stays in bounds.
------------------------------------------------------------------------------------------------------------------------------------
*/
struct s_river_seed_plan
{
    uint32_t seed;
    int16_t base_wx;
    int16_t base_wy;
    int16_t downstream;
};



/*
------------------------------------------------------------------------------------------------------------------------------------
    Predict_Random() -- Local replica of the game's PRNG
------------------------------------------------------------------------------------------------------------------------------------
    This is a pure, side-effect-free copy of Random() (random.c, WZD s22p08).  It operates on a caller-supplied seed_state
    pointer instead of the global random_seed, so the tests can predict the RNG sequence without touching global state.

    The algorithm is an XOR-shift PRNG:
        1)  state ^= (state << 13)      left shift 13, XOR into state
        2)  state ^= (state >> 17)      right shift 17, XOR into state
        3)  state ^= (state <<  5)      left shift 5, XOR into state
        4)  Write the new state back through the pointer.
        5)  Return ((state >> 16) % max) + 1.

    The >> 16 extracts the upper 16 bits of the 32-bit state for the modulo, and the +1 shifts the range from [0..max-1]
    to [1..max].  This matches the original SimTex implementation byte-for-byte.

    Parameters:
        seed_state  Pointer to the caller's running RNG state.  Modified in place on each call.
        max         The upper bound.  Return value is in the range [1..max].

    Returns:
        A pseudo-random int16_t in [1..max].
------------------------------------------------------------------------------------------------------------------------------------
*/
static int16_t Predict_Random(uint32_t * seed_state, int16_t max)
{
    uint32_t state = *seed_state;

    state ^= (state << 13);
    state ^= (state >> 17);
    state ^= (state << 5);

    *seed_state = state;

    return (int16_t)(((state >> 16) % max) + 1);
}



/*
------------------------------------------------------------------------------------------------------------------------------------
    Count_Accepts_For_Seed() -- Simulates the direction-selection loop to check if a seed produces a usable path
------------------------------------------------------------------------------------------------------------------------------------
    River_Path() walks up to 30 attempts trying to accumulate 3 accepted downstream steps.  This helper replays that same logic
    using Predict_Random() to determine whether a given seed will produce at least 3 downstream-direction steps before the
    30-attempt limit.

    It does NOT check terrain or bounds -- only whether the RNG sequence yields enough downstream steps.  The terrain and bounds
    checks are handled separately by Find_Usable_Seed().

    Walk-through of the logic:

    1)  Consume the first three Random() calls that River_Path() makes:
            (void)Predict_Random(... WORLD_WIDTH  - 8)      consumes the base_wx roll
            (void)Predict_Random(... WORLD_HEIGHT - 8)      consumes the base_wy roll
            downstream = Predict_Random(... 4) - 1           consumes the downstream direction roll

    2)  Enter the direction-selection loop (mirrors River_Path lines 4379-4435):

        On each iteration:
            a)  If Random(2) > 1  OR  this is the first step (same_dir == ST_UNDEFINED):
                    direction = downstream.
                The "first step" case guarantees the river always starts by moving in the downstream direction.
                The Random(2) > 1 gives a 50% chance of continuing downstream on subsequent steps.

            b)  Otherwise, pick a random direction that is not the upstream opposite:
                    do { direction = Random(4) - 1; } while(upstream_tbl[downstream] == direction);
                The upstream_tbl maps each direction to its opposite:
                    { 2, 3, 0, 1 }  means  South->North, West->East, North->South, East->West
                This prevents the river from flowing backwards.

            c)  If direction == downstream, increment accepted_steps.
            d)  If attempts > 30, give up -- return ST_FALSE.

    3)  If we accumulate 3 accepted downstream steps, return ST_TRUE.

    Parameters:
        seed    The RNG seed to test.

    Returns:
        ST_TRUE (1) if the seed produces >= 3 downstream steps within 30 attempts.
        ST_FALSE (0) if the 30-attempt limit is exceeded.
------------------------------------------------------------------------------------------------------------------------------------
*/
static int16_t Count_Accepts_For_Seed(uint32_t seed)
{
    const int16_t upstream_tbl[4] = { 2, 3, 0, 1 };

    uint32_t seed_state = seed;
    int16_t downstream = 0;
    int16_t same_dir = ST_UNDEFINED;
    int16_t accepted_steps = 0;
    int16_t attempts = 0;
    int16_t direction = 0;

    (void)Predict_Random(&seed_state, (WORLD_WIDTH - 8));
    (void)Predict_Random(&seed_state, (WORLD_HEIGHT - 8));
    downstream = (Predict_Random(&seed_state, 4) - 1);

    while(accepted_steps < 3)
    {
        if((Predict_Random(&seed_state, 2) > 1) || (same_dir == ST_UNDEFINED))
        {
            direction = downstream;
        }
        else
        {
            do
            {
                direction = (Predict_Random(&seed_state, 4) - 1);
            } while(upstream_tbl[downstream] == direction);
        }

        same_dir = direction;
        attempts++;

        if(attempts > 30)
        {
            return ST_FALSE;
        }

        if(direction == downstream)
        {
            accepted_steps++;
        }
    }

    return ST_TRUE;
}



/*
------------------------------------------------------------------------------------------------------------------------------------
    Find_Usable_Seed() -- Brute-force search for an RNG seed that produces a straight, in-bounds, 4-square river path
------------------------------------------------------------------------------------------------------------------------------------
    The ManufacturedCandidate test needs a seed where:
        (a) The RNG direction loop yields at least 3 downstream steps  (checked by Count_Accepts_For_Seed).
        (b) The resulting 4-square straight-line path (base + 3 downstream steps) stays within the world bounds.
        (c) The ocean outflow square (one step perpendicular past the endpoint) also stays in bounds.

    This function iterates seeds 1 through 99,999 and returns the first one that satisfies all three conditions.  In practice,
    a valid seed is found very quickly (typically within the first few hundred iterations).

    How it works, step by step:

    1)  For each candidate seed, replay the first three Random() calls to derive base_wx, base_wy, and downstream.
        These formulas match River_Path() exactly:
            base_wx    = 4 + Predict_Random(&seed_state, WORLD_WIDTH  - 8)     range [5..56]
            base_wy    = 4 + Predict_Random(&seed_state, WORLD_HEIGHT - 8)     range [5..35]
            downstream = Predict_Random(&seed_state, 4) - 1                    range [0..3]

    2)  Compute the endpoint assuming 3 straight downstream steps:
            end_wx = base_wx + 3 * dir_wx[downstream]
            end_wy = base_wy + 3 * dir_wy[downstream]
        where:
            dir_wx[4] = { 0, -1,  0, 1 }       dir_wy[4] = { 1,  0, -1, 0 }

    3)  Compute the ocean outflow square (one step perpendicular to downstream, past the endpoint):
            ocean_wx = end_wx + outflow_wx[downstream]
            ocean_wy = end_wy + outflow_wy[downstream]
        where:
            outflow_wx[4] = { 1,  0, -1, 0 }   outflow_wy[4] = { 0,  1,  0, -1 }
        This is where the test will place an Ocean square so that River_Path() detects outflow and accepts the path.

    4)  Bounds check:  if the endpoint or ocean square is at or beyond any world edge, skip this seed.
        The strict inequality (<=XMIN, >=XMAX) keeps a 1-square margin from the edge for safety.

    5)  Run Count_Accepts_For_Seed() to verify the RNG direction loop will actually produce 3 downstream steps.

    6)  If all checks pass, populate the plan structure and return ST_TRUE.
        If no seed in 1..99999 works, return ST_FALSE.  (This should never happen in practice.)

    Parameters:
        plan    Output structure.  On success, filled with the seed, base coordinates, and downstream direction.

    Returns:
        ST_TRUE (1) if a usable seed was found.
        ST_FALSE (0) if the search was exhausted (should not happen).
------------------------------------------------------------------------------------------------------------------------------------
*/
static int16_t Find_Usable_Seed(s_river_seed_plan * plan)
{
    const int16_t dir_wx[4] = { 0, -1, 0, 1 };
    const int16_t dir_wy[4] = { 1, 0, -1, 0 };
    const int16_t outflow_wx[4] = { 1, 0, -1, 0 };
    const int16_t outflow_wy[4] = { 0, 1, 0, -1 };

    for(uint32_t seed = 1; seed < 100000; seed++)
    {
        uint32_t seed_state = seed;
        int16_t base_wx = (4 + Predict_Random(&seed_state, (WORLD_WIDTH - 8)));
        int16_t base_wy = (4 + Predict_Random(&seed_state, (WORLD_HEIGHT - 8)));
        int16_t downstream = (Predict_Random(&seed_state, 4) - 1);
        int16_t end_wx = (base_wx + (3 * dir_wx[downstream]));
        int16_t end_wy = (base_wy + (3 * dir_wy[downstream]));
        int16_t ocean_wx = (end_wx + outflow_wx[downstream]);
        int16_t ocean_wy = (end_wy + outflow_wy[downstream]);

        if(
            (end_wx <= WORLD_XMIN) || (end_wx >= WORLD_XMAX)
            ||
            (end_wy <= WORLD_YMIN) || (end_wy >= WORLD_YMAX)
            ||
            (ocean_wx <= WORLD_XMIN) || (ocean_wx >= WORLD_XMAX)
            ||
            (ocean_wy <= WORLD_YMIN) || (ocean_wy >= WORLD_YMAX)
        )
        {
            continue;
        }

        if(Count_Accepts_For_Seed(seed) == ST_TRUE)
        {
            plan->seed = seed;
            plan->base_wx = base_wx;
            plan->base_wy = base_wy;
            plan->downstream = downstream;
            return ST_TRUE;
        }
    }

    return ST_FALSE;
}



/*
------------------------------------------------------------------------------------------------------------------------------------
    River_Path_test -- Google Test fixture for River_Path()
------------------------------------------------------------------------------------------------------------------------------------
    This fixture allocates and initializes the three global data structures that River_Path() reads and writes:

    1)  _world_maps / p_world_map
            The terrain type map.  Allocated as a flat buffer of (WORLD_SIZE * NUM_PLANES * sizeof(int16_t)) = 9,600 bytes.
            Cast to p_world_map, a pointer to int16_t[WORLD_HEIGHT][WORLD_WIDTH] which gives 3D access as
            p_world_map[plane][wy][wx].

            Filled with tt_Grasslands1 (0xA2 = 162) for both planes.  Grasslands is a terrain type that does NOT block river
            placement -- it is not Ocean, Mountain, Hills, Desert, Node, or River.  This is the "neutral" terrain for testing.

    2)  _map_square_terrain_specials
            A flat byte array of (WORLD_SIZE * NUM_PLANES) = 4,800 bytes.  Indexed as [(wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx].
            Each byte is the terrain special value for that square.  River_Path() rejects any square where this value is nonzero.

            SetUp() fills this with 1 (memset to 1), meaning every square has a terrain special.  This is the DEFAULT state --
            which guarantees that River_Path() will always fail in the default fixture state.  Tests that need a successful path
            must explicitly clear the specials to 0 on the squares they want the river to use.

            WHY default to 1:  This is a defensive testing strategy.  By defaulting to "blocked everywhere", we ensure that a
            test which forgets to set up terrain specials will get a clean failure (ST_FALSE) rather than an unpredictable
            success that depends on RNG.

    3)  _NODES
            Array of s_NODE structures, NUM_NODES (30) entries.  These represent the magic power nodes placed on the world map.
            River_Path() calls Square_Is_Node_NewGame() which checks whether any node occupies a given square by scanning this
            array.  All nodes are initialized with wx/wy/wp = ST_UNDEFINED (-1), meaning no node occupies any real square.
            This ensures that the "is it a node?" check never blocks river placement unless a test explicitly places a node.

    TearDown() frees all three allocations and nullifies the pointers to catch use-after-free bugs.
------------------------------------------------------------------------------------------------------------------------------------
*/
class River_Path_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        _world_maps = static_cast<uint8_t *>(malloc(WORLD_SIZE * NUM_PLANES * sizeof(int16_t)));
        ASSERT_NE(_world_maps, nullptr);

        p_world_map = (int16_t (*)[WORLD_HEIGHT][WORLD_WIDTH])_world_maps;

        _map_square_terrain_specials = static_cast<uint8_t *>(malloc(WORLD_SIZE * NUM_PLANES));
        ASSERT_NE(_map_square_terrain_specials, nullptr);

        _NODES = static_cast<struct s_NODE *>(malloc(sizeof(struct s_NODE) * NUM_NODES));
        ASSERT_NE(_NODES, nullptr);

        for(int16_t itr = 0; itr < NUM_NODES; itr++)
        {
            _NODES[itr].wx = ST_UNDEFINED;
            _NODES[itr].wy = ST_UNDEFINED;
            _NODES[itr].wp = ST_UNDEFINED;
        }

        for(int16_t wp = 0; wp < NUM_PLANES; wp++)
        {
            for(int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
            {
                for(int16_t wx = 0; wx < WORLD_WIDTH; wx++)
                {
                    p_world_map[wp][wy][wx] = tt_Grasslands1;
                }
            }
        }

        memset(_map_square_terrain_specials, 1, WORLD_SIZE * NUM_PLANES);
    }

    void TearDown() override
    {
        free(_NODES);
        _NODES = nullptr;

        free(_map_square_terrain_specials);
        _map_square_terrain_specials = nullptr;

        free(_world_maps);
        _world_maps = nullptr;
        p_world_map = nullptr;
    }
};



/*
------------------------------------------------------------------------------------------------------------------------------------
    TEST: TerrainSpecialAtStart_RejectsPathAndLeavesMapUnchanged
------------------------------------------------------------------------------------------------------------------------------------
    Purpose:
        Verify that River_Path() returns ST_FALSE and does not modify any map square when the starting square has a nonzero
        terrain special.

    Background:
        The very first validation in River_Path() (MAPGEN.c:4352) is:
            if(_map_square_terrain_specials[...] != 0) { return ST_FALSE; }
        This check fires BEFORE any ocean, mountain, or other terrain checks.  If the randomly chosen starting square has a
        terrain special, the function bails out immediately without writing anything to the map.

    Fixture state going into this test:
        - All squares are tt_Grasslands1 (valid terrain for a river).
        - All terrain specials are 1 (nonzero -- blocks river placement on every square).
        - All nodes have coordinates ST_UNDEFINED (-1), so no node blocks placement.
        Because terrain specials are 1 everywhere, no matter what base_wx/base_wy the RNG picks, the special check will fire.

    What the test does:
        1) Call River_Path(ARCANUS_PLANE).
        2) Assert the return value is ST_FALSE (0).
        3) Scan every square on ARCANUS_PLANE and assert that none contain the placeholder value 1000.
           This confirms the function truly left the map completely untouched.

    What this proves:
        - The terrain special check is the first guard and it works.
        - A failed River_Path() call has zero side effects on the map.
------------------------------------------------------------------------------------------------------------------------------------
*/
TEST_F(River_Path_test, TerrainSpecialAtStart_RejectsPathAndLeavesMapUnchanged)
{
    const int16_t result = River_Path(ARCANUS_PLANE);

    EXPECT_EQ(result, ST_FALSE);

    for(int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            EXPECT_NE(p_world_map[ARCANUS_PLANE][wy][wx], 1000);
        }
    }
}



/*
------------------------------------------------------------------------------------------------------------------------------------
    TEST: ManufacturedCandidate_SucceedsAndWritesExpectedPath
------------------------------------------------------------------------------------------------------------------------------------
    Purpose:
        Verify that River_Path() returns ST_TRUE and writes exactly 4 placeholder squares at the predicted coordinates when the
        map and RNG are set up for a valid straight-line river path.

    Background:
        This is the "happy path" test.  It constructs a world state where River_Path() is guaranteed to succeed by:
            (a) Seeding the RNG to a known value that produces a 3-step downstream path.
            (b) Setting up the terrain map so the path squares are Grasslands (passable) and everything else is Desert (blocked).
            (c) Placing an Ocean square at the outflow point so the river finds its way to water.

    Detailed walk-through:

    PHASE 1 -- Find a usable RNG seed.
        s_river_seed_plan plan = { 0, 0, 0, 0 };
        ASSERT_EQ(Find_Usable_Seed(&plan), ST_TRUE);

        Find_Usable_Seed() brute-forces seeds 1..99999 until it finds one where:
            - The first 3 Random() calls produce base_wx, base_wy, downstream that keep the 4-square path in bounds.
            - Count_Accepts_For_Seed() confirms the direction loop will yield 3 downstream steps within 30 attempts.
        The ASSERT_EQ will abort the test if no seed is found (should never happen).

    PHASE 2 -- Prepare the world map to force a known outcome.
        The fixture default is tt_Grasslands1 with terrain specials = 1 everywhere.  This test overwrites both planes:
            - Set every square to tt_Desert1.  Desert is rejected by River_Path() step 4c, so the river can ONLY walk on
              squares we explicitly set to Grasslands.
            - Clear all terrain specials to 0.  This removes the blanket "terrain special" block from the fixture.

        Then, carefully paint exactly the squares the river needs:
            p_world_map[wp][plan.base_wy][plan.base_wx] = tt_Grasslands1;   The starting square.
            p_world_map[wp][step1_wy][step1_wx]         = tt_Grasslands1;   1st downstream step.
            p_world_map[wp][step2_wy][step2_wx]         = tt_Grasslands1;   2nd downstream step.
            p_world_map[wp][step3_wy][step3_wx]         = tt_Grasslands1;   3rd downstream step.
            p_world_map[wp][ocean_wy][ocean_wx]         = tt_Ocean1;        The outflow ocean square.

        The step coordinates are computed from the plan:
            step1 = base + 1 * dir[downstream]
            step2 = base + 2 * dir[downstream]
            step3 = base + 3 * dir[downstream]
            ocean = step3 + outflow[downstream]     (one square perpendicular to the downstream direction)

        Why Desert for the background?  Because River_Path() has this check (MAPGEN.c:4416):
            if(Square_Is_Desert_NewGame(next_wx, next_wy, wp) == ST_TRUE) { continue; }
        Desert squares cause the direction to be retried (the step is skipped).  By making the entire background Desert and only
        the intended path Grasslands, we ensure the river CANNOT wander off the planned path.  Any non-downstream direction will
        hit Desert and be retried, forcing the river to pick the downstream direction again.

    PHASE 3 -- Seed the RNG and call River_Path.
        Set_Random_Seed(plan.seed);
        const int16_t result = River_Path(wp);

        With the seed locked and the terrain funneled, River_Path() will:
            1) Random() for base_wx  -> produces plan.base_wx  (we set this square to Grasslands)
            2) Random() for base_wy  -> produces plan.base_wy
            3) Random() for downstream -> produces plan.downstream
            4) Walk downstream 3 steps, each landing on Grasslands
            5) Detect ocean at the outflow point
            6) Write 1000 to all 4 squares and return ST_TRUE

    PHASE 4 -- Verify the results.
        EXPECT_EQ(result, ST_TRUE);
            The function reports success.

        Count all squares with value 1000:
            EXPECT_EQ(river_placeholder_count, 4);
            Exactly 4 squares should be marked -- no more, no less.

        Verify each specific square:
            EXPECT_EQ(p_world_map[wp][plan.base_wy][plan.base_wx], 1000);   Starting square.
            EXPECT_EQ(p_world_map[wp][step1_wy][step1_wx], 1000);           1st step.
            EXPECT_EQ(p_world_map[wp][step2_wy][step2_wx], 1000);           2nd step.
            EXPECT_EQ(p_world_map[wp][step3_wy][step3_wx], 1000);           3rd step.

    What this proves:
        - River_Path() correctly consumes the RNG to derive starting point and direction.
        - The direction-selection loop favors the downstream direction.
        - Desert terrain correctly blocks steps, forcing the river onto the prepared path.
        - Ocean detection at the outflow point terminates the path.
        - The placeholder value 1000 (TT_RIVER_PLACEHOLDER) is written to exactly the expected squares.
        - The minimum river length of 4 squares (base + 3 steps) is met and accepted.
        - The function returns ST_TRUE on a valid placement.
------------------------------------------------------------------------------------------------------------------------------------
*/
TEST_F(River_Path_test, ManufacturedCandidate_SucceedsAndWritesExpectedPath)
{
    const int16_t dir_wx[4] = { 0, -1, 0, 1 };
    const int16_t dir_wy[4] = { 1, 0, -1, 0 };
    const int16_t outflow_wx[4] = { 1, 0, -1, 0 };
    const int16_t outflow_wy[4] = { 0, 1, 0, -1 };
    const int16_t wp = ARCANUS_PLANE;

    s_river_seed_plan plan = { 0, 0, 0, 0 };
    ASSERT_EQ(Find_Usable_Seed(&plan), ST_TRUE);

    for(int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            p_world_map[wp][wy][wx] = tt_Desert1;
            _map_square_terrain_specials[(wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx] = 0;
        }
    }

    const int16_t step1_wx = (plan.base_wx + dir_wx[plan.downstream]);
    const int16_t step1_wy = (plan.base_wy + dir_wy[plan.downstream]);
    const int16_t step2_wx = (plan.base_wx + (2 * dir_wx[plan.downstream]));
    const int16_t step2_wy = (plan.base_wy + (2 * dir_wy[plan.downstream]));
    const int16_t step3_wx = (plan.base_wx + (3 * dir_wx[plan.downstream]));
    const int16_t step3_wy = (plan.base_wy + (3 * dir_wy[plan.downstream]));
    const int16_t ocean_wx = (step3_wx + outflow_wx[plan.downstream]);
    const int16_t ocean_wy = (step3_wy + outflow_wy[plan.downstream]);

    p_world_map[wp][plan.base_wy][plan.base_wx] = tt_Grasslands1;
    p_world_map[wp][step1_wy][step1_wx] = tt_Grasslands1;
    p_world_map[wp][step2_wy][step2_wx] = tt_Grasslands1;
    p_world_map[wp][step3_wy][step3_wx] = tt_Grasslands1;
    p_world_map[wp][ocean_wy][ocean_wx] = tt_Ocean1;

    Set_Random_Seed(plan.seed);

    const int16_t result = River_Path(wp);
    EXPECT_EQ(result, ST_TRUE);

    int16_t river_placeholder_count = 0;
    for(int16_t wy = 0; wy < WORLD_HEIGHT; wy++)
    {
        for(int16_t wx = 0; wx < WORLD_WIDTH; wx++)
        {
            if(p_world_map[wp][wy][wx] == 1000)
            {
                river_placeholder_count++;
            }
        }
    }

    EXPECT_EQ(river_placeholder_count, 4);
    EXPECT_EQ(p_world_map[wp][plan.base_wy][plan.base_wx], 1000);
    EXPECT_EQ(p_world_map[wp][step1_wy][step1_wx], 1000);
    EXPECT_EQ(p_world_map[wp][step2_wy][step2_wx], 1000);
    EXPECT_EQ(p_world_map[wp][step3_wy][step3_wx], 1000);
}
