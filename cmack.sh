
# SDL2-2.32.2
# SDL2_mixer-2.8.1

# sudo apt-get install libsdl2-dev libsdl2-2.0-0

# mkdir build
# cd build
# cmake ..
# make
# sudo make install

cmake --build . --target clean
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON .
cmake --build .

cmake --fresh
rm out/build/clang-nosound-debug/CMakeCache.txt
rm -rf out/build/clang-nosound-debug/CMakeFiles/
cmake --preset clang-nosound-debug
cmake --build --preset clang-nosound-debug
ctest --preset clang-nosound-debug

sudp aptr install clang
sudp aptr install 
sudo apt install build-essential cmake curl

chmod u+x ./scripts/install-cmake-linux.sh
./scripts/install-cmake-linux.sh
chmod u+x ./scripts/install-sdl3-linux.sh
./scripts/install-sdl3-linux.sh

sudo apt-get install build-essential git make \
pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev \
libaudio-dev libfribidi-dev libjack-dev libsndio-dev libx11-dev libxext-dev \
libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev \
libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev \
libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev libthai-dev \
libpipewire-0.3-dev libwayland-dev libdecor-0-dev liburing-dev

https://wiki.libsdl.org/SDL3/README-cmake
Building SDL on UNIX
SDL will build with very few dependencies, but for full functionality you should install the packages detailed in README-linux.md.
Assuming you're in the SDL source directory, building and installing to /usr/local can be done with:
cmake -S . -B build
cmake --build build
sudo cmake --install build --prefix /usr/local
