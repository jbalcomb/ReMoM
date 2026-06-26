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

#include <cstdint>
#include <cstdlib>
#include <cstring>

/*
    Build_Connectivity_Graph(move_map, result_map)

    Slices one plane's 60x40 movement-cost map into a 12x8 grid of 5x5 sections and writes, per
    section, a connectivity bitflag byte: low nibble = N/E/S/W neighbour reachable (1/2/4/8),
    high nibble (0x10/0x20/0x40/0x80) mirrors those as "edge passable". Impassable tile =
    (char)-1 (0xFF). No Random() -> fully deterministic.

    OOB padding: the OG reconstruction reads one row before/after the map (row -1 at the top
    grid row reaches move_map[-60..-1]; the bottom grid row's builds reach ~move_map[2459]), and
    the preserved east-column OGBUG writes result_map[96]. So move_map gets a leading WORLD_WIDTH
    and trailing WORLD_OVERFLOW of padding, and result_map slack.
*/

namespace
{
    constexpr int LEAD = WORLD_WIDTH;                       /* 60 bytes before move_map[0] */
    constexpr int BUF  = LEAD + WORLD_SIZE + WORLD_OVERFLOW;

    /* Run the worker over a prepared (LEAD-offset) move_map and copy out the 96 result bytes. */
    void Run_Worker(int8_t * move_map, uint8_t result_out[96])
    {
        uint8_t result_map[128];
        memset(result_map, 0xCC, sizeof(result_map));       /* sentinel; OGBUG writes index 96 */
        Build_Connectivity_Graph(move_map, result_map);
        memcpy(result_out, result_map, 96);
    }

    /* Run the worker over a uniform map (every tile == fill). */
    void Run_Uniform(int8_t fill, uint8_t result_out[96])
    {
        int8_t * buffer = static_cast<int8_t *>(malloc(BUF));
        ASSERT_NE(buffer, nullptr);
        memset(buffer, fill, BUF);                          /* padding shares the fill value */
        Run_Worker(buffer + LEAD, result_out);
        free(buffer);
    }
}

TEST(Build_Connectivity_Graph_test, AllPassable_EverySectionConnected)
{
    uint8_t result[96];
    Run_Uniform(0, result);   /* 0 == passable (entry cost 0) everywhere */

    /* Interior sections connect on all four sides -> 0xFF. The top row has no north neighbour
       (bit 1 / mirror 0x10 never set) -> 0xEE; the bottom row has no south (bit 4 / 0x40) ->
       0xBB. Horizontal connectivity wraps, so every section keeps W (8) and E (2). */
    for(int y = 0; y < 8; y++)
    {
        uint8_t expected = (y == 0) ? 0xEE : (y == 7) ? 0xBB : 0xFF;
        for(int x = 0; x < 12; x++)
        {
            EXPECT_EQ(result[(y * 12) + x], expected) << "section x=" << x << " y=" << y;
        }
    }
}

TEST(Build_Connectivity_Graph_test, AllImpassable_NoConnectivity)
{
    uint8_t result[96];
    Run_Uniform(static_cast<int8_t>(0xFF), result);   /* 0xFF == -1 == impassable */

    for(int i = 0; i < 96; i++)
    {
        EXPECT_EQ(result[i], 0x00) << "section index " << i;
    }
}

TEST(Build_Connectivity_Graph_test, HorizontalBarrier_DisconnectsSectionRows)
{
    /* Passable everywhere except a full-width 2-row wall at world rows 19 & 20 -- the boundary
       between section-row 3 (world rows 15-19) and section-row 4 (rows 20-24). The south probe
       of row 3 targets world row 20 and the north probe of row 4 targets world row 19, so the
       barrier severs exactly those two links: section-row 3 loses SOUTH (-> 0xBB, like a bottom
       row) and section-row 4 loses NORTH (-> 0xEE, like a top row). Every other section stays
       fully connected. This is the discriminating case the uniform maps can't exercise. */
    int8_t * buffer = static_cast<int8_t *>(malloc(BUF));
    ASSERT_NE(buffer, nullptr);
    memset(buffer, 0, BUF);                                 /* all passable */
    int8_t * move_map = buffer + LEAD;
    for(int x = 0; x < WORLD_WIDTH; x++)
    {
        move_map[(19 * WORLD_WIDTH) + x] = static_cast<int8_t>(0xFF);
        move_map[(20 * WORLD_WIDTH) + x] = static_cast<int8_t>(0xFF);
    }

    uint8_t result[96];
    Run_Worker(move_map, result);
    free(buffer);

    for(int y = 0; y < 8; y++)
    {
        uint8_t expected = (y == 0 || y == 4) ? 0xEE      /* top row, and row 4 with no north */
                         : (y == 3 || y == 7) ? 0xBB      /* bottom row, and row 3 with no south */
                         : 0xFF;
        for(int x = 0; x < 12; x++)
        {
            EXPECT_EQ(result[(y * 12) + x], expected) << "section x=" << x << " y=" << y;
        }
    }
}
