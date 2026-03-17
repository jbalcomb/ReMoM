# Master of Magic - Reassembled
# ReMoM

Remaking the game 'Master of Magic' from 1994-95, all from disassembling the original executables and reassembling and translating it to C.

The goal of this project is to recreate a functional source code for Master of Magic, to allow the community to improve and expand the Simtex Universe to their hearts content.

To get involved, or even just hang out with other MoM fans, join us on the the "Master of Magic - Fans" Discord Server.
This particular project is hosted by the [#mom-reverse-engineering Channel](https://discord.gg/kSNA8bE6Wt), on that server.

Alpha Demo Video:
https://youtu.be/d-PlvxV_Ek0

## How to Build

### Prerequisites

- **CMake** 3.25+ (for preset support)
- **SDL3** 3.4.2+ development files
- **SDL3_mixer** (optional — use a no-sound preset to build without it)
- A C/C++ toolchain:
  - **Windows:** Visual Studio 2022 (x64 C++ tools)
  - **Linux:** GCC or Clang, plus Ninja or Make

Build presets are defined in `CMakePresets.json`.

---

### Windows (MSVC)

```powershell
cmake --workflow --preset=MSVC-debug
```

This runs configure, build, test, and package steps in one command.

Without SDL3_mixer:

```powershell
cmake --workflow --preset=MSVC-nosound-debug
```

Or run steps individually:

```powershell
cmake --preset MSVC-debug
cmake --build --preset MSVC-debug
ctest --preset MSVC-debug
```

SDL3 and SDL3_mixer are expected at `C:\devellib\SDL3-3.4.2` and `C:\devellib\SDL3_mixer-3.2.0`. To use a different location, pass `-DCMAKE_PREFIX_PATH=C:\your\path` when configuring.

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

#### 2. Install SDL3

SDL3 is not yet in most distro repos. Build and install from source:

```bash
curl -fsSL https://raw.githubusercontent.com/jbalcomb/ReMoM/main/scripts/install-sdl3-linux.sh | bash
```

This installs SDL3 3.4.2 to `/usr/local`. The script will list the apt packages needed for a full build (X11, Wayland, PulseAudio, etc.) and prompt before proceeding.

#### 3. Build ReMoM

```bash
cmake --workflow --preset=clang-nosound-debug
```

Or run steps individually:

```bash
cmake --preset clang-nosound-debug
cmake --build --preset clang-nosound-debug
ctest --preset clang-nosound-debug
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