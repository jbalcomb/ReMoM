
# CMake


SEEALSO:  VSC && CMake



cd ~/devel/ReMoM/build
rm -rf *
jbalcomb@Velociraptor:~/devel/ReMoM/build$ cmake -S /mnt/c/STU/devel/ReMoM/ -B .

SDL2DIR
SDL2MIXERDIR
[cmake] -- ENV{SDL2DIR}: C:\devellib\SDL2-2.32.2
[cmake] -- ENV{SDL2MIXERDIR}: C:\devellib\SDL2_mixer-2.8.1
jbalcomb@Velociraptor:~/devel/ReMoM/build$ find /usr/lib -name SDL2
/usr/lib/x86_64-linux-gnu/cmake/SDL2
/usr/lib/x86_64-linux-gnu/installed-tests/SDL2

dpkg-query -L <package_name>

dpkg-query -L libsdl2-2.0-0
dpkg-query -L libsdl2-dev

dpkg-query -L libsdl2-mixer-2.0-0
dpkg-query -L libsdl2-mixer-dev

jbalcomb@Velociraptor:~/devel/ReMoM/build$ dpkg-query -L libsdl2-2.0-0
/usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2
/usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0





[proc] Executing command: "C:\Program Files\CMake\bin\cmake.EXE" -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE --no-warn-unused-cli -S C:/STU/devel/ReMoM -B c:/STU/devel/ReMoM/build -G "Visual Studio 17 2022" -T host=x64 -A x64

[proc] Executing command: "C:\Program Files\CMake\bin\cmake.EXE" --build c:/STU/devel/ReMoM/build --config Debug --target sdl2_ReMoM -j 4 --






add_library()
add_executable()
add_subdirectory()

include_directories()
target_include_directories()

### ¿ target_include_directories() vs. include_directories() ?



target_compile_options()
target_compile_definitions()
target_compile_features()



## ¿ DEBUG ?

get_target_property()
message()



## FetchContent

include(FetchContent)
¿ Set(FETCHCONTENT_QUIET FALSE) ?
~ Options
    set(BUILD_SHARED_LIBS OFF)
    option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
FetchContent_Declare()
FetchContent_MakeAvailable()

e.g., SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG        release-2.32.0
)
FetchContent_MakeAvailable(SDL2)
¿ set(SDL_INCLUDE_DIR "${SDL2_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/SDL2") ?
