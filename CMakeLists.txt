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
include(CTest)

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
FetchContent_MakeAvailable(googletest)

# https://wiki.libsdl.org/SDL3/README/cmake
list(APPEND CMAKE_PREFIX_PATH "C:/devellib/SDL3-3.4.2")
find_package(SDL3 REQUIRED)
if(NOT DISABLE_AUDIO)
    list(APPEND CMAKE_PREFIX_PATH "C:/devellib/SDL3_mixer-3.2.0")
    find_package(SDL3_mixer REQUIRED)
else()
    add_compile_definitions(NO_SOUND_LIBRARY)
endif()

# All executables and DLLs go to one directory (per-config on multi-config generators).
# This avoids needing POST_BUILD copy commands for SDL3.dll.
# https://github.com/libsdl-org/SDL/blob/main/docs/README-cmake.md
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

# Copy SDL3 runtime DLLs to both Debug and Release output dirs at configure time.
if(WIN32)
    get_target_property(_sdl3_imported_loc SDL3::SDL3 IMPORTED_LOCATION)
    if(NOT _sdl3_imported_loc)
        get_target_property(_sdl3_imported_loc SDL3::SDL3 IMPORTED_LOCATION_RELEASE)
    endif()
    if(_sdl3_imported_loc)
        file(COPY "${_sdl3_imported_loc}" DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug")
        file(COPY "${_sdl3_imported_loc}" DESTINATION "${CMAKE_BINARY_DIR}/bin/Release")
    endif()
    if(NOT DISABLE_AUDIO AND TARGET SDL3_mixer::SDL3_mixer)
        get_target_property(_mixer_imported_loc SDL3_mixer::SDL3_mixer IMPORTED_LOCATION)
        if(NOT _mixer_imported_loc)
            get_target_property(_mixer_imported_loc SDL3_mixer::SDL3_mixer IMPORTED_LOCATION_RELEASE)
        endif()
        if(_mixer_imported_loc)
            file(COPY "${_mixer_imported_loc}" DESTINATION "${CMAKE_BINARY_DIR}/bin/Debug")
            file(COPY "${_mixer_imported_loc}" DESTINATION "${CMAKE_BINARY_DIR}/bin/Release")
        endif()
    endif()
endif()

add_compile_definitions($<$<CONFIG:Debug>:STU_DEBUG>)

add_subdirectory(STU)
add_subdirectory(platform)
add_subdirectory(MoX)
add_subdirectory(MoM)
add_subdirectory(src)
add_subdirectory(tests)

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
include(CPack)
