
#include "Platform.h"
#include "Platform_Perf.h"  /* CLAUDE: live frame-time readout base title */
#include "Platform_Input_Metrics.h"  /* CLAUDE: Platform-Input Layer 1 metrics */

#include "sdl2_Audio.h"

#include "../../STU/src/STU_DBG.h"
#include "../../STU/src/STU_LOG.h"

#include "sdl2_KD.h"
#include "sdl2_MOM.h"
#include "sdl2_PFL.h"

#include <SDL.h>
#ifndef NO_SOUND_LIBRARY
#include <SDL_mixer.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>  /* random(), getenv() */
#include <string.h>  /* CLAUDE: strcmp() for the metrics gate */

/*
    Platform-Layer Screen Buffer / Window Surface
*/

/*
    IBM-PC VGA DAC
    required for color-cycling - change what is displayed, without changing what is drawn

*/
// MOM_PFL.C  uint8_t platform_palette_buffer[1024];  // Platform's Shadow Palette: 256 colors * bits/bytes/components per color as required by the platform  e.g., RGB, XBGR, RGBA



/*
    English     Latin
    canvas      carbasa
    paint       pingere

*/

int16_t platform_start_id;

// make Valgrind says something else, maybe
void Platform_Report_Startup_Platform(void)
{
#ifdef _WIN32
    platform_start_id = rand();
#else
    platform_start_id = random();
#endif
    LOG_INFO(LOG_CAT_SDL2_INIT, "ReMoM: Starting up platform layer...  (%d)", platform_start_id);
}


/* CLAUDE: bridge SDL's own log stream into STU_LOG so SDL/X11/render internals land
   in the same file as everything else.  Installed before SDL_Init; priority stays at
   SDL's default (errors only) unless REMOM_SDL_LOG is set, so normal runs are quiet. */
static void sdl2_Log_SDL_Output(void * userdata, int category, SDL_LogPriority priority, const char * message)
{
    (void)userdata;
    (void)category;
    if(priority >= SDL_LOG_PRIORITY_ERROR)
    {
        LOG_ERROR(LOG_CAT_SDL2_INIT, "SDL: %s", message);
    }
    else if(priority >= SDL_LOG_PRIORITY_WARN)
    {
        LOG_WARN(LOG_CAT_SDL2_INIT, "SDL: %s", message);
    }
    else
    {
        LOG_INFO(LOG_CAT_SDL2_INIT, "SDL: %s", message);
    }
}

/* CLAUDE: render a renderer's capability flags into buf as a human-readable list. */
static void sdl2_Describe_Renderer_Flags(Uint32 flags, char * buf, size_t cap)
{
    snprintf(buf, cap, "%s%s%s%s",
        (flags & SDL_RENDERER_SOFTWARE)      ? "software "      : "",
        (flags & SDL_RENDERER_ACCELERATED)   ? "accelerated "   : "",
        (flags & SDL_RENDERER_PRESENTVSYNC)  ? "vsync "         : "",
        (flags & SDL_RENDERER_TARGETTEXTURE) ? "target-texture" : "");
}

/* CLAUDE: one-shot dump of the graphics environment -- SDL version, the env vars that
   steer driver selection, and every video/render driver SDL found.  This is the record
   that turns a "crashes on startup" report into a diagnosable one, so we emit it before
   the window/renderer are created (the risky calls).  Costs a dozen log lines, once. */
static void sdl2_Log_Graphics_Environment(void)
{
    static const char * const env_keys[] = {
        "SDL_VIDEODRIVER", "SDL_RENDER_DRIVER", "SDL_RENDER_VSYNC",
        "XDG_SESSION_TYPE", "WAYLAND_DISPLAY", "DISPLAY", "XDG_RUNTIME_DIR",
        "LIBGL_ALWAYS_SOFTWARE", "__GLX_VENDOR_LIBRARY_NAME",
        "MESA_LOADER_DRIVER_OVERRIDE", "__NV_PRIME_RENDER_OFFLOAD", "DRI_PRIME"
    };
    SDL_version compiled;
    SDL_version linked;
    int i;
    int n;

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);
    LOG_INFO(LOG_CAT_SDL2_INIT, "graphics: SDL compiled %d.%d.%d, linked %d.%d.%d",
        compiled.major, compiled.minor, compiled.patch,
        linked.major, linked.minor, linked.patch);

    for(i = 0; i < (int)(sizeof(env_keys) / sizeof(env_keys[0])); i++)
    {
        const char * v = getenv(env_keys[i]);
        if(v != NULL && v[0] != '\0')
        {
            LOG_INFO(LOG_CAT_SDL2_INIT, "graphics: env %s=%s", env_keys[i], v);
        }
    }

    n = SDL_GetNumVideoDrivers();
    LOG_INFO(LOG_CAT_SDL2_INIT, "graphics: current video driver = %s (of %d available)",
        SDL_GetCurrentVideoDriver() ? SDL_GetCurrentVideoDriver() : "(none)", n);
    for(i = 0; i < n; i++)
    {
        LOG_INFO(LOG_CAT_SDL2_INIT, "graphics:   video driver [%d] = %s", i, SDL_GetVideoDriver(i));
    }

    /* The render-driver list is the crux: index -1 picks the FIRST here, and on X11
       that is "opengl" -> glXCreateContext.  Over XWayland that GLX call can hard-fail
       with a fatal Xlib BadValue, killing the process INSIDE SDL_CreateRenderer before
       it can return NULL.  Seeing this list plus which driver is first tells us whether
       a run is about to walk into that.  Workarounds: SDL_RENDER_DRIVER=software (skip
       GL) or SDL_VIDEODRIVER=wayland (EGL instead of GLX).  See doc/@Devel. */
    n = SDL_GetNumRenderDrivers();
    LOG_INFO(LOG_CAT_SDL2_INIT, "graphics: %d render drivers available (index -1 picks the first)", n);
    for(i = 0; i < n; i++)
    {
        SDL_RendererInfo info;
        if(SDL_GetRenderDriverInfo(i, &info) == 0)
        {
            char flags[64];
            sdl2_Describe_Renderer_Flags(info.flags, flags, sizeof(flags));
            LOG_INFO(LOG_CAT_SDL2_INIT, "graphics:   render driver [%d] = %-11s [%s]", i, info.name, flags);
        }
    }

    /* Each display's desktop mode -- the REFRESH RATE is the number that matters for
       "choppy framerate" reports.  ReMoM presents through a vsync'd renderer, so a
       high-refresh or VRR (G-Sync/FreeSync) panel paces frames differently than a plain
       60 Hz one.  tools/remom_video_probe --timing measures the resulting jitter.  See
       doc/@Devel/Devel-Graphics.md. */
    n = SDL_GetNumVideoDisplays();
    for(i = 0; i < n; i++)
    {
        SDL_DisplayMode mode;
        if(SDL_GetDesktopDisplayMode(i, &mode) == 0)
        {
            const char * dname = SDL_GetDisplayName(i);
            LOG_INFO(LOG_CAT_SDL2_INIT, "graphics:   display [%d] %s = %dx%d @ %d Hz",
                i, dname ? dname : "(unnamed)", mode.w, mode.h, mode.refresh_rate);
        }
    }
}

/* CLAUDE: create the renderer defensively.  Try the requested/default driver, but log
   the intent and FLUSH first, so if the GL path hard-exits via a fatal Xlib error the
   log still ends with the breadcrumb.  On a SOFT failure (NULL return) fall back to the
   software renderer, which never touches GLX.  The software fallback cannot rescue the
   HARD Xlib-exit case -- only avoiding GL up front can -- but it recovers every driver
   that fails politely.  Returns NULL only if even software fails. */
static SDL_Renderer * sdl2_Create_Renderer_With_Fallback(SDL_Window * window)
{
    SDL_Renderer * renderer;

    LOG_INFO(LOG_CAT_SDL2_INIT, "graphics: creating renderer (driver index -1, vsync)...");
    LOG_INFO(LOG_CAT_SDL2_INIT, "graphics: if this is the LAST line in the log, renderer creation crashed "
                                "hard (likely GLX over XWayland) -- retry with SDL_RENDER_DRIVER=software.  "
                                "See doc/@Devel/Devel-Graphics.md");
    STU_Log_Flush_All();  /* survive a fatal Xlib exit inside the call below */

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
    {
        LOG_WARN(LOG_CAT_SDL2_INIT, "graphics: default renderer failed (%s); retrying software renderer", SDL_GetError());
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    }

    if(renderer != NULL)
    {
        SDL_RendererInfo info;
        if(SDL_GetRendererInfo(renderer, &info) == 0)
        {
            char flags[64];
            sdl2_Describe_Renderer_Flags(info.flags, flags, sizeof(flags));
            LOG_INFO(LOG_CAT_SDL2_INIT, "graphics: renderer created = %s [%s], max texture %dx%d",
                info.name, flags, info.max_texture_width, info.max_texture_height);
        }
    }
    else
    {
        LOG_ERROR(LOG_CAT_SDL2_INIT, "graphics: NO renderer could be created, not even software: %s", SDL_GetError());
    }
    return renderer;
}

void Startup_Platform(void)
{
    int w = 0;
    int h = 0;
    Uint32 sdl2_window_flags = 0;

    Platform_Report_Startup_Platform();

    /* CLAUDE: capture SDL's own diagnostics into our log before anything can fail.
       REMOM_SDL_LOG=1 raises SDL to VERBOSE; otherwise SDL's default (errors) still
       flows through, at no extra noise. */
    SDL_LogSetOutputFunction(sdl2_Log_SDL_Output, NULL);
    {
        const char * sdl_log_env = getenv("REMOM_SDL_LOG");
        if(sdl_log_env != NULL && sdl_log_env[0] != '\0' && strcmp(sdl_log_env, "0") != 0)
        {
            SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
            LOG_INFO(LOG_CAT_SDL2_INIT, "graphics: REMOM_SDL_LOG set -- SDL verbose logging enabled");
        }
    }

#ifndef NO_SOUND_LIBRARY
    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
#else
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
#endif

    sdl2_Log_Graphics_Environment();

    sdl2_ticks_startup = (uint64_t)SDL_GetTicks();  // the number of milliseconds since SDL library initialization
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_PFL, "sdl2_ticks_startup: %llu", sdl2_ticks_startup);
#endif

    sdl2_performance_counter = SDL_GetPerformanceCounter();
#ifdef STU_DEBUG
    LOG_DEBUG(LOG_CAT_PFL, "sdl2_performance_counter: %llu", sdl2_performance_counter);
#endif

    w = sdl2_window_width;
    h = sdl2_window_height;
    assert(w >= PLATFORM_SCREEN_WIDTH && "window width must be >= PLATFORM_SCREEN_WIDTH");
    assert(h >= PLATFORM_SCREEN_HEIGHT && "window height must be >= PLATFORM_SCREEN_HEIGHT");

    sdl2_window_flags = SDL_WINDOW_RESIZABLE;

    sdl2_window = SDL_CreateWindow(
        sdl2_window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, sdl2_window_flags
    );
    assert(sdl2_window != NULL);

    /* CLAUDE: register the real title so the live readout appends to it, not to a default. */
    Perf_Live_Set_Base_Title(sdl2_window_title);
    {
        int actual_w, actual_h;
        SDL_GetWindowSize(sdl2_window, &actual_w, &actual_h);
        LOG_INFO(LOG_CAT_SDL2_INIT, "CLAUDE: requested window %dx%d, got %dx%d", w, h, actual_w, actual_h);
        assert(actual_w == w && "SDL created window with wrong width");
        assert(actual_h == h && "SDL created window with wrong height");
    }

    sdl2_renderer = sdl2_Create_Renderer_With_Fallback(sdl2_window);  /* CLAUDE: logs + software fallback */
    assert(sdl2_renderer != NULL);

    // Create the 8-bit paletted and the 32-bit RGBA screenbuffer surfaces.

    sdl2_surface_RGB666 = SDL_CreateRGBSurface(0, PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT, 8, 0, 0, 0, 0);
    assert(sdl2_surface_RGB666 != NULL);
    /* SDL2 auto-creates a palette for 8-bit surfaces — no manual creation needed. */
    assert(sdl2_surface_RGB666->format->palette != NULL && "SDL2 must auto-create palette for 8-bit surface");

    SDL_FillRect(sdl2_surface_RGB666, NULL, 0);

    sdl2_surface_ARGB8888 = SDL_CreateRGBSurfaceWithFormat(0, PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT, 32, SDL_PIXELFORMAT_ARGB8888);
    assert(sdl2_surface_ARGB8888 != NULL);

    sdl2_texture = SDL_CreateTexture(sdl2_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, PLATFORM_SCREEN_WIDTH, PLATFORM_SCREEN_HEIGHT);
    assert(sdl2_texture != NULL);

    /* CLAUDE: HW-cursor prototype shows the OS cursor; the default path hides it and software-draws. */
    if(Platform_HW_Cursor_Active())
    {
        SDL_ShowCursor(SDL_ENABLE);
    }
    else
    {
        SDL_ShowCursor(SDL_DISABLE);
    }

    // SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);

    Build_Key_Xlat();

#ifndef NO_SOUND_LIBRARY
    sdl2_Audio_Init();
#endif

    /* CLAUDE: Platform-Input Layer 1 -- runtime-gated input responsiveness metrics, OFF by default.
       REMOM_INPUT_METRICS=1 writes ./remom_input_metrics.fwv; =PATH writes PATH; unset/0 stays off.
       See doc/@AI_Plans/{BRA,PRD,PLAN}-Platform-Input.md. */
    {
        const char * im_env = getenv("REMOM_INPUT_METRICS");
        if(im_env != NULL && im_env[0] != '\0' && strcmp(im_env, "0") != 0)
        {
            const char * im_path = (strcmp(im_env, "1") == 0) ? "remom_input_metrics.fwv" : im_env;
            int im_scale = sdl2_window_width / PLATFORM_SCREEN_WIDTH;
            int im_refresh = 0;
            SDL_DisplayMode im_mode;
            if(im_scale < 1) { im_scale = 1; }
            if(SDL_GetWindowDisplayMode(sdl2_window, &im_mode) == 0)
            {
                im_refresh = im_mode.refresh_rate;
            }
            Input_Metrics_Init(im_path, "SDL2", SDL_GetCurrentVideoDriver(), im_scale, im_refresh);
            LOG_INFO(LOG_CAT_SDL2_INIT, "CLAUDE: input metrics ENABLED -> %s", im_path);
        }
    }

}

void Shutdown_Platform(void)
{
    Input_Metrics_Shutdown();  /* CLAUDE: flush the input-metrics .fwv (also runs via atexit) */

    SDL_ShowCursor(SDL_ENABLE);

#ifndef NO_SOUND_LIBRARY
    sdl2_Audio_Deinit();
#endif

    SDL_DestroyRenderer(sdl2_renderer);

    SDL_DestroyWindow(sdl2_window);

    SDL_Quit();
}

void Platform_Mouse_Input_Enable(void)
{
    platform_mouse_input_enabled = ST_TRUE;
}

void Platform_Mouse_Input_Disable(void)
{
    platform_mouse_input_enabled = ST_FALSE;
}
