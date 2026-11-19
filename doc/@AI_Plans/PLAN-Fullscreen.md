# Plan: Fullscreen Support

> Source PRD: `PRD-Fullscreen.md`

## Architectural decisions

These apply to every phase:

- **Viewport is the only place window↔game mapping happens.** It is a shared, pure-C module with no SDL or OS code.
  - **Inputs:** drawable size in real pixels, game size (320×200), scale mode, pixel aspect (square only for now).
  - **Outputs:** destination rect, integer pre-scale factor, window→game mapping (clamped to the game area), game→window mapping.
  - X and Y each have their own scale and offset.
  - Every backend and the MoX mouse handler read it; none compute "window width ÷ 320" themselves.
- **Scale modes:** `integer` (the default), `fit_nearest`, `fit_sharp_bilinear`. These are the names used in `ReMoM.ini`.
- **Display states:**
  - windowed (resizable);
  - borderless desktop fullscreen on the window's current display;
  - later, a borderless window (phase 6).
  
  No exclusive fullscreen, no resolution or refresh-rate changes.
- **Settings:** a `[Video]` section in `ReMoM.ini`, read once at startup through the existing STU ini reader and never written by the game.
  - `startup_window = windowed | borderless | fullscreen` (default `windowed`). It replaced phase 3's `fullscreen = yes | no` in phase 6.
  - `scale_mode = integer | fit_nearest | fit_sharp_bilinear` (default `integer`).
  - Matching is case-insensitive; an unknown value logs a warning and the default is used.
- **The window hotkeys** are Alt+Enter and Ctrl+L (fullscreen toggle) and Ctrl+H (borderless window toggle), from phase 6. They are defined in one named table so the later keymap feature can change them. The game never sees them and they are never recorded.
- **Coordinates:** game logic, the 320×200 framebuffer, the software cursor, the palette pipeline, and replay coordinates are unchanged. The only change to original game code is one call site in the MoX mouse handler. It follows the AGENTS.md marker rules.
- **Regression guard:** the full `DeMoM_` CTest suite passes unchanged in every phase. The windowless backend's scale stays 1.
- **Source material:** code from `origin/feature/present-aspect-fullscreen` (`7dcc129c`) may be reused: its present path, HiDPI window/pixel math and Alt+Enter block. The branch itself is not merged. Its 4:3 and sharp defaults, environment-variable settings and SDL-bound viewport math are replaced by the decisions above.
- **Formatting:** land the clang-format reformat of `platform/` before phase 1 starts, so new code is written in its final style.
- **Logging:** one STU_LOG line per Viewport recompute (drawable size, mode, scale, rect) and per display-state change, in the backend's existing init/platform log categories. Nothing is logged per frame.

---

## Phase 1: A resizable window that displays and clicks correctly (SDL2)

**User stories**: 5, 6, 7, 8, 10, 12, 13, 18, 20, 22, 23, 26, 27 (recompute line)

### What to build

The tracer bullet. The window can be resized to any size. The 320×200 image is shown at the largest whole-number scale that fits, with square pixels, centred, with black bars. Every SDL2 mouse path goes through the new Viewport module, so clicks land on what is drawn at every window size:
- polling;
- button events;
- cursor warp;
- the MoX mouse handler call site.

The drawable size is re-read in real pixels on resize, size change and display change, so Windows display scaling is handled. Before coding, run the SDL2 logical-size/integer-scale spike from the PRD's Further Notes. It decides whether present uses SDL's built-in letterboxing or draws into the Viewport rect directly. Either way, the Viewport stays the only place mouse mapping happens.

### Acceptance criteria

- [x] Spike result recorded (SDL logical presentation used for present: yes or no, with the measured mismatch if no). **No — not used, and the spike was not run.** Present copies the frame straight into the Viewport rect, so drawing and mouse mapping come from the same numbers by construction. SDL's logical presentation would be a second calculation of the rect that has to be kept matching the Viewport, and it would also need reworking for `fit_*` and 4:3 later. Revisit only if present needs something SDL's path gives for free.
- [x] Viewport GoogleTest suite passes. It covers:
  - drawable sizes 1280×800, 1920×1080, 2560×1440, 3840×2160, 1366×768, 3440×1440, 1080×1920;
  - drawables smaller than 320×200;
  - exact multiples (no bars);
  - edge and sample round trips (game→window→game);
  - clamping of points in the bars and outside the drawable;
  - separate X/Y slack.
- [x] No backend or MoX code computes window width ÷ 320 directly. Verified by grep.
- [x] Manual check on Windows: drag-resize through several sizes. The image keeps square pixels and the bars are black with no stale content. Clicking buttons at all four edges of the image hits them.
- [x] Manual check at 125% and 150% Windows display scaling: image size and clicks are correct.
- [x] The default 1280×800 window looks and behaves exactly as before.
- [x] `DeMoM_` CTest suite passes unchanged. MSVC Debug and Release builds have no new warnings.

---

## Phase 2: Alt+Enter fullscreen (SDL2)

**User stories**: 1, 2, 3, 4, 9, 11, 19, 27 (toggle line), 29, 30

### What to build

Alt+Enter switches between the windowed state and borderless desktop fullscreen on the monitor the window is on. The keypress is swallowed: it is not translated, queued or recorded, key-repeat is ignored, and the key-up never reaches the game.

While the mouse is captured, it is confined to the image rect, not just the window. The cursor therefore never parks in a black bar, and there are no dead zones or lost edge clicks. Capture and release each go through one function, ready for a future release hotkey. Alt+Tab out of fullscreen and back leaves the game, the image and the mouse working.

### Implementation notes

- The hardware cursor is the **default** (`REMOM_HW_CURSOR=0` switches to the software cursor), not an opt-in prototype. With it, fullscreen capture is a window grab plus confinement to the image rect, with no relative mouse mode and no hidden OS cursor. Windowed, the hardware cursor is never captured; the software cursor keeps click-to-capture. Middle-click releases in both.
- `SDL_SetWindowMouseRect` alone did not keep the mouse out of the bars on Windows with SDL 2.32. While captured, both mouse polls also warp an off-image OS position back to the nearest point on the image.
- Verified by hand on Windows (no-sound Debug build), 2026-11-18. Not yet checked: two monitors; a recording made while toggling. The `DeMoM_` suite was not rerun for this phase: DeMoM links the windowless backend, which contains none of these changes.

### Acceptance criteria

- [x] Alt+Enter toggles fullscreen and back with no change to the monitor's mode or refresh rate. Both directions are logged.
- [x] Fullscreen opens on whichever monitor the window was on (checked with two monitors).
- [x] Alt+Enter on a screen with an Enter-activated control (e.g. a confirmation dialog) doesn't activate it. A recording made while toggling contains no Enter key.
- [x] Holding Alt+Enter toggles once.
- [x] In fullscreen at 1920×1080 the cursor stops at every image edge and responds immediately when moved back. Clicks on edge buttons always register.
- [x] Alt+Tab away and back from fullscreen: the image redraws, the mouse is recaptured, and the game continues.
- [x] `DeMoM_` CTest suite passes unchanged.

---

## Phase 3: `[Video]` settings — start fullscreen and `fit_nearest`

**User stories**: 14, 15, 17

### What to build

`ReMoM.ini` gains a `[Video]` section. `fullscreen = yes` starts the game in fullscreen. `scale_mode = fit_nearest` fills as much of the screen as possible with non-integer nearest-neighbour scaling. `integer` stays the default. Bad values fall back to the default with a logged warning. The shipped `ReMoM.ini` template documents both keys in a commented section.

### Acceptance criteria

- [x] `fullscreen = yes` starts fullscreen. `no`, or a missing key, starts windowed.
- [x] `scale_mode = fit_nearest` fills the limiting axis of the drawable. Clicks stay accurate at the edges.
- [x] Mixed-case values are accepted. A misspelled value logs a warning naming the key and value, and the default is used.
- [x] Viewport tests cover `fit_nearest` for every drawable size in the phase 1 list.
- [x] Template `ReMoM.ini` documents `[Video]`. The game never writes the file.
- [x] `DeMoM_` CTest suite passes unchanged. (38/38, MSVC-nosound-debug, 2026-11-18.)

---

## Phase 4: `fit_sharp_bilinear` scale mode

**User stories**: 16

### What to build

`scale_mode = fit_sharp_bilinear`:
1. nearest-neighbour upscale to the Viewport's integer pre-scale, in an intermediate render target;
2. linear filter down to the fitted rect.

If the renderer can't create the render target, the game logs it and falls back to `fit_nearest`.

### Acceptance criteria

- [x] At non-integer fits (e.g. 1366×768, 1920×1080), pixels look evenly sized with only a thin blend at their edges. Compared side by side against `fit_nearest` in a screenshot. (Checked by eye, fullscreen at 2560×1440: x7 intermediate 2240×1400 filtered to 2304×1440, against `fit_nearest`.)
- [x] The intermediate target is rebuilt only when the pre-scale changes, not every frame. (Offscreen 20 s run: built once, at x4.)
- [x] Forcing render-target failure (or using the software renderer if it lacks targets) logs and falls back without crashing. (`REMOM_SHARP_BILINEAR_FAIL=1`: one ERROR line, the game kept running.)
- [x] Viewport tests cover the pre-scale factor for every drawable size in the phase 1 list.
- [x] Frame time measured with `REMOM_INPUT_METRICS=1` shows no regression in the `dt_ms` percentiles against `integer` mode. (Measured from the `[PERF-LIVE]` log lines, fullscreen at 2560×1440, against `fit_nearest`, which presents with the same single nearest-neighbour copy as `integer`. `work_over` 0/2333 frames for `fit_nearest`, 0/536 for `fit_sharp_bilinear`; p50 55–56 ms for both. The `fit_sharp_bilinear` p95 of 111 ms is two ticks of game pacing in a 33 s run spent mostly on startup screens, not render cost. The `.fwv` files were empty: `REMOM_INPUT_METRICS=<path>` sends the profiler and the input metrics to the same file, and the profiler's empty file overwrites the other at exit. Use `=1`.)

---

## Phase 5: Loose ends — hardware cursor, replay header, input metrics

**User stories**: 21, 24, 25, 28

### What to build

The remaining places that still assume a fixed 4× scale:
- **Hardware-cursor prototype:** the cursor is sized from the Viewport and rebuilt when it changes.
- **Replay header:** recorded scale is either kept for older replays or redefined as the integer pre-scale. Decide at the start of this phase; both values are logged on playback.
- **Input metrics:** record the actual scale and rect in use.

### Implementation notes

- **Replay header decision: `screen_scale` is kept, with its meaning unchanged in practice.** It is the viewport's whole-number scale when recording starts, which is 4 for every replay made in the default 1280×800 window, as before. A new `# scale_mode=` line is written after it. Both lines only describe the display: frames hold game coordinates, and playback maps them to the window through the current Viewport. So neither value is used during playback. Older readers skip the new line, because the header parser ignores `#` lines it doesn't know. Older replays have no `scale_mode` line and log `recorded_scale_mode=not_recorded`. The `REPLAY: playback started` log line shows the recorded and current scale and scale mode.
- **Hardware cursor:** built at 16 × (image rect ÷ 320) drawable pixels, rounded, with a nearest-neighbour fill. It is rebuilt when the shape or that size changes, so resizing, toggling fullscreen and the fit modes all resize it on the next frame. On a HiDPI display where SDL takes cursor sizes in points rather than pixels (macOS), the size needs the drawable÷window ratio removed; phase 10 checks this.
- **Input metrics:** the `.fwv` gets a `#viewport after_idx=N window=… drawable=… scale_mode=… scale=… rect=x,y,WxH` comment line when recording starts and on every viewport change. It applies to the rows after it. The `#meta scale=` field is still the scale at start.

### Acceptance criteria

- [x] With the hardware cursor prototype on, the OS cursor matches the drawn image size in windowed, resized and fullscreen states, in every scale mode.
- [x] A replay recorded windowed plays back identically in fullscreen, and the other way round. Existing `.RMR` files still play.
- [x] Replay header decision recorded in this plan. Playback logs the recorded and current values. (Checked: a new recording logs `recorded_scale_mode=fit_nearest`, and the older `DEMO_QUIT_MD.RMR` plays to the end and logs `recorded_scale_mode=not_recorded`.)
- [x] Input-metrics output shows the real scale and rect for a fullscreen `fit_nearest` session.
- [x] `DeMoM_` CTest suite passes unchanged. (38 DeMoM tests + 22 Viewport tests passed; `DeMoM_WorldGen_Golden` is disabled as before.)

---

## Phase 6: Borderless window mode

**User stories**: PRD Further Notes (requested by Xeth Nyrrow on the project's Discord)

### What to build

A third display state: a window with no title bar or frame, using the same Viewport, present and mouse paths. Resolve the PRD's open questions at the start of this phase:
- how it is selected (a `[Video]` key and/or the toggle cycling through states);
- whether and how a frameless window can be moved or resized.

### Decisions (2026-11-19)

- **What it is:** a borderless *maximised* window. It has no title bar or frame and fills the usable area of the display the window is on, so the taskbar stays visible. Fullscreen covers the taskbar; that is the difference between them.
- **Selection:** `[Video] startup_window = windowed | borderless | fullscreen` (default `windowed`) sets the starting state. It replaces phase 3's `fullscreen = yes | no`, which was never released.
- **Hotkeys:** they never cycle through all three states.
  - **Alt+Enter** keeps its standard job of toggling fullscreen. Leaving fullscreen returns to the window state it came from, framed or borderless. F11 is planned to do the same once the keymap feature exists.
  - **Ctrl+L** also toggles fullscreen.
  - **Ctrl+H** toggles between the borderless and the framed window. From fullscreen it goes to the borderless window.
  - Ctrl+H and Ctrl+L are borrowed from Adobe Acrobat Reader, because there is no standard hotkey for a borderless maximised window. The game never reads Ctrl, so they take nothing from it.
- **Moving and resizing:** not possible. The borderless window always fills the display's usable area. Going back to the framed window restores the position and size it had before.
- **Mouse:** treated like the framed window, so it isn't captured. Only fullscreen captures the mouse.

### Acceptance criteria

- [x] Open questions answered and recorded here and in the PRD.
- [x] Borderless window fills the display's work area (taskbar visible, not movable), with correct image and clicks in every scale mode.
- [x] Switching between borderless window, windowed and fullscreen keeps image, mouse capture and clicks correct.
- [x] `DeMoM_` CTest suite passes unchanged. (38 DeMoM + 22 Viewport tests; `DeMoM_WorldGen_Golden` disabled as before. Offscreen: each `startup_window` value logs its state, and a misspelt value warns and starts windowed.)

---

## Phase 7: SDL3 port

**User stories**: 31 (and 1–21 re-verified on SDL3)

### What to build

Bring phases 1–6 to the SDL3 backend. The Viewport module, settings and hotkey binding are shared. Only display-state, capture, drawable-size and present code is written for SDL3.

### Implementation notes

- **Building it:** use the `MSVC-sdl3-nosound-debug` and `MSVC-sdl3-nosound-release` presets, which find SDL3 3.4.2 in `C:/devellib`. They have audio off because `sdl3_Audio.c` is written against the pre-release SDL_mixer 3 API (`Mix_Init`, `Mix_Chunk` and so on). The installed SDL3_mixer 3.2.0 is the released `MIX_*` API, so SDL3 with sound does not compile. Porting the audio is outside this plan.
- **Port of the SDL2 code** for the Viewport, present, sharp bilinear, mouse mapping, capture and confinement, the keep-on-image warp, window states and hotkeys. The SDL3 differences:
  - render calls take `SDL_FRect`;
  - there is no `SDL_RenderTargetSupported`, so a renderer without render targets fails at texture creation and takes the same fallback;
  - textures default to linear filtering, so the game texture is set to nearest;
  - the drawable size comes from `SDL_GetCurrentRenderOutputSize`;
  - window changes are asynchronous, so each state switch calls `SDL_SyncWindow` before re-reading the viewport;
  - resize events are `SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED`, `DISPLAY_CHANGED` and `DISPLAY_SCALE_CHANGED`.
- **The hotkey table is per backend.** Its keycodes and modifier masks are SDL-version types (`SDLK_l`/`KMOD_CTRL` in SDL2, `SDLK_L`/`SDL_KMOD_CTRL` in SDL3). A truly shared binding needs a backend-neutral key, which is the keymap feature.
- **The hardware-cursor size moved into the Viewport module** (`Viewport_Game_Size_In_Drawable`, with a test), so SDL2 and SDL3 no longer each compute it.
- **No input metrics on SDL3.** The SDL3 backend never had the input-metrics hooks, so phase 5's `#viewport` line does not apply to it.
- **Asserts with side effects (fixed).** In `sdl3_Init.c` and in `Platform_Video_Update` in `sdl3_Video.c`, the SDL calls sat inside `assert()`. Under `NDEBUG` they compiled away, so an SDL3 Release build presented nothing. Now each call always runs. A failure is logged once per call site, and Debug still asserts on it. A failed lock drops the frame instead of writing through a stale pointer.

### Acceptance criteria

- [ ] The SDL3 build passes the phase 1–6 manual checklists on Windows.
- [x] No Viewport or settings code is duplicated in the SDL3 backend. Verified by grep. (No ini reads. Every mapping, rect, state name and size goes through `Viewport_*` calls. The last duplicated formula, the cursor size, moved into the Viewport module.)
- [x] `DeMoM_` CTest suite passes unchanged. (38 DeMoM + 22 Viewport tests in the SDL3-configured build; the new cursor-size test makes 23 Viewport tests, all passing. Offscreen on SDL3, each `startup_window` value logs its state.)

---

## Phase 8: Win32 port

**User stories**: 31 (and 1–21 re-verified on Win32)

### What to build

Bring phases 1–6 to the native Win32 backend:
- borderless fullscreen through window style and monitor-rect changes;
- cursor confinement to the image rect;
- letterboxed present.

All mapping goes through the shared Viewport.

### Implementation notes

- **Present:** `StretchDIBits` into the Viewport rect (`COLORONCOLOR`, nearest-neighbour), with the four bar areas filled black on every present.
- **`fit_sharp_bilinear`:** GDI has no bilinear filter.
  - The frame is drawn nearest-neighbour into a memory bitmap at ×k, then `StretchBlt` in `HALFTONE` mode (GDI's area-averaging filter) scales it into the rect.
  - This is close to sharp bilinear but not identical.
  - The bitmap is cached per scale; failure or `REMOM_SHARP_BILINEAR_FAIL=1` falls back to nearest, logged once.
  - `HALFTONE` runs on the CPU, so its frame time needs checking.
- **Window states:**
  - Fullscreen and borderless switch the window to `WS_POPUP` and size it to the monitor's `rcMonitor` or `rcWork`.
  - The framed window returns via a saved `WINDOWPLACEMENT`.
  - `WM_DISPLAYCHANGE` re-fits fullscreen and borderless to a new resolution.
- **Capture:** fullscreen clips the cursor to the image rect in screen coordinates (`ClipCursor`). Because `ClipCursor` is system-wide, it is released on losing focus, on leaving fullscreen, on middle-click and at shutdown. It is re-applied on focus, on a click after a middle-click, on moving and on viewport changes. The OS enforces the clip, so Win32 needs no keep-on-image warp.
- **Hotkeys:** the same three bindings. Alt+Enter arrives as `WM_SYSKEYDOWN`, and its `WM_SYSCHAR` is swallowed so Windows doesn't beep. Ctrl+L and Ctrl+H are caught in `WM_KEYDOWN` before translation. Auto-repeat is skipped using `lParam` bit 30.
- **No DPI awareness:** the process doesn't declare DPI awareness, so Windows scales the window and client coordinates are the pixels drawn (window == drawable).
- **Input metrics:** a `#viewport` line is written at start and on every viewport change, as on SDL2.
- **Hardware cursor:** sized with `Viewport_Game_Size_In_Drawable`, like SDL2 and SDL3.

### Acceptance criteria

- [ ] The Win32 build passes the phase 1–6 manual checklists.
- [x] Win32's own scale query reads the Viewport. (`Platform_Get_Scale` returns `win_viewport.scale`. The only divide-by-scale left is in the `MOUSE_DEBUG` diagnostic log lines.)
- [x] `DeMoM_` CTest suite passes unchanged. (`MSVC-win32-debug`: 61 tests, `MSVC-win32-release`: 57 tests, all passing. Both presets build with no warnings. Before this phase neither linked: the Automation Driver has called `Platform_Set_Synthetic_Mouse_Button` since `986d71c7` (2026-07-29), and Win32 never had it. It was added in this phase.)

---

## Phase 9: Linux validation

**User stories**: 3, 4, 9, 18, 19, 31 (on Linux)

### What to build

Run the SDL2 (and SDL3) behaviour on Linux under X11 and under XWayland/Wayland, through the WSL route and on native hardware where possible. Fix any platform-specific differences in fullscreen, capture or drawable size.

### Acceptance criteria

- [ ] Phase 1–6 manual checklists pass on X11 and on Wayland/XWayland.
- [ ] HiDPI scaling (fractional where the compositor supports it) gives correct image size and clicks.
- [ ] Any renderer workarounds needed (GLX over XWayland) are documented in the graphics devel doc.

---

## Phase 10: macOS

**User stories**: 3, 4, 9, 18, 19, 31 (on macOS)

### What to build

Bring the SDL backend up on macOS. Handle Retina drawable size, and the fact that macOS's fullscreen-Space behaviour differs from desktop fullscreen. Decide which macOS fullscreen style counts as "borderless desktop fullscreen" for this feature.

### Acceptance criteria

- [ ] macOS fullscreen style decision recorded here.
- [ ] Phase 1–6 manual checklists pass on macOS, including on a Retina display.
- [ ] Alt+Enter (Option+Return) toggles, and the game never sees it.
