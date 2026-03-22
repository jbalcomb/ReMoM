# PRD: SDL2 Platform Backend & Dual-Backend Build System

**Version:** 1.0
**Date:** 2026-03-22
**Status:** Implemented


## 1. Problem Statement

The ReMoM project's platform layer was built exclusively against SDL3. SDL3 is bleeding-edge and not yet available in most Linux distro package repositories, macOS Homebrew (as a default), or MSYS2 package databases. This creates a significant barrier to entry for contributors: before they can even build the project, they must compile SDL3 from source and install it manually.

SDL2, by contrast, is universally available as a pre-built package on every major platform and package manager. The project needed an SDL2 backend to make the build accessible out of the box, while preserving SDL3 as an opt-in path for developers who want it.


## 2. Goals

1. **Zero-friction builds on Linux, macOS, and MSYS2** — `apt install libsdl2-dev && ./configure && make` should just work.
2. **Preserve SDL3 as an opt-in backend** — no SDL3 code is removed; developers can switch to it with a single flag.
3. **Single Platform.h interface** — game code (MoX, MoM, src/) is completely unaware of which SDL version is in use.
4. **Feature parity** — the SDL2 backend supports every feature the SDL3 backend does: video, input, keyboard, mouse, audio, timers, palette cycling, Record & Replay, debug overlays, and dynamic window scaling.
5. **Minimal maintenance surface** — the two backends are structurally parallel (same file count, same function signatures, same state variables) so changes to one can be mechanically ported to the other.


## 3. Non-Goals

- Dropping SDL3 support entirely.
- Runtime SDL version detection or dynamic backend switching (it is a compile-time choice).
- Abstracting SDL behind a custom HAL beyond what Platform.h already provides.
- Supporting SDL versions older than SDL2 2.0.12 or SDL3 3.2.0.


## 4. Architecture

### 4.1 Directory Layout

```
platform/
    include/
        Platform.h              # Public API (unchanged)
        Platform_Keys.h
        Platform_Replay.h
    sdl2/                       # NEW — SDL2 backend
        sdl2_Audio.c
        sdl2_EMM.c
        sdl2_Init.c
        sdl2_Input.c
        sdl2_KD.c
        sdl2_MD.c
        sdl2_PFL.c
        sdl2_SDLK.c
        sdl2_State.c
        sdl2_Timer.c
        sdl2_Video.c
        sdl2_PFL.h
        sdl2_KD.h
        sdl2_SCCC.h
        sdl2_Audio.h
        sdl2_MOM.h
        sdl2_SDLK.h
        MOM_PFL.h
        PFL_Audio_Internal.h
    sdl3/                       # Existing SDL3 backend (unchanged)
        sdl3_Audio.c
        sdl3_EMM.c
        sdl3_Init.c
        ...
    replay/
        Replay.c                # Shared by both backends
```

### 4.2 Backend Selection

The build system selects exactly one backend at configure time. No SDL headers or libraries from the unselected backend are included or linked.

| Build System | Default  | SDL3 Opt-in Flag     |
|-------------|----------|----------------------|
| CMake       | SDL2     | `-DUSE_SDL3=ON`      |
| autoconf    | SDL2     | `--with-sdl3`        |

### 4.3 Compile-Time Define

When SDL3 is selected, the preprocessor macro `USE_SDL3` is defined (via `target_compile_definitions` in CMake or `AC_DEFINE` in autoconf). Code that must branch on SDL version (currently only `src/ReMoM.c` for the SDL include path) uses `#ifdef USE_SDL3`.

Game code in MoX/ and MoM/ never references `USE_SDL3` — they call Platform.h functions only.


## 5. SDL3-to-SDL2 API Translation

The SDL2 backend was created by copying the SDL3 backend and mechanically reverting SDL3-specific API calls to their SDL2 equivalents. The key translations are:

### 5.1 Initialization (sdl2_Init.c)

| SDL3 | SDL2 |
|------|------|
| `SDL_CreateWindow(title, w, h, flags)` | `SDL_CreateWindow(title, x, y, w, h, flags)` |
| `SDL_CreateRenderer(win, NULL)` | `SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC)` |
| `SDL_CreateSurface(w, h, fmt)` | `SDL_CreateRGBSurface(0, w, h, 8, 0,0,0,0)` |
| `SDL_AllocPalette(256)` + `SDL_SetSurfacePalette()` | Automatic (SDL2 creates palette for 8-bit surfaces) |
| `SDL_FillSurfaceRect()` | `SDL_FillRect()` |
| `SDL_HideCursor()` / `SDL_ShowCursor()` | `SDL_ShowCursor(SDL_DISABLE)` / `SDL_ShowCursor(SDL_ENABLE)` |

### 5.2 Video (sdl2_Video.c)

| SDL3 | SDL2 |
|------|------|
| `SDL_GetSurfacePalette(surface)` | `surface->format->palette` |
| `SDL_RenderTexture()` | `SDL_RenderCopy()` |
| `SDL_RenderPresent()` returns bool | `SDL_RenderPresent()` returns void |

### 5.3 Events (sdl2_PFL.c)

| SDL3 | SDL2 |
|------|------|
| `SDL_EVENT_KEY_DOWN` | `SDL_KEYDOWN` |
| `SDL_EVENT_QUIT` | `SDL_QUIT` |
| `SDL_EVENT_MOUSE_BUTTON_DOWN` | `SDL_MOUSEBUTTONDOWN` |
| `SDL_EVENT_WINDOW_*` (individual types) | `SDL_WINDOWEVENT` + sub-switch on `event.window.event` |
| `SDL_GetMouseState(&float, &float)` | `SDL_GetMouseState(&int, &int)` |
| `event.button.down` | `event.button.state == SDL_PRESSED` |
| `SDL_SetWindowRelativeMouseMode(win, b)` | `SDL_SetRelativeMouseMode(b)` |
| `SDL_SetWindowMouseGrab()` | `SDL_SetWindowGrab()` |
| `SDL_EVENT_FIRST` / `SDL_EVENT_LAST` | `SDL_FIRSTEVENT` / `SDL_LASTEVENT` |

### 5.4 Keyboard (sdl2_KD.c)

| SDL3 | SDL2 |
|------|------|
| Single `key_xlat[SDL_SCANCODE_COUNT]` | Dual `key_xlat_key[128]` + `key_xlat_scan[SDL_NUM_SCANCODES]` |
| `event->key.key` | `event->key.keysym.sym` |
| `event->key.scancode` | `event->key.keysym.scancode` |
| `SDLK_Q` (uppercase) | `SDLK_q` (lowercase, ASCII-valued) |
| `SDL_KMOD_SHIFT` | `KMOD_SHIFT` |
| `SDL_Keymod` | `uint16_t` |
| `SDL_StartTextInput(window)` | `SDL_StartTextInput()` |

### 5.5 Mouse (sdl2_MD.c)

| SDL3 | SDL2 |
|------|------|
| `SDL_MouseButtonFlags` | `Uint32` |
| `SDL_BUTTON_LMASK` | `SDL_BUTTON(SDL_BUTTON_LEFT)` |
| `SDL_BUTTON_RMASK` | `SDL_BUTTON(SDL_BUTTON_RIGHT)` |

### 5.6 Audio (sdl2_Audio.c)

| SDL3 | SDL2 |
|------|------|
| `#include <SDL3_mixer/SDL_mixer.h>` | `#include <SDL_mixer.h>` |
| `Mix_OpenAudio(0, NULL)` | `Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024)` |
| `SDL_IOStream` / `SDL_IOFromMem()` | `SDL_RWops` / `SDL_RWFromMem()` |
| `SDL_CloseIO()` | `SDL_FreeRW()` |
| `Mix_LoadWAV_IO()` | `Mix_LoadWAV_RW()` |
| `Mix_LoadMUSType_IO()` | `Mix_LoadMUSType_RW()` |

### 5.7 Timer (sdl2_Timer.c)

| SDL3 | SDL2 |
|------|------|
| `SDL_GetTicks()` returns `uint64_t` | `SDL_GetTicks()` returns `Uint32` (cast to `uint64_t`) |


## 6. Build System Changes

### 6.1 CMake

**Root CMakeLists.txt:**
- `option(USE_SDL3 "Use SDL3 instead of SDL2" OFF)`
- `find_package(SDL2 REQUIRED)` by default; `find_package(SDL3 REQUIRED)` when `USE_SDL3=ON`
- DLL copy logic uses variables `${_sdl_target}` / `${_mixer_target}` that resolve to the correct imported target

**platform/CMakeLists.txt:**
- Conditional source list: `platform/sdl2/sdl2_*.c` or `platform/sdl3/sdl3_*.c`
- Conditional link libraries: `SDL2::SDL2` or `SDL3::SDL3`
- `target_compile_definitions(Platform PUBLIC USE_SDL3)` when SDL3 is selected
- Private include directory points to the selected backend

**src/CMakeLists.txt:**
- Conditional `_sdl_main_lib` variable for linking `SDL2::SDL2` or `SDL3::SDL3`

### 6.2 Autoconf (configure.ac)

- New `--with-sdl3` option (default: no)
- `PKG_CHECK_MODULES([SDL], [sdl2])` by default; `PKG_CHECK_MODULES([SDL], [sdl3])` when `--with-sdl3`
- Mixer detection: `SDL2_mixer` or `SDL3_mixer` depending on backend
- `AM_CONDITIONAL([USE_SDL3], ...)` for Makefile.am
- `AC_DEFINE([USE_SDL3])` when SDL3 is selected
- Unified variable names: `SDL_CFLAGS`, `SDL_LIBS`, `SDL_MIXER_CFLAGS`, `SDL_MIXER_LIBS`
- Static linking (`--enable-static-sdl`) works with either backend
- Summary output shows which backend was selected

### 6.3 Automake (Makefile.am)

- `if USE_SDL3` / `else` conditionals select source files and backend include directory
- Audio source conditional is nested: `if HAVE_AUDIO` + `if USE_SDL3`
- All `$(SDL3_CFLAGS)` / `$(SDL3_LIBS)` references replaced with `$(SDL_CFLAGS)` / `$(SDL_LIBS)`

### 6.4 Source Code (src/ReMoM.c)

```c
#ifdef USE_SDL3
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#else
#include <SDL.h>
#include <SDL_main.h>
#endif
```

This is the only game source file that references SDL directly (for `SDL_main`).


## 7. Files Created / Modified

### New Files (platform/sdl2/)

| File | Description |
|------|-------------|
| `sdl2_Init.c` | Window, renderer, surface, texture creation; cursor control |
| `sdl2_Video.c` | Palette update, screen blit, render present |
| `sdl2_PFL.c` | Event loop, platform globals, window events |
| `sdl2_KD.c` | Keyboard driver, key translation tables, text input |
| `sdl2_MD.c` | Mouse driver, button state, cursor warp |
| `sdl2_Audio.c` | SDL2_mixer init, WAV/music loading via RWops |
| `sdl2_Timer.c` | Tick counter wrapper |
| `sdl2_EMM.c` | Memory management (SDL_malloc/free) |
| `sdl2_Input.c` | Input state helpers |
| `sdl2_SDLK.c` | SDL keycode utilities |
| `sdl2_State.c` | Shared platform state variables (no SDL calls) |
| `sdl2_PFL.h` | Backend header (SDL2 types, function declarations) |
| `sdl2_KD.h` | Keyboard driver header |
| `sdl2_SCCC.h` | Scancode-to-character mapping |
| `sdl2_Audio.h` | Audio subsystem header |
| `sdl2_MOM.h` | MoM integration header |
| `sdl2_SDLK.h` | SDL keycode header |
| `MOM_PFL.h` | Platform-MoM bridge header |
| `PFL_Audio_Internal.h` | Internal audio types |

### Modified Files

| File | Change |
|------|--------|
| `configure.ac` | SDL2 default, `--with-sdl3` opt-in, unified variable names |
| `Makefile.am` | Conditional backend source selection via `USE_SDL3` |
| `CMakeLists.txt` (root) | `USE_SDL3` option, dual `find_package` branching |
| `platform/CMakeLists.txt` | Conditional sources, libs, includes, compile definitions |
| `src/CMakeLists.txt` | Conditional SDL link target |
| `src/ReMoM.c` | `#ifdef USE_SDL3` for SDL include path |
| `BUILDING.md` | SDL2 as default instructions, SDL3 as opt-in section |


## 8. Verification Criteria

| Test | Command | Expected Result |
|------|---------|-----------------|
| CMake SDL2 build (Windows) | `cmake -B build && cmake --build build` | Finds SDL2, compiles, links, runs |
| CMake SDL3 build (Windows) | `cmake -B build -DUSE_SDL3=ON && cmake --build build` | Finds SDL3, compiles, links, runs |
| autoconf SDL2 build (Linux) | `./autogen.sh && ./configure && make` | Finds SDL2 via pkg-config, builds |
| autoconf SDL3 build (Linux) | `./autogen.sh && ./configure --with-sdl3 && make` | Finds SDL3 via pkg-config, builds |
| autoconf no-audio build | `./configure --disable-audio && make` | Builds without mixer dependency |
| autoconf static build | `./configure --enable-static-sdl && make` | Links SDL statically |
| Functional (both backends) | Run `ReMoMber` | Main menu loads, input works, palette cycles, audio plays |
| Replay (both backends) | Record a replay on one backend, play on both | Frame-identical playback |


## 9. Risks and Mitigations

| Risk | Mitigation |
|------|------------|
| SDL2/SDL3 API drift as SDL3 matures | Backends are isolated; SDL3 changes only touch `platform/sdl3/` |
| Behavioral differences between backends | Platform.h contract is the spec; replay system can cross-validate |
| SDL2 deprecation (long-term) | SDL2 is in maintenance mode with a multi-year support window; SDL3 backend is ready when distros catch up |
| Contributor confusion about which backend to use | BUILDING.md defaults to SDL2; SDL3 is clearly marked opt-in |
