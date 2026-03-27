
Platform Troubleshooting with Claude Opus 4.6



Here's what the log reveals. The data tells a clear story:

Handler Call Pattern: Bimodal
Gap	Count	What it means
0–1ms	254 (45%)	Rapid-fire — multiple Platform_Event_Handler() calls per frame
2–55ms	38 (7%)	Normal — within a single 55ms frame tick
56ms+	277 (49%)	Slow — handler isn't called for an entire frame or more
Almost half the handler calls have 56ms+ gaps. The worst spike is 387ms (~7 frames of input blackout). There's a sustained period (t=12576–14889) where every gap is ~125ms — the handler is only called every other frame.

Root Cause: The 55ms Frame Loop
The game only calls Platform_Event_Handler() once per Get_Input() which runs once per 55ms BIOS tick. Between handler calls, mouse movement is invisible to the engine — the cursor position only updates when the handler runs.

The 125ms gaps (~2 frame ticks) suggest the engine sometimes skips a frame — likely busy doing game logic (world simulation, rendering) and not returning to the input loop in time.

Click Processing is Fast (not the problem)
Click-to-buffer latency is 0–1ms — once BTN_DOWN fires, UMH and BUFFERED happen in the same millisecond. The click pipeline itself is fine.

Cursor Redraw is ~30ms Apart
MAYBE_MOVE calls happen at ~30ms intervals (the cursor redraw timer). Between handler calls, the cursor keeps redrawing at its stale position — so the visual cursor lags behind the actual OS cursor by up to 125–387ms.

Also Notable: enabled=0
Every UMH call shows enabled=0 — meaning mouse_enabled is 0. This means cursor drawing is disabled during these events. If that's intentional for certain screens, fine — but if it's unexpected, clicks may visually feel unresponsive because no cursor feedback is shown.

Summary
The sluggishness is not in event processing or click detection — it's structural:

Input is polled at ~18 Hz (55ms) — the OS cursor moves smoothly at 60+ Hz but the game cursor can only update at 18 Hz
Frame skips double this to ~125ms gaps (half the log), making the cursor visibly stutter
The 387ms spike is a full third-of-a-second freeze — likely a heavy game operation blocking the main loop
To fix the perceived lag, the options are:

Decouple cursor rendering from the game loop — draw the cursor at a higher rate (e.g., during MAYBE_MOVE calls which already run at ~30ms)
Investigate the 125ms gaps — find what's blocking the main loop every other frame
Use the OS hardware cursor instead of the software cursor — it would track 1:1 with the mouse, no lag at all
Want me to investigate what causes the 125ms frame skips, or look into one of the fix approaches?


All Layer 3 instrumentation is done. Here's a summary of what was added:

Files modified:

File	Changes
MOM_SCR.c	SCR ENTER/LEAVE timestamps for all 7 screen cases
INTRO.c	SCR INTRO start/end + per-scene loading/playing timestamps; added Platform.h include
MainMenu.c	SCR MENU timestamps at picture load, palette+settings load, loop entry; field-click logging with button names
LoadScr.c	SCR LOAD timestamps at fade_out, resource load, loop entry, savegame load; field-click logging; added Platform.h include
MainScr.c	SCR MAIN timestamps at picture load, map setup, loop entry; field-click logging
Input.c	SCR GET_INPUT logging (already done in previous session)
How to use:


# After running, filter screen transition timestamps:
grep "SCR " mouse_debug.log

# Find freezes — large time gaps between SCR lines:
grep "SCR " mouse_debug.log | awk -F'[= ]' '{t=$2} NR>1{d=t-p; if(d>50) print d"ms gap before: "$0} {p=t}'


cd /c/STU/devel/ReMoM && cmake -B build -DUSE_SDL3=ON -DMOUSE_DEBUG=ON && cmake --build build
cd /c/STU/devel/ReMoM/build/bin/Debug && ./ReMoMber.exe













But there's a deeper problem — the MOX_tests target doesn't get SDL include paths at all, since Platform links SDL PRIVATE. And this test references sdl2_surface_RGB666 from the platform internals, so it's inherently backend-specific.
For the current SDL2 default build, MOX_tests needs the SDL2 include directories. The proper fix: Platform should expose SDL headers to its test consumers, or this test should go through the Platform API instead of reaching into SDL directly.
What's your preference — should MOX_tests link SDL2 directly for now, or do you want to refactor the test to not depend on SDL internals?
refactor the test to not depend on SDL internals
Read c:\STU\devel\ReMoM\MoX\tests\test_Fonts.cpp
The file has two groups of tests:
Update_Cycle tests (lines 24–127) — pure logic, no SDL needed at all
Cycle_Palette_Color tests (lines 129–333) — deeply coupled to SDL surface/palette internals (SDL3-specific API calls)
The cleanest approach: remove the SDL dependency from the file entirely, and conditionally compile the palette tests only when SDL3 is the backend.
Done. The changes:
Removed the top-level #include <SDL.h> and extern SDL_Surface * — no SDL dependency for the Update_Cycle tests
Wrapped the entire Cycle_Palette_Color_test fixture and its tests in #ifdef USE_SDL3 — these tests use SDL3-specific API (SDL_CreateSurface, SDL_GetSurfacePalette, etc.) so they only compile when the SDL3 backend is active
The SDL3 include and the sdl2_surface_RGB666 extern are now inside the #ifdef block
The Update_Cycle tests (pure math/logic) will run on all backends. The palette tests only run with -DUSE_SDL3=ON.





Done. Here's what was added:

Layer 1 — MOUSE_DEBUG per-event traces (compile-time, off by default):

HANDLER_START — logged at top of Platform_Event_Handler() in all 3 backends
BTN_DOWN — logged on mouse button down events
POLL — logged when SDL polls mouse position (SDL2/SDL3 only)
WM_MOUSEMOVE — logged on Win32 mouse move messages
UMH / UMH_OOB — logged in User_Mouse_Handler() (accepted / out-of-bounds rejected)
BUFFERED — logged in Check_Mouse_Buffer() when a click is recorded
MAYBE_MOVE — logged in Platform_Maybe_Move_Mouse()
Layer 2 — Handler delta tracking (always on, in DBG_Frame_Reset):

max_delta and avg_delta added to the existing per-60-frame summary line
Shows the longest and average gap between consecutive Platform_Event_Handler() calls
Usage:


# Build with verbose mouse logging
cmake -B build -DUSE_SDL3=ON -DMOUSE_DEBUG=ON && cmake --build build

# Run and capture
./build/bin/Debug/ReMoMber.exe 2> mouse_debug.log

# Check handler call frequency (always visible, even without MOUSE_DEBUG)
grep "DBG frame=" mouse_debug.log

# Trace a click end-to-end
grep "BTN_DOWN\|UMH\|BUFFERED" mouse_debug.log





# Mouse Input Debug Instrumentation

## Context

Users report the mouse feels "laggy or sluggish." The game runs at ~18.2 Hz (55ms per frame), and mouse position is only polled once per `Platform_Event_Handler()` call. We need diagnostic tools to confirm where latency accumulates and compare behavior across backends.

The mouse pipeline has three stages where lag can hide:
1. **OS → Platform**: SDL polls events; position sampled once per handler call
2. **Platform → Engine**: `User_Mouse_Handler()` scales coords, bounds-checks, updates globals
3. **Engine → Screen**: Software cursor save/restore/draw cycle, then `Platform_Video_Update()`

## Approach: `MOUSE_DEBUG` compile flag + enhanced frame summary

Two layers of instrumentation, both gated behind a single `MOUSE_DEBUG` compile definition (off by default):

### Layer 1: Per-Event Trace Log (verbose, opt-in via `MOUSE_DEBUG`)

Add `fprintf(stderr, ...)` at each stage of the pipeline so the full path of a single mouse event can be traced end-to-end with timestamps.

**Log points (all `#ifdef MOUSE_DEBUG`):**

| Location | Log line | What it answers |
|----------|----------|-----------------|
| `Platform_Event_Handler()` top | `MOUSE t=<ms> HANDLER_START delta=<ms> ptr=<x,y>` | How often is the handler called? What's the gap? |
| SDL BUTTON_DOWN case | `MOUSE t=<ms> BTN_DOWN btn=<1\|2> wx=<x> wy=<y>` | When did the OS report the click? |
| SDL_GetMouseState poll | `MOUSE t=<ms> POLL wx=<x> wy=<y> gx=<x> gy=<y>` | What does the OS say the position is? |
| `User_Mouse_Handler()` | `MOUSE t=<ms> UMH btn=<b> wx=<x> wy=<y> gx=<x> gy=<y> scale=<s> enabled=<0\|1> interrupt=<0\|1>` | Did the event get through? Was cursor drawing skipped? |
| `User_Mouse_Handler()` OOB return | `MOUSE t=<ms> UMH_OOB wx=<x> wy=<y> gx=<x> gy=<y>` | Are events being silently dropped? |
| `Check_Mouse_Buffer()` | `MOUSE t=<ms> BUFFERED x=<x> y=<y> btn=<b>` | Was the click actually recorded? |
| `Platform_Maybe_Move_Mouse()` | `MOUSE t=<ms> MAYBE_MOVE ptr=<x,y> img=<n>` | Is the cursor update path running? |

Win32 backend gets equivalent logs in `WM_MOUSEMOVE`, `WM_LBUTTONDOWN`, `WM_RBUTTONDOWN`.

### Layer 2: Handler-Call Delta Tracking (in existing `DBG_Frame_Reset`)

Enhance the existing always-on frame summary counters to track:
- `dbg_max_handler_delta` — longest gap between consecutive `Platform_Event_Handler()` calls within the 60-frame window
- `dbg_total_handler_delta` — sum of all deltas (for computing average)

The existing summary line becomes:
```
DBG frame=120 frametime=55ms handler_calls=3 max_delta=58ms avg_delta=52ms events: key=0 mdown=1 ...
```

This is always compiled in (matches existing pattern), no `#ifdef` needed. If `max_delta` is consistently 55ms+, the frame rate is the bottleneck. If it spikes to 100ms+, something is blocking the event loop.

## Files to modify

| File | Change |
|------|--------|
| [sdl3_PFL.c](platform/sdl3/sdl3_PFL.c) | Layer 1 traces in `Platform_Event_Handler()`, `Platform_Update_Mouse_Position()`, `Platform_Maybe_Move_Mouse()`. Layer 2 delta counters + `DBG_Frame_Reset()` update. |
| [sdl2_PFL.c](platform/sdl2/sdl2_PFL.c) | Same as SDL3 (keep in sync). |
| [win_PFL.cpp](platform/win32/win_PFL.cpp) | Layer 1 traces in `Win_Window_Proc` cases and `Platform_Maybe_Move_Mouse()`. No Layer 2 (Win32 has no `DBG_Frame_Reset`). |
| [Mouse.c](MoX/src/Mouse.c) | Layer 1 traces in `User_Mouse_Handler()` and `Check_Mouse_Buffer()`. |
| [platform/CMakeLists.txt](platform/CMakeLists.txt) | Add `option(MOUSE_DEBUG ...)` → `target_compile_definitions(Platform PRIVATE MOUSE_DEBUG)`. Also propagate to MoX target for Mouse.c. |
| [CMakeLists.txt](CMakeLists.txt) | Forward `MOUSE_DEBUG` option to MoX target. |

## How to use

```bash
# Build with mouse debug logging enabled
cmake -B build -DUSE_SDL3=ON -DMOUSE_DEBUG=ON && cmake --build build

# Run, redirect stderr to a log file
./build/bin/Debug/ReMoMber.exe 2> mouse_debug.log

# Quick check: handler call frequency
grep HANDLER_START mouse_debug.log | head -20

# Trace a single click end-to-end
grep "BTN_DOWN\|UMH\|BUFFERED" mouse_debug.log

# Check for dropped events (OOB)
grep UMH_OOB mouse_debug.log
```

## Verification

1. Build SDL3 with `-DMOUSE_DEBUG=ON` — compiles clean
2. Build SDL3 without `-DMOUSE_DEBUG` — no debug output, no perf impact
3. Build SDL2 with `-DMOUSE_DEBUG=ON` — compiles clean
4. Build Win32 with `-DMOUSE_DEBUG=ON` — compiles clean
5. Run with debug enabled, move mouse around, click — verify log output on stderr shows all trace points
6. Verify `DBG_Frame_Reset` summary line includes `max_delta` and `avg_delta` (always, even without `MOUSE_DEBUG`)
