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
        // Guard mode explicitly off: Pool_Init() would read REMOM_POOL_GUARDS from the environment, and the packed-layout tests below assume no padding.
        Pool_Init_Guard_Mode(0);
    }
};

class AllocatePoolGuardTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        Pool_Init_Guard_Mode(1);
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

// ==============================================================================
// Guard mode
// ==============================================================================

TEST_F(AllocatePoolTest, Guard_Mode_Off_Reports_Nothing_And_Packs_Tightly)
{
    struct s_POOL_GUARD_DAMAGE damage[4];
    uint8_t * a = Pool_Carve(32);
    uint8_t * b = Pool_Carve(32);

    EXPECT_EQ(0, Pool_Guards_Enabled());
    EXPECT_EQ(a + 32, b);  // no padding between arenas
    a[32] = 0x00;          // an overrun off a lands in b, silently
    EXPECT_EQ(0, Pool_Check_Guards(damage, 4));
}

TEST_F(AllocatePoolGuardTest, Arenas_Are_Separated_By_Guard_Bytes)
{
    uint8_t * a = Pool_Carve(32);
    uint8_t * b = Pool_Carve(32);

    EXPECT_NE(0, Pool_Guards_Enabled());
    EXPECT_EQ(a + 32 + POOL_GUARD_BYTES, b);
    for(int ofst = 0; ofst < POOL_GUARD_BYTES; ofst++)
    {
        ASSERT_EQ(POOL_GUARD_BYTE, a[32 + ofst]) << "guard offset " << ofst;
    }
}

TEST_F(AllocatePoolGuardTest, Clean_Pool_Reports_No_Damage)
{
    struct s_POOL_GUARD_DAMAGE damage[4];
    uint8_t * a = Pool_Carve(32);
    std::memset(a, 0x11, 32);  // writing the whole arena, and no further, is fine
    Pool_Carve(48);

    EXPECT_EQ(0, Pool_Check_Guards(damage, 4));
}

TEST_F(AllocatePoolGuardTest, Overrun_Names_The_Arena_That_Wrote_Past_Its_End)
{
    struct s_POOL_GUARD_DAMAGE damage[4];
    uint8_t * a = Pool_Carve(32);
    Pool_Note_Carve_Site("victim_file.c", 20);
    Pool_Carve(32);
    Pool_Note_Carve_Site("overrunner_file.c", 99);
    Pool_Note_Carve_Label("TERRAIN", 1);
    uint8_t * c = Pool_Carve(16);
    Pool_Carve(16);

    (void)a;
    c[16 + 3] = 0x00;  // 4th byte past the end of arena #2
    c[16 + 4] = 0x00;

    ASSERT_EQ(1, Pool_Check_Guards(damage, 4));
    EXPECT_EQ(2, damage[0].arena_idx);
    EXPECT_EQ(c, damage[0].arena_base);
    EXPECT_EQ(16u, damage[0].arena_bytes);
    EXPECT_STREQ("overrunner_file.c", damage[0].site_file);
    EXPECT_EQ(99, damage[0].site_line);
    EXPECT_STREQ("TERRAIN", damage[0].label);
    EXPECT_EQ(1, damage[0].label_num);
    EXPECT_EQ(3u, damage[0].first_bad_offset);
    EXPECT_EQ(2u, damage[0].bad_byte_count);
}

TEST_F(AllocatePoolGuardTest, Carve_Site_And_Label_Apply_To_One_Carve_Only)
{
    struct s_POOL_GUARD_DAMAGE damage[4];
    Pool_Note_Carve_Site("tagged.c", 7);
    Pool_Note_Carve_Label("LABELED", 3);
    Pool_Carve(16);
    uint8_t * untagged = Pool_Carve(16);

    untagged[16] = 0x00;

    ASSERT_EQ(1, Pool_Check_Guards(damage, 4));
    EXPECT_EQ(1, damage[0].arena_idx);
    EXPECT_EQ(nullptr, damage[0].site_file);
    EXPECT_STREQ("", damage[0].label);
    EXPECT_EQ(-1, damage[0].label_num);
}

TEST_F(AllocatePoolGuardTest, Long_Label_Is_Truncated_Not_Overflowed)
{
    struct s_POOL_GUARD_DAMAGE damage[1];
    Pool_Note_Carve_Label("AN_LBX_NAME_LONGER_THAN_THE_LABEL_FIELD", 0);
    uint8_t * a = Pool_Carve(16);
    a[16] = 0x00;

    ASSERT_EQ(1, Pool_Check_Guards(damage, 1));
    EXPECT_EQ((size_t)(POOL_CARVE_LABEL_LEN - 1), std::strlen(damage[0].label));
}

TEST_F(AllocatePoolGuardTest, Underrun_Into_Leading_Guard_Is_Reported)
{
    struct s_POOL_GUARD_DAMAGE damage[4];
    uint8_t * first = Pool_Carve(16);
    first[-1] = 0x00;

    ASSERT_EQ(1, Pool_Check_Guards(damage, 4));
    EXPECT_EQ(POOL_GUARD_REGION_LEADING, damage[0].arena_idx);
    EXPECT_EQ((uint32_t)(POOL_LEADING_GUARD - 1), damage[0].first_bad_offset);
}

TEST_F(AllocatePoolGuardTest, Reset_Guards_Clears_Reported_Damage)
{
    struct s_POOL_GUARD_DAMAGE damage[4];
    uint8_t * a = Pool_Carve(16);
    a[16] = 0x00;
    ASSERT_EQ(1, Pool_Check_Guards(damage, 4));

    Pool_Reset_Guards();

    EXPECT_EQ(0, Pool_Check_Guards(damage, 4));
    EXPECT_EQ(POOL_GUARD_BYTE, a[16]);
}

TEST_F(AllocatePoolGuardTest, Damage_Count_Exceeds_Buffer_When_Many_Guards_Hit)
{
    struct s_POOL_GUARD_DAMAGE damage[2];
    for(int itr = 0; itr < 5; itr++)
    {
        uint8_t * p = Pool_Carve(16);
        p[16] = 0x00;
    }

    EXPECT_EQ(5, Pool_Check_Guards(damage, 2));  // total reported, only 2 filled in
    EXPECT_EQ(0, damage[0].arena_idx);
    EXPECT_EQ(1, damage[1].arena_idx);
}

TEST_F(AllocatePoolGuardTest, Guard_Bytes_Count_Against_Capacity)
{
    // A carve that fits without its guard but not with it must still be fatal.
    EXPECT_EXIT(Pool_Carve(POOL_ARENA_CAPACITY - (POOL_GUARD_BYTES / 2)), ::testing::ExitedWithCode(STUB_ALLOCATION_ERROR_EXIT_CODE), "Allocation_Error");
}
