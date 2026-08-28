
...not devel_edumat/CMake
...just ReMoM specific CMake considerations



...so someone can just `code .` from ./ReMoM/



VSC CMake Tools
cmake.openSettings

What we have done:
Create a C++ CMake Project.
Click the ‘Switch Views’ toolbar button in Solution Explorer.
Double-click ‘CMake Targets View’ item.
Open the ‘CMakeLists.txt’ file.
Try to open CMake settings from the “Project” -> “CMake Settings for CMakeProject”.
Double-click the CMakeSettings.json file in Solution Explorer.
Repeat step5 again.
Result: An System.ArgumentException: Class not registered error pops up.
If this is not the real problem of yours, could you please provide us more information to us for the further investigation, such as the exception details, your detailed steps to reproduce the problem, the Screenshot/Video/GIF and anything else?


sumtin, sumtin, ... conan added CMakeUserPresets.json, which just includes build/CMakePresets.json, which has the 
    "name": "conan-debug",
    "displayName": "'conan-debug' config",
    "description": "'conan-debug' configure using 'Ninja-multiconfig' generator",
    "generator": "Ninja-multiconfig",
    "toolset": { "value": "v143", "strategy": "external" },
    "architecture": { "value": "x64", "strategy": "external" },
    "toolchainFile": "conan_toolchain.cmake",
    "binaryDir": "c:\\STU\\devel\\ReMoM\\build"
    ...
    conan_toolchain.cmake
    ...sets compiler, standard, flags, out dirs

[proc] Executing command: "C:\Program Files\CMake\bin\cmake.EXE" --version
[proc] Executing command: "C:\Program Files\CMake\bin\cmake.EXE" -E capabilities
[kit] Successfully loaded 19 kits from C:\Users\jbalcomb\AppData\Local\CMakeTools\cmake-tools-kits.json
[presetController] Reading and validating the presets file "C:\STU\devel\ReMoM\CMakePresets.json"
[presetController] Successfully validated C:\STU\devel\ReMoM\CMakePresets.json against presets schema
[presetController] Expanding presets file C:\STU\devel\ReMoM\CMakePresets.json
[presetController] Successfully expanded presets file C:\STU\devel\ReMoM\CMakePresets.json
[presetController] Reading and validating the presets file "C:\STU\devel\ReMoM\CMakeUserPresets.json"
[presetController] Successfully validated C:\STU\devel\ReMoM\CMakeUserPresets.json against presets schema
[presetController] Reading and validating the presets file "C:\STU\devel\ReMoM\build\CMakePresets.json"
[presetController] Successfully validated C:\STU\devel\ReMoM\build\CMakePresets.json against presets schema
[presetController] Expanding presets file C:\STU\devel\ReMoM\CMakeUserPresets.json
[presetController] Successfully expanded presets file C:\STU\devel\ReMoM\CMakeUserPresets.json
[proc] The command: C:\Windows\System32\where.exe cl exited with code: 1
[proc] The command: C:\Windows\System32\where.exe ninja exited with code: 1
[preset] Configure preset MSVC_x64: No toolset version specified for cl.exe, using latest by default
[preset] Using developer environment from Visual Studio (instance ef3ed198, version 17.14.36310.24, installed at "C:\Program Files\Microsoft Visual Studio\2022\Community")
[proc] Executing command: chcp
[main] Configuring project: ReMoM 
[proc] Executing command: "C:\Program Files\CMake\bin\cmake.EXE" -DCMAKE_INSTALL_PREFIX=C:/STU/devel/ReMoM/out/install/MSVC_x64 -DCMAKE_C_COMPILER=cl.exe -DCMAKE_CXX_COMPILER=cl.exe -S C:/STU/devel/ReMoM -B C:/STU/devel/ReMoM/out/build/MSVC_x64 -G "Visual Studio 17 2022" -T host=x64 -A x64
[cmake] -- Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.19045.
[cmake] -- Configuring done (4.5s)
[cmake] -- Generating done (2.5s)
[cmake] -- Build files have been written to: C:/STU/devel/ReMoM/out/build/MSVC_x64


CMake: Show Configure Command
[cmakefileapi-driver] "C:\Program Files\CMake\bin\cmake.EXE" -DCMAKE_INSTALL_PREFIX=C:/STU/devel/ReMoM/out/install/MSVC_x64 -DCMAKE_C_COMPILER=cl.exe -DCMAKE_CXX_COMPILER=cl.exe -S C:/STU/devel/ReMoM -B C:/STU/devel/ReMoM/out/build/MSVC_x64 -G "Visual Studio 17 2022" -T host=x64 -A x64

CMake: Show Build Command
[main] "C:\Program Files\CMake\bin\cmake.EXE" --build C:/STU/devel/ReMoM/out/build/MSVC_x64 --config Debug --target sdl2_ReMoM --


[main] Building folder: C:/STU/devel/ReMoM/out/build/MSVC_x64 
[build] Starting build
[proc] Executing command: "C:\Program Files\CMake\bin\cmake.EXE" --build C:/STU/devel/ReMoM/out/build/MSVC_x64 --config Debug --target sdl2_ReMoM --
[build] MSBuild version 17.14.14+a129329f1 for .NET Framework
[build] 
[build]   1>Checking Build System
[build]   Building Custom Rule C:/STU/devel/ReMoM/src/MOX/CMakeLists.txt
...
[build]   Generating Code...
[build]   MOX.vcxproj -> C:\STU\devel\ReMoM\out\build\MSVC_x64\src\MOX\Debug\MOX.lib
[build]   Building Custom Rule C:/STU/devel/ReMoM/src/STU/CMakeLists.txt
...
[build]   Generating Code...
[build]   STU.vcxproj -> C:\STU\devel\ReMoM\out\build\MSVC_x64\src\STU\Debug\STU.lib
[build]   Building Custom Rule C:/STU/devel/ReMoM/src/CMakeLists.txt
...
[build]   Generating Code...
[build]   momlib.vcxproj -> C:\STU\devel\ReMoM\out\build\MSVC_x64\src\Debug\momlib.lib
[build]   Building Custom Rule C:/STU/devel/ReMoM/src/CMakeLists.txt
...
[build]   Generating Code...
[build]   sdl2_ReMoM.vcxproj -> C:\STU\devel\ReMoM\out\build\MSVC_x64\src\Debug\sdl2_ReMoM.exe
[driver] Build completed: 00:00:17.929
[build] Build finished with exit code 0



VS Code CMake Tools settings can be configured using a variety of methods, including settings.json files, CMake Presets, and CMake Kits. You can customize build settings, generator, configure arguments, and more to tailor CMake's behavior within your VS Code environment. 



[proc] The command: C:\Windows\System32\where.exe cl exited with code: 1
[proc] The command: C:\Windows\System32\where.exe ninja exited with code: 1
[preset] Configure preset MSVC_x64: No toolset version specified for cl.exe, using latest by default
[preset] Using developer environment from Visual Studio (instance ef3ed198, version 17.14.36310.24, installed at "C:\Program Files\Microsoft Visual Studio\2022\Community")
[proc] Executing command: chcp
[main] Configuring project: ReMoM 
[proc] Executing command: "C:\Program Files\CMake\bin\cmake.EXE" -DCMAKE_INSTALL_PREFIX=C:/STU/devel/ReMoM/out/install/MSVC_x64 -DCMAKE_C_COMPILER=cl.exe -DCMAKE_CXX_COMPILER=cl.exe -S C:/STU/devel/ReMoM -B C:/STU/devel/ReMoM/out/build/MSVC_x64 -G "Visual Studio 17 2022" -T host=x64 -A x64
[cmake] -- Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.19045.
[cmake] -- The C compiler identification is MSVC 19.44.35213.0
[cmake] -- Detecting C compiler ABI info
[cmake] -- Detecting C compiler ABI info - done
[cmake] -- Check for working C compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe - skipped
[cmake] -- Detecting C compile features
[cmake] -- Detecting C compile features - done



# WSL on Velociraptor (WKS#1)
rm -rf build/*

cmake -S /mnt/c/STU/devel/ReMoM/ -B build -D CMAKE_GENERATOR="Unix Makefiles"
cmake --build build --config Debug
cmake --install build --config Debug

cmake -S /mnt/c/STU/devel/ReMoM/ -B build -D CMAKE_GENERATOR="Ninja Multi-Config"
cmake --build build --config Debug
cmake --install build --config Debug

sudo apt-get install ninja-build

sudo apt-get install libsdl2-2.0-0
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-mixer-2.0-0
sudo apt-get install libsdl2-mixer-dev

sudo apt search sdl2
libghc-sdl2-dev
libghc-sdl2-doc
libghc-sdl2-prof
libghc-sdl2-ttf-dev
libghc-sdl2-ttf-doc
libghc-sdl2-ttf-prof
libsdl2-2.0-0
libsdl2-dev
libsdl2-doc
libsdl2-gfx-1.0-0
libsdl2-gfx-dev
libsdl2-gfx-doc
libsdl2-image-2.0-0
libsdl2-image-dev
libsdl2-mixer-2.0-0
libsdl2-mixer-dev
libsdl2-net-2.0-0
libsdl2-net-dev
libsdl2-ttf-2.0-0
libsdl2-ttf-dev



TODO  split code in to library-per-module  (because symbol name conflicts)

TODO  eliminate sub-project overlap  (make it ~*impossible?)



## ./cmake

## Root / Top-Level CmakeLists.txt

## src CmakeLists.tst

## src/MOX CmakeLists.tst

## src/STU CmakeLists.txt



## MOVEME

"source dir" vs. "binary dir"
`./` vs. `./build`


get_target_property(inc_dir MOX INCLUDE_DIRECTORIES)
message(STATUS "MOX INCLUDE_DIRECTORIES: ${inc_dir}")

get_target_property(inc_dir STU INCLUDE_DIRECTORIES)
message(STATUS "STU INCLUDE_DIRECTORIES: ${inc_dir}")


add_library(momlib STATIC ${MOMLIB_SOURCES})
set_target_properties(momlib PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(momlib PUBLIC
target_include_directories(momlib PUBLIC
target_compile_definitions(momlib PRIVATE

add_executable(sdl2_ReMoM ReMoM.c ${SDL2_SOURCES})
set_target_properties(sdl2_ReMoM PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(sdl2_ReMoM PUBLIC
target_include_directories(sdl2_ReMoM PUBLIC
target_compile_definitions(sdl2_ReMoM PRIVATE


# target_compile_features(ReMoM PUBLIC cxx_std_23)
# target_compile_definitions(ReMoM PUBLIC -DNOMINMAX)
# target_compile_options(ReMoM PUBLIC /W4 /WX /Debug /Od /Z7)
# target_compile_options(ReMoM PUBLIC "-Wall")
# target_compile_definitions(momlib PRIVATE STU_DEBUG=TRUE)
# target_compile_definitions(momlib PRIVATE STU_TRACE=TRUE)


#     set_target_properties(ReMoM PROPERTIES WIN32_EXECUTABLE TRUE)


# if(_STU_WIN32)
#     add_library(momlib STATIC ${MOMLIB_SOURCES} ${WIN32_SOURCES})
# endif()
# if(_STU_SDL2)
#     add_library(momlib STATIC ${MOMLIB_SOURCES} ${SDL2_SOURCES})
# endif()
add_library(momlib STATIC ${MOMLIB_SOURCES})


message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message(STATUS "CMAKE_SOUCE_PATH: ${CMAKE_SOUCE_PATH}")
message(STATUS "CURRENT_CMAKE_SOURCE_PATH: ${CURRENT_CMAKE_SOURCE_PATH}")


message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
# Add SDL2 CMake modules
# list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)
# list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/sdl2)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/sdl2)
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")


# Turn off SDL2 Shared Library build
set(BUILD_SHARED_LIBS OFF)



### Compile Definitions

_CRT_SECURE_NO_WARNINGS
WIN32_LEAN_AND_MEAN
MAY     add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
MAY     add_compile_definitions(WIN32_LEAN_AND_MEAN)
...any real/good reason not to do so?
...if not, *should* be in top-level CML?

NAY     add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
YAY     target_compile_definitions(momlib PRIVATE $<$<CONFIG:Debug>:_CRT_SECURE_NO_WARNINGS>)

# This is the most precise and recommended way, applying the definition only to the targets that need it.
#     add_executable(MyApplication main.cpp)
#     target_compile_definitions(MyApplication PRIVATE WIN32_LEAN_AND_MEAN)
# Or, if you have a library:
#     add_library(MyLibrary STATIC library.cpp)
#     target_compile_definitions(MyLibrary PUBLIC WIN32_LEAN_AND_MEAN)
# Using PUBLIC or INTERFACE will propagate the definition to targets that link against MyLibrary.

# if ( WIN32 )
#     add_compile_definitions(_STU_Win32)
# else ()
#     add_compile_definitions(_STU_SDL2)
# endif ()



### GTest

¿ minimal impact approach ?



```sh
# Eh? ...install via conan, winget ... apt-get install 
# find_package(GTest)
# C:\Program Files\CMake\share\cmake-3.27\Modules\FindGTest.cmake

include(FetchContent)
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG v1.14.0)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# add_executable(hello_test hello_test.c)
# target_link_libraries(hello_test GTest::gtest_main)
# include(GoogleTest)
# gtest_discover_tests(hello_test)

# via Conant
# include(cmake/tests.cmake)
# find_package(GTest REQUIRED)
# enable_testing()
# add_subdirectory(tests)



# WSL on Velociraptor (WKS#1)
# rm -rf build/*
# cmake -S /mnt/c/STU/devel/ReMoM/ -B build -D CMAKE_GENERATOR="Ninja Multi-Config"
# cmake --build build --config Debug
# cmake --install build --config Debug
```