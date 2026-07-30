/*
    remom_video_probe -- SDL2 graphics-stack probe for Linux display bugs.

    ReMoM creates its window with a plain SDL_CreateRenderer(win, -1, ...), which lets
    SDL pick the first render driver -- "opengl" on X11.  Over XWayland (e.g. Kubuntu on
    Wayland) that GL path can hit a FATAL Xlib error inside glXCreateContext:

        X Error of failed request:  BadValue ...
        Major opcode of failed request:  149 (GLX)
        Minor opcode of failed request:  3 (X_GLXCreateContext)

    Xlib's default handler prints that and calls exit(), so the whole process dies the
    instant a bad combination is tried -- you cannot "try it and recover" in one process.

    This probe sidesteps that by running EACH (video driver x render driver) attempt in a
    FORKED CHILD.  A fatal exit only takes the child down; the parent records it as a
    failure and keeps going, so on the broken machine you still get a full table of which
    combinations work.  Hand this to the reporter, run it, paste the table -- that tells
    us exactly which driver ReMoM should prefer or which env var to document.

    It also has a "choppy framerate" side (--timing): it reports each display's refresh
    rate and measures real present-interval jitter under vsync at several frame rates, to
    tell whether a gaming monitor's VRR / low-framerate-compensation is the culprit.

    Usage:
        remom_video_probe                 # enumerate, then probe the full driver matrix
        remom_video_probe --list          # just enumerate drivers, no window creation
        remom_video_probe --once VD RD    # one attempt in-process (VD/RD may be "" = default)
                                          #   -- lets SDL's real error text reach the terminal
        remom_video_probe --timing        # display refresh rates + present-cadence/jitter

    Build: UNIX + SDL2 only (needs fork()).  Dev/QA tool; not shipped in any release.
*/

#include <SDL.h>

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* Render drivers we attempt by name.  SDL only actually offers a subset per video
   driver; a name it can't provide just yields a NULL renderer (a clean FAIL), never a
   crash -- so probing a fixed superset is safe and keeps the table columns stable. */
static const char * const RENDER_CANDIDATES[] = {
    "opengl", "opengles2", "opengles", "vulkan", "software"
};
#define RENDER_CANDIDATE_COUNT ((int)(sizeof(RENDER_CANDIDATES) / sizeof(RENDER_CANDIDATES[0])))

/* Child exit codes -- the parent maps these back to a human verdict.  Anything else
   (notably Xlib's own exit(1), or a signal) is reported as a hard crash. */
#define PROBE_OK            0
#define PROBE_INIT_FAILED   2
#define PROBE_WINDOW_FAILED 3
#define PROBE_RENDER_NULL   4

/* Interesting environment that steers SDL's driver choice.  Printed once up front so a
   pasted report is self-contained. */
static void print_environment(void)
{
    static const char * const keys[] = {
        "SDL_VIDEODRIVER", "SDL_RENDER_DRIVER", "XDG_SESSION_TYPE",
        "WAYLAND_DISPLAY", "DISPLAY", "XDG_RUNTIME_DIR",
        "LIBGL_ALWAYS_SOFTWARE", "__GLX_VENDOR_LIBRARY_NAME",
        "MESA_LOADER_DRIVER_OVERRIDE", "__NV_PRIME_RENDER_OFFLOAD", "DRI_PRIME"
    };
    SDL_version linked;
    int i;

    SDL_GetVersion(&linked);
    printf("SDL linked version: %d.%d.%d\n", linked.major, linked.minor, linked.patch);
    printf("environment:\n");
    for(i = 0; i < (int)(sizeof(keys) / sizeof(keys[0])); i++)
    {
        const char * v = getenv(keys[i]);
        printf("  %-28s = %s\n", keys[i], (v && v[0]) ? v : "(unset)");
    }
    putchar('\n');
}

/* List the video drivers SDL was built with (no init, no window needed). */
static void list_video_drivers(void)
{
    int n = SDL_GetNumVideoDrivers();
    int i;
    printf("video drivers built in (%d):\n", n);
    for(i = 0; i < n; i++)
    {
        printf("  [%d] %s\n", i, SDL_GetVideoDriver(i));
    }
    putchar('\n');
}

/* Perform ONE attempt, in whatever process calls this.  Returns a PROBE_* code.  In
   --once mode this runs in the parent so SDL's error text is visible; in matrix mode it
   runs inside a fork so a fatal Xlib exit is contained. */
static int attempt(const char * video_driver, const char * render_driver, int verbose)
{
    SDL_Window * win;
    SDL_Renderer * ren;

    if(video_driver && video_driver[0])
    {
        SDL_setenv("SDL_VIDEODRIVER", video_driver, 1);
    }
    if(render_driver && render_driver[0])
    {
        SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, render_driver, SDL_HINT_OVERRIDE);
    }

    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        if(verbose) { fprintf(stderr, "SDL_Init(VIDEO) failed: %s\n", SDL_GetError()); }
        return PROBE_INIT_FAILED;
    }

    win = SDL_CreateWindow("remom_video_probe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                           320, 200, SDL_WINDOW_SHOWN);
    if(win == NULL)
    {
        if(verbose) { fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError()); }
        SDL_Quit();
        return PROBE_WINDOW_FAILED;
    }

    /* The line that can fatally exit on a broken GLX-over-XWayland stack. */
    ren = SDL_CreateRenderer(win, -1, 0);
    if(ren == NULL)
    {
        if(verbose) { fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError()); }
        SDL_DestroyWindow(win);
        SDL_Quit();
        return PROBE_RENDER_NULL;
    }

    /* Actually draw + present -- some stacks only fault on first use, not on create. */
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);

    if(verbose)
    {
        SDL_RendererInfo info;
        if(SDL_GetRendererInfo(ren, &info) == 0)
        {
            fprintf(stderr, "OK: video=%s render=%s\n",
                    SDL_GetCurrentVideoDriver() ? SDL_GetCurrentVideoDriver() : "?", info.name);
        }
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return PROBE_OK;
}

/* Translate a child's waitpid() status into a short verdict string. */
static const char * verdict(int status)
{
    if(WIFSIGNALED(status))
    {
        return "CRASH (killed by signal)";
    }
    if(WIFEXITED(status))
    {
        switch(WEXITSTATUS(status))
        {
            case PROBE_OK:            return "OK";
            case PROBE_INIT_FAILED:   return "fail: SDL_Init";
            case PROBE_WINDOW_FAILED: return "fail: CreateWindow";
            case PROBE_RENDER_NULL:   return "fail: renderer NULL";
            default:                  return "CRASH (fatal exit, e.g. Xlib error)";
        }
    }
    return "unknown";
}

/* Run one attempt in a child process so a fatal exit can't take the probe down. */
static void probe_forked(const char * video_driver, const char * render_driver)
{
    pid_t pid;

    /* Flush BEFORE fork: piped/redirected stdout is fully buffered, and a child would
       otherwise inherit -- and later re-emit -- the parent's unflushed buffer, printing
       our header twice.  (main() also sets line buffering, but this is the guarantee.) */
    fflush(stdout);

    pid = fork();
    if(pid < 0)
    {
        printf("    %-10s : fork() failed: %s\n", render_driver, strerror(errno));
        return;
    }
    if(pid == 0)
    {
        /* Child: silence SDL's stderr chatter (the parent reports the verdict), attempt,
           and _exit with the PROBE_* code.  A fatal Xlib exit()/abort() also lands here. */
        freopen("/dev/null", "w", stderr);
        _exit(attempt(video_driver, render_driver, 0));
    }
    else
    {
        int status = 0;
        waitpid(pid, &status, 0);
        printf("    render %-10s : %s\n", render_driver, verdict(status));
    }
}

static void probe_matrix(void)
{
    int nv = SDL_GetNumVideoDrivers();
    int vi;
    int ri;

    printf("probing (video driver x render driver), each in its own process:\n\n");
    for(vi = 0; vi < nv; vi++)
    {
        const char * vd = SDL_GetVideoDriver(vi);
        printf("  video %s:\n", vd);
        for(ri = 0; ri < RENDER_CANDIDATE_COUNT; ri++)
        {
            probe_forked(vd, RENDER_CANDIDATES[ri]);
        }
        putchar('\n');
    }
    printf("Legend: OK = window+renderer created and presented a frame.\n");
    printf("        CRASH = the combination faults fatally (do not use it).\n");
    printf("        fail:* = SDL declined cleanly (combination simply unavailable).\n");
}

/* ------------------------------------------------------------------------- *
 *  Display + present-timing probe (the "choppy framerate" side)              *
 *                                                                            *
 *  ReMoM's game logic is wall-clock paced (a 55 ms / 18.2 Hz DOS tick), so a *
 *  high-refresh monitor does NOT make it run fast -- reports of "choppy" are *
 *  about how frames LAND, not game speed.  Presents go through an SDL         *
 *  vsync'd renderer at an irregular, low rate.  The things that make that     *
 *  judder on a gaming monitor -- refresh rate, whether vsync is honored, and  *
 *  present-interval jitter (the LFC/VRR tell) -- are all measurable here.     *
 * ------------------------------------------------------------------------- */

/* List every display and its mode.  Refresh rate is the headline number: a 60 Hz
   panel and a 144/240 Hz gaming panel pace vsync very differently. */
static void report_displays(void)
{
    int nd;
    int i;

    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("displays: SDL_Init(VIDEO) failed: %s\n\n", SDL_GetError());
        return;
    }

    nd = SDL_GetNumVideoDisplays();
    printf("displays (%d), current video driver = %s:\n", nd,
           SDL_GetCurrentVideoDriver() ? SDL_GetCurrentVideoDriver() : "?");
    for(i = 0; i < nd; i++)
    {
        SDL_DisplayMode desktop;
        SDL_DisplayMode current;
        const char * name = SDL_GetDisplayName(i);

        printf("  [%d] %s\n", i, name ? name : "(unnamed)");
        if(SDL_GetDesktopDisplayMode(i, &desktop) == 0)
        {
            printf("      desktop mode : %dx%d @ %d Hz  (%s)\n",
                   desktop.w, desktop.h, desktop.refresh_rate,
                   SDL_GetPixelFormatName(desktop.format));
        }
        if(SDL_GetCurrentDisplayMode(i, &current) == 0 &&
           (current.w != desktop.w || current.h != desktop.h || current.refresh_rate != desktop.refresh_rate))
        {
            printf("      current mode : %dx%d @ %d Hz\n", current.w, current.h, current.refresh_rate);
        }
        printf("      modes listed : %d\n", SDL_GetNumDisplayModes(i));
    }
    printf("  note: SDL2 has no API to read VRR / G-Sync / FreeSync state -- a variable-\n");
    printf("        refresh panel reports its MAX Hz here; infer VRR from jitter below.\n\n");
    SDL_Quit();
}

/* Present `frames` frames and report the inter-present interval statistics.  vsync
   selects whether SDL_RenderSetVSync is on.  target_fps > 0 paces the loop to that rate
   (to mimic ReMoM's sparse ~18 fps output, which is what actually exercises a monitor's
   VRR / low-framerate-compensation path); target_fps <= 0 presents as fast as the stack
   allows.  Runs in its own child so a fault here can't take the probe down. */
static void measure_cadence(const char * label, int vsync, int target_fps, int frames)
{
    pid_t pid;

    fflush(stdout);
    pid = fork();
    if(pid < 0)
    {
        printf("  %-22s : fork() failed: %s\n", label, strerror(errno));
        return;
    }
    if(pid != 0)
    {
        int status = 0;
        waitpid(pid, &status, 0);
        if(!WIFEXITED(status) || WEXITSTATUS(status) != 0)
        {
            printf("  %-22s : measurement CRASHED (%s)\n", label,
                   WIFSIGNALED(status) ? "signal" : "nonzero exit");
        }
        return;
    }

    /* ---- child ---- */
    {
        SDL_Window *   win;
        SDL_Renderer * ren;
        SDL_RendererInfo info;
        Uint64 freq = SDL_GetPerformanceFrequency();
        double target_ms = (target_fps > 0) ? (1000.0 / target_fps) : 0.0;
        double sum = 0.0, sumsq = 0.0, mn = 1e30, mx = 0.0;
        int granted_vsync;
        int i;

        freopen("/dev/null", "w", stderr);  /* keep SDL chatter out of the table */

        if(SDL_Init(SDL_INIT_VIDEO) != 0) { _exit(2); }
        win = SDL_CreateWindow("remom_video_probe timing",
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               640, 400, SDL_WINDOW_SHOWN);
        if(win == NULL) { SDL_Quit(); _exit(3); }
        ren = SDL_CreateRenderer(win, -1, 0);
        if(ren == NULL) { SDL_DestroyWindow(win); SDL_Quit(); _exit(4); }

        SDL_RenderSetVSync(ren, vsync ? 1 : 0);
        granted_vsync = (SDL_GetRendererInfo(ren, &info) == 0) &&
                        (info.flags & SDL_RENDERER_PRESENTVSYNC);

        /* Warm up: the first few presents include allocation/first-flip costs. */
        for(i = 0; i < 10; i++)
        {
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
            SDL_RenderClear(ren);
            SDL_RenderPresent(ren);
        }

        for(i = 0; i < frames; i++)
        {
            Uint64 t0 = SDL_GetPerformanceCounter();
            Uint64 t1;
            double dt;

            /* Change the colour every frame so a compositor can't collapse identical
               frames and hide real present cost. */
            SDL_SetRenderDrawColor(ren, (Uint8)(i * 7), (Uint8)(i * 3), 32, 255);
            SDL_RenderClear(ren);
            SDL_RenderPresent(ren);
            t1 = SDL_GetPerformanceCounter();

            dt = (double)(t1 - t0) * 1000.0 / (double)freq;

            /* Pace to target_fps by sleeping the remainder -- models ReMoM's own
               Release_Time() wait, so the present cadence we measure is the one the
               game actually produces. */
            if(target_ms > 0.0 && dt < target_ms)
            {
                SDL_Delay((Uint32)(target_ms - dt));
                t1 = SDL_GetPerformanceCounter();
                dt = (double)(t1 - t0) * 1000.0 / (double)freq;
            }

            sum += dt;
            sumsq += dt * dt;
            if(dt < mn) { mn = dt; }
            if(dt > mx) { mx = dt; }
        }

        {
            double mean = sum / frames;
            double var = (sumsq / frames) - (mean * mean);
            double stddev = (var > 0.0) ? sqrt(var) : 0.0;
            printf("  %-22s : mean %6.2f ms  min %6.2f  max %6.2f  jitter(sd) %5.2f ms  -> %6.1f fps  [vsync %s]\n",
                   label, mean, mn, mx, stddev, 1000.0 / mean,
                   granted_vsync ? "granted" : (vsync ? "REQUESTED-not-granted" : "off"));
        }

        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        _exit(0);
    }
}

static void probe_timing(void)
{
    report_displays();

    printf("present-cadence measurements (each in its own process):\n\n");
    measure_cadence("vsync OFF, uncapped",   0,   0, 180);
    measure_cadence("vsync ON,  uncapped",   1,   0, 180);
    measure_cadence("vsync ON,  ~18 fps",    1,  18, 90);   /* ReMoM-like sparse output */
    measure_cadence("vsync ON,  ~30 fps",    1,  30, 90);
    measure_cadence("vsync ON,  ~60 fps",    1,  60, 120);
    putchar('\n');
    printf("How to read it:\n");
    printf("  - vsync ON, uncapped: mean should ~= 1000/refresh (e.g. 6.9 ms @144, 16.7 @60).\n");
    printf("    If mean is ~0 ms, vsync is NOT being honored (a driver/compositor override).\n");
    printf("  - jitter (sd) is the choppiness number.  Low = smooth.  A large max vs mean, or\n");
    printf("    high sd, at the ~18/30 fps rows is the signature of a gaming monitor's VRR /\n");
    printf("    low-framerate-compensation kicking in below its VRR floor -- the likely cause\n");
    printf("    of 'choppy on my 144Hz/G-Sync panel, fine everywhere else'.\n");
    printf("  - Compare a normal 60Hz panel run against the gaming-monitor run: the numbers\n");
    printf("    say which layer is misbehaving.\n");
}

int main(int argc, char ** argv)
{
    /* Line-buffer stdout so output interleaves sanely and, with the fflush before each
       fork, no child ever inherits unflushed parent output. */
    setvbuf(stdout, NULL, _IOLBF, 0);

    if(argc >= 2 && strcmp(argv[1], "--once") == 0)
    {
        const char * vd = (argc >= 3) ? argv[2] : "";
        const char * rd = (argc >= 4) ? argv[3] : "";
        int rc = attempt(vd, rd, 1);
        printf("result: %s\n", (rc == PROBE_OK) ? "OK" : "FAILED");
        return (rc == PROBE_OK) ? 0 : 1;
    }

    print_environment();

    if(argc >= 2 && strcmp(argv[1], "--timing") == 0)
    {
        probe_timing();
        return 0;
    }

    list_video_drivers();

    if(argc >= 2 && strcmp(argv[1], "--list") == 0)
    {
        return 0;
    }

    probe_matrix();
    return 0;
}
