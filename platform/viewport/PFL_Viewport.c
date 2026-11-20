/**
 * PFL_Viewport.c — where the game image goes in a window, and the window <-> game coordinate mapping.
 *
 * See Platform_Viewport.h. Pure C: no SDL, no OS calls, no globals, so it is unit-tested directly (platform/tests).
 */

#include "../include/Platform_Viewport.h"

#include <stddef.h>  /* NULL */
#include <stdint.h>



/* Division rounding toward negative infinity (C rounds toward zero). divisor must be > 0. */
static int64_t Viewport_Floor_Divide(int64_t dividend, int64_t divisor)
{
    int64_t quotient = dividend / divisor;
    if((dividend % divisor != 0) && (dividend < 0))
    {
        quotient--;
    }
    return quotient;
}

static int Viewport_At_Least_One(int value)
{
    return (value < 1) ? 1 : value;
}

static int64_t Viewport_Clamp(int64_t value, int64_t minimum, int64_t maximum)
{
    if(value < minimum)
    {
        return minimum;
    }
    if(value > maximum)
    {
        return maximum;
    }
    return value;
}

void Viewport_Compute(struct s_VIEWPORT * viewport, int window_width, int window_height, int drawable_width, int drawable_height, int game_width, int game_height, int scale_mode, int pixel_aspect)
{
    int scale_x = 0;
    int scale_y = 0;
    int scale = 0;
    int shape_width = 0;
    int shape_height = 0;

    viewport->window_width = Viewport_At_Least_One(window_width);
    viewport->window_height = Viewport_At_Least_One(window_height);
    viewport->drawable_width = Viewport_At_Least_One(drawable_width);
    viewport->drawable_height = Viewport_At_Least_One(drawable_height);
    viewport->game_width = Viewport_At_Least_One(game_width);
    viewport->game_height = Viewport_At_Least_One(game_height);
    viewport->scale_mode = Viewport_Scale_Mode_Normalize(scale_mode);
    viewport->pixel_aspect = Viewport_Pixel_Aspect_Normalize(pixel_aspect);

    /* The shape the whole image is shown at, as shape_width : shape_height. SQUARE: the game's own 320:200 (16:10). 4_3: 4:3,
       as on a 1994 CRT -- the game's width by three quarters of it (320:240), so each game pixel is 1.2 times taller than wide. */
    shape_width = viewport->game_width;
    shape_height = viewport->game_height;
    if(viewport->pixel_aspect == VIEWPORT_PIXEL_ASPECT_4_3)
    {
        shape_height = Viewport_At_Least_One((viewport->game_width * 3) / 4);
    }

    if((viewport->scale_mode == VIEWPORT_SCALE_MODE_FIT_NEAREST) || (viewport->scale_mode == VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR))
    {
        /* FIT_*: as large as the drawable allows at the shape -- the full width, or the full height if the full width would be
           too tall. Non-integer. FIT_NEAREST samples nearest-neighbour; FIT_SHARP_BILINEAR goes through `scale`. */
        viewport->rect_width = viewport->drawable_width;
        viewport->rect_height = (int)(((int64_t)viewport->drawable_width * shape_height) / shape_width);
        if(viewport->rect_height > viewport->drawable_height)
        {
            viewport->rect_height = viewport->drawable_height;
            viewport->rect_width = (int)(((int64_t)viewport->drawable_height * shape_width) / shape_height);
        }
        viewport->rect_width = Viewport_At_Least_One(viewport->rect_width);
        viewport->rect_height = Viewport_At_Least_One(viewport->rect_height);
    }
    else
    {
        /* INTEGER: the largest whole-number multiple of the shape that fits, never below 1. SQUARE: game_width x game_height
           times it, every game pixel the same size. 4_3: exactly 4:3, so the 200 game rows spread over 240k drawable rows and
           are uneven except when k is a multiple of 5; the columns are always even. */
        scale_x = viewport->drawable_width / shape_width;
        scale_y = viewport->drawable_height / shape_height;
        scale = (scale_x < scale_y) ? scale_x : scale_y;
        scale = Viewport_At_Least_One(scale);
        viewport->rect_width = shape_width * scale;
        viewport->rect_height = shape_height * scale;
    }

    /* The largest whole-number scale at which the game image fits inside the rect, never below 1: the whole-number pre-scale the
       sharp-bilinear intermediate is drawn at (one number for both axes; the final filter stretches it to the rect). For SQUARE
       it is INTEGER's scale, and for the fit modes the same as fitting the game in the drawable, since the rect fills the
       limiting axis. */
    scale_x = viewport->rect_width / viewport->game_width;
    scale_y = viewport->rect_height / viewport->game_height;
    scale = (scale_x < scale_y) ? scale_x : scale_y;
    viewport->scale = Viewport_At_Least_One(scale);
    viewport->rect_x = (int)Viewport_Floor_Divide((int64_t)viewport->drawable_width - viewport->rect_width, 2);
    viewport->rect_y = (int)Viewport_Floor_Divide((int64_t)viewport->drawable_height - viewport->rect_height, 2);
}

/*
 * The row rule (PLAN-Aspect-Ratio.md phase 11): a window coordinate w stands for its centre, w + 1/2, which is drawable position
 * p = (w + 1/2) * drawable / window, and it maps to the game pixel p falls in:
 *     floor((p - rect_x) * game / rect_width)  =  floor(((2w + 1) * drawable - 2 * rect_x * window) * game / (2 * rect_width * window))
 * computed exactly in 64-bit integers, so nothing is lost to rounding window -> pixel first. With window = drawable this is the
 * game pixel under the drawable pixel's centre: what every renderer's nearest-neighbour scaling draws there (remom_video_probe
 * --row-check). A pixel centre exactly on a boundary between two game pixels goes to the later one, as Direct3D 11 does; other
 * renderers break that tie either way, so there the click can be one game pixel off for that one drawable pixel.
 */
int Viewport_Window_To_Game(const struct s_VIEWPORT * viewport, int window_x, int window_y, int * game_x, int * game_y)
{
    int64_t numerator_x = (((int64_t)2 * window_x + 1) * viewport->drawable_width - (int64_t)2 * viewport->rect_x * viewport->window_width) * viewport->game_width;
    int64_t numerator_y = (((int64_t)2 * window_y + 1) * viewport->drawable_height - (int64_t)2 * viewport->rect_y * viewport->window_height) * viewport->game_height;
    int64_t unclamped_x = Viewport_Floor_Divide(numerator_x, (int64_t)2 * viewport->rect_width * viewport->window_width);
    int64_t unclamped_y = Viewport_Floor_Divide(numerator_y, (int64_t)2 * viewport->rect_height * viewport->window_height);
    int on_image = ((unclamped_x >= 0) && (unclamped_x < viewport->game_width) && (unclamped_y >= 0) && (unclamped_y < viewport->game_height));

    *game_x = (int)Viewport_Clamp(unclamped_x, 0, viewport->game_width - 1);
    *game_y = (int)Viewport_Clamp(unclamped_y, 0, viewport->game_height - 1);
    return on_image;
}

/* Ceiling division (divisor must be > 0). */
static int64_t Viewport_Ceiling_Divide(int64_t dividend, int64_t divisor)
{
    return -Viewport_Floor_Divide(-dividend, divisor);
}

/*
 * Game pixel g covers drawable [rect + g * size / game, rect + (g + 1) * size / game). By the row rule (Viewport_Window_To_Game)
 * window coordinate w is on it when its centre is:
 *     2 * (rect * game + g * size) * window  <=  (2w + 1) * drawable * game  <  2 * (rect * game + (g + 1) * size) * window
 * With D = drawable * game, the first such w is the ceiling of (left side - D) / 2D, and the last is the ceiling of
 * (right side - D) / 2D, minus 1. The answer is the middle of those, so it maps back to g whenever g has a window coordinate of
 * its own, at any scale, integer or not. (Taking the pixel's exact centre and rounding down does not: at a non-integer scale
 * under 2 it can land on the pixel before.)
 */
static int Viewport_Game_To_Window_Axis(int game, int rect_start, int rect_size, int game_size, int window_size, int drawable_size)
{
    int64_t divisor = (int64_t)drawable_size * game_size;
    int64_t first = Viewport_Ceiling_Divide((int64_t)2 * ((int64_t)rect_start * game_size + (int64_t)game * rect_size) * window_size - divisor, 2 * divisor);
    int64_t last = Viewport_Ceiling_Divide((int64_t)2 * ((int64_t)rect_start * game_size + ((int64_t)game + 1) * rect_size) * window_size - divisor, 2 * divisor) - 1;
    if(last < first)
    {
        return (int)first;  /* the game pixel is narrower than one window unit: no window coordinate is only on it */
    }
    return (int)Viewport_Floor_Divide(first + last, 2);
}

void Viewport_Game_To_Window(const struct s_VIEWPORT * viewport, int game_x, int game_y, int * window_x, int * window_y)
{
    *window_x = Viewport_Game_To_Window_Axis(game_x, viewport->rect_x, viewport->rect_width, viewport->game_width, viewport->window_width, viewport->drawable_width);
    *window_y = Viewport_Game_To_Window_Axis(game_y, viewport->rect_y, viewport->rect_height, viewport->game_height, viewport->window_height, viewport->drawable_height);
}

/*
 * By the row rule, window coordinate w is on the image when 2 * rect_start * window <= (2w + 1) * drawable < 2 * rect_end * window
 * (see Viewport_Window_To_Game), so the first such w is ceil((2 * rect_start * window - drawable) / (2 * drawable)) and the last is
 * ceil((2 * rect_end * window - drawable) / (2 * drawable)) - 1.
 */
static int Viewport_First_Window_At(int64_t drawable_position, int window_size, int drawable_size)
{
    return (int)Viewport_Ceiling_Divide((int64_t)2 * drawable_position * window_size - drawable_size, (int64_t)2 * drawable_size);
}

void Viewport_Image_Rect_In_Window(const struct s_VIEWPORT * viewport, int * window_x, int * window_y, int * window_width, int * window_height)
{
    int first_x = Viewport_First_Window_At(viewport->rect_x, viewport->window_width, viewport->drawable_width);
    int first_y = Viewport_First_Window_At(viewport->rect_y, viewport->window_height, viewport->drawable_height);
    int last_x = Viewport_First_Window_At((int64_t)viewport->rect_x + viewport->rect_width, viewport->window_width, viewport->drawable_width) - 1;
    int last_y = Viewport_First_Window_At((int64_t)viewport->rect_y + viewport->rect_height, viewport->window_height, viewport->drawable_height) - 1;

    first_x = (int)Viewport_Clamp(first_x, 0, viewport->window_width - 1);
    first_y = (int)Viewport_Clamp(first_y, 0, viewport->window_height - 1);
    last_x = (int)Viewport_Clamp(last_x, first_x, viewport->window_width - 1);
    last_y = (int)Viewport_Clamp(last_y, first_y, viewport->window_height - 1);

    *window_x = first_x;
    *window_y = first_y;
    *window_width = last_x - first_x + 1;
    *window_height = last_y - first_y + 1;
}

int Viewport_Equal(const struct s_VIEWPORT * viewport_a, const struct s_VIEWPORT * viewport_b)
{
    return (
        (viewport_a->window_width == viewport_b->window_width) &&
        (viewport_a->window_height == viewport_b->window_height) &&
        (viewport_a->drawable_width == viewport_b->drawable_width) &&
        (viewport_a->drawable_height == viewport_b->drawable_height) &&
        (viewport_a->game_width == viewport_b->game_width) &&
        (viewport_a->game_height == viewport_b->game_height) &&
        (viewport_a->scale_mode == viewport_b->scale_mode) &&
        (viewport_a->pixel_aspect == viewport_b->pixel_aspect) &&
        (viewport_a->scale == viewport_b->scale) &&
        (viewport_a->rect_x == viewport_b->rect_x) &&
        (viewport_a->rect_y == viewport_b->rect_y) &&
        (viewport_a->rect_width == viewport_b->rect_width) &&
        (viewport_a->rect_height == viewport_b->rect_height)
    );
}

static int Viewport_Lower_Case(int character)
{
    return ((character >= 'A') && (character <= 'Z')) ? (character - 'A' + 'a') : character;
}

/* Case-insensitive equality, independent of locale. */
static int Viewport_Name_Equals(const char * name, const char * expected)
{
    while((*name != '\0') && (*expected != '\0'))
    {
        if(Viewport_Lower_Case((unsigned char)*name) != Viewport_Lower_Case((unsigned char)*expected))
        {
            return 0;
        }
        name++;
        expected++;
    }
    return ((*name == '\0') && (*expected == '\0'));
}

int Viewport_Scale_Mode_From_Name(const char * name, int * scale_mode)
{
    if(name == NULL)
    {
        return 0;
    }
    if(Viewport_Name_Equals(name, "integer"))
    {
        *scale_mode = VIEWPORT_SCALE_MODE_INTEGER;
        return 1;
    }
    if(Viewport_Name_Equals(name, "fit_nearest"))
    {
        *scale_mode = VIEWPORT_SCALE_MODE_FIT_NEAREST;
        return 1;
    }
    if(Viewport_Name_Equals(name, "fit_sharp_bilinear"))
    {
        *scale_mode = VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR;
        return 1;
    }
    return 0;
}

const char * Viewport_Scale_Mode_Name(int scale_mode)
{
    switch(Viewport_Scale_Mode_Normalize(scale_mode))
    {
        case VIEWPORT_SCALE_MODE_FIT_NEAREST:
            return "fit_nearest";
        case VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR:
            return "fit_sharp_bilinear";
        default:
            return "integer";
    }
}

int Viewport_Scale_Mode_Normalize(int scale_mode)
{
    if((scale_mode == VIEWPORT_SCALE_MODE_FIT_NEAREST) || (scale_mode == VIEWPORT_SCALE_MODE_FIT_SHARP_BILINEAR))
    {
        return scale_mode;
    }
    return VIEWPORT_SCALE_MODE_INTEGER;
}

int Viewport_Pixel_Aspect_Normalize(int pixel_aspect)
{
    if(pixel_aspect == VIEWPORT_PIXEL_ASPECT_4_3)
    {
        return pixel_aspect;
    }
    return VIEWPORT_PIXEL_ASPECT_SQUARE;
}

int Viewport_Aspect_Ratio_From_Name(const char * name, int * pixel_aspect)
{
    if(name == NULL)
    {
        return 0;
    }
    if(Viewport_Name_Equals(name, "16:10"))
    {
        *pixel_aspect = VIEWPORT_PIXEL_ASPECT_SQUARE;
        return 1;
    }
    if(Viewport_Name_Equals(name, "4:3"))
    {
        *pixel_aspect = VIEWPORT_PIXEL_ASPECT_4_3;
        return 1;
    }
    return 0;
}

const char * Viewport_Aspect_Ratio_Name(int pixel_aspect)
{
    switch(Viewport_Pixel_Aspect_Normalize(pixel_aspect))
    {
        case VIEWPORT_PIXEL_ASPECT_4_3:
            return "4:3";
        default:
            return "16:10";
    }
}

void Viewport_Game_Size_In_Drawable(const struct s_VIEWPORT * viewport, int game_width, int game_height, int * drawable_width, int * drawable_height)
{
    int64_t width = ((int64_t)game_width * viewport->rect_width + (viewport->game_width / 2)) / viewport->game_width;
    int64_t height = ((int64_t)game_height * viewport->rect_height + (viewport->game_height / 2)) / viewport->game_height;
    if(width < game_width)
    {
        width = game_width;
    }
    if(height < game_height)
    {
        height = game_height;
    }
    *drawable_width = (int)width;
    *drawable_height = (int)height;
}

int Viewport_Window_State_Normalize(int window_state)
{
    if((window_state == VIEWPORT_WINDOW_STATE_BORDERLESS) || (window_state == VIEWPORT_WINDOW_STATE_FULLSCREEN))
    {
        return window_state;
    }
    return VIEWPORT_WINDOW_STATE_WINDOWED;
}

int Viewport_Window_State_From_Name(const char * name, int * window_state)
{
    if(name == NULL)
    {
        return 0;
    }
    if(Viewport_Name_Equals(name, "windowed"))
    {
        *window_state = VIEWPORT_WINDOW_STATE_WINDOWED;
        return 1;
    }
    if(Viewport_Name_Equals(name, "borderless"))
    {
        *window_state = VIEWPORT_WINDOW_STATE_BORDERLESS;
        return 1;
    }
    if(Viewport_Name_Equals(name, "fullscreen"))
    {
        *window_state = VIEWPORT_WINDOW_STATE_FULLSCREEN;
        return 1;
    }
    return 0;
}

const char * Viewport_Window_State_Name(int window_state)
{
    switch(Viewport_Window_State_Normalize(window_state))
    {
        case VIEWPORT_WINDOW_STATE_BORDERLESS:
            return "borderless";
        case VIEWPORT_WINDOW_STATE_FULLSCREEN:
            return "fullscreen";
        default:
            return "windowed";
    }
}
