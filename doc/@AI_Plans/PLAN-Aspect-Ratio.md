# Plan: 4:3 Aspect Ratio

> Source PRD: `PRD-Aspect-Ratio.md`. Builds on the Viewport, video settings and backends from `PLAN-Fullscreen.md`. The phases are numbered 11–14 to continue from that plan's phases 1–10: one effort.

## Architectural decisions

These apply to every phase:

- **The setting:** `[Video] aspect_ratio = 16:10 | 4:3` in `ReMoM.ini`.
  - The default is `16:10` (square pixels, as today). Matching is case-insensitive.
  - An unknown value logs a warning and the default is used.
  - It is read once at startup. There is no hotkey, and the game never writes it.
- **The Viewport is the only place the 4:3 math lives.**
  - The pixel aspect stays an enum (square and 4:3), passed through the existing `Viewport_Compute` interface.
  - At 4:3 the image's target shape is 320 × 240 in game-pixel widths: x scale k, y scale 1.2k.
  - Backends never compute pixel shapes themselves.
- **The rect at 4:3, per scale mode (always centred):**
  - `integer`: the largest whole-number k for which 320k × 240k fits. The shape is exact 4:3, and rows are uneven except at ×5.
  - `fit_nearest` and `fit_sharp_bilinear`: the largest 4:3 rect that fits.
  - `fit_sharp_bilinear` keeps a single whole-number intermediate scale; the final filter does the vertical stretch.
- **The row rule:** a drawable pixel belongs to the game pixel its centre falls in. The window→game mapping uses this rule, and every backend's drawing must match it. Each backend phase checks that it does.
- **Windowed size:** 1280×960 at 4:3, 1280×800 at 16:10.
- **Unchanged:** game logic, the 320×200 framebuffer, the software cursor (it stretches with the image), the palette, screen capture, replay coordinates and the windowless backend (square, scale 1). No original game code changes.
- **Regression guard:** the full `DeMoM_` CTest suite and the existing Viewport tests pass unchanged in every phase.
- **Logging:** the aspect ratio goes into the existing startup `[Video]` line and the Viewport recompute line. Nothing is logged per frame.

---

## Phase 11: 4:3 on SDL2, end to end

**User stories**: 1–17, 20, 21, 24, 27, 28, 30–34

### What to build

A player sets `aspect_ratio = 4:3` and the SDL2 build shows the game at 4:3:
- in every scale mode and every window state;
- in a 1280×960 window;
- with every click landing on the game pixel drawn under the cursor, including rows drawn taller than others.

It starts with the row check: confirm that SDL2's GPU renderer and SDL's software renderer give each drawable row the same game row as the row rule. Use a 4:3 rect that isn't ×5, such as 1280×960 (rows 4 or 5 pixels tall). Record the result here before writing the backend code. If a renderer rounds differently, decide here whether the rule follows it or that path draws the rows itself.

The Viewport gains the 4:3 aspect, its name functions and unit tests. Startup reads the key and the video settings carry it. SDL2 passes it into every recompute and sizes its window from it. Mouse capture and the keep-on-image warp follow the 4:3 rect automatically.

### Row check result (2026-11-19)

The new `remom_video_probe --row-check [RD]` mode measures this. For each render driver it copies a 320×200 texture, where every texel encodes its own x and y, into six rects with NEAREST scaling. It does this on a render target in a hidden window, reads the pixels back, and checks every drawable row and column against the two candidate rules.

| Rect (rows, columns per game pixel) | D3D11 | D3D9 | OpenGL / GLES2 | software |
|---|---|---|---|---|
| 4:3 ×4, 1280×960 (4.8, 4) | centre | centre | centre | centre |
| 4:3 ×3, 960×720 (3.6, 3) | centre | centre | centre | centre |
| 4:3 ×5, 1600×1200 (6, 5) | both | both | both | both |
| 4:3 fit, 1440×1080 (5.4, **4.5**) | centre | rows centre; ties mixed | rows centre; ties mixed | rows centre; ties go down |
| 4:3 fit, 1067×800 (4, 3.33) | centre, 2 columns off | centre | centre | centre, 6 columns off |
| 16:10 fit, 1728×1080 (5.4, 5.4) | centre | centre | centre | centre |

`direct3d12` isn't available in this SDL2 build.

- **Every driver draws by the centre rule:** a drawable pixel shows the game pixel its centre falls in.
- **They differ only on exact ties,** where a pixel centre sits exactly on a boundary between two game pixels. That happens at 4.5 per game pixel, and each driver breaks the tie its own way. Float or fixed-point rounding also moves a handful of pixels next to a boundary.
- **Decision:** the Viewport uses the centre rule, and an exact tie goes to the later game pixel, as Direct3D 11 does. No backend draws the rows itself. The worst case is a click one game pixel off, on the single drawable pixel at a seam, where a game pixel is 3 or more drawable pixels wide.
- **This also fixes the 16:10 fit modes.** The old mapping used the edge rule, which disagreed with the drawing on 80 of 1080 rows and 128 of 1728 columns at 1920×1080 `fit_nearest`. `integer` at 16:10 is unaffected, because there every boundary is on a whole pixel.

### Implementation notes

- **Rects:** at 4:3 the Viewport's target shape is 320×240.
  - `integer`: k = min(width ÷ 320, height ÷ 240), and the rect is 320k × 240k.
  - The fit modes: the largest 4:3 rect that fits.
  - The whole-number `scale`, used for the sharp-bilinear pre-scale, is now the largest k at which 320×200 fits inside the rect. At 16:10 this is the same number as before.
- **Mapping:** `Window_To_Game`, `Game_To_Window` and `Image_Rect_In_Window` all use the centre rule (window coordinate w counts as w + ½), so they stay consistent with each other.
- **Settings:** `Platform_Set_Video_Settings` takes the pixel aspect, and `Platform_Video_Pixel_Aspect()` reads it back. SDL3 and Win32 still pass square until phases 13 and 14.
- **SDL2:** the window opens at 1280×960 at 4:3, and the viewport log line names the aspect.

### Acceptance criteria

- [x] Row check result recorded here, for the SDL2 GPU renderer and the software renderer.
- [x] The Viewport unit tests cover:
  - 4:3 rects in every scale mode at 1280×960, 1280×800, 1920×1080, 2560×1440 and 1600×1200, plus a minimised window and a drawable smaller than the image;
  - exactly 6 drawable rows per game row at ×5;
  - monotonic rows, with every game row covered, at other scales;
  - round trips at 4:3 and at high DPI;
  - bars reported as off the image;
  - the in-window image rect equals the on-image set.
- [x] The existing Viewport tests pass unchanged. (34 tests: the 23 existing ones, unchanged, plus 11 new.)
- [x] A unit test covers the `aspect_ratio` names: `16:10` and `4:3` in any case are recognised, and other values are rejected.
- [x] An offscreen run with an unknown value logs the warning and uses `16:10`. The startup line shows all three `[Video]` settings. (`aspect_ratio = 4x3`: `is not 16:10 or 4:3 -- using 16:10`. Offscreen 4:3 runs, windowed, borderless and fullscreen in each scale mode, log a 1280×960 window and the expected rects.)
- [x] `ReMoM.ini` documents the key, with a commented-out default line.
- [ ] Manual check on Windows, SDL2, `aspect_ratio = 4:3`:
  - every scale mode, in windowed, borderless and fullscreen;
  - resizing keeps the image 4:3 and centred;
  - clicks at the four image edges and on a taller row hit the game pixel drawn there;
  - clicks in the bars are ignored;
  - the captured mouse stays on the image.
- [ ] With the key absent or set to `16:10`, everything looks and behaves exactly as before. One intended exception: in the 16:10 fit modes, clicks now follow the centre rule (see the row check), which is the game pixel actually drawn under the cursor. Offscreen, a missing key and `16:10` both log the same 1280×800 window and rect as before.
- [x] `DeMoM_` CTest suite passes unchanged. MSVC Debug and Release builds have no new warnings. (`MSVC-debug`: 72/72 `DeMoM_|Viewport`. `MSVC-release`: 34/34 Viewport.)

---

## Phase 12: Cursor, replay and metrics at 4:3 (SDL2)

**User stories**: 18, 19, 25, 26, 29, 35

### What to build

Everything that reads the image's shape outside the drawing itself is correct at 4:3:
- the hardware cursor is the shape and size of the stretched game pixels;
- the software cursor stretches with the image;
- replays record the aspect ratio in their header, and play back the same at either aspect;
- the input-metrics viewport line shows the aspect;
- Automation Driver clicks land on the same game pixels at 4:3 as at 16:10.

### Implementation notes

- **Cursors needed no change.** The hardware cursor (SDL2, SDL3 and Win32) is already sized by `Viewport_Game_Size_In_Drawable` and filled nearest-neighbour per axis, so at 4:3 it is 1.2 times taller than wide. The software cursor is drawn into the 320×200 frame and stretches with it.
- **Replay:** the header gains `# aspect_ratio=16:10|4:3` after `scale_mode`. Playback logs `recorded_aspect_ratio` and `current_aspect_ratio`; replays without the line read as `not_recorded`.
- **Input metrics:** `Input_Metrics_Note_Viewport` takes the aspect name, and the `#viewport` line has `aspect_ratio=`. The SDL2 and Win32 callers pass it. SDL3 has never had input metrics, which predates this plan.

### Acceptance criteria

- [x] The cursor-size unit test at 4:3 gives a block taller than it is wide by the image's ratio. (16×16 at 1280×960 integer is 64×77; at ×5 it is 80×96.)
- [ ] Manual check: with the hardware cursor on, the OS cursor matches the drawn image's pixel shape in every scale mode and window state.
- [x] A new replay's header has an `aspect_ratio` line. Older replays play, and log the aspect as "not recorded". (`DEMO_QUIT_MD.RMR`: `recorded_aspect_ratio=not_recorded`.)
- [x] A replay recorded at 4:3 plays back the same at 16:10, and the other way round. (Offscreen, `Stage_0.hms` recorded at 4:3 plays all 519 of 519 frames at 16:10, on SDL2, and 525 of 525 on SDL3. DeMoM's replay tests pass.)
- [x] The input-metrics `#viewport` line includes the aspect ratio. (`#viewport after_idx=0 window=1280x960 drawable=1280x960 scale_mode=fit_nearest aspect_ratio=4:3 scale=4 rect=0,0,1280x960`.)
- [x] An Automation Driver scenario clicks the same game pixels at 4:3 and at 16:10 (offscreen). (`Stage_0.hms` with `--seed1 12345`: the 15 recorded click positions are identical at both aspects and equal the scenario's own coordinates, on SDL2 and SDL3.)
- [x] `DeMoM_` CTest suite passes unchanged. (73/73 `DeMoM_|Viewport` on `MSVC-debug`, `MSVC-sdl3-nosound-debug` and `MSVC-win32-debug`.)

---

## Phase 13: SDL3 port

**User stories**: 22

### What to build

The SDL3 backend gets phases 11–12:
- it passes the aspect into its recompute and sizes its window from it;
- its cursor, capture and warp follow the 4:3 rect;
- the row check is run for SDL3's renderer, where the game texture is set to nearest-neighbour explicitly.

### Implementation notes

- SDL3 passes `Platform_Video_Pixel_Aspect()` into its recompute, opens a 1280×960 window at 4:3, and names the aspect in its viewport log line.
- The row check for SDL3 is a new tool, `remom_row_check_sdl3`. It is built when `USE_SDL3` is on and shares `tools/remom_row_check.h` with the SDL2 probe and the GDI tool.

### Row check result (SDL3 3.4.2, 2026-11-20)

The results are identical to SDL2's, driver for driver (see phase 11):
- Direct3D 11, Direct3D 9, OpenGL, OpenGL ES 2 and software all follow the centre rule on every 4:3 row.
- They differ only on the exact-tie columns at 4.5 per game pixel, and on 2 to 6 rounding columns in the 1067-wide case.

`direct3d12`, `vulkan` and `gpu` aren't available on this machine. The centre rule needs no change for SDL3.

### Acceptance criteria

- [x] Row check result recorded here for SDL3.
- [x] `MSVC-sdl3-nosound-debug` and `-release` build with no new warnings. Tests pass. (Debug: 73/73 `DeMoM_|Viewport`. Release: 35/35 Viewport.)
- [x] Offscreen runs at 4:3 in every scale mode and window state log the expected rect. (They are the same as SDL2: a 1280×960 window, and 960×720 at 32,24 on the 1024×768 dummy display for `integer`. An unknown value falls back to 16:10. In one batch run the borderless case logged nothing; it logged the expected rects on both re-runs, and the cause wasn't found.)
- [ ] The phase 11–12 manual checklists pass on SDL3.

---

## Phase 14: Win32 port

**User stories**: 23

### What to build

The Win32 backend gets phases 11–12:
- it passes the aspect into its recompute and sizes its window from it;
- `StretchDIBits` draws into the 4:3 rect;
- the HALFTONE sharp-bilinear path filters into the 4:3 rect;
- `ClipCursor` and the cursor follow the 4:3 rect;
- the row check is run for GDI `StretchDIBits` with `COLORONCOLOR`.

The backend stays plain GDI.

### Implementation notes

- Win32 passes `Platform_Video_Pixel_Aspect()` into its recompute, and sets the window height to 960 at 4:3 before the window is created.
- The aspect is named in its viewport log line and in its metrics `#viewport` line.
- `StretchDIBits`, the HALFTONE sharp-bilinear path, `ClipCursor` and the cursor already work from the Viewport rect, so they follow 4:3 unchanged.
- The row check for GDI is a new tool, `remom_row_check_gdi` (Windows only). It makes the same `SetStretchBltMode(COLORONCOLOR)` + `StretchDIBits` calls the Win32 present makes for `integer` and `fit_nearest`, drawing into a 32-bit DIB section in a memory DC. No window is involved.
- `fit_sharp_bilinear` draws at a whole-number scale first, which is exact, then filters with HALFTONE, so there is no nearest-neighbour row to check in that mode.

### Row check result (GDI, Windows 10, 2026-11-20)

GDI follows the centre rule on every row and every column of every rect. It is the only backend with no exceptions: exact ties go to the later game pixel, which is the Viewport's choice, and there is no rounding drift.

### Acceptance criteria

- [x] Row check result recorded here for GDI.
- [x] `MSVC-win32-debug` and `-release` build with no new warnings. Tests pass. (Debug: 73/73 `DeMoM_|Viewport`. Release: 35/35 Viewport.)
- [ ] The phase 11–12 manual checklists pass on Win32. (Win32 has no offscreen mode, so nothing at 4:3 has been run in a window yet.)
