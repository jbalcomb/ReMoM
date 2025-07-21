
cmake_minimum_required(VERSION 3.22)

project(
    ReMoM
    VERSION "0.0.1"
    DESCRIPTION "Reassembly of Master of Magic v1.31 (Simtex, 1995)"
    LANGUAGES C
)
# TODO  set(CMAKE_C_STANDARD 99)
# TODO  set(CMAKE_C_STANDARD_REQUIRED ON) # Make the standard mandatory
# TODO  set(C_EXTENSIONS OFF)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# if(DO_TEST)
# GoogleTest requires at least C++17
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# ¿ TODO  set(CXX_EXTENSIONS OFF) ?
enable_testing()
# endif()

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/sdl2)

set(BUILD_SHARED_LIBS OFF)

include(FetchContent)
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG v1.14.0)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(src)

add_subdirectory(tests)
