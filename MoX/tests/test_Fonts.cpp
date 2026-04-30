#include <gtest/gtest.h>

#include "../src/MOX_BASE.h"
#include "../src/Fonts.h"
#include "../../platform/include/Platform.h"

// The three globals are module-private in Fonts.c but are linked into MOX.
// Declare them here so we can set up and inspect state directly.
extern "C"
{
    extern int16_t cycle_direction_flag;   // 0 = forward, 1 = reverse, -1 = disabled
    extern int16_t cycle_color_value;
    extern int16_t cycle_step_value;
}

// ============================================================================
// Helper: set all three globals at once before each call  (used by Update_Cycle tests)
// ============================================================================
static void Setup_Cycle(int16_t direction, int16_t value, int16_t step)
{
    cycle_direction_flag = direction;
    cycle_color_value    = value;
    cycle_step_value     = step;
}

// ============================================================================
// Update_Cycle — forward direction (flag == 0), positive gradient (max > min)
// ============================================================================

TEST(Update_Cycle_test, Forward_PositiveGradient_NormalStep)
{
    // value=10, step=3, range=[0,20] → value becomes 13, direction stays 0
    int16_t lo = 0, hi = 20;
    Setup_Cycle(0, 10, 3);
    Update_Cycle(&lo, &hi);
    EXPECT_EQ(cycle_color_value,    13);
    EXPECT_EQ(cycle_direction_flag, 0);
}

TEST(Update_Cycle_test, Forward_PositiveGradient_BounceAtMax)
{
    // value=19, step=5, range=[0,20] → value would be 24; clamped to 19, direction → 1
    int16_t lo = 0, hi = 20;
    Setup_Cycle(0, 19, 5);
    Update_Cycle(&lo, &hi);
    EXPECT_EQ(cycle_color_value,    hi - 1);  // 19
    EXPECT_EQ(cycle_direction_flag, 1);
}

// ============================================================================
// Update_Cycle — forward direction (flag == 0), negative gradient (max <= min)
//
// OGBUG: in the negative-gradient branches Update_Cycle compares the new
// cycle_color_value against the *origin* bound (color_min for forward,
// color_max for reverse) instead of the *destination* bound, so any value
// inside the legal range trips the bounce on the first step.  The disassembly
// confirms this — see the "¿ OGBUG" notes in MoX/src/Fonts.c — so the buggy
// behaviour is preserved.  The "NormalStep" cases below encode the *intended*
// semantics of negative gradient (decrease in forward, increase in reverse,
// no bounce while inside range) which the actual game code never exhibits;
// they are disabled until a fix is sanctioned.
//
// TODO  GUI/integration test: drive Cycle_Palette_Color() through enough
// frames to render the cycle visually in all four direction × gradient
// combinations — forward+positive, reverse+positive, forward+negative,
// reverse+negative — including the two suspected-broken negative cases.
// The visual output is the ground truth: if the original game's palette
// cycles look wrong in negative gradient on real hardware, the OGBUG is real;
// if they look right despite the comparison anomaly, the disassembly may
// have been misread or there's a calling-convention twist we missed.
// ============================================================================

TEST(Update_Cycle_test, DISABLED_Forward_NegativeGradient_NormalStep)
{
    // min=30, max=10 (negative gradient): forward means decreasing
    // value=25, step=3 → value becomes 22, direction stays 0
    //
    // DISABLED — Update_Cycle's forward-negative-gradient branch compares against
    // *color_min instead of *color_max, so any starting value inside the legal
    // range bounces immediately.  See OGBUG note above.  Re-enable once the GUI
    // test confirms the intended semantics.
    int16_t lo = 30, hi = 10;
    Setup_Cycle(0, 25, 3);
    Update_Cycle(&lo, &hi);
    EXPECT_EQ(cycle_color_value,    22);
    EXPECT_EQ(cycle_direction_flag, 0);
}

TEST(Update_Cycle_test, Forward_NegativeGradient_BounceAtMin)
{
    // min=30, max=10: value=31, step=5 → value would be 26; 26 < min(30) → clamp to 31, direction → 1
    int16_t lo = 30, hi = 10;
    Setup_Cycle(0, 31, 5);
    Update_Cycle(&lo, &hi);
    EXPECT_EQ(cycle_color_value,    lo + 1);  // 31
    EXPECT_EQ(cycle_direction_flag, 1);
}

// ============================================================================
// Update_Cycle — reverse direction (flag == 1), positive gradient (max > min)
// ============================================================================

TEST(Update_Cycle_test, Reverse_PositiveGradient_NormalStep)
{
    // value=15, step=4, range=[0,20]: reverse means decreasing → value becomes 11, direction stays 1
    int16_t lo = 0, hi = 20;
    Setup_Cycle(1, 15, 4);
    Update_Cycle(&lo, &hi);
    EXPECT_EQ(cycle_color_value,    11);
    EXPECT_EQ(cycle_direction_flag, 1);
}

TEST(Update_Cycle_test, Reverse_PositiveGradient_BounceAtMin)
{
    // value=2, step=5, range=[0,20] → value would be -3; -3 < 0 → clamp to 1, direction → 0
    int16_t lo = 0, hi = 20;
    Setup_Cycle(1, 2, 5);
    Update_Cycle(&lo, &hi);
    EXPECT_EQ(cycle_color_value,    lo + 1);  // 1
    EXPECT_EQ(cycle_direction_flag, 0);
}

// ============================================================================
// Update_Cycle — reverse direction (flag == 1), negative gradient (max <= min)
// ============================================================================

TEST(Update_Cycle_test, DISABLED_Reverse_NegativeGradient_NormalStep)
{
    // min=30, max=10: reverse of negative gradient means increasing
    // value=15, step=3 → value becomes 18, direction stays 1
    //
    // DISABLED — Update_Cycle's reverse-negative-gradient branch compares against
    // *color_max instead of *color_min, so any starting value inside the legal
    // range bounces immediately.  See OGBUG note above the forward case.
    int16_t lo = 30, hi = 10;
    Setup_Cycle(1, 15, 3);
    Update_Cycle(&lo, &hi);
    EXPECT_EQ(cycle_color_value,    18);
    EXPECT_EQ(cycle_direction_flag, 1);
}

TEST(Update_Cycle_test, Reverse_NegativeGradient_BounceAtMax)
{
    // min=30, max=10: value=8, step=5 → value would be 13; 13 > max(10) → clamp to 9, direction → 0
    int16_t lo = 30, hi = 10;
    Setup_Cycle(1, 8, 5);
    Update_Cycle(&lo, &hi);
    EXPECT_EQ(cycle_color_value,    hi - 1);  // 9
    EXPECT_EQ(cycle_direction_flag, 0);
}

// ============================================================================
// Cycle_Palette_Color tests
//
// The function writes to the platform palette via Platform_Set_Palette_Color(),
// so we read back via Platform_Get_Palette_Color() — no direct SDL access needed.
//
// On first call after Reset_Cycle_Palette_Color() (cycle_direction_flag == -1)
// the function seeds cycle_color_value from the min of the primary channel, so
// store_primary == primary_min and the two secondary channels are also at their
// minima (fraction == 0).  This makes the expected palette RGB straightforward
// to compute: each stored component is its own _min value.
//
// SDL color scaling: (vga6bit * 255) / 63  (integer division)
// ============================================================================

class Cycle_Palette_Color_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Zero out the palette buffer so tests start clean.
        memset(platform_palette_buffer, 0, sizeof(platform_palette_buffer));
        Reset_Cycle_Palette_Color();    // sets cycle_direction_flag = -1
    }

    // Read back a palette entry through the Platform API.
    void Read_Palette(int index, uint8_t &r, uint8_t &g, uint8_t &b) const
    {
        Platform_Get_Palette_Color((uint8_t)index, &r, &g, &b);
    }

    // Scale a 6-bit VGA component (0-63) to 8-bit SDL (0-255), matching the
    // formula used inside Cycle_Palette_Color.
    static int Scale(int vga6) { return (vga6 * 255) / 63; }
};

// ----------------------------------------------------------------------------
// Primary channel selection: RED
// delta_red=30 >= delta_green=20, delta_blue=10  → primary == Red (case 0)
// First call seeds cycle_color_value = red_min; secondaries land on their minima.
// ----------------------------------------------------------------------------
TEST_F(Cycle_Palette_Color_test, FirstCall_RedPrimary_SeedsFromRedMin)
{
    const int16_t rlo=10, rhi=40, glo=5, ghi=25, blo=2, bhi=12;
    const int color_num = 7;

    Cycle_Palette_Color(color_num, rlo, glo, blo, rhi, ghi, bhi, /*step=*/3);

    uint8_t r, g, b;
    Read_Palette(color_num, r, g, b);
    EXPECT_EQ(r, Scale(rlo));   // store_red   = cycle_color_value = red_min
    EXPECT_EQ(g, Scale(glo));   // interpolation fraction == 0  => green_min
    EXPECT_EQ(b, Scale(blo));   // interpolation fraction == 0  => blue_min
}

TEST_F(Cycle_Palette_Color_test, FirstCall_RedPrimary_SetsDirectionForward)
{
    const int16_t rlo=10, rhi=40, glo=5, ghi=25, blo=2, bhi=12;

    Cycle_Palette_Color(0, rlo, glo, blo, rhi, ghi, bhi, /*step=*/3);

    EXPECT_EQ(cycle_direction_flag, 0);      // initialized → forward
    EXPECT_EQ(cycle_color_value, rlo + 3);   // seeded at red_min, then advanced by step
}

// ----------------------------------------------------------------------------
// Primary channel selection: GREEN
// delta_red=6  < delta_green=40 >= delta_blue=12  → primary == Green (case 1)
// ----------------------------------------------------------------------------
TEST_F(Cycle_Palette_Color_test, FirstCall_GreenPrimary_SeedsFromGreenMin)
{
    const int16_t rlo=2, rhi=8, glo=10, ghi=50, blo=3, bhi=15;
    const int color_num = 5;

    Cycle_Palette_Color(color_num, rlo, glo, blo, rhi, ghi, bhi, /*step=*/2);

    uint8_t r, g, b;
    Read_Palette(color_num, r, g, b);
    EXPECT_EQ(r, Scale(rlo));   // fraction == 0 → red_min
    EXPECT_EQ(g, Scale(glo));   // store_green = green_min
    EXPECT_EQ(b, Scale(blo));   // fraction == 0 → blue_min
}

TEST_F(Cycle_Palette_Color_test, FirstCall_GreenPrimary_SetsDirectionForward)
{
    const int16_t rlo=2, rhi=8, glo=10, ghi=50, blo=3, bhi=15;

    Cycle_Palette_Color(0, rlo, glo, blo, rhi, ghi, bhi, /*step=*/2);

    EXPECT_EQ(cycle_direction_flag, 0);
    EXPECT_EQ(cycle_color_value, glo + 2);   // seeded at green_min, advanced by step
}

// ----------------------------------------------------------------------------
// Primary channel selection: BLUE
// delta_red=4  < delta_blue=39, delta_green=9  < delta_blue=39  → primary == Blue (case 2)
// ----------------------------------------------------------------------------
TEST_F(Cycle_Palette_Color_test, FirstCall_BluePrimary_SeedsFromBlueMin)
{
    const int16_t rlo=1, rhi=5, glo=3, ghi=12, blo=8, bhi=47;
    const int color_num = 15;

    Cycle_Palette_Color(color_num, rlo, glo, blo, rhi, ghi, bhi, /*step=*/4);

    uint8_t r, g, b;
    Read_Palette(color_num, r, g, b);
    EXPECT_EQ(r, Scale(rlo));   // fraction == 0 → red_min
    EXPECT_EQ(g, Scale(glo));   // fraction == 0 → green_min
    EXPECT_EQ(b, Scale(blo));   // store_blue = blue_min
}

TEST_F(Cycle_Palette_Color_test, FirstCall_BluePrimary_SetsDirectionForward)
{
    const int16_t rlo=1, rhi=5, glo=3, ghi=12, blo=8, bhi=47;

    Cycle_Palette_Color(0, rlo, glo, blo, rhi, ghi, bhi, /*step=*/4);

    EXPECT_EQ(cycle_direction_flag, 0);
    EXPECT_EQ(cycle_color_value, blo + 4);   // seeded at blue_min, advanced by step
}

// ----------------------------------------------------------------------------
// Tie-breaking: delta_red == delta_green; Red wins (checked first in the code)
// ----------------------------------------------------------------------------
TEST_F(Cycle_Palette_Color_test, Tie_RedGreen_RedWins)
{
    // delta_red = delta_green = 30, delta_blue = 10 → Red should be primary
    const int16_t rlo=0, rhi=30, glo=5, ghi=35, blo=0, bhi=10;
    const int color_num = 1;

    Cycle_Palette_Color(color_num, rlo, glo, blo, rhi, ghi, bhi, /*step=*/1);

    uint8_t r, g, b;
    Read_Palette(color_num, r, g, b);
    // If Red is primary: store_red = red_min = 0 → r == 0
    EXPECT_EQ(r, Scale(rlo));
    EXPECT_EQ(cycle_color_value, rlo + 1);   // advanced by step from red_min
}

// ----------------------------------------------------------------------------
// Second call: verify the palette entry reflects the stepped-forward value
// (cycle_color_value was advanced by step after the first call).
// ----------------------------------------------------------------------------
TEST_F(Cycle_Palette_Color_test, SecondCall_RedPrimary_UsesSteppedValue)
{
    // Red primary, step=5, range [0, 20].
    // Call 1: renders with red_min=0 → r=0; then advances to 5.
    // Call 2: renders with cycle_color_value=5 → r = (5*255)/63 = 20.
    const int16_t rlo=0, rhi=20, glo=0, ghi=0, blo=0, bhi=0;
    const int color_num = 3;

    Cycle_Palette_Color(color_num, rlo, glo, blo, rhi, ghi, bhi, /*step=*/5);
    Cycle_Palette_Color(color_num, rlo, glo, blo, rhi, ghi, bhi, /*step=*/5);

    uint8_t r, g, b;
    Read_Palette(color_num, r, g, b);
    EXPECT_EQ(r, Scale(5));    // second call uses value == 5 (= red_min + step)
    EXPECT_EQ(cycle_color_value, rlo + 5 + 5);  // advanced again after second call
}

// ----------------------------------------------------------------------------
// No palette write when cycle continues into a color that is at a mid-range
// (non-trivial interpolation). Uses red primary with non-zero secondary ranges.
// Second call: cycle_color_value = red_min + step; interpolated secondaries.
//
// green_interp = ((ghi-glo)*(val-rlo))/(rhi-rlo) + glo
// blue_interp  = ((bhi-blo)*(val-rlo))/(rhi-rlo) + blo
// ----------------------------------------------------------------------------
TEST_F(Cycle_Palette_Color_test, SecondCall_RedPrimary_InterpolatesSecondaries)
{
    // red [10,40], green [5,35], blue [2,12], step=10
    // Call 1 renders at red_min=10; secondaries at their min.
    // After call 1: cycle_color_value = 10 + 10 = 20.
    // Call 2 renders at 20:
    //   store_red   = 20
    //   store_green = (30*(20-10))/(40-10) + 5 = (30*10)/30 + 5 = 10+5 = 15
    //   store_blue  = (10*(20-10))/(40-10) + 2 = (10*10)/30 + 2 = 3+2  = 5
    const int16_t rlo=10, rhi=40, glo=5, ghi=35, blo=2, bhi=12;
    const int color_num = 42;

    Cycle_Palette_Color(color_num, rlo, glo, blo, rhi, ghi, bhi, /*step=*/10);
    Cycle_Palette_Color(color_num, rlo, glo, blo, rhi, ghi, bhi, /*step=*/10);

    uint8_t r, g, b;
    Read_Palette(color_num, r, g, b);
    EXPECT_EQ(r, Scale(20));
    EXPECT_EQ(g, Scale(15));
    EXPECT_EQ(b, Scale(5));
}
