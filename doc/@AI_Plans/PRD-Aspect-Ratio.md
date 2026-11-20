# PRD — 4:3 Aspect Ratio

> Follows `PRD-Fullscreen.md`, which left non-square aspects out of scope. Builds on the Viewport module from `PLAN-Fullscreen.md`.

## Problem Statement

Master of Magic draws a 320×200 image. In 1994 that image filled a 4:3 CRT, so every game pixel was 1.2 times taller than it was wide. ReMoM shows the game pixels square, so the whole image is 16:10.

Square pixels make the game look slightly flat and wide next to how it looked on the original hardware. Circles look like ovals, and portraits, fonts and the world map look a little squashed. Players who remember the original, or who compare against DOSBox with aspect correction on, see the difference.

There is no way to get the original shape today. Every scale mode (`integer`, `fit_nearest`, `fit_sharp_bilinear`) keeps pixels square.

## Solution

Add a `[Video] aspect_ratio` key to `ReMoM.ini`, with two values:

- `aspect_ratio = 16:10`: square pixels, as today. This is the default, so nothing changes for existing players.
- `aspect_ratio = 4:3`: the image is shown at 4:3, the shape it had on a CRT. Each game pixel is 1.2 times taller than it is wide.

The setting is read once at startup, like `scale_mode`. It works with every scale mode and every window state (windowed, borderless, fullscreen). With `4:3`, the windowed window opens at 1280×960, so the image fills it exactly.

The mouse, the hardware cursor, the Automation Driver and replays all keep working at 4:3, because they all map through the same Viewport the image is drawn with.

**How each scale mode looks at 4:3:**

- `integer`: the image is exactly 4:3, at the largest whole-number width that fits (320×k wide, 240×k tall). Rows are drawn nearest-neighbour, so some rows are one pixel taller than others. The exception is ×5 (1600×1200), where every game row is exactly 6 pixels tall. Columns are always even.
- `fit_nearest`: the largest 4:3 rect that fits, nearest-neighbour. Rows and columns can both be uneven, as they already are in this mode.
- `fit_sharp_bilinear`: the largest 4:3 rect that fits. The image is drawn nearest-neighbour at a whole-number scale, then filtered to the rect, which hides the uneven rows. This is the mode to recommend for 4:3.

## User Stories

1. As a player, I want to set `aspect_ratio = 4:3`, so that the game looks the shape it did on my CRT in 1994.
2. As a player, I want the default to stay `16:10`, so that updating ReMoM does not change how my game looks.
3. As a player, I want the key in the same `[Video]` section as `scale_mode` and `startup_window`, so that all the display settings are in one place.
4. As a player, I want `ReMoM.ini` to explain what `16:10` and `4:3` mean, so that I can pick one without reading the source.
5. As a player, I want the value to be case-insensitive, so that a capital letter does not stop it working.
6. As a player who types an unknown value, I want a warning in the log and the `16:10` default, so that a typo never stops the game from starting.
7. As a player using 4:3 in `integer` mode, I want the image to be exactly 4:3, so that the shape is right even if some rows are a pixel taller.
8. As a player using 4:3 in `integer` mode, I want the largest whole-number width that fits, so that the image is as big as possible.
9. As a player using 4:3 in `fit_nearest` mode, I want the largest 4:3 image that fits my window or screen.
10. As a player using 4:3 in `fit_sharp_bilinear` mode, I want the largest 4:3 image that fits, with even-looking pixels, so that it looks like a CRT without blocky uneven rows.
11. As a player using 4:3 in a window, I want the window to open at 1280×960, so that the image fills it with no black bars.
12. As a player using 16:10 in a window, I want the window to open at 1280×800 as it does today.
13. As a player using 4:3 in fullscreen or borderless, I want the image centred with black bars at the sides on a widescreen display.
14. As a player resizing a 4:3 window, I want the image to stay 4:3 and stay centred, so that resizing never distorts it.
15. As a player using 4:3, I want every click to land on the game pixel under the cursor, including on rows that are drawn taller than others.
16. As a player using 4:3, I want clicks in the black bars to be ignored, as they are at 16:10.
17. As a player using 4:3 with mouse capture in fullscreen, I want the mouse kept on the 4:3 image, so that it never sits in a bar.
18. As a player using 4:3 with the hardware cursor, I want the OS cursor to be the same shape and size as the drawn image's pixels, so that it does not look squashed next to the game.
19. As a player using 4:3, I want the software cursor to stretch with the image, so that it looks like the rest of the game.
20. As a player using Alt+Enter, Ctrl+L or Ctrl+H at 4:3, I want the image to stay 4:3 in every window state.
21. As a player on SDL2, I want 4:3 to work.
22. As a player on SDL3, I want 4:3 to work the same way.
23. As a player on the Win32 build, I want 4:3 to work the same way.
24. As a player with a high-DPI display, I want 4:3 to give the same image shape and correct clicks as at 100% scaling.
25. As a player recording a replay at 4:3, I want it to play back the same at 16:10, and the other way round, because replays hold game coordinates.
26. As a developer reading a replay, I want the header to record the aspect ratio it was made at, so that I can tell a display difference from a game difference.
27. As a developer, I want the log to say which aspect ratio is in use at startup, next to the scale mode and window state.
28. As a developer, I want the Viewport recompute log line to include the aspect ratio, so that a wrong rect can be traced to its inputs.
29. As a developer, I want the input-metrics `#viewport` line to include the aspect ratio, so that metrics runs at different aspects can be told apart.
30. As a developer, I want all the 4:3 math in the Viewport module, so that no backend computes pixel shapes itself.
31. As a developer, I want the mouse mapping to use the same row rule as the drawing, so that what is drawn and what is clicked agree on every row.
32. As a developer, I want unit tests for the 4:3 rects, mapping and cursor size, so that a change to the Viewport cannot quietly break 4:3.
33. As a developer, I want a unit test for the `aspect_ratio` names, so that the ini parsing is checked without starting the game.
34. As a developer running the DeMoM tests, I want every golden file to stay unchanged, so that I know the aspect work stayed in the platform layer.
35. As a modder or automation user, I want Automation Driver clicks to land on the same game pixels at 4:3 as at 16:10.

## Implementation Decisions

### Modules

- **Viewport (shared, pure C, already exists).** The deep module. It gains a 4:3 pixel aspect beside the existing square one. The interface does not change: `Viewport_Compute` already takes a pixel aspect, and the image rect, the window↔game mapping and the cursor-size function already work per axis.
  - **Rect at 4:3:** the image's target shape is 320 × 240 game-width units: x scale k, y scale 1.2k.
    - `integer`: the largest whole number k for which 320k × 240k fits the drawable; the rect is 320k × 240k, centred.
    - `fit_nearest` and `fit_sharp_bilinear`: the largest 4:3 rect that fits, centred.
    - The existing "image larger than the drawable" handling (negative offsets) applies unchanged.
  - **The whole-number scale** that `fit_sharp_bilinear` uses for its intermediate image stays a single number for both axes. The final filter to the 4:3 rect is what stretches it vertically.
  - **Row rule:** the window→game mapping and the drawing must assign each drawable row to the same game row. The Viewport defines the rule: a drawable pixel belongs to the game pixel its centre falls in. Each backend must draw so that it matches this rule. See the spike in Further Notes.
  - **Names:** new name functions for the aspect ratio, matching the existing scale-mode and window-state ones: `16:10` and `4:3`, case-insensitive, unknown → not recognised; plus a normalise function and a log-name function.
- **Video settings (shared, already exists).** It gains the aspect ratio beside the startup window state and the scale mode: the application sets it before the platform starts, and the backends read it.
- **Application startup.** Reads `[Video] aspect_ratio` with the other `[Video]` keys. An unknown value logs a warning naming the valid values and uses `16:10`. The startup log line lists all three settings.
- **SDL2, SDL3 and Win32 backends.**
  - Pass the aspect ratio into every Viewport recompute.
  - Open the windowed window at 1280×960 when the aspect is 4:3, and at 1280×800 otherwise.
  - Present into the Viewport rect as today. SDL scales the texture into a non-uniform rect, and GDI's `StretchDIBits` does the same.
  - Nothing else in the backends should need to change: the mouse mapping, capture rect, keep-on-image warp and hardware cursor size already read the Viewport per axis.
- **Replay.** The header gains an `aspect_ratio` line beside `scale_mode`. Playback logs the recorded and current values. Replays without the line read as "not recorded". Frames hold game coordinates, so playback never uses it to map the mouse.
- **Input metrics.** The `#viewport` comment line gains the aspect ratio.
- **`ReMoM.ini`.** The `[Video]` section documents `aspect_ratio = 16:10 | 4:3` (default `16:10`), with a commented-out default line, matching the other keys.

### Architectural decisions

- **The default is `16:10`.** Players who don't set the key see no change.
- **The setting is read once at startup.** There is no hotkey, and the game never writes it.
- **The only choices are `16:10` and `4:3`.** The internal pixel aspect stays an enum, so another value can be added later without an interface change.
- **At 4:3, `integer` means an exact 4:3 shape with a whole-number width,** not a whole-number scale on both axes. Uneven rows are accepted. `fit_sharp_bilinear` is the recommended mode for 4:3.
- **Game logic, the 320×200 framebuffer, the software cursor, the palette, screen capture and replay coordinates are unchanged.** No original game code changes.
- **The windowless backend stays square at scale 1,** so DeMoM is unaffected.

## Testing Decisions

- **What a good test is here:** it checks external behaviour, for example "given this drawable size, scale mode and aspect, the image lands here, and this window pixel maps to that game pixel". It does not check how the Viewport works that out.
- **Viewport GoogleTest cases**, following the existing Viewport tests:
  - 4:3 rects in every scale mode, at common sizes: 1280×960, 1280×800, 1920×1080, 2560×1440, 1600×1200, a minimised window, and a drawable smaller than the image.
  - At ×5 (1600×1200), every game row maps from exactly 6 drawable rows.
  - At other scales: every drawable row inside the rect maps to a game row, rows are monotonic, and every game row gets at least one drawable row.
  - Window→game→window round trips land on the same game pixel at 4:3, and at high DPI (window ≠ drawable).
  - Points in the bars are reported as off the image.
  - The image rect in window coordinates is exactly the set of points reported as on the image.
  - Cursor size at 4:3 is taller than it is wide by the image's ratio.
  - The 16:10 results are unchanged: the existing tests pass as they are.
- **Name parsing test:** `16:10` and `4:3` are recognised in any case, and unknown values are rejected. The "warn and use 16:10" behaviour lives at startup and is checked with an offscreen run's log.
- **Not automated (manual):** how 4:3 looks in each backend, window state and scale mode, plus clicks on the image edges and on rows drawn taller, and the hardware cursor shape. These follow the fullscreen plan's manual checklists, with `aspect_ratio = 4:3` set.
- **Regression guard:** the full `DeMoM_` CTest suite passes unchanged.

## Out of Scope

- Any aspect other than `16:10` and `4:3`, including stretch-to-fill and 16:9.
- Switching the aspect while playing (a hotkey), and saving it back to `ReMoM.ini`.
- An in-game options screen.
- Whole-number scaling on both axes that comes close to 4:3 (for example ×4 wide, ×5 tall).
- CRT effects such as scanlines, shaders or curvature (see `PRD-Graphics-Upscale.md` for upscaling).
- macOS. It follows the fullscreen plan's phase 10.

## Further Notes

- **Spike before coding:** check that each backend's nearest-neighbour drawing gives each drawable row the same game row that the Viewport's row rule does, at a 4:3 rect that isn't ×5 (for example 1280×960, where rows are 4 or 5 pixels tall). Check in this order:
  1. SDL2 and SDL3's GPU renderers;
  2. SDL's software renderer;
  3. GDI's `StretchDIBits` with `COLORONCOLOR`.

  If any of them rounds differently, either the Viewport's rule follows that backend, or that backend draws the rows itself. Record the result in the plan. A one-row disagreement shows up as clicks hitting the row above or below the one drawn under the cursor.
- **The old branch `origin/feature/present-aspect-fullscreen` (`7dcc129c`)** showed the image at 4:3 by default, with a 1280×960 window, set through environment variables. Its present path can be looked at for ideas. Its defaults and its environment-variable settings are replaced by the decisions above.
- **Logging:** the aspect ratio goes into the existing startup line and the Viewport recompute line, in the backends' existing log categories. Nothing is logged per frame.
