#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../MoX/src/MOM_DAT.h"
#include "../../MoX/src/MOM_DEF.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_TYPE.h"
#include "../src/MAPGEN.h"
#ifdef __cplusplus
}
#endif

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>

/*
    Find_Shortest_Path(SrcX, SrcY, TgtX, TgtY, Wdt, Hgt, movepath_cost_map, mvpth_x, mvpth_y)

    The generic (NIU) Bellman-Ford solver shared in shape with the overland and combat movers.
    Pure function: its three working arrays (shortest_path_*) are static file-scope, so no
    global setup is needed.

    - movepath_cost_map: uint8_t cost grid (Wdt*Hgt). INF (0xFF) = impassable; a passable tile's
      value is the cost to enter it. With every passable tile = 1, accumulated cost equals the
      hop count, so the returned length is the 8-connectivity (Chebyshev) shortest distance.
    - Returns the hop count (0 if no path, or if the source/target tile is impassable).
    - mvpth_x/mvpth_y hold the path in forward order; entry [len-1] is the target.
*/

namespace
{
    constexpr int16_t WDT = 5;
    constexpr int16_t HGT = 7;
    constexpr uint8_t PASS = 1;     /* passable, entry cost 1 */
    constexpr uint8_t WALL = INF;   /* 0xFF, impassable */

    inline int Idx(int x, int y) { return (y * WDT) + x; }

    /* Validate that the returned path is a contiguous, in-bounds, passable 8-neighbour chain
       from a neighbour of the source to the target -- a strong invariant for any input, so the
       cases that don't have a single unique route still get more than a length check. */
    void CheckPath(int srcx, int srcy, int tgtx, int tgty, int len, const uint8_t * xs, const uint8_t * ys, const uint8_t * map, int wdt, int hgt)
    {
        ASSERT_GT(len, 0);
        EXPECT_EQ(static_cast<int>(xs[len - 1]), tgtx) << "path does not end at target X";
        EXPECT_EQ(static_cast<int>(ys[len - 1]), tgty) << "path does not end at target Y";

        int first_dx = std::abs(static_cast<int>(xs[0]) - srcx);
        int first_dy = std::abs(static_cast<int>(ys[0]) - srcy);
        EXPECT_LE(std::max(first_dx, first_dy), 1) << "first hop is not adjacent to the source";
        EXPECT_GT(first_dx + first_dy, 0) << "first hop is the source tile itself";

        for(int i = 0; i < len; i++)
        {
            ASSERT_LT(static_cast<int>(xs[i]), wdt);
            ASSERT_LT(static_cast<int>(ys[i]), hgt);
            EXPECT_NE(static_cast<int>(map[(ys[i] * wdt) + xs[i]]), static_cast<int>(INF)) << "path tile " << i << " is impassable";
            if(i > 0)
            {
                int dx = std::abs(static_cast<int>(xs[i]) - static_cast<int>(xs[i - 1]));
                int dy = std::abs(static_cast<int>(ys[i]) - static_cast<int>(ys[i - 1]));
                EXPECT_LE(std::max(dx, dy), 1) << "step " << i << " is not an 8-neighbour move";
                EXPECT_GT(dx + dy, 0) << "step " << i << " repeats the previous tile";
            }
        }
    }
}

TEST(Find_Shortest_Path_test, ClearGrid_ReturnsChebyshevLength)
{
    uint8_t map[WDT * HGT];
    std::memset(map, PASS, sizeof(map));
    uint8_t xs[64] = { 0 };
    uint8_t ys[64] = { 0 };

    /* src (0,0) -> tgt (4,4): 8-connectivity Chebyshev distance = max(4,4) = 4. */
    int16_t len = Find_Shortest_Path(0, 0, 4, 4, WDT, HGT, map, xs, ys);

    EXPECT_EQ(len, 4);
    CheckPath(0, 0, 4, 4, len, xs, ys, map, WDT, HGT);
}

TEST(Find_Shortest_Path_test, OrthogonalAdjacent_ReturnsLengthOne)
{
    uint8_t map[WDT * HGT];
    std::memset(map, PASS, sizeof(map));
    uint8_t xs[64] = { 0 };
    uint8_t ys[64] = { 0 };

    int16_t len = Find_Shortest_Path(2, 3, 3, 3, WDT, HGT, map, xs, ys);

    EXPECT_EQ(len, 1);
    ASSERT_GT(len, 0);
    EXPECT_EQ(xs[0], 3);
    EXPECT_EQ(ys[0], 3);
}

TEST(Find_Shortest_Path_test, DiagonalAdjacent_ReturnsLengthOne)
{
    /* 8-connectivity: a diagonal neighbour is a single hop. */
    uint8_t map[WDT * HGT];
    std::memset(map, PASS, sizeof(map));
    uint8_t xs[64] = { 0 };
    uint8_t ys[64] = { 0 };

    int16_t len = Find_Shortest_Path(2, 3, 3, 4, WDT, HGT, map, xs, ys);

    EXPECT_EQ(len, 1);
}

TEST(Find_Shortest_Path_test, SingleRowCorridor_ForcesExactUniquePath)
{
    /* Only row 2 is passable; every other tile is a wall, so the (0,2)->(4,2) path is forced
       straight along the corridor -- the unique route, so every hop is asserted exactly. */
    uint8_t map[WDT * HGT];
    std::memset(map, WALL, sizeof(map));
    for(int x = 0; x < WDT; x++) { map[Idx(x, 2)] = PASS; }
    uint8_t xs[64] = { 0 };
    uint8_t ys[64] = { 0 };

    int16_t len = Find_Shortest_Path(0, 2, 4, 2, WDT, HGT, map, xs, ys);

    ASSERT_EQ(len, 4);
    const uint8_t expected_x[4] = { 1, 2, 3, 4 };
    for(int i = 0; i < 4; i++)
    {
        EXPECT_EQ(xs[i], expected_x[i]) << "hop " << i << " X";
        EXPECT_EQ(ys[i], 2)             << "hop " << i << " Y";
    }
    CheckPath(0, 2, 4, 2, len, xs, ys, map, WDT, HGT);
}

TEST(Find_Shortest_Path_test, ImpassableTarget_ReturnsZero)
{
    uint8_t map[WDT * HGT];
    std::memset(map, PASS, sizeof(map));
    map[Idx(4, 4)] = WALL;
    uint8_t xs[64] = { 0 };
    uint8_t ys[64] = { 0 };

    int16_t len = Find_Shortest_Path(0, 0, 4, 4, WDT, HGT, map, xs, ys);

    EXPECT_EQ(len, 0);
}

TEST(Find_Shortest_Path_test, ImpassableSource_ReturnsZero)
{
    uint8_t map[WDT * HGT];
    std::memset(map, PASS, sizeof(map));
    map[Idx(0, 0)] = WALL;
    uint8_t xs[64] = { 0 };
    uint8_t ys[64] = { 0 };

    int16_t len = Find_Shortest_Path(0, 0, 4, 4, WDT, HGT, map, xs, ys);

    EXPECT_EQ(len, 0);
}

TEST(Find_Shortest_Path_test, FullWallBetween_ReturnsZero)
{
    uint8_t map[WDT * HGT];
    std::memset(map, PASS, sizeof(map));
    for(int y = 0; y < HGT; y++) { map[Idx(2, y)] = WALL; }   /* full column-2 wall */
    uint8_t xs[64] = { 0 };
    uint8_t ys[64] = { 0 };

    int16_t len = Find_Shortest_Path(0, 3, 4, 3, WDT, HGT, map, xs, ys);

    EXPECT_EQ(len, 0);
}

TEST(Find_Shortest_Path_test, WallForcesDetour_LongerThanOpenGrid)
{
    uint8_t map[WDT * HGT];
    std::memset(map, PASS, sizeof(map));
    for(int y = 0; y <= 4; y++) { map[Idx(2, y)] = WALL; }   /* wall column 2, gap at y=5,6 */
    uint8_t xs[64] = { 0 };
    uint8_t ys[64] = { 0 };

    /* Open-grid (0,0)->(4,0) would be 4 hops; the wall forces a detour to the y=5/6 gap. */
    int16_t len = Find_Shortest_Path(0, 0, 4, 0, WDT, HGT, map, xs, ys);

    EXPECT_GT(len, 4);   /* longer than the unobstructed Chebyshev distance */
    CheckPath(0, 0, 4, 0, len, xs, ys, map, WDT, HGT);   /* and a genuinely valid path */
}
