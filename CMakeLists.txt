cmake_minimum_required(VERSION 3.22)

project(
    game
    VERSION "0.0.1"
    DESCRIPTION "Reassembly of Master of Magic v1.31 (Simtex, 1995)"
    LANGUAGES C CXX
)

include(FetchContent)

# set(BUILD_SHARED_LIBS OFF)
# option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# # Fetch the external library
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

# cmake -G "Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ..

# DOME
# cmake --build . --target clean
# cmake .
# cmake --build .
