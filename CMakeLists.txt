cmake_minimum_required(VERSION 3.15)

project(
    ReMoM
    VERSION "0.0.1"
    DESCRIPTION "Reassembly of Master of Magic v1.31 (Simtex, 1995)"
    LANGUAGES C
)

# GoogleTest requires at least C++17
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# noworkie?  set(ENV{CTEST_OUTPUT_ON_FAILURE} ON)
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
# set(ENV{CTEST_PARALLEL_LEVEL} 2)
# list(APPEND CMAKE_CTEST_ARGUMENTS "--parallel 2")
include(CTest)

# Valgrind memcheck support — Linux / GCC / Clang only
option(ENABLE_VALGRIND "Run CTest memcheck pass under Valgrind (Linux GCC/Clang only)" OFF)
if(ENABLE_VALGRIND)
    if(WIN32)
        message(WARNING "ENABLE_VALGRIND is not supported on Windows; ignoring.")
    else()
        find_program(VALGRIND_EXECUTABLE valgrind)
        if(VALGRIND_EXECUTABLE)
            set(MEMORYCHECK_COMMAND "${VALGRIND_EXECUTABLE}")
            set(MEMORYCHECK_COMMAND_OPTIONS
                "--error-exitcode=1 --leak-check=full --track-origins=yes --show-leak-kinds=all")
            message(STATUS "Valgrind: ${VALGRIND_EXECUTABLE}")
        else()
            message(WARNING "ENABLE_VALGRIND is ON but valgrind was not found in PATH.")
        endif()
    endif()
endif()

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "Disable FetchContent update checks" FORCE)
set(FETCHCONTENT_UPDATES_DISCONNECTED_GOOGLETEST ON CACHE BOOL "Disable googletest update checks" FORCE)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
    EXCLUDE_FROM_ALL
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# Don't pull googletest's install rules into our package — they only matter at dev/test time.
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Custom Find modules for SDL2 (from chocolate-doom)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# SDL backend selection: SDL2 (default) or SDL3 (opt-in with -DUSE_SDL3=ON)
# https://wiki.libsdl.org/SDL3/README/cmake
option(USE_SDL3 "Use SDL3 instead of SDL2 (default: SDL2)" OFF)

option(USE_WIN32 "Use native Win32 backend instead of SDL (no SDL dependency)" OFF)

if(USE_WIN32)
    add_compile_definitions(NO_SOUND_LIBRARY)
elseif(USE_SDL3)
    if(WIN32)
        list(APPEND CMAKE_PREFIX_PATH "C:/devellib/SDL3-3.4.2")
    endif()
    find_package(SDL3 REQUIRED)
    if(NOT DISABLE_AUDIO)
        if(WIN32)
            list(APPEND CMAKE_PREFIX_PATH "C:/devellib/SDL3_mixer-3.2.0")
        endif()
        find_package(SDL3_mixer REQUIRED)
    else()
        add_compile_definitions(NO_SOUND_LIBRARY)
    endif()
else()
    if(WIN32)
        list(APPEND CMAKE_PREFIX_PATH "C:/devellib/SDL2-2.32.2")
        set(SDL2_DIR "C:/devellib/SDL2-2.32.2" CACHE PATH "" FORCE)
    endif()
    find_package(SDL2 REQUIRED)
    if(NOT DISABLE_AUDIO)
        if(WIN32)
            list(APPEND CMAKE_PREFIX_PATH "C:/devellib/SDL2_mixer-2.8.1")
            set(SDL2_MIXER_DIR "C:/devellib/SDL2_mixer-2.8.1" CACHE PATH "" FORCE)
        endif()
        find_package(SDL2_mixer REQUIRED)
    else()
        add_compile_definitions(NO_SOUND_LIBRARY)
    endif()
endif()

# All executables and DLLs go to one directory (per-config on multi-config generators).
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

# Single asset-staging step.  All executables (HeMoM, ReMoMber, savedump,
# rmr2hms, tests, ...) share the same bin/$<CONFIG> directory, so the asset
# tree is copied there exactly once per build instead of being copied
# redundantly by each executable's POST_BUILD.
add_custom_target(copy_assets ALL
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    COMMENT "Staging game assets to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    VERBATIM
)

# Copy SDL runtime DLLs to both Debug and Release output dirs at configure time.
if(WIN32 AND NOT USE_WIN32)
    if(USE_SDL3)
        set(_sdl_target SDL3::SDL3)
        set(_mixer_target SDL3_mixer::SDL3_mixer)
    else()
        set(_sdl_target SDL2::SDL2)
        set(_mixer_target SDL2_mixer::SDL2_mixer)
    endif()
    get_target_property(_sdl_imported_loc ${_sdl_target} IMPORTED_LOCATION)
    if(NOT _sdl_imported_loc)
        get_target_property(_sdl_imported_loc ${_sdl_target} IMPORTED_LOCATION_RELEASE)
    endif()
    if(_sdl_imported_loc)
        file(COPY "${_sdl_imported_loc}" DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug")
        file(COPY "${_sdl_imported_loc}" DESTINATION "${CMAKE_BINARY_DIR}/bin/Release")
    endif()
    if(NOT DISABLE_AUDIO AND TARGET ${_mixer_target})
        get_target_property(_mixer_imported_loc ${_mixer_target} IMPORTED_LOCATION)
        if(NOT _mixer_imported_loc)
            get_target_property(_mixer_imported_loc ${_mixer_target} IMPORTED_LOCATION_RELEASE)
        endif()
        if(_mixer_imported_loc)
            file(COPY "${_mixer_imported_loc}" DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug")
            file(COPY "${_mixer_imported_loc}" DESTINATION "${CMAKE_BINARY_DIR}/bin/Release")
        endif()
    endif()
endif()

add_compile_definitions($<$<CONFIG:Debug>:STU_DEBUG>)

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    # add_compile_options(/wd4068)
    add_compile_options(
        "/W4"
        "/WX"
        # DONT  "/wd4013"  # 'stu_strcat' undefined; assuming extern returning int
        # DONT  "/wd4057"  # '=': 'uint8_t *' differs in indirection to slightly different base types from 'int8_t *'
        "/wd4100"  # unreferenced parameter
        "/wd4101"  # unreferenced local variable
        "/wd4127"  # conditional expression is constant
        "/wd4189"  # local variable is initialized but not referenced
        "/wd4201"  # nonstandard extension used: nameless struct/union
        "/wd4206"  # nonstandard extension used: translation unit is empty
        "/wd4244"  # integer type is converted to a smaller integer type, possible loss of data
        "/wd4245"  # '=': conversion from 'type1' to 'type2', signed/unsigned mismatch
        "/wd4310"  # cast truncates constant value
        "/wd4389"  # '!=': signed/unsigned mismatch
        # DONT  "/wd4456"  # declaration of 'identifier' hides previous local declaration
        # DONT  "/wd4457"  # declaration of 'identifier' hides function parameter
        "/wd4459"  # declaration of 'identifier' hides global declaration
        "/wd4701"  # potentially uninitialized local variable
        "/wd4702"  # unreachable code
        "/wd4996"  # 'strcat': This function or variable may be unsafe. Consider using strcat_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
        # "/wd4068"
        # "/wd4365"
        # "/wd4820"
    )
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-g -O0 -fno-limit-debug-info)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        "-Wno-unsequenced"
    )
endif()
if(CMAKE_COMPILER_IS_GNUCC)
    add_compile_options(
        "-Wno-array-bounds="
        "-Wno-maybe-uninitialized"
        "-Wno-missing-braces"
        "-Wno-sequence-point"
        "-Wno-unknown-pragmas"
        "-Wno-unused-label"
        "-Wno-unused-but-set-variable"
        "-Wno-unsequenced"
    )
endif()



# Summary of Differences
# Method            Portability     When it runs
# add_dependencies  All Generators  Before the target's first source is compiled.
# OUTPUT Command    All Generators  Only if the generated file is out-of-date.
# Pro Tip: Use the cmake -E command for common tasks (like copy, make_directory, or echo) to ensure your pre-build scripts remain platform-independent. 

# # Define the custom task
# add_custom_target(run_pre_build_task
#     COMMAND ${CMAKE_COMMAND} -E echo "Running pre-build task..."
#     # Add your specific command here
#     VERBATIM
# )
# 
# # Make your main target depend on it
# add_dependencies(my_executable run_pre_build_task)

# add_custom_command(
#     OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/generated.h"
#     COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_SOURCE_DIR}/generate_header.cmake"
#     DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/template.h.in"
#     VERBATIM
# )
# 
# # The command will run because the executable depends on 'generated.h'
# add_executable(my_executable main.cpp "${CMAKE_CURRENT_BINARY_DIR}/generated.h")



add_subdirectory(STU)
add_subdirectory(platform)
add_subdirectory(MoX)
add_subdirectory(MoM)
add_subdirectory(src)
add_subdirectory(tools)
add_subdirectory(tests)

# --- Installation / Packaging ---

install(TARGETS
        ReMoMber
        HeMoM
        savedump
        rmr2hms
        check_save_fields
        compare_save_text
        check_ai_metrics
    RUNTIME DESTINATION .
)

# Bundle SDL DLLs on Windows so the player doesn't need them installed.
if(WIN32 AND NOT USE_WIN32)
    if(_sdl_imported_loc)
        install(FILES "${_sdl_imported_loc}" DESTINATION .)
    endif()
    if(NOT DISABLE_AUDIO AND _mixer_imported_loc)
        install(FILES "${_mixer_imported_loc}" DESTINATION .)
    endif()
endif()

# Bundle game assets and the PLAYING readme.
install(DIRECTORY "${PROJECT_SOURCE_DIR}/assets/" DESTINATION .)
if(EXISTS "${PROJECT_SOURCE_DIR}/PLAYING.md")
    install(FILES "${PROJECT_SOURCE_DIR}/PLAYING.md" DESTINATION .)
endif()

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "ReMoM")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_FILE_NAME "ReMoM-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
    set(CPACK_NSIS_PACKAGE_NAME "ReMoM")
    set(CPACK_NSIS_DISPLAY_NAME "ReMoM ${PROJECT_VERSION}")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "ReMoM")
    set(CPACK_NSIS_MUI_FINISHPAGE_RUN "ReMoMber.exe")
else()
    set(CPACK_GENERATOR "ZIP;TGZ")
endif()

set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "ReMoM-${PROJECT_VERSION}-Source")
set(CPACK_SOURCE_IGNORE_FILES
    "/assets/"
    "/\\.LBX/"
    "\\.LBX\\$"
    "/\\.git/"
    "/build/"
    "/out/"
)
include(CPack)
