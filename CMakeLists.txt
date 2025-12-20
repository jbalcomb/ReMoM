
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
# cmake --build build --config Debug --verbose > _cmake_build.txt
# ctest --test-dir build -C Debug --rerun-failed --output-on-failure -VV

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



set(CMAKE_INSTALL_LOCAL_ONLY TRUE)
# CMAKE_INSTALL_PREFIX to a location within your user directory (e.g., C:\Users\<YourUser>\Install).
# # Use this snippet *after* PROJECT(xxx):
# IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
#   SET(CMAKE_INSTALL_PREFIX <path> CACHE PATH <comment> FORCE)
# ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)


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
# set(BUILD_SHARED_LIBS ON)

include(FetchContent)
FetchContent_Declare(googletest GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG v1.14.0)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
message(STATUS "GTEST_INCLUDE_DIR='${GTEST_INCLUDE_DIR}'")
message(STATUS "GTEST_INCLUDE_DIRS='${GTEST_INCLUDE_DIRS}'")

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)


# # SDL2::Main  SDL2::Core  SDL2::Mixer
# get_target_property(imported_implib SDL2::Main IMPORTED_IMPLIB)
# message(STATUS "imported_implib: ${imported_implib}")
# get_target_property(imported_implib SDL2::Core IMPORTED_IMPLIB)
# message(STATUS "imported_implib: ${imported_implib}")
# get_target_property(imported_implib SDL2::Mixer IMPORTED_IMPLIB)
# message(STATUS "imported_implib: ${imported_implib}")
# # >>> [cmake] -- imported_implib: imported_implib-NOTFOUND
# # >>> [cmake] -- imported_implib: imported_implib-NOTFOUND
# # >>> [cmake] -- imported_implib: imported_implib-NOTFOUND
# # ...SDL provides package configuration files which create imported targets, defining the DLL location(s) via IMPORTED_LOCATION_RELEASE or IMPORTED_LOCATION_DEBUG.
# get_target_property(imported_location_debug SDL2::Main IMPORTED_LOCATION_DEBUG)
# message(STATUS "imported_location_debug: ${imported_location_debug}")
# get_target_property(imported_location_debug SDL2::Core IMPORTED_LOCATION_DEBUG)
# message(STATUS "imported_location_debug: ${imported_location_debug}")
# get_target_property(imported_location_debug SDL2::Mixer IMPORTED_LOCATION_DEBUG)
# message(STATUS "imported_location_debug: ${imported_location_debug}")
# # >>> [cmake] -- imported_location_debug: imported_location_debug-NOTFOUND
# # >>> [cmake] -- imported_location_debug: imported_location_debug-NOTFOUND
# # >>> [cmake] -- imported_location_debug: imported_location_debug-NOTFOUND
# get_target_property(imported_location_release SDL2::Main IMPORTED_LOCATION_RELEASE)
# message(STATUS "imported_location_release: ${imported_location_release}")
# get_target_property(imported_location_release SDL2::Core IMPORTED_LOCATION_RELEASE)
# message(STATUS "imported_location_release: ${imported_location_release}")
# get_target_property(imported_location_release SDL2::Mixer IMPORTED_LOCATION_RELEASE)
# message(STATUS "imported_location_release: ${imported_location_release}")
# # >>> [cmake] -- imported_location_release: imported_location_release-NOTFOUND
# # >>> [cmake] -- imported_location_release: imported_location_release-NOTFOUND
# # >>> [cmake] -- imported_location_release: imported_location_release-NOTFOUND
# get_target_property(imported_location SDL2::Main IMPORTED_LOCATION)
# message(STATUS "imported_location: ${imported_location}")
# get_target_property(imported_location SDL2::Core IMPORTED_LOCATION)
# message(STATUS "imported_location: ${imported_location}")
# get_target_property(imported_location SDL2::Mixer IMPORTED_LOCATION)
# message(STATUS "imported_location: ${imported_location}")
# # >>> [cmake] -- imported_location: imported_location-NOTFOUND
# # >>> [cmake] -- imported_location: C:/devellib/SDL2-2.32.2/lib/x64/SDL2.lib
# # >>> [cmake] -- imported_location: C:/devellib/SDL2_mixer-2.8.1/lib/x64/SDL2_mixer.lib

add_subdirectory(STU)
add_subdirectory(MoX)
add_subdirectory(MoM)
add_subdirectory(tests)
add_subdirectory(src)



install(TARGETS ReMoMber)
# install(FILES $<TARGET_FILE:SDL2::SDL2> DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES $<TARGET_FILE:SDL2::Core> DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES $<TARGET_FILE:SDL2::Mixer> DESTINATION ${CMAKE_INSTALL_BINDIR})
message(STATUS "CMAKE_INSTALL_BINDIR='${CMAKE_INSTALL_BINDIR}'")

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "MoM v1.31 Reassembly")
set(CPACK_PACKAGE_VERSION "0.0.1")
set(CPACK_PACKAGE_FILE_NAME "ReMoMv131")
set(CPACK_GENERATOR "ZIP")
set(CPACK_SOURCE_GENERATOR "ZIP")
include(CPack)
