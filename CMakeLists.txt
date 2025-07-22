
# move winlibs-x86_64-posix-seh-gcc-15.1.0-mingw-w64msvcrt-13.0.0-r2\mingw64 to C:\mingw64
# sysdm.cpl > Advanced > Environment Variables > User variables > Path > Edit > New > C:\mingw64\bin
# export PATH=$PATH:/c/mingw64/bin
# set PATH=%PATH%;C:\mingw64\bin
# DO NOT setx PATH %PATH%;C:\mingw64\bin - in will destory variable in existsing, as well as combine the User and System PATH variables
# rm -rf build/*
# cmake -S . -B build -DCMAKE_GENERATOR="Ninja Multi-Config"
# cmake --build build --config Debug --verbose
# ...
# rm -rf build/*
# cmake -S . --preset base
# cmake --build --preset base --target all


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



# add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
# add_compile_definitions(WIN32_LEAN_AND_MEAN)
# set(CMAKE_POSITION_INDEPENDENT_CODE ON)



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

install(TARGETS sdl2_ReMoM)

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "MoM v1.31 Reassembly")
set(CPACK_PACKAGE_VERSION "0.0.1")
set(CPACK_PACKAGE_FILE_NAME "ReMoMv131")
set(CPACK_GENERATOR "ZIP")
set(CPACK_SOURCE_GENERATOR "ZIP")
include(CPack)
