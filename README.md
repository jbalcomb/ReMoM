# Master of Magic - Reassembled
# ReMoM

Remaking the game ‘Master of Magic’ from 1994-95, all from disassembling the original executables and reassembling and translating it to C.  

The goal of this project is to recreate a functional source code for Master of Magic, to allow the community to improve and expand the Simtex Universe to their hearts content.  

To get involved, or even just hang out with other MoM fans, join us on the the "Master of Magic - Fans" Discord Server.  
This particular project is hosted by the [#mom-reverse-engineering Channel](https://discord.gg/kSNA8bE6Wt), on that server.  

Alpha Demo Video:  
https://youtu.be/d-PlvxV_Ek0

## How to Build

### Prerequisites

- CMake 4.2+ (project currently requires `cmake_minimum_required(VERSION 4.2)`).
- A C/C++ toolchain:
	- **Windows (recommended):** Visual Studio 2022 (x64 C++ tools), or
	- **Clang + Ninja** (presets are included for this as well).
- SDL2 development files available to CMake (`find_package(SDL2 REQUIRED)`).
- Optional audio support: SDL2_mixer development files (or use the no-sound preset).

### Windows quick start (MSVC, Debug)

From the repository root:

```powershell
cmake --workflow --preset=MSVC-debug
```

This runs configure, build, test, and package steps defined in `CmakePresets.json`.

### Windows quick start (MSVC, Debug, no sound)

If `SDL2_mixer` is not installed:

```powershell
cmake --workflow --preset=MSVC-nosound-debug
```

### Manual preset commands

You can also run steps individually:

```powershell
cmake --preset MSVC-debug
cmake --build --preset MSVC-debug
ctest --preset MSVC-debug --output-on-failure
```

### Clang + Ninja preset example

```powershell
cmake --preset Clang-Ninja-debug
cmake --build out/build/Clang-Ninja-debug --config Debug
ctest --test-dir out/build/Clang-Ninja-debug -C Debug --output-on-failure
```

### Notes

- Build/test preset names are defined in `CmakePresets.json`.
- If SDL2 or SDL2_mixer is not found, update your local library install paths/environment so `find_package(...)` can resolve them.
