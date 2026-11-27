


# Building ReMoM


## Prerequisites

You need the original **Master of Magic** (DOS, v1.31) game data files.
Copy every `.LBX` file and `CONFIG.MOM` from the game into the `assets/`
directory before building. The executable expects these files in its
working directory at runtime.

## Dependencies

- C compiler (gcc, clang, or MSVC)
- **SDL2** development libraries (default backend)
- **SDL2_mixer** development libraries (optional, for audio)
- autoconf, automake, pkg-config (for ./configure builds)

**SDL3** is a future feature, in development for developers only. It is not an
alternative to the SDL2 backend (or, on Windows, to the native Win32 backend): no
release is built with it, and it has no sound or music yet. See
[SDL3 (in development)](#sdl3-in-development) below.


## Installing Dependencies

### Linux (Debian / Ubuntu)

```sh
sudo apt install build-essential autoconf automake pkg-config \
    libsdl2-dev libsdl2-mixer-dev
```

### Linux (Fedora)

```sh
sudo dnf install gcc make autoconf automake pkg-config \
    SDL2-devel SDL2_mixer-devel
```

### macOS

```sh
brew install autoconf automake pkg-config sdl2 sdl2_mixer
```

### Windows (MSYS2 / MinGW64)

```sh
pacman -S base-devel mingw-w64-x86_64-toolchain \
    mingw-w64-x86_64-autotools mingw-w64-x86_64-pkg-config \
    mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_mixer
```


## Building (autoconf)

After installing dependencies, the build steps are the same on every
platform:

```sh
./autogen.sh
./configure
make
sudo make install          # Linux / macOS only
```

On MSYS2 / MinGW64 you can optionally pass `--enable-static-sdl` to
statically link SDL and eliminate DLL dependencies at runtime.


## Building (CMake)

CMake is the canonical build system. It needs **CMake 3.25+** for preset support. The named presets in
`CMakePresets.json` (`clang-debug`, `MSVC-debug`, `MSVC-release`, …) are exactly what CI uses, so use them
instead of ad-hoc `cmake -B build` invocations. For how releases are built and published, see
[RELEASES.md](RELEASES.md).

GoogleTest v1.17.0 is fetched automatically through CMake's FetchContent; there's nothing to install.
Game assets (the `.LBX` files and `CONFIG.MOM`) are expected in `assets/` and are copied to the build
output automatically.

### Windows (Visual Studio 2022)

You need Visual Studio 2022 with the x64 C++ tools.

The `MSVC-*` presets use the **SDL2** backend. CMake looks for SDL under `C:\devellib\`:

- SDL2: `C:\devellib\SDL2-2.32.2\` (from `SDL2-devel-2.32.2-VC.zip` on the
  [SDL releases page](https://github.com/libsdl-org/SDL/releases))
- SDL2_mixer: `C:\devellib\SDL2_mixer-2.8.1\` (from `SDL2_mixer-devel-2.8.1-VC.zip` on the
  [SDL_mixer releases page](https://github.com/libsdl-org/SDL_mixer/releases))

Pass `-DCMAKE_PREFIX_PATH=C:\your\path` to use a different location.

Configure, build, test and package in one command:

```sh
cmake --workflow --preset=MSVC-debug
```

Or step by step:

```sh
cmake --preset MSVC-debug
cmake --build --preset MSVC-debug
ctest --preset MSVC-debug
```

Other Windows presets:

| Preset | Backend |
|--------|---------|
| `MSVC-nosound-debug` | SDL2, no audio |
| `MSVC-win32-debug`, `MSVC-win32-release` | the native **Win32** (GDI) backend: no SDL, no audio, self-contained |
| `MSVC-sdl3-nosound-debug`, `MSVC-sdl3-nosound-release` | SDL3, **in development, for developers only**: no sound or music (see [SDL3 (in development)](#sdl3-in-development)) |

### Linux (Ubuntu / Debian / Kubuntu)

Install the build tools and SDL2:

```sh
sudo apt install build-essential cmake ninja-build curl pkg-config clang
sudo apt install libsdl2-dev libsdl2-mixer-dev
```

If your distro's CMake is older than 3.25, install the latest from Kitware:

```sh
curl -fsSL https://raw.githubusercontent.com/jbalcomb/ReMoM/main/scripts/install-cmake-linux.sh | bash
```

Then build:

```sh
cmake --workflow --preset=clang-debug
```

Or step by step:

```sh
cmake --preset clang-debug
cmake --build --preset clang-debug
ctest --preset clang-debug
```

To build without audio, use the `clang-nosound-debug` preset. `clang-release` has a configure preset but
no build or test preset: build it with `cmake --build out/build/clang-release`.

### macOS (CMake)

`macos-debug` and `macos-release` presets exist, but the arm64 macOS linker currently rejects the
game's packed DOS structures, so macOS doesn't build yet. See RELEASES.md, "Known limitations".

## Configure Options

| Option                | Default | Description                              |
|-----------------------|---------|------------------------------------------|
| `--with-sdl3`        | no      | SDL3 backend, in development, for developers only; no sound (see [SDL3 (in development)](#sdl3-in-development)) |
| `--enable-static-sdl` | no      | Link SDL statically (no DLLs at runtime) |
| `--disable-audio`     | enabled | Build without SDL_mixer / sound support  |


## Build Without Audio

```sh
./configure --disable-audio
make
```


## SDL3 (in development)

The SDL3 backend is a future feature, kept building for developers. It is not an
alternative to the SDL2 backend, which every release uses, or to the native Win32
backend on Windows. Graphics, input, fullscreen and the mouse work on Windows and Linux;
**it has no sound and no music**, for two reasons:

1. **The mixer API changed.** `platform/sdl3/sdl3_Audio.c` was written against the
   pre-release SDL3_mixer API (`Mix_*` functions, like SDL2_mixer's). The released
   SDL3_mixer 3.x is a rewrite with a different API (`MIX_Mixer`, `MIX_Audio`,
   `MIX_Track`, `MIX_*` functions), so SDL3 with sound does not compile.
2. **MIDI.** Master of Magic's music is XMIDI, which ReMoM converts to standard MIDI
   and hands to the mixer. Porting to the `MIX_*` API would give sound effects, not
   music:
   - SDL3_mixer 3.2.0 has **no native-MIDI decoder**. The released Windows build plays
     its music through Windows' own built-in synthesizer, SDL2_mixer's `NATIVEMIDI`
     decoder, with nothing extra shipped. SDL3_mixer cannot do that.
   - Its MIDI decoders are **FluidSynth** and **TiMidity**: software synthesizers that
     need a soundfont (FluidSynth) or a patch set (TiMidity). FluidSynth takes the
     soundfont per file (`MIX_PROP_DECODER_FLUIDSYNTH_SOUNDFONT_PATH_STRING`) and, on
     Linux, falls back to `/usr/share/sounds/sf2/default-GM.sf2`; TiMidity reads
     `TIMIDITY_CFG` or `/etc/timidity*.cfg`. Both are on by default in SDL3_mixer's own
     CMake build, but FluidSynth is left out of its vendored builds.
   - The prebuilt Windows SDL3_mixer 3.2.0 (`C:\devellib\SDL3_mixer-3.2.0`) has neither:
     its decoders are WAV, STBVORBIS, DRFLAC, VOC, AIFF, AU, DRMP3, SINEWAVE and RAW.
   - Ubuntu 25.10 packages SDL3, SDL3_image and SDL3_ttf, but **not SDL3_mixer**.

   So SDL3 music needs SDL3_mixer built from source with FluidSynth; and on Windows
   either a FluidSynth DLL and a General MIDI soundfont shipped with the game, or a
   separate path to Windows' built-in synthesizer. That choice is still open.

Build it with the SDL3 presets, which turn sound off:

```sh
cmake --preset MSVC-sdl3-nosound-debug          # Windows: SDL3 from C:\devellib\SDL3-3.4.2
cmake --build --preset MSVC-sdl3-nosound-debug
cmake --preset clang-sdl3-nosound-debug         # Linux: SDL3 from your distro (libsdl3-dev)
cmake --build --preset clang-sdl3-nosound-debug
```

`MSVC-sdl3-nosound-release` is the Windows Release build. With autotools, use
`./configure --with-sdl3 --disable-audio`: without `--disable-audio`, an installed SDL3_mixer is
picked up and the old audio code fails to compile.

On Linux, test SDL3 changes in a throwaway login session, never on a desktop with work
open: on GNOME 49 / mutter 49.0 an SDL3 pointer warp crashed GNOME Shell and the whole
session (see `MUTTER-49.0-CRASH` in `platform/sdl3/sdl3_MD.c`).

SDL3 packages may need to be built from source if not available in your
distro. Source tarballs and build instructions are at:

- https://github.com/libsdl-org/SDL/releases
- https://github.com/libsdl-org/SDL_mixer/releases

Building SDL3 itself from source on Linux needs these packages (from the
[SDL3 Linux README](https://wiki.libsdl.org/SDL3/README-linux#build-dependencies)):

```sh
sudo apt-get install build-essential git make     pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev     libaudio-dev libfribidi-dev libjack-dev libsndio-dev libx11-dev libxext-dev     libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev     libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev     libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev libthai-dev     libpipewire-0.3-dev libwayland-dev libdecor-0-dev liburing-dev
```

`scripts/install-sdl3-linux.sh` automates the SDL3 build.

### SDL3 dependency packages (if available)

**Debian / Ubuntu:** `libsdl3-dev` (Ubuntu 25.10 has no SDL3_mixer package)
**Fedora:** `SDL3-devel SDL3_mixer-devel`
**macOS:** `brew install sdl3 sdl3_mixer`
**MSYS2:** `mingw-w64-x86_64-SDL3 mingw-w64-x86_64-SDL3_mixer`


## Building with Valgrind
### Configure with it enabled:
`cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_VALGRIND=ON -DSTU_DEBUG:BOOL=TRUE -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++`
### Build and run memcheck:
`cmake --build build`
`ctest --test-dir build -T memcheck`
Results land in build/Testing/*/MemoryChecker.*.log. The -T memcheck flag is what actually invokes Valgrind — normal ctest runs still skip it even when ENABLE_VALGRIND=ON.
cd ~/STU/devel/ReMoM/build/Testing/Temporary/

How to use Valgrind
Once installed, you can run your compiled program under Valgrind to check for memory errors. Compile your program with the -g flag to include debugging information so Valgrind can provide exact line numbers in its reports. 
g++ -g myprogram.cpp -o myprogram
valgrind --leak-check=yes ./myprogram
clang -g -O0 your_program.c -o your_program
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./your_program
...add suppressions, use the --gen-suppressions=yes option 
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --gen-suppressions=yes ./ReMoM
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --gen-suppressions=all ./ReMoM
Here's the suppressions used by a run of valgrind -v --tool=memcheck ls -l:

--suppressions=ReMoM.supp
Each time it runs, valgrind looks for options in files called ~/.valgrindrc and ./.valgrindrc
put a general suppression in the first and one that's more specific to your project in the second
~/.valgrindrc containing:
--memcheck:leak-check=full
--show-reachable=yes
--suppressions=/home/david/devel/wxGTK-2.8.12.supp
--suppressions=/home/david/devel/wxGTK-2.9.supp
To check that valgrind is actually using the suppression files, run it with the -v option. The list of suppression files read is near the beginning of the output.

https://wiki.wxwidgets.org/Parse_valgrind_suppressions.sh
grindmerge <input >output
or, more cleverly:
valgrind --show-leak-kinds=all --leak-check=full  foo 3>&1 1>&2 2>&3 | grindmerge -f oldrules >newrules

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --gen-suppressions=all ./ReMoM
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --gen-suppressions=all ./ReMoM 2> ~/STU/devel/ReMoM/valgrind-gen-suppressions-all.npp

valgrind ./ReMoM
Meh. Can't line up locations of ReMoM executable, the game assets, and the repo ver of .valgrindrc.
cp ~/STU/devel/ReMoM/.valgrindrc ~/
cd ~/STU/devel/ReMoM/out/build/clang-debug/bin/Debug
valgrind ./ReMoM
...some stil showed
==509202== For lists of detected and suppressed errors, rerun with: -s
==509202== ERROR SUMMARY: 1035 errors from 107 contexts (suppressed: 1887 from 1416)

valgrind --gen-suppressions=all ./ReMoM 2> ~/STU/devel/ReMoM/valgrind-gen-suppressions-more.npp

==509881==  Uninitialised value was created by a stack allocation
==509881==    at 0x414A1A0: Startup_Platform (platform/sdl2/sdl2_Init.c:43)
^^^^^^^^^^  ...
useless         WTF? line 43 in sdl2_Init is the opening brace
Program Id      of the definition of the function Startup_Platform()


objdump --syms <executable_name>
jbalcomb@iMustAi:~/STU/devel/ReMoM/out/build/clang-debug/bin/Debug$ objdump --syms ReMoM | head -10
objdump -h <executable_name> | grep debug
...sections like .debug_info, .debug_aranges, etc.
Display raw DWARF debug information:
objdump -g <executable_name> | less
readelf --debug-dump=info <executable_name>


https://valgrind.org/docs/manual/quick-start.html
Preparing your program
Compile your program with -g to include debugging information so that Memcheck's error messages include exact line numbers.
use -O0, but sofa-king-slow
with -O1 line numbers in error messages can be inaccurate
don't use -O2+  (reports uninitialised-value errors which don't exist)

https://plus.tuni.fi/graderT/static/compcs300-compcs300-october-2024/lectures/trees/valgrind/tools.html#:~:text=Valgrind%20tools%20%C2%B6%20detect%20memory%20issues%20Valgrind,program%20execution%20correlate%20errors%20with%20source%20code

https://web.stanford.edu/class/archive/cs/cs107/cs107.1176/guide_valgrind.html#:~:text=Running%20a%20program%20under%20Valgrind%20which%20starts,and%20runs%20the%20program%20inside%20of%20it.
Memory errors versus memory leaks
Finding memory errors
    ==4651== Invalid write of size 1
    ==4651==    at 0x80486A4: main (mywhich.c:58)
Finding leaks
    valgrind --leak-check=full --show-leak-kinds=all myprog arg1 arg2
    ==5942== 12 bytes in 1 blocks are definitely lost in loss record 1 of 1
    ==5942==    at 0x43BC3C0: malloc (vg_replace_malloc.c:149)
    ==5942==    by 0x804863D: main (mywhich.c:51)
Using gdb and Valgrind together
    valgrind --db-attach=yes myprog arg1 arg2

    --vgdb=no|yes|full        activate gdbserver? [yes]
                              full is slower but provides precise watchpoint/step
    --vgdb-error=<number>     invoke gdbserver after <number> errors [999999999]
                              to get started quickly, use --vgdb-error=0
                              and follow the on-screen directions

Extra options read from ~/.valgrindrc, $VALGRIND_OPTS, ./.valgrindrc



Alternative: Address Sanitizer (ASan) 
...consider using Clang's built-in Address Sanitizer (ASan). ASan is a compiler flag that instruments the code at compile time...
cmake -DCMAKE_C_FLAGS="-fsanitize=address" -DCMAKE_CXX_FLAGS="-fsanitize=address" ...
