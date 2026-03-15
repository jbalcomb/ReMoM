#include <gtest/gtest.h>

#include "../src/MOX_BASE.h"
#include "../src/Graphics.h"
#include "../src/Video.h"

#include <vector>

// Helper: linear index of pixel (x, y) in a SCREEN_WIDTH-stride buffer
static inline size_t px(int x, int y)
{
    return static_cast<size_t>(y) * static_cast<size_t>(SCREEN_WIDTH)
         + static_cast<size_t>(x);
}


// ============================================================================
// UU_VGA_DrawDblRect
// ============================================================================

/*
    Fixture: allocates a zeroed, full-screen buffer and points current_video_page
    at it for the duration of each test, then restores the original pointer.

    Rectangle under test: (10, 5) to (50, 30) — fits well within 320x200.
    Colours: Color1=0x11  Color2=0x22  Color3=0x33  Color4=0x44

    Expected pixel layout after the call:
        Outer rect (10,5)-(50,30):
            top  edge  y=5,  x=10..50  → Color1
            left edge  x=10, y=5..30   → Color1
            bottom edge y=30, x=11..50 → Color3  (starts 1 px right of corner)
            right edge  x=50, y=6..30  → Color3  (starts 1 px below corner)
        Inner rect (11,6)-(49,29):
            top  edge  y=6,  x=11..49  → Color2
            left edge  x=11, y=6..29   → Color2
            bottom edge y=29, x=12..49 → Color4
            right edge  x=49, y=7..29  → Color4
        Interior pixels (x=12..48, y=7..28): unchanged (0x00)
*/
class UU_VGA_DrawDblRect_test : public ::testing::Test
{
protected:
    std::vector<uint8_t> screen;
    uint8_t * original_video_page = nullptr;

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

TEST_F(UU_VGA_DrawDblRect_test, OuterTopAndLeftEdgesUseColor1)
{
    UU_VGA_DrawDblRect(10, 5, 50, 30, 0x11, 0x22, 0x33, 0x44);

    // Top-left corner of outer rect
    EXPECT_EQ(screen[px(10, 5)], 0x11u);
    // Top-right corner of outer rect (top edge ends at x2=50)
    EXPECT_EQ(screen[px(50, 5)], 0x11u);
    // Mid-point of outer top edge
    EXPECT_EQ(screen[px(30, 5)], 0x11u);
    // Mid-point of outer left edge
    EXPECT_EQ(screen[px(10, 17)], 0x11u);
    // Bottom-left corner (x1,y2) — only left edge reaches here; bottom starts at x1+1
    EXPECT_EQ(screen[px(10, 30)], 0x11u);
}

TEST_F(UU_VGA_DrawDblRect_test, OuterBottomAndRightEdgesUseColor3)
{
    UU_VGA_DrawDblRect(10, 5, 50, 30, 0x11, 0x22, 0x33, 0x44);

    // Mid-point of outer bottom edge (y=30, starts at x=11)
    EXPECT_EQ(screen[px(30, 30)], 0x33u);
    // Mid-point of outer right edge (x=50, starts at y=6)
    EXPECT_EQ(screen[px(50, 17)], 0x33u);
    // Bottom-right corner (x2,y2): both bottom and right edges end here, both Color3
    EXPECT_EQ(screen[px(50, 30)], 0x33u);
}

TEST_F(UU_VGA_DrawDblRect_test, InnerTopAndLeftEdgesUseColor2)
{
    UU_VGA_DrawDblRect(10, 5, 50, 30, 0x11, 0x22, 0x33, 0x44);

    // Inner top-left corner (x1+1, y1+1) = (11, 6)
    EXPECT_EQ(screen[px(11, 6)], 0x22u);
    // Inner top-right corner (x2-1, y1+1) = (49, 6) — top edge ends at x2-1=49
    EXPECT_EQ(screen[px(49, 6)], 0x22u);
    // Mid-point of inner top edge
    EXPECT_EQ(screen[px(30, 6)], 0x22u);
    // Mid-point of inner left edge
    EXPECT_EQ(screen[px(11, 17)], 0x22u);
    // Inner bottom-left (x1+1, y2-1) = (11,29) — only inner left edge; inner bottom starts at x1+2=12
    EXPECT_EQ(screen[px(11, 29)], 0x22u);
}

TEST_F(UU_VGA_DrawDblRect_test, InnerBottomAndRightEdgesUseColor4)
{
    UU_VGA_DrawDblRect(10, 5, 50, 30, 0x11, 0x22, 0x33, 0x44);

    // Mid-point of inner bottom edge (y=29, x starts at 12)
    EXPECT_EQ(screen[px(30, 29)], 0x44u);
    // Mid-point of inner right edge (x=49, y starts at 7)
    EXPECT_EQ(screen[px(49, 17)], 0x44u);
    // Inner bottom-right corner (x2-1, y2-1) = (49,29): both inner bottom and right end here, both Color4
    EXPECT_EQ(screen[px(49, 29)], 0x44u);
}

TEST_F(UU_VGA_DrawDblRect_test, PixelsInsideBothRectsAreUnchanged)
{
    UU_VGA_DrawDblRect(10, 5, 50, 30, 0x11, 0x22, 0x33, 0x44);

    // Well-interior pixels — untouched by any of the eight Line() calls
    EXPECT_EQ(screen[px(25, 15)], 0x00u);
    EXPECT_EQ(screen[px(30, 20)], 0x00u);
    EXPECT_EQ(screen[px(40, 25)], 0x00u);
}


// ============================================================================
// UU_VGA_WndDrawRect
// ============================================================================

/*
    Fixture: allocates a zeroed, full-screen buffer and points current_video_page
    at it for the duration of each test, then restores the original pointer.
    The screen window is reset to full-screen before each test and restored after.

    Rectangle under test with Width=41, Height=26 → corners (10,5) to (50,30):
        Right  = Left + Width  - 1 = 10 + 41 - 1 = 50
        Bottom = Top  + Height - 1 =  5 + 26 - 1 = 30
        top  edge : y=5,  x=10..50  → Color
        left edge : x=10, y=5..30   → Color
        bottom    : y=30, x=11..50  → Color  (starts Left+1)
        right     : x=50, y=6..30   → Color  (starts Top+1)
        interior  : unchanged (0x00)
*/
class UU_VGA_WndDrawRect_test : public ::testing::Test
{
protected:
    std::vector<uint8_t> screen;
    uint8_t * original_video_page = nullptr;
    int16_t saved_x1, saved_y1, saved_x2, saved_y2;

    void SetUp() override
    {
        const size_t sz = static_cast<size_t>(SCREEN_WIDTH)
                        * static_cast<size_t>(SCREEN_HEIGHT);
        screen.assign(sz, 0x00u);
        original_video_page = current_video_page;
        current_video_page = screen.data();

        saved_x1 = screen_window_x1;
        saved_y1 = screen_window_y1;
        saved_x2 = screen_window_x2;
        saved_y2 = screen_window_y2;
        Reset_Window();
    }

    void TearDown() override
    {
        current_video_page = original_video_page;
        screen_window_x1 = saved_x1;
        screen_window_y1 = saved_y1;
        screen_window_x2 = saved_x2;
        screen_window_y2 = saved_y2;
    }
};

TEST_F(UU_VGA_WndDrawRect_test, TopAndLeftEdgesAreDrawn)
{
    UU_VGA_WndDrawRect(10, 5, 41, 26, (char)0xAAu);

    // Top-left corner
    EXPECT_EQ(screen[px(10, 5)], 0xAAu);
    // Top-right corner (Right = 10+41-1 = 50)
    EXPECT_EQ(screen[px(50, 5)], 0xAAu);
    // Mid-point of top edge
    EXPECT_EQ(screen[px(30, 5)], 0xAAu);
    // Mid-point of left edge (Bottom = 5+26-1 = 30)
    EXPECT_EQ(screen[px(10, 17)], 0xAAu);
    // Bottom-left corner — left edge extends down to (10, 30)
    EXPECT_EQ(screen[px(10, 30)], 0xAAu);
}

TEST_F(UU_VGA_WndDrawRect_test, BottomAndRightEdgesAreDrawn)
{
    UU_VGA_WndDrawRect(10, 5, 41, 26, (char)0xAAu);

    // Mid-point of bottom edge (y=30, starts at x=11)
    EXPECT_EQ(screen[px(30, 30)], 0xAAu);
    // Bottom-right corner
    EXPECT_EQ(screen[px(50, 30)], 0xAAu);
    // Mid-point of right edge (x=50, starts at y=6)
    EXPECT_EQ(screen[px(50, 17)], 0xAAu);
}

TEST_F(UU_VGA_WndDrawRect_test, BottomEdgeStartsOnePixelRightOfCorner)
{
    UU_VGA_WndDrawRect(10, 5, 41, 26, (char)0xAAu);

    // Bottom-left corner pixel (10, 30): drawn only by left edge, not bottom edge
    EXPECT_EQ(screen[px(10, 30)], 0xAAu);
    // x=11, y=30: drawn by bottom edge
    EXPECT_EQ(screen[px(11, 30)], 0xAAu);
}

TEST_F(UU_VGA_WndDrawRect_test, RightEdgeStartsOnePixelBelowCorner)
{
    UU_VGA_WndDrawRect(10, 5, 41, 26, (char)0xAAu);

    // Top-right corner (50, 5): drawn only by top edge, not right edge
    EXPECT_EQ(screen[px(50, 5)], 0xAAu);
    // (50, 6): first pixel of right edge
    EXPECT_EQ(screen[px(50, 6)], 0xAAu);
}

TEST_F(UU_VGA_WndDrawRect_test, InteriorPixelsAreUnchanged)
{
    UU_VGA_WndDrawRect(10, 5, 41, 26, (char)0xAAu);

    EXPECT_EQ(screen[px(25, 15)], 0x00u);
    EXPECT_EQ(screen[px(30, 20)], 0x00u);
    EXPECT_EQ(screen[px(40, 25)], 0x00u);
}

TEST_F(UU_VGA_WndDrawRect_test, EdgesClippedToScreenWindow)
{
    // Window clips to x=20..40, y=10..20 — the rect (10,5)-(50,30) extends beyond
    Set_Window(20, 10, 40, 20);

    UU_VGA_WndDrawRect(10, 5, 41, 26, (char)0xBBu);

    // Pixels outside the window must be unchanged
    EXPECT_EQ(screen[px(10, 5)], 0x00u);   // top-left corner — outside window
    EXPECT_EQ(screen[px(50, 30)], 0x00u);  // bottom-right corner — outside window

    // Top edge at y=5 is fully above the window (y_min=10) — not drawn
    EXPECT_EQ(screen[px(30, 5)], 0x00u);

    // Pixel on left edge at (10, 15): x=10 is to the left of window x_min=20 — not drawn
    EXPECT_EQ(screen[px(10, 15)], 0x00u);
}


// ============================================================================
// UU_VGA_DrawRect
// ============================================================================

/*
    Fixture: allocates a zeroed, full-screen buffer and points current_video_page
    at it for the duration of each test, then restores the original pointer.

    Rectangle: Left=10, Top=5, Width=41, Height=26
        Right  = 10 + 41 - 1 = 50
        Bottom =  5 + 26 - 1 = 30
        top  edge : y=5,  x=10..50  → Color
        left edge : x=10, y=5..30   → Color
        bottom    : y=30, x=11..50  → Color  (starts Left+1)
        right     : x=50, y=6..30   → Color  (starts Top+1)
        interior  : unchanged (0x00)

    Unlike UU_VGA_WndDrawRect, this function uses Line() with no clip check,
    so there is no screen-window interaction to test.
*/
class UU_VGA_DrawRect_test : public ::testing::Test
{
protected:
    std::vector<uint8_t> screen;
    uint8_t * original_video_page = nullptr;

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

TEST_F(UU_VGA_DrawRect_test, TopAndLeftEdgesAreDrawn)
{
    UU_VGA_DrawRect(10, 5, 41, 26, 0xCCu);

    // Top-left corner
    EXPECT_EQ(screen[px(10, 5)], 0xCCu);
    // Top-right corner (Right = 10+41-1 = 50)
    EXPECT_EQ(screen[px(50, 5)], 0xCCu);
    // Mid-point of top edge
    EXPECT_EQ(screen[px(30, 5)], 0xCCu);
    // Mid-point of left edge (Bottom = 5+26-1 = 30)
    EXPECT_EQ(screen[px(10, 17)], 0xCCu);
    // Bottom-left corner — left edge extends down to (10, 30)
    EXPECT_EQ(screen[px(10, 30)], 0xCCu);
}

TEST_F(UU_VGA_DrawRect_test, BottomAndRightEdgesAreDrawn)
{
    UU_VGA_DrawRect(10, 5, 41, 26, 0xCCu);

    // Mid-point of bottom edge (y=30, starts at x=11)
    EXPECT_EQ(screen[px(30, 30)], 0xCCu);
    // Bottom-right corner
    EXPECT_EQ(screen[px(50, 30)], 0xCCu);
    // Mid-point of right edge (x=50, starts at y=6)
    EXPECT_EQ(screen[px(50, 17)], 0xCCu);
}

TEST_F(UU_VGA_DrawRect_test, BottomEdgeStartsOnePixelRightOfCorner)
{
    UU_VGA_DrawRect(10, 5, 41, 26, 0xCCu);

    // (10, 30): painted by left edge only — not by bottom (which starts at x=11)
    EXPECT_EQ(screen[px(10, 30)], 0xCCu);
    // (11, 30): first pixel of bottom edge
    EXPECT_EQ(screen[px(11, 30)], 0xCCu);
}

TEST_F(UU_VGA_DrawRect_test, RightEdgeStartsOnePixelBelowCorner)
{
    UU_VGA_DrawRect(10, 5, 41, 26, 0xCCu);

    // (50, 5): painted by top edge only — right edge starts at y=6
    EXPECT_EQ(screen[px(50, 5)], 0xCCu);
    // (50, 6): first pixel of right edge
    EXPECT_EQ(screen[px(50, 6)], 0xCCu);
}

TEST_F(UU_VGA_DrawRect_test, InteriorPixelsAreUnchanged)
{
    UU_VGA_DrawRect(10, 5, 41, 26, 0xCCu);

    EXPECT_EQ(screen[px(25, 15)], 0x00u);
    EXPECT_EQ(screen[px(30, 20)], 0x00u);
    EXPECT_EQ(screen[px(40, 25)], 0x00u);
}

TEST_F(UU_VGA_DrawRect_test, PixelsOutsideRectAreUnchanged)
{
    UU_VGA_DrawRect(10, 5, 41, 26, 0xCCu);

    // One pixel above the top edge
    EXPECT_EQ(screen[px(30, 4)], 0x00u);
    // One pixel left of the left edge
    EXPECT_EQ(screen[px(9, 17)], 0x00u);
    // One pixel below the bottom edge
    EXPECT_EQ(screen[px(30, 31)], 0x00u);
    // One pixel right of the right edge
    EXPECT_EQ(screen[px(51, 17)], 0x00u);
}


// ============================================================================
// UU_Interlaced_Fill
// ============================================================================

/*
    Fixture: allocates a zeroed, full-screen buffer and points current_video_page
    at it for the duration of each test, then restores the original pointer.

    Rectangle under test: (10, 5) to (50, 30) — corner coordinates.
    Color1 = 0xAA  (top edge + left edge)
    Color2 = 0xBB  (bottom edge + right edge)

    Expected pixel layout after the call:
        top  edge  y=5,  x=10..50  → Color1
        left edge  x=10, y=5..30   → Color1
        bottom edge y=30, x=11..50 → Color2  (starts 1 px right of corner)
        right edge  x=50, y=6..30  → Color2  (starts 1 px below corner)
        Interior pixels (x=11..49, y=6..29): unchanged (0x00)
*/
class UU_Interlaced_Fill_test : public ::testing::Test
{
protected:
    std::vector<uint8_t> screen;
    uint8_t * original_video_page = nullptr;

    void SetUp() override
    {
        const size_t sz = static_cast<size_t>(SCREEN_WIDTH)
                        * static_cast<size_t>(SCREEN_HEIGHT);
        screen.assign(sz, 0x00u);
        original_video_page = current_video_page;
        current_video_page  = screen.data();
    }

    void TearDown() override
    {
        current_video_page = original_video_page;
    }
};

// Top edge (y=5, x=10..50) and left edge (x=10, y=5..30) must be Color1
TEST_F(UU_Interlaced_Fill_test, TopAndLeftEdgesAreColor1)
{
    UU_Interlaced_Fill(10, 5, 50, 30, 0xAAu, 0xBBu);

    // Top-left corner
    EXPECT_EQ(screen[px(10, 5)], 0xAAu);
    // Top edge midpoint
    EXPECT_EQ(screen[px(30, 5)], 0xAAu);
    // Top-right corner (drawn by top edge)
    EXPECT_EQ(screen[px(50, 5)], 0xAAu);
    // Left edge midpoint
    EXPECT_EQ(screen[px(10, 17)], 0xAAu);
    // Bottom-left corner (drawn by left edge)
    EXPECT_EQ(screen[px(10, 30)], 0xAAu);
}

// Bottom edge (y=30, x=11..50) and right edge (x=50, y=6..30) must be Color2
TEST_F(UU_Interlaced_Fill_test, BottomAndRightEdgesAreColor2)
{
    UU_Interlaced_Fill(10, 5, 50, 30, 0xAAu, 0xBBu);

    // Bottom edge midpoint
    EXPECT_EQ(screen[px(30, 30)], 0xBBu);
    // Bottom-right corner
    EXPECT_EQ(screen[px(50, 30)], 0xBBu);
    // Right edge midpoint
    EXPECT_EQ(screen[px(50, 17)], 0xBBu);
}

// Bottom edge starts at x1+1, so the bottom-left corner pixel belongs to Color1 (left edge)
TEST_F(UU_Interlaced_Fill_test, BottomEdgeStartsOnePixelRightOfCorner)
{
    UU_Interlaced_Fill(10, 5, 50, 30, 0xAAu, 0xBBu);

    // (10,30): written by left edge as Color1; bottom edge starts at x=11
    EXPECT_EQ(screen[px(10, 30)], 0xAAu);
    // (11,30): first pixel of the bottom edge → Color2
    EXPECT_EQ(screen[px(11, 30)], 0xBBu);
}

// Right edge starts at y1+1, so the top-right corner pixel belongs to Color1 (top edge)
TEST_F(UU_Interlaced_Fill_test, RightEdgeStartsOnePixelBelowCorner)
{
    UU_Interlaced_Fill(10, 5, 50, 30, 0xAAu, 0xBBu);

    // (50,5): written by top edge as Color1; right edge starts at y=6
    EXPECT_EQ(screen[px(50, 5)], 0xAAu);
    // (50,6): first pixel of the right edge → Color2
    EXPECT_EQ(screen[px(50, 6)], 0xBBu);
}

// Interior pixels must not be touched
TEST_F(UU_Interlaced_Fill_test, InteriorPixelsAreUnchanged)
{
    UU_Interlaced_Fill(10, 5, 50, 30, 0xAAu, 0xBBu);

    EXPECT_EQ(screen[px(25, 15)], 0x00u);
    EXPECT_EQ(screen[px(30, 20)], 0x00u);
    EXPECT_EQ(screen[px(40, 25)], 0x00u);
}

// Pixels just outside each edge must not be touched
TEST_F(UU_Interlaced_Fill_test, PixelsOutsideRectAreUnchanged)
{
    UU_Interlaced_Fill(10, 5, 50, 30, 0xAAu, 0xBBu);

    // One pixel above the top edge
    EXPECT_EQ(screen[px(30, 4)], 0x00u);
    // One pixel left of the left edge
    EXPECT_EQ(screen[px(9, 17)], 0x00u);
    // One pixel below the bottom edge
    EXPECT_EQ(screen[px(30, 31)], 0x00u);
    // One pixel right of the right edge
    EXPECT_EQ(screen[px(51, 17)], 0x00u);
}
