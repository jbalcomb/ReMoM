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
- `ReMoM.exe --scenario FILE.hms` loads it and registers the Artificial Human Player as a
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
`ffmpeg` -- INSTALLED 2026-07-21: version 8.1.2-full_build (gyan.dev), with libx264.  Needed for
encoding only, never at capture time; it reads a scratch directory of raw pixels and writes an mp4.
Tier C under `doc/#Devel/Dependency-Vetting.md` (dev-only, offline, scratch data).  Vetting record:
`python tools/vet_dependency.py --repo GyanD/codexffmpeg --upstream FFmpeg/FFmpeg --author "Gyan Doshi" --tier C`
-- no flags triggered; open gaps are build reproducibility and key-compromise exposure.

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
`--capture <dir>` and `--capture-fps <n>` on ReMoM, parsed alongside the existing
`--record` / `--replay` / `--demo` / `--scenario` flags in [ReMoM.c](../../src/ReMoM.c#L377).
Typical render invocation:
```
ReMoM.exe --scenario showcase/03_overland.hms --capture out/capture/03_overland
```

### Repo home
- `showcase/*.hms` — the scripted scenes (the editable source of the video).
- `showcase/*.RMR` — the raw recordings the scripts were derived from (kept for re-derivation).
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

**Gate passed.** `ReMoM.exe --replay DEMO_QUIT_KD.RMR --capture <dir> --capture-fps 30` produced
2586 frames over an 86-second run — 2586 / 30 = 86.2 s, i.e. the constant-rate sampling tracks wall
clock exactly.  Decoded frames are pixel-exact with correct palettes (verified on the intro sequence
and the credits/main-menu screen).  Throughput measured at 5.8 MB/s at 30 fps, as predicted.

Known gap, not blocking: a scenario has to *end* for the clean finalize path to run.  The DEMO_QUIT
recordings did not reach their quit within the test window, so both tracer runs were killed — hence
the up-front info file.  Demo scenes should end with an explicit quit.

### Phase 2 -- SDL backend + audio -- COMPLETE
**Backend decision: a new `MSVC-sdl2-debug` preset** (MSVC toolchain, `USE_WIN32=FALSE`).  The
`clang-debug` preset was tried first and is BROKEN independently of this work -- clang cannot find the
MSVC CRT headers (`inttypes.h` not found in `sdl2_KD.c` / `sdl2_PFL.c`), so it needs a VS developer
environment.  MSVC + SDL2 sidesteps that and keeps the render on the same toolchain as everything
else.  SDL2 2.32.2 and SDL2_mixer 2.8.1 are present at `C:/devellib/`.

- `Mix_SetPostMix` tap in [sdl2_Audio.c](../../platform/sdl2/sdl2_Audio.c) feeds
  `Platform_Capture_Audio` the final mixed device stream -- music plus every SFX channel, exactly
  what the player hears.
- **Two pre-existing `/WX` C4456 shadowed-local errors had to be fixed first**, in `sdl2_KD.c`
  (`mox_character`) and `sdl2_Audio.c` (`sdl2_rw_ops`).  They were latent because the SDL2 backend
  had apparently never been compiled with MSVC `/W4 /WX`.  Neither was introduced here.

**Gate passed.**  A 39.1 s render produced 1920x1080 H.264 + 44.1 kHz stereo AAC.  Audio verified as
real signal by measuring the raw WAV: peak amplitude 24576/32767, ~50% of samples above the noise
floor.  A/V alignment is structural -- both streams share `t = 0` and the video is constant-rate.

Three bugs found and fixed by actually running it, all of them mine:
1. **Ordering.** `Startup_Platform()` initialises audio (and declares the device format) BEFORE the
   CLI is parsed and `Platform_Capture_Start()` runs.  `Set_Audio_Format` had bailed out when capture
   was not yet active, silently producing video-only captures.  The WAV is now opened by
   `Capture_Open_Wav_If_Ready()`, called from both entry points and idempotent.
2. **SDL format bitfield.** The float test was hand-rolled as `format & 0x8000` -- but in SDL that bit
   is `SDL_AUDIO_MASK_SIGNED`; float is `0x100`.  Every capture was labelling signed 16-bit PCM as
   IEEE_FLOAT, and ffmpeg reported `codec_name=unknown`.  Now uses `SDL_AUDIO_ISFLOAT` /
   `SDL_AUDIO_BITSIZE` -- do not hand-roll this.
3. **Unfinalized WAV header** when a scene is killed by `--max-seconds`; `render_demo.py` now patches
   the RIFF/data sizes from the file length, alongside the raw-video partial-frame truncation.

Rendering with audio:
```
python tools/render_demo.py --scene NAME --scenario showcase/NAME.hms     --game out/build/MSVC-sdl2-debug/bin/Debug/ReMoM.exe
```
Note `--target ReMoM` alone does not stage assets (`copy_assets` is a separate `ALL` target); build
`copy_assets` once per preset, or build without `--target`.


### Phase 3 — scene authoring
Record and hand-tune the highlight-reel scenes.  This is the bulk of the *time* but none of the risk;
it's editing text files and re-running.

### Phase 4 -- render pipeline -- SUBSTANTIALLY DONE (titles + VS Code task outstanding)
[tools/render_demo.py](../../tools/render_demo.py) does capture -> encode -> concat in one command:
```
python tools/render_demo.py --scene overland --scenario showcase/03_overland.hms
python tools/render_demo.py --concat "out/demo/*.mp4" --output out/demo/reel.mp4
```
Verified end to end: a 19.2 s capture encoded to a 1920x1080 H.264 mp4 (1.4 MB) with correct 4:3
pillarboxing and crisp nearest-neighbour pixels, confirmed by extracting a frame back out of the
finished mp4.  Encoding runs faster than real time.  `--square-pixels` switches to the 16:10
square-pixel look; `--seed` pins the RNG; the large raw intermediate is deleted after a successful
encode unless `--keep-raw`.

Killing the game via `--max-seconds` can interrupt a frame mid-write; the script truncates the raw
to a whole number of frames before encoding, which removes ffmpeg's "Invalid buffer size" complaint.
(Verified with a synthetic 3-frames-plus-1000-bytes file, since a real capture happened to land on a
frame boundary and would not have exercised the path.)  Scenes that end by quitting never hit this.

## Risks being watched
- **Scenario robustness over minutes.** `.hms` is coordinate- and timing-based; a scene that depends
  on RNG (combat rolls, map gen) can diverge.  Mitigation: pin the seed (`--seed1 N`, already
  supported) and start scenes from checked-in saves rather than replaying long prefixes.  Keep scenes
  SHORT — that is the main reason the highlight-reel format is the right first target.
- **Capture back-pressure.** Writing 11.5 MB/s from inside the present path could stall the frame
  loop and change game pacing.  Watch for it; if it bites, buffer the writes on a thread.  Note that
  under capture the game's own real-time pacing is what the video preserves, so a stall is visible.
- **Win32 audio stub** — see BLOCKER above.
