
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

# cd C:\STU\devel\ReMoM\
# cmake --workflow --preset=MSVC-debug
# ctest --preset=MSVC-debug
# Errors while running CTest
# Output from these tests are in: C:/STU/devel/ReMoM/out/build/MSVC-debug/Testing/Temporary/LastTest.log
# Use "--rerun-failed --output-on-failure" to re-run the failed cases verbosely.
# ctest --preset=MSVC-debug --rerun-failed --output-on-failure

# cmake_minimum_required(VERSION 3.22)
cmake_minimum_required(VERSION 4.2)

project(
    ReMoM
    VERSION "0.0.1"
    DESCRIPTION "Reassembly of Master of Magic v1.31 (Simtex, 1995)"
    LANGUAGES C
)
# set(CMAKE_C_STANDARD 99)
# set(CMAKE_C_STANDARD_REQUIRED ON) # Make the standard mandatory
# set(C_EXTENSIONS OFF)

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
include(CTest)
# enable_testing()
# endif()

# Modern CMake
#"cacheVariables": {
#    "TOTES_DEBUG": "TOTES_TRUE"
#}
# # Check if the variable was set by the preset
# if(TOTES_DEBUG)
#     target_compile_definitions(
#         ReMoM
#         PRIVATE
#         "STU_DEBUG=TRUE"
#     )
# endif()
# Optional: Explicitly add NDEBUG to the Release configuration if not already present
# target_compile_definitions(
#     ReMoMber
#     PRIVATE
#     "$<$<CONFIG:Release>:NDEBUG>"
# )
# [cmake]   Cannot specify compile definitions for target "ReMoMber" which is not built
# [cmake]   by this project.

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/sdl2)
include(${PROJECT_SOURCE_DIR}/cmake/ReMoM_Common_Compile_Definitions.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/ReMoM_Common_Compile_Options.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/ReMoM_PostBuild_Copies.cmake)

set(BUILD_SHARED_LIBS OFF)
# set(BUILD_SHARED_LIBS ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
    EXCLUDE_FROM_ALL
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
message(STATUS "GTEST_INCLUDE_DIR='${GTEST_INCLUDE_DIR}'")
message(STATUS "GTEST_INCLUDE_DIRS='${GTEST_INCLUDE_DIRS}'")
# NOTE: Do not set global PRE_TEST discovery; configure per target where runtime file staging requires it.

find_package(Doxygen COMPONENTS doxygen dot)
if (DOXYGEN_FOUND)
    message(STATUS "Doxygen found. Generating documentation.")
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
    doxygen_add_docs(doxyReMoM
        ${DOXYGEN_INPUT_DIR}
        DOXYGEN_PROJECT_NAME "ReMoM"
        DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxy/ReMoM/html
    )
    set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/MoM/src)
    doxygen_add_docs(doxyMOMlib
        ${DOXYGEN_INPUT_DIR}
        DOXYGEN_PROJECT_NAME "MOM Library"
        DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxy/MOMlib/html
    )
    set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/MoX/src)
    doxygen_add_docs(doxyMOXlib
        ${DOXYGEN_INPUT_DIR}
        DOXYGEN_PROJECT_NAME "MOX Library"
        DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxy/MOXlib/html
    )
    set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/STU/src)
    doxygen_add_docs(doxySTUlib
        ${DOXYGEN_INPUT_DIR}
        DOXYGEN_PROJECT_NAME "MOX Library"
        DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxy/STUlib/html
    )

    # set(ALL_DOC_TARGETS)
    add_custom_target(docs COMMENT "Building all project documentation")
    # to rebuild everytime, add the ALL target add_custom_target(docs ALL COMMENT "Building all project documentation")
    # add_dependencies(docs ${ALL_DOC_TARGETS})
    add_dependencies(docs doxyReMoM doxyMOMlib doxyMOXlib doxySTUlib)

else()
    message(STATUS "Doxygen not found. Documentation will not be generated.")
endif()
# # Locate Doxygen and related tools
# find_package(Doxygen REQUIRED)
# if (DOXYGEN_FOUND)
#     # Define source directories to scan
#     set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
# 
#     # Add a custom target named 'docs'
#     doxygen_add_docs(docs ${DOXYGEN_INPUT_DIR}
#         COMMENT "Generating API documentation with Doxygen"
#         # Optional: set specific Doxygen options as CMake variables
#         # DOXYGEN_PROJECT_NAME "My Awesome Project"
#         # DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/html
#     )
# 
#     # Optional: make documentation part of the 'all' build target
#     # add_custom_target(docs ALL ...) # The doxygen_add_docs handles ALL internally if you use the right arguments
# endif()
# cmake --build . --target docs
# cmake --build . --target doxyReMoM

# Coverage support
option(ENABLE_COVERAGE "Enable code coverage instrumentation and targets" OFF)
if(ENABLE_COVERAGE)
    message(STATUS "Code coverage enabled")
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        message(STATUS "Using Clang coverage flags")
        add_compile_options(-g -O0 -fprofile-instr-generate -fcoverage-mapping)
        add_link_options(-fprofile-instr-generate)
    elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        message(STATUS "Using GCC coverage flags")
        add_compile_options(-g -O0 --coverage)
        add_link_options(--coverage)
    else()
        message(WARNING "Coverage enabled but unsupported compiler (${CMAKE_C_COMPILER_ID}); coverage targets may fail")
    endif()
endif()

# https://wiki.libsdl.org/SDL3/README/cmake
list(APPEND CMAKE_PREFIX_PATH "C:/devellib/SDL3-3.4.2")
find_package(SDL3 REQUIRED)
if(NOT DISABLE_AUDIO)
    list(APPEND CMAKE_PREFIX_PATH "C:/devellib/SDL3_mixer-3.2.0")
    find_package(SDL3_mixer REQUIRED)
else()
    add_compile_definitions(NO_SOUND_LIBRARY)
endif()

# Extract SDL3.dll path from the imported target for post-build copies
get_target_property(SDL3_DLL_FILE_PATH SDL3::SDL3-shared IMPORTED_LOCATION)
message(CHECK_START "Finding SDL3.dll")
if(SDL3_DLL_FILE_PATH AND EXISTS "${SDL3_DLL_FILE_PATH}")
    message(CHECK_PASS "The file exists: ${SDL3_DLL_FILE_PATH}")
else()
    message(CHECK_FAIL "SDL3.dll not found via imported target")
endif()

if(NOT DISABLE_AUDIO)
    get_target_property(SDL3_MIXER_DLL_FILE_PATH SDL3_mixer::SDL3_mixer-shared IMPORTED_LOCATION)
    message(CHECK_START "Finding SDL3_mixer.dll")
    if(SDL3_MIXER_DLL_FILE_PATH AND EXISTS "${SDL3_MIXER_DLL_FILE_PATH}")
        message(CHECK_PASS "The file exists: ${SDL3_MIXER_DLL_FILE_PATH}")
    else()
        message(CHECK_FAIL "SDL3_mixer.dll not found via imported target")
    endif()
endif()
set(GMOCK_DLL_FILE_PATH ${PROJECT_SOURCE_DIR})
string(APPEND GMOCK_DLL_FILE_PATH "/bin")
string(APPEND GMOCK_DLL_FILE_PATH "/gmock.dll")
message(CHECK_START "Finding gmock.dll")
if(EXISTS "${GMOCK_DLL_FILE_PATH}")
    message(CHECK_PASS "The file exists: ${GMOCK_DLL_FILE_PATH}")
else()
    message(CHECK_FAIL "The file does not exist: ${GMOCK_DLL_FILE_PATH}")
endif()

# so GTest can find the DLLs
# # set(REMOM_TEST_PATH_ENV "PATH=${PROJECT_SOURCE_DIR}/bin;$ENV{PATH}")
# # # -D "TEST_PROPERTIES=ENVIRONMENT";"PATH=C:/STU/devel/ReMoM/bin\";"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.44.35207\bin\HostX64\x64\";"C:\Program Files\Micro
# set(MY_CUSTOM_PATH "${PROJECT_SOURCE_DIR}/bin")
# set(REMOM_TEST_PATH_ENV "PATH=${MY_CUSTOM_PATH}$<SEMICOLON>$ENV{PATH}")
# message(STATUS "REMOM_TEST_PATH_ENV: ${REMOM_TEST_PATH_ENV}")
set(REMOM_TEST_PATH_ENV "PATH=${PROJECT_SOURCE_DIR}/bin")


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

set(PLATFORM "SDL3")

add_subdirectory(STU)
add_subdirectory(platform)
add_subdirectory(MoX)
add_subdirectory(MoM)
add_subdirectory(src)
add_subdirectory(tests)

# Simple coverage helper target: runs tests (requires tests to be configured)
if(ENABLE_COVERAGE)
    if(WIN32)
        add_custom_target(coverage
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -j $<NUMBER_OF_PROCESSORS>
            COMMAND powershell -NoProfile -ExecutionPolicy Bypass -File "${CMAKE_SOURCE_DIR}/cmake/generate_coverage.ps1" -BuildDir "${CMAKE_BINARY_DIR}" -OutDir "${CMAKE_BINARY_DIR}/coverage"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Run tests and generate coverage HTML (Windows: uses llvm-profdata/llvm-cov if available)"
            VERBATIM
        )
    else()
        add_custom_target(coverage
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -j $<NUMBER_OF_PROCESSORS>
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Run tests for coverage data collection (use llvm-profdata/llvm-cov or lcov/genhtml to generate reports)"
            VERBATIM
        )
    endif()
endif()


# Installation rules for the executable (runtime components)
# install(TARGETS MyExe RUNTIME DESTINATION bin)
# install(TARGETS ReMoMber)
install(
    TARGETS ReMoMber
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Installation rules for libraries
if(WIN32)

    install(FILES ${SDL3_DLL_FILE_PATH} DESTINATION ${CMAKE_INSTALL_BINDIR})
    if(NOT DISABLE_AUDIO)
        install(FILES ${SDL3_MIXER_DLL_FILE_PATH} DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
    message(STATUS "CMAKE_INSTALL_BINDIR='${CMAKE_INSTALL_BINDIR}'")

else()

    # On Linux/Unix, shared libraries (e.g., .so files) are installed in the 'lib' directory.
    # The LIBRARY DESTINATION handles this automatically for non-Windows platforms.
    install(TARGETS ReMoMber 
            LIBRARY DESTINATION lib 
            ARCHIVE DESTINATION lib 
            CONFIGURATIONS Release)

endif()

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "MoM v1.31 Reassembly")
set(CPACK_PACKAGE_VERSION "0.0.1")
set(CPACK_PACKAGE_FILE_NAME "ReMoMv131")
set(CPACK_GENERATOR "ZIP")
set(CPACK_SOURCE_GENERATOR "ZIP")
# Include optional test data staging helper (defines `stage_test_data` target)
if(EXISTS "${CMAKE_SOURCE_DIR}/cmake/stage_test_data.cmake")
    include("${CMAKE_SOURCE_DIR}/cmake/stage_test_data.cmake")
endif()

include(CPack)
