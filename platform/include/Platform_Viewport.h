/**
 * Platform_Viewport.h — where the 320x200 game image goes in a window, and the window <-> game coordinate mapping.
 *
 * Pure C: no SDL, no OS calls, no globals. A backend fills a s_VIEWPORT from its window and drawable sizes whenever either
 * changes, draws the game image into the rect, and maps every mouse position through the same s_VIEWPORT, so what is drawn
 * and what is clicked always agree.
 *
 * Two coordinate spaces, because they differ on high-DPI displays:
 *   window coordinates    -- the space mouse events and cursor warps use (SDL_GetWindowSize)
 *   drawable pixels       -- the real pixels the renderer draws into (SDL_GetRendererOutputSize)
 * The rect is in drawable pixels. X and Y each have their own scale and offset.
 *
 * See doc/@AI_Plans/PRD-Fullscreen.md and PLAN-Fullscreen.md.
 */

#ifndef PLATFORM_VIEWPORT_H
#define PLATFORM_VIEWPORT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Scale modes ([Video] scale_mode in ReMoM.ini).
   INTEGER      the largest whole-number scale that fits, centred, with black bars (the default)
   FIT_NEAREST  as large as fits at the game's shape, non-integer, nearest-neighbour
   FIT_SHARP_BILINEAR  the same rect as FIT_NEAREST; drawn nearest-neighbour at the whole-number scale (scale) into an
                       intermediate image, which is then filtered linearly up to the rect -- even pixel sizes, soft edges */
#define VIEWPORT_SCALE_MODE_INTEGER             0
#define VIEWPORT_SCALE_MODE_FIT_NEAREST         1
#define VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR  2

/* Window states ([Video] startup_window in ReMoM.ini; Alt+Enter switches between them). PLAN-Fullscreen.md phase 6.
   WINDOWED    a normal window with a title bar and frame, resizable
   FULLSCREEN  a borderless window covering the whole display the window is on, taskbar included (the default). No display-mode
               change: ReMoM has no exclusive fullscreen. */
#define VIEWPORT_WINDOW_STATE_WINDOWED    0
#define VIEWPORT_WINDOW_STATE_FULLSCREEN  2

/* Pixel aspects ([Video] aspect_ratio in ReMoM.ini). PLAN-Aspect-Ratio.md.
   SQUARE  square pixels: 320x200 shows as 16:10 (aspect_ratio = 16:10, the default)
   4_3     the whole image shows as 4:3, as on a 1994 CRT: each game pixel is 1.2 times taller than it is wide
           (aspect_ratio = 4:3). INTEGER is then the largest whole-number width that fits (320k x 240k), with uneven rows
           except at x5; the fit modes are the largest 4:3 rect that fits. */
#define VIEWPORT_PIXEL_ASPECT_SQUARE  0
#define VIEWPORT_PIXEL_ASPECT_4_3     1

struct s_VIEWPORT
{
    int window_width;     /* window coordinates */
    int window_height;
    int drawable_width;   /* drawable pixels */
    int drawable_height;
    int game_width;       /* game pixels (320x200) */
    int game_height;
    int scale_mode;       /* VIEWPORT_SCALE_MODE_* */
    int pixel_aspect;     /* VIEWPORT_PIXEL_ASPECT_* */
    int scale;            /* whole-number scale: one game pixel is scale x scale drawable pixels */
    int rect_x;           /* where the game image is drawn, in drawable pixels; negative when the drawable is smaller than the image */
    int rect_y;
    int rect_width;
    int rect_height;
};

/**
 * Fill viewport for the given window and drawable sizes. Sizes below 1 are treated as 1, so a minimised window never divides
 * by zero. Unknown scale modes and pixel aspects are treated as INTEGER and SQUARE.
 */
void Viewport_Compute(struct s_VIEWPORT * viewport, int window_width, int window_height, int drawable_width, int drawable_height, int game_width, int game_height, int scale_mode, int pixel_aspect);

/**
 * Window coordinates -> game coordinates. The outputs are clamped to the game area (a point in a black bar or outside the
 * window maps to the nearest edge pixel). Returns 1 if the point is on the game image, 0 if it was clamped.
 * The row rule: a window coordinate maps to the game pixel its centre falls in, which is how every renderer's nearest-neighbour
 * scaling picks the game pixel it draws (measured with `remom_video_probe --row-check`; PLAN-Aspect-Ratio.md phase 11), so what
 * is clicked is what is drawn under the cursor even where game pixels are uneven (fit modes, and 4:3 rows).
 */
int Viewport_Window_To_Game(const struct s_VIEWPORT * viewport, int window_x, int window_y, int * game_x, int * game_y);

/**
 * Game coordinates -> window coordinates of the middle of that game pixel, so that mapping the result back with
 * Viewport_Window_To_Game returns the same game pixel whenever a game pixel is at least two window units wide.
 */
void Viewport_Game_To_Window(const struct s_VIEWPORT * viewport, int game_x, int game_y, int * window_x, int * window_y);

/**
 * The game image in window coordinates: exactly the window points that Viewport_Window_To_Game() reports as on the image.
 * For confining the mouse to the image (so it can never sit in a black bar). The rect is clipped to the window.
 */
void Viewport_Image_Rect_In_Window(const struct s_VIEWPORT * viewport, int * window_x, int * window_y, int * window_width, int * window_height);

/**
 * How many drawable pixels a block of game_width x game_height game pixels covers in the rect, rounded to the nearest pixel and
 * never less than the block itself. For sizing an OS cursor to match the drawn image in every scale mode (PLAN-Fullscreen.md
 * phases 5 and 7).
 */
void Viewport_Game_Size_In_Drawable(const struct s_VIEWPORT * viewport, int game_width, int game_height, int * drawable_width, int * drawable_height);

/** A VIEWPORT_PIXEL_ASPECT_* value, or SQUARE for anything else. */
int Viewport_Pixel_Aspect_Normalize(int pixel_aspect);

/** ReMoM.ini [Video] aspect_ratio names, case-insensitive: "16:10" (SQUARE) | "4:3" (4_3). Returns 1 and sets *pixel_aspect
    if known, else 0 (unchanged). */
int Viewport_Aspect_Ratio_From_Name(const char * name, int * pixel_aspect);

/** The ReMoM.ini aspect_ratio name of a pixel aspect, for logging. */
const char * Viewport_Aspect_Ratio_Name(int pixel_aspect);

/** 1 if the two viewports have the same sizes, mode, scale and rect. */
int Viewport_Equal(const struct s_VIEWPORT * viewport_a, const struct s_VIEWPORT * viewport_b);

/** A VIEWPORT_SCALE_MODE_* value, or INTEGER for anything else. */
int Viewport_Scale_Mode_Normalize(int scale_mode);

/** ReMoM.ini names, case-insensitive: "integer" | "fit_nearest" | "fit_sharp_bilinear". Returns 1 and sets *scale_mode if
    known, else 0 (unchanged). */
int Viewport_Scale_Mode_From_Name(const char * name, int * scale_mode);

/** The ReMoM.ini name of a scale mode, for logging. */
const char * Viewport_Scale_Mode_Name(int scale_mode);

/** A VIEWPORT_WINDOW_STATE_* value, or WINDOWED for anything else. */
int Viewport_Window_State_Normalize(int window_state);

/** ReMoM.ini names, case-insensitive: "windowed" | "fullscreen". Returns 1 and sets *window_state if known, else 0 (unchanged). */
int Viewport_Window_State_From_Name(const char * name, int * window_state);

/** The ReMoM.ini name of a window state, for logging. */
const char * Viewport_Window_State_Name(int window_state);

#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_VIEWPORT_H */
