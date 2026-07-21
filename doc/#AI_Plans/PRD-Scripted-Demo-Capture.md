# PRD: Scripted Demo Capture — render the ReMoM showcase video from a script

## Goal
Produce the YouTube demo video **entirely from inside the engine**, driven by checked-in scripts, with
no OBS Studio and no desktop capture.  The game plays itself from a `.hms` scenario, records its own
frames and audio, and `ffmpeg` muxes the result.  Re-rendering the video is a build step, not a
performance.

Target for the first cut: a **2-4 minute highlight reel** — a stitched set of short scripted scenes
(intro, new game / wizard select, overland, city, combat, a spell, the Spell of Mastery win), each its
own `.hms`, cut together in post.

## Why not OBS + AutoIt
- Desktop capture picks up compositor artifacts, dropped/duplicated frames, and whatever else is on
  screen.  In-engine capture is pixel-exact by construction.
- AutoIt would drive the game through the OS input queue — timing-fragile and unrepeatable.  The game
  already has a scripted-input path (`--scenario`) that runs *inside* the frame loop.
- A scripted render can be re-cut by editing a text file and re-running, at any output resolution.

## What already exists (verified)

### Input scripting — done, and it's text
- `.hms` is a human-readable scenario script: `click X Y`, `rclick X Y`, `type "STRING"`,
  `key K`, `escape` / `enter` / `backspace`, `wait Nms`.  Format documented in
  [tools/rmr2hms.c](../../tools/rmr2hms.c).
- `ReMoMber.exe --scenario FILE.hms` loads it and registers the Artificial Human Player as a
  per-frame callback ([ReMoM.c:424](../../src/ReMoM.c#L424) →
  `HeMoM_Player_Load_Scenario` + `Platform_Register_Frame_Callback(HeMoM_Player_Frame)`).
  Clicks warp the real cursor via `Set_Pointer_Position`
  ([Artificial_Human_Player.c:742](../../src/Artificial_Human_Player.c#L742)).
- **Authoring loop**: play the scene by hand with `--record scene.RMR` → `rmr2hms scene.RMR scene.hms`
  → hand-edit the waits/coords for pacing → `--scenario scene.hms`.  The recording is a rough draft;
  the script is the artifact we keep.

### Video tap — one choke point per backend
`Platform_Video_Update()` has the finished 320x200 indexed framebuffer
(`video_page_buffer[draw_page_num]`) and the live 256-entry palette (`platform_palette_buffer`) in
hand immediately before it presents.  Three implementations:
- [platform/win32/win_PFL.cpp:230](../../platform/win32/win_PFL.cpp#L230)
- [platform/sdl2/sdl2_Video.c:72](../../platform/sdl2/sdl2_Video.c#L72)
- [platform/sdl3/sdl3_Video.c:74](../../platform/sdl3/sdl3_Video.c#L74)

### Audio tap — SDL_mixer only
Sound goes through SDL_mixer (`Mix_PlayChannel` / music) in
[sdl2_Audio.c:278](../../platform/sdl2/sdl2_Audio.c#L278) and
[sdl3_Audio.c:235](../../platform/sdl3/sdl3_Audio.c#L235).  `Mix_SetPostMix` yields the final mixed
device stream — exactly what we want to write as a WAV, already in sync with what the player hears.

## BLOCKER FOUND: the default build is silent
[win_PFL.cpp:479](../../platform/win32/win_PFL.cpp#L479) `Platform_Audio_Play_Sound` is a stub that
returns `-1` and plays nothing.  **Every MSVC preset sets `USE_WIN32=TRUE`** (`MSVC-debug`,
`MSVC-release`, `MSVC-nosound-debug`), so the build normally used for development has no audio at all.

Consequence: the demo must be **rendered from an SDL backend**.  Options, cheapest first:
1. Use the existing `clang-release` / `clang-debug` presets (no `USE_WIN32` → SDL2 default).
2. Add an `MSVC-sdl2-release` preset (MSVC toolchain, `USE_WIN32=OFF`) so the render uses the same
   compiler as everything else.
Decision needed before the audio phase; the video phase is backend-agnostic and can start on either.

## External dependency
`ffmpeg` is **not installed** on this machine (not on PATH).  Needed for muxing/encoding only, not at
capture time.  Install with either:
```
winget install Gyan.FFmpeg
choco install ffmpeg
```

## Design

### Capture module
New backend-agnostic `platform/capture/PFL_Capture.c` + `platform/include/Platform_Capture.h`:
```
int  Platform_Capture_Start(const char * out_dir, int fps);
void Platform_Capture_Video_Frame(const uint8_t * indexed, int w, int h, const PFL_Color * palette);
void Platform_Capture_Audio(const uint8_t * stream, int len);
void Platform_Capture_Stop(void);
int  Platform_Capture_Active(void);
```
Each backend's `Platform_Video_Update()` calls `Platform_Capture_Video_Frame(...)` right before it
presents (guarded by `Platform_Capture_Active()`, so a non-capturing run pays one predictable branch).
The SDL backends register `Mix_SetPostMix` to feed `Platform_Capture_Audio`.

### A/V sync: sample to constant frame rate, don't record present events
The game does not present at a constant rate — the DOS-derived timer, `Release_Time()` waits and
per-screen loops all vary.  Recording one video frame per present would yield a variable-rate stream
that cannot be trivially aligned to a constant-rate audio track.

Instead, `Platform_Capture_Video_Frame` keeps a wall clock started at `Platform_Capture_Start` and
emits however many `1/fps`-second frames have elapsed since the last emit, **duplicating the previous
framebuffer** to fill gaps.  Output is a constant-rate RGB24 raw stream that lines up with the WAV by
construction, with a single shared `t = 0`.  No timestamp manifest, no VFR muxing, no drift.

Cost at 320x200x3: 11.5 MB/s at 60 fps, 5.8 MB/s at 30 fps.  A 4-minute reel is ~2.8 GB / ~1.4 GB of
temp raw — fine as scratch, deleted after encode.  `fps` is a flag; default 60.

### Output
```
<out_dir>/video_rgb24.raw     constant-rate RGB24, 320x200
<out_dir>/audio.wav           post-mix PCM, device format from Mix_QuerySpec
<out_dir>/capture_info.txt    fps, dimensions, audio format, frame count -- feeds the encode step
```
Encode (nearest-neighbour upscale, 4:3 pillarbox to 1080p):
```
ffmpeg -f rawvideo -pix_fmt rgb24 -s 320x200 -r 60 -i video_rgb24.raw -i audio.wav \
       -vf "scale=1440:1080:flags=neighbor,pad=1920:1080:240:0" \
       -c:v libx264 -crf 18 -pix_fmt yuv420p -c:a aac -b:a 192k scene.mp4
```
Nearest-neighbour is deliberate — the pixels are the point.

### CLI
`--capture <dir>` and `--capture-fps <n>` on ReMoMber, parsed alongside the existing
`--record` / `--replay` / `--demo` / `--scenario` flags in [ReMoM.c](../../src/ReMoM.c#L377).
Typical render invocation:
```
ReMoMber.exe --scenario demo/scenes/03_overland.hms --capture out/capture/03_overland
```

### Repo home
- `demo/scenes/*.hms` — the scripted scenes (the editable source of the video).
- `demo/scenes/*.RMR` — the raw recordings the scripts were derived from (kept for re-derivation).
- `tools/render_demo.*` — drives scenario → capture → ffmpeg for one scene, then concatenates.
- A VS Code task + CMake target so the whole reel re-renders from a clean checkout.

## Phases (tracer-bullet)

### Phase 1 — video capture tracer — COMPLETE
- [platform/include/Platform_Capture.h](../../platform/include/Platform_Capture.h) +
  [platform/capture/PFL_Capture.c](../../platform/capture/PFL_Capture.c), added to `PLATFORM_SOURCES`
  so every backend gets it.
- Hooked into all three backends' `Platform_Video_Update()`: win32, sdl2, sdl3 (5 lines each, guarded
  by `Platform_Capture_Active()`).
- `--capture <dir>` / `--capture-fps <n>` in [ReMoM.c](../../src/ReMoM.c#L377), parsed alongside
  `--record` / `--replay`.  Capture starts after argument parsing and finalizes via `atexit` — the
  game exits through paths that never return to `main` (`Exit_With_Message`, the endgame Respawn
  hack), so `atexit` is what guarantees the WAV header and info file get written.
- `capture_info.txt` is written at Start as well as at Stop, so a run that is killed rather than
  exiting cleanly still leaves an encodable capture (frame count is recoverable from the raw size).

**Gate passed.** `ReMoMber.exe --replay DEMO_QUIT_KD.RMR --capture <dir> --capture-fps 30` produced
2586 frames over an 86-second run — 2586 / 30 = 86.2 s, i.e. the constant-rate sampling tracks wall
clock exactly.  Decoded frames are pixel-exact with correct palettes (verified on the intro sequence
and the credits/main-menu screen).  Throughput measured at 5.8 MB/s at 30 fps, as predicted.

Known gap, not blocking: a scenario has to *end* for the clean finalize path to run.  The DEMO_QUIT
recordings did not reach their quit within the test window, so both tracer runs were killed — hence
the up-front info file.  Demo scenes should end with an explicit quit.

### Phase 2 — SDL backend + audio
Settle the backend question above.  Wire capture into `sdl2_Video.c` / `sdl3_Video.c`, add the
`Mix_SetPostMix` tap and WAV writer.  **Gate**: a scripted scene renders with audible, in-sync game
music and SFX.

### Phase 3 — scene authoring
Record and hand-tune the highlight-reel scenes.  This is the bulk of the *time* but none of the risk;
it's editing text files and re-running.

### Phase 4 — render pipeline
`render_demo` script: per-scene capture → encode → concat → titles.  VS Code task.

## Risks being watched
- **Scenario robustness over minutes.** `.hms` is coordinate- and timing-based; a scene that depends
  on RNG (combat rolls, map gen) can diverge.  Mitigation: pin the seed (`--seed1 N`, already
  supported) and start scenes from checked-in saves rather than replaying long prefixes.  Keep scenes
  SHORT — that is the main reason the highlight-reel format is the right first target.
- **Capture back-pressure.** Writing 11.5 MB/s from inside the present path could stall the frame
  loop and change game pacing.  Watch for it; if it bites, buffer the writes on a thread.  Note that
  under capture the game's own real-time pacing is what the video preserves, so a stall is visible.
- **Win32 audio stub** — see BLOCKER above.
