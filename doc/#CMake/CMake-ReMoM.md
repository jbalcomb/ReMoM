
...not devel_edumat/CMake
...just ReMoM specific CMake considerations



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