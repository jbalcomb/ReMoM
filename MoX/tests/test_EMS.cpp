/*
 * Unit tests for EMS/EMS.c
 * Minimal characterization coverage for the current host-backed EMS layer.
 */

#include <gtest/gtest.h>
#include <cstring>

#include "../src/Allocate.h"
#include "../src/EMS/EMS.h"
#include "../src/MOX_BASE.h"
#include "../src/MOX_TYPE.h"

extern "C" {
SAMB_ptr EMMDATAH_Allocate_First(uint16_t nparas);
SAMB_ptr EMMDATAH_Allocate_Next(uint16_t nparas);
}

class EMSTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        EMS_PFBA = nullptr;
        EMM_Startup();
    }
};

TEST_F(EMSTest, GetHandleReusesNameAndRoundTripsBytes)
{
    uint8_t source[64];
    uint8_t target[64];
    EMM_HANDLE handle_a;
    EMM_HANDLE handle_b;

    for (size_t itr = 0; itr < sizeof(source); ++itr) {
        source[itr] = static_cast<uint8_t>((itr * 3u) & 0xFFu);
    }
    std::memset(target, 0, sizeof(target));

    handle_a = EMM_Get_Handle(2, "ROUNDTRP", ST_FALSE);
    ASSERT_NE(handle_a, EMM_HANDLE_INVALID);
    EXPECT_EQ(EMM_GetPageCount(handle_a), 2);

    handle_b = EMM_Get_Handle(1, "ROUNDTRP", ST_FALSE);
    EXPECT_EQ(handle_b, handle_a);

    EMM_MapnWrite(0, source, 0, static_cast<uint16_t>(sizeof(source)), handle_a);
    EMM_MapnRead(0, target, 0, static_cast<uint16_t>(sizeof(target)), handle_a);

    EXPECT_EQ(std::memcmp(source, target, sizeof(source)), 0);
}

TEST_F(EMSTest, DISABLED_GameLikeEmmDataScratchpadPath)
{
    SAMB_ptr first;
    SAMB_ptr next;
    SAMB_ptr replay;

    EMMDATAH_Map();
    ASSERT_NE(EMS_PFBA, nullptr);

    first = EMMDATAH_Allocate_First(8);
    ASSERT_NE(first, nullptr);
    std::memset(first, 0x11, 8 * SZ_PARAGRAPH_B);

    EMMDATAH_Save_Last_Offset();

    next = EMMDATAH_Allocate_Next(4);
    ASSERT_NE(next, nullptr);
    EXPECT_NE(next, first);
    std::memset(next, 0x22, 4 * SZ_PARAGRAPH_B);

    EMMDATAH_Restore_Last_Offset();

    replay = EMMDATAH_Allocate_Next(4);
    ASSERT_NE(replay, nullptr);
    EXPECT_EQ(replay, next);

    EXPECT_EQ(SA_GET_MEMSIG1(first - SZ_PARAGRAPH_B), _SA_MEMSIG1);
    EXPECT_EQ(SA_GET_MEMSIG2(first - SZ_PARAGRAPH_B), _SA_MEMSIG2);
}
