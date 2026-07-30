/*
    remom_video_probe -- SDL2 graphics-stack probe for display bugs (Linux and Windows).

    ReMoM creates its window with a plain SDL_CreateRenderer(win, -1, ...), which lets
    SDL pick the first render driver.  That one line is where a whole family of "won't
    start" reports land, and the failure is platform-specific:

      Linux/X11 -- the first driver is "opengl" -> glXCreateContext.  Over XWayland
      (e.g. Kubuntu on Wayland) that can hit a FATAL Xlib error:

        X Error of failed request:  BadValue ...
        Major opcode of failed request:  149 (GLX)
        Minor opcode of failed request:  3 (X_GLXCreateContext)

      Xlib's default handler prints that and calls exit(), so the whole process dies the
      instant a bad combination is tried -- you cannot "try it and recover" in one process.

      Windows -- the first driver is "direct3d11" (or "direct3d").  A stale/broken GPU ICD,
      a Remote Desktop session, or a VM without 3D can fault INSIDE the driver DLL, which
      raises an unhandled SEH exception rather than returning NULL.

    This probe sidesteps both by running EACH (video driver x render driver) attempt in a
    SEPARATE PROCESS.  A fatal exit only takes the child down; the parent records it as a
    failure and keeps going, so on the broken machine you still get a full table of which
    combinations work.  Hand this to the reporter, run it, paste the table -- that tells
    us exactly which driver ReMoM should prefer or which env var to document.

      - UNIX gets that isolation with fork(): the child inherits everything and simply
        _exit()s with a PROBE_* code.
      - Windows has no fork(), so the parent re-spawns ITSELF with "--child VD RD" and
        reads the child's exit code.  A crashed child exits with its exception code
        (0xC0000005 and friends), which the parent reports as a CRASH.  The child also
        has to suppress every modal error dialog Windows would otherwise pop -- a Windows
        Error Reporting box on a tester's machine would hang the probe forever waiting
        for a click.  See probe_Harden_Child().

    It also has a "choppy framerate" side (--timing): it reports each display's refresh
    rate and measures real present-interval jitter under vsync at several frame rates, to
    tell whether a gaming monitor's VRR / low-framerate-compensation is the culprit.

    Usage:
        remom_video_probe                 # enumerate, then probe the full driver matrix
        remom_video_probe --list          # just enumerate drivers, no window creation
        remom_video_probe --once VD RD    # one attempt in-process (VD/RD may be "" = default)
                                          #   -- lets SDL's real error text reach the terminal
        remom_video_probe --timing        # display refresh rates + present-cadence/jitter

    (--child and --child-timing are internal re-entry points used by the Windows spawn
    path; they are not meant to be typed by hand.)

    Build: SDL2, UNIX or Windows.  Dev/QA + support tool.
*/

/* SDL2 on Windows #defines main -> SDL_main unless we claim the entry point.  This is a
   plain console tool that does not link SDL2main, so claim it and tell SDL we did. */
#define SDL_MAIN_HANDLED

#include <SDL.h>

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define COBJMACROS                  /* lets us call the DXGI COM vtables from C */
    #include <windows.h>
    #include <dxgi.h>
    #include <process.h>                /* _spawnv, _P_WAIT */
    #include <crtdbg.h>                 /* _CrtSetReportMode -- silence debug-CRT assert boxes */
    #include <mmsystem.h>               /* timeBeginPeriod -- see measure_cadence() */
    #define PROBE_NULL_DEVICE "NUL"
#else
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <unistd.h>
    #define PROBE_NULL_DEVICE "/dev/null"
#endif

/* Render drivers we attempt by name.  SDL only actually offers a subset per video
   driver; a name it can't provide just yields a NULL renderer (a clean FAIL), never a
   crash -- so probing a fixed superset is safe and keeps the table columns stable.
   The lists are per-platform because the interesting axis differs: on Linux the video
   driver (x11 vs wayland) is what varies, while on Windows there is effectively only
   "windows", and it is the D3D-vs-GL-vs-software choice that decides whether a machine
   starts at all. */
#ifdef _WIN32
static const char * const RENDER_CANDIDATES[] = {
    "direct3d11", "direct3d12", "direct3d", "opengl", "opengles2", "software"
};
#else
static const char * const RENDER_CANDIDATES[] = {
    "opengl", "opengles2", "opengles", "vulkan", "software"
};
#endif
#define RENDER_CANDIDATE_COUNT ((int)(sizeof(RENDER_CANDIDATES) / sizeof(RENDER_CANDIDATES[0])))

/* Child exit codes -- the parent maps these back to a human verdict.  Anything else
   (notably Xlib's own exit(1), a signal, or a Windows exception code) is reported as a
   hard crash.  Keep these small and away from the 0xC0000000 exception range. */
#define PROBE_OK            0
#define PROBE_INIT_FAILED   2
#define PROBE_WINDOW_FAILED 3
#define PROBE_RENDER_NULL   4

/* The parent passes driver names to a spawned child as argv.  An empty name means "let
   SDL choose", which cannot survive a command line intact on Windows, so it travels as
   this sentinel.  probe_Decode_Driver_Arg() turns it back into "". */
#define PROBE_DEFAULT_DRIVER_ARG "-"

static const char * probe_Decode_Driver_Arg(const char * arg)
{
    if(arg == NULL || strcmp(arg, PROBE_DEFAULT_DRIVER_ARG) == 0) { return ""; }
    return arg;
}

#ifdef _WIN32
/* Only the Windows path encodes -- UNIX passes driver names to the fork child in memory,
   so there is no command line for an empty string to get lost on. */
static const char * probe_Encode_Driver_Arg(const char * name)
{
    if(name == NULL || name[0] == '\0') { return PROBE_DEFAULT_DRIVER_ARG; }
    return name;
}
#endif


/* ------------------------------------------------------------------------- *
 *  Child hardening                                                           *
 *                                                                            *
 *  The whole point of the per-attempt child is that a fault is CONTAINED and *
 *  REPORTED.  On Windows the default fault behaviour is neither: an access   *
 *  violation inside a GPU driver pops a Windows Error Reporting dialog, and  *
 *  a debug-CRT assert pops its own box.  Both block on a mouse click that no *
 *  tester is watching for -- the probe would hang, not fail.  Kill every     *
 *  dialog path so the child dies quietly with a readable exit code.          *
 * ------------------------------------------------------------------------- */
static void probe_Harden_Child(void)
{
    /* Keep SDL's chatter out of the table; the parent reports the verdict. */
    freopen(PROBE_NULL_DEVICE, "w", stderr);

#ifdef _WIN32
    /* No "app has stopped working" box, no missing-DLL box, no drive-not-ready box.
       SEM_FAILCRITICALERRORS matters here because a broken ICD often manifests as a
       failed LoadLibrary of a driver DLL, which is otherwise a modal error. */
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX | SEM_NOALIGNMENTFAULTEXCEPT);

    /* abort() must not report to WER or print to stderr; just set the exit code. */
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);

    /* Debug-CRT assertion dialogs (only present in a Debug build, but a Debug build of
       the probe is exactly what a developer runs while chasing this). */
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif
}


/* Interesting environment that steers SDL's driver choice.  Printed once up front so a
   pasted report is self-contained.  The Linux list is dominated by X11/Mesa knobs; on
   Windows almost none of those exist and the equivalent facts (which GPU, are we inside
   Remote Desktop) are not environment variables at all -- see print_windows_graphics(). */
static void print_environment(void)
{
    static const char * const keys[] = {
#ifdef _WIN32
        "SDL_VIDEODRIVER", "SDL_RENDER_DRIVER", "SDL_RENDER_VSYNC",
        "SDL_RENDER_SCALE_QUALITY", "SDL_WINDOWS_DPI_AWARENESS", "SDL_FRAMEBUFFER_ACCELERATION"
#else
        "SDL_VIDEODRIVER", "SDL_RENDER_DRIVER", "XDG_SESSION_TYPE",
        "WAYLAND_DISPLAY", "DISPLAY", "XDG_RUNTIME_DIR",
        "LIBGL_ALWAYS_SOFTWARE", "__GLX_VENDOR_LIBRARY_NAME",
        "MESA_LOADER_DRIVER_OVERRIDE", "__NV_PRIME_RENDER_OFFLOAD", "DRI_PRIME"
#endif
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


#ifdef _WIN32
/* The real OS build number.  GetVersionEx() lies to unmanifested processes (it caps at
   6.2 / Windows 8), so go straight to the kernel's own RtlGetVersion, which does not. */
static void print_windows_version(void)
{
    typedef LONG (WINAPI * RtlGetVersion_fn)(PRTL_OSVERSIONINFOW);
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    RtlGetVersion_fn get_version = NULL;
    RTL_OSVERSIONINFOW vi;

    if(ntdll != NULL)
    {
        get_version = (RtlGetVersion_fn)(void *)GetProcAddress(ntdll, "RtlGetVersion");
    }
    if(get_version == NULL)
    {
        printf("  windows version             = (RtlGetVersion unavailable)\n");
        return;
    }

    memset(&vi, 0, sizeof(vi));
    vi.dwOSVersionInfoSize = sizeof(vi);
    if(get_version(&vi) == 0)
    {
        printf("  windows version             = %lu.%lu build %lu\n",
               (unsigned long)vi.dwMajorVersion, (unsigned long)vi.dwMinorVersion, (unsigned long)vi.dwBuildNumber);
    }
}

/* Enumerate the graphics adapters via DXGI.  SDL2 exposes no adapter information at all,
   and this is the single most useful fact in a Windows "won't start" report: it names the
   GPU whose driver is faulting, and it exposes the WARP/software adapter (the tell for a
   VM or a machine with no usable 3D at all).  DXGI is present on every supported Windows;
   a failure here is reported and skipped, never fatal. */
static void print_windows_adapters(void)
{
    IDXGIFactory1 * factory = NULL;
    HRESULT hr;
    UINT index;

    hr = CreateDXGIFactory1(&IID_IDXGIFactory1, (void **)&factory);
    if(FAILED(hr) || factory == NULL)
    {
        printf("  graphics adapters           = (DXGI unavailable, hr=0x%08lX)\n", (unsigned long)hr);
        return;
    }

    printf("  graphics adapters:\n");
    for(index = 0; ; index++)
    {
        IDXGIAdapter1 *    adapter = NULL;
        DXGI_ADAPTER_DESC1 desc;
        char               name[256];

        if(IDXGIFactory1_EnumAdapters1(factory, index, &adapter) == DXGI_ERROR_NOT_FOUND) { break; }
        if(adapter == NULL) { break; }

        if(SUCCEEDED(IDXGIAdapter1_GetDesc1(adapter, &desc)))
        {
            if(WideCharToMultiByte(CP_UTF8, 0, desc.Description, -1, name, (int)sizeof(name), NULL, NULL) == 0)
            {
                strcpy(name, "(unreadable)");
            }
            printf("    [%u] %s  vendor 0x%04X device 0x%04X  VRAM %llu MB%s\n",
                   (unsigned)index, name, (unsigned)desc.VendorId, (unsigned)desc.DeviceId,
                   (unsigned long long)(desc.DedicatedVideoMemory / (1024 * 1024)),
                   (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) ? "  [SOFTWARE/WARP]" : "");
        }
        IDXGIAdapter1_Release(adapter);
    }
    IDXGIFactory1_Release(factory);
}

/* Windows facts that have no environment-variable equivalent but decide the outcome.
   Remote Desktop is the big one: an RDP session has no real GPU, so the D3D paths fail
   or fall back to software, and the framerate is at the mercy of the network -- both of
   which read as "ReMoM is broken" in a report that never mentions RDP. */
static void print_windows_graphics(void)
{
    printf("windows:\n");
    print_windows_version();
    printf("  remote desktop session      = %s\n", GetSystemMetrics(SM_REMOTESESSION) ? "YES -- no real GPU; software/slow rendering is EXPECTED" : "no");
    print_windows_adapters();
    putchar('\n');
}
#endif  /* _WIN32 */


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
   runs inside a fork (UNIX) or a re-spawned child (Windows) so a fatal exit is contained.

   report_actual makes the attempt print the renderer SDL actually GRANTED, to stdout,
   with no trailing newline -- the matrix parent has already printed the row prefix and
   will print the verdict after we exit.  This matters because SDL_HINT_RENDER_DRIVER is
   a preference, not a demand: with index -1 SDL tries the hinted driver first and then
   falls through the rest of its list, so a bare "OK" can mean "some OTHER driver worked".
   On Linux the video-driver axis carries the signal and that ambiguity is tolerable; on
   Windows there is effectively only the "windows" video driver, so the render column IS
   the table and an unlabelled OK would be actively misleading. */
static int attempt(const char * video_driver, const char * render_driver, int verbose, int report_actual)
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

    /* The line that can fatally exit on a broken GLX-over-XWayland stack, or fault
       inside a broken D3D/OpenGL ICD on Windows. */
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

    if(report_actual)
    {
        SDL_RendererInfo info;
        if(SDL_GetRendererInfo(ren, &info) == 0)
        {
            printf("[actual: %s] ", info.name);
            fflush(stdout);  /* _exit() past this point would not flush it */
        }
    }

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


/* ------------------------------------------------------------------------- *
 *  Per-attempt isolation                                                     *
 *                                                                            *
 *  probe_Run_Isolated() runs one (video x render) attempt in its own process *
 *  and prints the verdict.  The two implementations differ only in HOW the   *
 *  process is made and how its status decodes; everything above and below is *
 *  shared.                                                                   *
 * ------------------------------------------------------------------------- */

#ifdef _WIN32

/* Our own image path.  argv[0] is not dependable (it is whatever the launcher passed,
   and is relative when the tool is started from a shortcut or a different CWD), and we
   are about to hand it to _spawnv as the image to run -- so ask the loader instead. */
static const char * probe_Self_Path(void)
{
    static char path[MAX_PATH];
    if(path[0] == '\0')
    {
        DWORD n = GetModuleFileNameA(NULL, path, (DWORD)sizeof(path));
        if(n == 0 || n >= sizeof(path)) { path[0] = '\0'; return NULL; }
    }
    return path;
}

/* Decode a child's exit code.  A child that faulted exits with the EXCEPTION code, so
   anything outside our small PROBE_* set is a crash -- and the code itself is worth
   printing, since 0xC0000005 (access violation) vs 0xC0000135 (missing DLL) vs
   0xC0000409 (stack cookie / fast-fail) point at different broken layers. */
static const char * verdict_from_exit_code(unsigned long code)
{
    static char buf[64];
    switch(code)
    {
        case PROBE_OK:            return "OK";
        case PROBE_INIT_FAILED:   return "fail: SDL_Init";
        case PROBE_WINDOW_FAILED: return "fail: CreateWindow";
        case PROBE_RENDER_NULL:   return "fail: renderer NULL";
        default: break;
    }
    snprintf(buf, sizeof(buf), "CRASH (exit/exception 0x%08lX)", code);
    return buf;
}

/* Re-spawn ourselves with an internal --child mode.  _spawnv(_P_WAIT) returns the
   child's exit code directly, so there is no wait-status decoding to do. */
static int probe_Spawn_Child(const char * mode, const char * arg1, const char * arg2)
{
    const char * self = probe_Self_Path();
    const char * argv[5];
    intptr_t rc;

    if(self == NULL) { return -1; }

    argv[0] = self;
    argv[1] = mode;
    argv[2] = arg1;
    argv[3] = arg2;
    argv[4] = NULL;

    fflush(stdout);
    rc = _spawnv(_P_WAIT, self, argv);
    return (int)rc;
}

static void probe_Run_Isolated(const char * video_driver, const char * render_driver)
{
    int rc;

    /* Row prefix first, then the child's "[actual: ...]" (if it got that far), then our
       verdict -- one line assembled by two processes.  stdout is unbuffered here. */
    printf("    render %-10s : ", render_driver);

    rc = probe_Spawn_Child("--child", probe_Encode_Driver_Arg(video_driver), probe_Encode_Driver_Arg(render_driver));
    if(rc == -1)
    {
        printf("spawn failed: %s\n", strerror(errno));
        return;
    }
    printf("%s\n", verdict_from_exit_code((unsigned long)rc));
}

#else  /* UNIX */

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
static void probe_Run_Isolated(const char * video_driver, const char * render_driver)
{
    pid_t pid;

    /* Row prefix first, then the child's "[actual: ...]" (if it got that far), then our
       verdict -- one line assembled by two processes. */
    printf("    render %-10s : ", render_driver);

    /* Flush BEFORE fork: piped/redirected stdout is fully buffered, and a child would
       otherwise inherit -- and later re-emit -- the parent's unflushed buffer, printing
       our header (and now the row prefix) twice.  main() also sets line buffering, but
       the prefix above has no newline, so this flush is what actually guarantees it. */
    fflush(stdout);

    pid = fork();
    if(pid < 0)
    {
        printf("fork() failed: %s\n", strerror(errno));
        return;
    }
    if(pid == 0)
    {
        /* Child: silence SDL's stderr chatter (the parent reports the verdict), attempt,
           and _exit with the PROBE_* code.  A fatal Xlib exit()/abort() also lands here. */
        probe_Harden_Child();
        _exit(attempt(video_driver, render_driver, 0, 1));
    }
    else
    {
        int status = 0;
        waitpid(pid, &status, 0);
        printf("%s\n", verdict(status));
    }
}

#endif  /* _WIN32 */


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
            probe_Run_Isolated(vd, RENDER_CANDIDATES[ri]);
        }
        putchar('\n');
    }
    printf("Legend: OK = window+renderer created and presented a frame.\n");
    printf("        CRASH = the combination faults fatally (do not use it).\n");
    printf("        fail:* = SDL declined cleanly (combination simply unavailable).\n");
    printf("        [actual: X] = the driver SDL really used.  A request is only a PREFERENCE;\n");
    printf("        when it differs from the requested column, that driver was unavailable and\n");
    printf("        SDL fell back -- so the row says nothing about the one you asked for.\n");
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

/* The measurements --timing runs, as a table rather than a call list, so the Windows
   spawn path can name a case by INDEX.  (Passing the label as a command-line argument
   would mean quoting a string with spaces and commas through the CRT's argv parser for
   no benefit.)  The ~18 fps row is the ReMoM-like one: sparse output is what actually
   drives a monitor below its VRR floor. */
struct timing_case
{
    const char * label;
    int          vsync;
    int          target_fps;
    int          frames;
};

static const struct timing_case TIMING_CASES[] = {
    { "vsync OFF, uncapped", 0,  0, 180 },
    { "vsync ON,  uncapped", 1,  0, 180 },
    { "vsync ON,  ~18 fps",  1, 18,  90 },   /* ReMoM-like sparse output */
    { "vsync ON,  ~30 fps",  1, 30,  90 },
    { "vsync ON,  ~60 fps",  1, 60, 120 }
};
#define TIMING_CASE_COUNT ((int)(sizeof(TIMING_CASES) / sizeof(TIMING_CASES[0])))

/* Present `frames` frames and report the inter-present interval statistics for one case.
   Runs in the CURRENT process -- the caller is responsible for isolation.  Returns a
   PROBE_* code. */
static int measure_cadence_here(const struct timing_case * tc)
{
    SDL_Window *   win;
    SDL_Renderer * ren;
    SDL_RendererInfo info;
    Uint64 freq = SDL_GetPerformanceFrequency();
    double target_ms = (tc->target_fps > 0) ? (1000.0 / tc->target_fps) : 0.0;
    double sum = 0.0, sumsq = 0.0, mn = 1e30, mx = 0.0;
    int granted_vsync;
    int i;
#ifdef _WIN32
    int raised_timer_resolution = 0;
#endif

    if(SDL_Init(SDL_INIT_VIDEO) != 0) { return PROBE_INIT_FAILED; }
    win = SDL_CreateWindow("remom_video_probe timing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 400, SDL_WINDOW_SHOWN);
    if(win == NULL) { SDL_Quit(); return PROBE_WINDOW_FAILED; }
    ren = SDL_CreateRenderer(win, -1, 0);
    if(ren == NULL) { SDL_DestroyWindow(win); SDL_Quit(); return PROBE_RENDER_NULL; }

    SDL_RenderSetVSync(ren, tc->vsync ? 1 : 0);
    granted_vsync = (SDL_GetRendererInfo(ren, &info) == 0) && (info.flags & SDL_RENDERER_PRESENTVSYNC);

#ifdef _WIN32
    /* Windows' default timer granularity is ~15.6 ms, so an unraised SDL_Delay() would
       overshoot every paced frame and report jitter that is an artifact of the SLEEP,
       not of the display -- precisely the false positive this mode exists to rule out.
       Raise the resolution for the duration of the measurement, and say so in the row. */
    if(target_ms > 0.0 && timeBeginPeriod(1) == TIMERR_NOERROR)
    {
        raised_timer_resolution = 1;
    }
#endif

    /* Warm up: the first few presents include allocation/first-flip costs. */
    for(i = 0; i < 10; i++)
    {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        SDL_RenderPresent(ren);
    }

    for(i = 0; i < tc->frames; i++)
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

#ifdef _WIN32
    if(raised_timer_resolution) { timeEndPeriod(1); }
#endif

    {
        double mean = sum / tc->frames;
        double var = (sumsq / tc->frames) - (mean * mean);
        double stddev = (var > 0.0) ? sqrt(var) : 0.0;
        printf("  %-22s : mean %6.2f ms  min %6.2f  max %6.2f  jitter(sd) %5.2f ms  -> %6.1f fps  [vsync %s]\n",
               tc->label, mean, mn, mx, stddev, 1000.0 / mean,
               granted_vsync ? "granted" : (tc->vsync ? "REQUESTED-not-granted" : "off"));
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return PROBE_OK;
}

/* Run one timing case in its own process so a fault here can't take the probe down. */
static void measure_cadence(int case_index)
{
    const struct timing_case * tc = &TIMING_CASES[case_index];

#ifdef _WIN32
    char index_arg[16];
    int rc;

    snprintf(index_arg, sizeof(index_arg), "%d", case_index);
    rc = probe_Spawn_Child("--child-timing", index_arg, PROBE_DEFAULT_DRIVER_ARG);
    if(rc == -1)
    {
        printf("  %-22s : spawn failed: %s\n", tc->label, strerror(errno));
    }
    else if(rc != PROBE_OK)
    {
        printf("  %-22s : measurement FAILED (%s)\n", tc->label, verdict_from_exit_code((unsigned long)rc));
    }
#else
    pid_t pid;

    fflush(stdout);
    pid = fork();
    if(pid < 0)
    {
        printf("  %-22s : fork() failed: %s\n", tc->label, strerror(errno));
        return;
    }
    if(pid != 0)
    {
        int status = 0;
        waitpid(pid, &status, 0);
        if(!WIFEXITED(status) || WEXITSTATUS(status) != 0)
        {
            printf("  %-22s : measurement CRASHED (%s)\n", tc->label, WIFSIGNALED(status) ? "signal" : "nonzero exit");
        }
        return;
    }

    /* ---- child ---- */
    probe_Harden_Child();
    _exit(measure_cadence_here(tc));
#endif
}

static void probe_timing(void)
{
    int i;

    report_displays();

    printf("present-cadence measurements (each in its own process):\n\n");
    for(i = 0; i < TIMING_CASE_COUNT; i++)
    {
        measure_cadence(i);
    }
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
#ifdef _WIN32
    printf("  - The paced rows raise the Windows timer resolution to 1 ms for the duration of\n");
    printf("    the measurement, so the numbers reflect the DISPLAY, not Windows' default\n");
    printf("    ~15.6 ms sleep granularity.\n");
#endif
}

int main(int argc, char ** argv)
{
    /* We claimed the entry point with SDL_MAIN_HANDLED; tell SDL its init assumptions hold. */
    SDL_SetMainReady();

    /* Keep stdout interleaving sane across the parent and its per-attempt children.
       UNIX: line-buffer, so with the fflush before each fork no child ever inherits --
       and re-emits -- unflushed parent output.
       Windows: MSVC's CRT has no line buffering (_IOLBF degrades to _IOFBF), and worse,
       it REJECTS a zero size for anything but _IONBF -- the invalid-parameter handler
       aborts the process before a single line is printed.  Go unbuffered instead: the
       spawn path starts fresh processes that have no parent buffer to duplicate, so
       ordering is all we need, and a diagnostic tool's output volume is trivial. */
#ifdef _WIN32
    setvbuf(stdout, NULL, _IONBF, 0);
#else
    setvbuf(stdout, NULL, _IOLBF, 0);
#endif

    /* Internal re-entry points: the Windows isolation path re-runs this executable for
       each attempt, because there is no fork() to inherit state from.  Silent by design
       -- the parent prints the verdict from the exit code. */
    if(argc >= 4 && strcmp(argv[1], "--child") == 0)
    {
        probe_Harden_Child();
        return attempt(probe_Decode_Driver_Arg(argv[2]), probe_Decode_Driver_Arg(argv[3]), 0, 1);
    }
    if(argc >= 3 && strcmp(argv[1], "--child-timing") == 0)
    {
        int case_index = atoi(argv[2]);
        if(case_index < 0 || case_index >= TIMING_CASE_COUNT) { return PROBE_INIT_FAILED; }
        probe_Harden_Child();
        return measure_cadence_here(&TIMING_CASES[case_index]);
    }

    if(argc >= 2 && strcmp(argv[1], "--once") == 0)
    {
        const char * vd = (argc >= 3) ? argv[2] : "";
        const char * rd = (argc >= 4) ? argv[3] : "";
        int rc = attempt(vd, rd, 1, 0);
        printf("result: %s\n", (rc == PROBE_OK) ? "OK" : "FAILED");
        return (rc == PROBE_OK) ? 0 : 1;
    }

    print_environment();
#ifdef _WIN32
    print_windows_graphics();
#endif

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
