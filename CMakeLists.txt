# cmake_minimum_required(VERSION 3.22)
# project(
#     ReMoM
#     VERSION "0.0.1"
#     DESCRIPTION "Reassembly of Master of Magic v1.31 (Simtex, 1995)"
#     LANGUAGES C CXX
# )

# cmake_minimum_required(VERSION 3.1)
# set(CMAKE_C_STANDARD 89)
# set(CMAKE_C_STANDARD_REQUIRED TRUE)
# set(CMAKE_CXX_STANDARD 98)
# set(CMAKE_CXX_EXTENSIONS OFF)
cmake_minimum_required(VERSION 3.22)
# set(CMAKE_C_STANDARD 99)
# set(CMAKE_CXX_STANDARD 98)
project(
    ReMoM
    VERSION "0.0.1"
    DESCRIPTION "Reassembly of Master of Magic v1.31 (Simtex, 1995)"
    LANGUAGES C
)

set(BUILD_SHARED_LIBS OFF)

# downloaded SDL2-devel-2.32.2-VC.zip; unzipped into C:\devellib\SDL2-2.32.2\; added SDL2_DIR as "C:\devellib\SDL2-2.32.2"
set(SDL2_DIR "$ENV{SDL2DIR}")
find_package(SDL2 REQUIRED)

include_directories(SYSTEM ${SDL2_INCLUDE_DIR})

# include_directories(SYSTEM C:/devellib/SDL2-2.32.2/include)
# include_directories(SYSTEM C:/devellib/SDL2_mixer-2.8.1/include)
include_directories(C:/devellib/SDL2-2.32.2/include)
include_directories(C:/devellib/SDL2_mixer-2.8.1/include)
#include "C:\devellib\SDL2-2.32.2\include\SDL.h"
#include "C:\devellib\SDL2_mixer-2.8.1\include\SDL_mixer.h"

# get_target_property(include_dirs ${PROJECT_NAME} INCLUDE_DIRECTORIES)
# get_target_property(include_dirs SYSTEM INCLUDE_DIRECTORIES)
# get_target_property(include_dirs ${CMAKE_CURRENT_SOURCE_DIR} INCLUDE_DIRECTORIES)  ...called with non-existent target "C:/STU/devel/ReMoM"
# message(STATUS "Include directories for ${PROJECT_NAME}: ${include_dirs}")



# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# prior to CMake 3.18, set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
# ...it has to be set prior to the invocation of cmake...
# cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 /path/to/src
# ...explicitly specify a generator like this:  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 /path/to/src -G Ninja

# include(FetchContent)
# 
# # Set(FETCHCONTENT_QUIET FALSE)
# 
# set(BUILD_SHARED_LIBS OFF)
# # option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
# set(BUILD_SHARED_LIBS FALSE)
# set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
# # Disable Shared Libs
# set(BUILD_SHARED_LIBS OFF)
# 
# # find_package(SDL2)
# # if(NOT SDL2_FOUND)
#     set(SDL_SHARED OFF)
#     # set(SDL_STATIC ON)
# 
#     # Disable AVIF Support
#     # set(SDL3IMAGE_AVIF OFF)
# 
#     set(SDL2_DISABLE_UNINSTALL ON)  ## WTF  [cmake] --   SDL2_DISABLE_UNINSTALL      (Wanted: ON): OFF
# 
#     # [cmake] --  CMAKE_C_FLAGS_DEBUG:   /Zi /Ob0 /Od 
#     # [cmake] --  CMAKE_CXX_FLAGS_DEBUG: /Zi /Ob0 /Od /RTC1
#     # [cmake] --  CFLAGS:        /DWIN32 /D_WINDOWS
#     FetchContent_Declare(
#         SDL2
#         GIT_REPOSITORY https://github.com/libsdl-org/SDL
#         GIT_TAG        release-2.32.0
#     )
#     FetchContent_MakeAvailable(SDL2)
#     set(BUILD_SHARED_LIBS "" CACHE BOOL "Build shared libraries" FORCE) # Clear the variable after usage
#     set(SDL_INCLUDE_DIR "${SDL2_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}/SDL2")
#     message("SDL_INCLUDE_DIR: ${SDL_INCLUDE_DIR}")
# # endif ()



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

# set_target_properties(MOX PROPERTIES LINKER_LANGUAGE C)
# set_target_properties(STU PROPERTIES LINKER_LANGUAGE C)

# add_subdirectory(platform)


## TODO  change to target_compile_definitions()
# e.g.,  # Define DEBUG for MyTarget when building in Debug configuration
# e.g.,  target_compile_definitions(MyTarget PRIVATE
# e.g.,      $<$<CONFIG:Debug>:DEBUG>
# e.g.,  )
add_compile_definitions(WIN32_LEAN_AND_MEAN)
# This is the most precise and recommended way, applying the definition only to the targets that need it.
#     add_executable(MyApplication main.cpp)
#     target_compile_definitions(MyApplication PRIVATE WIN32_LEAN_AND_MEAN)
# Or, if you have a library:
#     add_library(MyLibrary STATIC library.cpp)
#     target_compile_definitions(MyLibrary PUBLIC WIN32_LEAN_AND_MEAN)
# Using PUBLIC or INTERFACE will propagate the definition to targets that link against MyLibrary.
add_compile_definitions(STU_DEBUG=TRUE)
add_compile_definitions(STU_TRACE=TRUE)
# if ( WIN32 )
#     add_compile_definitions(_STU_Win32)
# else ()
#     add_compile_definitions(_STU_SDL2)
# endif ()
add_compile_definitions(_STU_SDL2=TRUE)



# CMAKE_MSVC_FLAGS for project-wide MSVC settings
# ¿ CMAKE_GNUCC_FLAGS ?
# ¿ CMAKE_CLANG_FLAGS ?



# cmake -G "Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ..

# DOME
# cmake --build . --target clean
# cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON .
# cmake --build .
