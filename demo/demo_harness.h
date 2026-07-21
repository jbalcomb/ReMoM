/*
    demo_harness.h — shared scaffolding for stand-alone VGA / video-effect demos.

    The harness owns the minimal engine boot and a few helpers, so each effect file only has to
    express its own animation loop.  None of this is game code; it links the engine libraries and
    calls their public API.

    Boot is asset-aware:
      - If the game data (FONTS.LBX) is present in the working directory, Demo_Boot() runs the
        engine's own driver init (Init_Drivers): real video pages, fonts, palette, mouse.  Effects
        can then load real MoM screens via Demo_Load_Screen().
      - Otherwise it falls back to a minimal manual boot with a synthetic palette, and
        Demo_Load_Screen() reports failure so effects use Demo_Fill_Pattern() instead.  This keeps
        the demo building, headless-smoke-running, and working with no data files.
*/

#ifndef DEMO_HARNESS_H
#define DEMO_HARNESS_H

#include <stdint.h>

/* Install the unhandled-exception reporter (symbolized crash stack; see demo_crash.c).  The demos
   drive real game code from real saves, so a fault should name a function, not just say "crashed". */
void Demo_Install_Crash_Handler(void);

/* Bring up / tear down the engine state an effect needs (asset-aware; see above). */
void Demo_Boot(void);
void Demo_Shutdown(void);

/* Non-zero once Demo_Boot() has brought up the real asset pipeline (FONTS.LBX found). */
int Demo_Assets_Ready(void);

/*
    Load a real MoM screen: LBX entry 'entry' from '<lbx>.LBX' is blitted (via FLIC_Draw) into
    video_page_buffer[page], and (if palette_entry >= 0) Load_Palette(palette_entry) is applied.
    Returns 1 on success, 0 if assets are unavailable or the file is missing (in which case the
    caller should fall back to Demo_Fill_Pattern()).  The file is existence-checked first, because
    LBX_Load() aborts the process on a missing file.
*/
int Demo_Load_Screen(const char * lbx, int entry, int palette_entry, int page);

/*
    Load a raw LBX entry (sprite/animation) and return its segment pointer, or NULL if assets are
    unavailable or the file is missing.  Unlike Demo_Load_Screen this neither draws nor touches the
    palette -- it just hands back the loaded picture for the caller to draw (e.g. with FLIC_Draw).
*/
void * Demo_Lbx_Load(const char * lbx, int entry);

/* Fill a 320x200 video page with a shifting bar pattern (synthetic fallback content). */
void Demo_Fill_Pattern(uint8_t * page, int phase);

/* Apply the current palette and present the current draw page to the window. */
void Demo_Present(void);

/* Pump platform events once; returns non-zero if the user asked to quit (e.g. closed the window). */
int Demo_Quit(void);

/* Responsive wait: pumps events and re-presents the current frame until 'millies' elapse or quit. */
void Demo_Dwell(uint64_t millies);

/* Self-contained PRNG returning 0..n-1, for jitter. */
int Demo_Rand(int n);

/* CLI passthrough: main() stores the arguments that follow the effect name; effects read them to
   override which screen(s) to load.  Demo_Arg returns the nth (0-based) such argument or NULL;
   Demo_Arg_Int parses it as an int, returning 'fallback' if absent. */
void Demo_Set_Args(int count, char ** args);
const char * Demo_Arg(int index);
int Demo_Arg_Int(int index, int fallback);

#endif /* DEMO_HARNESS_H */
