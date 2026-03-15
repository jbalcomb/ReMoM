#include <gtest/gtest.h>

#include "../src/MOX_BASE.h"
#include "../src/Video.h"
#include "../src/capture.h"

#include <vector>

extern uint8_t * current_palette;
extern int16_t release_flag;

class VGA_ReadScreenLine_test : public ::testing::Test
{
protected:
    std::vector<uint8_t> screen;
    uint8_t *original_video_page = nullptr;

    void SetUp() override
    {
        const size_t sz = static_cast<size_t>(SCREEN_WIDTH)
                        * static_cast<size_t>(SCREEN_HEIGHT);
        screen.assign(sz, 0x00u);

        original_video_page = current_video_page;
        current_video_page = screen.data();
    }

    void TearDown() override
    {
        current_video_page = original_video_page;
    }
};

TEST_F(VGA_ReadScreenLine_test, CopiesRequestedLineIntoBuffer)
{
    const int line = 7;
    char scanline_buffer[SCREEN_WIDTH];

    for(int x = 0; x < SCREEN_WIDTH; ++x)
    {
        screen[static_cast<size_t>(line) * SCREEN_WIDTH + static_cast<size_t>(x)] = static_cast<uint8_t>((x * 3) & 0xFF);
        scanline_buffer[x] = static_cast<char>(0x7F);
    }

    VGA_ReadScreenLine(line, scanline_buffer);

    for(int x = 0; x < SCREEN_WIDTH; ++x)
    {
        EXPECT_EQ(static_cast<uint8_t>(scanline_buffer[x]), screen[static_cast<size_t>(line) * SCREEN_WIDTH + static_cast<size_t>(x)]);
    }
}

TEST_F(VGA_ReadScreenLine_test, NullBufferIsSafeNoOp)
{
    for(int x = 0; x < SCREEN_WIDTH; ++x)
    {
        screen[static_cast<size_t>(3) * SCREEN_WIDTH + static_cast<size_t>(x)] = 0xA5u;
    }

    VGA_ReadScreenLine(3, nullptr);

    for(int x = 0; x < SCREEN_WIDTH; ++x)
    {
        EXPECT_EQ(screen[static_cast<size_t>(3) * SCREEN_WIDTH + static_cast<size_t>(x)], 0xA5u);
    }
}

TEST_F(VGA_ReadScreenLine_test, NegativeLineLeavesBufferUnchanged)
{
    char scanline_buffer[SCREEN_WIDTH];

    for(int x = 0; x < SCREEN_WIDTH; ++x)
    {
        scanline_buffer[x] = static_cast<char>(0x5Au);
    }

    VGA_ReadScreenLine(-1, scanline_buffer);

    for(int x = 0; x < SCREEN_WIDTH; ++x)
    {
        EXPECT_EQ(static_cast<uint8_t>(scanline_buffer[x]), 0x5Au);
    }
}

TEST_F(VGA_ReadScreenLine_test, OutOfRangeLineLeavesBufferUnchanged)
{
    char scanline_buffer[SCREEN_WIDTH];

    for(int x = 0; x < SCREEN_WIDTH; ++x)
    {
        scanline_buffer[x] = static_cast<char>(0x3Cu);
    }

    VGA_ReadScreenLine(SCREEN_HEIGHT, scanline_buffer);

    for(int x = 0; x < SCREEN_WIDTH; ++x)
    {
        EXPECT_EQ(static_cast<uint8_t>(scanline_buffer[x]), 0x3Cu);
    }
}


class VGA_GetDACBlock_test : public ::testing::Test
{
protected:
    std::vector<uint8_t> palette;
    uint8_t * original_palette = nullptr;

    void SetUp() override
    {
        palette.resize(256u * 3u);

        for(size_t i = 0; i < palette.size(); ++i)
        {
            palette[i] = static_cast<uint8_t>((i * 5u + 11u) & 0xFFu);
        }

        original_palette = current_palette;
        current_palette = palette.data();
    }

    void TearDown() override
    {
        current_palette = original_palette;
    }
};

TEST_F(VGA_GetDACBlock_test, CopiesBlockZeroExactly)
{
    uint8_t buffer[64 * 3];

    for(int i = 0; i < (64 * 3); ++i)
    {
        buffer[i] = 0xEEu;
    }

    VGA_GetDACBlock(0, buffer);

    for(int i = 0; i < (64 * 3); ++i)
    {
        EXPECT_EQ(buffer[i], palette[static_cast<size_t>(i)]);
    }
}

TEST_F(VGA_GetDACBlock_test, CopiesBlockTwoFromCorrectOffset)
{
    uint8_t buffer[64 * 3];
    const int expected_offset = (2 * 64 * 3);

    VGA_GetDACBlock(2, buffer);

    for(int i = 0; i < (64 * 3); ++i)
    {
        EXPECT_EQ(buffer[i], palette[static_cast<size_t>(expected_offset + i)]);
    }
}

TEST_F(VGA_GetDACBlock_test, NegativeBlockIsClampedToZero)
{
    uint8_t buffer_neg[64 * 3];
    uint8_t buffer_zero[64 * 3];

    VGA_GetDACBlock(-5, buffer_neg);
    VGA_GetDACBlock(0, buffer_zero);

    for(int i = 0; i < (64 * 3); ++i)
    {
        EXPECT_EQ(buffer_neg[i], buffer_zero[i]);
    }
}

TEST_F(VGA_GetDACBlock_test, LargeBlockIsClampedToThree)
{
    uint8_t buffer_large[64 * 3];
    uint8_t buffer_three[64 * 3];

    VGA_GetDACBlock(99, buffer_large);
    VGA_GetDACBlock(3, buffer_three);

    for(int i = 0; i < (64 * 3); ++i)
    {
        EXPECT_EQ(buffer_large[i], buffer_three[i]);
    }
}

TEST_F(VGA_GetDACBlock_test, NullBufferIsSafeNoOp)
{
    SUCCEED();
    VGA_GetDACBlock(1, nullptr);
}


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

TEST_F(Screen_Flic_Capture_test, ReturnsZeroWhenReleaseFlagIsTrue)
{
    release_flag = ST_TRUE;

    EXPECT_EQ(Screen_Flic_Capture(), 0);
}

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
        const size_t sz = static_cast<size_t>(SCREEN_WIDTH)
                        * static_cast<size_t>(SCREEN_HEIGHT);
        screen.assign(sz, 0x00u);

        for(int y = 0; y < SCREEN_HEIGHT; ++y)
        {
            for(int x = 0; x < SCREEN_WIDTH; ++x)
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
    for(int y = 3; y <= 5; ++y)
    {
        for(int x = 2; x <= 4; ++x)
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
        const size_t sz = static_cast<size_t>(SCREEN_WIDTH)
                        * static_cast<size_t>(SCREEN_HEIGHT);
        screen.assign(sz, 0x00u);

        for(int y = 0; y < SCREEN_HEIGHT; ++y)
        {
            for(int x = 0; x < SCREEN_WIDTH; ++x)
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

TEST_F(Screen_Picture_Capture_test, CopiesRectanglePixelsIntoPayloadAfterHeader)
{
    const int x1 = 12;
    const int y1 = 8;
    const int x2 = 14;
    const int y2 = 9;

    std::vector<uint8_t> pict(64u, 0xEEu);

    Screen_Picture_Capture(x1, y1, x2, y2, pict.data());

    size_t out = 16u; // SZ_FLIC_HDR
    for(int y = y1; y <= y2; ++y)
    {
        for(int x = x1; x <= x2; ++x)
        {
            const uint8_t expected = screen[static_cast<size_t>(y) * SCREEN_WIDTH + static_cast<size_t>(x)];
            EXPECT_EQ(pict[out], expected);
            ++out;
        }
    }
}

TEST_F(Screen_Picture_Capture_test, DoesNotWriteBeforeHeaderOffset)
{
    std::vector<uint8_t> pict(32u, 0xAAu);

    Screen_Picture_Capture(1, 1, 2, 2, pict.data());

    for(size_t i = 0; i < 16u; ++i)
    {
        EXPECT_EQ(pict[i], 0xAAu);
    }
}
