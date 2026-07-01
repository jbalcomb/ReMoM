# Master of Magic - Reassembled
# ReMoM

Remaking the game 'Master of Magic' from 1994-95, all from disassembling the original executables and reassembling and translating it to C.

The goal of this project is to recreate a functional source code for Master of Magic, to allow the community to improve and expand the Simtex Universe to their hearts content.

To get involved, or even just hang out with other MoM fans, join us on the the "Master of Magic - Fans" Discord Server.
This particular project is hosted by the [#mom-reverse-engineering Channel](https://discord.gg/kSNA8bE6Wt), on that server.

Alpha Demo Video:
https://youtu.be/d-PlvxV_Ek0

## Download and Play

Pre-built, **engine-only** binaries are available on the [Releases](https://github.com/jbalcomb/ReMoM/releases) page. You supply your own original game data files.

1. Download the artifact for your platform:
   - **Windows:** the `.zip`, or run the `.exe` installer. *(Audio is not yet wired up on the Windows build — it runs silent.)*
   - **Linux:** the `.AppImage` (self-contained) — or the `.zip`/`.tar.gz` if you already have SDL2 installed.
   - **macOS:** the `.zip`. It's unsigned, so on first run clear the quarantine flag: `xattr -dr com.apple.quarantine ReMoMber`.
2. Extract it (on Linux/macOS, `chmod +x` the AppImage / `ReMoMber`).
3. Copy your original **Master of Magic v1.31** game files (all `.LBX` files and `CONFIG.MOM`) into the same folder as the executable.
4. Run `ReMoMber`.

See [PLAYING.md](PLAYING.md) for more details and troubleshooting. Maintainers cutting a release: see [RELEASES.md](RELEASES.md).

## How to Build

### Prerequisites

- **CMake** 3.25+ (for preset support)
- A rendering/audio backend, depending on platform/preset:
  - **Windows (`MSVC-*` presets):** none — these use the native **Win32** backend (no SDL).
  - **Linux / macOS:** **SDL2** + **SDL2_mixer** development files. SDL2 is the default backend; SDL3 is opt-in via `-DUSE_SDL3=ON` (use a no-sound preset to build without the mixer).
- A C/C++ toolchain:
  - **Windows:** Visual Studio 2022 (x64 C++ tools)
  - **Linux:** GCC or Clang, plus Ninja or Make

Build presets are defined in `CMakePresets.json`.

---

### Windows (MSVC)

```powershell
cmake --workflow --preset=MSVC-debug
```

This runs configure, build, test, and package steps in one command. The `MSVC-*`
presets use the native **Win32** backend, so no SDL install is needed on Windows.

Explicitly without audio:

```powershell
cmake --workflow --preset=MSVC-nosound-debug
```

Or run steps individually:

```powershell
cmake --preset MSVC-debug
cmake --build --preset MSVC-debug
ctest --preset MSVC-debug
```

To build an SDL-based Windows binary instead (opt out of the Win32 backend with `-DUSE_WIN32=OFF`), CMake looks for SDL under `C:\devellib\` (e.g. `SDL2-2.32.2`, or `SDL3-3.4.2` with `-DUSE_SDL3=ON`). Pass `-DCMAKE_PREFIX_PATH=C:\your\path` to use a different location. See [BUILDING.md](BUILDING.md).

---

### Linux (Kubuntu 24.04 / Ubuntu / Debian)

#### 1. Install build tools

```bash
sudo apt install build-essential cmake ninja-build curl pkg-config clang
```

If your distro's CMake is too old (need 3.25+), install the latest from Kitware:

```bash
curl -fsSL https://raw.githubusercontent.com/jbalcomb/ReMoM/main/scripts/install-cmake-linux.sh | bash
```

#### 2. Install SDL2

SDL2 is the default backend and is in the distro repos:

```bash
sudo apt install libsdl2-dev libsdl2-mixer-dev
```

(SDL3 is optional — opt in with `-DUSE_SDL3=ON`; see [BUILDING.md](BUILDING.md). To build without audio, use the `clang-nosound-debug` preset.)

#### 3. Build ReMoM

```bash
cmake --workflow --preset=clang-debug
```

Or run steps individually:

```bash
cmake --preset clang-debug
cmake --build --preset clang-debug
ctest --preset clang-debug
```

---

### Notes

- All available presets are listed in `CMakePresets.json`.
- GoogleTest v1.17.0 is fetched automatically via CMake's FetchContent — no manual install needed.
- Game assets (LBX files, `CONFIG.MOM`) are expected in the `assets/` directory and are copied to the build output automatically.

https://wiki.libsdl.org/SDL3/README-linux#build-dependencies

sudo apt-get install build-essential git make \
pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev \
libaudio-dev libfribidi-dev libjack-dev libsndio-dev libx11-dev libxext-dev \
libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev \
libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev \
libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev libthai-dev \
libpipewire-0.3-dev libwayland-dev libdecor-0-dev liburing-dev