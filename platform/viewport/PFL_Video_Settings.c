/**
 * PFL_Video_Settings.c — the [Video] settings (ReMoM.ini) the windowed backend starts with.
 *
 * ReMoM reads ReMoM.ini and calls Platform_Set_Video_Settings() before Startup_Platform(); the backend reads them back.
 * Kept out of the backends so every windowed backend shares one copy. See doc/@AI_Plans/PLAN-Fullscreen.md, Phase 3.
 */

#include "../include/Platform.h"
#include "../include/Platform_Viewport.h"



static int platform_video_startup_window = VIEWPORT_WINDOW_STATE_FULLSCREEN;  /* [Video] startup_window = fullscreen */
static int platform_video_scale_mode = VIEWPORT_SCALE_MODE_INTEGER;  /* [Video] scale_mode = integer */
static int platform_video_pixel_aspect = VIEWPORT_PIXEL_ASPECT_SQUARE;  /* [Video] aspect_ratio = 16:10 (PLAN-Aspect-Ratio.md) */

void Platform_Set_Video_Settings(int startup_window, int scale_mode, int pixel_aspect)
{
    platform_video_startup_window = Viewport_Window_State_Normalize(startup_window);
    platform_video_scale_mode = Viewport_Scale_Mode_Normalize(scale_mode);
    platform_video_pixel_aspect = Viewport_Pixel_Aspect_Normalize(pixel_aspect);
}

int Platform_Video_Startup_Window(void)
{
    return platform_video_startup_window;
}

int Platform_Video_Scale_Mode(void)
{
    return platform_video_scale_mode;
}

int Platform_Video_Pixel_Aspect(void)
{
    return platform_video_pixel_aspect;
}
