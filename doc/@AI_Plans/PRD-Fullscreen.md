# PRD — Fullscreen Support

**Status:** Draft
**Owner:** TBD
**Date:** 2026-11-11
**Related:** `PRD-Graphics-Upscale.md` (output-frame upscaling, Family A), `PRD-Platform-Input.md` (mouse/input metrics), `PRD-SDL2-Platform-Backend.md`

---

## Problem Statement

ReMoM can only run in a fixed 1280×800 window (320×200 at 4×). There is no way to play fullscreen. On a typical 1920×1080 or larger monitor the game is a small window on the desktop with no way to fill the screen.

Resizing the window doesn't help either. The window can be resized, but nothing handles a resize. The game image stretches to the new window shape and loses its aspect ratio. The mouse keeps using the original 4× mapping, so clicks land in the wrong place. Because fullscreen is effectively a resize to the monitor's size, it would hit both problems.

The root cause: the platform layer assumes the window is exactly a whole-number multiple of 320×200, starting at the window's top-left corner. Several separate places (mouse polling, mouse events, cursor warp, the hardware-cursor prototype, the game's mouse handler, replay injection, relative-mouse speed) each compute the scale themselves as window width ÷ 320. They use the X axis only, with no offset for black bars.

## Solution

The player presses **Alt+Enter** to switch between windowed and borderless desktop fullscreen on the monitor the window is currently on. In fullscreen:

- The 320×200 image is shown at the largest whole-number scale that fits, centred, with black bars filling the rest. On 1920×1080 that is 5×: a 1600×1000 image with 160-pixel bars left and right and 40-pixel bars top and bottom.
- Pixels stay square, so the default aspect is 16:10.
- The mouse is captured inside the window. Clicks land exactly where the cursor is drawn, and the cursor cannot move into the black bars.
- Alt+Enter never reaches the game as a keypress and is never recorded in a replay.

The same scaling and mouse handling also apply to a resized window, so a resizable window works correctly too.

A `[Video]` section in `ReMoM.ini` lets the player:

- start in fullscreen;
- pick one of two alternative scaling modes instead of integer-only:
  - **fit (nearest)** — fills as much of the screen as possible with non-integer nearest-neighbour scaling;
  - **fit (sharp bilinear)** — integer pre-scale, then a linear filter down to the fitted size.

The game never writes to `ReMoM.ini`. An Alt+Enter toggle lasts for the current session only.

Rollout order: SDL2 (the default backend) first, then SDL3, then Win32, then Linux, then macOS.

## User Stories

1. As a player, I want to press Alt+Enter to go fullscreen, so that the game fills my monitor.
2. As a player, I want to press Alt+Enter again to go back to a window, so that I can use other applications.
3. As a player, I want fullscreen to switch instantly without changing my monitor's resolution or refresh rate, so that my desktop and other monitors aren't disturbed.
4. As a player, I want fullscreen to use the monitor the game window is currently on, so that I control which screen the game appears on.
5. As a player, I want the image scaled by a whole number by default, so that every original pixel is the same size and the art stays crisp.
6. As a player, I want the image centred with black bars when the integer scale doesn't fill the screen, so that the picture is never distorted.
7. As a player, I want square pixels (16:10) by default, so that the image matches what the windowed game already shows.
8. As a player, I want mouse clicks to land exactly on what I see in fullscreen, so that buttons, map squares and units are hit correctly.
9. As a player, I want the mouse to stay inside the game window while fullscreen, so that I don't accidentally click onto another monitor or the desktop.
10. As a player, I want the cursor to stop at the edge of the game image rather than disappearing into the black bars, so that I always know where it is.
11. As a player, I want Alt+Enter not to act as Enter in the game, so that toggling fullscreen never confirms a dialog or ends a turn.
12. As a player, I want to resize the game window freely, so that I can pick a window size that suits my desktop.
13. As a player, I want a resized window to use the same scaling and black-bar rules as fullscreen, so that resizing never distorts the image or breaks the mouse.
14. As a player, I want to set `fullscreen = yes` in `ReMoM.ini`, so that the game starts fullscreen every time.
15. As a player, I want to set a scale mode in `ReMoM.ini` to fill more of the screen, so that I can trade pixel-perfect crispness for screen coverage if I prefer.
16. As a player, I want a "fit (sharp bilinear)" mode, so that a non-integer fit doesn't produce uneven pixel widths.
17. As a player, I want an invalid `[Video]` value in `ReMoM.ini` to fall back to the default with a log message, so that a typo never stops the game from starting.
18. As a player on a high-DPI display with Windows display scaling, I want the scaling and mouse mapping to use the real pixel size of the screen, so that fullscreen and mouse aim are still correct at 125%/150%/200% scaling.
19. As a player, I want the game to keep working if I Alt+Tab away from fullscreen and back, so that switching applications doesn't lose the game or the mouse.
20. As a player, I want the software-drawn mouse cursor to keep its current look in fullscreen, so that nothing about the game's appearance changes except its size on screen.
21. As a player trying the hardware-cursor prototype, I want the OS cursor to be sized to the current scale, so that it matches the game image in fullscreen too.
22. As a developer, I want one shared window↔game coordinate transform, so that every mouse path agrees and a future scaling change is made in one place.
23. As a developer, I want that transform to be pure C with no SDL dependency, so that it can be unit-tested and reused by the SDL2, SDL3 and Win32 backends.
24. As a developer, I want replays recorded windowed to play back correctly fullscreen (and the other way round), so that replay files don't depend on display settings.
25. As a developer, I want the replay header's recorded scale to stay meaningful (or be replaced with something that is), so that replay diagnostics don't report a misleading number once scale can be non-integer.
26. As a developer, I want DeMoM windowless runs and the CTest suite to be unaffected, so that fullscreen work can't break the deterministic tests.
27. As a developer, I want fullscreen toggles, display size, chosen scale, and black-bar offsets logged through STU_LOG, so that a player's bug report shows what the display was doing.
28. As a developer, I want the input-metrics recorder to report the actual scale in use, so that metrics captured fullscreen are interpreted correctly.
29. As a developer, I want the Alt+Enter handling placed so that a future keymap feature can rebind it, so that F11 (or any key) can become the toggle later without restructuring.
30. As a developer, I want the mouse-capture code to have a single release/re-grab point, so that a "release mouse" hotkey can be added later.
31. As a developer porting to SDL3, Win32, Linux, and macOS, I want the backend-specific part to be a small, well-defined surface (toggle fullscreen, capture mouse, report drawable size, present into a rect), so that each port is mostly glue.

## Implementation Decisions

### Modules

1. **Viewport (new, shared, pure C).** The deep module. There is no SDL or OS code in it.
   - **Inputs:**
     - drawable size in real pixels (width, height);
     - game size (320×200);
     - scale mode: `integer`, `fit_nearest` or `fit_sharp_bilinear`;
     - pixel aspect (square only for now; the parameter exists so 4:3 and other aspects can be added later without an interface change).
   - **Outputs:**
     - the destination rectangle on the drawable;
     - the integer pre-scale factor (used by sharp bilinear);
     - a window→game mapping that clamps to the game area;
     - a game→window mapping (used for cursor warp and replay injection).
   - Each axis has its own scale and offset. Nothing assumes X scale equals Y scale.
   - The Viewport is recomputed whenever the drawable size changes, and every consumer reads the current one.

2. **Every mouse coordinate path switches to Viewport.** Each existing "window width ÷ 320" calculation is replaced by a Viewport call:
   - the backends' mouse polling, mouse event handling and cursor warp;
   - the hardware-cursor size;
   - the relative-mouse scale;
   - the game-side mouse handler in MoX;
   - replay injection;
   - the platform "get scale" / "get window width" query functions.
   
   The game-side mouse handler is reconstructed original code already carrying a `/* CLAUDE */` modification. That change follows the house rule: comment out the existing line, add the new one below it, mark it `/* CLAUDE */`.

3. **Display Mode (per backend, starting with SDL2).**
   - Toggles borderless desktop fullscreen on the window's current display.
   - Handles window resize, size-change and display-change events by re-querying the drawable size *in pixels* (not window units, because of HiDPI) and recomputing the Viewport.
   - Captures the mouse while fullscreen and releases it when windowed or when focus is lost. Capture and release go through one function each, so a future release hotkey has one place to hook in.
   - The existing focus-gained/focus-lost handling is kept.

4. **Present (per backend).**
   - Clears the whole drawable to black every frame, so bars never show stale content after a resize.
   - Draws the frame into the Viewport destination rect instead of stretching it over the whole window.
   - `integer` and `fit_nearest` use nearest-neighbour sampling.
   - `fit_sharp_bilinear` uses an intermediate render target at the integer pre-scale (nearest), then linear filtering into the destination rect.
   - Whether SDL2's built-in logical-size/integer-scale support can replace hand-written rect math is an open spike (see Further Notes). Even if it is used, the Viewport module stays the single source of mouse mapping.

5. **Alt+Enter hotkey (per backend key handler).**
   - Detected on key-down of Enter with Alt held, in the same place as the existing Ctrl/Shift/Alt+Q quit check.
   - Unlike that check, it stops processing. It is never translated into a game key, never queued, and so never recorded (recording happens when the game consumes a key).
   - Key-repeat is ignored, so holding the keys doesn't toggle repeatedly.
   - The matching key-up is also swallowed.
   - The binding is one named hotkey constant, so the later keymap feature can replace it and free F11.

6. **`[Video]` settings in `ReMoM.ini`.**
   - Read once at startup through the existing STU ini reader (the same one used for `[Paths]`).
   - **Keys:**
     - `fullscreen = yes | no` — default `no`.
     - `scale_mode = integer | fit_nearest | fit_sharp_bilinear` — default `integer`.
   - Values are case-insensitive. An unknown value logs a warning and uses the default.
   - The game never writes the file. The shipped `ReMoM.ini` template gets a commented `[Video]` section documenting the keys.

### Architectural decisions

- Borderless desktop fullscreen only. No exclusive mode, no resolution or refresh-rate changes. This keeps vsync pacing on the desktop's refresh rate, which the existing frame-rate work assumes.
- Fullscreen uses the window's current display. Choosing a display in config is later work.
- Game logic, the 320×200 framebuffer, the software cursor and the palette pipeline are untouched. All changes are in the platform layer, plus the one MoX mouse-handler call site.
- Replay stays in game coordinates (it already is). The header's recorded integer scale is kept for older replays, or reinterpreted as "integer pre-scale". Pick one when that slice is implemented and log both values.
- Backend order is SDL2, SDL3, Win32, Linux, macOS. Each backend is a separate slice. The Viewport module and ini settings are shared by all of them from day one.

## Testing Decisions

- **What a good test is here:** it checks external behaviour — "given this drawable size and mode, the image lands *here* and this window pixel maps to *that* game pixel". It does not check how the Viewport computes it.
- **Viewport is the only module with automated tests.** GoogleTest unit tests, following the existing `hello_test` / `gtest_discover_tests` setup in the tests directory. Cases:
  - Common drawable sizes: 1280×800, 1920×1080, 2560×1440, 3840×2160, 1366×768, 3440×1440 ultrawide, and portrait 1080×1920. Check scale, rect and bar offsets for every mode.
  - Drawable smaller than 320×200: scale floors at 1 and nothing divides by zero.
  - Exact multiples: no bars.
  - Round trip: game→window→game returns the original game pixel for every pixel on the edges and a sample of interior pixels, in every mode.
  - Clamping: window points inside the black bars or outside the drawable map to the nearest game-area edge pixel.
  - Separate axes: a drawable with different horizontal and vertical slack (the 1920×1080 case) maps X and Y independently and correctly.
- **Not automated (manual testing required):** fullscreen toggling, mouse capture, Alt+Tab, HiDPI, multiple monitors, present quality, Alt+Enter swallowing. These need real hardware. Each backend slice gets a short manual checklist covering user stories 1–21 on Windows. The Linux slice also covers X11 and XWayland, given the known GLX-over-XWayland renderer issues.
- **Regression guard:** the full `DeMoM_` CTest suite must pass unchanged. The windowless backend's scale stays 1, so any golden-file change means the work leaked out of the platform layer.

## Out of Scope

- 4:3 aspect correction and other non-square or modern aspect ratios (planned later).
- OpenCV / ImageMagick upscaler options and any other asset-level or shader-based upscaling (see `PRD-Graphics-Upscale.md`).
- Exclusive fullscreen and resolution or refresh-rate switching.
- A config option for which display to use.
- Rebinding the toggle key (needs the keymap feature; F11 becomes available then).
- A mouse-release hotkey while fullscreen.
- Saving the fullscreen or scale state back to `ReMoM.ini` or anywhere else.
- An in-game options screen for any of these settings.
- The SDL3, Win32, Linux and macOS slices beyond keeping the shared design ready for them. Each gets its own plan.

## Further Notes

- **Spike before coding the SDL2 slice:** check whether SDL2's logical-size + integer-scale presentation (and its window-to-logical mapping) matches the Viewport results exactly, letterbox offsets included, under HiDPI on the user's Windows machine. If it matches, Present can use it, and Viewport stays the single place mouse mapping happens and the reference the tests check. If it doesn't, Present draws into the Viewport rect directly. The SDL2 in use is 2.32.2.
- **Fixing resize is the first tracer slice.** It needs the Viewport, routing every mouse path through it, letterboxed present, and drawable-size requery — with no fullscreen yet. Once a resized window maps the mouse correctly, the fullscreen toggle, capture, Alt+Enter and ini keys are a thin second slice.
- **Phase 2 — borderless window mode** (requested by Xeth Nyrrow on the project's Discord). This is a borderless *windowed* mode: a window with no title bar or frame, separate from the borderless desktop fullscreen in phase 1. It uses the same Viewport, Present and mouse paths. Still to decide: whether it is a third display state alongside windowed and fullscreen, how it is chosen (a `ReMoM.ini` key and/or the toggle), and whether the window can be moved or resized without a frame.
- **Logging:** use the backend's existing init/platform log categories. Log one line per toggle and per Viewport recompute (drawable size, mode, scale, rect). No per-frame logging.
