


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


## Windows (Visual Studio 2022)

The CMake build defaults to SDL2. From the project root:

```sh
cmake -B build
cmake --build build --config Debug
```

SDL2 DLLs from `C:\devellib\SDL2-*` are needed at runtime. To use SDL3
instead, pass `-DUSE_SDL3=ON`.


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
