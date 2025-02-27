cmake_minimum_required(VERSION 3.22)

project(
    ReMoM
    VERSION "0.0.1"
    DESCRIPTION "Reassembly of Master of Magic v1.31 (Simtex, 1995)"
    LANGUAGES C CXX
)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# prior to CMake 3.18, set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
# ...it has to be set prior to the invocation of cmake...
# cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 /path/to/src
# ...explicitly specify a generator like this:  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 /path/to/src -G Ninja

include(FetchContent)

# Set(FETCHCONTENT_QUIET FALSE)

set(BUILD_SHARED_LIBS OFF)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
set(BUILD_SHARED_LIBS FALSE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
# Disable Shared Libs
set(BUILD_SHARED_LIBS OFF)
set(SDL_SHARED OFF)
# set(SDL_STATIC ON)

# Disable AVIF Support
# set(SDL3IMAGE_AVIF OFF)

set(SDL2_DISABLE_UNINSTALL ON)  ## WTF  [cmake] --   SDL2_DISABLE_UNINSTALL      (Wanted: ON): OFF

# [cmake] --  CMAKE_C_FLAGS_DEBUG:   /Zi /Ob0 /Od 
# [cmake] --  CMAKE_CXX_FLAGS_DEBUG: /Zi /Ob0 /Od /RTC1
# [cmake] --  CFLAGS:        /DWIN32 /D_WINDOWS
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG        release-2.32.0
)
FetchContent_MakeAvailable(SDL2)
set(BUILD_SHARED_LIBS "" CACHE BOOL "Build shared libraries" FORCE) # Clear the variable after usage
set(SDL_INCLUDE_DIR "${SDL2_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/SDL2")
message("SDL_INCLUDE_DIR: ${SDL_INCLUDE_DIR}")

# FetchContent_Declare(
#     json-c
#     GIT_REPOSITORY https://github.com/json-c/json-c
#     GIT_TAG json-c-0.18-20240915)
# FetchContent_MakeAvailable(json-c)
# # NO-WORKIE  # Get the include directories from the fetched library
# # NO-WORKIE  get_target_property(json-c_INCLUDE_DIRS json-c INTERFACE_INCLUDE_DIRECTORIES)
# # NO-WORKIE  message(json-c_INCLUDE_DIRS="${json-c_INCLUDE_DIRS}")
# # NO-WORKIE  # json-c_INCLUDE_DIRS="$<BUILD_INTERFACE:C:/STU/devel/ReMoM/_deps/json-c-src>$<BUILD_INTERFACE:C:/STU/devel/ReMoM/_deps/json-c-build>"
# # NO-WORKIE  # Add the include directories to your project target
# # NO-WORKIE  # target_include_directories(json-c PRIVATE ${json-c_INCLUDE_DIRS})
# # NO-WORKIE  target_include_directories(json-c PUBLIC ${json-c_INCLUDE_DIRS})
# # NO-WORKIE  # target_include_directories(STU PRIVATE ${json-c_INCLUDE_DIRS})

add_subdirectory(src)

# add_subdirectory(platform)


# if ( WIN32 )
#     add_compile_definitions(_STU_Win32)
# else ()
#     add_compile_definitions(_STU_SDL2)
# endif ()
add_compile_definitions(_STU_SDL2)



# CMAKE_MSVC_FLAGS for project-wide MSVC settings
# ¿ CMAKE_GNUCC_FLAGS ?
# ¿ CMAKE_CLANG_FLAGS ?



# cmake -G "Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ..

# DOME
# cmake --build . --target clean
# cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON .
# cmake --build .
