/*
 * Unit tests for Allocate_Pool.c - the static pool behind the SimTex Space Alloc.
 *
 * This target compiles Allocate_Pool.c directly and stubs Allocation_Error(),
 * so it builds and runs in isolation (no MOX library, no MoM concepts), per
 * PLAN-Static-Pool-Allocator Phase 1. Tests observe external behavior only:
 * returned addresses, sentinel bytes, byte counters, and the fatal exit.
 */

#include <gtest/gtest.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../src/Allocate_Pool.h"

/* Exit code the Allocation_Error stub terminates with; asserted by the death test. */
#define STUB_ALLOCATION_ERROR_EXIT_CODE 42

extern "C" void Allocation_Error(uint16_t error_num, uint16_t blocks)
{
    std::fprintf(stderr, "Allocation_Error(%u, %u)\n", (unsigned)error_num, (unsigned)blocks);
    std::exit(STUB_ALLOCATION_ERROR_EXIT_CODE);
}

class AllocatePoolTest : public ::testing::Test {
protected:
    void SetUp() override {
        Pool_Init();
    }
};

// ==============================================================================
// Pool_Init
// ==============================================================================

TEST_F(AllocatePoolTest, Init_Fills_Entire_Pool_With_Sentinel)
{
    // Pool_Carve hands out memory without writing to it, so one carve lets us
    // locate the pool base and scan every byte - guard, carvable region, and
    // trailing margin - for the sentinel.
    uint8_t * first = Pool_Carve(16);
    const uint8_t * pool_base = first - POOL_LEADING_GUARD;

    for (uint32_t ofst = 0; ofst < POOL_SIZE; ofst++)
    {
        ASSERT_EQ(POOL_SENTINEL_BYTE, pool_base[ofst]) << "offset " << ofst;
    }
}

TEST_F(AllocatePoolTest, Init_Resets_After_Writes)
{
    uint8_t * p = Pool_Carve(64);
    std::memset(p, 0xAB, 64);

    Pool_Init();

    uint8_t * q = Pool_Carve(64);
    EXPECT_EQ(p, q);  // cursor reset to just past the leading guard
    for (int ofst = 0; ofst < 64; ofst++)
    {
        ASSERT_EQ(POOL_SENTINEL_BYTE, q[ofst]) << "offset " << ofst;
    }
}

// ==============================================================================
// Pool_Carve
// ==============================================================================

TEST_F(AllocatePoolTest, First_Carve_Starts_Past_Leading_Guard)
{
    uint8_t * first = Pool_Carve(16);

    // The guard region directly precedes the first carve and stays at sentinel.
    EXPECT_EQ(POOL_SENTINEL_BYTE, first[-1]);
    EXPECT_EQ(POOL_SENTINEL_BYTE, first[-(int)POOL_LEADING_GUARD]);
    // Before any carve, exactly the guard is reserved.
    Pool_Init();
    EXPECT_EQ((uint32_t)POOL_LEADING_GUARD, Pool_Bytes_Used());
}

TEST_F(AllocatePoolTest, Consecutive_Carves_Return_Adjacent_Addresses)
{
    uint8_t * p1 = Pool_Carve(100);
    uint8_t * p2 = Pool_Carve(52);
    uint8_t * p3 = Pool_Carve(16);

    EXPECT_EQ(p1 + 100, p2);
    EXPECT_EQ(p2 + 52, p3);
}

TEST_F(AllocatePoolTest, Carve_Advances_Used_By_Exactly_The_Request)
{
    uint32_t used_before = Pool_Bytes_Used();

    Pool_Carve(123);
    EXPECT_EQ(used_before + 123, Pool_Bytes_Used());

    Pool_Carve(1);
    EXPECT_EQ(used_before + 124, Pool_Bytes_Used());
}

TEST_F(AllocatePoolTest, Used_Plus_Free_Equals_Pool_Size)
{
    EXPECT_EQ((uint32_t)POOL_SIZE, Pool_Bytes_Used() + Pool_Bytes_Free());

    Pool_Carve(4096);
    EXPECT_EQ((uint32_t)POOL_SIZE, Pool_Bytes_Used() + Pool_Bytes_Free());

    Pool_Carve(1);
    EXPECT_EQ((uint32_t)POOL_SIZE, Pool_Bytes_Used() + Pool_Bytes_Free());
}

TEST_F(AllocatePoolTest, Reinit_Produces_Identical_Carve_Sequence)
{
    uint8_t * run1[3];
    uint8_t * run2[3];

    run1[0] = Pool_Carve(48);
    run1[1] = Pool_Carve(4000);
    run1[2] = Pool_Carve(16);

    Pool_Init();

    run2[0] = Pool_Carve(48);
    run2[1] = Pool_Carve(4000);
    run2[2] = Pool_Carve(16);

    EXPECT_EQ(run1[0], run2[0]);
    EXPECT_EQ(run1[1], run2[1]);
    EXPECT_EQ(run1[2], run2[2]);
}

TEST_F(AllocatePoolTest, Writes_Stay_Inside_Their_Carve_And_Slack_Stays_Sentinel)
{
    uint8_t * a = Pool_Carve(256);
    uint8_t * b = Pool_Carve(256);

    std::memset(a, 0x11, 256);

    // b was carved but never written: still sentinel throughout.
    for (int ofst = 0; ofst < 256; ofst++)
    {
        ASSERT_EQ(POOL_SENTINEL_BYTE, b[ofst]) << "offset " << ofst;
    }
    // a's bytes hold the written pattern, right up to its final byte.
    EXPECT_EQ(0x11, a[0]);
    EXPECT_EQ(0x11, a[255]);
}

// ==============================================================================
// Exhaustion
// ==============================================================================

TEST_F(AllocatePoolTest, Carve_Of_Full_Capacity_Succeeds)
{
    // Everything between the guard and the trailing margin is carvable in one go.
    uint8_t * p = Pool_Carve(POOL_ARENA_CAPACITY);
    EXPECT_NE(nullptr, p);
    EXPECT_EQ((uint32_t)POOL_FIXED_MARGIN, Pool_Bytes_Free());
}

TEST_F(AllocatePoolTest, Carve_Exceeding_Capacity_Is_Fatal)
{
    EXPECT_EXIT(Pool_Carve(POOL_ARENA_CAPACITY + 1), ::testing::ExitedWithCode(STUB_ALLOCATION_ERROR_EXIT_CODE), "Allocation_Error");
}

TEST_F(AllocatePoolTest, Trailing_Margin_Is_Never_Handed_Out)
{
    Pool_Carve(POOL_ARENA_CAPACITY);
    // The margin is reported free but must not be carvable.
    EXPECT_EQ((uint32_t)POOL_FIXED_MARGIN, Pool_Bytes_Free());
    EXPECT_EXIT(Pool_Carve(1), ::testing::ExitedWithCode(STUB_ALLOCATION_ERROR_EXIT_CODE), "Allocation_Error");
}
