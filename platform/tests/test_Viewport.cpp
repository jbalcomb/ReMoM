/*
 * test_Viewport.cpp — Viewport (platform/viewport/PFL_Viewport.c): where the 320x200 image lands in a window, and the
 * window <-> game mouse mapping. External behaviour only: rects, scales, mapped coordinates.
 *
 * See doc/@AI_Plans/PLAN-Fullscreen.md, Phase 1.
 */

#include <gtest/gtest.h>

#include "Platform_Viewport.h"

namespace
{

    const int GAME_WIDTH = 320;
    const int GAME_HEIGHT = 200;

    struct s_VIEWPORT Make_Viewport(int window_width, int window_height, int drawable_width, int drawable_height)
    {
        struct s_VIEWPORT viewport;
        Viewport_Compute(&viewport, window_width, window_height, drawable_width, drawable_height, GAME_WIDTH, GAME_HEIGHT, VIEWPORT_SCALE_MODE_INTEGER, VIEWPORT_PIXEL_ASPECT_SQUARE);
        return viewport;
    }

    struct s_VIEWPORT Make_Viewport(int width, int height)
    {
        return Make_Viewport(width, height, width, height);
    }

    struct Expected_Rect
    {
        int drawable_width;
        int drawable_height;
        int scale;
        int rect_x;
        int rect_y;
    };

/* Integer mode: scale = min(width / 320, height / 200), at least 1; the image is centred. */
    const Expected_Rect expected_rects[] = {
        { 1280, 800, 4, 0, 0 },      /* the default window: exact 4x, no bars */
        { 1920, 1080, 5, 160, 40 },  /* 1600x1000 */
        { 2560, 1440, 7, 160, 20 },  /* 2240x1400 */
        { 3840, 2160, 10, 320, 80 }, /* 3200x2000 */
        { 1366, 768, 3, 203, 84 },   /* 960x600 */
        { 3440, 1440, 7, 600, 20 },  /* ultrawide, 2240x1400 */
        { 1080, 1920, 3, 60, 660 },  /* portrait, 960x600 */
        { 960, 600, 3, 0, 0 },       /* exact 3x, no bars */
        { 320, 200, 1, 0, 0 },       /* exact 1x */
    };

} // namespace

TEST(Viewport, IntegerScaleAndCentredRect)
{
    for(const Expected_Rect & expected : expected_rects)
    {
        struct s_VIEWPORT viewport = Make_Viewport(expected.drawable_width, expected.drawable_height);
        SCOPED_TRACE(testing::Message() << expected.drawable_width << "x" << expected.drawable_height);
        EXPECT_EQ(viewport.scale, expected.scale);
        EXPECT_EQ(viewport.rect_width, GAME_WIDTH * expected.scale);
        EXPECT_EQ(viewport.rect_height, GAME_HEIGHT * expected.scale);
        EXPECT_EQ(viewport.rect_x, expected.rect_x);
        EXPECT_EQ(viewport.rect_y, expected.rect_y);
    }
}

TEST(Viewport, DrawableSmallerThanGameFloorsScaleAtOne)
{
    struct s_VIEWPORT viewport = Make_Viewport(200, 100);
    EXPECT_EQ(viewport.scale, 1);
    EXPECT_EQ(viewport.rect_width, 320);
    EXPECT_EQ(viewport.rect_height, 200);
    EXPECT_EQ(viewport.rect_x, -60);
    EXPECT_EQ(viewport.rect_y, -50);
}

TEST(Viewport, ZeroSizedWindowDoesNotDivideByZero)
{
    struct s_VIEWPORT viewport = Make_Viewport(0, 0, 0, 0);
    int game_x = -1;
    int game_y = -1;
    int window_x = -1;
    int window_y = -1;
    EXPECT_EQ(viewport.scale, 1);
    Viewport_Window_To_Game(&viewport, 0, 0, &game_x, &game_y);
    Viewport_Game_To_Window(&viewport, 0, 0, &window_x, &window_y);
    EXPECT_GE(game_x, 0);
    EXPECT_LE(game_x, GAME_WIDTH - 1);
    EXPECT_GE(game_y, 0);
    EXPECT_LE(game_y, GAME_HEIGHT - 1);
}

TEST(Viewport, RoundTripEveryGamePixel)
{
    for(const Expected_Rect & expected : expected_rects)
    {
        struct s_VIEWPORT viewport = Make_Viewport(expected.drawable_width, expected.drawable_height);
        SCOPED_TRACE(testing::Message() << expected.drawable_width << "x" << expected.drawable_height);
        for(int game_y = 0; game_y < GAME_HEIGHT; game_y++)
        {
            for(int game_x = 0; game_x < GAME_WIDTH; game_x++)
            {
                int window_x = 0;
                int window_y = 0;
                int back_x = -1;
                int back_y = -1;
                Viewport_Game_To_Window(&viewport, game_x, game_y, &window_x, &window_y);
                ASSERT_EQ(Viewport_Window_To_Game(&viewport, window_x, window_y, &back_x, &back_y), 1) << game_x << "," << game_y;
                ASSERT_EQ(back_x, game_x);
                ASSERT_EQ(back_y, game_y);
            }
        }
    }
}

TEST(Viewport, WindowPixelsMapToTheGamePixelDrawnThere)
{
    /* 1920x1080: 5x, image at (160,40)-(1759,1039). X and Y have different slack. */
    struct s_VIEWPORT viewport = Make_Viewport(1920, 1080);
    int game_x = -1;
    int game_y = -1;

    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 160, 40, &game_x, &game_y), 1);
    EXPECT_EQ(game_x, 0);
    EXPECT_EQ(game_y, 0);

    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 164, 44, &game_x, &game_y), 1);
    EXPECT_EQ(game_x, 0);
    EXPECT_EQ(game_y, 0);

    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 165, 45, &game_x, &game_y), 1);
    EXPECT_EQ(game_x, 1);
    EXPECT_EQ(game_y, 1);

    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 1759, 1039, &game_x, &game_y), 1);
    EXPECT_EQ(game_x, 319);
    EXPECT_EQ(game_y, 199);

    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 960, 540, &game_x, &game_y), 1);
    EXPECT_EQ(game_x, 160);
    EXPECT_EQ(game_y, 100);
}

TEST(Viewport, PointsInTheBarsClampToTheNearestEdge)
{
    struct s_VIEWPORT viewport = Make_Viewport(1920, 1080);
    int game_x = -1;
    int game_y = -1;

    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 159, 540, &game_x, &game_y), 0); /* left bar */
    EXPECT_EQ(game_x, 0);
    EXPECT_EQ(game_y, 100);

    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 1760, 540, &game_x, &game_y), 0); /* right bar */
    EXPECT_EQ(game_x, 319);
    EXPECT_EQ(game_y, 100);

    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 960, 39, &game_x, &game_y), 0); /* top bar */
    EXPECT_EQ(game_x, 160);
    EXPECT_EQ(game_y, 0);

    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 960, 1040, &game_x, &game_y), 0); /* bottom bar */
    EXPECT_EQ(game_x, 160);
    EXPECT_EQ(game_y, 199);

    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 0, 0, &game_x, &game_y), 0); /* corner */
    EXPECT_EQ(game_x, 0);
    EXPECT_EQ(game_y, 0);
}

TEST(Viewport, PointsOutsideTheWindowClamp)
{
    struct s_VIEWPORT viewport = Make_Viewport(1280, 800);
    int game_x = -1;
    int game_y = -1;

    EXPECT_EQ(Viewport_Window_To_Game(&viewport, -50, -50, &game_x, &game_y), 0);
    EXPECT_EQ(game_x, 0);
    EXPECT_EQ(game_y, 0);

    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 5000, 5000, &game_x, &game_y), 0);
    EXPECT_EQ(game_x, 319);
    EXPECT_EQ(game_y, 199);

    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 1280, 400, &game_x, &game_y), 0); /* one past the right edge */
    EXPECT_EQ(game_x, 319);
}

TEST(Viewport, DefaultWindowMatchesTheOldFourTimesMapping)
{
    /* Before the Viewport, the 1280x800 window mapped window -> game by dividing by 4. Nothing may change there. */
    struct s_VIEWPORT viewport = Make_Viewport(1280, 800);
    for(int window_y = 0; window_y < 800; window_y += 7)
    {
        for(int window_x = 0; window_x < 1280; window_x += 3)
        {
            int game_x = -1;
            int game_y = -1;
            ASSERT_EQ(Viewport_Window_To_Game(&viewport, window_x, window_y, &game_x, &game_y), 1);
            ASSERT_EQ(game_x, window_x / 4);
            ASSERT_EQ(game_y, window_y / 4);
        }
    }
}

TEST(Viewport, HighDpiUsesDrawablePixelsForTheRectAndWindowCoordinatesForTheMouse)
{
    /* 150%: window 1280x800, drawable 1920x1200 -> 6x, no bars. */
    struct s_VIEWPORT viewport = Make_Viewport(1280, 800, 1920, 1200);
    int game_x = -1;
    int game_y = -1;
    EXPECT_EQ(viewport.scale, 6);
    EXPECT_EQ(viewport.rect_x, 0);
    EXPECT_EQ(viewport.rect_y, 0);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 640, 400, &game_x, &game_y), 1);
    EXPECT_EQ(game_x, 160);
    EXPECT_EQ(game_y, 100);

    /* 200%: window 1280x720, drawable 2560x1440 -> 7x, image at (160,20) pixels = (80,10) window units, 1120x700 units. */
    viewport = Make_Viewport(1280, 720, 2560, 1440);
    EXPECT_EQ(viewport.scale, 7);
    EXPECT_EQ(viewport.rect_x, 160);
    EXPECT_EQ(viewport.rect_y, 20);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 80, 10, &game_x, &game_y), 1);
    EXPECT_EQ(game_x, 0);
    EXPECT_EQ(game_y, 0);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 79, 10, &game_x, &game_y), 0);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 1199, 709, &game_x, &game_y), 1);
    EXPECT_EQ(game_x, 319);
    EXPECT_EQ(game_y, 199);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 1200, 710, &game_x, &game_y), 0);
}

TEST(Viewport, HighDpiRoundTripEveryGamePixel)
{
    const int sizes[][4] = {
        { 1280, 800, 1920, 1200 }, /* 150% */
        { 1280, 720, 2560, 1440 }, /* 200%, letterboxed */
        { 1536, 864, 1920, 1080 }, /* 125% of a 1080p panel */
    };
    for(const auto & size : sizes)
    {
        struct s_VIEWPORT viewport = Make_Viewport(size[0], size[1], size[2], size[3]);
        SCOPED_TRACE(testing::Message() << size[0] << "x" << size[1] << " -> " << size[2] << "x" << size[3]);
        for(int game_y = 0; game_y < GAME_HEIGHT; game_y++)
        {
            for(int game_x = 0; game_x < GAME_WIDTH; game_x++)
            {
                int window_x = 0;
                int window_y = 0;
                int back_x = -1;
                int back_y = -1;
                Viewport_Game_To_Window(&viewport, game_x, game_y, &window_x, &window_y);
                ASSERT_EQ(Viewport_Window_To_Game(&viewport, window_x, window_y, &back_x, &back_y), 1) << game_x << "," << game_y;
                ASSERT_EQ(back_x, game_x);
                ASSERT_EQ(back_y, game_y);
            }
        }
    }
}

TEST(Viewport, EqualDetectsChanges)
{
    struct s_VIEWPORT viewport_a = Make_Viewport(1280, 800);
    struct s_VIEWPORT viewport_b = Make_Viewport(1280, 800);
    struct s_VIEWPORT viewport_c = Make_Viewport(1920, 1080);
    EXPECT_EQ(Viewport_Equal(&viewport_a, &viewport_b), 1);
    EXPECT_EQ(Viewport_Equal(&viewport_a, &viewport_c), 0);
}

TEST(Viewport, ImageRectInWindowIs1920x1080Letterbox)
{
    struct s_VIEWPORT viewport = Make_Viewport(1920, 1080);
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    Viewport_Image_Rect_In_Window(&viewport, &x, &y, &width, &height);
    EXPECT_EQ(x, 160);
    EXPECT_EQ(y, 40);
    EXPECT_EQ(width, 1600);
    EXPECT_EQ(height, 1000);
}

TEST(Viewport, ImageRectInWindowMatchesWindowToGameExactly)
{
    /* The mouse is confined to this rect while captured, so every point on its border must be on the image and every point
       one step outside it must not be (or must be outside the window). */
    const int sizes[][4] = {
        { 1280, 800, 1280, 800 }, { 1920, 1080, 1920, 1080 }, { 1366, 768, 1366, 768 }, { 3440, 1440, 3440, 1440 }, { 1080, 1920, 1080, 1920 }, { 1280, 800, 1920, 1200 },  /* 150% */
        { 1280, 720, 2560, 1440 },  /* 200%, letterboxed */
        { 1536, 864, 1920, 1080 },  /* 125% */
        { 1229, 691, 1920, 1080 },  /* ~156%: window units do not divide the drawable evenly */
    };
    for(const auto & size : sizes)
    {
        struct s_VIEWPORT viewport = Make_Viewport(size[0], size[1], size[2], size[3]);
        int x = 0;
        int y = 0;
        int width = 0;
        int height = 0;
        int game_x = 0;
        int game_y = 0;
        SCOPED_TRACE(testing::Message() << size[0] << "x" << size[1] << " -> " << size[2] << "x" << size[3]);
        Viewport_Image_Rect_In_Window(&viewport, &x, &y, &width, &height);
        ASSERT_GT(width, 0);
        ASSERT_GT(height, 0);
        for(int window_x = x; window_x < x + width; window_x++)
        {
            ASSERT_EQ(Viewport_Window_To_Game(&viewport, window_x, y, &game_x, &game_y), 1) << window_x;
            ASSERT_EQ(Viewport_Window_To_Game(&viewport, window_x, y + height - 1, &game_x, &game_y), 1) << window_x;
            if(y > 0)
            {
                ASSERT_EQ(Viewport_Window_To_Game(&viewport, window_x, y - 1, &game_x, &game_y), 0) << window_x;
            }
            if(y + height < size[1])
            {
                ASSERT_EQ(Viewport_Window_To_Game(&viewport, window_x, y + height, &game_x, &game_y), 0) << window_x;
            }
        }
        for(int window_y = y; window_y < y + height; window_y++)
        {
            ASSERT_EQ(Viewport_Window_To_Game(&viewport, x, window_y, &game_x, &game_y), 1) << window_y;
            ASSERT_EQ(Viewport_Window_To_Game(&viewport, x + width - 1, window_y, &game_x, &game_y), 1) << window_y;
            if(x > 0)
            {
                ASSERT_EQ(Viewport_Window_To_Game(&viewport, x - 1, window_y, &game_x, &game_y), 0) << window_y;
            }
            if(x + width < size[0])
            {
                ASSERT_EQ(Viewport_Window_To_Game(&viewport, x + width, window_y, &game_x, &game_y), 0) << window_y;
            }
        }
    }
}

namespace
{

    struct s_VIEWPORT Make_Fit_Nearest_Viewport(int window_width, int window_height, int drawable_width, int drawable_height)
    {
        struct s_VIEWPORT viewport;
        Viewport_Compute(&viewport, window_width, window_height, drawable_width, drawable_height, GAME_WIDTH, GAME_HEIGHT, VIEWPORT_SCALE_MODE_FIT_NEAREST, VIEWPORT_PIXEL_ASPECT_SQUARE);
        return viewport;
    }

    struct Expected_Fit
    {
        int drawable_width;
        int drawable_height;
        int rect_x;
        int rect_y;
        int rect_width;
        int rect_height;
    };

/* FIT_NEAREST: the full width, or the full height when the full width would be too tall, at 320:200. */
    const Expected_Fit expected_fits[] = {
        { 1280, 800, 0, 0, 1280, 800 },     /* exact */
        { 1920, 1080, 96, 0, 1728, 1080 },  /* full height, 5.4x */
        { 2560, 1440, 128, 0, 2304, 1440 }, /* 7.2x */
        { 3840, 2160, 192, 0, 3456, 2160 }, /* 10.8x */
        { 1366, 768, 69, 0, 1228, 768 },    /* 3.84x */
        { 3440, 1440, 568, 0, 2304, 1440 }, /* ultrawide */
        { 1080, 1920, 0, 622, 1080, 675 },  /* portrait: full width */
        { 960, 600, 0, 0, 960, 600 },       /* exact */
        { 400, 250, 0, 0, 400, 250 },       /* 1.25x: game pixels 1 or 2 drawable pixels wide */
    };

} // namespace

TEST(Viewport, FitNearestFillsTheLimitingAxis)
{
    for(const Expected_Fit & expected : expected_fits)
    {
        struct s_VIEWPORT viewport = Make_Fit_Nearest_Viewport(expected.drawable_width, expected.drawable_height, expected.drawable_width, expected.drawable_height);
        SCOPED_TRACE(testing::Message() << expected.drawable_width << "x" << expected.drawable_height);
        EXPECT_EQ(viewport.scale_mode, VIEWPORT_SCALE_MODE_FIT_NEAREST);
        EXPECT_EQ(viewport.rect_x, expected.rect_x);
        EXPECT_EQ(viewport.rect_y, expected.rect_y);
        EXPECT_EQ(viewport.rect_width, expected.rect_width);
        EXPECT_EQ(viewport.rect_height, expected.rect_height);
    }
}

TEST(Viewport, FitNearestKeepsTheWholeNumberPreScale)
{
    EXPECT_EQ(Make_Fit_Nearest_Viewport(1920, 1080, 1920, 1080).scale, 5);
    EXPECT_EQ(Make_Fit_Nearest_Viewport(1366, 768, 1366, 768).scale, 3);
    EXPECT_EQ(Make_Fit_Nearest_Viewport(400, 250, 400, 250).scale, 1);
}

TEST(Viewport, FitNearestRoundTripEveryGamePixel)
{
    const int sizes[][4] = {
        { 1920, 1080, 1920, 1080 }, { 1366, 768, 1366, 768 }, { 1080, 1920, 1080, 1920 }, { 3440, 1440, 3440, 1440 }, { 400, 250, 400, 250 }, { 1280, 720, 2560, 1440 }, /* 200%: 4.5 window units per game pixel */
        { 1229, 691, 1920, 1080 }, /* ~156% */
    };
    for(const auto & size : sizes)
    {
        struct s_VIEWPORT viewport = Make_Fit_Nearest_Viewport(size[0], size[1], size[2], size[3]);
        SCOPED_TRACE(testing::Message() << size[0] << "x" << size[1] << " -> " << size[2] << "x" << size[3]);
        for(int game_y = 0; game_y < GAME_HEIGHT; game_y++)
        {
            for(int game_x = 0; game_x < GAME_WIDTH; game_x++)
            {
                int window_x = 0;
                int window_y = 0;
                int back_x = -1;
                int back_y = -1;
                Viewport_Game_To_Window(&viewport, game_x, game_y, &window_x, &window_y);
                ASSERT_EQ(Viewport_Window_To_Game(&viewport, window_x, window_y, &back_x, &back_y), 1) << game_x << "," << game_y;
                ASSERT_EQ(back_x, game_x);
                ASSERT_EQ(back_y, game_y);
            }
        }
    }
}

TEST(Viewport, FitNearestImageRectMatchesWindowToGame)
{
    struct s_VIEWPORT viewport = Make_Fit_Nearest_Viewport(1920, 1080, 1920, 1080);
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    int game_x = 0;
    int game_y = 0;
    Viewport_Image_Rect_In_Window(&viewport, &x, &y, &width, &height);
    EXPECT_EQ(x, 96);
    EXPECT_EQ(y, 0);
    EXPECT_EQ(width, 1728);
    EXPECT_EQ(height, 1080);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 96, 540, &game_x, &game_y), 1);
    EXPECT_EQ(game_x, 0);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 95, 540, &game_x, &game_y), 0);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 1823, 540, &game_x, &game_y), 1);
    EXPECT_EQ(game_x, 319);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 1824, 540, &game_x, &game_y), 0);
}

TEST(Viewport, UnknownScaleModeIsInteger)
{
    struct s_VIEWPORT viewport;
    Viewport_Compute(&viewport, 1920, 1080, 1920, 1080, GAME_WIDTH, GAME_HEIGHT, 99, VIEWPORT_PIXEL_ASPECT_SQUARE);
    EXPECT_EQ(viewport.scale_mode, VIEWPORT_SCALE_MODE_INTEGER);
    EXPECT_EQ(viewport.rect_width, 1600);
}

TEST(Viewport, ScaleModeNamesAreCaseInsensitive)
{
    int scale_mode = -1;
    EXPECT_EQ(Viewport_Scale_Mode_From_Name("integer", &scale_mode), 1);
    EXPECT_EQ(scale_mode, VIEWPORT_SCALE_MODE_INTEGER);
    EXPECT_EQ(Viewport_Scale_Mode_From_Name("FIT_Nearest", &scale_mode), 1);
    EXPECT_EQ(scale_mode, VIEWPORT_SCALE_MODE_FIT_NEAREST);
    EXPECT_EQ(Viewport_Scale_Mode_From_Name("Integer", &scale_mode), 1);
    EXPECT_EQ(scale_mode, VIEWPORT_SCALE_MODE_INTEGER);

    scale_mode = -1;
    EXPECT_EQ(Viewport_Scale_Mode_From_Name("fit-nearest", &scale_mode), 0);
    EXPECT_EQ(Viewport_Scale_Mode_From_Name("", &scale_mode), 0);
    EXPECT_EQ(Viewport_Scale_Mode_From_Name("fit_nearestx", &scale_mode), 0);
    EXPECT_EQ(Viewport_Scale_Mode_From_Name(nullptr, &scale_mode), 0);
    EXPECT_EQ(scale_mode, -1);

    EXPECT_STREQ(Viewport_Scale_Mode_Name(VIEWPORT_SCALE_MODE_INTEGER), "integer");
    EXPECT_STREQ(Viewport_Scale_Mode_Name(VIEWPORT_SCALE_MODE_FIT_NEAREST), "fit_nearest");
}

TEST(Viewport, GameSizeInDrawableMatchesTheImageInEveryScaleMode)
{
    /* drawable, scale mode, then the 16x16 cursor's size in drawable pixels */
    const int cases[][5] = {
        { 1280, 800, VIEWPORT_SCALE_MODE_INTEGER, 64, 64 },
        { 2560, 1440, VIEWPORT_SCALE_MODE_INTEGER, 112, 112 },
        { 2560, 1440, VIEWPORT_SCALE_MODE_FIT_NEAREST, 115, 115 },          /* 2304 / 320 = 7.2 */
        { 2560, 1440, VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR, 115, 115 },
        { 1366, 768, VIEWPORT_SCALE_MODE_FIT_NEAREST, 61, 61 },             /* 1228.8 / 320 x 16 = 61.44 */
        { 200, 100, VIEWPORT_SCALE_MODE_INTEGER, 16, 16 },                  /* never smaller than the sprite */
    };
    for(const auto & c : cases)
    {
        struct s_VIEWPORT viewport;
        int width = 0;
        int height = 0;
        Viewport_Compute(&viewport, c[0], c[1], c[0], c[1], GAME_WIDTH, GAME_HEIGHT, c[2], VIEWPORT_PIXEL_ASPECT_SQUARE);
        Viewport_Game_Size_In_Drawable(&viewport, 16, 16, &width, &height);
        EXPECT_EQ(width, c[3]) << c[0] << "x" << c[1] << " mode " << c[2];
        EXPECT_EQ(height, c[4]) << c[0] << "x" << c[1] << " mode " << c[2];
    }
}

TEST(Viewport, WindowStateNamesAreCaseInsensitive)
{
    int window_state = -1;
    EXPECT_EQ(Viewport_Window_State_From_Name("windowed", &window_state), 1);
    EXPECT_EQ(window_state, VIEWPORT_WINDOW_STATE_WINDOWED);
    EXPECT_EQ(Viewport_Window_State_From_Name("Borderless", &window_state), 1);
    EXPECT_EQ(window_state, VIEWPORT_WINDOW_STATE_BORDERLESS);
    EXPECT_EQ(Viewport_Window_State_From_Name("FULLSCREEN", &window_state), 1);
    EXPECT_EQ(window_state, VIEWPORT_WINDOW_STATE_FULLSCREEN);

    window_state = -1;
    EXPECT_EQ(Viewport_Window_State_From_Name("yes", &window_state), 0);
    EXPECT_EQ(Viewport_Window_State_From_Name("border-less", &window_state), 0);
    EXPECT_EQ(Viewport_Window_State_From_Name("", &window_state), 0);
    EXPECT_EQ(Viewport_Window_State_From_Name(nullptr, &window_state), 0);
    EXPECT_EQ(window_state, -1);

    EXPECT_STREQ(Viewport_Window_State_Name(VIEWPORT_WINDOW_STATE_WINDOWED), "windowed");
    EXPECT_STREQ(Viewport_Window_State_Name(VIEWPORT_WINDOW_STATE_BORDERLESS), "borderless");
    EXPECT_STREQ(Viewport_Window_State_Name(VIEWPORT_WINDOW_STATE_FULLSCREEN), "fullscreen");
    EXPECT_EQ(Viewport_Window_State_Normalize(99), VIEWPORT_WINDOW_STATE_WINDOWED);
}

TEST(Viewport, FitSharpBilinearUsesTheFitRectAndTheWholeNumberPreScale)
{
    /* drawable, then the pre-scale: the largest whole number that fits (the intermediate image is 320 x 200 times it). */
    const int cases[][3] = {
        { 1280, 800, 4 },
        { 1920, 1080, 5 },
        { 2560, 1440, 7 },
        { 3840, 2160, 10 },
        { 1366, 768, 3 },
        { 3440, 1440, 7 },
        { 1080, 1920, 3 },
        { 960, 600, 3 },
        { 400, 250, 1 },
    };
    for(const auto & item : cases)
    {
        struct s_VIEWPORT sharp;
        struct s_VIEWPORT nearest = Make_Fit_Nearest_Viewport(item[0], item[1], item[0], item[1]);
        Viewport_Compute(&sharp, item[0], item[1], item[0], item[1], GAME_WIDTH, GAME_HEIGHT, VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR, VIEWPORT_PIXEL_ASPECT_SQUARE);
        SCOPED_TRACE(testing::Message() << item[0] << "x" << item[1]);
        EXPECT_EQ(sharp.scale_mode, VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR);
        EXPECT_EQ(sharp.scale, item[2]);
        EXPECT_EQ(sharp.rect_x, nearest.rect_x);
        EXPECT_EQ(sharp.rect_y, nearest.rect_y);
        EXPECT_EQ(sharp.rect_width, nearest.rect_width);
        EXPECT_EQ(sharp.rect_height, nearest.rect_height);
        /* the intermediate is never larger than the rect, so the linear step only ever enlarges */
        EXPECT_LE(GAME_WIDTH * sharp.scale, sharp.rect_width);
        EXPECT_LE(GAME_HEIGHT * sharp.scale, sharp.rect_height);
    }
}

TEST(Viewport, FitSharpBilinearName)
{
    int scale_mode = -1;
    EXPECT_EQ(Viewport_Scale_Mode_From_Name("Fit_Sharp_Bilinear", &scale_mode), 1);
    EXPECT_EQ(scale_mode, VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR);
    EXPECT_STREQ(Viewport_Scale_Mode_Name(VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR), "fit_sharp_bilinear");
    EXPECT_EQ(Viewport_Scale_Mode_Normalize(VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR), VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR);
    EXPECT_EQ(Viewport_Scale_Mode_Normalize(7), VIEWPORT_SCALE_MODE_INTEGER);
}

/* ---- 4:3 aspect ratio (PLAN-Aspect-Ratio.md phase 11) ---- */

namespace
{

    struct s_VIEWPORT Make_4_3_Viewport(int window_width, int window_height, int drawable_width, int drawable_height, int scale_mode)
    {
        struct s_VIEWPORT viewport;
        Viewport_Compute(&viewport, window_width, window_height, drawable_width, drawable_height, GAME_WIDTH, GAME_HEIGHT, scale_mode, VIEWPORT_PIXEL_ASPECT_4_3);
        return viewport;
    }

    struct s_VIEWPORT Make_4_3_Viewport(int width, int height, int scale_mode)
    {
        return Make_4_3_Viewport(width, height, width, height, scale_mode);
    }

    /* Every game pixel maps to a window coordinate that maps back to it. */
    void Expect_Round_Trip(const struct s_VIEWPORT & viewport)
    {
        for(int game_y = 0; game_y < GAME_HEIGHT; game_y++)
        {
            for(int game_x = 0; game_x < GAME_WIDTH; game_x++)
            {
                int window_x = 0;
                int window_y = 0;
                int back_x = -1;
                int back_y = -1;
                Viewport_Game_To_Window(&viewport, game_x, game_y, &window_x, &window_y);
                ASSERT_EQ(Viewport_Window_To_Game(&viewport, window_x, window_y, &back_x, &back_y), 1) << game_x << "," << game_y;
                ASSERT_EQ(back_x, game_x);
                ASSERT_EQ(back_y, game_y);
            }
        }
    }

    /* The in-window image rect is exactly the set of window points reported as on the image (every row of the window, at the
       columns either side of each edge and in the middle). */
    void Expect_Image_Rect_Matches_Window_To_Game(const struct s_VIEWPORT & viewport)
    {
        int x = 0;
        int y = 0;
        int width = 0;
        int height = 0;
        int game_x = 0;
        int game_y = 0;
        Viewport_Image_Rect_In_Window(&viewport, &x, &y, &width, &height);
        ASSERT_GT(width, 0);
        ASSERT_GT(height, 0);
        for(int window_y = 0; window_y < viewport.window_height; window_y++)
        {
            int inside_y = (window_y >= y) && (window_y < y + height);
            int window_x_values[] = { x - 1, x, x + width / 2, x + width - 1, x + width };
            for(int window_x : window_x_values)
            {
                int inside = inside_y && (window_x >= x) && (window_x < x + width);
                if((window_x < 0) || (window_x >= viewport.window_width))
                {
                    continue;
                }
                ASSERT_EQ(Viewport_Window_To_Game(&viewport, window_x, window_y, &game_x, &game_y), inside) << window_x << "," << window_y;
            }
        }
    }

    /* Down the middle column: game rows never go backwards, every game row is hit, and each takes the floor or the ceiling of
       rect_height / 200 drawable rows (uneven rows differ by at most one). */
    void Expect_Rows_Monotonic_And_Even_Within_One(const struct s_VIEWPORT & viewport)
    {
        int counts[GAME_HEIGHT] = { 0 };
        int previous = -1;
        int window_x = viewport.rect_x + viewport.rect_width / 2;
        for(int window_y = viewport.rect_y; window_y < viewport.rect_y + viewport.rect_height; window_y++)
        {
            int game_x = -1;
            int game_y = -1;
            ASSERT_EQ(Viewport_Window_To_Game(&viewport, window_x, window_y, &game_x, &game_y), 1) << window_y;
            ASSERT_GE(game_y, previous) << window_y;
            previous = game_y;
            counts[game_y]++;
        }
        for(int game_y = 0; game_y < GAME_HEIGHT; game_y++)
        {
            ASSERT_GE(counts[game_y], viewport.rect_height / GAME_HEIGHT) << "game row " << game_y;
            ASSERT_LE(counts[game_y], (viewport.rect_height + GAME_HEIGHT - 1) / GAME_HEIGHT) << "game row " << game_y;
        }
    }

    struct Expected_4_3
    {
        int drawable_width;
        int drawable_height;
        int scale_mode;
        int scale;
        int rect_x;
        int rect_y;
        int rect_width;
        int rect_height;
    };

/* INTEGER: 320k x 240k, the largest k that fits. FIT_*: the largest 4:3 rect that fits. scale: the largest whole number at which
   320x200 fits in the rect (the sharp-bilinear pre-scale). */
    const Expected_4_3 expected_4_3[] = {
        { 1280, 960, VIEWPORT_SCALE_MODE_INTEGER, 4, 0, 0, 1280, 960 },          /* the 4:3 window: x4, no bars */
        { 1280, 800, VIEWPORT_SCALE_MODE_INTEGER, 3, 160, 40, 960, 720 },        /* the 16:10 window */
        { 1920, 1080, VIEWPORT_SCALE_MODE_INTEGER, 4, 320, 60, 1280, 960 },
        { 2560, 1440, VIEWPORT_SCALE_MODE_INTEGER, 6, 320, 0, 1920, 1440 },
        { 1600, 1200, VIEWPORT_SCALE_MODE_INTEGER, 5, 0, 0, 1600, 1200 },        /* x5: every game row exactly 6 rows */
        { 200, 100, VIEWPORT_SCALE_MODE_INTEGER, 1, -60, -70, 320, 240 },        /* smaller than the image */
        { 1920, 1080, VIEWPORT_SCALE_MODE_FIT_NEAREST, 4, 240, 0, 1440, 1080 },
        { 1280, 800, VIEWPORT_SCALE_MODE_FIT_NEAREST, 3, 107, 0, 1066, 800 },
        { 1280, 960, VIEWPORT_SCALE_MODE_FIT_NEAREST, 4, 0, 0, 1280, 960 },
        { 2560, 1440, VIEWPORT_SCALE_MODE_FIT_NEAREST, 6, 320, 0, 1920, 1440 },
        { 1080, 1920, VIEWPORT_SCALE_MODE_FIT_NEAREST, 3, 0, 555, 1080, 810 },   /* portrait: full width */
        { 1920, 1080, VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR, 4, 240, 0, 1440, 1080 },
        { 1280, 800, VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR, 3, 107, 0, 1066, 800 },
    };

} // namespace

TEST(Viewport, AspectRatio4_3RectsInEveryScaleMode)
{
    for(const Expected_4_3 & expected : expected_4_3)
    {
        struct s_VIEWPORT viewport = Make_4_3_Viewport(expected.drawable_width, expected.drawable_height, expected.scale_mode);
        SCOPED_TRACE(testing::Message() << expected.drawable_width << "x" << expected.drawable_height << " mode " << expected.scale_mode);
        EXPECT_EQ(viewport.pixel_aspect, VIEWPORT_PIXEL_ASPECT_4_3);
        EXPECT_EQ(viewport.scale, expected.scale);
        EXPECT_EQ(viewport.rect_x, expected.rect_x);
        EXPECT_EQ(viewport.rect_y, expected.rect_y);
        EXPECT_EQ(viewport.rect_width, expected.rect_width);
        EXPECT_EQ(viewport.rect_height, expected.rect_height);
    }
}

TEST(Viewport, AspectRatio4_3SharpBilinearIntermediateNeverExceedsTheRect)
{
    const int sizes[][2] = { { 1280, 960 }, { 1280, 800 }, { 1920, 1080 }, { 2560, 1440 }, { 3840, 2160 }, { 1366, 768 }, { 1080, 1920 } };
    for(const auto & size : sizes)
    {
        struct s_VIEWPORT viewport = Make_4_3_Viewport(size[0], size[1], VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR);
        SCOPED_TRACE(testing::Message() << size[0] << "x" << size[1]);
        EXPECT_LE(GAME_WIDTH * viewport.scale, viewport.rect_width);
        EXPECT_LE(GAME_HEIGHT * viewport.scale, viewport.rect_height);
        EXPECT_GT(GAME_WIDTH * (viewport.scale + 1), viewport.rect_width);  /* and it is the largest whole number that fits */
    }
}

TEST(Viewport, AspectRatio4_3ZeroSizedWindowDoesNotDivideByZero)
{
    struct s_VIEWPORT viewport = Make_4_3_Viewport(0, 0, VIEWPORT_SCALE_MODE_INTEGER);
    int game_x = -1;
    int game_y = -1;
    EXPECT_EQ(viewport.scale, 1);
    EXPECT_EQ(viewport.rect_width, 320);
    EXPECT_EQ(viewport.rect_height, 240);
    Viewport_Window_To_Game(&viewport, 0, 0, &game_x, &game_y);
    EXPECT_GE(game_y, 0);
    EXPECT_LE(game_y, GAME_HEIGHT - 1);
    viewport = Make_4_3_Viewport(0, 0, VIEWPORT_SCALE_MODE_FIT_NEAREST);
    EXPECT_EQ(viewport.rect_width, 1);
    EXPECT_EQ(viewport.rect_height, 1);
}

TEST(Viewport, AspectRatio4_3AtFiveTimesEveryGameRowIsSixRows)
{
    struct s_VIEWPORT viewport = Make_4_3_Viewport(1600, 1200, VIEWPORT_SCALE_MODE_INTEGER);
    for(int window_y = 0; window_y < 1200; window_y++)
    {
        for(int window_x = 0; window_x < 1600; window_x += 37)
        {
            int game_x = -1;
            int game_y = -1;
            ASSERT_EQ(Viewport_Window_To_Game(&viewport, window_x, window_y, &game_x, &game_y), 1);
            ASSERT_EQ(game_x, window_x / 5);
            ASSERT_EQ(game_y, window_y / 6);
        }
    }
}

TEST(Viewport, AspectRatio4_3UnevenRowsAreMonotonicAndCoverEveryGameRow)
{
    const int cases[][3] = {
        { 1280, 960, VIEWPORT_SCALE_MODE_INTEGER },             /* 4.8 rows per game row */
        { 1280, 800, VIEWPORT_SCALE_MODE_INTEGER },             /* 3.6 */
        { 2560, 1440, VIEWPORT_SCALE_MODE_INTEGER },            /* 7.2 */
        { 1920, 1080, VIEWPORT_SCALE_MODE_FIT_NEAREST },        /* 5.4 */
        { 1280, 800, VIEWPORT_SCALE_MODE_FIT_NEAREST },         /* 4.0 */
        { 1080, 1920, VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR }, /* 4.05 */
    };
    for(const auto & c : cases)
    {
        struct s_VIEWPORT viewport = Make_4_3_Viewport(c[0], c[1], c[2]);
        SCOPED_TRACE(testing::Message() << c[0] << "x" << c[1] << " mode " << c[2]);
        Expect_Rows_Monotonic_And_Even_Within_One(viewport);
    }
}

TEST(Viewport, RowRuleIsThePixelCentreAsEveryRendererDraws)
{
    /* Measured with remom_video_probe --row-check on Direct3D 11, Direct3D 9, OpenGL, OpenGL ES 2 and the software renderer:
       nearest-neighbour scaling draws, in each drawable row, the game row that the drawable row's centre falls in. */
    int game_x = -1;
    int game_y = -1;

    /* 4:3 at x4, 1280x960: game row 2 ends at 14.4, so drawable row 14 (centre 14.5) shows game row 3 and row 13 shows row 2. */
    struct s_VIEWPORT viewport = Make_4_3_Viewport(1280, 960, VIEWPORT_SCALE_MODE_INTEGER);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 640, 13, &game_x, &game_y), 1);
    EXPECT_EQ(game_y, 2);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 640, 14, &game_x, &game_y), 1);
    EXPECT_EQ(game_y, 3);

    /* 16:10 fit_nearest, 1920x1080: 5.4 drawable rows per game row; game row 3 starts at 16.2, so drawable row 16 (centre 16.5)
       shows game row 3. */
    viewport = Make_Fit_Nearest_Viewport(1920, 1080, 1920, 1080);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 960, 15, &game_x, &game_y), 1);
    EXPECT_EQ(game_y, 2);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 960, 16, &game_x, &game_y), 1);
    EXPECT_EQ(game_y, 3);
}

TEST(Viewport, AspectRatio4_3RoundTripEveryGamePixel)
{
    const int sizes[][5] = {
        { 1280, 960, 1280, 960, VIEWPORT_SCALE_MODE_INTEGER },
        { 1280, 800, 1280, 800, VIEWPORT_SCALE_MODE_INTEGER },
        { 1920, 1080, 1920, 1080, VIEWPORT_SCALE_MODE_FIT_NEAREST },
        { 1280, 800, 1280, 800, VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR },
        { 1280, 960, 1920, 1440, VIEWPORT_SCALE_MODE_INTEGER },     /* 150% */
        { 1536, 864, 1920, 1080, VIEWPORT_SCALE_MODE_FIT_NEAREST }, /* 125% */
        { 1229, 691, 1920, 1080, VIEWPORT_SCALE_MODE_FIT_NEAREST }, /* ~156% */
    };
    for(const auto & size : sizes)
    {
        struct s_VIEWPORT viewport = Make_4_3_Viewport(size[0], size[1], size[2], size[3], size[4]);
        SCOPED_TRACE(testing::Message() << size[0] << "x" << size[1] << " -> " << size[2] << "x" << size[3] << " mode " << size[4]);
        Expect_Round_Trip(viewport);
    }
}

TEST(Viewport, AspectRatio4_3ImageRectMatchesWindowToGameExactly)
{
    const int sizes[][5] = {
        { 1280, 960, 1280, 960, VIEWPORT_SCALE_MODE_INTEGER },
        { 1280, 800, 1280, 800, VIEWPORT_SCALE_MODE_INTEGER },
        { 1920, 1080, 1920, 1080, VIEWPORT_SCALE_MODE_INTEGER },
        { 1920, 1080, 1920, 1080, VIEWPORT_SCALE_MODE_FIT_NEAREST },
        { 1280, 800, 1280, 800, VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR },
        { 1080, 1920, 1080, 1920, VIEWPORT_SCALE_MODE_FIT_NEAREST },
        { 1280, 960, 1920, 1440, VIEWPORT_SCALE_MODE_INTEGER },     /* 150% */
        { 1229, 691, 1920, 1080, VIEWPORT_SCALE_MODE_FIT_NEAREST }, /* ~156% */
    };
    for(const auto & size : sizes)
    {
        struct s_VIEWPORT viewport = Make_4_3_Viewport(size[0], size[1], size[2], size[3], size[4]);
        SCOPED_TRACE(testing::Message() << size[0] << "x" << size[1] << " -> " << size[2] << "x" << size[3] << " mode " << size[4]);
        Expect_Image_Rect_Matches_Window_To_Game(viewport);
    }
}

TEST(Viewport, AspectRatio4_3PointsInTheBarsAreOffTheImage)
{
    /* 1920x1080 integer: 1280x960 at (320,60). */
    struct s_VIEWPORT viewport = Make_4_3_Viewport(1920, 1080, VIEWPORT_SCALE_MODE_INTEGER);
    int game_x = -1;
    int game_y = -1;
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 319, 540, &game_x, &game_y), 0);
    EXPECT_EQ(game_x, 0);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 320, 540, &game_x, &game_y), 1);
    EXPECT_EQ(game_x, 0);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 1599, 540, &game_x, &game_y), 1);
    EXPECT_EQ(game_x, 319);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 1600, 540, &game_x, &game_y), 0);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 960, 59, &game_x, &game_y), 0);
    EXPECT_EQ(game_y, 0);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 960, 60, &game_x, &game_y), 1);
    EXPECT_EQ(game_y, 0);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 960, 1019, &game_x, &game_y), 1);
    EXPECT_EQ(game_y, 199);
    EXPECT_EQ(Viewport_Window_To_Game(&viewport, 960, 1020, &game_x, &game_y), 0);
    EXPECT_EQ(game_y, 199);
}

TEST(Viewport, AspectRatioChangesAreDetectedAndUnknownAspectsAreSquare)
{
    struct s_VIEWPORT square = Make_Viewport(1280, 800);
    struct s_VIEWPORT four_three = Make_4_3_Viewport(1280, 800, VIEWPORT_SCALE_MODE_INTEGER);
    struct s_VIEWPORT unknown;
    EXPECT_EQ(Viewport_Equal(&square, &four_three), 0);
    Viewport_Compute(&unknown, 1280, 800, 1280, 800, GAME_WIDTH, GAME_HEIGHT, VIEWPORT_SCALE_MODE_INTEGER, 99);
    EXPECT_EQ(unknown.pixel_aspect, VIEWPORT_PIXEL_ASPECT_SQUARE);
    EXPECT_EQ(Viewport_Equal(&square, &unknown), 1);
    EXPECT_EQ(Viewport_Pixel_Aspect_Normalize(VIEWPORT_PIXEL_ASPECT_4_3), VIEWPORT_PIXEL_ASPECT_4_3);
    EXPECT_EQ(Viewport_Pixel_Aspect_Normalize(-1), VIEWPORT_PIXEL_ASPECT_SQUARE);
}

TEST(Viewport, AspectRatioNamesAreCaseInsensitive)
{
    int pixel_aspect = -1;
    EXPECT_EQ(Viewport_Aspect_Ratio_From_Name("16:10", &pixel_aspect), 1);
    EXPECT_EQ(pixel_aspect, VIEWPORT_PIXEL_ASPECT_SQUARE);
    EXPECT_EQ(Viewport_Aspect_Ratio_From_Name("4:3", &pixel_aspect), 1);
    EXPECT_EQ(pixel_aspect, VIEWPORT_PIXEL_ASPECT_4_3);

    pixel_aspect = -1;
    EXPECT_EQ(Viewport_Aspect_Ratio_From_Name("4x3", &pixel_aspect), 0);
    EXPECT_EQ(Viewport_Aspect_Ratio_From_Name("4:3 ", &pixel_aspect), 0);
    EXPECT_EQ(Viewport_Aspect_Ratio_From_Name("square", &pixel_aspect), 0);
    EXPECT_EQ(Viewport_Aspect_Ratio_From_Name("16:9", &pixel_aspect), 0);
    EXPECT_EQ(Viewport_Aspect_Ratio_From_Name("", &pixel_aspect), 0);
    EXPECT_EQ(Viewport_Aspect_Ratio_From_Name(nullptr, &pixel_aspect), 0);
    EXPECT_EQ(pixel_aspect, -1);

    EXPECT_STREQ(Viewport_Aspect_Ratio_Name(VIEWPORT_PIXEL_ASPECT_SQUARE), "16:10");
    EXPECT_STREQ(Viewport_Aspect_Ratio_Name(VIEWPORT_PIXEL_ASPECT_4_3), "4:3");
    EXPECT_STREQ(Viewport_Aspect_Ratio_Name(42), "16:10");
}

TEST(Viewport, AspectRatio4_3CursorIsTallerThanWideByTheImageRatio)
{
    /* PLAN-Aspect-Ratio.md phase 12: the 16x16 hardware cursor covers as many drawable pixels as 16x16 game pixels of the image,
       so at 4:3 it is 1.2 times taller than it is wide, like the game pixels. drawable, scale mode, then width and height. */
    const int cases[][5] = {
        { 1280, 960, VIEWPORT_SCALE_MODE_INTEGER, 64, 77 },              /* 16 x 4, 16 x 4.8 = 76.8 */
        { 1600, 1200, VIEWPORT_SCALE_MODE_INTEGER, 80, 96 },             /* x5: exactly 5 x 6 per game pixel */
        { 1920, 1080, VIEWPORT_SCALE_MODE_FIT_NEAREST, 72, 86 },         /* 1440x1080: 4.5 and 5.4 */
        { 1920, 1080, VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR, 72, 86 },
        { 200, 100, VIEWPORT_SCALE_MODE_INTEGER, 16, 19 },               /* 320x240: never smaller than the sprite */
    };
    for(const auto & c : cases)
    {
        struct s_VIEWPORT viewport = Make_4_3_Viewport(c[0], c[1], c[2]);
        int width = 0;
        int height = 0;
        Viewport_Game_Size_In_Drawable(&viewport, 16, 16, &width, &height);
        EXPECT_EQ(width, c[3]) << c[0] << "x" << c[1] << " mode " << c[2];
        EXPECT_EQ(height, c[4]) << c[0] << "x" << c[1] << " mode " << c[2];
    }
}
