#include <gtest/gtest.h>

#include "../src/MOX_BASE.h"
#include "../src/Video.h"
#include "../src/capture.h"

#include <vector>

#ifdef __cplusplus
extern "C"
{
#endif

    extern uint8_t *current_palette;
    extern int16_t release_flag;

#ifdef __cplusplus
}
#endif

class Screen_Flic_Capture_test : public ::testing::Test
{
protected:
    int16_t original_release_flag = ST_FALSE;

    void SetUp() override
    {
        original_release_flag = release_flag;
    }

    void TearDown() override
    {
        release_flag = original_release_flag;
    }
};

TEST_F(Screen_Flic_Capture_test, KeepsReleaseFlagSetOnEarlyReturn)
{
    release_flag = ST_TRUE;

    (void)Screen_Flic_Capture();

    EXPECT_EQ(release_flag, ST_TRUE);
}

class Capture_Screen_Block_test : public ::testing::Test
{
protected:
    std::vector<uint8_t> screen;
    uint8_t *original_video_page = nullptr;

    void SetUp() override
    {
        const size_t sz = static_cast<size_t>(SCREEN_WIDTH) * static_cast<size_t>(SCREEN_HEIGHT);
        screen.assign(sz, 0x00u);

        for (int y = 0; y < SCREEN_HEIGHT; ++y)
        {
            for (int x = 0; x < SCREEN_WIDTH; ++x)
            {
                screen[static_cast<size_t>(y) * SCREEN_WIDTH + static_cast<size_t>(x)] =
                    static_cast<uint8_t>((x + (3 * y)) & 0xFF);
            }
        }

        original_video_page = current_video_page;
        current_video_page = screen.data();
    }

    void TearDown() override
    {
        current_video_page = original_video_page;
    }
};

TEST_F(Capture_Screen_Block_test, CopiesRectangleInRowMajorOrder)
{
    std::vector<uint8_t> dest(9u, 0x00u);

    Capture_Screen_Block(dest.data(), 2, 3, 4, 5);

    size_t out = 0;
    for (int y = 3; y <= 5; ++y)
    {
        for (int x = 2; x <= 4; ++x)
        {
            const uint8_t expected = screen[static_cast<size_t>(y) * SCREEN_WIDTH + static_cast<size_t>(x)];
            EXPECT_EQ(dest[out], expected);
            ++out;
        }
    }
}

TEST_F(Capture_Screen_Block_test, CopiesSinglePixelRectangle)
{
    uint8_t dest = 0x00u;

    Capture_Screen_Block(&dest, 10, 20, 10, 20);

    EXPECT_EQ(dest, screen[static_cast<size_t>(20) * SCREEN_WIDTH + static_cast<size_t>(10)]);
}

TEST_F(Capture_Screen_Block_test, WritesOnlyExpectedByteCount)
{
    std::vector<uint8_t> dest(12u, 0xAAu);

    // Width=2, Height=3 => 6 bytes written starting at dest[3]
    Capture_Screen_Block(dest.data() + 3, 7, 8, 8, 10);

    EXPECT_EQ(dest[0], 0xAAu);
    EXPECT_EQ(dest[1], 0xAAu);
    EXPECT_EQ(dest[2], 0xAAu);
    EXPECT_EQ(dest[9], 0xAAu);
    EXPECT_EQ(dest[10], 0xAAu);
    EXPECT_EQ(dest[11], 0xAAu);
}

class Screen_Picture_Capture_test : public ::testing::Test
{
protected:
    std::vector<uint8_t> screen;
    uint8_t *original_video_page = nullptr;

    void SetUp() override
    {
        const size_t sz = static_cast<size_t>(SCREEN_WIDTH) * static_cast<size_t>(SCREEN_HEIGHT);
        screen.assign(sz, 0x00u);

        for (int y = 0; y < SCREEN_HEIGHT; ++y)
        {
            for (int x = 0; x < SCREEN_WIDTH; ++x)
            {
                screen[static_cast<size_t>(y) * SCREEN_WIDTH + static_cast<size_t>(x)] =
                    static_cast<uint8_t>(((x * 7) + y) & 0xFF);
            }
        }

        original_video_page = current_video_page;
        current_video_page = screen.data();
    }

    void TearDown() override
    {
        current_video_page = original_video_page;
    }
};

TEST_F(Screen_Picture_Capture_test, DISABLED_CopiesRectanglePixelsIntoPayloadAfterHeader)
{
    const int x1 = 12;
    const int y1 = 8;
    const int x2 = 14;
    const int y2 = 9;

    std::vector<uint8_t> pict(64u, 0xEEu);

    Screen_Picture_Capture(x1, y1, x2, y2, pict.data());

    size_t out = 16u; // SZ_FLIC_HDR
    for (int y = y1; y <= y2; ++y)
    {
        for (int x = x1; x <= x2; ++x)
        {
            const uint8_t expected = screen[static_cast<size_t>(y) * SCREEN_WIDTH + static_cast<size_t>(x)];
            EXPECT_EQ(pict[out], expected);
            ++out;
        }
    }
}

TEST_F(Screen_Picture_Capture_test, DISABLED_DoesNotWriteBeforeHeaderOffset)
{
    std::vector<uint8_t> pict(32u, 0xAAu);

    Screen_Picture_Capture(1, 1, 2, 2, pict.data());

    for (size_t i = 0; i < 16u; ++i)
    {
        EXPECT_EQ(pict[i], 0xAAu);
    }
}
