


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

Optionally, you can build with **SDL3** instead (`--with-sdl3`), but SDL3
is relatively new and may not be in your distro's default package
repositories yet. See the [SDL3 backend](#sdl3-backend) section below.


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

CMake defaults to the SDL2 backend. The custom Find modules in `cmake/`
handle locating SDL2 and SDL2_mixer automatically via pkg-config on
Linux/macOS, or via cache variables on Windows.

### Linux / macOS (CMake)

After installing the SDL2 dev packages (see above), CMake finds them
automatically through pkg-config:

```sh
cmake -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

To build without audio:

```sh
cmake -B build -DDISABLE_AUDIO=ON
cmake --build build
```

### Windows (Visual Studio 2022)

1. Download the SDL2 development package for Visual C++ from
   https://github.com/libsdl-org/SDL/releases/tag/release-2.30.12
   (the file named `SDL2-devel-2.30.12-VC.zip`).
2. Extract it so the directory structure looks like
   `C:\devellib\SDL2-2.30.12\include\`, `C:\devellib\SDL2-2.30.12\lib\x64\`, etc.
3. Do the same for SDL2_mixer from
   https://github.com/libsdl-org/SDL_mixer/releases/tag/release-2.8.1
   (`SDL2_mixer-devel-2.8.1-VC.zip` → `C:\devellib\SDL2_mixer-2.8.1\`).

Then from the project root:

```sh
cmake -B build -DSDL2_DIR=C:\devellib\SDL2-2.30.12 -DSDL2_MIXER_DIR=C:\devellib\SDL2_mixer-2.8.1
cmake --build build --config Debug
```

The default presets already point at `C:\devellib\SDL2-2.30.12` and
`C:\devellib\SDL2_mixer-2.8.1`, so if you extract to those exact paths
you can skip the `-D` flags and just use:

```sh
cmake --workflow --preset=MSVC-debug
```

To use SDL3 instead, pass `-DUSE_SDL3=ON` (see [SDL3 backend](#sdl3-backend)).


## Configure Options

| Option                | Default | Description                              |
|-----------------------|---------|------------------------------------------|
| `--with-sdl3`        | no      | Use SDL3 instead of SDL2                 |
| `--enable-static-sdl` | no      | Link SDL statically (no DLLs at runtime) |
| `--disable-audio`     | enabled | Build without SDL_mixer / sound support  |


## Build Without Audio

```sh
./configure --disable-audio
make
```


## SDL3 Backend

To build with SDL3 instead of SDL2:

```sh
./configure --with-sdl3
make
```

Or with CMake:

```sh
cmake -B build -DUSE_SDL3=ON
cmake --build build
```

SDL3 packages may need to be built from source if not available in your
distro. Source tarballs and build instructions are at:

- https://github.com/libsdl-org/SDL/releases
- https://github.com/libsdl-org/SDL_mixer/releases

### SDL3 dependency packages (if available)

**Debian / Ubuntu:** `libsdl3-dev libsdl3-mixer-dev`
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
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --gen-suppressions=yes ./ReMoMber
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --gen-suppressions=all ./ReMoMber
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

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --gen-suppressions=all ./ReMoMber
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --gen-suppressions=all ./ReMoMber 2> ~/STU/devel/ReMoM/valgrind-gen-suppressions-all.npp

valgrind ./ReMoMber
Meh. Can't line up locations of ReMoMber executable, the game assets, and the repo ver of .valgrindrc.
cp ~/STU/devel/ReMoM/.valgrindrc ~/
cd ~/STU/devel/ReMoM/out/build/clang-debug/bin/Debug
valgrind ./ReMoMber
...some stil showed
==509202== For lists of detected and suppressed errors, rerun with: -s
==509202== ERROR SUMMARY: 1035 errors from 107 contexts (suppressed: 1887 from 1416)

valgrind --gen-suppressions=all ./ReMoMber 2> ~/STU/devel/ReMoM/valgrind-gen-suppressions-more.npp

==509881==  Uninitialised value was created by a stack allocation
==509881==    at 0x414A1A0: Startup_Platform (platform/sdl2/sdl2_Init.c:43)
^^^^^^^^^^  ...
useless         WTF? line 43 in sdl2_Init is the opening brace
Program Id      of the definition of the function Startup_Platform()


objdump --syms <executable_name>
jbalcomb@iMustAi:~/STU/devel/ReMoM/out/build/clang-debug/bin/Debug$ objdump --syms ReMoMber | head -10
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
